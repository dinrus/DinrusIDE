#ifndef _DinrusPro_Tpl_h_
#define _DinrusPro_Tpl_h_
#include <DinrusPro/Funcs.h>
///////////////////////
namespace ДинрусРНЦП  {
///////////////////////
т_хэш дайХэшЗначУкз(const ук a) ;
ук TinyAlloc(цел size);
ук разместиПам(т_мера size);
///////////////////////////////
template <class T>
inline цел зн(T a) { return a > 0 ? 1 : a < 0 ? -1 : 0; }

template <class T>
inline T табы(T a) { return (a >= 0 ? a : -a); }

template <class T> inline T абс(T a) { return (a >= 0 ? a : -a); }

template <class T>
inline цел сравни(const T& a, const T& b) { return a > b ? 1 : a < b ? -1 : 0; }

#ifdef CPU_SIMD
template <class T>
проц т_копирпам(ук t, const T *s, т_мера счёт)
{
#ifdef CPU_X86
	if((sizeof(T) & 15) == 0)
		копирпам128(t, s, счёт * (sizeof(T) >> 4));
	else
	if((sizeof(T) & 7) == 0)
		копирпам64(t, s, счёт * (sizeof(T) >> 3));
	else
#endif
	if((sizeof(T) & 3) == 0)
		копирпам32(t, s, счёт * (sizeof(T) >> 2));
	else
	if((sizeof(T) & 1) == 0)
		копирпам16(t, s, счёт * (sizeof(T) >> 1));
	else
		копирпам8(t, s, счёт * sizeof(T));
}
#else
template <class T>
проц т_копирпам(ук t, const T *s, т_мера count)
{
	копирпам8(t, s, count * sizeof(T));
}
#endif

template <class T>
бул т_равнпам(const T *p, const T *q, т_мера счёт)
{
	if((sizeof(T) & 15) == 0)
		return равнпам128(p, q, счёт * (sizeof(T) >> 4));
	else
	if((sizeof(T) & 7) == 0)
		return равнпам64(p, q, счёт * (sizeof(T) >> 3));
	else
	if((sizeof(T) & 3) == 0)
		return равнпам32(p, q, счёт * (sizeof(T) >> 2));
	else
	if((sizeof(T) & 1) == 0)
		return равнпам16(p, q, счёт * (sizeof(T) >> 1));
	else
		return равнпам8(p, q, счёт * sizeof(T));
}

template <class T>
цел сравниЧисла__(const T& a, const T& b)
{
	if(a < b) return -1;
	if(a > b) return 1;
	return 0;
}

template <class T>
inline т_хэш дайХэшЗнач(T *укз)                             { return РНЦП::дайХэшЗначУкз(reinterpret_cast<const ук>(укз)); }

template <class T>
inline т_хэш дайХэшЗнач(const T& x)                              { return x.дайХэшЗнач(); }

template <class T>
inline проц заполни(T *dst, const T *lim, const T& x) {
	while(dst < lim)
		*dst++ = x;
}

template <class T>
inline проц копируй(T *dst, const T *ист, const T *lim) {
	while(ист < lim)
		*dst++ = *ист++;
}

template <class T>
inline проц копируй(T *dst, const T *ист, цел n) {
	копируй(dst, ист, ист + n);
}

template <class I>
inline проц IterSwap(I a, I b) { разверни(*a, *b); }

template<class T>
inline цел сравниЗнак(const T& a, const T& b)
{
	return a.сравни(b);
}

template <class C>
бул равныМапы(const C& a, const C& b)
{
	if(a.дайСчёт() != b.дайСчёт())
		return false;
	for(цел i = 0; i < a.дайСчёт(); i++)
		if(a.дайКлюч(i) != b.дайКлюч(i) || a[i] != b[i])
			return false;
	return true;
}

template <class C>
цел сравниМап(const C& a, const C& b)
{
	цел n = мин(a.дайСчёт(), b.дайСчёт());
	for(цел i = 0; i < n; i++) {
		цел q = сравниЗнак(a.дайКлюч(i), b.дайКлюч(i));
		if(q)
			return q;
		q = сравниЗнак(a[i], b[i]);
		if(q)
			return q;
	}
	return сравниЗнак(a.дайСчёт(), b.дайСчёт());
}

template <class T>
constexpr const T& мин(const T& a, const T& b)
{
	return a < b ? a : b;
}

template <class T, typename... Арги>
constexpr const T& мин(const T& a, const T& b, const Арги& ...арги)
{
	return мин(a, мин(b, арги...));
}

template <class T>
constexpr const T& макс(const T& a, const T& b)
{
	return a > b ? a : b;
}

template <class T, typename... Арги>
constexpr const T& макс(const T& a, const T& b, const Арги& ...арги)
{
	return макс(a, макс(b, арги...));
}

//$+

template <class T>
constexpr T clamp(const T& x, const T& min_, const T& max_)
{
	return мин(макс(x, min_), max_);
}

//$-constexpr цел найдиарг(const T& x, const T1& p0, ...);

template <class T, class K>
constexpr цел найдиарг(const T& x, const K& k)
{
	return x == k ? 0 : -1;
}

template <class T, class K, typename... L>
constexpr цел найдиарг(const T& sel, const K& k, const L& ...арги)
{
	if(sel == k)
		return 0;
	цел q = найдиарг(sel, арги...);
	return q >= 0 ? q + 1 : -1;
}

//$-constexpr auto раскодируй(const T& x, const T1& p0, const V1& v0, ...);

template <class T>
constexpr кткст0 decode_chr_(const T& sel, кткст0 опр)
{
	return опр;
}

template <class T, class K, typename... L>
constexpr кткст0 decode_chr_(const T& sel, const K& k, кткст0 v, const L& ...арги)
{
	return sel == k ? v : decode_chr_(sel, арги...);
}

template <class T, class K, typename... L>
constexpr кткст0 раскодируй(const T& sel, const K& k, кткст0 v, const L& ...арги)
{
	return decode_chr_(sel, k, v, арги...);
}

template <class T, class V>
constexpr const V& раскодируй(const T& sel, const V& опр)
{
	return опр;
}

template <class T, class K, class V, typename... L>
constexpr V раскодируй(const T& sel, const K& k, const V& v, const L& ...арги)
{
	return sel == k ? v : (V)раскодируй(sel, арги...);
}

//$-constexpr T get_i(цел i, const T& p0, const T1& p1, ...);

template <typename A, typename... T>
constexpr A get_i(цел i, const A& p0, const T& ...арги)
{
	A list[] = { p0, (A)арги... };
	return list[clamp(i, 0, (цел)sizeof...(арги))];
}

template <typename P, typename... T>
constexpr const P *get_i(цел i, const P* p0, const T& ...арги)
{
	const P *list[] = { p0, арги... };
	return list[clamp(i, 0, (цел)sizeof...(арги))];
}

//$-проц foreach_arg(F фн, const T& p0, const T1& p1, ...);

template <class F, class V>
проц foreach_arg(F фн, V&& v)
{
	фн(std::forward<V>(v));
}

template <class F, class V, typename... Арги>
проц foreach_arg(F фн, V&& v, Арги&& ...арги)
{
	фн(std::forward<V>(v));
	foreach_arg(фн, std::forward<Арги>(арги)...);
}

//$+

template <class I, typename... Арги>
проц iter_set(I t, Арги&& ...арги)
{
	foreach_arg([&](auto&& v) { *t++ = std::forward<decltype(v)>(v); }, std::forward<Арги>(арги)...);
}

template <class I, typename... Арги>
проц iter_get(I s, Арги& ...арги)
{
	foreach_arg([&](auto& v) { v = *s++; }, арги...);
}

template <class C, typename... Арги>
C gather(Арги&& ...арги)
{
	C x(sizeof...(арги));
	iter_set(x.begin(), std::forward<Арги>(арги)...);
	return x;
}

template <class C, typename... Арги>
цел scatter_n(цел n, const C& c, Арги& ...арги)
{
	if(n < (цел)sizeof...(арги))
		return 0;
	iter_get(c.begin(), арги...);
	return sizeof...(арги);
}

template <class C, typename... Арги>
цел scatter(const C& c, Арги& ...арги)
{
	return scatter_n(c.size(), c, арги...);
}

// DEPRECATED

template <class T> // deprecated имя, use clamp
inline T минмакс(T x, T _min, T _max)                           { return мин(макс(x, _min), _max); }


template <class T, class... Арги>
T *tiny_new(Арги... арги)
{
	return new(TinyAlloc(sizeof(T))) T(арги...);
}

template <class T>
проц tiny_delete(T *укз)
{
	укз->~T();
	TinyFree(sizeof(T), укз);
}

template <class T>
inline бцел номерТипаЗнач(const T *)                 { return StaticTypeNo<T>() + 0x8000000; }

template <class T> проц устПусто(T& x) { x = Null; }

template <class T> бул пусто_ли(const T& x)       { return x.экзПусто_ли(); }

template <class T>
inline проц разверни(T& a, T& b) { T tmp = пикуй(a); a = пикуй(b); b = пикуй(tmp); }

///
#ifdef _ОТЛАДКА
проц всёКогдаПодобрано(ук укз);
проц ВСЁ_КОГДА_ПОДОБРАНО__(ук укз);

template <class T>
проц ВСЁ_КОГДА_ПОДОБРАНО(T& x)
{
	ВСЁ_КОГДА_ПОДОБРАНО__(&x);
}
#endif

template <class T>
inline проц стройЗаполниГлубКопию(T *t, const T *end, const T& x) {
	while(t != end)
		new(t++) T(клонируй(x));
}

template <class T>
inline проц строй(T *t, const T *lim) {
	while(t < lim)
		new(t++) T;
}

template <class T>
inline проц разрушь(T *t, const T *end)
{
	while(t != end) {
		t->~T();
		t++;
	}
}

template <class T, class S>
inline проц стройГлубКопию(T *t, const S *s, const S *end) {
	while(s != end)
		new (t++) T(клонируй(*s++));
}

//////////////////////////////////////////////
template <class B>
class АТкст : public B {
	typedef typename B::tchar  tchar;
	typedef typename B::bchar  bchar;
	typedef typename B::Буфер буфер;
	typedef typename B::Ткст  Ткст;

public:
	проц очисть()                                              { B::освободи(); B::обнули(); }
	цел  дайДлину() const                                    { return B::дайСчёт(); }
	бул пустой() const                                      { return B::дайСчёт() == 0; }

