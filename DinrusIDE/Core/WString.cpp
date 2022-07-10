#include "Core.h"

namespace РНЦПДинрус {

wchar *WString0::размести(int& count)
{
	if(count <= SMALL) {
		count = SMALL;
		wchar *p = (wchar *)MemoryAlloc((SMALL + 1) * sizeof(wchar));
		return p;
	}
	size_t sz = sizeof(Атомар) + ((size_t)count + 1) * sizeof(wchar);
	Атомар *rc = (Атомар *)MemoryAllocSz(sz);
	if(count != INT_MAX)
		count = int(((sz - sizeof(Атомар)) / sizeof(wchar)) - 1);
	*rc = 1;
	return (wchar *)(rc + 1);
}

void WString0::освободи()
{
	if(alloc > 0) {
		if(IsRc()) {
			Атомар& rc = Rc();
			if(атомнДек(rc) == 0)
				MemoryFree(&rc);
		}
		else
			MemoryFree(ptr);
	}
}

#ifdef _ОТЛАДКА
void WString0::Dsyn()
{
	ШТкст *d_str = static_cast<ШТкст *>(this);
	d_str->s = старт();
	d_str->len = дайСчёт();
}
#endif

wchar *WString0::вставь(int pos, int count, const wchar *s)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos <= дайСчёт());
	int newlen = length + count;
	if(newlen < length)
		паника("ШТкст is too big!");
	if(newlen < alloc && !IsShared() && (!s || s < ptr || s > ptr + length)) {
		if(pos < length)
			memmove(ptr + pos + count, ptr + pos, (length - pos) * sizeof(wchar));
		length = newlen;
		ptr[newlen] = 0;
		if(s)
			memcpy_t(ptr + pos, s, count);
		Dsyn();
		return ptr + pos;
	}
	int all = max(length >= int((int64)2 * INT_MAX / 3) ? INT_MAX : length + (length >> 1), newlen);
	wchar *p = размести(all);
	if(pos > 0)
		memcpy_t(p, ptr, pos);
	if(pos < length)
		memcpy_t(p + pos + count, ptr + pos, length - pos);
	if(s)
		memcpy_t(p + pos, s, count);
	ПРОВЕРЬ(pos + count <= all);
	p[newlen] = 0;
	освободи();
	ptr = p;
	length = newlen;
	alloc = all;
	Dsyn();
	return ptr + pos;
}

void WString0::уст0(const WString0& ист)
{
	if(ист.alloc <= 0) {
		static wchar h[2];
		ptr = h;
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
		ptr = ист.ptr;
		атомнИнк(Rc());
	}
	else {
		ptr = (wchar *)MemoryAlloc((SMALL + 1) * sizeof(wchar));
		memcpy(ptr, ист.ptr, sizeof(wchar) * (SMALL + 1));
	}
	Dsyn();
}

void WString0::конкат(const wchar *s, int l)
{
	if(length + l >= alloc || IsShared())
		вставь(length, l, s);
	else {
		memcpy_t(ptr + length, s, l);
		ptr[length += l] = 0;
	}
	Dsyn();
}

void WString0::уст(const wchar *s, int length)
{
	освободи();
	уст0(s, length);
}

void WString0::LCat(int c)
{
	*вставь(length, 1, NULL) = c;
}

void WString0::уст0(const wchar *s, int l)
{
	alloc = length = l;
	memcpy_t(ptr = размести(alloc), s, l);
	ptr[l] = 0;
	Dsyn();
}

void WString0::UnShare()
{
	if(!IsShared()) return;
	int al = length;
	wchar *p = размести(al);
	memcpy_t(p, ptr, length + 1);
	освободи();
	ptr = p;
	alloc = al;
}

void WString0::уст(int pos, int ch)
{
	ПРОВЕРЬ(pos >= 0 && pos < дайСчёт());
	UnShare();
	ptr[pos] = ch;
}

void WString0::обрежь(int pos)
{
	ПРОВЕРЬ(pos >= 0 && pos <= дайСчёт());
	UnShare();
	length = pos;
	ptr[pos] = 0;
	Dsyn();
}

void WString0::удали(int pos, int count)
{
	ПРОВЕРЬ(pos >= 0 && count >= 0 && pos + count <= дайСчёт());
	UnShare();
	memmove(ptr + pos, ptr + pos + count, (дайСчёт() - pos - count + 1) * sizeof(wchar));
	length -= count;
	Dsyn();
}

void WString0::вставь(int pos, const wchar *s, int count)
{
	вставь(pos, count, s);
	Dsyn();
}

int WString0::сравни(const WString0& s) const
{
	const wchar *a = старт();
	const wchar *ae = a + дайДлину();
	const wchar *b = s.старт();
	const wchar *be = b + s.дайДлину();
	for(;;) {
		if(a >= ae)
			return b >= be ? 0 : -1;
		if(b >= be)
			return 1;
		int q = cmpval__(*a++) - cmpval__(*b++);
		if(q)
			return q;
	}
}

ШТкст& ШТкст::operator=(const wchar *s)
{
	int  len = дайСчёт();
	wchar *str = (wchar *)старт();
	if(s >= str && s <= str + len)
		return *this = ШТкст(s, strlen__(s));
	WString0::освободи();
	WString0::уст0(s, strlen__(s));
	return *this;
}

ШТкст::ШТкст(ШТкстБуф& b)
{
	length = b.дайДлину();
	ptr = b.pbegin;
	ptr[length] = 0;
	alloc = b.дайРазмест();
	if(дайРазмест() > 4 * дайДлину() / 3)
		сожми();
	b.обнули();
	Dsyn();
}

