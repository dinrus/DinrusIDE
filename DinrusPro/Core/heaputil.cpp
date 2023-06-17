#include <DinrusPro/DinrusCore.h>

#ifdef КУЧА_РНЦП

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

#include "HeapImp.h"

проц паникаВнеПамяти(т_мера size)
{
	сим h[200];
	sprintf(h, "выведи of memory!\nnU++ allocated memory: %d KB", MemoryUsedKb());
	паника(h);
}

т_мера Куча::huge_4KB_count;
цел    Куча::free_4KB;
т_мера Куча::big_size;
т_мера Куча::big_count;
т_мера Куча::sys_size;
т_мера Куча::sys_count;
т_мера Куча::huge_chunks;
т_мера Куча::huge_4KB_count_max;

цел MemoryUsedKb()
{
	return цел(4 * (Куча::huge_4KB_count - Куча::free_4KB));
}

цел MemoryUsedKbMax()
{
	return цел(4 * Куча::huge_4KB_count_max);
}

ук SysAllocRaw(т_мера size, т_мера reqsize)
{
	ук укз = NULL;
#ifdef PLATFORM_WIN32
	укз = VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#elif PLATFORM_LINUX
	укз = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(укз == MAP_FAILED)
		укз = NULL;
#else
	укз = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	if(укз == MAP_FAILED)
		укз = NULL;
#endif
	if(!укз)
		паникаВнеПамяти(size);
	return укз;
}

проц  SysFreeRaw(ук укз, т_мера size)
{
#ifdef PLATFORM_WIN32
	VirtualFree(укз, 0, MEM_RELEASE);
#else
	munmap(укз, size);
#endif
}

ук разместиПамПерманентно(т_мера size)
{
	Стопор::Замок __(Куча::mutex);
	if(size > 10000)
		return SysAllocRaw(size, size);
	static ббайт *укз = NULL;
	static ббайт *limit = NULL;
	ПРОВЕРЬ(size < INT_MAX);
	if(укз + size >= limit) {
		укз = (ббайт *)SysAllocRaw(16384, 16384);
		limit = укз + 16384;
	}
	ук p = укз;
	укз += size;
	return p;
}

проц паникаКучи(кткст0 text, ук pos, цел size)
{
	RLOG("\n\n" << text << "\n");
	гексДамп(VppLog(), pos, size, 1024);
	паника(text);
}

#ifdef HEAPDBG

ук Куча::проверьСвобОтлK(ук p, цел k)
{
	Страница *page = дайСтраницу(p);
	ПРОВЕРЬ((ббайт *)page + sizeof(Страница) <= (ббайт *)p && (ббайт *)p < (ббайт *)page + 4096);
	ПРОВЕРЬ((4096 - ((uintptr_t)p & (uintptr_t)4095)) % Ksz(k) == 0);
	ПРОВЕРЬ(page->klass == k);
	проверьСвобОтл((СвобЛинк *)p + 1, Ksz(k) - sizeof(СвобЛинк));
	return p;
}

проц Куча::заполниСвобОтлK(ук p, цел k)
{
	заполниСвобОтл((СвобЛинк *)p + 1, Ksz(k) - sizeof(СвобЛинк));
}

#endif


проц Куча::сделай(ПрофильПамяти& f)
{
	Стопор::Замок __(mutex);
	memset((проц *)&f, 0, sizeof(ПрофильПамяти));
	for(цел i = 0; i < NKLASS; i++) {
		цел qq = Ksz(i) / 4;
		Страница *p = work[i]->next;
		while(p != work[i]) {
			f.allocated[qq] += p->active;
			f.fragments[qq] += p->счёт() - p->active;
			p = p->next;
		}
		p = full[i]->next;
		while(p != full[i]) {
			f.allocated[qq] += p->active;
			p = p->next;
		}
		if(empty[i])
			f.freepages++;
		p = aux.empty[i];
		while(p) {
			f.freepages++;
			p = p->next;
		}
	}
	DLink *m = large->next;
	while(m != large) {
		КучаЛардж::ЗагБлока *h = m->дайПерв();
		for(;;) {
			if(h->свободен_ли()) {
				f.large_fragments_count++;
				цел sz = LUNIT * h->дайРазм();
				f.large_fragments_total += sz;
				if(h->size < 2048)
					f.large_fragments[sz >> 8]++;
			}
			else {
				f.large_count++;
				f.large_total += LUNIT * h->size;
			}
			if(h->последний_ли())
				break;
			h = h->дайСледщЗаг();
		}
		m = m->next;
	}

	f.sys_count = (цел)sys_count;
	f.sys_total = sys_size;
	
	f.huge_count = цел(big_count - sys_count);
	f.huge_total = big_size - sys_size; // this is not 100% correct, but approximate
	
	f.master_chunks = (цел)huge_chunks;

	СтраницаХьюдж *pg = huge_pages;
	while(pg) {
		ПрефиксБлока *h = (ПрефиксБлока *)pg->page;
		for(;;) {
			if(h->свободен_ли()) {
				бкрат sz = h->дайРазм();
				f.huge_fragments[sz]++;
				f.huge_fragments_count++;
				f.huge_fragments_total += sz;
			}
			if(h->последний_ли())
				break;
			h = h->дайСледщЗаг(4096);
		}
		pg = pg->next;
	}
}

