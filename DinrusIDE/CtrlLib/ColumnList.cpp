#include "CtrlLib.h"

namespace РНЦП {

void СписокКолонок::колесоМыши(Точка p, int zdelta, dword keyflags) {
	sb.Wheel(zdelta);
}

int  СписокКолонок::GetColumnCx(int i) const {
	if (mode == MODE_ROWS)
		return cx;
	int szcx = дайРазм().cx;
	int cx = max(szcx / max(ncl, 1), Draw::GetStdFontCy());
	return i == ncl - 1 ? szcx - i * cx : cx;
}

int СписокКолонок::дайЭлтыКолонки() const {
	if(cy == 0) return 0;
	return дайРазм().cy / cy;
}

int  СписокКолонок::GetDragColumn(int x) const {
	if (mode == MODE_ROWS)
		return -1;
	int cx = GetColumnCx();
	int i = 0;
	if(cx == 0) return -1;
	for(;;) {
		x -= GetColumnCx(i);
		if(x >= -3 && x < 4)
			return i;
		if(x < 0) return -1;
		i++;
	}
}

void СписокКолонок::ShiftSelect() {
	if(anchor < 0)
		anchor = cursor;
	int l = min(anchor, cursor);
	int h = max(anchor, cursor);
	for(int i = 0; i < дайСчёт(); i++) {
		bool sel = i >= l && i <= h;
		if(элт[i].canselect && элт[i].sel != sel) {
			элт[i].sel = sel;
			освежиЭлт(i);
		}
	}
	UpdateSelect();
}

Рисунок СписокКолонок::рисКурсора(Точка p, dword)
{
	return HasCapture() || GetDragColumn(p.x) >= 0 ? Рисунок::SizeHorz() : Рисунок::Arrow();
}

int СписокКолонок::дайЭлт(Точка p)
{
	int i;
	int max = дайЭлтыСтраницы();
	if (mode == MODE_ROWS) {
		i = ((p.y + sb % cy) / cy) * ncl + (p.x / cx);
		max += ncl*2;
	}
	else
		i = p.y / cy + дайЭлтыКолонки() * (p.x / GetColumnCx(0));
	if(i < 0 || i >= max)
		return -1;
	i += GetSbPos();
	return i >= 0 && i < дайСчёт() ? i : -1;
}

void СписокКолонок::PointDown(Точка p)
{
	int i = дайЭлт(p);
	if(i >= 0 && i < дайСчёт())
		устКурсор0(i, false);
	else
	if(clickkill)
		анулируйКурсор();
}

void СписокКолонок::кликни(Точка p, dword flags)
{
	SetWantFocus();
	PointDown(p);
	p.y %= cy;
	p.x %= GetColumnCx(0);
	if(multi && cursor >= 0) {
		if(flags & K_SHIFT)
			ShiftSelect();
		else
		if(flags & K_CTRL)
			выделиОдин(cursor, !выделен(cursor));
		else {
			очистьВыделение();
			выделиОдин(cursor, true);
			anchor = cursor;
		}
	}
	ПриЛевКлике();
	WhenLeftClickPos(p);
}

void СписокКолонок::левуюВниз(Точка p, dword flags) {
	selclick = false;
	int i = GetDragColumn(p.x);
	if(i >= 0) {
		ci = i;
		dx = p.x - GetColumnCx(0) * (i + 1);
		mpos = p.x;
		SetCapture();
		освежи(mpos - dx, 0, 1, дайРазм().cy);
		синхИнфо();
	}
	else {
		int i = дайЭлт(p);
		if(i >= 0 && i < дайСчёт() && multi && IsSel(i))
			selclick = true;
		else
			кликни(p, flags);
	}
}

void СписокКолонок::леваяВнизу(Точка p, dword flags) {
	левуюВниз(p, flags);
}

void СписокКолонок::леваяВверху(Точка p, dword flags)
{
	if(selclick)
		кликни(p, flags);
	selclick = false;
	if(HasCapture()) {
		освежи();
		синхИнфо();
	}
	ReleaseCapture();
}

void СписокКолонок::леваяТяг(Точка p, dword keyflags)
{
	if(!HasCapture())
		WhenDrag();
}

void СписокКолонок::праваяВнизу(Точка p, dword flags) {
	int i = дайЭлт(p);
	if(!(i >= 0 && multi && IsSel(i))) // Do not change selection if clicking on it
		кликни(p, 0);
	БарМеню::выполни(WhenBar);
}

void СписокКолонок::леваяДКлик(Точка p, dword flags) {
	левуюВниз(p, flags);
	WhenLeftDouble();
}

void СписокКолонок::синхИнфо()
{
	if((естьМышь() || info.естьМышь()) && !HasCapture() && popupex) {
		Точка p = GetMouseViewPos();
		int i = дайЭлт(p);
		if(i >= 0) {
			Прям r = дайПрямЭлта(i);
			if(p.x > r.left + 3 && p.x < r.right - 2) {
				Цвет ink, paper;
				dword style;
				дайСтильЭлта(i, ink, paper, style);
				Элемент& m = элт[i];
				info.уст(this, r, m.значение, m.дисплей ? m.дисплей : дисплей, ink, paper, style);
				return;
			}
		}
	}
	info.Cancel();
}

void СписокКолонок::двигМыши(Точка p, dword)
{
	if(HasCapture()) {
		int cx = sb.дайРазмОбзора().cx;
		освежи(mpos - dx, 0, 1, дайРазм().cy);
		mpos = p.x;
		освежи(mpos - dx, 0, 1, дайРазм().cy);
		int ocl = ncl;
		ncl = max(1, cx * (ci + 1) / max(p.x, Draw::GetStdFontCy()));
		if(ncl != ocl)
			освежи();
		SetSb();
	}
	синхИнфо();
}

void СписокКолонок::режимОтмены()
{
	info.Cancel();
	dropitem = -1;
	anchor = -1;
	selclick = false;
}

void СписокКолонок::выходМыши()
{
	if(!info.открыт())
		info.Cancel();
}

Прям СписокКолонок::дайПрямЭлта(int i) const {
	if (mode == MODE_ROWS)
		return (i >= 0) ? RectC( (i % ncl) * cx, (i / ncl) * cy - sb, cx, cy) : Прям(0, 0, 0, 0);
	i -= GetSbPos();
	int n = дайЭлтыКолонки();
	if(i < 0 || n == 0) 
		return Прям(0, 0, 0, 0);
	int cl = i / n;
	int x = 0;
	for(int q = 0; q < cl; q++)
		x += GetColumnCx(q);
	return RectC(x, i % n * cy, GetColumnCx(cl), cy);
}

void СписокКолонок::Page(bool down) {
	int q = GetSbPos();
	if(down)
		sb.следщСтраница();
	else
		sb.предшСтраница();
	if(q == sb)
		устКурсор(down ? дайСчёт() - 1 : 0);
	else
		устКурсор(cursor - q + GetSbPos());
}

dword СписокКолонок::поменяйКлюч(dword ключ)
{
	if (mode == MODE_ROWS) {
		switch(ключ) {
		case K_UP:
			return K_LEFT;
		case K_DOWN:
			return K_RIGHT;
		case K_LEFT:
			return K_UP;
		case K_RIGHT:
			return K_DOWN;
		}
	}
	return ключ;
}

bool СписокКолонок::Ключ(dword _key, int count) {
	int c = cursor;
	bool sel = (_key & K_SHIFT) && multi;
	int ключ = _key & ~K_SHIFT;
	ключ = поменяйКлюч(ключ);
	switch(ключ) {
	case K_UP:
		if(c < 0)
			c = дайСчёт() - 1;
		else
		if(c > 0)
			c--;
		break;
	case K_DOWN:
		if(c < 0)
			c = 0;
		else
		if(c < дайСчёт() - 1)
			c++;
		break;
	case K_LEFT:
		c = max(c - ((mode == MODE_ROWS) ? ncl : дайЭлтыКолонки()), 0);
		break;
	case K_RIGHT:
		c = min(c + ((mode == MODE_ROWS) ? ncl : дайЭлтыКолонки()), дайСчёт() - 1);
		break;
	case K_PAGEUP:
		Page(false);
		return true;
	case K_PAGEDOWN:
		Page(true);
		return true;
	case K_HOME:
		c = 0;
		break;
	case K_END:
		c = дайСчёт() - 1;
		break;
	default:
		return БарМеню::скан(WhenBar, _key);
	}
	if(sel) {
		устКурсор0(c, false);
		ShiftSelect();
	}
	else {
		устКурсор(c);
		anchor = c;
	}
	return true;
}

void СписокКолонок::освежиВыд()
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(элт[i].sel)
			освежиЭлт(i);
}

