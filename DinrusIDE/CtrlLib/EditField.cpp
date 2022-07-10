#include "CtrlLib.h"

namespace РНЦП {

CH_VALUE(ViewEdge, CtrlsImg::VE());

Значение EditFieldEdge()
{
	return EditField::дефСтиль().edge[0];
}

КтрлФрейм& EditFieldFrame()
{
	static LookFrame m(EditFieldEdge);
	return m;
}

КтрлФрейм& ViewFrame()
{
	static LookFrame m(ViewEdge);
	return m;
}

bool IsWCh(int c)
{
	return IsLeNum(c) || c == '_';
}

bool TextArrayOps::GetWordSelection(int64 c, int64& l, int64& h)
{
	if(IsWCh(дайСимПо(c))) {
		l = h = c;
		while(l > 0 && IsWCh(дайСимПо(l - 1)))
			l--;
		while(h < дайВсего() && IsWCh(дайСимПо(h)))
			h++;
		if(h != c)
			while(h < дайВсего() && дайСимПо(h) == ' ')
				h++;
		return true;
	}
	return false;
}

int64 TextArrayOps::GetNextWord(int64 cursor)
{
	bool a = IsWCh(дайСимПо(cursor));
	int n = 0;
	int64 c = cursor;
	while(c <= дайВсего() && IsWCh(дайСимПо(c)) == a) {
		if(++n > 10000) return cursor;
		c++;
	}
	return c;
}

int64 TextArrayOps::GetPrevWord(int64 cursor)
{
	int n = 0;
	int64 c = cursor;
	if(c == 0) return 0;
	bool a = IsWCh(дайСимПо(c - 1));
	while(c > 0 && IsWCh(дайСимПо(c - 1)) == a) {
		if(++n > 10000) return cursor;
		c--;
	}
	return c;
}

void LookFrame::выложиФрейм(Прям& r)
{
	Прям m = LookMargins(r, дай());
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
}

void LookFrame::рисуйФрейм(Draw& w, const Прям& r)
{
	ChPaintEdge(w, r, дай());
}

void LookFrame::добавьРазмФрейма(Размер& sz)
{
	Прям m = ChMargins(дай());
	sz.cx += m.left + m.right;
	sz.cy += m.top + m.bottom;
}

CH_STYLE(EditField, Стиль, дефСтиль)
{
	paper = SColorPaper();
	disabled = SColorFace();
	focus = paper;
	invalid = смешай(paper, Цвет(255, 0, 0), 32);
	text = SColorText();
	textdisabled = SColorDisabled();
	selected = SColorHighlight();
	selectedtext = SColorHighlightText();
	selected0 = SColorDkShadow();
	selectedtext0 = SColorHighlightText();
	for(int i = 0; i < 4; i++)
		edge[i] = CtrlsImg::EFE();
	activeedge = false;
	vfm = 2;
	coloredge = Null;
}

bool EditField::FrameIsEdge()
{
	return &дайФрейм() == &edge;
}

void EditField::RefreshAll()
{
	Цвет paper = GetPaper();
	edge.устЦвет(style->coloredge, paper);
	WhenPaper(paper);
	освежиФрейм();
}

void EditField::входМыши(Точка p, dword keyflags)
{
	edge.Mouse(true);
	RefreshAll();
}

void EditField::выходМыши()
{
	edge.Mouse(false);
	RefreshAll();
}

EditField& EditField::устСтиль(const Стиль& s)
{
	style = &s;
	edge.уст(this, style->edge, style->activeedge);
	освежиВыкладку();
	RefreshAll();
	return *this;
}

void EditField::режимОтмены()
{
	keep_selection = false;
	selclick = false;
	dropcaret.очисть();
}

int EditField::GetTextCx(const wchar *txt, int n, bool password, Шрифт fnt) const
{
	if(password)
		return n * font['*'];
	const wchar *s = txt;
	int x = 0;
	while(n--)
		x += GetCharWidth(*s++);
	return x;
}

int  EditField::дайКаретку(int cursor) const
{
	return GetTextCx(text, cursor, password, font);
}

int  EditField::GetViewHeight(Шрифт font)
{
	Размер sz(0, 0);
	EditFieldFrame().добавьРазмФрейма(sz);
	return font.GetCy() + (sz.cy <= 2 ? 4 : sz.cy <= 4 ? 2 : 0);
}

int  EditField::GetStdHeight(Шрифт font)
{
	Размер sz = Размер(10, GetViewHeight());
	EditFieldFrame().добавьРазмФрейма(sz);
	return sz.cy;
}

Размер EditField::дайМинРазм() const
{
	return дайРазмФрейма(10, font.GetCy() + (no_internal_margin ? 0 : 4));
}

int  EditField::дайКурсор(int posx)
{
	posx -= 2;
	if(posx <= 0) return 0;

	int count = text.дайДлину();
	if(password)
		return min((posx + font['*'] / 2) / font['*'], count);

	int x = 0;
	const wchar *s = text;
	int i = 0;
	while(i < count) {
		int witdh = GetCharWidth(*s);
		if(posx < x + witdh / 2)
			break;
		x += witdh;
		s++; i++;
	}
	return i;
}

Рисунок EditField::рисКурсора(Точка, dword)
{
	return Рисунок::IBeam();
}

int  EditField::GetTy() const
{
	return (дайРазм().cy + 1 - font.GetCy()) / 2;
}

void EditField::HighlightText(Вектор<Highlight>& hl)
{
	WhenHighlight(hl);
}

void EditField::Paints(Draw& w, int& x, int fcy, const wchar *&txt,
					   Цвет ink, Цвет paper, int n, bool password, Шрифт fnt, Цвет underline,
					   bool showspaces)
{
	if(n < 0) return;
	int cx = GetTextCx(txt, n, password, font);
	w.DrawRect(x, 0, cx, fcy, paper);
	if(password) {
		Ткст h;
		h.конкат('*', n);
		w.DrawText(x, 0, ~h, fnt, ink, n);
	}
	else {
		const wchar *txts = txt;
		Буфер<wchar> h;
		const wchar *e = txt + n;
		for(const wchar *q = txt; q < e; q++)
			if(*q < 32) {
				h.размести(n);
				wchar *t = ~h;
				for(const wchar *q = txt; q < e; q++)
					*t++ = *q < 32 ? LowChar(*q) : *q;
				txts = ~h;
			}
		if(!пусто_ли(underline))
			w.DrawRect(x, fnt.GetAscent() + 1, cx, 1, underline);
		w.DrawText(x, 0, txts, fnt, ink, n);
		if(showspaces) {
			int xx = x;
			Размер sz = дайРазмТекста(" ", fnt) / 2;
			e = txts + n;
			for(const wchar *q = txts; q < e; q++) {
				if(*q == ' ')
					w.DrawRect(xx + sz.cx, sz.cy, 2, 2, смешай(SColorHighlight(), SColorPaper()));
				xx += fnt[*q];
			}
		}
	}
	txt += n;
	x += cx;
}

void EditField::State(int)
{
	RefreshAll();
}

Цвет EditField::GetPaper()
{
	bool enabled = IsShowEnabled();
	Цвет paper = enabled && !толькочтен_ли() ? (естьФокус() ? style->focus : style->paper)
	                                       : style->disabled;
	if(nobg)
		paper = Null;
	if(enabled && (convert && convert->скан(text).ошибка_ли() || errorbg))
		paper = style->invalid;
	return paper;
}

void EditField::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	bool enabled = IsShowEnabled();
	Цвет paper = GetPaper();
	Цвет ink = enabled ? Nvl(textcolor, style->text) : style->textdisabled;
	int fcy = font.GetCy();
	int yy = GetTy();
	if(!no_internal_margin) {
		w.DrawRect(0, 0, 2, sz.cy, paper);
		w.DrawRect(0, 0, sz.cx, yy, paper);
		w.DrawRect(0, yy + fcy, sz.cx, sz.cy - yy - fcy, paper);
		w.DrawRect(sz.cx - 2, 0, 2, sz.cy, paper);
		w.Clipoff(2, yy, sz.cx - 4, fcy);
	}
	int x = -sc;
	w.DrawRect(0, 0, sz.cx, fcy, paper);
	if(пусто_ли(text) && (!пусто_ли(nulltext) || !пусто_ли(nullicon))) {
		const wchar *txt = nulltext;
		if(!пусто_ли(nullicon)) {
			int icx = nullicon.дайШирину();
			w.DrawRect(x, 0, icx + 4, fcy, paper);
			w.DrawImage(x, (fcy - nullicon.дайВысоту()) / 2, nullicon);
			x += icx + 4;
		}
		Paints(w, x, fcy, txt, nullink, paper, nulltext.дайДлину(), false, nullfont, Null, false);
	}
	else {
		const wchar *txt = text;
		int len = дайДлину();
		Вектор<Highlight> hl;
		hl.устСчёт(len);
		for(int i = 0; i < len; i++) {
			hl[i].ink = ink;
			hl[i].paper = paper;
			hl[i].underline = Null;
		}
		HighlightText(hl);
		len = hl.дайСчёт();
		int l, h;
		if(дайВыделение(l, h)) {
			h = min(h, len);
			for(int i = l; i < h; i++) {
				hl[i].ink = enabled ? естьФокус() ? style->selectedtext : style->selectedtext0 : paper;
				hl[i].paper = enabled ? естьФокус() ? style->selected : style->selected0 : ink;
			}
		}
		int b = 0;
		for(int i = 0; i <= len; i++)
			if((i == len || hl[i] != hl[b]) && b < len) {
				Paints(w, x, fcy, txt, hl[b].ink, hl[b].paper, i - b, password, font, hl[b].underline, showspaces);
				b = i;
			}
	}
	if(!no_internal_margin)
		w.стоп();
	DrawTiles(w, dropcaret, CtrlImg::checkers());
}

