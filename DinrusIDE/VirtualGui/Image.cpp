#include <CtrlCore/CtrlCore.h>

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

void устПоверхность(SystemDraw& w, int x, int y, int cx, int cy, const КЗСА *pixels)
{
	ЗамкниГип __;
	// Empty as CanSetSurface is false
}

void устПоверхность(SystemDraw& w, const Прям& dest, const КЗСА *pixels, Размер psz, Точка poff)
{
	ЗамкниГип __;
	// Empty as CanSetSurface is false
}

#define IMAGECLASS FBImg
#define IMAGEFILE <VirtualGui/FB.iml>
#include <Draw/iml_source.h>

#define STD_CURSOR(имя) \
Рисунок Рисунок::имя() { static Рисунок img; ONCELOCK { img = FBImg::имя(); } return img; }

STD_CURSOR(Arrow)
STD_CURSOR(жди)
STD_CURSOR(IBeam)
STD_CURSOR(No)
STD_CURSOR(SizeAll)
STD_CURSOR(SizeHorz)
STD_CURSOR(SizeVert)
STD_CURSOR(SizeTopLeft)
STD_CURSOR(SizeTop)
STD_CURSOR(SizeTopRight)
STD_CURSOR(SizeLeft)
STD_CURSOR(SizeRight)
STD_CURSOR(SizeBottomLeft)
STD_CURSOR(SizeBottom)
STD_CURSOR(SizeBottomRight)
STD_CURSOR(Hand)

END_UPP_NAMESPACE

#endif
