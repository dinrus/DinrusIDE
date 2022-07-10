inline
void Рисовало::очисть(const КЗСА& color)
{
	ClearOp(color);
}

inline
Рисовало& Рисовало::Move(const ТочкаПЗ& p, bool rel)
{
	двигОп(p, rel);
	return *this;
}

inline
Рисовало& Рисовало::Строка(const ТочкаПЗ& p, bool rel)
{
	линияОп(p, rel);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel)
{
	квадрОп(p1, p, rel);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(const ТочкаПЗ& p, bool rel)
{
	квадрОп(p, rel);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel)
{
	кубОп(p1, p2, p, rel);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel)
{
	кубОп(p2, p, rel);
	return *this;
}

inline
Рисовало& Рисовало::Arc(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel)
{
	дугОп(c, r, angle, sweep, rel);
	return *this;
}

inline
Рисовало& Рисовало::SvgArc(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p, bool rel)
{
	SvgArcOp(r, xangle, large, sweep, p, rel);
	return *this;
}

inline
Рисовало& Рисовало::закрой()
{
	закройОп();
	return *this;
}

inline
Рисовало& Рисовало::Div()
{
	делиОп();
	return *this;
}

inline
Рисовало& Рисовало::Fill(const КЗСА& color)
{
	FillOp(color);
	return *this;
}

inline
Рисовало& Рисовало::Fill(const Рисунок& image, const Xform2D& transsrc, dword flags)
{
	FillOp(image, transsrc, flags);
	return *this;
}

inline
Рисовало& Рисовало::Fill(const ТочкаПЗ& p1, const КЗСА& color1, const ТочкаПЗ& p2, const КЗСА& color2, int style)
{
	FillOp(p1, color1, p2, color2, style);
	return *this;
}

inline
Рисовало& Рисовало::Fill(const ТочкаПЗ& f, const КЗСА& color1, const ТочкаПЗ& c, double r, const КЗСА& color2, int style)
{
	FillOp(f, color1, c, r, color2, style);
	return *this;
}

inline
Рисовало& Рисовало::Stroke(double width, const КЗСА& color)
{
	StrokeOp(width, color);
	return *this;
}

inline
Рисовало& Рисовало::Stroke(double width, const Рисунок& image, const Xform2D& transsrc, dword flags)
{
	StrokeOp(width, image, transsrc, flags);
	return *this;
}

inline
Рисовало& Рисовало::Stroke(double width, const ТочкаПЗ& p1, const КЗСА& color1, const ТочкаПЗ& p2, const КЗСА& color2, int style)
{
	StrokeOp(width, p1, color1, p2, color2, style);
	return *this;
}

inline
Рисовало& Рисовало::Stroke(double width, const ТочкаПЗ& f, const КЗСА& color1, const ТочкаПЗ& c, double r, const КЗСА& color2, int style)
{
	StrokeOp(width, f, color1, c, r, color2, style);
	return *this;
}

inline
Рисовало& Рисовало::Clip()
{
	ClipOp();
	return *this;
}

inline Рисовало& Рисовало::ColorStop(double pos, const КЗСА& color)
{
	ColorStopOp(pos, color);
	return *this;
}

inline Рисовало& Рисовало::ClearStops()
{
	ClearStopsOp();
	return *this;
}

inline Рисовало& Рисовало::Opacity(double o)
{
	OpacityOp(o);
	return *this;
}

inline Рисовало& Рисовало::LineCap(int linecap)
{
	LineCapOp(linecap);
	return *this;
}

inline Рисовало& Рисовало::LineJoin(int linejoin)
{
	LineJoinOp(linejoin);
	return *this;
}

inline Рисовало& Рисовало::MiterLimit(double l)
{
	MiterLimitOp(l);
	return *this;
}

inline Рисовало& Рисовало::EvenOdd(bool evenodd)
{
	EvenOddOp(evenodd);
	return *this;
}

inline Рисовало& Рисовало::Invert(bool b)
{
	InvertOp(b);
	return *this;
}

inline Рисовало& Рисовало::Dash(const Вектор<double>& dash, double start)
{
	if(dash.дайСчёт() & 1) {
		Вектор<double> dash1;
		dash1.приставь(dash);
		dash1.приставь(dash);
		DashOp(dash1, start);
	}
	else
		DashOp(dash, start);
	return *this;
}

inline Рисовало& Рисовало::Dash(const char *dash, double start)
{
	DashOp(dash, start);
	return *this;
}

inline Рисовало& Рисовало::Transform(const Xform2D& m)
{
	TransformOp(m);
	return *this;
}

inline void Рисовало::старт()
{
	BeginOp();
}

inline void Рисовало::стоп()
{
	EndOp();
}

inline void Рисовало::BeginMask()
{
	BeginMaskOp();
}

inline void Рисовало::BeginOnPath(double q, bool абс)
{
	BeginOnPathOp(q, абс);
}

inline Рисовало& Рисовало::Character(const ТочкаПЗ& p, int ch, Шрифт fnt)
{
	симвОп(p, ch, fnt);
	return *this;
}

inline
Рисовало& Рисовало::устТекст(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n, const double *dx)
{
	TextOp(p, text, fnt, n, dx);
	return *this;
}

inline
Рисовало& Рисовало::Move(const ТочкаПЗ& p)
{
	Move(p, false);
	return *this;
}

inline
Рисовало& Рисовало::Строка(const ТочкаПЗ& p)
{
	Строка(p, false);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p)
{
	квадрОп(p1, p, false);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(const ТочкаПЗ& p)
{
	квадрОп(p, false);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p)
{
	кубОп(p1, p2, p, false);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(const ТочкаПЗ& p2, const ТочкаПЗ& p)
{
	кубОп(p2, p, false);
	return *this;
}

inline
Рисовало& Рисовало::RelMove(const ТочкаПЗ& p)
{
	двигОп(p, true);
	return *this;
}

inline
Рисовало& Рисовало::RelLine(const ТочкаПЗ& p)
{
	линияОп(p, true);
	return *this;
}

inline
Рисовало& Рисовало::RelQuadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p)
{
	квадрОп(p1, p, true);
	return *this;
}

inline
Рисовало& Рисовало::RelQuadratic(const ТочкаПЗ& p)
{
	квадрОп(p, true);
	return *this;
}

inline
Рисовало& Рисовало::RelCubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p)
{
	кубОп(p1, p2, p, true);
	return *this;
}

inline
Рисовало& Рисовало::RelCubic(const ТочкаПЗ& p2, const ТочкаПЗ& p)
{
	кубОп(p2, p, true);
	return *this;
}

inline
Рисовало& Рисовало::Move(double x, double y, bool rel)
{
	двигОп(ТочкаПЗ(x, y), rel);
	return *this;
}

inline
Рисовало& Рисовало::Строка(double x, double y, bool rel)
{
	линияОп(ТочкаПЗ(x, y), rel);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(double x1, double y1, double x, double y, bool rel)
{
	квадрОп(ТочкаПЗ(x1, y1), ТочкаПЗ(x, y), rel);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(double x, double y, bool rel)
{
	квадрОп(ТочкаПЗ(x, y), rel);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(double x1, double y1, double x2, double y2, double x, double y, bool rel)
{
	кубОп(ТочкаПЗ(x1, y1), ТочкаПЗ(x2, y2), ТочкаПЗ(x, y), rel);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(double x2, double y2, double x, double y, bool rel)
{
	кубОп(ТочкаПЗ(x2, y2), ТочкаПЗ(x, y), rel);
	return *this;
}

inline
Рисовало& Рисовало::Move(double x, double y)
{
	Move(x, y, false);
	return *this;
}

inline
Рисовало& Рисовало::Строка(double x, double y)
{
	Строка(x, y, false);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(double x1, double y1, double x, double y)
{
	Quadratic(x1, y1, x, y, false);
	return *this;
}

inline
Рисовало& Рисовало::Quadratic(double x, double y)
{
	Quadratic(x, y, false);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(double x1, double y1, double x2, double y2, double x, double y)
{
	Cubic(x1, y1, x2, y2, x, y, false);
	return *this;
}

inline
Рисовало& Рисовало::Cubic(double x2, double y2, double x, double y)
{
	Cubic(x2, y2, x, y, false);
	return *this;
}

inline
Рисовало& Рисовало::RelMove(double x, double y)
{
	Move(x, y, true);
	return *this;
}

inline
Рисовало& Рисовало::RelLine(double x, double y)
{
	Строка(x, y, true);
	return *this;
}

inline
Рисовало& Рисовало::RelQuadratic(double x1, double y1, double x, double y)
{
	Quadratic(x1, y1, x, y, true);
	return *this;
}

inline
Рисовало& Рисовало::RelQuadratic(double x, double y)
{
	Quadratic(x, y, true);
	return *this;
}

inline
Рисовало& Рисовало::RelCubic(double x1, double y1, double x2, double y2, double x, double y)
{
	Cubic(x1, y1, x2, y2, x, y, true);
	return *this;
}

inline
Рисовало& Рисовало::RelCubic(double x2, double y2, double x, double y)
{
	Cubic(x2, y2, x, y, true);
	return *this;
}

inline
Рисовало& Рисовало::Arc(const ТочкаПЗ& c, double rx, double ry, double angle, double sweep, bool rel)
{
	return Arc(c, ТочкаПЗ(rx, ry), angle, sweep, rel);
}

inline
Рисовало& Рисовало::Arc(const ТочкаПЗ& c, double r, double angle, double sweep, bool rel)
{
	return Arc(c, ТочкаПЗ(r, r), angle, sweep, rel);
}

inline
Рисовало& Рисовало::Arc(double x, double y, double rx, double ry, double angle, double sweep, bool rel)
{
	return Arc(ТочкаПЗ(x, y), rx, ry, angle, sweep, rel);
}

inline
Рисовало& Рисовало::Arc(double x, double y, double r, double angle, double sweep, bool rel)
{
	return Arc(ТочкаПЗ(x, y), r, angle, sweep, rel);
}

inline
Рисовало& Рисовало::Arc(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, false);
}

inline
Рисовало& Рисовало::Arc(const ТочкаПЗ& c, double rx, double ry, double angle, double sweep)
{
	return Arc(c, rx, ry, angle, sweep, false);
}

inline
Рисовало& Рисовало::Arc(const ТочкаПЗ& c, double r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, false);
}

inline
Рисовало& Рисовало::Arc(double x, double y, double rx, double ry, double angle, double sweep)
{
	return Arc(x, y, rx, ry, angle, sweep, false);
}

inline
Рисовало& Рисовало::Arc(double x, double y, double r, double angle, double sweep)
{
	return Arc(x, y, r, angle, sweep, false);
}


inline
Рисовало& Рисовало::RelArc(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, true);
}

inline
Рисовало& Рисовало::RelArc(const ТочкаПЗ& c, double rx, double ry, double angle, double sweep)
{
	return Arc(c, rx, ry, angle, sweep, true);
}

inline
Рисовало& Рисовало::RelArc(const ТочкаПЗ& c, double r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, true);
}

inline
Рисовало& Рисовало::RelArc(double x, double y, double rx, double ry, double angle, double sweep)
{
	return Arc(x, y, rx, ry, angle, sweep, true);
}

inline
Рисовало& Рисовало::RelArc(double x, double y, double r, double angle, double sweep)
{
	return Arc(x, y, r, angle, sweep, true);
}

inline
Рисовало& Рисовало::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, const ТочкаПЗ& p, bool rel)
{
	return SvgArc(ТочкаПЗ(rx, ry), xangle, large, sweep, p, rel);
}

inline
Рисовало& Рисовало::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y, bool rel)
{
	return SvgArc(ТочкаПЗ(rx, ry), xangle, large, sweep, ТочкаПЗ(x, y), rel);
}

inline
Рисовало& Рисовало::SvgArc(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p)
{
	return SvgArc(r, xangle, large, sweep, p, false);
}

inline
Рисовало& Рисовало::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, const ТочкаПЗ& p)
{
	return SvgArc(rx, ry, xangle, large, sweep, p, false);
}

inline
Рисовало& Рисовало::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y)
{
	return SvgArc(rx, ry, xangle, large, sweep, x, y, false);
}

inline
Рисовало& Рисовало::RelSvgArc(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p)
{
	return SvgArc(r, xangle, large, sweep, p, true);
}

inline
Рисовало& Рисовало::RelSvgArc(double rx, double ry, double xangle, bool large, bool sweep, const ТочкаПЗ& p)
{
	return SvgArc(rx, ry, xangle, large, sweep, p, true);
}

inline
Рисовало& Рисовало::RelSvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y)
{
	return SvgArc(rx, ry, xangle, large, sweep, x, y, true);
}
