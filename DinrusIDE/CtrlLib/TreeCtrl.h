class КтрлДерево : public Ктрл {
public:
	virtual void  режимОтмены();
	virtual void  рисуй(Draw& w);
	virtual void  Выкладка();
	virtual void  леваяВнизу(Точка p, dword flags);
	virtual void  леваяВверху(Точка p, dword flags);
	virtual void  леваяДКлик(Точка p, dword flags);
	virtual void  леваяТяг(Точка p, dword keyflags);
	virtual void  праваяВнизу(Точка p, dword flags);
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void  двигМыши(Точка, dword);
	virtual bool  Ключ(dword ключ, int);
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual void  отпрыскФок();
	virtual void  отпрыскРасфок();
	virtual void  отпрыскУдалён(Ктрл *);
	virtual void  устДанные(const Значение& data);
	virtual Значение дайДанные() const;
	virtual void  тягВойди();
	virtual void  тягИБрос(Точка p, PasteClip& d);
	virtual void  тягПовтори(Точка p);
	virtual void  тягПокинь();

public:
	class Узел
	{
		void           иниц() { дисплей = NULL; size = Null; margin = 2; canopen = false; canselect = true; }

	public:
		Рисунок          image;
		int            margin;
		Значение          ключ;
		Значение          значение;
		const Дисплей *дисплей;
		Размер           size;
		Ук<Ктрл>      ctrl;
		bool           canopen;
		bool           canselect;

		Узел& устРисунок(const Рисунок& img)          { image = img; return *this; }
		Узел& SetMargin(int m)                    { margin = m; return *this; }
		Узел& уст(const Значение& v)                 { ключ = значение = v; return *this; }
		Узел& уст(const Значение& v, const Значение& t) { ключ = v; значение = t; return *this; }
		Узел& устДисплей(const Дисплей& d)        { дисплей = &d; return *this; }
		Узел& устРазм(Размер sz)                    { size = sz; return *this; }
		Узел& устКтрл(Ктрл& _ctrl)                { ctrl = &_ctrl; return *this; }
		Узел& CanOpen(bool b = true)              { canopen = b; return *this; }
		Узел& CanSelect(bool b)                   { canselect = b; return *this; }

		Узел();
		Узел(const Рисунок& img, const Значение& v);
		Узел(const Рисунок& img, const Значение& v, const Значение& t);
		Узел(const Значение& v);
		Узел(const Значение& v, const Значение& t);
		Узел(Ктрл& ctrl);
		Узел(const Рисунок& img, Ктрл& ctrl, int cx = 0, int cy = 0);
	};

private:
	struct Элемент : Узел {
		union {
			int            parent;
			int            freelink;
		};

		bool           free;
		bool           isopen;
		bool           sel;
		Вектор<int>    child;
		int            linei;

		Размер GetValueSize(const Дисплей *treedisplay) const;
		Размер GetCtrlSize() const;
		Размер дайРазм(const Дисплей *treedisplay) const;

		Элемент() { isopen = false; linei = -1; parent = -1; canselect = true; sel = false; free = false; }
	};
	struct Строка : Движимое<Строка> {
		int  level;
		int  itemi;
		int  ll;
		int  y;
	};

	Массив<Элемент>  элт;
	Вектор<Строка> line;
	int          cursor;
	int          freelist;
	int          levelcx;
	int          anchor;
	int          selectcount;
	bool         nocursor;
	bool         noroot;
	bool         dirty;
	bool         hasctrls;
	bool         multiselect;
	bool         nobg;
	bool         popupex;
	bool         chldlck;
	bool         mousemove;
	bool         accel;
	bool         highlight_ctrl;
	bool         multiroot;
	Рисунок        imgEmpty;

	bool         selclick;
	int          dropitem, dropinsert;
	Точка        repoint;
	int          retime;
	Размер         treesize;
	Точка        itemclickpos;

	ПрокрутБары   sb;
	СтатичПрям   sb_box;
	Промотчик     scroller;

	DisplayPopup info;

	const Дисплей *дисплей;

	struct LineLess {
		bool   operator()(int y, const Строка& l) const            { return y < l.y; }
	};
	
	Один<EditString> edit_string;
	Ктрл           *editor = NULL;
	int             edit_cursor = -1;

	struct SortOrder;

	const Дисплей *дайСтиль(int i, Цвет& fg, Цвет& bg, dword& st);
	void   Dirty(int id = 0);
	void   ReLine(int, int, Размер&);
	void   RemoveSubtree(int id);
	void   промотай();
	int    FindLine(int y) const;
	void   освежиСтроку(int i, int ex = 0);
	void   освежиЭлт(int id, int ex);
	void   SetCursorLineSync(int i);
	void   MoveCursorLine(int c, int incr);
	void   SetCursorLine(int i, bool sc, bool sel, bool cb);
	void   устКурсор(int id, bool sc, bool sel, bool cb);
	void   RemoveCtrls(int itemi);
	void   SyncCtrls(bool add, Ктрл *restorefocus);
	bool   Вкладка(int d);
	void   сортируй0(int id, const ПорядокЗнач& order, bool byvalue);
	void   SortDeep0(int id, const ПорядокЗнач& order, bool byvalue);
	void   сортируй0(int id, const ValuePairOrder& order);
	void   SortDeep0(int id, const ValuePairOrder& order);
	void   GatherOpened(int id, Вектор<int>& o);
	void   SelClear(int id);
	void   UpdateSelect();
	void   ShiftSelect(int l1, int l2);
	void   SelectOne0(int id, bool sel, bool cb = false);
	void   ТиБ(int itemid, int insert);
	bool   DnDInserti(int ii, PasteClip& d, bool bottom);
	bool   вставьТиБ(int ii, int py, int q, PasteClip& d);
	void   освежиВыд();
	void   GatherSel(int id, Вектор<int>& sel) const;
	void   кликни(Точка p, dword flags, bool down, bool canedit);
	void   синхИнфо();
	void   SyncAfterSync(Ук<Ктрл> restorefocus);
	Прям   GetValueRect(const Строка& l) const;
	void   StartEdit();
	void   KillEdit();
	bool   IsEdit() const                       { return editor && editor->дайРодителя() == this; }

	enum {
		TIMEID_STARTEDIT = Ктрл::TIMEID_COUNT,
		TIMEID_COUNT
	};

	using Ктрл::закрой;

protected:
	virtual void SetOption(int id);
	void SyncTree(bool immediate = false);
	virtual void выдели();
	
	friend class PopUpTree;

public:
	Событие<int>  WhenOpen;
	Событие<int>  WhenClose;
	Событие<>     ПриЛевКлике;
	Событие<>     WhenLeftDouble;
	Событие<Бар&> WhenBar;
	Событие<>     WhenSel;

	Событие<>                     WhenDrag;
	Событие<int, PasteClip&>      WhenDropItem;
	Событие<int, int, PasteClip&> WhenDropInsert;
	Событие<PasteClip&>           WhenDrop;
	
	Событие<int>                  WhenStartEdit;
	Событие<int, const Значение&>    WhenEdited;

	// deprecated - use WhenSel
	Событие<>         WhenCursor;
	Событие<>         WhenSelection;

	void   SetRoot(const Узел& n);
	void   SetRoot(const Рисунок& img, Значение v);
	void   SetRoot(const Рисунок& img, Значение ключ, Значение text);
	void   SetRoot(const Рисунок& img, Ктрл& ctrl, int cx = 0, int cy = 0);
	int    вставь(int parentid, int i, const Узел& n);
	int    вставь(int parentid, int i);
	int    вставь(int parentid, int i, const Рисунок& img, Значение значение, bool withopen = false);
	int    вставь(int parentid, int i, const Рисунок& img, Значение ключ, Значение значение, bool withopen = false);
	int    вставь(int parentid, int i, const Рисунок& img, Значение ключ, const Ткст& значение, bool withopen = false);
	int    вставь(int parentid, int i, const Рисунок& img, Значение ключ, const char *значение, bool withopen = false);
	int    вставь(int parentid, int i, const Рисунок& img, Ктрл& c, int cx = 0, int cy = 0, bool wo = false);
	int    добавь(int parentid, const Узел& n);
	int    добавь(int parentid);
	int    добавь(int parentid, const Рисунок& img, Значение значение, bool withopen = false);
	int    добавь(int parentid, const Рисунок& img, Значение ключ, Значение значение, bool withopen = false);
	int    добавь(int parentid, const Рисунок& img, Значение ключ, const Ткст& значение, bool withopen = false);
	int    добавь(int parentid, const Рисунок& img, Значение ключ, const char *значение, bool withopen = false);
	int    добавь(int parentid, const Рисунок& img, Ктрл& ctrl, int cx = 0, int cy = 0, bool withopen = false);
	void   удали(int id);
	void   RemoveChildren(int id);

	void   разверни(int id1, int id2);
	void   SwapChildren(int parentid, int i1, int i2);

	int    GetChildCount(int id) const                         { return элт[id].child.дайСчёт(); }
	int    GetChild(int id, int i) const                       { return элт[id].child[i]; }
	int    GetChildIndex(int parentid, int childid) const;
	int    дайРодителя(int id) const                             { return элт[id].parent; }

	Значение  дай(int id) const;
	Значение  дайЗначение(int id) const;
	Значение  operator[](int id) const                            { return дай(id); }
	void   уст(int id, Значение v);
	void   уст(int id, Значение ключ, Значение значение);
	
	void   устДисплей(int id, const Дисплей& дисплей);
	const Дисплей& дайДисплей(int id) const;
	void   освежиЭлт(int id)                                 { освежиЭлт(id, 0); }

	int    дайСчётСтрок();
	int    GetItemAtLine(int i);
	int    GetLineAtItem(int id);

	Узел   GetNode(int id) const                               { return элт[id]; }
	void   SetNode(int id, const Узел& n);

	bool   пригоден(int id) const;

	bool   открыт(int id) const;
	void   открой(int id, bool open = true);
	void   закрой(int id)                                       { открой(id, false); }
	Вектор<int> GetOpenIds() const;
	void   OpenIds(const Вектор<int>& ids);

	void   OpenDeep(int id, bool open = true);
	void   CloseDeep(int id)                                   { OpenDeep(id, false); }

	void   MakeVisible(int id);

	void   SetCursorLine(int i);
	int    GetCursorLine() const                               { return cursor; }
	void   анулируйКурсор();
	void   устКурсор(int id);
	int    дайКурсор() const;
	bool   курсор_ли() const                                    { return дайКурсор() >= 0; }

	Точка  GetItemClickPos() const                             { return itemclickpos; }

	Точка  дайПромотку() const;
	void   ScrollTo(Точка sc);

	Значение  дай() const;
	Значение  дайЗначение() const;
	
	void   устЗначение(const Значение& v);

	int    найди(Значение ключ);
	bool   FindSetCursor(Значение ключ);

	void   сортируй(int id, const ПорядокЗнач& order, bool byvalue = false);
	void   SortDeep(int id, const ПорядокЗнач& order, bool byvalue = false);

	void   сортируй(int id, int (*compare)(const Значение& v1, const Значение& v2) = сравниСтдЗнач,
	            bool byvalue = false);
	void   SortDeep(int id, int (*compare)(const Значение& v1, const Значение& v2) = сравниСтдЗнач,
	                bool byvalue = false);

	void   SortByValue(int id, const ПорядокЗнач& order);
	void   SortDeepByValue(int id, const ПорядокЗнач& order);

	void   SortByValue(int id, int (*compare)(const Значение& v1, const Значение& v2) = сравниСтдЗнач);
	void   SortDeepByValue(int id, int (*compare)(const Значение& v1, const Значение& v2) = сравниСтдЗнач);

	void   сортируй(int id, const ValuePairOrder& order);
	void   SortDeep(int id, const ValuePairOrder& order);
	void   сортируй(int id, int (*compare)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2));
	void   SortDeep(int id, int (*compare)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2));

	void   очисть();

	void         очистьВыделение();
	void         выделиОдин(int id, bool sel = true);
	bool         выделен(int id) const       { return элт[id].sel; }
	bool         IsSel(int id) const;
	int          дайСчётВыделений() const         { return selectcount; }
	bool         IsSelDeep(int id) const;
	Вектор<int>  GetSel() const;
	void         удали(const Вектор<int>& id);
	void         удалиВыделение()              { удали(GetSel()); }

	void         AdjustAction(int parent, PasteClip& x);
	Вектор<int>  вставьБрос(int parent, int ii, const КтрлДерево& src, PasteClip& x);
	Вектор<int>  вставьБрос(int parent, int ii, PasteClip& d);
	Рисунок        дайСэиплТяга();

	void         StdLeftDouble();

	void         ScrollIntoLine(int i);
	void         CenterLine(int i);

	void         ScrollIntoCursor();
	void         курсорПоЦентру();

	Размер         GetTreeSize() const         { return treesize; }

	void         OkEdit();
	void         EndEdit();

	КтрлДерево& NoCursor(bool b = true)        { nocursor = b; if(b) анулируйКурсор(); return *this; }
	КтрлДерево& NoRoot(bool b = true)          { noroot = b; Dirty(); освежи(); return *this; }
	КтрлДерево& LevelCx(int cx)                { levelcx = cx; Dirty(); return *this; }
	КтрлДерево& MultiSelect(bool b = true)     { multiselect = b; return *this; }
	КтрлДерево& NoBackground(bool b = true)    { nobg = b; Transparent(); освежи(); return *this; }
	КтрлДерево& PopUpEx(bool b = true)         { popupex = b; return *this; }
	КтрлДерево& NoPopUpEx()                    { return PopUpEx(false); }
	КтрлДерево& MouseMoveCursor(bool m = true) { mousemove = m; return *this; }
	КтрлДерево& Аксель(bool a = true)           { accel = a; return *this; }
	КтрлДерево& устДисплей(const Дисплей& d);
	КтрлДерево& HighlightCtrl(bool a = true)   { highlight_ctrl = a; освежи(); return *this; }
	КтрлДерево& RenderMultiRoot(bool a = true) { multiroot = a; освежи(); return *this; }
	КтрлДерево& EmptyNodeIcon(const Рисунок& a)  { imgEmpty = a; освежи(); return *this; }
	КтрлДерево& Editor(Ктрл& e)                { editor = &e; return *this; }
	
	КтрлДерево& устСтильПромотБара(const ПромотБар::Стиль& s) { sb.устСтиль(s); return *this; }

	typedef КтрлДерево ИМЯ_КЛАССА;

	КтрлДерево();
	~КтрлДерево();
};

