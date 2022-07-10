#include "Draw.h"

namespace РНЦП {

#define LLOG(x)    // RLOG(x)
#define LTIMING(x) // RTIMING(x)
//#define BENCH

static СтатическийСтопор sDrawLock;

ИНИЦБЛОК {
	Значение::регистрируй<Painting>("Painting");
	Значение::регистрируй<Чертёж>("Чертёж");
}

Цвет Draw::GetDefaultInk() const
{
	dword w = GetInfo();
	if(w & DRAWING)
		return DefaultInk();
	if(w & DOTS)
		return чёрный();
	return SColorText();
}

void Draw::SinCos(int angle, double& sina, double& cosa)
{
	angle = angle % 3600;
	switch(angle) {
	case    0: sina =  0; cosa =  1; break;
	case  900: sina =  1; cosa =  0; break;
	case 1800: sina =  0; cosa = -1; break;
	case 2700: sina = -1; cosa =  0; break;
	default:
		double a = angle * M_PI / 1800.0;
		sina = sin(a);
		cosa = cos(a);
		break;
	}
}

Draw::~Draw() {}

Размер Draw::GetPixelsPerInch() const
{
	return IsNative() ? GetNativeDpi() : Dots() ? Размер(600, 600) : Размер(96, 96);
}

Размер Draw::GetPageMMs() const
{
	return GetPageSize() * 254 / (10 * GetPixelsPerInch());
}

int Draw::GetNativeX(int x) const
{
	Размер sz = GetNativeDpi();
	return Dots() && sz.cx != 600 ? iscale(x, sz.cx, 600) : x;
}

int Draw::GetNativeY(int y) const
{
	Размер sz = GetNativeDpi();
	return Dots() && sz.cx != 600 ? iscale(y, sz.cy, 600) : y;
}

void Draw::Native(int& x, int& y) const
{
	x = GetNativeX(x);
	y = GetNativeY(y);
}

void Draw::Native(Точка& p) const
{
	Native(p.x, p.y);
}

void Draw::Native(Размер& sz) const
{
	Native(sz.cx, sz.cy);
}

void Draw::Native(Прям& r) const
{
	Native(r.left, r.top);
	Native(r.right, r.bottom);
}

#ifdef BENCH
static TimingInspector sDrawTiming("DRAW");
#endif

void EnterDraw() {
	sDrawLock.войди();
#ifdef BENCH
	sDrawTiming.старт();
#endif
}

void LeaveDraw() {
#ifdef BENCH
	sDrawTiming.стоп();
#endif
	sDrawLock.выйди();
}

Размер Draw::GetPageSize() const
{
	return Размер(INT_MAX / 2, INT_MAX / 2);
}

Прям Draw::GetPaintRect() const
{
	return Прям(-(INT_MAX / 2), -(INT_MAX / 2), INT_MAX / 2, INT_MAX / 2);
}


void Draw::StartPage() {}
void Draw::EndPage() {}

Размер Draw::GetNativeDpi() const { return Dots() ? Размер(600, 600) : Размер(96, 96); }
void Draw::BeginNative() {}
void Draw::EndNative() {}

int Draw::GetCloffLevel() const { return 0; }

void Draw::Escape(const Ткст& data) {}

// -------------------------------

void Draw::SysDrawImageOp(int x, int y, const Рисунок& img, Цвет color)
{
	NEVER();
}

void Draw::SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color)
{
	if(src == Прям(img.дайРазм()))
		SysDrawImageOp(x, y, img, color);
	else {
		Clipoff(x, y, src.дайШирину(), src.дайВысоту());
		SysDrawImageOp(-src.left, -src.top, img, color);
		стоп();
	}
}

