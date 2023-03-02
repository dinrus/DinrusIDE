#include "Core.h"

namespace РНЦПДинрус {

template <class S, class Char, class F>
Вектор<S> SplitGeneric(int maxcount, const F& delim, const Char *s, bool ignoreempty = true)
{
	Вектор<S> r;
	r.резервируй(min(maxcount, 8));
	const Char *t = s;
	while(*t && r.дайСчёт() < maxcount) {
		const Char *q = delim(t);
		if(q) {
			if(!ignoreempty || t > s)
				r.добавь().уст(s, int(t - s)); // This is faster than r.добавь(Ткст(s, t))...
			t = s = q;
		}
		else
			t++;
	}
	if((!ignoreempty || t > s) && r.дайСчёт() < maxcount)
		r.добавь().уст(s, int(t - s));
	return r;
}

Вектор<Ткст> разбей(int maxcount, const char *s, const char * (*text_filter)(const char *), bool ignoreempty)
{
	return SplitGeneric<Ткст>(maxcount, text_filter, s, ignoreempty);
}

struct SplitDelimFilter__ {
	int (*filter)(int);
	const char *operator()(const char *s) const { return (*filter)((byte)*s) ? s + 1 : NULL; }
};

Вектор<Ткст> разбей(int maxcount, const char *s, int (*filter)(int), bool ignoreempty)
{
	SplitDelimFilter__ delim;
	delim.filter = filter;
	return SplitGeneric<Ткст>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimChar__ {
	int chr;
	const char *operator()(const char *s) const { return *s == chr ? s + 1 : NULL; }
};

Вектор<Ткст> разбей(int maxcount, const char *s, int chr, bool ignoreempty)
{
	SplitDelimChar__ delim;
	delim.chr = chr;
	return SplitGeneric<Ткст>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimText__ {
	const char *ds;
	int l;
	const char *operator()(const char *s) const { return strncmp(s, ds, l) == 0 ? s + l : NULL; }
};

Вектор<Ткст> разбей(int maxcount, const char *s, const char *text, bool ignoreempty)
{
	SplitDelimText__ delim;
	delim.ds = text;
	delim.l = (int)strlen(text);
	return delim.l ? SplitGeneric<Ткст>(maxcount, delim, s, ignoreempty) : Вектор<Ткст>();
}

Вектор<Ткст> разбей(const char *s, const char * (*text_filter)(const char *), bool ignoreempty)
{
	return разбей(INT_MAX, s, text_filter, ignoreempty);
}

Вектор<Ткст> разбей(const char *s, int (*filter)(int), bool ignoreempty)
{
	return разбей(INT_MAX, s, filter, ignoreempty);
}

Вектор<Ткст> разбей(const char *s, int chr, bool ignoreempty)
{
	return разбей(INT_MAX, s, chr, ignoreempty);
}

Вектор<Ткст> разбей(const char *s, const char *text, bool ignoreempty)
{
	return разбей(INT_MAX, s, text, ignoreempty);
}

Вектор<ШТкст> разбей(int maxcount, const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty)
{
	return SplitGeneric<ШТкст>(maxcount, text_filter, s, ignoreempty);
}

struct SplitDelimWFilter__ {
	int (*filter)(int);
	const wchar *operator()(const wchar *s) const { return (*filter)((byte)*s) ? s + 1 : NULL; }
};

Вектор<ШТкст> разбей(int maxcount, const wchar *s, int (*filter)(int), bool ignoreempty)
{
	SplitDelimWFilter__ delim;
	delim.filter = filter;
	return SplitGeneric<ШТкст>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimWChar__ {
	int chr;
	const wchar *operator()(const wchar *s) const { return *s == chr ? s + 1 : NULL; }
};

Вектор<ШТкст> разбей(int maxcount, const wchar *s, int chr, bool ignoreempty)
{
	SplitDelimWChar__ delim;
	delim.chr = chr;
	return SplitGeneric<ШТкст>(maxcount, delim, s, ignoreempty);
}

int w_strncmp(const wchar *s, const wchar *t, int n)
{
	while(*s && *t && n > 0) {
		int q = (int)(uint16)*s - (int)(uint16)*t;
		if(q)
			return q;
		s++;
		t++;
		n--;
	}
	return 0;
}

struct SplitDelimWText {
	const wchar *ds;
	int l;
	const wchar *operator()(const wchar *s) const { return w_strncmp(s, ds, l) == 0 ? s + l : NULL; }
};

Вектор<ШТкст> разбей(int maxcount, const wchar *s, const wchar *text, bool ignoreempty)
{
	SplitDelimWText delim;
	delim.ds = text;
	delim.l = strlen__(text);
	return delim.l ? SplitGeneric<ШТкст>(maxcount, delim, s, ignoreempty) : Вектор<ШТкст>();
}

Вектор<ШТкст> разбей(const wchar *s, int (*filter)(int), bool ignoreempty)
{
	return разбей(INT_MAX, s, filter, ignoreempty);
}

Вектор<ШТкст> разбей(const wchar *s, int chr, bool ignoreempty)
{
	return разбей(INT_MAX, s, chr, ignoreempty);
}

Вектор<ШТкст> разбей(const wchar *s, const wchar *text, bool ignoreempty)
{
	return разбей(INT_MAX, s, text, ignoreempty);
}

Вектор<ШТкст> разбей(const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty)
{
	return SplitGeneric<ШТкст>(INT_MAX, text_filter, s, ignoreempty);
}

template <class B, class T>
T Join_(const Вектор<T>& im, const T& delim, bool ignoreempty) {
	B r;
	bool next = false;
	for(int i = 0; i < im.дайСчёт(); i++)
		if(!ignoreempty || im[i].дайСчёт()) {
			if(next)
				r.конкат(delim);
			r.конкат(im[i]);
			next = true;
		}
	return T(r);
}

Ткст Join(const Вектор<Ткст>& im, const Ткст& delim, bool ignoreempty) {
	return Join_<ТкстБуф, Ткст>(im, delim, ignoreempty);
}

ШТкст Join(const Вектор<ШТкст>& im, const ШТкст& delim, bool ignoreempty) {
	return Join_<ШТкстБуф, ШТкст>(im, delim, ignoreempty);
}

}
