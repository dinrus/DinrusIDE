#include "CodeEditor.h"

namespace РНЦП {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

#define IMAGEVECTOR Вектор
#define IMAGECLASS  CodeEditorImg
#define IMAGEFILE   <CodeEditor/CodeEditor.iml>
#include <Draw/iml_source.h>

#define  TFILE <CodeEditor/CodeEditor.t>
#include <Core/t.h>

void RegisterSyntaxModules();

ИНИЦБЛОК {
	RegisterSyntaxModules();
}

Один<EditorSyntax> РедакторКода::дайСинтакс(int line)
{
	LTIMING("дайСинтакс");
	Один<EditorSyntax> syntax = EditorSyntax::создай(highlight);
	syntax->SpellCheckComments(spellcheck_comments);
	int ln = 0;
	for(int i = 0; i < __countof(syntax_cache); i++)
		if(line >= syntax_cache[i].line && syntax_cache[i].line > 0) {
			syntax->уст(syntax_cache[i].data);
			ln = syntax_cache[i].line;
			LLOG("Found " << line << " " << syntax_cache[i].line);
			break;
		}
	line = min(line, дайСчётСтрок());
	if(line - ln > 10000) { // optimization hack for huge files
		syntax = EditorSyntax::создай(highlight);
		ln = line - 10000;
	}
	while(ln < line) {
		ШТкст l = дайШСтроку(ln);
		CTIMING("ScanSyntax3");
		syntax->ScanSyntax(l, l.стоп(), ln, GetTabSize());
		ln++;
		static int d[] = { 0, 100, 2000, 10000, 50000 };
		for(int i = 0; i < __countof(d); i++)
			if(ln == cline - d[i]) {
				syntax_cache[i + 1].data = syntax->дай();
				syntax_cache[i + 1].line = ln;
			}
	}
	syntax_cache[0].data = syntax->дай();
	syntax_cache[0].line = ln;
	return pick(syntax);
}

void РедакторКода::Highlight(const Ткст& h)
{
	highlight = h;
	устЦвет(СтрокРедакт::INK_NORMAL, hl_style[HighlightSetup::INK_NORMAL].color);
	устЦвет(СтрокРедакт::INK_DISABLED, hl_style[HighlightSetup::INK_DISABLED].color);
	устЦвет(СтрокРедакт::INK_SELECTED, hl_style[HighlightSetup::INK_SELECTED].color);
	устЦвет(СтрокРедакт::PAPER_NORMAL, hl_style[HighlightSetup::PAPER_NORMAL].color);
	устЦвет(СтрокРедакт::PAPER_READONLY, hl_style[HighlightSetup::PAPER_READONLY].color);
	устЦвет(СтрокРедакт::PAPER_SELECTED, hl_style[HighlightSetup::PAPER_SELECTED].color);
	устЦвет(СтрокРедакт::WHITESPACE, hl_style[HighlightSetup::WHITESPACE].color);
	устЦвет(СтрокРедакт::WARN_WHITESPACE, hl_style[HighlightSetup::WARN_WHITESPACE].color);
	освежи();
	EditorBarLayout();
}

void РедакторКода::DirtyFrom(int line) {
	for(int i = 0; i < __countof(syntax_cache); i++)
		if(syntax_cache[i].line >= line)
			syntax_cache[i].очисть();

	if(check_edited) {
		bar.очистьОшибки(line);
		bar.освежи();
	}
}

inline bool коммент_ли(int a, int b) {
	return a == '/' && b == '*' || a == '*' && b == '/' || a == '/' && b == '/';
}

inline bool RBR(int c) {
	return isbrkt(c);
}

Ткст РедакторКода::дайОсвежиИнфо(int pos)
{
	int ii = дайСтроку(pos) + 1;
	return ii < дайСчётСтрок() ? дайСинтакс(ii)->дай() : Ткст();
}

void РедакторКода::проверьОсвежиСинтакс(int64 pos, const ШТкст& text)
{
	дайСинтакс(дайСтроку(pos))->проверьОсвежиСинтакс(*this, pos, text);
}

void РедакторКода::вставьПеред(int pos, const ШТкст& text)
{
	refresh_info = дайОсвежиИнфо(pos);
}

void РедакторКода::вставьПосле(int pos, const ШТкст& text) {
	if(check_edited)
		bar.SetEdited(дайСтроку(pos));
	if(!IsFullRefresh()) {
		if(text.дайСчёт() > 200 || дайОсвежиИнфо(pos) != refresh_info || text.найди('\n') >= 0)
			освежи();
		else
			проверьОсвежиСинтакс(pos, text);
	}
	WhenUpdate();
	EditorBarLayout();
}

void РедакторКода::удалиПеред(int pos, int size) {
	if(IsFullRefresh()) return;
	if(size > 200)
		освежи();
	else {
		ШТкст text = дайШ(pos, size);
		if(text.найди('\n') >= 0)
			освежи();
		else {
			проверьОсвежиСинтакс(pos, text);
			refresh_info = дайОсвежиИнфо(pos);
		}
	}
}

void РедакторКода::удалиПосле(int pos, int size) {
	if(check_edited)
		bar.SetEdited(дайСтроку(pos));
	WhenUpdate();
	EditorBarLayout();
	if(дайОсвежиИнфо(pos) != refresh_info)
		освежи();
}

void РедакторКода::очистьСтроки() {
	bar.очистьСтроки();
}

void РедакторКода::вставьСтроки(int line, int count) {
	if(IsView())
		return;
	bar.вставьСтроки(line, count);
	if(line <= line2.дайСчёт())
		line2.вставь(line, дайСтроку2(line), count);
	EditorBarLayout();
}

void РедакторКода::удалиСтроки(int line, int count) {
	bar.удалиСтроки(line, count);
	if(line + count <= line2.дайСчёт())
		line2.удали(line, count);
	EditorBarLayout();
}

void РедакторКода::Renumber2()
{
	if(IsView())
		return;
	line2.устСчёт(дайСчётСтрок());
	for(int i = 0; i < дайСчётСтрок(); i++)
		line2[i] = i;
}

int РедакторКода::дайСтроку2(int i) const
{
	return line2.дайСчёт() ? line2[min(line2.дайСчёт() - 1, i)] : 0;
}

void РедакторКода::NewScrollPos() {
	bar.промотай();
}

Ткст РедакторКода::дайВставьТекст()
{
	Ткст h;
	WhenPaste(h);
	return h;
}

bool РедакторКода::IsCursorBracket(int64 pos) const
{
	return pos == highlight_bracket_pos0 && hilite_bracket;
}

bool РедакторКода::IsMatchingBracket(int64 pos) const
{
	return pos == highlight_bracket_pos && (hilite_bracket == 1 || hilite_bracket == 2 && bracket_flash);
}

void РедакторКода::проверьФигСкобы()
{
	CancelBracketHighlight(highlight_bracket_pos0);
	CancelBracketHighlight(highlight_bracket_pos);
	if(!выделение_ли()) {
		if(дайСинтакс(GetCursorLine())->проверьФигСкобы(*this, highlight_bracket_pos0, highlight_bracket_pos)) {
			освежиСтроку(дайСтроку(highlight_bracket_pos0));
			освежиСтроку(дайСтроку(highlight_bracket_pos));
			bracket_start = GetTimeClick();
		}
	}
	WhenSelection();
}

void РедакторКода::копируйСлово() {
	int64 p = дайКурсор64();
	if(iscidl(дайСим(p)) || (p > 0 && iscidl(дайСим(--p)))) {
		int64 e = дайДлину64();
		int64 f = p;
		while(--p >= 0 && iscidl(дайСим(p))) {}
		++p;
		while(++f < e && iscidl(дайСим(f)));
		ШТкст txt = дайШ(p, ограничьРазм(f - p));
		WriteClipboardUnicodeText(txt);
		AppendClipboardText(txt.вТкст());
	}
}

void РедакторКода::дублируйСтроку()
{
	if(толькочтен_ли()) return;
	int i = дайСтроку(cursor);
	int pos = дайПоз32(i);
	int len = дайДлинуСтроки(i);
	вставь(pos + len, "\n" + дайШ(pos, len));
}

void РедакторКода::поменяйСимы() {
	if(толькочтен_ли()) return;
	int i = дайСтроку(cursor);
	int j = дайПоз32(i);
	if (j < cursor && cursor - j < дайДлинуСтроки(i)) {
		int p = (int)cursor;
		ШТкст txt(дайСим(p-1),1);
		удали(p-1,1);
		вставь(p, txt, true);
		поместиКаретку(p);
	}
}

void РедакторКода::помести(int chr)
{
	вставь((int)cursor++, ШТкст(chr, 1), true);
}

void РедакторКода::FinishPut()
{
	поместиКаретку(cursor);
	Action();
}

void РедакторКода::перефмтКоммент()
{
	if(толькочтен_ли()) return;
	NextUndo();
	дайСинтакс(дайСтроку(cursor))->перефмтКоммент(*this);
}

void РедакторКода::CancelBracketHighlight(int64& pos)
{
	if(pos >= 0) {
		освежиСтроку(дайСтроку(pos));
		pos = -1;
	}
}

void РедакторКода::Periodic()
{
	bool b = (((GetTimeClick() - bracket_start) >> 8) & 1) == 0;
	if(b != bracket_flash && EditorSyntax::hilite_bracket == 2) {
		bracket_flash = b;
		if(highlight_bracket_pos0 >= 0)
			освежиСтроку(дайСтроку(highlight_bracket_pos0));
		if(highlight_bracket_pos >= 0)
			освежиСтроку(дайСтроку(highlight_bracket_pos));
	}
}

void РедакторКода::SelectionChanged()
{
	int64 l, h;
	ШТкст nilluminated;
	bool sel = дайВыделение(l, h);
	bool ill = false;
	if(sel && h - l < 128) {
		for(int64 i = l; i < h; i++) {
			int c = дайСим(i);
			if(c == '\n') {
				nilluminated.очисть();
				break;
			}
			if(!IsSpace(c))
				ill = true;
			nilluminated.конкат(c);
		}
	}
	if(!ill)
		nilluminated.очисть();
	if(illuminated != nilluminated) {
		illuminated = nilluminated;
		освежи();
	}
	if(!foundsel) {
		if(!persistent_find_replace)
			закройНайдиЗам();
		found = false;
		notfoundfw = notfoundbk = false;
		findreplace.amend.откл();
	}
	проверьФигСкобы();
	bar.освежи();
}

void РедакторКода::Illuminate(const ШТкст& text)
{
	if(illuminated != text) {
		illuminated = text;
		освежи();
	}
}

bool РедакторКода::InsertRS(int chr, int count) {
	if(толькочтен_ли()) return true;
	if(выделение_ли()) {
		InsertChar(chr, count);
		return true;
	}
	return false;
}

void РедакторКода::IndentInsert(int chr, int count) {
	if(InsertRS(chr)) return;
	Один<EditorSyntax> s = дайСинтакс(GetCursorLine());
	if(s)
		s->IndentInsert(*this, chr, count);
	else
		InsertChar(chr, count);
}

void РедакторКода::сделай(Событие<Ткст&> op)
{
	if(толькочтен_ли()) return;
	Точка cursor = GetColumnLine(дайКурсор32());
	Точка scroll = дайПозПромота();
	int l, h;
	bool is_sel = дайВыделение32(l, h);
	if(!is_sel) { l = 0; h = дайДлину32(); }
	if(h <= l)
	{
		бипВосклицание();
		return;
	}
	l = дайПоз32(дайСтроку(l));
	h = дайПоз32(дайСтроку(h - 1) + 1);
	Ткст substring = дай(l, h - l);
	Ткст out = substring;
	op(out);
	if(out == substring)
	{
		бипИнформация();
		return;
	}
	удали(l, h - l);
	вставь(l, out.вШТкст());
	if(is_sel)
		устВыделение(l, l + out.дайДлину());
	устКурсор(GetGPos(cursor.y, cursor.x));
	SetScrollPos(scroll);
}

void РедакторКода::TabsOrSpaces(Ткст& out, bool maketabs)
{
	Ткст substring = out;
	out.очисть();
	int tab = GetTabSize();
	if(tab <= 0) tab = 8;
	for(const char *p = substring.старт(), *e = substring.стоп(); p < e;)
	{
		int pos = 0;
		for(; p < e; p++)
			if(*p == '\t')
				pos = (pos / tab + 1) * tab;
			else if(*p == ' ')
				pos++;
			else
				break;
		if(maketabs)
		{
			out.конкат('\t', pos / tab);
			const char *b = p;
			while(p < e && *p++ != '\n')
				;
			out.конкат(b, int(p - b));
		}
		else
		{
			out.конкат(' ', pos);
			for(; p < e && *p != '\n'; p++)
				if(*p == '\t')
				{
					int npos = (pos / tab + 1) * tab;
					out.конкат(' ', npos - pos);
					pos = npos;
				}
				else
				{
					out.конкат(*p);
					pos++;
				}
			if(p < e)
				out.конкат(*p++);
		}
	}
}

void РедакторКода::MakeTabsOrSpaces(bool maketabs)
{
	сделай(THISBACK1(TabsOrSpaces, maketabs));
}

void РедакторКода::LineEnds(Ткст& out)
{
	Ткст substring = out;
	out.очисть();
	const char *q = ~substring;
	const char *b = q;
	for(const char *p = b, *e = substring.стоп(); p < e; p++)
	{
		if(*p == '\n') {
			out.конкат(b, q);
			out.конкат("\r\n");
			b = q = p + 1;
		}
		else
		if(*p != '\t' && *p != ' ' && *p != '\r')
			q = p + 1;
	}
	out.конкат(b, substring.стоп());
}

void РедакторКода::MakeLineEnds()
{
	сделай(THISBACK(LineEnds));
}

void РедакторКода::MoveNextWord(bool sel) {
	int64 p = дайКурсор64();
	int64 e = дайДлину64();
	if(iscidl(дайСим(p)))
		while(p < e && iscidl(дайСим(p))) p++;
	else
		while(p < e && !iscidl(дайСим(p))) p++;
	поместиКаретку(p, sel);
}

void РедакторКода::MovePrevWord(bool sel) {
	int64 p = дайКурсор64();
	if(p == 0) return;
	if(iscidl(дайСим(p - 1)))
		while(p > 0 && iscidl(дайСим(p - 1))) p--;
	else
		while(p > 0 && !iscidl(дайСим(p - 1))) p--;
	поместиКаретку(p, sel);
}

void РедакторКода::MoveNextBrk(bool sel) {
	int64 p = дайКурсор64();
	int64 e = дайДлину64();
	if(!islbrkt(дайСим(p)))
		while(p < e && !islbrkt(дайСим(p))) p++;
	else {
		int lvl = 1;
		p++;
		for(;;) {
			if(p >= e) break;
			int c = дайСим(p++);
			if(islbrkt(c)) lvl++;
			if(isrbrkt(c) && --lvl == 0) break;
		}
	}
	поместиКаретку(p, sel);
}

void РедакторКода::MovePrevBrk(bool sel) {
	int64 p = дайКурсор64();
	if(p < 2) return;
	if(!isrbrkt(дайСим(p - 1))) {
		if(p < дайДлину64() - 1 && isrbrkt(дайСим(p)))
			p++;
		else {
			while(p > 0 && !isrbrkt(дайСим(p - 1))) p--;
			поместиКаретку(p, sel);
			return;
		}
	}
	int lvl = 1;
	p -= 2;
	for(;;) {
		if(p <= 1) break;
		int c = дайСим(p);
		if(isrbrkt(c)) lvl++;
		if(islbrkt(c) && --lvl == 0) break;
		p--;
	}
	поместиКаретку(p, sel);
}

bool isspctab(int c) {
	return c == ' ' || c == '\t';
}

void РедакторКода::DeleteWord() {
	if(толькочтен_ли() || удалиВыделение()) return;
	int p = дайКурсор32();
	int e = дайДлину32();
	int c = дайСим(p);
	if(iscidl(c))
		while(p < e && iscidl(дайСим(p))) p++;
	else
	if(isspctab(c))
		while(p < e && isspctab(дайСим(p))) p++;
	else {
		DeleteChar();
		return;
	}
	удали(дайКурсор32(), p - дайКурсор32());
}

void РедакторКода::DeleteWordBack() {
	if(толькочтен_ли() || удалиВыделение()) return;
	int p = дайКурсор32();
	if(p < 1) return;
	int c = дайСим(p - 1);
	if(iscidl(дайСим(p - 1)))
		while(p > 1 && iscidl(дайСим(p - 1))) p--;
	else
	if(isspctab(c))
		while(p > 1 && isspctab(дайСим(p - 1))) p--;
	else {
		Backspace();
		return;
	}
	удали(p, дайКурсор32() - p);
	поместиКаретку(p);
}

void РедакторКода::SetLineSelection(int l, int h) {
	устВыделение(дайПоз64(l), дайПоз64(h));
}

bool РедакторКода::GetLineSelection(int& l, int& h) {
	int64 ll, hh;
	if(!дайВыделение(ll, hh)) return false;
	l = дайСтроку(ll);
	h = дайПозСтроки64(hh);
	if(hh && h < дайСчётСтрок()) h++;
	SetLineSelection(l, h);
	return true;
}

void РедакторКода::SwapUpDown(bool up)
{
	int l, h;
	if(дайВыделение(l, h)) {
		l = дайСтроку(l);
		int hh = h;
		h = GetLinePos(hh);
		if(hh && h < дайСчётСтрок()) h++;
	}
	else {
		l = дайСтроку(cursor);
		h = l + 1;
	}
	if(up) {
		if(l == 0)
			return;
		вставь(дайПоз(h), дайШСтроку(l - 1) + "\n");
		удали(дайПоз(l - 1), дайДлинуСтроки(l - 1) + 1);
		устВыделение(дайПоз(l - 1), дайПоз(h - 1));
	}
	else {
		if(h >= дайСчётСтрок() - 1)
			return;
		ШТкст line = дайШСтроку(h) + "\n";
		удали(дайПоз(h), дайДлинуСтроки(h) + 1);
		вставь(дайПоз(l), line);
		устВыделение(дайПоз(l + 1), дайПоз(h + 1));
	}
}

void РедакторКода::TabRight() {
	if(толькочтен_ли()) return;
	int l, h;
	if(!GetLineSelection(l, h)) return;
	int ll = l;
	Ткст tab(indent_spaces ? ' ' : '\t', indent_spaces ? GetTabSize() : 1);
	while(l < h)
		вставь(дайПоз32(l++), tab);
	SetLineSelection(ll, h);
}

void РедакторКода::TabLeft() {
	if(толькочтен_ли()) return;
	int l, h;
	if(!GetLineSelection(l, h)) return;
	int ll = l;
	while(l < h) {
		ШТкст ln = дайШСтроку(l);
		int spc = 0;
		while(spc < tabsize && ln[spc] == ' ') spc++;
		if(spc < tabsize && ln[spc] == '\t') spc++;
		удали(дайПоз32(l++), spc);
	}
	SetLineSelection(ll, h);
}

bool РедакторКода::GetWordPos(int64 pos, int64& l, int64& h) {
	l = h = pos;
	if(!iscidl(дайСим(pos))) return false;
	while(l > 0 && iscidl(дайСим(l - 1))) l--;
	while(iscidl(дайСим(h))) h++;
	return true;
}

Ткст РедакторКода::GetWord(int64 pos)
{
	int64 l, h;
	GetWordPos(pos, l, h);
	return дай(l, ограничьРазм(h - l));
}

Ткст РедакторКода::GetWord()
{
	return GetWord(cursor);
}

void РедакторКода::леваяДКлик(Точка p, dword keyflags) {
	int64 l, h;
	int64 pos = дайПозМыши(p);
	if(GetWordPos(pos, l, h))
		устВыделение(l, h);
	else
		устВыделение(pos, pos + 1);
	selkind = SEL_WORDS;
	устФокус();
	SetCapture();
}

void РедакторКода::LeftTriple(Точка p, dword keyflags)
{
	СтрокРедакт::LeftTriple(p, keyflags);
	selkind = SEL_LINES;
	устФокус();
	SetCapture();
}

void РедакторКода::леваяВнизу(Точка p, dword keyflags) {
	if((keyflags & K_CTRL) && WhenCtrlClick) {
		WhenCtrlClick(дайПозМыши(p));
		return;
	}
	СтрокРедакт::леваяВнизу(p, keyflags);
	WhenLeftDown();
	закройНайдиЗам();
	selkind = SEL_CHARS;
}

void РедакторКода::Подсказка::рисуй(Draw& w)
{
	Прям r = дайРазм();
	w.DrawRect(r, SColorInfo());
	r.left++;
	if(d)
		d->рисуй(w, r, v, SColorText(), SColorPaper(), 0);
}

РедакторКода::Подсказка::Подсказка()
{
	устФрейм(фреймЧёрный());
	BackPaint();
}

void РедакторКода::SyncTip()
{
	MouseTip mt;
	mt.pos = tippos;
	if(tippos >= 0 && видим_ли() && WhenTip(mt)) {
		tip.d = mt.дисплей;
		tip.v = mt.значение;
		Точка p = РНЦП::дайПозМыши();
		Размер sz = tip.дайРазмФрейма(mt.sz);
		tip.устПрям(p.x, p.y + 24, sz.cx, sz.cy);
		if(!tip.открыт())
			tip.PopUp(this, false, false, true);
		tip.освежи();
	}
	else
		CloseTip();
}

bool РедакторКода::MouseSelSpecial(Точка p, dword flags) {
	if((flags & K_MOUSELEFT) && естьФокус() && HasCapture() && !(flags & K_ALT) && selkind != SEL_CHARS) {
		int64 c = дайПозМыши(p);
		int64 l, h;
		
		if(selkind == SEL_LINES) {
			l = c;
			int i = дайПозСтроки64(l);
			l = c - l;
			h = min(l + дайДлинуСтроки(i) + 1, дайДлину64() - 1);
			c = c < anchor ? l : h;
		}
		else
			c = iscidl(дайСим(c - 1)) && GetWordPos(c, l, h) ? c < anchor ? l : h : c;
		поместиКаретку(c, mpos != c);
		return true;
	}
	return false;
}

void РедакторКода::леваяПовтори(Точка p, dword flags)
{
	if(!MouseSelSpecial(p, flags))
		СтрокРедакт::леваяПовтори(p, flags);
}

void РедакторКода::двигМыши(Точка p, dword flags) {
	if(!MouseSelSpecial(p, flags))
		СтрокРедакт::двигМыши(p, flags);
	if(выделение_ли()) return;
	int64 h = дайПозМыши(p);
	tippos = h < INT_MAX ? (int)h : -1;
	SyncTip();
}

Рисунок РедакторКода::рисКурсора(Точка p, dword keyflags)
{
	if(WhenCtrlClick && (keyflags & K_CTRL))
		return Рисунок::Hand();
	if(tip.открыт())
		return Рисунок::Arrow();
	return СтрокРедакт::рисКурсора(p, keyflags);
}

void РедакторКода::выходМыши()
{
	tippos = -1;
	СтрокРедакт::выходМыши();
}

ШТкст РедакторКода::GetI()
{
	int64 l, h;
	ШТкст ft;
	if((дайВыделение(l, h) || GetWordPos(дайКурсор64(), l, h)) && h - l < 60)
		while(l < h) {
			int c = дайСим(l++);
			if(c == '\n')
				break;
			ft.конкат(c);
		}
	return ft;
}

//void РедакторКода::FindWord(bool back)
//{
//	ШТкст I = GetI();
//	if(!пусто_ли(I))
//		найди(back, I, true, false, false, false, false);
//}

void РедакторКода::SetI(Ктрл *edit)
{
	*edit <<= GetI();
}

void РедакторКода::идиК() {
	Ткст line = какТкст(GetCursorLine());
	if(редактируйТекст(line, t_("иди to"), t_("Строка:")))
		устКурсор(дайПоз64(atoi(line) - 1));
}

bool РедакторКода::ToggleSimpleComment(int &start_line, int &end_line, bool usestars)
{
	if(толькочтен_ли()) return true;

	int l, h;
	if(!дайВыделение32(l, h))
		return true;

	int pos = h;
	start_line = дайСтроку(l);
	end_line = дайПозСтроки32(pos);

	if(usestars && start_line == end_line) {
		Enclose("/*", "*/", l, h);
		return true;
	}

	if(pos && end_line < дайСчётСтрок()) end_line++;
	SetLineSelection(start_line, end_line);

	return false;
}

void РедакторКода::ToggleLineComments(bool usestars)
{
	if(толькочтен_ли()) return;

	int start_line, end_line;
	if(ToggleSimpleComment(start_line, end_line))
		return;

	int us = static_cast<int>(usestars);

	bool is_commented = true;

	if(usestars) {
		is_commented &= дайСим(дайПоз64(start_line) + 0) == '/' &&
						дайСим(дайПоз64(start_line) + 1) == '*';

		is_commented &= дайСим(дайПоз64(end_line - 1) + 1) == '*' &&
						дайСим(дайПоз64(end_line - 1) + 2) == '/';
	}

	for(int line = start_line + us; is_commented && (line < end_line - us * 2); ++line)
		is_commented &= дайСим(дайПоз64(line)) == (usestars ? ' ' : '/') &&
						дайСим(дайПоз64(line)+1) == (usestars ? '*' : '/');

	if(!is_commented) {

		if(usestars) {
			вставь(дайПоз32(end_line)," */\n");
			вставь(дайПоз32(start_line),"/*\n");
		}

		for(int line = start_line + us; line < end_line + us; ++line)
			вставь(дайПоз32(line), usestars ? " * " : "//");
	}
	else
	{
		int line = start_line;
		if(usestars)
			удали(дайПоз32(start_line), 3);
		for(; line < end_line - us * 2; ++line)
			удали(дайПоз32(line), 2 + us);
		if(usestars)
			удали(дайПоз32(line), 4);
	}

	if(usestars)
		SetLineSelection(start_line, end_line + (is_commented ? -2 : 2));
	else
		SetLineSelection(start_line, end_line);
}

void РедакторКода::ToggleStarComments()
{
	if(толькочтен_ли()) return;

	int start_line, end_line;
	if(ToggleSimpleComment(start_line, end_line))
		return;

	bool is_commented =
		дайСим(дайПоз64(start_line)) == '/' &&
		дайСим(дайПоз64(start_line)+1) == '*' &&
		дайСим(дайПоз64(start_line)+2) == '\n' &&
		дайСим(дайПоз64(end_line-1)) == '*' &&
		дайСим(дайПоз64(end_line-1)+1) == '/' &&
		дайСим(дайПоз64(end_line-1)+2) == '\n';

	if(!is_commented) {
		// Backwards because inserting changes the end line #
		вставь(дайПоз32(end_line),"*/\n");
		вставь(дайПоз32(start_line),"/*\n");
		SetLineSelection(start_line, end_line+2);
	} else {
		// Backwards because inserting changes the end line #
		удали(дайПоз32(end_line-1),3);
		удали(дайПоз32(start_line),3);
		SetLineSelection(start_line, end_line-2);
	}
}

void РедакторКода::Enclose(const char *c1, const char *c2, int l, int h)
{
	if(толькочтен_ли()) return;

	if((l < 0 || h < 0) && !дайВыделение32(l, h))
		return;
	вставь(l, ШТкст(c1));
	вставь(h + (int)strlen(c1), ШТкст(c2));
	очистьВыделение();
	устКурсор(h + (int)strlen(c1) + (int)strlen(c2));
}

bool РедакторКода::Ключ(dword code, int count) {
	Время key_time = дайСисВремя();
	double diff;
	if(!пусто_ли(last_key_time) && (diff = int(key_time - last_key_time)) <= 3)
		stat_edit_time += diff;
	last_key_time = key_time;

	NextUndo();
	if(code == replace_key) {
		замени();
		return true;
	}
	switch(code) {
	case K_SHIFT_CTRL_UP:
		SwapUpDown(true);
		return true;
	case K_SHIFT_CTRL_DOWN:
		SwapUpDown(false);
		return true;
	case K_CTRL_DELETE:
		DeleteWord();
		return true;
	case K_CTRL_BACKSPACE:
		DeleteWordBack();
		return true;
	case K_BACKSPACE:
		if(!толькочтен_ли() && !IsAnySelection() && indent_spaces) {
			int c = дайКурсор32();
			Точка ixln = GetIndexLine(c);
			ШТкст ln = дайШСтроку(ixln.y);
			bool white = true;
			int startindex = -1, pos = 0, tabsz = GetTabSize();
			for(int i = 0; i < ixln.x; i++) {
				if(ln[i] == '\t' || ln[i] == ' ') {
					if(pos == 0)
						startindex = i;
					if(ln[i] == '\t' || ++pos >= tabsz)
						pos = 0;
				}
				else {
					white = false;
					break;
				}
			}
			if(white && startindex >= 0) {
				int count = ixln.x - startindex;
				поместиКаретку(c - count);
				удали(c - count, count);
				Action();
				return true;
			}
		}
		break;
	case K_SHIFT_CTRL_TAB:
		return СтрокРедакт::Ключ(K_TAB, count);
	case K_ENTER:
		IndentInsert('\n', count);
		return true;
	}
	bool sel = code & K_SHIFT;
	switch(code & ~K_SHIFT) {
	case K_CTRL_F:
		if(withfindreplace) {
			FindReplace(sel, true, false);
			return true;
		}
		break;
	case K_CTRL_H:
		if(withfindreplace) {
			FindReplace(sel, true, true);
			return true;
		}
		break;
	case K_F3:
		if(sel)
			найдиПредш();
		else
			найдиСледщ();
		return true;
//	case K_CTRL_F3:
//		FindWord(sel);
//		return true;
	case K_CTRL_RIGHT:
		MoveNextWord(sel);
		return true;
	case K_CTRL_LEFT:
		MovePrevWord(sel);
		return true;
	case K_CTRL_RBRACKET:
		MoveNextBrk(sel);
		return true;
	case K_CTRL_LBRACKET:
		MovePrevBrk(sel);
		return true;
	case K_CTRL_ADD:
		Zoom(1);
		return true;
	case K_CTRL_SUBTRACT:
		Zoom(-1);
		return true;
	case K_TAB:
		if(!толькочтен_ли()) {
			if(выделение_ли()) {
				if(sel)
					TabLeft();
				else
					TabRight();
				return true;
			}
			if(!sel && indent_spaces) {
				int x = GetColumnLine(дайКурсор64()).x;
				int add = GetTabSize() - x % GetTabSize();
				InsertChar(' ', add, false);
				return true;
			}
		}
	default:
		if(выделение_ли() && auto_enclose) {
			if(code == '(') {
				Enclose("(", ")");
				return true;
			}
			if(code == '{') {
				Enclose("{", "}");
				return true;
			}
			if(code == '\"') {
				Enclose("\"", "\"");
				return true;
			}
			if(code == '[') {
				Enclose("[", "]");
				return true;
			}
			if(code == '/') {
				//Enclose("/*", "*/");
				ToggleLineComments(false);
				return true;
			}
			if(code == K_CTRL_SLASH)
			{
				ToggleLineComments(true);
				return true;
			}
			if(code == '*') {
				//Enclose("/*", "*/");
				ToggleStarComments();
				return true;
			}
		}
		if(wordwrap && code > 0 && code < 65535) {
			int limit = GetBorderColumn();
			int pos = дайКурсор32();
			int lp = pos;
			int l = дайПозСтроки32(lp);
			if(limit > 10 && GetColumnLine(pos).x >= limit && lp == дайДлинуСтроки(l)) {
				int lp0 = дайПоз32(l);
				ШТкст ln = дайШСтроку(l);
				int wl = (int)GetGPos(l, limit) - lp0;
				while(wl > 0 && ln[wl - 1] != ' ')
					wl--;
				int sl = wl - 1;
				while(sl > 0 && ln[wl - 1] != '\n' && ln[sl - 1] == ' ')
					sl--;
				wordwrap = false;
				удали(lp0 + sl, pos - (lp0 + sl));
				устКурсор(lp0 + sl);
				помести('\n');
				for(int i = 0; i < wl && findarg(ln[i], ' ', '\t') >= 0; i++)
					помести(ln[i]);
				for(int i = wl; i < ln.дайСчёт(); i++)
					помести(ln[i]);
				while(count--)
					помести(code);
				FinishPut();
				wordwrap = true;
				return true;
			}
	
		}
		if(code >= 32 && code < 128 && count == 1) {
			IndentInsert(code, 1);
			return true;
		}
		if(code == 9 && выделение_ли())
			return true;
	}
	if(дайНабсим() != CHARSET_UTF8)
		if(code >= 128 && code < 65536 && изЮникода((wchar)code, дайНабсим()) == DEFAULTCHAR)
			return true;
	return СтрокРедакт::Ключ(code, count);
}

void РедакторКода::ForwardWhenBreakpoint(int i) {
	WhenBreakpoint(i);
}

void РедакторКода::GotoLine(int line)
{
	устКурсор(дайПоз64(дайНомСтр(line)));
}

void РедакторКода::сериализуй(Поток& s) {
	int version = 0;
	s / version;
	SerializeFind(s);
}

void РедакторКода::SetLineInfo(const LineInfo& lf)
{
	bar.SetLineInfo(lf, дайСчётСтрок());
}

void РедакторКода::HighlightLine(int line, Вектор<СтрокРедакт::Highlight>& hl, int64 pos)
{
	CTIMING("HighlightLine");
	HighlightOutput hls(hl);
	ШТкст l = дайШСтроку(line);
	дайСинтакс(line)->Highlight(l.старт(), l.стоп(), hls, this, line, pos);
	if(illuminated.дайСчёт()) {
		int q = 0;
		while(q < l.дайСчёт() && q < hl.дайСчёт()) {
			q = l.найди(illuminated, q);
			if(q < 0)
				break;
			int n = illuminated.дайСчёт();
			if(n > 1 || !iscid(illuminated[0]) ||
			   (q == 0 || !iscid(l[q - 1])) && (q + n >= l.дайСчёт() || !iscid(l[q + n])))
				while(n-- && q < hl.дайСчёт()) {
					const HlStyle& st = hl_style[PAPER_SELWORD];
					hl[q].paper = st.color;
					if(st.bold)
						hl[q].font.Bold();
					q++;
				}
			else
				q++;
		}
	}
}

void РедакторКода::PutI(WithDropChoice<EditString>& edit)
{
	edit.AddButton().SetMonoImage(CodeEditorImg::I()).Подсказка(t_("уст word/selection (Ктрл+I)"))
	    <<= THISBACK1(SetI, &edit);
}

void РедакторКода::Zoom(int d)
{
	Шрифт f = дайШрифт();
	int h = f.GetCy();
	int q = f.дайВысоту();
	while(f.GetCy() == h && (d < 0 ? f.GetCy() > 5 : f.GetCy() < 80))
		f.устВысоту(q += d);
	устШрифт(f);
	EditorBarLayout();
}

void РедакторКода::колесоМыши(Точка p, int zdelta, dword keyFlags) {
	if(keyFlags & K_CTRL) {
		Zoom(зн(zdelta));
	}
	else
		СтрокРедакт::колесоМыши(p, zdelta, keyFlags);
}

void РедакторКода::очисть()
{
	for(SyntaxPos& p : syntax_cache)
		p.очисть();
	СтрокРедакт::очисть();
	found = notfoundfw = notfoundbk = false;
}

РедакторКода::РедакторКода() {
	bracket_flash = false;
	highlight_bracket_pos0 = 0;
	bracket_start = 0;
	stat_edit_time = 0;
	last_key_time = Null;
	устШрифт(CourierZ(12));
	добавьФрейм(bar);
	bar.SetEditor(this);
	UndoSteps(10000);
	иницНайтиЗаменить();
	bar.WhenBreakpoint = THISBACK(ForwardWhenBreakpoint);
	bar.WhenAnnotationMove = WhenAnnotationMove.прокси();
	bar.WhenAnnotationClick = WhenAnnotationClick.прокси();
	bar.WhenAnnotationRightClick = WhenAnnotationRightClick.прокси();
	barline = true;
	sb.WithSizeGrip();
	DefaultHlStyles();
	Highlight(Null);
	sb.y.безАвтоСкрой();
	sb.y.добавьФрейм(topsbbutton);
	sb.y.добавьФрейм(topsbbutton1);
	topsbbutton.скрой();
	topsbbutton1.скрой();
	highlight_bracket_pos = 10;
	устОбрвызВремени(-20, THISBACK(Periodic), TIMEID_PERIODIC);
	auto_enclose = false;
	mark_lines = true;
	check_edited = false;
	tippos = -1;
	selkind = SEL_CHARS;
	withfindreplace = true;
	wordwrap = false;
}

РедакторКода::~РедакторКода() {}

}