void Draw::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color)
{
	LTIMING("DrawImageOp");
	LLOG("DrawImageOp " << RectC(x, y, cx, cy) << ", imgsize: " << img.дайРазм() << ", src: " << src << ", color: " << color);
	bool tonative = !IsNative();
	if(tonative) {
		BeginNative();
		Native(x, y);
		Native(cx, cy);
	}
	Размер sz = Размер(cx, cy);
	if((cx > 2000 || cy > 1500) && пусто_ли(color) && IsPrinter()) {
		int yy = 0;
		ImageRaster ir(img);
		RescaleImage rm;
		rm.создай(Размер(cx, cy), ir, src);
		while(yy < cy) {
			int ccy = min(cy - yy, 16);
			ImageBuffer ib(cx, ccy);
			for(int q = 0; q < ccy; q++)
				rm.дай(ib[q]);
			DrawImageBandRLE(*this, x, y + yy, ib, 16);
			yy += ccy;
		}
	}
	else
	if(src.дайРазм() == sz)
		SysDrawImageOp(x, y, img, src, color);
	else {
		Рисунок h = Rescale(img, Размер(cx, cy), src);
		SysDrawImageOp(x, y, h, h.дайРазм(), color);
	}
	if(tonative)
		EndNative();
}

// -------------------------------

void Draw::DrawRect(const Прям& rect, Цвет color)
{
	DrawRect(rect.left, rect.top, rect.дайШирину(), rect.дайВысоту(), color);
}

void Draw::DrawRect(int x, int y, int cx, int cy, Цвет color)
{
	DrawRectOp(x, y, cx, cy, color);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src)
{
	DrawImageOp(x, y, cx, cy, img, src, Null);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Рисунок& img)
{
	DrawImage(x, y, cx, cy, img, img.дайРазм());
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color)
{
	if(пусто_ли(color)) return;
	DrawImageOp(x, y, cx, cy, img, src, color);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Рисунок& img, Цвет color)
{
	if(пусто_ли(color)) return;
	DrawImage(x, y, cx, cy, img, img.дайРазм(), color);
}

void Draw::DrawImage(const Прям& r, const Рисунок& img, const Прям& src)
{
	DrawImage(r.left, r.top, r.устШирину(), r.устВысоту(), img, src);
}

void Draw::DrawImage(const Прям& r, const Рисунок& img)
{
	DrawImage(r.left, r.top, r.устШирину(), r.устВысоту(), img);
}

void Draw::DrawImage(const Прям& r, const Рисунок& img, const Прям& src, Цвет color)
{
	if(пусто_ли(color)) return;
	DrawImage(r.left, r.top, r.устШирину(), r.устВысоту(), img, src, color);
}

void Draw::DrawImage(const Прям& r, const Рисунок& img, Цвет color)
{
	if(пусто_ли(color)) return;
	DrawImage(r.left, r.top, r.устШирину(), r.устВысоту(), img, color);
}

void Draw::DrawImage(int x, int y, const Рисунок& img, const Прям& src)
{
	Размер sz = src.дайРазм();
	DrawImageOp(x, y, sz.cx, sz.cy, img, src, Null);
}

void Draw::DrawImage(int x, int y, const Рисунок& img)
{
	Размер sz = img.дайРазм();
	DrawImageOp(x, y, sz.cx, sz.cy, img, img.дайРазм(), Null);
}

void Draw::DrawImage(int x, int y, const Рисунок& img, const Прям& src, Цвет color)
{
	if(пусто_ли(color)) return;
	DrawImageOp(x, y, src.устШирину(), src.устВысоту(), img, src, color);
}

void Draw::DrawImage(int x, int y, const Рисунок& img, Цвет color)
{
	if(пусто_ли(color)) return;
	Размер sz = img.дайРазм();
	DrawImageOp(x, y, sz.cx, sz.cy, img, sz, color);
}

void Draw::DrawData(int x, int y, int cx, int cy, const Ткст& data, const char *тип)
{
	DrawDataOp(x, y, cx, cy, data, тип);
}

void Draw::DrawData(const Прям& r, const Ткст& data, const char *тип)
{
	DrawDataOp(r.left, r.top, r.дайШирину(), r.дайВысоту(), data, тип);
}

void Draw::DrawLine(Точка p1, Точка p2, int width, Цвет color)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y, width, color);
}

void Draw::DrawLine(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	DrawLineOp(x1, y1, x2, y2, width, ResolveInk(color));
}

void Draw::DrawPolyPolyline(const Точка *vertices, int vertex_count,
                            const int *counts, int count_count,
                            int width, Цвет color, Цвет doxor)
{
	DrawPolyPolylineOp(vertices, vertex_count, counts, count_count, width,
	                   ResolveInk(color), doxor);
}

