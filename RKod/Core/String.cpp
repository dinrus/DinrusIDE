#include "Core.h"

namespace РНЦПДинрус {

#ifdef _ОТЛАДКА
void Ткст0::Dsyn()
{
	Ткст *d_str = static_cast<Ткст *>(this);
	d_str->s = старт();
	d_str->len = дайСчёт();
}
#endif

Ткст0::Rc Ткст0::voidptr[2];

#ifndef КУЧА_РНЦП

inline void  *MemoryAlloc32_i()              { return new byte[32]; }
inline void   MemoryFree32_i(void *ptr)      { delete[] (byte *)ptr; }

#include "StringMem.i"

#endif

bool Ткст0::LEq(const Ткст0& s) const
{
	int l = дайСчёт();
	return l == s.дайСчёт() && inline_memeq8_aligned(begin(), s.begin(), l);
}

hash_t Ткст0::LHashValue() const
{
	int l = LLen();
	if(l < 15) { // must be the same as small hash
#ifdef HASH64
		qword m[2];
		m[0] = m[1] = 0;
		memcpy8((char *)m, ptr, l);
		((byte *)m)[SLEN] = l;
		return комбинируйХэш(m[0], m[1]);
#else
		dword m[4];
		m[0] = m[1] = m[2] = m[3] = 0;
		memcpy8((char *)m, ptr, l);
		((byte *)m)[SLEN] = l;
		return комбинируйХэш(m[0], m[1], m[2], m[3]);
#endif
	}
	return memhash(ptr, l);
}

int Ткст0::CompareL(const Ткст0& s) const
{
	const char *a = старт();
	int la = дайДлину();
	const char *b = s.старт();
	int lb = s.дайДлину();
	int q = inline_memcmp_aligned(a, b, min(la, lb));
	return q ? q : сравниЗнак(la, lb);
}

char *Ткст0::вставь(int pos, int count, const char *s)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos <= дайСчёт());
	int len = дайСчёт();
	int newlen = len + count;
	if(newlen < len) // overflow, string >2GB
		паника("Ткст is too big!");
	char *str = (char *)старт();
	if(newlen < дайРазмест() && !IsSharedRef() && (!s || s < str || s > str + len)) {
		if(pos < len)
			memmove(str + pos + count, str + pos, len - pos);
		if(смолл_ли())
			SLen() = newlen;
		else
			LLen() = newlen;
		str[newlen] = 0;
		if(s)
			memcpy8(str + pos, s, count);
		Dsyn();
		return str + pos;
	}
	char kind;
	char *p = размести(max(len >= int((int64)2 * INT_MAX / 3) ? INT_MAX : len + (len >> 1), newlen),
	                kind);
	if(pos > 0)
		memcpy8(p, str, pos);
	if(pos < len)
		memcpy8(p + pos + count, str + pos, len - pos);
	if(s)
		memcpy8(p + pos, s, count);
	p[newlen] = 0;
	освободи();
	ptr = p;
	LLen() = newlen;
	SLen() = 15;
	chr[KIND] = kind;
	Dsyn();
	return ptr + pos;
}

void Ткст0::UnShare()
{
	if(IsSharedRef()) {
		int len = LLen();
		char kind;
		char *p = размести(len, kind);
		memcpy8(p, ptr, len + 1);
		освободи();
		chr[KIND] = kind;
		ptr = p;
	}
}

void Ткст0::SetSLen(int l)
{
	SLen() = l;
	memset(chr + l, 0, 15 - l);
}

void Ткст0::удали(int pos, int count)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos + count <= дайСчёт());
	UnShare();
	char *s = (char *)старт();
	memmove(s + pos, s + pos + count, дайСчёт() - pos - count + 1);
	if(смолл_ли())
		SetSLen(SLen() - count);
	else
		LLen() -= count;
	Dsyn();
}

void Ткст0::уст(int pos, int chr)
{
	ПРОВЕРЬ(pos >= 0 && pos < дайСчёт());
	UnShare();
	Ук()[pos] = chr;
}

void Ткст0::обрежь(int pos)
{
	ПРОВЕРЬ(pos >= 0 && pos <= дайСчёт());
	if(смолл_ли()) {
		chr[pos] = 0;
		SetSLen(pos);
	}
	else {
		UnShare();
		ptr[pos] = 0;
		LLen() = pos;
	}
	Dsyn();
}

void Ткст0::конкат(const char *s, int len)
{
	if(смолл_ли()) {
		if(SLen() + len < 14) {
			memcpy8(chr + SLen(), s, len);
			SLen() += len;
			chr[(int)SLen()] = 0;
			Dsyn();
			return;
		}
	}
	else
		if((int)LLen() + len < LAlloc() && !IsSharedRef()) {
			memcpy8(ptr + LLen(), s, len);
			LLen() += len;
			ptr[LLen()] = 0;
			Dsyn();
			return;
		}
	вставь(дайСчёт(), len, s);
}

void Ткст0::резервируй(int r)
{
	int l = дайСчёт();
	вставь(дайСчёт(), r, NULL);
	обрежь(l);
}

