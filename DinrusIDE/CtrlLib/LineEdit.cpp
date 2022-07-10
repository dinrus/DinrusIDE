#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x) //  LOG(x)
#define LTIMING(x) // DTIMING(x)

СтрокРедакт::СтрокРедакт() {
	isdrag = false;
	nohbar = false;
	showtabs = false;
	tabsize = 4;
	font = CourierZ(12);
	SyncFont();
	устФрейм(ViewFrame());
	sb.безБокса();
	добавьФрейм(sb);
	sb.ПриПромоте = THISBACK(промотай);
	cutline = true;
	bordercolumn = -1;
	bordercolor = Null;
	overwrite = false;
	filter = NULL;
	showspaces = false;
	showlines = false;
	showreadonly = true;
	dorectsel = false;
	hline = vline = Null;
	vlinex = 0;
	warnwhitespace = false;
}

СтрокРедакт::~СтрокРедакт() {}

Шрифт СтрокРедакт::GetPreeditFont()
{
	return font;
}

void СтрокРедакт::колесоМыши(Точка, int zdelta, dword keyflags) {
	if(keyflags & K_SHIFT)
		sb.колесоХ(zdelta);
	else
		sb.колесоУ(zdelta);
}

void   СтрокРедакт::очисть() {
	gcolumn = 0;
	ТекстКтрл::очисть();
	sb.устВсего(0, 0);
	sb.уст(0, 0);
	NewScrollPos();
	поместиКаретку(0, false);
}

СтрокРедакт& СтрокРедакт::TabSize(int n) {
	tabsize = n;
	PlaceCaret0(GetColumnLine(cursor));
	освежи();
	return *this;
}

СтрокРедакт& СтрокРедакт::BorderColumn(int col, Цвет c)
{
	bordercolumn = col;
	bordercolor = c;
	освежи();
	return *this;
}

СтрокРедакт& СтрокРедакт::устШрифт(Шрифт f) {
	font = f;
	Выкладка();
	TabSize(tabsize);
	SetSb();
	return *this;
}

void СтрокРедакт::SyncFont()
{
	fsz = GetFontSize();
}

Размер СтрокРедакт::GetFontSize() const {
	return Размер(font.GetMonoWidth(), font.GetCy());
}

bool СтрокРедакт::IsDoubleChar(int ch) const
{
	return ch >= 2048 && (IsDoubleWidth(ch) || font.дайШирину(ch) > fsz.cx);
}

void СтрокРедакт::SetRectSelection(int64 anchor, int64 cursor)
{
	dorectsel = true;
	устВыделение(anchor, cursor);
	dorectsel = false;
}

void СтрокРедакт::SetRectSelection(const Прям& rect)
{
	SetRectSelection(GetGPos(rect.top, rect.left), GetGPos(rect.bottom, rect.right));
}

Прям СтрокРедакт::GetRectSelection() const
{
	if(IsRectSelection()) {
		int64 sell, selh;
		дайВыделение(sell, selh);
		Прям r(GetColumnLine(sell), GetColumnLine(selh));
		if(r.left > r.right)
			разверни(r.left, r.right);
		return r;
	}
	return Null;
}

bool СтрокРедакт::GetRectSelection(const Прям& rect, int line, int64& l, int64& h)
{
	if(line >= rect.top && line <= rect.bottom) {
		l = GetGPos(line, rect.left);
		h = GetGPos(line, rect.right);
		return true;
	}
	l = h = 0;
	return false;
}

int СтрокРедакт::RemoveRectSelection()
{
	Прям rect = GetRectSelection();
	ШТкст txt;
	for(int i = rect.top; i <= rect.bottom; i++) {
		int64 l, h;
		CacheLinePos(i);
		GetRectSelection(rect, i, l, h);
		ШТкст s = дайШСтроку(i);
		s.удали(int(l - дайПоз64(i)), int(h - l));
		txt.конкат(s);
		txt.конкат('\n');
	}
	int l = дайПоз32(rect.top);
	int h = дайПоз32(rect.bottom) + дайДлинуСтроки(rect.bottom);
	if(h < дайДлину32())
		h++;
	удали((int)l, int(h - l));
	вставь((int)l, txt);
	return (int)GetGPos(rect.bottom, rect.left);
}

ШТкст СтрокРедакт::CopyRectSelection()
{
	ШТкст txt;
	Прям rect = GetRectSelection();
	for(int i = rect.top; i <= rect.bottom && txt.дайСчёт() < max_total; i++) {
		int64 l, h;
		CacheLinePos(i);
		int64 pos = дайПоз64(i);
		GetRectSelection(rect, i, l, h);
		txt.конкат(дайШСтроку(i).середина(int(l - pos), int(h - l)));
#ifdef PLATFORM_WIN32
		txt.конкат('\r');
#endif
		txt.конкат('\n');
	}
	return txt;
}

int СтрокРедакт::PasteRectSelection(const ШТкст& s)
{
	Вектор<ШТкст> cl = разбей(s, '\n', false);
	Прям rect = GetRectSelection();
	int64 pos = cursor;
	int n = 0;
	for(int i = 0; i < cl.дайСчёт() && rect.top + i <= rect.bottom; i++) {
		int64 l, h;
		CacheLinePos(i);
		GetRectSelection(rect, i + rect.top, l, h);
		удали((int)l, int(h - l));
		int nn = вставь((int)l, cl[i]);
		n += nn;
		pos = l + nn;
	}
	поместиКаретку(pos);
	return n;
}

