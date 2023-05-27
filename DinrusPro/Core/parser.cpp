#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

проц СиПарсер::выведиОш(кткст0 s) {
	LLOG("СиПарсер::Ошибка: " << s);
	LLOG(~Ткст(term, мин((цел)strlen((const char *)term), 512)));
	Поз pos = дайПоз();
	Ошибка err(фн + фмт("(%d,%d): ", line, pos.дайКолонку()) + s);
//	err.term = (const char *)term;
	throw err;
}

СиПарсер& СиПарсер::пропустиКомменты(бул b)
{
	skipcomments = b;
	term = wspc;
	пробелы0();
	return *this;
}

СиПарсер& СиПарсер::гнездиКомменты(бул b)
{
	nestcomments = b;
	term = wspc;
	пробелы0();
	return *this;
}

бул СиПарсер::пробелы0() {
	LTIMING("пробелы");
	if(!term)
		return false;
	if((ббайт)*term > ' ' &&
	   !(term[0] == '/' && term[1] == '/') &&
	   !(term[0] == '/' && term[1] == '*'))
		return false;
	for(;;) {
		if(*term == LINEINFO_ESC) {
			term++;
			фн.очисть();
			while(*term) {
				if(*term == LINEINFO_ESC) {
					++term;
					break;
				}
				if(*term == '\3') {
					line = atoi(++term);
					while(*term) {
						if(*term == LINEINFO_ESC) {
							++term;
							break;
						}
						term++;
					}
					break;
				}
				фн.кат(*term++);
			}
			continue;
		}
		else
		if(term[0] == '/' && term[1] == '/' && skipcomments) {
			term += 2;
			while(*term && *term != '\n')
				term++;
		}
		else
		if(term[0] == '/' && term[1] == '*' && skipcomments) {
			if(nestcomments) {
				цел count = 1;
				term += 2;
				while(*term) {
					if(term[0] == '*' && term[1] == '/') {
						term += 2;
						count--;
						if (count == 0)
							break;
					}
					else if(term[0] == '/' && term[1] == '*')
						count++;
					
					if(*term++ == '\n') {
						line++;
						lineptr = term;
					}
				}
			}
			else {
				term += 2;
				while(*term) {
					if(term[0] == '*' && term[1] == '/') {
						term += 2;
						break;
					}
					if(*term++ == '\n') {
						line++;
						lineptr = term;
					}
				}
			}
		}
		if(!*term) break;
		if((ббайт)*term > ' ') break;
		if(*term == '\n') {
			line++;
			lineptr = term + 1;
		}
		term++;
	}
	return true;
}

Ткст СиПарсер::LineInfoComment(const Ткст& file, цел line, цел column)
{
	return Ткст().кат() << (char)LINEINFO_ESC << file << '\3'
	                      << line << '\3' << column << (char)LINEINFO_ESC;
}

Ткст СиПарсер::GetLineInfoComment(цел tabsize) const
{
	return LineInfoComment(дайИмяф(), дайСтроку(), дайКолонку(tabsize));
}

кткст0 СиПарсер::ид0_ли(кткст0 s) const {
	кткст0 t = term + 1;
	s++;
	while(*s) {
		if(*t != *s)
			return NULL;
		t++;
		s++;
	}
	return IsAlNum(*t) || *t == '_' ? NULL : t;
}

бул СиПарсер::ид0(кткст0 s) {
	LTIMING("Ид");
	кткст0 t = ид0_ли(s);
	if(!t)
		return false;
	term = t;
	сделайПробелы();
	return true;
}

проц СиПарсер::передайИд(кткст0 s) {
	LTIMING("передайИд");
	if(!Ид(s))
		выведиОш(Ткст("отсутствует '") + s + "\'");
}

проц СиПарсер::передайСим(char c) {
	LTIMING("передайСим");
	if(!char(c))
		выведиОш(Ткст("отсутствует '") + c + "\'");
}

проц СиПарсер::передайСим2(char c1, char c2) {
	LTIMING("передайСим2");
	if(!сим2(c1, c2))
		выведиОш(Ткст("отсутствует '") + c1 + c2 + "\'");
}

