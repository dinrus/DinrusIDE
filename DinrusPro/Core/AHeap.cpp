#include <DinrusPro/DinrusCore.h>

#define LTIMING(x) // RTIMING(x)

#ifdef КУЧА_РНЦП

#include "HeapImp.h"

#define LLOG(x) //  LOG((проц *)this << ' ' << x)

inline проц Куча::Страница::фмт(цел k)
{
	заполниСвобОтл(старт(), стоп() - старт());
	klass = k;
	active = 0;
	цел sz = Ksz(k);
	ббайт *укз = стоп() - sz;
	ббайт *b = старт();
	СвобЛинк *l = NULL;
	while(укз >= b) {
		((СвобЛинк *)укз)->next = l;
		l = (СвобЛинк *)укз;
		укз -= sz;
	}
	freelist = l;
}

Куча::Страница *Куча::WorkPage(цел k) // get a new workpage with empty blocks
{
	LLOG("AllocK - next work page not available " << k << " empty: " << (проц *)empty[k]);
	Страница *page = empty[k]; // hot empty page of the same klass
	empty[k] = NULL;
	if(!page) { // try to reacquire pages freed remotely
		LLOG("AllocK - trying FreeRemote");
		SmallFreeRemote();
		if(work[k]->freelist) { // partially free page found
			LLOG("AllocK - work available after FreeRemote " << k);
			return work[k];
		}
		page = empty[k]; // hot empty page
		empty[k] = NULL;
	}
	if(!page)
		for(цел i = 0; i < NKLASS; i++) // Try hot empty page of different klass
			if(empty[i]) {
				LLOG("AllocK - free page available for reformatting " << k);
				page = empty[i];
				empty[i] = NULL;
				page->фмт(k); // reformat the page for the required klass
				break;
			}
	if(!page) { // Attempt to найди page in global storage of free pages
		Стопор::Замок __(mutex);
		if(this != &aux) { // Try to take the page from the aux
			aux.SmallFreeRemoteRaw();
			if(aux.work[k]->next != aux.work[k]) { // Try page of the same klass first
				page = aux.work[k]->next;
				page->отлинкуй();
				LLOG("AllocK - adopting aux page " << k << " page: " << (проц *)page << ", free " << (проц *)page->freelist);
			}
			if(!page && aux.empty[k]) { // Try free page of the same klass (no need to формат it)
				page = aux.empty[k];
				aux.empty[k] = page->next;
				free_4KB--;
				ПРОВЕРЬ(free_4KB < max_free_spages);
				LLOG("AllocK - empty aux page available of the same формат " << k << " page: " << (проц *)page << ", free " << (проц *)page->freelist);
			}
			if(!page)
				for(цел i = 0; i < NKLASS; i++) // Finally try to найди free page of different klass
					if(aux.empty[i]) {
						page = aux.empty[i];
						aux.empty[i] = page->next;
						free_4KB--;
						page->фмт(k);
						ПРОВЕРЬ(free_4KB < max_free_spages);
						LLOG("AllocK - empty aux page available for reformatting " << k << " page: " << (проц *)page << ", free " << (проц *)page->freelist);
						break;
					}
		}
		if(!page) { // No free memory was found, ask huge for the new page
			page = (Страница *)разместиХьюдж(1);
			LLOG("AllocK - allocated new system page " << (проц *)page << " " << k);
			page->фмт(k);
		}
		page->heap = this;
	}
	page->Линк(work[k]);
	ПРОВЕРЬ(page->klass == k);
	return page;
}

ук Куча::AllocK(цел k)
{
	LLOG("AllocK " << k);
	if(!initialized)
		иниц();
	Страница *page = work[k]->next;
	for(;;) {
		ПРОВЕРЬ(page->klass == k);
		СвобЛинк *p = page->freelist;
		if(p) {
			LLOG("AllocK allocating from " << (проц *)page << " " << (проц *)p);
			page->freelist = p->next;
			++page->active;
			return p;
		}
		LLOG("AllocK - page exhausted " << k << " page: " << (проц *)page);
		if(page->next != page) {
			LLOG("Moving " << (проц *)page << " to full");
			page->отлинкуй();
			page->Линк(full[k]);
			page = work[k]->next;
		}
		if(page->next == page)
			page = WorkPage(k);
	}
}

форс_инлайн
ук Куча::Allok(цел k)
{ // Try to alloc from the front-cache first
	LTIMING("Allok");
	СвобЛинк *укз = cache[k];
	if(укз) {
		cachen[k]++;
		cache[k] = укз->next;
		return проверьСвобОтлK(укз, k);
	}
	return проверьСвобОтлK(AllocK(k), k);
}