void СтрокРедакт::PasteColumn(const ШТкст& text)
{
	Вектор<ШТкст> cl = разбей(text, '\n', false);
	if(cl.дайСчёт() && cl.верх().пустой())
		cl.сбрось();
	if(cl.дайСчёт() == 0)
		return;
	int pos;
	if(IsRectSelection()) {
		Прям t = GetRectSelection();
		удалиВыделение();
		Точка p = t.верхЛево();
		pos = (int)cursor;
		for(int i = 0; i < t.bottom - t.top + 1; i++) {
			CacheLinePos(i + p.y);
			int l = (int)GetGPos(i + p.y, p.x);
			pos = l + вставь(l, cl[i % cl.дайСчёт()]);
		}
	}
	else {
		удалиВыделение();
		Точка p = GetColumnLine(cursor);
		pos = (int)cursor;
		for(int i = 0; i < cl.дайСчёт(); i++) {
			CacheLinePos(i + p.y);
			int li = p.y + i;
			if(li < дайСчётСтрок()) {
				int l = (int)GetGPos(i + p.y, p.x);
				pos = l + вставь(l, cl[i]);
			}
			else {
				вставь(дайДлину32(), cl[i] + "\n");
				pos = дайДлину32();
			}
		}
	}
	поместиКаретку(pos);
}

void СтрокРедакт::PasteColumn()
{
	ШТкст w = ReadClipboardUnicodeText();
	if(w.пустой())
		w = ReadClipboardText().вШТкст();
	PasteColumn(w);
	Action();
}

bool sSortLineOrder(const ШТкст& l1, const ШТкст& l2)
{
	return взаг(l1) < взаг(l2);
}

void СтрокРедакт::сортируй()
{
	if(!IsRectSelection())
		return;
	CopyRectSelection();
	Прям rect = GetRectSelection();
	Вектор<ШТкст> ключ;
	Вектор<ШТкст> ln;
	for(int i = rect.top; i <= rect.bottom; i++) {
		int64 l, h;
		GetRectSelection(rect, i, l, h);
		ключ.добавь(дайШ((int)l, int(h - l)));
		ln.добавь(дайШСтроку(i));
	}
	int sell = дайПоз32(rect.top);
	int selh = rect.bottom + 1 < дайСчётСтрок() ? дайПоз32(rect.bottom + 1) : дайДлину32();
	IndexSort(ключ, ln, sSortLineOrder);
	удали(sell, selh - sell);
	вставь(sell, Join(ln, "\n"));
}

class sOptimizedRectRenderer {
	Draw& w;
	Прям  cr;
	Цвет color;

public:
	void DrawRect(const Прям& r, Цвет color);
	void DrawRect(int x, int y, int cx, int cy, Цвет color) { DrawRect(RectC(x, y, cx, cy), color); }
	void слей();

	sOptimizedRectRenderer(Draw& w) : w(w) { cr = Null; color = Null; }
	~sOptimizedRectRenderer()              { слей(); }
};

void sOptimizedRectRenderer::слей()
{
	LTIMING("RectFlush");
	if(!пусто_ли(cr)) {
		w.DrawRect(cr, color);
		cr = Null;
	}
}

void sOptimizedRectRenderer::DrawRect(const Прям& r, Цвет c)
{
	LTIMING("DrawRect");
	if(cr.top == r.top && cr.bottom == r.bottom && cr.right == r.left && c == color) {
		cr.right = r.right;
		return;
	}
	слей();
	cr = r;
	color = c;
}

#if 1 // This is a more ambitious approach combining non-continual chunks of text, it is a bit faster...
class sOptimizedTextRenderer {
	Draw&       w;
	int         y;
	struct Chrs : Движимое<Chrs> {
		Вектор<int> x;
		Вектор<int> width;
		ШТкст     text;
	};
	ВекторМап< Tuple2<Шрифт, Цвет>, Chrs > cache;

public:
	void DrawChar(int x, int y, int chr, int width, Шрифт afont, Цвет acolor);
	void слей();

	sOptimizedTextRenderer(Draw& w) : w(w) { y = Null; }
	~sOptimizedTextRenderer()              { слей(); }
};

void sOptimizedTextRenderer::слей()
{
	if(cache.дайСчёт() == 0)
		return;
	LTIMING("слей");
	for(int i = 0; i < cache.дайСчёт(); i++) {
		Chrs& c = cache[i];
		if(c.x.дайСчёт()) {
			Tuple2<Шрифт, Цвет> fc = cache.дайКлюч(i);
			int x = c.x[0];
			for(int i = 0; i < c.x.дайСчёт() - 1; i++)
				c.x[i] = c.x[i + 1] - c.x[i];
			c.x.верх() = c.width.верх();
			w.DrawText(x, y, c.text, fc.a, fc.b, c.x);
		}
	}
	cache.очисть();
}

void sOptimizedTextRenderer::DrawChar(int x, int _y, int chr, int width, Шрифт font, Цвет color)
{
	LTIMING("DrawChar");
	if(y != _y) {
		слей();
		y = _y;
	}
	Chrs *c;
	{
		LTIMING("вКарту");
		c = &cache.дайДобавь(сделайКортеж(font, color));
	}
	if(c->x.дайСчёт() && c->x.верх() > x || font.IsUnderline() && cache.дайСчёт() > 1) {
		слей();
		c = &cache.дайДобавь(сделайКортеж(font, color));
	}
	c->text.конкат(chr);
	c->width.добавь(width);
	c->x.добавь(x);
}
#else
class sOptimizedTextRenderer {
	Draw&       w;
	int         x, y;
	int         xpos;
	Вектор<int> dx;
	ШТкст     text;
	Шрифт        font;
	Цвет       color;

public:
	void DrawChar(int x, int y, int chr, int width, Шрифт afont, Цвет acolor);
	void слей();

