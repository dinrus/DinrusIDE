#ifndef _Painter_Painter_h_
#define _Painter_Painter_h_

#include <Draw/Draw.h>

#ifdef flagTIMING
#define PAINTER_TIMING(x)     RTIMING(x)
#else
#define PAINTER_TIMING(x)  // RTIMING(x)
#endif

namespace РНЦП {

ИНИЦИАЛИЗУЙ(PaintPainting)

enum XformClass { // classification of Xform (simpler forms can be optimized)
	XFORM_REGULAR = 32, // same scale in X and Y, does not skew line width

	XFORM_IDENTITY = 0|XFORM_REGULAR, // not transformation
	XFORM_TRANSLATION = 1|XFORM_REGULAR,
	XFORM_REGULAR_SCALE = 2|XFORM_REGULAR, // just scale, same in X and Y
	XFORM_SCALE = 2, // just scale, but X scale != Y scale
	XFORM_ANY = 0,
};

struct Xform2D {
	ТочкаПЗ x, y, t;
	
	ТочкаПЗ Transform(double px, double py) const { return ТочкаПЗ(px * x.x + py * x.y + t.x, px * y.x + py * y.y + t.y); }
	ТочкаПЗ Transform(const ТочкаПЗ& f) const      { return Transform(f.x, f.y); }

	ТочкаПЗ GetScaleXY() const;
	double GetScale() const;
	bool   IsRegular() const;
	
	byte   GetClass() const;
	
	static Xform2D Identity();
	static Xform2D Translation(double x, double y);
	static Xform2D Scale(double sx, double sy);
	static Xform2D Scale(double scale);
	static Xform2D Rotation(double fi);
	static Xform2D Sheer(double fi);
	static Xform2D SheerX(double fi);
	static Xform2D вКарту(ТочкаПЗ s1, ТочкаПЗ s2, ТочкаПЗ s3); // maps 0,0 -> s3, 1,0 -> s1, 0,1 -> s2
	static Xform2D вКарту(ТочкаПЗ s1, ТочкаПЗ s2, ТочкаПЗ s3, ТочкаПЗ t1, ТочкаПЗ t2, ТочкаПЗ t3);
	
	Xform2D();
};

Xform2D operator*(const Xform2D& a, const Xform2D& b);
Xform2D Inverse(const Xform2D& m);

enum PainterOptions {
	LINECAP_BUTT,
	LINECAP_SQUARE,
	LINECAP_ROUND,

	LINEJOIN_MITER,
	LINEJOIN_ROUND,
	LINEJOIN_BEVEL,
	
	FILL_EXACT      = 0,

	FILL_HPAD       = 1,
	FILL_HREPEAT    = 2,
	FILL_HREFLECT   = 3,

	FILL_VPAD       = 4,
	FILL_VREPEAT    = 8,
	FILL_VREFLECT   = 12,

	FILL_PAD        = FILL_HPAD|FILL_VPAD,
	FILL_REPEAT     = FILL_HREPEAT|FILL_VREPEAT,
	FILL_REFLECT    = FILL_HREFLECT|FILL_VREFLECT,
	
	FILL_FAST       = 128,
	
	GRADIENT_PAD     = 0,
	GRADIENT_REPEAT  = 1,
	GRADIENT_REFLECT = 2,
};

class Рисовало : public Draw {
public:
	virtual dword GetInfo() const;

	virtual void OffsetOp(Точка p);
	virtual bool ClipOp(const Прям& r);
	virtual bool ClipoffOp(const Прям& r);
	virtual bool ExcludeClipOp(const Прям& r);
	virtual bool IntersectClipOp(const Прям& r);
	virtual bool IsPaintingOp(const Прям& r) const;

	virtual void DrawRectOp(int x, int y, int cx, int cy, Цвет color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color);
	virtual void DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts,
	                                int count_count, int width, Цвет color, Цвет doxor);
	virtual void DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc, Цвет color,
	                                   int width, Цвет outline, uint64 pattern, Цвет doxor);
	virtual void DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color);
	virtual void DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx);
	virtual void DrawPaintingOp(const Прям& target, const Painting& p);

