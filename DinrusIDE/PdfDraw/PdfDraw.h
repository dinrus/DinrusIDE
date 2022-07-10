#ifndef _PDF_pdf_h_
#define _PDF_pdf_h_

#include <Draw/Draw.h>
#include <plugin/z/z.h>
#include <Painter/Painter.h>

namespace РНЦП {

ИНИЦИАЛИЗУЙ(PdfDraw);

class TTFReader {
	struct TTFStream {
		struct Fail {};

		virtual void дайРяд(void *ptr, int count) = 0;

		TTFStream& operator%(byte& q);
		TTFStream& operator%(int32& q);
		TTFStream& operator%(uint32& q);
		TTFStream& operator%(int16& q);
		TTFStream& operator%(uint16& q);

		virtual ~TTFStream() {}
	};

	struct TTFStreamIn : TTFStream {
		const char *beg;
		const char *s;
		const char *lim;

		virtual void дайРяд(void *ptr, int count);

		void   перейди(int offset);
		int    дай8();
		int    дай16();
		int    дай32();
		Ткст дай(int n);

		virtual ~TTFStreamIn() {}
	};

	struct TTFStreamOut : TTFStream {
		Ткст out;

		virtual void дайРяд(void *ptr, int count) { out.конкат((const char *)ptr, count); }

		void   Put8(int data)       { out.конкат(data); }
		void   помести16(int data)      { Put8(HIBYTE(data)); Put8(LOBYTE(data)); }
		void   помести32(int data)      { помести16(HIWORD(data)); помести16(LOWORD(data)); }
		void   помести(const Ткст& s) { out.конкат(s); }

		operator Ткст() const     { return out; }

		virtual ~TTFStreamOut() {}
	};

	Шрифт font;
	Ткст current_table;

	struct Table : Движимое<Table> {
		int offset;
		int length;
	};
	ВекторМап<Ткст, Table> table;
	
	ВекторМап<wchar, int> glyph_map;

	struct GlyphInfo : Движимое<GlyphInfo> {
		int    offset;
		int    size;
		uint16 advanceWidth;
		int16  leftSideBearing;
	};

	Вектор<GlyphInfo> glyphinfo;

	void   SetGlyph(wchar ch, word glyph);

	struct Fail {};

	static void Ошибка() { throw Fail(); }

	int    Peek8(const char *s);
	int    подбери16(const char *s);
	int    подбери32(const char *s);
	int    Peek8(const char *s, int i);
	int    подбери16(const char *s, int i);
	int    подбери32(const char *s, int i);
	int    Read8(const char *&s);
	int    Read16(const char *&s);
	int    Read32(const char *&s);
	Ткст читай(const char *&s, int n);

	void   переустанов();

	const char *перейди(const char *tab, int& len);
	const char *перейди(const char *tab);
	void        перейди(const char *tab, TTFStreamIn& s);
	Ткст      GetTable(const char *tab);

	enum {
		ARG_1_AND_2_ARE_WORDS     = (1<<0),
		ARGS_ARE_XY_VALUES        = (1<<1),
		ROUND_XY_TO_GRID          = (1<<2),
		WE_HAVE_A_SCALE           = (1<<3),
		MORE_COMPONENTS           = (1<<5),
		WE_HAVE_AN_X_AND_Y_SCALE  = (1<<6),
		WE_HAVE_A_TWO_BY_TWO      = (1<<7),
		WE_HAVE_INSTRUCTIONS      = (1<<8),
		USE_MY_METRICS            = (1<<9),
	};

	struct NewTable {
		Ткст имя;
		Ткст data;
		dword  checksum;
	};

	static void   добавь(Массив<NewTable>& t, const char *имя, const Ткст& data);
	static void   уст(NewTable& m, const Ткст& data);
	static Ткст CreateHeader(const Массив<NewTable>& t);

	static inline int    Copy8(TTFStreamOut& out, TTFStreamIn& in)
		{ int q = in.дай8(); out.Put8(q); return q; }
	static inline int    Copy16(TTFStreamOut& out, TTFStreamIn& in)
		{ int q = in.дай16(); out.помести16(q); return q; }
	static inline int    Copy32(TTFStreamOut& out, TTFStreamIn& in)
		{ int q = in.дай32(); out.помести32(q); return q; }
	static inline Ткст копируй(TTFStreamOut& out, TTFStreamIn& in, int n)
		{ Ткст q = in.дай(n); out.помести(q); return q; }

public:
	struct дайГолову {
		dword   version;
		dword   fontRevision;
		uint32  checkсуммаAdjustment;
		uint32  magicNumber;
		uint16  flags;
		uint16  unitsPerEm;
		byte    created[8];
		byte    modified[8];
		int16   xMin;
		int16   yMin;
		int16   xMax;
		int16   yMax;
		uint16  macStyle;
		uint16  lowestRecPPEM;
		int16   fontDirectionHint;
		int16   indexToLocFormat;
		int16   glyphDataFormat;

		void сериализуй(TTFStream& s);
	};

	struct Hhea {
		uint32  version;
		int16   ascent;
		int16   descent;
		int16   lineGap;
		uint16  advanceWidthMax;
		int16   minLeftSideBearing;
		int16   minRightSideBearing;
		int16   xMaxExtent;
		int16   caretSlopeRise;
		int16   caretSlopeRun;
		int16   caretOffset;
		int16   reserved1;
		int16   reserved2;
		int16   reserved3;
		int16   reserved4;
		int16   metricDataFormat;
		uint16  numOfLongHorMetrics;

		void сериализуй(TTFStream& s);
	};

