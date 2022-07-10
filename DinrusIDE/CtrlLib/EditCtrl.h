struct TextArrayOps {
	virtual int64 дайВсего() const = 0;
	virtual int   дайСимПо(int64 i) const = 0;

	bool   GetWordSelection(int64 c, int64& sell, int64& selh);
	int64  GetNextWord(int64 c);
	int64  GetPrevWord(int64 c);

	virtual ~TextArrayOps() {}
};

class LookFrame : public КтрлФрейм {
public:
	virtual void выложиФрейм(Прям& r);
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);

private:
	Значение (*look)();
	Значение lookv;
	Значение дай() const { return look ? (*look)() : lookv; }

public:
	void  уст(const Значение& v)                  { look = NULL; lookv = v; }
	void  уст(Значение (*l)())                    { look = l; }
	LookFrame(Значение (*look)()) : look(look)    {}
	LookFrame(const Значение& v) : lookv(v) { look = NULL; }
	LookFrame() { look = NULL; }
};

Значение EditFieldEdge();
Значение ViewEdge();

void ViewEdge_Write(Значение);

КтрлФрейм& EditFieldFrame();
КтрлФрейм& ViewFrame();

class EditField : public Ктрл, private TextArrayOps {
public:
	virtual void  Выкладка();
	virtual void  рисуй(Draw& draw);
	virtual void  леваяВнизу(Точка p, dword keyflags);
	virtual void  MiddleDown(Точка p, dword keyflags);
	virtual void  двигМыши(Точка p, dword keyflags);
	virtual void  леваяДКлик(Точка p, dword keyflags);
	virtual void  LeftTriple(Точка p, dword keyflags);
	virtual void  леваяТяг(Точка p, dword flags);
	virtual void  леваяВверху(Точка p, dword flags);
	virtual void  праваяВнизу(Точка p, dword keyflags);
	virtual void  входМыши(Точка p, dword keyflags);
	virtual void  выходМыши();
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual void  тягИБрос(Точка p, PasteClip& d);
	virtual void  тягПовтори(Точка p);
	virtual void  тягПокинь();
	virtual bool  Ключ(dword ключ, int rep);
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual Размер  дайМинРазм() const;
	virtual void  устДанные(const Значение& data);
	virtual Значение дайДанные() const;
	virtual void  режимОтмены();
	virtual Ткст GetSelectionData(const Ткст& fmt) const;
	virtual void   State(int);

public:
	struct Стиль : ChStyle<Стиль> {
		Цвет paper;
		Цвет disabled;
		Цвет focus;
		Цвет invalid;
		Цвет text, textdisabled;
		Цвет selected, selectedtext;
		Цвет selected0, selectedtext0; // If does not have focus...
		Значение edge[4]; // border (for various active edge states)
		Значение coloredge; // border mask for adding color, e.g. round borders with red Ошибка
		bool  activeedge;
		int   vfm;
	};
	
	struct Highlight : Движимое<Highlight> {
		Цвет ink;
		Цвет paper;
		Цвет underline;

		bool operator!=(const Highlight& b) const { return ink != b.ink || paper != b.paper || underline != b.underline; }
	};

protected:
	const Стиль *style;
	
	ActiveEdgeFrame edge;

	ШТкст    text;
	int        sc;
	int        cursor, anchor;

	ШТкст    undotext;
	int        undocursor, undoanchor;

	CharFilter      filter;
	const Преобр  *convert;
	const Преобр  *inactive_convert;
	Шрифт            font;
	Цвет           textcolor;

	ШТкст         nulltext;
	Цвет           nullink;
	Шрифт            nullfont;
	Рисунок           nullicon;
	int             maxlen;
	int             autosize;
	byte            charset;
	int             fsell, fselh; // used to hold selection after расфокусирован for X11 middle mouse copy

	int        dropcursor;
	Прям       dropcaret;
	bool       selclick;

	bool       password:1;
	bool       autoformat:1;
	bool       initcaps:1;
	bool       keep_selection:1;
	bool       clickselect:1;
	bool       nobg:1;
	bool       alignright:1;
	bool       errorbg:1;
	bool       showspaces:1;
	bool       no_internal_margin:1;

