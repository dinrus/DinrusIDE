void DropEdge_Write(Значение);

class PopUpTable : public КтрлМассив {
public:
	virtual void леваяВверху(Точка p, dword keyflags);
	virtual bool Ключ(dword ключ, int);

protected:
	void PopupDeactivate();
	void PopupCancelMode();

	struct Popup : Ктрл {
		PopUpTable *table;
		
		virtual void откл() { table->PopupDeactivate(); }
		virtual void режимОтмены() { table->PopupCancelMode(); }
	};

	int          droplines;
	int          inpopup;
	bool         open;
	Один<Popup>   popup;

	void         DoClose();

public:
	void         PopUp(Ктрл *owner, int x, int top, int bottom, int width);
	void         PopUp(Ктрл *owner, int width);
	void         PopUp(Ктрл *owner);

	Событие<>      WhenCancel;
	Событие<>      WhenSelect;

	PopUpTable&  SetDropLines(int _droplines)          { droplines = _droplines; return *this; }

	void         Normal();

	PopUpTable();
	virtual ~PopUpTable();
};

class СписокБроса : public MultiButton, public Преобр {
public:
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual bool  Ключ(dword ключ, int);
	virtual void  устДанные(const Значение& data);
	virtual Значение дайДанные() const;

	virtual Значение фмт(const Значение& q) const;

private:
	PopUpTable         list;
	Индекс<Значение>       ключ;
	Значение              значение;
	int                dropwidth;
	const Преобр     *valueconvert;
	const Дисплей     *valuedisplay;
	bool               displayall;
	bool               dropfocus;
	bool               notnull;
	bool               alwaysdrop;
	bool               usewheel;

	void          выдели();
	void          Cancel();
	void          Change(int q);
	void          вклБрос(bool b = true)         { MainButton().вкл(b || alwaysdrop); }
	void          синх();

	typedef       СписокБроса ИМЯ_КЛАССА;

public:
	typedef MultiButton::Стиль Стиль;

	Событие<>       WhenDrop;

	СписокБроса&     добавь(const Значение& ключ, const Значение& значение, bool enable = true);
	СписокБроса&     добавь(const Значение& значение)         { return добавь(значение, значение); }
	СписокБроса&     добавь(std::initializer_list<std::pair<Значение, Значение>> init);

	void          удали(int i);
	void          очистьСписок();
	void          очисть();
	
	СписокБроса&     добавьСепаратор();

	void          сбрось();

	const Значение& operator=(const Значение& v)        { устДанные(v); return v; }
	operator Значение() const                        { return дайДанные(); }

	void          SetIndex(int i)                 { устДанные(дайКлюч(i)); }
	int           дайИндекс() const                { return найдиКлюч(значение); }
	void          идиВНач()                       { if(дайСчёт()) SetIndex(0); }
	void          идиВКон()                         { if(дайСчёт()) SetIndex(дайСчёт() - 1); }
	void          идиПредш()                        { Change(-1); }
	void          идиСледщ()                        { Change(1); }

	bool          HasKey(const Значение& k) const    { return найдиКлюч(k) >= 0; }
	int           найдиКлюч(const Значение& k) const;
	int           найди(const Значение& k) const      { return найдиКлюч(k); }
	int           найдиЗначение(const Значение& v) const { return list.найди(v); }

	int           дайСчёт() const                { return ключ.дайСчёт(); }
	void          обрежь(int n);
	const Значение&  дайКлюч(int i) const             { return ключ[i]; }

	Значение         дайЗначение(int i) const           { return list.дай(i, 0); }
	Значение         дайЗначение() const;
	void          устЗначение(int i, const Значение& v);
	void          устЗначение(const Значение& v);
	Значение         operator[](int i) const         { return дайЗначение(i); }

	void          Adjust();
	void          Adjust(const Значение& k);

	const PopUpTable& GetList() const                   { return list; }
	PopUpTable&   ListObject()                          { return list; }

	СписокБроса&     SetDropLines(int d)                   { list.SetDropLines(d); return *this; }
	СписокБроса&     SetValueConvert(const Преобр& cv);
	СписокБроса&     SetConvert(const Преобр& cv);
	СписокБроса&     устДисплей(int i, const Дисплей& d);
	СписокБроса&     устДисплей(const Дисплей& d);
	СписокБроса&     SetLineCy(int i, int lcy)             { list.SetLineCy(i, lcy); return *this; }
	СписокБроса&     SetLineCy(int lcy)                    { list.SetLineCy(lcy); return *this; }
	СписокБроса&     устДисплей(const Дисплей& d, int lcy);
	СписокБроса&     ValueDisplay(const Дисплей& d);
	СписокБроса&     DisplayAll(bool b = true)             { displayall = b; return *this; }
	СписокБроса&     DropFocus(bool b = true)              { dropfocus = b; return *this; }
	СписокБроса&     NoDropFocus()                         { return DropFocus(false); }
	СписокБроса&     бросВсегда(bool e = true);
	СписокБроса&     устСтиль(const Стиль& s)              { MultiButton::устСтиль(s); return *this; }
	СписокБроса&     неПусто(bool b = true)                { notnull = b; return *this; }
	СписокБроса&     DropWidth(int w)                      { dropwidth = w; return *this; }
	СписокБроса&     DropWidthZ(int w)                     { dropwidth = HorzLayoutZoom(w); return *this; }
	СписокБроса&     Wheel(bool b = true)                  { usewheel = b; return *this; }
	СписокБроса&     NoWheel()                             { return Wheel(false); }

