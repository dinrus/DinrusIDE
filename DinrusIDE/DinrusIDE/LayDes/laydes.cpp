#include "LayDes.h"

using namespace LayoutKeys;

#define IMAGECLASS LayImg
#define IMAGEFILE  <DinrusIDE/LayDes/LayDes.iml>
#include <Draw/iml_source.h>

#define LTIMING(x) // TIMING(x)
#define LLOG(x)

#define MARGIN 8

static void sLay1(int& pos, int& r, int align, int a, int b, int sz)
{
	pos = a;
	int size = b;
	switch(align) {
	case Ктрл::CENTER:
		pos = (sz - b) / 2 + a;
		break;
	case Ктрл::RIGHT:
		pos = sz - (a + b);
		break;
	case Ктрл::SIZE:
		size = sz - (a + b);
		break;
	}
	r = pos + max(size, 0);
}

Прям LayDes::CtrlRect(Ктрл::ПозЛога pos, Размер sz)
{
	Прям r;
	sLay1(r.left, r.right, pos.x.дайЛин(), pos.x.GetA(), pos.x.дайС(), sz.cx);
	sLay1(r.top, r.bottom, pos.y.дайЛин(), pos.y.GetA(), pos.y.дайС(), sz.cy);
	return r;
}

Прям LayDes::CtrlRectZ(Ктрл::ПозЛога pos, Размер sz)
{
	Прям r = CtrlRect(pos, sz);
	r.left = HorzLayoutZoom(r.left);
	r.right = HorzLayoutZoom(r.right);
	r.top = VertLayoutZoom(r.top);
	r.bottom = VertLayoutZoom(r.bottom);
	return r;
}

void LayDes::AddHandle(Draw& w, int x, int y)
{
	w.DrawRect(x, y, DPI(6), DPI(6), SColorMark);
	handle.добавь(Точка(x, y));
}

Точка LayDes::нормализуй(Точка p)
{
	p += sb;
	p.смещение(-MARGIN, -MARGIN);
	return p;
}

Точка LayDes::ZPoint(Точка p)
{
	Размер csz, dsz;
	GetZoomRatio(csz, dsz);
	if(csz.cx && csz.cy && dsz.cx && dsz.cy) {
		p.x = p.x * dsz.cx / csz.cx;
		p.y = p.y * dsz.cy / csz.cy;
	}
	return p;
}

void LayDes::SetSb()
{
	Размер sz = Размер(0, 0);
	if(!пусто_ли(currentlayout)) {
		LayoutData& l = CurrentLayout();
		sz = l.size;
		for(int i = 0; i < l.элт.дайСчёт(); i++)
			sz = max(sz, (Размер)CtrlRect(l.элт[i].pos, l.size).низПраво());
	}
	sz += Размер(MARGIN, MARGIN);
	Размер csz, dsz;
	GetZoomRatio(csz, dsz);
	if(csz.cx && csz.cy && dsz.cx && dsz.cy) {
		sz.cx = sz.cx * csz.cx / dsz.cx;
		sz.cy = sz.cy * csz.cy / dsz.cy;
	}
	sb.устВсего(sz);
	sb.устСтраницу(sb.дайРедуцРазмОбзора());
}

void LayDes::промотай()
{
	освежи();
}

void LayDes::Выкладка()
{
	SetSb();
	list.ScrollIntoCursor();
}

void LayDes::GetSprings(Прям& l, Прям& t, Прям& r, Прям& b)
{
	if(пусто_ли(currentlayout) || !cursor.дайСчёт()) {
		l = t = r = b = Null;
		return;
	}
	Прям ir = CtrlRectZ(CurrentItem().pos, CurrentLayout().size);
	int h4 = ir.устВысоту() / 4;
	int w4 = ir.устШирину() / 4;
	int fv = DPI(5);
	int tn = DPI(10);
	l = RectC(-MARGIN, ir.top + h4 - fv, ir.left + MARGIN, tn);
	t = RectC(ir.left + w4 - fv, -MARGIN, tn, ir.top + MARGIN);
	r = RectC(ir.right, ir.bottom - h4 - fv, 9999, tn);
	b = RectC(ir.right - w4 - fv, ir.bottom, tn, 9999);
}

void LayDes::DrawSpring(Draw& w, const Прям& r, bool horz, bool hard)
{
	if(hard)
		w.DrawRect(horz ? r.вертДефлят(4) : r.горизДефлят(4), красный);
	else
		if(horz)
			for(int x = r.left; x < r.right; x += 4)
				w.DrawRect(x, r.top, 1, r.устВысоту(), SColorShadow);
		else
			for(int y = r.top; y < r.bottom; y += 4)
				w.DrawRect(r.left, y, r.устШирину(), 1, SColorShadow);
}

int LayDes::ParseLayoutRef(Ткст cls, Ткст& base) const
{
	const char *p = cls;
	if(p[0] != 'W' || p[1] != 'i' || p[2] != 't' || p[3] != 'h')
		return -1;
	const char *b = (p += 4);
	while(IsAlNum(*p) || *p == '_')
		p++;
	if(p <= b)
		return -1;
	Ткст layoutid(b, p);
	int fi = найдиИндексПоля(layout, &LayoutData::имя, layoutid);
	if(fi < 0)
		return -1;
	while(*p && (byte)*p <= ' ')
		p++;
	if(*p++ != '<')
		return -1;
	while(*p && (byte)*p <= ' ')
		p++;
	const char *e = cls.стоп();
	while(e > p && (byte)e[-1] <= ' ')
		e--;
	if(e > p && e[-1] == '>')
		e--;
	while(e > p && (byte)e[-1] <= ' ')
		e--;
	base = Ткст(p, e);
	return fi;
}

void LayDes::PaintLayoutItems(Draw& w, int layid, Размер size, Индекс<int>& passed, const Вектор<bool>& cursor)
{
	if(passed.найди(layid) >= 0)
		return;
	passed.добавь(layid);
	LayoutData& l = layout[layid];
	Буфер<int> lrs(l.элт.дайСчёт());
	Ткст dummy;
	for(int i = 0; i < l.элт.дайСчёт(); i++)
		lrs[i] = ParseLayoutRef(l.элт[i].тип, dummy);
	for(int i = 0; i < l.элт.дайСчёт(); i++) {
		LayoutItem& m = l.элт[i];
		if(m.hide)
			continue;
		Прям r = CtrlRectZ(m.pos, size);
		DrawFrame(w, r, смешай(SLtGray(), SCyan(), 35));
		bool show = true;
		if(i < cursor.дайСчёт() && !cursor[i] && m.тип != "БоксНадпись") // does this widget hide part of some widget that is current selected?
			for(int j = 0; j < l.элт.дайСчёт(); j++)
				if(j != i && j < cursor.дайСчёт() && cursor[j]) {
					LayoutItem& mm = l.элт[j];
					if(!mm.hide && r.пересекается(CtrlRectZ(mm.pos, size)) && (LayoutTypes().найди(mm.тип) >= 0 || lrs[j] >= 0)
					   && mm.тип != "БоксНадпись" && HasCapture() && findarg(draghandle, 11, 12) >= 0) {
						show = false;
						break;
					}
				}
		
		if(show) {
			w.Clipoff(r);
			if(lrs[i] < 0)
				m.рисуй(w, r.размер());
			else {
				Размер sz = r.дайРазм();
				PaintLayoutItems(w, lrs[i], CtrlRect(m.pos, size).дайРазм(), passed, Вектор<bool>());
				Шрифт fnt = LayFont();
				Ткст s = m.тип;
				Размер tsz = дайРазмТекста(s, fnt);
				int tcy2 = tsz.cy / 2;
				if(tsz.cx + tcy2 > sz.cx && s.обрежьСтарт("With"))
					tsz = дайРазмТекста(s, fnt);
				int q = s.найди('<');
				if(tsz.cx + tcy2 > sz.cx && q >= 0) {
					s.обрежь(q);
					tsz = дайРазмТекста(s, fnt);
				}
				if(tsz.cx + tcy2 > sz.cx && s.обрежьКонец("Выкладка"))
					tsz = дайРазмТекста(s, fnt);
				w.DrawRect(sz.cx - tsz.cx - tsz.cy, sz.cy - tsz.cy, tsz.cx + tsz.cy, tsz.cy, SGray());
				w.DrawText(sz.cx - tsz.cx - tsz.cy / 2, sz.cy - tsz.cy, s, fnt, SYellow());
			}
			w.стоп();
		}
	}
	passed.сбрось();
}

