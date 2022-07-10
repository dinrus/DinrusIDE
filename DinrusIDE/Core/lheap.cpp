#include "Core.h"

#define LTIMING(x)    // RTIMING(x)
#define LHITCOUNT(x)  // RHITCOUNT(x)
#define LLOG(x)       //  LOG((void *)this << ' ' << x)

namespace РНЦПДинрус {

#ifdef КУЧА_РНЦП

#include "HeapImp.h"

int Куча::lclass[] = { 0, 4, 5, 6, 7, 8, 9, 11, 13, 15, 18, 22, 27, 33, 40, 49, 60, 73, 89, 109, 134, 164, 201, 225, 255 };
int Куча::free_lclass[LPAGE + 1]; // free block size -> lclass, size is >= class sz
int Куча::alloc_lclass[LPAGE + 1]; // allocation size -> lclass, size <= class sz


void Куча::LargeHeapDetail::LinkFree(BlkHeader_<LUNIT> *h)
{
	Dbl_LinkAfter(h, freelist[free_lclass[h->дайРазм()]]);
}

void Куча::LИниt()
{
	ПРОВЕРЬ(__countof(lheap.freelist) == __countof(lclass));
	ONCELOCK {
		int ai = 0;
		int fi = 0;
		for(int i = 0; i <= 255; i++) {
			if(i > lclass[ai])
				ai++;
			if(i >= lclass[fi + 1])
				fi++;
			alloc_lclass[i] = ai;
			free_lclass[i] = fi;
		}
	}
	for(int i = 0; i <= __countof(lheap.freelist); i++)
		Dbl_Self(lheap.freelist[i]);
	big->линкуйся();
}

void *Куча::TryLAlloc(int i0, word wcount)
{
	LTIMING("TryLAlloc");
	for(int i = i0; i < __countof(lheap.freelist); i++) {
		LBlkHeader *l = lheap.freelist[i];
		LBlkHeader *h = l->next;
		if(h != l) {
			ПРОВЕРЬ(h->дайРазм() >= wcount);
			if(h->дайРазм() == LPAGE && this != &aux) {
				free_lpages--;
				ПРОВЕРЬ(free_lpages >= 0);
			}
			lheap.MakeAlloc(h, wcount);
			h->heap = this;
			return (BlkPrefix *)h + 1;
		}
		LHITCOUNT("TryLAlloc 2");
	}
	return NULL;
}

#ifdef LSTAT
int stat[65536];

ЭКЗИТБЛОК {
	int cnt = 0;
	for(int i = 0; i < 65536; i++) {
		cnt += stat[i];
		if(stat[i])
			RLOG(i * 256 << ": " << stat[i] << " / " << cnt);
	}
}
#endif

void *Куча::LAlloc(size_t& size)
{
	if(!initialized)
		иниц();

	if(size > LUNIT * LPAGE - sizeof(BlkPrefix)) { // big block allocation
		LTIMING("Big alloc");
		Стопор::Замок __(mutex);
		size_t count = (size + sizeof(DLink) + sizeof(BlkPrefix) + 4095) >> 12;
		DLink *d = (DLink *)HugeAlloc(count);
		d->Линк(big);
		d->size = size = (count << 12) - sizeof(DLink) - sizeof(BlkPrefix);
		BlkPrefix *h = (BlkPrefix *)(d + 1);
		h->heap = NULL; // mark this as huge block
		big_size += size;
		big_count++;
		LLOG("Big alloc " << size << ": " << h + 1);
		return h + 1;
	}

	word wcount = word((size + sizeof(BlkPrefix) + LUNIT - 1) >> 8);

#ifdef LSTAT
	stat[wcount]++;
#endif

	LTIMING("LAlloc");

	size = ((int)wcount * LUNIT) - sizeof(BlkPrefix);
	int i0 = alloc_lclass[wcount];

	if(large_remote_list)  // there might be blocks of this heap freed in other threads
		LargeFreeRemote(); // free them first

	void *ptr = TryLAlloc(i0, wcount);
	if(ptr)
		return ptr;

	Стопор::Замок __(mutex);
	aux.LargeFreeRemoteRaw();
	ptr = aux.TryLAlloc(i0, wcount);
	if(ptr) { // found in aux, we need to move large page from aux to this heap
		LLOG("Found in aux");
		BlkPrefix *h = (BlkPrefix *)ptr - 1;
		while(!h->IsFirst()) // find the start of large page to get page header
			h = h->GetPrevHeader(LUNIT);
		MoveLargeTo((DLink *)((byte *)h - LOFFSET), this);
		return ptr;
	}

	LTIMING("Large ещё");
	DLink *ml = (DLink *)HugeAlloc(((LPAGE + 1) * LUNIT) / 4096);
	ml->Линк(large);
	LBlkHeader *h = ml->дайПерв();
	lheap.AddChunk(h, LPAGE);
	lheap.MakeAlloc(h, wcount);
	h->heap = this;
	return (BlkPrefix *)h + 1;
}

void Куча::FreeLargePage(DLink *l)
{
	LLOG("Moving empty large " << (void *)l << " to global storage, lcount " << lcount);
	l->отлинкуй();
	Стопор::Замок __(mutex);
	HugeFree(l);
}

void Куча::LFree(void *ptr)
{
	BlkPrefix *h = (BlkPrefix *)ptr - 1;

	if(h->heap == this) {
		LTIMING("Large освободи");
		LBlkHeader *fh = lheap.освободи((LBlkHeader *)h);
		if(fh->дайРазм() == LPAGE) {
			if(free_lpages >= max_free_lpages || this == &aux) {
				LTIMING("FreeLargePage");
				fh->UnlinkFree();
				FreeLargePage((DLink *)((byte *)fh - LOFFSET));
			}
			else
				free_lpages++;
		}
		return;
	}

	Стопор::Замок __(mutex);
	if(h->heap == NULL) { // this is big block
		LTIMING("Big освободи");
		DLink *d = (DLink *)h - 1;
		big_size -= d->size;
		big_count--;
		d->отлинкуй();
		LLOG("Big free " << (void *) ptr << " size " << h->size);
		HugeFree(d);
		return;
	}

	LTIMING("Remote освободи");
	// this is remote heap
	FreeLink *f = (FreeLink *)ptr;
	f->next = h->heap->large_remote_list;
	h->heap->large_remote_list = f;
}

bool   Куча::TryRealloc(void *ptr, size_t& newsize)
{
	LTIMING("TryRealloc");
	ПРОВЕРЬ(ptr);

#ifdef _ОТЛАДКА
	if(смолл_ли(ptr))
		return false;
#endif

	BlkPrefix *h = (BlkPrefix *)ptr - 1;

	if(h->heap == this) {
		if(newsize > LUNIT * LPAGE - sizeof(BlkPrefix))
			return false;
		word wcount = word(((newsize ? newsize : 1) + sizeof(BlkPrefix) + LUNIT - 1) >> 8);
		size_t dummy = 0;
		if(wcount == h->дайРазм() || lheap.TryRealloc(h, wcount, dummy)) {
			newsize = ((int)wcount * LUNIT) - sizeof(BlkPrefix);
			LHITCOUNT("Large realloc true");
			return true;
		}
	}

	Стопор::Замок __(mutex);
	if(h->heap == NULL) { // this is big block
		LTIMING("Big realloc");

		DLink *d = (DLink *)h - 1;

		size_t count = (newsize + sizeof(DLink) + sizeof(BlkPrefix) + 4095) >> 12;

		if(HugeTryRealloc(d, count)) {
			big_size -= d->size;
			d->size = newsize = (count << 12) - sizeof(DLink) - sizeof(BlkPrefix);
			big_size += d->size;
			return true;
		}
	}

	// TODO: When small block fits, we could still return true

	return false;
}

size_t Куча::LGetBlockSize(void *ptr) {
	BlkPrefix *h = (BlkPrefix *)ptr - 1;

	if(h->heap == NULL) { // huge block
		Стопор::Замок __(mutex);
		DLink *hh = (DLink *)h - 1;
		return hh->size;
	}

	return ((int)h->дайРазм() * LUNIT) - sizeof(BlkPrefix);
}

#endif

}