#include "Painter.h"

namespace РНЦП {

struct PaintCharPath : FontGlyphConsumer {
	Рисовало *sw;
	
	virtual void Move(ТочкаПЗ p) {
		sw->Move(p);
	}
	virtual void Строка(ТочкаПЗ p) {
		sw->Строка(p);
	}
	virtual void Quadratic(ТочкаПЗ p1, ТочкаПЗ p2) {
		sw->Quadratic(p1, p2);
	}
	virtual void Cubic(ТочкаПЗ p1, ТочкаПЗ p2, ТочкаПЗ p3) {
		sw->Cubic(p1, p2, p3);
	}
	virtual void закрой() {
		sw->закрой();
	}
};

void рисуйСимвол(Рисовало& sw, const ТочкаПЗ& p, int chr, Шрифт font)
{
	GlyphInfo gi = GetGlyphInfo(font, chr);
	PaintCharPath pw;
	pw.sw = &sw;
	if(gi.IsNormal())
		font.Render(pw, p.x, p.y, chr);
	else
	if(gi.IsReplaced()) {
		Шрифт fnt = font;
		fnt.Face(gi.lspc);
		fnt.устВысоту(gi.rspc);
		fnt.Render(pw, p.x, p.y + font.GetAscent() - fnt.GetAscent(), chr);
	}
	else
	if(gi.IsComposed()) {
		ComposedGlyph cg;
		Compose(font, chr, cg);
		font.Render(pw, p.x, p.y, cg.basic_char);
		sw.Div();
		cg.mark_font.Render(pw, p.x + cg.mark_pos.x, p.y + cg.mark_pos.y, cg.mark_char);
	}
	sw.EvenOdd(true);
}

Xform2D GetLineSzXform(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const РазмерПЗ& sz)
{
	Xform2D m = Xform2D::Scale(расстояние(p1, p2) / sz.cx);
	m = m * Xform2D::Rotation(Bearing(p2 - p1));
	m = m * Xform2D::Translation(p1.x, p1.y);
	return m;
}

Рисовало& Рисовало::Fill(const Рисунок& image, ТочкаПЗ p1, ТочкаПЗ p2, dword flags)
{
	return Fill(image, GetLineSzXform(p1, p2, image.дайРазм()), flags);
}

Рисовало& Рисовало::Fill(const Рисунок& image, double x1, double y1,
                       double x2, double y2, dword flags)
{
	return Fill(image, ТочкаПЗ(x1, y1), ТочкаПЗ(x2, y2), flags);
}

Рисовало& Рисовало::Fill(double x1, double y1, const КЗСА& color1, double x2, double y2, const КЗСА& color2, int style)
{
	return Fill(ТочкаПЗ(x1, y1), color1, ТочкаПЗ(x2, y2), color2, style);
}

Рисовало& Рисовало::Fill(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, dword flags)
{
	FillOp(color1, color2, transsrc, flags);
	return *this;
}

Рисовало& Рисовало::Fill(double fx, double fy, const КЗСА& color1, double cx, double cy, double r, const КЗСА& color2, int style)
{
	return Fill(ТочкаПЗ(fx, fy), color1, ТочкаПЗ(cx, cy), r, color2, style);
}

Рисовало& Рисовало::Fill(const ТочкаПЗ& c, const КЗСА& color1, double r, const КЗСА& color2, int style)
{
	return Fill(c, color1, c, r, color2, style);
}

Рисовало& Рисовало::Fill(double x, double y, const КЗСА& color1, double r, const КЗСА& color2, int style)
{
	return Fill(ТочкаПЗ(x, y), color1, r, color2, style);
}

Рисовало& Рисовало::Fill(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	FillOp(f, color1, color2, transsrc, style);
	return *this;
}

Рисовало& Рисовало::Stroke(double width, const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style)
{
	StrokeOp(width, f, color1, color2, transsrc, style);
	return *this;
}

Рисовало& Рисовало::Translate(double x, double y)
{
	Transform(Xform2D::Translation(x, y));
	return *this;
}

Рисовало& Рисовало::Translate(const ТочкаПЗ& p)
{
	return Translate(p.x, p.y);
}

Рисовало& Рисовало::Stroke(double width, const Рисунок& image, const ТочкаПЗ& p1, const ТочкаПЗ& p2, dword flags)
{
	return Stroke(width, image, GetLineSzXform(p1, p2, image.дайРазм()), flags);
}

Рисовало& Рисовало::Stroke(double width, const Рисунок& image, double x1, double y1, double x2, double y2, dword flags)
{
	return Stroke(width, image, ТочкаПЗ(x1, y1), ТочкаПЗ(x2, y2), flags);
}

Рисовало& Рисовало::Stroke(double width, double x1, double y1, const КЗСА& color1, double x2, double y2, const КЗСА& color2, int style)
{
	return Stroke(width, ТочкаПЗ(x1, y1), color1, ТочкаПЗ(x2, y2), color2, style);
}

Рисовало& Рисовало::Stroke(double width, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, dword flags)
{
	StrokeOp(width, color1, color2, transsrc, flags);
	return *this;
}

Рисовало& Рисовало::Stroke(double width, double fx, double fy, const КЗСА& color1, double cx, double cy, double r, const КЗСА& color2, int style)
{
	return Stroke(width, ТочкаПЗ(fx, fy), color1, ТочкаПЗ(cx, cy), r, color2, style);
}

Рисовало& Рисовало::Stroke(double width, const ТочкаПЗ& c, const КЗСА& color1, double r, const КЗСА& color2, int style)
{
	return Stroke(width, c, color1, c, r, color2, style);
}

Рисовало& Рисовало::Stroke(double width, double x, double y, const КЗСА& color1, double r, const КЗСА& color2, int style)
{
	return Stroke(width, ТочкаПЗ(x, y), color1, r, color2, style);
}

Рисовало& Рисовало::Rotate(double a)
{
	Transform(Xform2D::Rotation(a));
	return *this;
}

Рисовало& Рисовало::Scale(double scalex, double scaley)
{
	Transform(Xform2D::Scale(scalex, scaley));
	return *this;
}

Рисовало& Рисовало::Scale(double scale)
{
	Scale(scale, scale);
	return *this;
}

Вектор<double> StringToDash(const Ткст& dash, double& start)
{
	Вектор<double> d;
	СиПарсер p(dash);
	try {
		while(!p.кф_ли())
			if(p.сим(':'))
				start = p.читайДво();
			else
				d.добавь(p.читайДво());
	}
	catch(СиПарсер::Ошибка) {}
	if(d.дайСчёт() & 1) {
		Вектор<double> dash1;
		dash1.приставь(d);
		dash1.приставь(d);
		return dash1;
	}
	return d;
}

void Рисовало::DashOp(const Ткст& dash, double start)
{
	Вектор<double> h = StringToDash(dash, start);
	DashOp(h, start);
}

Рисовало& Рисовало::Character(double x, double y, int ch, Шрифт fnt)
{
	return Character(ТочкаПЗ(x, y), ch, fnt);
}

void Рисовало::TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n, const double *dx)
{
	if(n == 0) {
		Move(0, 0);
		return;
	}
	Шрифт fi = fnt;
	double m = 1;
	if(fnt.дайВысоту() < 50) {
		fi.устВысоту(100 * fnt.дайВысоту());
		m = 0.01;
	}
	else
	if(fnt.дайВысоту() < 500) {
		fi.устВысоту(10 * fnt.дайВысоту());
		m = 0.1;
	}
	double x = p.x;
	while(n) {
		int ch = *text++;
		Character(x, p.y, ch, fnt);
		Div();
		if(dx)
			x += *dx++;
		else
			x += fi[ch] * m;
		n--;
	}
	if(fnt.IsUnderline() || fnt.IsStrikeout()) {
		double a = fnt.GetAscent();
		double cy = max(a / 16, 1.0);
		double cx = x - p.x;
		if(fnt.IsUnderline())
			Rectangle(p.x, p.y + a + cy, cx, cy);
		if(fnt.IsStrikeout())
			Rectangle(p.x, p.y + 2 * a / 3, cx, cy);
	}
}