void LayDes::рисуй(Draw& w)
{
	LTIMING("рисуй");
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorPaper);
	if(!пусто_ли(fileerror))
		w.DrawText(16, 16, "FILE Ошибка: " + fileerror, ArialZ(14).Bold(), красный);
	if(пусто_ли(currentlayout))
		return;
	w.смещение(-sb.дай());
	LayoutData& l = CurrentLayout();
	w.смещение(MARGIN, MARGIN);
	Размер lsz = LayoutZoom(l.size);
	w.DrawRect(0, 0, lsz.cx, lsz.cy, SLtGray);
	if(setting.paintgrid) {
		int gx = minmax((int)~setting.gridx, 1, 32);
		int gy = minmax((int)~setting.gridy, 1, 32);
		for(int x = 0; x < lsz.cx; x += gx)
			for(int y = 0; y < lsz.cy; y += gy)
				w.DrawRect(x, y, 1, 1, SColorPaper);
	}
	DrawFrame(w, -1, -1, lsz.cx + 2, lsz.cy + 2, SColorText);
	handle.очисть();
	AddHandle(w, lsz.cx, lsz.cy / 2 - DPI(3));
	AddHandle(w, lsz.cx / 2 - DPI(3), lsz.cy);
	AddHandle(w, lsz.cx, lsz.cy);
	int i;
	Индекс<int> passed;
	Вектор<bool> cursorflags;
	if(!cursor.пустой()) {
		cursorflags.устСчёт(l.элт.дайСчёт(), cursor.пустой());
		for(i = 0; i < cursor.дайСчёт(); i++)
			cursorflags[cursor[i]] = true;
	}
	PaintLayoutItems(w, currentlayout, l.size, passed, cursorflags);
	if(!HasCapture() || draghandle == 14) {
		for(i = 0; i < cursor.дайСчёт(); i++) {
			LayoutItem& m = l.элт[cursor[i]];
			Прям r = CtrlRectZ(m.pos, l.size);
			DrawFatFrame(w, r, i == cursor.дайСчёт() - 1 ? цыан : коричневый, DPI(-3));
			if(i == cursor.дайСчёт() - 1) {
				int lrm = r.left + r.устШирину() / 2 - DPI(3);
				int tbm = r.top + r.устВысоту() / 2 - DPI(3);
				AddHandle(w, r.left - DPI(6), r.top - DPI(6));
				AddHandle(w, lrm, r.top - DPI(6));
				AddHandle(w, r.right, r.top - DPI(6));
				AddHandle(w, r.left - DPI(6), tbm);
				AddHandle(w, r.right, tbm);
				AddHandle(w, r.left - DPI(6), r.bottom);
				AddHandle(w, lrm, r.bottom);
				AddHandle(w, r.right, r.bottom);
			}
		}
		if(cursor.дайСчёт()) {
			LayoutItem& m = CurrentItem();
			Прям l, t, r, b;
			GetSprings(l, t, r, b);
			DrawSpring(w, l, true, m.pos.x.дайЛин() & Ктрл::LEFT);
			DrawSpring(w, r, true, m.pos.x.дайЛин() & Ктрл::RIGHT);
			DrawSpring(w, t, false, m.pos.y.дайЛин() & Ктрл::TOP);
			DrawSpring(w, b, false, m.pos.y.дайЛин() & Ктрл::BOTTOM);
		}
	}
	if(HasCapture() && draghandle == 14)
		DrawFrame(w, dragrect.нормализат(), светлоКрасный);
	w.стоп();
	w.стоп();
}

void  LayDes::SaveState()
{
	if(пусто_ли(currentlayout))
		return;
	CurrentLayout().SaveState();
	SetBar();
}

void  LayDes::SetStatus(bool down)
{
	Ткст s;
	if(!пусто_ли(currentlayout)) {
		Размер sz = CurrentLayout().size;
		s << sz;
		if(cursor.дайСчёт()) {
			Прям r = CtrlRect(CurrentItem().pos, sz);
			s << ": " << r << " - {" << sz.cx - r.right << ", " << sz.cy - r.bottom << '}';
		}
	}
	status.устНадпись(s);
	освежи();
	SetBar();
	SetSb();
}

int   LayDes::FindHandle(Точка p)
{
	for(int i = 0; i < handle.дайСчёт(); i++) {
		Точка h = handle[i];
		if(p.x >= h.x - DPI(1) && p.x < h.x + DPI(7) && p.y >= h.y - DPI(1) && p.y < h.y + DPI(7))
			return i;
	}
	return -1;
}

int   LayDes::FindItem(Точка p, bool cursor_first)
{
	if(пусто_ли(currentlayout))
		return -1;
	LayoutData& l = CurrentLayout();
	int ii = -1;
	int min = INT_MAX;
	for(int pass = !cursor_first; pass < 2 && ii < 0; pass++)
		for(int i = 0; i < (pass ? l.элт.дайСчёт() : cursor.дайСчёт()); i++) {
			int i2 = pass ? i : cursor[i];
			LayoutItem& m = l.элт[i2];
			if(m.hide)
				continue;
			Прям r = CtrlRect(m.pos, l.size);
			if(r.содержит(p)) {
				int mm = r.устШирину() * r.устВысоту();
				if(mm < min) {
					ii = i2;
					min = mm;
				}
			}
		}
	return ii;
}

Рисунок LayDes::рисКурсора(Точка p, dword keyflags)
{
	int hi;
	if(HasCapture())
		hi = draghandle;
	else
		hi = FindHandle(нормализуй(p));
	Рисунок (*id[11])() = {
		Рисунок::SizeHorz, Рисунок::SizeVert, Рисунок::SizeBottomRight,
		Рисунок::SizeTopLeft, Рисунок::SizeVert, Рисунок::SizeTopRight,
		Рисунок::SizeHorz, Рисунок::SizeHorz,
		Рисунок::SizeBottomLeft, Рисунок::SizeVert, Рисунок::SizeBottomRight,
	};
	if(hi >= 0 && hi < 11)
		return (*id[hi])();
	return Рисунок::Arrow();
}

Ктрл::Logc MakeLogc(int align, int a, int b, int sz)
{
	int isz = b - a;
	switch(align) {
	case Ктрл::LEFT:
		return Ктрл::позЛев(a, isz);
	case Ктрл::RIGHT:
		return Ктрл::позПрав(sz - b, isz);
	case Ктрл::CENTER:
		return Ктрл::позЦентр(isz, a - (sz - isz) / 2);
	}
	return Ктрл::позРазмер(a, sz - b);
}

Ктрл::ПозЛога MakeLogPos(int ax, int ay, const Прям& r, Размер sz)
{
	return Ктрл::ПозЛога(MakeLogc(ax, r.left, r.right, sz.cx),
	                    MakeLogc(ay, r.top, r.bottom, sz.cy));
}

