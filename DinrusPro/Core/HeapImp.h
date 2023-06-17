#include <DinrusPro/DinrusCore.h>

struct Куча;

// This is used internally by U++ to manage large (64KB) and huge (up to 256MB) blocks

struct ПрефиксБлока { // this part is at the start of Blk allocated block, client must not touch it
	бкрат        prev_size;
	бкрат        size;
	бул        free;
	бул        last;
	Куча       *heap; // we need this for 4KB pages and large blocks, NULL for Хьюдж blocks
#ifdef CPU_32
	бцел       filler;
#endif

	проц  устРазм(бкрат sz)           { size = sz; }
	проц  устПредшРазм(бкрат sz)       { prev_size = sz; }
	проц  устСвободен(бул b)            { free = b; }
	проц  устПоследний(бул b)            { last = b; }

	бкрат  дайРазм()                  { return size; }
	бкрат  дайПредшРазм()              { return prev_size; }
	бул  первый_ли()                  { return дайПредшРазм() == 0; }
	бул  свободен_ли()                   { return free; }
	бул  последний_ли()                   { return last; }

	ПрефиксБлока *дайПредшЗаг(цел размБлк) { return (ПрефиксБлока *)((ббайт *)this - размБлк * дайПредшРазм()); }
	ПрефиксБлока *дайСледщЗаг(цел размБлк) { return (ПрефиксБлока *)((ббайт *)this + размБлк * дайРазм()); }
};

template <цел размБлк>
struct ЗагБлока_ : ПрефиксБлока { // free block record
	ЗагБлока_ *prev; // linked list of free blocks
	ЗагБлока_ *next; // linked list of free blocks

	ЗагБлока_ *дайПредшЗаг()      { return (ЗагБлока_ *)ПрефиксБлока::дайПредшЗаг(размБлк); }
	ЗагБлока_ *дайСледщЗаг()      { return (ЗагБлока_ *)ПрефиксБлока::дайСледщЗаг(размБлк); }

	проц  устСледщПредшТн()            { if(!последний_ли()) дайСледщЗаг()->устПредшРазм(дайРазм()); }

	проц  отлинкуйСвоб()               { Dbl_Unlink(this); }
};

template <typename Деталь, цел размБлк>
struct КучаБлока : Деталь {
	typedef ЗагБлока_<размБлк> ЗагБлока;
	typedef Деталь D;

	бул       присоединиСледщ(ЗагБлока *h, бкрат needs_count = 0);
	проц       разбей(ЗагБлока *h, бкрат wcount, бул fill = false);
	проц       добавьКусок(ЗагБлока *h, цел count);
	проц      *сделайРазмест(ЗагБлока *h, бкрат wcount);
	ЗагБлока *освободи(ЗагБлока *h); // returns final joined block
	бул       пробуйПеремест(ук укз, т_мера count, т_мера& n);
	проц       проверьБлок(ук page, цел size, бул check_heap = false);

	static проц  подтверди(бул b);
#ifdef HEAPDBG
	static проц  заполниСвобОтл(ук укз, т_мера size);
	static проц  проверьСвобОтл(ук укз, т_мера size);
	static проц  заполниСвоб(ЗагБлока *h);
	static проц  проверьСвоб(ЗагБлока *h);
#else
	static проц  заполниСвобОтл(ук укз, т_мера size) {}
	static проц  проверьСвобОтл(ук укз, т_мера size) {}
	static проц  заполниСвоб(ЗагБлока *h) {}
	static проц  проверьСвоб(ЗагБлока *h) {}
#endif
};

template <typename Деталь, цел размБлк>
проц КучаБлока<Деталь, размБлк>::подтверди(бул b)
{
	if(!b)
		паника("Куча повреждена!");
}

#ifdef HEAPDBG

template <typename Деталь, цел размБлк>
проц КучаБлока<Деталь, размБлк>::заполниСвобОтл(ук p, т_мера size)
{
	т_мера count = size >> 2;
	бцел *укз = (бцел *)p;
	while(count--)
		*укз++ = 0x65657246;
}

