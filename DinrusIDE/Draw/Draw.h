#ifndef DRAW_H
#define DRAW_H

#define SYSTEMDRAW 1

#include <Core/Core.h>

#ifdef flagCFONTS
#define CUSTOM_FONTSYS
#endif

namespace РНЦП {

class Чертёж;
class Draw;
class Painting;
class SystemDraw;
class ImageDraw;

#include "Image.h"

const int FONT_V = 40;

struct FontGlyphConsumer {
	virtual void Move(ТочкаПЗ p) = 0;
	virtual void Строка(ТочкаПЗ p) = 0;
	virtual void Quadratic(ТочкаПЗ p1, ТочкаПЗ p2) = 0;
	virtual void Cubic(ТочкаПЗ p1, ТочкаПЗ p2, ТочкаПЗ p3) = 0;
	virtual void закрой() = 0;
};

#include "FontInt.h"

class FontInfo;


inline
bool PreferColorEmoji(int c)
{ // for these codepoints we prefer replacemnet color emoji even if glyphs is in the font
	return c >= 0x2600 && c <= 0x27ef || c >= 0x1f004 && c <= 0x1f251 || c >= 0x1f300 && c <= 0x1faf6;
}

class Шрифт : public ТипЗнач<Шрифт, FONT_V, Движимое<Шрифт> >{
	union {
		int64 data;
		struct {
			word  face;
			word  flags;
			int16 height;
			int16 width;
		} v;
	};
	
	enum {
		FONT_BOLD = 0x8000,
		FONT_ITALIC = 0x4000,
		FONT_UNDERLINE = 0x2000,
		FONT_STRIKEOUT = 0x1000,
		FONT_NON_ANTI_ALIASED = 0x800,
		FONT_TRUE_TYPE_ONLY = 0x400
	};

	static Шрифт AStdFont;
	static Размер StdFontSize;
	static bool std_font_override;

	static void SetStdFont0(Шрифт font);
	static Вектор<FaceInfo>& FaceList();
	static void SyncStdFont();
	static void InitStdFont();

	const CommonFontInfo& Fi() const;
	
	friend void   sInitFonts();
	friend Ткст GetFontDataSys(Шрифт font, const char *table, int offset, int size);
	
public:
	enum {
		FIXEDPITCH  = 0x0001,
		SCALEABLE   = 0x0002,
		TTF         = 0x0004,
		SPECIAL     = 0x0010,
		SERIFSTYLE  = 0x0020,
		SCRIPTSTYLE = 0x0040,
		COLORIMG    = 0x0080, // freetype color bitmap font (emojis)
	};

	static int    GetFaceCount();
	static Ткст GetFaceName(int Индекс);
	static int    FindFaceNameIndex(const Ткст& имя);
	static dword  GetFaceInfo(int Индекс);
	static void   SetFace(int Индекс, const Ткст& имя, dword info);
	static void   SetFace(int Индекс, const Ткст& имя);

	static void   SetDefaultFont(Шрифт font);
	static void   SetStdFont(Шрифт font);
	static Шрифт   GetStdFont();
	static Размер   GetStdFontSize();

	enum {
		STDFONT,
		SERIF,
		SANSSERIF,
		MONOSPACE,
	#ifdef PLATFORM_WIN32
		SYMBOL, // deprecated
		WINGDINGS, // deprecated
		TAHOMA, // deprecated
	#endif
		OTHER, // deprecated

	// Backward compatibility:
		ROMAN = SERIF,
		ARIAL = SANSSERIF,
		COURIER = MONOSPACE,
		SCREEN_SERIF = SERIF,
		SCREEN_SANS = SANSSERIF,
		SCREEN_FIXED = MONOSPACE,
	};
	
	int    GetFace() const          { return v.face; }
	int    дайВысоту() const;
	int    дайШирину() const         { return v.width; }
	bool   IsBold() const           { return v.flags & FONT_BOLD; }
	bool   IsItalic() const         { return v.flags & FONT_ITALIC; }
	bool   IsUnderline() const      { return v.flags & FONT_UNDERLINE; }
	bool   IsStrikeout() const      { return v.flags & FONT_STRIKEOUT; }
	bool   IsNonAntiAliased() const { return v.flags & FONT_NON_ANTI_ALIASED; } // deprecated
	bool   IsTrueTypeOnly() const   { return v.flags & FONT_TRUE_TYPE_ONLY; } // deprecated
	Ткст GetFaceName() const;
	Ткст GetFaceNameStd() const;
	dword  GetFaceInfo() const;
	int64  AsInt64() const          { return data; }
	
	void   RealizeStd();