void СписокКолонок::очистьВыделение() {
	bool upd = false;
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(элт[i].sel) {
			элт[i].sel = false;
			upd = true;
			освежиЭлт(i);
		}
	if(upd)
		UpdateSelect();
}

void СписокКолонок::выделиОдин(int i, bool sel) {
	if(!multi) return;
	if(элт[i].canselect)
		элт[i].sel = sel;
	UpdateSelect();
	освежиЭлт(i);
}

bool СписокКолонок::выделен(int i) const {
	return элт[i].sel;
}

bool СписокКолонок::IsSel(int i) const
{
	return IsMultiSelect() ? выделен(i) : дайКурсор() == i;
}

void СписокКолонок::UpdateSelect() {
	selcount = 0;
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(элт[i].sel)
			selcount++;
	синхИнфо();
	Action();
	WhenSelection();
	WhenSel();
}

void СписокКолонок::дайСтильЭлта(int i, Цвет& fg, Цвет& bg, dword& st)
{
	st = 0;
	bool hasfocus = HasFocusDeep() && !IsDragAndDropTarget();
	bool drop = i == dropitem && !insert;
	if(толькочтен_ли())
		st |= Дисплей::READONLY;
	if(выделен(i))
		st |= Дисплей::SELECT;
	if(i == cursor)
		st |= Дисплей::CURSOR;
	if(drop) {
		hasfocus = true;
		st |= Дисплей::CURSOR;
	}
	if(hasfocus)
		st |= Дисплей::FOCUS;
	fg = SColorText;
	bg = SColorPaper;
	if(nobg)
		bg = Null;
	if((st & Дисплей::SELECT) ||
	   (!multi || !элт[i].canselect && selcount == 0) && (st & Дисплей::CURSOR) ||
	   drop) {
		fg = hasfocus ? SColorHighlightText : SColorText;
		bg = hasfocus ? SColorHighlight : смешай(SColorDisabled, SColorPaper);
	}
}

