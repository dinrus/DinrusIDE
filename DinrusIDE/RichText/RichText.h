#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <Draw/Draw.h>
#include <Draw/Draw.h>
#include <plugin/png/png.h>

namespace РНЦП {

#define IMAGECLASS RichTextImg
#define IMAGEFILE <RichText/RichText.iml>
#include <Draw/iml_header.h>

ИНИЦИАЛИЗУЙ(RichImage);

class  PasteClip;
struct RichPara;
class  RichTable;
class  RichTxt;

inline Цвет VoidColor() { return Цвет::особый(223); } // Used for mixed/no change in дай/SetCellFormat

struct Zoom {
	int m, d;

	int operator*(int x) const   { return d ? iscale(x, m, d) : 0; }
	int operator&(int x) const   { int q = d ? iscale(x, m, d) : 0; return x > 0 ? max(q, 1) : q; }

	double AsDouble() const      { return (double)m / d; }
	Zoom   Reciprocal() const    { return Zoom(d, m); }

	Zoom()                       { m = d = 1; }
	Zoom(const Обнул&)          { m = d = 0; }
	Zoom(int _m, int _d)         { m = _m, d = _d; }

	void сериализуй(Поток& s)    { s % m % d; }

	bool operator==(Zoom a)      { return m == a.m && d == a.d; }
	bool operator!=(Zoom a)      { return m != a.m || d != a.d; }
	
	Ткст вТкст() const      { return Ткст() << m << "/" << d; }

	friend int operator/(int x, Zoom z)  { return z.m ? iscale(x, z.d, z.m) : 0; }
};

inline bool пусто_ли(Zoom z) { return (z.m | z.d) == 0; }

inline int operator*(int x, Zoom m)
{
	return m * x;
}

inline void operator*=(int& i, Zoom m)
{
	i = m * i;
}

inline void operator*=(Прям& r, Zoom m)
{
	r.left *= m;
	r.right *= m;
	r.top *= m;
	r.bottom *= m;
}

inline Размер operator*(Zoom m, Размер sz)
{
	return Размер(m * sz.cx, m * sz.cy);
}

inline Размер operator/(Размер sz, Zoom m)
{
	return Размер(sz.cx / m, sz.cy / m);
}

struct PageY : Движимое<PageY, RelOps<PageY> > {
	int page;
	int y;

	PageY(int page, int y) : page(page), y(y) {}
	PageY() { page = y = 0; }

#ifdef _ОТЛАДКА
	Ткст вТкст() const    { return какТкст(page) + ":" + какТкст(y); }
#endif
};

inline bool operator<(PageY a, PageY b)
{
	return a.page < b.page ? true : a.page == b.page ? a.y < b.y : false;
}

inline bool operator==(PageY a, PageY b)
{
	return a.page == b.page && a.y == b.y;
}

inline PageY operator+(PageY a, int b)
{
	return PageY(a.page, a.y + b);
}

struct PageRect : public Прям {
	int  page;

	operator int() const                 { return page; }
	operator PageY() const               { return PageY(page, top); }
	PageRect& operator=(const Прям& r)   { (Прям&)(*this) = r; page = 0; return *this; }
	PageRect(const Прям& r)              { (Прям&)(*this) = r; page = 0; }
	PageRect()                           { очисть(); page = 0; }
};

struct RichTextLayoutTracer {
	virtual void  Paragraph(const Прям& page, PageY y, const RichPara& para);
	virtual void  EndParagraph(PageY y);
	virtual void  Table(const Прям& page, PageY y, const RichTable& table);
	virtual void  EndTable(PageY y);
	virtual void  TableRow(const Прям& page, PageY y, int i, const RichTable& table);
	virtual void  EndTableRow(PageY y);
	virtual void  TableCell(const Прям& page, PageY y, int i, int j, const RichTable& table, PageY ny);
	virtual void  EndTableCell(PageY y);
};

struct PageDraw {
	virtual Draw& Page(int i) = 0;
	