	Шрифт& Face(int n)               { v.face = n; return *this; }
	Шрифт& устВысоту(int n)             { v.height = n; return *this; }
	Шрифт& устШирину(int n)              { v.width = n; return *this; }
	Шрифт& Bold()                    { v.flags |= FONT_BOLD; return *this; }
	Шрифт& NoBold()                  { v.flags &= ~FONT_BOLD; return *this; }
	Шрифт& Bold(bool b)              { return b ? Bold() : NoBold(); }
	Шрифт& Italic()                  { v.flags |= FONT_ITALIC; return *this; }
	Шрифт& NoItalic()                { v.flags &= ~FONT_ITALIC; return *this; }
	Шрифт& Italic(bool b)            { return b ? Italic() : NoItalic(); }
	Шрифт& Underline()               { v.flags |= FONT_UNDERLINE; return *this; }
	Шрифт& NoUnderline()             { v.flags &= ~FONT_UNDERLINE; return *this; }
	Шрифт& Underline(bool b)         { return b ? Underline() : NoUnderline(); }
	Шрифт& Strikeout()               { v.flags |= FONT_STRIKEOUT; return *this; }
	Шрифт& NoStrikeout()             { v.flags &= ~FONT_STRIKEOUT; return *this; }
	Шрифт& Strikeout(bool b)         { return b ? Strikeout() : NoStrikeout(); }
	Шрифт& NonAntiAliased()          { v.flags |= FONT_NON_ANTI_ALIASED; return *this; }
	Шрифт& NoNonAntiAliased()        { v.flags &= ~FONT_NON_ANTI_ALIASED; return *this; } // deprecated
	Шрифт& NonAntiAliased(bool b)    { return b ? NonAntiAliased() : NoNonAntiAliased(); } // deprecated
	Шрифт& TrueTypeOnly()            { v.flags |= FONT_TRUE_TYPE_ONLY; return *this; } // deprecated
	Шрифт& NoTrueTypeOnly()          { v.flags &= ~FONT_TRUE_TYPE_ONLY; return *this; } // deprecated
	Шрифт& TrueTypeOnly(bool b)      { return b ? TrueTypeOnly() : NoTrueTypeOnly(); } // deprecated

	Шрифт& FaceName(const Ткст& имя);

	Шрифт  operator()() const        { return *this; }
	Шрифт  operator()(int n) const   { return Шрифт(*this).устВысоту(n); }

	int   GetAscent() const                  { return Fi().ascent; }
	int   GetDescent() const                 { return Fi().descent; }
	int   GetCy() const                      { return GetAscent() + GetDescent(); }
	int   GetExternal() const                { return Fi().external; }
	int   GetInternal() const                { return Fi().internal; }
	int   GetLineHeight() const              { return GetCy() + GetExternal(); }
	int   GetOverhang() const                { return Fi().overhang; }
	int   GetAveWidth() const                { return Fi().avewidth; }
	int   GetMaxWidth() const                { return Fi().maxwidth; }
	int   GetMonoWidth() const               { return max(дайШирину('M'), дайШирину('W')); }
	bool  IsNormal(int ch) const;
	bool  IsComposed(int ch) const;
	bool  IsReplaced(int ch) const;
	bool  IsMissing(int ch) const;
	int   HasChar(int ch) const;
	int   дайШирину(int c) const;
	int   operator[](int c) const            { return дайШирину(c); }
	int   GetLeftSpace(int c) const;
	int   GetRightSpace(int c) const;
	bool  IsFixedPitch() const               { return Fi().fixedpitch; }
	bool  IsScaleable() const                { return Fi().scaleable; }
	bool  особый_ли() const                  { return GetFaceInfo() & SPECIAL; }
	bool  IsTrueType() const                 { return Fi().ttf; }
	bool  IsSerif() const                    { return GetFaceInfo() & SERIFSTYLE; }
	bool  IsScript() const                   { return GetFaceInfo() & SCRIPTSTYLE; }

	Ткст GetTextFlags() const;
	void   ParseTextFlags(const char *s);
	
	Ткст дайДанные(const char *table = NULL, int offset = 0, int size = INT_MAX) const;
	
	void   Render(FontGlyphConsumer& sw, double x, double y, int ch) const;

	void  сериализуй(Поток& s);
	void  вДжейсон(ДжейсонВВ& jio);
	void  вРяр(РярВВ& xio);

	bool  operator==(Шрифт f) const  { return v.face == f.v.face && v.flags == f.v.flags &&
	                                        v.width == f.v.width && v.height == f.v.height; }
	bool  operator!=(Шрифт f) const  { return !operator==(f); }

	hash_t дайХэшЗнач() const     { return комбинируйХэш(v.width, v.flags, v.height, v.face); }
	bool   экзПусто_ли() const   { return v.face == 0xffff; }
	void   устПусто()                { v.face = 0xffff; v.height = v.width = 0; v.flags = 0; }
	Шрифт()                          { v.height = v.width = 0; v.face = v.flags = 0; }
	Шрифт(int face, int height)      { v.face = face; v.height = height; v.flags = 0; v.width = 0; }
	Шрифт(const Обнул&)             { устПусто(); }