void Draw::DrawPolyPolyline(const Вектор<Точка>& vertices, const Вектор<int>& counts,
                            int width, Цвет color, Цвет doxor)
{
	DrawPolyPolyline(vertices.старт(), vertices.дайСчёт(),
		             counts.старт(), counts.дайСчёт(),
		             width, color, doxor);
}

void Draw::DrawPolyline(const Точка *vertices, int count,
                        int width, Цвет color, Цвет doxor)
{
	DrawPolyPolyline(vertices, count, &count, 1, width, color, doxor);
}

void Draw::DrawPolyline(const Вектор<Точка>& vertices,
                        int width, Цвет color, Цвет doxor)
{
	DrawPolyline(vertices.старт(), vertices.дайСчёт(), width, color, doxor);
}

void Draw::DrawPolyPolyPolygon(const Точка *vertices, int vertex_count,
                              const int *subpolygon_counts, int subpolygon_count_count,
                              const int *disjunct_polygon_counts,
                              int disjunct_polygon_count_count, Цвет color,
                              int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	DrawPolyPolyPolygonOp(vertices, vertex_count, subpolygon_counts, subpolygon_count_count,
	                      disjunct_polygon_counts, disjunct_polygon_count_count, ResolveInk(color),
	                      width, outline, pattern, doxor);
}

