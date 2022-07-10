#include "CtrlLib.h"

namespace РНЦП {

void ДокРедакт::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	sb.Wheel(zdelta);
}

void ДокРедакт::очистьСтроки()
{
	para.очисть();
	ПРОВЕРЬ(дайСчётСтрок() == para.дайСчёт());
}

void ДокРедакт::вставьСтроки(int line, int count)
{
	para.вставь(line, Para(), count);
	ПРОВЕРЬ(дайСчётСтрок() == para.дайСчёт());
}

void ДокРедакт::удалиСтроки(int line, int count)
{
	para.удали(line, count);
	ПРОВЕРЬ(дайСчётСтрок() == para.дайСчёт());
}

ДокРедакт::Fmt ДокРедакт::фмт(const ШТкст& text) const
{
	FontInfo fi = font.Info();
	Fmt fmt;
	int tcx = fi['x'] * 4;
	fmt.len = text.дайДлину();
	fmt.text.размести(text.дайДлину());
	memcpy(fmt.text, text, text.дайДлину() * sizeof(wchar));
	fmt.width.размести(text.дайДлину());
	fmt.line.добавь(0);
	int *w = fmt.width;
	int x = 0;
	const wchar *space = NULL;
	int spacex = 0;
	for(wchar *s = fmt.text; s < fmt.text + fmt.len; s++) {
		int cw;
		if(*s == '\t')
			cw = (x + tcx) / tcx * tcx - x;
		else
			cw = fi[*s];
		*w++ = cw;
		if(*s == ' ' || *s == '\t') {
			space = s;
			spacex = x + cw;
			*s = ' ';
		}
		x += cw;
		if(x > cx) {
			if(space && space <= s) {
				space++;
				fmt.line.добавь(int(space - fmt.text));
				space = NULL;
				x -= spacex;
			}
			else {
				fmt.line.добавь(int(s - fmt.text));
				x = cw;
			}
		}
	}
	fmt.fi = fi;
	return fmt;
}

int  ДокРедакт::дайВысоту(int i) {
	Para& p = para[i];
	if(p.cx == cx) return p.cy;
	Fmt fmt = фмт(дайШСтроку(i));
	p.cx = cx;
	p.cy = fmt.line.дайСчёт() * (fmt.fi.дайВысоту()) + after;
	return p.cy;
}

int ДокРедакт::дайУ(int parai) {
	int y = 1;
	for(int i = 0; i < parai; i++)
		y += дайВысоту(i);
	return y;
}

void ДокРедакт::инвалидируйСтроку(int i)
{
	para[i].cx = -1;
}

void ДокРедакт::освежиСтроку(int i) {
	int q = para[i].cx >= 0 ? para[i].cy : -1;
	освежи(1, дайУ(i) - sb, cx, дайВысоту(i));
	if(q < 0 || q != para[i].cy)
		освежи();
}

int sсумма(const int *w, int n)
{
	int m = 0;
	while(n--)
		m += *w++;
	return m;
}

