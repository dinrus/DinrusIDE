class ПаноБара : public КтрлРодитель {
public:
	virtual void леваяВнизу(Точка pt, dword keyflags);
	virtual void двигМыши(Точка p, dword);

private:
	struct Элемент {
		Ктрл *ctrl;
		int   gapsize;
	};
	Массив<Элемент>    элт;
	Вектор<int>    breakpos;
	bool           horz;
	Ктрл          *phelpctrl;
	int            vmargin, hmargin;
	bool           menu;

	Размер     LayOut(bool horz, int maxsize, bool repos);

public:
	Событие<>  ПриЛевКлике;

	void  PaintBar(Draw& w, const КтрлСепаратор::Стиль& ss,
	               const Значение& pane, const Значение& iconbar = Null, int iconsz = 0);

	void  IClear();
	void  очисть();
	bool  пустой() const                    { return элт.пустой(); }

	void  добавь(Ктрл *ctrl, int gapsize);
	void  AddBreak()                         { добавь(NULL, Null); }
	void  AddGap(int gapsize)                { добавь(NULL, gapsize); }

	void  Margin(int v, int h)               { vmargin = v; hmargin = h; }

	Размер  Repos(bool horz, int maxsize);
	Размер  GetPaneSize(bool _horz, int maxsize) const;

	int   дайСчёт() const                   { return breakpos.дайСчёт() + 1; }

	void  SubMenu()                          { menu = true; }

	ПаноБара();
	virtual ~ПаноБара();
};

class Бар : public Ктрл {
public:
	struct Элемент {
		virtual Элемент& устТекст(const char *text);
		virtual Элемент& Ключ(dword ключ);
		virtual Элемент& повтори(bool repeat = true);
		virtual Элемент& Рисунок(const class Рисунок& img);
		virtual Элемент& Check(bool check);
		virtual Элемент& Radio(bool check);
		virtual Элемент& вкл(bool _enable = true);
		virtual Элемент& Bold(bool bold = true);
		virtual Элемент& Подсказка(const char *tip);
		virtual Элемент& Help(const char *help);
		virtual Элемент& Topic(const char *topic);
		virtual Элемент& Description(const char *desc);
		virtual void  FinalSync();

		Элемент&   Надпись(const char *text);
		Элемент&   RightLabel(const char *text);

		Элемент& Ключ(ИнфОКлюче& (*ключ)());

		Элемент();
		virtual ~Элемент();
	};

protected:
	virtual Элемент&  добавьЭлт(Событие<>  cb) = 0;
	virtual Элемент&  добавьСубМеню(Событие<Бар&> proc) = 0;
	virtual void   добавьКтрл(Ктрл *ctrl, int gapsize) = 0;
	virtual void   добавьКтрл(Ктрл *ctrl, Размер sz) = 0;

	class ScanKeys;
	
	friend class БарМеню;

public:
	virtual bool   пустой() const = 0;
	virtual void   Separator() = 0;

	static  Элемент&  NilItem();
	static bool    скан(Событие<Бар&> proc, dword ключ);

	void   Break();
	void   Gap(int size = 8);
	void   GapRight()                               { Gap(INT_MAX); }

	void   AddNC(Ктрл& ctrl);
	void   добавь(Ктрл& ctrl)                          { добавьКтрл(&ctrl, ctrl.дайМинРазм()); }
	void   добавь(Ктрл& ctrl, Размер sz)                 { добавьКтрл(&ctrl, sz); }
	void   добавь(Ктрл& ctrl, int cx, int cy = 0)      { добавьКтрл(&ctrl, Размер(cx, cy)); }

	void   добавь(bool en, Ктрл& ctrl)                     { добавь(ctrl); ctrl.вкл(en); }
	void   добавь(bool en, Ктрл& ctrl, Размер sz)            { добавь(ctrl, sz); ctrl.вкл(en); }
	void   добавь(bool en, Ктрл& ctrl, int cx, int cy = 0) { добавь(ctrl, cx, cy); ctrl.вкл(en); }

	Элемент&  добавь(bool enable, const char *text, const РНЦП::Рисунок& image, const Callback& callback);
	Элемент&  добавь(bool enable, const Ткст& text, const РНЦП::Рисунок& image, const Callback& callback) { return добавь(enable, ~text, image, callback); }
	Элемент&  добавь(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Callback& callback);
	Элемент&  добавь(const char *text, const РНЦП::Рисунок& image, const Callback& callback);
	Элемент&  добавь(const Ткст& text, const РНЦП::Рисунок& image, const Callback& callback) { return добавь(~text, image, callback); }
	Элемент&  добавь(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Callback& callback);