dword СписокКолонок::рисуйЭлт(Draw& w, int i, const Прям& r)
{
	Цвет ink, paper;
	dword style;
	дайСтильЭлта(i, ink, paper, style);
	const Элемент& m = элт[i];
	w.Clip(r);
	(m.дисплей ? m.дисплей : дисплей)->рисуй(w, r, m.значение, ink, paper, style);
	w.стоп();
	return style;
}

void СписокКолонок::рисуй(Draw& w) {
	Размер sz = дайРазм();
	if(!GetColumnCx(0) || !cy) return;
	if (mode == MODE_ROWS)
		return рисуйРяды(w, sz);
	else {
		int x = 0;
		int i = GetSbPos(sz);
		int coli = 0;
		while(x < sz.cx) {
			int cx = GetColumnCx(coli++);
			int y = 0;
			while(y + cy <= sz.cy) {
				Прям rect = RectC(x, y, cx, cy);
				if(i < дайСчёт()) {
					if(w.IsPainting(rect)) {
						Прям r = rect;
						r.right--;
						dword style = рисуйЭлт(w, i, r);
						w.DrawRect(rect.right - 1, rect.top, 1, rect.устВысоту(),
						           x + cx < sz.cx ? SColorDisabled : SColorPaper);
						if(i == cursor && selcount != 1 && multi && элт[i].canselect)
							DrawFocus(w, r, style & Дисплей::SELECT ? SColorPaper() : SColorText());
					}
				}
				else
					w.DrawRect(rect, SColorPaper);
				if(i == dropitem && insert)
					DrawHorzDrop(w, x, y, cx);
				if(i + 1 == dropitem && insert && y + 2 * cy > sz.cy)
					DrawHorzDrop(w, x, y + cy - 2, cx);
				y += cy;
				i++;
			}
			w.DrawRect(x, y, cx, sz.cy - y, SColorPaper);
			x += cx;
		}
	}
	if(HasCapture())
		w.DrawRect(mpos - dx, 0, 1, sz.cy, смешай(SColorHighlight, SColorFace));
	(mode == MODE_COLUMN) ? 
		scroller.уст(Точка(sb, 0)) : 
		scroller.уст(sb);
}