Рисовало& Рисовало::устТекст(double x, double y, const wchar *text, Шрифт fnt, int n, const double *dx)
{
	return устТекст(ТочкаПЗ(x, y), text, fnt, n < 0 ? strlen__(text) : n, dx);
}

Рисовало& Рисовало::устТекст(const ТочкаПЗ& p, const ШТкст& s, Шрифт fnt, const double *dx)
{
	return устТекст(p, ~s, fnt, s.дайДлину(), dx);
}

Рисовало& Рисовало::устТекст(double x, double y, const ШТкст& s, Шрифт fnt, const double *dx)
{
	return устТекст(ТочкаПЗ(x, y), s, fnt, dx);
}

Рисовало& Рисовало::устТекст(const ТочкаПЗ& p, const Ткст& s, Шрифт fnt, const double *dx)
{
	return устТекст(p, s.вШТкст(), fnt, dx);
}

Рисовало& Рисовало::устТекст(double x, double y, const Ткст& s, Шрифт fnt, const double *dx)
{
	return устТекст(ТочкаПЗ(x, y), s, fnt, dx);
}

Рисовало& Рисовало::устТекст(const ТочкаПЗ& p, const char *text, Шрифт fnt, int n, const double *dx)
{
	ШТкст s = вУтф32(text, n);
	return устТекст(p, s, fnt, n < 0 ? s.дайСчёт() : n, dx);
}

Рисовало& Рисовало::устТекст(double x, double y, const char *text, Шрифт fnt, int n, const double *dx)
{
	return устТекст(ТочкаПЗ(x, y), text, fnt, n, dx);
}