	Элемент&  добавь(bool enable, const char *text, const Callback& callback);
	Элемент&  добавь(bool enable, const Ткст& text, const Callback& callback) { return добавь(enable, ~text, callback); }
	Элемент&  добавь(bool enable, ИнфОКлюче& (*ключ)(), const Callback& callback);
	Элемент&  добавь(const char *text, const Callback& callback);
	Элемент&  добавь(const Ткст& text, const Callback& callback) { return добавь(~text, callback); }
	Элемент&  добавь(ИнфОКлюче& (*ключ)(), const Callback& callback);

	Элемент&  добавь(bool enable, const char *text, const РНЦП::Рисунок& image, const Функция<void ()>& фн);
	Элемент&  добавь(bool enable, const Ткст& text, const РНЦП::Рисунок& image, const Функция<void ()>& фн) { return добавь(enable, ~text, image, фн); }
	Элемент&  добавь(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Функция<void ()>& фн);
	Элемент&  добавь(const char *text, const РНЦП::Рисунок& image, const Функция<void ()>& фн);
	Элемент&  добавь(const Ткст& text, const РНЦП::Рисунок& image, const Функция<void ()>& фн) { return добавь(~text, image, фн); }
//	Элемент&  добавь(const Ткст& text, const РНЦП::Рисунок& image, const Функция<void ()>& фн);
	Элемент&  добавь(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Функция<void ()>& фн);

	Элемент&  добавь(bool enable, const char *text, const Функция<void ()>& фн);
	Элемент&  добавь(bool enable, const Ткст& text, const Функция<void ()>& фн) { return добавь(enable, ~text, фн); }
	Элемент&  добавь(bool enable, ИнфОКлюче& (*ключ)(), const Функция<void ()>& фн);
	Элемент&  добавь(const char *text, const Функция<void ()>& фн);
	Элемент&  добавь(const Ткст& text, const Функция<void ()>& фн) { return добавь(~text, фн); }
	Элемент&  добавь(ИнфОКлюче& (*ключ)(), const Функция<void ()>& фн);

	void   MenuSeparator();
	void   MenuBreak();
	void   MenuGap(int size = 8);
	void   MenuGapRight()                           { MenuGap(INT_MAX); }

	void   добавьМеню(Ктрл& ctrl);
	void   добавьМеню(Ктрл& ctrl, Размер sz);
	void   добавьМеню(Ктрл& ctrl, int cx, int cy = 0)  { добавьМеню(ctrl, Размер(cx, cy)); }

	Элемент&  добавьМеню(bool enable, const char *text, const РНЦП::Рисунок& image, const Callback& callback);
	Элемент&  добавьМеню(bool enable, const Ткст& text, const РНЦП::Рисунок& image, const Callback& callback) { return добавьМеню(enable, ~text, image, callback); }
	Элемент&  добавьМеню(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Callback& callback);
	Элемент&  добавьМеню(const char *text, const РНЦП::Рисунок& image, const Callback& callback);
	Элемент&  добавьМеню(const Ткст& text, const РНЦП::Рисунок& m, const Callback& c) { return добавьМеню(~text, m, c); }
	Элемент&  добавьМеню(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& m, const Callback& c);

	Элемент&  добавьМеню(bool enable, const char *text, const РНЦП::Рисунок& image, const Функция<void ()>& фн);
	Элемент&  добавьМеню(bool enable, const Ткст& text, const РНЦП::Рисунок& image, const Функция<void ()>& фн) { return добавьМеню(enable, ~text, image, фн); }
	Элемент&  добавьМеню(bool enable, ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& image, const Функция<void ()>& фн);
	Элемент&  добавьМеню(const char *text, const РНЦП::Рисунок& image, const Функция<void ()>& фн);
	Элемент&  добавьМеню(const Ткст& text, const РНЦП::Рисунок& m, const Функция<void ()>& фн) { return добавьМеню(~text, m, фн); }
	Элемент&  добавьМеню(ИнфОКлюче& (*ключ)(), const РНЦП::Рисунок& m, const Функция<void ()>& фн);

