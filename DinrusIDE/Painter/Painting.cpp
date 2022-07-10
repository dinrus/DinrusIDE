#include "Painter.h"

namespace РНЦП {

void PaintingPainter::ClearOp(const КЗСА& color)
{
	помести(PAINTING_CLEAR);
	помести(color);
}

void PaintingPainter::двигОп(const ТочкаПЗ& p, bool rel)
{
	помести(PAINTING_MOVE + rel);
	Putf(p);
}

void PaintingPainter::линияОп(const ТочкаПЗ& p, bool rel)
{
	помести(PAINTING_LINE + rel);
	Putf(p);
}

void PaintingPainter::квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel)
{
	помести(PAINTING_QUADRATIC + rel);
	Putf(p1);
	Putf(p);
}

void PaintingPainter::квадрОп(const ТочкаПЗ& p, bool rel)
{
	помести(PAINTING_QUADRATIC_S + rel);
	Putf(p);
}

void PaintingPainter::кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel)
{
	помести(PAINTING_CUBIC + rel);
	Putf(p1);
	Putf(p2);
	Putf(p);
}

void PaintingPainter::кубОп(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel)
{
	помести(PAINTING_CUBIC_S + rel);
	Putf(p2);
	Putf(p);
}

void PaintingPainter::дугОп(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel)
{
	помести(PAINTING_ARC + rel);
	Putf(c);
	Putf(r);
	Putf(angle);
	Putf(sweep);
}

void PaintingPainter::SvgArcOp(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p, bool rel)
{
	помести(PAINTING_SVGARC + rel);
	Putf(r);
	Putf(xangle);
	помести(large);
	помести(sweep);
	Putf(p);
}

void PaintingPainter::закройОп()
{
	помести(PAINTING_CLOSE);
}

void PaintingPainter::делиОп()
{
	помести(PAINTING_DIV);
}

void PaintingPainter::FillOp(const КЗСА& color)
{
	помести(PAINTING_FILL_SOLID);
	помести(color);
}

void PaintingPainter::FillOp(const Рисунок& image, const Xform2D& transsrc, dword flags)
{
	помести(PAINTING_FILL_IMAGE);
	Putf(transsrc);
	помести(flags);
	data.добавь(image);
}

void PaintingPainter::FillOp(const ТочкаПЗ& p1, const КЗСА& color1, const ТочкаПЗ& p2,
                             const КЗСА& color2, int style)
{
	помести(PAINTING_FILL_GRADIENT);
	Putf(p1);
	помести(color1);
	Putf(p2);
	помести(color2);
	помести(style);
}

void PaintingPainter::FillOp(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	помести(PAINTING_FILL_GRADIENT_X);
	помести(color1);
	помести(color2);
	Putf(transsrc);
	помести(style);
}

void PaintingPainter::FillOp(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	помести(PAINTING_FILL_RADIAL_X);
	Putf(f);
	помести(color1);
	помести(color2);
	Putf(transsrc);
	помести(style);
}

void PaintingPainter::StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1,
                               const КЗСА& color2, const Xform2D& transsrc, int style)
{
	помести(PAINTING_FILL_RADIAL_X);
	Putf(width);
	Putf(f);
	помести(color1);
	помести(color2);
	Putf(transsrc);
	помести(style);
}


void PaintingPainter::FillOp(const ТочкаПЗ& f, const КЗСА& color1,
                             const ТочкаПЗ& p, double r, const КЗСА& color2, int style)
{
	помести(PAINTING_FILL_RADIAL);
	Putf(f);
	помести(color1);
	Putf(p);
	Putf(r);
	помести(color2);
	помести(style);
}

void PaintingPainter::StrokeOp(double width, const КЗСА& color)
{
	помести(PAINTING_STROKE_SOLID);
	Putf(width);
	помести(color);
}

void PaintingPainter::StrokeOp(double width, const Рисунок& image,
                               const Xform2D& transsrc, dword flags)
{
	помести(PAINTING_STROKE_IMAGE);
	Putf(width);
	Putf(transsrc);
	помести(flags);
	data.добавь(image);
}

