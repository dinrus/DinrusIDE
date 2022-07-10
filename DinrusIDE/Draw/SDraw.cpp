#include "Draw.h"

namespace РНЦП {

dword SDraw::GetInfo() const
{
	return DRAWTEXTLINES;
}

void SDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	// TODO
}

void SDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	if(!пусто_ли(color)) {
		docolor = color;
		Polygon().Ellipse(r).Fill();
	}
	if(!пусто_ли(pen) && !пусто_ли(pencolor)) {
		docolor = pencolor;
		устШирину(pen);
		Ellipse(r);
	}
}

void SDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	if(пусто_ли(width) || пусто_ли(color))
		return;
	устШирину(width);
	docolor = color;
	Move(Точка(x1, y1));
	Строка(Точка(x2, y2));
}

void SDraw::DrawPolyPolyPolygonOp(const Точка *vertices0, int /*vertex_count*/,
                                  const int *subpolygon_counts0,
                                  int /*scc*/, const int *disjunct_polygon_counts0, int dpcc0,
                                  Цвет color, int width, Цвет outline,
                                  uint64 /*pattern*/, Цвет /*doxor*/) // последний two parameters ignored
{
	for(int pass = 0; pass < 1 + (width > 0); pass++) {
		const Точка *vertices = vertices0;
		const int *subpolygon_counts = subpolygon_counts0;
		const int *disjunct_polygon_counts = disjunct_polygon_counts0;
		int dpcc = dpcc0;
		while(--dpcc >= 0) {
			const Точка *sp = vertices;
			vertices += *disjunct_polygon_counts++;
			if(pass == 0)
				Polygon();
			else {
				docolor = outline;
				устШирину(width);
			}
			while(sp < vertices) {
				const Точка *pp = sp;
				sp += *subpolygon_counts++;
				Move(*pp);
				while(++pp < sp)
					Строка(*pp);
				закрой();
			}
			if(pass == 0 && !пусто_ли(color)) {
				docolor = color;
				Fill();
			}
		}
	}
}

void SDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
                               const int *counts, int count_count, int width, Цвет color,
                               Цвет /*doxor*/) // последний parameter ignored
{
	if(width == 0)
		width = 1;
	if(пусто_ли(color))
		return;
	устШирину(width);
	docolor = color;
	while(--count_count >= 0) {
		const Точка *lp = vertices;
		vertices += *counts++;
		Move(*lp);
		while(++lp < vertices)
			Строка(*lp);
	}
}

}