int копируй(КтрлДерево& dst, int dstid, int i, const КтрлДерево& src, int srcid);

class OptionTree : public КтрлДерево {
	Вектор<Опция *> option;
	Массив<Опция>    aux;
	bool             manualmode;

protected:
	virtual void SetOption(int id);
	virtual void SetChildren(int id, bool b);

public:
	Событие<>  WhenOption;

	void SetRoot(const Рисунок& img, Опция& option, const char *text = NULL);
	void SetRoot(Опция& option, const char *text = NULL);
	void SetRoot(const Рисунок& img, const char *text);
	void SetRoot(const char *text);
	int  вставь(int parent, int i, const Рисунок& img, Опция& option, const char *text = NULL);
	int  вставь(int parent, int i, Опция& option, const char *text = NULL);
	int  вставь(int parent, int i, const Рисунок& img, const char *text);
	int  вставь(int parent, int i, const char *text);
	int  добавь(int parent, const Рисунок& img, Опция& option, const char *text = NULL);
	int  добавь(int parent, Опция& option, const char *text = NULL);
	int  добавь(int parent, const Рисунок& img, const char *text);
	int  добавь(int parent, const char *text);
	
	void   устНадпись(int id, const char *text);
	Ткст GetLabel(int id) const                   { return option[id]->GetLabel(); }