bool EditField::дайВыделение(int& l, int& h) const
{
	if(anchor < 0 || anchor == cursor) {
		l = h = cursor;
		return false;
	}
	if(anchor < cursor) {
		l = anchor;
		h = cursor;
	}
	else {
		l = cursor;
		h = anchor;
	}
	return true;
}

bool EditField::выделение_ли() const
{
	return anchor >= 0 && anchor != cursor;
}

Прям EditField::GetCaretRect(int pos) const
{
	return RectC(дайКаретку(pos) - sc + 2 * !no_internal_margin
	               - font.GetRightSpace('o') + font.GetLeftSpace('o'), GetTy(),
	             DPI(1), min(дайРазм().cy - 2 * GetTy(), font.GetCy()));
}

void EditField::SyncCaret()
{
	Прям r = GetCaretRect(cursor);
	устКаретку(r.left, r.top, r.дайШирину(), r.дайВысоту());
}

void EditField::финиш(bool refresh)
{
	if(anchor > text.дайДлину()) anchor = text.дайДлину();
	if(cursor > text.дайДлину()) cursor = text.дайДлину();
	if(cursor < 0) cursor = 0;
	Размер sz = дайРазм();
	if(autosize) {
		Прям r = дайПрям();
		int mw = min(r.устШирину(), Draw::GetStdFontSize().cx);
		sz.cx = дайКаретку(text.дайДлину()) + 4;
		sz = дайРазмФрейма(sz);
		if(дайРодителя())
			sz.cx = min(sz.cx, дайРодителя()->дайРазм().cx - r.left);
		sz.cx = minmax(sz.cx, mw, autosize);
		if(sz.cx != r.устШирину())
			LeftPos(r.left, sz.cx);
		sz = дайРазм();
	}
	sz.cx -= 2;
	if(sz.cx <= 0) return;
	int x = дайКаретку(cursor);
	int rspc = max(font.GetRightSpace('o'), font.GetCy() / 5); // sometimes RightSpace is not implemented (0)
	int wx = x + rspc;
	if(alignright) {
		int cx = дайКаретку(text.дайСчёт());
		sc = cx - sz.cx + rspc + 2;
		RefreshAll();
	}
	if(wx > sz.cx + sc - 1) {
		sc = wx - sz.cx + 1;
		RefreshAll();
	}
	if(x < sc) {
		sc = x;
		RefreshAll();
	}
	if(refresh)
		RefreshAll();
	SyncCaret();
}