	Элемент&  добавь(bool enable, const char *text, const Обрвыз1<Бар&>& proc);
	Элемент&  добавь(bool enable, const Ткст& text, const Обрвыз1<Бар&>& proc) { return добавь(enable, ~text, proc); }
	Элемент&  добавь(const char *text, const Обрвыз1<Бар&>& proc);
	Элемент&  добавь(const Ткст& text, const Обрвыз1<Бар&>& proc) { return добавь(~text, proc); }
	Элемент&  добавь(bool enable, const char *text, const РНЦП::Рисунок& image, const Обрвыз1<Бар&>& proc);
	Элемент&  добавь(bool enable, const Ткст& text, const РНЦП::Рисунок& image, const Обрвыз1<Бар&>& proc) { return добавь(enable, ~text, proc); }
	Элемент&  добавь(const char *text, const РНЦП::Рисунок& image, const Обрвыз1<Бар&>& proc);
	Элемент&  добавь(const Ткст& text, const РНЦП::Рисунок& image, const Обрвыз1<Бар&>& proc) { return добавь(~text, image, proc); }
	Элемент&  Sub(bool enable, const char *text, const Функция<void (Бар&)>& submenu);
	Элемент&  Sub(bool enable, const Ткст& text, const Функция<void (Бар&)>& submenu) { return Sub(enable, ~text, submenu); }
	Элемент&  Sub(const char *text, const Функция<void (Бар&)>& submenu);
	Элемент&  Sub(const Ткст& text, const Функция<void (Бар&)>& submenu) { return Sub(~text, submenu); }
	Элемент&  Sub(bool enable, const char *text, const РНЦП::Рисунок& image, const Функция<void (Бар&)>& submenu);
	Элемент&  Sub(bool enable, const Ткст& text, const РНЦП::Рисунок& image, const Функция<void (Бар&)>& submenu) { return Sub(enable, ~text, image, submenu); }
	Элемент&  Sub(const char *text, const РНЦП::Рисунок& image, const Функция<void (Бар&)>& submenu);
	Элемент&  Sub(const Ткст& text, const РНЦП::Рисунок& image, const Функция<void (Бар&)>& submenu) { return Sub(~text, image, submenu); }

	void   ToolSeparator();
	void   ToolBreak();
	void   ToolGap(int size = 8);
	void   ToolGapRight()                           { ToolGap(INT_MAX); }

	void   добавьИнстр(Ктрл& ctrl);
	void   добавьИнстр(Ктрл& ctrl, Размер sz);
	void   добавьИнстр(Ктрл& ctrl, int cx, int cy = 0)  { добавьИнстр(ctrl, Размер(cx, cy)); }

	Элемент&  добавь(const РНЦП::Рисунок& image, Событие<>  callback);
	Элемент&  добавь(bool enable, const РНЦП::Рисунок& image, Событие<>  callback);

	virtual void добавьКлюч(dword ключ, Событие<>  cb);
	        void добавьКлюч(ИнфОКлюче& (*ключ)(), Событие<>  cb);

	virtual bool барМеню_ли() const                  { return false; }
	virtual bool барИнстр_ли() const                  { return false; }
	virtual bool IsScanKeys() const                 { return false; }
	virtual bool IsScanHelp() const                 { return false; }

	typedef Бар ИМЯ_КЛАССА;
	Бар();
	virtual ~Бар();
};

class КтрлБар : public Бар, public КтрлФрейм {
public:
	virtual void   Выкладка();

	virtual void   выложиФрейм(Прям& r);
	virtual void   добавьРазмФрейма(Размер& sz);
	virtual void   добавьКФрейму(Ктрл& ctrl);
	virtual void   удалиФрейм();

	virtual bool   пустой() const;
	virtual void   Separator();

protected:
	virtual void   добавьКтрл(Ктрл *ctrl, int gapsize);
	virtual void   добавьКтрл(Ктрл *ctrl, Размер sz);

private:
	class КтрлРазмер : public КтрлРодитель {
	public:
		virtual Размер дайМинРазм() const;

	private:
		Размер size;

	public:
		void  устРазм(Размер sz)             { size = sz; }

		КтрлРазмер()                         { NoWantFocus(); }
	};

	int                  sii;
	Массив<КтрлСепаратор> separator;
	int                  zii;
	Массив<КтрлРазмер>      sizer;
	int                  align;

protected:
	ПаноБара pane;
	int     ssize;
	int     wrap;
	int     lsepm, rsepm;
	const КтрлСепаратор::Стиль *sepstyle;

	void     SyncBar();

	void     IClear();
	void     IFinish();

	void     очисть();

	virtual Значение дайФон() const;