	int  дай(int id) const                          { return option[id]->дай(); }

	void очисть();

	typedef OptionTree ИМЯ_КЛАССА;

	OptionTree& ManualMode()						{ manualmode = true; return *this; }

	OptionTree();
	~OptionTree();
};

class PopUpTree : public КтрлДерево {
public:
	virtual void откл();
	virtual void выдели();
	virtual bool Ключ(dword ключ, int);
	virtual void режимОтмены();

private:
	int          maxheight;
	Точка        showpos;
	bool         autosize;
	int          showwidth;
	bool         up;
	bool         open;

	void         DoClose();
	void         OpenClose(int i);

public:
	typedef PopUpTree ИМЯ_КЛАССА;
	PopUpTree();
	virtual ~PopUpTree();

	void         PopUp(Ктрл *owner, int x, int top, int bottom, int width);
	void         PopUp(Ктрл *owner, int width);
	void         PopUp(Ктрл *owner);

	Событие<>      WhenCancel;
	Событие<>      WhenSelect;

	PopUpTree&   MaxHeight(int maxheight_)          { maxheight = maxheight_; return *this; }
	int          GetMaxHeight() const               { return maxheight; }
	PopUpTree&   AutoResize(bool b = true)          { autosize = b; return *this; }
	PopUpTree&   SetDropLines(int n)                { MaxHeight(n * (GetStdFontCy() + 4)); return *this; }
};