	sOptimizedTextRenderer(Draw& w) : w(w) { y = Null; }
	~sOptimizedTextRenderer()              { слей(); }
};

void sOptimizedTextRenderer::слей()
{
	if(text.дайСчёт() == 0)
		return;
	LTIMING("слей");
	w.DrawText(x, y, text, font, color, dx);
	y = Null;
	text.очисть();
	dx.очисть();
}

void sOptimizedTextRenderer::DrawChar(int _x, int _y, int chr, int width, Шрифт _font, Цвет _color)
{
	LTIMING("DrawChar");
	if(y == _y && font == _font && color == _color && dx.дайСчёт() && _x >= xpos - dx.верх())
		dx.верх() += _x - xpos;
	else {
		LTIMING("DrawChar flush");
		слей();
		x = _x;
		y = _y;
		font = _font;
		color = _color;
	}
	dx.добавь(width);
	text.конкат(chr);
	xpos = _x + width;
}
#endif

void   СтрокРедакт::рисуй0(Draw& w) {
	LTIMING("СтрокРедакт::рисуй0");
	ЗамкниГип __;
	int64 sell, selh;
	дайВыделение(sell, selh);
	if(!включен_ли())
		sell = selh = 0;
	Прям rect(0, 0, 0, 0);
	bool rectsel = IsRectSelection();
	if(rectsel)
		rect = GetRectSelection();
	Размер sz = дайРазм();
	Точка sc = sb;
	int ll = min(дайСчётСтрок(), sz.cy / fsz.cy + sc.y + 1);
	int  y = 0;
	sc.y = minmax(sc.y, 0, дайСчётСтрок() - 1);
	cpos = дайПоз64(sc.y);
	cline = sc.y;
	sell -= cpos;
	selh -= cpos;
	int64 pos = cpos;
	int fascent = font.Info().GetAscent();
	int cursorline = дайСтроку(cursor);
	Highlight ih;
	ih.ink = color[IsShowEnabled() ? INK_NORMAL : INK_DISABLED];
	ih.paper = color[толькочтен_ли() && showreadonly || !IsShowEnabled() ? PAPER_READONLY : PAPER_NORMAL];
	if(nobg)
		ih.paper = Null;
	ih.font = font;
	ih.chr = 0;
	for(int i = sc.y; i < ll; i++) {
		Цвет showcolor = color[WHITESPACE];
		ШТкст tx = дайШСтроку(i);
		bool warn_whitespace = false;
		if(warnwhitespace && !выделение_ли()) {
			int64 pos = дайКурсор64();
			int linei = дайПозСтроки64(pos);
			if(linei != i || pos < tx.дайСчёт()) {
				int wkind = 0;
				bool empty = true;
				for(const wchar *s = tx; *s; s++) {
					if(*s == '\t') {
						if(wkind == ' ') {
							warn_whitespace = true;
							break;
						}
						wkind = '\t';
					}
					else
					if(*s == ' ')
						wkind = ' ';
					else
					if(*s > ' ') {
						empty = false;
						wkind = 0;
					}
				}
				if(wkind && !empty)
					warn_whitespace = true;
				if(warn_whitespace)
					showcolor = color[WARN_WHITESPACE];
			}
		}
		bool do_highlight = tx.дайСчёт() < 100000;
		int len = tx.дайДлину();
		if(w.IsPainting(0, y, sz.cx, fsz.cy)) {
			LTIMING("PaintLine");
			Вектор<Highlight> hl;
			int ln;
			if(do_highlight) {
				hl.устСчёт(len + 1, ih);
				for(int q = 0; q < tx.дайСчёт(); q++)
					hl[q].chr = tx[q];
				LTIMING("HighlightLine");
				HighlightLine(i, hl, pos);
				ln = hl.дайСчёт() - 1;
			}
			else
				ln = tx.дайСчёт();
			int lgp = -1;
			for(int pass = 0; pass < 3; pass++) {
				int gp = 0;
				int scx = fsz.cx * sc.x;
				sOptimizedRectRenderer rw(w);
				if(ln >= 0) {
					int q = 0;
					int x = 0;
					int scx2 = scx - max(2, tabsize) * fsz.cx;
					while(q < ln && x < scx2) { // пропусти part before left border
						wchar chr = do_highlight ? hl[q++].chr : tx[q++];
						if(chr == '\t') {
							gp = (gp + tabsize) / tabsize * tabsize;
							x = fsz.cx * gp;
						}
						else
						if(IsDoubleChar(chr)) {
							x += 2 * fsz.cx;
							gp += 2;
						}
						else {
							x += fsz.cx;
							gp++;
						}
					}
					sOptimizedTextRenderer tw(w);
					Highlight lastHighlight;
					while(q < ln) {
						if(q == tx.дайСчёт())
							lgp = gp;
						Highlight h;
						if(do_highlight)
							h = hl[q];
						else {
							h = ih;
							h.chr = tx[q];
						}
						int pos = min(q, len); // Highligting can add chars at the end of line
						if(rectsel ? i >= rect.top && i <= rect.bottom && gp >= rect.left && gp < rect.right
						           : pos >= sell && pos < selh) {
							h.paper = color[PAPER_SELECTED];
							h.ink = color[INK_SELECTED];
						}
						int x = gp * fsz.cx - scx;
						bool cjk = IsDoubleChar(h.chr);
						int xx = x + (gp + 1 + cjk) * fsz.cx;
						if(h.chr == '\t') {
							int ngp = (gp + tabsize) / tabsize * tabsize;
							int l = ngp - gp;
							LLOG("Highlight -> tab[" << q << "] paper = " << h.paper);
							if(x >= -fsz.cy * tabsize) {
								if(pass == 0) {
									rw.DrawRect(x, y, fsz.cx * l, fsz.cy, h.paper);
									if((showtabs || warn_whitespace) &&
									   h.paper != SColorHighlight && q < tx.дайДлину()) {
										rw.DrawRect(x + 2, y + fsz.cy / 2, l * fsz.cx - 4, 1, showcolor);
										rw.DrawRect(ngp * fsz.cx - scx - 3, y + 3, 1, fsz.cy - 6, showcolor);
									}
									if(bordercolumn > 0 && bordercolumn >= gp && bordercolumn < gp + l)
										rw.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
								}
								if(pass == 2) // resolve underlined tabs
									tw.DrawChar(x, y, ' ', fsz.cx * l, h.font, h.ink);
							}
							q++;
							gp = ngp;
						}
						else
						if(h.chr == ' ') {
							LLOG("Highlight -> space[" << q << "] paper = " << h.paper);
							if(x >= -fsz.cy) {
								if(pass == 0) {
									rw.DrawRect(x, y, fsz.cx, fsz.cy, h.paper);
									if((showspaces || warn_whitespace)
									   && h.paper != SColorHighlight && q < tx.дайДлину()) {
										int n = fsz.cy / 10 + 1;
										rw.DrawRect(x + fsz.cx / 2, y + fsz.cy / 2, n, n, showcolor);
									}
									if(bordercolumn > 0 && bordercolumn >= gp && bordercolumn < gp + 1)
										rw.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
								}
								if(pass == 2) // resolve underlined пробелы
									tw.DrawChar(x, y, ' ', fsz.cx, h.font, h.ink);
							}
							q++;
							gp++;
						}
						else {
							LLOG("Highlight -> paper[" << q << "] = " << h.paper);
							if(max(x, 0) < min(xx, sz.cx) && fsz.cx >= -fsz.cy) {
								if(pass == 0) {
									rw.DrawRect(x, y, (cjk + 1) * fsz.cx, fsz.cy, h.paper);
									if(bordercolumn > 0 && bordercolumn >= gp && bordercolumn < gp + 1 + cjk)
										rw.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
								}
								if(pass == 1 && (h.flags & SPELLERROR))
									rw.DrawRect(x, max(y, y + fsz.cy - Zy(1)), (cjk + 1) * fsz.cx, Zy(1), светлоКрасный());
								if(pass == 2)
									tw.DrawChar(x + (h.flags & SHIFT_L ? -fsz.cx / 6 : h.flags & SHIFT_R ? fsz.cx / 6 : 0),
									            y + fascent - h.font.GetAscent(),
									            h.chr, (cjk + 1) * fsz.cx, h.font, h.ink);
							}
							q++;
							gp += 1 + cjk;
							if(x > sz.cx)
								break;
						}
					}
				}
				if(pass == 0) {
					int gpx = gp * fsz.cx - scx;
					rw.DrawRect(gpx, y, sz.cx - gpx, fsz.cy,
					            !rectsel && sell <= len && len < selh ? color[PAPER_SELECTED]
					            : (do_highlight ? hl.верх() : ih).paper);
					if(bordercolumn > 0 && bordercolumn >= gp)
						rw.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
					if((showlines || warn_whitespace)) {
						int yy = 2 * fsz.cy / 3;
						int x = (lgp >= 0 ? lgp : gp) * fsz.cx - scx;
						rw.DrawRect(x, y + yy, fsz.cx / 2, 1, showcolor);
						if(fsz.cx > 2)
							rw.DrawRect(x + 1, y + yy - 1, 1, 3, showcolor);
						if(fsz.cx > 5)
							rw.DrawRect(x + 2, y + yy - 2, 1, 5, showcolor);
						rw.DrawRect(x + fsz.cx / 2, y + yy / 2, 1, yy - yy / 2, showcolor);
					}
					if(sell == selh) {
						if(!пусто_ли(hline) && i == cursorline) {
							rw.DrawRect(0, y, sz.cx, 1, hline);
							rw.DrawRect(0, y + fsz.cy - 1, sz.cx, 1, hline);
						}
						if(!пусто_ли(vline))
							rw.DrawRect(caretpos.x, y, 1, fsz.cy, vline);
					}
					if(rectsel && rect.left == rect.right && i >= rect.top && i <= rect.bottom)
						rw.DrawRect(rect.left * fsz.cx - scx, y, 2, fsz.cy, смешай(color[PAPER_SELECTED], color[PAPER_NORMAL]));
				}
			}
		}
		y += fsz.cy;
		sell -= len + 1;
		selh -= len + 1;
		pos += len + 1;
	}
	w.DrawRect(0, y, sz.cx, sz.cy - y, color[толькочтен_ли() && showreadonly || !IsShowEnabled() ? PAPER_READONLY : PAPER_NORMAL]);
	DrawTiles(w, DropCaret(), CtrlImg::checkers());
	vlinex = caretpos.x;
}