void EditField::Выкладка()
{
	Ктрл::Выкладка();
	sc = 0;
	финиш();
}

void EditField::SelSource()
{
	if(дайВыделение(fsell, fselh))
		SetSelectionSource(ClipFmtsText());
	else
		fsell = fselh = -1;
	CancelMyPreedit();
}

void EditField::сфокусирован()
{
	if(autoformat && редактируем_ли() && !пусто_ли(text) && inactive_convert) {
		Значение v = convert->скан(text);
		if(!v.ошибка_ли()) {
			ШТкст s = convert->фмт(v);
			if(s != text) text = s;
		}
	}
	if(!keep_selection && !выделение_ли()) {
		anchor = 0;
		cursor = text.дайДлину();
	}
	SelSource();
	финиш();
	RefreshAll();
}

void EditField::расфокусирован()
{
	if(autoformat && редактируем_ли() && !пусто_ли(text) && !IsDragAndDropSource()) {
		Значение v = convert->скан(text);
		if(!v.ошибка_ли()) {
			const Преобр * cv = inactive_convert ? inactive_convert : convert;
			ШТкст s = cv->фмт(v);
			if(s != text) text = s;
		}
	}
	if(!keep_selection) {
		anchor = -1;
		cursor = sc = 0;
		if(alignright)
			финиш();
	}
	RefreshAll();
}