void ДокРедакт::рисуй(Draw& w) {
	Размер sz = дайРазм();
	Цвет bg =  color[IsShowEnabled() && !толькочтен_ли() ? PAPER_NORMAL : PAPER_READONLY];
	if(nobg)
		bg = Null;
	int y = -sb + 1;
	int pos = 0;
	int sell, selh;
	дайВыделение32(sell, selh);
	for(int i = 0; i < para.дайСчёт() && y < sz.cy; i++) {
		int h = дайВысоту(i);
		if(y + h >= 0) {
			ШТкст text = дайШСтроку(i);
			Fmt fmt = фмт(text);
			int p = pos;
			for(int i = 0; i < fmt.line.дайСчёт(); i++) {
				int n = fmt.LineEnd(i) - fmt.line[i];
				int a = minmax(sell - p, 0, n);
				int b = minmax(selh - p, 0, n) - a;
				int c = n - a - b;
				int *wa = fmt.width + fmt.line[i];
				int *wb = fmt.width + fmt.line[i] + a;
				int *wc = fmt.width + fmt.line[i] + a + b;
				int acx = sсумма(wa, a);
				int bcx = sсумма(wb, b);
				int ccx = sсумма(wc, c);
				w.DrawRect(1, y, acx, fmt.fi.дайВысоту(), bg);
				w.DrawText(1, y, ~fmt.text + fmt.line[i], font,
				           IsShowEnabled() ? color[INK_NORMAL] : color[INK_DISABLED], a, wa);
				w.DrawRect(1 + acx, y, bcx, fmt.fi.дайВысоту(), color[PAPER_SELECTED]);
				w.DrawText(1 + acx, y, ~fmt.text + fmt.line[i] + a, font, color[INK_SELECTED], b, wb);
				w.DrawRect(1 + acx + bcx, y, ccx, fmt.fi.дайВысоту(), bg);
				w.DrawText(1 + acx + bcx, y, ~fmt.text + fmt.line[i] + a + b, font, color[INK_NORMAL], c, wc);
				p += n;
				w.DrawRect(1 + acx + bcx + ccx, y, cx - (acx + bcx + ccx), fmt.fi.дайВысоту(),
				           p >= sell && p < selh ? color[PAPER_SELECTED] : bg);
				y += fmt.fi.дайВысоту();
			}
			w.DrawRect(1, y, cx, after, color[PAPER_NORMAL]);
			y += after;
		}
		else
			y += h;
		pos += дайДлинуСтроки(i) + 1;
	}
	w.DrawRect(0, -sb, sz.cx, 1, bg);
	w.DrawRect(0, 0, 1, sz.cy, bg);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, bg);
	if(eofline)
		w.DrawRect(1, y++, cx, 1, SColorShadow);
	if(y < sz.cy)
		w.DrawRect(1, y, cx, sz.cy - y, bg);
	DrawTiles(w, DropCaret(), CtrlImg::checkers());
}

void ДокРедакт::SetSb()
{
	Размер sz = дайРазм();
	cx = max(Draw::GetStdFontCy(), sz.cx - 2);
	sb.устСтраницу(дайРазм().cy);
	sb.устВсего(дайУ(para.дайСчёт()) + 2);
	поместиКаретку(false);
}

void ДокРедакт::Выкладка()
{
	SetSb();
	инивалидируй();
}

Точка ДокРедакт::дайКаретку(int pos) {
	int i = дайПозСтроки32(pos);
	Fmt fmt = фмт(дайШСтроку(i));
	int l;
	for(l = 0; l < fmt.line.дайСчёт(); l++)
		if(pos < fmt.line[l])
			break;
	l--;
	const int *w = fmt.width + fmt.line[l];
	pos -= fmt.line[l];
	int x = 0;
	while(pos-- > 0)
		x += *w++;
	return Точка(x, дайУ(i) + l * fmt.fi.дайВысоту());
}

int  ДокРедакт::GetCursorPos(Точка p) {
	int pos = 0;
	for(int i = 0; i < para.дайСчёт(); i++) {
		int h = дайВысоту(i);
		if(p.y < h) {
			ШТкст text = дайШСтроку(i);
			Fmt fmt = фмт(text);
			int x = 0;
			int l = p.y / fmt.fi.дайВысоту();
			if(l < 0)
				return pos;
			if(l >= fmt.line.дайСчёт())
				return pos + text.дайДлину();
			const int *w = fmt.width + fmt.line[l];
			const int *e = fmt.width + fmt.LineEnd(l);
			while(w < e) {
				if(p.x < x + *w / 2)
					return int(w - fmt.width) + pos;
				x += *w++;
			}
			int p = int(e - fmt.width);
			if(p > 0 && text[p - 1] == ' ' && l < fmt.line.дайСчёт() - 1)
			   p--;
			return p + pos;
		}
		p.y -= h;
		pos += дайДлинуСтроки(i) + 1;
	}
	return дайДлину32();
}

