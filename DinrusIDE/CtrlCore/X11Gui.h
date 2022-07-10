#define GUI_X11
#define PLATFORM_X11 // To keep legacy code happy

#define Время    XTime
#define Шрифт    XFont
#define Дисплей XDisplay
#define Picture XPicture

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xinerama.h>

#undef Picture
#undef Время
#undef Шрифт
#undef Дисплей

#undef True
#undef False

#define XFalse 0
#define XTrue  1

namespace РНЦП {

extern XDisplay   *Xdisplay;
extern Visual     *Xvisual;
extern int         Xscreenno;
extern Window      Xroot;
extern Screen     *Xscreen;
extern Colormap    Xcolormap;
extern int         Xheight;
extern int         Xwidth;
extern int         XheightMM;
extern int         XwidthMM;
extern int         Xdepth;
extern dword       Xblack;
extern dword       Xwhite;
extern int         Xconnection;

extern dword   (*Xgetpixel)(int r, int g, int b);

void          InitX11Draw(const char *dispname = NULL);
void          InitX11Draw(XDisplay *дисплей);

void   XError();
void   XError(const char *s);

inline dword GetXPixel(int r, int g, int b) { return (*Xgetpixel)(r, g, b); }
inline dword GetXPixel(Цвет color)         { return (*Xgetpixel)(color.дайК(), color.дайЗ(), color.дайС()); }

enum {
	X11_ROP2_ZERO,
	X11_ROP2_AND,
	X11_ROP2_AND_NOT,
	X11_ROP2_COPY,
	X11_ROP2_NOT_AND,
	X11_ROP2_NOP,
	X11_ROP2_XOR,
	X11_ROP2_OR,
	X11_ROP2_NOT_AND_NOT,
	X11_ROP2_NOT_XOR,
	X11_ROP2_INVERT,
	X11_ROP2_OR_NOT,
	X11_ROP2_NOT_COPY,
	X11_ROP2_NOT_OR,
	X11_ROP2_NOT_OR_NOT,
	X11_ROP2_ONE,
};

void SetClip(GC gc, XftDraw *xftdraw, const Вектор<Прям>& cl);

class SystemDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Размер  GetPageSize() const;

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
	virtual void SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color);
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
	
	virtual Размер GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();

	virtual int  GetCloffLevel() const;

private:
	Размер    pageSize;

	SystemDraw();

	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Шрифт;
	friend class  TopFrameDraw;

	friend void StaticExitDraw_();
	friend Шрифт StdFont();

	Точка        actual_offset;
	Точка        actual_offset_bak;
	struct Cloff : Движимое<Cloff> {
		Точка org;
		int clipi;
		int offseti;
	};

	Массив<Cloff> cloff;
	Прям         drawingclip;
	Вектор< Вектор<Прям> > clip;
	Вектор<Точка> offset;

	Drawable   dw;
	GC         gc;
	XftDraw   *xftdraw;

	int        foreground;
	int        linewidth;

	void       иниц();
	void       иниц(const Вектор<Прям>& clip, Точка offset = Точка(0, 0));
	void       CloneClip();

	friend class BackDraw;
	friend class ScreenDraw;

public:
	static void слей()                                 { XSync(Xdisplay, false); }
	static Рисунок X11Cursor(int c);
	static void *CursorX11(const Рисунок& img);

	Точка    дайСмещ() const                          { return actual_offset; }

	static XftFont *CreateXftFont(Шрифт f, int angle);
	XftDraw *GetXftDraw() const               { return xftdraw; }

	Прям  GetClip() const;

	void  устПП(Цвет color);
	void  SetLineStyle(int width);
	void  SetClip();

	Drawable GetDrawable() const              { return dw; }
	GC       GetGC() const                    { return gc; }
	const Вектор<Прям>& GetClipList() const   { return clip.верх(); }

	SystemDraw(Drawable dw, GC gc, XftDraw *xftdraw, const Вектор<Прям>& clip);
	
	bool CanSetSurface()                      { return true; }
};

Рисунок X11Cursor(int c);
void *CursorX11(const Рисунок& img);

class BackDraw : public SystemDraw {
public:
	virtual bool  IsPaintingOp(const Прям& r) const;

protected:
	Pixmap  pixmap;
	Размер    size;
	Draw   *painting;
	Точка   painting_offset;

public:
	void  помести(SystemDraw& w, int x, int y);
	void  помести(SystemDraw& w, Точка p)                  { помести(w, p.x, p.y); }

	void создай(SystemDraw& w, int cx, int cy);
	void создай(SystemDraw& w, Размер sz)                { создай(w, sz.cx, sz.cy); }
	void разрушь();

	void SetPaintingDraw(Draw& w, Точка off)           { painting = &w; painting_offset = off; }

	BackDraw();
	~BackDraw();
};

class ImageDraw : public SystemDraw {
	Размер       size;
	SystemDraw alpha;
	bool       has_alpha;

	void иниц();
	Рисунок дай(bool pm) const;

public:
	Draw& Alpha();                       

	operator Рисунок() const;
	
	Рисунок GetStraight() const;
	
	ImageDraw(Размер sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
};

}

#include "X11/keysym.h"


#define GUIPLATFORM_KEYCODES_INCLUDE "X11Keys.h"


#define GUIPLATFORM_CTRL_TOP_DECLS \
	Window         window;


#define GUIPLATFORM_CTRL_DECLS_INCLUDE "X11Ctrl.h"

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE "X11Top.h"

#define GUIPLATFORM_PASTECLIP_DECLS \
	int          тип;

namespace РНЦП {

Ткст XAtomName(Atom atom);
Atom   XAtom(const char *имя);

Ткст      GetProperty(Window w, Atom property, Atom rtype = AnyPropertyType);
Вектор<int> GetPropertyInts(Window w, Atom property, Atom rtype = AnyPropertyType);
Ткст      ReadPropertyData(Window w, Atom property, Atom rtype = AnyPropertyType);

Индекс<Atom>& _NET_Supported();

#include "stdids.h"

#define GUIPLATFORM_INCLUDE_AFTER "X11GuiA.h"

#define HAS_TopFrameDraw

}