protected:
	virtual void   ClearOp(const КЗСА& color) = 0;

	virtual void   двигОп(const ТочкаПЗ& p, bool rel) = 0;
	virtual void   линияОп(const ТочкаПЗ& p, bool rel) = 0;
	virtual void   квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel) = 0;
	virtual void   квадрОп(const ТочкаПЗ& p, bool rel) = 0;
	virtual void   кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel) = 0;
	virtual void   кубОп(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel) = 0;
	virtual void   дугОп(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel) = 0;
	virtual void   SvgArcOp(const ТочкаПЗ& r, double xangle, bool large, bool sweep,
	                        const ТочкаПЗ& p, bool rel) = 0;
	virtual void   закройОп() = 0;
	virtual void   делиОп() = 0;

	virtual void   FillOp(const КЗСА& color) = 0;
	virtual void   FillOp(const Рисунок& image, const Xform2D& transsrc, dword flags) = 0;
	virtual void   FillOp(const ТочкаПЗ& p1, const КЗСА& color1,
	                      const ТочкаПЗ& p2, const КЗСА& color2,
	                      int style) = 0;
	virtual void   FillOp(const ТочкаПЗ& f, const КЗСА& color1,
	                      const ТочкаПЗ& c, double r, const КЗСА& color2,
	                      int style) = 0;
	virtual void   FillOp(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc,
	                      int style) = 0;
	virtual void   FillOp(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2,
	                      const Xform2D& transsrc, int style) = 0;

	virtual void   StrokeOp(double width, const КЗСА& rgba) = 0;
	virtual void   StrokeOp(double width, const Рисунок& image, const Xform2D& transsrc,
	                        dword flags) = 0;
	virtual void   StrokeOp(double width, const ТочкаПЗ& p1, const КЗСА& color1,
	                        const ТочкаПЗ& p2, const КЗСА& color2,
	                        int style) = 0;
	virtual void   StrokeOp(double width, const КЗСА& color1, const КЗСА& color2,
	                        const Xform2D& transsrc, int style) = 0;
	virtual void   StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1,
	                        const ТочкаПЗ& c, double r, const КЗСА& color2,
	                        int style) = 0;
	virtual void   StrokeOp(double width, const ТочкаПЗ& f,
	                        const КЗСА& color1, const КЗСА& color2,
	                        const Xform2D& transsrc, int style) = 0;

	virtual void   ClipOp() = 0;

	virtual void   симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt) = 0;
	virtual void   TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n = -1,
	                      const double *dx = NULL);

	virtual void   ColorStopOp(double pos, const КЗСА& color) = 0;
	virtual void   ClearStopsOp() = 0;
	
	virtual void   OpacityOp(double o) = 0;
	virtual void   LineCapOp(int linecap) = 0;
	virtual void   LineJoinOp(int linejoin) = 0;
	virtual void   MiterLimitOp(double l) = 0;
	virtual void   EvenOddOp(bool evenodd) = 0;
	virtual void   DashOp(const Вектор<double>& dash, double start = 0) = 0;
	virtual void   DashOp(const Ткст& dash, double start = 0);
	virtual void   InvertOp(bool invert) = 0;

	virtual void   TransformOp(const Xform2D& m) = 0;

	virtual void   BeginOp() = 0;
	virtual void   EndOp() = 0;

	virtual void   BeginMaskOp() = 0;
	virtual void   BeginOnPathOp(double q, bool absolute) = 0;

protected:
	void   DoArc0(double theta, double th_sweep, const Xform2D& m);
	void   делайДугу(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, double xangle);
	void   DoSvgArc(const ТочкаПЗ& rr, double xangle, int large, int sweep,
	                 const ТочкаПЗ& p, const ТочкаПЗ& p0);
	void   DrawLineStroke(int width, Цвет color);

