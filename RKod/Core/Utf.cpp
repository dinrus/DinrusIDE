#include "Core.h"

namespace РНЦПДинрус {

bool проверьУтф8(const char *s, int len)
{
	return FromUtf8_([](wchar) {}, s, len);
}

int длинаУтф8(const wchar *s, int len)
{
	int rlen = 0;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf8_([&](char) { rlen++; }, *s);
	return rlen;
}

void вУтф8(char *t, const wchar *s, int len)
{
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { *t++ = c; }, *s);
}

Ткст вУтф8(const wchar *s, int len)
{
	Ткст r;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf8_([&](char c) { r.конкат(c); }, *s);
	return r;
}

int длинаУтф8(const char16 *s, int len)
{
	int rlen = 0;
	FromUtf16_([&](wchar code) { ToUtf8_([&](char c) { rlen++; }, code); }, s, len);
	return rlen;
}

void вУтф8(char *t, const char16 *s, int len)
{
	FromUtf16_([&](wchar code) { ToUtf8_([&](char c) { *t++ = c; }, code); }, s, len);
}

Ткст вУтф8(const char16 *s, int len)
{
	ТкстБуф r;
	r.резервируй(len);
	FromUtf16_([&](wchar code) { ToUtf8_([&](char c) { r.конкат(c); }, code); }, s, len);
	return Ткст(r);
}

int длинаУтф16(const wchar *s, int len)
{
	int rlen = 0;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16) { rlen++; }, *s);
	return rlen;
}

int вУтф16(char16 *t, const wchar *s, int len)
{
	char16 *t0 = t;
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16 c) { *t++ = c; }, *s);
	return int(t - t0);
}

Вектор<char16> вУтф16(const wchar *s, int len)
{
	Вектор<char16> r;
	r.резервируй(len);
	for(const wchar *lim = s + len; s < lim; s++)
		ToUtf16_([&](char16 c) { r.добавь(c); }, *s);
	return r;
}

int длинаУтф16(const char *s, int len)
{
	int rlen = 0;
	FromUtf8_([&](wchar code) {
		ToUtf16_([&](char16) { rlen++; }, code);
	}, s, len);
	return rlen;
}

int вУтф16(char16 *t, const char *s, int len)
{
	char16 *t0 = t;
	FromUtf8_([&](wchar code) {
		ToUtf16_([&](char16 c) { *t++ = c; }, code);
	}, s, len);
	return int(t - t0);
}

Вектор<char16> вУтф16(const char *s, int len)
{
	Вектор<char16> r;
	FromUtf8_([&](wchar code) {
		ToUtf16_([&](char16 c) { r.добавь(c); } , code);
	}, s, len);
	return r;
}

int длинаУтф32(const char *s, int len)
{
	int rlen = 0;
	FromUtf8_([&](wchar) { rlen++; }, s, len);
	return rlen;
}

void вУтф32(wchar *t, const char *s, int len)
{
	FromUtf8_([&](wchar c) { *t++ = c; }, s, len);
}

ШТкст вУтф32(const char *s, int len)
{
	ШТкстБуф r;
	FromUtf8_([&](wchar c) { r.конкат(c); }, s, len);
	return ШТкст(r);
}

int длинаУтф32(const char16 *s, int len)
{
	int rlen = 0;
	FromUtf16_([&](wchar) { rlen++; }, s, len);
	return rlen;
}

void вУтф32(wchar *t, const char16 *s, int len)
{
	FromUtf16_([&](wchar c) { *t++ = c; }, s, len);
}

ШТкст вУтф32(const char16 *s, int len)
{
	ШТкстБуф r;
	r.резервируй(len);
	FromUtf16_([&](wchar c) { r.конкат(c); }, s, len);
	return ШТкст(r);
}

Ткст утф8ВАски(const Ткст& ист)
{
	ТкстБуф r(ист.дайДлину());
	const char *s = ист.begin();
	const char *lim = ист.end();

	char *t = r;
	while(s < lim)
		*t++ = (byte)*s < 128 ? *s++ : вАски(достаньУтф8(s, lim));
	r.устДлину(int(t - ~r));
	return Ткст(r);
}

Ткст утф8ВАскиЗаг(const Ткст& ист)
{
	ТкстБуф r(ист.дайДлину());
	const char *s = ист.begin();
	const char *lim = ист.end();

	char *t = r;
	while(s < lim) {
		*t++ = (byte)*s <= 'Z' ? *s++ : вАскиЗаг(достаньУтф8(s, lim));
	}
	r.устДлину(int(t - ~r));
	return Ткст(r);
}

Ткст утф8ВАскиПроп(const Ткст& ист)
{
	ТкстБуф r(ист.дайДлину());
	const char *s = ист.begin();
	const char *lim = ист.end();

	char *t = r;
	while(s < lim)
		*t++ = вАскиПроп(достаньУтф8(s, lim));
	r.устДлину(int(t - ~r));
	return Ткст(r);
}

};