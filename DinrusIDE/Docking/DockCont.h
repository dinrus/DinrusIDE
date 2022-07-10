class ОкноДок;

struct ImgButton : public Толкач {
	private:
		const Значение *look;
		Рисунок img;
	public:		
		ImgButton()										{ Transparent(true); }
		virtual void рисуй(Draw& w);
		virtual void входМыши(Точка p, dword kflags) 	{ освежи(); Толкач::входМыши(p, kflags); }
		virtual void выходМыши() 						{ освежи(); Толкач::выходМыши(); }

		ImgButton& 	устРисунок(const Рисунок& _img) 		{ img = _img; освежи(); return *this; }
		ImgButton& 	SetLook(const Значение *_look) 		{ look = _look; освежи(); return *this; }
};

class DockCont : public ToolWin {
public:
	typedef DockCont ИМЯ_КЛАССА;

	virtual void праваяВнизу(Точка p, dword keyflags) 	{ WindowMenu(); }
	
	virtual void Выкладка();
	virtual void отпрыскУдалён(Ктрл *child);
	virtual void отпрыскДобавлен(Ктрл *child);
	virtual bool Ключ(dword ключ, int count);


/*	virtual void отпрыскФок() 						{ handle.освежиФокус(true); ТопОкно::отпрыскФок(); }
	virtual void отпрыскРасфок() 						{ handle.освежиФокус(HasFocusDeep()); ТопОкно::отпрыскРасфок(); }
	virtual void сфокусирован() 							{ handle.освежиФокус(true); }
	virtual void расфокусирован() 							{ handle.освежиФокус(HasFocusDeep()); } */
public:
	enum DockState {
		STATE_NONE = -1,
		STATE_FLOATING,
		STATE_DOCKED, 
		STATE_AUTOHIDE,
		STATE_TABBED
	};

private:
	class DockContMenu : public DockMenu {
	public:
		typedef DockContMenu ИМЯ_КЛАССА;
		DockCont *cont;
		
		DockContMenu(ОкноДок *dockwindow = NULL) : DockMenu(dockwindow) { };
		
		void ContainerMenu(Бар& bar, DockCont *c, bool withgroups  = true);
	private:		
		virtual void MenuDock(int align, DockableCtrl *dc);
		virtual void MenuFloat(DockableCtrl *dc);
		virtual void MenuAutoHide(int align, DockableCtrl *dc);	
		virtual void MenuClose(DockableCtrl *dc);	
	};	
	friend class DockContMenu;
		
	class DockContFrame : public КтрлФрейм {
		virtual void выложиФрейм(Прям& r)                   { r.дефлируй(2); }
		virtual void рисуйФрейм(Draw& w, const Прям& r) {
			DrawFrame(w, r, SColorShadow());
			DrawFrame(w, r.дефлят(1), SColorPaper);
		}
		virtual void добавьРазмФрейма(Размер& sz) { sz += 4; }
	};		
		
	struct DockContHandle : public ФреймКтрл<Ктрл> {
		Callback WhenContext;
		Callback WhenLeftDrag;
		Callback WhenLeftDouble;
		DockContHandle() 										{ dc = NULL; /*focus = false;*/ }
		DockableCtrl *dc;
//		bool focus;
		virtual void выложиФрейм(Прям& r);
		virtual void добавьРазмФрейма(Размер& sz);
		
		virtual void рисуй(Draw& w);
		virtual void леваяТяг(Точка p, dword keyflags)	{ /*освежиФокус(true);*/ WhenLeftDrag(); }
		virtual void леваяДКлик(Точка p, dword keyflags) { /*освежиФокус(true);*/ WhenLeftDouble(); }
		virtual void праваяВнизу(Точка p, dword keyflags) { /*освежиФокус(true);*/ WhenContext(); }
		
		//void	освежиФокус(bool _focus);
		int		GetHandleSize(const DockableCtrl::Стиль& s) const;
	};
	
	DockState		dockstate;	
	DockTabBar 		tabbar;
	DockContHandle 	handle;
	ImgButton 		close, autohide, windowpos;	
	Размер 			usersize;
	bool 			waitsync:1;	
	bool			animating:1;
	ОкноДок *	base;
	const DockableCtrl::Стиль *style;

	// Callbacks
	// Вкладка callbacks
	void 	TabSelected();
	void	TabDragged(int ix);
	void	TabContext(int ix);
	void	TabClosed0(Значение v);								
	void	TabClosed(Значение v);								
	void	TabsClosed(МассивЗнач vv);
	// Menus/Buttons
	void 	Float();
	void 	Dock(int align);
	void 	автоСкрой();
	void 	AutoHideAlign(int align);
	void	RestoreCurrent();

	void 	CloseAll();

	DockableCtrl *Get0(int ix) const;
	DockableCtrl *GetCurrent0() const							{ return Get0(tabbar.дайКурсор()); }
	