	СписокБроса&     устСтильПромотБара(const ПромотБар::Стиль& s) { list.устСтильПромотБара(s); return *this; }

	СписокБроса();
	virtual ~СписокБроса();
};

void приставь(СписокБроса& list, const ВекторМап<Значение, Значение>& values);
void приставь(СписокБроса& list, const ВекторМап<int, Ткст>& values);
void приставь(МапПреобр& convert, const ВекторМап<Значение, Значение>& values);
void приставь(МапПреобр& convert, const ВекторМап<int, Ткст>& values);
void приставь(СписокБроса& list, const МапПреобр& convert);

void operator*=(СписокБроса& list, const ВекторМап<Значение, Значение>& values);
void operator*=(СписокБроса& list, const ВекторМап<int, Ткст>& values);
void operator*=(МапПреобр& convert, const ВекторМап<Значение, Значение>& values);
void operator*=(МапПреобр& convert, const ВекторМап<int, Ткст>& values);
void operator*=(СписокБроса& list, const МапПреобр& convert);

class DropChoice : public MultiButtonFrame {
public:
	virtual void       сериализуй(Поток& s); //empty

protected:
	PopUpTable         list;
	Ктрл              *owner;
	bool               appending : 1;
	bool               dropfocus : 1;
	bool               always_drop : 1;
	bool               hide_drop : 1;
	bool               updownkeys : 1;
	bool               rodrop : 1;

	void        выдели();
	void        сбрось();
	void        вклБрос(bool b);
	void        PseudoPush();

	int         dropwidth;

	typedef DropChoice ИМЯ_КЛАССА;

public:
	Событие<>     WhenDrop;
	Событие<>     WhenSelect;

	bool        DoKey(dword ключ);
	void        DoWheel(int zdelta);

	void        очисть();
	void        добавь(const Значение& data);
	int         найди(const Значение& data) const         { return list.найди(data); }
	void        найдиДобавь(const Значение& data);
	void        уст(int i, const Значение& data)         { list.уст(i, 0, data); }
	void        удали(int i);
	void        SerializeList(Поток& s);
	
	int         дайСчёт() const                      { return list.дайСчёт(); }
	Значение       дай(int i) const                      { return list.дай(i, 0); }

	void        AddHistory(const Значение& data, int max = 12);

	void        добавьК(Ктрл& _owner);
	bool        активен() const                      { return открыт(); }

	Значение       дай() const;
	int         дайИндекс() const;

	DropChoice& устДисплей(int i, const Дисплей& d)   { list.устДисплей(i, 0, d); return *this; }
	DropChoice& устДисплей(const Дисплей& d)          { list.колонкаПо(0).устДисплей(d); return *this; }
	DropChoice& SetLineCy(int lcy)                    { list.SetLineCy(lcy); return *this; }
	DropChoice& устДисплей(const Дисплей& d, int lcy) { устДисплей(d); SetLineCy(lcy); return *this; }
	DropChoice& SetConvert(const Преобр& d)          { list.колонкаПо(0).SetConvert(d); return *this; }
	DropChoice& SetDropLines(int n)                   { list.SetDropLines(n); return *this; }
	DropChoice& Appending()                           { appending = true; return *this; }
	DropChoice& бросВсегда(bool e = true);
	DropChoice& HideDrop(bool e = true)               { hide_drop = e; бросВсегда(always_drop); return *this; }
	DropChoice& RdOnlyDrop(bool e = true)             { rodrop = e; return *this; }
	DropChoice& NoDropFocus()                         { dropfocus = false; return *this; }

	DropChoice& DropWidth(int w)                      { dropwidth = w; return *this; }
	DropChoice& DropWidthZ(int w)                     { dropwidth = HorzLayoutZoom(w); return *this; }
	DropChoice& UpDownKeys(bool b = true)             { updownkeys = b; return *this; }

	DropChoice& устСтильПромотБара(const ПромотБар::Стиль& s) { list.устСтильПромотБара(s); return *this; }

	DropChoice();

	static bool DataSelect(Ктрл& owner, DropChoice& drop, const Ткст& appends);
};

template <class T>
class WithDropChoice : public T {
public:
	virtual bool   Ключ(dword ключ, int repcnt);
	virtual void   входМыши(Точка p, dword keyflags);
	virtual void   выходМыши();
	virtual void   колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void   сфокусирован();
	virtual void   расфокусирован();

protected:
	DropChoice      select;
	Ткст          appends;
	bool            withwheel;

