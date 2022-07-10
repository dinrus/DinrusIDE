#ifndef CTRLCORE_H
#define CTRLCORE_H

#include <RichText/RichText.h>
#include <Painter/Painter.h>

#ifdef  flagNOGTK
#undef  flagGTK
#define flagX11
#endif

#include <guiplatform.h>

#ifndef GUIPLATFORM_INCLUDE
	#ifdef flagVIRTUALGUI
		#define VIRTUALGUI 1
	#endif

	#ifdef flagTURTLE
		#define GUIPLATFORM_KEYCODES_INCLUDE <Turtle/Keys.h>
		//need to make SDL_keysym.h known before K_ enum
		#define GUIPLATFORM_INCLUDE          <Turtle/Turtle.h>
		#define GUIPLATFORM_NOSCROLL
		#define PLATFORM_TURTLE
		#define TURTLE
	#elif VIRTUALGUI
		#define GUIPLATFORM_KEYCODES_INCLUDE <VirtualGui/Keys.h>
		#define GUIPLATFORM_INCLUDE          <VirtualGui/VirtualGui.h>
	#elif PLATFORM_COCOA
		#define GUIPLATFORM_INCLUDE          "Coco.h"
		#define GUIPLATFORM_NOSCROLL
	#elif PLATFORM_WIN32
		#define GUIPLATFORM_INCLUDE "Win32Gui.h"
	#else
		#ifdef flagX11
			#define GUIPLATFORM_INCLUDE "X11Gui.h"
		#else
			#ifndef flagGTK
				#define flagGTK
			#endif
			#define GUIPLATFORM_INCLUDE "Gtk.h"
		#endif
	#endif
	
#endif

#define GUI_APP_MAIN_HOOK

#include GUIPLATFORM_INCLUDE

namespace РНЦП {

ИНИЦИАЛИЗУЙ(CtrlCore)

void войдиВСтопорГип();
bool пробуйВойтиВСтопорГип();
void покиньСтопорГип();

int  покиньВсеСтопорыГип();
void войдиВСтопорГип(int n);

bool уНитиЕстьЗамокГип();
int  дайУровеньЗамкаГип();

struct ЗамкниГип {
	ЗамкниГип()  { войдиВСтопорГип(); }
	~ЗамкниГип() { покиньСтопорГип(); }
};

class ОтомкниГип {
	int n;

public:
	ОтомкниГип()  { n = покиньВсеСтопорыГип(); }
	~ОтомкниГип() { войдиВСтопорГип(n); }
};

bool ScreenInPaletteMode(); // Deprecated

typedef ImageDraw SystemImageDraw;

void устПоверхность(Draw& w, const Прям& dest, const КЗСА *pixels, Размер srcsz, Точка poff);
void устПоверхность(Draw& w, int x, int y, int cx, int cy, const КЗСА *pixels);

enum {
	K_DELTA        = 0x200000,
	K_CHAR_LIM     = 0x200000, // lower that this, ключ in Ключ is Unicode codepoint

	K_ALT          = 0x1000000,
	K_SHIFT        = 0x800000,
	K_CTRL         = 0x400000,
#ifdef PLATFORM_COCOA
	K_OPTION       = 0x2000000,
#endif

	K_KEYUP        = 0x4000000,

	K_MOUSEMIDDLE  = 0x2,
	K_MOUSERIGHT   = 0x4,
	K_MOUSELEFT    = 0x8,
	K_MOUSEDOUBLE  = 0x10,
	K_MOUSETRIPLE  = 0x20,

	K_SHIFT_CTRL = K_SHIFT|K_CTRL,


	K_PEN          = 0x80,

	IK_DBL_CLICK   = 0x40000001, // this is just to get the info that the entry is equal to dbl-click to the menu

	K_MOUSE_FORWARD = 0x80000001,
	K_MOUSE_BACKWARD = 0x80000002,
};

#include "MKeys.h"

bool дайШифт();
bool дайКтрл();
bool дайАльт();
bool дайКапсЛок();
bool дайЛевуюМыши();
bool дайПравуюМыши();
bool дайСреднююМыши();

#ifdef PLATFORM_COCOA
bool дайОпцию();
#endif

enum {
	DELAY_MINIMAL = 0
};

void  устОбрвызВремени(int delay_ms, Функция<void ()> cb, void *id = NULL); // delay_ms < 0 -> periodic
void  глушиОбрвызВремени(void *id);
bool  естьОбрвызВремени(void *id);
dword GetTimeClick();

inline
void  постОбрвыз(Функция<void ()> cb, void *id = NULL)  { устОбрвызВремени(1, cb, id); }

class ОбрвызВремени
{
public:
	~ОбрвызВремени()                               { глуши(); (void)dummy; }

	void уст(int delay, Функция<void ()> cb)     { РНЦП::устОбрвызВремени(delay, cb, this); }
	void пост(Функция<void ()> cb)               { РНЦП::постОбрвыз(cb, this); }
	void глуши()                                   { РНЦП::глушиОбрвызВремени(this); }
	void глушиУст(int delay, Функция<void ()> cb) { глуши(); уст(delay, cb); }
	void глушиПост(Функция<void ()> cb)           { глуши(); пост(cb); }

private:
	byte dummy;
};

class Ктрл;

class КтрлФрейм {
public:
	virtual void выложиФрейм(Прям& r) = 0;
	virtual void добавьРазмФрейма(Размер& sz) = 0;
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void добавьКФрейму(Ктрл& parent);
	virtual void удалиФрейм();
	virtual int  рисуйПоверх() const;

	КтрлФрейм() {}
	virtual ~КтрлФрейм() {}

private:
	КтрлФрейм(const КтрлФрейм&);
	void operator=(const КтрлФрейм&);
};

struct КлассФреймаПусто : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r);
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);
};

КтрлФрейм& фреймПусто();

class ФреймГраницы : public КтрлФрейм {
public:
	virtual void выложиФрейм(Прям& r);
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);

protected:
	const ColorF *border;

public:
	ФреймГраницы(const ColorF *border) : border(border) {}
};

КтрлФрейм& фреймИнсет();
КтрлФрейм& фреймАутсет();
КтрлФрейм& фреймКнопка();
КтрлФрейм& фреймТонкийИнсет();
КтрлФрейм& фреймТонкийАутсет();
КтрлФрейм& фреймЧёрный();
КтрлФрейм& фреймБелый();

КтрлФрейм& XPFieldFrame();

КтрлФрейм& фреймПоле();
// КтрлФрейм& EditFieldFrame(); //TODO remove

КтрлФрейм& фреймВерхнСепаратора();
КтрлФрейм& фреймНижнСепаратора();
КтрлФрейм& фреймЛевСепаратора();
КтрлФрейм& фреймПравСепаратора();

КтрлФрейм& фреймПравПроёма();

void выложиФреймСлева(Прям& r, Ктрл *ctrl, int cx);
void выложиФреймСправа(Прям& r, Ктрл *ctrl, int cx);
void выложиФреймСверху(Прям& r, Ктрл *ctrl, int cy);
void выложиФреймСнизу(Прям& r, Ктрл *ctrl, int cy);

Точка дайПозМыши();
dword дайФлагиМыши();

#define IMAGECLASS CtrlCoreImg
#define IMAGEFILE <CtrlCore/CtrlCore.iml>
#include <Draw/iml_header.h>

class ТопОкно;
class ИконкаТрея;
class ГЛКтрл;

enum {
	DND_NONE = 0,
	DND_COPY = 1,
	DND_MOVE = 2,

	DND_ALL  = 3,
	
	DND_EXACTIMAGE = 0x80000000,
};

struct ЦельЮБроса;

struct ClipData : Движимое<ClipData> {
	Значение  data;
	Ткст (*render)(const Значение& data);

	Ткст  Render() const                   { return render ? (*render)(data) : ~data; }

	ClipData(const Значение& data, Ткст (*render)(const Значение& data));
	ClipData(const Ткст& data);
	ClipData();
};

class PasteClip {
	friend struct ЦельЮБроса;
	friend class  Ктрл;
	friend PasteClip sMakeDropClip(bool paste);

	GUIPLATFORM_PASTECLIP_DECLS

