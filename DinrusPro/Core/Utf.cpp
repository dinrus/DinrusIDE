#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

бул проверьУтф8(кткст0 s, цел len)
{
	return FromUtf8_([](шим) {}, s, len);
}

цел длинаУтф8(const шим *s, цел len)
{
	цел rlen = 0;
	for(const шим *lim = s + len; s < lim; s++)
		ToUtf8_([&](char) { rlen++; }, *s);
	return rlen;
}

проц вУтф8(char *t, const шим *s, цел len)
{
	for(const шим *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { *t++ = c; }, *s);
}

Ткст вУтф8(const шим *s, цел len)
{
	Ткст r;
	for(const шим *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { r.кат(c); }, *s);
	return r;
}

цел длинаУтф8(const char16 *s, цел len)
{
	цел rlen = 0;
	FromUtf16_([&](шим code) { ToUtf8_([&](char c) { rlen++; }, code); }, s, len);
	return rlen;
}

проц вУтф8(char *t, const char16 *s, цел len)
{
	FromUtf16_([&](шим code) { ToUtf8_([&](char c) { *t++ = c; }, code); }, s, len);
}

Ткст вУтф8(const char16 *s, цел len)
{
	ТкстБуф r;
	r.резервируй(len);
	FromUtf16_([&](шим code) { ToUtf8_([&](char c) { r.кат(c); }, code); }, s, len);
	return Ткст(r);
}

цел длинаУтф16(const шим *s, цел len)
{
	цел rlen = 0;
	for(const шим *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16) { rlen++; }, *s);
	return rlen;
}

цел вУтф16(char16 *t, const шим *s, цел len)
{
	char16 *t0 = t;
	for(const шим *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16 c) { *t++ = c; }, *s);
	return цел(t - t0);
}

Вектор<char16> вУтф16(const шим *s, цел len)
{
	Вектор<char16> r;
	r.резервируй(len);
	for(const шим *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16 c) { r.добавь(c); }, *s);
	return r;
}

цел длинаУтф16(кткст0 s, цел len)
{
	цел rlen = 0;
	FromUtf8_([&](шим code) {
		ToUtf16_([&](char16) { rlen++; }, code);
	}, s, len);
	return rlen;
}

цел вУтф16(char16 *t, кткст0 s, цел len)
{
	char16 *t0 = t;
	FromUtf8_([&](шим code) {
		ToUtf16_([&](char16 c) { *t++ = c; }, code);
	}, s, len);
	return цел(t - t0);
}

Вектор<char16> вУтф16(кткст0 s, цел len)
{
	Вектор<char16> r;
	FromUtf8_([&](шим code) {
		ToUtf16_([&](char16 c) { r.добавь(c); } , code);
	}, s, len);
	return r;
}

цел длинаУтф32(кткст0 s, цел len)
{
	цел rlen = 0;
	FromUtf8_([&](шим) { rlen++; }, s, len);
	return rlen;
}

проц вУтф32(шим *t, кткст0 s, цел len)
{
	FromUtf8_([&](шим c) { *t++ = c; }, s, len);
}

ШТкст вУтф32(кткст0 s, цел len)
{
	ШТкстБуф r;
	FromUtf8_([&](шим c) { r.кат(c); }, s, len);
	return ШТкст(r);
}

цел длинаУтф32(const char16 *s, цел len)
{
	цел rlen = 0;
	FromUtf16_([&](шим) { rlen++; }, s, len);
	return rlen;
}

проц вУтф32(шим *t, const char16 *s, цел len)
{
	FromUtf16_([&](шим c) { *t++ = c; }, s, len);
}

ШТкст вУтф32(const char16 *s, цел len)
{
	ШТкстБуф r;
	r.резервируй(len);
	FromUtf16_([&](шим c) { r.кат(c); }, s, len);
	return ШТкст(r);
}

Ткст утф8ВАски(const Ткст& ист)
{
	ТкстБуф r(ист.дайДлину());
	кткст0 s = ист.begin();
	кткст0 lim = ист.end();

	char *t = r;
	while(s < lim)
		*t++ = (ббайт)*s < 128 ? *s++ : вАски(достаньУтф8(s, lim));
	r.устДлину(цел(t - ~r));
	return Ткст(r);
}

Ткст утф8ВАскиЗаг(const Ткст& ист)
{
	ТкстБуф r(ист.дайДлину());
	кткст0 s = ист.begin();
	кткст0 lim = ист.end();

	char *t = r;
	while(s < lim) {
		*t++ = (ббайт)*s <= 'Z' ? *s++ : вАскиЗаг(достаньУтф8(s, lim));
	}
	r.устДлину(цел(t - ~r));
	return Ткст(r);
}

Ткст утф8ВАскиПроп(const Ткст& ист)
{
	ТкстБуф r(ист.дайДлину());
	кткст0 s = ист.begin();
	кткст0 lim = ист.end();

	char *t = r;
	while(s < lim)
		*t++ = вАскиПроп(достаньУтф8(s, lim));
	r.устДлину(цел(t - ~r));
	return Ткст(r);
}

};