проц  OutOfMemoryPanic();

ук SysAllocRaw(т_мера size, т_мера reqsize);
проц  SysFreeRaw(ук укз, т_мера size);

кткст0 какТкст(цел i);
кткст0 какТкст(ук укз);

struct Куча;

// This is used internally by U++ to manage large (64KB) and huge (up to 256MB) blocks

struct BlkPrefix { // this part is at the start of Blk allocated block, client must not touch it
	бкрат        prev_size;
	бкрат        size;
	бул        free;
	бул        last;
	Куча       *heap; // we need this for 4KB pages and large blocks, NULL for Huge blocks
#ifdef CPU_32
	бцел       filler;
#endif

	проц  устРазм(бкрат sz)           { size = sz; }
	проц  SetPrevSize(бкрат sz)       { prev_size = sz; }
	проц  SetFree(бул b)            { free = b; }
	проц  SetLast(бул b)            { last = b; }

	бкрат  дайРазм()                  { return size; }
	бкрат  GetPrevSize()              { return prev_size; }
	бул  IsFirst()                  { return GetPrevSize() == 0; }
	бул  IsFree()                   { return free; }
	бул  IsLast()                   { return last; }

	BlkPrefix *GetPrevHeader(цел BlkSize) { return (BlkPrefix *)((ббайт *)this - BlkSize * GetPrevSize()); }
	BlkPrefix *GetNextHeader(цел BlkSize) { return (BlkPrefix *)((ббайт *)this + BlkSize * дайРазм()); }
};

template <цел BlkSize>
struct BlkHeader_ : BlkPrefix { // free block record
	BlkHeader_ *prev; // linked list of free blocks
	BlkHeader_ *next; // linked list of free blocks

	BlkHeader_ *GetPrevHeader()      { return (BlkHeader_ *)BlkPrefix::GetPrevHeader(BlkSize); }
	BlkHeader_ *GetNextHeader()      { return (BlkHeader_ *)BlkPrefix::GetNextHeader(BlkSize); }

	проц  SetNextPrevSz()            { if(!IsLast()) GetNextHeader()->SetPrevSize(дайРазм()); }

	проц  UnlinkFree()               { Dbl_Unlink(this); }
};

template <typename Detail, цел BlkSize>
struct BlkHeap : Detail {
	typedef BlkHeader_<BlkSize> BlkHeader;
	typedef Detail D;

	бул       JoinNext(BlkHeader *h, бкрат needs_count = 0);
	проц       разбей(BlkHeader *h, бкрат wcount, бул fill = false);
	проц       AddChunk(BlkHeader *h, цел count);
	проц      *MakeAlloc(BlkHeader *h, бкрат wcount);
	BlkHeader *освободи(BlkHeader *h); // returns final joined block
	бул       TryRealloc(ук укз, т_мера count, т_мера& n);
	проц       BlkCheck(ук page, цел size, бул check_heap = false);

	static проц  Assert(бул b);
#ifdef HEAPDBG
	static проц  DbgFreeFill(ук укз, т_мера size);
	static проц  DbgFreeCheck(ук укз, т_мера size);
	static проц  FillFree(BlkHeader *h);
	static проц  CheckFree(BlkHeader *h);
#else
	static проц  DbgFreeFill(ук укз, т_мера size) {}
	static проц  DbgFreeCheck(ук укз, т_мера size) {}
	static проц  FillFree(BlkHeader *h) {}
	static проц  CheckFree(BlkHeader *h) {}
#endif
};

template <typename Detail, цел BlkSize>
проц BlkHeap<Detail, BlkSize>::Assert(бул b)
{
	if(!b)
		паника("Куча повреждена!");
}

#ifdef HEAPDBG

template <typename Detail, цел BlkSize>
проц BlkHeap<Detail, BlkSize>::DbgFreeFill(ук p, т_мера size)
{
	т_мера count = size >> 2;
	бцел *укз = (бцел *)p;
	while(count--)
		*укз++ = 0x65657246;
}

template <typename Detail, цел BlkSize>
проц BlkHeap<Detail, BlkSize>::DbgFreeCheck(ук p, т_мера size)
{
	т_мера count = size >> 2;
	бцел *укз = (бцел *)p;
	while(count--)
		if(*укз++ != 0x65657246)
			паника("Writes to freed blocks detected");
}

