#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x)

SystemDraw::~SystemDraw()
{
	FlushText();
}

void SystemDraw::устЦвет(Цвет c)
{
	cairo_set_source_rgb(cr, c.дайК() / 255.0, c.дайЗ() / 255.0, c.дайС() / 255.0);
}

dword SystemDraw::GetInfo() const
{
	return DRAWTEXTLINES;
}

Точка SystemDraw::дайСмещ() const
{
	return offset.дайСчёт() ? offset.верх() : Точка(0, 0);
}

Прям  SystemDraw::GetClip() const
{
	return clip.дайСчёт() ? clip.верх() : Прям(-999999, -999999, 999999, 999999);
}

void SystemDraw::сунь()
{
	cairo_save(cr);
	offset.добавь(дайСмещ());
	clip.добавь(GetClip());
}

void SystemDraw::вынь()
{
	if(offset.дайСчёт())
		offset.сбрось();
	if(clip.дайСчёт())
		clip.сбрось();
	cairo_restore(cr);
}

void SystemDraw::BeginOp()
{
	FlushText();
	сунь();
}

void SystemDraw::EndOp()
{
	FlushText();
	вынь();
}

void SystemDraw::OffsetOp(Точка p)
{
	FlushText();
	сунь();
	offset.верх() += p;
	cairo_translate(cr, p.x, p.y);
}

void SystemDraw::RectPath(const Прям& r)
{
	cairo_rectangle(cr, r.left, r.top, r.дайШирину(), r.дайВысоту());
}

bool SystemDraw::ClipOp(const Прям& r)
{
	FlushText();
	сунь();
	clip.верх() &= r.смещенец(дайСмещ());
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool SystemDraw::ClipoffOp(const Прям& r)
{
	FlushText();
	сунь();
	clip.верх() &= r.смещенец(дайСмещ());
	offset.верх() += r.верхЛево();
	RectPath(r);
	cairo_clip(cr);
	cairo_translate(cr, r.left, r.top);
	return true;
}

bool SystemDraw::ExcludeClipOp(const Прям& r)
{
	FlushText();
	RectPath(Прям(-99999, -99999, 99999, r.top));
	RectPath(Прям(-99999, r.top, r.left, 99999));
	RectPath(Прям(r.right, r.top, 99999, 99999));
	RectPath(Прям(r.left, r.bottom, r.right, 99999));
	cairo_clip(cr);
	return true;
}

bool SystemDraw::IntersectClipOp(const Прям& r)
{
	FlushText();
	RectPath(r);
	cairo_clip(cr);
	return true;
}

bool SystemDraw::IsPaintingOp(const Прям& r) const
{
	Прям cr = r.смещенец(дайСмещ());
	cr.пересек(GetClip());
	if(cr.пустой())
		return false;
	if(invalid.дайСчёт() == 0)
		return true;
	for(const Прям& ir : invalid)
		if(cr.пересекается(ir))
			return true;
	return false;
}

Прям SystemDraw::GetPaintRect() const
{
	return Прям(0, 0, INT_MAX, INT_MAX);
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	if(пусто_ли(color))
		return;
	FlushText();
	cairo_rectangle(cr, x, y, cx, cy);
	if(color == InvertColor()) {
		устЦвет(белый());
		cairo_set_operator(cr, CAIRO_OPERATOR_DIFFERENCE);
		cairo_fill(cr);
		cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
	}
	else {
		устЦвет(color);
		cairo_fill(cr);
	}
	cairo_new_path(cr);
}

static void sDrawLineStroke(cairo_t *cr, int width)
{
	static double dash[] = { 18, 6 };
	static double dot[] = { 3, 3 };
	static double dashdot[] = { 9, 6, 3, 6 };
	static double dashdotdot[] = { 9, 3, 3, 3, 3, 3 };
	if(пусто_ли(width))
		return;
	switch(width) {
	case PEN_NULL:       return;
	case PEN_DASH:       cairo_set_dash(cr, dash, __countof(dash), 0); break;
	case PEN_DOT:        cairo_set_dash(cr, dot, __countof(dot), 0); break;
	case PEN_DASHDOT:    cairo_set_dash(cr, dashdot, __countof(dashdot), 0); break;
	case PEN_DASHDOTDOT: cairo_set_dash(cr, dashdotdot, __countof(dashdotdot), 0); break;
	default:             break;
	}
	cairo_set_line_width(cr, width < 0 ? 1 : width);
	cairo_stroke(cr);
	if(width < 0)
		cairo_set_dash(cr, dot, 0, 0);
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	if(пусто_ли(width) || пусто_ли(color))
		return;
	if(width == 0)
		width = 1;
	FlushText();
	устЦвет(color);
	if(width == PEN_SOLID)
		width = 1;
	if(y1 == y2 && width >= 0)
		DrawRect(x1, y1 - width / 2, x2 - x1, width, color);
	else
	if(x1 == x2 && width >= 0)
		DrawRect(x1 - width / 2, y1, width, y2 - y1, color);
	else {
		int w = width < 0 ? 1 : width;
		double d = w / 2.0;
		if(y1 == y2) {
			cairo_move_to(cr, min(x1, x2) + 0.5, y1 + d);
			cairo_line_to(cr, max(x1, x2) - 0.5, y1 + d);
		}
		else
		if(x1 == x2) {
			cairo_move_to(cr, x1 + d, min(y1, y2) + 0.5);
			cairo_line_to(cr, x1 + d, max(y1, y2) - 0.5);
		}
		else {
			cairo_move_to(cr, x1, y1);
			cairo_line_to(cr, x2, y2);
		}
		sDrawLineStroke(cr, width);
	}
	cairo_new_path(cr);
}

void SystemDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts,
                                    int count_count, int width, Цвет color, Цвет doxor)
{
	if(vertex_count < 2 || пусто_ли(color))
		return;
	FlushText();
	устЦвет(color);
	while(--count_count >= 0) {
		const Точка *pp = vertices;
		vertices += *counts++;
		cairo_move_to(cr, pp->x, pp->y);
		while(++pp < vertices)
			cairo_line_to(cr, pp->x, pp->y);
		sDrawLineStroke(cr, width);
	}
	cairo_new_path(cr);
}

