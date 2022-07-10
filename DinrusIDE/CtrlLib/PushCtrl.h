class Толкач : public Ктрл {
public:
	virtual void   режимОтмены();
	virtual void   леваяВнизу(Точка, dword);
	virtual void   двигМыши(Точка, dword);
	virtual void   выходМыши();
	virtual void   леваяПовтори(Точка, dword);
	virtual void   леваяВверху(Точка, dword);
	virtual void   сфокусирован();
	virtual void   расфокусирован();
	virtual void   State(int);
	virtual Ткст дайОпис() const;
	virtual bool   Ключ(dword ключ, int);
	virtual bool   горячаяКлав(dword ключ);
	virtual dword  дайКлючиДоступа() const;
	virtual void   присвойКлючиДоступа(dword used);

private:
	bool    push:1;
	bool    keypush:1;
	bool    clickfocus:1;

	void    EndPush();

protected:
	int     accesskey;
	Ткст  label;
	Шрифт    font;

	void    KeyPush();
	bool    IsPush() const                                  { return push || keypush; }
	bool    IsKeyPush()                                     { return keypush; }
	bool    FinishPush();

protected:
	virtual void  RefreshPush();
	virtual void  освежиФокус();
	virtual void  выполниАкцию();

public:
	Толкач&  устШрифт(Шрифт fnt);
	Толкач&  устНадпись(const char *text);
	Толкач&  ClickFocus(bool cf = true);
	Толкач&  NoClickFocus()                                 { return ClickFocus(false); }
	bool     IsClickFocus() const                           { return clickfocus; }
	
	Шрифт     дайШрифт() const                                { return font; }
	Ткст   GetLabel() const                               { return label; }

	void     PseudoPush();

	int      GetVisualState() const;

	Событие<>  WhenPush;
	Событие<>  WhenRepeat;

	Толкач();
	virtual ~Толкач();
};

class Кнопка : public Толкач {
public:
	virtual void   рисуй(Draw& draw);
	virtual bool   Ключ(dword ключ, int);
	virtual bool   горячаяКлав(dword ключ);
	virtual void   входМыши(Точка, dword);
	virtual void   выходМыши();
	virtual dword  дайКлючиДоступа() const;
	virtual void   присвойКлючиДоступа(dword used);
	virtual void   Выкладка();
	virtual void   сфокусирован();
	virtual void   расфокусирован();
	virtual int    рисуйПоверх() const;

public:
	struct Стиль : ChStyle<Стиль> {
		Значение look[4];
		Цвет monocolor[4], textcolor[4];
		Точка pressoffset;
		int   focusmargin;
		int   overpaint;
		Шрифт  font;
		Рисунок ok, cancel, exit;
		bool  transparent;
		bool  focus_use_ok;
	};

protected:
	enum { NORMAL, OK, CANCEL, EXIT };
	const Стиль *style;
	Рисунок   img;
	bool    monoimg;
	byte    тип;

	void  RefreshOK(Ктрл *p);
	const Стиль *St() const;

public:
	Кнопка&  устРисунок(const Рисунок& img);
	Кнопка&  SetMonoImage(const Рисунок& img);

	static const Стиль& StyleNormal();
	static const Стиль& StyleOk();
	static const Стиль& StyleEdge();
	static const Стиль& StyleLeftEdge();
	static const Стиль& StyleScroll();
	static const Стиль& StyleNaked();

	Кнопка&  устСтиль(const Стиль& s);
	Кнопка&  AutoStyle();

	Кнопка&  NormalStyle()                        { return устСтиль(StyleNormal()); }
	Кнопка&  EdgeStyle()                          { return устСтиль(StyleEdge()); }
	Кнопка&  LeftEdgeStyle()                      { return устСтиль(StyleLeftEdge()); }
	Кнопка&  ScrollStyle()                        { return устСтиль(StyleScroll()); }
	Кнопка&  NakedStyle()                         { return устСтиль(StyleNaked()); }

	Кнопка&  Ok();
	Кнопка&  Cancel();
	Кнопка&  выход();
	Кнопка&  Normal()                             { тип = NORMAL; освежи(); return *this; }

	Кнопка();
	virtual ~Кнопка();
};

Цвет ButtonMonoColor(int i);

class СпинКнопки : public КтрлФрейм {
public:
	virtual void выложиФрейм(Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);
	virtual void добавьКФрейму(Ктрл& ctrl);
	virtual void удалиФрейм();

public:
	struct Стиль : ChStyle<Стиль> {
		Кнопка::Стиль inc;
		Кнопка::Стиль dec;
		int           width;
		int           over;
		bool          onsides;
	};

private:
	bool         visible;
	const Стиль *style;

public:
	Кнопка inc;
	Кнопка dec;