	byte         action;
	byte         allowed;
	bool         paste;
	bool         accepted;
	Ткст       fmt;
	Ткст       data;

	void GuiPlatformConstruct();

public:
	bool   IsAvailable(const char *fmt) const;
	Ткст дай(const char *fmt) const;

	bool   прими();

	bool   прими(const char *fmt);
	Ткст дайФормат()                  { return fmt; }
	Ткст дай() const                  { return data; }
	operator Ткст() const             { return дай(); }
	Ткст operator ~() const           { return дай(); }

	void   отклони()                     { accepted = false; }

	int    GetAction() const            { return action; }
	int    GetAllowedActions() const    { return allowed; }
	void   SetAction(int x)             { action = x; }

	bool   IsAccepted() const           { return accepted; }

	bool   IsQuery() const              { return !paste; }
	bool   IsPaste() const              { return paste; }

	PasteClip();
};

Ткст  Unicode__(const ШТкст& w);
ШТкст Unicode__(const Ткст& s);

void GuiPlatformAdjustDragImage(ImageBuffer& b);

Рисунок MakeDragImage(const Рисунок& arrow, Рисунок sample);

const char *ClipFmtsText();
bool        AcceptText	(PasteClip& clip);
Ткст      дайТкст(PasteClip& clip);
ШТкст     дайШТкст(PasteClip& clip);
Ткст      GetTextClip(const Ткст& text, const Ткст& fmt);
Ткст      GetTextClip(const ШТкст& text, const Ткст& fmt);
void        приставь(ВекторМап<Ткст, ClipData>& data, const Ткст& text);
void        приставь(ВекторМап<Ткст, ClipData>& data, const ШТкст& text);

const char *ClipFmtsImage();
bool        AcceptImage(PasteClip& clip);
Рисунок       GetImage(PasteClip& clip);
Ткст      GetImageClip(const Рисунок& m, const Ткст& fmt);
void        приставь(ВекторМап<Ткст, ClipData>& data, const Рисунок& img);

bool            IsAvailableFiles(PasteClip& clip);
bool            AcceptFiles(PasteClip& clip);
Вектор<Ткст>  GetClipFiles(const Ткст& data);
Вектор<Ткст>  GetFiles(PasteClip& clip);
void            AppendFiles(ВекторМап<Ткст, ClipData>& data, const Вектор<Ткст>& files);

template <class T>
Ткст ClipFmt()
{
	return Ткст("U++ тип: ") + typeid(T).name();
}

template <class T>
bool   прими(PasteClip& clip)
{
	return clip.прими(ClipFmt<T>());
}

Ткст GetInternalDropId__(const char *тип, const char *id);
void NewInternalDrop__(const void *ptr);
const void *GetInternalDropPtr__();

template <class T>
ВекторМап<Ткст, ClipData> InternalClip(const T& x, const char *id = "")
{
	NewInternalDrop__(&x);
	ВекторМап<Ткст, ClipData> d;
	d.добавь(GetInternalDropId__(typeid(T).name(), id));
	return d;
}

template <class T>
bool IsAvailableInternal(PasteClip& d, const char *id = "")
{
	return d.IsAvailable(GetInternalDropId__(typeid(T).name(), id));
}

template <class T>
bool AcceptInternal(PasteClip& d, const char *id = "")
{
	return d.прими(GetInternalDropId__(typeid(T).name(), id));
}

template <class T>
const T& GetInternal(PasteClip& d)
{
	return *(T *)GetInternalDropPtr__();
}

template <class T>
const T *GetInternalPtr(PasteClip& d, const char *id = "")
{
	return IsAvailableInternal<T>(d, id) ? (T *)GetInternalDropPtr__() : NULL;
}

enum { PEN_DOWN = 1, PEN_UP = 2 };

struct ИнфОПере {
	int    action = 0;
	bool   barrel = false;
	bool   inverted = false;
	bool   eraser = false;
	bool   history = false;
	double pressure = Null;
	double rotation = Null;
	ТочкаПЗ tilt = Null;
};

class Ктрл : public Pte<Ктрл> {
public:
	enum PlacementConstants {
		CENTER   = 0,
		MIDDLE   = 0,
		LEFT     = 1,
		RIGHT    = 2,
		TOP      = 1,
		BOTTOM   = 2,
		SIZE     = 3,

		MINSIZE  = -16380,
		MAXSIZE  = -16381,
		STDSIZE  = -16382,
	};

	class Logc {
		dword data;

		static int LSGN(dword d)       { return int16((d & 0x7fff) | ((d & 0x4000) << 1)); }

	public:
		bool  operator==(Logc q) const { return data == q.data; }
		bool  operator!=(Logc q) const { return data != q.data; }
		int   дайЛин() const         { return (data >> 30) & 3; }
		int   GetA() const             { return LSGN(data >> 15); }
		int   дайС() const             { return LSGN(data); }
		void  устЛин(int align)      { data = (data & ~(3 << 30)) | (align << 30); }
		void  SetA(int a)              { data = (data & ~(0x7fff << 15)) | ((a & 0x7fff) << 15); }
		void  SetB(int b)              { data = (data & ~0x7fff) | (b & 0x7fff); }
		bool  пустой() const;

		Logc(int al, int a, int b)     { data = (al << 30) | ((a & 0x7fff) << 15) | (b & 0x7fff); }
		Logc()                         { data = 0xffffffff; }
	};

	struct ПозЛога : Движимое<ПозЛога> {
		Logc x, y;

		bool operator==(ПозЛога b) const   { return x == b.x && y == b.y; }
		bool operator!=(ПозЛога b) const   { return !(*this == b); }

		ПозЛога(Logc x, Logc y)            : x(x), y(y) {}
		ПозЛога()                          {}
	};

	static Logc позЛев(int pos, int size)       { return Logc(LEFT, pos, size); }
	static Logc позПрав(int pos, int size)      { return Logc(RIGHT, pos, size); }
	static Logc позВерх(int pos, int size)        { return Logc(TOP, pos, size); }
	static Logc позНиз(int pos, int size)     { return Logc(BOTTOM, pos, size); }
	static Logc позРазмер(int lpos, int rpos)      { return Logc(SIZE, lpos, rpos); }
	static Logc позЦентр(int size, int offset)  { return Logc(CENTER, offset, size); }
	static Logc позЦентр(int size)              { return Logc(CENTER, 0, size); }

	typedef bool (*ХукМыш)(Ктрл *ctrl, bool inframe, int event, Точка p,
	                          int zdelta, dword keyflags);
	typedef bool (*ХукКлав)(Ктрл *ctrl, dword ключ, int count);
	typedef bool (*ХукСост)(Ктрл *ctrl, int reason);

	static dword KEYtoK(dword);

private:
	Ктрл(Ктрл&);
	void operator=(Ктрл&);

private:
	struct Фрейм : Движимое<Фрейм> {
		КтрлФрейм *frame;
		Прям16     view;

		Фрейм()    { view.очисть(); }
	};
	Ктрл        *parent;

	struct Промот : Движимое<Промот> {
		Прям rect;
		int  dx;
		int  dy;
	};

	struct КтрлПеремест : Движимое<КтрлПеремест> {
		Ук<Ктрл>  ctrl;
		Прям       from;
		Прям       to;
	};

	friend struct ЦельЮБроса;

	struct Верх {
		GUIPLATFORM_CTRL_TOP_DECLS
		Вектор<Промот> scroll;
		ВекторМап<Ктрл *, КтрлПеремест> move;
		ВекторМап<Ктрл *, КтрлПеремест> scroll_move;
		Ук<Ктрл>      owner;
	};

	Верх         *top;
	int          exitcode;

	Ктрл        *prev, *next;
	Ктрл        *firstchild, *lastchild;//16
	ПозЛога       pos;//8
	Прям16       rect;
	Mitor<Фрейм> frame;//16
	Ткст       info;//16
	int16        caretx, carety, caretcx, caretcy;//8

	byte         overpaint;

	bool         unicode:1;

	bool         fullrefresh:1;

	bool         transparent:1;
	bool         visible:1;
	bool         enabled:1;
	bool         wantfocus:1;
	bool         initfocus:1;
	bool         activepopup:1;
	bool         editable:1;
	bool         modify:1;
	bool         ignoremouse:1;
	bool         inframe:1;
	bool         inloop:1;
	bool         isopen:1;
	bool         popup:1;
	bool         popupgrab:1;
	byte         backpaint:2;//2