Ктрл::ПозЛога MakeLogPos(Ктрл::ПозЛога p, const Прям& r, Размер sz)
{
	return MakeLogPos(p.x.дайЛин(), p.y.дайЛин(), r, sz);
}

struct IDisplay : public Дисплей {
	Цвет paper, ink;
	Шрифт  font;
	
	Размер дайСтдРазм(const Значение& q) const {
		Размер sz = GetSmartTextSize(~q, font);
		sz.cx += 2 * DPI(4);
		return sz;
	}

	void рисуй(Draw& w, const Прям& r, const Значение& q,
	           Цвет, Цвет, dword s) const {
		w.DrawRect(r, paper);
		DrawSmartText(w, r.left + DPI(4), r.top, r.устШирину(), ~q, font, ink);
	}
	IDisplay(Цвет paper, Цвет ink, Шрифт font = StdFont())
		: paper(paper), ink(ink), font(font) {}
};

struct TDisplay : public Дисплей {
	Цвет paper, ink;
	Шрифт  font;

	Размер дайСтдРазм(const Значение& q) const {
		Размер sz = GetSmartTextSize(~q, font);
		sz.cx += 2 * DPI(4) + DPI(6) + sz.cy * 8 / 3;
		return sz;
	}

	void рисуй(Draw& w, const Прям& r, const Значение& q,
	           Цвет, Цвет, dword s) const {
		w.DrawRect(r, paper);
		int dx = r.устВысоту() * 8 / 3;
		Рисунок icon = GetTypeIcon(Ткст(q), dx, r.устВысоту(), 1, paper);
		w.DrawImage(r.left + DPI(4), r.top + (r.устВысоту() - icon.дайРазм().cy) / 2, icon);
		DrawSmartText(w, r.left + dx + DPI(6), r.top, r.устШирину(), ~q, font, ink);
	}
	TDisplay(Цвет paper, Цвет ink, Шрифт font = StdFont())
		: paper(paper), ink(ink), font(font) {}
};

void LayDes::SyncItems()
{
	LTIMING("SyncItems");
	if(пусто_ли(currentlayout))
		return;
	int i;
	for(i = 0; i < элт.дайСчёт(); i++)
		SyncItem(i, 0);
	for(i = 0; i < cursor.дайСчёт(); i++)
		SyncItem(cursor[i], 1);
	if(cursor.дайСчёт()) {
		SyncItem(cursor.верх(), 2);
		элт.устКурсор(cursor.верх());
	}
	else
		элт.анулируйКурсор();
	SetStatus();
	SyncProperties(true);
}

void LayDes::SyncItem(int i, int style)
{
	if(CurrentLayout().элт[i].hide) {
		static IDisplay dnormal(SColorPaper, SColorText, StdFont().Italic());
		static IDisplay dselect(SColorLtFace, SColorText, StdFont().Italic());
		static IDisplay dcursor(SColorFace, SColorText, StdFont().Italic());
		static TDisplay tnormal(SColorPaper, SColorText, StdFont().Italic());
		static TDisplay tselect(SColorLtFace, SColorText, StdFont().Italic());
		static TDisplay tcursor(SColorFace, SColorText, StdFont().Italic());
		static IDisplay lnormal(SColorPaper, зелёный, StdFont().Italic());
		static IDisplay lselect(SColorLtFace, зелёный, StdFont().Italic());
		static IDisplay lcursor(SColorFace, зелёный, StdFont().Italic());
		const Дисплей *noicon[] = { &dnormal, &dselect, &dcursor };
		const Дисплей *isicon[] = { &tnormal, &tselect, &tcursor };
		const Дисплей *label[] = { &lnormal, &lselect, &lcursor };
		bool icons = setting.showicons;
		элт.устДисплей(i, 0, *(icons ? isicon : noicon)[style]);
		элт.устДисплей(i, 1, *(пусто_ли(CurrentLayout().элт[i].variable) ? label : noicon)[style]);
	}
	else {
		static IDisplay dnormal(SColorPaper, SColorText);
		static IDisplay dselect(SColorLtFace, SColorText);
		static IDisplay dcursor(SColorFace, SColorText);
		static TDisplay tnormal(SColorPaper, SColorText);
		static TDisplay tselect(SColorLtFace, SColorText);
		static TDisplay tcursor(SColorFace, SColorText);
		static IDisplay lnormal(SColorPaper, зелёный);
		static IDisplay lselect(SColorLtFace, зелёный);
		static IDisplay lcursor(SColorFace, зелёный);
		const Дисплей *noicon[] = { &dnormal, &dselect, &dcursor };
		const Дисплей *isicon[] = { &tnormal, &tselect, &tcursor };
		const Дисплей *label[] = { &lnormal, &lselect, &lcursor };
		bool icons = setting.showicons;
		элт.устДисплей(i, 0, *(icons ? isicon : noicon)[style]);
		элт.устДисплей(i, 1, *(пусто_ли(CurrentLayout().элт[i].variable) ? label : noicon)[style]);
	}
}

void LayDes::SyncProperties(bool sync_type_var)
{
	property.очисть();
	if(sync_type_var) {
		тип.откл();
		variable.откл();
		тип <<= variable <<= Null;
	}
	if(cursor.дайСчёт()) {
		LayoutItem& m = CurrentItem();
		int i;
		for(i = 0; i < m.property.дайСчёт(); i++) {
			property.добавь(m.property[i]);
			m.property[i].WhenAction = THISBACK(PropertyChanged);
		}
		if(sync_type_var) {
			Ткст tp = m.тип;
			тип <<= m.тип;
			for(i = 0; i < cursor.дайСчёт() - 1; i++)
				if(CurrentLayout().элт[cursor[i]].тип != tp) {
					тип <<= Null;
					break;
				}
			variable <<= m.variable;
			тип.вкл();
			variable.вкл();
		}
	}
}

void LayDes::выделиОдин(int ii, dword flags)
{
	if(ii < 0) {
		if(flags & (K_SHIFT|K_CTRL))
			return;
		cursor.очисть();
	}
	else
	if(flags & (K_SHIFT|K_CTRL)) {
		int q = найдиИндекс(cursor, ii);
		if(q >= 0)
			cursor.удали(q);
		else
			cursor.добавь(ii);
	}
	else {
		cursor.очисть();
		cursor.добавь(ii);
	}
	SyncItems();
}

void LayDes::StoreItemRects()
{
	if(пусто_ли(currentlayout))
		return;
	LayoutData& l = CurrentLayout();
	itemrect.устСчёт(cursor.дайСчёт());
	for(int i = 0; i < cursor.дайСчёт(); i++)
		itemrect[i] = CtrlRect(l.элт[cursor[i]].pos, l.size);
}

void  LayDes::леваяВнизу(Точка p, dword keyflags)
{
	if(пусто_ли(currentlayout))
		return;
	SaveState();
	устФокус();
	SetCapture();
	LayoutData& l = CurrentLayout();
	draglayoutsize = l.size;
	p = нормализуй(p);
	draghandle = FindHandle(p);
	dragbase = ZPoint(p);
	if(draghandle >= 0)
		StoreItemRects();
	else {
		int ii = FindItem(dragbase, true);
		if(ii >= 0) {
			if(дайШифт() || дайКтрл() || найдиИндекс(cursor, ii) < 0)
				выделиОдин(ii, keyflags);
			if(cursor.дайСчёт()) {
				draghandle = 11;
				StoreItemRects();
				SetStatus();
			}
		}
		else {
			if(cursor.дайСчёт()) {
				LayoutItem& m = CurrentItem();
				StoreItemRects();
				Прям lr, tr, rr, br;
				GetSprings(lr, tr, rr, br);
				int xa = m.pos.x.дайЛин();
				int ya = m.pos.y.дайЛин();
				if(lr.содержит(p))
					xa ^= 1;
				if(rr.содержит(p))
					xa ^= 2;
				if(tr.содержит(p))
					ya ^= 1;
				if(br.содержит(p))
					ya ^= 2;
				if(xa != m.pos.x.дайЛин() || ya != m.pos.y.дайЛин()) {
					SetSprings(MAKELONG(xa, ya));
					SetStatus();
					return;
				}
			}
			dragrect.left = dragrect.right = p.x;
			dragrect.top = dragrect.bottom = p.y;
			draghandle = 14;
			if(дайКтрл() || дайШифт())
				basesel = cursor.дайСчёт();
			else {
				basesel = 0;
				cursor.очисть();
			}
			SyncItems();
		}
	}
	SetStatus(true);
}