public:
	void     очисть(const КЗСА& color);

	Рисовало& Move(const ТочкаПЗ& p, bool rel);
	Рисовало& Move(const ТочкаПЗ& p);
	Рисовало& Move(double x, double y, bool rel);
	Рисовало& Move(double x, double y);
	Рисовало& RelMove(const ТочкаПЗ& p);
	Рисовало& RelMove(double x, double y);

	Рисовало& Строка(const ТочкаПЗ& p, bool rel);
	Рисовало& Строка(const ТочкаПЗ& p);
	Рисовало& Строка(double x, double y, bool rel);
	Рисовало& Строка(double x, double y);
	Рисовало& RelLine(const ТочкаПЗ& p);
	Рисовало& RelLine(double x, double y);

	Рисовало& Quadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel);
	Рисовало& Quadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p);
	Рисовало& Quadratic(const ТочкаПЗ& p);
	Рисовало& Quadratic(double x, double y, bool rel);
	Рисовало& Quadratic(double x1, double y1, double x, double y, bool rel);
	Рисовало& Quadratic(const ТочкаПЗ& p, bool rel);
	Рисовало& Quadratic(double x1, double y1, double x, double y);
	Рисовало& Quadratic(double x, double y);
	Рисовало& RelQuadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p);
	Рисовало& RelQuadratic(double x1, double y1, double x, double y);
	Рисовало& RelQuadratic(double x, double y);
	Рисовало& RelQuadratic(const ТочкаПЗ& p);

	Рисовало& Cubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel);
	Рисовало& Cubic(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel);
	Рисовало& Cubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p);
	Рисовало& Cubic(const ТочкаПЗ& p2, const ТочкаПЗ& p);
	Рисовало& Cubic(double x1, double y1, double x2, double y2, double x, double y, bool rel);
	Рисовало& Cubic(double x2, double y2, double x, double y, bool rel);
	Рисовало& Cubic(double x1, double y1, double x2, double y2, double x, double y);
	Рисовало& Cubic(double x2, double y2, double x, double y);
	Рисовало& RelCubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p);
	Рисовало& RelCubic(const ТочкаПЗ& p2, const ТочкаПЗ& p);
	Рисовало& RelCubic(double x1, double y1, double x2, double y2, double x, double y);
	Рисовало& RelCubic(double x2, double y2, double x, double y);

	Рисовало& Arc(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel);
	Рисовало& Arc(const ТочкаПЗ& c, double rx, double ry, double angle, double sweep, bool rel);
	Рисовало& Arc(const ТочкаПЗ& c, double r, double angle, double sweep, bool rel);
	Рисовало& Arc(double x, double y, double rx, double ry, double angle, double sweep, bool rel);
	Рисовало& Arc(double x, double y, double r, double angle, double sweep, bool rel);
	Рисовало& Arc(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep);
	Рисовало& Arc(const ТочкаПЗ& c, double rx, double ry, double angle, double sweep);
	Рисовало& Arc(const ТочкаПЗ& c, double r, double angle, double sweep);
	Рисовало& Arc(double x, double y, double rx, double ry, double angle, double sweep);
	Рисовало& Arc(double x, double y, double r, double angle, double sweep);
	Рисовало& RelArc(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep);
	Рисовало& RelArc(const ТочкаПЗ& c, double rx, double ry, double angle, double sweep);
	Рисовало& RelArc(const ТочкаПЗ& c, double r, double angle, double sweep);
	Рисовало& RelArc(double x, double y, double rx, double ry, double angle, double sweep);
	Рисовало& RelArc(double x, double y, double r, double angle, double sweep);

	Рисовало& SvgArc(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p, bool rel);
	Рисовало& SvgArc(double rx, double ry, double xangle, bool large, bool sweep, const ТочкаПЗ& p, bool rel);
	Рисовало& SvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y, bool rel);
	Рисовало& SvgArc(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p);
	Рисовало& SvgArc(double rx, double ry, double xangle, bool large, bool sweep, const ТочкаПЗ& p);
	Рисовало& SvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y);
	Рисовало& RelSvgArc(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p);
	Рисовало& RelSvgArc(double rx, double ry, double xangle, bool large, bool sweep, const ТочкаПЗ& p);
	Рисовало& RelSvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y);

	Рисовало& закрой();
	Рисовало& Div();

	Рисовало& Path(СиПарсер& p);
	Рисовало& Path(const char *path);

	Рисовало& Fill(const КЗСА& color);
	Рисовало& Fill(const Рисунок& image, const Xform2D& transsrc = Xform2D::Identity(), dword flags = 0);
	Рисовало& Fill(const Рисунок& image, ТочкаПЗ p1, ТочкаПЗ p2, dword flags = 0);
	Рисовало& Fill(const Рисунок& image, double x1, double y1, double x2, double y2,
	              dword flags = 0);
	Рисовало& Fill(const ТочкаПЗ& p1, const КЗСА& color1,
	              const ТочкаПЗ& p2, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Fill(double x1, double y1, const КЗСА& color1,
	              double x2, double y2, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Fill(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc,
	              dword flags = 0);
	Рисовало& Fill(const ТочкаПЗ& f, const КЗСА& color1,
	              const ТочкаПЗ& c, double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Fill(double fx, double fy, const КЗСА& color1,
	              double cx, double cy, double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Fill(const ТочкаПЗ& c, const КЗСА& color1,
	              double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Fill(double x, double y, const КЗСА& color1,
	              double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Fill(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2,
	              const Xform2D& transsrc, int style = GRADIENT_PAD);

	Рисовало& Stroke(double width, const КЗСА& color);
	Рисовало& Stroke(double width, const Рисунок& image, const Xform2D& transsrc, dword flags = 0);
	Рисовало& Stroke(double width, const Рисунок& image, const ТочкаПЗ& p1, const ТочкаПЗ& p2,
	                dword flags = 0);
	Рисовало& Stroke(double width, const Рисунок& image, double x1, double y1, double x2, double y2,
	                dword flags = 0);
	Рисовало& Stroke(double width, const ТочкаПЗ& p1, const КЗСА& color1,
	                const ТочкаПЗ& p2, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Stroke(double width, double x1, double y1, const КЗСА& color1,
	                double x2, double y2, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Stroke(double width, const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc,
	                dword flags = 0);
	Рисовало& Stroke(double width, const ТочкаПЗ& f, const КЗСА& color1,
	                const ТочкаПЗ& c, double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Stroke(double width, double fx, double fy, const КЗСА& color1,
	                double cx, double cy, double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Stroke(double width, const ТочкаПЗ& c, const КЗСА& color1,
	                double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Stroke(double width, double x, double y, const КЗСА& color1,
	                double r, const КЗСА& color2, int style = GRADIENT_PAD);
	Рисовало& Stroke(double width, const ТочкаПЗ& f,
	                const КЗСА& color1, const КЗСА& color2,
	                const Xform2D& transsrc, int style = GRADIENT_PAD);

	Рисовало& Clip();

	Рисовало& Character(const ТочкаПЗ& p, int ch, Шрифт fnt);
	Рисовало& Character(double x, double y, int ch, Шрифт fnt);
	Рисовало& устТекст(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n = -1, const double *dx = NULL);
	Рисовало& устТекст(double x, double y, const wchar *text, Шрифт fnt, int n = -1, const double *dx = NULL);
	Рисовало& устТекст(const ТочкаПЗ& p, const ШТкст& s, Шрифт fnt, const double *dx = NULL);
	Рисовало& устТекст(double x, double y, const ШТкст& s, Шрифт fnt, const double *dx = NULL);
	Рисовало& устТекст(const ТочкаПЗ& p, const Ткст& s, Шрифт fnt, const double *dx = NULL);
	Рисовало& устТекст(double x, double y, const Ткст& s, Шрифт fnt, const double *dx = NULL);
	Рисовало& устТекст(const ТочкаПЗ& p, const char *text, Шрифт fnt, int n = -1, const double *dx = NULL);
	Рисовало& устТекст(double x, double y, const char *text, Шрифт fnt, int n = -1, const double *dx = NULL);
	
	void EndPath()                                                    { Stroke(0, обнулиКЗСА()); }

	void старт();
	void стоп();

	void BeginMask();
	void BeginOnPath(double q, bool absolute = false);

	Рисовало& ColorStop(double pos, const КЗСА& color);
	Рисовало& ClearStops();
	Рисовало& Opacity(double o);
	Рисовало& LineCap(int linecap);
	Рисовало& LineJoin(int linejoin);
	Рисовало& MiterLimit(double l);
	Рисовало& EvenOdd(bool evenodd = true);
	Рисовало& Dash(const Вектор<double>& dash, double start);
	Рисовало& Dash(const char *dash, double start = 0);
	Рисовало& Invert(bool b = true);

	Рисовало& Transform(const Xform2D& m);
	Рисовало& Translate(double x, double y);
	Рисовало& Translate(const ТочкаПЗ& p);
	Рисовало& Rotate(double a);
	Рисовало& Scale(double scalex, double scaley);
	Рисовало& Scale(double scale);

	void     рисуй(const Painting& p);

	Рисовало& Rectangle(double x, double y, double cx, double cy);
	Рисовало& RoundedRectangle(double x, double y, double cx, double cy, double r);
	Рисовало& RoundedRectangle(double x, double y, double cx, double cy, double r1, double r2);
	Рисовало& Ellipse(double x, double y, double rx, double ry);
	Рисовало& Circle(double x, double y, double r);
	
	Рисовало& RectPath(int x, int y, int cx, int cy);
	Рисовало& RectPath(const Прям& r);
	Рисовало& RectPath(double x, double y, double cx, double cy);
	Рисовало& RectPath(const ПрямПЗ& r);
};

void рисуйСимвол(Рисовало& sw, const ТочкаПЗ& p, int ch, Шрифт fnt);

#include "Painter.hpp"
#include "Painting.h"
#include "LinearPath.h"
#include "BufferPainter.h"

class ImageBuffer__ {
protected:
	ImageBuffer ib;
	
public:
	ImageBuffer__(Размер sz) : ib(sz) {}
};

class ImagePainter : private ImageBuffer__, public БуфРисовало {
public:
	ImagePainter(Размер sz, int mode = MODE_ANTIALIASED);
	ImagePainter(int cx, int cy, int mode = MODE_ANTIALIASED);
	
	Рисунок дайРез() { финиш(); return ImageBuffer__::ib; }
	operator Рисунок()  { return дайРез(); }
};

class DrawPainter : public ImagePainter {
	Draw& w;

public:
	DrawPainter(Draw& w, Размер sz, int mode = MODE_ANTIALIASED);
	DrawPainter(Draw& w, int cx, int cy, int mode = MODE_ANTIALIASED) : DrawPainter(w, Размер(cx, cy), mode) {}
	~DrawPainter();
};

class NilPainter : public Рисовало {
protected:
	virtual void   ClearOp(const КЗСА& color);

	virtual void   двигОп(const ТочкаПЗ& p, bool rel);
	virtual void   линияОп(const ТочкаПЗ& p, bool rel);
	virtual void   квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel);
	virtual void   квадрОп(const ТочкаПЗ& p, bool rel);
	virtual void   кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel);
	virtual void   кубОп(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel);
	virtual void   дугОп(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel);
	virtual void   SvgArcOp(const ТочкаПЗ& r, double xangle, bool large, bool sweep,
	                        const ТочкаПЗ& p, bool rel);
	virtual void   закройОп();
	virtual void   делиОп();

	virtual void   FillOp(const КЗСА& color);
	virtual void   FillOp(const Рисунок& image, const Xform2D& transsrc, dword flags);
	virtual void   FillOp(const ТочкаПЗ& p1, const КЗСА& color1,
	                      const ТочкаПЗ& p2, const КЗСА& color2,
	                      int style);
	virtual void   FillOp(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc,
	                      int style);
	virtual void   FillOp(const ТочкаПЗ& f, const КЗСА& color1,
	                      const ТочкаПЗ& c, double r, const КЗСА& color2,
	                      int style);
	virtual void   FillOp(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2,
	                      const Xform2D& transsrc, int style);

	virtual void   StrokeOp(double width, const КЗСА& rgba);
	virtual void   StrokeOp(double width, const Рисунок& image, const Xform2D& transsrc,
	                        dword flags);
	virtual void   StrokeOp(double width, const ТочкаПЗ& p1, const КЗСА& color1,
	                        const ТочкаПЗ& p2, const КЗСА& color2,
	                        int style);
	virtual void   StrokeOp(double width, const КЗСА& color1, const КЗСА& color2,
	                        const Xform2D& transsrc,
	                        int style);
	virtual void   StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1,
	                        const ТочкаПЗ& c, double r, const КЗСА& color2,
	                        int style);
	virtual void   StrokeOp(double width, const ТочкаПЗ& f,
	                        const КЗСА& color1, const КЗСА& color2,
	                        const Xform2D& transsrc, int style);

	virtual void   ClipOp();

	virtual void   симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt);
	virtual void   TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n = -1,
	                      const double *dx = NULL);

	virtual void   ColorStopOp(double pos, const КЗСА& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const Вектор<double>& dash, double start);
	virtual void   InvertOp(bool invert);

	virtual void   TransformOp(const Xform2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();

	virtual void   BeginMaskOp();
	virtual void   BeginOnPathOp(double q, bool абс);
};

bool  RenderSVG(Рисовало& p, const char *svg, Событие<Ткст, Ткст&> resloader);
bool  RenderSVG(Рисовало& p, const char *svg);

void  GetSVGDimensions(const char *svg, РазмерПЗ& sz, ПрямПЗ& viewbox);
ПрямПЗ GetSVGBoundingBox(const char *svg);
ПрямПЗ GetSVGPathBoundingBox(const char *path);

Рисунок RenderSVGImage(Размер sz, const char *svg, Событие<Ткст, Ткст&> resloader);
Рисунок RenderSVGImage(Размер sz, const char *svg);

bool IsSVG(const char *svg);

}

#endif