template <typename Деталь, цел размБлк>
проц КучаБлока<Деталь, размБлк>::проверьСвобОтл(ук p, т_мера size)
{
	т_мера count = size >> 2;
	бцел *укз = (бцел *)p;
	while(count--)
		if(*укз++ != 0x65657246)
			паника("Writes to freed blocks detected");
}

template <typename Деталь, цел размБлк>
проц КучаБлока<Деталь, размБлк>::заполниСвоб(ЗагБлока *h)
{
	if(размБлк == 4096) // it is too slow to check huge blocks
		return;
	заполниСвобОтл(h + 1, h->дайРазм() * размБлк - sizeof(ЗагБлока));
}

template <typename Деталь, цел размБлк>
проц КучаБлока<Деталь, размБлк>::проверьСвоб(ЗагБлока *h)
{
	if(размБлк == 4096) // it is too slow to check huge blocks
		return;
	проверьСвобОтл(h + 1, h->дайРазм() * размБлк - sizeof(ЗагБлока));
}

#endif

template <typename Деталь, цел размБлк>
проц КучаБлока<Деталь, размБлк>::проверьБлок(ук page, цел page_size, бул check_heap)
{
	#define CLOG(x) // LOG(x)
	CLOG("=== " << какТкст(page_size) << " " << какТкст(page));
	ПрефиксБлока *h = (ПрефиксБлока *)page;
	цел size = 0;
	цел psize = 0;
	подтверди(h->первый_ли());
	for(;;) {
		size += h->дайРазм();
		CLOG("h: " << какТкст(h) << ", дайРазм: " << какТкст(h->дайРазм())
		     << ", size: " << какТкст(size) << ", islast: " << какТкст(h->последний_ли()));
		подтверди(h->дайРазм());
		подтверди(h->дайПредшРазм() == psize);
		подтверди(!check_heap || h->свободен_ли() || h->heap);
		if(h->свободен_ли())
			проверьСвоб((ЗагБлока *)h);
		psize = h->дайРазм();
		if(h->последний_ли() && size == page_size)
			return;
		подтверди(size < page_size);
		h = h->дайСледщЗаг(размБлк);
	}
	#undef CLOG
}

template <typename Деталь, цел размБлк>
форс_инлайн
бул КучаБлока<Деталь, размБлк>::присоединиСледщ(ЗагБлока *h, бкрат needs_count)
{ // try to join with next header if it is free, does not link it to free
	if(h->последний_ли())
		return false;
	ЗагБлока *nh = h->дайСледщЗаг();
	if(!nh->свободен_ли() || h->дайРазм() + nh->дайРазм() < needs_count)
		return false;
	проверьСвоб(nh);
	h->устПоследний(nh->последний_ли());
	nh->отлинкуйСвоб();
	бкрат nsz = h->дайРазм() + nh->дайРазм();
	h->устРазм(nsz);
	h->устСледщПредшТн();
	return true;
}

template <typename Деталь, цел размБлк>
форс_инлайн
проц КучаБлока<Деталь, размБлк>::разбей(ЗагБлока *h, бкрат wcount, бул fill)
{ // splits the block if bigger, links new block to free
	ПРОВЕРЬ(размБлк != 4096 || ((бцел)(uintptr_t)h & 4095) == 0);
	ЗагБлока *h2 = (ЗагБлока *)((ббайт *)h + размБлк * (цел)wcount);
	бкрат nsz = h->дайРазм() - wcount;
	if(nsz == 0) // nothing to split
		return;

	h2->устСвободен(true);
	h2->устПоследний(h->последний_ли());
	h2->устРазм(nsz);
	h2->устПредшРазм(wcount);
	h2->устСледщПредшТн();
	D::линкуйСвод(h2);
	if(fill)
		заполниСвоб(h2);

	h->устРазм(wcount);
	h->устПоследний(false);
}

