#include "IconDes.h"

namespace РНЦП {

void IconDes::Interpolate()
{
	if(!IsCurrent())
		return;
	FinishPaste();
	SaveUndo();
	Слот& c = Current();
	c.base_image = c.image;
	РНЦП::InterpolateImage(c.image, c.image.дайРазм());
	MaskSelection();
}

bool IconDes::BeginTransform()
{
	SaveUndo();
	освежи();
	SyncShow();
	if(!IsPasting()) {
		if(SelectionRect() == GetImageSize())
			return false;
		Move();
	}
	return true;
}

void IconDes::KeyMove(int dx, int dy)
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	if(BeginTransform()) {
		c.pastepos.x += dx;
		c.pastepos.y += dy;
		MakePaste();
	}
	else {
		Рисунок h = c.image;
		c.image = CreateImage(h.дайРазм(), Null);
		РНЦП::копируй(c.image, Точка(dx, dy), h, h.дайРазм());
	}
	синх();
}

void IconDes::MirrorX()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	if(BeginTransform()) {
		MirrorHorz(c.paste_image, c.paste_image.дайРазм());
		MakePaste();
	}
	else
		MirrorHorz(c.image, c.image.дайРазм());
	SyncShow();
}

void IconDes::SymmX()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	if(BeginTransform()) {
		if(c.paste_image.дайДлину() > 1024 * 1024)
			return;
		Рисунок m = c.paste_image;
		Размер sz = m.дайРазм();
		MirrorHorz(m, m.дайРазм());
		Рисунок h = CreateImage(Размер(2 * sz.cx, sz.cy), Null);
		РНЦП::копируй(h, Точка(0, 0), c.paste_image, sz);
		РНЦП::копируй(h, Точка(sz.cx, 0), m, sz);
		c.paste_image = h;
		MakePaste();
	}
	else {
		Размер sz = c.image.дайРазм();
		if(sz.cx < 2)
			return;
		РНЦП::копируй(c.image, Точка(sz.cx - sz.cx / 2, 0), c.image, Размер(sz.cx / 2, sz.cy));
		MirrorHorz(c.image, RectC(sz.cx - sz.cx / 2, 0, sz.cx / 2, sz.cy));
	}
	SyncShow();
}

void IconDes::MirrorY()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	if(BeginTransform()) {
		MirrorVert(c.paste_image, c.paste_image.дайРазм());
		MakePaste();
	}
	else
		MirrorVert(c.image, c.image.дайРазм());
	SyncShow();
}

void IconDes::SymmY()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	if(BeginTransform()) {
		if(c.paste_image.дайДлину() > 1024 * 1024)
			return;
		Рисунок m = c.paste_image;
		Размер sz = m.дайРазм();
		MirrorVert(m, m.дайРазм());
		Рисунок h = CreateImage(Размер(sz.cx, 2 * sz.cy), Null);
		РНЦП::копируй(h, Точка(0, 0), c.paste_image, sz);
		РНЦП::копируй(h, Точка(0, sz.cy), m, sz);
		c.paste_image = h;
		MakePaste();
	}
	else {
		Размер sz = c.image.дайРазм();
		if(sz.cy < 2)
			return;
		РНЦП::копируй(c.image, Точка(0, sz.cy - sz.cy / 2), c.image, Размер(sz.cx, sz.cy / 2));
		MirrorVert(c.image, RectC(0, sz.cy - sz.cy / 2, sz.cx, sz.cy / 2));
	}
	SyncShow();
}

void IconDes::Rotate()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	if(BeginTransform()) {
		c.paste_image = RotateClockwise(c.paste_image);
		MakePaste();
	}
	else {
		c.image = RotateClockwise(c.image);
		SetSelect(255);
	}
	SyncShow();
}

