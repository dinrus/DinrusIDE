#include "IconDes.h"

namespace РНЦП {

void IconDraw::PutHorz(int x, int y, int cx)
{
	if(y < 0 || y >= image.дайВысоту())
		return;
	int r = minmax(x + cx, 0, image.дайШирину());
	x = minmax(x, 0, image.дайШирину());
	cx = r - x;
	if(cx <= 0)
		return;
	РНЦП::Fill(image[y] + x, docolor, cx);
}

void IconDraw::PutVert(int x, int y, int cy)
{
	if(x < 0 || x >= image.дайШирину())
		return;
	int b = minmax(y + cy, 0, image.дайВысоту());
	y = minmax(y, 0, image.дайВысоту());
	cy = b - y;
	if(cy <= 0)
		return;
	КЗСА *t = image[y] + x;
	while(cy-- > 0) {
		*t = docolor;
		t += image.дайШирину();
	}
}

void IconDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	docolor = color;
	int b = minmax(y + cy, 0, image.дайВысоту());
	y = minmax(y, 0, image.дайВысоту());
	cy = b - y;
	while(cy-- > 0)
		PutHorz(x, y++, cx);
}

void IconDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	docolor = color;
	устШирину(width);
	Move(Точка(x1, y1));
	Строка(Точка(x2, y2));
}

void IconDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
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

}