проц СиПарсер::передайСим3(char c1, char c2, char c3) {
	LTIMING("передайСим3");
	if(!сим3(c1, c2, c3))
		выведиОш(Ткст("отсутствует '") + c1 + c2 + c3 + "\'");
}

Ткст СиПарсер::читайИд() {
	if(!ид_ли())
		выведиОш("отсутствует ид");
	Ткст result;
	кткст0 b = term;
	кткст0 p = b;
	while(iscid(*p))
		p++;
	term = p;
	сделайПробелы();
	return Ткст(b, (цел)(uintptr_t)(p - b));
}

Ткст СиПарсер::ReadIdt() {
	if(!ид_ли())
		выведиОш("отсутствует ид");
	ТкстБуф result;
	цел lvl = 0;
	while(IsAlNum(*term) || *term == '_' || *term == '<' || *term == '>' ||
	      *term == ':' || (*term == ',' || *term == ' ' ) && lvl > 0 ) {
		if(*term == '<') lvl++;
		if(*term == '>') lvl--;
		result.кат(*term++);
	}
	сделайПробелы();
	return Ткст(result);
}

бул СиПарсер::цел_ли() const {
	LTIMING("цел_ли");
	кткст0 t = term;
	if(*t == '-' || *t == '+') {
		t++;
		while(*t <= ' ')
			t++;
	}
	return цифра_ли(*t);
}

цел  СиПарсер::знак()
{
	цел sign = 1;
	if(*term == '-') {
		sign = -1;
		term++;
	}
	else
	if(*term == '+')
		term++;
	пробелы();
	return sign;
}

цел  СиПарсер::читайЦел() {
	LTIMING("читайЦел");
	цел n;
	бул overflow = false;
	кткст0 t = сканЦел<char, ббайт, бцел, цел, 10>(n, term, overflow);
	if(!t) выведиОш("отсутствует число");
	if(overflow) выведиОш("число слишком велико");
	term = t;
	сделайПробелы();
	return n;
}

цел СиПарсер::читайЦел(цел мин, цел макс)
{
	цел n = читайЦел();
	if(n < мин || n > макс)
		выведиОш("число выходит за диапазон");
	return n;
}

дол СиПарсер::читайЦел64()
{
	LTIMING("читайЦел64");
	дол n;
	бул overflow = false;
	кткст0 t = сканЦел<char, ббайт, бдол, дол, 10>(n, term, overflow);
	if(!t) выведиОш("отсутствует число");
	if(overflow) выведиОш("число слишком велико");
	term = t;
	сделайПробелы();
	return n;
}

дол СиПарсер::читайЦел64(дол мин, дол макс)
{
	дол n = читайЦел64();
	if(n < мин || n > макс)
		выведиОш("число выходит за диапазон");
	return n;
}

бул СиПарсер::число_ли(цел base) const
{
    if(цифра_ли(*term)) {
        цел q = *term - '0';
        return q >= 0 && q < base;
    }
	цел q = взаг(*term) - 'A';
    return q >= 0 && q < base - 10;
}

бцел  СиПарсер::читайЧисло(цел base)
{
	LTIMING("читайЧисло");

	бцел n;
	бул overflow = false;
	кткст0 t;

	switch(base) {
	case 10:
		t = сканБцел<char, ббайт, бцел, 10>(n, term, overflow);
		break;
	case 8:
		t = сканБцел<char, ббайт, бцел, 8>(n, term, overflow);
		break;
	case 2:
		t = сканБцел<char, ббайт, бцел, 2>(n, term, overflow);
		break;
	case 16:
		t = сканБцел<char, ббайт, бцел, 16>(n, term, overflow);
		break;
	default:
		бцел n = 0;
		цел q = ctoi(*term);
		if(q < 0 || q >= base)
			выведиОш("отсутсвует число");
		for(;;) {
			цел c = ctoi(*term);
			if(c < 0 || c >= base)
				break;
			бцел n1 = n;
			n = base * n + c;
			if(n1 > n)
				выведиОш("число слишком велико");
			term++;
		}
		сделайПробелы();
		return n;
	}
	if(!t) выведиОш("отсутсвует число");
	if(overflow) выведиОш("число слишком велико");
	term = t;
	сделайПробелы();
	return n;
}