void СтрокРедакт::рисуй(Draw& w)
{
	рисуй0(w);
	scroller.уст(sb);
}

struct СтрокРедакт::RefreshDraw : public NilDraw {
	Ктрл  *ctrl;
	bool (*chars)(int c);
	Размер   fsz;
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт,
	                        Цвет, int n, const int *dx) {
		if(dx)
			while(n > 0) {
				if((*chars)(*text))
					ctrl->освежи(x, y, fsz.cx, fsz.cy);
				text++;
				x += *dx++;
				n--;
			}
	}
	bool IsPaintingOp(const Прям& r) const {
		return true;
	}
};

void СтрокРедакт::RefreshChars(bool (*chars)(int c))
{
	RefreshDraw rw;
	rw.ctrl = this;
	rw.fsz = GetFontSize();
	rw.chars = chars;
	рисуй(rw);
}

void   СтрокРедакт::Выкладка() {
	SyncFont();
	Размер sz = sb.дайРедуцРазмОбзора();
	if(nohbar || isdrag)
		sz.cy = дайРазм().cy;
	sb.устСтраницу(sz / GetFontSize());
	SetHBar();
}

int64  СтрокРедакт::GetGPos(int ln, int cl) const {
	ln = minmax(ln, 0, дайСчётСтрок() - 1);
	Ткст h = дайУтф8Строку(ln);
	const char *s = h.begin();
	const char *e = h.end();
	const char *b = s;
	int gl = 0;
	int wpos = 0;
	while(s < e) {
		if(*s == '\t')
			gl = (gl + tabsize) / tabsize * tabsize;
		else
		if((byte)*s < 128)
			gl++;
		else {
			ШТкст txt = вУтф32(s, int(e - s));
			const wchar *b = txt;
			const wchar *e = txt.стоп();
			const wchar *s = b;
			while(s < e) {
				if(*s == '\t')
					gl = (gl + tabsize) / tabsize * tabsize;
				else
					gl += 1 + IsDoubleChar(*s);
				if(cl < gl) break;
				s++;
			}
			wpos = int(s - b);
			break;
		}
		if(cl < gl) break;
		s++;
	}
	
	return дайПоз64(ln, int(s - b) + wpos);
}

