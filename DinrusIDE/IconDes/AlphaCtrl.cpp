#include "IconDes.h"

namespace РНЦП {

void AlphaCtrl::Выкладка()
{
	MakeImage();
}

void AlphaCtrl::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Размер isz = IconDesImg::Alpha().дайРазм();
	w.DrawImage(0, isz.cy, img);
	DrawFrame(w, 0, isz.cy, sz.cx, sz.cy - isz.cy, SColorText());
	int h = sz.cy - isz.cy - 2;
	if(!mask) {
		w.DrawRect(DPI(15), isz.cy + 1, 1, h, SColorPaper());
		w.DrawRect(DPI(16), isz.cy + 1, 1, h, SColorText());
		w.DrawRect(DPI(17), isz.cy + 1, 1, h, SColorPaper());
		w.DrawRect(sz.cx - DPI(17), isz.cy + 1, 1, h, SColorPaper());
		w.DrawRect(sz.cx - DPI(16), isz.cy + 1, 1, h, SColorText());
		w.DrawRect(sz.cx - DPI(15), isz.cy + 1, 1, h, SColorPaper());
	}
	int a = mask ? maskvalue : alpha;
	int x = a == 0   ? DPI(8) - isz.cx / 2 :
	        a == 255 ? sz.cx - DPI(8) - isz.cx / 2
	                 : DPI(16) + a * (sz.cx - DPI(32)) / 255 - isz.cx / 2;
	w.DrawImage(x, 0, IconDesImg::Alpha());
}

void AlphaCtrl::DoPoint(Точка p)
{
	Размер sz = дайРазм();
	if(mask)
		maskvalue = p.x < sz.cx / 2 ? 0 : 255;
	else
		if(p.x < DPI(16) || sz.cx < DPI(32))
			alpha = 0;
		else
		if(p.x > sz.cx - DPI(16))
			alpha = 255;
		else
			alpha = 255 * (p.x - DPI(16)) / (sz.cx - DPI(32));
	освежи();
	UpdateActionRefresh();
}

void AlphaCtrl::леваяВнизу(Точка p, dword keyflags)
{
	DoPoint(p);
	SetCapture();
}

void AlphaCtrl::двигМыши(Точка p, dword keyflags)
{
	if(HasCapture())
		DoPoint(p);
}

static void sPaintCol(ImageBuffer& b, int x, int y, int h, КЗСА c)
{
	КЗСА *t = b[y] + x;
	while(h-- > 0) {
		*t = c;
		t += b.дайШирину();
	}
}

void AlphaCtrl::MakeImage()
{
	Размер sz = дайРазм();
	sz.cy -= IconDesImg::Alpha().дайВысоту();
	if(sz.cx <= DPI(32) || sz.cy < 0) {
		img = Null;
		return;
	}
	ImageBuffer b(дайРазм());
	Цвет c = mask ? чёрный() : color;
	for(int i = 1; i < sz.cx; i++) {
		int a = mask ? i < sz.cx / 2 ? 0 : 255
		             : i < DPI(16) ? 0 : i > sz.cx - DPI(16) ? 255 : min(255, 255 * i / (sz.cx - DPI(32)) - 1);
		int h = sz.cy / 2;
		sPaintCol(b, i, 0, h, смешай((i & DPI(8)) ? белый : чёрный, c, a));
		sPaintCol(b, i, h, h, смешай((i & DPI(8)) ? чёрный : белый, c, a));
	}
	img = b;
	освежи();
}

void AlphaCtrl::устЦвет(Цвет c)
{
	color = c;
	MakeImage();
}

void AlphaCtrl::Mask(bool m)
{
	mask = m;
	MakeImage();
	освежи();
}


void AlphaCtrl::уст(int a)
{
	(mask ? maskvalue : alpha) = a;
	обновиОсвежи();
}

int AlphaCtrl::дай() const
{
	return mask ? maskvalue : alpha;
}

AlphaCtrl::AlphaCtrl()
{
	BackPaint();
	Transparent();
	alpha = 255;
	mask = false;
	maskvalue = 255;
}

}