	RichTextLayoutTracer *tracer;
	
	PageDraw() { tracer = NULL; }

	virtual ~PageDraw() {}
};

class RichObject;
class Бар;

struct RichObjectType {
	virtual Ткст GetTypeName(const Значение& v) const = 0;
	virtual Ткст GetCreateName() const;
	virtual Значение  читай(const Ткст& s) const;
	virtual Ткст пиши(const Значение& v) const;
	virtual bool   текст_ли() const;

	virtual bool   прими(PasteClip& clip);
	virtual Значение  читай(PasteClip& clip);
	virtual Ткст GetClipFmts() const;
	virtual Ткст GetClip(const Значение& data, const Ткст& fmt) const;

	virtual Размер   GetDefaultSize(const Значение& data, Размер maxsize, void *context) const;
	virtual Размер   GetPhysicalSize(const Значение& data, void *context) const;
	virtual Размер   GetPixelSize(const Значение& data, void *context) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz, void *context) const;
	virtual Рисунок  ToImage(int64 serial_id, const Значение& data, Размер sz, void *context) const;
	virtual void   Menu(Бар& bar, RichObject& ex, void *context) const;
	virtual void   DefaultAction(RichObject& ex, void *context) const;
	virtual Ткст GetLink(const Значение& data, Точка pt, Размер sz, void *context) const;
	
	Размер           StdDefaultSize(const Значение& data, Размер maxsize, void *context) const;

	RichObjectType();
	virtual ~RichObjectType();
	
protected:
	virtual Размер   GetDefaultSize(const Значение& data, Размер maxsize) const;
	virtual Размер   GetPhysicalSize(const Значение& data) const;
	virtual Размер   GetPixelSize(const Значение& data) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz) const;
	virtual Рисунок  ToImage(int64 serial_id, const Значение& data, Размер sz) const;
	virtual void   Menu(Бар& bar, RichObject& ex) const;
	virtual void   DefaultAction(RichObject& ex) const;
	virtual Ткст GetLink(const Значение& data, Точка pt, Размер sz) const;
};

class RichObject : Движимое<RichObject> {
	Значение                 data;
	int                   ydelta;
	Размер                  size;
	Размер                  physical_size;
	Размер                  pixel_size;
	bool                  keepratio;
	const RichObjectType *тип;
	int64                 serial;
	Ткст                type_name;

	static ВекторМап<Ткст, RichObjectType *>& вКарту();

	void                  NewSerial();
	void                  AdjustPhysicalSize();

public:
	static void   регистрируй(const char *имя, RichObjectType *тип) init_;
	static int    GetTypeCount()                 { return вКарту().дайСчёт(); }
	static int    FindType(const Ткст& имя)   { return вКарту().найди(имя); }
	static RichObjectType& дайТип(int i)        { return *вКарту()[i]; }
	static Ткст GetTypeName(int i)             { return вКарту().дайКлюч(i); }
	
	void   устРазм(int cx, int cy)               { size = Размер(cx, cy); }
	void   устРазм(Размер sz)                      { устРазм(sz.cx, sz.cy); }
	Размер   дайРазм() const                       { return size; }
	void   рисуй(Draw& w, Размер sz, void *context = NULL) const;
	Рисунок  ToImage(Размер sz, void *context = NULL) const;
	Размер   GetPhysicalSize() const               { return physical_size; }
	Размер   GetPixelSize() const                  { return pixel_size; }
	Размер   GetDefaultSize(Размер maxsize, void *context = NULL) const { return тип ? тип->GetDefaultSize(data, maxsize, context) : physical_size; }

	void   уст(RichObjectType *тип, const Значение& data, Размер maxsize = Размер(3967, 3967), void *context = NULL);
	bool   уст(const Ткст& type_name, const Значение& data, Размер maxsize = Размер(3967, 3967), void *context = NULL);
	void   устДанные(const Значение& v);