	bool    FrameIsEdge();
	void    SetEdge(int i);
	void    RefreshAll();
	int     LowChar(int c) const { return 0x25af /*c + 0x2400*/; }
	int     GetCharWidth(int c) const { return font[c < 32 ? LowChar(c) : c]; }
	int     GetTextCx(const wchar *text, int n, bool password, Шрифт fnt) const;
	void    Paints(Draw& w, int& x, int fcy, const wchar *&txt,
		           Цвет ink, Цвет paper, int n, bool pwd, Шрифт fnt, Цвет underline, bool showspaces);
	int     GetStringCx(const wchar *text, int n);
	int     дайКаретку(int cursor) const;
	int     дайКурсор(int posx);
	void    SyncCaret();
	void    финиш(bool refresh = true);
	void    SaveUndo();
	void    DoAutoFormat();
	int     GetTy() const;
	void    StdPasteFilter(ШТкст&);
	void    SelSource();
	Цвет   GetPaper();
	int     GetRightSpace() const;

protected:
	virtual void  HighlightText(Вектор<Highlight>& hl);
	virtual int64 дайВсего() const             { return text.дайДлину(); }
	virtual int   дайСимПо(int64 pos) const   { return text[(int)pos]; }

public:
	Событие<Бар&>               WhenBar;
	Событие<>                   WhenEnter;
	Событие<ШТкст&>           WhenPasteFilter;
	Событие<Вектор<Highlight>&> WhenHighlight;
	Событие<Цвет>              WhenPaper; // needed to improve visuals of DropChoice

	static const Стиль& дефСтиль();
	EditField&  устСтиль(const Стиль& s);

	static  int   GetViewHeight(Шрифт font = StdFont());
	static  int   GetStdHeight(Шрифт font = StdFont());

	int     вставь(int pos, const ШТкст& text);
	int     вставь(int pos, const Ткст& text)         { return вставь(pos, text.вШТкст()); }
	int     вставь(int pos, const char *text)           { return вставь(pos, ШТкст(text)); }
	void    удали(int pos, int n);

	void    вставь(const ШТкст& text);
	void    вставь(const Ткст& text)                  { return вставь(text.вШТкст()); }
	void    вставь(const char *text)                    { return вставь(ШТкст(text)); }
	void    вставь(int chr);

	void    Move(int newpos, bool select = false);

	void    устВыделение(int l = 0, int h = INT_MAX);
	bool    дайВыделение(int& l, int& h) const;
	bool    выделение_ли() const;
	bool    удалиВыделение();
	void    CancelSelection();
	void    копируй();
	void    Undo();
	void    вырежь();
	void    Paste();
	void    Erase();
	void    выбериВсе();

	void    StdBar(Бар& menu);

	void           устТекст(const ШТкст& text);
	void           устТекст(const Ткст& t)  { устТекст(t.вШТкст()); }
	void           устТекст(const char *t)    { устТекст(ШТкст(t)); }
	const ШТкст& дайТекст() const           { return text; }

	operator const ШТкст&() const          { return text; } // Deprecated, use ~
	void operator=(const ШТкст& s)         { устТекст(s); } // Deprecated, use operator<<=
	int     дайДлину() const                { return text.дайДлину(); }
	int     дайСим(int i) const             { return text[i]; }

	Прям    GetCaretRect(int pos) const;
	Прям    GetCaretRect() const             { return GetCaretRect(cursor); }

	void    очисть();
	void    переустанов();
	
	void    Ошибка(bool Ошибка = true)         { if(errorbg != Ошибка) { errorbg = Ошибка; RefreshAll(); } }
	
