Вектор<Ткст> разбей(цел maxcount, кткст0 s, кткст0  (*text_фильтр)(кткст0 ), бул ignoreempty = true);
Вектор<Ткст> разбей(цел maxcount, кткст0 s, цел (*фильтр)(цел), бул ignoreempty = true);
Вектор<Ткст> разбей(цел maxcount, кткст0 s, цел chr, бул ignoreempty = true);
Вектор<Ткст> разбей(цел maxcount, кткст0 s, кткст0 text, бул ignoreempty = true);
Вектор<Ткст> разбей(кткст0 s, кткст0  (*text_фильтр)(кткст0 ), бул ignoreempty = true);
Вектор<Ткст> разбей(кткст0 s, цел (*фильтр)(цел), бул ignoreempty = true);
Вектор<Ткст> разбей(кткст0 s, цел chr, бул ignoreempty = true);
Вектор<Ткст> разбей(кткст0 s, кткст0 text, бул ignoreempty = true);

Вектор<ШТкст> разбей(цел maxcount, const шим *s, const шим * (*text_фильтр)(const шим *), бул ignoreempty = true);
Вектор<ШТкст> разбей(цел maxcount, const шим *s, цел (*фильтр)(цел), бул ignoreempty = true);
Вектор<ШТкст> разбей(цел maxcount, const шим *s, цел chr, бул ignoreempty = true);
Вектор<ШТкст> разбей(цел maxcount, const шим *s, const шим *text, бул ignoreempty = true);
Вектор<ШТкст> разбей(const шим *s, const шим * (*text_фильтр)(const шим *), бул ignoreempty = true);
Вектор<ШТкст> разбей(const шим *s, цел (*фильтр)(цел), бул ignoreempty = true);
Вектор<ШТкст> разбей(const шим *s, цел chr, бул ignoreempty = true);
Вектор<ШТкст> разбей(const шим *s, const шим *text, бул ignoreempty = true);

Ткст  соедини(const Вектор<Ткст>& im, const Ткст& delim, бул ignoreempty = false);
ШТкст соедини(const Вектор<ШТкст>& im, const ШТкст& delim, бул ignoreempty = false);

//$ бул   SplitTo(кткст0 s, цел delim, бул ignoreempty, Ткст& p1...);
//$ бул   SplitTo(кткст0 s, цел delim, Ткст& p1...);
//$ бул   SplitTo(кткст0 s, цел (*фильтр)(цел), Ткст& p1...);
//$ бул   SplitTo(кткст0 s, цел (*фильтр)(цел), Ткст& p1...);
//$ бул   SplitTo(кткст0 s, кткст0 delim, бул ignoreempty, Ткст& p1...);
//$ бул   SplitTo(кткст0 s, кткст0 delim, Ткст& p1...);

//$ бул   SplitTo(const шим *s, цел delim, бул ignoreempty, ШТкст& p1...);
//$ бул   SplitTo(const шим *s, цел delim, ШТкст& p1...);
//$ бул   SplitTo(const шим *s, цел (*фильтр)(цел), ШТкст& p1...);
//$ бул   SplitTo(const шим *s, цел (*фильтр)(цел), ШТкст& p1...);
//$ бул   SplitTo(const шим *s, const шим *delim, бул ignoreempty, ШТкст& p1...);
//$ бул   SplitTo(const шим *s, const шим *delim, ШТкст& p1...);

//$ Ткст Merge(кткст0 delim, Ткст& p1...);
//$ ШТкст Merge(const шим *delim, ШТкст& p1...);

//$ проц   MergeWith(Ткст& приёмник, кткст0 delim, Ткст& p1...);
//$ проц   MergeWith(ШТкст& приёмник, const шим *delim, ШТкст& p1...);

//$-
template <typename... Арги>
бул SplitTo(кткст0 s, цел delim, бул ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
бул SplitTo(кткст0 s, цел delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
бул SplitTo(кткст0 s, цел (*delim)(цел), бул ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
бул SplitTo(кткст0 s, цел (*delim)(цел), Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
бул SplitTo(кткст0 s, кткст0 delim, бул ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
бул SplitTo(кткст0 s, кткст0 delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
проц MergeWith(Ткст& приёмник, кткст0 delim, const Арги& ...арги)
{
	foreach_arg([&](const Ткст& арг) {
		if(арг.дайСчёт()) {
			if(приёмник.дайСчёт())
				приёмник << delim;
			приёмник << арг;
		}
	}, арги...);
}

template <typename... Арги>
Ткст Merge(кткст0 delim, const Арги& ...арги)
{
	Ткст r;
	MergeWith(r, delim, арги...);
	return r;
}

template <typename... Арги>
бул SplitTo(const шим *s, цел delim, бул ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
бул SplitTo(const шим *s, цел delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
бул SplitTo(const шим *s, цел (*delim)(цел), бул ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
бул SplitTo(const шим *s, цел (*delim)(цел), Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
бул SplitTo(const шим *s, const шим *delim, бул ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
бул SplitTo(const шим *s, const шим *delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
проц MergeWith(ШТкст& приёмник, const шим *delim, const Арги& ...арги)
{
	foreach_arg([&](const ШТкст& арг) {
		if(арг.дайСчёт()) {
			if(приёмник.дайСчёт())
				приёмник << delim;
			приёмник << арг;
		}
	}, арги...);
}

template <typename... Арги>
ШТкст Merge(const шим *delim, const Арги& ...арги)
{
	ШТкст r;
	MergeWith(r, delim, арги...);
	return r;
}

//$-
