#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

#ifdef КУЧА_РНЦП

#include "HeapImp.h"

#define LLOG(x) //  DLOG(x) // LOG((проц *)this << ' ' << x)

бкрат Куча::HPAGE = 16 * 256; // 16MB default значение
бкрат Куча::сис_блок_лимит = 16 * 256; // 16MB default значение
цел  Куча::max_free_hpages = 1; // default значение
цел  Куча::max_free_lpages = 2; // default значение
цел  Куча::max_free_spages = 256; // default значение (1MB)

ОпцииПамяти::ОпцииПамяти()
{
	мастер_блок = 4 * Куча::HPAGE;
	сис_блок_лимит = 4 * Куча::сис_блок_лимит;
	мастер_резерв = Куча::max_free_hpages;
	малый_резерв = Куча::max_free_spages;
	большой_резерв = Куча::max_free_lpages;
}

ОпцииПамяти::~ОпцииПамяти()
{
	Куча::HPAGE = (бкрат)clamp(мастер_блок / 4, 256, 65535);
	Куча::сис_блок_лимит = (бкрат)clamp((цел)сис_блок_лимит / 4, 16, (цел)Куча::HPAGE);
	Куча::max_free_hpages = мастер_резерв;
	Куча::max_free_spages = малый_резерв;
	Куча::max_free_lpages = большой_резерв;
}

кткст0 какТкст(цел i)
{
	static thread_local char h[4][1024];
	static thread_local цел ii;
	ii = (ii + 1) & 3;
	sprintf(h[ii], "%d", i);
	return h[ii];
}

кткст0 какТкст(ук укз)
{
	static thread_local char h[4][1024];
	static thread_local цел ii;
	ii = (ii + 1) & 3;
	sprintf(h[ii], "%p", укз);
	return h[ii];
}


Куча::DLink Куча::big[1];
СтатическийСтопор Куча::mutex;

// Not associated with thread, locked access, to store orphans on thread exit and provide after
// exit allocations. Access serialized with Куча::mutex.
Куча        Куча::aux;

проц Куча::иниц()
{
	if(initialized)
		return;
	LLOG("иниц heap " << (проц *)this);

	for(цел i = 0; i < NKLASS; i++) {
		empty[i] = NULL;
		full[i]->линкуйся();
		work[i]->линкуйся();
		work[i]->freelist = NULL;
		work[i]->klass = i;
		cachen[i] = 3500 / Ksz(i);
	}
	LИниt();
	large->линкуйся();
	if(this != &aux && !aux.initialized) {
		Стопор::Замок __(mutex);
		aux.иниц();
	}
	initialized = true;
	out_ptr = out;
	out_size = 0;
}

проц Куча::FreeRemoteRaw()
{
	LLOG("FreeRemoteRaw");
	SmallFreeRemoteRaw();
	LargeFreeRemoteRaw();
}

проц Куча::MoveLargeTo(DLink *ml, Куча *to_heap)
{
	LLOG("MoveLargePage " << какТкст(ml) << " to " << какТкст(to_heap));
	ml->отлинкуй();
	ml->Линк(to_heap->large);
	LBlkHeader *h = ml->дайПерв();
	for(;;) {
		LLOG("Large block " << какТкст(h) << " size " << какТкст(h->дайРазм() * 256) << (h->IsFree() ? " free" : ""));
		h->heap = to_heap;
		if(h->IsFree()) {
			h->UnlinkFree(); // will link it when adopting
			to_heap->lheap.LinkFree(h);
		}
		if(h->IsLast())
			break;
		h = h->GetNextHeader();
	}
}

проц Куча::MoveLargeTo(Куча *to_heap)
{
	while(large != large->next)
		MoveLargeTo(large->next, to_heap);
}

проц Куча::DblCheck(Страница *p)
{
	Страница *l = p;
	do {
		Assert(l->next->prev == l && l->prev->next == l);
		l = l->next;
	}
	while(p != l);
}

цел Куча::CheckFree(FreeLink *l, цел k, бул pg)
{
	char h[200];
	цел n = 0;
	
	Страница *page = дайСтраницу(l);

	if(l && page->klass != k) {
		sprintf(h, "Invalid freelist block at 0x%p sz: %d (klass mismatch)", l, Ksz(k));
		паника(h);
	}
	
	while(l) {
		if(l->next) {
			Страница *lp = дайСтраницу(l->next);
			if(pg && lp != page) {
				sprintf(h, "Invalid freelist block at 0x%p sz: %d (out of page) (-> 0x%p)", l, Ksz(k), l->next);
				паника(h);
			}
			if((4096 - ((uintptr_t)(l->next) & (uintptr_t)4095)) % Ksz(k) != 0) {
				sprintf(h, "Invalid freelist block at 0x%p sz: %d (invalid address)", l, Ksz(k));
				паника(h);
			}
			if(lp->klass != k) {
				sprintf(h, "Invalid freelist block at 0x%p sz: %d (next klass mismatch)", l, Ksz(k));
				паника(h);
			}
		}

		DbgFreeCheckK(l, k);

		l = l->next;
		n++;
	}
	return n;
}

