#include <DinrusPro/DinrusPro.h>

#define LLOG(x)

namespace ДинрусРНЦП {

дол NewInVectorSerial()
{
	static дол x;
	INTERLOCKED {
		++x;
	}
	return x;
}


thread_local дол invector_cache_serial_;
thread_local цел   invector_cache_blki_;
thread_local цел   invector_cache_offset_;
thread_local цел   invector_cache_end_;

проц SetInvectorCache__(дол serial, цел blki, цел offset, цел end)
{
	invector_cache_serial_ = serial;
	invector_cache_blki_ = blki;
	invector_cache_offset_ = offset;
	invector_cache_end_ = end;
}

проц ClearInvectorCache__()
{
	invector_cache_serial_ = 0;
}

цел FindInvectorCache__(дол serial, цел& pos, цел& off)
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

проц Биты::очисть()
{
	if(bp)
		освободиПам(bp);
	alloc = 0;
	bp = NULL;
}

проц Биты::переразмести(цел nalloc)
{
	т_мера sz = sizeof(бцел) * nalloc;
	бцел *nbp = (бцел *)разместиПамТн(sz);
	nalloc = цел(sz / sizeof(бцел));
	if(bp) {
		копируй(nbp, bp, bp + мин(alloc, nalloc));
		освободиПам(bp);
	}
	if(nalloc > alloc)
		заполни(nbp + alloc, nbp + nalloc, (бцел)0);
	bp = nbp;
	alloc = nalloc;
}

проц Биты::расширь(цел q)
{
	переразмести(3 * q / 2 + 1);
}

проц Биты::резервируй(цел nbits)
{
	цел n = (nbits + 31) >> 5;
	if(n > alloc)
		переразмести(n);
}

цел Биты::дайПоследн() const
{
	цел lasti = alloc - 1;
	while(lasti > 0 && bp[lasti] == 0)
		lasti--;
	return lasti;
}

проц Биты::сожми()
{
	цел nalloc = дайПоследн() + 1;
	if(nalloc != alloc)
		переразмести(nalloc);
}

бцел *Биты::создайРяд(цел n_dwords)
{
	
	очисть();
	переразмести(n_dwords);
	return bp;
}

Ткст Биты::вТкст() const
{
	ТкстБуф ss;
	for(цел i = дайПоследн(); i >= 0; i--)
		ss << фмтЦелГекс(bp[i]);
	return Ткст(ss);
}

проц Биты::сериализуй(Поток& s)
{
	цел dwords;
	if(s.сохраняется())
		dwords = дайПоследн() + 1;
	s % dwords;
	if(s.грузится())
		создайРяд(dwords);
	s.SerializeRaw(bp, dwords);
}

}
