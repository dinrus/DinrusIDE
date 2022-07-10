//$ namespace РНЦП {
//$ class Ктрл {
	bool         ignoretakefocus:1;
protected:
	struct XWindow {
		Ук<Ктрл>    ctrl;
		bool         exposed;
		Вектор<Прям> invalid;
		Ук<Ктрл>    owner;
		Ук<Ктрл>    last_active;
		XIC          xic;
	};

private:
	static МассивМап<Window, XWindow>& Xwindow();
	static int       WndCaretTime;
	static bool      WndCaretVisible;
	static int       Xbuttons;
	static int       Xbuttontime;
	static Точка     Xbuttonpos;
	static Window    grabWindow, focusWindow;
	static Точка     mousePos;
	static int       PopupGrab;
	static Ук<Ктрл> popupWnd;
	static Индекс<Ткст> sel_formats;
	static Ук<Ктрл>     sel_ctrl;
	static void     обработайСобытие(XСобытие *event);
	static void     TimerAndPaint();
	static void     обработайСобытие(XСобытие& event);
	       void     инивалидируй(XWindow& xw, const Прям& r);
	static void     анимируйКаретку();
	       void     DoPaint(const Вектор<Прям>& invalid);
	       void     SetLastActive(XWindow *w, Ктрл *la);
	       XWindow *GetXWindow();
	static void     SyncMousePos();
	static void     ReleaseGrab();
	static Вектор<Событие<> > hotkey;
	static Вектор<dword> modhot;
	static Вектор<dword> keyhot;

	       void  StartPopupGrab();
	static void  EndPopupGrab();
	static void  SyncIMPosition();

	friend bool  дайПравуюМыши();
	friend bool  дайЛевуюМыши();
	friend bool  дайСреднююМыши();
	friend Точка дайПозМыши();
	friend void  sPanicMessageBox(const char *title, const char *text);

protected:
	       void   создай(Ктрл *owner, bool redirect, bool savebits);
	       void   SyncExpose();
	       void   TakeFocus();
	static Window GetXServerFocusWindow();
	       void   AddGlobalRepaint();
	static void   KillFocus(Window w);
	static void   FocusSync();

	       void DropСобытие(XWindow& w, XСобытие *event);
	static void DropStatusСобытие(XСобытие *event);
	static Индекс<Ткст> drop_formats;
	static Ткст  Unicode(const ШТкст& w);
	static ШТкст Unicode(const Ткст& s);
	static bool   ClipHas(int тип, const char *fmt);
	static Ткст ClipGet(int тип, const char *fmt);

	       XWindow *AddXWindow(Window &w);
	       void RemoveXWindow(Window &w);
	       XWindow *XWindowFromWindow(Window &w);

public:
	struct Xclipboard {
		Window win;

		ВекторМап<int, ClipData> data;

		Ткст читай(int fmt, int selection, int property);
		void   пиши(int fmt, const ClipData& data);
		bool   IsAvailable(int fmt, const char *тип);

		void   очисть()                     { data.очисть(); }
		void   Request(XSelectionRequestСобытие *se);

		Xclipboard();
		~Xclipboard();
	};

	static Xclipboard& xclipboard();

	static int  Xeventtime;

	static XIM  xim;

	void ТиБ(Window src, bool paste);

	virtual void    СобытиеProc(XWindow& w, XСобытие *event);
	virtual bool    HookProc(XСобытие *event);
	Window  GetWindow() const         { return top ? top->window : None; }
	static  Ктрл   *CtrlFromWindow(Window w);
	static bool    TrapX11Errors();
	static void    UntrapX11Errors(bool b);

	Window GetParentWindow(void) const;
	Ктрл *GetParentWindowCtrl(void) const;
	Прям GetRectInParentWindow(void) const;

	static void SyncNativeWindows(void);
public:
	static void InitX11(const char *дисплей);
	static void ExitX11();
	static void GuiFlush()                              { XFlush(Xdisplay); }
//$ }};