void Ткст::присвойДлин(const char *s, int slen)
{
	int  len = дайСчёт();
	char *str = (char *)старт();
	if(s >= str && s <= str + len)
		*this = Ткст(s, slen);
	else {
		Ткст0::освободи();
		Ткст0::уст0(s, slen);
	}
}

Ткст Ткст::дайПроц()
{
	Ткст s;
	s.ptr = (char *)(voidptr + 1);
	s.LLen() = 0;
	s.SLen() = 15;
	s.chr[KIND] = 50;
	return s;
}

bool Ткст::проц_ли() const
{
	return реф_ли() && ptr == (char *)(voidptr + 1);
}

ШТкст Ткст::вШТкст() const
{
	return ШТкст(старт(), дайСчёт());
}

int Ткст::дайСчётСим() const
{
	return дайДефНабСим() == НАБСИМ_УТФ8 ? длинаУтф32(старт(), дайСчёт()) : дайСчёт();
}

Ткст::Ткст(char16 *s) : Ткст(вУтф8(s)) {}
Ткст::Ткст(wchar *s) : Ткст(вУтф8(s)) {}

Ткст::Ткст(ТкстБуф& b)
{
	обнули();
	if(b.pbegin == b.буфер) {
		Ткст0::уст0(b.pbegin, (int)(uintptr_t)(b.pend - b.pbegin));
		return;
	}
	int l = b.дайДлину();
	if(l <= 14) {
		memcpy8(chr, b.pbegin, l);
		SLen() = l;
		b.освободи();
	}
	else {
		ptr = b.pbegin;
		ptr[l] = 0;
		SLen() = 15;
		LLen() = l;
		chr[KIND] = min(b.дайРазмест(), 255);
		if(дайРазмест() > 4 * дайДлину() / 3)
			сожми();
	}
	b.обнули();

//	char h[100];
//	DLOG(sprintf(h, "Ткст(ТкстБуф) end %p (%p)", ptr, this));
	Dsyn();
//	DLOG(sprintf(h, "Ткст(ТкстБуф) end2 %p (%p)", ptr, this));
}

void ТкстБуф::переразмести(dword n, const char *cat, int l)
{
	int al;
	size_t ep = pend - pbegin;
	if(n > INT_MAX)
		n = INT_MAX;
	bool realloced = false;
	char *p;
	if((int)(limit - pbegin) > 800) {
		size_t sz = sizeof(Rc) + n + 1;
		Rc *rc = (Rc *)pbegin - 1;
		if(MemoryTryRealloc(rc, sz)) {
			realloced = true;
			al = rc->alloc = (int)min((size_t)INT_MAX, sz - sizeof(Rc) - 1);
			p = pbegin;
		}
	}
	if(!realloced) {
		p = размести(n, al);
		memcpy8(p, pbegin, min((dword)дайДлину(), n));
	}
	if(cat) {
		if(ep + l > INT_MAX)
			паника("ТкстБуф is too big (>2GB)!");
		memcpy8(p + ep, cat, l);
		ep += l;
	}
	if(!realloced) {
		освободи();
		pbegin = p;
	}
	pend = pbegin + ep;
	limit = pbegin + al;
}

void ТкстБуф::расширь()
{
	переразмести(дайДлину() * 3 / 2);
	if(pend == limit)
		паника("ТкстБуф is too big!");
}

void ТкстБуф::устДлину(int l)
{
	if(l > дайРазмест())
		переразмести(l);
	pend = pbegin + l;
}

void ТкстБуф::сожми()
{
	int l = дайДлину();
	if(l < дайРазмест() && l > 14)
		переразмести(l);
	pend = pbegin + l;
}

void ТкстБуф::ReallocL(const char *s, int l)
{
	переразмести(max(дайДлину(), l) + дайДлину(), s, l);
}

Ткст обрежьЛево(const Ткст& str)
{
	const char *s = str;
	if(!пробел_ли(*s))
		return str;
	while(пробел_ли((byte)*s)) s++;
	return Ткст(s, str.стоп());
}

Ткст обрежьПраво(const Ткст& str)
{
	if(str.пустой())
		return str;
	const char *s = str.последний();
	if(!пробел_ли(*s))
		return str;
	while(s >= ~str && пробел_ли((byte)*s)) s--;
	return Ткст(~str, s + 1);
}

Ткст обрежьОба(const Ткст& str)
{
	return обрежьЛево(обрежьПраво(str));
}

Ткст обрежьЛево(const char *sw, int len, const Ткст& s)
{
	return s.начинаетсяС(sw, len) ? s.середина(len) : s;
}

Ткст обрежьПраво(const char *sw, int len, const Ткст& s)
{
	return s.заканчиваетсяНа(sw, len) ? s.середина(0, s.дайСчёт() - len) : s;
}

struct StringICompare__
{
	int encoding;
	int operator()(char a, char b) const { return взаг(a, encoding) - взаг(b, encoding); }

	StringICompare__(int e) : encoding(e) {}
};

int CompareNoCase(const Ткст& a, const Ткст& b)
{
	return CompareNoCase(вУтф32(a), вУтф32(b));
}

int CompareNoCase(const Ткст& a, const char *b)
{
	return CompareNoCase(вУтф32(a), вУтф32(b, (int)strlen(b)));
}

}
