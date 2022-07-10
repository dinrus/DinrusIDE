#include "CtrlCore.h"

namespace РНЦП {

#define  TFILE <CtrlCore/CtrlCore.t>
#include <Core/t.h>

static Рисунок sRenderGlyph(int cx, int x, Шрифт font, int chr, int py, int pcy, Цвет fg, Цвет bg)
{
	ImageDraw iw(cx, pcy);
	iw.DrawRect(0, 0, cx, pcy, bg);
	iw.DrawText(x, -py, ШТкст(chr, 1), font, fg);
	return iw;
}

void SetRenderGlyph(Рисунок (*f)(int cx, int x, Шрифт font, int chr, int py, int pcy, Цвет fg, Цвет bg));

ИНИЦИАЛИЗАТОР(CtrlCore) {
	SetRenderGlyph(sRenderGlyph);
}

}