бдол  СиПарсер::читайЧисло64(цел base)
{
	LTIMING("читайЧисло64");

	бцел n;
	бул overflow = false;
	кткст0 t;

	switch(base) {
	case 10:
		t = сканБцел<char, ббайт, бцел, 10>(n, term, overflow);
		break;
	case 8:
		t = сканБцел<char, ббайт, бцел, 8>(n, term, overflow);
		break;
	case 2:
		t = сканБцел<char, ббайт, бцел, 2>(n, term, overflow);
		break;
	case 16:
		t = сканБцел<char, ббайт, бцел, 16>(n, term, overflow);
		break;
	default:
		бдол n = 0;
		цел q = ctoi(*term);
		if(q < 0 || q >= base)
			выведиОш("отсутсвует число");
		for(;;) {
			цел c = ctoi(*term);
			if(c < 0 || c >= base)
				break;
			бдол n1 = n;
			n = base * n + c;
			if(n1 > n)
				выведиОш("число слишком велико");
			term++;
		}
		сделайПробелы();
		return n;
	}
	if(!t) выведиОш("отсутсвует число");
	if(overflow) выведиОш("число слишком велико");
	term = t;
	сделайПробелы();
	return n;
}

бул СиПарсер::дво2_ли() const
{
	кткст0 t = term;
	if(*t == '-' || *t == '+') {
		t++;
		while(*t <= ' ')
			t++;
	}
	if(*t == '.') {
		t++;
		while(*t <= ' ')
			t++;
	}
	return цифра_ли(*t);
}

бцел СиПарсер::читайГекс()
{
	цел hex = 0;
	while(IsXDigit(*++term))
		hex = (hex << 4) + ctoi(*term);
	return hex;
}

бул СиПарсер::читайГекс(бцел& hex, цел n)
{
	hex = 0;
	while(n--) {
		if(!IsXDigit(*++term))
			return false;
		hex = (hex << 4) + ctoi(*term);
	}
	term++;
	return true;
}

Ткст СиПарсер::читай1Ткст(цел delim, бул chkend) {
	if(!сим_ли(delim))
		выведиОш("отсутствует ткст");
	term++;
	ТкстБуф result;
	for(;;) {
		if(*term == delim) {
			term++;
			сделайПробелы();
			return Ткст(result);
		}
		else
		if(*term == '\\') {
			switch(*++term) {
			case 'a': result.кат('\a'); term++; break;
			case 'b': result.кат('\b'); term++; break;
			case 't': result.кат('\t'); term++; break;
			case 'v': result.кат('\v'); term++; break;
			case 'n': result.кат('\n'); term++; break;
			case 'r': result.кат('\r'); term++; break;
			case 'f': result.кат('\f'); term++; break;
			case 'x': {
				цел hex = читайГекс();
				result.кат(hex);
				break;
			}
			case 'u':
				if(uescape) {
					бцел hex;
					if(!читайГекс(hex, 4))
						выведиОш("Неполный универсальный символ");
					if(hex >= 0xD800 && hex < 0xDBFF) {
						if(term[0] == '\\' && term[1] == 'u') {
							term++;
							бцел hex2;
							if(!читайГекс(hex2, 4))
								выведиОш("Неполный универсальный символ");
							if(hex2 >= 0xDC00 && hex2 <= 0xDFFF) {
								result.кат(вУтф8(((hex & 0x3ff) << 10) | (hex2 & 0x3ff) + 0x10000));
								break;
							}
						}
						выведиОш("Неправильная суррогатная пара UTF-16");
					}
					else
						result.кат(вУтф8(hex));
				}
				else
					result.кат(*term++);
				break;
			case 'U':
				if(uescape) {
					бцел hex;
					if(!читайГекс(hex, 8))
						выведиОш("Неполный универсальный символ");
					if(hex > 0x10ffff)
						выведиОш("Универсальный символ вне диапазона Юникода");
					result.кат(вУтф8(hex));
				}
				else
					result.кат(*term++);
				break;
			default:
				if(*term >= '0' && *term <= '7') {
					цел oct = *term++ - '0';
					if(*term >= '0' && *term <= '7')
						oct = 8 * oct + *term++ - '0';
					if(*term >= '0' && *term <= '7')
						oct = 8 * oct + *term++ - '0';
					result.кат(oct);
				}
				else
					result.кат(*term++);
				break;
			}
		}
		else {
			if((ббайт)*term < ' ' && *term != '\t') {
				if(chkend) {
					выведиОш("Незавершённый ткст");
					return Ткст(result);
				}
				if(*term == '\0')
					return Ткст(result);
			}
			result.кат(*term++);
		}
	}
	сделайПробелы();
	return Ткст(result);
}