cairo_surface_t *CreateCairoSurface(const Рисунок& img);

void SystemDraw::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
                                  const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts,
                                  int dpcc, Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	FlushText();
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
			cairo_move_to(cr, pp->x, pp->y);
			while(++pp < sp)
				cairo_line_to(cr, pp->x, pp->y);
			cairo_close_path(cr);
		}
		if(!пусто_ли(fill_img)) {
			cairo_surface_t *surface = CreateCairoSurface(fill_img);
			сунь();
			cairo_set_source_surface(cr, surface, 0, 0);
			cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
			cairo_fill_preserve(cr);
			вынь();
			cairo_surface_destroy(surface);
		}
		else
		if(!пусто_ли(color)) {
			устЦвет(color);
			cairo_fill_preserve(cr);
		}
		if(!пусто_ли(outline)) {
			устЦвет(outline);
			sDrawLineStroke(cr, width);
		}
	}
	cairo_new_path(cr);
}

void SystemDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	if(rc.устШирину() <= 0 || rc.устВысоту() <= 0)
		return;
	FlushText();
	РазмерПЗ radius = РазмерПЗ(rc.размер()) / 2.0;
	ТочкаПЗ center = ТочкаПЗ(rc.верхЛево()) + radius;
	double ang1 = Bearing((ТочкаПЗ(start) - center) / radius);
	double ang2 = Bearing((ТочкаПЗ(end) - center) / radius);

	if(ang1 == ang2)
		ang1 -= 0.000001;
	
	cairo_move_to(cr, center.x + radius.cx * cos(ang1), center.y + radius.cy * sin(ang1));
	cairo_save(cr);
	cairo_translate(cr, rc.left + radius.cx, rc.top + radius.cy);
	cairo_scale(cr, radius.cx, radius.cy);
	cairo_arc_negative(cr, 0, 0, 1, ang1, ang2);
	cairo_restore(cr);

	устЦвет(color);
	sDrawLineStroke(cr, width);
	cairo_new_path(cr);
}

void SystemDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	FlushText();
	cairo_save(cr);
	РазмерПЗ h = РазмерПЗ(r.дайРазм()) / 2.0;
	cairo_translate (cr, r.left + h.cx, r.top + h.cy);
	cairo_scale(cr, h.cx, h.cy);
	cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
	cairo_restore(cr);
	if(!пусто_ли(color)) {
		устЦвет(color);
		cairo_fill_preserve(cr);
	}
	if(!пусто_ли(pencolor)) {
		устЦвет(pencolor);
		sDrawLineStroke(cr, pen);
	}
	cairo_new_path(cr);
}

}

#endif