	friend class ПаноБара;

public:
	Событие<const Ткст&> WhenHelp;
	Событие<>  ПриЛевКлике;

	static КтрлБар *GetBarCtrlParent(Ктрл *child);
	static void     SendHelpLine(Ктрл *q);
	static void     ClearHelpLine(Ктрл *q);

	enum {
		BAR_LEFT, BAR_RIGHT, BAR_TOP, BAR_BOTTOM
	};

	void  PaintBar(Draw& w, const КтрлСепаратор::Стиль& ss,
	               const Значение& pane, const Значение& iconbar = Null, int iconsz = 0);

	int      дайВысоту() const           { return pane.GetPaneSize(true, INT_MAX).cy; }

	КтрлБар& Align(int align);
	КтрлБар& верх()                       { return Align(BAR_TOP); }
	КтрлБар& низ()                    { return Align(BAR_BOTTOM); }
	КтрлБар& лево()                      { return Align(BAR_LEFT); }
	КтрлБар& право()                     { return Align(BAR_RIGHT); }
	int      дайЛин() const            { return align; }

	КтрлБар& Wrap(int q = 1)             { wrap = q; SyncBar(); return *this; }
	КтрлБар& NoWrap()                    { return Wrap(-1); }

	typedef КтрлБар ИМЯ_КЛАССА;

	КтрлБар();
	virtual ~КтрлБар();
};

class MenuItemBase;

КтрлФрейм& MenuFrame();

class БарМеню : public КтрлБар {
public:
	virtual void  леваяВнизу(Точка, dword);
	virtual bool  Ключ(dword ключ, int count);
	virtual bool  горячаяКлав(dword ключ);
	virtual void  отпрыскФок();
	virtual void  отпрыскРасфок();
	virtual void  откл();
	virtual void  режимОтмены();
	virtual void  рисуй(Draw& w);
	virtual bool  барМеню_ли() const                  { return true; }
	virtual bool  пустой() const;
	virtual void  Separator();

protected:
	virtual Элемент& добавьЭлт(Событие<> cb);
	virtual Элемент& добавьСубМеню(Событие<Бар&> proc);
	virtual Значение дайФон() const;

public:
	struct Стиль : ChStyle<Стиль> {
		Значение элт; // hot menu элт background in popup menu
		Значение topitem[3]; // top menu элт background normal/hot/pressed
		Значение topbar; // deprecated
		Цвет menutext; // normal state popup menu элт text
		Цвет itemtext; // hot state popup menu элт text
		Цвет topitemtext[3]; // top menu элт text normal/hot/pressed
		КтрлСепаратор::Стиль breaksep; // separator between menu bars
		Значение look; // top menu background
		Значение arealook; // top menu backgroung if arealook and in frame (can be null, then 'look')
		Значение popupframe; // static frame of whole popup menu
		Значение popupbody; // background of whole popup menu
		Значение popupiconbar; // if there is special icon background in popup menu
		КтрлСепаратор::Стиль separator;
		Размер  maxiconsize; // limit of icon size
		int   leftgap; // between left border and icon
		int   textgap;
		int   lsepm;
		int   rsepm;
		Точка pullshift; // offset of submenu popup
		bool  opaquetest; // If true, topmenu элт can change hot text color
		Значение icheck; // background of Check (or Radio) элт with image
	};

private:
	Массив<MenuItemBase> элт;

	БарМеню     *parentmenu;
	БарМеню     *submenu;
	Ктрл        *submenuitem;
	Ук<Ктрл>    restorefocus;
	bool         doeffect;
	Шрифт         font;
	int          leftgap;
	int          lock;
	const Стиль *style;
	int          arealook;
	Размер         maxiconsize;
	LookFrame    frame;
	bool         nodarkadjust;
	bool         action_taken = false; // local menu resulted in action invoked (not cancel)

#ifdef GUI_COCOA
	Один<Бар>     host_bar;
	bool ExecuteHostBar(Ктрл *owner, Точка p);
	void CreateHostBar(Один<Бар>& bar);
#endif

	friend class MenuItemBase;
	friend class ЭлтМеню;
	friend class SubMenuBase;
	friend class TopSubMenuItem;
	friend class ЭлтСубМеню;