Ткст СиПарсер::читай1Ткст(бул chkend)
{
	return читай1Ткст('\"', chkend);
}

Ткст СиПарсер::читайТкст(цел delim, бул chkend) {
	LTIMING("читайТкст");
	Ткст result;
	do
		result.кат(читай1Ткст(delim, chkend));
	while(сим_ли(delim));
	return result;
}

Ткст СиПарсер::читайТкст(бул chkend)
{
	return читайТкст('\"', chkend);
}

char СиПарсер::дайСим()
{
	char c = *term++;
	if(c == '\n') {
		line++;
		lineptr = term;
	}
	return c;
}

проц СиПарсер::пропусти()
{
	LTIMING("пропусти");
	if(ид_ли())
		while(iscid(*term))
			term++;
	else
	if(число_ли())
		while(цифра_ли(*term))
			term++;
	else
	if(ткст_ли())
		читайТкст();
	else
	if(сим_ли('\''))
		читайТкст('\'', false);
	else
	if(*term) {
		if(*term == '\n') {
			line++;
			lineptr = term + 1;
		}
		term++;
	}
	сделайПробелы();
}

СиПарсер::Поз СиПарсер::дайПоз() const
{
	Поз p;
	p.line = line;
	p.фн = фн;
	p.укз = term;
	p.wspc = wspc;
	p.lineptr = lineptr;
	return p;
}

цел СиПарсер::Поз::дайКолонку(цел tabsize) const
{
	цел pos = 1;
	for(кткст0 s = lineptr; s < укз; s++) {
		if(*s == СиПарсер::LINEINFO_ESC) {
			s++;
			while(s < укз && *s != СиПарсер::LINEINFO_ESC)
				if(*s++ == '\3')
					pos = atoi(s);
		}
		else
		if(*s == '\t')
			pos = (pos + tabsize - 1) / tabsize * tabsize + 1;
		else
			pos++;
	}
	return pos;
}

цел СиПарсер::дайКолонку(цел tabsize) const
{
	return дайПоз().дайКолонку(tabsize);
}

проц СиПарсер::устПоз(const СиПарсер::Поз& p)
{
	LLOG("устПоз " << p.фн << ":" << p.line);
	line = p.line;
	фн = p.фн;
	term = p.укз;
	wspc = p.wspc;
	lineptr = p.lineptr;
	if(skipspaces)
		сделайПробелы();
}

СиПарсер::СиПарсер(кткст0 укз)
: term(укз), wspc(укз), lineptr(укз)
{
	line = 1;
	skipspaces = skipcomments = true;
	nestcomments = false;
	uescape = true;
	пробелы();
}

СиПарсер::СиПарсер(кткст0 укз, кткст0 фн, цел line)
: term(укз), wspc(укз), lineptr(укз), line(line), фн(фн)
{
	skipspaces = skipcomments = true;
	nestcomments = false;
	uescape = true;
	пробелы();
}

СиПарсер::СиПарсер()
{
	term = lineptr = wspc = NULL;
	line = 0;
	skipspaces = skipcomments = true;
	nestcomments = false;
	uescape = true;
}