	EditField& Password(bool pwd = true)     { password = pwd; финиш(); return *this; }
	bool       IsPassword() const            { return password; }
	EditField& устФильтр(int (*f)(int))      { filter = f; return *this; }
	EditField& SetConvert(const Преобр& c)  { convert = &c; освежи(); return *this; }
	EditField& SetInactiveConvert(const Преобр& c) { inactive_convert = &c; освежи(); return *this; }
	EditField& AutoFormat(bool b = true)     { autoformat = b; return *this; }
	EditField& NoAutoFormat()                { return AutoFormat(false); }
	bool       IsAutoFormat() const          { return autoformat; }
	EditField& устНабсим(byte cs)           { charset = cs; return *this; }
	EditField& устШрифт(Шрифт _font);
	EditField& устЦвет(Цвет c);
	EditField& ClickSelect(bool b = true)    { clickselect = b; return *this; }
	bool       IsClickSelect() const         { return clickselect; }
	EditField& иницШапки(bool b = true)       { initcaps = b; return *this; }
	bool       IsInitCaps() const            { return initcaps; }
	EditField& NullText(const Рисунок& icon, const char *text = t_("(default)"), Цвет ink = SColorDisabled);
	EditField& NullText(const Рисунок& icon, const char *text, Шрифт fnt, Цвет ink);
	EditField& NullText(const char *text = t_("(default)"), Цвет ink = SColorDisabled);
	EditField& NullText(const char *text, Шрифт fnt, Цвет ink);
	EditField& MaxChars(int mc)              { maxlen = mc; return *this; }
	int        GetMaxChars() const           { return maxlen; }
	EditField& AutoSize(int maxcx = INT_MAX) { autosize = maxcx; финиш(); return *this; }
	EditField& NoBackground(bool b = true)   { nobg = b; Transparent(); освежи(); return *this; }
	EditField& AlignRight(bool b = true)     { alignright = b; освежи(); return *this; }
	bool       IsNoBackground() const        { return nobg; }
	bool       IsAlignRight() const          { return alignright; }
	EditField& ShowSpaces(bool b = true)     { showspaces = b; освежи(); return *this; }
	EditField& NoInternalMargin(bool b = true) { no_internal_margin = b; return *this; }

	CharFilter     GetFilter() const         { return filter; }
	const Преобр& GetConvert() const        { return *convert; }
	Шрифт           дайШрифт() const           { return font; }

	typedef EditField ИМЯ_КЛАССА;

	EditField();
	virtual ~EditField();
};

template <class DataType, class Cv>
class EditValue : public EditField, public Cv {
public:
	EditValue& operator=(const DataType& t)  { EditField::устДанные(t); return *this; }  // Deprecated, use operator<<=
	operator DataType() const                { return EditField::дайДанные(); } // Deprecated, use ~

	EditValue()                              { SetConvert(*this); }
};

template <class DataType, class Cv>
class EditMinMax : public EditValue<DataType, Cv> {
public:
	EditMinMax& operator=(const DataType& t)          { EditField::устДанные(t); return *this; }  // Deprecated, use operator<<=

	EditMinMax() {}
	EditMinMax(DataType min, DataType max)            { Cv::минмакс(min, max); }

	EditMinMax& мин(DataType min)                     { Cv::мин(min); Ктрл::освежи(); return *this; }
	EditMinMax& макс(DataType max)                     { Cv::макс(max); Ктрл::освежи(); return *this; }
	EditMinMax& минмакс(DataType min, DataType max)    { мин(min); return макс(max); }
	EditMinMax& неПусто(bool nn = true)               { Cv::неПусто(nn); Ктрл::освежи(); return *this; }
};

template <class DataType, class Основа>
class EditMinMaxNotNull : public Основа {
public:
	EditMinMaxNotNull& operator=(const DataType& t)   { EditField::устДанные(t); return *this; }  // Deprecated, use operator<<=

	EditMinMaxNotNull()                               { Основа::неПусто(); }
	EditMinMaxNotNull(DataType min, DataType max)     { Основа::неПусто(); Основа::минмакс(min, max); }

	EditMinMaxNotNull& мин(DataType min)              { Основа::мин(min); return *this; }
	EditMinMaxNotNull& макс(DataType max)              { Основа::макс(max); return *this; }
	EditMinMaxNotNull& минмакс(DataType min, DataType max) { Основа::минмакс(min, max); return *this; }
	EditMinMaxNotNull& неПусто(bool nn = true)        { Основа::неПусто(nn); return *this; }
};

typedef EditMinMax<int, ПреобрЦел>              EditInt;
typedef EditMinMax<int64, ПреобрЦел64>          EditInt64;
typedef EditMinMax<double, ПреобрДво>        EditDouble;
typedef EditMinMax<Дата, ПреобрДату>            EditDate;
typedef EditMinMax<Время, ПреобрВремя>            EditTime;
typedef EditMinMaxNotNull<int, EditInt>          EditIntNotNull;
typedef EditMinMaxNotNull<int64, EditInt64>      EditInt64NotNull;
typedef EditMinMaxNotNull<double, EditDouble>    EditDoubleNotNull;
typedef EditMinMaxNotNull<Дата, EditDate>        EditDateNotNull;
typedef EditMinMaxNotNull<Время, EditTime>        EditTimeNotNull;

class EditString : public EditValue<ШТкст, ПреобрТкст> {
public:
	operator const ШТкст&() const                  { return дайТекст(); }  // Deprecated, use ~