void  LayDes::леваяПовтори(Точка p, dword keyflags)
{
	двигМыши(p, keyflags);
}

void  LayDes::двигМыши(Точка p, dword keyflags)
{
	if(!HasCapture() || пусто_ли(currentlayout))
		return;
	Точка pz = нормализуй(p);
	p = ZPoint(pz);
	LayoutData& l = CurrentLayout();
	bool smallmove = max(абс(p.x - dragbase.x), абс(p.y - dragbase.y)) < 4;
	if(draghandle == 14) {
		dragrect.right = pz.x;
		dragrect.bottom = pz.y;
		cursor.устСчёт(basesel);
		Прям r = dragrect.нормализат();
		r = Прям(ZPoint(r.верхЛево()), ZPoint(r.низПраво()));
		int mind = INT_MAX;
		int mini = -1;
		for(int i = 0; i < l.элт.дайСчёт(); i++) {
			LayoutItem& m = l.элт[i];
			Прям ir = CtrlRect(m.pos, l.size);
			if(r.содержит(ir) && найдиИндекс(cursor, i) < 0) {
				Точка ip = ir.центрТочка();
				int mm = (ip.x - dragrect.left) * (ip.x - dragrect.left)
				       + (ip.y - dragrect.top) * (ip.y - dragrect.top);
				if(mm < mind) {
					mind = mm;
					mini = cursor.дайСчёт();
				}
				cursor.добавь(i);
			}
		}
		if(mini >= 0)
			разверни(cursor.верх(), cursor[mini]);
		if(cursor.дайСчёт())
			элт.устКурсор(cursor.верх());
		else
			элт.анулируйКурсор();
		SyncItems();
		SetStatus(false);
		LTIMING("двигМыши синх");
		синх();
		return;
	}
	int gx = 1;
	int gy = 1;
	if(usegrid == !(keyflags & K_ALT)) {
		gx = minmax((int)~setting.gridx, 1, 32);
		gy = minmax((int)~setting.gridy, 1, 32);
	}
	p -= dragbase;
	if(draghandle < 3) {
		Вектор<Прям> r;
		r.устСчёт(l.элт.дайСчёт());
		for(int i = 0; i < l.элт.дайСчёт(); i++)
			r[i] = CtrlRect(l.элт[i].pos, l.size);
		if((draghandle + 1) & 1)
			l.size.cx = max(0, draglayoutsize.cx + p.x) / gx * gx;
		if((draghandle + 1) & 2)
			l.size.cy = max(0, draglayoutsize.cy + p.y) / gy * gy;
		if(!sizespring)
			for(int i = 0; i < l.элт.дайСчёт(); i++) {
				LayoutItem& m = l.элт[i];
				m.pos = MakeLogPos(m.pos, r[i], l.size);
			}
		SetStatus(true);
		SetSb();
		синх();
		return;
	}
	if(!элт.курсор_ли())
		return;
	if(draghandle == 11) {
		if(smallmove)
			return;
		draghandle = 12;
	}
	Точка md = Точка(0, 0);
	for(int i = 0; i < cursor.дайСчёт(); i++) {
		LayoutItem& m = l.элт[cursor[i]];
		Прям r = itemrect[i];
		Размер minsize = ignoreminsize ? Размер(0, 0) : m.дайМинРазм();
		if(keyflags & K_CTRL)
			minsize = Размер(0, 0);
		if(draghandle == 3 || draghandle == 4 || draghandle == 5)
			r.top = min(r.bottom - minsize.cy, (r.top + p.y) / gy * gy);
		if(draghandle == 8 || draghandle == 9 || draghandle == 10)
			r.bottom = max(r.top + minsize.cy, (r.bottom + p.y) / gy * gy);
		if(draghandle == 3 || draghandle == 6 || draghandle == 8)
			r.left = min(r.right - minsize.cx, (r.left + p.x) / gy * gy);
		if(draghandle == 5 || draghandle == 7 || draghandle == 10)
			r.right = max(r.left + minsize.cx, (r.right + p.x) / gy * gy);
		if(draghandle == 12) {
			Размер sz = r.дайРазм();
			if(i == 0) {
				Прям q = r;
				r.left = (r.left + p.x) / gx * gx;
				r.top = (r.top + p.y) / gy * gy;
				md = r.верхЛево() - q.верхЛево();
			}
			else
				r += md;
			r.устРазм(sz);
		}
		m.pos = MakeLogPos(m.pos, r, draglayoutsize);
	}
	SetStatus(true);
	синх();
}

void  LayDes::леваяВверху(Точка p, dword keyflags)
{
	if(draghandle == 11 && (keyflags & (K_SHIFT|K_CTRL)) == 0)
		выделиОдин(FindItem(ZPoint(нормализуй(p))), 0);
	draghandle = -1;
	SyncItems();
}

void LayDes::CreateCtrl(const Ткст& _type)
{
	if(пусто_ли(currentlayout))
		return;
	LayoutData& l = CurrentLayout();
	int c = l.элт.дайСчёт();
	if(cursor.дайСчёт())
		c = макс(cursor) + 1;
	LayoutItem& m = l.элт.вставь(c);
	m.создай(_type);
	Точка p = dragbase;
	Размер sza, szb;
	GetZoomRatio(sza, szb);
	if(sza.cx)
		p.x = szb.cx * p.x / sza.cx;
	if(sza.cy)
		p.y = szb.cy * p.y / sza.cy;
	if(usegrid) {
		p.x = p.x / (int)~setting.gridx * (int)~setting.gridx;
		p.y = p.y / (int)~setting.gridy * (int)~setting.gridy;
	}
	Прям r(p, m.дайСтдРазм());
	m.pos = MakeLogPos(Ктрл::LEFT, Ктрл::TOP, r, l.size);
	cursor.очисть();
	cursor.добавь(c);
	ReloadItems();
	if(!search.естьФокус()) {
		if(пусто_ли(_type))
			тип.устФокус();
		else {
			int q = m.FindProperty("устНадпись");
			if(q >= 0 && findarg(_type, "Надпись", "БоксНадпись") >= 0)
				m.property[q].PlaceFocus(0, 0);
			else
				variable.устФокус();
		}
	}
}

void LayDes::Группа(Бар& bar, const Ткст& группа)
{
	int i;
	Вектор<Ткст> тип;
	for(i = 0; i < LayoutTypes().дайСчёт(); i++) {
		LayoutType& m = LayoutTypes()[i];
		if((пусто_ли(группа) || m.группа == группа) && m.kind == LAYOUT_CTRL)
			тип.добавь(LayoutTypes().дайКлюч(i));
	}
	сортируй(тип);
	int h = 3 * StdFont().Info().дайВысоту() / 2;
	int w = 8 * h / 3;
	if(auto *mb = dynamic_cast<БарМеню *>(&bar))
		mb->LeftGap(w + 6);
	int q = 0;
	for(i = 0; i < тип.дайСчёт(); i++) {
		bar.добавь(тип[i], GetTypeIcon(тип[i], w, h, 0, SLtGray),
		        THISBACK1(CreateCtrl, тип[i]));
		if((q++ + 2) % 16 == 0)
			bar.Break();
	}
}