	void 	AddRemoveButton(Ктрл& c, bool state);
	bool 	IsDockAllowed0(int align, const Значение& v) const;
	void	SyncButtons(DockableCtrl& dc);
	Ктрл   *FindFirstChild() const;
	void	ChildTitleChanged(Ктрл *child, ШТкст title, Рисунок icon);

	void	State(ОкноДок& dock, DockCont::DockState state);	

	static Ктрл            *CtrlCast(const Значение& v)  	{ return IsDockCont(v) ? (Ктрл *)ContCast(v) : (Ктрл *)DockCast(v); }
	static DockCont        *ContCast(const Значение& v)  	{ return ValueTo<DockCont *>(v); } 
	static DockableCtrl    *DockCast(const Значение& v)  	{ return ValueTo<DockableCtrl *>(v); }
	static bool			    IsDockCont(const Значение& v) 	{ return IsType<DockCont *>(v); }
	Ктрл                   *дайКтрл(int ix) const 		{ return CtrlCast(tabbar.дайКлюч(ix)); }
	static Значение 			ValueCast(DockableCtrl *dc) { return RawToValue<DockableCtrl *>(dc); }
	static Значение 			ValueCast(DockCont *dc)  	{ return RawToValue<DockCont *>(dc); }
public:
	void 			устКурсор(int ix)					{ tabbar.устКурсор(ix); TabSelected(); }	
	void 			устКурсор(Ктрл& c);
	int 			дайКурсор()	const					{ return tabbar.дайКурсор(); }
	DockableCtrl& 	дай(int ix) const					{ return *Get0(ix); }
	DockableCtrl& 	GetCurrent() const					{ return дай(tabbar.дайКурсор()); }
	void 			AddFrom(DockCont& cont, int except = -1);
	int				дайСчёт() const					{ return tabbar.дайСчёт(); }
	void 			очисть();	

	void 			сортируйТабы(bool b);
	void 			сортируйТабы(ПорядокЗнач &sorter);
	void 			сортируйТабыРаз();
	void 			сортируйТабыРаз(ПорядокЗнач &sorter);

	virtual void 	MoveBegin();
	virtual void 	Moving();
	virtual void 	MoveEnd();		
	virtual void 	WindowMenu();	
		
	void 			анимируй(Прям target, int ticks, int interval = 0);
		
	bool 			IsDocked() const			{ return dockstate == STATE_DOCKED; }
	int				GetDockAlign() const;		
	int				GetAutoHideAlign() const;		
	bool			IsFloating() const			{ return dockstate == STATE_FLOATING; }
	bool 			автоСкрой_ли() const			{ return dockstate == STATE_AUTOHIDE; }	
	bool			IsTabbed() const			{ return dockstate == STATE_TABBED; }
	bool 			IsDockedAny() const 		{ return dockstate != STATE_FLOATING && dockstate != STATE_NONE; }
	bool			скрыт_ли() const			{ return dockstate == STATE_NONE; }	
	DockState		GetDockState() const		{ return dockstate; }
	
	void			StateNotDocked(ОкноДок *dock = NULL) 	{ if (dock) base = dock; dockstate = STATE_NONE; }
	void			StateDocked(ОкноДок& dock)				{ State(dock, STATE_DOCKED); }
	void 			StateFloating(ОкноДок& dock)				{ State(dock, STATE_FLOATING); Титул(дайТитул()); }
	void			StateAutoHide(ОкноДок& dock)				{ State(dock, STATE_AUTOHIDE); скрой(); }
	void			StateTabbed(ОкноДок& dock)				{ State(dock, STATE_TABBED); скрой(); }	
	
	void			SetAllDockerPos();
	
	ОкноДок     *GetDockWindow() const			{ return base; }
	void			SyncButtons()					{ if (дайСчёт()) SyncButtons(GetCurrent()); }
	void			синхТабы(int align, bool text);				
	void 			Замок(bool lock);
	void 			SyncFrames();
	void 			SyncFrames(bool hidehandle);
	void			SignalStateChange();	
	void			SignalStateChange(int from, int to);	
	
	void 			сГруппингом(bool grouping)			{ tabbar.сГруппингом(grouping); GroupRefresh(); }
	void			GroupRefresh();	
	void			GetGroups(Вектор<Ткст>& groups);
	
	void			WindowButtons(bool menu, bool hide, bool close);

	void			Highlight();
	Рисунок 			GetHighlightImage();
	
	virtual Размер	дайМинРазм() const;
	virtual Размер	дайМаксРазм() const;
	virtual Размер	дайСтдРазм() const;	
	Размер			GetUserSize() const				{ return usersize; }
	void 			SetUserSize(Размер sz)			{ usersize = sz; }
	void			SyncUserSize(bool h, bool v);		

	ШТкст 		дайТитул(bool force_count = false) const; 
	void			ChildTitleChanged(DockableCtrl &dc);
	void			ChildTitleChanged(DockCont &dc);
	
	bool			IsDockAllowed(int align, int dc_ix = -1) const;
		
	virtual void 	сериализуй(Поток& s);
	
	DockCont();		
};