void IconDes::SmoothRescale()
{
	if(!IsCurrent())
		return;
	WithRescaleLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Rescale");
	dlg.cx <<= dlg.cy <<= dlg.method <<= dlg.Breaker();
	Слот& c = Current();
	BeginTransform();
	Рисунок bk = IsPasting() ? c.paste_image : c.image;
	dlg.cx <<= bk.дайШирину();
	dlg.cy <<= bk.дайВысоту();
	dlg.keep <<= true;
	dlg.method.добавь(Null, "Rylek");
	dlg.method.добавь(FILTER_NEAREST, "Nearest");
	dlg.method.добавь(FILTER_BILINEAR, "Bilinear");
	dlg.method.добавь(FILTER_BSPLINE, "Bspline");
	dlg.method.добавь(FILTER_COSTELLO, "Costello");
	dlg.method.добавь(FILTER_BICUBIC_MITCHELL, "Bicubic Mitchell");
	dlg.method.добавь(FILTER_BICUBIC_CATMULLROM, "Bicubic Catmull Rom");
	dlg.method.добавь(FILTER_LANCZOS2, "Lanczos 2");
	dlg.method.добавь(FILTER_LANCZOS3, "Lanczos 3");
	dlg.method.добавь(FILTER_LANCZOS4, "Lanczos 4");
//	dlg.method.добавь(FILTER_LANCZOS5, "Lanczos 5");
	for(;;) {
		Размер sz(minmax((int)~dlg.cx, 1, 9999), minmax((int)~dlg.cy, 1, 9999));
		Рисунок m = RescaleFilter(bk, sz, ~dlg.method);
		SetRes(m, bk.GetResolution());
		if(IsPasting()) {
			c.paste_image = m;
			MakePaste();
		}
		else {
			c.image = m;
			освежи();
		}
		SyncImage();
		SyncShow();
		switch(dlg.пуск()) {
		case IDCANCEL:
			if(IsPasting()) {
				c.paste_image = bk;
				MakePaste();
			}
			else {
				c.image = bk;
				освежи();
			}
			SyncImage();
			SyncShow();
			return;
		case IDOK:
			SyncList();
			return;
		}
		if(dlg.keep) {
			if(dlg.cx.естьФокус() && bk.дайШирину() > 0)
				dlg.cy <<= (int)~dlg.cx * bk.дайВысоту() / bk.дайШирину();
			if(dlg.cy.естьФокус() && bk.дайВысоту() > 0)
				dlg.cx <<= (int)~dlg.cy * bk.дайШирину() / bk.дайВысоту();
		}
	}
}

Рисунок IconDes::ImageStart()
{
	if(!IsCurrent())
		return CreateImage(Размер(1, 1), чёрный);
	SaveUndo();
	освежи();
	SyncShow();
	Слот& c = Current();
	if(!IsPasting())
		c.base_image = c.image;
	return IsPasting() ? c.paste_image : c.image;
}

void IconDes::ImageSet(const Рисунок& m)
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	if(IsPasting()) {
		c.paste_image = m;
		MakePaste();
	}
	else {
		c.image = m;
		MaskSelection();
	}
	освежи();
	SyncShow();
}

