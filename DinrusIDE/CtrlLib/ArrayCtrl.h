template <class T>
void дефФабрикаКтрлФн(Один<Ктрл>& ctrl)
{
	ctrl.создай<T>();
}

template <class T>
Обрвыз1<Один<Ктрл>&> дефФабрикаКтрл()
{
	return callback(дефФабрикаКтрлФн<T>);
}

class КтрлМассив : public Ктрл {
public:
	virtual void  режимОтмены();
	virtual bool  прими();
	virtual void  отклони();
	virtual void  рисуй(Draw& w);
	virtual void  Выкладка();
	virtual bool  Ключ(dword ключ, int);
	virtual void  леваяВнизу(Точка p, dword);
	virtual void  леваяДКлик(Точка p, dword);
	virtual void  леваяТяг(Точка p, dword);
	virtual void  леваяВверху(Точка p, dword flags);
	virtual void  праваяВнизу(Точка p, dword);
	virtual void  двигМыши(Точка p, dword);
	virtual void  выходМыши();
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual Рисунок рисКурсора(Точка p, dword);
	virtual void  тягИБрос(Точка p, PasteClip& d);
	virtual void  тягПовтори(Точка p);
	virtual void  тягПокинь();
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual void  отпрыскФок();
	virtual void  отпрыскРасфок();
	virtual void  сериализуй(Поток& s);

public:
	struct ИнфОбИд {
		Значение           insertval;
		ГенЗначения       *insertgen;
		int            (*accel)(int);

		ИнфОбИд& вставьЗнач(const Значение& v)  { insertval = v; return *this; }
		ИнфОбИд& вставьЗнач(ГенЗначения& g)     { insertgen = &g; return *this; }
		ИнфОбИд& Аксель(int (*filter)(int))    { accel = filter; return *this; }
		ИнфОбИд& Аксель()                      { return Аксель(CharFilterDefaultToUpperAscii); }

		Значение   дайВставьЗнач()             { return insertgen ? insertgen->дай() : insertval; }

		ИнфОбИд()                             { insertgen = NULL; accel = NULL; }
	};

	class Колонка : ПреобрФормата {
		КтрлМассив            *arrayctrl;
		int                   Индекс;
		Mitor<int>            pos;
		const Преобр        *convert;
		Функция<Значение(const Значение&)> convertby;
		Ук<Ктрл>             edit;
		const Дисплей        *дисплей;
		Событие<int, Один<Ктрл>&> factory;
		Событие<Один<Ктрл>&>     factory1;
		int                 (*accel)(int);
		int                   margin;
		bool                  cached;
		bool                  clickedit;
		mutable Любое           cache;
		const ПорядокЗнач     *order;
		Функция<int (const Значение& a, const Значение& b)> сравни;
		Врата<int, int>        line_order;


		void   инвалидируйКэш(int i);
		void   вставьКэш(int i, int n);
		void   удалиКэш(int i);
		void   очистьКэш();
		void   Sorts();
		
		typedef Колонка ИМЯ_КЛАССА;

		friend class КтрлМассив;

	public:
		Колонка& добавь(int _pos)                      { pos.добавь(_pos); return *this; }
		Колонка& добавь(const Ид& id)                  { pos.добавь(-arrayctrl->AsNdx(id)); return *this; }
		Колонка& добавьИндекс(const Ид& id)             { arrayctrl->добавьИндекс(id); return добавь(id); }
		Колонка& добавьИндекс()                         { добавь(arrayctrl->дайСчётИндексов()); arrayctrl->добавьИндекс(); return *this; }

		Колонка& SetConvert(const Преобр& c);
		Колонка& ConvertBy(Функция<Значение(const Значение&)> cv);
		Колонка& устФормат(const char *fmt);
		Колонка& устДисплей(const Дисплей& d);
		Колонка& NoEdit();
		Колонка& Edit(Ктрл& e);
		template <class T>
		Колонка& Ctrls()                            { return Ctrls(дефФабрикаКтрл<T>()); }
		Колонка& WithLined(Событие<int, Один<Ктрл>&> factory);
		Колонка& With(Событие<Один<Ктрл>&> factory);
		Колонка& вставьЗнач(const Значение& v);
		Колонка& вставьЗнач(ГенЗначения& g);
		Колонка& NoClickEdit()                      { clickedit = false; return *this; }
		Колонка& Кэш();
		Колонка& Аксель(int (*filter)(int))          { accel = filter; return *this; }
		Колонка& Аксель()                            { return Аксель(CharFilterDefaultToUpperAscii); }