	void         покажи(bool s = true);
	bool         видим_ли() const          { return visible; }

	static const Стиль& дефСтиль();
	static const Стиль& StyleOnSides();

	СпинКнопки& устСтиль(const Стиль& s);
	
	СпинКнопки& OnSides(bool b = true)     { return устСтиль(b ? StyleOnSides() : дефСтиль()); }
	bool         IsOnSides() const          { return style->onsides; }

	СпинКнопки();
	virtual ~СпинКнопки();
};

class Опция : public Толкач {
public:
	virtual void   рисуй(Draw& draw);
	virtual Размер   дайМинРазм() const;
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;
	virtual void   входМыши(Точка, dword);
	virtual void   выходМыши();
	virtual void   State(int);

protected:
	virtual void  RefreshPush();
	virtual void  освежиФокус();
	virtual void  выполниАкцию();

protected:
	Рисунок  edge, edged;
	int    option;
	bool   switchimage;
	bool   threestate;
	bool   notnull;
	bool   blackedge;
	bool   showlabel;
	bool   box;
	bool   autobox;
	Цвет  color;
	
	void   автоСинх();

public:
	Опция& уст(int b);
	int     дай() const                           { return option; }

	operator int() const                          { return option; }
	void operator=(int b)                         { уст(b); }
	
	void    активируйБокс(bool b);
	void    активируйБокс()                           { активируйБокс(option); }

	Опция& BlackEdge(bool b = true)              { blackedge = b; освежи(); return *this; }
	bool    IsBlackEdge() const                   { return blackedge; }
	Опция& SwitchImage(bool b = true)            { switchimage = b; освежи(); return *this; }
	bool    IsSwitchImage() const                 { return switchimage; }
	Опция& ThreeState(bool b = true)             { threestate = b; notnull = false; return *this; }
	bool    IsThreeState() const                  { return threestate; }
	Опция& ShowLabel(bool b = true)              { showlabel = b; освежи(); return *this; }
	bool    IsShowLabel() const                   { return showlabel; }
	Опция& неПусто(bool nn = true)               { notnull = nn; освежи(); return *this; }
	Опция& NoNotNull()                           { return неПусто(false); }
	bool    неПусто_ли() const                     { return notnull; }
	Опция& устЦвет(Цвет c)                     { color = c; освежи(); return *this; }
	Опция& Box(bool b = true)                    { box = b; return *this; }
	Опция& AutoBox(bool b = true)                { Box(autobox = b); return *this; }

	Опция();
	virtual ~Опция();
};

class OptionBox : public Опция {
public:
	OptionBox() { Box(); }
};

class ButtonOption : public Ктрл {
public:
	virtual void  рисуй(Draw& w);
	virtual void  леваяВнизу(Точка, dword);
	virtual void  леваяВверху(Точка, dword);
	virtual void  двигМыши(Точка, dword);
	virtual void  входМыши(Точка, dword);
	virtual void  выходМыши();
	virtual void  устДанные(const Значение& v);
	virtual Значение дайДанные() const;
	virtual void  сериализуй(Поток& s);
	virtual dword дайКлючиДоступа() const;
	virtual void  присвойКлючиДоступа(dword used);

public:
	struct Стиль : ChStyle<Стиль> {
		Значение look[4];
		Цвет textcolor[4];
		bool  drawfocus;
	};

private:
	Рисунок        image;
	Рисунок        image1;
	Ткст       label;
	const Стиль *style;
	int          accesskey;
	bool         option;
	bool         push;

public:
	ButtonOption&  устРисунок(const Рисунок& img)                 { image = img; освежи(); return *this; }
	ButtonOption&  устРисунок(const Рисунок& m, const Рисунок& m1)  { image = m; image1 = m1; освежи(); return *this; }
	void operator=(const Рисунок& img)                          { устРисунок(img); }
	
	ButtonOption& устНадпись(const Ткст& text);
	Ткст GetLabel() const                                   { return label; }
	
	void уст(bool b)                                          { option = b; обновиОсвежи(); }
	bool дай() const                                          { return option; }

	void operator=(bool b)                                    { уст(b); }
	operator bool() const                                     { return дай(); }

	static const Стиль& дефСтиль();
	static const Стиль& StyleFlat();
	