проц Куча::Check() {
	Стопор::Замок __(mutex);
	иниц();
	if(!work[0]->next)
		иниц();
	for(цел i = 0; i < NKLASS; i++) {
		DblCheck(work[i]);
		DblCheck(full[i]);
		Страница *p = work[i]->next;
		while(p != work[i]) {
			Assert(p->heap == this);
			Assert(CheckFree(p->freelist, p->klass) + p->active == p->счёт());
			p = p->next;
		}
		p = full[i]->next;
		while(p != full[i]) {
			Assert(p->heap == this);
			Assert(p->klass == i);
			Assert(p->active == p->счёт());
			p = p->next;
		}
		p = empty[i];
		while(p) {
			Assert(p->heap == this);
			Assert(p->active == 0);
			Assert(p->klass == i);
			Assert(CheckFree(p->freelist, i) == p->счёт());
			if(this != &aux)
				break;
			p = p->next;
		}
		CheckFree(cache[i], i, false);
	}

	DLink *l = large->next;
	while(l != large) {
		lheap.BlkCheck(l->дайПерв(), 255, true);
		l = l->next;
	}

	HugePage *pg = huge_pages;
	while(pg) {
		BlkCheck(pg->page, HPAGE);
		pg = pg->next;
	}

	if(this != &aux)
		aux.Check();
}

проц Куча::AssertLeaks(бул b)
{
	if(!b)
		паника("Memory leaks detected! (final check)");
}

проц Куча::AuxFinalCheck()
{
	Стопор::Замок __(mutex);
	aux.иниц();
	aux.FreeRemoteRaw();
	aux.Check();
	if(!aux.work[0]->next)
		aux.иниц();
	for(цел i = 0; i < NKLASS; i++) {
		Assert(!aux.cache[i]);
		DblCheck(aux.work[i]);
		DblCheck(aux.full[i]);
		AssertLeaks(aux.work[i] == aux.work[i]->next);
		AssertLeaks(aux.full[i] == aux.full[i]->next);
		Страница *p = aux.empty[i];
		while(p) {
			Assert(p->heap == &aux);
			Assert(p->active == 0);
			Assert(CheckFree(p->freelist, p->klass) == p->счёт());
			p = p->next;
		}
	}
	AssertLeaks(aux.large == aux.large->next);
	AssertLeaks(big == big->next);
}

#ifdef MEMORY_SHRINK
проц Куча::сожми()
{
	LLOG("MemoryShrink");
	Стопор::Замок __(mutex);
#if 0
	for(цел i = 0; i < NKLASS; i++) {
		Страница *p = aux.empty[i];
		while(p) {
			Страница *q = p;
			p = p->next;
			FreeRaw4KB(q);
		}
		aux.empty[i] = NULL;
	}
#endif
	DLink *m = lempty->next;
	while(m != lempty) {
		DLink *q = m;
		m = m->next;
		q->отлинкуй();
		FreeRaw64KB(q);
	}
}

проц MemoryShrink()
{
	Куча::сожми();
}
#endif

#endif

}

#if defined(КУЧА_РНЦП) && !defined(STD_NEWDELETE)
#include <new>

#ifdef COMPILER_GCC
#pragma GCC diagnostic ignored "-Wdeprecated" // silence modern GCC warning about throw(std::bad_alloc)
#endif

ук operator new(т_мера size)                                    { ук укз = РНЦП::разместиПам(size); return укз; }
проц operator  delete(ук укз) noexcept(true)                    { РНЦП::освободиПам(укз); }

ук operator new[](т_мера size)                                  { ук укз = РНЦП::разместиПам(size); return укз; }
проц operator  delete[](ук укз) noexcept(true)                  { РНЦП::освободиПам(укз); }

ук operator new(т_мера size, const std::nothrow_t&) noexcept    { ук укз = РНЦП::разместиПам(size); return укз; }
проц operator  delete(ук укз, const std::nothrow_t&) noexcept   { РНЦП::освободиПам(укз); }

ук operator new[](т_мера size, const std::nothrow_t&) noexcept  { ук укз = РНЦП::разместиПам(size); return укз; }
проц operator  delete[](ук укз, const std::nothrow_t&) noexcept { РНЦП::освободиПам(укз); }

#if defined(PLATFORM_WIN32) && defined(COMPILER_CLANG)
//  this is temporary fix before llvm-mingw fixes weak references
проц __attribute__((__noreturn__)) std::__throw_bad_alloc (проц) { throw bad_alloc(); }
#endif

#endif