		Колонка& Sorting();
		Колонка& Sorting(const ПорядокЗнач& o);
		Колонка& SortingLined(Врата<int, int> line_order);
		Колонка& SortingBy(Функция<int (const Значение& a, const Значение& b)> сравни);
		Колонка& SortDefault(bool desc = false);

		Колонка& Margin(int m)                      { margin = m; return *this; }

		HeaderCtrl::Колонка& HeaderTab();
		const HeaderCtrl::Колонка& HeaderTab() const;
		Колонка& Подсказка(const char *tip)               { HeaderTab().Подсказка(tip); return *this; }

		Колонка();

// deprecated (due to overloading issues)
		Колонка& Ctrls(Обрвыз1<Один<Ктрл>&> factory);
		Колонка& Ctrls(void (*factory)(Один<Ктрл>&)) { return Ctrls(Событие<int, Один<Ктрл>&>([=](int, Один<Ктрл>& h) { factory(h); })); }
		Колонка& Ctrls(Событие<int, Один<Ктрл>&> factory);
		Колонка& Ctrls(void (*factory)(int, Один<Ктрл>&)) { return Ctrls(Событие<int, Один<Ктрл>&>([=](int a, Один<Ктрл>& b){ factory(a, b); })); }
		Колонка& Sorting(Врата<int, int> order) { return SortingLined(order); }
		Колонка& Sorting(int (*c)(const Значение& a, const Значение& b)) { return SortingBy(c); }
	};

	struct Порядок {
		virtual bool operator()(const Вектор<Значение>& row1, const Вектор<Значение>& row2) const = 0;
		virtual ~Порядок() {}
	};

private:
	struct ПорядокЭлтов;
	struct ПорядокРядов;
	struct ПредикатСортировки;

	struct Контрол : Движимое<Контрол> {
		int   pos;
		Ктрл *ctrl;
	};

	struct КтрлЯчейка : КтрлРодитель {
		virtual void леваяВнизу(Точка, dword);

		bool       owned;
		bool       значение;
		Ктрл      *ctrl;
	};

	struct ИнфОЯчейке : Движимое<ИнфОЯчейке> {
		БитИУк ptr;

		void           освободи();
		void           уст(Ктрл *ctrl, bool owned, bool значение);
		bool           ктрл_ли() const             { return ptr.дайБит(); }
		КтрлЯчейка&      дайКтрл() const            { ПРОВЕРЬ(ктрл_ли()); return *(КтрлЯчейка *)ptr.дайУк(); }

		void           уст(const Дисплей& d)      { ПРОВЕРЬ(!ктрл_ли()); ptr.уст0((void *)&d); }
		bool           дисплей_ли() const          { return !ptr.дайБит() && ptr.дайУк(); }
		const Дисплей& дайДисплей() const         { ПРОВЕРЬ(дисплей_ли()); return *(const Дисплей *)ptr.дайУк(); }

		ИнфОЯчейке(ИнфОЯчейке&& s);
		ИнфОЯчейке() {}
		~ИнфОЯчейке();
	};

	friend class Колонка;
	friend struct ПредикатСортировки;

	struct Ln : Движимое<Ln> {
		int y;
		int cy;
		Ln()               { cy = Null; }
	};

	struct Строка : Движимое<Строка> {
		bool          select:1;
		bool          enabled:1;
		bool          visible:1;
		Цвет         paper;
		Вектор<Значение> line;

		Строка() { select = false; enabled = true; visible = true; paper = Null; }
	};
	
	static int сравниСтдЗнач(const Значение& a, const Значение& b) { return РНЦП::сравниСтдЗнач(a, b); }


	Вектор<Строка>               array;
	HeaderCtrl                 header;
	ПромотБар                  sb;
	Промотчик                   scroller;
	Массив<Колонка>              column;
	Вектор<Контрол>            control;
	МассивМап<Ид, ИнфОбИд>       idx;
	Вектор<Ln>                 ln;
	Вектор< Вектор<ИнфОЯчейке> > cellinfo;
	Вектор<bool>               modify;
	ФреймНиз<КтрлРодитель>    scrollbox;
	Вектор<int>                column_width, column_pos;
	DisplayPopup               info;
	const Порядок               *columnsortsecondary;
	int                        min_visible_line, max_visible_line;
	int                        ctrl_low, ctrl_high;
	int                        sorting_from;
	Индекс<Ткст>              id_ndx;

	int   keypos;
	int   cursor;
	int   anchor;
	int   linecy;
	int   virtualcount;
	Точка clickpos;
	int   dropline, dropcol;
	int   sortcolumn;
	bool  sortcolumndescending;
	bool  columnsortfindkey;
	int   acceptingrow;