	const tchar *стоп() const                                  { return B::старт() + дайДлину(); }
	const tchar *end() const                                  { return стоп(); }
	const tchar *последний() const                                 { return стоп() - !!B::дайСчёт(); }
	const tchar *дайОбх(цел i) const                         { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт() + i; }

	цел operator*() const                                     { return *B::старт(); }
	цел operator[](цел i) const                               { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт()[i]; }

	operator const tchar *() const                            { return B::старт(); }
	const tchar *operator~() const                            { return B::старт(); }
	operator const bchar *() const                            { return (bchar *)B::старт(); }
	operator const проц *() const                             { return B::старт(); }

	проц вставь(цел pos, цел c)                               { *B::вставь(pos, 1, NULL) = c; }
	проц вставь(цел pos, const tchar *s, цел count)           { B::вставь(pos, count, s); }
	проц вставь(цел pos, const Ткст& s)                     { вставь(pos, s, s.дайСчёт()); }
	проц вставь(цел pos, кткст0 s);

	проц обрежьПоследн(цел count = 1)                              { B::обрежь(B::дайСчёт() - count); }

	проц  кат(цел c)                                          { B::кат(c); }
	проц  кат(const tchar *s, цел len)                        { B::кат(s, len); }
	проц  кат(const tchar *s);
	проц  кат(const Ткст& s)                                { кат(~s, s.дайДлину()); }
	проц  кат(цел c, цел count);
	проц  кат(const tchar *s, const tchar *lim)               { ПРОВЕРЬ(s <= lim); кат(s, цел(lim - s)); }
	проц  кат(const Ткст& s, цел len)                       { B::кат(~s, len); }
	проц  кат(const bchar *s, цел len)                        { кат((const tchar *) s, len); }

