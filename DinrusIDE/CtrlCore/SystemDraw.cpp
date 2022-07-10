#include "CtrlCore.h"

namespace РНЦП {

#define LTIMING(x)

ImageBuffer::ImageBuffer(ImageDraw& iw)
{
	Рисунок m = iw;
	уст(m);
}

void ImageAnyDrawSystem(Draw *(*f)(Размер sz), Рисунок (*e)(Draw *w));

static Draw *sCD(Размер sz)
{
	return new ImageDraw(sz);
}

static Рисунок sED(Draw *w)
{
	ImageDraw *ip = dynamic_cast<ImageDraw *>(w);
	return ip ? (Рисунок)(*ip) : Рисунок();
}

ИНИЦБЛОК {
	ImageAnyDrawSystem(sCD, sED);
}

void устПоверхность(SystemDraw& w, const Прям& dest, const КЗСА *pixels, Размер psz, Точка poff);

void устПоверхность(Draw& w, const Прям& dest, const КЗСА *pixels, Размер srcsz, Точка poff)
{
	SystemDraw *sw = dynamic_cast<SystemDraw *>(&w);
	if(sw && sw->CanSetSurface())
		устПоверхность(*sw, dest, pixels, srcsz, poff);
	else {
		ImageBuffer ib(dest.дайШирину(), dest.дайВысоту());
		for(int i = 0; i < ib.дайВысоту(); i++) {
			int sl = poff.y + i;
			if(i >= 0 && i < srcsz.cy)
				копируй(ib[i], pixels + srcsz.cx * sl + poff.x,
				     minmax(srcsz.cx - poff.x, 0, min(dest.дайШирину(), srcsz.cx)));
		}
		w.DrawImage(dest.left, dest.top, ib);
	}
}

void устПоверхность(Draw& w, int x, int y, int cx, int cy, const КЗСА *pixels)
{
	устПоверхность(w, RectC(x, y, cx, cy), pixels, Размер(cx, cy), Точка(0, 0));
}

SystemDraw& ScreenInfo();

bool BackDraw::IsPaintingOp(const Прям& r) const
{
	Прям rr = r + дайСмещ();
	if(!rr.пересекается(size))
		return false;
	return painting ? painting->IsPainting(rr + painting_offset) : true;
}

BackDraw::BackDraw()
{
	painting = NULL;
	painting_offset = Точка(0, 0);
}

BackDraw::~BackDraw()
{
	разрушь();
}

}