void EditField::леваяВнизу(Точка p, dword flags)
{
	int c = дайКурсор(p.x + sc);
	if(!естьФокус()) {
		устФокус();
		if(clickselect) {
			устВыделение();
			финиш();
			return;
		}
		sc = 0;
		Move(c);
	}
	int l, h;
	selclick = false;
	if(дайВыделение(l, h) && c >= l && c < h) {
		selclick = true;
		return;
	}
	SetCapture();
	Move(c, flags & K_SHIFT);
	финиш();
}

void EditField::MiddleDown(Точка p, dword flags)
{
	if(толькочтен_ли())
		return;
	if(AcceptText(Selection())) {
		ШТкст w = дайШТкст(Selection());
		selclick = false;
		леваяВнизу(p, flags);
		вставь(w);
		Action();
		финиш();
	}
}

void EditField::леваяВверху(Точка p, dword flags)
{
	int c = дайКурсор(p.x + sc);
	int l, h;
	if(дайВыделение(l, h) && c >= l && c < h && !HasCapture() && selclick)
		Move(c, false);
	финиш();
	selclick = false;
}

void EditField::леваяДКлик(Точка p, dword flags)
{
	int64 l, h;
	if(GetWordSelection(cursor, l, h))
		устВыделение((int)l, (int)h);
}

void EditField::LeftTriple(Точка p, dword keyflags)
{
	anchor = 0;
	cursor = text.дайДлину();
	финиш();
}

void EditField::двигМыши(Точка p, dword flags)
{
	if(!HasCapture()) return;
	Move(дайКурсор(p.x + sc), true);
	финиш();
}

void EditField::SaveUndo()
{
	undotext = text;
	undoanchor = anchor;
	undocursor = cursor;
}

void EditField::Move(int newpos, bool select)
{
	bool refresh = anchor >= 0;
	if(select) {
		if(anchor < 0) anchor = cursor;
		refresh = true;
	}
	else
		anchor = -1;
	cursor = newpos;
	финиш(refresh);
	SelSource();
}

void EditField::устВыделение(int l, int h)
{
	if(l < h) {
		anchor = max(l, 0);
		cursor = min(h, text.дайДлину());
	}
	else {
		cursor = l;
		anchor = -1;
	}
	SelSource();
	финиш();
}

void EditField::CancelSelection()
{
	int l, h;
	if(дайВыделение(l, h)) {
		cursor = l;
		anchor = -1;
		fsell = fselh = -1;
		sc = 0;
		финиш();
	}
}

bool EditField::удалиВыделение()
{
	int l, h;
	if(!дайВыделение(l, h)) {
		anchor = -1;
		return false;
	}
	SaveUndo();
	удали(l, h - l);
	cursor = l;
	anchor = -1;
	sc = 0;
	return true;
}

void EditField::копируй()
{
	int l, h;
	if(password) return;
	if(!дайВыделение(l, h)) {
		l = 0;
		h = text.дайДлину();
	}
	WriteClipboardUnicodeText(text.середина(l, h - l));
}