template <typename Detail, цел BlkSize>
проц BlkHeap<Detail, BlkSize>::FillFree(BlkHeader *h)
{
	if(BlkSize == 4096) // it is too slow to check huge blocks
		return;
	DbgFreeFill(h + 1, h->дайРазм() * BlkSize - sizeof(BlkHeader));
}

template <typename Detail, цел BlkSize>
проц BlkHeap<Detail, BlkSize>::CheckFree(BlkHeader *h)
{
	if(BlkSize == 4096) // it is too slow to check huge blocks
		return;
	DbgFreeCheck(h + 1, h->дайРазм() * BlkSize - sizeof(BlkHeader));
}

#endif

template <typename Detail, цел BlkSize>
проц BlkHeap<Detail, BlkSize>::BlkCheck(ук page, цел page_size, бул check_heap)
{
	#define CLOG(x) // LOG(x)
	CLOG("=== " << какТкст(page_size) << " " << какТкст(page));
	BlkPrefix *h = (BlkPrefix *)page;
	цел size = 0;
	цел psize = 0;
	Assert(h->IsFirst());
	for(;;) {
		size += h->дайРазм();
		CLOG("h: " << какТкст(h) << ", дайРазм: " << какТкст(h->дайРазм())
		     << ", size: " << какТкст(size) << ", islast: " << какТкст(h->IsLast()));
		Assert(h->дайРазм());
		Assert(h->GetPrevSize() == psize);
		Assert(!check_heap || h->IsFree() || h->heap);
		if(h->IsFree())
			CheckFree((BlkHeader *)h);
		psize = h->дайРазм();
		if(h->IsLast() && size == page_size)
			return;
		Assert(size < page_size);
		h = h->GetNextHeader(BlkSize);
	}
	#undef CLOG
}

template <typename Detail, цел BlkSize>
force_inline
бул BlkHeap<Detail, BlkSize>::JoinNext(BlkHeader *h, бкрат needs_count)
{ // try to join with next header if it is free, does not link it to free
	if(h->IsLast())
		return false;
	BlkHeader *nh = h->GetNextHeader();
	if(!nh->IsFree() || h->дайРазм() + nh->дайРазм() < needs_count)
		return false;
	CheckFree(nh);
	h->SetLast(nh->IsLast());
	nh->UnlinkFree();
	бкрат nsz = h->дайРазм() + nh->дайРазм();
	h->устРазм(nsz);
	h->SetNextPrevSz();
	return true;
}

template <typename Detail, цел BlkSize>
force_inline
проц BlkHeap<Detail, BlkSize>::разбей(BlkHeader *h, бкрат wcount, бул fill)
{ // splits the block if bigger, links new block to free
	ПРОВЕРЬ(BlkSize != 4096 || ((бцел)(uintptr_t)h & 4095) == 0);
	BlkHeader *h2 = (BlkHeader *)((ббайт *)h + BlkSize * (цел)wcount);
	бкрат nsz = h->дайРазм() - wcount;
	if(nsz == 0) // nothing to split
		return;

	h2->SetFree(true);
	h2->SetLast(h->IsLast());
	h2->устРазм(nsz);
	h2->SetPrevSize(wcount);
	h2->SetNextPrevSz();
	D::LinkFree(h2);
	if(fill)
		FillFree(h2);

	h->устРазм(wcount);
	h->SetLast(false);
}

template <typename Detail, цел BlkSize>
проц BlkHeap<Detail, BlkSize>::AddChunk(BlkHeader *h, цел count)
{
	h->устРазм(count);
	h->SetPrevSize(0); // is first
	h->SetLast(true);
	h->SetFree(true);
	D::LinkFree(h);
	FillFree(h);
}

template <typename Detail, цел BlkSize>
force_inline
ук BlkHeap<Detail, BlkSize>::MakeAlloc(BlkHeader *h, бкрат wcount)
{
	h->UnlinkFree();
	h->SetFree(false);
	разбей(h, wcount);
	CheckFree(h);
	return h;
}