template <typename Деталь, цел размБлк>
проц КучаБлока<Деталь, размБлк>::добавьКусок(ЗагБлока *h, цел count)
{
	h->устРазм(count);
	h->устПредшРазм(0); // is first
	h->устПоследний(true);
	h->устСвободен(true);
	D::линкуйСвод(h);
	заполниСвоб(h);
}

template <typename Деталь, цел размБлк>
форс_инлайн
ук КучаБлока<Деталь, размБлк>::сделайРазмест(ЗагБлока *h, бкрат wcount)
{
	h->отлинкуйСвоб();
	h->устСвободен(false);
	разбей(h, wcount);
	проверьСвоб(h);
	return h;
}

template <typename Деталь, цел размБлк>
бул КучаБлока<Деталь, размБлк>::пробуйПеремест(ук укз, т_мера count, т_мера& n)
{
	ПРОВЕРЬ(count);

	ЗагБлока *h = (ЗагБлока *)укз;
	if(h->size == 0)
		return false;

	бкрат sz = h->дайРазм();
	if(sz != count) {
		if(!присоединиСледщ(h, (бкрат)count) && count > sz)
			return false;
		разбей(h, (бкрат)count, true);
		n = n - sz + count;
	}
	return true;
}

template <typename Деталь, цел размБлк>
форс_инлайн
typename КучаБлока<Деталь, размБлк>::ЗагБлока *КучаБлока<Деталь, размБлк>::освободи(ЗагБлока *h)
{
	ПРОВЕРЬ(размБлк != 4096 || ((бцел)(uintptr_t)h & 4095) == 0);
	присоединиСледщ(h);
	if(!h->первый_ли()) { // try to join with previous header if it is free
		ЗагБлока *ph = h->дайПредшЗаг();
		if(ph->свободен_ли()) {
			ph->отлинкуйСвоб(); // remove because size will change, might end in another bucket then
			бкрат nsz = ph->дайРазм() + h->дайРазм();
			ph->устРазм(nsz);
			ph->устПоследний(h->последний_ли());
			ph->устСледщПредшТн();
			h = ph;
		}
	}
	h->устСвободен(true);
	D::линкуйСвод(h); // was not joined with previous header
	заполниСвоб(h);
	return h;
}

struct ДетальКучиХьюдж {
	static ЗагБлока_<4096> freelist[20][1];

	static цел  Cv(цел n)                         { return n < 16 ? 0 : значимыеБиты(n - 16) + 1; }
	static проц линкуйСвод(ЗагБлока_<4096> *h)     { Dbl_LinkAfter(h, freelist[Cv(h->дайРазм())]); }
	static проц новРазмСвоб(ЗагБлока_<4096> *h)  {}
};

struct Куча : КучаБлока<ДетальКучиХьюдж, 4096> {
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
	static цел  max_free_spages; // maximum free small pages kept in reserve (but разместиХьюдж also converts them)
	static бкрат сис_блок_лимит; // > this (in 4KB) blocks are managed directly by system

	ук разместиХьюдж(т_мера count); // count in 4KB, client needs to not touch HugePrefix
	цел   освободиХьюдж(ук укз);
	бул  пробуйПереместХьюдж(ук укз, т_мера count);

	static цел Ksz(цел k) { // small block size classes
		static цел sz[] = {
		//  0   1   2   3    4    5    6    7    8    9    10   11
			32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384,
			448, 576, 672, 800, 992, 8, 16, 24, 40, 48, 56
		//  12   13   14   15   16  17  18  19  20  21  22
		//  8 - 56 sizes are only available with разместиТини
		};
		static_assert(__количество(sz) == 23, "NKLASS mismatch");
		return sz[k];
	}

	struct СвобЛинк {
		СвобЛинк *next;
	};

	struct Страница : ПрефиксБлока { // small block Страница
		ббайт         klass;    // size class
		бкрат         active;   // number of used (active) blocks in this page
		СвобЛинк    *freelist; // single linked list of free blocks in Страница
		Страница        *next;     // Pages are in work/full/empty lists
		Страница        *prev;