	operator Значение() const          { return богатыйВЗнач(*this); }
	Шрифт(const Значение& q)            { *this = q.дай<Шрифт>(); }

// BW compatibility
	FontInfo Info() const;
};


//BW compatibility
class FontInfo { // Obsolete!
	Шрифт font;
	friend class Шрифт;
public:
	int    GetAscent() const                  { return font.GetAscent(); }
	int    GetDescent() const                 { return font.GetDescent(); }
	int    GetExternal() const                { return font.GetExternal(); }
	int    GetInternal() const                { return font.GetInternal(); }
	int    дайВысоту() const                  { return font.GetCy(); }
	int    GetLineHeight() const              { return font.GetLineHeight(); }
	int    GetOverhang() const                { return font.GetOverhang(); }
	int    GetAveWidth() const                { return font.GetAveWidth(); }
	int    GetMaxWidth() const                { return font.GetMaxWidth(); }
	int    HasChar(int c) const               { return font.HasChar(c); }
	int    дайШирину(int c) const              { return font.дайШирину(c); }
	int    operator[](int c) const            { return дайШирину(c); }
	int    GetLeftSpace(int c) const          { return font.GetLeftSpace(c); }
	int    GetRightSpace(int c) const         { return font.GetRightSpace(c); }
	bool   IsFixedPitch() const               { return font.IsFixedPitch(); }
	bool   IsScaleable() const                { return font.IsScaleable(); }
	int    GetFontHeight() const              { return font.дайВысоту(); }
	Шрифт   дайШрифт() const                    { return font; }
};

struct ComposedGlyph {
	wchar  basic_char;
	Точка  mark_pos;
	wchar  mark_char;
	Шрифт   mark_font;
};

bool Compose(Шрифт fnt, int chr, ComposedGlyph& cs);

template<>
Ткст какТкст(const Шрифт& f);

inline void SetStdFont(Шрифт font)                   { Шрифт::SetStdFont(font); }
inline Шрифт GetStdFont()                            { return Шрифт::GetStdFont(); }
inline Размер GetStdFontSize()                        { return Шрифт::GetStdFontSize(); } // deprecated
inline int  GetStdFontCy()                          { return GetStdFontSize().cy; }

Шрифт StdFont();

inline Шрифт StdFont(int h)                          { return StdFont().устВысоту(h); }

inline Шрифт Serif(int n = -32000) { return Шрифт(Шрифт::SCREEN_SERIF, n); }
inline Шрифт SansSerif(int n = -32000) { return Шрифт(Шрифт::SCREEN_SANS, n); }
inline Шрифт Monospace(int n = -32000) { return Шрифт(Шрифт::SCREEN_FIXED, n); }

inline Шрифт Roman(int n = -32000) { return Шрифт(Шрифт::SCREEN_SERIF, n); } // deprecated
inline Шрифт Arial(int n = -32000) { return Шрифт(Шрифт::SCREEN_SANS, n); } // deprecated
inline Шрифт Courier(int n = -32000) { return Шрифт(Шрифт::SCREEN_FIXED, n); } // deprecated

inline Шрифт ScreenSerif(int n = -32000) { return Шрифт(Шрифт::SCREEN_SERIF, n); } // deprecated
inline Шрифт ScreenSans(int n = -32000) { return Шрифт(Шрифт::SCREEN_SANS, n); } // deprecated
inline Шрифт ScreenFixed(int n = -32000) { return Шрифт(Шрифт::SCREEN_FIXED, n); } // deprecated

#ifdef PLATFORM_WIN32 // backward comaptibility
inline Шрифт Tahoma(int n = -32000) { return Шрифт(Шрифт::TAHOMA, n); }
#endif

Размер дайРазмТекста(const wchar *text, Шрифт font, int n = -1);
Размер дайРазмТекста(const ШТкст& text, Шрифт font);
Размер дайРазмТекста(const char *text, byte charset, Шрифт font, int n = -1);
Размер дайРазмТекста(const char *text, Шрифт font, int n = -1);
Размер дайРазмТекста(const Ткст& text, Шрифт font);

enum {
	PEN_NULL = -1,
	PEN_SOLID = -2,
	PEN_DASH = -3,
#ifndef PLATFORM_WINCE
	PEN_DOT = -4,
	PEN_DASHDOT = -5,
	PEN_DASHDOTDOT = -6,
#endif
};

class Рисунок;

Цвет SBlack();
Цвет SGray();
Цвет SLtGray();
Цвет SWhiteGray();
Цвет SWhite();
Цвет SRed();
Цвет SGreen();
Цвет SBrown();
Цвет SBlue();
Цвет SMagenta();
Цвет SCyan();
Цвет SYellow();
Цвет SLtRed();
Цвет SLtGreen();
Цвет SLtYellow();
Цвет SLtBlue();
Цвет SLtMagenta();
Цвет SLtCyan();

Цвет SColorPaper();
Цвет SColorText();
Цвет SColorFace();
Цвет SColorHighlight();
Цвет SColorHighlightText();
Цвет SColorMenu();
Цвет SColorMenuText();
Цвет SColorInfo();
Цвет SColorInfoText();
Цвет SColorMark();
Цвет SColorMenuMark();
Цвет SColorDisabled();
Цвет SColorLight();
Цвет SColorLabel();
Цвет SColorShadow();

Цвет SColorLtFace();
Цвет SColorDkShadow();

void SBlack_Write(Цвет c);
void SGray_Write(Цвет c);
void SLtGray_Write(Цвет c);
void SWhiteGray_Write(Цвет c);
void SWhite_Write(Цвет c);
void SRed_Write(Цвет c);
void SGreen_Write(Цвет c);
void SBrown_Write(Цвет c);
void SBlue_Write(Цвет c);
void SMagenta_Write(Цвет c);
void SCyan_Write(Цвет c);
void SYellow_Write(Цвет c);
void SLtRed_Write(Цвет c);
void SLtGreen_Write(Цвет c);
void SLtYellow_Write(Цвет c);
void SLtBlue_Write(Цвет c);
void SLtMagenta_Write(Цвет c);
void SLtCyan_Write(Цвет c);

void SColorPaper_Write(Цвет c);
void SColorText_Write(Цвет c);
void SColorHighlight_Write(Цвет c);
void SColorHighlightText_Write(Цвет c);
void SColorMenu_Write(Цвет c);
void SColorMenuText_Write(Цвет c);
void SColorInfo_Write(Цвет c);
void SColorInfoText_Write(Цвет c);
void SColorMark_Write(Цвет c);
void SColorMenuMark_Write(Цвет c);
void SColorDisabled_Write(Цвет c);
void SColorLight_Write(Цвет c);
void SColorFace_Write(Цвет c);
void SColorLabel_Write(Цвет c);
void SColorShadow_Write(Цвет c);

void SColorLtFace_Write(Цвет c);
void SColorDkShadow_Write(Цвет c);

inline Цвет InvertColor() { return Цвет::особый(255); } // особый color that with DrawRect actually inverts the rectangle
inline Цвет DefaultInk() { return Цвет::особый(254); } // SColorText on screen, чёрный on other outputs

extern bool dark_theme__;

inline bool  IsDarkTheme()           { return dark_theme__; }

inline Цвет AdjustIfDark(Цвет c)   { return IsDarkTheme() ? тёмнаяТемаИзКэша(c) : c; }

Чертёж AsDrawing(const Painting& pw);

class Painting : public ТипЗнач<Painting, 48, Движимое<Painting> > {
	Ткст     cmd;
	МассивЗнач data;
	РазмерПЗ      size;
	
