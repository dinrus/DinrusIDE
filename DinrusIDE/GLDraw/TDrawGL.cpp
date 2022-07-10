#include "GLDraw.h"

namespace РНЦП {

void DrawGL::иниц(Размер sz, double alpha)
{
	Cloff& c = cloff.добавь();
	c.clip = view_size = sz;
	c.offset = ТочкаПЗ(0, 0);
	dd.уст(sz);
	dd.alpha = alpha;
	glEnable(GL_SCISSOR_TEST);
	scissor = Null;
	SyncScissor();
	prev = Точка(0, 0);
	path_done = false;
}

DrawGL::~DrawGL()
{
	слей();
	glDisable(GL_SCISSOR_TEST);
}

dword DrawGL::GetInfo() const
{
	return DRAWTEXTLINES;
}

void DrawGL::сунь()
{
	auto& s = state.добавь();
	s.dash = clone(dash);
	s.dash_start = clone(dash_start);
	s.alpha = dd.alpha;
}

void DrawGL::BeginOp()
{
	слей();
	Cloff c = cloff.верх();
	cloff.добавь(c);
	сунь();
}

bool DrawGL::ClipOp(const Прям& r)
{
	Cloff c = cloff.верх();
	Cloff& c1 = cloff.добавь();
	c1.clip = c.clip & (r + c.offset);
	c1.offset = c.offset;
	SyncScissor();
	сунь();
	return !c1.clip.пустой();
}

bool DrawGL::ClipoffOp(const Прям& r)
{
	Cloff c = cloff.верх();
	Cloff& c1 = cloff.добавь();
	c1.clip = c.clip & (r + c.offset);
	c1.offset = c.offset + (ТочкаПЗ)r.верхЛево();
	SyncScissor();
	сунь();
	return !c1.clip.пустой();
}

bool DrawGL::IntersectClipOp(const Прям& r)
{
	Cloff& c = cloff.верх();
	c.clip = c.clip & (r + c.offset);
	SyncScissor();
	сунь();
	return !c.clip.пустой();
}

bool DrawGL::ExcludeClipOp(const Прям& r)
{
	// does not work with DrawGL
	return true;
}

bool DrawGL::IsPaintingOp(const Прям& r) const
{
	return true;
}

void DrawGL::OffsetOp(Точка p)
{
	Cloff c = cloff.верх();
	Cloff& c1 = cloff.добавь();
	c1.clip = c.clip;
	c1.offset = c.offset + (ТочкаПЗ)p;
	сунь();
}

void DrawGL::EndOp()
{
	ПРОВЕРЬ(cloff.дайСчёт());
	if(cloff.дайСчёт())
		cloff.сбрось();
	if(state.дайСчёт()) {
		auto& s = state.верх();
		dash = pick(s.dash);
		dash_start = s.dash_start;
		dd.alpha = s.alpha;
		state.сбрось();
	}
	SyncScissor();
}

void DrawGL::SyncScissor()
{
	GL_TIMING("SyncScissor");
	Прям clip = cloff.верх().clip;
	if(clip != scissor) {
		слей();
		Размер sz = clip.дайРазм();
		glScissor(clip.left, view_size.cy - sz.cy - clip.top, sz.cx, sz.cy);
	}
}

ТочкаПЗ DrawGL::Off(int x, int y)
{
	ТочкаПЗ o = cloff.верх().offset;
	return ТочкаПЗ(x + o.x, y + o.y);
}

ПрямПЗ DrawGL::Off(int x, int y, int cx, int cy)
{
	Точка o = cloff.верх().offset;
	return RectfC(x + o.x, y + o.y, cx, cy);
}

ПрямПЗ DrawGL::Off(int x, int y, Размер sz)
{
	return Off(x, y, sz.cx, sz.cy);
}

void DrawGL::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& image, const Прям& src, Цвет color)
{
	слей();
	GLDrawImage(dd, Off(x, y, cx, cy), пусто_ли(color) ? image : CachedSetColorKeepAlpha(image, color),
	            src);
}

void DrawGL::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx)
{
	слей();
	GLDrawText(dd, Off(x, y), angle * M_2PI / 3600, text, font, ink, n, dx);
}

void DrawGL::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	Точка off = cloff.верх().offset;
	int a = Vertex(x + off.x, y + off.y, color, dd.alpha);
	int b = Vertex(x + off.x + cx, y + off.y, color, dd.alpha);
	int c = Vertex(x + off.x + cx, y + off.y + cy, color, dd.alpha);
	int d = Vertex(x + off.x, y + off.y + cy, color, dd.alpha);

	Triangle(a, b, c);
	Triangle(a, c, d);
}

