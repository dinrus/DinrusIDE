enum {
	IDEXIT = 9999
};

class ТопОкно : public Ктрл {
public:
	virtual Размер     дайМинРазм() const;
	virtual Размер     дайСтдРазм() const;
	virtual void     активируй();
	virtual void     откл();
	virtual bool     прими();
	virtual void     отклони();
	virtual void     рисуй(Draw& w);
	virtual bool     IsShowEnabled() const;
	virtual bool     Ключ(dword ключ, int count);
	virtual void     закрой();
	virtual Ткст   дайОпис() const;
	virtual void     отпрыскФок();
	virtual void     устМинРазм(Размер sz);

public:
	struct ТопСтиль : ChStyle<ТопСтиль> {
		Значение background;
	};

protected:
	enum {
		TIMEID_DEFSYNCTITLE = Ктрл::TIMEID_COUNT,
		TIMEID_COUNT,
	};

private:
	struct Abreak : Pte<Abreak> {
		int         ИД;
		ТопОкно *dlg;

		void    прими()      { dlg->AcceptBreak(ИД); }
		void    отклони()      { dlg->RejectBreak(ИД); }
		void    Break()       { dlg->Break(ИД); }
	};

	Массив<Abreak> action;
	Ук<Ктрл>     activefocus;
	Ткст        backup;
	РисПрям     background;
	Размер          minsize;
	bool          dokeys;
	bool          fullscreen;

	byte          center:2;

	void          PlaceFocus();
	void          ActiveFocus0(Ктрл& ctrl);
	Abreak       *FindAddAction(int ИД);
	Abreak       *FindAction(int ИД);

	Прям        overlapped;

	void        SyncSizeHints();
	void        SyncTitle();
	void        SyncCaption();

	void        SetupRect(Ктрл *owner);
	
	void        FixIcons();

	void        DefaultBreak();

	enum { MINIMIZED, MAXIMIZED, OVERLAPPED };

	ШТкст     title;
	bool        minimizebox:1;
	bool        maximizebox:1;
	bool        noclosebox:1;
	bool        sizeable:1;
	bool        tool:1;
	bool        frameless:1;
	bool        urgent:1;
	bool        close_rejects:1;
	byte        state;
	Рисунок       icon, largeicon;

	const ТопСтиль *st;
	
	void        GuiPlatformConstruct();
	void        GuiPlatformDestruct();

#ifdef GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE
	#include GUIPLATFORM_TOPWINDOW_DECLS_INCLUDE
#else
	GUIPLATFORM_TOPWINDOW_DECLS
#endif

public:
	virtual     void ShutdownWindow();

	Событие<>     WhenClose;

	void        Backup();
	void        Restore();

	void        Break(int ИД = IDEXIT);
	bool        AcceptBreak(int ИД);
	void        RejectBreak(int ИД);

	void        WorkAreaTrim();

	Callback    Breaker(int ИД = IDEXIT);
	Callback    Acceptor(int ИД);
	Callback    Rejector(int ИД);

	ТопОкно&  Breaker(Ктрл& m, int ИД = -1);
	ТопОкно&  Acceptor(Ктрл& m, int ИД);
	ТопОкно&  Rejector(Ктрл& m, int ИД);

	ТопОкно&  NoCenter()                          { center = 0; return *this; }
	ТопОкно&  CenterOwner()                       { center = 1; return *this; }
	ТопОкно&  CenterScreen()                      { center = 2; return *this; }

	void       открой(Ктрл *owner);
	void       открой();
	void       откройГлавн();
	int        пуск(bool appmodal = false);
	int        пускПрилМодально()                          { return пуск(true); }
	int        выполни();
	bool       выполниОК()                            { return выполни() == IDOK;     }
	bool       выполниОтмена()                        { return выполни() == IDCANCEL; }

	void       сверни(bool effect = false);
	void       разверни(bool effect = false);
	void       нахлёст(bool effect = false);

	bool       развёрнуто() const                    { return state == MAXIMIZED; }
	bool       свёрнуто() const                    { return state == MINIMIZED; }
	bool       IsOverlapped() const                   { return state == OVERLAPPED; }

	ТопОкно& ActiveFocus(Ктрл& ctrl)                { ActiveFocus0(ctrl); return *this; }