	friend class PaintingPainter;
	friend class Рисовало;

public:
	РазмерПЗ   дайРазм() const                     { return size; }

	void    очисть()                             { size = Null; data.очисть(); cmd.очисть(); }
	void    сериализуй(Поток& s)                { s % cmd % data % size; }
	void    вРяр(РярВВ& xio)                  { XmlizeBySerialize(xio, *this); }
	void    вДжейсон(ДжейсонВВ& jio)                { JsonizeBySerialize(jio, *this); }
	bool    экзПусто_ли() const              { return cmd.пустой(); }
	void    устПусто()                           { size = Null; }
	bool    operator==(const Painting& b) const { return cmd == b.cmd && data == b.data && size == b.size; }
	hash_t  дайХэшЗнач() const                { return комбинируйХэш(cmd, data); }
	Ткст  вТкст() const                    { return "painting " + какТкст(size); }

	operator Значение() const                      { return богатыйВЗнач(*this); }
	Painting(const Значение& q)                    { *this = q.дай<Painting>(); }

	Painting()                                  { устПусто(); }
	Painting(const Обнул&)                     { устПусто(); }
};

enum {
	MODE_ANTIALIASED = 0,
	MODE_NOAA        = 1,
	MODE_SUBPIXEL    = 2,
};

bool HasPainter();
void PaintImageBuffer(ImageBuffer& ib, const Painting& p, Размер sz, Точка pos, int mode = MODE_ANTIALIASED);
void PaintImageBuffer(ImageBuffer& ib, const Painting& p, int mode = MODE_ANTIALIASED);
void PaintImageBuffer(ImageBuffer& ib, const Чертёж& p, int mode = MODE_ANTIALIASED);

class Draw : БезКопий {
	struct DrawingPos;
	
public:
	enum {
		DOTS = 0x001,
		PRINTER = 0x004,
		NATIVE = 0x008,
		DATABANDS = 0x010,
		DRAWTEXTLINES = 0x020,
		DRAWING = 0x040,
	};

	virtual dword GetInfo() const = 0;

	virtual Размер GetPageSize() const;
	virtual void StartPage();
	virtual void EndPage();