void СписокКолонок::рисуйРяды(Draw &w, Размер &sz)
{
	int pos = sb % cy;
	int y = -pos;
	int i = GetSbPos(sz);
	while(y < sz.cy-pos+cy) {
		int x = 0;
		while (x + cx <= sz.cx) {
			Прям rect = RectC(x, y, cx, cy);
			if(i < дайСчёт()) {
				if(w.IsPainting(rect)) {
					Прям r = rect;
					dword style = рисуйЭлт(w, i, r);
//					w.DrawRect(rect.right - 1, rect.top, 1, rect.устВысоту(),
//					           x + cx < sz.cx ? SColorDisabled : SColorPaper);
					if(i == cursor && selcount != 1 && multi && элт[i].canselect)
						DrawFocus(w, r, style & Дисплей::SELECT ? SColorPaper() : SColorText());
				}
			}
			else
				w.DrawRect(rect, SColorPaper);
			if(i == dropitem && insert)
				DrawHorzDrop(w, x, y, cx);
			if(i + 1 == dropitem && insert && y + 2 * cy > sz.cy)
				DrawHorzDrop(w, x, y + cy - 2, cx);
			x += cx;
			i++;
		}
		w.DrawRect(x, y, cx, sz.cy - y, SColorPaper);
		y += cy;
	}
}

Рисунок СписокКолонок::дайСэиплТяга()
{
	Размер sz = StdSampleSize();
	ImageDraw iw(sz);
	int y = 0;
	for(int i = 0; i < дайСчёт() && y < sz.cy; i++)
		if(IsSel(i)) {
			рисуйЭлт(iw, i, RectC(0, y, sz.cx, cy));
			y += cy;
		}
	return Crop(iw, 0, 0, sz.cx, y);;
}

int  СписокКолонок::дайЭлтыСтраницы() const 
{
	return ncl * дайЭлтыКолонки();
}

void СписокКолонок::SetSb() 
{
	switch (mode) {
	case MODE_LIST:
		sb.устВсего(дайСчёт());
		sb.устСтраницу(дайЭлтыСтраницы());
		sb.устСтроку(1);
		break;
	case MODE_COLUMN: {
		int icnt = max(1, дайЭлтыКолонки());
		int ccnt = дайСчёт()/icnt;
		ccnt += (дайСчёт() % icnt) ? 1 : 0;
		sb.устВсего(ccnt);
		sb.устСтраницу(ncl);
		sb.устСтроку(1);
		break;
		}
	case MODE_ROWS:	{
		int rcnt = дайСчёт()/ncl;
		rcnt += (дайСчёт() % ncl) ? 1 : 0;
		sb.устВсего(rcnt*cy);
		sb.устСтраницу((дайРазм().cy/cy)*cy);
		sb.устСтроку(cy);
		break;
		}
	}
}

