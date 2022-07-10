#include "IconDes.h"

namespace РНЦП {

void IconDes::MaskSelection()
{
	ImageBuffer ib(Current().image);
	КЗСА *t = ~ib;
	const КЗСА *e = t + ib.дайДлину();
	const КЗСА *s = Current().base_image;
	const КЗСА *k = ~Current().selection;
	while(t < e) {
		if(!k->r)
			*t = *s;
		t++;
		s++;
		k++;
	}
	Current().image = ib;
	освежи();
	SyncShow();
}

void IconDes::ApplyDraw(IconDraw& iw, dword flags)
{
	ApplyImage(iw, flags);
}

void IconDes::ApplyImage(Рисунок m, dword flags, bool alpha)
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	Размер isz = GetImageSize();
	КЗСА cc = CurrentColor();
	int empty = doselection ? cc.r : 0;
	ImageBuffer ib(CurrentImage());
	for(int y = 0; y < isz.cy; y++) {
		const КЗСА *s = c.base_image[y];
		КЗСА *t = ib[y];
		const КЗСА *d = m[y];
		const КЗСА *k = doselection ? d : c.selection[y];
		for(int x = 0; x < isz.cx; x++) {
			КЗСА c = *s;
			if(alpha) {
				cc.a = d->r;
				AlphaBlendStraight(&c, &cc, 1);
			}
			else {
				if(d->r == 255) {
					if(flags & K_ALT)
						c.a = cc.a;
					else
					if(flags & K_CTRL) {
						КЗСА h = cc;
						h.a = c.a;
						c = h;
					}
					else
						c = cc;
				}
				if(d->r == 128)
					c.a = c.r = c.g = c.b = empty;
			}
			if(c != *t && (doselection || k->r)) {
				*t = c;
				RefreshPixel(x, y);
			}
			t++;
			s++;
			d++;
			k++;
		}
	}
	CurrentImage() = ib;
}

void IconDes::уст(Точка p, КЗСА rgba, dword flags)
{
	if(!IsCurrent())
		return;
	if(InImage(p)) {
		ImageBuffer ib(CurrentImage());
		КЗСА& t = ib[p.y][p.x];
		if(t != rgba && (doselection || Current().selection[p.y][p.x].r)) {
			if(flags & K_ALT)
				t.a = rgba.a;
			else
			if(flags & K_CTRL) {
				rgba.a = t.a;
				t = rgba;
			}
			else
				t = rgba;
			RefreshPixel(p);
		}
		CurrentImage() = ib;
	}
}

}