template <typename Detail, цел BlkSize>
бул BlkHeap<Detail, BlkSize>::TryRealloc(ук укз, т_мера count, т_мера& n)
{
	ПРОВЕРЬ(count);

	BlkHeader *h = (BlkHeader *)укз;
	if(h->size == 0)
		return false;

	бкрат sz = h->дайРазм();
	if(sz != count) {
		if(!JoinNext(h, (бкрат)count) && count > sz)
			return false;
		разбей(h, (бкрат)count, true);
		n = n - sz + count;
	}
	return true;
}

template <typename Detail, цел BlkSize>
force_inline
typename BlkHeap<Detail, BlkSize>::BlkHeader *BlkHeap<Detail, BlkSize>::освободи(BlkHeader *h)
{
	ПРОВЕРЬ(BlkSize != 4096 || ((бцел)(uintptr_t)h & 4095) == 0);
	JoinNext(h);
	if(!h->IsFirst()) { // try to join with previous header if it is free
		BlkHeader *ph = h->GetPrevHeader();
		if(ph->IsFree()) {
			ph->UnlinkFree(); // remove because size will change, might end in another bucket then
			бкрат nsz = ph->дайРазм() + h->дайРазм();
			ph->устРазм(nsz);
			ph->SetLast(h->IsLast());
			ph->SetNextPrevSz();
			h = ph;
		}
	}
	h->SetFree(true);
	D::LinkFree(h); // was not joined with previous header
	FillFree(h);
	return h;
}

struct HugeHeapDetail {
	static BlkHeader_<4096> freelist[20][1];

	static цел  Cv(цел n)                         { return n < 16 ? 0 : значимыеБиты(n - 16) + 1; }
	static проц LinkFree(BlkHeader_<4096> *h)     { Dbl_LinkAfter(h, freelist[Cv(h->дайРазм())]); }
	static проц NewFreeSize(BlkHeader_<4096> *h)  {}
};

struct Куча : BlkHeap<HugeHeapDetail, 4096> {
	enum {
		LUNIT = 256, // granularity of large blocks (size always a multiple of this)
		LPAGE = 255, // number of LUNITs in large page (need to fix freelist and lclass if changing)
		LOFFSET = 64, // offset from 64KB start to the first block header

		NKLASS = 23, // number of small size classes

		REMOTE_OUT_SZ = 2000, // maximum size of remote frees to be buffered to flush at once
	};

	// allocator options:
	static бкрат HPAGE; // size of master page, in 4KB units
	static цел  max_free_hpages; // maximum free master pages kept in reserve (if more, they are returned to the system)
	static цел  max_free_lpages; // maximum free large pages kept in reserve (if more, they are returned to huge system)
	static цел  max_free_spages; // maximum free small pages kept in reserve (but HugeAlloc also converts them)
	static бкрат сис_блок_лимит; // > this (in 4KB) blocks are managed directly by system

	ук HugeAlloc(т_мера count); // count in 4KB, client needs to not touch HugePrefix
	цел   HugeFree(ук укз);
	бул  HugeTryRealloc(ук укз, т_мера count);

	static цел Ksz(цел k) { // small block size classes
		static цел sz[] = {
		//  0   1   2   3    4    5    6    7    8    9    10   11
			32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384,
			448, 576, 672, 800, 992, 8, 16, 24, 40, 48, 56
		//  12   13   14   15   16  17  18  19  20  21  22
		//  8 - 56 sizes are only available with TinyAlloc
		};
		static_assert(__countof(sz) == 23, "NKLASS mismatch");
		return sz[k];
	}

	struct FreeLink {
		FreeLink *next;
	};

	struct Страница : BlkPrefix { // small block Страница
		ббайт         klass;    // size class
		бкрат         active;   // number of used (active) blocks in this page
		FreeLink    *freelist; // single linked list of free blocks in Страница
		Страница        *next;     // Pages are in work/full/empty lists
		Страница        *prev;

		проц         линкуйся()            { Dbl_Self(this); }
		проц         отлинкуй()              { Dbl_Unlink(this); }
		проц         Линк(Страница *lnk)       { Dbl_LinkAfter(this, lnk);  }

		проц         фмт(цел k);

		ббайт *старт()                      { return (ббайт *)this + sizeof(Страница); }
		ббайт *стоп()                        { return (ббайт *)this + 4096; }
		цел   счёт()                      { return (цел)(uintptr_t)(стоп() - старт()) / Ksz(klass); }
	};