	virtual void BeginOp() = 0;
	virtual void EndOp() = 0;
	virtual void OffsetOp(Точка p) = 0;
	virtual bool ClipOp(const Прям& r) = 0;
	virtual bool ClipoffOp(const Прям& r) = 0;
	virtual bool ExcludeClipOp(const Прям& r) = 0;
	virtual bool IntersectClipOp(const Прям& r) = 0;
	virtual bool IsPaintingOp(const Прям& r) const = 0;
	virtual Прям GetPaintRect() const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Цвет color) = 0;
	virtual void SysDrawImageOp(int x, int y, const Рисунок& img, Цвет color);
	virtual void SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color) = 0;

	virtual void DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Цвет color, Цвет doxor) = 0;
	virtual void DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Цвет color, int width, Цвет outline,
	                                   uint64 pattern, Цвет doxor) = 0;
	virtual void DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color) = 0;

	virtual void DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor) = 0;
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font,
		                    Цвет ink, int n, const int *dx) = 0;
	virtual void DrawDrawingOp(const Прям& target, const Чертёж& w);
	virtual void DrawPaintingOp(const Прям& target, const Painting& w);
	
	virtual Размер GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();

	virtual int  GetCloffLevel() const;
	
	virtual void Escape(const Ткст& data);

	virtual ~Draw();

// --------------
	Размер  GetPixelsPerInch() const;
	Размер  GetPageMMs() const;

	bool  Dots() const                                  { return GetInfo() & DOTS; }
	bool  Pixels() const                                { return !Dots(); }
	bool  IsPrinter() const                             { return GetInfo() & PRINTER; }
	bool  IsNative() const                              { return GetInfo() & NATIVE; }

	int  GetNativeX(int x) const;
	int  GetNativeY(int y) const;
	void Native(int& x, int& y) const;
	void Native(Точка& p) const;
	void Native(Размер& sz) const;
	void Native(Прям& r) const;

	void  старт()                                       { BeginOp(); }
	void  стоп()                                         { EndOp(); }
	void  смещение(Точка p)                               { OffsetOp(p); }
	void  смещение(int x, int y);
	bool  Clip(const Прям& r)                           { return ClipOp(r); }
	bool  Clip(int x, int y, int cx, int cy);
	bool  Clipoff(const Прям& r)                        { return ClipoffOp(r); }
	bool  Clipoff(int x, int y, int cx, int cy);
	bool  ExcludeClip(const Прям& r)                    { return ExcludeClipOp(r); }
	bool  ExcludeClip(int x, int y, int cx, int cy);
	bool  IntersectClip(const Прям& r)                  { return IntersectClipOp(r); }
	bool  IntersectClip(int x, int y, int cx, int cy);
	bool  IsPainting(const Прям& r) const               { return IsPaintingOp(r); }
	bool  IsPainting(int x, int y, int cx, int cy) const;

	void DrawRect(int x, int y, int cx, int cy, Цвет color);
	void DrawRect(const Прям& rect, Цвет color);

	void DrawImage(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src);
	void DrawImage(int x, int y, int cx, int cy, const Рисунок& img);
	void DrawImage(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
	void DrawImage(int x, int y, int cx, int cy, const Рисунок& img, Цвет color);

	void DrawImage(const Прям& r, const Рисунок& img, const Прям& src);
	void DrawImage(const Прям& r, const Рисунок& img);
	void DrawImage(const Прям& r, const Рисунок& img, const Прям& src, Цвет color);
	void DrawImage(const Прям& r, const Рисунок& img, Цвет color);

	void DrawImage(int x, int y, const Рисунок& img, const Прям& src);
	void DrawImage(int x, int y, const Рисунок& img);
	void DrawImage(int x, int y, const Рисунок& img, const Прям& src, Цвет color);
	void DrawImage(int x, int y, const Рисунок& img, Цвет color);

	void DrawData(int x, int y, int cx, int cy, const Ткст& data, const char *тип);
	void DrawData(const Прям& r, const Ткст& data, const char *тип);

	void DrawLine(int x1, int y1, int x2, int y2, int width = 0, Цвет color = DefaultInk());
	void DrawLine(Точка p1, Точка p2, int width = 0, Цвет color = DefaultInk());

	void DrawEllipse(const Прям& r, Цвет color = DefaultInk(),
	                 int pen = Null, Цвет pencolor = DefaultInk());
	void DrawEllipse(int x, int y, int cx, int cy, Цвет color = DefaultInk(),
		             int pen = Null, Цвет pencolor = DefaultInk());

	void DrawArc(const Прям& rc, Точка start, Точка end, int width = 0, Цвет color = DefaultInk());

	void DrawPolyPolyline(const Точка *vertices, int vertex_count,
	                      const int *counts, int count_count,
	                      int width = 0, Цвет color = DefaultInk(), Цвет doxor = Null);
	void DrawPolyPolyline(const Вектор<Точка>& vertices, const Вектор<int>& counts,
		                  int width = 0, Цвет color = DefaultInk(), Цвет doxor = Null);
	void DrawPolyline(const Точка *vertices, int count,
		              int width = 0, Цвет color = DefaultInk(), Цвет doxor = Null);
	void DrawPolyline(const Вектор<Точка>& vertices,
		              int width = 0, Цвет color = DefaultInk(), Цвет doxor = Null);

	void   DrawPolyPolyPolygon(const Точка *vertices, int vertex_count,
		                       const int *subpolygon_counts, int subpolygon_count_count,
		                       const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
		                       Цвет color = DefaultInk(), int width = 0, Цвет outline = Null,
		                       uint64 pattern = 0, Цвет doxor = Null);
	void   DrawPolyPolyPolygon(const Вектор<Точка>& vertices,
	                           const Вектор<int>& subpolygon_counts,
	                           const Вектор<int>& disjunct_polygon_counts,
	                           Цвет color = DefaultInk(), int width = 0, Цвет outline = Null, uint64 pattern = 0, Цвет doxor = Null);
	void   DrawPolyPolygon(const Точка *vertices, int vertex_count,
	                       const int *subpolygon_counts, int subpolygon_count_count,
	                       Цвет color = DefaultInk(), int width = 0, Цвет outline = Null, uint64 pattern = 0, Цвет doxor = Null);
	void   DrawPolyPolygon(const Вектор<Точка>& vertices, const Вектор<int>& subpolygon_counts,
	                       Цвет color = DefaultInk(), int width = 0, Цвет outline = Null, uint64 pattern = 0, Цвет doxor = Null);
	void   DrawPolygons(const Точка *vertices, int vertex_count,
	                    const int *polygon_counts, int polygon_count_count,
	                    Цвет color = DefaultInk(), int width = 0, Цвет outline = Null, uint64 pattern = 0, Цвет doxor = Null);
	void   DrawPolygons(const Вектор<Точка>& vertices, const Вектор<int>& polygon_counts,
	                    Цвет color = DefaultInk(), int width = 0, Цвет outline = Null, uint64 pattern = 0, Цвет doxor = Null);
	void   DrawPolygon(const Точка *vertices, int vertex_count,
	                   Цвет color = DefaultInk(), int width = 0, Цвет outline = Null, uint64 pattern = 0, Цвет doxor = Null);
	void   DrawPolygon(const Вектор<Точка>& vertices,
	                   Цвет color = DefaultInk(), int width = 0, Цвет outline = Null, uint64 pattern = 0, Цвет doxor = Null);

	void DrawDrawing(const Прям& r, const Чертёж& iw) { DrawDrawingOp(r, iw); }
	void DrawDrawing(int x, int y, int cx, int cy, const Чертёж& iw);
	void DrawDrawing(int x, int y, const Чертёж& iw);

	void DrawPainting(const Прям& r, const Painting& iw) { DrawPaintingOp(r, iw); }
	void DrawPainting(int x, int y, int cx, int cy, const Painting& iw);
	void DrawPainting(int x, int y, const Painting& iw);

	void DrawText(int x, int y, int angle, const wchar *text, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), int n = -1, const int *dx = NULL);
	void DrawText(int x, int y, const wchar *text, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), int n = -1, const int *dx = NULL);

	void DrawText(int x, int y, const ШТкст& text, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), const int *dx = NULL);
	void DrawText(int x, int y, int angle, const ШТкст& text, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), const int *dx = NULL);

	void DrawText(int x, int y, int angle, const char *text, byte charset,
	              Шрифт font = StdFont(), Цвет ink = DefaultInk(), int n = -1, const int *dx = NULL);
	void DrawText(int x, int y, const char *text, byte charset, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), int n = -1, const int *dx = NULL);

	void DrawText(int x, int y, int angle, const char *text,
	              Шрифт font = StdFont(), Цвет ink = DefaultInk(), int n = -1, const int *dx = NULL);
	void DrawText(int x, int y, const char *text, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), int n = -1, const int *dx = NULL);

	void DrawText(int x, int y, const Ткст& text, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), const int *dx = NULL);
	void DrawText(int x, int y, int angle, const Ткст& text, Шрифт font = StdFont(),
		          Цвет ink = DefaultInk(), const int *dx = NULL);

	static void SinCos(int angle, double& sina, double& cosa);
	
	// deprecated:
	static void SetStdFont(Шрифт font)                   { РНЦП::SetStdFont(font); }
	static Шрифт GetStdFont()                            { return РНЦП::GetStdFont(); }
	static Размер GetStdFontSize()                        { return РНЦП::GetStdFontSize(); }
	static int  GetStdFontCy()                          { return GetStdFontSize().cy; }
	Размер   GetPagePixels() const                        { return GetPageSize(); }