void LayDes::TemplateGroup(Бар& bar, TempGroup tg)
{
	int i;
	Вектор<Ткст> тип;
	for(i = 0; i < LayoutTypes().дайСчёт(); i++) {
		LayoutType& m = LayoutTypes()[i];
		if((пусто_ли(tg.группа) || m.группа == tg.группа) && m.kind == LAYOUT_CTRL)
			тип.добавь(LayoutTypes().дайКлюч(i));
	}
	сортируй(тип);
	int h = 3 * StdFont().Info().дайВысоту() / 2;
	int w = 8 * h / 3;
	if(auto *mb = dynamic_cast<БарМеню *>(&bar))
		mb->LeftGap(w + 6);
	int q = 0;
	for(i = 0; i < тип.дайСчёт(); i++) {
		bar.добавь(тип[i], GetTypeIcon(тип[i], w, h, 0, SLtGray),
		        THISBACK1(CreateCtrl, tg.temp + '<' + тип[i] + '>'));
		if((q++ + 2) % 16 == 0)
			bar.Break();
	}
}

void LayDes::Template(Бар& bar, const Ткст& temp)
{
	Индекс<Ткст> группа;
	Вектор<Ткст> тип;
	int h = 3 * StdFont().Info().дайВысоту() / 2;
	int w = 8 * h / 3;
	if(auto *mb = dynamic_cast<БарМеню *>(&bar))
		mb->LeftGap(w + 6);
	int i;
	for(i = 0; i < LayoutTypes().дайСчёт(); i++) {
		LayoutType& m = LayoutTypes()[i];
		if(!пусто_ли(m.группа))
			группа.найдиДобавь(m.группа);
		else
		if(m.kind == LAYOUT_CTRL)
			тип.добавь(LayoutTypes().дайКлюч(i));
	}
	Вектор<Ткст> sg = группа.подбериКлючи();
	сортируй(sg);
	сортируй(тип);
	int q = 0;
	for(i = 0; i < sg.дайСчёт(); i++) {
		bar.добавь(sg[i], THISBACK1(TemplateGroup, TempGroup(temp, sg[i])));
		if((q++ + 2) % 16 == 0)
			bar.Break();
	}
	bar.добавь("All", THISBACK1(TemplateGroup, TempGroup(temp, Ткст())));
	if((q++ + 2) % 16 == 0)
		bar.Break();
	for(i = 0; i < тип.дайСчёт(); i++) {
		bar.добавь(тип[i], GetTypeIcon(тип[i], w, h, 0, SLtGray),
		        THISBACK1(CreateCtrl, temp + '<' + тип[i] + '>'));
		if((q++ + 2) % 16 == 0)
			bar.Break();
	}
}

void LayDes::Templates(Бар& bar)
{
	Вектор<Ткст> temp;
	int i;
	for(i = 0; i < LayoutTypes().дайСчёт(); i++)
		if(LayoutTypes()[i].kind == LAYOUT_TEMPLATE)
			temp.добавь(LayoutTypes().дайКлюч(i));
	сортируй(temp);
	int q = 0;
	for(i = 0; i < temp.дайСчёт(); i++) {
		bar.добавь(temp[i], THISBACK1(Template, temp[i]));
		if((q++ + 2) % 16 == 0)
			bar.Break();
	}
}

void LayDes::праваяВнизу(Точка p, dword keyflags)
{
	if(пусто_ли(currentlayout) || HasCapture()) return;
	dragbase = нормализуй(p);
	БарМеню menu;
	menu.MaxIconSize(Размер(64, 64));
	int h = StdFont().Info().дайВысоту();
	int w = 8 * h / 3;
	menu.LeftGap(w + 2);
	menu.добавь("User class", THISBACK1(CreateCtrl, ""));
	menu.Separator();
	Индекс<Ткст> группа;
	Вектор<Ткст> тип;
	int i;
	for(i = 0; i < LayoutTypes().дайСчёт(); i++) {
		LayoutType& m = LayoutTypes()[i];
		if(!пусто_ли(m.группа))
			группа.найдиДобавь(m.группа);
		else
		if(m.kind == LAYOUT_CTRL)
			тип.добавь(LayoutTypes().дайКлюч(i));
	}
	Вектор<Ткст> sg = группа.подбериКлючи();
	сортируй(sg);
	сортируй(тип);
	int q = 0;
	for(i = 0; i < sg.дайСчёт(); i++) {
		menu.добавь(sg[i], THISBACK1(Группа, sg[i]));
		if((q++ + 2) % 16 == 0)
			menu.Break();
	}
	menu.добавь("All", THISBACK1(Группа, Ткст()));
	menu.добавь("Templates", THISBACK(Templates));
	if((q++ + 2) % 16 == 0)
		menu.Break();
	for(i = 0; i < тип.дайСчёт(); i++) {
		menu.добавь(тип[i], GetTypeIcon(тип[i], w, h, 0, SLtGray),
		                  THISBACK1(CreateCtrl, тип[i]));
		if((q++ + 2) % 16 == 0)
			menu.Break();
	}
	menu.выполни();
}

void  LayDes::LoadItems()
{
	int nitems = CurrentLayout().элт.дайСчёт();
	элт.устСчёт(nitems);
	for(int i = 0; i < nitems; i++)
		LoadItem(i);
	property.очисть();
}

Ткст GetLabel(const LayoutItem& m)
{
	EscValue l = m.ExecuteMethod("GetLabelMethod");
	if(l.проц_ли())
		for(int p = 0; p < m.property.дайСчёт(); p++)
			if(m.property[p].имя == "устНадпись") {
				Значение prop = ~m.property[p];
				return ткст_ли(prop) && !пусто_ли(prop) ? какТкстСи(prop) : Null;
			}
	return l;
}

void LayDes::LoadItem(int ii)
{
	const LayoutItem& m = CurrentLayout().элт[ii];
	Ткст varlbl = m.variable;
	if(пусто_ли(varlbl))
		varlbl = GetLabel(m);
	элт.уст(ii, 0, m.тип);
	элт.уст(ii, 1, varlbl);
	элт.уст(ii, 2, m.hide);
}

void  LayDes::ReloadItems()
{
	int q = элт.дайПромотку();
	LoadItems();
	элт.ScrollTo(q);
	SyncItems();
}

void  LayDes::Undo()
{
	if(пусто_ли(currentlayout))
		return;
	if(CurrentLayout().IsUndo()) {
		CurrentLayout().Undo();
		cursor.очисть();
		ReloadItems();
	}
}

void LayDes::Redo()
{
	if(пусто_ли(currentlayout))
		return;
	if(CurrentLayout().IsRedo()) {
		CurrentLayout().Redo();
		cursor.очисть();
		ReloadItems();
	}
}

void LayDes::вырежь()
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	копируй();
	Delete();
}

void LayDes::Delete()
{
	SaveState();
	Вектор<int> sel(cursor, 1);
	сортируй(sel);
	cursor.очисть();
	CurrentLayout().элт.удали(sel);
	ReloadItems();
}

Ткст LayDes::SaveSelection(bool scrolled)
{
	return CurrentLayout().сохрани(cursor, scrolled * ZPoint(sb).y, "\r\n") + "\r\n";
}

LayoutData LayDes::LoadLayoutData(const Ткст& s)
{
	try {
		LayoutData l;
		l.устНабсим(charset);
		СиПарсер p(s);
		l.читай(p);
		return l;
	}
	catch(СиПарсер::Ошибка) {}
	return LayoutData();
}

void LayDes::копируй()
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	WriteClipboardUnicodeText(вЮникод(SaveSelection(), charset));
}