проц СиПарсер::уст(кткст0 _ptr, кткст0 _fn, цел _line)
{
	term = lineptr = wspc = _ptr;
	фн = _fn;
	line = _line;
	if(skipspaces)
		пробелы();
	LLOG("уст " << фн << ":" << line);
}

проц СиПарсер::уст(кткст0 _ptr)
{
	уст(_ptr, "", 1);
}

inline проц NextCStringLine(ТкстБуф& t, кткст0 linepfx, цел& pl)
{
	t << "\"\r\n" << (linepfx ? linepfx : "") << "\"";
	pl = t.дайДлину();
}

inline цел HexDigit(цел c)
{
	return "0123456789ABCDEF"[c & 15];
}

static inline проц sCatHex(ТкстБуф& t, бкрат q)
{
	char h[6];
	h[0] = '\\';
	h[1] = 'u';
	h[2] = HexDigit(q >> 12);
	h[3] = HexDigit(q >> 8);
	h[4] = HexDigit(q >> 4);
	h[5] = HexDigit(q);
	t.кат(h, 6);
}

Ткст какТкстСи(кткст0 s, кткст0 lim, цел linemax, кткст0 linepfx, бцел flags)
{
	ТкстБуф t;
	t.кат('\"');
	цел pl = 0;
	бул wasspace = false;
	ббайт cs = дайДефНабСим();
	бул toutf8 = дайДефНабСим() != CHARSET_UTF8;
	while(s < lim) {
		if(t.дайДлину() - pl > linemax && (!(flags & ASCSTRING_SMART) || wasspace))
			NextCStringLine(t, linepfx, pl);
		wasspace = *s == ' ';
		switch(*s) {
		case '\a': t.кат("\\a"); break;
		case '\b': t.кат("\\b"); break;
		case '\f': t.кат("\\f"); break;
		case '\t': t.кат("\\t"); break;
		case '\v': t.кат("\\v"); break;
		case '\r': t.кат("\\r"); break;
		case '\"': t.кат("\\\""); break;
		case '\\': t.кат("\\\\"); break;
		case '\n': t.кат("\\n"); wasspace = true; break;
		default:
			if(flags & ASCSTRING_JSON) {
				if((ббайт)*s < 32) {
					sCatHex(t, (ббайт)*s++);
				}
				else
				if((ббайт)*s >= 0x7f) {
					if((ббайт)*s == 0x7f) {
						sCatHex(t, 0x7f);
						s++;
					}
					else {
						кткст0 s0 = s;
						бцел c = toutf8 ? вЮникод((ббайт)*s++, cs) : достаньУтф8(s, lim);
						if(c < 0x10000)
							t.кат(s0, s);
						else {
							c -= 0x10000;
							sCatHex(t, шим(0xD800 + (0x3ff & (c >> 10))));
							sCatHex(t, шим(0xDC00 + (0x3ff & c)));
						}
					}
				}
				else
					t.кат(*s++);
				continue; // skip s++
			}
			else {
				if(ббайт(*s) < 32 || (ббайт)*s >= 0x7f && (flags & ASCSTRING_OCTALHI) || (ббайт)*s == 0xff || (ббайт)*s == 0x7f) {
					char h[4];
					цел q = (ббайт)*s;
					h[0] = '\\';
					h[1] = (3 & (q >> 6)) + '0';
					h[2] = (7 & (q >> 3)) + '0';
					h[3] = (7 & q) + '0';
					t.кат(h, 4);
				}
				else
					t.кат(*s);
			}
			break;
		}
		s++;
	}
	t.кат('\"');
	return Ткст(t);
}

Ткст какТкстСи(кткст0 s, цел linemax, кткст0 linepfx, бцел flags)
{
	return какТкстСи(s, s + strlen(s), linemax, linepfx, flags);
}

Ткст какТкстСи(const Ткст& s, цел linemax, кткст0 linepfx, бцел flags)
{
	return какТкстСи(s, s.стоп(), linemax, linepfx, flags);
}

}