const Вектор<double>& DrawGL::GetDash(int& width)
{
	static Вектор<double> nodash;
	static Вектор<double> dash = { 18, 6 };
	static Вектор<double> dot = { 3, 3 };
	static Вектор<double> dashdot = { 9, 6, 3, 6 };
	static Вектор<double> dashdotdot = { 9, 3, 3, 3, 3, 3 };

	if(width == 0)
		width = 1;
	if(width > 0)
		return nodash;
	if(width == PEN_NULL || пусто_ли(width)) {
		width = 0;
		return nodash;
	}
	int w = width;
	width = 1;
	return *decode(w, PEN_DASH, &dash,
	                  PEN_DOT, &dot,
	                  PEN_DASHDOT, &dashdot,
	                  PEN_DASHDOTDOT, &dashdotdot,
	                  &nodash);
}

void DrawGL::ApplyDash(Вектор<Вектор<ТочкаПЗ>>& polyline, int& width)
{
	const Вектор<double>& dash = GetDash(width);
	GetDash(width);
	if(dash.дайСчёт()) {
		Вектор<Вектор<ТочкаПЗ>> r;
		for(auto& l : polyline)
			DashPolyline(r, l, dash);
		polyline = pick(r);
	}
}

void DrawGL::DoDrawPolylines(Вектор<Вектор<ТочкаПЗ>>& poly, int width, Цвет color, bool close)
{
	ApplyDash(poly, width);
	for(const auto& p : poly)
		Polyline(*this, p, width, color, dd.alpha, close);
}

void DrawGL::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	Вектор<Вектор<ТочкаПЗ>> poly;
	poly.добавь().добавь(Off(x1, y1));
	poly.верх().добавь(Off(x2, y2));
	DoDrawPolylines(poly, width, color);
}

void DrawGL::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	GL_TIMING("DrawGL::DrawArcOp");
	Вектор<Вектор<ТочкаПЗ>> poly;
	GLArc(poly, rc, start, end);
	DoDrawPolylines(poly, width, color);
}

void DrawGL::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	// TODO: Dash, ellipse stroke
	if(!r.пустой())
		РНЦП::Ellipse(*this, ТочкаПЗ(r.центрТочка()) + cloff.верх().offset,
		             РазмерПЗ(r.дайРазм()) / 2, color, pen, pencolor, dd.alpha);
}

void DrawGL::делайПуть(Вектор<Вектор<ТочкаПЗ>>& poly, const Точка *pp, const Точка *end)
{
	poly.добавь().добавь(Off(*pp++));
	while(pp < end)
		poly.верх().добавь(Off(*pp++));
}

void DrawGL::DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts, int count_count, int width, Цвет color, Цвет doxor)
{
	if(vertex_count < 2 || пусто_ли(color))
		return;
	Вектор<Вектор<ТочкаПЗ>> poly;
	while(--count_count >= 0) {
		const Точка *pp = vertices;
		vertices += *counts++;
		делайПуть(poly, pp, vertices);
	}
	
	DoDrawPolylines(poly, width, color);
}

extern int sTesselateCounter;

void DrawGL::DoDrawPolygons(const Вектор<Вектор<ТочкаПЗ>>& path, Цвет color)
{
	const int TESS_LIMIT = 200;
	int n = 0;
	for(const auto& p : path) {
		n += p.дайСчёт();
		if(n > TESS_LIMIT) {
			слей();
			GLVertexData data;
			GLPolygons(data, path);
			GLDrawPolygons(dd, ТочкаПЗ(0, 0), data, РазмерПЗ(1, 1), color);
			return;
		}
	}
	Вектор<ТочкаПЗ> vertex;
	Вектор<Кортеж<int, int, int>> triangle;
	sTesselateCounter++;
	Tesselate(path, vertex, triangle, false);
	int ii0;
	for(int i = 0; i < vertex.дайСчёт(); i++) {
		int q = Vertex(vertex[i], color, dd.alpha);
		if(i == 0) ii0 = q;
	}
	for(const auto& t : triangle)
		Triangle(t.a + ii0, t.b + ii0, t.c + ii0);
}

void DrawGL::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count, const int *subpolygon_counts, int scc,
                                   const int *disjunct_polygon_counts, int dpcc, Цвет color,
                                   int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	Вектор<Вектор<ТочкаПЗ>> poly;
	while(--dpcc >= 0) {
		const Точка *sp = vertices;
		vertices += *disjunct_polygon_counts++;
		while(sp < vertices) {
			const Точка *pp = sp;
			sp += *subpolygon_counts++;
			делайПуть(poly, pp, sp);
		}
	}

	if(poly.дайСчёт() == 0)
		return;

	if(!пусто_ли(color))
		DoDrawPolygons(poly, color);

	if(!пусто_ли(outline))
		DoDrawPolylines(poly, width, outline, true);
}


void DrawGL::слей()
{
	GLTriangles::Draw(dd);
	GLTriangles::очисть();
}

};