форс_инлайн
ук Куча::AllocSz(т_мера& sz)
{
	LTIMING("размести");
	LLOG("размести " << какТкст(sz));
	Stat(sz);
	if(sz <= 384) {
		if(sz == 0)
			sz = 1;
		цел k = ((цел)sz - 1) >> 5;
		sz = (k + 1) << 5;
		return Allok(k);
	}
	if(sz <= 992) {
		if(sz <= 448) {
			sz = 448;
			return Allok(12);
		}
		if(sz <= 576) {
			sz = 576;
			return Allok(13);
		}
		if(sz <= 672) {
			sz = 672;
			return Allok(14);
		}
		if(sz <= 800) {
			sz = 800;
			return Allok(15);
		}
		sz = 992;
		return Allok(16);
	}
	return бРазмести(sz);
}

форс_инлайн
проц Куча::FreeK(ук укз, Страница *page, цел k)
{
	if(page->freelist) {
		LLOG("освободи next in work page " << k);
		((СвобЛинк *)укз)->next = page->freelist;
	}
	else {
		LLOG("освободи full to work " << k << " heap: " << (проц *)page->heap);
		page->отлинкуй();
		page->Линк(work[k]);
		((СвобЛинк *)укз)->next = NULL;
	}
	page->freelist = (СвобЛинк *)укз;
	if(--page->active == 0) { // there are no more allocated blocks in this page
		LLOG("освободи page is empty " << (проц *)page);
		page->отлинкуй();
		if(this == &aux) {
			LLOG("...is aux " << какТкст(free_4KB));
			Стопор::Замок __(mutex);
			Free4KB(k, page);
		}
		else {
			if(empty[k]) { // Keep one hot empty page per klass in thread, put rest to 'aux' global storage
				Стопор::Замок __(mutex);
				Free4KB(k, empty[k]); // освободи текущ hot page to reserve/huge
			}
			empty[k] = page; // this empty page is now hot
		}
	}
}

проц Куча::Free4KB(цел k, Страница *page)
{ // put empty 4KB to aux reserve or back to huge blocks if the reserve is full
	LLOG("Global Free4KB " << k << " " << (проц *)empty);
	if(free_4KB < max_free_spages) { // only keep max_free_spages, release if more
		page->heap = &aux;
		page->next = aux.empty[k];
		aux.empty[k] = page;
		free_4KB++;
		LLOG("резервируй 4KB " << какТкст(free_4KB));
	}
	else {
		aux.освободиХьюдж(page);
		LLOG("освободиХьюдж 4KB " << какТкст(free_4KB));
	}
}

форс_инлайн
проц Куча::освободи(ук укз, Страница *page, цел k)
{
	LTIMING("Small освободи");
	LLOG("Small free page: " << (проц *)page << ", k: " << k << ", ksz: " << Ksz(k));
	ПРОВЕРЬ((4096 - ((uintptr_t)укз & (uintptr_t)4095)) % Ksz(k) == 0);
	if(page->heap != this) { // freeing block allocated in different thread
		RemoteFree(укз, Ksz(k)); // add to originating heap's list of free pages to be properly freed later
		return;
	}
	заполниСвобОтлK(укз, k);
	if(cachen[k]) {
		cachen[k]--;
		СвобЛинк *l = (СвобЛинк *)укз;
		l->next = cache[k];
		cache[k] = l;
		return;
	}
	FreeK(укз, page, k);
}

форс_инлайн
проц Куча::освободи(ук укз)
{
	if(!укз) return;
	LLOG("освободи " << укз);
	if(малый(укз)) {
		Страница *page = дайСтраницу(укз);
		освободи(укз, page, page->klass);
	}
	else
		бОсвободи(укз);
}

т_мера Куча::GetBlockSize(ук укз)
{
	if(!укз) return 0;
	LLOG("GetBlockSize " << укз);
	if(малый(укз)) {
		Страница *page = дайСтраницу(укз);
		цел k = page->klass;
		return Ksz(k);
	}
	return LGetBlockSize(укз);
}

проц Куча::SmallFreeDirect(ук укз)
{ // does not need to check for target heap or small vs large
	LLOG("освободи Direct " << укз);
	Страница *page = дайСтраницу(укз);
	ПРОВЕРЬ(page->heap == this);
	цел k = page->klass;
	LLOG("Small free page: " << (проц *)page << ", k: " << k << ", ksz: " << Ksz(k));
	ПРОВЕРЬ((4096 - ((uintptr_t)укз & (uintptr_t)4095)) % Ksz(k) == 0);
	заполниСвобОтлK(укз, k);
	FreeK(укз, page, k);
}