	void     SetParentMenu(БарМеню *parent)    { parentmenu = parent; style = parent->style; }
	БарМеню *GetParentMenu()                   { return parentmenu; }
	void     SetActiveSubmenu(БарМеню *sm, Ктрл *menuitem);
	БарМеню *GetActiveSubmenu()                { return submenu; }
	БарМеню *GetMasterMenu();
	БарМеню *GetLastSubmenu();
	void     DelayedClose();
	void     KillDelayedClose();
	void     SubmenuClose();
	void     PostDeactivate();
	void     SyncState();
	void     SetupRestoreFocus();
	void     PopUp(Ктрл *owner, Точка p, Размер rsz = Размер(0, 0));

protected:
	enum {
		TIMEID_STOP = КтрлБар::TIMEID_COUNT,
		TIMEID_SUBMENUCLOSE,
		TIMEID_POST,
		TIMEID_COUNT
	};

public:
	Событие<>  WhenSubMenuOpen;
	Событие<>  WhenSubMenuClose;

	static int GetStdHeight(Шрифт font = StdFont());

	void     закройМеню();

	void     уст(Событие<Бар&> menu);
	void     пост(Событие<Бар&> bar);

	void     PopUp(Точка p)                         { PopUp(дайАктивныйКтрл(), p); }
	void     PopUp()                                { PopUp(дайПозМыши()); }

	bool     выполни(Ктрл *owner, Точка p);
	bool     выполни(Точка p)                       { return выполни(дайАктивныйКтрл(), p); }
	bool     выполни()                              { return выполни(дайПозМыши()); }

	static bool выполни(Ктрл *owner, Событие<Бар&> proc, Точка p);
	static bool выполни(Событие<Бар&> proc, Точка p)  { return выполни(дайАктивныйКтрл(), proc, p); }
	static bool выполни(Событие<Бар&> proc)           { return выполни(proc, дайПозМыши()); }

	void     очисть();

	static const Стиль& дефСтиль();

	БарМеню& LeftGap(int cx)                        { leftgap = cx; return *this; }
	БарМеню& устШрифт(Шрифт f)                        { font = f; return *this; }
	БарМеню& устСтиль(const Стиль& s);
	Шрифт     дайШрифт() const                        { return font; }
	БарМеню& AreaLook(int q = 1)                    { arealook = q; освежи(); return *this; }
	БарМеню& MaxIconSize(Размер sz)                   { maxiconsize = sz; return *this; }
	Размер     GetMaxIconSize() const                 { return maxiconsize; }
	БарМеню& NoDarkAdjust(bool b = true)            { nodarkadjust = b; return *this; }
#ifdef GUI_COCOA
	БарМеню& UppMenu()                              { host_bar.очисть(); return *this; }
#endif

	typedef БарМеню ИМЯ_КЛАССА;

	БарМеню();
	virtual ~БарМеню();
};

class ToolButton : public Ктрл, public Бар::Элемент {
	using Ктрл::Ключ;

public:
	virtual void   рисуй(Draw& w);
	virtual void   входМыши(Точка, dword);
	virtual void   выходМыши();
	virtual Размер   дайМинРазм() const;
	virtual void   леваяВнизу(Точка, dword);
	virtual void   леваяПовтори(Точка, dword);
	virtual void   леваяВверху(Точка, dword);
	virtual bool   горячаяКлав(dword ключ);
	virtual Ткст дайОпис() const;
	virtual int    рисуйПоверх() const;

	virtual Бар::Элемент& устТекст(const char *text);
	virtual Бар::Элемент& Ключ(dword ключ);
	virtual Бар::Элемент& повтори(bool repeat = true);
	virtual Бар::Элемент& Рисунок(const РНЦП::Рисунок& img);
	virtual Бар::Элемент& вкл(bool _enable = true);
	virtual Бар::Элемент& Подсказка(const char *tip);
	virtual Бар::Элемент& Help(const char *help);
	virtual Бар::Элемент& Topic(const char *help);
	virtual Бар::Элемент& Description(const char *desc);
	virtual Бар::Элемент& Radio(bool check);
	virtual Бар::Элемент& Check(bool check);
	virtual void       FinalSync();

public:
	struct Стиль : ChStyle<Стиль> {
		Значение  look[6];
		Шрифт   font;
		Цвет  textcolor[6];
		bool   light[6];
		int    contrast[6];
		Точка  offset[6];
		int    overpaint;
	};

protected:
	Ткст  text;
	Ткст  tiptext;
	dword   accel;
	bool    checked;
	bool    paint_checked;
	bool    repeat;

	byte    kind;
	Размер    minsize;
	Размер    maxiconsize;
	bool    nodarkadjust;

	const Стиль      *style;

private:
	РНЦП::Рисунок img;