	struct Maxp {
		uint32  version;
		uint16  numGlyphs;
		uint16  maxPoints;
		uint16  maxContours;
		uint16  maxComponentPoints;
		uint16  maxComponentContours;
		uint16  maxZones;
		uint16  maxTwilightPoints;
		uint16  maxStorage;
		uint16  maxFunctionDefs;
		uint16  maxInstructionDefs;
		uint16  maxStackElements;
		uint16  maxSizeOfInstructions;
		uint16  maxComponentElements;
		uint16  maxComponentDepth;

		void сериализуй(TTFStream& s);
	};

	struct пост {
		uint32  формат;
		int32   italicAngle;
		int16   underlinePosition;
		int16   underlineThickness;
		uint16  isFixedPitch;
		uint16  reserved;
		uint32  minMemType42;
		uint32  maxMemType42;
		uint32  minMemType1;
		uint32  maxMemType1;

		void сериализуй(TTFStream& s);
	};

	дайГолову   head;
	Hhea   hhea;
	Maxp   maxp;
	пост   post;
	Ткст ps_name;

	int    GetGlyph(wchar chr)               { return glyph_map.дай(chr, 0); }
	word   GetAdvanceWidth(wchar chr)        { int i = glyph_map.дай(chr, 0); return i < glyphinfo.дайСчёт() ? glyphinfo[GetGlyph(chr)].advanceWidth : 0; }

	Ткст Subset(const Вектор<wchar>& chars, int first = 0, bool os2 = false);
	bool   открой(const Шрифт& fnt, bool symbol = false, bool justcheck = false);

	TTFReader();
};

struct PdfSignatureInfo {
	Ткст pkey;
	Ткст cert;

	Ткст имя;
	Ткст location;
	Ткст reason;
	Ткст contact_info;

	Время   time;
	
	PdfSignatureInfo() { time = Null; }
};

class PdfDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Размер  GetPageSize() const;

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

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Цвет color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color);
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
	virtual void DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font,
		                    Цвет ink, int n, const int *dx);

	virtual void Escape(const Ткст& data);

private:
	struct CharPos : Движимое<CharPos>   { word fi, ci; };

	struct OutlineInfo : Движимое<OutlineInfo> {
		bool standard_ttf;
		bool sitalic;
		bool sbold;
	};
	
	struct UrlInfo {
		ПрямПЗ  rect;
		Ткст url;
	};

	ВекторМап<Шрифт, OutlineInfo>                outline_info;
	ВекторМап<Шрифт, Вектор<wchar>>              pdffont;
	ВекторМап<Шрифт, ВекторМап<wchar, CharPos>>  fontchars;
	Индекс<uint64>                               patterns;
	ВекторМап<Tuple2<int64, Прям>, Рисунок>       images;
	Массив<Массив<UrlInfo>>                       page_url;
	Ткст                                      data; // temporary escape data, e.g. JPEG
	Вектор<Ткст>                              jpeg;
	
	Вектор<int>  offset;
	ТкстБуф out;
	ТкстБуф page;
	Размер        pgsz;
	Цвет       rgcolor;
	Цвет       RGcolor;
	int         fontid;
	double      textht;
	double      linewidth;
	int         margin;
	uint64      patternid;
	bool        pdfa;
	bool        empty;
	Ткст      url;
	Вектор<Точка> offset_stack;
	Точка       current_offset;

	double Pt(double dot)               { return 0.12 * dot; }
	Ткст Ptf(double dot)              { return фмтФ(Pt(dot), 5); }

	int    Поз()                        { return offset.дайСчёт() + 1; }
	int    BeginObj();
	void   EndObj();
	int    PutStream(const Ткст& data, const Ткст& keys = Null, bool compress = true);

	void    PutRect(const Прям& rc);
	void    PutrgColor(Цвет rg, uint64 pattern = 0);
	void    PutRGColor(Цвет RG);
	void    PutFontHeight(int fi, double ht);
	void    PutLineWidth(int lw);

	CharPos GetCharPos(Шрифт fnt, wchar chr);
	void    FlushText(int dx, int fi, int height, const Ткст& txt);
	static Ткст PdfColor(Цвет c);
	static Ткст PdfString(const char *s);

	void PushOffset();
	void PopOffset();

	OutlineInfo GetOutlineInfo(Шрифт fnt);

	void иниц(int pagecx, int pagecy, int margin, bool pdfa);

	struct RGlyph : Движимое<RGlyph> {
		Ткст data;
		Размер   sz;
		int    x;
		int    color_image = -1;
	};
	
	struct CGlyph : Движимое<CGlyph> {
		Размер   sz;
		int    x;
		int    image;
	};
	
	ВекторМап<Кортеж<Шрифт, int>, CGlyph> color_glyph;

	int    PdfImage(const Рисунок& img, const Прям& src);
	CGlyph ColorGlyph(Шрифт fnt, int chr);
	RGlyph RasterGlyph(Шрифт fnt, int chr);

public:
	Ткст финиш(const PdfSignatureInfo *sign = NULL);
	void   очисть();
	bool   пустой() const                                   { return empty; }
	
	PdfDraw(int pagecx, int pagecy, bool pdfa = false)       { иниц(pagecx, pagecy, 0, pdfa); }
	PdfDraw(Размер pgsz = Размер(5100, 6600), bool pdfa = false) { иниц(pgsz.cx, pgsz.cy, 0, pdfa); }
};

void DrawJPEG(Draw& w, int x, int y, int cx, int cy, const Ткст& jpeg_data);

Ткст Pdf(const Массив<Чертёж>& report, Размер pagesize, int margin, bool pdfa = false,
           const PdfSignatureInfo *sign = NULL);

}

#endif
