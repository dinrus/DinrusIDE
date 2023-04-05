#include "Core.h"

namespace РНЦПДинрус {

#ifdef КУЧА_РНЦП

#include "HeapImp.h"

#define LLOG(x) //  DLOG(x) // LOG((void *)this << ' ' << x)

word Куча::HPAGE = 16 * 256; // 16MB default значение
word Куча::sys_block_limit = 16 * 256; // 16MB default значение
int  Куча::max_free_hpages = 1; // default значение
int  Куча::max_free_lpages = 2; // default значение
int  Куча::max_free_spages = 256; // default значение (1MB)

ОпцииПамяти::ОпцииПамяти()
{
	master_block = 4 * Куча::HPAGE;
	sys_block_limit = 4 * Куча::sys_block_limit;
	master_reserve = Куча::max_free_hpages;
	small_reserve = Куча::max_free_spages;
	large_reserve = Куча::max_free_lpages;
}

ОпцииПамяти::~ОпцииПамяти()
{
	Куча::HPAGE = (word)clamp(master_block / 4, 256, 65535);
	Куча::sys_block_limit = (word)clamp((int)sys_block_limit / 4, 16, (int)Куча::HPAGE);
	Куча::max_free_hpages = master_reserve;
	Куча::max_free_spages = small_reserve;
	Куча::max_free_lpages = large_reserve;
}

const char *какТкст(int i)
{
	static thread_local char h[4][1024];
	static thread_local int ii;
	ii = (ii + 1) & 3;
	sprintf(h[ii], "%d", i);
	return h[ii];
}

const char *какТкст(void *ptr)
{
	static thread_local char h[4][1024];
	static thread_local int ii;
	ii = (ii + 1) & 3;
	sprintf(h[ii], "%p", ptr);
	return h[ii];
}


Куча::DLink Куча::big[1];
СтатическийСтопор Куча::mutex;

// Not associated with thread, locked access, to store orphans on thread exit and provide after
// exit allocations. Access serialized with Куча::mutex.
Куча        Куча::aux;

void Куча::иниц()
{
	if(initialized)
		return;
	LLOG("иниц heap " << (void *)this);

	for(int i = 0; i < NKLASS; i++) {
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

void Куча::FreeRemoteRaw()
{
	LLOG("FreeRemoteRaw");
	SmallFreeRemoteRaw();
	LargeFreeRemoteRaw();
}

void Куча::MoveLargeTo(DLink *ml, Куча *to_heap)
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

void Куча::MoveLargeTo(Куча *to_heap)
{
	while(large != large->next)
		MoveLargeTo(large->next, to_heap);
}

void Куча::DblCheck(Page *p)
{
	Page *l = p;
	do {
		Assert(l->next->prev == l && l->prev->next == l);
		l = l->next;
	}
	while(p != l);
}

int Куча::CheckFree(FreeLink *l, int k, bool pg)
{
	char h[200];
	int n = 0;
	
	Page *page = дайСтраницу(l);

	if(l && page->klass != k) {
		sprintf(h, "Invalid freelist block at 0x%p sz: %d (klass mismatch)", l, Ksz(k));
		паника(h);
	}
	
	while(l) {
		if(l->next) {
			Page *lp = дайСтраницу(l->next);
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

void Куча::Check() {
	Стопор::Замок __(mutex);
	иниц();
	if(!work[0]->next)
		иниц();
	for(int i = 0; i < NKLASS; i++) {
		DblCheck(work[i]);
		DblCheck(full[i]);
		Page *p = work[i]->next;
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

void Куча::AssertLeaks(bool b)
{
	if(!b)
		паника("Memory leaks detected! (final check)");
}

void Куча::AuxFinalCheck()
{
	Стопор::Замок __(mutex);
	aux.иниц();
	aux.FreeRemoteRaw();
	aux.Check();
	if(!aux.work[0]->next)
		aux.иниц();
	for(int i = 0; i < NKLASS; i++) {
		Assert(!aux.cache[i]);
		DblCheck(aux.work[i]);
		DblCheck(aux.full[i]);
		AssertLeaks(aux.work[i] == aux.work[i]->next);
		AssertLeaks(aux.full[i] == aux.full[i]->next);
		Page *p = aux.empty[i];
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
void Куча::сожми()
{
	LLOG("MemoryShrink");
	Стопор::Замок __(mutex);
#if 0
	for(int i = 0; i < NKLASS; i++) {
		Page *p = aux.empty[i];
		while(p) {
			Page *q = p;
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

void MemoryShrink()
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

void *operator new(size_t size)                                    { void *ptr = РНЦП::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr) noexcept(true)                    { РНЦП::MemoryFree(ptr); }

void *operator new[](size_t size)                                  { void *ptr = РНЦП::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr) noexcept(true)                  { РНЦП::MemoryFree(ptr); }

void *operator new(size_t size, const std::nothrow_t&) noexcept    { void *ptr = РНЦП::MemoryAlloc(size); return ptr; }
void operator  delete(void *ptr, const std::nothrow_t&) noexcept   { РНЦП::MemoryFree(ptr); }

void *operator new[](size_t size, const std::nothrow_t&) noexcept  { void *ptr = РНЦП::MemoryAlloc(size); return ptr; }
void operator  delete[](void *ptr, const std::nothrow_t&) noexcept { РНЦП::MemoryFree(ptr); }

#if defined(PLATFORM_WIN32) && defined(COMPILER_CLANG)
//  this is temporary fix before llvm-mingw fixes weak references
void __attribute__((__noreturn__)) std::__throw_bad_alloc (void) { throw bad_alloc(); }
#endif

#endif
