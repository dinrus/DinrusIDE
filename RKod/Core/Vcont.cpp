#include "Core.h"

#define LLOG(x)

namespace РНЦПДинрус {

int64 NewInVectorSerial()
{
	static int64 x;
	INTERLOCKED {
		++x;
	}
	return x;
}


thread_local int64 invector_cache_serial_;
thread_local int   invector_cache_blki_;
thread_local int   invector_cache_offset_;
thread_local int   invector_cache_end_;

void SetInvectorCache__(int64 serial, int blki, int offset, int end)
{
	invector_cache_serial_ = serial;
	invector_cache_blki_ = blki;
	invector_cache_offset_ = offset;
	invector_cache_end_ = end;
}

void ClearInvectorCache__()
{
	invector_cache_serial_ = 0;
}

int FindInvectorCache__(int64 serial, int& pos, int& off)
{
	if(invector_cache_serial_ == serial && pos >= invector_cache_offset_ &&
	   pos < invector_cache_end_) {
		LLOG("Found in cache, serial: " << invector_cache_serial_ << ", offset: " << invector_cache_offset_ << ", end: " << invector_cache_end_);
		off = invector_cache_offset_;
		pos -= off;
		return invector_cache_blki_;
	}
	return -1;
}

void Биты::очисть()
{
	if(bp)
		MemoryFree(bp);
	alloc = 0;
	bp = NULL;
}

void Биты::переразмести(int nalloc)
{
	size_t sz = sizeof(dword) * nalloc;
	dword *nbp = (dword *)MemoryAllocSz(sz);
	nalloc = int(sz / sizeof(dword));
	if(bp) {
		копируй(nbp, bp, bp + min(alloc, nalloc));
		MemoryFree(bp);
	}
	if(nalloc > alloc)
		Fill(nbp + alloc, nbp + nalloc, (dword)0);
	bp = nbp;
	alloc = nalloc;
}

void Биты::расширь(int q)
{
	переразмести(3 * q / 2 + 1);
}

void Биты::резервируй(int nbits)
{
	int n = (nbits + 31) >> 5;
	if(n > alloc)
		переразмести(n);
}

int Биты::дайПоследн() const
{
	int lasti = alloc - 1;
	while(lasti > 0 && bp[lasti] == 0)
		lasti--;
	return lasti;
}

void Биты::сожми()
{
	int nalloc = дайПоследн() + 1;
	if(nalloc != alloc)
		переразмести(nalloc);
}

dword *Биты::создайРяд(int n_dwords)
{
	
	очисть();
	переразмести(n_dwords);
	return bp;
}

Ткст Биты::вТкст() const
{
	ТкстБуф ss;
	for(int i = дайПоследн(); i >= 0; i--)
		ss << фмтЦелГекс(bp[i]);
	return Ткст(ss);
}

void Биты::сериализуй(Поток& s)
{
	int dwords;
	if(s.сохраняется())
		dwords = дайПоследн() + 1;
	s % dwords;
	if(s.грузится())
		создайРяд(dwords);
	s.SerializeRaw(bp, dwords);
}

}
