#include "Core.h"

namespace РНЦПДинрус {

int strlen16(const char16 *s)
{
	if(!s) return 0;
	const char16 *s0 = s;
	while(*s) s++;
	return int(s - s0);
}

int strlen32(const wchar *s)
{
	if(!s) return 0;
	const wchar *s0 = s;
	while(*s) s++;
	return int(s - s0);
}

unsigned ctoi(int c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	return (unsigned)-1;
}

int CharFilterAscii(int c)
{
	return c >= 32 && c < 256 ? c : 0;
}

int CharFilterAscii128(int c)
{
	return c >= 32 && c < 128 ? c : 0;
}

int CharFilterUnicode(int c)
{
	return c >= 32 && c < 0x10FFFF ? c : 0;
}

int CharFilterDigit(int c)
{
	return цифра_ли(c) ? c : 0;
}

int CharFilterInt(int c)
{
	if(c == '+' || c == '-') return c;
	return CharFilterDigit(c);
}

int CharFilterDouble(int c)
{
	if(c == ',' || c == '.') return '.';
	if(c == 'e' || c == 'E') return 'E';
	return CharFilterInt(c);
}

int CharFilterWhitespace(int c)
{
	return пробел_ли(c) ? c : 0;
}

int CharFilterNotWhitespace(int c)
{
	return пробел_ли(c) ? 0 : c;
}

int CharFilterAlpha(int c)
{
	return альфа_ли(c) ? c : 0;
}

int CharFilterToUpper(int c)
{
	return взаг(c);
}

int CharFilterToLower(int c)
{
	return впроп(c);
}

int CharFilterToUpperAscii(int c)
{
	return вАскиЗаг(c);
}

int CharFilterAlphaToUpper(int c)
{
	return альфа_ли(c) ? проп_ли(c) ? c : взаг(c) : 0;
}

int CharFilterAlphaToLower(int c)
{
	return альфа_ли(c) ? заг_ли(c) ? c : впроп(c) : 0;
}

int CharFilterDefaultToUpperAscii(int c)
{
	return взаг(вАски(c, CHARSET_DEFAULT));
}

int CharFilterCrLf(int c)
{
	return c == '\r' || c == '\n' ? c : 0;
}

int CharFilterNoCrLf(int c)
{
	return c != '\r' && c != '\n' ? c : 0;
}

Ткст фильтруй(const char *s, int (*filter)(int))
{
	Ткст result;
	while(*s) {
		int c = (*filter)((byte)*s++);
		if(c) result.конкат(c);
	}
	return result;
}

Ткст FilterWhile(const char *s, int (*filter)(int))
{
	Ткст result;
	while(*s) {
		int c = (*filter)((byte)*s++);
		if(!c) break;
		result.конкат(c);
	}
	return result;
}

ШТкст фильтруй(const wchar *s, int (*filter)(int))
{
	ШТкст result;
	while(*s) {
		int c = (*filter)(*s++);
		if(c) result.конкат(c);
	}
	return result;
}

ШТкст FilterWhile(const wchar *s, int (*filter)(int))
{
	ШТкст result;
	while(*s) {
		int c = (*filter)(*s++);
		if(!c) break;
		result.конкат(c);
	}
	return result;
}

}
