#include "Core.h"

#ifdef КУЧА_РНЦП

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

namespace РНЦПДинрус {

#include "HeapImp.h"

void OutOfMemoryPanic(size_t size)
{
	char h[200];
	sprintf(h, "выведи of memory!\nnU++ allocated memory: %d KB", MemoryUsedKb());
	паника(h);
}

size_t Куча::huge_4KB_count;
int    Куча::free_4KB;
size_t Куча::big_size;
size_t Куча::big_count;
size_t Куча::sys_size;
size_t Куча::sys_count;
size_t Куча::huge_chunks;
size_t Куча::huge_4KB_count_max;

int MemoryUsedKb()
{
	return int(4 * (Куча::huge_4KB_count - Куча::free_4KB));
}

int MemoryUsedKbMax()
{
	return int(4 * Куча::huge_4KB_count_max);
}

void *SysAllocRaw(size_t size, size_t reqsize)
{
	void *ptr = NULL;
#ifdef PLATFORM_WIN32
	ptr = VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#elif PLATFORM_LINUX
	ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(ptr == MAP_FAILED)
		ptr = NULL;
#else
	ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	if(ptr == MAP_FAILED)
		ptr = NULL;
#endif
	if(!ptr)
		OutOfMemoryPanic(size);
	return ptr;
}

void  SysFreeRaw(void *ptr, size_t size)
{
#ifdef PLATFORM_WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, size);
#endif
}

void *MemoryAllocPermanent(size_t size)
{
	Стопор::Замок __(Куча::mutex);
	if(size > 10000)
		return SysAllocRaw(size, size);
	static byte *ptr = NULL;
	static byte *limit = NULL;
	ПРОВЕРЬ(size < INT_MAX);
	if(ptr + size >= limit) {
		ptr = (byte *)SysAllocRaw(16384, 16384);
		limit = ptr + 16384;
	}
	void *p = ptr;
	ptr += size;
	return p;
}

void паникаКучи(const char *text, void *pos, int size)
{
	RLOG("\n\n" << text << "\n");
	гексДамп(VppLog(), pos, size, 1024);
	паника(text);
}

#ifdef HEAPDBG

void *Куча::DbgFreeCheckK(void *p, int k)
{
	Page *page = дайСтраницу(p);
	ПРОВЕРЬ((byte *)page + sizeof(Page) <= (byte *)p && (byte *)p < (byte *)page + 4096);
	ПРОВЕРЬ((4096 - ((uintptr_t)p & (uintptr_t)4095)) % Ksz(k) == 0);
	ПРОВЕРЬ(page->klass == k);
	DbgFreeCheck((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
	return p;
}

void Куча::DbgFreeFillK(void *p, int k)
{
	DbgFreeFill((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
}

#endif


void Куча::сделай(ПрофильПамяти& f)
{
	Стопор::Замок __(mutex);
	memset((void *)&f, 0, sizeof(ПрофильПамяти));
	for(int i = 0; i < NKLASS; i++) {
		int qq = Ksz(i) / 4;
		Page *p = work[i]->next;
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
		LargeHeap::BlkHeader *h = m->дайПерв();
		for(;;) {
			if(h->IsFree()) {
				f.large_fragments_count++;
				int sz = LUNIT * h->дайРазм();
				f.large_fragments_total += sz;
				if(h->size < 2048)
					f.large_fragments[sz >> 8]++;
			}
			else {
				f.large_count++;
				f.large_total += LUNIT * h->size;
			}
			if(h->IsLast())
				break;
			h = h->GetNextHeader();
		}
		m = m->next;
	}

	f.sys_count = (int)sys_count;
	f.sys_total = sys_size;
	
	f.huge_count = int(big_count - sys_count);
	f.huge_total = big_size - sys_size; // this is not 100% correct, but approximate
	
	f.master_chunks = (int)huge_chunks;

	HugePage *pg = huge_pages;
	while(pg) {
		BlkPrefix *h = (BlkPrefix *)pg->page;
		for(;;) {
			if(h->IsFree()) {
				word sz = h->дайРазм();
				f.huge_fragments[sz]++;
				f.huge_fragments_count++;
				f.huge_fragments_total += sz;
			}
			if(h->IsLast())
				break;
			h = h->GetNextHeader(4096);
		}
		pg = pg->next;
	}
}

void Куча::DumpLarge()
{
	Стопор::Замок __(mutex);
	DLink *m = large->next;
	auto& out = VppLog();
	while(m != large) {
		LargeHeap::BlkHeader *h = m->дайПерв();
		out << h << ": ";
		for(;;) {
			if(h->IsFree())
				out << "#";
			out << h->дайРазм() * 0.25 << ' ';
			if(h->IsLast())
				break;
			h = h->GetNextHeader();
		}
		out << "\r\n";
		m = m->next;
	}
}

void Куча::DumpHuge()
{
	Стопор::Замок __(mutex);
	HugePage *pg = huge_pages;
	auto& out = VppLog();
	while(pg) {
		BlkPrefix *h = (BlkPrefix *)pg->page;
		out << h << ": ";
		for(;;) {
			if(h->IsFree())
				out << "#";
			out << 4 * h->дайРазм() << ' ';
			if(h->IsLast())
				break;
			h = h->GetNextHeader(4096);
		}
		out << "\r\n";
		pg = pg->next;
	}
}

Ткст какТкст(const ПрофильПамяти& mem)
{
	Ткст text;
#ifdef КУЧА_РНЦП
	int acount = 0;
	size_t asize = 0;
	int fcount = 0;
	size_t fsize = 0;
	text << "Memory peak: " << MemoryUsedKbMax() << " KB, текущ: " << MemoryUsedKb() << "KB \n";
	for(int i = 0; i < 1024; i++)
		if(mem.allocated[i]) {
			int sz = 4 * i;
			text << фмт("%4d B, %7d allocated (%6d KB), %6d fragments (%6d KB)\n",
			              sz, mem.allocated[i], (mem.allocated[i] * sz) >> 10,
			              mem.fragments[i], (mem.fragments[i] * sz) >> 10);
			acount += mem.allocated[i];
			asize += mem.allocated[i] * sz;
			fcount += mem.fragments[i];
			fsize += mem.fragments[i] * sz;
		}
	text << фмт(" TOTAL, %7d allocated (%6d KB), %6d fragments (%6d KB)\n",
	              acount, int(asize >> 10), fcount, int(fsize >> 10));
	text << "Empty 4KB pages " << mem.freepages << " (" << mem.freepages * 4 << " KB)\n";
	text << "Large block count " << mem.large_count
	     << ", total size " << (mem.large_total >> 10) << " KB\n";
	text << "Large fragments count " << mem.large_fragments_count
	     << ", total size " << (mem.large_fragments_total >> 10) << " KB\n";
	text << "Huge block count " << mem.huge_count
	     << ", total size " << int(mem.huge_total >> 10) << " KB\n";
	text << "Huge fragments count " << mem.huge_fragments_count
	     << ", total size " << 4 * mem.huge_fragments_total << " KB\n";
	text << "Sys block count " << mem.sys_count
	     << ", total size " << int(mem.sys_total >> 10) << " KB\n";
	text << Куча::HPAGE * 4 / 1024 << "MB master blocks " << mem.master_chunks << "\n";
	text << "\nLarge fragments:\n";
	for(int i = 0; i < 2048; i++)
		if(mem.large_fragments[i])
			text << 256.0 * i / 1024 << " KB: " << mem.large_fragments[i] << "\n";
	text << "\nHuge fragments:\n";
	for(int i = 0; i < 65535; i++)
		if(mem.huge_fragments[i])
			text << i * 4 << " KB: " << mem.huge_fragments[i] << "\n";
#endif
	return text;
}

#ifdef flagHEAPSTAT
int stat[65536];
int bigstat;

void Куча::Stat(size_t sz)
{
	if(sz < 65536)
		stat[sz]++;
	else
		bigstat++;
}

ЭКЗИТБЛОК {
	int sum = 0;
	for(int i = 0; i < 65536; i++)
		sum += stat[i];
	sum += bigstat;
	int total = 0;
	VppLog() << спринтф("Allocation statistics: (total allocations: %d)\n", sum);
	for(int i = 0; i < 65536; i++)
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

}

#endif