void Draw::DrawPolyPolyPolygon(const Вектор<Точка>& vertices,
                           const Вектор<int>& subpolygon_counts,
                           const Вектор<int>& disjunct_polygon_counts,
                           Цвет color, int width, Цвет outline,
                           uint64 pattern, Цвет doxor)
{
	DrawPolyPolyPolygon(vertices.старт(), vertices.дайСчёт(),
	                    subpolygon_counts.старт(), subpolygon_counts.дайСчёт(),
	                    disjunct_polygon_counts.старт(), disjunct_polygon_counts.дайСчёт(),
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolyPolygon(const Точка *vertices, int vertex_count,
                       const int *subpolygon_counts, int subpolygon_count_count,
                       Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count,
	                    subpolygon_counts, subpolygon_count_count, &vertex_count, 1,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolyPolygon(const Вектор<Точка>& vertices, const Вектор<int>& subpolygon_counts,
                       Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	DrawPolyPolygon(vertices.старт(), vertices.дайСчёт(),
	                subpolygon_counts.старт(), subpolygon_counts.дайСчёт(),
	                color, width, outline, pattern, doxor);
}

void Draw::DrawPolygons(const Точка *vertices, int vertex_count,
                    const int *polygon_counts, int polygon_count_count,
                    Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count,
	                    polygon_counts, polygon_count_count,
	                    polygon_counts, polygon_count_count,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolygons(const Вектор<Точка>& vertices, const Вектор<int>& polygon_counts,
                    Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	DrawPolygons(vertices.старт(), vertices.дайСчёт(),
                 polygon_counts.старт(), polygon_counts.дайСчёт(),
                 color, width, outline, pattern, doxor);
}

void Draw::DrawPolygon(const Точка *vertices, int vertex_count,
                   Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count, &vertex_count, 1, &vertex_count, 1,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolygon(const Вектор<Точка>& vertices,
                       Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	DrawPolygon(vertices.старт(), vertices.дайСчёт(), color, width, outline, pattern, doxor);
}

void Draw::DrawEllipse(int x, int y, int cx, int cy, Цвет color, int pen, Цвет pencolor)
{
	DrawEllipse(RectC(x, y, cx, cy), color, pen, pencolor);
}

void Draw::DrawEllipse(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	DrawEllipseOp(r, ResolveInk(color), pen, ResolveInk(pencolor));
}

void Draw::DrawArc(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	DrawArcOp(rc, start, end, width, ResolveInk(color));
}

void Draw::смещение(int x, int y)
{
	смещение(Точка(x, y));
}

bool Draw::Clip(int x, int y, int cx, int cy)
{
	return Clip(RectC(x, y, cx, cy));
}

bool Draw::Clipoff(int x, int y, int cx, int cy)
{
	return Clipoff(RectC(x, y, cx, cy));
}

bool Draw::ExcludeClip(int x, int y, int cx, int cy)
{
	return ExcludeClip(RectC(x, y, cx, cy));
}

bool Draw::IntersectClip(int x, int y, int cx, int cy)
{
	return IntersectClip(RectC(x, y, cx, cy));
}

bool Draw::IsPainting(int x, int y, int cx, int cy) const
{
	return IsPainting(RectC(x, y, cx, cy));
}

static void  (*sIgfn)(ImageBuffer& ib, const Painting& pw, Размер sz, Точка pos, int mode);
static void  (*sIwfn)(ImageBuffer& ib, const Чертёж& p, int mode);
static Рисунок (*sRG)(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz);

void RegisterPaintingFns__(void (*ig)(ImageBuffer& ib, const Painting& pw, Размер sz, Точка pos, int mode),
                           void (*iw)(ImageBuffer& ib, const Чертёж& p, int mode),
                           Рисунок (*rg)(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz))
{
	sIgfn = ig;
	sIwfn = iw;
	sRG = rg;
}

bool HasPainter()
{
	return sIgfn && sIwfn && sRG;
}

Рисунок RenderGlyphByPainter(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz)
{
	return sRG ? (*sRG)(at, angle, chr, fnt, color, sz) : Рисунок();
}

void PaintImageBuffer(ImageBuffer& ib, const Painting& p, Размер sz, Точка pos, int mode)
{
	if(sIgfn)
		(*sIgfn)(ib, p, sz, pos, mode);
}

void PaintImageBuffer(ImageBuffer& ib, const Painting& p, int mode)
{
	PaintImageBuffer(ib, p, ib.дайРазм(), Точка(0, 0), mode);
}

void PaintImageBuffer(ImageBuffer& ib, const Чертёж& iw, int mode)
{
	if(sIwfn)
		(*sIwfn)(ib, iw, mode);
}

Чертёж AsDrawing(const Painting& pw)
{
	Размер sz = pw.дайРазм();
	DrawingDraw dw(sz);
	dw.DrawPainting(sz, pw);
	return dw.дайРез();
}

void Draw::DrawPaintingOp(const Прям& target, const Painting& pw)
{
	if(!HasPainter())
		return;
	Размер sz = target.дайРазм();
	if(sz.cx > 2000 || sz.cy > 1500) {
		int yy = 0;
		while(yy < sz.cy) {
			int ccy = min(sz.cy - yy, 100);
			ImageBuffer ib(sz.cx, ccy);
			Fill(~ib, IsPrinter() ? белый() : Null, ib.дайДлину());
			PaintImageBuffer(ib, pw, sz, Точка(0, yy), MODE_ANTIALIASED);
			DrawImageBandRLE(*this, target.left, target.top + yy, ib, 16);
			yy += ccy;
		}
	}
	else {
		ImageBuffer ib(sz);
		Fill(~ib, IsPrinter() ? белый() : Null, ib.дайДлину());
		PaintImageBuffer(ib, pw, sz, Точка(0, 0), MODE_ANTIALIASED);
		DrawImage(target.left, target.top, ib);
	}
}

void Draw::DrawPainting(int x, int y, int cx, int cy, const Painting& ig)
{
	DrawPainting(RectC(x, y, cx, cy), ig);
}

void Draw::DrawPainting(int x, int y, const Painting& iw)
{
	Размер sz = iw.дайРазм();
	DrawPainting(RectC(x, y, sz.cx, sz.cy), iw);
}

// ---------------------------

dword NilDraw::GetInfo() const { return DOTS; }
Размер NilDraw::GetPageSize() const { return Размер(0, 0); }
void NilDraw::BeginOp() {}
void NilDraw::EndOp() {}
void NilDraw::OffsetOp(Точка p) {}
bool NilDraw::ClipOp(const Прям& r) { return false; }
bool NilDraw::ClipoffOp(const Прям& r) { return false; }
bool NilDraw::ExcludeClipOp(const Прям& r) { return false; }
bool NilDraw::IntersectClipOp(const Прям& r) { return false; }
bool NilDraw::IsPaintingOp(const Прям& r) const { return false; }
Прям NilDraw::GetPaintRect() const { return Прям(0, 0, 0, 0); }
void NilDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color) {}
void NilDraw::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color) {}
void NilDraw::DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id) {}
void NilDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color) {}
void NilDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts, int count_count, int width, Цвет color, Цвет doxor) {}
void NilDraw::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor) {}
void NilDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color) {}
void NilDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor) {}
void NilDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx) {}
void NilDraw::DrawDrawingOp(const Прям& target, const Чертёж& w) {}
void NilDraw::DrawPaintingOp(const Прям& target, const Painting& w) {}

