struct Navigator {
	virtual int GetCurrentLine() = 0;

	struct NavLine : Движимое<NavLine> {
		bool           impl:1;
		int            file:31;
		int            line;
		
		bool operator<(const NavLine& b) const;
	};
	
	struct NavItem {
		Ткст          nest;
		Ткст          qitem;
		Ткст          имя;
		Ткст          uname;
		Ткст          natural;
		Ткст          тип;
		Ткст          pname;
		Ткст          ptype;
		Ткст          tname;
		Ткст          ctname;
		byte            access;
		byte            kind;
		int16           at;
		int             line;
		int             file;
		int             decl_line; // header position
		int             decl_file;
		bool            impl;
		bool            decl;
		int8            pass;
		Вектор<NavLine> linefo;
		
		void уст(const CppItem& m);
	};
	
	enum KindEnum { KIND_LINE = 123, KIND_NEST, KIND_FILE, KIND_SRCFILE };
	
	struct ScopeDisplay : Дисплей {
		Navigator *navigator;

		int DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
		virtual Размер дайСтдРазм(const Значение& q) const;
	};
	
	struct LineDisplay : Дисплей {
		int DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style, int x) const;
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
		virtual Размер дайСтдРазм(const Значение& q) const;
	};

	struct NavigatorDisplay : Дисплей {
		const Вектор<NavItem *>& элт;
	
		int DoPaint(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
		virtual void PaintBackground(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;
		virtual Размер дайСтдРазм(const Значение& q) const;
	
		NavigatorDisplay(const Вектор<NavItem *>& элт) : элт(элт) {}
	};

	Иср             *theide;

	Массив<NavItem>                             nitem;
	ВекторМап<Ткст, Вектор<NavItem *> >      gitem;
	Вектор<NavItem *>                          litem;
	Массив<NavItem>                             nest_item;
	ВекторМап<int, SortedVectorMap<int, int> > linefo;
	NavigatorDisplay navidisplay;
	bool             navigating;
	ОбрвызВремени     search_trigger;
	bool             navigator_global;
	ToolButton       sortitems;
	bool             sorting;
	bool             dlgmode;

	КтрлМассив         scope;
	КтрлМассив         list;
	КтрлМассив         navlines;
	EditString        search;
	
	ScopeDisplay      scope_display;
	
	void TriggerSearch();
	void NavGroup(bool local);
	void ищи();
	void Scope();
	void ListLineEnabled(int i, bool& b);
	void NaviSort();

	Вектор<NavLine> GetNavLines(const NavItem& m);

	void           Navigate();
	void           ScopeDblClk();
	void           NavigatorClick();
	void           NavigatorEnter();
	void           SyncLines();
	void           SyncNavLines();
	void           GoToNavLine();
	void           SyncCursor();

	static bool SortByLines(const NavItem *a, const NavItem *b);
	static bool SortByNames(const NavItem *a, const NavItem *b);
	
	typedef Navigator ИМЯ_КЛАССА;

	Navigator();
};

struct AssistEditor : РедакторКода, Navigator {
	virtual bool Ключ(dword ключ, int count);
	virtual void расфокусирован();
	virtual void колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void SelectionChanged();
	virtual void DirtyFrom(int line);
	virtual void State(int reason);

	virtual int  GetCurrentLine();

	struct IndexItem : Движимое<IndexItem> {
		Ткст text;
		Цвет  ink;
		int    line;
	};
	bool           navigator;
	SplitterFrame  navigatorframe;
	СтатичПрям     navigatorpane;
	Сплиттер       navigator_splitter;
	
	struct AssistItemConvert : Преобр {
		AssistEditor *editor;

		virtual Значение фмт(const Значение& q) const;
	}
	assist_convert;
	
	Сплиттер       popup;
	КтрлМассив      assist;
	КтрлМассив      тип;
	Индекс<Ткст>      assist_type;
	Массив<CppItemInfo> assist_item;
	Вектор<int>        assist_item_ndx;
	RichTextCtrl   annotation_popup;
	
	int            assist_cursor;
	bool           auto_assist;
	bool           auto_check;
	bool           assist_active;
	bool           commentdp;
	bool           inbody;
	bool           thisback, thisbackn;
	bool           include_assist;
	ШТкст        cachedline;
	int            cachedpos;
	int            cachedln;
	bool           include_local;
	int            include_back;
	Ткст         include_path;
	int            scan_counter;
	
	RichTextCtrl   param_info;
	Ткст         param_qtf;
	struct ParamInfo {
		int            line;
		int            pos;
		ШТкст        test;
		CppItem        элт;
		Ткст         editfile;
		
		ParamInfo()    { line = -1; }
	};
	enum { PARAMN = 16 };
	ParamInfo param[PARAMN];
	int       parami;

	Ткст    current_type;
	
	static Ук<Ктрл> assist_ptr;
	
	bool      navigator_right = true;


	void           PopUpAssist(bool auto_insert = false);
	void           CloseAssist();
	static bool    WheelHook(Ктрл *, bool inframe, int event, Точка p, int zdelta, dword keyflags);
	void           Assist();
	bool           IncludeAssist();
	Ткст         ReadIdBackPos(int& pos, bool include);
	Ткст         ReadIdBack(int q, bool include = false, bool *destructor = NULL);
	void           SyncAssist();
	void           AssistInsert();
	bool           InCode();
	
	void           SyncParamInfo();
	void           StartParamInfo(const CppItem& m, int pos);

	void           Complete();
	void           Abbr();

	void           Контекст(ParserContext& parser, int pos);
	void           ExpressionType(const Ткст& тип,
	                              const Ткст& context_type,
	                              const Ткст& usings,
	                              const Вектор<Ткст>& xp, int ii,
	                              Индекс<Ткст>& typeset, bool variable,
	                              bool can_shortcut_operator, Индекс<Ткст>& visited_bases,
	                              int lvl);
	void           ExpressionType(const Ткст& тип,
	                              const Ткст& context_type,
	                              const Ткст& usings,
	                              const Вектор<Ткст>& xp, int ii,
	                              Индекс<Ткст>& typeset, bool variable, int lvl);
//	void           ExpressionType(const Ткст& тип, const Вектор<Ткст>& xp, int ii,
//	                              Индекс<Ткст>& typeset);
	Индекс<Ткст>  ExpressionType(const ParserContext& parser, const Вектор<Ткст>& xp);

	Индекс<Ткст>  EvaluateExpressionType(const ParserContext& parser, const Вектор<Ткст>& xp);

	Ткст         RemoveDefPar(const char *s);
	Ткст         MakeDefinition(const Ткст& cls, const Ткст& _n);
	void           DCopy();
	void           Virtuals();
	void           Thisbacks();
	void           AssistItemAdd(const Ткст& scope, const CppItem& m, int typei);
	void           GatherItems(const Ткст& тип, bool only_public, Индекс<Ткст>& in_types,
	                           bool types);
	void           RemoveDuplicates();

	void           SelParam();
	int            Ch(int q);
	int            ParsBack(int q);
	Вектор<Ткст> ReadBack(int q, const Индекс<Ткст>& locals);
	void           SkipSpcBack(int& q);
	Ткст         IdBack(int& qq);
	Ткст         CompleteIdBack(int& q, const Индекс<Ткст>& locals);

	void           SwapSContext(ParserContext& p);
	
	bool           GetAnnotationRefs(Вектор<Ткст>& tl, Ткст& coderef, int q = -1);
	bool           GetAnnotationRef(Ткст& t, Ткст& coderef, int q = -1);
	void           SyncAnnotationPopup();
	void           EditAnnotation(bool fastedit);
	void           Annotate(const Ткст& filename);
	void           OpenTopic(Ткст topic, Ткст create, bool before);
	void           NewTopic(Ткст группа, Ткст coderef);
	
	bool           Esc();
	
	bool           IsNavigator() const                             { return navigator; }
	void           Navigator(bool navigator);
	void           SyncNavigatorShow();
	void           SyncNavigator();
	void           SerializeNavigator(Поток& s);
	void           SyncNavigatorPlacement();
	
	Событие<int>     WhenFontScroll;
	Событие<>        WhenSelectionChanged;

	typedef AssistEditor ИМЯ_КЛАССА;

	AssistEditor();
};