	mutable int   selectcount;

	Ткст row_name;
	Цвет  gridcolor;
	Цвет  evenpaper, evenink, oddpaper, oddink;

	bool  horzgrid:1;
	bool  vertgrid:1;
	bool  nocursor:1;
	bool  mousemove:1;
	bool  editmode:1;
	bool  insertmode:1;

	bool  inserting:1;
	bool  appending:1;
	bool  appendline:1;
	bool  noinsertappend:1;
	bool  autoappend:1;
	bool  removing:1;
	bool  moving:1;
	bool  askremove:1;
	bool  duplicating:1;
	bool  multiselect:1;
	bool  hasctrls:1;
	bool  headerctrls:1;
	bool  popupex:1;
	bool  nobg:1;
	bool  focussetcursor:1;
	bool  allsorting:1;
	bool  spanwidecells:1;

	mutable bool  selectiondirty:1;

	unsigned  bains:2;
	
	bool  isdrag:1;
	bool  selclick:1;

	Рисунок cursor_override;

	int    Поз(int np) const;

	void   вставьКэш(int i);
	void   удалиКэш(int i);

	void   SetSb();
	void   MinMaxLine();
	void   SyncColumnsPos();
	void   HeaderLayout();
	void   HeaderScroll();
	void   промотай();
	int    FindEnabled(int i, int dir);
	void   Page(int q);

	void   DoPoint(Точка p, bool dosel = true);
	void   кликни(Точка p, dword flags);
	int    GetClickColumn(int ii, Точка p);
	void   ClickColumn(Точка p);
	void   ClickSel(dword flags);

	Точка           FindCellCtrl(Ктрл *c);
	Ктрл           *SyncLineCtrls(int i, Ктрл *p = NULL);
	void            SyncPageCtrls();
	void            SyncCtrls(int i0 = 0);
	bool            ктрл_ли(int i, int j) const;
	const КтрлЯчейка& дайКтрлЯчейка(int i, int j) const;
	КтрлЯчейка&       дайКтрлЯчейка(int i, int j);
	void            устЗначениеКтрл(int i, int ii, const Значение& v);

	void   PlaceEdits();
	void   EndEdit();
	void   ColEditSetData(int col);
	void   устДанныеКтрл(int col);
	Значение  Get0(int i, int ii) const;
	void   уст0(int i, int ii, const Значение& v);
	void   AfterSet(int i, bool sync_ctrls = true);

	void   Reline(int i, int y);
	void   удали0(int i);
	void   отклКтрлы();
	void   SetCtrls();
	void   DoRemovem();
	bool   KillCursor0();

	const Дисплей& GetCellInfo(int i, int j, bool f0, Значение& v, Цвет& fg, Цвет& bg, dword& st);
	Ктрл&  устКтрл(int i, int j, Ктрл *newctrl, bool owned, bool значение);
	Размер   DoPaint(Draw& w, bool sample);
	void   SpanWideCell(const Дисплей& d, const Значение& q, int cm, int& cw, Прям& r, int i, int& j);

	bool   TestKey(int i, int ключ);

	bool   устКурсор0(int i, bool dosel = true);

	void   SyncSelection() const;
	void   KeyMultiSelect(int aanchor, dword ключ);

	void   HeaderScrollVisibility();

	void   освежиВыд();
	bool   вставьТиБ(int line, int py, PasteClip& d, int q);
	void   ТиБ(int line, int col);
	enum { DND_INSERTLINE = -1, DND_LINE = -2 };

	bool   ColumnSortPred(int i1, int i2, int column, const ПорядокЗнач *o);
	bool   OrderPred(int i1, int i2, const КтрлМассив::Порядок *o);
	bool   DescendingPred(int i1, int i2, const Врата<int, int> *pred);
	void   синхИнфо();
	void   SortA();
	void   SortB(const Вектор<int>& o);

	void   выделиОдин(int i, bool sel = true, bool raise = true);
	
	int    AsNdx(const Ткст& id)              { return id_ndx.найдиДобавь(id); }

	using Ктрл::изменено;

	// These are listed here as private because имя has changed to устМап/добавьМап
	void       уст(int i, const МапЗнач& m);
	void       добавь(const МапЗнач& m);
	