void LayDes::выбериВсе()
{
	if(пусто_ли(currentlayout))
		return;
	LayoutData& l = CurrentLayout();
	int q = cursor.дайСчёт() ? cursor.верх() : -1;
	cursor.очисть();
	for(int i = 0; i < l.элт.дайСчёт(); i++)
		if(i != q)
			cursor.добавь(i);
	if(q >= 0)
		cursor.добавь(q);
	SyncItems();
}

void LayDes::Duplicate()
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	SaveState();
	LayoutData& l = CurrentLayout();
	LayoutData d = LoadLayoutData(SaveSelection(false));
	int q = макс(cursor) + 1;
	cursor.очисть();
	for(int i = 0; i < d.элт.дайСчёт(); i++) {
		LayoutItem& m = d.элт[i];
		d.элт[i].pos = MakeLogPos(m.pos, CtrlRect(m.pos, l.size).смещенец(0, 24), l.size);
		cursor.добавь(q + i);
	}
	CurrentLayout().элт.вставьПодбор(q, pick(d.элт));
	ReloadItems();
}

void LayDes::Matrix()
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	SaveState();
	if(matrix.выполни() != IDOK)
		return;
	LayoutData& l = CurrentLayout();
	Прям r = CtrlRect(l.элт[cursor[0]].pos, l.size);
	for(int i = 1; i < cursor.дайСчёт(); i++)
		r.союз(CtrlRect(l.элт[cursor[i]].pos, l.size));
	Ткст ls = SaveSelection();
	int q = макс(cursor) + 1;
	for(int x = 0; x < Nvl((int)~matrix.nx, 1); x++)
		for(int y = 0; y < Nvl((int)~matrix.ny, 1); y++)
			if(x || y) {
				LayoutData d = LoadLayoutData(ls);
				for(int i = 0; i < d.элт.дайСчёт(); i++) {
					LayoutItem& m = d.элт[i];
					Прям r = CtrlRect(m.pos, l.size);
					r.смещение((r.устШирину() + Nvl((int)~matrix.dx)) * x,
					         (r.устВысоту() + Nvl((int)~matrix.dy)) * y);
					d.элт[i].pos = MakeLogPos(m.pos, r, l.size);
					cursor.добавь(q + i);
				}
				int w = d.элт.дайСчёт();
				CurrentLayout().элт.вставьПодбор(q, pick(d.элт));
				q += w;
			}
	ReloadItems();
}

void LayDes::Paste()
{
	if(пусто_ли(currentlayout))
		return;
	SaveState();
	try {
		LayoutData l = LoadLayoutData(изЮникода(ReadClipboardUnicodeText(), charset));
		int q = элт.дайСчёт();
		if(cursor.дайСчёт())
		{
			q = 0;
			for(int i = 0; i < cursor.дайСчёт(); i++)
				q = max(q, cursor[i] + 1);
		}
		cursor.очисть();
		for(int i = 0; i < l.элт.дайСчёт(); i++)
			cursor.добавь(i + q);
		CurrentLayout().элт.вставьПодбор(q, pick(l.элт));
		ReloadItems();
	}
	catch(СиПарсер::Ошибка) {}
}

void LayDes::Align(int тип)
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	SaveState();
	LayoutData& l = CurrentLayout();
	Прям cr = CtrlRect(l.элт[cursor.верх()].pos, l.size);
	for(int i = 0; i < cursor.дайСчёт(); i++) {
		LayoutItem& m = l.элт[cursor[i]];
		Прям r = CtrlRect(m.pos, l.size);
		switch(тип) {
		case A_LEFT:
			r.смещениеГориз(cr.left - r.left);
			break;
		case A_HCENTER:
			r.смещениеГориз(cr.left + (cr.устШирину() - r.устШирину()) / 2 - r.left);
			break;
		case A_RIGHT:
			r.смещениеГориз(cr.right - r.right);
			break;
		case A_TOP:
			r.смещениеВерт(cr.top - r.top);
			break;
		case A_VCENTER:
			r.смещениеВерт(cr.top + (cr.устВысоту() - r.устВысоту()) / 2 - r.top);
			break;
		case A_BOTTOM:
			r.смещениеВерт(cr.bottom - r.bottom);
			break;
		case A_SAMEWIDTH:
			r.right = r.left + cr.устШирину();
			break;
		case A_SAMEHEIGHT:
			r.bottom = r.top + cr.устВысоту();
			break;
		case A_SAMESIZE:
			r.устРазм(cr.размер());
			break;
		case A_HORZCENTER:
			r.смещениеГориз((l.size.cx - r.устШирину()) / 2 - r.left);
			break;
		case A_VERTCENTER:
			r.смещениеВерт((l.size.cy - r.устВысоту()) / 2 - r.top);
			break;
		case A_MINWIDTH:
			r.устРазм(m.дайМинРазм().cx, r.устВысоту());
			break;
		case A_MINHEIGHT:
			r.устРазм(r.устШирину(), m.дайМинРазм().cy);
			break;
		case A_LABEL:
			if(m.тип == "Надпись") {
				Прям rr = r;
				int q = cursor[i] - 1;
				while(q >= 0) {
					if(l.элт[q].тип != "Надпись") {
						rr = CtrlRect(l.элт[q].pos, l.size);
						break;
					}
					q--;
				}
				q = cursor[i] + 1;
				while(q < l.элт.дайСчёт()) {
					if(l.элт[q].тип != "Надпись") {
						rr = CtrlRect(l.элт[q].pos, l.size);
						break;
					}
					q++;
				}
				r.смещениеВерт(rr.top + (rr.устВысоту() - r.устВысоту()) / 2 - r.top);
			}
			break;
		}
		m.pos = MakeLogPos(m.pos, r, l.size);
//		if(i == cursor.дайСчёт() - 1)
//			sb.промотайДо(r.смещенец(MARGIN, MARGIN));
	}
	SetStatus();
}

void LayDes::SetSprings(dword s)
{
	if(пусто_ли(currentlayout))
		return;
	LayoutData& l = CurrentLayout();
	SaveState();
	int xa = (int16)LOWORD(s);
	int ya = (int16)HIWORD(s);
	for(int i = 0; i < cursor.дайСчёт(); i++) {
		Ктрл::ПозЛога& pos = l.элт[cursor[i]].pos;
		Прям r = CtrlRect(pos, l.size);
		if(xa >= 0)
			pos.x = MakeLogc(xa, r.left, r.right, l.size.cx);
		if(ya >= 0)
			pos.y = MakeLogc(ya, r.top, r.bottom, l.size.cy);
		if(xa == AUTOSPRING) {
			pos.x = MakeLogc((r.left < l.size.cx / 2 ? LEFT : 0) |
			                 (r.right > l.size.cx / 2 ? RIGHT : 0),
			                 r.left, r.right, l.size.cx);
			pos.y = MakeLogc((r.top < l.size.cy / 2 ? TOP : 0) |
			                 (r.bottom > l.size.cy/ 2 ? BOTTOM : 0),
			                 r.top, r.bottom, l.size.cy);
		}
	}
	SetStatus();
}

void LayDes::ShowSelection(bool s)
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	LayoutData& l = CurrentLayout();
	for(int i = 0; i < cursor.дайСчёт(); i++)
		l.элт[cursor[i]].hide = !s;
	SyncItems();
	освежи();
}

void LayDes::MoveUp()
{
	SaveState();
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	LayoutData& l = CurrentLayout();
	Вектор<int> sc(cursor, 1);
	сортируй(sc);
	int q = 0;
	while(q < sc.дайСчёт() && sc[q] == q)
		q++;
	int im = q;
	while(q < sc.дайСчёт()) {
		int i = sc[q++];
		l.элт.разверни(i, i - 1);
	}
	for(q = 0; q < cursor.дайСчёт(); q++)
		if(cursor[q] >= im)
			cursor[q]--;
	ReloadItems();
}