	bool         akv:1;
	bool         destroying:1;

	static  Ук<Ктрл> eventCtrl;
	static  Ук<Ктрл> mouseCtrl;
	static  Точка     mousepos;
	static  Точка     leftmousepos, rightmousepos, middlemousepos;
	static  Ук<Ктрл> focusCtrl;
	static  Ук<Ктрл> focusCtrlWnd;
	static  Ук<Ктрл> lastActiveWnd;
	static  Ук<Ктрл> caretCtrl;
	static  Прям      caretRect;
	static  Ук<Ктрл> captureCtrl;
	static  bool      ignoreclick;
	static  bool      ignoremouseup;
	static  bool      ignorekeyup;
	static  bool      mouseinview;
	static  bool      mouseinframe;
	static  bool      globalbackpaint;
	static  bool      globalbackbuffer;
	static  bool      painting;
	static  int       СобытиеLevel;
	static  int       LoopLevel;
	static  Ктрл     *LoopCtrl;
	static  int64     СобытиеLoopNo;
	static  int64     EndSessionLoopNo;
	static  int64     eventid;

	static  Ук<Ктрл>           defferedSetFocus;
	static  Вектор< Ук<Ктрл> > defferedChildLostFocus;

	static  Ук<Ктрл> repeatTopCtrl;
	static  Точка     repeatMousePos;
	
	static  ИнфОПере   pen;
	static  bool      is_pen_event;

	static  Вектор<ХукМыш>& mousehook();
	static  Вектор<ХукКлав>&   keyhook();
	static  Вектор<ХукСост>& statehook();

	static Ук<Ктрл> фокусируйКтрл() { return focusCtrl; }
	static void      фокусируйКтрл(Ук<Ктрл> fc) { focusCtrl = fc; }

	void    StateDeep(int state);

	void    удалиОтпрыск0(Ктрл *q);

	static int       найдиПереместиКтрл(const ВекторМап<Ктрл *, КтрлПеремест>& m, Ктрл *x);
	static КтрлПеремест *найдиПереместиУкКтрл(ВекторМап<Ктрл *, КтрлПеремест>& m, Ктрл *x);

	Размер    PosVal(int v) const;
	void    Lay1(int& pos, int& r, int align, int a, int b, int sz) const;
	Прям    вычислиПрям(ПозЛога pos, const Прям& prect, const Прям& pview) const;
	Прям    вычислиПрям(const Прям& prect, const Прям& pview) const;
	void    обновиПрям0(bool sync = true);
	void    обновиПрям(bool sync = true);
	void    устПоз0(ПозЛога p, bool inframe);
	void    SetWndRect(const Прям& r);
	void    SyncMoves();

	static  void  EndIgnore();
	static  void  LRep();
	static  bool  NotDrag(Точка p);
	static  void  LHold();
	static  void  LRepeat();
	static  void  RRep();
	static  void  RHold();
	static  void  RRepeat();
	static  void  MRep();
	static  void  MHold();
	static  void  MRepeat();
	static  void  KillRepeat();
	static  void  CheckMouseCtrl();
	static  void  DoCursorShape();
	static  Рисунок& CursorOverride();
	bool    IsMouseActive() const;
	Рисунок   MouseEvent0(int event, Точка p, int zdelta, dword keyflags);
	Рисунок   MouseEventH(int event, Точка p, int zdelta, dword keyflags);
	Рисунок   FrameMouseEventH(int event, Точка p, int zdelta, dword keyflags);
	Рисунок   MСобытие0(int e, Точка p, int zd);
	Рисунок   DispatchMouse(int e, Точка p, int zd = 0);
	Рисунок   DispatchMouseEvent(int e, Точка p, int zd = 0);
	void    LogMouseEvent(const char *f, const Ктрл *ctrl, int event, Точка p, int zdelta, dword keyflags);

	struct CallBox;
	static void PerformCall(CallBox *cbox);

	void    StateH(int reason);

	void    RefreshAccessKeys();
	void    RefreshAccessKeysDo(bool vis);
	static  void  DefferedFocusSync();
	static  void  SyncCaret();
	static  void  RefreshCaret();
	static  bool  DispatchKey(dword keycode, int count);
	void    SetFocusWnd();
	void    KillFocusWnd();

	static Ук<Ктрл> dndctrl;
	static Точка     dndpos;
	static bool      dndframe;
	static PasteClip dndclip;

	void    ТиБ(Точка p, PasteClip& clip);
	static void DnDRepeat();
	static void DnDLeave();

	void    SyncLayout(int force = 0);
	bool    AddScroll(const Прям& sr, int dx, int dy);
	Прям    GetClippedView();
	void    ScrollRefresh(const Прям& r, int dx, int dy);
	void    промотайКтрл(Верх *top, Ктрл *q, const Прям& r, Прям cr, int dx, int dy);
	void    синхПромот();
	void    Refresh0(const Прям& area);
	void    PaintCaret(SystemDraw& w);
	void    CtrlPaint(SystemDraw& w, const Прям& clip);
	void    RemoveFullRefresh();
	bool    PaintOpaqueAreas(SystemDraw& w, const Прям& r, const Прям& clip, bool nochild = false);
	void    GatherTransparentAreas(Вектор<Прям>& area, SystemDraw& w, Прям r, const Прям& clip);
	Ктрл   *FindBestOpaque(const Прям& clip);
	void    ExcludeDHCtrls(SystemDraw& w, const Прям& r, const Прям& clip);
	void    UpdateArea0(SystemDraw& draw, const Прям& clip, int backpaint);
	void    UpdateArea(SystemDraw& draw, const Прям& clip);
	Ктрл   *дайВерхПрям(Прям& r, bool inframe, bool clip = true);
	void    DoSync(Ктрл *q, Прям r, bool inframe);
	void    SetInfoPart(int i, const char *txt);
	Ткст  GetInfoPart(int i) const;

	Прям    GetPreeditScreenRect();
	void    SyncPreedit();
	void    ShowPreedit(const ШТкст& text, int cursor = INT_MAX);
	static void HidePreedit();
	static void PreeditSync(void (*enable_preedit)(Ктрл *top, bool enable));

// System window interface...
	void WndShow(bool b);
	void WndSetPos(const Прям& rect);

	bool IsWndOpen() const;

	bool SetWndCapture();
	bool HasWndCapture() const;
	bool ReleaseWndCapture();

	static void устКурсорМыши(const Рисунок& m);

	static void DoDeactivate(Ук<Ктрл> pfocusCtrl, Ук<Ктрл> nfocusCtrl);
	static void DoKillFocus(Ук<Ктрл> pfocusCtrl, Ук<Ктрл> nfocusCtrl);
	static void DoSetFocus(Ук<Ктрл> pfocusCtrl, Ук<Ктрл> nfocusCtrl, bool activate);

	bool устФокус0(bool activate);
	void активируйОкно();
	void ClickActivateWnd();
	bool устФокусОкна();
	bool естьФокусОкна() const;

	void WndInvalidateRect(const Прям& r);

	void WndScrollView(const Прям& r, int dx, int dy);

	void устППОкна();
	bool ппОкна_ли() const;

	void WndEnable(bool b);

	Прям GetWndScreenRect() const;

	void обновиОкно();
	void обновиОкно(const Прям& r);

	void освободиОкно();
	void разрушьОкно();

	void SysEndLoop();

	Ткст имя0() const;

	static void иницТаймер();

	static Ткст appname;

	static Размер Bsize;
	static Размер Dsize;
	static Размер Csize;
	static bool IsNoLayoutZoom;
	static void Csizeinit();
	static void (*skin)();
	
	static void (*cancel_preedit)();

	friend void  InitRichTextZoom();
	friend void  AvoidPaintingCheck__();
	friend dword GetKeyStateSafe(dword what);
	friend void  CtrlSetDefaultSkin(void (*_skin)());
	friend class DHCtrl;
	friend class TopFrameDraw;
	friend class ViewDraw;
	friend class ТопОкно;
	friend class ИконкаТрея;
	friend class ГЛКтрл;
	friend class WaitCursor;
	friend struct UDropSource;
	friend class DnDAction;
	friend class PasteClip;

