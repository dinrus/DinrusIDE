#include <DinrusPro/DinrusCore.h>

#define LTIMING(x)    // RTIMING(x)
#define LHITCOUNT(x)  // RHITCOUNT(x)
#define LLOG(x)       //  LOG((проц *)this << ' ' << x)


#ifdef КУЧА_РНЦП

#include "HeapImp.h"

цел Куча::lclass[] = { 0, 4, 5, 6, 7, 8, 9, 11, 13, 15, 18, 22, 27, 33, 40, 49, 60, 73, 89, 109, 134, 164, 201, 225, 255 };
цел Куча::free_lclass[LPAGE + 1]; // free block size -> lclass, size is >= class sz
цел Куча::alloc_lclass[LPAGE + 1]; // allocation size -> lclass, size <= class sz


проц Куча::ДетальКучиЛардж::линкуйСвод(ЗагБлока_<LUNIT> *h)
{
	Dbl_LinkAfter(h, freelist[free_lclass[h->дайРазм()]]);
}

проц Куча::иницЛ()
{
	ПРОВЕРЬ(__количество(lheap.freelist) == __количество(lclass));
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
	for(цел i = 0; i <= __количество(lheap.freelist); i++)
		Dbl_Self(lheap.freelist[i]);
	big->линкуйся();
}

ук Куча::TryLAlloc(цел i0, бкрат wcount)
{
	LTIMING("TryLAlloc");
	for(цел i = i0; i < __количество(lheap.freelist); i++) {
		ЛЗагБлока *l = lheap.freelist[i];
		ЛЗагБлока *h = l->next;
		if(h != l) {
			ПРОВЕРЬ(h->дайРазм() >= wcount);
			if(h->дайРазм() == LPAGE && this != &aux) {
				free_lpages--;
				ПРОВЕРЬ(free_lpages >= 0);
			}
			lheap.сделайРазмест(h, wcount);
			h->heap = this;
			return (ПрефиксБлока *)h + 1;
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

ук Куча::бРазмести(т_мера& size)
{
	if(!initialized)
		иниц();

	if(size > LUNIT * LPAGE - sizeof(ПрефиксБлока)) { // big block allocation
		LTIMING("Big alloc");
		Стопор::Замок __(mutex);
		т_мера count = (size + sizeof(DLink) + sizeof(ПрефиксБлока) + 4095) >> 12;
		DLink *d = (DLink *)разместиХьюдж(count);
		d->Линк(big);
		d->size = size = (count << 12) - sizeof(DLink) - sizeof(ПрефиксБлока);
		ПрефиксБлока *h = (ПрефиксБлока *)(d + 1);
		h->heap = NULL; // mark this as huge block
		big_size += size;
		big_count++;
		LLOG("Big alloc " << size << ": " << h + 1);
		return h + 1;
	}

	бкрат wcount = бкрат((size + sizeof(ПрефиксБлока) + LUNIT - 1) >> 8);

#ifdef LSTAT
	stat[wcount]++;
#endif

	LTIMING("бРазмести");

	size = ((цел)wcount * LUNIT) - sizeof(ПрефиксБлока);
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
		ПрефиксБлока *h = (ПрефиксБлока *)укз - 1;
		while(!h->первый_ли()) // найди the start of large page to get page header
			h = h->дайПредшЗаг(LUNIT);
		MoveLargeTo((DLink *)((ббайт *)h - LOFFSET), this);
		return укз;
	}

	LTIMING("Large ещё");
	DLink *ml = (DLink *)разместиХьюдж(((LPAGE + 1) * LUNIT) / 4096);
	ml->Линк(large);
	ЛЗагБлока *h = ml->дайПерв();
	lheap.добавьКусок(h, LPAGE);
	lheap.сделайРазмест(h, wcount);
	h->heap = this;
	return (ПрефиксБлока *)h + 1;
}

проц Куча::FreeLargePage(DLink *l)
{
	LLOG("Moving empty large " << (проц *)l << " to global storage, lcount " << lcount);
	l->отлинкуй();
	Стопор::Замок __(mutex);
	освободиХьюдж(l);
}

проц Куча::бОсвободи(ук укз)
{
	ПрефиксБлока *h = (ПрефиксБлока *)укз - 1;

	if(h->heap == this) {
		LTIMING("Large освободи");
		ЛЗагБлока *fh = lheap.освободи((ЛЗагБлока *)h);
		if(fh->дайРазм() == LPAGE) {
			if(free_lpages >= max_free_lpages || this == &aux) {
				LTIMING("FreeLargePage");
				fh->отлинкуйСвоб();
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
		освободиХьюдж(d);
		return;
	}

	LTIMING("Remote освободи");
	// this is remote heap
	СвобЛинк *f = (СвобЛинк *)укз;
	f->next = h->heap->large_remote_list;
	h->heap->large_remote_list = f;
}

бул   Куча::пробуйПеремест(ук укз, т_мера& newsize)
{
	LTIMING("пробуйПеремест");
	ПРОВЕРЬ(укз);

#ifdef _ОТЛАДКА
	if(малый(укз))
		return false;
#endif

	ПрефиксБлока *h = (ПрефиксБлока *)укз - 1;

	if(h->heap == this) {
		if(newsize > LUNIT * LPAGE - sizeof(ПрефиксБлока))
			return false;
		бкрат wcount = бкрат(((newsize ? newsize : 1) + sizeof(ПрефиксБлока) + LUNIT - 1) >> 8);
		т_мера dummy = 0;
		if(wcount == h->дайРазм() || lheap.пробуйПеремест(h, wcount, dummy)) {
			newsize = ((цел)wcount * LUNIT) - sizeof(ПрефиксБлока);
			LHITCOUNT("Large realloc true");
			return true;
		}
	}

	Стопор::Замок __(mutex);
	if(h->heap == NULL) { // this is big block
		LTIMING("Big realloc");

		DLink *d = (DLink *)h - 1;

		т_мера count = (newsize + sizeof(DLink) + sizeof(ПрефиксБлока) + 4095) >> 12;

		if(пробуйПереместХьюдж(d, count)) {
			big_size -= d->size;
			d->size = newsize = (count << 12) - sizeof(DLink) - sizeof(ПрефиксБлока);
			big_size += d->size;
			return true;
		}
	}

	// TODO: When small block fits, we could still return true

	return false;
}

т_мера Куча::LGetBlockSize(ук укз) {
	ПрефиксБлока *h = (ПрефиксБлока *)укз - 1;

	if(h->heap == NULL) { // huge block
		Стопор::Замок __(mutex);
		DLink *hh = (DLink *)h - 1;
		return hh->size;
	}

	return ((цел)h->дайРазм() * LUNIT) - sizeof(ПрефиксБлока);
}

#endif

