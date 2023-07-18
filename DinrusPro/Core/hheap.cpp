#include <DinrusPro/DinrusCore.h>

#define LTIMING(x)  // RTIMING(x)
// #define LSTAT


#ifdef КУЧА_РНЦП

#include "HeapImp.h"

// this part reserves very large (HPAGE*4 KB, default 16MB)
// chunks form the system and then serves as 4KB rounded allocator
// used as manager of huge memory blocks. 4KB and 64KB blocks are allocated from here too
// also able to deal with bigger blocks, those are directly allocated / freed from system

ЗагБлока_<4096> ДетальКучиХьюдж::freelist[20][1]; // only single global Хьюдж heap...
Куча::СтраницаХьюдж *Куча::huge_pages;

#ifdef LSTAT
static цел hstat[65536];

ЭКЗИТБЛОК {
	цел cnt = 0;
	for(цел i = 0; i < 65536; i++) {
		cnt += hstat[i];
		if(hstat[i])
			RLOG(i * 4 << " KB: " << hstat[i] << " / " << cnt);
	}
}
#endif

т_мера sKBLimit = INT_MAX;

проц  MemoryLimitKb(цел kb)
{
	sKBLimit = kb;
}

static ПрофильПамяти *sPeak;

ПрофильПамяти *PeakMemoryProfile()
{
	if(sPeak)
		return sPeak;
	sPeak = (ПрофильПамяти *)разместиПамПерманентно(sizeof(ПрофильПамяти));
	memset((проц *)sPeak, 0, sizeof(ПрофильПамяти));
	return NULL;
}

Куча::СтраницаХьюдж *Куча::free_huge_pages;
цел             Куча::free_hpages;

ук Куча::разместиХьюдж(т_мера count) // count in 4kb pages
{
	LTIMING("разместиХьюдж");
	ПРОВЕРЬ(count);

#ifdef LSTAT
	if(count < 65536)
		hstat[count]++;
#endif

	huge_4KB_count += count;

	auto MaxMem = [&] {
		if(huge_4KB_count > huge_4KB_count_max) {
			huge_4KB_count_max = huge_4KB_count;
			if(4 * (Куча::huge_4KB_count - Куча::free_4KB) > sKBLimit)
				паника("MemoryLimitKb breached!");
			if(sPeak)
				сделай(*sPeak);
		}
	};

	if(!D::freelist[0]->next) { // initialization
		for(цел i = 0; i < __количество(D::freelist); i++)
			Dbl_Self(D::freelist[i]);
	}
		
	if(count > сис_блок_лимит) { // we are wasting 4KB to store just 4 bytes here, but this is n MB after all..
		LTIMING("SysAlloc");
		ббайт *sysblk = (ббайт *)SysAllocRaw((count + 1) * 4096, 0);
		ЗагБлока *h = (ЗагБлока *)(sysblk + 4096);
		h->size = 0;
		*((т_мера *)sysblk) = count;
		sys_count++;
		sys_size += 4096 * count;
		MaxMem();
		return h;
	}
	
	LTIMING("Хьюдж размести");

	бкрат wcount = (бкрат)count;
	
	for(цел pass = 0; pass < 2; pass++) {
		for(цел i = Cv(wcount); i < __количество(D::freelist); i++) {
			ЗагБлока *l = D::freelist[i];
			ЗагБлока *h = l->next;
			while(h != l) {
				бкрат sz = h->дайРазм();
				if(sz >= count) {
					if(h->первый_ли() && h->последний_ли()) // this is whole free page
						free_hpages--;
					ук укз = сделайРазмест(h, wcount);
					MaxMem();
					return укз;
				}
				h = h->next;
			}
		}

		if(!FreeSmallEmpty(wcount, INT_MAX)) { // try to coalesce 4KB small free blocks back to huge storage
			ук укз = SysAllocRaw(HPAGE * 4096, 0); // failed, add HPAGE from the system

			СтраницаХьюдж *pg; // record in set of huge pages
			if(free_huge_pages) {
				pg = free_huge_pages;
				free_huge_pages = free_huge_pages->next;
			}
			else
				pg = (СтраницаХьюдж *)разместиПамПерманентно(sizeof(СтраницаХьюдж));

			pg->page = укз;
			pg->next = huge_pages;
			huge_pages = pg;
			huge_chunks++;
			free_hpages++;
			добавьКусок((ЗагБлока *)укз, HPAGE);
		}
	}
	паника("выведи of memory");
	return NULL;
}

цел Куча::освободиХьюдж(ук укз)
{
	LTIMING("освободиХьюдж");
	ЗагБлока *h = (ЗагБлока *)укз;
	if(h->size == 0) {
		LTIMING("сис освободи");
		ббайт *sysblk = (ббайт *)h - 4096;
		т_мера count = *((т_мера *)sysblk);
		SysFreeRaw(sysblk, (count + 1) * 4096);
		huge_4KB_count -= count;
		sys_count--;
		sys_size -= 4096 * count;
		return 0;
	}
	LTIMING("Хьюдж освободи");
	huge_4KB_count -= h->дайРазм();
	h = КучаБлока::освободи(h);
	цел sz = h->дайРазм();
	if(h->первый_ли() && h->последний_ли()) {
		if(free_hpages >= max_free_hpages) { // we have enough pages in the reserve, return to the system
			LTIMING("освободи Хьюдж Страница");
			h->отлинкуйСвоб();
			СтраницаХьюдж *p = NULL;
			while(huge_pages) { // remove the page from the set of huge pages
				СтраницаХьюдж *n = huge_pages->next;
				if(huge_pages->page != h) {
					huge_pages->next = p;
					p = huge_pages;
				}
				huge_pages = n;
			}
			huge_pages = p;
			huge_chunks--;
			SysFreeRaw(h, sz * 4096);
		}
		else
			free_hpages++;
	}
	return sz;
}

бул Куча::пробуйПереместХьюдж(ук укз, т_мера count)
{
	return count <= HPAGE && КучаБлока::пробуйПеремест(укз, count, huge_4KB_count);
}

#endif

