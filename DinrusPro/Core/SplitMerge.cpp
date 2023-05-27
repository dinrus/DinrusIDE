#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

template <class S, class Char, class F>
Вектор<S> SplitGeneric(цел maxcount, const F& delim, const Char *s, бул ignoreempty = true)
{
	Вектор<S> r;
	r.резервируй(мин(maxcount, 8));
	const Char *t = s;
	while(*t && r.дайСчёт() < maxcount) {
		const Char *q = delim(t);
		if(q) {
			if(!ignoreempty || t > s)
				r.добавь().уст(s, цел(t - s)); // This is faster than r.добавь(Ткст(s, t))...
			t = s = q;
		}
		else
			t++;
	}
	if((!ignoreempty || t > s) && r.дайСчёт() < maxcount)
		r.добавь().уст(s, цел(t - s));
	return r;
}

Вектор<Ткст> разбей(цел maxcount, кткст0 s, кткст0  (*text_filter)(const char *), бул ignoreempty)
{
	return SplitGeneric<Ткст>(maxcount, text_filter, s, ignoreempty);
}

struct SplitDelimFilter__ {
	цел (*filter)(цел);
	кткст0 operator()(кткст0 s) const { return (*filter)((ббайт)*s) ? s + 1 : NULL; }
};

Вектор<Ткст> разбей(цел maxcount, кткст0 s, цел (*filter)(цел), бул ignoreempty)
{
	SplitDelimFilter__ delim;
	delim.filter = filter;
	return SplitGeneric<Ткст>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimChar__ {
	цел chr;
	кткст0 operator()(кткст0 s) const { return *s == chr ? s + 1 : NULL; }
};

Вектор<Ткст> разбей(цел maxcount, кткст0 s, цел chr, бул ignoreempty)
{
	SplitDelimChar__ delim;
	delim.chr = chr;
	return SplitGeneric<Ткст>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimText__ {
	кткст0 ds;
	цел l;
	кткст0 operator()(кткст0 s) const { return strncmp(s, ds, l) == 0 ? s + l : NULL; }
};

Вектор<Ткст> разбей(цел maxcount, кткст0 s, кткст0 text, бул ignoreempty)
{
	SplitDelimText__ delim;
	delim.ds = text;
	delim.l = (цел)strlen(text);
	return delim.l ? SplitGeneric<Ткст>(maxcount, delim, s, ignoreempty) : Вектор<Ткст>();
}

Вектор<Ткст> разбей(кткст0 s, кткст0  (*text_filter)(const char *), бул ignoreempty)
{
	return разбей(INT_MAX, s, text_filter, ignoreempty);
}

Вектор<Ткст> разбей(кткст0 s, цел (*filter)(цел), бул ignoreempty)
{
	return разбей(INT_MAX, s, filter, ignoreempty);
}

Вектор<Ткст> разбей(кткст0 s, цел chr, бул ignoreempty)
{
	return разбей(INT_MAX, s, chr, ignoreempty);
}

Вектор<Ткст> разбей(кткст0 s, кткст0 text, бул ignoreempty)
{
	return разбей(INT_MAX, s, text, ignoreempty);
}

Вектор<ШТкст> разбей(цел maxcount, const шим *s, const шим * (*text_filter)(const шим *), бул ignoreempty)
{
	return SplitGeneric<ШТкст>(maxcount, text_filter, s, ignoreempty);
}

struct SplitDelimWFilter__ {
	цел (*filter)(цел);
	const шим *operator()(const шим *s) const { return (*filter)((ббайт)*s) ? s + 1 : NULL; }
};

Вектор<ШТкст> разбей(цел maxcount, const шим *s, цел (*filter)(цел), бул ignoreempty)
{
	SplitDelimWFilter__ delim;
	delim.filter = filter;
	return SplitGeneric<ШТкст>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimWChar__ {
	цел chr;
	const шим *operator()(const шим *s) const { return *s == chr ? s + 1 : NULL; }
};

Вектор<ШТкст> разбей(цел maxcount, const шим *s, цел chr, бул ignoreempty)
{
	SplitDelimWChar__ delim;
	delim.chr = chr;
	return SplitGeneric<ШТкст>(maxcount, delim, s, ignoreempty);
}

цел w_strncmp(const шим *s, const шим *t, цел n)
{
	while(*s && *t && n > 0) {
		цел q = (цел)(бкрат)*s - (цел)(бкрат)*t;
		if(q)
			return q;
		s++;
		t++;
		n--;
	}
	return 0;
}

struct SplitDelimWText {
	const шим *ds;
	цел l;
	const шим *operator()(const шим *s) const { return w_strncmp(s, ds, l) == 0 ? s + l : NULL; }
};

Вектор<ШТкст> разбей(цел maxcount, const шим *s, const шим *text, бул ignoreempty)
{
	SplitDelimWText delim;
	delim.ds = text;
	delim.l = длинтекс__(text);
	return delim.l ? SplitGeneric<ШТкст>(maxcount, delim, s, ignoreempty) : Вектор<ШТкст>();
}

Вектор<ШТкст> разбей(const шим *s, цел (*filter)(цел), бул ignoreempty)
{
	return разбей(INT_MAX, s, filter, ignoreempty);
}

Вектор<ШТкст> разбей(const шим *s, цел chr, бул ignoreempty)
{
	return разбей(INT_MAX, s, chr, ignoreempty);
}

Вектор<ШТкст> разбей(const шим *s, const шим *text, бул ignoreempty)
{
	return разбей(INT_MAX, s, text, ignoreempty);
}

Вектор<ШТкст> разбей(const шим *s, const шим * (*text_filter)(const шим *), бул ignoreempty)
{
	return SplitGeneric<ШТкст>(INT_MAX, text_filter, s, ignoreempty);
}

template <class B, class T>
T Join_(const Вектор<T>& im, const T& delim, бул ignoreempty) {
	B r;
	бул next = false;
	for(цел i = 0; i < im.дайСчёт(); i++)
		if(!ignoreempty || im[i].дайСчёт()) {
			if(next)
				r.кат(delim);
			r.кат(im[i]);
			next = true;
		}
	return T(r);
}

Ткст Join(const Вектор<Ткст>& im, const Ткст& delim, бул ignoreempty) {
	return Join_<ТкстБуф, Ткст>(im, delim, ignoreempty);
}

ШТкст Join(const Вектор<ШТкст>& im, const ШТкст& delim, бул ignoreempty) {
	return Join_<ШТкстБуф, ШТкст>(im, delim, ignoreempty);
}

}