	Ткст GetTypeName() const;
	Значение  дайДанные() const                       { return data; }
	Ткст GetLink(Точка pt, Размер sz, void *context = NULL) const { return тип ? тип->GetLink(data, pt, sz, context) : Ткст(); }

	const RichObjectType& дайТип() const;

	bool   читай(const Ткст& тип, const Ткст& data, Размер sz, void *context = NULL);
	Ткст пиши() const                         { return тип ? тип->пиши(data) : (Ткст)data; }
	bool   текст_ли() const                        { return тип ? тип->текст_ли() : false; }

	void   KeepRatio(bool b)                     { keepratio = b; }
	bool   IsKeepRatio() const                   { return keepratio; }

	void   SetYDelta(int yd)                     { ydelta = yd; }
	int    GetYDelta() const                     { return ydelta; }

	void   Menu(Бар& bar, void *context = NULL)  { if(тип) тип->Menu(bar, *this, context); }
	void   DefaultAction(void *context = NULL)   { if(тип) тип->DefaultAction(*this, context); }

	operator bool() const                        { return !пусто_ли(data); }

	void   очисть();

	int64  GetSerialId() const                   { return serial; }
	
	void   InitSize(int cx, int cy, void *context = NULL);

	RichObject();
	RichObject(RichObjectType *тип, const Значение& data, Размер maxsize = Размер(3967, 3967));
	RichObject(const Ткст& тип, const Значение& data, Размер maxsize = Размер(3967, 3967));
};

RichObject CreateDrawingObject(const Чертёж& dwg, Размер dot_size, Размер size);
RichObject CreateDrawingObject(const Чертёж& dwg, int cx = 0, int cy = 0);
RichObject CreatePaintingObject(const Painting& dwg, Размер dot_size, Размер size);
RichObject CreatePaintingObject(const Painting& dwg, int cx = 0, int cy = 0);
RichObject CreatePNGObject(const Рисунок& img, Размер dot_size, Размер size);
RichObject CreatePNGObject(const Рисунок& img, int cx = 0, int cy = 0);
RichObject CreateRawImageObject(const Ткст& s, int cx = 0, int cy = 0);

enum {
	RICHHOT_LM = -1,
	RICHHOT_RM = -2,
};

struct RichHotPos { // used for resizing table dimensions by mouse
	int table;
	int column; // can be RICHHOT_LM or RICHHOT_RM or columns Индекс
	int delta;
	int left, cx;
	int textleft, textcx;

	RichHotPos()       { table = 0; column = Null; left = cx = 0; }
};

struct RichValPos : Движимое<RichValPos> {
	PageY   py;
	int     pos;
	ШТкст data;
};

struct PaintInfo {
	Zoom    zoom;
	int     sell, selh;
	int     tablesel;
	Прям    cells;
	PageY   top;
	PageY   bottom;
	Цвет   hyperlink;
	Цвет   indexentry;
	Цвет   textcolor; // not Null overrides text color, makes paper transparent, useful for QTFDisplay
	bool    indexentrybg;
	bool    usecache;
	bool    sizetracking;
	Цвет   showcodes;
	Биты  (*spellingchecker)(const RichPara& para);
	int     highlightpara;
	Цвет   highlight;
	bool    darktheme;
	void   *context;
	bool    showlabels;
	bool    shrink_oversized_objects;
	void  (*DrawSelection)(Draw& w, int x, int y, int cx, int cy);
	
	Цвет   ResolveInk(Цвет ink) const;
	Цвет   ResolvePaper(Цвет paper) const;

	PaintInfo();
};

int LineZoom(Zoom z, int a);

class RichTable;
class RichText;
struct RichStyle;

typedef МассивМап<Uuid, RichStyle> RichStyles;

struct RichContext {
	const RichText   *text;
	const RichStyles *styles;
	RichText         *header, *footer;
	int               header_cy, footer_cy; // next page header/footer size
	int               current_header_cy, current_footer_cy; // current header/footer size
	Прям              page;
	PageY             py;