void СписокКолонок::промотайДо(int pos)
{
	switch(mode) {
	case MODE_ROWS:
		sb.промотайДо((pos / ncl) * cy, max(0, sb.дайСтроку() - (дайРазм().cy - sb.дайСтраницу())));
		return;
	case MODE_COLUMN:
		sb.промотайДо(pos / max(1, дайЭлтыКолонки()));
		return;
	case MODE_LIST:
		sb.промотайДо(pos);
		return;
	}
}

void СписокКолонок::Выкладка() 
{
	if (mode == MODE_ROWS)
		ncl = max(дайРазм().cx / cx, 1);
	SetSb();
}

void СписокКолонок::промотай() 
{
	Размер sz = дайРазм();
	switch (mode) {
	case MODE_LIST:
		sz.cy = sz.cy / cy * cy;
		scroller.промотай(*this, sz, sb, cy);
		break;
	case MODE_COLUMN:
		sz.cy = sz.cy / cy * cy;
		scroller.промотай(*this, sz, Точка(sb, 0), Размер(GetColumnCx(0), 0));
		break;
	case MODE_ROWS:
		scroller.промотай(*this, sz, sb, 1);
		break;
	}
	info.Cancel();
}

void СписокКолонок::освежиЭлт(int i, int ex)
{
	if(i >= 0) {
		освежи(дайПрямЭлта(i).вертИнфлят(ex));
		синхИнфо();
	}
}

void СписокКолонок::устКурсор0(int c, bool sel)
{
	int c0 = cursor;
	c = minmax(c, 0, дайСчёт() - 1);
	if(c < 0 || cursor < 0)
		освежи();
	else
		RefreshCursor();
	cursor = c;
	int q = sb;
	промотайДо(cursor);
	if(q != sb)
		освежи();
	else
		RefreshCursor();
	if(sel && multi) {
		очистьВыделение();
		if(cursor >= 0) {
			выделиОдин(cursor, true);
			anchor = cursor;
		}
	}
	if(c0 != cursor) {
		if(cursor >= 0)
			WhenEnterItem();
		else
			WhenKillCursor();
		WhenSel();
	}
	синхИнфо();
	Action();
}

void СписокКолонок::устКурсор(int c)
{
	устКурсор0(c, true);
}

int СписокКолонок::GetSbPos(const Размер& sz) const
{
	switch (mode) {
	case MODE_ROWS:
		return (sb / cy) * ncl;
	case MODE_COLUMN:
		return (cy ? sb * (sz.cy / cy) : 0);
	case MODE_LIST:
		return sb;
	}
	return sb;
}

void СписокКолонок::SetSbPos(int y)
{
	SetSb();
	sb = minmax(y, 0, дайСчёт() - дайЭлтыСтраницы());
}

void СписокКолонок::анулируйКурсор()
{
	if(cursor < 0) return;
	освежи();
	синхИнфо();
	cursor = -1;
	sb.старт();
	WhenKillCursor();
	WhenSel();
	Action();
}

void СписокКолонок::сфокусирован()
{
	if(cursor < 0 && дайСчёт())
		устКурсор(GetSbPos());
	освежи();
	синхИнфо();
}

void СписокКолонок::расфокусирован()
{
	освежи();
	синхИнфо();
}

int СписокКолонок::RoundedCy()
{
	if (mode != MODE_LIST)
	    return дайПрям().дайВысоту();
	Прям r = дайПрям();
	Прям rr = r;
	frame->выложиФрейм(r);
	return (cy ? r.устВысоту() / cy * cy : 0) + rr.устВысоту() - r.устВысоту();
}