Точка СтрокРедакт::GetColumnLine(int64 pos) const {
	Точка p;
	if(pos > дайДлину64()) pos = дайДлину64();
	p.y = дайПозСтроки64(pos);
	p.x = 0;
	ШТкст txt = дайШСтроку(p.y);
	const wchar *s = txt;
	while(pos--) {
		if(*s == '\t')
			p.x = (p.x + tabsize) / tabsize * tabsize;
		else
			p.x += 1 + IsDoubleChar(*s);
		s++;
	}
	return p;
}

Точка СтрокРедакт::GetIndexLine(int64 pos) const
{
	Точка p;
	if(pos > дайДлину64()) pos = дайДлину64();
	p.y = дайПозСтроки64(pos);
	p.x = minmax((int)pos, 0, дайДлинуСтроки(p.y));
	return p;
}

int64 СтрокРедакт::GetIndexLinePos(Точка pos) const
{
	if(pos.y < 0)
		return 0;
	if(pos.y >= дайСчётСтрок())
		return дайДлину64();
	return дайПоз64(pos.y, minmax(pos.x, 0, дайДлинуСтроки(pos.y)));
}

void СтрокРедакт::освежиСтроку(int i) {
	Размер sz = дайРазм();
	int fcy = GetFontSize().cy;
	освежи(0, (i - sb.дай().y) * fcy, sz.cx, fcy);
}

Прям СтрокРедакт::GetLineScreenRect(int line) const {
	int fcy = GetFontSize().cy;
	Прям r = RectC(0, (line - sb.дай().y) * fcy, дайРазм().cx, fcy);
	r.смещение(GetScreenView().верхЛево());
	return r;
}

void СтрокРедакт::SetSb() {
	sb.устУВсего(дайСчётСтрок());
	SetHBar();
}

void СтрокРедакт::NewScrollPos() {}
void СтрокРедакт::HighlightLine(int line, Вектор<Highlight>& h, int64 pos) {}

void СтрокРедакт::AlignChar() {
	int c = дайКурсор32();
	if(c == 0)
		return;
	Точка pos = GetColumnLine(c);
	if(pos.x == 0)
		return;
	for(int d = 1; d <= pos.y && d < 100; d++) {
		int lny = pos.y - d;
		ШТкст above = дайШСтроку(lny);
		int offset = int(GetGPos(lny, pos.x) - дайПоз64(lny));
		int end = offset;
		char ch = дайСим(c - 1);
		if(ch == ' ')
		{
			offset++;
			while(end < above.дайДлину() && above[end] != ' ')
				end++;
			while(end < above.дайДлину() && above[end] == ' ')
				end++;
		}
		else
			while(end < above.дайДлину() && above[end] != ch)
				end++;
		if(end < above.дайДлину()) {
			int count = end - offset + 1;
			ШТкст s(' ', count);
			вставь(c - 1, s, true);
			устКурсор(c + count);
			return;
		}
	}
}

void СтрокРедакт::PlaceCaret0(Точка p) {
	Размер fsz = GetFontSize();
	p -= sb;
	caretpos = Точка(p.x * fsz.cx, p.y * fsz.cy);
	if(overwrite)
		устКаретку(caretpos.x, caretpos.y + fsz.cy - 2, fsz.cx, 2);
	else
		устКаретку(caretpos.x, caretpos.y, 2, fsz.cy);
}

