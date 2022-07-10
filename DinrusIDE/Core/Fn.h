#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

//$-template <class T> inline constexpr const T& min(const T& a, const T& b, ...);
//$-template <class T> inline constexpr const T& max(const T& a, const T& b, ...);

template <class T>
constexpr const T& min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template <class T, typename... Арги>
constexpr const T& min(const T& a, const T& b, const Арги& ...арги)
{
	return min(a, min(b, арги...));
}

template <class T>
constexpr const T& max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template <class T, typename... Арги>
constexpr const T& max(const T& a, const T& b, const Арги& ...арги)
{
	return max(a, max(b, арги...));
}

//$+

template <class T>
constexpr T clamp(const T& x, const T& min_, const T& max_)
{
	return min(max(x, min_), max_);
}

//$-constexpr int findarg(const T& x, const T1& p0, ...);

template <class T, class K>
constexpr int findarg(const T& x, const K& k)
{
	return x == k ? 0 : -1;
}

template <class T, class K, typename... L>
constexpr int findarg(const T& sel, const K& k, const L& ...арги)
{
	if(sel == k)
		return 0;
	int q = findarg(sel, арги...);
	return q >= 0 ? q + 1 : -1;
}

//$-constexpr auto decode(const T& x, const T1& p0, const V1& v0, ...);

template <class T>
constexpr const char *decode_chr_(const T& sel, const char *опр)
{
	return опр;
}

template <class T, class K, typename... L>
constexpr const char *decode_chr_(const T& sel, const K& k, const char *v, const L& ...арги)
{
	return sel == k ? v : decode_chr_(sel, арги...);
}

template <class T, class K, typename... L>
constexpr const char *decode(const T& sel, const K& k, const char *v, const L& ...арги)
{
	return decode_chr_(sel, k, v, арги...);
}

template <class T, class V>
constexpr const V& decode(const T& sel, const V& опр)
{
	return опр;
}

template <class T, class K, class V, typename... L>
constexpr V decode(const T& sel, const K& k, const V& v, const L& ...арги)
{
	return sel == k ? v : (V)decode(sel, арги...);
}

//$-constexpr T get_i(int i, const T& p0, const T1& p1, ...);

template <typename A, typename... T>
constexpr A get_i(int i, const A& p0, const T& ...арги)
{
	A list[] = { p0, (A)арги... };
	return list[clamp(i, 0, (int)sizeof...(арги))];
}

template <typename P, typename... T>
constexpr const P *get_i(int i, const P* p0, const T& ...арги)
{
	const P *list[] = { p0, арги... };
	return list[clamp(i, 0, (int)sizeof...(арги))];
}

//$-void foreach_arg(F фн, const T& p0, const T1& p1, ...);

template <class F, class V>
void foreach_arg(F фн, V&& v)
{
	фн(std::forward<V>(v));
}

template <class F, class V, typename... Арги>
void foreach_arg(F фн, V&& v, Арги&& ...арги)
{
	фн(std::forward<V>(v));
	foreach_arg(фн, std::forward<Арги>(арги)...);
}

//$+

template <class I, typename... Арги>
void iter_set(I t, Арги&& ...арги)
{
	foreach_arg([&](auto&& v) { *t++ = std::forward<decltype(v)>(v); }, std::forward<Арги>(арги)...);
}

template <class I, typename... Арги>
void iter_get(I s, Арги& ...арги)
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
int scatter_n(int n, const C& c, Арги& ...арги)
{
	if(n < (int)sizeof...(арги))
		return 0;
	iter_get(c.begin(), арги...);
	return sizeof...(арги);
}

template <class C, typename... Арги>
int scatter(const C& c, Арги& ...арги)
{
	return scatter_n(c.size(), c, арги...);
}

// DEPRECATED

template <class T> // deprecated имя, use clamp
inline T minmax(T x, T _min, T _max)                           { return min(max(x, _min), _max); }
