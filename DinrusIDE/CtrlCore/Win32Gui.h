#define GUI_WIN
#define GUI_WIN32

namespace РНЦП {

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
	Размер  pageSize;
	Размер  nativeSize;
	Размер  nativeDpi;
	bool  palette:1;
	bool  color16:1;
	bool  is_mono:1;
	int   native;

	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Шрифт;

	friend void StaticExitDraw_();

	Точка     actual_offset_bak;

	struct Cloff : Движимое<Cloff> {
		Точка org;
		HRGN  hrgn;
		Прям  drawingclip;
	};

	Массив<Cloff> cloff;
	Прям         drawingclip;

	COLORREF  lastTextColor;
	Цвет     lastColor;
	HBRUSH    orgBrush;
	HBRUSH    actBrush;
	HPEN      orgPen;
	HPEN      actPen;
	int       lastPen;
	Цвет     lastPenColor;

	void   Unselect0();
	void   Cinit();

	void   LoadCaps();
	void   SetPrinterMode();
	void   переустанов();
	void   SetOrg();
	friend HPALETTE GetQlibPalette();
	void   DotsMode();

	static void      InitColors();
	
	friend class BackDraw;
	friend class ScreenDraw;
	friend class PrintDraw;

protected:
	dword style;
	HDC   handle, dcMem;
	Точка actual_offset;

	SystemDraw();
	void   иниц();
	void   InitClip(const Прям& clip);

public:
	static Прям GetVirtualScreenArea();

	static void SetAutoPalette(bool ap);
	static bool AutoPalette();
	bool PaletteMode()                                  { return palette; }

	static void слей()                                 { GdiFlush(); }

	static Рисунок Win32IconCursor(LPCSTR id, int iconsize, bool cursor);
	static HICON IconWin32(const Рисунок& img, bool cursor = false);

	COLORREF дайЦвет(Цвет color) const;
	
	Точка    дайСмещ() const                          { return actual_offset; }

#ifndef PLATFORM_WINCE
	Точка LPtoDP(Точка p) const;
	Точка DPtoLP(Точка p) const;
	Прям  LPtoDP(const Прям& r) const;
	Прям  DPtoLP(const Прям& r) const;
#endif

	void устЦвет(Цвет color);
	void SetDrawPen(int width, Цвет color);

	Размер  GetSizeCaps(int i, int j) const;
	HDC   BeginGdi();
	void  EndGdi();
	HDC   дайУк()                    { return handle; }
	operator HDC() const                 { return handle; }
	void  Unselect();
	void  прикрепи(HDC ahandle);
	HDC   открепи();
	
	HDC   GetCompatibleDC()              { return dcMem; }

	SystemDraw(HDC hdc);
	virtual ~SystemDraw();
	
	bool CanSetSurface()                 { return Pixels(); }
};

#ifndef PLATFORM_WINCE
class ВинМетаФайл {
	Размер size;
	HENHMETAFILE hemf;

	void     иниц();

public:
	void         прикрепи(HENHMETAFILE emf);
	HENHMETAFILE открепи();
	
	void     уст(const void *data, dword len);
	void     уст(const Ткст& data)        { уст(~data, data.дайСчёт()); }
	
	Ткст   дай() const;

	operator bool() const                   { return hemf; }
	void     устРазм(const Размер& sz)        { size = sz; }
	Размер     дайРазм() const                { return hemf ? size : Размер(0, 0); }

	void     очисть();

	void     рисуй(Draw& w, const Прям& r) const;
	void     рисуй(Draw& w, int x, int y, int cx, int cy) const;

	void     сериализуй(Поток& s);

	void     ReadClipboard();
	void     WriteClipboard() const;
	void     грузи(const char *file)         { уст(загрузиФайл(file)); }

	ВинМетаФайл()                           { иниц(); }
	ВинМетаФайл(HENHMETAFILE hemf);
	ВинМетаФайл(HENHMETAFILE hemf, Размер sz);
	ВинМетаФайл(const char *file);
	ВинМетаФайл(void *data, int len);
	ВинМетаФайл(const Ткст& data);

	~ВинМетаФайл()                                { очисть(); }

	HENHMETAFILE GetHEMF() const                  { return hemf; }
};

class WinMetaFileDraw : public SystemDraw {
	Размер size;

public:
	bool        создай(HDC hdc, int cx, int cy, const char *app = NULL, const char *имя = NULL, const char *file = NULL);
	bool        создай(int cx, int cy, const char *app = NULL, const char *имя = NULL, const char *file = NULL);
	ВинМетаФайл закрой();

	WinMetaFileDraw() {}
	WinMetaFileDraw(HDC hdc, int cx, int cy, const char *app = NULL, const char *имя = NULL, const char *file = NULL);
	WinMetaFileDraw(int cx, int cy, const char *app = NULL, const char *имя = NULL, const char *file = NULL);
	~WinMetaFileDraw();
};

