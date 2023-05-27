#include <DinrusPro/DinrusPro.h>
//#BLITZ_APPROVE

namespace ДинрусРНЦП {

// Old формат ---------------------------

Ткст  вфмт(кткст0 fmt, va_list укз) {
	цел limit = 2 * (цел)strlen(fmt) + 1024;
	if(limit < 1500) {
		char буфер[1500];
		vsprintf(буфер, fmt, укз);
		va_end(укз);
		цел len = (цел)strlen(буфер);
		ПРОВЕРЬ(len <= limit);
		return Ткст(буфер, len);
	}
	else {
		Буфер<char> буфер(limit);
		vsprintf(буфер, fmt, укз);
		va_end(укз);
		цел len = (цел)strlen(буфер);
		ПРОВЕРЬ(len <= limit);
		return Ткст(буфер, len);
	}
}

// Форматирование routines ---------------------------

// utoa32, utoa64 inspired by
// https://github.com/miloyip/itoa-benchmark/blob/940542a7770155ee3e9f2777ebc178dc899b43e0/ист/branchlut.cpp
// by Milo Yip

const char s100[] =
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
;

namespace utoa_private {

force_inline
проц  Do2(char *t, бцел d) {
	memcpy(t, s100 + 2 * d, 2);
};

force_inline
проц  Do4(char *t, бцел значение) {
	Do2(t, значение / 100);
	Do2(t + 2, значение % 100);
}

force_inline
проц  Do8(char *t, бцел значение) {
	Do4(t, значение / 10000);
	Do4(t + 4, значение % 10000);
}

};

цел utoa32(бцел значение, char *буфер)
{
	using namespace utoa_private;

	if (значение < 10000) {
		if(значение < 100) {
			if(значение < 10) {
				*буфер = char(значение + '0');
				return 1;
			}
			Do2(буфер, значение % 100);
			return 2;
		}

		if(значение < 1000) {
			*буфер = char(значение / 100 + '0');
			Do2(буфер + 1, значение % 100);
			return 3;
		}
		
		Do4(буфер, значение);
		return 4;
	}
	else if (значение < 100000000) {
		if(значение < 10000000) {
			if(значение < 100000) {
				*буфер = char(значение / 10000 + '0');
				Do4(буфер + 1, значение % 10000);
				return 5;
			}
			if(значение < 1000000) {
				Do2(буфер, значение / 10000);
				Do4(буфер + 2, значение % 10000);
				return 6;
			}
			*буфер = char(значение / 1000000 + '0');
			Do2(буфер + 1, значение / 10000 % 100);
			Do4(буфер + 3, значение % 10000);
			return 7;
		}
		
		Do8(буфер, значение);
		return 8;
	}
	else {
		бцел a = значение / 100000000; // 2 digits
		значение %= 100000000;

		if(a < 10) {
			*буфер = char(a + '0');
			Do8(буфер + 1, значение);
			return 9;
		}

		Do2(буфер, a);
		Do8(буфер + 2, значение);
		return 10;
	}
}

цел utoa64(бдол значение, char *буфер)
{
	using namespace utoa_private;

	if(значение <= 0xffffffff)
		return utoa32((бцел)значение, буфер);
	if(значение < (бдол)1000000000 * 100000000) {
		цел q = utoa32(бцел(значение / 100000000), буфер);
		Do8(буфер + q, значение % 100000000);
		return q + 8;
	}
	цел q = utoa32(бцел(значение / ((бдол)100000000 * 100000000)), буфер);
	Do8(буфер + q, значение / 100000000 % 100000000);
	Do8(буфер + 8 + q, значение % 100000000);
	return q + 16;
}

Ткст фмтБцел64(бдол w)
{
	if(w < 100000000000000)
		return Ткст::сделай(14, [&](char *s) { return utoa64(w, s); });
	else
		return Ткст::сделай(20, [&](char *s) { return utoa64(w, s); });
}

Ткст фмтЦел64(дол i)
{
	if(пусто_ли(i))
		return Ткст();
	if(i < 0) {
		i = -i;
		if(i < 10000000000000)
			return Ткст::сделай(14, [&](char *s) {
				*s++ = '-';
				return utoa64(i, s) + 1;
			});
		return Ткст::сделай(20, [&](char *s) {
			*s++ = '-';
			return utoa64(i, s) + 1;
		});
	}
	if(i < 100000000000000)
		return Ткст::сделай(14, [&](char *s) { return utoa64(i, s); });
	return Ткст::сделай(20, [&](char *s) { return utoa64(i, s); });
}

Ткст фмтЦелОснова(цел i, цел base, цел width, char lpad, цел sign, бул upper)
{
	enum { BUFFER = sizeof(цел) * 8 + 1 };
	if(base < 2 || base > 36)
		return "<invalid base>";
	char буфер[BUFFER];
	char *const e = буфер + (цел)BUFFER;
	char *p = e;
	кткст0 itoc = upper ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
	if(sign < 0 || !пусто_ли(i))
	{
		unsigned x = i;
		if(sign >= 0 && i < 0)
			x = -i;
		do
			*--p = itoc[x % base];
		while(x /= base);
	}
	бул minus = (sign >= 0 && i < 0 && !пусто_ли(i));
	бул do_sign = (sign > 0 || sign >= 0 && minus);
	if(do_sign && lpad != '0')
		*--p = (minus ? '-' : '+');
	if(width > e - p)
	{
		char *b = e - мин<цел>(width, BUFFER);
		while(p > b)
			*--p = lpad;
	}
	if(do_sign && lpad == '0')
		*--p = (minus ? '-' : '+');
	цел dwd = (цел)(e - p);
	цел pad = (width = макс(width, dwd)) - dwd;
	ТкстБуф out(width);
	char *o = out;
	if(dwd < width)
		memset(o, lpad, pad);
	копирпам8(o + pad, p, dwd);
	return Ткст(out);
}

Ткст фмтЦелДес(цел i, цел width, char lpad, бул always_sign)
{
	return фмтЦелОснова(i, 10, width, lpad, always_sign ? 1 : 0);
}

Ткст фмтЦелГекс(цел i, цел width, char lpad)
{
	return фмтЦелОснова(i, 16, width, lpad, -1);
}

Ткст фмтЦелГексВерхРег(цел i, цел width, char lpad)
{
	return фмтЦелОснова(i, 16, width, lpad, -1, true);
}

Ткст фмтЦелВосьм(цел i, цел width, char lpad)
{
	return фмтЦелОснова(i, 8, width, lpad, -1);
}

Ткст фмтЦелАльфа(цел i, бул upper)
{
	if(пусто_ли(i) || i == 0)
		return Null;
	Ткст out;
	if(i < 0)
	{
		out << '-';
		i = -i;
	}
	char temp[10], *p = temp + 10;
	кткст0 itoc = upper ? "ZABCDEFGHIJKLMNOPQRSTUVWXYZ" : "zabcdefghijklmnopqrstuvwxyz";
	do
		*--p = itoc[i-- % 26];
	while(i /= 26);
	out.кат(p, temp + 10);
	return out;
}

Ткст фмтЦелРим(цел i, бул upper)
{
	if(пусто_ли(i) || i == 0)
		return Null;

	Ткст out;
	if(i < 0)
	{
		out << '-';
		i = -i;
	}
	цел m = i / 1000;
	if(m)
	{
		out.кат('M', m);
		i -= 1000 * m;
	}

	char shift = upper ? 0 : 'a' - 'A';
	static const цел  значение[] =  { 1000, 500, 100, 50,  10,  5,   1 };
	static const char letter[] = { 'M',  'D', 'C', 'L', 'X', 'V', 'I' };
	for(цел n = 0; i && n < __countof(значение); n++)
	{
		цел v = значение[n];
		while(i >= v)
		{
			out << (char)(letter[n] + shift);
			i -= v;
		}
		if(n < __countof(значение) - 1)
			for(цел j = n + (значение[n + 1] * 2 >= v ? 2 : 1); j < __countof(значение); j++)
				if(i >= v - значение[j])
				{ // subtraction scheme
					out << (char)(letter[j] + shift) << (char)(letter[n] + shift);
					i -= v - значение[j];
					break;
				}
	}
	return out;
}

Ткст фмт64Гекс(бдол a)
{
	char b[50];
	char *p = b + 50;
	do {
		*--p = "0123456789abcdef"[a & 15];
		a >>= 4;
	}
	while(a);
	return Ткст(p, b + 50);
}

Ткст FormatBool(бул a)              { return a ? "true" : "false"; }
Ткст фмтУк(const ук p)        { return "0x" + фмтГекс(p); }

Ткст фмтДата(Дата date, кткст0 формат, цел язык)
{
	if(пусто_ли(date))
		return Null;
	if(!формат || !*формат)
		return фмт(date);
	return фмтВремя(воВремя(date), формат, язык);
}

Ткст фмтВремя(Время t, кткст0 s, цел язык)
{
	if(пусто_ли(t))
		return Null;
	Ткст result;
	if(!s || !*s)
		return фмт(t);
	while(*s) {
		цел q = 0;
		if(*s == 'M') {
			while(*s == 'M') { s++; q++; }
			if(q == 1)
				result.кат(фмт("%d", t.month));
			else
				result.кат(фмт("%02d", t.month));
		}
		else
		if(*s == 'D') {
			while(*s == 'D') { s++; q++; }
			if(q == 1)
				result.кат(фмт("%d", t.day));
			else
				result.кат(фмт("%02d", t.day));
		}
		else
		if(*s == 'Y') {
			while(*s == 'Y') { s++; q++; }
			if(q == 1)
				result.кат(фмт("%d", t.year % 100));
			else
			if(q == 2)
				result.кат(фмт("%02d", t.year % 100));
			else
				result.кат(фмт("%d", t.year));
		}
		else
		if(*s == 'h') {
			while(*s == 'h') { s++; q++; }
			if(q == 1)
				result.кат(фмт("%d", t.hour));
			else
				result.кат(фмт("%02d", t.hour));
		}
		else
		if(*s == 'H') {
			while(*s == 'H') { s++; q++; }
			цел h = ((t.hour + 11) % 12 + 1);
			if(q == 1)
				result.кат(фмт("%d", h));
			else
				result.кат(фмт("%02d", h));
		}
		else
		if(*s == '<') {
			s++;
			while(*s && *s != '/') {
				if(t.hour <= 12)
					result.кат(*s);
				s++;
			}
			if(!*s) break;
			s++;
			while(*s && *s != '>') {
				if(t.hour > 12)
					result.кат(*s);
				s++;
			}
			if(!*s) break;
			s++;
		}
		else
		if(*s == 'm') {
			while(*s == 'm') { s++; q++; }
			if(q == 1)
				result.кат(фмт("%d", t.minute));
			else
				result.кат(фмт("%02d", t.minute));
		}
		else
		if(*s == 's') {
			while(*s == 's') { s++; q++; }
			if(q == 1)
				result.кат(фмт("%d", t.second));
			else
				result.кат(фмт("%02d", t.second));
		}
		else
		if(*s == '`' && s[1]) {
			s++;
			result.кат(*s++);
		}
		else
			result.кат(*s++);
	}
	return result;
}

// нов формат ----------------------------


проц TrimChar(Ткст& s, цел n)
{
	if(дайДефНабСим() == НАБСИМ_УТФ8) {
		ШТкст h(s);
		h.обрежь(n);
		s = h.вТкст();
	}
	else
		s.обрежь(n);
}

struct FormId : Движ<FormId> {
	FormId(Ткст ид, цел тип) : ид(ид), тип(тип) {}
	Ткст ид;
	цел    тип;
};

т_хэш дайХэшЗнач(const FormId& fid)
{
	return комбинируйХэш(fid.тип, дайХэшЗнач(fid.ид));
}

бул operator==(const FormId& a, const FormId& b)
{
	return a.тип == b.тип && a.ид == b.ид;
}

ВекторМап<FormId, Форматировщик>& formatmap()
{
	return Single< ВекторМап<FormId, Форматировщик> > ();
}

проц регФорматировщик(цел тип, кткст0 ид, Форматировщик f)
{
	проверьНаОН();
	INTERLOCKED {
		FormId fid(ид, тип);
		formatmap().найдиДобавь(fid, f);
		formatmap().найди(fid);
	}
}

проц регФорматировщикЗначений(кткст0 ид, Форматировщик f)
{
	регФорматировщик(VALUE_V, ид, f);
}

проц регФорматировщикПусто(кткст0 ид, Форматировщик f)
{
	регФорматировщик(VOID_V, ид, f);
	регФорматировщик(ERROR_V, ид, f);
}

проц регФорматировщикЧисел(кткст0 ид, Форматировщик f)
{
	регФорматировщик(DOUBLE_V, ид, f);
	регФорматировщик(INT64_V, ид, f);
	регФорматировщик(INT_V, ид, f);
	регФорматировщик(BOOL_V, ид, f);
	регФорматировщикПусто(ид, f);
}

проц регФорматировщикТекста(кткст0 ид, Форматировщик f)
{
	регФорматировщик(WSTRING_V, ид, f);
	регФорматировщик(STRING_V, ид, f);
	регФорматировщикПусто(ид, f);
}

проц регФорматировщикДатВремени(кткст0 ид, Форматировщик f)
{
	регФорматировщик(TIME_V, ид, f);
	регФорматировщик(DATE_V, ид, f);
	регФорматировщикПусто(ид, f);
}

Ткст IntFormatter(const Форматирование& f)
{
	if(f.формат.дайСчёт() == 0 && f.ид[0] == 'd' && f.ид[1] == 0)
		return какТкст((цел)f.арг);
	ТкстБуф q;
	q.устДлину(1000);
	q.устДлину(sprintf(q, '%' + f.формат + f.ид, (цел)f.арг));
	return Ткст(q);
}

Ткст Int64Formatter(const Форматирование& f)
{
	ТкстБуф q;
	q.устДлину(1000);
	q.устДлину(sprintf(q, '%' + f.формат + f.ид, (дол)f.арг));
	return Ткст(q);
}

Ткст IntLowerAlphaFormatter(const Форматирование& f)
{
	return фмтЦелАльфа(f.арг, false);
}

Ткст IntUpperAlphaFormatter(const Форматирование& f)
{
	return фмтЦелАльфа(f.арг, true);
}

Ткст IntLowerRomanFormatter(const Форматирование& f)
{
	return фмтЦелРим(f.арг, false);
}

Ткст IntUpperRomanFormatter(const Форматирование& f)
{
	return фмтЦелРим(f.арг, true);
}

Ткст DoubleFormatter(const Форматирование& f)
{
	кткст0 s = f.формат;
	
	бул fillz = false;
	бул wd = true;
	бул left = false;
	цел width = 0;
	цел precision = 6;

	цел flags = FD_SIGN_EXP|FD_SPECIAL|FD_MINUS0;
	кткст0 ид = f.ид;
	if(*ид++ == 'M')
		flags |= FD_CAP_E;
	бул lng = false;
	if(*ид == 'l') {
		lng = true;
		ид++;
	}
	if(*ид == 'E') flags |= FD_EXP|FD_CAP_E;
	if(*ид == 'e') flags |= FD_EXP;
	if(*ид == 'f') flags |= FD_FIX|FD_ZEROS;

	while(*s) {
		if(цифра_ли(*s)) {
			if(wd && *s == '0')
				fillz = true;
			бцел n;
			бул overflow = false;
			s = сканБцел<char, ббайт, бцел, 10>(n, s, overflow);
			if(overflow || !s || n > (wd ? 1000u : 100u))
				return Null;
			(wd ? width : precision) = n;
		}
		else
		switch(*s++) {
		case '-': left = true; break;
		case '+': flags |= FD_SIGN; break;
		case ' ': flags |= FD_SIGN_SPACE; break;
		case ',': flags |= FD_COMMA; wd = false; break;
		case '.': flags &= ~FD_COMMA; wd = false; break;
		case '!': flags |= FD_ZEROS; break;
		case '?': flags &= ~FD_SPECIAL; break;
		case '_': flags &= ~FD_MINUS0; break;
		case '^': flags &= ~FD_SIGN_EXP; break;
		case '&': flags |= FD_MINIMAL_EXP; break;
		case '#': flags |= FD_ZEROS|FD_POINT; break;
		}
	}
	Ткст r = фмтДво(f.арг, precision, flags);
	if(lng) {
		цел q = r.найди('.');
		if(q >= 0)
			r = r.середина(0, q) + GetLanguageInfo(f.язык).decimal_point + r.середина(q + 1);
	}
	if(width > r.дайСчёт()) {
		if(fillz && !left && !пусто_ли(f.арг))
			return цифра_ли(*r) ? Ткст('0', width - r.дайСчёт()) + r
			                   : r.середина(0, 1) + Ткст('0', width - r.дайСчёт()) + r.середина(1);
		return left ? r + Ткст(' ', width - r.дайСчёт()) : Ткст(' ', width - r.дайСчёт()) + r;
	}
	return r;
}

Ткст RealFormatter(const Форматирование& f)
{
	if(пусто_ли(f.арг))
		return Null;
	дво значение = f.арг;
	кткст0 s = f.формат;
	цел digits = 6;
	кткст0 ид = f.ид;
	бул фн = *ид++ != 'v';
	цел flags = 0;
	if(*s == '+') {
		flags |= FD_SIGN;
		s++;
	}
	if(цифра_ли(*s) || *s == '-' && цифра_ли(s[1])) {
		digits = (цел)strtol(s, NULL, 10);
		while(цифра_ли(*++s))
			;
	}
	if(*s == '@') { s++; flags |= FD_NOTHSEPS; }
	if(*s == ',') { s++; flags |= FD_COMMA; }
	if(*s == '!') { s++; flags |= FD_ZEROS; }
	if(*s == '^') {
		if(*++s == '+') {
			flags |= FD_SIGN_EXP;
			s++;
		}
		while(цифра_ли(*++s))
			;
	}
	бул lng = false;
	if(*ид == 'l') {
		lng = true;
		ид++;
	}
	if(*ид == 'e') flags |= FD_EXP;
	else if(*ид == 'f') flags |= FD_FIX;
	if(фн && значение >= 1e-15 && значение <= 1e15)
		flags |= FD_FIX;
	if(lng)
		return GetLanguageInfo(f.язык).фмтДво(значение, digits, flags, 0);
	else
		return фмтДво(значение, digits, flags);
}

Ткст StringFormatter(const Форматирование& f)
{
	const Ткст& s = f.арг;
	if(f.формат.дайСчёт() == 0 && f.ид[0] == 's' && f.ид[1] == 0)
		return s;
	цел len = s.дайСчётСим();
	цел width = len;
	цел precision = len;
	бул lpad = false;
	СиПарсер p(f.формат);
	if(p.сим('-'))
		lpad = true;
	if(p.число_ли())
		width = p.читайЦел();
	if(p.сим('.') && p.число_ли())
		precision = p.читайЦел();
//	if(precision > len)
//		return ШТкст(~s, precision).вТкст();
	Ткст q = s;
	if(precision < len) {
		TrimChar(q, precision);
		len = precision;
	}
	Ткст r;
	if(lpad)
		r.кат(q);
	if(width > len)
		r.кат(' ', width - len);
	if(!lpad)
		r.кат(q);
	return r;
}

static inline
проц sFixPoint(char *s) // We do not want locale to affect decimal point, convert ',' to '.'
{
	while(*s) {
		if(*s == ',')
			*s = '.';
		s++;
	}
}

Ткст FloatFormatter(const Форматирование& f)
{
	дво d = f.арг;
	Ткст fmt = '%' + f.формат + f.ид;
	char h[256];
#ifdef COMPILER_MSC
	цел n = _snprintf(h, 256, fmt, d);
	if(n < 0)
#else
	цел n = snprintf(h, 255, fmt, d);
	if(n >= 254)
#endif
	{
#ifdef COMPILER_MSC
		n = _scprintf(fmt, d);
#endif
		Буфер<char> ah(n + 1);
		sprintf(ah, fmt, d);
		sFixPoint(ah);
		return Ткст(ah, n);
	}
	if(n < 0)
		return Ткст();
	sFixPoint(h);
	return Ткст(h, n);
}

Ткст DateFormatter(const Форматирование& f)
{
	return GetLanguageInfo(f.язык).фмтДата(f.арг);
}

Ткст TimeFormatter(const Форматирование& f)
{
	return GetLanguageInfo(f.язык).фмтВремя(f.арг);
}

Ткст SwitchFormatter(const Форматирование& f)
{
	кткст0 s = f.формат;
	цел i = f.арг;
	цел o = i;
	for(;;) {
		цел n = 0;
		while(цифра_ли(*s))
			n = 10 * n + *s++ - '0';
		if(!*s) return Null;
		if(*s == '%') {
			o = i % макс(n, 1);
			s++;
		}
		else
		if(*s == ',' || *s == ':') {
			if(o == n) {
				while(*s && *s != ':')
					s++;
				if(!*s) return Null;
				++s;
				кткст0 b = s;
				while(*s && *s != ';')
					s++;
				return Ткст(b, s);
			}
			if(*s == ':')
				while(*s && *s != ';')
					s++;
			if(!*s) return Null;
			s++;
		}
		else
			return s;
	}
	return Ткст();
}

Ткст StdFormatFormatter(const Форматирование& f)
{
	Ткст out = стдФормат(f.арг);
	if(!пусто_ли(out))
		return out;
	return f.формат;
}

Ткст MonthFormatter(const Форматирование& f)
{
	return имяМесяца((цел)f.арг - 1, f.язык);
}

Ткст MONTHFormatter(const Форматирование& f)
{
	return взаг(MonthFormatter(f), GetLNGCharset(f.язык));
}

Ткст monthFormatter(const Форматирование& f)
{
	return впроп(MonthFormatter(f), GetLNGCharset(f.язык));
}

Ткст MonFormatter(const Форматирование& f)
{
	return MonName((цел)f.арг - 1, f.язык);
}

Ткст MONFormatter(const Форматирование& f)
{
	return взаг(MonFormatter(f), GetLNGCharset(f.язык));
}

Ткст monFormatter(const Форматирование& f)
{
	return впроп(MonFormatter(f), GetLNGCharset(f.язык));
}

Ткст DayFormatter(const Форматирование& f)
{
	return имяДня((цел)f.арг, f.язык);
}

Ткст DAYFormatter(const Форматирование& f)
{
	return взаг(DayFormatter(f), GetLNGCharset(f.язык));
}

Ткст dayFormatter(const Форматирование& f)
{
	return впроп(DayFormatter(f), GetLNGCharset(f.язык));
}

Ткст DyFormatter(const Форматирование& f)
{
	return DyName((цел)f.арг, f.язык);
}

Ткст DYFormatter(const Форматирование& f)
{
	return взаг(DyFormatter(f), GetLNGCharset(f.язык));
}

Ткст dyFormatter(const Форматирование& f)
{
	return впроп(DyFormatter(f), GetLNGCharset(f.язык));
}

Ткст twFormatter(const Форматирование& f)
{
	цел q = f.арг;
	return спринтф(*f.формат == '0' ? "%02d" : "%d", q ? q % 12 : 12);
}

Ткст NumberFormatter(const Форматирование& f)
{
	цел q = f.арг;
	return какТкст(q);
}

проц IntDoubleRegister(цел тип)
{
	регФорматировщик(тип, "", &NumberFormatter);

	регФорматировщик(тип, "c", &IntFormatter);
	регФорматировщик(тип, "d", &IntFormatter);
	регФорматировщик(тип, "i", &IntFormatter);
	регФорматировщик(тип, "o", &IntFormatter);
	регФорматировщик(тип, "x", &IntFormatter);
	регФорматировщик(тип, "X", &IntFormatter);
	регФорматировщик(тип, "ld", &IntFormatter);
	регФорматировщик(тип, "li", &IntFormatter);
	регФорматировщик(тип, "lo", &IntFormatter);
	регФорматировщик(тип, "lx", &IntFormatter);
	регФорматировщик(тип, "lX", &IntFormatter);

	регФорматировщик(тип, "lld", &Int64Formatter);
	регФорматировщик(тип, "lli", &Int64Formatter);
	регФорматировщик(тип, "llo", &Int64Formatter);
	регФорматировщик(тип, "llx", &Int64Formatter);
	регФорматировщик(тип, "llX", &Int64Formatter);

	регФорматировщик(тип, "e", &FloatFormatter);
	регФорматировщик(тип, "E", &FloatFormatter);
	регФорматировщик(тип, "f", &FloatFormatter);
	регФорматировщик(тип, "g", &FloatFormatter);
	регФорматировщик(тип, "G", &FloatFormatter);

	регФорматировщик(тип, "s", &SwitchFormatter);

	регФорматировщик(тип, "month", &monthFormatter);
	регФорматировщик(тип, "Month", &MonthFormatter);
	регФорматировщик(тип, "MONTH", &MONTHFormatter);
	регФорматировщик(тип, "mon", &monFormatter);
	регФорматировщик(тип, "Mon", &MonFormatter);
	регФорматировщик(тип, "MON", &MONFormatter);
	регФорматировщик(тип, "Day", &DayFormatter);
	регФорматировщик(тип, "DAY", &DAYFormatter);
	регФорматировщик(тип, "day", &dayFormatter);
	регФорматировщик(тип, "Dy", &DyFormatter);
	регФорматировщик(тип, "DY", &DYFormatter);
	регФорматировщик(тип, "dy", &dyFormatter);
	регФорматировщик(тип, "tw", &twFormatter);
}

static проц sRegisterFormatters()
{
	ONCELOCK {
		IntDoubleRegister(BOOL_V);
		IntDoubleRegister(INT_V);
		IntDoubleRegister(INT64_V);
		IntDoubleRegister(DOUBLE_V);

		регФорматировщикТекста("s", &StringFormatter);
		регФорматировщикПусто("", &DateFormatter);
		регФорматировщик(DATE_V, "", &DateFormatter);
		регФорматировщик(TIME_V, "", &TimeFormatter);

		регФорматировщикЧисел("n",  &RealFormatter);
		регФорматировщикЧисел("ne", &RealFormatter);
		регФорматировщикЧисел("nf", &RealFormatter);
		регФорматировщикЧисел("nl", &RealFormatter);
		регФорматировщикЧисел("nle", &RealFormatter);
		регФорматировщикЧисел("nlf", &RealFormatter);
		регФорматировщикЧисел("v",  &RealFormatter);
		регФорматировщикЧисел("ve", &RealFormatter);
		регФорматировщикЧисел("vf", &RealFormatter);
		регФорматировщикЧисел("vl", &RealFormatter);
		регФорматировщикЧисел("vle", &RealFormatter);
		регФорматировщикЧисел("vlf", &RealFormatter);

		// real number formats (n = fixed decimals, v = valid decimals)
		// ne, ve - force exponential notation; nf, vf - force fixed notation; nl, vl - язык-based formatting
		// Options: [+][[-]<digits>][!][^[+]<expdig>]
		// + .. always prepend sign
		// [-]<digits> .. number of decimals to print (negative = left of decimal point, default = 6)
		// ! .. keep insignificant zeros
		// ^ .. exponent options:
		// + .. always prepend sign to exponent
		// <expdig> exponent padding width

		регФорматировщикЧисел("m",  &DoubleFormatter);
		регФорматировщикЧисел("me", &DoubleFormatter);
		регФорматировщикЧисел("mf", &DoubleFormatter);
		регФорматировщикЧисел("ml", &DoubleFormatter);
		регФорматировщикЧисел("mle", &DoubleFormatter);
		регФорматировщикЧисел("mlf", &DoubleFormatter);
		регФорматировщикЧисел("M",  &DoubleFormatter);
		регФорматировщикЧисел("mE", &DoubleFormatter);
		регФорматировщикЧисел("Ml", &DoubleFormatter);
		регФорматировщикЧисел("mlE", &DoubleFormatter);

		регФорматировщикЧисел("a", &IntLowerAlphaFormatter);
		регФорматировщикЧисел("A", &IntUpperAlphaFormatter);
		регФорматировщикЧисел("r", &IntLowerRomanFormatter);
		регФорматировщикЧисел("R", &IntUpperRomanFormatter);

		регФорматировщикЗначений("vt", &StdFormatFormatter);
		регФорматировщикЗначений("", &StdFormatFormatter);
	}
}

ИНИЦБЛОК {
	sRegisterFormatters();
}

Ткст фмт(цел язык, кткст0 s, const Вектор<Значение>& v)
{
	sRegisterFormatters();
	Форматирование f;
	f.язык = язык;
	Ткст result;
	цел pos = 0;
	кткст0 b;
	for(;;) {
		цел n = 0;
		b = s;
		for(;;) {
			while(*s && *s != '%')
				++s;
			result.кат(b, (цел)(s - b));
			if(*s == '\0')
				return result;
			++s;
			if(*s == '%') {
				result.кат('%');
				++s;
			}
			else
				break;
			b = s;
		}
		f.формат.очисть();
		f.ид.очисть();
		b = s;
		цел pad = -1;
		цел padn = 0;
		Ткст nvl_value = Ткст::дайПроц();
		for(;;) {
			if(*s == '$') {
				pos = n - 1;
				b = ++s;
				n = 0;
			}
			else
			if(*s == ':') {
				pos = n - 1;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '*') {
				f.формат.кат(b, (цел)(s - b));
				b = ++s;
				цел i = v[pos++];
				if(*s == ':' || *s == '$' || *s == '<' || *s == '>' || *s == '=')
					n = i;
				else
					f.формат.кат(фмтЦел(i));
			}
			else
			if(*s == '<') {
				padn = n;
				pad = ALIGN_LEFT;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '>') {
				padn = n;
				pad = ALIGN_RIGHT;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '=') {
				padn = n;
				pad = ALIGN_CENTER;
				b = ++s;
				n = 0;
			}
			else
			if(*s == '[') {
				f.формат.кат(b, (цел)(s - b));
				s++;
				b = s;
				while(*s && *s != ']')
					s++;
				f.формат.кат(b, (цел)(s - b));
				if(*s) s++;
				b = s;
				if(!альфа_ли(*s) && *s != '~') break;
			}
			else if(*s == '~') {
				nvl_value = f.формат;
				f.формат = Null;
				b = ++s;
			}
			else
			if(!*s || *s == '`' || альфа_ли(*s))
				break;
			else {
				if(!*s)
					return result + "<unexpected end>";
				if(цифра_ли(*s))
					n = 10 * n + *s - '0';
				else
					n = 0;
				s++;
			}
		}
		f.формат.кат(b, (цел)(s - b));
		b = s;
		while(альфа_ли(*s))
			s++;
		f.ид = Ткст(b, s);
		if(pos < 0 || pos >= v.дайСчёт())
		{
			result << "<invalid pos=" << pos << ">";
			if(*s == '`')
				s++;
			continue;
		}
		f.арг = v[pos++];
		Ткст r;
		if(!nvl_value.проц_ли() && пусто_ли(f.арг))
			r = nvl_value;
		else
		{
			Форматировщик ft = NULL;
#ifdef _ОТЛАДКА
			цел fi = formatmap().найди(FormId(f.ид, f.арг.дайТип()));
			if(fi < 0) fi = formatmap().найди(FormId(f.ид, VALUE_V));
			if(fi >= 0)
				ft = formatmap()[fi];
#else
			for(;;) {
				цел fi = formatmap().найди(FormId(f.ид, f.арг.дайТип()));
				if(fi < 0) fi = formatmap().найди(FormId(f.ид, VALUE_V));
				if(fi >= 0)
				{
					ft = formatmap()[fi];
					break;
				}
				if(f.ид.дайДлину() == 0) break;
				f.ид.обрежь(f.ид.дайДлину() - 1);
				s--;
			}
#endif
			if(ft)
				r = (*ft)(f);
			else
				r << "<N/A '" << f.ид << "' for тип " << (цел)f.арг.дайТип() << ">";
		}
		цел len;
		if(padn < 0 || padn > 1000)
			r << "<invalid padding>";
		else
		switch(pad) {
		case ALIGN_LEFT:
			len = r.дайСчётСим();
			if(len < padn)
				r.кат(' ', padn - len);
			else
				TrimChar(r, padn);
			break;
		case ALIGN_RIGHT:
			len = r.дайСчётСим();
			if(len < padn) {
				Ткст fill(' ', padn - len);
				r = fill + r;
			}
			else
				TrimChar(r, padn);
			break;
		case ALIGN_CENTER:
			len = r.дайСчётСим();
			if(len < padn) {
				цел ll = (padn - len) / 2;
				r = Ткст(' ', ll) + r;
				r.кат(' ', padn - len - ll);
			}
			else
				TrimChar(r, padn);
			break;
		}
		result.кат(r);
		if(*s == '`')
			s++;
	}
}

Ткст фмт(кткст0 s, const Вектор<Значение>& v) { return фмт(GetCurrentLanguage(), s, v); }

Ткст спринтф(кткст0 fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	return вфмт(fmt, argptr);
}

Ткст деФормат(кткст0 text)
{
	ТкстБуф x;
	while(*text) {
		if(*text == '%')
			x.кат('%');
		x.кат(*text++);
	}
	return Ткст(x);
}

}