	bool       IsLineVisible0(int i) const { return i < 0 ? false : i < array.дайСчёт() ? array[i].visible : true; }

public: // temporary (TRC 06/07/28) // will be removed!
	Ктрл&  устКтрл(int i, int j, Ктрл *newctrl) { return устКтрл(i, j, newctrl, true, true); }

protected:
	virtual bool UpdateRow();
	virtual void RejectRow();

	void   ClearModify();

public:
	Событие<>           WhenSel; // the most usual КтрлМассив event

	Событие<>           WhenLeftDouble;
	Событие<Точка>      WhenMouseMove;
	Событие<>           WhenEnterKey;
	Событие<>           ПриЛевКлике;
	Событие<Бар&>       WhenBar;
	Врата<>            WhenAcceptRow;
	Событие<>           WhenUpdateRow;
	Событие<>           WhenArrayAction;
	Событие<>           WhenStartEdit;
	Событие<>           WhenAcceptEdit;
	Событие<>           WhenCtrlsAction;
	Событие<>           ПриПромоте;
	Событие<>           WhenHeaderLayout;
	Событие<>           WhenColumnSorted;

	Событие<int, bool&> WhenLineEnabled;
	Событие<int, bool&> WhenLineVisible;

	Событие<>                     WhenDrag;
	Событие<int, int, PasteClip&> WhenDropCell;
	Событие<int, PasteClip&>      WhenDropInsert;
	Событие<int, PasteClip&>      WhenDropLine;
	Событие<PasteClip&>           WhenDrop;

	//Deprecated - use WhenSel
	Событие<>           WhenEnterRow;
	Событие<>           WhenKillCursor;
	Событие<>           WhenCursor;
	Событие<>           WhenSelection;

	ИнфОбИд&    IndexInfo(int ii);
	ИнфОбИд&    IndexInfo(const Ид& id);
	ИнфОбИд&    добавьИндекс(const Ид& id);
	ИнфОбИд&    добавьИндекс();
	int        дайСчётИндексов() const        { return idx.дайСчёт(); }
	Ид         дайИд(int ii) const          { return idx.дайКлюч(ii); }
	int        дайПоз(const Ид& id) const   { return idx.найди(id); }
	ИнфОбИд&    устИд(int ii, const Ид& id);
	ИнфОбИд&    добавьКлюч(const Ид& id)         { ПРОВЕРЬ(idx.дайСчёт() == 0); return добавьИндекс(id); }
	ИнфОбИд&    добавьКлюч()                     { ПРОВЕРЬ(idx.дайСчёт() == 0); return добавьИндекс(); }
	Ид         дайИдКлюча() const             { return idx.дайКлюч(0); }

	Колонка&    добавьКолонку(const char *text = NULL, int w = 0);
	Колонка&    добавьКолонку(const Ид& id, const char *text, int w = 0);
	Колонка&    добавьКолонкуПо(int ii, const char *text, int w = 0);
	Колонка&    добавьКолонкуПо(const Ид& id, const char *text, int w = 0);
	Колонка&    AddRowNumColumn(const char *text = NULL, int w = 0);

	int                       дайСчётКолонок() const   { return column.дайСчёт(); }
	int                       найдиКолонкуСПоз(int pos) const;
	int                       найдиКолонкуСИд(const Ид& id) const;
	Вектор<int>               найдиКолонкиСПоз(int pos) const;
	Вектор<int>               найдиКолонкиСИд(const Ид& id) const;
	Колонка&                   колонкаПо(int i)          { return column[i]; }
	Колонка&                   колонкаПо(const Ид& id)   { return column[найдиКолонкуСИд(id)]; }
	HeaderCtrl::Колонка&       HeaderTab(int i)         { return header.Вкладка(i); }
	HeaderCtrl::Колонка&       HeaderTab(const Ид& id)  { return header.Вкладка(найдиКолонкуСИд(id)); }
	const Колонка&             колонкаПо(int i) const    { return column[i]; }
	const Колонка&             колонкаПо(const Ид& id) const   { return column[найдиКолонкуСИд(id)]; }
	const HeaderCtrl::Колонка& HeaderTab(int i) const   { return header.Вкладка(i); }
	const HeaderCtrl::Колонка& HeaderTab(const Ид& id) const   { return header.Вкладка(найдиКолонкуСИд(id)); }

	const HeaderCtrl&         HeaderObject() const     { return header; }
	HeaderCtrl&               HeaderObject()           { return header; }

	void       сериализуйЗаг(Поток& s)    { header.сериализуй(s); } // deprecated
	void       сериализуйНастройки(Поток& s);