	EditString& operator=(const ШТкст& data)       { устДанные(data); return *this; } // Deprecated, use operator<<=
	EditString& operator=(const Ткст& data)        { устДанные(data); return *this; } // Deprecated, use operator<<=

	EditString() {}
	EditString(int maxlen)                           { максдлин(maxlen); }

	EditString& максдлин(int maxlen)                   { ПреобрТкст::максдлин(maxlen); Ктрл::освежи(); return *this; }
	EditString& неПусто(bool nn = true)              { ПреобрТкст::неПусто(nn); Ктрл::освежи(); return *this; }
	EditString& обрежьЛево(bool b = true)              { ПреобрТкст::обрежьЛево(b); Ктрл::освежи(); return *this; }
	EditString& обрежьПраво(bool b = true)             { ПреобрТкст::обрежьПраво(b); Ктрл::освежи(); return *this; }
	EditString& обрежьОба(bool b = true)              { ПреобрТкст::обрежьОба(b); Ктрл::освежи(); return *this; }
};

class EditStringNotNull : public EditString {
public:
	EditStringNotNull& operator=(const ШТкст& data) { устДанные(data); return *this; } // Deprecated, use operator<<=
	EditStringNotNull& operator=(const Ткст& data)  { устДанные(data); return *this; } // Deprecated, use operator<<=

	EditStringNotNull()                               { неПусто(); }
	EditStringNotNull(int maxlen)                     { неПусто(); максдлин(maxlen); }
};

template <class IncType> IncType WithSpin_DefaultIncValue() { return 1; }
template <> inline       double  WithSpin_DefaultIncValue() { return 0.1; }

template <class IncType> IncType WithSpin_DefaultStartValue() { return 0; }
template <> inline       Дата    WithSpin_DefaultStartValue() { return дайСисДату(); }
template <> inline       Время    WithSpin_DefaultStartValue() { return дайСисВремя(); }

template <class DataType, class IncType>
void WithSpin_Add(DataType& значение, IncType inc, DataType min, bool roundfrommin)
{
	значение += inc;
}

template <> inline
void WithSpin_Add(int& значение, int inc, int min, bool roundfrommin) {
	if(roundfrommin)
		значение -= min;
	if(inc < 0) {
		inc = -inc;
		значение = (значение - inc) / inc * inc;
	}
	else
		значение = (значение + inc) / inc * inc;
	if(roundfrommin)
		значение += min;
}

template <> inline
void WithSpin_Add(double& значение, double inc, double min, bool roundfrommin) {
	if(roundfrommin)
		значение -= min;
	if(inc < 0) {
		inc = -inc;
		значение = (ceil(значение / inc - inc / 100) - 1) * inc;
	}
	else
		значение = (floor(значение / inc + inc / 100) + 1) * inc;
	if(roundfrommin)
		значение += min;
}

template <class DataType, class Основа, class IncType = DataType>
class WithSpin : public Основа {
public:
	virtual void колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual bool Ключ(dword ключ, int repcnt);

protected:
	void            Inc();
	void            Dec();
	void            иниц();

private:
	СпинКнопки     sb;
	IncType         inc;
	bool            roundfrommin;
	bool            mousewheel = true;
	bool            keys = true;

	typedef WithSpin ИМЯ_КЛАССА;
public:

	WithSpin&          SetInc(IncType _inc = 1)     { inc = _inc; return *this; }
	DataType           GetInc() const               { return inc; }

	WithSpin&          OnSides(bool b = true)       { sb.OnSides(b); return *this; }
	bool               IsOnSides() const            { return sb.IsOnSides(); }
	
	WithSpin&          ShowSpin(bool s = true)      { sb.покажи(s); return *this; }
	bool               IsSpinVisible() const        { return sb.видим_ли(); }
	
	WithSpin&          RoundFromMin(bool b = true)  { roundfrommin = b; return *this; }
	
	WithSpin&          MouseWheelSpin(bool b = true){ mousewheel = b; return *this; }
	WithSpin&          NoMouseWheelSpin()           { return MouseWheelSpin(false); }
	
	WithSpin&          KeySpin(bool b = true)       { keys = b; return *this; }
	WithSpin&          NoKeySpin()                  { return KeySpin(false); }

	СпинКнопки&       SpinButtonsObject()          { return sb; }
	const СпинКнопки& SpinButtonsObject() const    { return sb; }