int EditField::вставь(int pos, const ШТкст& itext)
{
	if(толькочтен_ли()) return 0;
	ШТкст ins;
	const wchar *s = itext;
	for(;;) {
		wchar chr = *s++;
		int count = 1;
		if(chr == '\t') {
			count = 4;
			chr = ' ';
		}
		if(chr >= ' ') {
			chr = (*filter)(chr);
			if(chr) {
				chr = convert->фильтруй(chr);
				if(chr && (charset == CHARSET_UTF8 || изЮникода(chr, charset, 0)))
					ins.конкат(chr, count);
			}
		}
		else
			break;
	}
	if(ins.дайСчёт() + text.дайСчёт() > maxlen) {
		бипВосклицание();
		return 0;
	}
	text.вставь(pos, ins);
	Update();
	return ins.дайДлину();
}

void EditField::удали(int pos, int n)
{
	if(толькочтен_ли()) return;
	text.удали(pos, n);
	if(cursor >= text.дайДлину()) {
		cursor = text.дайДлину();
		SyncCaret();
	}
	Update();
}

void EditField::вставь(int chr)
{
	if(толькочтен_ли()) return;
	if(initcaps && cursor == 0 && text.дайСчёт() == 0)
		chr = взаг(chr);
	cursor += вставь(cursor, ШТкст(chr, 1));
	финиш();
}

void EditField::вставь(const ШТкст& s)
{
	if(!удалиВыделение()) SaveUndo();
	cursor += вставь(cursor, s);
	финиш();
}

void EditField::тягИБрос(Точка p, PasteClip& d)
{
	if(толькочтен_ли()) return;
	int c = дайКурсор(p.x + sc);
	if(AcceptText(d)) {
		SaveUndo();
		int sell, selh;
		ШТкст txt = дайШТкст(d);
		if(дайВыделение(sell, selh)) {
			if(c >= sell && c < selh) {
				if(!толькочтен_ли())
					удалиВыделение();
				if(IsDragAndDropSource())
					d.SetAction(DND_COPY);
				c = sell;
			}
			else
			if(d.GetAction() == DND_MOVE && IsDragAndDropSource()) {
				if(c > sell)
					c -= selh - sell;
				if(!толькочтен_ли())
					удалиВыделение();
				d.SetAction(DND_COPY);
			}
		}
		int count = вставь(c, txt);
		устФокус();
		устВыделение(c, c + count);
		Action();
		return;
	}
	if(!d.IsAccepted()) return;
	Прям dc(0, 0, 0, 0);
	if(c >= 0) {
		int x = дайКаретку(c);
		dc = RectC(x - sc + 2 - font.GetRightSpace('o'), GetTy(),
		           1, min(дайРазм().cy - 2 * GetTy(), font.GetCy()));
	}
	if(dc != dropcaret) {
		освежи(dropcaret);
		dropcaret = dc;
		освежи(dropcaret);
	}
}

void EditField::тягПовтори(Точка p)
{
	if(толькочтен_ли())
		return;
	Размер sz = дайРазм();
	int sd = min(sz.cx / 6, 16);
	int d = 0;
	if(p.x < sd)
		d = -3;
	if(p.x > sz.cx - sd)
		d = 3;
	int a = minmax((int)sc + minmax(d, -16, 16), 0, max(0, дайКаретку(дайДлину()) - sz.cx + 2));
	if(a != sc) {
		sc = a;
		освежи();
		SyncCaret();
	}
}

void EditField::тягПокинь()
{
	освежи(dropcaret);
	dropcaret.очисть();
}

void EditField::леваяТяг(Точка p, dword flags)
{
	if(password)
		return;
	int c = дайКурсор(p.x + sc);
	Размер ssz = StdSampleSize();
	int sell, selh;
	if(!HasCapture() && дайВыделение(sell, selh) && c >= sell && c <= selh) {
		ШТкст sel = text.середина(sell, selh - sell);
		ImageDraw iw(ssz);
		iw.DrawText(0, 0, sel);
		iw.Alpha().DrawRect(ssz, чёрный);
		iw.Alpha().DrawText(0, 0, sel, StdFont(), белый);
		ВекторМап<Ткст, ClipData> data;
		приставь(data, sel);
		bool oks = keep_selection;
		keep_selection = true;
		if(DoDragAndDrop(data, iw) == DND_MOVE && !толькочтен_ли()) {
			CancelSelection();
			SaveUndo();
			удали(sell, selh - sell);
			sc = 0;
			финиш();
			Action();
		}
		keep_selection = oks;
	}
}

