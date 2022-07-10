class DockTabBar : public БарТаб { 
public:
	typedef DockTabBar ИМЯ_КЛАССА;

	Обрвыз1<int> 	WhenContext;
	Обрвыз1<int>  WhenDrag;
	
	virtual void 	добавьРазмФрейма(Размер& sz);
	virtual void	выложиФрейм(Прям& r);	
	bool 			автоСкрой_ли() const				{ return дайСчёт() <= autohide; }

	DockTabBar& 	AutoHideMin(int hidemin = 1)	{ autohide = hidemin; return *this; }
	DockTabBar& 	сИконками(bool b = true)			{ icons = b; return *this; }

	void			SyncRepos()						{ Repos(); }
	void			ShowText(bool show)				{ showtext = show; }

	DockTabBar();
protected:
	int autohide;
	bool icons:1;
	bool showtext:1;

	virtual Размер дайСтдРазм(const Вкладка &t);

	virtual void праваяВнизу(Точка p, dword keyflags);
	virtual void леваяВнизу(Точка p, dword keyflags)	{ БарТаб::леваяВнизу(p, keyflags &= ~K_SHIFT); }
	virtual void леваяВверху(Точка p, dword keyflags)	{ БарТаб::леваяВверху(p, keyflags &= ~K_SHIFT); }
	virtual void леваяТяг(Точка p, dword keyflags);
};

class DockCont;

class AutoHideBar : public DockTabBar {
public:
	typedef	AutoHideBar ИМЯ_КЛАССА;

	virtual void входМыши(Точка p, dword keyflags);	
	virtual void выходМыши();	

	void 	добавьКтрл(DockCont& c, const Ткст& группа = Null);
	int 	FindCtrl(const DockCont& c) const;
	DockCont *дайКтрл(int ix)	const				{ return ValueTo<DockCont *>(дайКлюч(ix));  }	
	void	RemoveCtrl(int ix);
	void	RemoveCtrl(DockCont& c)					{ return RemoveCtrl(c, FindCtrl(c)); }
	void	RemoveCtrl(DockCont& c, int ix);
	bool 	HasCtrl(const DockCont& c) const		{ return (FindCtrl(c) >= 0); }
	
	void	ShowAnimate(Ктрл *c);	
	
	static void SetTimeout(int delay_ms)	  		{ ПРОВЕРЬ(delay_ms > 0); autohide_timeout = delay_ms; }
	
	AutoHideBar();

	virtual void ComposeTab(Вкладка& tab, const Шрифт &font, Цвет ink, int style);
	
private:
	static int autohide_timeout;

	struct HidePopup : public Ктрл 	{
		HidePopup() { BackPaint(); }
		Callback WhenEnter;
		Callback WhenLeave;
		virtual void ChildMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags); 
	};

	enum { TIMEID_ACTION_CHECK = Ктрл::TIMEID_COUNT,
		   TIMEID_HIDE_TIMEOUT,
           TIMEID_COUNT };
	Ктрл 		*ctrl;
	HidePopup 	 popup;
	
	void 	TabDrag(int ix);
	void 	TabHighlight();	
	void	TabClose(Значение v);				
	void	HideAnimate(Ктрл *c);
	void 	настройРазм(Прям& r, const Размер& sz);
};

struct DockTabable
{
	virtual Значение GetSortValue() = 0;
};