void СписокКолонок::выложиФрейм(Прям& r)
{
	r.bottom = r.top + RoundedCy();
	frame->выложиФрейм(r);
}

void СписокКолонок::добавьРазмФрейма(Размер& sz)
{
	frame->добавьРазмФрейма(sz);
}

void СписокКолонок::рисуйФрейм(Draw& draw, const Прям& r)
{
	int ry = RoundedCy();
	Прям rr = r;
	rr.bottom = ry;
	frame->рисуйФрейм(draw, rr);
	rr = r;
	rr.top = ry;
	draw.DrawRect(rr, SColorFace);
}

СписокКолонок& СписокКолонок::RoundSize(bool b)
{
	if(b)
		Ктрл::устФрейм(0, *this);
	else
		Ктрл::устФрейм(0, *frame);
	освежиВыкладку();
	освежи();
	синхИнфо();
	return *this;
}

void СписокКолонок::устФрейм(КтрлФрейм& _frame)
{
	frame = &_frame;
	освежиВыкладку();
	освежи();
	синхИнфо();
}

struct СписокКолонок::ПорядокЭлтов {
	const ПорядокЗнач *order;
	bool operator()(const Элемент& a, const Элемент& b) const {
		return (*order)(a.значение, b.значение);
	}
};

void СписокКолонок::сортируй(const ПорядокЗнач& order)
{
	ПорядокЭлтов itemorder;
	itemorder.order = &order;
	анулируйКурсор();
	РНЦП::сортируй(элт, itemorder);
	sb.старт();
	синхИнфо();
}

Значение СписокКолонок::дайДанные() const
{
	if (!multi)
		return (cursor >= 0) ? дай(cursor) : Значение();
	for (int i = 0; i < элт.дайСчёт(); i++)
		if (элт[i].sel)
			return элт[i].ключ;
	return Значение();
}

void СписокКолонок::устДанные(const Значение& ключ)
{
	int ii = найди(ключ);
	if (ii >= 0) {
		выделиОдин(ii, true);
		устКурсор(ii);
	}
}

void СписокКолонок::очисть() {
	режимОтмены();
	анулируйКурсор();
	элт.очисть();
	selcount = 0;
	Update();
	освежи();
	синхИнфо();
	SetSb();
}

void СписокКолонок::вставь(int ii, const Значение& val, bool canselect)
{
	вставь(ii, val, val, canselect);
}

void СписокКолонок::вставь(int ii, const Значение& val, const Дисплей& дисплей, bool canselect)
{
	вставь(ii, val, val, дисплей, canselect);
}

void СписокКолонок::вставь(int ii, const Значение& ключ, const Значение& val, bool canselect)
{
	int c = -1;
	if(cursor >= ii) {
		c = cursor + 1;
		анулируйКурсор();
	}
	Элемент& m = элт.вставь(ii);
	m.ключ = ключ;
	m.значение = val;
	m.sel = false;
	m.canselect = canselect;
	m.дисплей = NULL;
	освежи();
	синхИнфо();
	SetSb();
	if(c >= 0)
		устКурсор(c);	
}

void СписокКолонок::вставь(int ii, const Значение& ключ, const Значение& val, const Дисплей& дисплей, bool canselect)
{
	вставь(ii, ключ, val, canselect);
	элт[ii].дисплей = &дисплей;
	синхИнфо();
}

void СписокКолонок::уст(int ii, const Значение& val, bool canselect)
{
	уст(ii, val, val, canselect);
}

void СписокКолонок::уст(int ii, const Значение& val, const Дисплей& дисплей, bool canselect)
{
	уст(ii, val, val, дисплей, canselect);
}

void СписокКолонок::уст(int ii, const Значение& ключ, const Значение& val, bool canselect)
{
	Элемент& m = элт[ii];
	m.значение = val;
	m.ключ = ключ;
	m.sel = false;
	m.canselect = canselect;
	m.дисплей = NULL;
	освежиЭлт(ii);
	синхИнфо();
	SetSb();
}

