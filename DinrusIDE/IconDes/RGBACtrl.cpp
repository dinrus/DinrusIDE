#include "IconDes.h"

namespace РНЦП {

Цвет s_stdcolor[] = {
	чёрный,
	красный,
	зелёный,
	коричневый,
	синий,
	магента,
	цыан,
	серый,
	светлоСерый,
	светлоКрасный,
	светлоЗелёный,
	светлоЖёлтый,
	светлоСиний,
	светлоМагента,
	светлоЦыан,
	жёлтый,
	белоСерый,
	белый,
};

Цвет RGBACtrl::дайЦвет(int i) const
{
	if(i < 18)
		return s_stdcolor[i];
	i -= 18;
	if(i < 18)
		return серыйЦвет(255 * (i + 1) / 20);
	i -= 18;
	int q = i % 18;
	i /= 18;
	Цвет c = Цвет(255 * (q < 9 ? q + 1 : 18 - q) / 9,
	                255 * (i < 6 ? i + 1 : 12 - i) / 6,
	                q < 9 ? i < 6 ? 0 : 200 : i < 6 ? 150 : 255);
	return c == белый() || c == жёлтый() ? Цвет(c.дайК(), c.дайЗ() - 1, c.дайС()) : c;
}

void RGBACtrl::Выкладка()
{
	DoLayout(дайРазм(), true);
}

int RGBACtrl::DoLayout(Размер sz, bool set)
{
	int ah = IconDesImg::Alpha().дайВысоту() + DPI(20);
	int th = text.GetStdHeight(text.дайШрифт());
	int eh = DPI(4 - 20) - ah - th - DPI(4);
	int rh = min(sz.cx - (sz.cx >> 3), sz.cy - eh);
	if(set)
		ramp.BottomPos(0, rh).HSizePos();
	int y = sz.cy - rh - DPI(4) - ah;
	if(set)
		alpha.TopPos(y, ah).HSizePos(1, 0);
	y -= DPI(4) + th;
	if(set)
		text.TopPos(y, th).HSizePos(1, 0);
	Размер cbx;
	cbx.cx = (sz.cx - 3) / 18;
	cbx.cy = minmax(cbx.cx, 4, Zy(16));
	int ch = 14 * cbx.cy;
	y -= ch + DPI(4);
	if(set) {
		cbox = cbx;
		cs.x = (sz.cx - 18 * cbox.cx) / 2 + 1;
		cs.y = y;
	}
	if(subctrl) {
		if(set)
			subctrl->TopPos(0, y - DPI(4)).HSizePos(1, 0);
		y -= DPI(4);
	}
	return y;
}

int RGBACtrl::дайВысоту(int cx)
{
	return 100000 - DoLayout(Размер(cx, 100000), false);
}

void RGBACtrl::рисуй(Draw& w)
{
	w.DrawRect(дайРазм(), SColorFace);
	if(alpha.IsMask())
		return;
	for(int x = 0; x <= 18; x++)
		w.DrawRect(x * cbox.cx + cs.x, cs.y, 1, cbox.cy * 14, SColorText());
	int i = 0;
	int my = cs.y + 1;
	w.DrawRect(cs.x, cs.y + 14 * cbox.cy, cbox.cx * 18 + 1, 1, SColorText());
	Точка pp = Null;
	for(int y = 0; y < 14; y++) {
		w.DrawRect(cs.x, my - 1, cbox.cx * 18 + 1, 1, SColorText());
		int mx = cs.x + 1;
		for(int x = 0; x < 18; x++) {
			Цвет c = дайЦвет(i++);
			w.DrawRect(mx, my, cbox.cx - 1, cbox.cy - 1, c);
			if(c == color)
				pp = Точка(mx, my);
			mx += cbox.cx;
		}
		my += cbox.cy;
	}
	if(!пусто_ли(pp)) {
		Размер isz = CtrlImg::wheel_cursor().дайРазм();
		pp = pp + (cbox - isz) / 2;
		w.DrawImage(pp.x, pp.y, CtrlImg::wheel_cursor(),
		            Grayscale(color) < 120 ? белый() : чёрный());
	}
}

void RGBACtrl::леваяВнизу(Точка p, dword keyflags)
{
	int x = (p.x - cs.x) / cbox.cx;
	int y = (p.y - cs.y) / cbox.cy;
	if(x >= 0 && x < 18 && y >= 0 && y < 18) {
		color = дайЦвет(x + 18 * y);
		ramp <<= color;
		alpha.уст(255);
		alpha.устЦвет(color);
		UpdateActionRefresh();
		SyncText();
	}
}

void RGBACtrl::двигМыши(Точка p, dword keyflags)
{
	if(keyflags & K_MOUSELEFT)
		леваяВнизу(p, keyflags);
}

void RGBACtrl::SyncText()
{
	КЗСА x = дай();
	text <<= фмт("КЗСА(%3d, %3d, %3d, %3d)", (int)x.r, (int)x.g, (int)x.b, (int)x.a);
}

void RGBACtrl::Ramp()
{
	color = ~ramp;
	alpha.устЦвет(color);
	UpdateActionRefresh();
	SyncText();
}

void RGBACtrl::Alpha()
{
	SyncText();
	UpdateAction();
}

void RGBACtrl::MaskSet(int a)
{
	alpha.уст(a);
	обновиОсвежи();
	SyncText();
}

void RGBACtrl::уст(КЗСА c)
{
	Unmultiply(&c, &c, 1);
	color = прямойЦвет(c);
	ramp <<= color;
	alpha.устЦвет(color);
	MaskSet(c.a);
}

КЗСА RGBACtrl::дай() const
{
	КЗСА c = color;
	c.a = alpha.дай();
	if(alpha.IsMask())
		c.r = c.g = c.b = c.a;
	Premultiply(&c, &c, 1);
	return c;
}

Цвет RGBACtrl::дайЦвет() const
{
	return color;
}

int RGBACtrl::GetAlpha() const
{
	return alpha.дай();
}

void RGBACtrl::Mask(bool b)
{
	alpha.Mask(b);
	ramp.покажи(!b);
	освежи();
}

void RGBACtrl::SubCtrl(Ктрл *c)
{
	if(subctrl)
		subctrl->удали();
	subctrl = c;
	if(c)
		добавь(*c);
	Выкладка();
}

RGBACtrl::RGBACtrl()
{
	color = чёрный();
	BackPaint();
	добавь(ramp);
	ramp <<= THISBACK(Ramp);
	добавь(alpha);
	alpha <<= THISBACK(Alpha);
	text.устШрифт(CourierZ(11));
	text.NoWantFocus();
	text.устТолькоЧтен();
	text.устФрейм(фреймЧёрный());
	text.добавьФрейм(setcolor);
	setcolor.устШирину(CtrlImg::color_edit().дайРазм().cx + DPI(8));
	setcolor.устРисунок(CtrlImg::color_edit());
	setcolor << [=] {
		Ткст text;
		if(!пусто_ли(color) && color != VoidColor())
			text = цветВГЯР(color);
		редактируйТекст(text, "уст Цвет", "Цвет значение");
		Цвет c = цветИзТекста(text);
		if(пусто_ли(c))
			return;
		уст(c);
		UpdateActionRefresh();
	};
	добавь(text);
	SyncText();
}

}
