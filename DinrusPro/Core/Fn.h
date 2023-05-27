#ifdef мин
#undef мин
#endif

#ifdef макс
#undef макс
#endif

//$-template <class T> inline constexpr const T& мин(const T& a, const T& b, ...);
//$-template <class T> inline constexpr const T& макс(const T& a, const T& b, ...);

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