protected:
	Цвет ResolveInk(Цвет c) const                     { return c == DefaultInk() ? GetDefaultInk() : c; }
	virtual Цвет GetDefaultInk() const;
};

void DrawImageBandRLE(Draw& w, int x, int y, const Рисунок& m, int minp);

class DataDrawer {
	typedef DataDrawer *(*Factory)();
	template <class T> static DataDrawer *FactoryFn() { return new T; }
	static void AddFormat(const char *id, Factory f);
	static ВекторМап<Ткст, void *>& вКарту();

public:
	virtual void  открой(const Ткст& data, int cx, int cy) = 0;
	virtual void  Render(ImageBuffer& ib) = 0;
	virtual ~DataDrawer();

	static  Один<DataDrawer> создай(const Ткст& id);

	template <class T>	static void регистрируй(const char *id)  { AddFormat(id, &DataDrawer::FactoryFn<T>); }
};

class Чертёж : public ТипЗнач<Чертёж, 49, Движимое<Чертёж> > {
	Размер       size;
	Ткст     data;
	МассивЗнач val;

	friend class DrawingDraw;
	friend class Draw;

public:
	operator bool() const          { return !data.пустой(); }
	Размер дайРазм() const           { return size; }
	void устРазм(Размер sz)          { size = sz; }
	void устРазм(int cx, int cy)   { size = Размер(cx, cy); }