		проц         линкуйся()            { Dbl_Self(this); }
		проц         отлинкуй()              { Dbl_Unlink(this); }
		проц         линкуй(Страница *lnk)       { Dbl_LinkAfter(this, lnk);  }

		проц         фмт(цел k);

		ббайт *старт()                      { return (ббайт *)this + sizeof(Страница); }
		ббайт *стоп()                        { return (ббайт *)this + 4096; }
		цел   счёт()                      { return (цел)(uintptr_t)(стоп() - старт()) / Ksz(klass); }
	};

	struct ДетальКучиЛардж {
		ЗагБлока_<LUNIT> freelist[25][1];
		проц линкуйСвод(ЗагБлока_<LUNIT> *h);
	};

	struct КучаЛардж : КучаБлока<ДетальКучиЛардж, LUNIT> {};

	typedef КучаЛардж::ЗагБлока ЛЗагБлока;

	struct DLink : ПрефиксБлока { // Large Страница header / big block header
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
		проц         линкуй(DLink *lnk)      { Dbl_LinkAfter(this, lnk);  }

		КучаЛардж::ЗагБлока *дайПерв()   { return (КучаЛардж::ЗагБлока *)((ббайт *)this + LOFFSET); } // pointer to данные area
	};

	static цел lclass[];
	static цел free_lclass[LPAGE + 1];
	static цел alloc_lclass[LPAGE + 1];

	static_assert(sizeof(ПрефиксБлока) == 16, "Wrong sizeof(ПрефиксБлока)");
	static_assert(sizeof(DLink) == 64, "Wrong sizeof(DLink)");

	static СтатическийСтопор mutex;

	Страница      work[NKLASS][1];   // circular list of pages that contain some empty blocks
	Страница      full[NKLASS][1];   // circular list of pages that contain NO empty blocks
	Страница     *empty[NKLASS];     // last fully freed page per klass (hot reserve) / shared global list of empty pages in aux
	СвобЛинк *cache[NKLASS];     // hot frontend cache of small blocks
	цел       cachen[NKLASS];    // counter of small blocks that are allowed to be stored in cache

	бул      initialized;

	КучаЛардж lheap;
	DLink     large[1]; // all large 64KB chunks that belong to this heap
	цел       free_lpages; // empty large pages (in reserve)

	проц     *out[REMOTE_OUT_SZ / 8 + 1];
	проц    **out_ptr;
	цел       out_size;

	ббайт      filler1[64]; // make sure the next ПЕРЕМЕННАЯ is in distinct cacheline

	СвобЛинк *small_remote_list; // list of remotely freed small blocks for lazy reclamation
	СвобЛинк *large_remote_list; // list of remotely freed large blocks for lazy reclamation

	struct СтраницаХьюдж { // to store the list of all huge pages in permanent storage
		проц     *page;
		СтраницаХьюдж *next;
	};

	static СтраницаХьюдж *huge_pages;

	static DLink  big[1]; // Список of all big blocks
	static Куча   aux;    // Сингл global auxiliary heap to store orphans and global list of free pages

	static т_мера huge_4KB_count; // total number of 4KB pages in small/large/huge blocks
	static цел    free_4KB; // number of empty 4KB pages (linked in aux.empty)
	static т_мера big_size; // blocks >~64KB
	static т_мера big_count;
	static т_мера sys_size;  // blocks allocated directly from system (included in big too)
	static т_мера sys_count;
	static т_мера huge_chunks; // 32MB master pages
	static т_мера huge_4KB_count_max; // peak huge memory allocated
	static СтраницаХьюдж *free_huge_pages; // list of records of freed hpages (to be reused)
	static цел       free_hpages; // empty huge pages (in reserve)

#ifdef HEAPDBG
	static проц  заполниСвобОтлK(ук укз, цел k);
	static ук проверьСвобОтлK(ук p, цел k);
#else
	static проц  заполниСвобОтлK(ук укз, цел k) {}
	static ук проверьСвобОтлK(ук p, цел k) { return p; }
#endif

#ifdef flagHEAPSTAT
	static проц  Stat(т_мера sz);
#else
	static проц  Stat(т_мера sz) {}
#endif