Рисовало& Рисовало::Rectangle(double x, double y, double cx, double cy)
{
	if (cx < 0) { x += cx; cx = -cx;}
	if (cy < 0) { y += cy; cy = -cy;}
	return Move(x, y).RelLine(cx, 0).RelLine(0, cy).RelLine(-cx, 0).закрой();
}

Рисовало& Рисовало::RoundedRectangle(double x, double y, double cx, double cy, double r)
{
	return RoundedRectangle(x, y, cx, cy, r, r);
}

Рисовало& Рисовало::RoundedRectangle(double x, double y, double cx, double cy, double rx, double ry)
{
	ПРОВЕРЬ(rx >= 0 && ry >= 0);
	if (cx < 0) { x += cx; cx = -cx;}
	if (cy < 0) { y += cy; cy = -cy;}
	Move(x + rx, y).Arc(x + rx, y + ry, rx, ry, -M_PI / 2, -M_PI / 2)
	.Строка(x, y + cy - ry).Arc(x + rx, y + cy - ry, rx, ry, M_PI, -M_PI / 2)
	.Строка(x + cx - rx, y + cy).Arc(x + cx - rx, y + cy - ry, rx, ry, M_PI / 2, -M_PI / 2)
	.Строка(x + cx, y + ry).Arc(x + cx - rx, y + ry, rx, ry, 0, -M_PI / 2).Строка(x + rx, y);
	return *this;
}

Рисовало& Рисовало::Ellipse(double x, double y, double rx, double ry)
{
	return Move(x + rx, y).Arc(x, y, rx, ry, 0, 2 * M_PI).закрой();
}

Рисовало& Рисовало::Circle(double x, double y, double r)
{
	return Ellipse(x, y, r, r);
}

void NilPainter::ClearOp(const КЗСА& color) {}
void NilPainter::двигОп(const ТочкаПЗ& p, bool rel) {}
void NilPainter::линияОп(const ТочкаПЗ& p, bool rel) {}
void NilPainter::квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel) {}
void NilPainter::квадрОп(const ТочкаПЗ& p, bool rel) {}
void NilPainter::кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel) {}
void NilPainter::кубОп(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel) {}
void NilPainter::дугОп(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel) {}
void NilPainter::SvgArcOp(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p, bool rel) {}
void NilPainter::закройОп() {}
void NilPainter::делиОп() {}
void NilPainter::FillOp(const КЗСА& color) {}
void NilPainter::FillOp(const Рисунок& image, const Xform2D& transsrc, dword flags) {}
void NilPainter::FillOp(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style) {}
void NilPainter::FillOp(const ТочкаПЗ& p1, const КЗСА& color1, const ТочкаПЗ& p2, const КЗСА& color2, int style) {}
void NilPainter::FillOp(const ТочкаПЗ& f, const КЗСА& color1, const ТочкаПЗ& c, double r, const КЗСА& color2, int style) {}
void NilPainter::FillOp(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style) {}
void NilPainter::StrokeOp(double width, const КЗСА& rgba) {}
void NilPainter::StrokeOp(double width, const Рисунок& image, const Xform2D& transsrc, dword flags) {}
void NilPainter::StrokeOp(double width, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style) {}
void NilPainter::StrokeOp(double width, const ТочкаПЗ& p1, const КЗСА& color1, const ТочкаПЗ& p2, const КЗСА& color2, int style) {}
void NilPainter::StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1, const ТочкаПЗ& c, double r, const КЗСА& color2, int style) {}
void NilPainter::StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc, int style) {}
void NilPainter::ClipOp() {}
void NilPainter::симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt) {}
void NilPainter::TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n, const double *dx) {}
void NilPainter::ColorStopOp(double pos, const КЗСА& color) {}
void NilPainter::ClearStopsOp() {}
void NilPainter::OpacityOp(double o) {}
void NilPainter::LineCapOp(int linecap) {}
void NilPainter::LineJoinOp(int linejoin) {}
void NilPainter::MiterLimitOp(double l) {}
void NilPainter::EvenOddOp(bool evenodd) {}
void NilPainter::InvertOp(bool invert) {}
void NilPainter::DashOp(const Вектор<double>& dash, double start) {}
void NilPainter::TransformOp(const Xform2D& m) {}
void NilPainter::BeginOp() {}
void NilPainter::EndOp() {}
void NilPainter::BeginMaskOp() {}
void NilPainter::BeginOnPathOp(double, bool) {}

ImagePainter::ImagePainter(Размер sz, int mode)
:	ImageBuffer__(sz), БуфРисовало(ImageBuffer__::ib, mode)
{}

ImagePainter::ImagePainter(int cx, int cy, int mode)
:	ImageBuffer__(Размер(cx, cy)), БуфРисовало(ImageBuffer__::ib, mode)
{}

DrawPainter::DrawPainter(Draw& w, Размер sz, int mode)
:	ImagePainter(sz, mode), w(w)
{}

DrawPainter::~DrawPainter()
{
	финиш();
	GetBuffer().PaintOnceHint();
	w.DrawImage(0, 0, *this);
}
	
}