dword DrawProxy::GetInfo() const
{
	return ptr->GetInfo();
}

Размер DrawProxy::GetPageSize() const
{
	return ptr->GetPageSize();
}

void DrawProxy::StartPage()
{
	ptr->StartPage();
}

void DrawProxy::EndPage()
{
	ptr->EndPage();
}

void DrawProxy::BeginOp()
{
	ptr->BeginOp();
}

void DrawProxy::EndOp()
{
	ptr->EndOp();
}

void DrawProxy::OffsetOp(Точка p)
{
	ptr->OffsetOp(p);
}

bool DrawProxy::ClipOp(const Прям& r)
{
	return ptr->ClipOp(r);
}

bool DrawProxy::ClipoffOp(const Прям& r)
{
	return ptr->ClipoffOp(r);
}

bool DrawProxy::ExcludeClipOp(const Прям& r)
{
	return ptr->ExcludeClipOp(r);
}

bool DrawProxy::IntersectClipOp(const Прям& r)
{
	return ptr->IntersectClipOp(r);
}

bool DrawProxy::IsPaintingOp(const Прям& r) const
{
	return ptr->IsPaintingOp(r);
}

Прям DrawProxy::GetPaintRect() const
{
	return ptr->GetPaintRect();
}

void DrawProxy::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	ptr->DrawRectOp(x, y, cx, cy, color);
}

void DrawProxy::SysDrawImageOp(int x, int y, const Рисунок& img, Цвет color)
{
	ptr->SysDrawImageOp(x, y, img, color);
}

void DrawProxy::SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color)
{
	ptr->SysDrawImageOp(x, y, img, src, color);
}

void DrawProxy::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color)
{
	ptr->DrawImageOp(x, y, cx, cy, img, src, color);
}

void DrawProxy::DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id)
{
	ptr->DrawDataOp(x, y, cx, cy, data, id);
}

void DrawProxy::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	ptr->DrawLineOp(x1, y1, x2, y2, width, color);
}

void DrawProxy::DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts,
                                   int count_count, int width, Цвет color, Цвет doxor)
{
	ptr->DrawPolyPolylineOp(vertices, vertex_count, counts, count_count, width, color, doxor);
}

void DrawProxy::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count, const int *subpolygon_counts,
                                      int scc, const int *disjunct_polygon_counts, int dpcc, Цвет color,
                                      int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	ptr->DrawPolyPolyPolygonOp(vertices, vertex_count, subpolygon_counts, scc, disjunct_polygon_counts,
	                           dpcc, color, width, outline, pattern, doxor);
}

void DrawProxy::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	ptr->DrawArcOp(rc, start, end, width, color);
}

void DrawProxy::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	ptr->DrawEllipseOp(r, color, pen, pencolor);
}

void DrawProxy::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx)
{
	ptr->DrawTextOp(x, y, angle, text, font, ink, n, dx);
}

void DrawProxy::DrawDrawingOp(const Прям& target, const Чертёж& w)
{
	ptr->DrawDrawingOp(target, w);
}

void DrawProxy::DrawPaintingOp(const Прям& target, const Painting& w)
{
	ptr->DrawPaintingOp(target, w);
}

Размер DrawProxy::GetNativeDpi() const
{
	return ptr->GetNativeDpi();
}

void DrawProxy::BeginNative()
{
	ptr->BeginNative();
}

void DrawProxy::EndNative()
{
	ptr->EndNative();
}

int DrawProxy::GetCloffLevel() const
{
	return ptr->GetCloffLevel();
}

void DrawProxy::Escape(const Ткст& data)
{
	ptr->Escape(data);
}

}