void СписокКолонок::уст(int ii, const Значение& ключ, const Значение& val, const Дисплей& дисплей, bool canselect)
{
	уст(ii, ключ, val, canselect);
	элт[ii].дисплей = &дисплей;
	синхИнфо();
}

void СписокКолонок::уст(const Значение &ключ, const Значение& val, const Дисплей& дисплей, bool canselect)
{
	int ii = найди(ключ);
	if (ii >= 0)
		уст(ii, ключ, val, дисплей, canselect);
}

void СписокКолонок::уст(const Значение &ключ, const Значение& val, bool canselect)
{
	int ii = найди(ключ);
	if (ii >= 0)
		уст(ii, ключ, val, canselect);
}

void СписокКолонок::удали(int ii)
{
	int c = -1;
	if(cursor >= ii) {
		c = max(ii, cursor - 1);
		анулируйКурсор();
	}
	элт.удали(ii);
	освежи();
	синхИнфо();
	SetSb();
	if(c >= 0 && c < дайСчёт())
		устКурсор(c);
}

void СписокКолонок::добавь(const Значение& val, bool canselect)
{
	вставь(элт.дайСчёт(), val, val, canselect);
}

void СписокКолонок::добавь(const Значение& val, const Дисплей& дисплей, bool canselect)
{
	добавь(val, val, дисплей, canselect);
}

void СписокКолонок::добавь(const Значение& ключ, const Значение& val, bool canselect)
{
	вставь(элт.дайСчёт(), ключ, val, canselect);
}

void СписокКолонок::добавь(const Значение& ключ, const Значение& val, const Дисплей& дисплей, bool canselect)
{
	добавь(ключ, val, canselect);
	элт.верх().дисплей = &дисплей;
}

void СписокКолонок::ТиБ(int _drop, bool _insert)
{
	if(dropitem != _drop || insert != _insert) {
		освежиЭлт(dropitem - 1);
		освежиЭлт(dropitem);
		dropitem = _drop;
		insert = _insert;
		освежиЭлт(dropitem - 1);
		освежиЭлт(dropitem);
	}
}

bool СписокКолонок::вставьТиБ(int i, int py, PasteClip& d, int q)
{
	Прям r = дайПрямЭлта(i);
	int cy = r.дайВысоту();
	if(py < r.top + cy / q) {
		WhenDropInsert(i, d);
		if(d.IsAccepted()) {
			ТиБ(i, true);
			return true;
		}
	}
	if(py >= r.top + (q - 1) * cy / q && i < дайСчёт()) {
		WhenDropInsert(i + 1, d);
		if(d.IsAccepted()) {
			ТиБ(i + 1, true);
			return true;
		}
	}
	return false;
}

void СписокКолонок::тягИБрос(Точка p, PasteClip& d)
{
	int i = дайЭлт(p);
	if(i >= 0) {
		if(вставьТиБ(i, p.y, d, 4))
			return;
		WhenDropItem(i, d);
		if(d.IsAccepted()) {
			ТиБ(i, false);
			return;
		}
		if(вставьТиБ(i, p.y, d, 2))
			return;
	}
	if(дайСчёт() == 0 && p.y < 4 || !WhenDrop) {
		WhenDropInsert(дайСчёт(), d);
		if(d.IsAccepted()) {
			ТиБ(дайСчёт(), true);
			return;
		}
	}
	WhenDrop(d);
	ТиБ(-1, false);
}

void СписокКолонок::тягПовтори(Точка p)
{
	sb = sb + ((mode == MODE_COLUMN) ? GetDragScroll(this, p, 1).x : GetDragScroll(this, p, 1).y);
}

void СписокКолонок::тягВойди()
{
	освежиВыд();
}

void СписокКолонок::тягПокинь()
{
	ТиБ(-1, false);
	освежиВыд();
}