	WithSpin();
	WithSpin(IncType inc); // deprecated
	WithSpin(DataType min, DataType max, IncType inc); // deprecated
	virtual ~WithSpin() {}
};

template <class DataType, class Основа, class IncType>
WithSpin<DataType, Основа, IncType>::WithSpin()
:	inc(WithSpin_DefaultIncValue<IncType>())
{
	иниц();
}

template <class DataType, class Основа, class IncType>
WithSpin<DataType, Основа, IncType>::WithSpin(IncType inc)
:	inc(inc)
{
	иниц();
}

template <class DataType, class Основа, class IncType>
WithSpin<DataType, Основа, IncType>::WithSpin(DataType min, DataType max, IncType inc)
:	inc(WithSpin_DefaultIncValue<IncType>())
{
	Основа::минмакс(min, max);
	иниц();
}

template <class DataType, class Основа, class IncType>
void WithSpin<DataType, Основа, IncType>::иниц()
{
	Ктрл::добавьФрейм(sb);
	sb.inc.WhenRepeat = sb.inc.WhenAction = THISBACK(Inc);
	sb.dec.WhenRepeat = sb.dec.WhenAction = THISBACK(Dec);
	roundfrommin = false;
}

template <class DataType, class Основа, class IncType>
void WithSpin<DataType, Основа, IncType>::Inc()
{
	if(Ктрл::толькочтен_ли()) {
		бипВосклицание();
		return;
	}
	DataType d = Основа::дайДанные();
	if(!пусто_ли(d)) {
		WithSpin_Add(d, inc, Основа::дайМин(), roundfrommin);
		if(пусто_ли(Основа::дайМакс()) || d <= Основа::дайМакс()) {
			Основа::устДанные(d);
			Ктрл::Action();
		}
	}
	else {
		DataType min = Основа::дайМин();
		if(пусто_ли(min) || min <= Основа::дайДефМин())
			Основа::устДанные(WithSpin_DefaultStartValue<DataType>());
		else
			Основа::устДанные(min);
	}
	Ктрл::устФокус();
}

template <class DataType, class Основа, class IncType>
void WithSpin<DataType, Основа, IncType>::Dec()
{
	if(Ктрл::толькочтен_ли()) {
		бипВосклицание();
		return;
	}
	DataType d = Основа::дайДанные();
	if(!пусто_ли(d)) {
		WithSpin_Add(d, -inc, Основа::дайМин(), roundfrommin);
		if(пусто_ли(Основа::дайМин()) || d >= Основа::дайМин()) {
			Основа::устДанные(d);
			Ктрл::Action();
		}
	}
	else {
		DataType max = Основа::дайМакс();
		if(пусто_ли(max) || max >= Основа::дайДефМакс())
			Основа::устДанные(WithSpin_DefaultStartValue<DataType>());
		else
			Основа::устДанные(max);
	}
	Ктрл::устФокус();
}

template <class DataType, class Основа, class IncType>
bool WithSpin<DataType, Основа, IncType>::Ключ(dword ключ, int repcnt)
{
	if(keys) {
		if(ключ == K_UP) {
			Inc();
			return true;
		}
		if(ключ == K_DOWN) {
			Dec();
			return true;
		}
	}
	return Основа::Ключ(ключ, repcnt);
}

template <class DataType, class Основа, class IncType>
void WithSpin<DataType, Основа, IncType>::колесоМыши(Точка, int zdelta, dword)
{
	if(mousewheel) {
		if(zdelta < 0)
			Dec();
		else
			Inc();
	}
}

typedef WithSpin<int, EditInt>               EditIntSpin;
typedef WithSpin<int64, EditInt64>           EditInt64Spin;
typedef WithSpin<double, EditDouble>         EditDoubleSpin;
typedef WithSpin<Дата, EditDate, int>        EditDateSpin;
typedef WithSpin<Время, EditTime, int>        EditTimeSpin;

typedef WithSpin<int, EditIntNotNull>        EditIntNotNullSpin;
typedef WithSpin<int64, EditInt64NotNull>    EditInt64NotNullSpin;
typedef WithSpin<double, EditDoubleNotNull>  EditDoubleNotNullSpin;
typedef WithSpin<Дата, EditDateNotNull, int> EditDateNotNullSpin;
typedef WithSpin<Время, EditTimeNotNull, int> EditTimeNotNullSpin;