	typedef Ктрл ИМЯ_КЛАССА;

	void        GuiPlatformConstruct();
	void        GuiPlatformDestruct();
	void        GuiPlatformRemove();
	void        GuiPlatformGetTopRect(Прям& r) const;
	bool        GuiPlatformRefreshFrameSpecial(const Прям& r);
	bool        GuiPlatformSetFullRefreshSpecial();
	static void GuiPlatformSelection(PasteClip& d);

#ifdef GUIPLATFORM_CTRL_DECLS_INCLUDE
	#include GUIPLATFORM_CTRL_DECLS_INCLUDE
#else
	GUIPLATFORM_CTRL_DECLS
#endif

	static void InstallPanicBox();
	
	bool IsDHCtrl() const;
	
	struct СобытиеLevelDo {
		СобытиеLevelDo() { СобытиеLevel++; };
		~СобытиеLevelDo() { СобытиеLevel--; };
	};

private:
			void    DoRemove();

protected:
	static void     TimerProc(dword time);

			Ктрл&   Unicode()                         { unicode = true; return *this; }

public:
	enum StateReason {
		FOCUS      = 10,
		ACTIVATE   = 11,
		DEACTIVATE = 12,
		SHOW       = 13,
		ENABLE     = 14,
		EDITABLE   = 15,
		OPEN       = 16,
		CLOSE      = 17,
		POSITION   = 100,
		LAYOUTPOS  = 101,
	};

	enum MouseEvents {
		BUTTON        = 0x0F,
		ACTION        = 0xFF0,

		MOUSEENTER    = 0x10,
		MOUSEMOVE     = 0x20,
		MOUSELEAVE    = 0x30,
		CURSORIMAGE   = 0x40,
		MOUSEWHEEL    = 0x50,

		DOWN          = 0x80,
		UP            = 0x90,
		DOUBLE        = 0xa0,
		REPEAT        = 0xb0,
		DRAG          = 0xc0,
		HOLD          = 0xd0,
		TRIPLE        = 0xe0,
		PEN           = 0xf0,
		PENLEAVE      = 0x100,

		LEFTDOWN      = LEFT|DOWN,
		LEFTDOUBLE    = LEFT|DOUBLE,
		LEFTREPEAT    = LEFT|REPEAT,
		LEFTUP        = LEFT|UP,
		LEFTDRAG      = LEFT|DRAG,
		LEFTHOLD      = LEFT|HOLD,
		LEFTTRIPLE    = LEFT|TRIPLE,

		RIGHTDOWN     = RIGHT|DOWN,
		RIGHTDOUBLE   = RIGHT|DOUBLE,
		RIGHTREPEAT   = RIGHT|REPEAT,
		RIGHTUP       = RIGHT|UP,
		RIGHTDRAG     = RIGHT|DRAG,
		RIGHTHOLD     = RIGHT|HOLD,
		RIGHTTRIPLE   = RIGHT|TRIPLE,

		MIDDLEDOWN     = MIDDLE|DOWN,
		MIDDLEDOUBLE   = MIDDLE|DOUBLE,
		MIDDLEREPEAT   = MIDDLE|REPEAT,
		MIDDLEUP       = MIDDLE|UP,
		MIDDLEDRAG     = MIDDLE|DRAG,
		MIDDLEHOLD     = MIDDLE|HOLD,
		MIDDLETRIPLE   = MIDDLE|TRIPLE
	};

	enum {
		NOBACKPAINT,
		FULLBACKPAINT,
		TRANSPARENTBACKPAINT,
		EXCLUDEPAINT,
	};

	static  Вектор<Ктрл *> дайТопКтрлы();
	static  Вектор<Ктрл *> дайТопОкна();
	static  void   закройТопКтрлы();

	static  void   устХукМыши(ХукМыш hook);
	static  void   DeinstallMouseHook(ХукМыш hook);

	static  void   InstallKeyHook(ХукКлав hook);
	static  void   DeinstallKeyHook(ХукКлав hook);

	static  void   InstallStateHook(ХукСост hook);
	static  void   DeinstallStateHook(ХукСост hook);
	
	static  int    RegisterSystemHotKey(dword ключ, Функция<void ()> cb);
	static  void   UnregisterSystemHotKey(int id);

	virtual bool   прими();
	virtual void   отклони();
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;
	virtual void   сериализуй(Поток& s);
	virtual void   вДжейсон(ДжейсонВВ& jio);
	virtual void   вРяр(РярВВ& xio);
	virtual void   SetModify();
	virtual void   ClearModify();
	virtual bool   изменено() const;

	virtual void   рисуй(Draw& w);
	virtual int    рисуйПоверх() const;

	virtual void   режимОтмены();

	virtual void   активируй();
	virtual void   дезактивируй();
	virtual void   дезактивируйПо(Ктрл *new_focus);

	virtual Рисунок  FrameMouseEvent(int event, Точка p, int zdelta, dword keyflags);
	virtual Рисунок  MouseEvent(int event, Точка p, int zdelta, dword keyflags);
	virtual void   входМыши(Точка p, dword keyflags);
	virtual void   двигМыши(Точка p, dword keyflags);
	virtual void   леваяВнизу(Точка p, dword keyflags);
	virtual void   леваяДКлик(Точка p, dword keyflags);
	virtual void   LeftTriple(Точка p, dword keyflags);
	virtual void   леваяПовтори(Точка p, dword keyflags);
	virtual void   леваяТяг(Точка p, dword keyflags);
	virtual void   LeftHold(Точка p, dword keyflags);
	virtual void   леваяВверху(Точка p, dword keyflags);
	virtual void   праваяВнизу(Точка p, dword keyflags);
	virtual void   RightDouble(Точка p, dword keyflags);
	virtual void   RightTriple(Точка p, dword keyflags);
	virtual void   RightRepeat(Точка p, dword keyflags);
	virtual void   RightDrag(Точка p, dword keyflags);
	virtual void   RightHold(Точка p, dword keyflags);
	virtual void   RightUp(Точка p, dword keyflags);
	virtual void   MiddleDown(Точка p, dword keyflags);
	virtual void   MiddleDouble(Точка p, dword keyflags);
	virtual void   MiddleTriple(Точка p, dword keyflags);
	virtual void   MiddleRepeat(Точка p, dword keyflags);
	virtual void   MiddleDrag(Точка p, dword keyflags);
	virtual void   MiddleHold(Точка p, dword keyflags);
	virtual void   MiddleUp(Точка p, dword keyflags);
	virtual void   колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void   выходМыши();
	
	virtual void   Pen(Точка p, const ИнфОПере& pen, dword keyflags);
	
	virtual Точка  GetPreedit();
	virtual Шрифт   GetPreeditFont();

	virtual void   тягИБрос(Точка p, PasteClip& d);
	virtual void   FrameDragAndDrop(Точка p, PasteClip& d);
	virtual void   тягПовтори(Точка p);
	virtual void   тягВойди();
	virtual void   тягПокинь();
	virtual Ткст GetDropData(const Ткст& fmt) const;
	virtual Ткст GetSelectionData(const Ткст& fmt) const;

	virtual Рисунок  рисКурсора(Точка p, dword keyflags);

	virtual bool   Ключ(dword ключ, int count);
	virtual void   сфокусирован();
	virtual void   расфокусирован();
	virtual bool   горячаяКлав(dword ключ);

	virtual dword  дайКлючиДоступа() const;
	virtual void   присвойКлючиДоступа(dword used);

	virtual void   PostInput(); // Deprecated