void PaintingPainter::StrokeOp(double width, const ТочкаПЗ& p1, const КЗСА& color1,
                               const ТочкаПЗ& p2, const КЗСА& color2, int style)
{
	помести(PAINTING_STROKE_GRADIENT);
	Putf(width);
	Putf(p1);
	помести(color1);
	Putf(p2);
	помести(color2);
	помести(style);
}

void PaintingPainter::StrokeOp(double width, const КЗСА& color1, const КЗСА& color2,
                               const Xform2D& transsrc, int style)
{
	помести(PAINTING_STROKE_GRADIENT_X);
	Putf(width);
	помести(color1);
	помести(color2);
	Putf(transsrc);
	помести(style);
}

void PaintingPainter::StrokeOp(double width, const ТочкаПЗ& f,
                               const КЗСА& color1, const ТочкаПЗ& p, double r,
                               const КЗСА& color2, int style)
{
	помести(PAINTING_STROKE_RADIAL);
	Putf(width);
	Putf(f);
	помести(color1);
	Putf(p);
	Putf(r);
	помести(color2);
	помести(style);
}

void PaintingPainter::ClipOp()
{
	помести(PAINTING_CLIP);
}

void PaintingPainter::симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt)
{
	помести(PAINTING_CHARACTER);
	Putf(p);
	помести32(ch);
	помести(fnt);
}

void PaintingPainter::TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n, const double *dx)
{
	помести(PAINTING_TEXT);
	Putf(p);
	помести32(n);
	помести((bool)dx);
	помести(fnt);
	for(int i = 0; i < n; i++) {
		помести32(text[i]);
		if(dx)
			Putf(dx[i]);
	}
}

void PaintingPainter::ColorStopOp(double pos, const КЗСА& color)
{
	помести(PAINTING_COLORSTOP);
	Putf(pos);
	помести(color);
}

void PaintingPainter::ClearStopsOp()
{
	помести(PAINTING_CLEARSTOPS);
}

void PaintingPainter::OpacityOp(double o)
{
	помести(PAINTING_OPACITY);
	Putf(o);
}

void PaintingPainter::LineCapOp(int linecap)
{
	помести(PAINTING_LINECAP);
	помести(linecap);
}

void PaintingPainter::LineJoinOp(int linejoin)
{
	помести(PAINTING_LINEJOIN);
	помести(linejoin);
}

void PaintingPainter::MiterLimitOp(double l)
{
	помести(PAINTING_MITERLIMIT);
	Putf(l);
}

void PaintingPainter::EvenOddOp(bool evenodd)
{
	помести(PAINTING_EVENODD);
	помести(evenodd);
}

void PaintingPainter::InvertOp(bool invert)
{
	помести(PAINTING_INVERT);
	помести(invert);
}

void PaintingPainter::DashOp(const Вектор<double>& dash, double start)
{
	помести(PAINTING_DASH);
	помести32(dash.дайСчёт());
	for(int i = 0; i < dash.дайСчёт(); i++)
		Putf(dash[i]);
	Putf(start);
}

void PaintingPainter::TransformOp(const Xform2D& m)
{
	помести(PAINTING_TRANSFORM);
	Putf(m);
}

void PaintingPainter::BeginOp()
{
	помести(PAINTING_BEGIN);
}

void PaintingPainter::EndOp()
{
	помести(PAINTING_END);
}

void PaintingPainter::BeginMaskOp()
{
	помести(PAINTING_BEGINMASK);
}

void PaintingPainter::BeginOnPathOp(double q, bool абс)
{
	помести(PAINTING_BEGINONPATH);
	Putf(q);
	помести(абс);
}

Painting PaintingPainter::дайРез()
{
	Painting p;
	p.cmd = cmd.дайРез();
	p.data = data;
	p.size = size;
	return p;
}

void PaintingPainter::создай(double cx, double cy)
{
	cmd.создай();
	size.cx = cx;
	size.cy = cy;
}

void PaintingPainter::создай(РазмерПЗ sz)
{
	создай(sz.cx, sz.cy);
}

}
