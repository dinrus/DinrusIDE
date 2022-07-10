#define GUI_GTK

#define PLATFORM_X11 // To keep legacy code happy

#define GDKEY(x) GDK_KEY_##x

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace РНЦП {

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
	void  RectPath(const Прям& r);
	void  сунь();
	void  вынь();
	Вектор<Точка> offset;
	Вектор<Прям>  clip;
	Вектор<Прям>  invalid; // for IsPainting checks, if empty, only clip extents is checked

	cairo_t      *cr;
	
	struct TextGlyph : Движимое<TextGlyph> {
		int x;
		int y;
		int Индекс;
	};

	Шрифт              textfont;
	Цвет             textink;
	int               textangle;
	Вектор<TextGlyph> textcache;
	
	SystemDraw() {}
	
	friend class ImageDraw;
	friend class BackDraw;
	friend class TopFrameDraw;

	Прям     GetClip() const;
	void     FlushText();
	
	friend void устПоверхность(SystemDraw& w, const Прям& dest, const КЗСА *pixels, Размер srcsz, Точка poff);

public:
	void  устЦвет(Цвет c);
	operator cairo_t*()               { return cr; }

	void     PickInvalid(Вектор<Прям>&& inv)   { invalid = pick(inv); }
	Точка    дайСмещ() const;
	bool     CanSetSurface()          { return true; }
	static void слей()               {} // TODO?

//	SystemDraw(cairo_t *cr, GdkDrawable *dw/* = NULL*/) : cr(cr), drawable(dw) { (void)drawable; invalid = NULL; }
	SystemDraw(cairo_t *cr) : cr(cr) {}
	~SystemDraw();
};

class ImageDraw : public SystemDraw {
	cairo_surface_t *surface;
	Размер             isz;
	
	SystemDraw       alpha;
	cairo_surface_t *alpha_surface;
	bool             del;

	void иниц(Размер sz);
	void FetchStraight(ImageBuffer& b) const;

public:
	Draw& Alpha();

	operator Рисунок() const;

	Рисунок GetStraight() const;

	ImageDraw(Размер sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();

	ImageDraw(cairo_t *cr, Размер sz); // особый variant for chameleon skinning
};

class BackDraw : public SystemDraw {
	Размер        size;
	Draw       *painting;
	Точка       painting_offset;
	
public:
	virtual bool  IsPaintingOp(const Прям& r) const;

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

struct GdkRect : GdkRectangle {
	operator GdkRectangle *() { return this; }
	GdkRect(const Прям& r);
};

class ImageGdk {
	GdkPixbuf *pixbuf;
	Рисунок      img;

	void освободи();

public:
	operator GdkPixbuf *()    { return pixbuf; }

	bool уст(const Рисунок& m);
	          
	ImageGdk();
	ImageGdk(const Рисунок& m);
	~ImageGdk();
};

Ткст FilesClipFromUrisFree(gchar **uris);
Ткст ImageClipFromPixbufUnref(GdkPixbuf *pixbuf);

GdkAtom GAtom(const Ткст& id);

#ifdef GDK_WINDOWING_X11
Вектор<int> GetPropertyInts(GdkWindow *w, const char *property);
#endif

#ifndef PLATFORM_WIN32
#include <CtrlCore/stdids.h>
#endif

#define GUIPLATFORM_KEYCODES_INCLUDE <CtrlCore/GtkKeys.h>

//$	struct Ктрл::верх {
#define GUIPLATFORM_CTRL_TOP_DECLS \
	GtkWidget            *window; \
	GtkIMContext         *im_context; \
	GtkIMContext         *im_context_simple; \
	GtkIMContext         *im_context_multi; \
	int64                 cursor_id; \
	int                   id; \
//$ }

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <CtrlCore/GtkCtrl.h>

#define GUIPLATFORM_PASTECLIP_DECLS \
int тип; \

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <CtrlCore/GtkTop.h>

#define GUIPLATFORM_NOSCROLL

int rmsecs();

}

#define GUIPLATFORM_INCLUDE_AFTER <CtrlCore/GtkAfter.h>

#define HAS_TopFrameDraw