	virtual void   ChildFrameMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags);
	virtual void   ChildMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags);
	virtual void   отпрыскФок();
	virtual void   отпрыскРасфок();
	virtual void   отпрыскДобавлен(Ктрл *child);
	virtual void   отпрыскУдалён(Ктрл *child);
	virtual void   ParentChange();

	virtual void   State(int reason);

	virtual void   Выкладка();

	virtual Размер   дайМинРазм() const;
	virtual Размер   дайСтдРазм() const;
	virtual Размер   дайМаксРазм() const;

	virtual bool   IsShowEnabled() const;

	virtual Прям   дайПлотныйПрям() const;
	virtual Прям   дайПроцПрям() const ;

	virtual void   обновлено();

	virtual void   закрой();

	virtual bool   IsOcxChild();

	virtual Ткст дайОпис() const;

	virtual void   устМинРазм(Размер sz) {}

	Событие<>          WhenAction;

	void             добавьОтпрыск(Ктрл *child);
	void             добавьОтпрыск(Ктрл *child, Ктрл *insafter);
	void             добавьОтпрыскПеред(Ктрл *child, Ктрл *insbefore);
	void             удалиОтпрыск(Ктрл *child);
	Ктрл            *дайРодителя() const           { return parent; }
	Ктрл            *GetLastChild() const        { return lastchild; }
	Ктрл            *дайПервОтпрыск() const       { return firstchild; }
	Ктрл            *дайПредш() const             { return parent ? prev : NULL; }
	Ктрл            *дайСледщ() const             { return parent ? next : NULL; }
	int              GetChildIndex(const Ктрл *child) const;
	Ктрл            *GetIndexChild(int i) const;
	int              GetChildCount() const;
	template <class T>
	T               *GetAscendant() const;

	int              GetViewChildIndex(const Ктрл *child) const;
	int              GetViewChildCount() const;
	Ктрл            *GetViewIndexChild(int ii) const;

	bool             отпрыск_ли() const             { return parent; }

	Ктрл            *отпрыскИзТочки(Точка& pt) const;

	bool             пп_ли() const;
	void             устПП();

	const Ктрл      *дайТопКтрл() const;
	Ктрл            *дайТопКтрл();
	const Ктрл      *дайВладельца() const;
	Ктрл            *дайВладельца();
	const Ктрл      *GetTopCtrlOwner() const;
	Ктрл            *GetTopCtrlOwner();

	Ктрл            *GetOwnerCtrl();
	const Ктрл      *GetOwnerCtrl() const;

	const ТопОкно *дайТопОкно() const;
	ТопОкно       *дайТопОкно();

	const ТопОкно *дайГлавнОкно() const;
	ТопОкно       *дайГлавнОкно();

	Ктрл&            устФрейм(int i, КтрлФрейм& frm);
	Ктрл&            устФрейм(КтрлФрейм& frm)            { return устФрейм(0, frm); }
	Ктрл&            добавьФрейм(КтрлФрейм& frm);
	const КтрлФрейм& дайФрейм(int i = 0) const           { return *frame[i].frame; }
	КтрлФрейм&       дайФрейм(int i = 0)                 { return *frame[i].frame; }
	void             удалиФрейм(int i);
	void             удалиФрейм(КтрлФрейм& frm);
	void             вставьФрейм(int i, КтрлФрейм& frm);
	int              найдиФрейм(КтрлФрейм& frm);
	int              дайСчётФреймов() const               { return frame.дайСчёт(); }
	void             очистьФреймы();

	bool        открыт() const;

	void        Shutdown()                               { destroying = true; }
	bool        IsShutdown() const                       { return destroying; }

	Ктрл&       устПоз(ПозЛога p, bool inframe);

	Ктрл&       устПоз(ПозЛога p);
	Ктрл&       устПоз(Logc x, Logc y)                   { return устПоз(ПозЛога(x, y)); }
	Ктрл&       SetPosX(Logc x);
	Ктрл&       SetPosY(Logc y);

	void        устПрям(const Прям& r);
	void        устПрям(int x, int y, int cx, int cy);
	void        SetRectX(int x, int cx);
	void        SetRectY(int y, int cy);

	Ктрл&       SetFramePos(ПозЛога p);
	Ктрл&       SetFramePos(Logc x, Logc y)              { return SetFramePos(ПозЛога(x, y)); }
	Ктрл&       SetFramePosX(Logc x);
	Ктрл&       SetFramePosY(Logc y);

	void        SetFrameRect(const Прям& r);
	void        SetFrameRect(int x, int y, int cx, int cy);
	void        SetFrameRectX(int x, int cx);
	void        SetFrameRectY(int y, int cy);

	bool        InFrame() const                          { return inframe; }
	bool        InView() const                           { return !inframe; }
	ПозЛога      дайПоз() const                           { return pos; }

	void        освежиВыкладку()                          { SyncLayout(1); }
	void        RefreshLayoutDeep()                      { SyncLayout(2); }
	void        освежиВыкладкуРодителя()                    { if(parent) parent->освежиВыкладку(); }
	
	void        UpdateLayout()                           { SyncLayout(); }
	void        UpdateParentLayout()                     { if(parent) parent->UpdateLayout(); }

	Ктрл&       LeftPos(int a, int size = STDSIZE);
	Ктрл&       RightPos(int a, int size = STDSIZE);
	Ктрл&       TopPos(int a, int size = STDSIZE);
	Ктрл&       BottomPos(int a, int size = STDSIZE);
	Ктрл&       HSizePos(int a = 0, int b = 0);
	Ктрл&       VSizePos(int a = 0, int b = 0);
	Ктрл&       SizePos();
	Ктрл&       HCenterPos(int size = STDSIZE, int delta = 0);
	Ктрл&       VCenterPos(int size = STDSIZE, int delta = 0);

	Ктрл&       LeftPosZ(int a, int size = STDSIZE);
	Ктрл&       RightPosZ(int a, int size = STDSIZE);
	Ктрл&       TopPosZ(int a, int size = STDSIZE);
	Ктрл&       BottomPosZ(int a, int size = STDSIZE);
	Ктрл&       HSizePosZ(int a = 0, int b = 0);
	Ктрл&       VSizePosZ(int a = 0, int b = 0);
	Ктрл&       HCenterPosZ(int size = STDSIZE, int delta = 0);
	Ктрл&       VCenterPosZ(int size = STDSIZE, int delta = 0);

	Прям        дайПрям() const;
	Прям        дайПрямЭкрана() const;

	Прям        GetView() const;
	Прям        GetScreenView() const;
	Размер        дайРазм() const;

	Прям        GetVisibleScreenRect() const;
	Прям        GetVisibleScreenView() const;

	Прям        GetWorkArea() const;

	Размер        дайРазмФрейма(int cx, int cy) const;
	Размер        дайРазмФрейма(Размер sz) const              { return дайРазмФрейма(sz.cx, sz.cy); }

	void        освежи(const Прям& r);
	void        освежи(int x, int y, int cx, int cy);
	void        освежи();
	bool        IsFullRefresh() const                    { return fullrefresh; }

	void        освежиФрейм(const Прям& r);
	void        освежиФрейм(int x, int y, int cx, int cy);
	void        освежиФрейм();
	
	static bool IsPainting()                             { return painting; }

	void        промотайОбзор(const Прям& r, int dx, int dy);
	void        промотайОбзор(int x, int y, int cx, int cy, int dx, int dy);
	void        промотайОбзор(int dx, int dy);
	void        промотайОбзор(const Прям& r, Размер delta)    { промотайОбзор(r, delta.cx, delta.cy); }
	void        промотайОбзор(Размер delta)                   { промотайОбзор(delta.cx, delta.cy); }

	void        синх();
	void        синх(const Прям& r);

	static Рисунок OverrideCursor(const Рисунок& m);

	void        DrawCtrl(Draw& w, int x = 0, int y = 0);
	void        DrawCtrlWithParent(Draw& w, int x = 0, int y = 0);

	bool    HasChild(Ктрл *ctrl) const;
	bool    HasChildDeep(Ктрл *ctrl) const;

	Ктрл&   IgnoreMouse(bool b = true)                   { ignoremouse = b; return *this; }
	Ктрл&   NoIgnoreMouse()                              { return IgnoreMouse(false); }
	bool    IsIgnoreMouse() const                        { return ignoremouse; }
	bool    естьМышь() const;
	bool    HasMouseDeep() const;
	bool    HasMouseInFrame(const Прям& r) const;
	bool    HasMouseIn(const Прям& r) const;
	Точка   GetMouseViewPos() const;
	static Ктрл *GetMouseCtrl();

	static void IgnoreMouseClick();
	static void IgnoreMouseUp();
	static void UnIgnoreMouse();

	bool    SetCapture();
	bool    ReleaseCapture();
	bool    HasCapture() const;
	static bool  ReleaseCtrlCapture();
	static Ктрл *GetCaptureCtrl();

	bool    устФокус();
	bool    естьФокус() const                   { return фокусируйКтрл() == this; }
	bool    HasFocusDeep() const;
	Ктрл&   WantFocus(bool ft = true)          { wantfocus = ft; return *this; }
	Ктрл&   NoWantFocus()                      { return WantFocus(false); }
	bool	IsWantFocus() const                { return wantfocus; }
	bool    SetWantFocus();
	Ктрл&   InitFocus(bool ft = true)          { initfocus = ft; return *this; }
	Ктрл&   NoInitFocus()                      { return InitFocus(false); }
	bool    IsInitFocus() const                { return initfocus; }
	Ктрл   *GetFocusChild() const              { return HasChild(фокусируйКтрл()) ? ~фокусируйКтрл() : NULL; }
	Ктрл   *GetFocusChildDeep() const          { return HasChildDeep(фокусируйКтрл()) ? ~фокусируйКтрл() : NULL; }

	void    CancelModeDeep();

	void    устКаретку(int x, int y, int cx, int cy);
	void    устКаретку(const Прям& r);
	Прям    дайКаретку() const;
	void    анулируйКаретку();
	
	static void  CancelPreedit();
	
	void   CancelMyPreedit()                   { if(естьФокус()) CancelPreedit(); }

	static Ктрл *GetFocusCtrl()                { return фокусируйКтрл(); }

	static Ктрл *GetСобытиеTopCtrl()             { return eventCtrl; }

	static bool  IterateFocusForward(Ктрл *ctrl, Ктрл *top, bool noframe = false, bool init = false, bool all = false);
	static bool  IterateFocusBackward(Ктрл *ctrl, Ктрл *top, bool noframe = false, bool all = false);

	static dword AccessKeyBit(int accesskey);
	dword   GetAccessKeysDeep() const;
	void    DistributeAccessKeys();
	bool    VisibleAccessKeys();

	void    покажи(bool show = true);
	void    скрой()                             { покажи(false); }
	bool    показан_ли() const                    { return visible; }
	bool    видим_ли() const;

	void    вкл(bool enable = true);
	void    откл()                          { вкл(false); }
	bool    включен_ли() const                  { return enabled; }

	Ктрл&   устРедактируем(bool editable = true);
	Ктрл&   устТолькоЧтен()                      { return устРедактируем(false); }
	bool    редактируем_ли() const                 { return editable; }
	bool    толькочтен_ли() const                 { return !editable; }

	void    ClearModifyDeep();
	bool    IsModifiedDeep() const;
	bool    IsModifySet() const                { return modify; } // deprecated

	void    обновиОсвежи();
	void    Update();
	void    Action();
	void    UpdateAction();
	void    UpdateActionRefresh();

	Ктрл&   BackPaint(int bp = FULLBACKPAINT)  { backpaint = bp; return *this; }
	Ктрл&   TransparentBackPaint()             { backpaint = TRANSPARENTBACKPAINT; return *this; }
	Ктрл&   NoBackPaint()                      { return BackPaint(NOBACKPAINT); }
	Ктрл&   BackPaintHint();
	int     GetBackPaint() const               { return backpaint; }
	Ктрл&   Transparent(bool bp = true)        { transparent = bp; return *this; }
	Ктрл&   NoTransparent()                    { return Transparent(false); }
	bool    IsTransparent() const              { return transparent; }

	Ктрл&   Info(const char *txt)              { info = txt; return *this; }
	Ткст  GetInfo() const                    { return info; }

	Ктрл&   Подсказка(const char *txt);
	Ктрл&   HelpLine(const char *txt);
	Ктрл&   Description(const char *txt);
	Ктрл&   HelpTopic(const char *txt);
	Ктрл&   LayoutId(const char *txt);

	Ткст  GetTip() const;
	Ткст  GetHelpLine() const;
	Ткст  GetDescription() const;
	Ткст  GetHelpTopic() const;
	Ткст  дайИдВыкладки() const;
	void    ClearInfo()                        { info.очисть(); }

	void    добавь(Ктрл& ctrl)                    { добавьОтпрыск(&ctrl); }
	Ктрл&   operator<<(Ктрл& ctrl)             { добавь(ctrl); return *this; }

	void    удали();

	Значение        operator~() const             { return дайДанные(); }
	const Значение& operator<<=(const Значение& v)   { устДанные(v); return v; }
	bool         экзПусто_ли() const        { return дайДанные().пусто_ли(); }

	Callback     operator<<=(Callback  action) { WhenAction = action; return action; }

	Событие<>&     operator<<(Событие<> action)    { return WhenAction << action; }
	Событие<>&     operator^=(Событие<> action)    { return WhenAction = action; }

	void    устОбрвызВремени(int delay_ms, Функция<void ()> cb, int id = 0);
	void    глушиОбрвызВремени(int id = 0);
	void    KillSetTimeCallback(int delay_ms, Функция<void ()> cb, int id);
	bool    естьОбрвызВремени(int id = 0) const;
	void    постОбрвыз(Функция<void ()> cb, int id = 0);
	void    KillPostCallback(Функция<void ()> cb, int id);
	
	enum { TIMEID_COUNT = 1 };

	static Ктрл *дайАктивныйКтрл();
	static Ктрл *GetActiveWindow();

	static Ктрл *GetVisibleChild(Ктрл *ctrl, Точка p, bool pointinframe);

	void   PopUp(Ктрл *owner = NULL, bool savebits = true, bool activate = true, bool dropshadow = false,
	             bool topmost = false);

	void   SetAlpha(byte alpha);

	static bool ожидаетСобытие();
	static bool обработайСобытие(bool *quit = NULL);
	static bool обработайСобытия(bool *quit = NULL);
	static int  GetСобытиеLevel()     { return СобытиеLevel; }

	bool   IsPopUp() const          { return popup; }


	static void  циклСобытий(Ктрл *loopctrl = NULL);
	static int   GetLoopLevel()     { return LoopLevel; }
	static Ктрл *GetLoopCtrl()      { return LoopCtrl; }

	void   EndLoop();
	void   EndLoop(int code);
	bool   InLoop() const;
	bool   InCurrentLoop() const;
	int    дайКодВыхода() const;

	static PasteClip& Clipboard();
	static PasteClip& Selection();

	void   SetSelectionSource(const char *fmts);
	
	static void RegisterDropFormats(const char *fmts); // MacOS requires drop formats to be registered

	int    DoDragAndDrop(const char *fmts, const Рисунок& sample, dword actions,
	                     const ВекторМап<Ткст, ClipData>& data);
	int    DoDragAndDrop(const char *fmts, const Рисунок& sample = Null, dword actions = DND_ALL);
	int    DoDragAndDrop(const ВекторМап<Ткст, ClipData>& data, const Рисунок& sample = Null,
	                     dword actions = DND_ALL);
	static Ктрл *GetDragAndDropSource();
	static Ктрл *GetDragAndDropTarget();
	bool   IsDragAndDropSource()    { return this == GetDragAndDropSource(); }
	bool   IsDragAndDropTarget()    { return this == GetDragAndDropTarget(); }
	static Размер  StdSampleSize()    { return Размер(DPI(126), DPI(106)); }
	
	static ИнфОПере GetPenInfo()     { return pen; }
	
