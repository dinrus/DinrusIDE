#include "Painter.h"

namespace РНЦП {

Рисунок БуфРисовало::MakeGradient(КЗСА color1, Вектор<ColorStop>& color_stop, КЗСА color2, int n)
{
	ImageBuffer ib(n, 1);
	КЗСА *t = ib[0];
	int l = 0;
	КЗСА cl = color1;
	for(int i = 0; i <= color_stop.дайСчёт(); i++) {
		int h;
		КЗСА ch;
		if(i < color_stop.дайСчёт()) {
			h = (int)(color_stop[i].stop * (n - 1));
			ch = color_stop[i].color;
		}
		else {
			h = n - 1;
			ch = color2;
		}
		int w = h - l;
		for(int j = 0; j < w; j++) {
			t->r = ((w - j) * cl.r + j * ch.r) / w;
			t->g = ((w - j) * cl.g + j * ch.g) / w;
			t->b = ((w - j) * cl.b + j * ch.b) / w;
			t->a = ((w - j) * cl.a + j * ch.a) / w;
			t++;
		}
		cl = ch;
		l = h;
	}
	*t = cl;
	return ib;
}

struct БуфРисовало::GradientImageMaker : public ImageMaker {
	КЗСА color1;
	Вектор<ColorStop> *color_stop;
	КЗСА color2;
	int n;

	virtual Ткст Ключ() const;
	virtual Рисунок  сделай() const;
};

Ткст БуфРисовало::GradientImageMaker::Ключ() const
{
	ТкстПоток ss;
	auto cc = const_cast<GradientImageMaker *>(this);
	ss % cc->color1 % *cc->color_stop % cc->color2 % cc->n;
	return ss.дайРез();
}

Рисунок БуфРисовало::GradientImageMaker::сделай() const
{
	return MakeGradient(color1, *color_stop, color2, n);
}

Рисунок БуфРисовало::MakeGradientCached(КЗСА color1, Вектор<ColorStop>& color_stop, КЗСА color2, int n)
{
	GradientImageMaker m;
	m.color1 = color1;
	m.color_stop = &color_stop;
	m.color2 = color2;
	m.n = n;
	return MakeImage(m);
}

Рисунок БуфРисовало::Gradient(const КЗСА& color1, const КЗСА& color2, int n)
{
	return (imagecache ? MakeGradientCached : MakeGradient)(color1, pathattr.color_stop, color2, n);
}

Рисунок БуфРисовало::Gradient(const КЗСА& color1, const КЗСА& color2, const ТочкаПЗ& p1, const ТочкаПЗ& p2)
{
	return Gradient(color1, color2, minmax(int(расстояние(p1, p2) * pathattr.mtx.GetScale()), 2, 16384));
}

static dword sLinearStyle(int style)
{
	dword s = style & ~FILL_FAST;
	return FILL_VPAD | (style & FILL_FAST) |
	       decode(s, GRADIENT_PAD, FILL_HPAD, GRADIENT_REPEAT, FILL_HREPEAT, FILL_HREFLECT);
}

void БуфРисовало::FillOp(const ТочкаПЗ& p1, const КЗСА& color1, const ТочкаПЗ& p2, const КЗСА& color2, int style)
{
	Fill(Gradient(color1, color2, p1, p2), p1, p2, sLinearStyle(style));
}

void БуфРисовало::FillOp(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	Fill(Gradient(color1, color2, 500), Xform2D::Scale(1.0 / 500) * transsrc, sLinearStyle(style));
}

void БуфРисовало::StrokeOp(double width, const ТочкаПЗ& p1, const КЗСА& color1, const ТочкаПЗ& p2, const КЗСА& color2, int style)
{
	Stroke(width, Gradient(color1, color2, p1, p2), p1, p2, sLinearStyle(style));
}

void БуфРисовало::StrokeOp(double width, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	Stroke(width, Gradient(color1, color2, 500), Xform2D::Scale(1.0 / 500) * transsrc, sLinearStyle(style));
}

}