	struct LargeHeapDetail {
		BlkHeader_<LUNIT> freelist[25][1];
		проц LinkFree(BlkHeader_<LUNIT> *h);
	};

	struct LargeHeap : BlkHeap<LargeHeapDetail, LUNIT> {};

	typedef LargeHeap::BlkHeader LBlkHeader;

	struct DLink : BlkPrefix { // Large Страница header / big block header
		DLink       *next;
		DLink       *prev;
		т_мера       size; // only used for big header
	#ifdef CPU_64
		бцел        filler[6]; // sizeof need to be 64 because of alignment
	#else
		бцел        filler[9];
	#endif

		проц         линкуйся()            { Dbl_Self(this); }
		проц         отлинкуй()              { Dbl_Unlink(this); }
		проц         Линк(DLink *lnk)      { Dbl_LinkAfter(this, lnk);  }

		LargeHeap::BlkHeader *дайПерв()   { return (LargeHeap::BlkHeader *)((ббайт *)this + LOFFSET); } // pointer to данные area
	};

	static цел lclass[];
	static цел free_lclass[LPAGE + 1];
	static цел alloc_lclass[LPAGE + 1];

	static_assert(sizeof(BlkPrefix) == 16, "Wrong sizeof(BlkPrefix)");
	static_assert(sizeof(DLink) == 64, "Wrong sizeof(DLink)");

	static СтатическийСтопор mutex;

	Страница      work[NKLASS][1];   // circular list of pages that contain some empty blocks
	Страница      full[NKLASS][1];   // circular list of pages that contain NO empty blocks
	Страница     *empty[NKLASS];     // last fully freed page per klass (hot reserve) / shared global list of empty pages in aux
	FreeLink *cache[NKLASS];     // hot frontend cache of small blocks
	цел       cachen[NKLASS];    // counter of small blocks that are allowed to be stored in cache

	бул      initialized;

	LargeHeap lheap;
	DLink     large[1]; // all large 64KB chunks that belong to this heap
	цел       free_lpages; // empty large pages (in reserve)

	проц     *out[REMOTE_OUT_SZ / 8 + 1];
	проц    **out_ptr;
	цел       out_size;

	ббайт      filler1[64]; // make sure the next ПЕРЕМЕННАЯ is in distinct cacheline

	FreeLink *small_remote_list; // list of remotely freed small blocks for lazy reclamation
	FreeLink *large_remote_list; // list of remotely freed large blocks for lazy reclamation

	struct HugePage { // to store the list of all huge pages in permanent storage
		проц     *page;
		HugePage *next;
	};

	static HugePage *huge_pages;

	static DLink  big[1]; // Список of all big blocks
	static Куча   aux;    // Single global auxiliary heap to store orphans and global list of free pages

	static т_мера huge_4KB_count; // total number of 4KB pages in small/large/huge blocks
	static цел    free_4KB; // number of empty 4KB pages (linked in aux.empty)
	static т_мера big_size; // blocks >~64KB
	static т_мера big_count;
	static т_мера sys_size;  // blocks allocated directly from system (included in big too)
	static т_мера sys_count;
	static т_мера huge_chunks; // 32MB master pages
	static т_мера huge_4KB_count_max; // peak huge memory allocated
	static HugePage *free_huge_pages; // list of records of freed hpages (to be reused)
	static цел       free_hpages; // empty huge pages (in reserve)

#ifdef HEAPDBG
	static проц  DbgFreeFillK(ук укз, цел k);
	static ук DbgFreeCheckK(ук p, цел k);
#else
	static проц  DbgFreeFillK(ук укз, цел k) {}
	static ук DbgFreeCheckK(ук p, цел k) { return p; }
#endif

#ifdef flagHEAPSTAT
	static проц  Stat(т_мера sz);
#else
	static проц  Stat(т_мера sz) {}
#endif

	проц  иниц();

	static цел   CheckFree(FreeLink *l, цел k, бул page = true);
	проц  Check();
	static проц  DblCheck(Страница *p);
	static проц  AssertLeaks(бул b);

	static бул  смолл_ли(ук укз) { return (((бцел)(uintptr_t)укз) & 16) == 0; }
	static Страница *дайСтраницу(ук укз) { return (Страница *)((uintptr_t)укз & ~(uintptr_t)4095); }