	void       SendHelpLine();
	void       ClearHelpLine();
	void       UpdateTip();

public:
	enum Kind { NOLABEL, RIGHTLABEL, BOTTOMLABEL };

	void  ResetKeepStyle();
	void  переустанов();

	static const Стиль& дефСтиль();
	static const Стиль& StyleSolid();

	bool		IsChecked()				 { return checked; }
	РНЦП::Рисунок  GetImage() const;

	ToolButton& устСтиль(const Стиль& s);
	ToolButton& минРазм(Размер sz)         { minsize = sz; return *this; }
	ToolButton& MaxIconSize(Размер sz)     { maxiconsize = sz; return *this; }
	ToolButton& Kind(int _kind);
	ToolButton& Надпись(const char *text, int kind);
	ToolButton& Надпись(const char *text);
	ToolButton& NoDarkAdjust(bool b = true) { nodarkadjust = b; return *this; }

	ToolButton();
	virtual ~ToolButton();
};

void PaintBarArea(Draw& w, Ктрл *x, const Значение& look, int bottom = Null);

class ToolBar : public КтрлБар {
public:
	virtual bool горячаяКлав(dword ключ);
	virtual void рисуй(Draw& w);

protected:
	virtual Элемент& добавьЭлт(Событие<>  cb);
	virtual Элемент& добавьСубМеню(Событие<Бар&> proc);

public:
	struct Стиль : ChStyle<Стиль> {
		ToolButton::Стиль    buttonstyle;
		Размер                 buttonminsize;
		Размер                 maxiconsize;
		int                  buttonkind;
		Значение                look, arealook;
		КтрлСепаратор::Стиль breaksep;
		КтрлСепаратор::Стиль separator;
	};

private:
	int               ii;
	Массив<ToolButton> элт;
	int               lock;
	Событие<Бар&>   proc;
	const Стиль      *style;
	int               arealook;

	Размер              buttonminsize;
	Размер              maxiconsize;
	int               kind;
	bool              nodarkadjust;

protected:
	enum {
		TIMEID_POST = КтрлБар::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	virtual bool барИнстр_ли() const                  { return true; }

	static int GetStdHeight();

	void очисть();
	void уст(Событие<Бар&> bar);
	void пост(Событие<Бар&> bar);

	static const Стиль& дефСтиль();

	ToolBar& устСтиль(const Стиль& s)               { style = &s; освежи(); return *this; }

	ToolBar& ButtonMinSize(Размер sz)                 { buttonminsize = sz; return *this; }
	ToolBar& MaxIconSize(Размер sz)                   { maxiconsize = sz; return *this; }
	ToolBar& ButtonKind(int _kind)                  { kind = _kind; return *this; }
	ToolBar& AreaLook(int q = 1)                    { arealook = q; освежи(); return *this; }
	ToolBar& NoDarkAdjust(bool b = true)            { nodarkadjust = b; return *this; }

	typedef ToolBar  ИМЯ_КЛАССА;

	ToolBar();
	virtual ~ToolBar();
};

class StaticBarArea : public Ктрл {
public:
	virtual void рисуй(Draw& w);

private:
	bool upperframe;

public:
	StaticBarArea& UpperFrame(bool b) { upperframe = b; освежи(); return *this; }
	StaticBarArea& NoUpperFrame()     { return UpperFrame(false); }

	StaticBarArea();
};

class LRUList {
	Вектор<Ткст> lru;
	int            limit;
	void           выдели(Ткст s, Событие<const Ткст&> WhenSelect);

public:
	static int GetStdHeight();

	void        сериализуй(Поток& stream);

	void        operator()(Бар& bar, Событие<const Ткст&> WhenSelect, int count = INT_MAX, int from = 0);

	void        NewEntry(const Ткст& path);
	void        RemoveEntry(const Ткст& path);

	int         дайСчёт() const                        { return lru.дайСчёт(); }

	LRUList&    Limit(int _limit)                       { limit = _limit; return *this; }
	int         GetLimit() const                        { return limit; }

	typedef LRUList ИМЯ_КЛАССА;

	LRUList()   { limit = 6; }
};

class ToolTip : public Ктрл {
public:
	virtual void рисуй(Draw& w);
	virtual Размер дайМинРазм() const;

private:
	Ткст  text;

public:
	void   уст(const char *_text)        { text = _text; }
	Ткст дай() const                   { return text; }

	void PopUp(Ктрл *owner, Точка p, bool effect);

	ToolTip();
};

void PerformDescription();
