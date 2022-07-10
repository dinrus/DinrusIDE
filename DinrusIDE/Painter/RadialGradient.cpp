#include "Painter.h"

namespace РНЦП {

struct PainterRadialSpan : SpanSource {
	LinearInterpolator interpolator;
	double      cx, cy, r, fx, fy;
	int         style;
	int         alpha;
	const КЗСА *gradient;
	double      C;
	
	void уст(double _x, double _y, double _r, double _fx, double _fy)
	{
		cx = _x;
		cy = _y;
		r = _r;
		fx = _fx;
		fy = _fy;
		fx -= cx;
		fy -= cy;
		C = fx * fx + fy * fy - r * r;
	}

	void дай(КЗСА *_span, int x, int y, unsigned len)
	{
		if(r <= 0)
			return;
		interpolator.старт(x, y, len);
		КЗСА *span = (КЗСА *)_span;
		while(len--) {
			Точка p = interpolator.дай();
			int h;
			const double q256 = 1 / 256.0;
			double dx = q256 * p.x - cx - fx;
			double dy = q256 * p.y - cy - fy;
			if(dx == 0 && dy == 0)
				h = 0;
			else {
				double A = dx * dx + dy * dy;
				double b = (fx * dx + fy * dy);
				double t = (sqrt(b * b - A * C) - b) / (A);
				h = t >= 0.001 ? int(2047 / t) : 2047;
			}
			if(style == GRADIENT_REPEAT)
				h = h & 2047;
			else
			if(style == GRADIENT_REFLECT)
				h = (h & 2048) ? (2047 - h & 2047) : (h & 2047);
			else
				h = minmax(h, 0, 2047);
			*span++ = gradient[h];
		}
	}
};

void БуфРисовало::RenderRadial(double width, const ТочкаПЗ& f, const КЗСА& color1,
                                 const ТочкаПЗ& c, double r, const КЗСА& color2,
                                 const Xform2D& m, int style)
{
	Рисунок gradient = Gradient(color1, color2, 2048);
	RenderPath(width, [=](Один<SpanSource>& ss) {
		PainterRadialSpan& sg = ss.создай<PainterRadialSpan>();
		sg.interpolator.уст(Inverse(m));
		sg.style = style;
		sg.уст(c.x, c.y, r, f.x, f.y);
		sg.gradient = gradient[0];
	}, обнулиКЗСА());
}

void БуфРисовало::FillOp(const ТочкаПЗ& f, const КЗСА& color1, const ТочкаПЗ& c, double r, const КЗСА& color2, int style)
{
	RenderRadial(-1, f, color1, c, r, color2, pathattr.mtx, style);
}

void БуфРисовало::StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1, const ТочкаПЗ& c, double r, const КЗСА& color2, int style)
{
	RenderRadial(width, f, color1, c, r, color2, pathattr.mtx, style);
}

void БуфРисовало::RenderRadial(double width, const ТочкаПЗ& f, const КЗСА& color1,
                                 const КЗСА& color2, const Xform2D& transsrc, int style)
{
	RenderRadial(width, f, color1, ТочкаПЗ(0, 0), 1, color2, transsrc * pathattr.mtx, style);
}

void БуфРисовало::FillOp(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	RenderRadial(-1, f, color1, color2, transsrc, style);
}

void БуфРисовало::StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	RenderRadial(width, f, color1, color2, transsrc, style);
}

}