	ButtonOption& устСтиль(const Стиль& s)                    { style = &s; освежи(); return *this; }
	ButtonOption& AutoStyle()                                 { style = NULL; освежи(); return *this; }

	ButtonOption();
};

class Switch : public Ктрл {
public:
	virtual void   рисуй(Draw& draw);
	virtual void   режимОтмены();
	virtual void   двигМыши(Точка p, dword keyflags);
	virtual void   леваяВнизу(Точка p, dword keyflags);
	virtual void   леваяВверху(Точка p, dword keyflags);
	virtual void   выходМыши();
	virtual bool   Ключ(dword ключ, int count);
	virtual bool   горячаяКлав(dword ключ);
	virtual dword  дайКлючиДоступа() const;
	virtual void   присвойКлючиДоступа(dword used);
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;
	virtual void   сфокусирован();
	virtual void   расфокусирован();

public:
	struct Case  {
		Ткст label;
		Значение  значение;
		int    accesskey = 0;
		bool   enabled = true;
		int    gap = 0;
		Прям16 rect = Прям16(0, 0, 0, 0);
	};

private:
	Шрифт         font;
	Значение        значение;
	int          pushindex;
	Массив<Case>  cs;
	int          linecy;
	int          light;
	int          mincy;
	int          direction;

	int   дайИндекс() const;
	int   дайИндекс(Точка p);
	bool  DoHot(dword ключ);
	void  Updates();

public:
	enum { GAP_SEPARATOR = 1 << 20 };

	Switch& устНадпись(int i, const char *text, int gap = 0);
	Switch& устНадпись(const char *text);
	Ткст  GetLabel() const                                    { return GetLabel(дайИндекс()); }
	Ткст  GetLabel(int i) const                               { return cs[i].label; }
	Switch& уст(int i, const Значение& val, const char *text, int gap = 0);
	Switch& уст(int i, const Значение& val);
	Switch& добавь(const Значение& val, const char *text, int gap = 0);
	Switch& добавь(const char *text, int gap = 0);

	void    EnableCase(int i, bool enable = true);
	void    DisableCase(int i)                                  { EnableCase(i, false); }

	void    EnableValue(const Значение& val, bool enable = true);
	void    DisableValue(const Значение& val)                      { EnableValue(val, false); }

	void  переустанов()                                               { cs.очисть(); }

	const Массив<Case>& GetCases() const                         { return cs; }

	operator int() const                                        { return дайДанные(); }
	void operator=(const Значение& v)                              { устДанные(v); }

	Switch& устШрифт(Шрифт f)                                     { font = f; освежи(); return *this; }
	Шрифт    дайШрифт() const                                     { return font; }
	Switch& MinCaseHeight(int cy)                               { mincy = cy; освежи(); return *this; }
	Switch& SetAutoDirection()                                  { direction = 0; return *this; }
	Switch& SetHorizontal()                                     { direction = 1; return *this; }
	Switch& SetVertical()                                       { direction = -1; return *this; }

	Switch();
	virtual ~Switch();
};

class DataPusher : public Толкач
{
public:
	virtual void   рисуй(Draw& draw);
	virtual Значение  дайДанные() const;
	virtual void   устДанные(const Значение& значение);

private:
	const Преобр *convert;
	const Дисплей *дисплей;
	Значение          data;

	ШТкст        nulltext;
	Цвет          nullink;
	Шрифт           nullfont;
	
	ActiveEdgeFrame edge;
	
	void  RefreshAll();
	Цвет GetPaper();
	
protected:
	virtual void   RefreshPush();
	virtual void   освежиФокус();
	virtual void   выполниАкцию();
	virtual void   DoAction();

public:
	Событие<>        WhenPreAction;

	DataPusher&    SetConvert(const Преобр& _convert) { convert = &_convert; освежи(); return *this; }
	const Преобр& GetConvert() const                  { return *convert; }

	DataPusher&    устДисплей(const Дисплей& _display) { дисплей = &_display; освежи(); return *this; }
	const Дисплей& дайДисплей() const                  { return *дисплей; }

	void           SetDataAction(const Значение& значение);

	void           уст(const Значение& значение);

	DataPusher&    NullText(const char *text = t_("(default)"), Цвет ink = коричневый);
	DataPusher&    NullText(const char *text, Шрифт fnt, Цвет ink);

	DataPusher();
	DataPusher(const Преобр& convert, const Дисплей& дисплей = StdDisplay()); // deprecated
	DataPusher(const Дисплей& дисплей); // deprecated
};
