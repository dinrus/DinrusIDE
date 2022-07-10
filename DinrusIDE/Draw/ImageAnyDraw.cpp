#include "Draw.h"

namespace РНЦП {

dword ImageAnyDraw::GetInfo() const
{
	return draw->GetInfo();
}

Размер ImageAnyDraw::GetPageSize() const
{
	return draw->GetPageSize();
}

void ImageAnyDraw::BeginOp()
{
	return draw->BeginOp();
}

void ImageAnyDraw::EndOp()
{
	return draw->EndOp();
}

void ImageAnyDraw::OffsetOp(Точка p)
{
	return draw->OffsetOp(p);
}

bool ImageAnyDraw::ClipOp(const Прям& r)
{
	return draw->ClipOp(r);
}

bool ImageAnyDraw::ClipoffOp(const Прям& r)
{
	return draw->ClipoffOp(r);
}

bool ImageAnyDraw::ExcludeClipOp(const Прям& r)
{
	return draw->ExcludeClipOp(r);
}

bool ImageAnyDraw::IntersectClipOp(const Прям& r)
{
	return draw->IntersectClipOp(r);
}

bool ImageAnyDraw::IsPaintingOp(const Прям& r) const
{
	return draw->IsPaintingOp(r);
}

Прям ImageAnyDraw::GetPaintRect() const
{
	return draw->GetPaintRect();
}

void ImageAnyDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	draw->DrawRectOp(x, y, cx, cy, color);
}

void ImageAnyDraw::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color)
{
	draw->DrawImageOp(x, y, cx, cy, img, src, color);
}

void ImageAnyDraw::DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id)
{
	draw->DrawDataOp(x, y, cx, cy, data, id);
}

void ImageAnyDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	draw->DrawLineOp(x1, y1, x2, y2, width, color);
}

void ImageAnyDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
                                      const int *counts, int count_count, int width,
                                      Цвет color, Цвет doxor)
{
	draw->DrawPolyPolylineOp(vertices, vertex_count, counts, count_count, width, color, doxor);
}

void ImageAnyDraw::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	draw->DrawPolyPolyPolygonOp(vertices, vertex_count, subpolygon_counts, scc,
	                            disjunct_polygon_counts, dpcc, color, width, outline,
	                            pattern, doxor);
}

void ImageAnyDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	draw->DrawArcOp(rc, start, end, width, color);
}

void ImageAnyDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	draw->DrawEllipseOp(r, color, pen, pencolor);
}

void ImageAnyDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink,
                              int n, const int *dx)
{
	draw->DrawTextOp(x, y, angle, text, font, ink, n, dx);
}

void ImageAnyDraw::DrawDrawingOp(const Прям& target, const Чертёж& w)
{
	draw->DrawDrawingOp(target, w);
}

void ImageAnyDraw::DrawPaintingOp(const Прям& target, const Painting& w)
{
	draw->DrawPaintingOp(target, w);
}

Draw *(*sCreateImageDraw)(Размер sz);
Рисунок (*sExtractImageDraw)(Draw *w);

void ImageAnyDrawPainter(Draw *(*f)(Размер sz), Рисунок (*e)(Draw *w))
{
	sCreateImageDraw = f;
	sExtractImageDraw = e;
}

void ImageAnyDrawSystem(Draw *(*f)(Размер sz), Рисунок (*e)(Draw *w))
{
	if(!sCreateImageDraw) {
		sCreateImageDraw = f;
		sExtractImageDraw = e;
	}
}

bool ImageAnyDraw::IsAvailable()
{
	return sCreateImageDraw;
}

void ImageAnyDraw::иниц(Размер sz)
{
	ПРОВЕРЬ(sCreateImageDraw);
	draw = (*sCreateImageDraw)(sz);
	ПРОВЕРЬ(draw);
}

ImageAnyDraw::operator Рисунок() const
{
	return (*sExtractImageDraw)(draw);
}

ImageAnyDraw::ImageAnyDraw(Размер sz)
{
	иниц(sz);
}

ImageAnyDraw::ImageAnyDraw(int cx, int cy)
{
	иниц(Размер(cx, cy));
}

ImageAnyDraw::~ImageAnyDraw()
{
	delete draw;
}

}
