#include "RichEdit.h"

namespace РНЦП {

#define IMAGECLASS RichEditImg
#define IMAGEFILE <RichEdit/RichEdit.iml>
#include <Draw/iml_source.h>

void HotPaint(Draw& w, int x, int y, const Рисунок& img)
{
	Точка p = img.GetHotSpot();
	w.DrawImage(x - p.x, y - p.y, img);
}

RichRuler::RichRuler()   { newtabalign = ALIGN_LEFT; }
RichRuler::~RichRuler()  {}

void RichRuler::выложиФрейм(Прям& r)
{
	выложиФреймСверху(r, this, Arial(Zy(10)).дайВысоту() + Zy(8));
}

void RichRuler::добавьРазмФрейма(Размер& sz)
{
	sz.cy += Arial(Zy(10)).дайВысоту() + Zy(8);
}

void RichRuler::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorFace);
	w.DrawRect(0, sz.cy - Zx(1), sz.cx, Zy(1), SColorShadow);
	int cx = zoom * pgcx;
	w.DrawRect(x0 - Zx(1), Zy(3), cx + Zx(3), sz.cy - Zy(6), SColorPaper);
	int i = 0;
	for(;;) {
		int x = fround(++i * grid) * zoom;
		if(x >= cx) break;
		int h = (sz.cy - Zy(6)) / 3;
		if(i % marks == 0)
			w.DrawRect(x0 + x, Zy(2) + h, Zx(1), h + Zy(2), SColorHighlight);
		else
			w.DrawRect(x0 + x, Zy(3) + h, Zx(1), h, SColorHighlight);
	}
	i = 0;
	int xs = 0;
	for(;;)
		if(++i % numbers == 0) {
			int x = fround(i * grid) * zoom;
			if(x >= cx) break;
			Ткст n = фмт("%d", (int)(i * numbermul + 0.5));
			Размер tsz = дайРазмТекста(n, ArialZ(9));
			int px = x0 + x - tsz.cx / 2;
			if(px >= xs && x + tsz.cx - tsz.cx / 2 < cx) {
				w.DrawRect(px, Zy(4), tsz.cx, sz.cy - Zy(8), SColorPaper);
				
				w.DrawText(px, Zy(4) + (sz.cy - Zy(8) - tsz.cy) / 2,
				           n, ArialZ(9), SColorText);
				xs = px + tsz.cx + Zx(4);
			}
		}
	фреймПоле().рисуйФрейм(w, RectC(x0 - Zx(1), Zy(3), cx + Zx(3), sz.cy - Zy(6)));
	for(i = marker.дайСчёт() - 1; i >= 0; --i) {
		const Marker& m = marker[i];
		if(!пусто_ли(m.pos))
			HotPaint(w, x0 + m.pos * zoom, m.top ? Zy(1) : sz.cy - Zy(4), DPI(m.image));
	}
	i = 0;
	if(tabsize)
		for(;;) {
			int xp = ++i * tabsize;
			int x = xp * zoom;
			if(x >= cx) break;
			if(xp > tabpos)
				w.DrawRect(x0 + x, sz.cy - Zy(4), Zx(1), Zy(3), SColorShadow);
		}
	w.DrawImage(Zx(4), Zy(6), newtabalign == ALIGN_RIGHT  ? RichEditImg::RightTab() :
	                          newtabalign == ALIGN_CENTER ? RichEditImg::CenterTab() :
	                                                        RichEditImg::LeftTab());
}

int RichRuler::FindMarker(Точка p)
{
	int x = p.x - x0;
	bool top = p.y < дайРазм().cy / 2;
	for(int i = 0; i < marker.дайСчёт(); i++) {
		const Marker& m = marker[i];
		int hx = m.image.GetHotSpot().x;
		int cx = m.image.дайРазм().cx;
		int p = zoom * m.pos;
		if(m.top == top && x >= p - hx && x < p + cx - hx)
			return i;
	}
	return -1;
}

void RichRuler::леваяВнизу(Точка p, dword)
{
	track = FindMarker(p);
	if(track >= 0) {
		trackdx = marker[track].pos * zoom + x0 - p.x;
		SetCapture();
		WhenBeginTrack();
	}
	else
	if(p.x < Zx(16)) {
		newtabalign++;
		if(newtabalign > ALIGN_CENTER) newtabalign = ALIGN_LEFT;
		освежи();
		return;
	}
	else {
		pos = ((p.x - x0) / zoom + snap / 2) / snap * snap;
		WhenLeftDown();
	}
}

void RichRuler::леваяДКлик(Точка p, dword)
{
	if(p.x < x0 - Zx(3)) {
		newtabalign++;
		if(newtabalign > ALIGN_CENTER) newtabalign = ALIGN_LEFT;
		освежи();
		return;
	}

	WhenLeftDouble();
}

void RichRuler::праваяВнизу(Точка p, dword)
{
	if(p.x < x0 - Zx(3)) {
		newtabalign--;
		if(newtabalign < ALIGN_LEFT) newtabalign = ALIGN_CENTER;
		освежи();
		return;
	}

	track = FindMarker(p);
	if(track < 0)
		pos = ((p.x - x0) / zoom + snap / 2) / snap * snap;
	WhenRightDown();
}

void RichRuler::леваяВверху(Точка p, dword)
{
	track = -1;
	WhenEndTrack();
}

void RichRuler::двигМыши(Точка p, dword flags)
{

	if(HasCapture() && track >= 0) {
		Marker& m = marker[track];
		if((p.y < Zy(-10) || p.y >= дайРазм().cy + Zy(10)) && m.deletable)
			m.pos = Null;
		else {
			int x = ((p.x + trackdx - x0) / zoom);
			if(!(flags & K_ALT))
				x = (x + snap / 2) / snap * snap;
			m.pos = minmax(x, m.minpos, m.maxpos);
		}
		освежи();
		WhenTrack();
	}
}

void RichRuler::SetLayout(int x, int _pgcx, Zoom _zoom,
                          double _grid, int _numbers, double _numbermul, int _marks, int _snap)
{
	if(x0 != x || pgcx != _pgcx || zoom != _zoom || grid != _grid || numbers != _numbers ||
	   numbermul != _numbermul || marks != _marks || snap != _snap) {
		x0 = x;
		pgcx = _pgcx;
		zoom = _zoom;
		grid = _grid;
		numbers = _numbers;
		numbermul = _numbermul;
		marks = _marks;
		snap = _snap;
		освежи();
	}
}

void RichRuler::очисть()
{
	if(marker.дайСчёт()) {
		marker.очисть();
		освежи();
	}
}

void RichRuler::устСчёт(int n)
{
	if(marker.дайСчёт() != n) {
		marker.устСчёт(n);
		освежи();
	}
}

void RichRuler::уст(int i, const Marker& m)
{
	if(i >= marker.дайСчёт() || marker[i] != m) {
		marker.по(i) = m;
		освежи();
	}
}

void RichRuler::SetTabs(int pos, int size)
{
	if(tabpos != pos || tabsize != size) {
		tabpos = pos;
		tabsize = size;
		освежи();
	}
}

}
