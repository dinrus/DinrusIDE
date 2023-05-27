#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

#ifdef _ОТЛАДКА
проц Ткст0::Dsyn()
{
	Ткст *d_str = static_cast<Ткст *>(this);
	d_str->s = старт();
	d_str->len = дайСчёт();
}
#endif

Ткст0::Rc Ткст0::voidptr[2];

#ifndef КУЧА_РНЦП

inline ук разместиПам32_i()              { return new ббайт[32]; }
inline проц   освободиПам32_i(ук укз)      { delete[] (ббайт *)укз; }

#include "StringMem.i"

#endif

бул Ткст0::дРавно(const Ткст0& s) const
{
	цел l = дайСчёт();
	return l == s.дайСчёт() && inline_равнпам8_aligned(begin(), s.begin(), l);
}

т_хэш Ткст0::дЗначХэша() const
{
	цел l = LLen();
	if(l < 15) { // must be the same as small hash
#ifdef HASH64
		qword m[2];
		m[0] = m[1] = 0;
		копирпам8((char *)m, укз, l);
		((ббайт *)m)[SLEN] = l;
		return комбинируйХэш(m[0], m[1]);
#else
		бцел m[4];
		m[0] = m[1] = m[2] = m[3] = 0;
		копирпам8((char *)m, укз, l);
		((ббайт *)m)[SLEN] = l;
		return комбинируйХэш(m[0], m[1], m[2], m[3]);
#endif
	}
	return хэшпам(укз, l);
}

цел Ткст0::сравниД(const Ткст0& s) const
{
	кткст0 a = старт();
	цел la = дайДлину();
	кткст0 b = s.старт();
	цел lb = s.дайДлину();
	цел q = inline_memcmp_aligned(a, b, мин(la, lb));
	return q ? q : сравниЗнак(la, lb);
}

char *Ткст0::вставь(цел pos, цел count, кткст0 s)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos <= дайСчёт());
	цел len = дайСчёт();
	цел newlen = len + count;
	if(newlen < len) // overflow, string >2GB
		паника("Ткст is too big!");
	char *str = (char *)старт();
	if(newlen < дайРазмест() && !шарссыл_ли() && (!s || s < str || s > str + len)) {
		if(pos < len)
			memmove(str + pos + count, str + pos, len - pos);
		if(смолл_ли())
			SLen() = newlen;
		else
			LLen() = newlen;
		str[newlen] = 0;
		if(s)
			копирпам8(str + pos, s, count);
		Dsyn();
		return str + pos;
	}
	char kind;
	char *p = размести(макс(len >= цел((дол)2 * INT_MAX / 3) ? INT_MAX : len + (len >> 1), newlen),
	                kind);
	if(pos > 0)
		копирпам8(p, str, pos);
	if(pos < len)
		копирпам8(p + pos + count, str + pos, len - pos);
	if(s)
		копирпам8(p + pos, s, count);
	p[newlen] = 0;
	освободи();
	укз = p;
	LLen() = newlen;
	SLen() = 15;
	chr[KIND] = kind;
	Dsyn();
	return укз + pos;
}

проц Ткст0::разшарь()
{
	if(шарссыл_ли()) {
		цел len = LLen();
		char kind;
		char *p = размести(len, kind);
		копирпам8(p, укз, len + 1);
		освободи();
		chr[KIND] = kind;
		укз = p;
	}
}

проц Ткст0::устДлинТ(цел l)
{
	SLen() = l;
	memset(chr + l, 0, 15 - l);
}

проц Ткст0::удали(цел pos, цел count)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos + count <= дайСчёт());
	разшарь();
	char *s = (char *)старт();
	memmove(s + pos, s + pos + count, дайСчёт() - pos - count + 1);
	if(смолл_ли())
		устДлинТ(SLen() - count);
	else
		LLen() -= count;
	Dsyn();
}

проц Ткст0::уст(цел pos, цел chr)
{
	ПРОВЕРЬ(pos >= 0 && pos < дайСчёт());
	разшарь();
	Ук()[pos] = chr;
}

проц Ткст0::обрежь(цел pos)
{
	ПРОВЕРЬ(pos >= 0 && pos <= дайСчёт());
	if(смолл_ли()) {
		chr[pos] = 0;
		устДлинТ(pos);
	}
	else {
		разшарь();
		укз[pos] = 0;
		LLen() = pos;
	}
	Dsyn();
}

проц Ткст0::кат(кткст0 s, цел len)
{
	if(смолл_ли()) {
		if(SLen() + len < 14) {
			копирпам8(chr + SLen(), s, len);
			SLen() += len;
			chr[(цел)SLen()] = 0;
			Dsyn();
			return;
		}
	}
	else
		if((цел)LLen() + len < LAlloc() && !шарссыл_ли()) {
			копирпам8(укз + LLen(), s, len);
			LLen() += len;
			укз[LLen()] = 0;
			Dsyn();
			return;
		}
	вставь(дайСчёт(), len, s);
}

проц Ткст0::резервируй(цел r)
{
	цел l = дайСчёт();
	вставь(дайСчёт(), r, NULL);
	обрежь(l);
}

