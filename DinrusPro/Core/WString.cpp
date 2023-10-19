#include <DinrusPro/DinrusCore.h>

шим *ШТкст0::размести(цел& count)
{
	if(count <= МАЛЫЙ) {
		count = МАЛЫЙ;
		шим *p = (шим *)разместиПам((МАЛЫЙ + 1) * sizeof(шим));
		return p;
	}
	т_мера sz = sizeof(Атомар) + ((т_мера)count + 1) * sizeof(шим);
	Атомар *rc = (Атомар *)разместиПамТн(sz);
	if(count != INT_MAX)
		count = цел(((sz - sizeof(Атомар)) / sizeof(шим)) - 1);
	*rc = 1;
	return (шим *)(rc + 1);
}

проц ШТкст0::освободи()
{
	if(alloc > 0) {
		if(IsRc()) {
			Атомар& rc = Rc();
			if(атомнДек(rc) == 0)
				освободиПам(&rc);
		}
		else
			освободиПам(укз);
	}
}

#ifdef _ОТЛАДКА
проц ШТкст0::Dsyn()
{
	ШТкст *d_str = static_cast<ШТкст *>(this);
	d_str->s = старт();
	d_str->len = дайСчёт();
}
#endif

шим *ШТкст0::вставь(цел pos, цел count, const шим *s)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos <= дайСчёт());
	цел newlen = length + count;
	if(newlen < length)
		паника("ШТкст is too big!");
	if(newlen < alloc && !совместный() && (!s || s < укз || s > укз + length)) {
		if(pos < length)
			memmove(укз + pos + count, укз + pos, (length - pos) * sizeof(шим));
		length = newlen;
		укз[newlen] = 0;
		if(s)
			т_копирпам(укз + pos, s, count);
		Dsyn();
		return укз + pos;
	}
	цел all = макс(length >= цел((дол)2 * INT_MAX / 3) ? INT_MAX : length + (length >> 1), newlen);
	шим *p = размести(all);
	if(pos > 0)
		т_копирпам(p, укз, pos);
	if(pos < length)
		т_копирпам(p + pos + count, укз + pos, length - pos);
	if(s)
		т_копирпам(p + pos, s, count);
	ПРОВЕРЬ(pos + count <= all);
	p[newlen] = 0;
	освободи();
	укз = p;
	length = newlen;
	alloc = all;
	Dsyn();
	return укз + pos;
}

проц ШТкст0::уст0(const ШТкст0& ист)
{
	if(ист.alloc <= 0) {
		static шим h[2];
		укз = h;
		length = 0;
		alloc = ист.alloc;
		Dsyn();
		return;
	}
	if(ист.length == 0) {
		обнули();
		return;
	}
	length = ист.length;
	alloc = ист.alloc;
	if(ист.IsRc()) {
		укз = ист.укз;
		атомнИнк(Rc());
	}
	else {
		укз = (шим *)разместиПам((МАЛЫЙ + 1) * sizeof(шим));
		memcpy(укз, ист.укз, sizeof(шим) * (МАЛЫЙ + 1));
	}
	Dsyn();
}

проц ШТкст0::кат(const шим *s, цел l)
{
	if(length + l >= alloc || совместный())
		вставь(length, l, s);
	else {
		т_копирпам(укз + length, s, l);
		укз[length += l] = 0;
	}
	Dsyn();
}

проц ШТкст0::уст(const шим *s, цел length)
{
	освободи();
	уст0(s, length);
}

проц ШТкст0::бКат(цел c)
{
	*вставь(length, 1, NULL) = c;
}

проц ШТкст0::уст0(const шим *s, цел l)
{
	alloc = length = l;
	т_копирпам(укз = размести(alloc), s, l);
	укз[l] = 0;
	Dsyn();
}

проц ШТкст0::разшарь()
{
	if(!совместный()) return;
	цел al = length;
	шим *p = размести(al);
	т_копирпам(p, укз, length + 1);
	освободи();
	укз = p;
	alloc = al;
}

