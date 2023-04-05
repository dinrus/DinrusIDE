Вектор<Ткст> разбей(int maxcount, const char *s, const char * (*text_filter)(const char *), bool ignoreempty = true);
Вектор<Ткст> разбей(int maxcount, const char *s, int (*filter)(int), bool ignoreempty = true);
Вектор<Ткст> разбей(int maxcount, const char *s, int chr, bool ignoreempty = true);
Вектор<Ткст> разбей(int maxcount, const char *s, const char *text, bool ignoreempty = true);
Вектор<Ткст> разбей(const char *s, const char * (*text_filter)(const char *), bool ignoreempty = true);
Вектор<Ткст> разбей(const char *s, int (*filter)(int), bool ignoreempty = true);
Вектор<Ткст> разбей(const char *s, int chr, bool ignoreempty = true);
Вектор<Ткст> разбей(const char *s, const char *text, bool ignoreempty = true);

Вектор<ШТкст> разбей(int maxcount, const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty = true);
Вектор<ШТкст> разбей(int maxcount, const wchar *s, int (*filter)(int), bool ignoreempty = true);
Вектор<ШТкст> разбей(int maxcount, const wchar *s, int chr, bool ignoreempty = true);
Вектор<ШТкст> разбей(int maxcount, const wchar *s, const wchar *text, bool ignoreempty = true);
Вектор<ШТкст> разбей(const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty = true);
Вектор<ШТкст> разбей(const wchar *s, int (*filter)(int), bool ignoreempty = true);
Вектор<ШТкст> разбей(const wchar *s, int chr, bool ignoreempty = true);
Вектор<ШТкст> разбей(const wchar *s, const wchar *text, bool ignoreempty = true);

Ткст  Join(const Вектор<Ткст>& im, const Ткст& delim, bool ignoreempty = false);
ШТкст Join(const Вектор<ШТкст>& im, const ШТкст& delim, bool ignoreempty = false);

//$ bool   SplitTo(const char *s, int delim, bool ignoreempty, Ткст& p1...);
//$ bool   SplitTo(const char *s, int delim, Ткст& p1...);
//$ bool   SplitTo(const char *s, int (*filter)(int), Ткст& p1...);
//$ bool   SplitTo(const char *s, int (*filter)(int), Ткст& p1...);
//$ bool   SplitTo(const char *s, const char *delim, bool ignoreempty, Ткст& p1...);
//$ bool   SplitTo(const char *s, const char *delim, Ткст& p1...);

//$ bool   SplitTo(const wchar *s, int delim, bool ignoreempty, ШТкст& p1...);
//$ bool   SplitTo(const wchar *s, int delim, ШТкст& p1...);
//$ bool   SplitTo(const wchar *s, int (*filter)(int), ШТкст& p1...);
//$ bool   SplitTo(const wchar *s, int (*filter)(int), ШТкст& p1...);
//$ bool   SplitTo(const wchar *s, const wchar *delim, bool ignoreempty, ШТкст& p1...);
//$ bool   SplitTo(const wchar *s, const wchar *delim, ШТкст& p1...);

//$ Ткст Merge(const char *delim, Ткст& p1...);
//$ ШТкст Merge(const wchar *delim, ШТкст& p1...);

//$ void   MergeWith(Ткст& dest, const char *delim, Ткст& p1...);
//$ void   MergeWith(ШТкст& dest, const wchar *delim, ШТкст& p1...);

//$-
template <typename... Арги>
bool SplitTo(const char *s, int delim, bool ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
bool SplitTo(const char *s, int delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
bool SplitTo(const char *s, int (*delim)(int), bool ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
bool SplitTo(const char *s, int (*delim)(int), Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
bool SplitTo(const char *s, const char *delim, bool ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
bool SplitTo(const char *s, const char *delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
void MergeWith(Ткст& dest, const char *delim, const Арги& ...арги)
{
	foreach_arg([&](const Ткст& арг) {
		if(арг.дайСчёт()) {
			if(dest.дайСчёт())
				dest << delim;
			dest << арг;
		}
	}, арги...);
}

template <typename... Арги>
Ткст Merge(const char *delim, const Арги& ...арги)
{
	Ткст r;
	MergeWith(r, delim, арги...);
	return r;
}

template <typename... Арги>
bool SplitTo(const wchar *s, int delim, bool ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
bool SplitTo(const wchar *s, int delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
bool SplitTo(const wchar *s, int (*delim)(int), bool ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
bool SplitTo(const wchar *s, int (*delim)(int), Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
bool SplitTo(const wchar *s, const wchar *delim, bool ignoreempty, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim, ignoreempty), арги...);
}

template <typename... Арги>
bool SplitTo(const wchar *s, const wchar *delim, Арги& ...арги)
{
	return scatter(разбей(sizeof...(арги), s, delim), арги...);
}

template <typename... Арги>
void MergeWith(ШТкст& dest, const wchar *delim, const Арги& ...арги)
{
	foreach_arg([&](const ШТкст& арг) {
		if(арг.дайСчёт()) {
			if(dest.дайСчёт())
				dest << delim;
			dest << арг;
		}
	}, арги...);
}

template <typename... Арги>
ШТкст Merge(const wchar *delim, const Арги& ...арги)
{
	ШТкст r;
	MergeWith(r, delim, арги...);
	return r;
}

//$-
