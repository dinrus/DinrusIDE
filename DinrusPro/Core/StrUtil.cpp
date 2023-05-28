#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {
	
#ifdef PLATFORM_POSIX
цел stricmp(кткст0 a, кткст0 b)         { return strcasecmp(a, b); }
цел strnicmp(кткст0 a, кткст0 b, цел n) { return strncasecmp(a, b, n); }
#endif

#ifdef PLATFORM_WINCE
цел stricmp(кткст0 a, кткст0 b)         { return _stricmp(a, b); }
цел strnicmp(кткст0 a, кткст0 b, цел n) { return _strnicmp(a, b, n); }
#endif

цел длинтекс__(кткст0 s)  { return длинтекс8(s); }
цел длинтекс__(const шим *s)       { return длинтекс32(s); }

цел сравнзн__(char x)               { return (ббайт)x; }
цел сравнзн__(шим x)              { return x; }
	
цел длинтекс8(кткст0 s) { return s ? (цел)strlen(s) : 0; }

цел длинтекс16(const char16 *s)
{
	if(!s) return 0;
	const char16 *s0 = s;
	while(*s) s++;
	return цел(s - s0);
}

цел длинтекс32(const шим *s)
{
	if(!s) return 0;
	const шим *s0 = s;
	while(*s) s++;
	return цел(s - s0);
}

unsigned ctoi(цел c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	return (unsigned)-1;
}

цел CharFilterAscii(цел c)
{
	return c >= 32 && c < 256 ? c : 0;
}

цел CharFilterAscii128(цел c)
{
	return c >= 32 && c < 128 ? c : 0;
}

цел CharFilterUnicode(цел c)
{
	return c >= 32 && c < 0x10FFFF ? c : 0;
}

цел CharFilterDigit(цел c)
{
	return цифра_ли(c) ? c : 0;
}

цел CharFilterInt(цел c)
{
	if(c == '+' || c == '-') return c;
	return CharFilterDigit(c);
}

цел CharFilterDouble(цел c)
{
	if(c == ',' || c == '.') return '.';
	if(c == 'e' || c == 'E') return 'E';
	return CharFilterInt(c);
}

цел CharFilterWhitespace(цел c)
{
	return пробел_ли(c) ? c : 0;
}

цел CharFilterNotWhitespace(цел c)
{
	return пробел_ли(c) ? 0 : c;
}

цел CharFilterAlpha(цел c)
{
	return альфа_ли(c) ? c : 0;
}

цел CharFilterToUpper(цел c)
{
	return взаг(c);
}

цел CharFilterToLower(цел c)
{
	return впроп(c);
}

цел CharFilterToUpperAscii(цел c)
{
	return вАскиЗаг(c);
}

цел CharFilterAlphaToUpper(цел c)
{
	return альфа_ли(c) ? проп_ли(c) ? c : взаг(c) : 0;
}

цел CharFilterAlphaToLower(цел c)
{
	return альфа_ли(c) ? заг_ли(c) ? c : впроп(c) : 0;
}

цел CharFilterDefaultToUpperAscii(цел c)
{
	return взаг(вАски(c, CHARSET_DEFAULT));
}

цел CharFilterCrLf(цел c)
{
	return c == '\r' || c == '\n' ? c : 0;
}

цел CharFilterNoCrLf(цел c)
{
	return c != '\r' && c != '\n' ? c : 0;
}

Ткст фильтруй(кткст0 s, цел (*фильтр)(цел))
{
	Ткст result;
	while(*s) {
		цел c = (*фильтр)((ббайт)*s++);
		if(c) result.кат(c);
	}
	return result;
}

Ткст FilterWhile(кткст0 s, цел (*фильтр)(цел))
{
	Ткст result;
	while(*s) {
		цел c = (*фильтр)((ббайт)*s++);
		if(!c) break;
		result.кат(c);
	}
	return result;
}

ШТкст фильтруй(const шим *s, цел (*фильтр)(цел))
{
	ШТкст result;
	while(*s) {
		цел c = (*фильтр)(*s++);
		if(c) result.кат(c);
	}
	return result;
}

ШТкст FilterWhile(const шим *s, цел (*фильтр)(цел))
{
	ШТкст result;
	while(*s) {
		цел c = (*фильтр)(*s++);
		if(!c) break;
		result.кат(c);
	}
	return result;
}

}