	ИнфОбИд&    добавьКтрл(Ктрл& ctrl);
	ИнфОбИд&    добавьКтрл(const Ид& id, Ктрл& ctrl);
	ИнфОбИд&    добавьИдКтрл(Ктрл& ctrl)         { return добавьКтрл(ctrl.дайИдВыкладки(), ctrl); }
	void       добавьКтрлПо(int ii, Ктрл& ctrl);
	void       добавьКтрлПо(const Ид& id, Ктрл& ctrl);
	void       AddRowNumCtrl(Ктрл& ctrl);

	void       устСчёт(int c);
	void       SetVirtualCount(int c);
	int        дайСчёт() const;
	void       очисть();
	void       сожми();
	Значение      дай(int i, int ii) const;
	Значение      дай(int i, const Ид& id) const;
	void       уст(int i, int ii, const Значение& v);
	void       уст(int i, const Ид& id, const Значение& v);

	Значение      дай(int ii) const;
	Значение      дай(const Ид& id) const;
	Значение      дайОригинал(int ii) const;
	Значение      дайОригинал(const Ид& id) const;
	bool       изменено(int ii) const;
	bool       изменено(const Ид& id) const;
	Значение      дайКлюч() const;
	Значение      дайОригиналКлюч() const;
	void       уст(int ii, const Значение& v);
	void       уст(const Ид& id, const Значение& v);

	Значение      дайКолонку(int row, int col) const;
	Значение      GetConvertedColumn(int row, int col) const;

	int        дайСчётВыделений() const;
	bool       выделение_ли() const                              { return дайСчётВыделений(); }
	void       выдели(int i, bool sel = true)                   { выделиОдин(i, sel); }
	void       выдели(int i, int count, bool sel = true);
	bool       выделен(int i) const                          { return i < array.дайСчёт() && array[i].select; }
	void       очистьВыделение(bool raise = true);
	bool       IsSel(int i) const;
	Вектор<int> GetSelKeys() const;

	void       активируйСтроку(int i, bool e);
	void       дезактивируйСтроку(int i)                               { активируйСтроку(i, false); }
	bool       строкаАктивирована(int i) const;
	bool       строкаДезактивирована(int i) const                      { return !строкаАктивирована(i); }

	void       покажиСтроку(int i, bool e);
	void       скройСтроку(int i)                                  { покажиСтроку(i, false); }
	bool       строкаВидима(int i) const;

	Вектор<Значение> читайРяд(int i) const; // deprecated имя
	Вектор<Значение> дайСтроку(int i) const                          { return читайРяд(i); }

	void       уст(int i, const Вектор<Значение>& v);
	void       уст(int i, Вектор<Значение>&& v);
	void       уст(int i, const ВекторМап<Ткст, Значение>& m);

	void       добавь();

#define  E__Add(I)      void добавь(__List##I(E__Value));
	__Expand(E__Add)
#undef   E__Add

	void       добавь(const Вектор<Значение>& v)                      { уст(array.дайСчёт(), v); }
	void       добавь(Вектор<Значение>&& v)                           { уст(array.дайСчёт(), pick(v)); }
	void       добавь(const Обнул& null)                          { добавь((Значение)Null); }
	void       добавь(const ВекторМап<Ткст, Значение>& m);
//$-void добавь(const Значение& [, const Значение& ]...);
	template <typename... Args>
	void       добавь(const Args& ...args)                         { добавь(gather<Вектор<Значение>>(args...)); }
//$+

	void       устМап(int i, const МапЗнач& m);
	void       добавьМап(const МапЗнач& m);
	МапЗнач   дайМап(int i) const;

	void       устМассив(int i, const МассивЗнач& va);
	void       добавьМассив(const МассивЗнач& va);
	МассивЗнач дайМассив(int i) const;

	void       добавьСепаратор();

	void       вставь(int i);
	void       вставь(int i, int count);
	void       вставь(int i, const Вектор<Значение>& v);
	void       вставь(int i, const Вектор< Вектор<Значение> >& v);

	void       удали(int i);

	void       удалиВыделение();

	Рисунок      дайСэиплТяга();

	void       вставьБрос(int line, const Вектор<Вектор<Значение>>& data, PasteClip& d, bool self);
	void       вставьБрос(int line, const Вектор<Значение>& data, PasteClip& d, bool self);
	void       вставьБрос(int line, const Значение& data, PasteClip& d, bool self);
	void       вставьБрос(int line, const КтрлМассив& src, PasteClip& d);
	void       вставьБрос(int line, PasteClip& d);