Ткст EditField::GetSelectionData(const Ткст& fmt) const
{
	if(password) return Ткст();
	if(fsell >= 0 && fselh >= 0 && fsell <= text.дайСчёт() && fselh <= text.дайСчёт())
		return GetTextClip(text.середина(fsell, fselh - fsell), fmt);
	return Ткст();
}

void EditField::Undo()
{
	if(!редактируем_ли())
		return;
	разверни(undotext, text);
	разверни(undoanchor, anchor);
	разверни(undocursor, cursor);
	anchor = -1;
	UpdateAction();
	финиш();
}

void EditField::вырежь()
{
	if(!редактируем_ли())
		return;
	копируй();
	удалиВыделение();
	Action();
	финиш();
}

void EditField::StdPasteFilter(ШТкст&)
{
}

void EditField::Paste()
{
	if(!редактируем_ли())
		return;
	ШТкст w = ReadClipboardUnicodeText();
	WhenPasteFilter(w);
	вставь(w);
	Action();
	финиш();
}

void EditField::Erase()
{
	if(!редактируем_ли())
		return;
	if(!выделение_ли())
		выбериВсе();
	удалиВыделение();
	финиш();
}

void EditField::выбериВсе()
{
	устВыделение();
	финиш();
}

void EditField::StdBar(Бар& menu) {
	menu.добавь(редактируем_ли(), t_("Undo"), THISBACK(Undo))
		.Ключ(K_ALT_BACKSPACE)
		.Ключ(K_CTRL_Z);
	menu.Separator();
	menu.добавь(редактируем_ли() && выделение_ли(), t_("вырежь"), CtrlImg::cut(), THISBACK(вырежь))
		.Ключ(K_SHIFT_DELETE)
		.Ключ(K_CTRL_X);
	menu.добавь(выделение_ли(), t_("копируй"), CtrlImg::copy(), THISBACK(копируй))
		.Ключ(K_CTRL_INSERT)
		.Ключ(K_CTRL_C);
	menu.добавь(редактируем_ли() && IsClipboardAvailableText(), t_("Paste"), CtrlImg::paste(), THISBACK(Paste))
		.Ключ(K_SHIFT_INSERT)
		.Ключ(K_CTRL_V);
	menu.добавь(редактируем_ли(), t_("Erase"), CtrlImg::remove(), THISBACK(Erase))
		.Ключ(K_DELETE);
	menu.Separator();
	menu.добавь(дайДлину(), t_("выдели all"), CtrlImg::select_all(), THISBACK(выбериВсе))
		.Ключ(K_CTRL_A);
}

void EditField::праваяВнизу(Точка p, dword keyflags)
{
	keep_selection = true;
	Ук<EditField> self = this;
	БарМеню::выполни(WhenBar);
	if(self) { // protect from destruction when in menu modal loop
		устФокус();
		keep_selection = false;
	}
}