ШТкст::ШТкст(const char *s) {
	обнули();
	*this = вЮникод(s, s ? (int)strlen(s) : 0, CHARSET_DEFAULT);
}

ШТкст::ШТкст(const char16 *s) {
	обнули();
	*this = вУтф32(s);
}

ШТкст::ШТкст(const char *s, int n) {
	обнули();
	*this = вЮникод(s, n, CHARSET_DEFAULT);
}

ШТкст::ШТкст(const char *s, const char *lim) {
	обнули();
	*this = вЮникод(s, s ? (int)(lim - s) : 0, CHARSET_DEFAULT);
}

Ткст ШТкст::вТкст() const
{
	return изЮникода(*this, CHARSET_DEFAULT);
}

Атомар WString0::voidptr[2];

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
	WString0::обнули();
	if(sizeof(wchar_t) == 4)
		*this = ШТкст((const wchar *)s.c_str(), (int)s.size());
	if(sizeof(wchar_t) == 2)
		*this = вУтф32((char16 *)s.c_str(), (int)s.size());
}

std::wstring ШТкст::вСтд() const
{
	if(sizeof(wchar_t) == 4)
		return std::wstring((const wchar_t *)begin(), дайСчёт());
	if(sizeof(wchar_t) == 2) {
		Вектор<char16> h = вУтф16(*this);
		return std::wstring((const wchar_t *)h.begin(), h.дайСчёт());
	}
}
#endif

void ШТкстБуф::обнули()
{
	static wchar h[2];
	pbegin = pend = limit = h;
}

wchar *ШТкстБуф::размести(int count, int& alloc)
{
	if(count <= 23) {
		wchar *s = (wchar *)MemoryAlloc(24 * sizeof(wchar));
		alloc = WString0::SMALL;
		return s;
	}
	else {
		size_t sz = sizeof(Атомар) + ((size_t)count + 1) * sizeof(wchar);
		Атомар *rc = (Атомар *)MemoryAlloc(sz);
		alloc = (int)min((size_t)INT_MAX, ((sz - sizeof(Атомар)) / sizeof(wchar)) - 1);
		ПРОВЕРЬ(alloc >= 0);
		*rc = 1;
		return (wchar *)(rc + 1);
	}
}

void ШТкстБуф::освободи()
{
	int all = (int)(limit - pbegin);
	if(all == WString0::SMALL)
		MemoryFree(pbegin);
	if(all > WString0::SMALL)
		MemoryFree((Атомар *)pbegin - 1);
}

void ШТкстБуф::расширь(dword n, const wchar *cat, int l)
{
	int al;
	size_t ep = pend - pbegin;
	if(n > INT_MAX)
		n = INT_MAX;
	wchar *p = размести(n, al);
	memcpy_t(p, pbegin, дайДлину());
	if(cat) {
		if(ep + l > INT_MAX)
			паника("ШТкстБуф is too big!");
		memcpy_t(p + ep, cat, l);
		ep += l;
	}
	освободи();
	pbegin = p;
	pend = pbegin + ep;
	limit = pbegin + al;
}

void ШТкстБуф::расширь()
{
	расширь(дайДлину() * 2);
	if(pend == limit)
		паника("ШТкстБуф is too big!");
}

void ШТкстБуф::устДлину(int l)
{
	if(l > (limit - pbegin))
		расширь(l);
	pend = pbegin + l;
}

void ШТкстБуф::конкат(const wchar *s, int l)
{
	if(pend + l > limit)
		расширь(max(дайДлину(), l) + дайДлину(), s, l);
	else {
		memcpy_t(pend, s, l);
		pend += l;
	}
}

void ШТкстБуф::конкат(int c, int l)
{
	if(pend + l > limit)
		расширь(max(дайДлину(), l) + дайДлину(), NULL, l);
	memset32(pend, c, l);
	pend += l;
}

void ШТкстБуф::уст(ШТкст& s)
{
	s.UnShare();
	int l = s.дайДлину();
	pbegin = s.ptr;
	pend = pbegin + l;
	limit = pbegin + s.дайРазмест();
	s.обнули();
}

ШТкст обрежьЛево(const ШТкст& str)
{
	const wchar *s = str;
	if(!IsSpace(*s))
		return s;
	while(IsSpace(*s)) s++;
	return ШТкст(s, str.стоп());
}

ШТкст обрежьПраво(const ШТкст& str)
{
	if(str.пустой())
		return str;
	const wchar *s = str.последний();
	if(!IsSpace(*s))
		return str;
	while(s >= ~str && IsSpace(*s)) s--;
	return ШТкст(~str, s + 1);
}

struct WStringICompare__
{
	int operator()(wchar a, wchar b) const { return взаг(a) - взаг(b); }
};

int CompareNoCase(const ШТкст& a, const ШТкст& b)
{
#ifdef DEPRECATED
	return IterCompare(a.старт(), a.стоп(), b.старт(), b.стоп(), WStringICompare__());
#else
	return сравниДиапазоны(a, b, WStringICompare__());
#endif
}

int CompareNoCase(const ШТкст& a, const wchar *b)
{
#ifdef DEPRECATED
	return IterCompare(a.старт(), a.стоп(), b, b + strlen__(b), WStringICompare__());
#else
	return сравниДиапазоны(a, СубДиапазон(b, b + strlen__(b)), WStringICompare__());
#endif
}

}