	проц  иниц();

	static цел   проверьСвоб(СвобЛинк *l, цел k, бул page = true);
	проц  Check();
	static проц  DblCheck(Страница *p);
	static проц  подтвердиLeaks(бул b);

	static бул  малый(ук укз) { return (((бцел)(uintptr_t)укз) & 16) == 0; }
	static Страница *дайСтраницу(ук укз) { return (Страница *)((uintptr_t)укз & ~(uintptr_t)4095); }

	Страница *WorkPage(цел k);
	ук AllocK(цел k);
	проц  FreeK(ук укз, Страница *page, цел k);
	ук Allok(цел k);
	проц  освободи(ук укз, Страница *page, цел k);
	проц  освободи(ук укз, цел k);
	проц  Free4KB(цел k, Страница *page);

	static бул FreeSmallEmpty(цел size4KB, цел count = INT_MAX);

	проц   иницЛ();
	ук TryLAlloc(цел i0, бкрат wcount);
	ук бРазмести(т_мера& size);
	проц   FreeLargePage(DLink *l);
	проц   бОсвободи(ук укз);
	бул   пробуйПереместЛ(ук укз, т_мера& newsize);
	т_мера LGetBlockSize(ук укз);

	проц   сделай(ПрофильПамяти& f);
	проц   DumpLarge();
	проц   DumpHuge();

	static проц сожми();

	проц SmallFreeDirect(ук укз);

	проц RemoteFlushRaw();
	проц RemoteFlush();
	проц RemoteFree(ук укз, цел size);
	проц SmallFreeRemoteRaw(СвобЛинк *list);
	проц SmallFreeRemoteRaw() { SmallFreeRemoteRaw(small_remote_list); small_remote_list = NULL; }
	проц SmallFreeRemote();
	проц LargeFreeRemoteRaw(СвобЛинк *list);
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

	бул   пробуйПеремест(ук укз, т_мера& newsize);
};

форс_инлайн
проц Куча::RemoteFlushRaw()
{ // transfer all buffered freed remote blocks to target heaps, no locking
	if(!initialized)
		иниц();
	for(проц **o = out; o < out_ptr; o++) {
		СвобЛинк *f = (СвобЛинк *)*o;
		Куча *heap = дайСтраницу(f)->heap;
		f->next = heap->small_remote_list;
		heap->small_remote_list = f;
	}
	out_ptr = out;
	out_size = 0;
}

форс_инлайн
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

форс_инлайн
проц Куча::SmallFreeRemoteRaw(СвобЛинк *list)
{
	while(list) {
		СвобЛинк *f = list;
		list = list->next;
		SmallFreeDirect(f);
	}
}

форс_инлайн
проц Куча::SmallFreeRemote()
{
	while(small_remote_list) { // avoid mutex if likely nothing to free
		СвобЛинк *list;
		{ // only пикуй values in mutex, resolve later
			Стопор::Замок __(mutex);
			list = small_remote_list;
			small_remote_list = NULL;
		}
		SmallFreeRemoteRaw(list);
	}
}

форс_инлайн
проц Куча::LargeFreeRemoteRaw(СвобЛинк *list)
{
	while(list) {
		СвобЛинк *f = list;
		list = list->next;
		бОсвободи(f);
	}
}

форс_инлайн
проц Куча::LargeFreeRemote()
{
	while(large_remote_list) { // avoid mutex if likely nothing to free
		СвобЛинк *list;
		{ // only пикуй values in mutex, resolve later
			Стопор::Замок __(mutex);
			list = large_remote_list;
			large_remote_list = NULL;
		}
		LargeFreeRemoteRaw(list);
	}
}

