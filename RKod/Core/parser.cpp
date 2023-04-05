#include "Core.h"

namespace РНЦПДинрус {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

void СиПарсер::выведиОш(const char *s) {
	LLOG("СиПарсер::Ошибка: " << s);
	LLOG(~Ткст(term, min((int)strlen((const char *)term), 512)));
	Поз pos = дайПоз();
	Ошибка err(фн + фмт("(%d,%d): ", line, pos.дайКолонку()) + s);
//	err.term = (const char *)term;
	throw err;
}

СиПарсер& СиПарсер::пропустиКомменты(bool b)
{
	skipcomments = b;
	term = wspc;
	пробелы0();
	return *this;
}

СиПарсер& СиПарсер::гнездиКомменты(bool b)
{
	nestcomments = b;
	term = wspc;
	пробелы0();
	return *this;
}

bool СиПарсер::пробелы0() {
	LTIMING("пробелы");
	if(!term)
		return false;
	if((byte)*term > ' ' &&
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
				фн.конкат(*term++);
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
				int count = 1;
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
		if((byte)*term > ' ') break;
		if(*term == '\n') {
			line++;
			lineptr = term + 1;
		}
		term++;
	}
	return true;
}

Ткст СиПарсер::LineInfoComment(const Ткст& file, int line, int column)
{
	return Ткст().конкат() << (char)LINEINFO_ESC << file << '\3'
	                      << line << '\3' << column << (char)LINEINFO_ESC;
}

Ткст СиПарсер::GetLineInfoComment(int tabsize) const
{
	return LineInfoComment(дайИмяф(), дайСтроку(), дайКолонку(tabsize));
}

const char *СиПарсер::ид0_ли(const char *s) const {
	const char *t = term + 1;
	s++;
	while(*s) {
		if(*t != *s)
			return NULL;
		t++;
		s++;
	}
	return IsAlNum(*t) || *t == '_' ? NULL : t;
}

bool СиПарсер::ид0(const char *s) {
	LTIMING("Ид");
	const char *t = ид0_ли(s);
	if(!t)
		return false;
	term = t;
	сделайПробелы();
	return true;
}

void СиПарсер::передайИд(const char *s) {
	LTIMING("передайИд");
	if(!Ид(s))
		выведиОш(Ткст("отсутствует '") + s + "\'");
}

void СиПарсер::передайСим(char c) {
	LTIMING("передайСим");
	if(!char(c))
		выведиОш(Ткст("отсутствует '") + c + "\'");
}

void СиПарсер::передайСим2(char c1, char c2) {
	LTIMING("передайСим2");
	if(!сим2(c1, c2))
		выведиОш(Ткст("отсутствует '") + c1 + c2 + "\'");
}

void СиПарсер::передайСим3(char c1, char c2, char c3) {
	LTIMING("передайСим3");
	if(!сим3(c1, c2, c3))
		выведиОш(Ткст("отсутствует '") + c1 + c2 + c3 + "\'");
}

Ткст СиПарсер::читайИд() {
	if(!ид_ли())
		выведиОш("отсутствует ид");
	Ткст result;
	const char *b = term;
	const char *p = b;
	while(iscid(*p))
		p++;
	term = p;
	сделайПробелы();
	return Ткст(b, (int)(uintptr_t)(p - b));
}

Ткст СиПарсер::ReadIdt() {
	if(!ид_ли())
		выведиОш("отсутствует ид");
	ТкстБуф result;
	int lvl = 0;
	while(IsAlNum(*term) || *term == '_' || *term == '<' || *term == '>' ||
	      *term == ':' || (*term == ',' || *term == ' ' ) && lvl > 0 ) {
		if(*term == '<') lvl++;
		if(*term == '>') lvl--;
		result.конкат(*term++);
	}
	сделайПробелы();
	return Ткст(result);
}

bool СиПарсер::цел_ли() const {
	LTIMING("цел_ли");
	const char *t = term;
	if(*t == '-' || *t == '+') {
		t++;
		while(*t <= ' ')
			t++;
	}
	return цифра_ли(*t);
}

int  СиПарсер::знак()
{
	int sign = 1;
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

int  СиПарсер::читайЦел() {
	LTIMING("читайЦел");
	int n;
	bool overflow = false;
	const char *t = сканЦел<char, byte, dword, int, 10>(n, term, overflow);
	if(!t) выведиОш("отсутствует число");
	if(overflow) выведиОш("число слишком велико");
	term = t;
	сделайПробелы();
	return n;
}

int СиПарсер::читайЦел(int min, int max)
{
	int n = читайЦел();
	if(n < min || n > max)
		выведиОш("число выходит за диапазон");
	return n;
}

int64 СиПарсер::читайЦел64()
{
	LTIMING("читайЦел64");
	int64 n;
	bool overflow = false;
	const char *t = сканЦел<char, byte, uint64, int64, 10>(n, term, overflow);
	if(!t) выведиОш("отсутствует число");
	if(overflow) выведиОш("число слишком велико");
	term = t;
	сделайПробелы();
	return n;
}

int64 СиПарсер::читайЦел64(int64 min, int64 max)
{
	int64 n = читайЦел64();
	if(n < min || n > max)
		выведиОш("число выходит за диапазон");
	return n;
}

bool СиПарсер::число_ли(int base) const
{
    if(цифра_ли(*term)) {
        int q = *term - '0';
        return q >= 0 && q < base;
    }
	int q = взаг(*term) - 'A';
    return q >= 0 && q < base - 10;
}

uint32  СиПарсер::читайЧисло(int base)
{
	LTIMING("читайЧисло");

	uint32 n;
	bool overflow = false;
	const char *t;

	switch(base) {
	case 10:
		t = сканБцел<char, byte, uint32, 10>(n, term, overflow);
		break;
	case 8:
		t = сканБцел<char, byte, uint32, 8>(n, term, overflow);
		break;
	case 2:
		t = сканБцел<char, byte, uint32, 2>(n, term, overflow);
		break;
	case 16:
		t = сканБцел<char, byte, uint32, 16>(n, term, overflow);
		break;
	default:
		uint32 n = 0;
		int q = ctoi(*term);
		if(q < 0 || q >= base)
			выведиОш("отсутсвует число");
		for(;;) {
			int c = ctoi(*term);
			if(c < 0 || c >= base)
				break;
			uint32 n1 = n;
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

uint64  СиПарсер::читайЧисло64(int base)
{
	LTIMING("читайЧисло64");

	uint32 n;
	bool overflow = false;
	const char *t;

	switch(base) {
	case 10:
		t = сканБцел<char, byte, uint32, 10>(n, term, overflow);
		break;
	case 8:
		t = сканБцел<char, byte, uint32, 8>(n, term, overflow);
		break;
	case 2:
		t = сканБцел<char, byte, uint32, 2>(n, term, overflow);
		break;
	case 16:
		t = сканБцел<char, byte, uint32, 16>(n, term, overflow);
		break;
	default:
		uint64 n = 0;
		int q = ctoi(*term);
		if(q < 0 || q >= base)
			выведиОш("отсутсвует число");
		for(;;) {
			int c = ctoi(*term);
			if(c < 0 || c >= base)
				break;
			uint64 n1 = n;
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

bool СиПарсер::дво2_ли() const
{
	const char *t = term;
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

dword СиПарсер::читайГекс()
{
	int hex = 0;
	while(IsXDigit(*++term))
		hex = (hex << 4) + ctoi(*term);
	return hex;
}

bool СиПарсер::читайГекс(dword& hex, int n)
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

Ткст СиПарсер::читай1Ткст(int delim, bool chkend) {
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
			case 'a': result.конкат('\a'); term++; break;
			case 'b': result.конкат('\b'); term++; break;
			case 't': result.конкат('\t'); term++; break;
			case 'v': result.конкат('\v'); term++; break;
			case 'n': result.конкат('\n'); term++; break;
			case 'r': result.конкат('\r'); term++; break;
			case 'f': result.конкат('\f'); term++; break;
			case 'x': {
				int hex = читайГекс();
				result.конкат(hex);
				break;
			}
			case 'u':
				if(uescape) {
					dword hex;
					if(!читайГекс(hex, 4))
						выведиОш("Неполный универсальный символ");
					if(hex >= 0xD800 && hex < 0xDBFF) {
						if(term[0] == '\\' && term[1] == 'u') {
							term++;
							dword hex2;
							if(!читайГекс(hex2, 4))
								выведиОш("Неполный универсальный символ");
							if(hex2 >= 0xDC00 && hex2 <= 0xDFFF) {
								result.конкат(вУтф8(((hex & 0x3ff) << 10) | (hex2 & 0x3ff) + 0x10000));
								break;
							}
						}
						выведиОш("Неправильная суррогатная пара UTF-16");
					}
					else
						result.конкат(вУтф8(hex));
				}
				else
					result.конкат(*term++);
				break;
			case 'U':
				if(uescape) {
					dword hex;
					if(!читайГекс(hex, 8))
						выведиОш("Неполный универсальный символ");
					if(hex > 0x10ffff)
						выведиОш("Универсальный символ вне диапазона Юникода");
					result.конкат(вУтф8(hex));
				}
				else
					result.конкат(*term++);
				break;
			default:
				if(*term >= '0' && *term <= '7') {
					int oct = *term++ - '0';
					if(*term >= '0' && *term <= '7')
						oct = 8 * oct + *term++ - '0';
					if(*term >= '0' && *term <= '7')
						oct = 8 * oct + *term++ - '0';
					result.конкат(oct);
				}
				else
					result.конкат(*term++);
				break;
			}
		}
		else {
			if((byte)*term < ' ' && *term != '\t') {
				if(chkend) {
					выведиОш("Незавершённый ткст");
					return Ткст(result);
				}
				if(*term == '\0')
					return Ткст(result);
			}
			result.конкат(*term++);
		}
	}
	сделайПробелы();
	return Ткст(result);
}

Ткст СиПарсер::читай1Ткст(bool chkend)
{
	return читай1Ткст('\"', chkend);
}

Ткст СиПарсер::читайТкст(int delim, bool chkend) {
	LTIMING("читайТкст");
	Ткст result;
	do
		result.конкат(читай1Ткст(delim, chkend));
	while(сим_ли(delim));
	return result;
}

Ткст СиПарсер::читайТкст(bool chkend)
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

void СиПарсер::пропусти()
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
	p.ptr = term;
	p.wspc = wspc;
	p.lineptr = lineptr;
	return p;
}

int СиПарсер::Поз::дайКолонку(int tabsize) const
{
	int pos = 1;
	for(const char *s = lineptr; s < ptr; s++) {
		if(*s == СиПарсер::LINEINFO_ESC) {
			s++;
			while(s < ptr && *s != СиПарсер::LINEINFO_ESC)
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

int СиПарсер::дайКолонку(int tabsize) const
{
	return дайПоз().дайКолонку(tabsize);
}

void СиПарсер::устПоз(const СиПарсер::Поз& p)
{
	LLOG("устПоз " << p.фн << ":" << p.line);
	line = p.line;
	фн = p.фн;
	term = p.ptr;
	wspc = p.wspc;
	lineptr = p.lineptr;
	if(skipspaces)
		сделайПробелы();
}

СиПарсер::СиПарсер(const char *ptr)
: term(ptr), wspc(ptr), lineptr(ptr)
{
	line = 1;
	skipspaces = skipcomments = true;
	nestcomments = false;
	uescape = true;
	пробелы();
}

СиПарсер::СиПарсер(const char *ptr, const char *фн, int line)
: term(ptr), wspc(ptr), lineptr(ptr), line(line), фн(фн)
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

void СиПарсер::уст(const char *_ptr, const char *_fn, int _line)
{
	term = lineptr = wspc = _ptr;
	фн = _fn;
	line = _line;
	if(skipspaces)
		пробелы();
	LLOG("уст " << фн << ":" << line);
}

void СиПарсер::уст(const char *_ptr)
{
	уст(_ptr, "", 1);
}

inline void NextCStringLine(ТкстБуф& t, const char *linepfx, int& pl)
{
	t << "\"\r\n" << (linepfx ? linepfx : "") << "\"";
	pl = t.дайДлину();
}

inline int HexDigit(int c)
{
	return "0123456789ABCDEF"[c & 15];
}

static inline void sCatHex(ТкстБуф& t, word q)
{
	char h[6];
	h[0] = '\\';
	h[1] = 'u';
	h[2] = HexDigit(q >> 12);
	h[3] = HexDigit(q >> 8);
	h[4] = HexDigit(q >> 4);
	h[5] = HexDigit(q);
	t.конкат(h, 6);
}

Ткст какТкстСи(const char *s, const char *lim, int linemax, const char *linepfx, dword flags)
{
	ТкстБуф t;
	t.конкат('\"');
	int pl = 0;
	bool wasspace = false;
	byte cs = дайДефНабСим();
	bool toutf8 = дайДефНабСим() != CHARSET_UTF8;
	while(s < lim) {
		if(t.дайДлину() - pl > linemax && (!(flags & ASCSTRING_SMART) || wasspace))
			NextCStringLine(t, linepfx, pl);
		wasspace = *s == ' ';
		switch(*s) {
		case '\a': t.конкат("\\a"); break;
		case '\b': t.конкат("\\b"); break;
		case '\f': t.конкат("\\f"); break;
		case '\t': t.конкат("\\t"); break;
		case '\v': t.конкат("\\v"); break;
		case '\r': t.конкат("\\r"); break;
		case '\"': t.конкат("\\\""); break;
		case '\\': t.конкат("\\\\"); break;
		case '\n': t.конкат("\\n"); wasspace = true; break;
		default:
			if(flags & ASCSTRING_JSON) {
				if((byte)*s < 32) {
					sCatHex(t, (byte)*s++);
				}
				else
				if((byte)*s >= 0x7f) {
					if((byte)*s == 0x7f) {
						sCatHex(t, 0x7f);
						s++;
					}
					else {
						const char *s0 = s;
						dword c = toutf8 ? вЮникод((byte)*s++, cs) : достаньУтф8(s, lim);
						if(c < 0x10000)
							t.конкат(s0, s);
						else {
							c -= 0x10000;
							sCatHex(t, wchar(0xD800 + (0x3ff & (c >> 10))));
							sCatHex(t, wchar(0xDC00 + (0x3ff & c)));
						}
					}
				}
				else
					t.конкат(*s++);
				continue; // skip s++
			}
			else {
				if(byte(*s) < 32 || (byte)*s >= 0x7f && (flags & ASCSTRING_OCTALHI) || (byte)*s == 0xff || (byte)*s == 0x7f) {
					char h[4];
					int q = (byte)*s;
					h[0] = '\\';
					h[1] = (3 & (q >> 6)) + '0';
					h[2] = (7 & (q >> 3)) + '0';
					h[3] = (7 & q) + '0';
					t.конкат(h, 4);
				}
				else
					t.конкат(*s);
			}
			break;
		}
		s++;
	}
	t.конкат('\"');
	return Ткст(t);
}

Ткст какТкстСи(const char *s, int linemax, const char *linepfx, dword flags)
{
	return какТкстСи(s, s + strlen(s), linemax, linepfx, flags);
}

Ткст какТкстСи(const Ткст& s, int linemax, const char *linepfx, dword flags)
{
	return какТкстСи(s, s.стоп(), linemax, linepfx, flags);
}

}