	bool       курсор_ли() const                        { return cursor >= 0; }
	bool       устКурсор(int i);
	bool       анулируйКурсор();
	void       CancelCursor();
	int        дайКурсор() const                       { return cursor; }
	void       идиВНач();
	void       идиВКон();
	int        GetCursorSc() const;
	void       ScCursor(int a);
	void       курсорПоЦентру();
	void       промотайДо(int line);
	void       ScrollIntoCursor();
	void       SetCursorEditFocus();
	int        дайПромотку() const;
	void       ScrollTo(int sc);
	void       ShowAppendLine();
	bool       AcceptRow(bool endedit = false);

	void       Move(int d);
	void       SwapUp();
	void       SwapDown();

	int        найди(const Значение& v, int ii = 0, int from = 0) const;
	int        найди(const Значение& v, const Ид& id, int from = 0) const;

	bool       FindSetCursor(const Значение& val, int ii = 0, int from = 0);
	bool       FindSetCursor(const Значение& val, const Ид& id, int from = 0);

	void       ReArrange(const Вектор<int>& order);

	void       сортируй(int from, int count, Врата<int, int> order);
	void       сортируй(Врата<int, int> order);
	void       сортируй(const КтрлМассив::Порядок& order);
	void       сортируй(int from, int count, const КтрлМассив::Порядок& order);
	void       сортируй(int (*compare)(const Вектор<Значение>& v1, const Вектор<Значение>& v2));
	void       сортируй(int from, int count,
	                int (*compare)(const Вектор<Значение>& v1, const Вектор<Значение>& v2));
	void       сортируй(int ii, int (*compare)(const Значение& v1, const Значение& v2)
	                = сравниСтдЗнач);
	void       сортируй(const Ид& id, int (*compare)(const Значение& v1, const Значение& v2)
	                = сравниСтдЗнач);
	void       сортируй()                                  { сортируй(0); }

	void       ColumnSort(int column, Врата<int, int> order);
	void       ColumnSort(int column, const ПорядокЗнач& order);
	void       ColumnSort(int column, int (*compare)(const Значение& a, const Значение& b) = сравниСтдЗнач);

	void       SetSortColumn(int ii, bool descending = false);
	void       ToggleSortColumn(int ii);
	void       DoColumnSort();
	int        GetSortColumn() const                   { return sortcolumn; }
	bool       IsSortDescending() const                { return sortcolumndescending; }

	bool       IsInsert() const                        { return insertmode; }

	void            устДисплей(int i, int col, const Дисплей& d);
	void            SetRowDisplay(int i, const Дисплей& d);
	void            SetColumnDisplay(int j, const Дисплей& d);
	const Дисплей&  дайДисплей(int row, int col);
	const Дисплей&  дайДисплей(int col);

	void       RefreshRow(int i);

	void       устКтрл(int i, int col, Ктрл& ctrl, bool значение = true);
	Ктрл      *дайКтрл(int i, int col);
	template <class T>
	T&         CreateCtrl(int i, int col, bool значение = true) { T *c = new T; устКтрл(i, col, c, true, значение); SyncLineCtrls(i); return *c; }

	КтрлМассив& SetLineCy(int cy);
	КтрлМассив& SetEditLineCy()                         { return SetLineCy(EditField::GetStdHeight() + DPI(4)); }
	void       SetLineCy(int i, int cy);
	int        GetLineCy() const                       { return linecy; }
	int        GetLineY(int i) const;
	int        GetLineCy(int i) const;
	int        GetTotalCy() const;
	int        GetLineAt(int y) const;
	
	void       SetLineColor(int i, Цвет c);

	Прям       GetCellRect(int i, int col) const;
	Прям       GetCellRectM(int i, int col) const;
	Прям       GetScreenCellRect(int i, int col) const;
	Прям       GetScreenCellRectM(int i, int col) const;

	Точка      GetClickPos() const                     { return clickpos; }
	int        GetClickColumn() const                  { return clickpos.x; }
	int        GetClickRow() const                     { return clickpos.y; }

	bool       StartEdit(int d = 0);
	int        GetEditColumn() const;
	bool       IsEdit() const                          { return editmode; }

	void       DoEdit();
	void       DoInsert(int cursor);
	void       DoInsertBefore();
	void       DoInsertAfter();
	void       DoAppend();
	bool       DoRemove();
	void       DoDuplicate();
	void       выделиВсе();
	void       StdBar(Бар& menu);

	bool       IsEditing() const;
	bool       AcceptEnter();

	void       очистьКэш();
	void       инвалидируйКэш(int i);

