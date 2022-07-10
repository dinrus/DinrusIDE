#include "Draw.h"

namespace РНЦП {

void SDraw::PutImage(Точка p, const Рисунок& img, const Прям& src, Цвет color)
{
	PutImage(p, (paintonly ? CachedSetColorKeepAlphaPaintOnly : CachedSetColorKeepAlpha)
	               (img, color), src);
}

void SDraw::SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color)
{
	Прям sr(Точка(x, y) + cloff.верх().offset, (src & img.дайРазм()).дайРазм());
	const Вектор<Прям>& clip = cloff.верх().clip;
	for(int i = 0; i < clip.дайСчёт(); i++) {
		Прям cr = clip[i] & sr;
		if(!cr.пустой()) {
			Точка p = cr.верхЛево();
			Прям r(cr.верхЛево() - sr.верхЛево() + src.верхЛево(), cr.дайРазм());
			if(пусто_ли(color))
				PutImage(p, img, r);
			else
				PutImage(p, img, r, color);
		}
	}
}

void SDraw::SysDrawImageOp(int x, int y, const Рисунок& img, Цвет color)
{
	SysDrawImageOp(x, y, img, img.дайРазм(), color);
}

void SDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	if(пусто_ли(color))
		return;
	Прям r = RectC(x, y, cx, cy);
	r += cloff.верх().offset;
	const Вектор<Прям>& clip = cloff.верх().clip;
	for(int i = 0; i < clip.дайСчёт(); i++) {
		Прям cr = clip[i] & r;
		if(!cr.пустой())
			PutRect(cr, color);
	}
}

void SDraw::PutHorz(int x, int y, int cx)
{
	DrawRect(x, y, cx, 1, docolor);
}

void SDraw::PutVert(int x, int y, int cy)
{
	DrawRect(x, y, 1, cy, docolor);
}

}