проц Куча::DumpLarge()
{
	Стопор::Замок __(mutex);
	DLink *m = large->next;
	auto& out = VppLog();
	while(m != large) {
		КучаЛардж::ЗагБлока *h = m->дайПерв();
		out << h << ": ";
		for(;;) {
			if(h->свободен_ли())
				out << "#";
			out << h->дайРазм() * 0.25 << ' ';
			if(h->последний_ли())
				break;
			h = h->дайСледщЗаг();
		}
		out << "\r\n";
		m = m->next;
	}
}

проц Куча::DumpHuge()
{
	Стопор::Замок __(mutex);
	СтраницаХьюдж *pg = huge_pages;
	auto& out = VppLog();
	while(pg) {
		ПрефиксБлока *h = (ПрефиксБлока *)pg->page;
		out << h << ": ";
		for(;;) {
			if(h->свободен_ли())
				out << "#";
			out << 4 * h->дайРазм() << ' ';
			if(h->последний_ли())
				break;
			h = h->дайСледщЗаг(4096);
		}
		out << "\r\n";
		pg = pg->next;
	}
}

Ткст какТкст(const ПрофильПамяти& mem)
{
	Ткст text;
#ifdef КУЧА_РНЦП
	цел acount = 0;
	т_мера asize = 0;
	цел fcount = 0;
	т_мера fsize = 0;
	text << "Memory peak: " << MemoryUsedKbMax() << " KB, текущ: " << MemoryUsedKb() << "KB \n";
	for(цел i = 0; i < 1024; i++)
		if(mem.allocated[i]) {
			цел sz = 4 * i;
			text << фмт("%4d B, %7d allocated (%6d KB), %6d fragments (%6d KB)\n",
			              sz, mem.allocated[i], (mem.allocated[i] * sz) >> 10,
			              mem.fragments[i], (mem.fragments[i] * sz) >> 10);
			acount += mem.allocated[i];
			asize += mem.allocated[i] * sz;
			fcount += mem.fragments[i];
			fsize += mem.fragments[i] * sz;
		}
	text << фмт(" TOTAL, %7d allocated (%6d KB), %6d fragments (%6d KB)\n",
	              acount, цел(asize >> 10), fcount, цел(fsize >> 10));
	text << "Empty 4KB pages " << mem.freepages << " (" << mem.freepages * 4 << " KB)\n";
	text << "Large block count " << mem.large_count
	     << ", total size " << (mem.large_total >> 10) << " KB\n";
	text << "Large fragments count " << mem.large_fragments_count
	     << ", total size " << (mem.large_fragments_total >> 10) << " KB\n";
	text << "Хьюдж block count " << mem.huge_count
	     << ", total size " << цел(mem.huge_total >> 10) << " KB\n";
	text << "Хьюдж fragments count " << mem.huge_fragments_count
	     << ", total size " << 4 * mem.huge_fragments_total << " KB\n";
	text << "сис block count " << mem.sys_count
	     << ", total size " << цел(mem.sys_total >> 10) << " KB\n";
	text << Куча::HPAGE * 4 / 1024 << "MB master blocks " << mem.master_chunks << "\n";
	text << "\nLarge fragments:\n";
	for(цел i = 0; i < 2048; i++)
		if(mem.large_fragments[i])
			text << 256.0 * i / 1024 << " KB: " << mem.large_fragments[i] << "\n";
	text << "\nHuge fragments:\n";
	for(цел i = 0; i < 65535; i++)
		if(mem.huge_fragments[i])
			text << i * 4 << " KB: " << mem.huge_fragments[i] << "\n";
#endif
	return text;
}

#ifdef flagHEAPSTAT
цел stat[65536];
цел bigstat;

проц Куча::Stat(т_мера sz)
{
	if(sz < 65536)
		stat[sz]++;
	else
		bigstat++;
}

ЭКЗИТБЛОК {
	цел sum = 0;
	for(цел i = 0; i < 65536; i++)
		sum += stat[i];
	sum += bigstat;
	цел total = 0;
	VppLog() << спринтф("Allocation statistics: (total allocations: %d)\n", sum);
	for(цел i = 0; i < 65536; i++)
		if(stat[i]) {
			total += stat[i];
			VppLog() << спринтф("%5d %8dx %2d%%, total %8dx %2d%%\n",
			                    i, stat[i], 100 * stat[i] / sum, total, 100 * total / sum);
		}
	if(bigstat) {
		total += bigstat;
		VppLog() << спринтф(">64KB %8dx %2d%%, total %8dx %2d%%\n",
		                    bigstat, 100 * bigstat / sum, total, 100 * total / sum);
	}
}
#endif

#endif