	Размер RatioSize(int cx, int cy) const;
	Размер RatioSize(Размер sz) const  { return RatioSize(sz.cx, sz.cy); }

	void очисть()                   { data.очисть(); size = Null; }

	void приставь(Чертёж& dw);

	void сериализуй(Поток& s);
	void вРяр(РярВВ& xio)        { XmlizeBySerialize(xio, *this); }
	void вДжейсон(ДжейсонВВ& jio)      { JsonizeBySerialize(jio, *this); }

	bool    экзПусто_ли() const             { return data.пустой(); }
	void    устПусто()                          { size = Null; data.очисть(); }

	bool    operator==(const Чертёж& b) const { return val == b.val && data == b.data && size == b.size; }
	Ткст  вТкст() const                   { return "drawing " + какТкст(size); }
	hash_t  дайХэшЗнач() const               { return комбинируйХэш(data, val); }

	operator Значение() const                     { return богатыйВЗнач(*this); }
	Чертёж(const Значение& src)                  { *this = src.дай<Чертёж>(); }

	Чертёж()                                  { устПусто(); }
	Чертёж(const Обнул&)                     { устПусто(); }
};

class DrawingDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Размер  GetPageSize() const;
	virtual Прям  GetPaintRect() const;
	
	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Точка p);
	virtual bool ClipOp(const Прям& r);
	virtual bool ClipoffOp(const Прям& r);
	virtual bool ExcludeClipOp(const Прям& r);
	virtual bool IntersectClipOp(const Прям& r);
	virtual bool IsPaintingOp(const Прям& r) const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Цвет color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color);
	virtual void DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Цвет color, Цвет doxor);
	virtual void DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Цвет color, int width, Цвет outline,
	                                   uint64 pattern, Цвет doxor);
	virtual void DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor);
	virtual void DrawArcOp(const Прям& rc, Точка start, Точка end, int pen, Цвет pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font,
		                    Цвет ink, int n, const int *dx);

	virtual void DrawDrawingOp(const Прям& target, const Чертёж& w);
	virtual void DrawPaintingOp(const Прям& target, const Painting& w);
	
	virtual void Escape(const Ткст& data);
	
private:
	Размер         size;
	bool         dots;
	ТкстПоток drawing;
	МассивЗнач   val;

	Поток&      DrawingOp(int code);

public:
	void     создай(int cx, int cy, bool dots = true);
	void     создай(Размер sz, bool dots = true);

	Размер     дайРазм() const                  { return size; }

	Чертёж  дайРез();
	operator Чертёж()                        { return дайРез(); }

	DrawingDraw();
	DrawingDraw(int cx, int cy, bool dots = true);
	DrawingDraw(Размер sz, bool dots = true);
};

class NilDraw : public Draw {
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
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id);
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
	virtual void DrawDrawingOp(const Прям& target, const Чертёж& w);
	virtual void DrawPaintingOp(const Прям& target, const Painting& w);
};

struct DrawProxy : Draw {
	Draw *ptr;
	
	void SetTarget(Draw *w) { ptr = w; }
	
	virtual dword GetInfo() const;

	virtual Размер GetPageSize() const;
	virtual void StartPage();
	virtual void EndPage();

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
	virtual void SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id);
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
	virtual void DrawDrawingOp(const Прям& target, const Чертёж& w);
	virtual void DrawPaintingOp(const Прям& target, const Painting& w);
	
	virtual Размер GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();

	virtual int  GetCloffLevel() const;
	
	virtual void Escape(const Ткст& data);
};

class ImageAnyDraw : public Draw {
	Draw *draw;
	
	void иниц(Размер sz);

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
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id);
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
	virtual void DrawDrawingOp(const Прям& target, const Чертёж& w);
	virtual void DrawPaintingOp(const Прям& target, const Painting& w);

public:
	static bool IsAvailable();

	operator Рисунок() const;
	
	ImageAnyDraw(Размер sz);
	ImageAnyDraw(int cx, int cy);
	
	~ImageAnyDraw();
};

void         AddNotEmpty(Вектор<Прям>& result, int left, int right, int top, int bottom);
bool         Subtract(const Прям& r, const Прям& sub, Вектор<Прям>& result);
bool         Subtract(const Вектор<Прям>& rr, const Прям& sub, Вектор<Прям>& result);
Вектор<Прям> Subtract(const Вектор<Прям>& rr, const Прям& sub, bool& changed);
Вектор<Прям> пересек(const Вектор<Прям>& b, const Прям& a, bool& changed);

void Subtract(Вектор<Прям>& rr, const Прям& sub);
void союз(Вектор<Прям>& rr, const Прям& add);

Вектор<Прям> Intersection(const Вектор<Прям>& b, const Прям& a);