	void              HeaderFooter(RichText *header, RichText *footer_qtf);
	void              AdjustPage();
	void              Page();
	void              уст(PageY p0, const Прям& first_page, const Прям& next_page, PageY p);

	RichContext(const RichStyles& styles, const RichText *text);
	RichContext() {}
};

#include "Para.h"

struct RichPos {
	int              tabtextparti;
	int              tabtextpartcount;
	int              tabposintabtext;
	int              tabtextlen;

	int              table; // current level table Индекс (unique in text) or zero if not in table
	Размер             tabsize;
	Точка            cell;

	int              tablen;
	int              posintab;

	int              celllen;
	int              posincell;

	int              parai;
	int              partcount;
	int              posinpara;
	int              paralen;

	int              level;
	int              parenttab;

	RichPara::фмт формат;
	int              chr;
	RichObject       object;
	Ид               field;
	Ткст           fieldparam;
	RichPara::CharFormat fieldformat;

#ifdef _ОТЛАДКА
	Ткст вТкст() const;
#endif

	RichPos();
};

inline bool InSameTxt(const RichPos& a, const RichPos& b)
{
	return a.table == b.table && (a.table == 0 || a.cell == b.cell);
}

struct RichCaret : PageRect {
	int  lineascent;
	int  caretascent;
	int  caretdescent;
	int  objectcy;
	int  objectyd;
	int  line;
	Прям textpage;

	RichCaret()         { lineascent = caretascent = caretdescent = 0; }
};

struct RichStyle {
	RichPara::фмт     формат;
	Ткст               имя;
	Uuid                 next;

	static Uuid              GetDefaultId();
	static const RichStyle&  GetDefault();

	RichStyle()          { next = GetDefaultId(); }
};

const RichStyle& дайСтиль(const RichStyles& s, const Uuid& id);
int   FindStyleWithName(const RichStyles& style, const Ткст& имя);

class RichText;

struct RichCellPos;

enum {
	QTF_BODY = 1,
	QTF_ALL_STYLES = 2,
	QTF_NOSTYLES = 4,
	QTF_CRLF = 8,
	QTF_NOCHARSET = 16,
	QTF_NOLANG = 32,
	
	QTF_ALL = 0xffffffff
};

#include "Txt.h"
#include "Table.h"
#include "Text.h"

struct RichCellPos {
	int               pos;

	int               textlen;

	Размер              tabsize;
	int               tabpos;
	int               tablen;
	int               cellpos;
	int               celllen;
	int               level;
	RichCell::фмт  формат;
	RichTable::фмт tableformat;

#ifdef _ОТЛАДКА
	Ткст вТкст() const;
#endif
};

Ткст DeQtf(const char *s);
Ткст DeQtfLf(const char *s);

Ткст QtfFormat(Цвет c);

struct QtfRichObject {
	RichObject obj;

public:
	Ткст вТкст() const;

	QtfRichObject() {}
	QtfRichObject(const RichObject& o);
};

RichText AsRichText(const RichObject& obj);
Ткст   AsQTF(const RichObject& obj);

bool ParseQTF(RichText& txt, const char *qtf, int accesskey = 0, void *context = NULL);

RichText ParseQTF(const char *qtf, int accesskey = 0, void *context = NULL);

RichText AsRichText(const wchar *s, const RichPara::фмт& f = RichPara::фмт());

Ткст   AsQTF(const RichText& doc, byte charset = CHARSET_UTF8,
               dword options = QTF_BODY|QTF_ALL_STYLES|QTF_CRLF);

inline Ткст StylesAsQTF(const RichText& doc, byte charset = CHARSET_UTF8)
{ return AsQTF(doc, charset, QTF_ALL_STYLES|QTF_CRLF); }

inline Ткст BodyAsQTF(const RichText& doc, byte charset = CHARSET_UTF8)
{ return AsQTF(doc, charset, QTF_BODY|QTF_CRLF); }


void SetQTF(Один<RichText>& txt, const Ткст& qtf);

int GetRichTextScreenStdFontHeight();