public:
	static void SetSkin(void (*skin)());

	static const char *GetZoomText();
	static void SetZoomSize(Размер sz, Размер bsz = Размер(0, 0));
	static int  HorzLayoutZoom(int cx);
	static double HorzLayoutZoomf(double cx);
	static int  VertLayoutZoom(int cy);
	static Размер LayoutZoom(int cx, int cy);
	static Размер LayoutZoom(Размер sz);
	static void NoLayoutZoom();
	static void GetZoomRatio(Размер& m, Размер& d);
	
	static void SetUHDEnabled(bool set = true);
	static bool IsUHDEnabled();
	
	static void SetDarkThemeEnabled(bool set = true);
	static bool IsDarkThemeEnabled();

	static bool ClickFocus();
	static void ClickFocus(bool cf);

	static Прям   GetVirtualWorkArea();
	static Прям   GetVirtualScreenArea();
	static Прям   GetPrimaryWorkArea();
	static Прям   GetPrimaryScreenArea();
	static void   GetWorkArea(Массив<Прям>& rc);
	static Прям   GetWorkArea(Точка pt);
	static Прям   GetMouseWorkArea()                     { return GetWorkArea(дайПозМыши()); }
	static int    GetKbdDelay();
	static int    GetKbdSpeed();
	static bool   IsAlphaSupported();
	static Прям   GetDefaultWindowRect();
	static Ткст дайИмяПрил();
	static void   устИмяПрил(const Ткст& appname);
	static bool   IsCompositedGui();

	static void   GlobalBackPaint(bool b = true);
	static void   GlobalBackPaintHint();
	static void   GlobalBackBuffer(bool b = true);

	static void   ReSkin();

	Ткст        Имя() const;
	static Ткст Имя(Ктрл *ctrl);