	ТопОкно& Титул(const ШТкст& _title);
	ТопОкно& Титул(const char *s);
	const ШТкст& дайТитул() const                   { return title; }
	ТопОкно& Sizeable(bool b = true);
	ТопОкно& NoSizeable()                           { return Sizeable(false); }
	bool       IsSizeable() const                     { return sizeable; }
	ТопОкно& MinimizeBox(bool b = true);
	ТопОкно& MaximizeBox(bool b = true);
	ТопОкно& Zoomable(bool b = true)                { MinimizeBox(b); return MaximizeBox(b); }
	ТопОкно& NoZoomable()                           { return Zoomable(false); }
	bool       IsZoomable() const                     { return maximizebox; }
	ТопОкно& фон(const РисПрям& prect);
	const РисПрям& дайФон() const            { return background; }
	ТопОкно& ToolWindow(bool b = true);
	ТопОкно& NoToolWindow()                         { return ToolWindow(false); }
	bool       IsToolWindow() const                   { return tool; }
	ТопОкно& наиверхнее(bool b = true, bool stay_top = true);
	ТопОкно& NoTopMost()                            { return наиверхнее(false); }
	bool       наиверхнее_ли() const;
	ТопОкно& полноэкранно(bool b = true);
	bool       IsFullScreen() const                   { return fullscreen; }
	ТопОкно& FrameLess(bool b = true);
	bool       IsFrameLess() const                    { return frameless; }
	ТопОкно& Urgent(bool b = true);
	bool       IsUrgent() const                       { return urgent; }
	ТопОкно& NoAccessKeysDistribution()             { dokeys = false; return *this; }
	ТопОкно& NoCloseBox(bool b = true)              { noclosebox = b; return *this; }
	ТопОкно& CloseBoxRejects(bool b = true)         { close_rejects = b; return *this; }

	ТопОкно& Иконка(const Рисунок& m);
	ТопОкно& LargeIcon(const Рисунок& m);
	ТопОкно& Иконка(const Рисунок& smallicon, const Рисунок& largeicon);

	static const ТопСтиль& дефСтиль();
	ТопОкно&  устСтиль(const ТопСтиль& s);
	
	Рисунок      дайИконку() const                        { return icon; }
	Рисунок      GetLargeIcon() const                   { return largeicon; }

	void       SerializePlacement(Поток& s, bool reminimize = false);
	
	static void ShutdownWindows();

	typedef ТопОкно ИМЯ_КЛАССА;

	ТопОкно();
	~ТопОкно();
};

void Maxisize(ТопОкно& win, int screencxmax);

void ArrangeOKCancel(Ктрл& ok, Ктрл& cancel);

int  SwapOKCancel();
void SwapOKCancel_Write(int b);

void SetLayout_Size(Ктрл& ctrl, Размер sz);
void SetLayout_Size(Ктрл& ctrl, int cx, int cy);

template <class L>
void InitLayout(Ктрл& ctrl, L& layout)
{
	InitLayout(ctrl, layout, layout, layout);
}

template <class T>
void CtrlLayout(T& ctrl) {
	InitLayout(ctrl, ctrl, ctrl, ctrl);
	Размер sz = T::GetLayoutSize();
	SetLayout_Size(ctrl, sz);
	ctrl.устПрям(sz);
}

template <class T>
void CtrlLayout(T& ctrl, const char *title) {
	CtrlLayout(ctrl);
	ctrl.Титул(title);
}

template <class T>
void CtrlLayoutOK(T& ctrl, const char *title) {
	CtrlLayout(ctrl, title);
	ctrl.Acceptor(ctrl.ok, IDOK);
	ctrl.ok.Ok();
}

template <class T>
void CtrlLayoutCancel(T& ctrl, const char *title) {
	CtrlLayout(ctrl, title);
	ctrl.Rejector(ctrl.cancel, IDCANCEL);
	ctrl.cancel.Cancel();
}

template <class T>
void CtrlLayoutOKCancel(T& ctrl, const char *title) {
	CtrlLayoutOK(ctrl, title);
	ctrl.Rejector(ctrl.cancel, IDCANCEL);
	ctrl.cancel.Cancel();
	ArrangeOKCancel(ctrl.ok, ctrl.cancel);
}

template <class T>
void CtrlLayoutExit(T& ctrl, const char *title) {
	CtrlLayout(ctrl, title);
	ctrl.Acceptor(ctrl.exit, IDEXIT);
	ctrl.exit.выход();
}