int СтрокРедакт::PlaceCaretNoG(int64 newcursor, bool sel) {
	if(newcursor > дайДлину64()) newcursor = дайДлину64();
	Точка p = GetColumnLine(newcursor);
	if(sel) {
		if(anchor < 0) {
			anchor = cursor;
		}
		if(rectsel || rectsel != dorectsel)
			освежи();
		else
			освежиСтроки(p.y, дайСтроку(cursor));
		rectsel = dorectsel;
	}
	else {
		if(anchor >= 0) {
			if(rectsel || dorectsel)
				освежи();
			else
				освежиСтроки(дайСтроку(cursor), дайСтроку(anchor));
			anchor = -1;
		}
		rectsel = false;
	}
	освежиСтроку(GetColumnLine(cursor).y);
	освежиСтроку(p.y);
	cursor = newcursor;
	ScrollIntoCursor();
	PlaceCaret0(p);
	SelectionChanged();
	WhenSel();
	if(IsAnySelection())
		SetSelectionSource(ClipFmtsText());
	if(!пусто_ли(hline)) {
		Размер sz = дайРазм();
		освежи(vlinex, 0, 1, sz.cy);
		освежи(caretpos.x, 0, 1, sz.cy);
	}
	return p.x;
}

void СтрокРедакт::поместиКаретку(int64 newcursor, bool sel) {
	gcolumn = PlaceCaretNoG(newcursor, sel);
}

void СтрокРедакт::TopCursor(int lines)
{
	sb.устУ(max(0, дайСтроку(cursor) - lines));
}

void СтрокРедакт::курсорПоЦентру() {
	int cy = sb.дайСтраницу().cy;
	if(cy > 4)
		sb.устУ(max(min(дайСтроку(cursor) - cy / 2, дайСчётСтрок() - cy), 0));
}

void СтрокРедакт::промотай() {
	PlaceCaret0(GetColumnLine(cursor));
	scroller.промотай(*this, дайРазм(), sb.дай(), GetFontSize());
	SetHBar();
	NewScrollPos();
	ПриПромоте();
}

int64 СтрокРедакт::дайПозМыши(Точка p) const {
	Размер fsz = GetFontSize();
	p = (p + fsz.cx / 2 + fsz * (Размер)sb.дай()) / fsz;
	return GetGPos(p.y, p.x);
}

void СтрокРедакт::леваяВнизу(Точка p, dword flags) {
	mpos = дайПозМыши(p);
	int64 l, h;
	if(дайВыделение(l, h) && mpos >= l && mpos < h) {
		selclick = true;
		return;
	}
	dorectsel = flags & K_ALT;
	поместиКаретку(mpos, (flags & K_SHIFT) || dorectsel);
	dorectsel = false;
	устФокус();
	SetCapture();
}

void СтрокРедакт::леваяВверху(Точка p, dword flags)
{
	if(!HasCapture() && selclick && !IsDragAndDropSource()) {
		mpos = дайПозМыши(p);
		поместиКаретку(mpos, flags & K_SHIFT);
		устФокус();
	}
	selclick = false;
	ReleaseCapture();
}

void СтрокРедакт::праваяВнизу(Точка p, dword flags)
{
	mpos = дайПозМыши(p);
	устФокус();
	int64 l, h;
	дайВыделение(l, h);
	if(!IsAnySelection() || !(mpos >= l && mpos < h))
		поместиКаретку(mpos, false);
	БарМеню::выполни(WhenBar);
}

void СтрокРедакт::леваяДКлик(Точка, dword)
{
	int64 l, h;
	if(GetWordSelection(cursor, l, h))
		устВыделение(l, h);
}

void СтрокРедакт::LeftTriple(Точка, dword)
{
	int64 q = cursor;
	int i = дайПозСтроки64(q);
	q = cursor - q;
	устВыделение(q, q + дайДлинуСтроки(i) + 1);
}

void СтрокРедакт::двигМыши(Точка p, dword flags) {
	if((flags & K_MOUSELEFT) && естьФокус() && HasCapture()) {
		int64 c = дайПозМыши(p);
		dorectsel = flags & K_ALT;
		поместиКаретку(c, mpos != c || HasCapture());
		dorectsel = false;
	}
}

void СтрокРедакт::леваяПовтори(Точка p, dword flags) {
	if(HasCapture()) {
		int64 c = дайПозМыши(p);
		if(mpos != c) {
			dorectsel = flags & K_ALT;
			поместиКаретку(c, true);
			dorectsel = false;
		}
	}
}

Рисунок СтрокРедакт::рисКурсора(Точка, dword) {
	return Рисунок::IBeam();
}

void СтрокРедакт::MoveUpDown(int n, bool sel) {
	int64 cl = cursor;
	int ln = дайПозСтроки64(cl);
	if(ln + n >= дайСчётСтрок())
		ждиВид(ln + n);
	ln = minmax(ln + n, 0, дайСчётСтрок() - 1);
	PlaceCaretNoG(GetGPos(ln, gcolumn), sel);
}

void СтрокРедакт::MoveLeft(bool sel) {
	if(cursor)
		поместиКаретку(cursor - 1, sel);
}

void СтрокРедакт::MoveRight(bool sel) {
	if(cursor < дайДлину64())
		поместиКаретку(cursor + 1, sel);
}