enum
{
	ROUNDOFF          = 1 << 20,
	MAX_FONTS         = 10000,
	MAX_DOTS          = 600 * 100,
	MAX_DOT_HEIGHT    = 1200,
	MAX_POINT_HEIGHT  = MAX_DOT_HEIGHT * 3 / 25,
};

inline int DotTwips (int dots) { return (dots * 12 +  5 * ROUNDOFF +  2) /  5 - ROUNDOFF; }
inline int TwipDots (int twp)  { return (twp  *  5 + 12 * ROUNDOFF +  6) / 12 - ROUNDOFF; }
inline int DotPoints(int dots) { return (dots *  3 + 25 * ROUNDOFF + 12) / 25 - ROUNDOFF; }
inline int PointDots(int pts)  { return (pts  * 25 +  3 * ROUNDOFF +  1) /  3 - ROUNDOFF; }
inline int TwipDotSize(int twp)  { return пусто_ли(twp) ? 0 : minmax<int>(TwipDots(twp), 0, MAX_DOTS); }
inline int PointDotHeight(int p) { return (minmax<int>(Nvl(p, 0), 0, MAX_POINT_HEIGHT) * 25 + 5) / 6; }

void  SetRichTextStdScreenZoom(int m, int d);
Zoom  GetRichTextStdScreenZoom();

const Дисплей& QTFDisplay();
const Дисплей& QTFDisplayVCenter();

class HtmlObjectSaver
{
public:
	virtual ~HtmlObjectSaver() {}
	
	virtual Ткст GetHtml(const RichObject& object)                     { return Null; }
	virtual Ткст GetHtml(const RichObject& object, const Ткст& link) { return GetHtml(object); }
};

Ткст EncodeHtml(const RichText& text, Индекс<Ткст>& css,
                  const ВекторМап<Ткст, Ткст>& links,
                  const ВекторМап<Ткст, Ткст>& labels,
                  const Ткст& path, const Ткст& base = Null, Zoom z = Zoom(8, 40),
                  const ВекторМап<Ткст, Ткст>& escape = ВекторМап<Ткст, Ткст>(),
                  int imtolerance = 0);
Ткст EncodeHtml(const RichText& text, Индекс<Ткст>& css,
                  const ВекторМап<Ткст, Ткст>& links,
                  const ВекторМап<Ткст, Ткст>& labels,
                  HtmlObjectSaver& object_saver, Zoom z = Zoom(8, 40),
                  const ВекторМап<Ткст, Ткст>& escape = ВекторМап<Ткст, Ткст>());
Ткст AsCss(Индекс<Ткст>& ss);

Ткст MakeHtml(const char *title, const Ткст& css, const Ткст& body);

inline //BW - no labels
Ткст EncodeHtml(const RichText& text, Индекс<Ткст>& css,
                  const ВекторМап<Ткст, Ткст>& links,
                  const Ткст& path, const Ткст& base = Null, Zoom z = Zoom(8, 40)) {
	return EncodeHtml(text, css, links, ВекторМап<Ткст, Ткст>(), path, base, z);
}

struct SimplePageDraw : PageDraw {
	Draw& w;

	virtual Draw& Page(int);

	SimplePageDraw(Draw& w) : w(w) {}
	virtual ~SimplePageDraw() {}
};

struct PrintPageDraw : PageDraw {
	int     page;
	Draw&   w;
	NilDraw nw;

	Draw& Page(int _page)     { return page == _page ? w : (Draw&)nw; }
	void  устСтраницу(int _page)  { page = _page; }

	PrintPageDraw(Draw& w) : w(w) {}
	virtual ~PrintPageDraw() {}
};

Массив<Чертёж> RenderPages(const RichText& txt, Размер pagesize = Размер(3968, 6074));

Ткст Pdf(const RichText& txt, Размер pagesize = Размер(3968, 6074), int margin = 200,
           bool pdfa = false, const PdfSignatureInfo *sign = NULL);


}

#endif
