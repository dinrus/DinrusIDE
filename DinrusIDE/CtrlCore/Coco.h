#define GUI_COCOA

namespace РНЦП {

struct PointCG; // represents CGPoint, used to isolate Cocoa/AppKit includes
struct RectCG; // represents CGRect, used to isolate Cocoa/AppKit includes

class SystemDraw : public Draw {
	virtual dword GetInfo() const;

	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Точка p);
	virtual bool ClipOp(const Прям& r);
	virtual bool ClipoffOp(const Прям& r);
	virtual bool ExcludeClipOp(const Прям& r);
	virtual bool IntersectClipOp(const Прям& r);
	virtual bool IsPaintingOp(const Прям& r) const;
	virtual Прям GetPaintRect() const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Цвет color);
	virtual void SysDrawImageOp(int x, int y, const Рисунок& img, Цвет color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color);

	virtual void DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Цвет color, Цвет doxor);
	virtual void DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Цвет color, int width, Цвет outline,
	                                   uint64 pattern, Цвет doxor);
	virtual void DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color);

	virtual void DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font,
		                    Цвет ink, int n, const int *dx);

private:
	Вектор<Точка> offset;
	Вектор<Прям>  clip;

	Цвет  fill = Null;

	void   сунь();
	void   вынь();
	
	void  *handle;
	void  *nsview;
	
	RectCG MakeRectCG(const Прям& r) const;
	void   ClipCG(const Прям& r);
	Прям   GetClip() const         { return clip.дайСчёт() ? clip.верх() : Прям(-999999, -999999, 999999, 999999); }
	RectCG Преобр(int x, int y, int cx, int cy);
	RectCG Преобр(const Прям& r);
	PointCG Преобр(int x, int y);

	void  уст(Цвет c);
	void  SetStroke(Цвет c);

	void иниц(void *cgContext, void *nsview);

	void Stroke(int width, Цвет color, bool fill);
	void делайПуть(const Точка *pp, const Точка *end);

	SystemDraw() {}
	
	friend class ImageDraw;
	friend class BackDraw;
	friend class TopFrameDraw;
	friend struct BackDraw__;

public:
	Точка    дайСмещ() const       { return offset.дайСчёт() ? offset.верх() : Точка(0, 0); }
	void    *GetCGHandle() const      { return handle; }

	bool     CanSetSurface()          { return false; }
	static void слей() {}

	SystemDraw(void *cgContext, void *nsview);
	~SystemDraw();
};

 
inline void устПоверхность(SystemDraw& w, const Прям& dest, const КЗСА *pixels, Размер srcsz, Точка poff)
{ // TODO: Unless we can do this...
	NEVER();
}

class ImageDraw : public SystemDraw {
	ImageBuffer ib;
	
	Один<ImageDraw> alpha;

	void иниц(int cx, int cy);

public:
	Draw& Alpha();

	operator Рисунок() const;

	Рисунок GetStraight() const;

	ImageDraw(Размер sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
};

struct BackDraw__ : public SystemDraw {
	BackDraw__() : SystemDraw() {}
};

class BackDraw : public BackDraw__ { // Dummy only, as we are running in GlobalBackBuffer mode
	Размер        size;
	Draw       *painting;
	Точка       painting_offset;
	ImageBuffer ib;
	
public:
	virtual bool  IsPaintingOp(const Прям& r) const;

public:
	void  помести(SystemDraw& w, int x, int y)             {}
	void  помести(SystemDraw& w, Точка p)                  { помести(w, p.x, p.y); }

	void создай(SystemDraw& w, int cx, int cy)         {}
	void создай(SystemDraw& w, Размер sz)                { создай(w, sz.cx, sz.cy); }
	void разрушь()                                     {}

	void SetPaintingDraw(Draw& w, Точка off)           { painting = &w; painting_offset = off; }

	Точка дайСмещ() const                            { return Точка(0, 0); }

	BackDraw();
	~BackDraw();
};

Рисунок GetIconForFile(const char *path);

#ifndef PLATFORM_WIN32
#include <CtrlCore/stdids.h>
#endif

#include "CocoCode.h"

#define GUIPLATFORM_KEYCODES_INCLUDE <CtrlCore/CocoKeys.h>

struct CocoTop;
struct MMCtrl;
struct MMImp;

#define GUIPLATFORM_CTRL_TOP_DECLS \
	CocoTop *coco = NULL; \

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <CtrlCore/CocoCtrl.h>

#define GUIPLATFORM_PASTECLIP_DECLS \
void *nspasteboard; \
friend struct MMImp;

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <CtrlCore/CocoTop.h>

};

#define GUIPLATFORM_INCLUDE_AFTER <CtrlCore/CocoAfter.h>

#define HAS_TopFrameDraw