void СтрокРедакт::MoveUp(bool sel) {
	MoveUpDown(-1, sel);
}

void СтрокРедакт::MoveDown(bool sel) {
	MoveUpDown(1, sel);
}

void СтрокРедакт::MovePage(int dir, bool sel) {
	int n = dir * max(дайРазм().cy / GetFontSize().cy - 2, 2);
	sb.устУ(Точка(sb).y + n);
	MoveUpDown(n, sel);
}

void СтрокРедакт::MovePageUp(bool sel) {
	MovePage(-1, sel);
}

void СтрокРедакт::MovePageDown(bool sel) {
	MovePage(1, sel);
}

inline bool sTabSpace(int c) { return c == '\t' || c == ' '; }

void СтрокРедакт::MoveHome(bool sel) {
	int64 cl = cursor;
	int li = дайПозСтроки64(cl);
	int i = 0;
	ШТкст l = дайШСтроку(li);
	while(sTabSpace(l[i]))
		i++;
	поместиКаретку(дайПоз64(li, cl == i ? 0 : i), sel);
}

void СтрокРедакт::MoveEnd(bool sel) {
	int i = дайСтроку(cursor);
	поместиКаретку(дайПоз64(i, дайДлинуСтроки(i)), sel);
}

void СтрокРедакт::двигайВНачТекста(bool sel) {
	поместиКаретку(0, sel);
}

void СтрокРедакт::двигайВКонТекста(bool sel) {
	ждиВид(INT_MAX, true);
	поместиКаретку(дайДлину64(), sel);
}

bool СтрокРедакт::InsertChar(dword ключ, int count, bool canow) {
	if(ключ == K_TAB && !processtab)
		return false;
	if(filter && ключ >= 32 && ключ < K_CHAR_LIM)
		ключ = (*filter)(ключ);
	if(!толькочтен_ли() && (ключ >= 32 && ключ < K_CHAR_LIM || ключ == '\t' || ключ == '\n' ||
	   ключ == K_ENTER && processenter || ключ == K_SHIFT_SPACE)) {
		if(ключ >= 128 && ключ < K_CHAR_LIM && (charset != CHARSET_UTF8 && charset != CHARSET_UTF8_BOM)
		   && изЮникода((wchar)ключ, charset) == DEFAULTCHAR)
			return true;
		if(!удалиВыделение() && overwrite && ключ != '\n' && ключ != K_ENTER && canow) {
			int64 q = cursor;
			int i = дайПозСтроки64(q);
			if(q + count - 1 < дайДлинуСтроки(i))
				удали((int)cursor, (int)count);
		}
		ШТкст text(ключ == K_ENTER ? '\n' : ключ == K_SHIFT_SPACE ? ' ' : ключ, count);
		вставь((int)cursor, text, true);
		поместиКаретку(cursor + count);
		Action();
		return true;
	}
	return false;
}

void СтрокРедакт::DeleteChar() {
	if(толькочтен_ли() || удалиВыделение()) {
		Action();
		return;
	}
	if(cursor < дайДлину32()) {
		удали((int)cursor, 1);
		Action();
	}
}

void СтрокРедакт::Backspace() {
	if(толькочтен_ли() || удалиВыделение() || cursor == 0) return;
	MoveLeft();
	DeleteChar();
	Action();
}

void СтрокРедакт::DeleteLine()
{
	int64 b, e;
	if(дайВыделение(b, e) && дайСтроку(b) != дайСтроку(e)) {
		удалиВыделение();
		return;
	}
	int i = дайСтроку(cursor);
	int p = дайПоз32(i);
	удали((int)p, дайДлинуСтроки(i) + 1);
	поместиКаретку(p);
	Action();
}

void СтрокРедакт::CutLine()
{
	if(толькочтен_ли()) return;
	int64 b, e;
	if(дайВыделение(b, e) && дайСтроку(b) != дайСтроку(e)) {
		вырежь();
		return;
	}
	int i = дайСтроку(cursor);
	int p = дайПоз32(i);
	ШТкст txt = дай(p, дайДлинуСтроки(i) + 1).вШТкст();
	WriteClipboardUnicodeText(txt);
	AppendClipboardText(txt.вТкст());
	очистьВыделение();
	DeleteLine();
}

СтрокРедакт::ПозРедакт СтрокРедакт::GetEditPos() const {
	ПозРедакт pos;
	pos.sby = sb.дай().y;
	pos.cursor = cursor;
	return pos;
}

void СтрокРедакт::SetEditPos(const СтрокРедакт::ПозРедакт& pos) {
	SetEditPosSbOnly(pos);
	устКурсор(pos.cursor);
}

void СтрокРедакт::SetEditPosSb(const СтрокРедакт::ПозРедакт& pos) {
	устКурсор(pos.cursor);
	SetEditPosSbOnly(pos);
}

void СтрокРедакт::SetEditPosSbOnly(const СтрокРедакт::ПозРедакт& pos) {
	sb.устУ(minmax(pos.sby, 0, дайСчётСтрок() - 1));
}

void СтрокРедакт::SetHBar()
{
	int mpos = 0;
	if(!nohbar && !isdrag) {
		int m = min(sb.y + sb.дайСтраницу().cy + 2, дайСчётСтрок());
		for(int i = sb.y; i < m; i++) {
			int pos = 0;
			ШТкст l = дайШСтроку(i);
			const wchar *s = l;
			const wchar *e = l.стоп();
			while(s < e) {
				if(*s == '\t')
					pos = (pos + tabsize) / tabsize * tabsize;
				else
					pos += 1 + IsDoubleChar(*s);
				s++;
			}
			mpos = max(mpos, pos);
		}
	}
	sb.устХВсего(mpos + 1);
}