	void            DoWhenSelect();
	void            DoWhenDrop()                          { WhenDrop(); }

public:
	Событие<>         WhenDrop;
	Событие<>         WhenSelect;

	void            очистьСписок()                           { select.очисть(); }
	void            добавьСписок(const Значение& data)            { select.добавь(data); }
	void            найдиДобавьСписок(const Значение& data)        { select.найдиДобавь(data); }
	int             найди(const Значение& data) const         { return select.найди(data); }
	void            уст(int i, const Значение& data)         { select.уст(i, data); }
	void            удали(int i)                         { select.удали(i); }
	void            SerializeList(Поток& s)              { select.SerializeList(s); }

	int             дайСчёт() const                      { return select.дайСчёт(); }
	Значение           дай(int i) const                      { return select.дай(i); }

	void            AddHistory(int max = 12)              { select.AddHistory(this->дайДанные(), max); }

	MultiButton::SubButton& AddButton()                   { return select.AddButton(); }
	int                     GetButtonCount() const        { return select.GetButtonCount(); }
	MultiButton::SubButton& GetButton(int i)              { return select.GetButton(i); }
	Прям                    GetPushScreenRect() const     { return select.GetPushScreenRect(); }

	const MultiButton::Стиль& дефСтиль()              { return select.StyleFrame(); }
	WithDropChoice& устСтиль(const MultiButton::Стиль& s) { select.устСтиль(s); return *this; }

	WithDropChoice& Dropping(bool b = true)               { select.MainButton().покажи(b); return *this; }
	WithDropChoice& NoDropping()                          { return Dropping(false); }
	WithDropChoice& NoDropFocus()                         { select.NoDropFocus(); return *this; }
	WithDropChoice& Appending(const Ткст& s = ", ")     { appends = s; select.Appending(); return *this; }
	WithDropChoice& SetDropLines(int n)                   { select.SetDropLines(n); return *this; }
	WithDropChoice& устДисплей(int i, const Дисплей& d)   { select.устДисплей(i, d); return *this; }
	WithDropChoice& устДисплей(const Дисплей& d)          { select.устДисплей(d); return *this; }
	WithDropChoice& SetLineCy(int lcy)                    { select.SetLineCy(lcy); return *this; }
	WithDropChoice& устДисплей(const Дисплей& d, int lcy) { select.устДисплей(d, lcy); return *this; }
	WithDropChoice& SetConvert(const Преобр& d)          { select.SetConvert(d); return *this; }
	WithDropChoice& бросВсегда(bool b = true)             { select.бросВсегда(b); return *this; }
	WithDropChoice& HideDrop(bool b = true)               { select.HideDrop(b); return *this; }
	WithDropChoice& RdOnlyDrop(bool b = true)             { select.RdOnlyDrop(b); return *this; }
	WithDropChoice& WithWheel(bool b = true)              { withwheel = b; return *this; }
	WithDropChoice& NoWithWheel()                         { return WithWheel(false); }
	WithDropChoice& DropWidth(int w)                      { select.DropWidth(w); return *this; }
	WithDropChoice& DropWidthZ(int w)                     { select.DropWidthZ(w); return *this; }
	WithDropChoice& UpDownKeys(bool b = true)             { select.UpDownKeys(b); return *this; }
	WithDropChoice& NoUpDownKeys()                        { return UpDownKeys(false); }

	WithDropChoice();
};

template <class T>
WithDropChoice<T>::WithDropChoice() {
	select.WhenDrop = callback(this, &WithDropChoice::DoWhenDrop);
	select.WhenSelect = callback(this, &WithDropChoice::DoWhenSelect);
	appends = Ткст::дайПроц();
	withwheel = true;
	устСтиль(дефСтиль());
	select.добавьК(*this);
}

template <class T>
bool WithDropChoice<T>::Ключ(dword ключ, int repcnt) {
	return select.DoKey(ключ) || T::Ключ(ключ, repcnt);
}

template <class T>
void WithDropChoice<T>::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	if(withwheel)
		select.DoWheel(zdelta);
}

template <class T>
void WithDropChoice<T>::входМыши(Точка p, dword keyflags)
{
	select.освежи();
	T::входМыши(p, keyflags);
}

template <class T>
void WithDropChoice<T>::выходМыши()
{
	select.освежи();
	T::выходМыши();
}

template <class T>
void WithDropChoice<T>::сфокусирован()
{
	select.освежи();
	T::сфокусирован();
}

template <class T>
void WithDropChoice<T>::расфокусирован()
{
	select.освежи();
	T::расфокусирован();
}

template <class T>
void WithDropChoice<T>::DoWhenSelect() {
	if(DropChoice::DataSelect(*this, select, appends)) {
		this->устФокус();
		WhenSelect();
	}
}