	void       ScrollUp()                              { sb.предшСтрочка(); }
	void       ScrollDown()                            { sb.следщСтрочка(); }
	void       ScrollPageUp()                          { sb.предшСтраница(); }
	void       ScrollPageDown()                        { sb.следщСтраница(); }
	void       ScrollEnd()                             { sb.стоп(); }
	void       ScrollBegin()                           { sb.старт(); }

	Ткст     AsText(Ткст (*формат)(const Значение&), bool sel = false,
	                  const char *tab = "\t", const char *row = "\r\n",
	                  const char *hdrtab = "\t", const char *hdrrow = "\r\n") const;
	void       SetClipboard(bool sel = false, bool hdr = true) const;
	Ткст     AsQtf(bool sel = false, bool hdr = true);
	Ткст     AsCsv(bool sel = false, int sep = ';', bool hdr = true);

	Ткст     фмтРяда(const char *s);

	КтрлМассив& RowName(const char *s)                  { row_name = s; return *this; }
	КтрлМассив& AppendLine(bool b = true)               { appendline = b; return *this; }
	КтрлМассив& NoAppendLine()                          { return AppendLine(false); }
	bool       IsAppendLine() const                    { return appendline; }
	КтрлМассив& Inserting(bool b = true)                { inserting = b; return AppendLine(b); }
	КтрлМассив& NoInserting()                           { return Inserting(false); }
	bool       IsInserting() const                     { return inserting; }
	КтрлМассив& Removing(bool b = true)                 { removing = b; return *this; }
	КтрлМассив& NoRemoving()                            { return Removing(false); }
	bool       IsRemoving() const                      { return removing; }
	КтрлМассив& Appending(bool b = true)                { appending = b; return *this; }
	bool       IsAppending() const                     { return appending || autoappend; }
	КтрлМассив& AutoAppending(bool b = true)            { autoappend = b; return *this; }
	bool       IsAutoAppending() const                 { return autoappend; }
	КтрлМассив& BeforeAfterInserting(int q = 1)         { bains = 1; return *this; }
	КтрлМассив& AfterBeforeInserting(int q = 2)         { bains = 2; return *this; }
	КтрлМассив& Duplicating(bool b = true)              { duplicating = b; return *this; }
	КтрлМассив& NoInsertAppend(bool b = true)           { noinsertappend = b; return *this; }
	bool       IsDuplicating() const                   { return duplicating; }
	КтрлМассив& Moving(bool b = true)                   { moving = b; return *this; }
	bool       IsMoving() const                        { return moving; }
	КтрлМассив& NoDuplicating()                         { return Duplicating(false); }
	КтрлМассив& AskRemove(bool b = true)                { askremove = b; return *this; }
	КтрлМассив& NoAskRemove()                           { return AskRemove(false); }
	bool       IsAskRemove() const                     { return askremove; }

	КтрлМассив& Header(bool b = true)                   { header.Invisible(!b); return *this; }
	КтрлМассив& NoHeader()                              { return Header(false); }
	КтрлМассив& Track(bool b = true)                    { header.Track(b); return *this; }
	КтрлМассив& NoTrack()                               { return Track(false); }
	КтрлМассив& VertGrid(bool b = true)                 { vertgrid = b; освежи(); return *this; }
	КтрлМассив& NoVertGrid()                            { return VertGrid(false); }
	КтрлМассив& HorzGrid(bool b = true)                 { horzgrid = b; освежи(); return *this; }
	КтрлМассив& NoHorzGrid()                            { return HorzGrid(false); }
	КтрлМассив& Grid(bool b = true)                     { return VertGrid(b).HorzGrid(b); }
	КтрлМассив& NoGrid()                                { return Grid(false); }
	КтрлМассив& GridColor(Цвет c)                      { gridcolor = c; return *this; }
	КтрлМассив& EvenRowColor(Цвет paper = смешай(SColorMark, SColorPaper, 220), Цвет ink = SColorText);
	КтрлМассив& OddRowColor(Цвет paper = SColorInfo, Цвет ink = SColorText);
	КтрлМассив& NoCursor(bool b = true)                 { nocursor = b; return *this; }
	КтрлМассив& MouseMoveCursor(bool b = true)          { mousemove = b; return *this; }
	КтрлМассив& NoMouseMoveCursor()                     { return MouseMoveCursor(false); }
	КтрлМассив& AutoHideSb(bool b = true)               { sb.автоСкрой(b); return *this; }
	КтрлМассив& NoAutoHideSb()                          { return AutoHideSb(false); }
	КтрлМассив& HideSb(bool b = true)                   { sb.покажи(!b); return *this; }
	КтрлМассив& AutoHideHorzSb(bool b = true)           { header.AutoHideSb(b); return *this; }
	КтрлМассив& NoAutoHideHorzSb()                      { return AutoHideHorzSb(false); }
	КтрлМассив& HideHorzSb(bool b = true)               { header.HideSb(b); return *this; }
	
