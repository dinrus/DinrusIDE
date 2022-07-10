#include "CodeEditor.h"

namespace РНЦП {
	
void CSyntax::IndentInsert(РедакторКода& e, int chr, int count)
{
	IndentInsert0(e, chr, count, false);
}

static void sSpaces(int& cp, const ШТкст& ln)
{
	while(cp < ln.дайСчёт() && findarg(ln[cp], '\t', ' ') >= 0)
		cp++;
}

int CSyntax::GetCommentPos(РедакторКода& e, int l, ШТкст& ch) const
{
	ШТкст ln = e.дайШСтроку(l);
	int cp = 0;
	sSpaces(cp, ln);
	Один<EditorSyntax> esyntax = e.дайСинтакс(e.GetCursorLine());
	CSyntax *syntax = dynamic_cast<CSyntax *>(~esyntax);
	if(syntax && syntax->comment && ln.найди("*/") < 0 ||
	   cp < ln.дайСчёт() - 2 && ln[cp] == '/' && ln[cp + 1] == '/') {
	    while(cp < ln.дайДлину() && findarg(ln[cp], '/', '*', '!', '<', '>', '%', '#', '@', '|') >= 0)
	        cp++;
	    ch = ln.середина(0, cp);
        sSpaces(cp, ln);
        if(cp < ln.дайСчёт())
	        return cp;
	}
	ch = "0";
	return -1;
}

void CSyntax::IndentInsert0(РедакторКода& e, int chr, int count, bool reformat)
{
	if(chr == '\n') {
		while(count--) {
			int pp = e.дайКурсор();
			int cl = e.GetLinePos(pp);
			ШТкст pl = e.дайШСтроку(cl);
			e.InsertChar('\n', 1);
			int p = e.дайКурсор();
			int l = e.GetLinePos(p);
			Один<EditorSyntax> esyntax;
			CSyntax *syntax = NULL;
			if(e.дайДлинуСтроки(l) == p) { // по the last char of line
				esyntax = e.дайСинтакс(e.GetCursorLine());
				syntax = dynamic_cast<CSyntax *>(~esyntax);
				if(!syntax) {
					TagSyntax *h = dynamic_cast<TagSyntax *>(~esyntax);
					if(h)
						syntax = h->GetScript();
				}
			}
			if(syntax && syntax->par.дайСчёт() && syntax->par.верх().line == cl && no_parenthesis_indent) {
				for(int i = 0; i < syntax->par.верх().pos && i < pl.дайДлину(); i++) // Indent e.g. next line of if(
					e.InsertChar(pl[i] == '\t' ? '\t' : ' ', 1);
				return;
			}
			if(syntax && syntax->endstmtline == cl && syntax->seline >= 0 && syntax->seline < e.дайСчётСтрок())
				pl = e.дайШСтроку(syntax->seline);
			else {
				int i;
				for(i = 0; i < pl.дайДлину(); i++) {
					if(pl[i] != ' ' && pl[i] != '\t')
						break;
				}
				if(pp < i)
					return;
			}
			const wchar *s = pl;
			while(*s == '\t' || *s == ' ')
				e.InsertChar(*s++, 1);
			if(!syntax)
				return;
			if(syntax->stmtline == cl || syntax->blk.дайСчёт() && syntax->blk.верх() == cl // statement (if, while..) || first line of block
			   || *pl.последний() == ':') { // label
				if(indent_spaces || (s > pl && s[-1] == ' '))
					e.InsertChar(' ', indent_amount);
				else
					e.InsertChar('\t', 1);
			}
		}
		return;
	}

	if(e.IsWordwrapComments()) {
		int limit = e.GetBorderColumn();
		int pos = e.дайКурсор();
		int lp = pos;
		int l = e.GetLinePos(lp);
		if(limit > 10 && e.GetColumnLine(pos).x >= limit && lp == e.дайДлинуСтроки(l)) {
			int lp0 = e.дайПоз(l);
			ШТкст ln = e.дайШСтроку(l);
			ШТкст dummy;
			int cp = GetCommentPos(e, l, dummy);
			if(cp >= 0) {
				int wl = (int)e.GetGPos(l, limit) - lp0;
				while(wl > cp && ln[wl - 1] != '\n' && ln[wl - 1] != ' ')
					wl--;
				int sl = wl - 1;
				while(sl > cp && ln[wl - 1] != '\n' && ln[sl - 1] == ' ')
					sl--;
				if(sl > cp) {
					e.удали(lp0 + sl, pos - (lp0 + sl));
					e.устКурсор(lp0 + sl);
					e.помести('\n');
					for(int i = 0; i < cp; i++)
						e.помести(ln[i]);
					for(int i = wl; i < ln.дайСчёт(); i++)
						e.помести(ln[i]);
					e.помести(chr);
					if(!reformat)
						e.FinishPut();
					return;
				}
			}
		}
	}
	
	if(reformat) {
		e.помести(chr);
		return;
	}

	// {, } inserted on line alone should be moved left sometimes:
	int cl = e.GetCursorLine();
	ШТкст l = e.дайШСтроку(cl);
	if(chr != '{' && chr != '}' || count > 1) {
		e.InsertChar(chr, 1, true);
		return;
	}
	const wchar *s;
	for(s = l; s < l.стоп(); s++)
		if(*s != ' ' && *s != '\t') {
			e.InsertChar(chr, 1);
			return;
		}
	int len = l.дайДлину();
	ШТкст tl;
	int pos = e.дайПоз(cl);
	if(chr == '{' && cl > 0 && stmtline == cl - 1)
		tl = e.дайШСтроку(cl - 1);
	else
	if(chr == '}' && blk.дайСчёт())
		tl = e.дайШСтроку(blk.верх());
	else {
		e.InsertChar(chr, 1);
		return;
	}
	e.устКурсор(pos);
	e.удали(pos, len);
	s = tl;
	while(*s == '\t' || *s == ' ')
		e.InsertChar(*s++, 1);
	e.InsertChar(chr, 1);
}

void CSyntax::перефмтКоммент(РедакторКода& e)
{
	if(!e.IsWordwrapComments())
		return;
	int first_line = e.дайСтроку(e.дайКурсор64());
	ШТкст ch;
	if(GetCommentPos(e, first_line, ch) < 0)
		return;
	int last_line = first_line;
	while(first_line > 0 && GetCommentHdr(e, first_line - 1) == ch)
		first_line--;
	while(last_line < e.дайСчётСтрок() - 1 && GetCommentHdr(e, last_line + 1) == ch)
		last_line++;
	ШТкст p;
	for(int i = first_line; i <= last_line; i++) {
		ШТкст ln = e.дайШСтроку(i);
		p << ln.середина(GetCommentPos(e, i, ch)) << ' ';
	}
	p = Join(разбей(p, CharFilterWhitespace), " ");
	ШТкст ln = e.дайШСтроку(first_line);
	int q = GetCommentPos(e, first_line, ch);
	p = ln.середина(0, q) + p;
	int p0 = e.дайПоз(first_line);
	e.устКурсор(p0);
	e.удали(p0, e.дайПоз(last_line + 1) - 1 - p0);
	for(wchar chr : p)
		IndentInsert0(e, chr, 1, true);
	e.FinishPut();
}

bool NotEscape(int64 pos, const ШТкст& s)
{
	return pos == 0 || s[(int)pos - 1] != '\\' ? true : !NotEscape(pos - 1, s);
}

bool CSyntax::CheckBracket(РедакторКода& e, int li, int64 pos, int64 ppos, int64 pos0, ШТкст ln,
                           int d, int limit, int64& bpos0, int64& bpos)
{
	int lvl = 1;
	pos += d;
	ppos += d;
	for(;;) {
		int c;
		for(;;) {
			while(pos < 0 || pos >= ln.дайДлину()) {
				li += d;
				if(d * li >= d * limit)
					return false;
				ln = e.дайШСтроку(li);
				int q = ln.найди("//");
				if(q >= 0)
					ln = ln.середина(0, q) + ШТкст(' ', ln.дайСчёт() - q);
				pos = d < 0 ? ln.дайДлину() - 1 : 0;
				ppos += d;
			}
			c = ln[(int)pos];
			if((c == '\"' || c == '\'') && (pos > 0 && NotEscape(pos, ln) && ln[(int)pos - 1] != '\'')) {
				pos += d;
				ppos += d;
				int lc = c;
				while(pos < ln.дайДлину() && pos > 0) {
					if(ln[(int)pos] == lc && NotEscape(pos, ln)) {
						pos += d;
						ppos += d;
						break;
					}
					pos += d;
					ppos += d;
				}
			}
			else
				break;
		}
		if(islbrkt(c))
			lvl += d;
		if(isrbrkt(c))
			lvl -= d;
		if(lvl <= 0) {
			bpos0 = pos0;
			bpos = ppos;
			return true;
		}
		pos += d;
		ppos += d;
	}
	return false;
}

bool CSyntax::CheckLeftBracket(РедакторКода& e, int64 pos, int64& bpos0, int64& bpos)
{
	if(pos < 0 || pos >= e.дайДлину64())
		return false;
	int64 ppos = pos;
	int li = e.дайПозСтроки64(pos);
	ШТкст ln = e.дайШСтроку(li);
	return islbrkt(ln[(int)pos]) &&
	       CheckBracket(e, li, pos, ppos, ppos, ln, 1, min(li + 3000, e.дайСчётСтрок()), bpos0, bpos);
}

bool CSyntax::CheckRightBracket(РедакторКода& e, int64 pos, int64& bpos0, int64& bpos)
{
	if(pos < 0 || pos >= e.дайДлину64())
		return false;
	int64 ppos = pos;
	int li = e.дайПозСтроки64(pos);
	ШТкст ln = e.дайШСтроку(li);
	return isrbrkt(ln[(int)pos]) &&
	       CheckBracket(e, li, pos, ppos, ppos, ln, -1, max(li - 3000, 0), bpos0, bpos);
}

bool CSyntax::проверьФигСкобы(РедакторКода& e, int64& bpos0, int64& bpos)
{
	int64 c = e.дайКурсор64();
	return CheckLeftBracket(e, c, bpos0, bpos) ||
	       CheckRightBracket(e, c, bpos0, bpos) ||
	       CheckLeftBracket(e, c - 1, bpos0, bpos) ||
	       CheckRightBracket(e, c - 1, bpos0, bpos);
}

bool CSyntax::CanAssist() const
{
	return !comment && !string && !linecomment;
}

Вектор<IfState> CSyntax::PickIfStack()
{
	return pick(ifstack);
}

void CSyntax::проверьОсвежиСинтакс(РедакторКода& e, int64 pos, const ШТкст& text)
{
	for(const wchar *s = text; *s; s++) {
		if(*s == '{' || *s == '(' || *s == '[' || *s == '/' || *s == '*' ||
		   *s == '}' || *s == ')' || *s == ']' || *s == '\\' || *s == ':') {
			e.освежи();
			break;
		}
	}
	Ткст s = обрежьЛево(e.дайУтф8Строку(e.дайСтроку(pos)));
	if(s.начинаетсяС("#if") || s.начинаетсяС("#e"))
		e.освежи();

	ШТкст h = e.дайШСтроку(e.дайПозСтроки64(pos)); // block highlighting changes if start of line is changed
	for(int i = 0; i < pos; i++)
		if(findarg(h[i], ' ', '\t') < 0)
			return;
	e.освежи();
}

}