void ДокРедакт::поместиКаретку(bool scroll) {
	Точка cr = дайКаретку((int)cursor);
	int fy = font.Info().GetLineHeight();
	if(scroll) {
		if(cursor == дайДлину32())
			sb.стоп();
		else
			sb.промотайДо(cr.y, fy + 2);
	}
	устКаретку(cr.x + 1, cr.y - sb, 1, fy);
	WhenSel();
}

void ДокРедакт::поместиКаретку(int64 newpos, bool select) {
	if(newpos > дайДлину32())
		newpos = дайДлину32();
	int z = дайСтроку(newpos);
	if(select) {
		if(anchor < 0)
			anchor = cursor;
		освежиСтроки(z, дайСтроку(cursor));
	}
	else
		if(anchor >= 0) {
			освежиСтроки(дайСтроку(cursor), дайСтроку(anchor));
			anchor = -1;
		}
	cursor = newpos;
	поместиКаретку(true);
	SelectionChanged();
	if(выделение_ли())
		SetSelectionSource(ClipFmtsText());
	CancelMyPreedit();
}

int ДокРедакт::дайПозМыши(Точка p)
{
	return GetCursorPos(Точка(p.x - 1, p.y + sb - 1));
}

void ДокРедакт::леваяВнизу(Точка p, dword flags) {
	устФокус();
	int c = дайПозМыши(p);
	int l, h;
	if(дайВыделение32(l, h) && c >= l && c < h) {
		selclick = true;
		return;
	}
	поместиКаретку(c, flags & K_SHIFT);
	SetCapture();
}

void ДокРедакт::леваяВверху(Точка p, dword flags)
{
	if(!HasCapture() && selclick) {
		int c = дайПозМыши(p);
		поместиКаретку(c, flags & K_SHIFT);
		устФокус();
	}
	selclick = false;
	ReleaseCapture();
}

void ДокРедакт::двигМыши(Точка p, dword flags) {
	if(!HasCapture()) return;
	поместиКаретку(дайПозМыши(p), true);
}

void ДокРедакт::леваяДКлик(Точка, dword)
{
	int64 l, h;
	if(GetWordSelection(cursor, l, h))
		устВыделение(l, h);
}

void ДокРедакт::LeftTriple(Точка, dword)
{
	int q = (int)cursor;
	int i = дайПозСтроки32(q);
	q = (int)cursor - q;
	устВыделение(q, q + дайДлинуСтроки(i) + 1);
}

Рисунок ДокРедакт::рисКурсора(Точка, dword) {
	return Рисунок::IBeam();
}

void ДокРедакт::сфокусирован() {
	освежи();
}

void ДокРедакт::расфокусирован() {
	освежи();
}

void ДокРедакт::VertMove(int delta, bool select, bool scs) {
	int hy = дайУ(para.дайСчёт());
	Точка p = дайКаретку((int)cursor);
	int yy = p.y;
	for(;;) {
		p.y += delta;
		if(p.y > hy) p.y = hy - 1;
		if(p.y < 0) p.y = 0;
		int q = GetCursorPos(p);
		if(q >= 0 && q != cursor && (delta < 0) == (q < cursor) && дайКаретку(q).y != yy) {
			поместиКаретку(q, select);
			break;
		}
		if(p.y == 0 || p.y >= hy - 1) {
			поместиКаретку(delta > 0 ? дайДлину32() : 0, select);
			break;
		}
		delta = зн(delta) * 4;
	}
	if(scs)
		sb = дайКаретку((int)cursor).y - (yy - sb);
	поместиКаретку(true);
}

void ДокРедакт::HomeEnd(int x, bool select) {
	Точка p = дайКаретку((int)cursor);
	p.x = x;
	поместиКаретку(GetCursorPos(p), select);
}