	КтрлМассив& MultiSelect(bool b = true)              { multiselect = b; return *this; }
	bool       IsMultiSelect() const                   { return multiselect; }
	КтрлМассив& NoBackground(bool b = true)             { nobg = b; Transparent(); освежи(); return *this; }
	КтрлМассив& PopUpEx(bool b = true)                  { popupex = b; синхИнфо(); return *this; }
	КтрлМассив& NoPopUpEx()                             { return PopUpEx(false); }
	КтрлМассив& NoFocusSetCursor()                      { focussetcursor = false; return *this; }
	КтрлМассив& MovingHeader(bool b)                    { header.Moving(b); return *this; }
	КтрлМассив& NoMovingHeader()                        { return MovingHeader(false); }
	КтрлМассив& ColumnSortFindKey(bool b = true)        { columnsortfindkey = b; return *this; }
	КтрлМассив& AllSorting();
	КтрлМассив& ColumnSortSecondary(const Порядок& order) { columnsortsecondary = &order; return *this; }
	КтрлМассив& NoColumnSortSecondary()                 { columnsortsecondary = NULL; return *this; }
	КтрлМассив& SortingFrom(int from)                   { sorting_from = from; return *this; }

	КтрлМассив& ColumnWidths(const char *s);
	Ткст     GetColumnWidths();

	КтрлМассив& устСтильПромотБара(const ПромотБар::Стиль& s)   { sb.устСтиль(s); header.устСтильПромотБара(s); return *this; }
	КтрлМассив& SetHeaderCtrlStyle(const HeaderCtrl::Стиль& s) { header.устСтиль(s); return *this; }

	КтрлМассив& CursorOverride(const Рисунок& arrow)             { cursor_override = arrow; return *this; }
	КтрлМассив& NoCursorOverride()                             { return CursorOverride(Null); }
	
	КтрлМассив& SpanWideCells(bool b = true)                   { spanwidecells = b; освежи(); return *this; }

	void переустанов();

	typedef КтрлМассив ИМЯ_КЛАССА;

	КтрлМассив();
	virtual ~КтрлМассив();
};

class ArrayOption : public Дисплей, public Pte<ArrayOption> {
public:
	typedef ArrayOption ИМЯ_КЛАССА;
	ArrayOption();
	virtual ~ArrayOption();

	virtual void       рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const;

	void               Connect(КтрлМассив& ac, int ii = 0);
	void               Connect(КтрлМассив& ac, const Ид& id)        { Connect(ac, ac.дайПоз(id)); }

	void               Disconnect();

	КтрлМассив::Колонка& добавьКолонку(КтрлМассив& ac, const char *text = NULL, int w = 0);
	КтрлМассив::Колонка& добавьКолонку(КтрлМассив& ac, const Ид& id, const char *text, int w = 0);

	ArrayOption&       TrueFalse(Значение _t, Значение _f)               { t = _t; f = _f; return *this; }
	Значение              GetFalse() const                            { return f; }
	Значение              GetTrue() const                             { return t; }
	ArrayOption&       ThreeState(bool b = true)                   { threestate = b; return *this; }
	ArrayOption&       NoThreeState()                              { return ThreeState(false); }
	bool               IsThreeState() const                        { return threestate; }

	ArrayOption&       HSwitch(bool hs = true)                     { hswitch = hs; return *this; }
	ArrayOption&       NoHSwitch()                                 { return HSwitch(false); }
	bool               IsHSwitch() const                           { return hswitch; }

	ArrayOption&       VSwitch(bool vs = true)                     { vswitch = vs; return *this; }
	ArrayOption&       NoVSwitch()                                 { return VSwitch(false); }
	bool               IsVSwitch() const                           { return vswitch; }

	ArrayOption&       Фрейм(bool frm = true)                      { frame = frm; return *this; }
	ArrayOption&       NoFrame()                                   { return Фрейм(false); }
	bool               IsFrame() const                             { return frame; }

	void               Click();

public:
	Событие<>            WhenAction;

	Событие<>            operator <<= (Событие<>  cb)                  { return WhenAction = cb; }

private:
	Вектор<int>        Индекс;
	КтрлМассив         *array;
	Значение              t, f;
	bool               hswitch, vswitch;
	bool               threestate;
	bool               frame;
};
