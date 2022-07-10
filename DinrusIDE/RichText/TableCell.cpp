#include "RichText.h"

namespace РНЦП {

int LineZoom(Zoom z, int a)
{
	return a ? max(1, z * a) : 0;
}

RichCell::фмт::фмт()
{
	margin.left = margin.right = 25;
	margin.top = margin.bottom = 15;
	border.очисть();
	align = ALIGN_CENTER;
	color = белый;
	bordercolor = чёрный;
	keep = round = false;
	minheight = 0;
}

void  RichCell::ClearText()
{
	text.очисть();
	RichPara h;
	text.конкат(h, RichStyles());
}

void  RichCell::ClearText(const RichPara::фмт& формат, const RichStyles& style)
{
	text.очисть();
	RichPara h;
	h.формат = формат;
	text.конкат(h, style);
}

void  RichCell::очисть()
{
	формат = фмт();
	vspan = hspan = 0;
	ClearText();
}

bool  RichCell::Reduce(RichContext& rc) const
{
	Прям br = rc.page;
	rc.page.top += формат.margin.top + формат.border.top;
	rc.page.bottom -= формат.margin.bottom + формат.border.bottom;
	rc.page.left += формат.margin.left + формат.border.left;
	rc.page.right -= формат.margin.right + формат.border.right;
	if(rc.page.пустой()) {
		rc.page = br;
		return false;
	}
	return true;
}

PageY RichCell::GetTop(RichContext rc) const
{
	if(!Reduce(rc))
		return rc.py;
	rc.py.y += формат.margin.top + формат.border.top;
	return text.GetTop(rc);
}

PageY RichCell::дайВысоту(RichContext rc) const
{
	if(Reduce(rc)) {
		PageY py = rc.py;
		rc.py.y += формат.margin.top + формат.border.top;
		rc.py = text.дайВысоту(rc);
		rc.py.y += формат.margin.bottom + формат.border.bottom;
		py.y += формат.margin.top + формат.border.top +
		        формат.minheight +
		        формат.margin.bottom + формат.border.bottom;
		return py.y <= rc.page.bottom && py > rc.py ? py : rc.py;
	}
	else {
		rc.py.y += формат.margin.top + формат.border.top;
		rc.py.y += 80;
		rc.py.y += формат.margin.bottom + формат.border.bottom;
		if(rc.py.y >= rc.page.bottom) {
			rc.Page();
			rc.py.y += 80;
		}
	}
	return rc.py;
}

void DrawRoundCorner(Draw& w, int x, int y, int rx1, int ry1, int rx2, int ry2, Цвет color)
{
	if(rx1 == rx2 || ry1 == ry2) {
		w.DrawRect(Прям(x + rx1, y + ry1, x, y + ry2).нормализат(), color);
		w.DrawRect(Прям(x + rx1, y + ry1, x + rx2, y).нормализат(), color);
		return;
	}
	int steps = max(rx1, ry1, 1000);
	Вектор<Точка> vertex;
	for(int i = 0; i <= steps; i++) {
		double angle = M_PI / 2 * i / steps;
		vertex.добавь() = Точка(int(rx1 * sin(angle) + x), int(ry1 * cos(angle) + y));
	}
	for(int i = steps; i >= 0; i--) {
		double angle = M_PI / 2 * i / steps;
		vertex.добавь() = Точка(int(rx2 * sin(angle) + x), int(ry2 * cos(angle) + y));
	}

	w.DrawPolygon(vertex, color);
}

void DrawCellBorder(Draw& w, const Прям& rect, const Прям& border, const Прям& margin,
                    Цвет color, bool round)
{
	Прям fb = border + margin;
	
	int h = rect.дайВысоту() - fb.top - fb.bottom;
	int y = rect.top + border.top + margin.top;
	int v = rect.дайШирину() - fb.left - fb.right;
	int x = rect.left + border.left + margin.left;

	if(h > 0 && v > 0 && round) {
		DrawRoundCorner(w, rect.left + fb.left, rect.top + fb.top,
		                -fb.left, -fb.top, -margin.left, -margin.top, color);
		DrawRoundCorner(w, rect.right - fb.right, rect.top + fb.top,
		                fb.right, -fb.top, margin.right, -margin.top, color);
		DrawRoundCorner(w, rect.left + fb.left, rect.bottom - fb.bottom,
		                -fb.left, fb.bottom, -margin.left, margin.bottom, color);
		DrawRoundCorner(w, rect.right - fb.right, rect.bottom - fb.bottom,
		                fb.right, fb.bottom, margin.right, margin.bottom, color);
	}
	else {
		y = rect.top;
		x = rect.left;
		h = rect.дайВысоту();
		v = rect.дайШирину();
	}
	w.DrawRect(rect.left, y, border.left, h, color);
	w.DrawRect(rect.right - border.right, y, border.right, h, color);
	w.DrawRect(x, rect.top, v, border.top, color);
	w.DrawRect(x, rect.bottom - border.bottom, v, border.bottom, color);
}

void RichCell::DrawCell(Draw& w, int l, int r, int y, int yy, const Прям& border,
                        const Прям& margin, const PaintInfo& pi) const
{
	w.DrawRect(l + border.left, y + border.top,
	           r - l - border.left - border.right, yy - y - border.top - border.bottom,
	           pi.ResolvePaper(формат.color));
	DrawCellBorder(w, Прям(l, y, r, yy), border, margin, pi.ResolveInk(формат.bordercolor), формат.round);
}

PageY RichCell::Align(const RichContext& rc, PageY npy) const
{
	PageY y = rc.py;
	y.y += формат.margin.top + формат.border.top;
	if(rc.py.page == npy.page) {
		int dx = npy.y - text.дайВысоту(rc).y
		         - формат.margin.top - формат.border.top
		         - формат.margin.bottom - формат.border.bottom;
		if(формат.align == ALIGN_CENTER)
			y.y += dx / 2;
		else
		if(формат.align == ALIGN_BOTTOM)
			y.y += dx;
	}
	return y;
}

void RichCell::рисуй(PageDraw& pw, RichContext rc, PageY npy,
                     const Прям& xpg, const Прям& nxpg,
                     int y, int ny, const PaintInfo& pi, bool select) const
{
	if(!Reduce(rc))
		return;
	Прям border(LineZoom(pi.zoom, формат.border.left), LineZoom(pi.zoom, формат.border.top),
	            LineZoom(pi.zoom, формат.border.right), LineZoom(pi.zoom, формат.border.bottom));
	Прям margin(LineZoom(pi.zoom, формат.margin.left), LineZoom(pi.zoom, формат.margin.top),
	            LineZoom(pi.zoom, формат.margin.right), LineZoom(pi.zoom, формат.margin.bottom));
	if(rc.py.page == npy.page)
		DrawCell(pw.Page(rc.py.page), xpg.left, xpg.right, y, ny, border, margin, pi);
	else {
		DrawCell(pw.Page(rc.py.page), xpg.left, xpg.right, y, xpg.bottom, border, margin, pi);
		for(int i = rc.py.page + 1; i < npy.page; i++)
			DrawCell(pw.Page(i), nxpg.left, nxpg.right, nxpg.top, nxpg.bottom, border, margin, pi);
		DrawCell(pw.Page(npy.page), nxpg.left, nxpg.right, nxpg.top, ny, border, margin, pi);
	}

	rc.py = Align(rc, npy);
	text.рисуй(pw, rc, pi);

	if(select) {
		int cx = xpg.right - xpg.left;
		if(rc.py.page == npy.page)
			pi.DrawSelection(pw.Page(rc.py.page), xpg.left, y, cx, ny - y);
		else {
			pi.DrawSelection(pw.Page(rc.py.page), xpg.left, y, cx, xpg.bottom - y);
			for(int i = rc.py.page + 1; i < npy.page; i++)
				pi.DrawSelection(pw.Page(i), nxpg.left, nxpg.top, cx, nxpg.устВысоту());
			pi.DrawSelection(pw.Page(npy.page), nxpg.left, nxpg.top, cx, ny - nxpg.top);
		}
	}
}

RichCaret RichCell::дайКаретку(int pos, RichContext rc, PageY pyy) const
{
	if(!Reduce(rc))
		return RichCaret();
	rc.py = Align(rc, pyy);
	return text.дайКаретку(pos, rc);
}

int RichCell::дайПоз(int x, PageY y, RichContext rc, PageY pyy) const
{
	if(!Reduce(rc))
		return 0;
	rc.py = Align(rc, pyy);
	return text.дайПоз(x, y, rc);
}

RichHotPos  RichCell::GetHotPos(int x, PageY y, int tolerance, RichContext rc, PageY pyy) const
{
	if(!Reduce(rc))
		return RichHotPos();
	rc.py = Align(rc, pyy);
	return text.GetHotPos(x, y, tolerance, rc);
}

void RichCell::GatherValPos(Вектор<RichValPos>& f, RichContext rc, PageY pyy, int pos, int тип) const
{
	if(!Reduce(rc))
		return;
	rc.py = Align(rc, pyy);
	text.GatherValPos(f, rc, pos, тип);
}

RichCell::RichCell()
{
	vspan = hspan = 0;
	очисть();
}

RichCell::RichCell(const RichCell& src, int)
{
	формат = src.формат;
	hspan = src.hspan;
	vspan = src.vspan;
	text <<= src.text;
}

}