void IconDes::BlurSharpen()
{
	WithSharpenLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Blur/Sharpen");
	PlaceDlg(dlg);
	dlg.level <<= 0;
	dlg.level <<= dlg.Breaker();
	dlg.passes <<= 1;
	dlg.passes <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	for(;;) {
		Рисунок m = bk;
		for(int q = 0; q < (int)~dlg.passes; q++)
			m = Sharpen(m, -int(256 * (double)~dlg.level));
		ImageSet(m);
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

Рисунок Colorize2(const Рисунок& img, Цвет color, int alpha, int gray)
{
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	ImageBuffer w(img.дайРазм());
	Unmultiply(w);
	КЗСА *t = w;
	byte r0 = color.дайК();
	byte g0 = color.дайЗ();
	byte b0 = color.дайС();
	alpha = alpha + (alpha >> 7);
	if(gray == 0)
		gray = 1;
	while(s < e) {
		int ga = Grayscale(*s);
		if(gray >= 255) {
			ga = ga + (ga >> 7);
			t->r = (alpha * (((ga * r0) >> 8) - s->r) >> 8) + s->r;
			t->g = (alpha * (((ga * g0) >> 8) - s->g) >> 8) + s->g;
			t->b = (alpha * (((ga * b0) >> 8) - s->b) >> 8) + s->b;
		}
		else {
			int r, g, b;
			if(ga <= gray) {
				r = ga * r0 / gray;
				g = ga * g0 / gray;
				b = ga * b0 / gray;
			}
			else {
				int div = 255 - gray;
				int ao = ga - gray;
				int ac = div - ao;
				r = (ao * s->r + ac * r0) / div;
				g = (ao * s->g + ac * g0) / div;
				b = (ao * s->b + ac * b0) / div;
			}
			t->r = (alpha * (r - s->r) >> 8) + s->r;
			t->g = (alpha * (g - s->g) >> 8) + s->g;
			t->b = (alpha * (b - s->b) >> 8) + s->b;
		}
		t->a = s->a;
		t++;
		s++;
	}
	Premultiply(w);
	w.SetHotSpots(img);
	return w;
}

void IconDes::Colorize()
{
	WithColorize2Layout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Colorize");
	PlaceDlg(dlg);
	dlg.level.минмакс(0, 1);
	dlg.level <<= 1;
	dlg.level <<= dlg.Breaker();
	dlg.gray.минмакс(0, 1);
	dlg.gray <<= 1;
	dlg.gray <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	for(;;) {
		ImageSet(Colorize2(bk, CurrentColor(),
		                  (int)(minmax((double)~dlg.level, 0.0, 1.0) * 255),
		                  (int)(minmax((double)~dlg.gray, 0.0, 1.0) * 255)));
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::FreeRotate()
{
	WithFreeRotateLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Rotate");
	PlaceDlg(dlg);
	dlg.angle <<= 0;
	dlg.angle <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	Размер tsz = bk.дайРазм();
	Рисунок src = Magnify(bk, 3, 3);
	for(;;) {
		Рисунок h = DownSample3x(РНЦП::Rotate(src, (int)~dlg.angle * 10));
		Размер sz = h.дайРазм();
		ImageSet(Crop(h, (sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, tsz.cx, tsz.cy));
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Chroma()
{
	WithColorizeLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Chroma");
	PlaceDlg(dlg);
	dlg.level.макс(10);
	dlg.level <<= 1;
	dlg.level <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	for(;;) {
		ImageSet(РНЦП::Grayscale(bk, 256 - (int)(minmax((double)~dlg.level, 0.0, 4.0) * 255)));
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Contrast()
{
	WithColorizeLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Contrast");
	PlaceDlg(dlg);
	dlg.level.макс(10);
	dlg.level <<= 1;
	dlg.level <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	for(;;) {
		ImageSet(РНЦП::Contrast(bk, (int)(minmax((double)~dlg.level, 0.0, 4.0) * 255)));
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Alpha()
{
	WithColorizeLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Alpha");
	PlaceDlg(dlg);
	dlg.level.макс(4);
	dlg.level <<= 1;
	dlg.level <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	for(;;) {
		int a = (int)(minmax((double)~dlg.level, 0.0, 4.0) * 255);
		ImageBuffer ib(bk.дайРазм());
		КЗСА *t = ib;
		const КЗСА *s = bk;
		const КЗСА *e = bk + bk.дайДлину();
		while(s < e) {
			*t = *s;
			t->a = Saturate255((s->a * a) >> 8);
			s++;
			t++;
		}
		ImageSet(ib);
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Colors()
{
	WithImgColorLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Alpha");
	PlaceDlg(dlg);
	dlg.r_mul <<= dlg.g_mul <<= dlg.b_mul <<= dlg.a_mul <<= 1;
	dlg.r_add <<= dlg.g_add <<= dlg.b_add <<= dlg.a_add <<= 0;
	dlg.all = true;
	dlg.r_mul <<= dlg.g_mul <<= dlg.b_mul <<= dlg.a_mul <<=
	dlg.r_add <<= dlg.g_add <<= dlg.b_add <<= dlg.a_add <<=
	dlg.all <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	for(;;) {
		bool all = dlg.all;
		dlg.g_mul.вкл(!all);
		dlg.g_add.вкл(!all);
		dlg.b_mul.вкл(!all);
		dlg.b_add.вкл(!all);
		if(all) {
			dlg.g_mul <<= dlg.b_mul <<= ~dlg.r_mul;
			dlg.g_add <<= dlg.b_add <<= ~dlg.r_add;
		}
		ImageBuffer ib(bk.дайРазм());
		КЗСА *t = ib;
		const КЗСА *s = bk;
		const КЗСА *e = bk + bk.дайДлину();
		int r_mul = int(256 * (double)~dlg.r_mul);
		int r_add = int(256 * (double)~dlg.r_add);
		int g_mul = int(256 * (double)~dlg.g_mul);
		int g_add = int(256 * (double)~dlg.g_add);
		int b_mul = int(256 * (double)~dlg.b_mul);
		int b_add = int(256 * (double)~dlg.b_add);
		int a_mul = int(256 * (double)~dlg.a_mul);
		int a_add = int(256 * (double)~dlg.a_add);
		while(s < e) {
			Unmultiply(t, t, 1);
			t->r = Saturate255(((r_mul * s->r) >> 8) + r_add);
			t->g = Saturate255(((g_mul * s->g) >> 8) + g_add);
			t->b = Saturate255(((b_mul * s->b) >> 8) + b_add);
			t->a = Saturate255(((a_mul * s->a) >> 8) + a_add);
			Premultiply(t, t, 1);
			s++;
			t++;
		}
		ImageSet(ib);
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

}