	Ткст& кат()                                             { return *(Ткст *)this; }

	цел    сравни(const Ткст& s) const                     { return B::сравни(s); }
	цел    сравни(const tchar *s) const;

	бул   равен(const Ткст& s) const                     { return B::равен(s); }
	бул   равен(const tchar *s) const                      { return B::равен(s); }

	Ткст середина(цел pos, цел length) const;
	Ткст середина(цел pos) const                                 { return середина(pos, дайДлину() - pos); }
	Ткст право(цел count) const                             { return середина(дайДлину() - count); }
	Ткст лево(цел count) const                              { return середина(0, count); }

	цел    найди(цел chr, цел from = 0) const;
	цел    найдирек(цел chr, цел from) const;
	цел    найдирек(цел chr) const;

	цел    найди(цел len, const tchar *s, цел from) const      { return найди(B::старт(), B::дайСчёт(), s, len, from); }
	цел    найди(const tchar *s, цел from = 0) const           { return найди(длинтекс__(s), s, from); }
	цел    найди(const Ткст& s, цел from = 0) const          { return найди(s.дайСчёт(), ~s, from); }

	цел    найдиПосле(const tchar *s, цел from = 0) const      { цел n = длинтекс__(s); цел q = найди(n, s, from); return q < 0 ? -1 : q + n; }
	цел    найдиПосле(const Ткст& s, цел from = 0) const     { цел n = s.дайСчёт(); цел q = найди(n, ~s, from); return q < 0 ? -1 : q + n; }

	цел    найдирек(цел len, const tchar *s, цел from) const;
	цел    найдирек(const tchar *s, цел from) const;
	цел    найдирек(const Ткст& s, цел from) const       { return найдирек(s.дайСчёт(), ~s, from); }
	цел    найдирек(const tchar *s) const                  { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}
	цел    найдирек(const Ткст& s) const                 { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}