bool ДокРедакт::Ключ(dword ключ, int cnt)
{
	NextUndo();
	bool h;
	int q;
	bool select = ключ & K_SHIFT;
	int pgsk = max(8, 6 * дайРазм().cy / 8);
	switch(ключ & ~K_SHIFT) {
	case K_CTRL_LEFT:
		поместиКаретку(GetPrevWord(cursor), select);
		break;
	case K_CTRL_RIGHT:
		поместиКаретку(GetNextWord(cursor), select);
		break;
	case K_HOME:
		HomeEnd(0, select);
		break;
	case K_END:
		HomeEnd(cx, select);
		break;
	case K_CTRL_HOME:
	case K_CTRL_PAGEUP:
		поместиКаретку(0, select);
		break;
	case K_CTRL_END:
	case K_CTRL_PAGEDOWN:
		поместиКаретку(дайДлину32(), select);
		break;
	case K_UP:
		if(дайКурсор() == 0)
			return !updownleave;
		VertMove(-8, select, false);
		return true;
	case K_DOWN:
		if(дайКурсор32() == дайДлину32())
			return !updownleave;
		VertMove(8, select, false);
		return true;
	case K_PAGEUP:
		VertMove(-pgsk, select, true);
		return true;
	case K_PAGEDOWN:
		VertMove(pgsk, select, true);
		return true;
	case K_LEFT:
		if(cursor)
			поместиКаретку(cursor - 1, select);
		break;
	case K_RIGHT:
		if(cursor < дайДлину32())
			поместиКаретку(cursor + 1, select);
		break;
	default:
		if(толькочтен_ли()) return БарМеню::скан(WhenBar, ключ);
		switch(ключ) {
		case K_BACKSPACE:
		case K_SHIFT|K_BACKSPACE:
			if(удалиВыделение()) break;
			if(cursor == 0) return true;
			cursor--;
			удали((int)cursor, 1);
			break;
		case K_CTRL_BACKSPACE:
			if(удалиВыделение()) break;
			if(cursor <= 0) return true;
			q = (int)cursor - 1;
			h = буква_ли(дайСим(q));
			while(q > 0 && буква_ли(дайСим(q - 1)) == h) q--;
			удали(q, (int)cursor - q);
			устКурсор(q);
			break;
		case K_DELETE:
			if(удалиВыделение()) break;
			if(cursor >= дайДлину32()) return true;
			if(cursor < дайДлину32())
				удали((int)cursor, 1);
			break;
		case K_CTRL_DELETE:
			if(удалиВыделение()) break;
			if(cursor >= дайДлину32()) return true;
			q = (int)cursor;
			h = буква_ли(дайСим(q));
			while(буква_ли(дайСим(q)) == h && q < дайДлину32()) q++;
			удали((int)cursor, q - (int)cursor);
			break;
		case K_ENTER:
			if(!processenter)
				return true;
			ключ = '\n';
		default:
			if(filter && ключ >= 32 && ключ < 65535)
				ключ = (*filter)(ключ);
			if(ключ >= ' ' && ключ < K_CHAR_LIM || ключ == '\n' || ключ == '\t' || ключ == K_SHIFT_SPACE) {
				if(ключ == K_TAB && !processtab)
					return false;
				if(ключ >= 128 && ключ < K_CHAR_LIM && (charset != CHARSET_UTF8 && charset != CHARSET_UTF8_BOM)
				   && изЮникода((wchar)ключ, charset) == DEFAULTCHAR)
					return true;
				удалиВыделение();
				вставь((int)cursor, ШТкст(ключ == K_SHIFT_SPACE ? ' ' : ключ, cnt), true);
				cursor += cnt;
				break;
			}
			return БарМеню::скан(WhenBar, ключ);
		}
		UpdateAction();
	}
	поместиКаретку(true);
	return true;
}