#ifdef _ОТЛАДКА
	virtual void   Dump() const;
	virtual void   Dump(Поток& s) const;

	static bool LogMessages;
#endif

	static void ShowRepaint(int ms);

	static bool MemoryCheck;

	static void гипСпи(int ms);

	static void SetTimerGranularity(int ms);

	static void Call(Функция<void ()> cb);

	static bool шатдаунНитей_ли()                     { return Нить::шатдаунНитей_ли(); }
	static void шатдаунНитей();
	
	static int64 GetСобытиеId()                           { return eventid; }

	Ктрл();
	virtual ~Ктрл();

private: // support for for(Ктрл& q : *this)
	class КтрлКонстОбходчик {
	protected:
		friend class Ктрл;
		const Ктрл *q;
	
	public:
		void operator++()                           { q = q->дайСледщ(); }
		bool operator!=(КтрлКонстОбходчик& b) const { return q != b.q; }
		const Ктрл& operator*() const               { return *q; }
	};
	
	class КтрлОбходчик : public КтрлКонстОбходчик { // support for(Ктрл *q : *this)
		friend class Ктрл;
	
	public:
		Ктрл& operator*()                           { return *const_cast<Ктрл *>(q); }
	};

public:
	КтрлКонстОбходчик begin() const { КтрлКонстОбходчик c; c.q = дайПервОтпрыск(); return c; }
	КтрлОбходчик begin()            { КтрлОбходчик c; c.q = дайПервОтпрыск(); return c; }
	КтрлКонстОбходчик end() const   { КтрлКонстОбходчик c; c.q = NULL; return c; }
	КтрлОбходчик end()              { КтрлОбходчик c; c.q = NULL; return c; }
};

inline Размер GetScreenSize()  { return Ктрл::GetVirtualScreenArea().дайРазм(); } // Deprecated

bool   GuiPlatformHasSizeGrip();
void   GuiPlatformGripResize(ТопОкно *q);
Цвет  GuiPlatformGetScreenPixel(int x, int y);
void   GuiPlatformAfterMenuPopUp();

inline int  Zx(int cx) { return Ктрл::HorzLayoutZoom(cx); }
inline double Zxf(double cx) { return Ктрл::HorzLayoutZoomf(cx); }
inline int  Zy(int cy) { return Ктрл::VertLayoutZoom(cy); }
inline Размер Zsz(int cx, int cy) { return Размер(Zx(cx), Zy(cy)); }
inline Размер Zsz(Размер sz) { return Zsz(sz.cx, sz.cy); }
inline int  InvZx(int cx) { return 100000 * cx / Zx(100000); }
inline double InvZxf(double cx) { return 100000 * cx / Zx(100000); }

Шрифт FontZ(int face, int height = 0);

Шрифт StdFontZ(int height = 0);
Шрифт SansSerifZ(int height = 0);
Шрифт SerifZ(int height = 0);
Шрифт MonospaceZ(int height = 0);
Шрифт RomanZ(int height = 0);
Шрифт ArialZ(int height = 0);
Шрифт CourierZ(int height = 0);

Шрифт ScreenSansZ(int height = 0); // deprecated
Шрифт ScreenSerifZ(int height = 0); // deprecated
Шрифт ScreenFixedZ(int height = 0); // deprecated

int   EditFieldIsThin();
Значение TopSeparator1();
Значение TopSeparator2();
int   FrameButtonWidth();
int   ScrollBarArrowSize();
Цвет FieldFrameColor();

enum { GUISTYLE_FLAT, GUISTYLE_CLASSIC, GUISTYLE_XP, GUISTYLE_X };
int GUI_GlobalStyle();

int GUI_DragFullWindow();

enum { GUIEFFECT_NONE, GUIEFFECT_SLIDE, GUIEFFECT_FADE };
int GUI_PopUpEffect();

int GUI_ToolTips();
int GUI_ToolTipDelay();

int GUI_DropShadows();
int GUI_AltAccessKeys();
int GUI_AKD_Conservative();
int GUI_DragDistance();
int GUI_DblClickTime();
int GUI_WheelScrollLines();

void GUI_GlobalStyle_Write(int);
void GUI_DragFullWindow_Write(int);
void GUI_PopUpEffect_Write(int);
void GUI_ToolTips_Write(int);
void GUI_ToolTipDelay_Write(int);
void GUI_DropShadows_Write(int);
void GUI_AltAccessKeys_Write(int);
void GUI_AKD_Conservative_Write(int);
void GUI_DragDistance_Write(int);
void GUI_DblClickTime_Write(int);
void GUI_WheelScrollLines_Write(int);

void  EditFieldIsThin_Write(int);
void  TopSeparator1_Write(Значение);
void  TopSeparator2_Write(Значение);
void  FrameButtonWidth_Write(int);
void  ScrollBarArrowSize_Write(int);
void  FieldFrameColor_Write(Цвет);

Ткст Имя(const Ктрл *ctrl);
Ткст Desc(const Ктрл *ctrl);
void   Dump(const Ктрл *ctrl);

inline Ктрл *operator<<(Ктрл *parent, Ктрл& child)
{
	parent->добавь(child);
	return parent;
}

inline hash_t дайХэшЗнач(Ктрл *x)
{
	return (hash_t)(intptr_t)x;
}

Ткст GetKeyDesc(dword ключ);

Вектор< Ук<Ктрл> > отклКтрлы(const Вектор<Ктрл *>& ctrl, Ктрл *exclude = NULL);
void вклКтрлы(const Вектор< Ук<Ктрл> >& ctrl);

template <class T>
class ФреймКтрл : public T, public КтрлФрейм {
public:
	virtual void добавьКФрейму(Ктрл& parent) { parent.добавь(*this); }
	virtual void удалиФрейм()          { this->Ктрл::удали(); }

	ФреймКтрл()                         { this->NoWantFocus(); }
};

template <class T>
class FrameLR : public ФреймКтрл<T> {
public:
	virtual void добавьРазмФрейма(Размер& sz) { sz.cx += (cx ? cx : sz.cy) * this->показан_ли(); }

protected:
	int cx;

public:
	FrameLR& устШирину(int _cx)             { cx = _cx; this->освежиВыкладкуРодителя(); return *this; }
	int      дайШирину() const           { return cx; }
	FrameLR()                           { cx = 0; }
};

template <class T>
class ФреймЛево : public FrameLR<T> {
public:
	virtual void выложиФрейм(Прям& r) {
		выложиФреймСлева(r, this, this->cx ? this->cx : FrameButtonWidth());
	}
};

template <class T>
class ФреймПраво : public FrameLR<T> {
public:
	virtual void выложиФрейм(Прям& r) {
		выложиФреймСправа(r, this, this->cx ? this->cx : FrameButtonWidth());
	}
};

template <class T>
class FrameTB : public ФреймКтрл<T> {
public:
	virtual void добавьРазмФрейма(Размер& sz) { sz.cy += (cy ? cy : sz.cx) * this->показан_ли(); }

protected:
	int cy;

public:
	FrameTB& устВысоту(int _cy)            { cy = _cy; this->освежиВыкладкуРодителя(); return *this; }
	int      дайВысоту() const          { return cy; }
	FrameTB()                           { cy = 0; }
};

template <class T>
class ФреймВерх : public FrameTB<T> {
public:
	virtual void выложиФрейм(Прям& r) {
		выложиФреймСверху(r, this, this->cy ? this->cy : r.устШирину());
	}
};