	цел    найдирекПосле(цел len, const tchar *s, цел from) const;
	цел    найдирекПосле(const tchar *s, цел from) const;
	цел    найдирекПосле(const Ткст& s, цел from) const  { return найдирекПосле(s.дайСчёт(), ~s, from); }
	цел    найдирекПосле(const tchar *s) const             { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}
	цел    найдирекПосле(const Ткст& s) const            { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}

	проц   замени(const tchar *найди, цел findlen, const tchar *replace, цел replacelen);
	проц   замени(const Ткст& найди, const Ткст& replace);
	проц   замени(const tchar *найди, const tchar *replace);
	проц   замени(const Ткст& найди, const tchar *replace);
	проц   замени(const tchar *найди, const Ткст& replace);

	бул   начинаетсяС(const tchar *s, цел len) const;
	бул   начинаетсяС(const tchar *s) const;
	бул   начинаетсяС(const Ткст& s) const                  { return начинаетсяС(~s, s.дайДлину()); }

	бул   обрежьСтарт(const tchar *s, цел len)                 { if(!начинаетсяС(s, len)) return false; B::удали(0, len); return true; }
	бул   обрежьСтарт(const tchar *s)                          { return обрежьСтарт(s, длинтекс__(s)); }
	бул   обрежьСтарт(const Ткст& s)                         { return обрежьСтарт(~s, s.дайДлину()); }

	бул   заканчиваетсяНа(const tchar *s, цел len) const;
	бул   заканчиваетсяНа(const tchar *s) const;
	бул   заканчиваетсяНа(const Ткст& s) const                    { return заканчиваетсяНа(~s, s.дайДлину()); }

	бул   обрежьКонец(const tchar *s, цел len)                   { if(!заканчиваетсяНа(s, len)) return false; обрежьПоследн(len); return true; }
	бул   обрежьКонец(const tchar *s)                            { return обрежьКонец(s, длинтекс__(s)); }
	бул   обрежьКонец(const Ткст& s)                           { return обрежьКонец(~s, s.дайДлину()); }

	цел    найдиПервыйИз(цел len, const tchar *set, цел from = 0) const;
	цел    найдиПервыйИз(const tchar *set, цел from = 0) const  { return найдиПервыйИз(длинтекс__(set), set, from); }
	цел    найдиПервыйИз(const Ткст& set, цел from = 0) const { return найдиПервыйИз(set.дайСчёт(), ~set, from); }

	friend бул operator<(const Ткст& a, const Ткст& b)   { return a.сравни(b) < 0; }
	friend бул operator<(const Ткст& a, const tchar *b)    { return a.сравни(b) < 0; }
	friend бул operator<(const tchar *a, const Ткст& b)    { return b.сравни(a) > 0; }

	friend бул operator<=(const Ткст& a, const Ткст& b)  { return a.сравни(b) <= 0; }
	friend бул operator<=(const Ткст& a, const tchar *b)   { return a.сравни(b) <= 0; }
	friend бул operator<=(const tchar *a, const Ткст& b)   { return b.сравни(a) >= 0; }

	friend бул operator>(const Ткст& a, const Ткст& b)   { return a.сравни(b) > 0; }
	friend бул operator>(const Ткст& a, const tchar *b)    { return a.сравни(b) > 0; }
	friend бул operator>(const tchar *a, const Ткст& b)    { return b.сравни(a) < 0; }

	friend бул operator>=(const Ткст& a, const Ткст& b)   { return a.сравни(b) >= 0; }
	friend бул operator>=(const Ткст& a, const tchar *b)    { return a.сравни(b) >= 0; }
	friend бул operator>=(const tchar *a, const Ткст& b)    { return b.сравни(a) <= 0; }

	friend бул operator==(const Ткст& a, const Ткст& b)   { return a.равен(b); }
	friend бул operator!=(const Ткст& a, const Ткст& b)   { return !a.равен(b); }
	friend бул operator==(const Ткст& a, const tchar *b)    { return a.равен(b); }
	friend бул operator==(const tchar *a, const Ткст& b)    { return b.равен(a); }
	friend бул operator!=(const Ткст& a, const tchar *b)    { return !a.равен(b); }
	friend бул operator!=(const tchar *a, const Ткст& b)    { return !b.равен(a); }

	friend Ткст operator+(const Ткст& a, const Ткст& b)  { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, const tchar *b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, tchar b)          { Ткст c(a); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const Ткст& b)       { Ткст c(пикуй(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const tchar *b)        { Ткст c(пикуй(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, tchar b)               { Ткст c(пикуй(a)); c += b; return c; }
	friend Ткст operator+(const tchar *a, const Ткст& b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(tchar a, const Ткст& b)          { Ткст c(a, 1); c += b; return c; }

};
////////////////////////////////////////////////////////////////////

///////////
}// ns end
////////////////
#endif
