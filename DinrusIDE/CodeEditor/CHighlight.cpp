#include "CodeEditor.h"

namespace РНЦП {

#define LTIMING(x)  // RTIMING(x)

bool cmps(const wchar *q, const char *s, int& n) {
	const wchar *t = q;
	while(*q)
		if(*q++ != *s++)
			return false;
	n += int(q - t);
	return *q == *s;
}

bool IsUpperString(const char *q)
{
	while(*q)
	{
		if(*q != '_' && (*q < '0' || *q > '9') && !проп_ли(*q))
			return false;
		q++;
    }
	return true;
}

Цвет CSyntax::BlockColor(int level)
{
	if(hilite_scope == 1)
		return  GetHlStyle(level & 1 ? PAPER_BLOCK1 : PAPER_NORMAL).color;
	if(hilite_scope == 2) {
		int q = level % 5;
		return  GetHlStyle(q ? PAPER_BLOCK1 + q - 1 : PAPER_NORMAL).color;
	}
	return GetHlStyle(PAPER_NORMAL).color;
}

void CSyntax::Bracket(int64 pos, HighlightOutput& hls, РедакторКода *editor) // TODO:SYNTAX: Cleanup passing bracket info
{
	if(!editor)
		return;
	if(editor->IsCursorBracket(pos)) {
		HlStyle& h = hl_style[PAPER_BRACKET0];
		hls.SetPaper(hls.pos, 1, h.color);
		hls.устШрифт(hls.pos, 1, h);
	}
	if(editor->IsMatchingBracket(pos)) {
		HlStyle& h = hl_style[PAPER_BRACKET];
		hls.SetPaper(hls.pos, 1, h.color);
		hls.устШрифт(hls.pos, 1, h);
	}
}

const wchar *HighlightNumber(HighlightOutput& hls, const wchar *p, bool ts, bool octal, bool css)
{
	int c = octal ? HighlightSetup::INK_CONST_OCT : HighlightSetup::INK_CONST_INT;
	const wchar *t = p;
	while(цифра_ли(*p)) p++;
	int fixdigits = int(p - t);
	bool u = false;
	if(*p == '.') {
		c = HighlightSetup::INK_CONST_FLOAT;
		p++;
	}
	while(цифра_ли(*p)) p++;
	if(*p == 'e' || *p == 'E') {
		c = HighlightSetup::INK_CONST_FLOAT;
		p++;
		p += *p == '-' || *p == '+';
		while(цифра_ли(*p)) p++;
		if(*p == 'f' || *p == 'F')
			p++;
	}
	else
	if(*p == 'u' && c != HighlightSetup::INK_CONST_FLOAT) {
		u = true;
		p++;
	}
	if(c == HighlightSetup::INK_CONST_OCT && p - t == (u ? 2 : 1))
		c = HighlightSetup::INK_CONST_INT;
	int n = int(p - t);
	for(int i = 0; i < n; i++) {
		if(t[i] == 'e')
			ts = false;
		hls.помести(HighlightSetup::hl_style[c],
		        c == HighlightSetup::INK_CONST_OCT || (fixdigits < 5 && n - fixdigits < 5)
		             || i == fixdigits || !ts ? 0 :
		        i < fixdigits ? decode((fixdigits - i) % 3, 1, СтрокРедакт::SHIFT_L, 0, СтрокРедакт::SHIFT_R, 0) :
		                        decode((i - fixdigits) % 3, 1, СтрокРедакт::SHIFT_R, 0, СтрокРедакт::SHIFT_L, 0));
	}
	return p;
}

const wchar *HighlightHexBin(HighlightOutput& hls, const wchar *p, int plen, bool thousands_separator)
{
	hls.помести(plen, HighlightSetup::hl_style[HighlightSetup::INK_CONST_HEX]);
	p += plen;
	const wchar *t = p;
	while(IsXDigit(*p))
		p++;
	int n = int(p - t);
	for(int i = 0; i < n; i++) {
		hls.помести(HighlightSetup::hl_style[HighlightSetup::INK_CONST_HEX],
		        thousands_separator && ((n - i) & 1) ? СтрокРедакт::SHIFT_L : 0);
	}
	return p;
}

const wchar *CSyntax::DoComment(HighlightOutput& hls, const wchar *p, const wchar *e)
{
	ШТкст w;
	for(const wchar *s = p; s < e && буква_ли(*s); s++)
		w.конкат(взаг(*s));
	int n = w.дайСчёт();
	word flags = 0;
	if(n) {
		if(comments_lang && !SpellWord(w, comments_lang))
			flags = СтрокРедакт::SPELLERROR;
	}
	else
		for(const wchar *s = p; s < e && !буква_ли(*s); s++)
			n++;
	hls.SetFlags(n, flags);
	static ШТкст todo = "TODO";
	static ШТкст fixme = "FIXME";
	if(w.дайСчёт() >= 4 && w.дайСчёт() <= 5 && findarg(w, todo, fixme) >= 0)
		hls.помести(n, hl_style[INK_COMMENT_WORD], hl_style[PAPER_COMMENT_WORD]);
	else
		hls.помести(n, hl_style[INK_COMMENT]);
	return p + n;
}

bool CSyntax::RawString(const wchar *p, int& n) {
	if(highlight != HIGHLIGHT_CPP)
		return false;
	const wchar *s = p;
	if(*s++ != 'R')
		return false;
	while(*s == ' ' || *s == '\t')
		s++;
	if(*s++ != '\"')
		return false;
	ШТкст rs;
	while(*s != '(') {
		if(*s == '\0')
			return false;
		rs.конкат(*s++);
	}
	raw_string = ")";
	raw_string.конкат(rs);
	raw_string.конкат('\"');
	n = int(s + 1 - p);
	return true;
};

void CSyntax::Highlight(const wchar *ltext, const wchar *e, HighlightOutput& hls, РедакторКода *editor, int line, int64 pos)
{
	ONCELOCK {
		InitKeywords();
	}
	
	bool include = false;
	
	int tabsize = editor ? editor->GetTabSize() : 4;
	
	LTIMING("HighlightLine");
	if(highlight < 0 || highlight >= keyword.дайСчёт())
		return;
	CSyntax next;
	next.уст(дай());
	next.ScanSyntax(ltext, e, line + 1, tabsize);
	bool macro = next.macro != MACRO_OFF;
	
	int linelen = int(e - ltext);
	const wchar *p = ltext;
	
	for(const wchar *ms = p; ms < e; ms++)
		if(*ms != ' ' && *ms != '\t') {
			macro = macro || *ms == '#';
			break;
		}
	
	Grounding(p, e);
	if(highlight == HIGHLIGHT_CALC) {
		if(editor && line == editor->дайСчётСтрок() - 1 || *p == '$')
			hls.SetPaper(0, linelen + 1, hl_style[PAPER_BLOCK1].color);
	}
	else
		hls.SetPaper(0, linelen + 1,
		             macro ? hl_style[PAPER_MACRO].color : hl_style[PAPER_NORMAL].color);
	int block_level = bid.дайСчёт() - 1;
	Ткст cppid;
	if(!comment && highlight != HIGHLIGHT_CALC) {
		if(!macro) {
			int i = 0, bid = 0, pos = 0;
			while(bid < this->bid.дайСчёт() - 1
			&& (i >= linelen || p[i] == ' ' || p[i] == '\t')) {
				hls.SetPaper(i, 1, BlockColor(bid));
				if(i < linelen && p[i] == '\t' || ++pos >= tabsize) {
					pos = 0;
					bid++;
				}
				i++;
			}
			hls.SetPaper(i, 1 + max(0, linelen - i), BlockColor(this->bid.дайСчёт() - 1));
		}
		while(*p == ' ' || *p == '\t') {
			p++;
			hls.помести(hl_style[INK_NORMAL]);
		}
		if(*p == '#' && findarg(highlight, HIGHLIGHT_CPP, HIGHLIGHT_CS) >= 0) {
			static Индекс<Ткст> macro;
			ONCELOCK {
				static const char *pd[] = {
					"include", "define", "Ошибка", "if", "elif", "else", "endif",
					"ifdef", "ifndef", "line", "undef", "pragma",
					// CLR
					"using"
				};
				for(int i = 0; i < __countof(pd); i++)
					macro.добавь(pd[i]);
			}
			const wchar *q = p + 1;
			while(*q == ' ' || *q == '\t')
				q++;
			ТкстБуф id;
			while(IsAlpha(*q))
				id.конкат(*q++);
			cppid = id;
			int mq = macro.найди(cppid);
			hls.помести(mq < 0 ? 1 : int(q - p), hl_style[INK_MACRO]);
			if(mq == 0)
				include = true;
			p = q;
		}
	}
	int lindent = int(p - ltext);
	int lbrace = -1;
	int lbclose = -1;
	Цвет lbcolor = Null;
	bool is_comment = false;
	while(p < e) {
		int  raw_n = 0;
		dword pair = MAKELONG(p[0], p[1]);
		if(pair == MAKELONG('/', '*') && highlight != HIGHLIGHT_JSON || comment) {
			if(!comment) {
				hls.помести(2, hl_style[INK_COMMENT]);
				p += 2;
			}
			for(const wchar *ce = p; ce < e - 1; ce++)
				if(ce[0] == '*' && ce[1] == '/') {
					while(p < ce)
						p = DoComment(hls, p, ce);
					hls.помести(2, hl_style[INK_COMMENT]);
					p += 2;
					comment = false;
					goto comment_ended;
				}
			while(p < e)
				p = DoComment(hls, p, e);
			comment = is_comment = true;
			break;
		comment_ended:;
		}
		else
		if(raw_string.дайСчёт() || RawString(p, raw_n)) {
			hls.помести(raw_n, hl_style[INK_CONST_STRING]);
			p += raw_n;
			const wchar *b = p;
			while(p < e) {
				const wchar *s = p;
				const wchar *r = raw_string;
				while(*s && *r) {
					if(*s != *r)
						break;
					s++;
					r++;
				}
				if(*r == '\0') {
					hls.помести(int(p - b), hl_style[INK_RAW_STRING]);
					hls.помести(int(s - p), hl_style[INK_CONST_STRING]);
					b = p = s;
					raw_string.очисть();
					break;
				}
				p++;
			}
			if(p != b)
				hls.помести(int(p - b), hl_style[INK_RAW_STRING]);
		}
		else
		if(linecomment && linecont || pair == MAKELONG('/', '/') &&
		   highlight != HIGHLIGHT_CSS && highlight != HIGHLIGHT_JSON ||
		   highlight == HIGHLIGHT_PHP && *p == '#') {
			while(p < e)
				p = DoComment(hls, p, e);
			is_comment = true;
			break;
		}
		else
		if((*p == '\"' || *p == '\'') || linecont && string)
			p = hls.CString(p);
		else
		if(*p == '(') {
			brk.добавь(')');
			Bracket(int(p - ltext) + pos, hls, editor);
			hls.помести(hl_style[INK_PAR0 + max(pl++, 0) % 4]);
			p++;
		}
		else
		if(*p == '{') {
			brk.добавь(was_namespace ? 0 : '}');
			Bracket(int(p - ltext) + pos, hls, editor);
			hls.помести(hl_style[INK_PAR0 + max(cl, 0) % 4]);
			if(was_namespace)
				was_namespace = false;
			else {
				++block_level;
				++cl;
			}
			if(hls.pos < linelen)
				hls.SetPaper(hls.pos, linelen - hls.pos + 1, BlockColor(block_level));
			p++;
		}
		else
		if(*p == '[') {
			brk.добавь(']');
			Bracket(int(p - ltext) + pos, hls, editor);
			hls.помести(hl_style[INK_PAR0 + max(bl++, 0) % 4]);
			p++;
		}
		else
		if(*p == ')' || *p == '}' || *p == ']') {
			int bc = brk.дайСчёт() ? brk.вынь() : 0;
			if(*p == '}' && hilite_scope && block_level > 0 && bc)
				hls.SetPaper(hls.pos, linelen + 1 - hls.pos, BlockColor(--block_level));
			Bracket(int(p - ltext) + pos, hls, editor);
			int& l = *p == ')' ? pl : *p == '}' ? cl : bl;
			if(bc && (bc != *p || l <= 0) || bc == 0 && *p != '}') {
				hls.помести(p == ltext || ignore_errors ? hl_style[INK_PAR0] : hl_style[INK_ERROR]);
				brk.очисть();
				cl = bl = pl = 0;
			}
			else {
				if(bc) --l;
				hls.помести(1, hl_style[INK_PAR0 + l % 4]);
			}
			p++;
		}
		else
		if(highlight == HIGHLIGHT_CSS ? *p == '#' : findarg(pair, MAKELONG('0', 'x'), MAKELONG('0', 'X'), MAKELONG('0', 'b'), MAKELONG('0', 'B')) >= 0)
			p = HighlightHexBin(hls, p, 1 + (highlight != HIGHLIGHT_CSS), thousands_separator);
		else
		if(цифра_ли(*p))
			p = HighlightNumber(hls, p, thousands_separator, *p == '0', highlight == HIGHLIGHT_CSS);
		else
		if(pair == MAKELONG('t', '_') && p[2] == '(' && p[3] == '\"') {
			int pos0 = hls.pos;
			hls.помести(3, hl_style[INK_UPP]);
			p = hls.CString(p + 3);
			if(*p == ')') {
				hls.помести(hl_style[INK_UPP]);
				p++;
			}
			hls.SetPaper(pos0, hls.pos - pos0, hl_style[PAPER_LNG].color);
		}
		else
		if(pair == MAKELONG('t', 't') && p[3] == '(' && p[4] == '\"') {
			int pos0 = hls.pos;
			hls.помести(4, hl_style[INK_UPP]);
			p = hls.CString(p + 4);
			if(*p == ')') {
				hls.помести(hl_style[INK_UPP]);
				p++;
			}
			hls.SetPaper(pos0, hls.pos - pos0, hl_style[PAPER_LNG].color);
		}
		else
		if(iscib(*p)) {
			const wchar *q = p;
			ТкстБуф id;
			while((iscidl(*q) || highlight == HIGHLIGHT_CSS && *q == '-') && q < e)
				id.конкат(*q++);
			Ткст iid = id;
			if(highlight == HIGHLIGHT_SQL)
				iid = взаг(iid);
			int uq = kw_upp.найди(iid);
			int nq = -1;
			hls.помести(int(q - p), !include && (nq = keyword[highlight].найди(iid)) >= 0 ? hl_style[INK_KEYWORD] :
			                    имя[highlight].найди(iid) >= 0 ? hl_style[INK_UPP] :
			                    uq >= 0 ? uq < kw_macros ? hl_style[INK_UPPMACROS] :
			                              uq < kw_logs ? hl_style[INK_UPPLOGS] :
			                              uq < kw_sql_base ? hl_style[INK_SQLBASE] :
			                              uq < kw_sql_func ? hl_style[INK_SQLFUNC] :
			                              hl_style[INK_SQLBOOL] :
			                    IsUpperString(iid) && !macro ? hl_style[INK_UPPER] :
			                    hl_style[INK_NORMAL]);
			p = q;
			if(nq == 0)
				was_namespace = true;
		}
		else {
			if(*p == ';')
				was_namespace = false;
			hls.помести(strchr("!+-*^/%~&|=[]:?<>.", *p) ? hl_style[INK_OPERATOR] : hl_style[INK_NORMAL]);
			p++;
		}
	}
	if(hilite_ifdef && !пусто_ли(cppid) && !is_comment) {
		if((cppid == "else" || cppid == "elif" || cppid == "endif") && !ifstack.пустой()) {
			ШТкстБуф ifln;
			ifln.конкат(" // ");
			ifln.конкат(ifstack.верх().iftext);
			if(ifstack.верх().ifline && hilite_ifdef == 2) {
				ifln.конкат(", line ");
				ifln.конкат(фмтЦел(ifstack.верх().ifline));
			}
			ifln.конкат('\t');
			int start = linelen;
			ШТкст ifs(ifln);
			hls.уст(start, ifs.дайДлину(), hl_style[INK_IFDEF]);
			for(int i = 0; i < ifs.дайСчёт(); i++)
				hls.SetChar(start + i, ifs[i]);
		}
	}
	if(hilite_scope) {
		if(lbrace >= 0 && lbclose < 0 && lbrace > lindent)
			hls.SetPaper(lindent, lbrace - lindent, lbcolor);
		if(lbrace < 0 && lbclose >= 0)
			hls.SetPaper(lbclose, linelen + 1 - lbclose, lbcolor);
	}
	if(findarg(cppid, "else", "elif", "endif", "if", "ifdef", "ifndef") >= 0)
	   hls.SetPaper(0, hls.v.дайСчёт(), hl_style[PAPER_IFDEF].color);
}

}