проц Ткст::присвойДлин(кткст0 s, цел slen)
{
	цел  len = дайСчёт();
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
	s.укз = (char *)(voidptr + 1);
	s.LLen() = 0;
	s.SLen() = 15;
	s.chr[KIND] = 50;
	return s;
}

бул Ткст::проц_ли() const
{
	return реф_ли() && укз == (char *)(voidptr + 1);
}

ШТкст Ткст::вШТкст() const
{
	return ШТкст(старт(), дайСчёт());
}

цел Ткст::дайСчётСим() const
{
	return дайДефНабСим() == НАБСИМ_УТФ8 ? длинаУтф32(старт(), дайСчёт()) : дайСчёт();
}

Ткст::Ткст(char16 *s) : Ткст(вУтф8(s)) {}
Ткст::Ткст(шим *s) : Ткст(вУтф8(s)) {}

Ткст::Ткст(ТкстБуф& b)
{
	обнули();
	if(b.pbegin == b.буфер) {
		Ткст0::уст0(b.pbegin, (цел)(uintptr_t)(b.pend - b.pbegin));
		return;
	}
	цел l = b.дайДлину();
	if(l <= 14) {
		копирпам8(chr, b.pbegin, l);
		SLen() = l;
		b.освободи();
	}
	else {
		укз = b.pbegin;
		укз[l] = 0;
		SLen() = 15;
		LLen() = l;
		chr[KIND] = мин(b.дайРазмест(), 255);
		if(дайРазмест() > 4 * дайДлину() / 3)
			сожми();
	}
	b.обнули();

//	char h[100];
//	DLOG(sprintf(h, "Ткст(ТкстБуф) end %p (%p)", укз, this));
	Dsyn();
//	DLOG(sprintf(h, "Ткст(ТкстБуф) end2 %p (%p)", укз, this));
}

проц ТкстБуф::переразмести(бцел n, кткст0 cat, цел l)
{
	цел al;
	т_мера ep = pend - pbegin;
	if(n > INT_MAX)
		n = INT_MAX;
	бул realloced = false;
	char *p;
	if((цел)(limit - pbegin) > 800) {
		т_мера sz = sizeof(Rc) + n + 1;
		Rc *rc = (Rc *)pbegin - 1;
		if(пробуйПереместПам(rc, sz)) {
			realloced = true;
			al = rc->alloc = (цел)мин((т_мера)INT_MAX, sz - sizeof(Rc) - 1);
			p = pbegin;
		}
	}
	if(!realloced) {
		p = размести(n, al);
		копирпам8(p, pbegin, мин((бцел)дайДлину(), n));
	}
	if(cat) {
		if(ep + l > INT_MAX)
			паника("ТкстБуф is too big (>2GB)!");
		копирпам8(p + ep, cat, l);
		ep += l;
	}
	if(!realloced) {
		освободи();
		pbegin = p;
	}
	pend = pbegin + ep;
	limit = pbegin + al;
}

проц ТкстБуф::расширь()
{
	переразмести(дайДлину() * 3 / 2);
	if(pend == limit)
		паника("ТкстБуф is too big!");
}

проц ТкстБуф::устДлину(цел l)
{
	if(l > дайРазмест())
		переразмести(l);
	pend = pbegin + l;
}

проц ТкстБуф::сожми()
{
	цел l = дайДлину();
	if(l < дайРазмест() && l > 14)
		переразмести(l);
	pend = pbegin + l;
}

проц ТкстБуф::переразместиД(кткст0 s, цел l)
{
	переразмести(макс(дайДлину(), l) + дайДлину(), s, l);
}

Ткст обрежьЛево(const Ткст& str)
{
	кткст0 s = str;
	if(!пробел_ли(*s))
		return str;
	while(пробел_ли((ббайт)*s)) s++;
	return Ткст(s, str.стоп());
}

Ткст обрежьПраво(const Ткст& str)
{
	if(str.пустой())
		return str;
	кткст0 s = str.последний();
	if(!пробел_ли(*s))
		return str;
	while(s >= ~str && пробел_ли((ббайт)*s)) s--;
	return Ткст(~str, s + 1);
}

Ткст обрежьОба(const Ткст& str)
{
	return обрежьЛево(обрежьПраво(str));
}

Ткст обрежьЛево(кткст0 sw, цел len, const Ткст& s)
{
	return s.начинаетсяС(sw, len) ? s.середина(len) : s;
}

Ткст обрежьПраво(кткст0 sw, цел len, const Ткст& s)
{
	return s.заканчиваетсяНа(sw, len) ? s.середина(0, s.дайСчёт() - len) : s;
}

struct StringICompare__
{
	цел encoding;
	цел operator()(char a, char b) const { return взаг(a, encoding) - взаг(b, encoding); }

	StringICompare__(цел e) : encoding(e) {}
};

цел сравнилюб(const Ткст& a, const Ткст& b)
{
	return сравнилюб(вУтф32(a), вУтф32(b));
}

цел сравнилюб(const Ткст& a, кткст0 b)
{
	return сравнилюб(вУтф32(a), вУтф32(b, (цел)strlen(b)));
}

}
