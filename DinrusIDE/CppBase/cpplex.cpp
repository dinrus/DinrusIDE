#include "CppBase.h"
#include "Internal.h"

// #define LOGNEXT _DBG_

namespace РНЦП {

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif

#define case_id \
	case '_':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h': \
	case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q': \
	case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z': \
	case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I': \
	case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R': \
	case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case '$'

#define case_nonzero_digit \
	case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9'


const char *_CppKeyword[] = {
#define CPPID(x) #x,
#include "keyword.i"
#undef  CPPID
	NULL
};

const char **CppKeyword() { return _CppKeyword; }


LexSymbolStat::LexSymbolStat() :
  minSymbol(0)
{
}

void LexSymbolStat::переустанов(int minSymbol, int maxSymbol)
{
	ПРОВЕРЬ(minSymbol <= maxSymbol);
	v.очисть();
	this->minSymbol = minSymbol;
	v.устСчёт(maxSymbol - minSymbol + 1, 0);
}

void LexSymbolStat::IncStat(int symbol)
{
	int symbolIndex = symbol - minSymbol;
	if(symbolIndex >= 0 && symbolIndex < v.дайСчёт())
	  v[symbolIndex]++;
}

int  LexSymbolStat::GetStat(int symbol) const
{
	int symbolIndex = symbol - minSymbol;
	return (symbolIndex >= 0 && symbolIndex < v.дайСчёт()) ?
	       v[symbolIndex] :
	       0;
}

int  LexSymbolStat::суммаStat(const Вектор<int> & symbols) const
{
	int sum = 0;
	for(int i = 0; i < symbols.дайСчёт(); i++)
		sum += GetStat(symbols[i]);
	return sum;
}

void LexSymbolStat::Merge(const LexSymbolStat & other)
{
	if(v.дайСчёт() == 0) {
		minSymbol = other.minSymbol;
		v <<= other.v;
		return;
	}
	ПРОВЕРЬ(other.minSymbol == minSymbol && other.v.дайСчёт() == v.дайСчёт());
	for(int i = 0; i < v.дайСчёт(); i++)
		v[i] += other.v[i];
}


Lex::Lex()
:	statsCollected(false)
{
	const char **cppk = CppKeyword();
	for(int i = 0; cppk[i]; i++)
		id.добавь(cppk[i]);
	endkey = id.дайСчёт();
	braceslevel = body = 0;
}

void Lex::иниц(const char *s)
{
	ptr = s;
}

void Lex::StartStatCollection()
{
	symbolStat.переустанов(-200, endkey+256);
	statsCollected = true;
}

const LexSymbolStat& Lex::FinishStatCollection()
{
	statsCollected = false;
	return symbolStat;
}

int Lex::дайСимвол()
{
	if(*ptr == '\0') return t_eof;
	int c = *ptr++;
	if(c == '\\') {
		c = *ptr++;
		switch(c) {
		case 'a': return '\a';
		case 'b': return '\b';
		case 't': return '\t';
		case 'v': return '\v';
		case 'n': return '\n';
		case 'r': return '\r';
		case 'f': return '\f';
		case 'x':
			c = 0;
			if(isxdigit(*ptr)) {
				c = (*ptr >= 'A' ? взаг(*ptr) - 'A' + 10 : *ptr - '0');
				ptr++;
				if(isxdigit(*ptr)) {
					c = 16 * c + (*ptr >= 'A' ? взаг(*ptr) - 'A' + 10 : *ptr - '0');
					ptr++;
				}
			}
			break;
		default:
			if(c >= '0' && c <= '7') {
				c -= '0';
				if(*ptr >= '0' && *ptr <= '7')
					c = 8 * c + *ptr++ - '0';
				if(*ptr >= '0' && *ptr <= '7')
					c = 8 * c + *ptr++ - '0';
			}
		}
	}
	return (byte)c;
}

void Lex::следщ()
{
	grounding = false;
	while((byte)*ptr <= ' ') {
		if(*ptr == '\2')
			grounding = true;
		if(*ptr == '\0') return;
		ptr++;
	}
	pos = ptr;
	int c = (byte)*ptr++;
	if(c == '\0') return;
	switch(c) {
	case_id: {
			const char *b = ptr - 1;
			while(iscid(*ptr))
				ptr++;
			Ткст x(b, ptr);
			int q = id.найдиДобавь(x);
			if(q == tk_rval_ - 256) { // simple hack for old rval macro
				добавьКод('&');
				добавьКод('&');
			}
			else
				добавьКод(q + 256);
			break;
		}
	case ':': добавьКод(сим(':') ? t_dblcolon : ':'); break;
	case '*': AssOp('*', t_mulass); break;
	case '/': AssOp('/', t_divass); break;
	case '%': AssOp('%', t_modass); break;
	case '^': AssOp('^', t_xorass); break;
	case '!': AssOp('!', t_neq); break;
	case '.':
		if(сим('*')) добавьКод(t_dot_asteriks);
		else
		if(*ptr == '.' && ptr[1] == '.') {
			добавьКод(t_elipsis);
			ptr += 2;
		}
		else
			добавьКод('.');
		break;
	case '+':
		if(сим('+')) добавьКод(t_inc);
		else
			AssOp('+', t_addass);
		return;
	case '-':
		if(сим('-')) добавьКод(t_dec);
		else
		if(сим('>'))
			добавьКод(сим('*') ? t_arrow_asteriks : t_arrow);
		else
			AssOp('-', t_subass);
		break;
	case '&':
		if(сим('&'))
			добавьКод(t_and);
		else
			AssOp('&', t_andass);
		break;
	case '|':
		if(сим('|'))
			добавьКод(t_or);
		else
			AssOp('|', t_orass);
		break;
	case '=':
		AssOp('=', t_eq);
		break;
	case '<':
		if(сим('<'))
			AssOp(t_shl, t_shlass);
		else
			AssOp('<', t_le);
		break;
	case '>':
		if(сим('>'))
			AssOp(t_shr, t_shrass);
		else
			AssOp('>', t_ge);
		break;
	case '0': {
			dword w = 0;
			if(сим('x') || сим('X')) {
				for(;;) {
					int d;
					if(*ptr >= '0' && *ptr <= '9')
						d = *ptr - '0';
					else
					if(*ptr >= 'A' && *ptr <= 'F')
						d = *ptr - 'A' + 10;
					else
					if(*ptr >= 'a' && *ptr <= 'f')
						d = *ptr - 'a' + 10;
					else
						break;
					if(w >= 0x8000000u - d) {
						добавьКод(te_integeroverflow);
						return;
					}
					w = w * 16 + d - '0';
					ptr++;
				}
			}
			else
				while(*ptr >= '0' && *ptr <= '7') {
					int d = *ptr++ - '0';
					if(w >= 0x1000000u - d) {
						добавьКод(te_integeroverflow);
						return;
					}
					w = w * 8 + d - '0';
				}
			прекрати& tm = term.добавьХвост();
			tm.code = t_integer;
			tm.ptr = pos;
			tm.number = w;
		}
		break;
	case_nonzero_digit: {
			double w = c - '0';
			bool fp = false;
			while(*ptr >= '0' && *ptr <= '9')
				w = w * 10 + *ptr++ - '0';
			if(*ptr == '.') { //TODO TO BE Completed !!!
				fp = true;
				ptr++;
				double x = 0.1;
				while(*ptr >= '0' && *ptr <= '9') {
					w += x * (*ptr++ - '0');
					x /= 10;
				}
			}
			прекрати& tm = term.добавьХвост();
			if(fp || w < INT_MIN || w > INT_MAX)
				tm.code = t_double;
			else
				tm.code = t_integer;
			tm.ptr = pos;
			tm.number = w;
		}
		break;
	case '\'': {
			прекрати& tm = term.добавьХвост();
			tm.code = t_character;
			tm.ptr = pos;
			tm.text = Ткст(дайСимвол(), 1);
			if(*ptr == '\'')
				ptr++;
			else
				tm.code = te_badcharacter;
		}
		break;
	case '\"': {
			прекрати& tm = term.добавьХвост();
			tm.code = t_string;
			tm.ptr = pos;
			for(;;) {
				while(*ptr != '\"') {
					if((byte)*ptr < ' ' && *ptr != 9) {
						tm.code = te_badstring;
						return;
					}
					tm.text.конкат(дайСимвол());
				}
				ptr++;
				while(*ptr && (byte)*ptr <= ' ') ptr++;
				if(*ptr != '\"') break;
				ptr++;
			}
		}
		break;
	default:
		добавьКод(c);
		return;
	}
}

bool Lex::Prepare(int pos) {
	while(term.дайСчёт() <= pos) {
		if(*ptr == '\0') return false;
		следщ();
	}
#ifdef _ОТЛАДКА
	pp = term[0].ptr;
#endif
	return true;
}

int Lex::Code(int pos)
{
	if(!Prepare(pos)) return t_eof;
	return term[pos].code;
}

bool   Lex::ид_ли(int pos)
{
	return Code(pos) >= endkey + 256;
}

void Lex::выведиОш(const char *e)
{
	WhenError(e);
	throw Parser::Ошибка();
}

Ткст Lex::Ид(int pos)
{
	if(!ид_ли(pos))
		выведиОш("ожидался ид");
	return id[Code(pos) - 256];
}

void Lex::дай(int n)
{
	while(n--) {
		if(term.дайСчёт()) {
			if(body && term.дайГолову().grounding)
				throw Grounding();
			int chr = term.дайГолову().code;
			if(statsCollected)
				symbolStat.IncStat(chr);
			if(chr == '{')
				braceslevel++;
			else
			if(chr == '}')
				braceslevel--;
			term.сбросьГолову();
		}
		if(term.дайСчёт() == 0)
			следщ();
		if(term.дайСчёт() == 0)
			break;
	}
#ifdef LOGNEXT
	Dump(0);
#endif
}

void Lex::Dump(int pos)
{
#ifdef LOGNEXT
	int code = Code(pos);
	switch(code) {
	case t_string: LOG(какТкстСи(устТекст(pos))); break;
	case t_double: LOG(Дво(pos)); break;
	case t_integer: LOG(Цел(pos)); break;
	case t_character: LOG("char " << какТкстСи(Ткст(Chr(pos), 1))); break;
	default:
		if(code < 0)
			LOG(decode(Code(),
				t_dblcolon, "::",
				t_mulass, "*=",
				t_divass, "/=",
				t_modass, "%=",
				t_xorass, "^=",
				t_neq, "!=",
				t_dot_asteriks, ".*",
				t_elipsis, "...",
				t_inc, "++",
				t_addass, "+=",
				t_dec, "--",
				t_arrow_asteriks, "->*",
				t_arrow, "->",
				t_subass, "-=",
				t_and, "&&",
				t_andass, "&=",
				t_or, "||",
				t_orass, "|=",
				t_eq, "==",
				t_shl, "<<",
				t_shlass, "<<=",
				t_le, "<=",
				t_shr, ">>",
				t_shrass, ">>=",
				t_ge, ">=",
				te_integeroverflow, "<integer overflow>",
				te_badcharacter, "<bad char>",
				te_badstring, "<bad string>",
				"???"));
		else
		if(code < 256)
			LOG((char)code);
		else
			LOG(id[code - 256]);
	}
#endif
}

void Lex::SkipToGrounding()
{
	for(;;) {
		if(term.дайСчёт() == 0)
			следщ();
		if(term.дайСчёт() == 0)
			break;
		int chr = term.дайГолову().code;
		if(chr == t_eof)
			return;
		if(term.дайГолову().grounding)
			return;
		if(chr == '{')
			braceslevel++;
		else
		if(chr == '}')
			braceslevel--;
		term.сбросьГолову();
	}
}

int Lex::Цел(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_integer)
		выведиОш("ожидался целочисленный литерал");
	return (int)term[pos].number;
}

double Lex::Дво(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_double)
		выведиОш("ожидался литерал с плавающей запятой");
	return term[pos].number;
}

Ткст Lex::устТекст(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_string)
		выведиОш("ождался строковый литерал");
	return term[pos].text;
}

int Lex::Chr(int pos)
{
	Prepare(pos);
	if(term[pos].code != t_character)
		выведиОш("ожидался символьный литерал");
	return (byte)*term[pos].text;
}

const char *Lex::Поз(int pos)
{
	Prepare(pos);
	return pos < term.дайСчёт() ? term[pos].ptr : ptr;
}

}