template <class T>
class ФреймНиз : public FrameTB<T> {
public:
	virtual void выложиФрейм(Прям& r) {
		выложиФреймСнизу(r, this, this->cy ? this->cy : r.устШирину());
	}
};

class Modality {
	Ук<Ктрл>           active;
	bool                fore_only;
	Вектор< Ук<Ктрл> > enable;

public:
	void старт(Ктрл *modal, bool fore_only = false);
	void стоп();

	~Modality()      { стоп(); }
};

class LocalLoop : public Ктрл {
public:
	virtual void режимОтмены();

private:
	Ктрл *master;

public:
	void  пуск();
	void  SetMaster(Ктрл& m)      { master = &m; }
	Ктрл& GetMaster() const       { return *master; }

	LocalLoop()                   { master = NULL; }
};

enum {
	DRAWDRAGRECT_SOLID, DRAWDRAGRECT_NORMAL, DRAWDRAGRECT_DASHED
};

bool PointLoop(Ктрл& ctrl, const Вектор<Рисунок>& ani, int ani_ms);
bool PointLoop(Ктрл& ctrl, const Рисунок& img);

class ПрямТрэкер : public LocalLoop {
public:
	virtual void  леваяВверху(Точка, dword);
	virtual void  RightUp(Точка, dword);
	virtual void  двигМыши(Точка p, dword);
	virtual void  Pen(Точка p, const ИнфОПере &pn, dword);
	virtual Рисунок рисКурсора(Точка, dword);
	virtual void  рисуй(Draw& w);

public:
	struct Rounder {
		virtual Прям Round(const Прям& r) = 0;
	};

protected:
	Рисунок           master_image;

	Прям            rect;
	int             tx, ty;
	Прям            maxrect;
	Размер            minsize, maxsize;
	bool            keepratio;
	Прям            clip;
	Цвет           color;
	Рисунок           cursorimage;
	int             width;
	int             pattern;
	int             animation;
	int             panim;
	Rounder        *rounder;

	Прям            org;
	Прям            o;
	Точка           op;

	Прям            Round(const Прям& r);

	virtual void    RefreshRect(const Прям& old, const Прям& r);
	virtual void    DrawRect(Draw& w, Прям r1);

public:
	Событие<Прям>  sync;
	Событие<Прям&> round;

	ПрямТрэкер&    SetCursorImage(const Рисунок& m) { cursorimage = m; return *this; }
	ПрямТрэкер&    минРазм(Размер sz)               { minsize = sz; return *this; }
	ПрямТрэкер&    максРазм(Размер sz)               { maxsize = sz; return *this; }
	ПрямТрэкер&    максПрям(const Прям& mr)        { maxrect = mr; return *this; }
	ПрямТрэкер&    Clip(const Прям& c)            { clip = c; return *this; }
	ПрямТрэкер&    устШирину(int n)                   { width = n; return *this; }
	ПрямТрэкер&    устЦвет(Цвет c)              { color = c; return *this; }
	ПрямТрэкер&    образец(int p)                 { pattern = p; return *this; }
	ПрямТрэкер&    Dashed()                       { return образец(DRAWDRAGRECT_DASHED); }
	ПрямТрэкер&    Solid()                        { return образец(DRAWDRAGRECT_SOLID); }
	ПрямТрэкер&    Normal()                       { return образец(DRAWDRAGRECT_NORMAL); }
	ПрямТрэкер&    Animation(int step_ms = 40)    { animation = step_ms; return *this; }
	ПрямТрэкер&    KeepRatio(bool b)              { keepratio = b; return *this; }
	ПрямТрэкер&    Round(Rounder& r)              { rounder = &r; return *this; }

	Прям            дай()                          { return rect; }

	Прям  Track(const Прям& r, int tx = ALIGN_RIGHT, int ty = ALIGN_BOTTOM);
	int   TrackHorzLine(int x0, int y0, int cx, int line);
	int   TrackVertLine(int x0, int y0, int cy, int line);
	Точка TrackLine(int x0, int y0);

	ПрямТрэкер(Ктрл& master);
};

class WaitCursor {
	Рисунок   prev;
	bool    flag;

public:
	void    покажи();

	WaitCursor(bool show = true);
	~WaitCursor();
};

class AutoWaitCursor : public WaitCursor {
protected:
	int&   avg;
	int    time0;

public:
	void Cancel()                   { time0 = 0; }

	AutoWaitCursor(int& avg);
	~AutoWaitCursor();
};

void    ClearClipboard();
void    AppendClipboard(const char *формат, const byte *data, int length);
void    AppendClipboard(const char *формат, const Ткст& data);
void    AppendClipboard(const char *формат, const Значение& data, Ткст (*render)(const Значение& data));
void    AppendClipboard(const char *формат, const ClipData& data);
void    AppendClipboard(const ВекторМап<Ткст, ClipData>& data);
Ткст  ReadClipboard(const char *формат);
bool    IsClipboardAvailable(const char *формат);

inline  void WriteClipboard(const char *формат, const Ткст& data)
	{ ClearClipboard(); AppendClipboard(формат, data); }

void    AppendClipboardText(const Ткст& s);
Ткст  ReadClipboardText();
void    AppendClipboardUnicodeText(const ШТкст& s);
ШТкст ReadClipboardUnicodeText();
bool    IsClipboardAvailableText();

inline  void WriteClipboardText(const Ткст& s)
	{ ClearClipboard(); AppendClipboardText(s); }
inline  void WriteClipboardUnicodeText(const ШТкст& s)
	{ ClearClipboard(); AppendClipboardUnicodeText(s); }

template <class T>
inline void AppendClipboardFormat(const T& object) {
	AppendClipboard(typeid(T).name(), сохраниКакТкст(const_cast<T&>(object)));
}

template <class T>
inline void WriteClipboardFormat(const T& object) {
	ClearClipboard();
	AppendClipboardFormat(object);
}

template <class T>
inline bool ReadClipboardFormat(T& object)
{
	Ткст s = ReadClipboard(typeid(T).name());
	return !пусто_ли(s) && грузиИзТкст(object, s);
}

template <class T>
bool IsClipboardFormatAvailable()
{
	return IsClipboardAvailable(typeid(T).name());
}

template <class T>
inline T ReadClipboardFormat() {
	T object;
	ReadClipboardFormat(object);
	return object;
}

Рисунок  ReadClipboardImage();
void   AppendClipboardImage(const Рисунок& img);

inline void WriteClipboardImage(const Рисунок& img)
	{ ClearClipboard(); AppendClipboardImage(img); }

bool (*&DisplayErrorFn())(const Значение& v);
inline bool DisplayError(const Значение& v) { return DisplayErrorFn()(v); }

const char *ClipFmtsRTF();

void       EncodeRTF(Поток& stream, const RichText& richtext, byte charset,
	Размер dot_page_size = Размер(4960, 7015), const Прям& dot_margin = Прям(472, 472, 472, 472),
	void *context = NULL);
Ткст     EncodeRTF(const RichText& richtext, byte charset,
	Размер dot_page_size = Размер(4960, 7015), const Прям& dot_margin = Прям(472, 472, 472, 472),
	void *context = NULL);
Ткст     EncodeRTF(const RichText& richtext, byte charset, int dot_page_width);
Ткст     EncodeRTF(const RichText& richtext);
RichText   ParseRTF(const char *rtf);

void WriteClipboardHTML(const Ткст& html);

#include <CtrlCore/TopWindow.h>

#include GUIPLATFORM_INCLUDE_AFTER

template <class T>
T *Ктрл::GetAscendant() const
{
	for(Ктрл *p = дайРодителя(); p; p = p->дайРодителя())
		if(T *a = dynamic_cast<T*>(p))
			return a;
	return NULL;
}

#ifdef HAS_TopFrameDraw

class ViewDraw : public TopFrameDraw {
public:
	ViewDraw(Ктрл *ctrl, const Прям& r);
	ViewDraw(Ктрл *ctrl) : ViewDraw(ctrl, ctrl->дайРазм()) {}
	ViewDraw(Ктрл *ctrl, int x, int y, int cx, int cy) : ViewDraw(ctrl, RectC(x, y, cx, cy)) {}
};

#endif

}

#endif