	Страница *WorkPage(цел k);
	ук AllocK(цел k);
	проц  FreeK(ук укз, Страница *page, цел k);
	ук Allok(цел k);
	проц  освободи(ук укз, Страница *page, цел k);
	проц  освободи(ук укз, цел k);
	проц  Free4KB(цел k, Страница *page);

	static бул FreeSmallEmpty(цел size4KB, цел count = INT_MAX);

	проц   LИниt();
	ук TryLAlloc(цел i0, бкрат wcount);
	ук LAlloc(т_мера& size);
	проц   FreeLargePage(DLink *l);
	проц   LFree(ук укз);
	бул   LTryRealloc(ук укз, т_мера& newsize);
	т_мера LGetBlockSize(ук укз);

	проц   сделай(ПрофильПамяти& f);
	проц   DumpLarge();
	проц   DumpHuge();

	static проц сожми();

	проц SmallFreeDirect(ук укз);

	проц RemoteFlushRaw();
	проц RemoteFlush();
	проц RemoteFree(ук укз, цел size);
	проц SmallFreeRemoteRaw(FreeLink *list);
	проц SmallFreeRemoteRaw() { SmallFreeRemoteRaw(small_remote_list); small_remote_list = NULL; }
	проц SmallFreeRemote();
	проц LargeFreeRemoteRaw(FreeLink *list);
	проц LargeFreeRemoteRaw() { LargeFreeRemoteRaw(large_remote_list); large_remote_list = NULL; }
	проц LargeFreeRemote();
	проц FreeRemoteRaw();
	static проц MoveLargeTo(DLink *ml, Куча *to_heap);
	проц MoveLargeTo(Куча *to_heap);

	проц Shutdown();
	static проц AuxFinalCheck();

	ук AllocSz(т_мера& sz);
	проц   освободи(ук укз);
	т_мера GetBlockSize(ук укз);
	ук Alloc32();
	проц   Free32(ук укз);
	ук Alloc48();
	проц   Free48(ук укз);

	бул   TryRealloc(ук укз, т_мера& newsize);
};

force_inline
проц Куча::RemoteFlushRaw()
{ // transfer all buffered freed remote blocks to target heaps, no locking
	if(!initialized)
		иниц();
	for(проц **o = out; o < out_ptr; o++) {
		FreeLink *f = (FreeLink *)*o;
		Куча *heap = дайСтраницу(f)->heap;
		f->next = heap->small_remote_list;
		heap->small_remote_list = f;
	}
	out_ptr = out;
	out_size = 0;
}

force_inline
проц Куча::RemoteFree(ук укз, цел size)
{ // буфер freed remote block until REMOTE_OUT_SZ is reached
	if(!initialized)
		иниц();
	ПРОВЕРЬ(out_ptr <= out + REMOTE_OUT_SZ / 8 + 1);
	*out_ptr++ = укз;
	out_size += size;
	if(out_size >= REMOTE_OUT_SZ) {
		Стопор::Замок __(mutex);
		RemoteFlushRaw();
	}
}

force_inline
проц Куча::SmallFreeRemoteRaw(FreeLink *list)
{
	while(list) {
		FreeLink *f = list;
		list = list->next;
		SmallFreeDirect(f);
	}
}

force_inline
проц Куча::SmallFreeRemote()
{
	while(small_remote_list) { // avoid mutex if likely nothing to free
		FreeLink *list;
		{ // only pick values in mutex, resolve later
			Стопор::Замок __(mutex);
			list = small_remote_list;
			small_remote_list = NULL;
		}
		SmallFreeRemoteRaw(list);
	}
}

force_inline
проц Куча::LargeFreeRemoteRaw(FreeLink *list)
{
	while(list) {
		FreeLink *f = list;
		list = list->next;
		LFree(f);
	}
}

force_inline
проц Куча::LargeFreeRemote()
{
	while(large_remote_list) { // avoid mutex if likely nothing to free
		FreeLink *list;
		{ // only pick values in mutex, resolve later
			Стопор::Замок __(mutex);
			list = large_remote_list;
			large_remote_list = NULL;
		}
		LargeFreeRemoteRaw(list);
	}
}