void СписокКолонок::удалиВыделение()
{
	анулируйКурсор();
	for(int i = дайСчёт() - 1; i >= 0; i--)
		if(IsSel(i))
			удали(i);
	selcount = 0;
}

int СписокКолонок::найди(const Значение &ключ) const
{
	for (int i = 0; i < элт.дайСчёт(); i++)
		if (элт[i].ключ == ключ)
			return i;
	return -1;
}

void СписокКолонок::вставьБрос(int ii, const Вектор<Значение>& keys, const Вектор<Значение>& data, PasteClip& d, bool self)
{
	if(data.дайСчёт() == 0)
		return;
	if(d.GetAction() == DND_MOVE && self) {
		for(int i = дайСчёт() - 1; i >= 0; i--) {
			if(IsSel(i)) {
				удали(i); // Optimize!
				if(i < ii)
					ii--;
			}
		}
		анулируйКурсор();
		d.SetAction(DND_COPY);
	}
	элт.вставьН(ii, data.дайСчёт());
	for(int i = 0; i < data.дайСчёт(); i++) {
		Элемент& m = элт[ii + i];
		m.значение = data[i];
		m.ключ = keys[i];
		m.sel = false;
		m.canselect = true;
		m.дисплей = NULL;
	}
	освежи();
	синхИнфо();
	SetSb();
	очистьВыделение();
	устКурсор(ii + data.дайСчёт() - 1);
	if(IsMultiSelect())
		for(int i = 0; i < data.дайСчёт(); i++)
			выделиОдин(ii + i, true);
}

void СписокКолонок::вставьБрос(int ii, const Вектор<Значение>& data, PasteClip& d, bool self)
{
	вставьБрос(ii, data, data, d, self);
}

void СписокКолонок::вставьБрос(int ii, const СписокКолонок& src, PasteClip& d)
{
	Вектор<Значение> keys;
	Вектор<Значение> data;
	for(int i = 0; i < src.дайСчёт(); i++)
		if(src.IsSel(i)) {
			data.добавь(src.дайЗначение(i));
			keys.добавь(src[i]);
		}
	вставьБрос(ii, keys, data, d, &src == this);
}

void СписокКолонок::вставьБрос(int ii, PasteClip& d)
{
	вставьБрос(ii, GetInternal<СписокКолонок>(d), d);
}

void СписокКолонок::сериализуйНастройки(Поток& s) {
	int version = 0;
	s / version;
	s / ncl;
	освежи();
	синхИнфо();
}

/* Ошибка-prone!
void СписокКолонок::сериализуй(Поток& s) {
	int version = 1;
	s / version / ncl;
	if(version >= 1) {
		int cnt = элт.дайСчёт();
		s.Magic();
		s / cnt;
		элт.устСчёт(cnt);
		for(int i = 0; i < cnt; i++) {
			Элемент &q = элт[i];
			s % q.ключ % q.значение % q.canselect;
			if (s.грузится()) {
				q.дисплей = NULL;
				q.sel = false;
			}
		}
		s.Magic();
	}
	освежи();
	синхИнфо();
}
*/

СписокКолонок& СписокКолонок::Режим(int m)
{
	mode = m;
	scroller.очисть();
	if(m == MODE_COLUMN)
		sb.гориз();
	else
		sb.верт();
	return *this;
}

СписокКолонок::СписокКолонок() {
	clickkill = false;
	ncl = 1;
	cx = 50;
	cy = max(DPI(18), Draw::GetStdFontCy());
	cursor = -1;
	ListMode();
	добавьФрейм(sb);
	sb.ПриПромоте = THISBACK(промотай);
	sb.автоСкрой();
	Ктрл::устФрейм(ViewFrame());
	frame = &ViewFrame();
	RoundSize();
	дисплей = &StdDisplay();
	multi = false;
	selcount = 0;
	nobg = false;
	popupex = true;
}

СписокКолонок::~СписокКолонок() {}

}