void AddRefreshRect(Вектор<Прям>& invalid, const Прям& _r);

void DrawDragFrame(Draw& w, const Прям& r, int n, const int *pattern, Цвет color, int animation);
void DrawDragFrame(Draw& w, const Прям& r, int n, int pattern, Цвет color, int animation);

void DrawRect(Draw& w, const Прям& rect, const Рисунок& img, bool ralgn = false); //??? TODO
void DrawRect(Draw& w, int x, int y, int cx, int cy, const Рисунок& img, bool ra = false);

void DrawTiles(Draw& w, int x, int y, int cx, int cy, const Рисунок& img);
void DrawTiles(Draw& w, const Прям& rect, const Рисунок& img);

void DrawFatFrame(Draw& w, int x, int y, int cx, int cy, Цвет color, int n);
void DrawFatFrame(Draw& w, const Прям& r, Цвет color, int n);

void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Цвет leftcolor, Цвет topcolor, Цвет rightcolor, Цвет bottomcolor);
void DrawFrame(Draw& w, const Прям& r,
			   Цвет leftcolor, Цвет topcolor, Цвет rightcolor, Цвет bottomcolor);
void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Цвет topleftcolor, Цвет bottomrightcolor);
void DrawFrame(Draw& w, const Прям& r,
			   Цвет topleftcolor, Цвет bottomrightcolor);
void DrawFrame(Draw& w, int x, int y, int cx, int cy, Цвет color);
void DrawFrame(Draw& w, const Прям& r, Цвет color);

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *colors_ltrd); //TODO
void DrawBorder(Draw& w, const Прям& r, const ColorF *colors_ltrd);

const ColorF *BlackBorder();
const ColorF *WhiteBorder();
const ColorF *ButtonPushBorder();
const ColorF *EdgeButtonBorder();
const ColorF *DefButtonBorder();
const ColorF *ButtonBorder();
const ColorF *InsetBorder();
const ColorF *OutsetBorder();
const ColorF *ThinOutsetBorder();
const ColorF *ThinInsetBorder();

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *(*colors_ltrd)());
void DrawBorder(Draw& w, const Прям& r, const ColorF *(*colors_ltrd)());

void DrawRectMinusRect(Draw& w, const Прям& rect, const Прям& inner, Цвет color);

void DrawHighlightImage(Draw& w, int x, int y, const Рисунок& img, bool highlight = true,
                        bool enabled = true, Цвет maskcolor = SColorPaper);

Цвет GradientColor(Цвет fc, Цвет tc, int i, int n);

void DrawTextEllipsis(Draw& w, int x, int y, int cx, const char *text, const char *ellipsis,
				      Шрифт font = StdFont(), Цвет ink = SColorText(), int n = -1);
void DrawTextEllipsis(Draw& w, int x, int y, int cx, const wchar *text, const char *ellipsis,
				      Шрифт font = StdFont(), Цвет ink = SColorText(), int n = -1);

Размер GetTLTextSize(const wchar *text, Шрифт font = StdFont());
int  GetTLTextHeight(const wchar *s, Шрифт font = StdFont());
void DrawTLText(Draw& draw, int x, int y, int cx, const wchar *text, Шрифт font = StdFont(),
                Цвет ink = SColorText(), int accesskey = 0);

enum {
	BUTTON_NORMAL, BUTTON_OK, BUTTON_HIGHLIGHT, BUTTON_PUSH, BUTTON_DISABLED, BUTTON_CHECKED,
	BUTTON_VERTICAL = 0x100,
	BUTTON_EDGE = 0x200,
	BUTTON_TOOL = 0x400,
	BUTTON_SCROLL = 0x800,
};

void DrawXPButton(Draw& w, Прям r, int тип);

struct PdfSignatureInfo;
typedef Ткст (*DrawingToPdfFnType)(const Массив<Чертёж>& report, Размер pagesize, int margin,
                                     bool pdfa, const PdfSignatureInfo *sign);
typedef void (*PdfDrawJPEGFnType)(Draw& w, int x, int y, int cx, int cy, const Ткст& jpeg_data);

void SetDrawingToPdfFn(DrawingToPdfFnType Pdf, PdfDrawJPEGFnType Jpeg);
DrawingToPdfFnType GetDrawingToPdfFn();
PdfDrawJPEGFnType GetPdfDrawJPEGFn();

typedef bool (*IsJPGFnType)(StreamRaster *s);
void SetIsJPGFn(IsJPGFnType isjpg);
IsJPGFnType GetIsJPGFn();

#include "Display.h"
#include "Cham.h"
#include "DDARasterizer.h"
#include "SDraw.h"

enum {
	CMAP_GLYPHS = 1,
	CMAP_ALLOW_SYMBOL = 2,
};

bool ReadCmap(const char *ptr, int count, Событие<int, int, int> range, dword flags = 0);
bool ReadCmap(Шрифт font, Событие<int, int, int> range, dword flags = 0);
bool GetPanoseNumber(Шрифт font, byte *panose);

}

#endif
