#include <DinrusPro/DinrusPro.h>

#define LTIMING(x)    // RTIMING(x)
#define LHITCOUNT(x)  // RHITCOUNT(x)
#define LLOG(x)       //  LOG((проц *)this << ' ' << x)

namespace ДинрусРНЦП {

#ifdef КУЧА_РНЦП

#include "HeapImp.h"

цел Куча::lclass[] = { 0, 4, 5, 6, 7, 8, 9, 11, 13, 15, 18, 22, 27, 33, 40, 49, 60, 73, 89, 109, 134, 164, 201, 225, 255 };
цел Куча::free_lclass[LPAGE + 1]; // free block size -> lclass, size is >= class sz
цел Куча::alloc_lclass[LPAGE + 1]; // allocation size -> lclass, size <= class sz


проц Куча::LargeHeapDetail::LinkFree(BlkHeader_<LUNIT> *h)
{
	Dbl_LinkAfter(h, freelist[free_lclass[h->дайРазм()]]);
}

проц Куча::LИниt()
{
	ПРОВЕРЬ(__countof(lheap.freelist) == __countof(lclass));
	ONCELOCK {
		цел ai = 0;
		цел fi = 0;
		for(цел i = 0; i <= 255; i++) {
			if(i > lclass[ai])
				ai++;
			if(i >= lclass[fi + 1])
				fi++;
			alloc_lclass[i] = ai;
			free_lclass[i] = fi;
		}
	}
	for(цел i = 0; i <= __countof(lheap.freelist); i++)
		Dbl_Self(lheap.freelist[i]);
	big->линкуйся();
}

ук Куча::TryLAlloc(цел i0, бкрат wcount)
{
	LTIMING("TryLAlloc");
	for(цел i = i0; i < __countof(lheap.freelist); i++) {
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
цел stat[65536];

ЭКЗИТБЛОК {
	цел cnt = 0;
	for(цел i = 0; i < 65536; i++) {
		cnt += stat[i];
		if(stat[i])
			RLOG(i * 256 << ": " << stat[i] << " / " << cnt);
	}
}
#endif

ук Куча::LAlloc(т_мера& size)
{
	if(!initialized)
		иниц();

	if(size > LUNIT * LPAGE - sizeof(BlkPrefix)) { // big block allocation
		LTIMING("Big alloc");
		Стопор::Замок __(mutex);
		т_мера count = (size + sizeof(DLink) + sizeof(BlkPrefix) + 4095) >> 12;
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

	бкрат wcount = бкрат((size + sizeof(BlkPrefix) + LUNIT - 1) >> 8);

#ifdef LSTAT
	stat[wcount]++;
#endif

	LTIMING("LAlloc");

	size = ((цел)wcount * LUNIT) - sizeof(BlkPrefix);
	цел i0 = alloc_lclass[wcount];

	if(large_remote_list)  // there might be blocks of this heap freed in other threads
		LargeFreeRemote(); // free them first

	ук укз = TryLAlloc(i0, wcount);
	if(укз)
		return укз;

	Стопор::Замок __(mutex);
	aux.LargeFreeRemoteRaw();
	укз = aux.TryLAlloc(i0, wcount);
	if(укз) { // found in aux, we need to move large page from aux to this heap
		LLOG("Found in aux");
		BlkPrefix *h = (BlkPrefix *)укз - 1;
		while(!h->IsFirst()) // найди the start of large page to get page header
			h = h->GetPrevHeader(LUNIT);
		MoveLargeTo((DLink *)((ббайт *)h - LOFFSET), this);
		return укз;
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

проц Куча::FreeLargePage(DLink *l)
{
	LLOG("Moving empty large " << (проц *)l << " to global storage, lcount " << lcount);
	l->отлинкуй();
	Стопор::Замок __(mutex);
	HugeFree(l);
}

проц Куча::LFree(ук укз)
{
	BlkPrefix *h = (BlkPrefix *)укз - 1;

	if(h->heap == this) {
		LTIMING("Large освободи");
		LBlkHeader *fh = lheap.освободи((LBlkHeader *)h);
		if(fh->дайРазм() == LPAGE) {
			if(free_lpages >= max_free_lpages || this == &aux) {
				LTIMING("FreeLargePage");
				fh->UnlinkFree();
				FreeLargePage((DLink *)((ббайт *)fh - LOFFSET));
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
		LLOG("Big free " << (проц *) укз << " size " << h->size);
		HugeFree(d);
		return;
	}

	LTIMING("Remote освободи");
	// this is remote heap
	FreeLink *f = (FreeLink *)укз;
	f->next = h->heap->large_remote_list;
	h->heap->large_remote_list = f;
}

бул   Куча::TryRealloc(ук укз, т_мера& newsize)
{
	LTIMING("TryRealloc");
	ПРОВЕРЬ(укз);

#ifdef _ОТЛАДКА
	if(смолл_ли(укз))
		return false;
#endif

	BlkPrefix *h = (BlkPrefix *)укз - 1;

	if(h->heap == this) {
		if(newsize > LUNIT * LPAGE - sizeof(BlkPrefix))
			return false;
		бкрат wcount = бкрат(((newsize ? newsize : 1) + sizeof(BlkPrefix) + LUNIT - 1) >> 8);
		т_мера dummy = 0;
		if(wcount == h->дайРазм() || lheap.TryRealloc(h, wcount, dummy)) {
			newsize = ((цел)wcount * LUNIT) - sizeof(BlkPrefix);
			LHITCOUNT("Large realloc true");
			return true;
		}
	}

	Стопор::Замок __(mutex);
	if(h->heap == NULL) { // this is big block
		LTIMING("Big realloc");

		DLink *d = (DLink *)h - 1;

		т_мера count = (newsize + sizeof(DLink) + sizeof(BlkPrefix) + 4095) >> 12;

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

т_мера Куча::LGetBlockSize(ук укз) {
	BlkPrefix *h = (BlkPrefix *)укз - 1;

	if(h->heap == NULL) { // huge block
		Стопор::Замок __(mutex);
		DLink *hh = (DLink *)h - 1;
		return hh->size;
	}

	return ((цел)h->дайРазм() * LUNIT) - sizeof(BlkPrefix);
}

#endif

}