void LayDes::MoveDown()
{
	SaveState();
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	LayoutData& l = CurrentLayout();
	Вектор<int> sc(cursor, 1);
	сортируй(sc);
	int q = sc.дайСчёт() - 1;
	int u = l.элт.дайСчёт() - 1;
	while(q >= 0 && sc[q] == u--)
		q--;
	int im = q >= 0 ? sc[q] : -1;
	while(q >= 0) {
		int i = sc[q--];
		l.элт.разверни(i, i + 1);
	}
	for(q = 0; q < cursor.дайСчёт(); q++)
		if(cursor[q] <= im)
			cursor[q]++;
	ReloadItems();
}

void LayDes::вставьТиБ(int line, PasteClip& d)
{
	if(GetInternalPtr<КтрлМассив>(d, "layout-элт") == &элт && элт.курсор_ли() &&
	   !пусто_ли(currentlayout) && cursor.дайСчёт() && d.прими()) {
		SaveState();
		LayoutData& l = CurrentLayout();
		Буфер<bool> sel(l.элт.дайСчёт(), false);
		int n = l.элт.дайСчёт();
		l.элт.вставьН(n, n);
		for(int i = 0; i < cursor.дайСчёт(); i++)
			sel[cursor[i]] = true;
		cursor.очисть();
		int j = n;
		for(int i = 0; i < line; i++)
			if(!sel[i])
				l.элт.разверни(j++, i);
		for(int i = 0; i < n; i++)
			if(sel[i]) {
				cursor.добавь(j - n);
				l.элт.разверни(j++, i);
			}
		for(int i = line; i < n; i++)
			if(!sel[i])
				l.элт.разверни(j++, i);
		l.элт.удали(0, n);
		ReloadItems();
	}
}

void LayDes::тяни()
{
	элт.DoDragAndDrop(InternalClip(элт, "layout-элт"), элт.дайСэиплТяга(), DND_MOVE);
}


bool RectLess(const Прям& a, const Прям& b)
{
	int d = min(a.bottom, b.bottom) - max(a.top, b.top);
	int w = min(a.дайВысоту(), b.дайВысоту());
	return d > w / 2 ? a.left < b.left : a.top < b.top;
}

void LayDes::SortItems()
{
	SaveState();
	if(пусто_ли(currentlayout) || cursor.дайСчёт() < 2)
		return;
	LayoutData& l = CurrentLayout();

	сортируй(cursor);
	int count = cursor.дайСчёт();

	Массив<LayoutItem> элт;
	Вектор<Прям> rect;
	for(int i = 0; i < count; ++i) {
		rect.добавь(CtrlRect(l.элт[cursor[i]].pos, l.size));
		элт.добавь() = pick(l.элт[cursor[i]]);
	}
	l.элт.удали(cursor);

	bool swap = false;
	do {
		swap = false;
		for(int i = 0; i < count - 1; i++)
			if(RectLess(rect[i + 1], rect[i])) {
				разверни(rect[i], rect[i + 1]);
				разверни(элт[i], элт[i + 1]);
				swap = true;
			}
	}
	while(swap);
	
	int ii = cursor[0];
	l.элт.вставьПодбор(ii, pick(элт));
	
	cursor.очисть();
	for(int i = 0; i < count; i++)
		cursor.добавь(i + ii);

	ReloadItems();
}

void LayDes::слей()
{
	currentlayout = Null;
}

LayoutData& LayDes::CurrentLayout()
{
	return layout[currentlayout];
}

void LayDes::LayoutCursor()
{
	слей();
	draghandle = -1;
	currentlayout = list.дайКлюч();
	cursor.очисть();
	тип.откл();
	variable.откл();
	property.очисть();
	if(пусто_ли(currentlayout))
		return;
	LoadItems();
	SyncItems();
	SetSb();
	if(!search.естьФокус())
		устФокус();
}

void LayDes::PrevLayout()
{
	list.Ключ(K_UP, 0);
}

void LayDes::NextLayout()
{
	list.Ключ(K_DOWN, 0);
}

void LayDes::SyncLayoutList()
{
	int sc = list.дайПромотку();
	int c = list.дайКлюч();
	list.очисть();
	Ткст s = взаг((Ткст)~search);
	for(int i = 0; i < layout.дайСчёт(); i++)
		if(взаг(layout[i].имя).найди(s) >= 0)
			list.добавь(i, layout[i].имя);
	list.ScrollTo(sc);
	if(!пусто_ли(c))
		list.FindSetCursor(c);
	LayoutCursor();
}

void LayDes::ищи()
{
	SyncLayoutList();
	if(!list.курсор_ли())
		list.идиВНач();
}

void LayDes::GoTo(int ключ)
{
	if(list.FindSetCursor(ключ))
		return;
	search <<= Null;
	SyncLayoutList();
	list.FindSetCursor(ключ);
}

void LayDes::AddLayout(bool insert)
{
	Ткст имя;
	for(;;) {
		if(!редактируйТекст(имя, "добавь new layout", "Выкладка", CharFilterCid))
			return;
		СиПарсер p(имя);
		if(p.ид_ли())
			break;
		Exclamation("Invalid имя!");
	}
	int q = list.дайКлюч();
	if(!insert || пусто_ли(q) || !(q >= 0 && q < layout.дайСчёт()))
		q = layout.дайСчёт();
	layout.вставь(q).имя = имя;
	SyncLayoutList();
	GoTo(q);
	LayoutCursor();
}

void LayDes::DuplicateLayout()
{
	if(пусто_ли(currentlayout))
		return;
	LayoutData& c = CurrentLayout();
	Ткст имя = c.имя;
	for(;;) {
		if(!редактируйТекст(имя, "Duplicate layout", "Выкладка", CharFilterCid))
			return;
		СиПарсер p(имя);
		if(p.ид_ли())
			break;
		Exclamation("Invalid имя!");
	}
	Ткст data = c.сохрани(0, "\r\n");
	СиПарсер p(data);
	int next = currentlayout + 1;
	LayoutData& d = layout.вставь(next);
	d.читай(p);
	d.имя = имя;
	SyncLayoutList();
	GoTo(next);
	LayoutCursor();
}

void LayDes::RenameLayout()
{
	if(пусто_ли(currentlayout))
		return;
	Ткст имя = layout[currentlayout].имя;
	if(!редактируйТекст(имя, "Rename layout", "Выкладка", CharFilterCid))
		return;
	int q = list.дайКлюч();
	layout[currentlayout].имя = имя;
	SyncLayoutList();
	GoTo(q);
	LayoutCursor();
}

void LayDes::RemoveLayout()
{
	if(пусто_ли(currentlayout) || !PromptYesNo("удали [* " + DeQtf(layout[currentlayout].имя) + "] ?"))
		return;
	int q = list.дайКлюч();
	layout.удали(currentlayout);
	SyncLayoutList();
	if(!пусто_ли(q)) {
		GoTo(q + 1);
		if(!list.курсор_ли())
			list.идиВКон();
	}
	LayoutCursor();
}

void LayDes::MoveLayoutUp()
{
	if(!пусто_ли(search)) {
		search <<= Null;
		SyncLayoutList();
	}
	int q = list.дайКлюч();
	if(q > 0) {
		layout.разверни(q, q - 1);
		SyncLayoutList();
		GoTo(q - 1);
	}
}

void LayDes::MoveLayoutDown()
{
	if(!пусто_ли(search)) {
		search <<= Null;
		SyncLayoutList();
	}
	int q = list.дайКлюч();
	if(q >= 0 && q < layout.дайСчёт() - 1) {
		layout.разверни(q, q + 1);
		SyncLayoutList();
		GoTo(q + 1);
	}
}

