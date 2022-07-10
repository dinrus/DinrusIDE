#include "Painter.h"

namespace РНЦП {

dword Рисовало::GetInfo() const
{
	return DOTS;
}

void Рисовало::OffsetOp(Точка p)
{
	старт();
	Translate(p.x, p.y);
}

Рисовало& Рисовало::RectPath(int x, int y, int cx, int cy)
{
	Move(x, y).Строка(x + cx, y).Строка(x + cx, y + cy).Строка(x, y + cy).закрой();
	return *this;
}

Рисовало& Рисовало::RectPath(const Прям& r)
{
	RectPath(r.left, r.top, r.дайШирину(), r.дайВысоту());
	return *this;
}

Рисовало& Рисовало::RectPath(double x, double y, double cx, double cy)
{
	Move(x, y).Строка(x + cx, y).Строка(x + cx, y + cy).Строка(x, y + cy).закрой();
	return *this;
}

Рисовало& Рисовало::RectPath(const ПрямПЗ& r)
{
	RectPath(r.left, r.top, r.дайШирину(), r.дайВысоту());
	return *this;
}

bool Рисовало::ClipOp(const Прям& r)
{
	старт();
	RectPath(r);
	Clip();
	return true;
}

bool Рисовало::ClipoffOp(const Прям& r)
{
	старт();
	RectPath(r);
	Clip();
	Translate(r.left, r.top);
	return true;
}

bool Рисовало::ExcludeClipOp(const Прям& r)
{
	RectPath(Прям(-99999, -99999, 99999, r.top));
	RectPath(Прям(-99999, r.top, r.left, 99999));
	RectPath(Прям(r.right, r.top, 99999, 99999));
	RectPath(Прям(r.left, r.bottom, r.right, 99999));
	Clip();
	return true;
}

bool Рисовало::IntersectClipOp(const Прям& r)
{
	RectPath(r);
	Clip();
	return true;
}

bool Рисовало::IsPaintingOp(const Прям& r) const
{
	return true;
}

void Рисовало::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	RectPath(x, y, cx, cy);
	if(color == InvertColor)
		Invert();
	Fill(color);
}

void Рисовало::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& image, const Прям& src, Цвет color)
{
	if(src.дайШирину() * src.дайВысоту() == 0)
		return;
	Рисунок img = пусто_ли(color) ? image : SetColorKeepAlpha(image, color);
	RectPath(x, y, cx, cy);
	double sw = (double)cx / src.дайШирину();
	double sh = (double)cy / src.дайВысоту();
	Fill(img, Xform2D::Scale(sw, sh) * Xform2D::Translation(x - sw * src.left, y - sh * src.top));
}

void Рисовало::DrawLineStroke(int width, Цвет color)
{
	if(пусто_ли(width) || пусто_ли(color))
		return;
	старт();
	LineCap(LINECAP_ROUND);
	switch(width) {
	case PEN_NULL:
		Stroke(0, color);
		стоп();
		return;
	case PEN_SOLID:
		Stroke(1, color);
		break;
	case PEN_DASH:
		Dash("18 6");
		break;
	case PEN_DOT:
		Dash("3 3");
		break;
	case PEN_DASHDOT:
		Dash("9 6 3 6");
		break;
	case PEN_DASHDOTDOT:
		Dash("9 3 3 3 3 3");
		break;
	default:
		Stroke(width == 0 ? 1 : width, color);
		стоп();
		return;
	}
	Stroke(1, color);
	стоп();
}

void Рисовало::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	Move(x1, y1);
	Строка(x2, y2);
	DrawLineStroke(width, color);
}

void Рисовало::DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts,
                                 int count_count, int width, Цвет color, Цвет doxor)
{
	while(--count_count >= 0) {
		const Точка *lp = vertices;
		vertices += *counts++;
		Move(*lp);
		while(++lp < vertices)
			Строка(*lp);
		DrawLineStroke(width, color);
	}
}

void Рисовало::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
                                  const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts,
                                  int dpcc, Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	Рисунок fill_img;
	if(pattern && !пусто_ли(color)) {
		ImageBuffer ibuf(8, 8);
		КЗСА r[2] = { color, обнулиКЗСА() };
		for(КЗСА *out = ibuf, *end = out + 64; out < end; pattern >>= 1)
			*out++ = r[(byte)pattern & 1];
		fill_img = ibuf;
	}
	
	while(--dpcc >= 0) {
		const Точка *sp = vertices;
		vertices += *disjunct_polygon_counts++;
		while(sp < vertices) {
			const Точка *pp = sp;
			sp += *subpolygon_counts++;
			Move(*pp);
			while(++pp < sp)
				Строка(*pp);
			закрой();
		}
		if(!пусто_ли(fill_img))
			Fill(fill_img, Xform2D::Identity(), FILL_HREPEAT | FILL_VREPEAT);
		else if(!пусто_ли(color))
			Fill(color);
		if(!пусто_ли(outline))
			DrawLineStroke(width, outline);
	}
}

void Рисовало::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	if(rc.устШирину() <= 0 || rc.устВысоту() <= 0)
		return;
	РазмерПЗ radius = РазмерПЗ(rc.размер()) / 2.0;
	ТочкаПЗ center = ТочкаПЗ(rc.верхЛево()) + radius;
	double ang1 = Bearing((ТочкаПЗ(start) - center) / radius);
	double ang2 = Bearing((ТочкаПЗ(end) - center) / radius);
	double sweep = ang1 - ang2;
	if(sweep <= 0)
		sweep += 2 * M_PI;
	Move(center.x + radius.cx * cos(ang1), center.y + radius.cy * sin(ang1));
	Arc(center, radius.cx, radius.cy, ang1, -sweep);
	DrawLineStroke(width, color);
}

void Рисовало::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	РазмерПЗ sz = r.дайРазм();
	Ellipse(r.left + sz.cx / 2, r.top + sz.cy / 2, sz.cx / 2, sz.cy / 2);
	Fill(color);
	DrawLineStroke(pen, pencolor);
}

void Рисовало::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx)
{
	старт();
	EvenOdd(true);
	Translate(x, y);
	if(angle)
		Rotate(-angle * M_2PI / 3600);
	if(n < 0)
		n = strlen__(text);
	double *ddx = NULL;
	Буфер<double> h;
	if(dx) {
		h.размести(n);
		ddx = h;
		for(int i = 0; i < n; i++)
			ddx[i] = dx[i];
	}
	устТекст(0, 0, text, font, n, ddx);
	Fill(ink);
	стоп();
}

void Рисовало::DrawPaintingOp(const Прям& target, const Painting& p)
{
	Размер sz = target.дайРазм();
	РазмерПЗ psz = p.дайРазм();
	старт();
	Translate(target.left, target.top);
	Scale(sz.cx / psz.cx, sz.cy / psz.cy);
	рисуй(p);
	стоп();
}

}