проц ШТкст0::уст(цел pos, цел ch)
{
	ПРОВЕРЬ(pos >= 0 && pos < дайСчёт());
	разшарь();
	укз[pos] = ch;
}

проц ШТкст0::обрежь(цел pos)
{
	ПРОВЕРЬ(pos >= 0 && pos <= дайСчёт());
	разшарь();
	length = pos;
	укз[pos] = 0;
	Dsyn();
}

проц ШТкст0::удали(цел pos, цел count)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos + count <= дайСчёт());
	разшарь();
	memmove(укз + pos, укз + pos + count, (дайСчёт() - pos - count + 1) * sizeof(шим));
	length -= count;
	Dsyn();
}

проц ШТкст0::вставь(цел pos, const шим *s, цел count)
{
	вставь(pos, count, s);
	Dsyn();
}

цел ШТкст0::сравни(const ШТкст0& s) const
{
	const шим *a = старт();
	const шим *ae = a + дайДлину();
	const шим *b = s.старт();
	const шим *be = b + s.дайДлину();
	for(;;) {
		if(a >= ae)
			return b >= be ? 0 : -1;
		if(b >= be)
			return 1;
		цел q = сравнзн__(*a++) - сравнзн__(*b++);
		if(q)
			return q;
	}
}

ШТкст& ШТкст::operator=(const шим *s)
{
	цел  len = дайСчёт();
	шим *str = (шим *)старт();
	if(s >= str && s <= str + len)
		return *this = ШТкст(s, длинтекс__(s));
	ШТкст0::освободи();
	ШТкст0::уст0(s, длинтекс__(s));
	return *this;
}

ШТкст::ШТкст(ШТкстБуф& b)
{
	length = b.дайДлину();
	укз = b.pbegin;
	укз[length] = 0;
	alloc = b.дайРазмест();
	if(дайРазмест() > 4 * дайДлину() / 3)
		сожми();
	b.обнули();
	Dsyn();
}

ШТкст::ШТкст(кткст0 s) {
	обнули();
	*this = вЮникод(s, s ? (цел)strlen(s) : 0, ДЕФНАБСИМ);
}

ШТкст::ШТкст(const сим16 *s) {
	обнули();
	*this = вУтф32(s);
}

ШТкст::ШТкст(кткст0 s, цел n) {
	обнули();
	*this = вЮникод(s, n, ДЕФНАБСИМ);
}

ШТкст::ШТкст(кткст0 s, кткст0 lim) {
	обнули();
	*this = вЮникод(s, s ? (цел)(lim - s) : 0, ДЕФНАБСИМ);
}
/*
Ткст ШТкст::вТкст() const
{
	return изЮникода(*this, ДЕФНАБСИМ);
}
*/
Атомар ШТкст0::voidptr[2];

ШТкст ШТкст::дайПроц()
{
	ШТкст b;
	b.alloc = -1;
	return b;
}

#ifndef _HAVE_NO_STDWSTRING
static_assert(sizeof(wchar_t) == 4 || sizeof(wchar_t) == 2, "Invalid wchar_t size");

ШТкст::ШТкст(const std::wstring& s)
{
	ШТкст0::обнули();
	if(sizeof(wchar_t) == 4)
		*this = ШТкст((const шим *)s.c_str(), (цел)s.size());
	if(sizeof(wchar_t) == 2)
		*this = вУтф32((сим16 *)s.c_str(), (цел)s.size());
}

std::wstring ШТкст::вСтд() const
{
	if(sizeof(wchar_t) == 4)
		return std::wstring((const wchar_t *)begin(), дайСчёт());
	if(sizeof(wchar_t) == 2) {
		Вектор<сим16> h = вУтф16(*this);
		return std::wstring((const wchar_t *)h.begin(), h.дайСчёт());
	}
}
#endif

проц ШТкстБуф::обнули()
{
	static шим h[2];
	pbegin = pend = limit = h;
}