бул Куча::FreeSmallEmpty(цел size4KB, цел count)
{ // attempt to release small 4KB pages to gain count4KB space or count of releases
	бул released;
	do {
		released = false;
		for(цел i = 0; i < NKLASS; i++)
			if(aux.empty[i]) {
				Страница *q = aux.empty[i];
				aux.empty[i] = q->next;
				free_4KB--;
				ПРОВЕРЬ(free_4KB < max_free_spages);
				if(aux.освободиХьюдж(q) >= size4KB || --count <= 0) // освободиХьюдж is really static, aux needed just to compile
					return true;
				released = true;
			}
	}
	while(released);
	return false;
}

форс_инлайн
ук Куча::Alloc32()
{
	Stat(32);
	return Allok(КЛАСС_32);
}

форс_инлайн
проц Куча::освободи(ук укз, цел k)
{
	освободи(укз, дайСтраницу(укз), k);
}

форс_инлайн
проц Куча::Free32(ук укз)
{
	освободи(укз, КЛАСС_32);
}

форс_инлайн
ук Куча::Alloc48()
{
	Stat(48);
	return Allok(КЛАСС_48);
}

форс_инлайн
проц Куча::Free48(ук укз)
{
	освободи(укз, КЛАСС_48);
}

static thread_local бул heap_closed__;
static thread_local Куча *heap_tls__;

проц Куча::Shutdown()
{ // перемести all active blocks, "orphans", to global aux heap
	LLOG("**** Shutdown heap " << какТкст(this));
	Стопор::Замок __(mutex);
	heap_closed__ = true;
	heap_tls__ = NULL;
	иниц();
	RemoteFlushRaw(); // перемести remote blocks to originating heaps
	FreeRemoteRaw(); // освободи all remotely freed blocks
	for(цел i = 0; i < NKLASS; i++) { // move all small pages to aux (some heap will пикуй them later)
		LLOG("освободи cache " << какТкст(i));
		СвобЛинк *l = cache[i];
		while(l) {
			СвобЛинк *h = l;
			l = l->next;
			SmallFreeDirect(h);
		}
		while(full[i]->next != full[i]) {
			Страница *p = full[i]->next;
			p->отлинкуй();
			p->heap = &aux;
			p->Линк(aux.full[i]);
			LLOG("Orphan full " << (проц *)p);
		}
		while(work[i]->next != work[i]) {
			Страница *p = work[i]->next;
			p->отлинкуй();
			p->heap = &aux;
			p->Линк(p->freelist ? aux.work[i] : aux.full[i]);
			LLOG("Orphan work " << (проц *)p);
		}
		if(empty[i]) {
			ПРОВЕРЬ(empty[i]->freelist);
			ПРОВЕРЬ(empty[i]->active == 0);
			Free4KB(i, empty[i]);
			LLOG("Orphan empty " << (проц *)empty[i]);
		}
	}
	MoveLargeTo(&aux); // move all large pages to aux, some heap will пикуй them later
	memset(this, 0, sizeof(Куча));
	LLOG("++++ Done Shutdown heap " << какТкст(this));
}

никогда_inline
проц EnterHeapMutex()
{
	Куча::mutex.войди();
}

никогда_inline
проц LeaveHeapMutex()
{
	Куча::mutex.выйди();
}

struct HeapMutexLock {
	HeapMutexLock()  { EnterHeapMutex(); }
	~HeapMutexLock() { LeaveHeapMutex(); }
};

проц освободиПамThread();

struct HeapExitThreadGuard {
	проц Used() {}
	~HeapExitThreadGuard() { освободиПамThread(); }
};

Куча *MakeHeap()
{
	if(heap_closed__)
		return &Куча::aux;

	static thread_local HeapExitThreadGuard __;
	__.Used(); // "odr-used", register allocator to be shutdown at thread exit

	static thread_local ббайт sHeap__[sizeof(Куча)]; // zero initialization is fine for us
	heap_tls__ = (Куча *)sHeap__;

	return heap_tls__;
}

struct ТекущаяКуча {
	бул  locked = false;
	Куча *heap;
	
	Куча *operator->() { return heap; }

	ТекущаяКуча() {
		heap = heap_tls__;
		if(!heap) {
			EnterHeapMutex();
			locked = true;
			heap = MakeHeap();
		}
	}
	~ТекущаяКуча() {
		if(locked)
			LeaveHeapMutex();
	}
};

проц освободиПамk__(цел klass, ук укз)
{
	Куча *heap = heap_tls__;
	if(heap)
		heap->освободи((проц *)укз, klass);
	else {
		HeapMutexLock __;
		MakeHeap()->освободи((проц *)укз, klass);
	}
}

ук MemoryAllok__(цел klass)
{
	Куча *heap = heap_tls__;
	if(heap)
		return heap->Allok(klass);
	else {
		HeapMutexLock __;
		return MakeHeap()->Allok(klass);
	}
}