class DropTree : public MultiButton {
public:
	virtual bool  Ключ(dword k, int cnt);
	virtual void  устДанные(const Значение& data);
	virtual Значение дайДанные() const;

private:
	PopUpTree          tree;
	const Дисплей     *valuedisplay;
	DisplayWithIcon    icond;
	int                dropwidth;
	bool               dropfocus;
	bool               notnull;
	
	void синх();
	void сбрось();
	void выдели();
	void Cancel();
	
	typedef DropTree ИМЯ_КЛАССА;

public:
	Событие<>       WhenDrop;

	void очисть();

	PopUpTree&   TreeObject()                       { return tree; }

	КтрлДерево *operator->()                          { return &tree; }
	const КтрлДерево *operator->() const              { return &tree; }

	DropTree& MaxHeight(int maxheight_)             { tree.MaxHeight(maxheight_); return *this; }
	DropTree& AutoResize(bool b = true)             { tree.AutoResize(b); return *this; }

	DropTree& ValueDisplay(const Дисплей& d);
	DropTree& DropFocus(bool b = true)              { dropfocus = b; return *this; }
	DropTree& NoDropFocus()                         { return DropFocus(false); }
	DropTree& SetDropLines(int d)                   { tree.SetDropLines(d); return *this; }
	DropTree& DropWidth(int w)                      { dropwidth = w; return *this; }
	DropTree& DropWidthZ(int w)                     { return DropWidth(Zx(w)); }

	DropTree();
};