шим *ШТкстБуф::размести(цел count, цел& alloc)
{
	if(count <= 23) {
		шим *s = (шим *)разместиПам(24 * sizeof(шим));
		alloc = ШТкст0::МАЛЫЙ;
		return s;
	}
	else {
		т_мера sz = sizeof(Атомар) + ((т_мера)count + 1) * sizeof(шим);
		Атомар *rc = (Атомар *)разместиПам(sz);
		alloc = (цел)мин((т_мера)INT_MAX, ((sz - sizeof(Атомар)) / sizeof(шим)) - 1);
		ПРОВЕРЬ(alloc >= 0);
		*rc = 1;
		return (шим *)(rc + 1);
	}
}

проц ШТкстБуф::освободи()
{
	цел all = (цел)(limit - pbegin);
	if(all == ШТкст0::МАЛЫЙ)
		освободиПам(pbegin);
	if(all > ШТкст0::МАЛЫЙ)
		освободиПам((Атомар *)pbegin - 1);
}

проц ШТкстБуф::расширь(бцел n, const шим *cat, цел l)
{
	цел al;
	т_мера ep = pend - pbegin;
	if(n > INT_MAX)
		n = INT_MAX;
	шим *p = размести(n, al);
	т_копирпам(p, pbegin, дайДлину());
	if(cat) {
		if(ep + l > INT_MAX)
			паника("ШТкстБуф is too big!");
		т_копирпам(p + ep, cat, l);
		ep += l;
	}
	освободи();
	pbegin = p;
	pend = pbegin + ep;
	limit = pbegin + al;
}

проц ШТкстБуф::расширь()
{
	расширь(дайДлину() * 2);
	if(pend == limit)
		паника("ШТкстБуф is too big!");
}

проц ШТкстБуф::устДлину(цел l)
{
	if(l > (limit - pbegin))
		расширь(l);
	pend = pbegin + l;
}

проц ШТкстБуф::кат(const шим *s, цел l)
{
	if(pend + l > limit)
		расширь(макс(дайДлину(), l) + дайДлину(), s, l);
	else {
		т_копирпам(pend, s, l);
		pend += l;
	}
}

проц ШТкстБуф::кат(цел c, цел l)
{
	if(pend + l > limit)
		расширь(макс(дайДлину(), l) + дайДлину(), NULL, l);
	устпам32(pend, c, l);
	pend += l;
}

проц ШТкстБуф::уст(ШТкст& s)
{
	s.разшарь();
	цел l = s.дайДлину();
	pbegin = s.укз;
	pend = pbegin + l;
	limit = pbegin + s.дайРазмест();
	s.обнули();
}

ШТкст обрежьЛево(const ШТкст& str)
{
	const шим *s = str;
	if(!IsSpace(*s))
		return s;
	while(IsSpace(*s)) s++;
	return ШТкст(s, str.стоп());
}

ШТкст обрежьПраво(const ШТкст& str)
{
	if(str.пустой())
		return str;
	const шим *s = str.последний();
	if(!IsSpace(*s))
		return str;
	while(s >= ~str && IsSpace(*s)) s--;
	return ШТкст(~str, s + 1);
}

struct WStringICompare__
{
	цел operator()(шим a, шим b) const { return взаг(a) - взаг(b); }
};

цел сравнилюб(const ШТкст& a, const ШТкст& b)
{
#ifdef DEPRECATED
	return IterCompare(a.старт(), a.стоп(), b.старт(), b.стоп(), WStringICompare__());
#else
	return сравниДиапазоны(a, b, WStringICompare__());
#endif
}

цел сравнилюб(const ШТкст& a, const шим *b)
{
#ifdef DEPRECATED
	return IterCompare(a.старт(), a.стоп(), b, b + длинтекс__(b), WStringICompare__());
#else
	return сравниДиапазоны(a, СубДиапазон(b, b + длинтекс__(b)), WStringICompare__());
#endif
}