bool EditField::Ключ(dword ключ, int rep)
{
	int q;
	bool h;
	Ткст s;
	bool select = ключ & K_SHIFT;
	switch(ключ & ~K_SHIFT) {
	case K_LEFT:
		Move(cursor - 1, select);
		return true;
	case K_CTRL_LEFT:
		Move((int)GetPrevWord(cursor), select);
		return true;
	case K_CTRL_RIGHT:
		Move((int)GetNextWord(cursor), select);
		return true;
	case K_RIGHT:
		Move(cursor + 1, select);
		return true;
	case K_HOME:
		Move(0, select);
		return true;
	case K_END:
		Move(text.дайДлину(), select);
		return true;
	}
	if(!редактируем_ли())
		return БарМеню::скан(WhenBar, ключ);
	switch(ключ) {
	case K_BACKSPACE:
	case K_SHIFT|K_BACKSPACE:
		if(удалиВыделение()) {
			Action();
			break;
		}
		if(cursor == 0 || толькочтен_ли()) return true;
		SaveUndo();
		cursor--;
		удали(cursor, 1);
		Action();
		break;
	case K_CTRL_BACKSPACE:
		if(удалиВыделение()) {
			Action();
			break;
		}
		if(cursor == 0 || толькочтен_ли()) return true;
		SaveUndo();
		q = cursor;
		h = IsWCh(text[--cursor]);
		while(cursor > 0 && IsWCh(text[cursor - 1]) == h)
			cursor--;
		удали(cursor, q - cursor);
		Action();
		break;
	case K_DELETE:
		if(удалиВыделение()) {
			Action();
			break;
		}
		if(cursor >= text.дайДлину()) return true;
		SaveUndo();
		удали(cursor, 1);
		Action();
		break;
	case K_CTRL_DELETE:
		if(удалиВыделение()) {
			Action();
			break;
		}
		if(cursor >= text.дайДлину()) return true;
		q = cursor;
		h = IsWCh(text[q]);
		while(IsWCh(text[q]) == h && q < text.дайДлину()) q++;
		SaveUndo();
		удали(cursor, q - cursor);
		Action();
		break;
	case K_ENTER:
		if(WhenEnter) {
			WhenEnter();
			return true;
		}
		return false;
	default:
		if(ключ >= ' ' && ключ < K_CHAR_LIM || ключ == K_SHIFT_SPACE) {
			if(!удалиВыделение()) SaveUndo();
			while(rep--)
				вставь(ключ == K_SHIFT_SPACE ? ' ' : ключ);
			Action();
			return true;
		}
		else
			return БарМеню::скан(WhenBar, ключ);
	}
	финиш();
	return true;
}

void EditField::устТекст(const ШТкст& txt)
{
	if(text == txt) {
		Update();
		return;
	}
	text = txt;
	sc = 0;
	if(естьФокус()) {
		cursor = txt.дайДлину();
		anchor = 0;
	}
	else {
		cursor = 0;
		anchor = -1;
	}
	Update();
	финиш();
}

void EditField::устДанные(const Значение& data)
{
	const Преобр * cv = convert;
	if(!естьФокус() && inactive_convert)
		cv = inactive_convert;
	устТекст((ШТкст) cv->фмт(data));
}

Значение EditField::дайДанные() const
{
	return convert->скан(text);
}

void EditField::очисть()
{
	устТекст(ШТкст());
	sc = cursor = 0;
}

void EditField::переустанов()
{
	очисть();
	ClearModify();
	sc = 0;
	cursor = 0;
	anchor = -1;
	password = false;
	autoformat = true;
	clickselect = false;
	filter = CharFilterUnicode;
	convert = &NoConvert();
	inactive_convert = NULL;
	initcaps = false;
	maxlen = INT_MAX;
	autosize = false;
	keep_selection = false;
	errorbg = nobg = false;
	charset = CHARSET_UTF8;
	alignright = false;
	устСтиль(дефСтиль());
	устФрейм(edge);
	font = StdFont();
	textcolor = Null;
	showspaces = false;
	no_internal_margin = false;
	fsell = fselh = -1;
}

EditField& EditField::устШрифт(Шрифт _font)
{
	font = _font;
	финиш(true);
	return *this;
}

EditField& EditField::устЦвет(Цвет c)
{
	textcolor = c;
	освежи();
	return *this;
}

EditField& EditField::NullText(const Рисунок& icon, const char *text, Шрифт fnt, Цвет ink)
{
	nullicon = icon;
	nulltext = text;
	nulltext << " ";
	nullink = ink;
	nullfont = fnt;
	освежи();
	return *this;
}

EditField& EditField::NullText(const Рисунок& icon, const char *text, Цвет ink)
{
	return NullText(icon, text, дайШрифт().Italic(), ink);
}

EditField& EditField::NullText(const char *text, Шрифт fnt, Цвет ink)
{
	return NullText(Null, text, fnt, ink);
}

EditField& EditField::NullText(const char *text, Цвет ink)
{
	return NullText(text, дайШрифт().Italic(), ink);
}

EditField::EditField()
{
	dropcaret = Прям(0, 0, 0, 0);
	Unicode();
	переустанов();
	WhenBar = THISBACK(StdBar);
}

EditField::~EditField() {}

}
