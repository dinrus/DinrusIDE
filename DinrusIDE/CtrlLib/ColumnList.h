class СписокКолонок : public Ктрл, private КтрлФрейм {
public:
	virtual void  рисуй(Draw& w);
	virtual void  Выкладка();
	virtual Рисунок рисКурсора(Точка p, dword);
	virtual void  леваяВнизу(Точка p, dword);
	virtual void  леваяВверху(Точка p, dword);
	virtual void  леваяДКлик(Точка p, dword);
	virtual void  праваяВнизу(Точка p, dword);
	virtual void  леваяТяг(Точка p, dword keyflags);
	virtual void  двигМыши(Точка p, dword);
	virtual void  выходМыши();
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual bool  Ключ(dword ключ, int count);
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual void  режимОтмены();
	virtual void  тягВойди();
	virtual void  тягИБрос(Точка p, PasteClip& d);
	virtual void  тягПовтори(Точка p);
	virtual void  тягПокинь();
	virtual Значение дайДанные() const;
	virtual void  устДанные(const Значение& ключ);

private:
	virtual void  выложиФрейм(Прям& r);
	virtual void  добавьРазмФрейма(Размер& sz);
	virtual void  рисуйФрейм(Draw& draw, const Прям& r);

private:
	int        ncl;
	int        cx, cy;
	int        cursor, anchor;
	int        dx;
	int        ci;
	int        mpos;
	ПромотБар  sb;
	Промотчик   scroller;
	КтрлФрейм *frame;
	int        dropitem;
	bool       insert;
	bool       clickkill;
	bool       nobg;
	bool       popupex;
	bool       selclick;
	int        mode;

	DisplayPopup info;

	const Дисплей *дисплей;

	struct Элемент {
		bool           sel;
		bool           canselect;
		Значение          ключ;
		Значение          значение;
		const Дисплей *дисплей;
	};

	Массив<Элемент> элт;
	int         selcount;
	bool        multi;

	struct  ПорядокЭлтов;
	friend struct ПорядокЭлтов;

	void    SetSb();
	void    промотай();
	int     GetDragColumn(int x) const;
	int     RoundedCy();
	void    Page(bool down);
	void    PointDown(Точка p);
	void    кликни(Точка p, dword flags);
	void    ShiftSelect();
	void    освежиЭлт(int i, int ex = 0);
	void    RefreshCursor()                    { освежиЭлт(cursor); }
	void    дайСтильЭлта(int i, Цвет& ink, Цвет& paper, dword& style);
	dword   рисуйЭлт(Draw& w, int i, const Прям& r);
	void    синхИнфо();
	void    устКурсор0(int c, bool sel);
	void    UpdateSelect();
	void    освежиВыд();
	void    левуюВниз(Точка p, dword);
	dword   поменяйКлюч(dword ключ);
	void    рисуйРяды(Draw &w, Размер &sz);
	int     GetSbPos(const Размер &sz) const;
	void    промотайДо(int pos);

	bool    вставьТиБ(int i, int py, PasteClip& d, int q);
	void    ТиБ(int _drop, bool _insert);
	
	friend class СписокФайлов;

public:
	enum {
		MODE_LIST,
		MODE_COLUMN,
		MODE_ROWS
	};

	Событие<>      ПриЛевКлике;
	Событие<Точка> WhenLeftClickPos;
	Событие<>      WhenLeftDouble;
	Событие<Бар&>  WhenBar;
	Событие<>      WhenSel;

	Событие<>                  WhenDrag;
	Событие<int, PasteClip&>   WhenDropItem;
	Событие<int, PasteClip&>   WhenDropInsert;
	Событие<PasteClip&>        WhenDrop;

	// deprecated - use WhenSel
	Событие<>          WhenSelection;
	Событие<>          WhenEnterItem;
	Событие<>          WhenKillCursor;

	int     дайЭлтыКолонки() const;
	int     GetColumnCx(int i = 0) const;
	int     дайЭлтыСтраницы() const;

	int     дайЭлт(Точка p);
	Прям    дайПрямЭлта(int i) const;

	int     дайКурсор() const                          { return cursor; }
	void    устКурсор(int c);
	void    анулируйКурсор();
	bool    курсор_ли() const                           { return cursor >= 0; }

	int     GetSbPos() const                           { return GetSbPos(дайРазм()); }
	void    SetSbPos(int y);
	
	int     дайПромотку() const                          { return sb; }
	void    ScrollTo(int a)                            { sb.уст(a); }

	void    устФрейм(КтрлФрейм& frame);

	void         очисть();
	void         добавь(const Значение& val, bool canselect = true);
	void         добавь(const Значение& val, const Дисплей& дисплей, bool canselect = true);
	void         добавь(const Значение& ключ, const Значение& val, bool canselect = true);
	void         добавь(const Значение& ключ, const Значение& val, const Дисплей& дисплей, bool canselect = true);

	int          дайСчёт() const                     { return элт.дайСчёт(); }
	const Значение& дай(int i) const                     { return элт[i].ключ; }
	const Значение& дайЗначение(int i) const                { return элт[i].значение; }
	const Значение& operator[](int i) const              { return элт[i].ключ; }

	void         уст(int ii, const Значение& ключ, const Значение& val, bool canselect = true);
	void         уст(int ii, const Значение& ключ, const Значение& val, const Дисплей& дисплей, bool canselect = true);
	void         уст(int ii, const Значение& val, bool canselect = true);
	void         уст(int ii, const Значение& val, const Дисплей& дисплей, bool canselect = true);

	void         уст(const Значение& ключ, const Значение& val, const Дисплей& дисплей, bool canselect = true);
	void         уст(const Значение& ключ, const Значение& val, bool canselect = true);

	void         вставь(int ii, const Значение& val, bool canselect = true);
	void         вставь(int ii, const Значение& val, const Дисплей& дисплей, bool canselect = true);
	void         вставь(int ii, const Значение& ключ, const Значение& val, bool canselect = true);
	void         вставь(int ii, const Значение& ключ, const Значение& val, const Дисплей& дисплей, bool canselect = true);
	void         удали(int ii);
	void         удали(const Значение & ключ)			  { int ii = найди(ключ); if (ii >= 0) удали(ii); }

	void         удалиВыделение();

	int          дайСчётВыделений() const               { return selcount; }
	bool         выделение_ли() const                  { return selcount > 0; }
	void         очистьВыделение();
	void         выделиОдин(int i, bool sel);
	bool         выделен(int i) const;
	bool         IsSel(int i) const;

	int          найди(const Значение& ключ) const;

	void         сортируй(const ПорядокЗнач& order);

	Рисунок        дайСэиплТяга();

	void         вставьБрос(int ii, const Вектор<Значение>& data, PasteClip& d, bool self);
	void         вставьБрос(int ii, const Вектор<Значение>& keys, const Вектор<Значение>& data, PasteClip& d, bool self);
	void         вставьБрос(int ii, const СписокКолонок& src, PasteClip& d);
	void         вставьБрос(int ii, PasteClip& d);

	void         сериализуйНастройки(Поток& s);

	СписокКолонок&  Режим(int m);
	СписокКолонок&  ListMode()                           { return Режим(MODE_LIST); }
	СписокКолонок&  RowMode()                            { return Режим(MODE_ROWS); }
	СписокКолонок&  ColumnMode()                         { return Режим(MODE_COLUMN); }
	СписокКолонок&  Columns(int _n)                      { ncl = _n; освежи(); return *this; }
	int          дайКолонки() const                   { return ncl; }
	СписокКолонок&  ItemHeight(int _cy)                  { cy = _cy; освежиВыкладку(); SetSb(); освежи(); return *this; }
	int          дайВысотуЭлта() const                { return cy; }
	СписокКолонок&  ItemWidth(int _cx)                   { cx = _cx; освежиВыкладку(); SetSb(); освежи(); return *this; }
	int          дайШиринуЭлта() const                 { return cx; }
	СписокКолонок&  RoundSize(bool b = true);
	СписокКолонок&  NoRoundSize()                        { return RoundSize(false); }
	СписокКолонок&  ClickKill(bool b = true)             { clickkill = b; return *this; }
	СписокКолонок&  NoClickKill()                        { return ClickKill(false); }
	СписокКолонок&  устДисплей(const Дисплей& d)         { дисплей = &d; return *this; }
	СписокКолонок&  NoBackground(bool b = true)          { nobg = b; Transparent(); освежи(); return *this; }
	СписокКолонок&  Multi(bool b = true)                 { multi = b; return *this; }
	bool         мульти_ли() const                      { return multi; }
	СписокКолонок&  MultiSelect(bool b = true)           { multi = b; return *this; }
	bool         IsMultiSelect() const                { return multi; }
	СписокКолонок&  PopUpEx(bool b = true)               { popupex = b; return *this; }
	СписокКолонок&  NoPopUpEx()                          { return PopUpEx(false); }
	СписокКолонок&  AutoHideSb(bool b = true)            { sb.автоСкрой(b); return *this; }
	СписокКолонок&  NoAutoHideSb()                       { return AutoHideSb(false); }

	СписокКолонок&  устСтильПромотБара(const ПромотБар::Стиль& s) { sb.устСтиль(s); return *this; }

	typedef СписокКолонок ИМЯ_КЛАССА;

	СписокКолонок();
	virtual ~СписокКолонок();
};