void DrawWMF(Draw& w, int x, int y, int cx, int cy, const Ткст& wmf);
void DrawWMF(Draw& w, int x, int y, const Ткст& wmf);
Чертёж LoadWMF(const char *path, int cx, int cy);
Чертёж LoadWMF(const char *path);

Ткст  AsWMF(const Чертёж& iw);

#endif

class ScreenDraw : public SystemDraw {
public:
	ScreenDraw(bool ic = false);
	~ScreenDraw();
};

#ifndef PLATFORM_WINCE
class PrintDraw : public SystemDraw {
public:
	virtual void StartPage();
	virtual void EndPage();

private:
	bool aborted;
	
	void  InitPrinter();
public:
	PrintDraw(HDC hdc, const char *jobname);
	~PrintDraw();
};
#endif

inline bool     BitBlt(HDC ddc, Точка d, HDC sdc, const Прям& s, dword rop = SRCCOPY)
{ return BitBlt(ddc, d.x, d.y, s.устШирину(), s.устВысоту(), sdc, s.left, s.top, rop); }

inline bool     StretchBlt(HDC ddc, const Прям& r, HDC sdc, const Прям& s, dword rop = SRCCOPY)
{ return StretchBlt(ddc, r.left, r.top, r.устШирину(), r.устВысоту(), sdc, s.left, s.top, s.устШирину(), s.устВысоту(), rop); }

inline bool     PatBlt(HDC dc, const Прям& r, dword rop = PATCOPY)
{ return PatBlt(dc, r.left, r.top, r.устШирину(), r.устВысоту(), rop); }

inline void     MoveTo(HDC hdc, Точка pt)                         { MoveToEx(hdc, pt.x, pt.y, 0); }
inline void     LineTo(HDC hdc, Точка pt)                         { LineTo(hdc, pt.x, pt.y); }

inline void     DrawLine(HDC hdc, Точка p, Точка q)               { MoveTo(hdc, p); LineTo(hdc, q); }
inline void     DrawLine(HDC hdc, int px, int py, int qx, int qy) { MoveToEx(hdc, px, py, 0); LineTo(hdc, qx, qy); }

#ifndef PLATFORM_WINCE
inline void     DrawArc(HDC hdc, const Прям& rc, Точка p, Точка q){ Arc(hdc, rc.left, rc.top, rc.right, rc.bottom, p.x, p.y, q.x, q.y); }
#endif
inline void     DrawCircle(HDC hdc, int x, int y, int radius)     { Ellipse(hdc, x - radius, y - radius, x + radius + 1, y + radius + 1); }
inline void     DrawCircle(HDC hdc, Точка centre, int radius)     { DrawCircle(hdc, centre.x, centre.y, radius); }
inline void     DrawEllipse(HDC hdc, const Прям& rc)              { Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom); }

inline void     DrawRect(HDC hdc, const Прям& rc)                 { Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); }

HDC      ScreenHDC();
HPALETTE GetQlibPalette();

Рисунок Win32Icon(LPCSTR id, int iconsize = 0);
Рисунок Win32Icon(int id, int iconsize = 0);
Рисунок Win32Cursor(LPCSTR id);
Рисунок Win32Cursor(int id);
Рисунок Win32DllIcon(const char *dll, int ii, bool large);

class BackDraw : public SystemDraw {
public:
	virtual bool  IsPaintingOp(const Прям& r) const;

protected:
	HBITMAP hbmpold;
	HBITMAP hbmp;

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
	Размер    size;

	struct  Section {
		HDC     dc;
		HBITMAP hbmp, hbmpOld;
		КЗСА   *pixels;

		void иниц(int cx, int cy);
		~Section();
	};

	Section     rgb;
	Section     a;
	SystemDraw  alpha;


	bool    has_alpha;

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

#define GUIPLATFORM_KEYCODES_INCLUDE "Win32Keys.h"


#define GUIPLATFORM_CTRL_TOP_DECLS \
	HWND           hwnd; \
	ЦельЮБроса   *dndtgt; \


#define GUIPLATFORM_CTRL_DECLS_INCLUDE "Win32Ctrl.h"


#define GUIPLATFORM_PASTECLIP_DECLS \
	ЦельЮБроса *dt; \

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE "Win32Top.h"

namespace РНЦП {

inline hash_t дайХэшЗнач(const HWND& hwnd)
{
	return (hash_t)(intptr_t)hwnd;
}
}

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

#include <shellapi.h>

#endif
#endif

#define GUIPLATFORM_INCLUDE_AFTER "Win32GuiA.h"

#define HAS_TopFrameDraw