void СтрокРедакт::ScrollIntoCursor()
{
	Точка p = GetColumnLine(дайКурсор64());
	sb.промотайДо(p);
	SetHBar();
	sb.промотайДо(p);
}

bool СтрокРедакт::Ключ(dword ключ, int count) {
	NextUndo();
	switch(ключ) {
	case K_CTRL_UP:
		ScrollUp();
		return true;
	case K_CTRL_DOWN:
		ScrollDown();
		return true;
	case K_INSERT:
		OverWriteMode(!IsOverWriteMode());
		break;
	}
	bool sel = ключ & K_SHIFT;
	dorectsel = ключ & K_ALT;
	dword k = ключ & ~K_SHIFT;
	if((ключ & (K_SHIFT|K_ALT)) == (K_SHIFT|K_ALT))
		k &= ~K_ALT;
	switch(k) {
	case K_CTRL_LEFT:
		{
			поместиКаретку(GetPrevWord(cursor), sel);
			break;
		}
	case K_CTRL_RIGHT:
		{
			поместиКаретку(GetNextWord(cursor), sel);
			break;
		}
	case K_LEFT:
		MoveLeft(sel);
		break;
	case K_RIGHT:
		MoveRight(sel);
		break;
	case K_HOME:
		MoveHome(sel);
		break;
	case K_END:
		MoveEnd(sel);
		break;
	case K_UP:
		MoveUp(sel);
		break;
	case K_DOWN:
		MoveDown(sel);
		break;
	case K_PAGEUP:
		MovePageUp(sel);
		break;
	case K_PAGEDOWN:
		MovePageDown(sel);
		break;
	case K_CTRL_PAGEUP:
	case K_CTRL_HOME:
		двигайВНачТекста(sel);
		break;
	case K_CTRL_PAGEDOWN:
	case K_CTRL_END:
		двигайВКонТекста(sel);
		break;
	case K_CTRL_C:
	case K_CTRL_INSERT:
		копируй();
		break;
	case K_CTRL_A:
		выбериВсе();
		break;
	default:
		dorectsel = false;
		if(толькочтен_ли())
			return БарМеню::скан(WhenBar, ключ);
		switch(ключ) {
		case K_DELETE:
			DeleteChar();
			break;
		case K_BACKSPACE:
		case K_SHIFT|K_BACKSPACE:
			Backspace();
			break;
		case K_SHIFT_TAB:
			AlignChar();
			break;
		case K_CTRL_Y:
		case K_CTRL_L:
			if(cutline) {
				CutLine();
				break;
			}
		default:
			if(InsertChar(ключ, count, true))
				return true;
			return БарМеню::скан(WhenBar, ключ);
		}
		return true;
	}
	dorectsel = false;
	синх();
	return true;
}

void СтрокРедакт::тягИБрос(Точка p, PasteClip& d)
{
	if(толькочтен_ли()) return;
	int c = GetMousePos32(p);
	if(AcceptText(d)) {
		NextUndo();
		int a = sb.y;
		int sell, selh;
		ШТкст text = дайШТкст(d);
		if(дайВыделение32(sell, selh)) {
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
		int count = вставь(c, text);
		sb.y = a;
		устФокус();
		устВыделение(c, c + count);
		Action();
		return;
	}
	if(!d.IsAccepted()) return;
	if(!isdrag) {
		isdrag = true;
		ScrollIntoCursor();
	}
	Точка dc = Null;
	if(c >= 0)
		dc = GetColumnLine(c);
	if(dc != dropcaret) {
		RefreshDropCaret();
		dropcaret = dc;
		RefreshDropCaret();
	}
}

Прям СтрокРедакт::DropCaret()
{
	if(пусто_ли(dropcaret))
		return Прям(0, 0, 0, 0);
	Размер fsz = GetFontSize();
	Точка p = dropcaret - sb;
	p = Точка(p.x * fsz.cx, p.y * fsz.cy);
	return RectC(p.x, p.y, 1, fsz.cy);
}

void СтрокРедакт::RefreshDropCaret()
{
	освежи(DropCaret());
}

void СтрокРедакт::тягПовтори(Точка p)
{
	sb.y = (int)sb.y + GetDragScroll(this, p, 1).y;
}

void СтрокРедакт::тягПокинь()
{
	RefreshDropCaret();
	dropcaret = Null;
	isdrag = false;
	Выкладка();
}

void СтрокРедакт::леваяТяг(Точка p, dword flags)
{
	int64 c = дайПозМыши(p);
	int64 l, h;
	if(!HasCapture() && дайВыделение(l, h) && c >= l && c < h) {
		ШТкст sample = дайШ(l, (int)min(h - l, (int64)3000));
		Размер sz = StdSampleSize();
		ImageDraw iw(sz);
		iw.DrawRect(sz, чёрный());
		iw.Alpha().DrawRect(sz, чёрный());
		DrawTLText(iw.Alpha(), 0, 0, 9999, sample, CourierZ(10), белый());
		NextUndo();
		if(DoDragAndDrop(ClipFmtsText(), iw) == DND_MOVE && !толькочтен_ли()) {
			удалиВыделение();
			Action();
		}
	}
}

}