void ДокРедакт::промотай()
{
	поместиКаретку(false);
	освежи();
}

void ДокРедакт::инивалидируй()
{
	for(int i = 0; i < para.дайСчёт(); i++)
		para[i].cx = -1;
	поместиКаретку(false);
}

void  ДокРедакт::RefreshStyle()
{
	cursor = 0;
	sb = 0;
	очистьВыделение();
	инивалидируй();
	Выкладка();
	освежи();
}

void ДокРедакт::праваяВнизу(Точка p, dword w)
{
	устФокус();
	int c = дайПозМыши(p);
	int l, h;
	if(!дайВыделение32(l, h) || c < l || c >= h)
		поместиКаретку(c, false);
	БарМеню::выполни(WhenBar);
}

ДокРедакт::ДокРедакт()
{
	updownleave = false;
	cx = 0;
	filter = NULL;
	after = 0;
	font = StdFont();
	AutoHideSb();
	устФрейм(ViewFrame());
	добавьФрейм(sb);
	sb.устСтроку(8);
	sb.ПриПромоте = THISBACK(промотай);
	вставьСтроки(0, 1);
	eofline = true;
	поместиКаретку(false);
}

ДокРедакт::~ДокРедакт() {}

void ДокРедакт::тягИБрос(Точка p, PasteClip& d)
{
	if(толькочтен_ли()) return;
	int c = дайПозМыши(p);
	if(AcceptText(d)) {
		NextUndo();
		int a = sb;
		int sell, selh;
		ШТкст txt = дайШТкст(d);
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
		int count = вставь(c, txt);
		sb = a;
		устФокус();
		устВыделение(c, c + count);
		Action();
		return;
	}
	if(!d.IsAccepted()) return;
	Точка dc = Null;
	if(c >= 0) {
		Точка cr = дайКаретку(c);
		dc = Точка(cr.x + 1, cr.y);
	}
	if(dc != dropcaret) {
		RefreshDropCaret();
		dropcaret = dc;
		RefreshDropCaret();
	}
}

Прям ДокРедакт::DropCaret()
{
	if(пусто_ли(dropcaret))
		return Прям(0, 0, 0, 0);
	return RectC(dropcaret.x, dropcaret.y - sb, 1, font.Info().GetLineHeight());
}

void ДокРедакт::RefreshDropCaret()
{
	освежи(DropCaret());
}

void ДокРедакт::тягПовтори(Точка p)
{
	sb = (int)sb + GetDragScroll(this, p, 16).y;
}

void ДокРедакт::тягПокинь()
{
	RefreshDropCaret();
	dropcaret = Null;
	isdrag = false;
	Выкладка();
}

void ДокРедакт::леваяТяг(Точка p, dword flags)
{
	int c = дайПозМыши(p);
	int l, h;
	if(!HasCapture() && дайВыделение32(l, h) && c >= l && c < h) {
		ШТкст sample = дайШ(l, min(h - l, 3000));
		Размер ssz = StdSampleSize();
		ImageDraw iw(ssz);
		iw.DrawRect(ssz, чёрный());
		iw.Alpha().DrawRect(ssz, чёрный());
		DrawTLText(iw.Alpha(), 0, 0, ssz.cx, sample, StdFont(), белый());
		NextUndo();
		if(DoDragAndDrop(ClipFmtsText(), iw) == DND_MOVE && !толькочтен_ли()) {
			удалиВыделение();
			Action();
		}
	}
}

ТекстКтрл::ПозРедакт ДокРедакт::GetEditPos() const {
	ПозРедакт pos;
	pos.sby = sb.дай();
	pos.cursor = cursor;
	return pos;
}

void ДокРедакт::SetEditPos(const ТекстКтрл::ПозРедакт& pos) {
	sb.уст(minmax(pos.sby, 0, дайУ(para.дайСчёт()) + 2));
	устКурсор(pos.cursor);
}

}