#if defined(HEAPDBG)

ук разместиПамТн_(т_мера& sz)
{
	return ТекущаяКуча()->AllocSz(sz);
}

проц  освободиПам_(ук укз)
{
	return ТекущаяКуча()->освободи(укз);
}

бул  MemoryпробуйПеремест_(ук укз, т_мера& size)
{
	return ТекущаяКуча()->пробуйПеремест(укз, size);
}

т_мера GetMemoryBlockSize_(ук укз)
{
	return ТекущаяКуча()->GetBlockSize(укз);
}

#else

#ifdef flagHEAPLOG

#undef AllocSz

СтатическийСтопор sHeapLogLock;

static FILE *sLog = fopen(дайФайлИзПапкиИсп("heap.log"), "w");

проц LogFree(ук укз)
{
	if(sLog) {
		Стопор::Замок __(sHeapLogLock);
		fprintf(sLog, "-%x %p\n", (цел)Нить::дайИдТекущ(), укз);
	}
}

ук LogAlloc(ук укз, т_мера sz)
{
	if(sLog) {
		Стопор::Замок __(sHeapLogLock);
		fprintf(sLog, "%x %zx %p\n", (цел)Нить::дайИдТекущ(), sz, укз);
	}
	return укз;
}

#else

inline проц LogFree(ук укз) {}

inline ук LogAlloc(ук укз, т_мера sz) { return укз; }

#endif

// xxx2 routines in the following code are to make things explicit for optimiser

никогда_inline
ук разместиПам2(т_мера& sz)
{
	HeapMutexLock __;
	return LogAlloc(MakeHeap()->AllocSz(sz), sz);
}

ук разместиПам(т_мера sz)
{
	LTIMING("разместиПам");
	Куча *heap = heap_tls__;
	if(heap)
		return LogAlloc(heap->AllocSz(sz), sz);
	return разместиПам2(sz);
}

ук разместиПамТн(т_мера& sz)
{
	LTIMING("разместиПамТн");
	Куча *heap = heap_tls__;
	if(heap)
		return LogAlloc(heap->AllocSz(sz), sz);
	return разместиПам2(sz);
}

никогда_inline
проц освободиПам2(ук укз)
{
	HeapMutexLock __;
	MakeHeap()->освободи(укз);
}

проц  освободиПам(ук укз)
{
	LTIMING("освободиПам");
	LogFree(укз);

	Куча *heap = heap_tls__;
	if(heap)
		heap->освободи(укз);
	else
		освободиПам2(укз);
}

т_мера дайРазмБлокаПам(ук укз)
{
	return ТекущаяКуча()->GetBlockSize(укз);
}

бул MemoryпробуйПеремест__(ук укз, т_мера& size)
{
	Куча *heap = heap_tls__;
	if(heap)
		return heap->пробуйПеремест(укз, size);
	else {
		HeapMutexLock __;
		return MakeHeap()->пробуйПеремест(укз, size);
	}
}

никогда_inline
ук разместиПам32_2()
{
	HeapMutexLock __;
	return LogAlloc(MakeHeap()->Alloc32(), 32);
}

форс_инлайн
ук разместиПам32_i()
{
	LTIMING("разместиПам32");
	Куча *heap = heap_tls__;
	if(heap)
		return LogAlloc(heap->Alloc32(), 32);
	return разместиПам32_2();
}

ук разместиПам32() { return разместиПам32_i(); }

никогда_inline
проц  освободиПам32_2(ук укз)
{
	HeapMutexLock __;
	MakeHeap()->Free32(укз);
}

форс_инлайн
проц  освободиПам32_i(ук укз)
{
	LTIMING("освободиПам32");
	LogFree(укз);
	Куча *heap = heap_tls__;
	if(heap)
		heap->Free32(укз);
	else
		освободиПам32_2(укз);
}

проц  освободиПам32(ук укз) { освободиПам32_i(укз); }

#endif

проц освободиПамThread()
{
	if(heap_closed__)
		return;
	ТекущаяКуча()->Shutdown();
}

проц проверьПам()
{
	ТекущаяКуча()->Check();
}

ПрофильПамяти::ПрофильПамяти()
{
	ТекущаяКуча()->сделай(*this);
}

проц MemoryDumpLarge()
{
	ТекущаяКуча()->DumpLarge();;
}

проц MemoryDumpHuge()
{
	ТекущаяКуча()->DumpHuge();
}

#ifdef HEAPDBG
форс_инлайн ук разместиПам32_i() { return разместиПам32(); }
форс_инлайн проц  освободиПам32_i(ук укз) { return освободиПам32(укз); }
#endif

#include "StringMem.i"

#endif