void LayDes::DnDInsertLayout(int line, PasteClip& d)
{
	if(GetInternalPtr<КтрлМассив>(d, "layout") == &list && list.курсор_ли() &&
	   line >= 0 && line <= layout.дайСчёт() && d.прими()) {
		if(!пусто_ли(search)) {
			search <<= Null;
			SyncLayoutList();
		}
		int c = list.дайКлюч();
		layout.перемести(c, line);
		if(c <= line)
			line--;
		SyncLayoutList();
		GoTo(line);
	}
}

void LayDes::DragLayout()
{
	list.DoDragAndDrop(InternalClip(list, "layout"), list.дайСэиплТяга(), DND_MOVE);
}

void LayDes::LayoutMenu(Бар& bar)
{
	bool iscursor = list.курсор_ли();
	bar.добавь("добавь new layout..", THISBACK1(AddLayout, false));
	bar.добавь("вставь new layout..", THISBACK1(AddLayout, true));
	bar.добавь(iscursor, "Duplicate layout..", THISBACK(DuplicateLayout));
	bar.добавь(iscursor, "Rename layout..", THISBACK(RenameLayout));
	bar.добавь(iscursor, "удали layout..", THISBACK(RemoveLayout));
	bar.Separator();
	int q = list.дайКлюч();
	bar.добавь(iscursor && q > 0,
	        AK_MOVELAYOUTUP, LayImg::MoveUp(), THISBACK(MoveLayoutUp));
	bar.добавь(iscursor && q < layout.дайСчёт() - 1,
	        AK_MOVELAYOUTDOWN, LayImg::MoveDown(), THISBACK(MoveLayoutDown));
}

LayoutItem& LayDes::CurrentItem()
{
	return CurrentLayout().элт[cursor.верх()];
}

void LayDes::PropertyChanged()
{
	if(элт.курсор_ли())
	{
		CurrentItem().инивалидируй();
		int c = элт.дайКурсор();
		LoadItem(c);
		SyncItem(c, 2);
	}
	освежи();
	SetBar();
}

void LayDes::FrameFocus()
{
	if(property.HasFocusDeep()) {
		SaveState();
		SetStatus();
	}
}

void LayDes::ItemClick()
{
	if(пусто_ли(currentlayout))
		return;
	SaveState();
	if(дайШифт()) {
		if(cursor.дайСчёт()) {
			int i = minmax(элт.дайКурсор(), 0, cursor.верх());
			int m = max(элт.дайКурсор(), cursor.верх());
			cursor.очисть();
			while(i <= m)
				cursor.добавь(i++);
			SyncItems();
		}
	}
	else if(элт.курсор_ли()) {
		if(!дайКтрл())
			cursor.очисть();
		int c = элт.дайКурсор();
		int q = найдиИндекс(cursor, c);
		if(q >= 0)
			cursor.удали(q);
		else
			cursor.добавь(c);
	}
	устФокус();
	SyncItems();
}

void LayDes::SyncUsc()
{
	тип.очистьСписок();
	for(int i = 0; i < LayoutTypes().дайСчёт(); i++)
		if(LayoutTypes()[i].kind != LAYOUT_SUBCTRL)
			тип.добавьСписок(LayoutTypes().дайКлюч(i));
	if(!пусто_ли(currentlayout)) {
		LayoutData& d = CurrentLayout();
		for(int i = 0; i < d.элт.дайСчёт(); i++)
			d.элт[i].инивалидируй();
	}
	освежи();
}

void LayDes::TypeEdit()
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	LayoutData& l = CurrentLayout();
	for(int i = 0; i < cursor.дайСчёт(); i++) {
		LayoutItem& m = l.элт[cursor[i]];
		m.устНабсим(charset);
		Ткст s = m.SaveProperties();
		m.создай(~тип);
		try {
			СиПарсер p(s);
			m.ReadProperties(p, false);
		}
		catch(СиПарсер::Ошибка&) {}
		элт.уст(cursor[i], 0, m.тип);
	}
	SyncProperties(false);
	SetStatus();
}

void LayDes::VariableEdit()
{
	if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
		return;
	LayoutData& l = CurrentLayout();
	LayoutItem& m = l.элт[cursor.верх()];
	m.variable = ~variable;
	if(пусто_ли(m.variable))
		элт.уст(cursor.верх(), 1, GetLabel(m));
	else
		элт.уст(cursor.верх(), 1, m.variable);
	SyncItem(cursor.верх(), 2);
}

static int RoundStep(int org, int d, int g)
{
	return d ? itimesfloor(org + d * g + (d > 0 ? 0 : g - 1), g) - org : 0;
}

bool LayDes::DoHotKey(dword ключ)
{
	if(ключ == K_CTRL_F) {
		search.устФокус();
		return true;
	}
	return false;
}

bool LayDes::DoKey(dword ключ, int count)
{
	SaveState();
	Точка move(0, 0);
	if(!пусто_ли(currentlayout) && !cursor.пустой()) {
		switch(ключ & ~K_CTRL) {
		case K_SHIFT_LEFT:   move.x = -1; break;
		case K_SHIFT_RIGHT:  move.x = +1; break;
		case K_SHIFT_UP:     move.y = -1; break;
		case K_SHIFT_DOWN:   move.y = +1; break;
		}
		if(move.x | move.y) {
			Размер grid(1, 1);
			if(usegrid) {
				grid.cx = minmax<int>(~setting.gridx, 1, 32);
				grid.cy = minmax<int>(~setting.gridy, 1, 32);
			}
			LayoutData& l = CurrentLayout();
			Прям master = CtrlRect(l.элт[cursor.верх()].pos, l.size);
			Размер shift;
			shift.cx = RoundStep(ключ & K_CTRL ? master.устШирину()  : master.left, move.x, grid.cx);
			shift.cy = RoundStep(ключ & K_CTRL ? master.устВысоту() : master.top,  move.y, grid.cy);
			for(int i = 0; i < cursor.дайСчёт(); i++) {
				LayoutItem& элт = l.элт[cursor[i]];
				Прям rc = CtrlRect(элт.pos, l.size);
				rc.right  += shift.cx;
				rc.bottom += shift.cy;
				if(!(ключ & K_CTRL)) {
					rc.left += shift.cx;
					rc.top  += shift.cy;
				}
				элт.pos = MakeLogPos(элт.pos, rc, l.size);
			}
			SetStatus(false);
			return true;
		}
	}
	switch(ключ) {
	case K_PAGEUP:
		PrevLayout();
		return true;
	case K_PAGEDOWN:
		NextLayout();
		return true;
	case K_UP:
	case K_DOWN:
		{
			dword k = (ключ == K_PAGEUP ? K_UP : ключ == K_PAGEDOWN ? K_DOWN : ключ);
			Ук<Ктрл> focus = GetFocusCtrl();
			if(!элт.курсор_ли() && элт.дайСчёт() > 0)
				элт.устКурсор(k == K_DOWN ? 0 : элт.дайСчёт() - 1);
			else
				элт.Ключ(k, count);
			ItemClick();
			if(!!focus)
				focus->SetWantFocus();
		}
		return true;
	default:
		if(ключ >= ' ' && ключ < 65536) {
			if(пусто_ли(currentlayout) || cursor.дайСчёт() == 0)
				return false;
			LayoutItem& m = CurrentItem();
			for(int i = 0; i < m.property.дайСчёт(); i++)
				if(m.property[i].имя == "устНадпись")
					return m.property[i].PlaceFocus(ключ, count);
		}
		break;
	}
	return БарМеню::скан(THISBACK(LayoutMenu), ключ);
}
