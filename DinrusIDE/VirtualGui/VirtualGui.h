#ifdef PLATFORM_POSIX
#include <CtrlCore/stdids.h>
#endif

namespace РНЦП {

#define IMAGECLASS FBImg
#define IMAGEFILE <VirtualGui/FB.iml>
#include <Draw/iml_header.h>

class SystemDraw : public DrawProxy {
public:
	bool    CanSetSurface()                         { return false; }
	static void слей()                             {}
};

enum KM {
	KM_NONE  = 0x00,

	KM_LSHIFT= 0x01,
	KM_RSHIFT= 0x02,
	KM_LCTRL = 0x04,
	KM_RCTRL = 0x08,
	KM_LALT  = 0x10,
	KM_RALT  = 0x20,

	KM_CAPS  = 0x40,
	KM_NUM   = 0x80,
	
	KM_CTRL = KM_LCTRL | KM_RCTRL,
	KM_SHIFT = KM_LSHIFT | KM_RSHIFT,
	KM_ALT = KM_LALT | KM_RALT,
};

enum GUI_OPTIONS {
	GUI_SETMOUSECURSOR = 0x01,
	GUI_SETCARET       = 0x02,
};

struct VirtualGui {
	virtual dword       GetOptions();
	virtual Размер        дайРазм() = 0;
	virtual dword       GetMouseButtons() = 0;
	virtual dword       GetModKeys() = 0;
	virtual bool        IsMouseIn() = 0;
	virtual bool        обработайСобытие(bool *quit) = 0;
	virtual void        WaitСобытие(int ms) = 0;
	virtual void        WakeUpGuiThread() = 0;
	virtual void        устКурсорМыши(const Рисунок& image);
	virtual void        устКаретку(const Прям& caret);
	virtual void        Quit() = 0;
	virtual bool        ожидаетСобытие() = 0;
	virtual SystemDraw& BeginDraw() = 0;
	virtual void        CommitDraw() = 0;
};

void RunVirtualGui(VirtualGui& gui, Событие<> app_main);

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

class ImageDraw : public SImageDraw { // using software renderer
public:
	ImageDraw(Размер sz) : SImageDraw(sz) {}
	ImageDraw(int cx, int cy) : SImageDraw(cx, cy) {}
};

void DrawDragRect(SystemDraw& w, const Прям& rect1, const Прям& rect2, const Прям& clip, int n,
                  Цвет color, uint64 pattern);

class TopWindowFrame;

#define GUIPLATFORM_CTRL_TOP_DECLS   Ктрл *owner_window;

#define GUIPLATFORM_CTRL_DECLS_INCLUDE <VirtualGui/Ктрл.h>

#define GUIPLATFORM_PASTECLIP_DECLS \
	bool dnd; \
	friend struct DnDLoop; \

#define GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE <VirtualGui/верх.h>

class PrinterJob { // Dummy only...
	NilDraw             nil_;
	Вектор<int>         pages;

public:
	Draw&               GetDraw()                       { return nil_; }
	operator            Draw&()                         { return GetDraw(); }
	const Вектор<int>&  GetPages() const                { return pages; }
	int                 operator[](int i) const         { return 0; }
	int                 GetPageCount() const            { return 0; }

	bool                выполни()                       { return false; }

	PrinterJob& Landscape(bool b = true)                { return *this; }
	PrinterJob& MinMaxPage(int minpage, int maxpage)    { return *this; }
	PrinterJob& PageCount(int n)                        { return *this; }
	PrinterJob& CurrentPage(int currentpage)            { return *this; }
	PrinterJob& Имя(const char *_name)                 { return *this; }

	PrinterJob(const char *имя = NULL)                 {}
	~PrinterJob()                                       {}
};

}

#define GUIPLATFORM_INCLUDE_AFTER <VirtualGui/After.h>
