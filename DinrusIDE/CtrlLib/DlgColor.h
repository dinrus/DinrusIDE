#ifndef __CtrlLib_DlgColor__
#define __CtrlLib_DlgColor__

class WheelRampCtrl : public Ктрл
{
public:
	WheelRampCtrl(bool ramp);
	~WheelRampCtrl();

	virtual void  Выкладка();
	virtual void  рисуй(Draw& draw);
	virtual void  устДанные(const Значение& значение);
	virtual Значение дайДанные() const                { return color; }

	virtual void  леваяВнизу(Точка pt, dword keyflags);
	virtual void  леваяВверху(Точка pt, dword keyflags);
	virtual void  леваяДКлик(Точка pt, dword keyflags);
	virtual void  двигМыши(Точка pt, dword keyflags);

	Событие<>       WhenLeftDouble;

private:
	void          устЦвет(Цвет color, bool set_norm, bool set_hsv);

	Рисунок         PaintRamp(Размер size);
	Рисунок         PaintWheel(Размер size);
	void          PaintColumn(Draw& draw);

	int           ClientToLevel(int y) const;
	int           LevelToClient(int l) const;

private:
	bool          ramp;
	Цвет         color;
	Цвет         normalized_color;
	int           h16, s16, v16;
	int           round_step;
	enum STYLE { S_WHEEL, S_RECT, S_HEXAGON };
	STYLE         style;
	Рисунок         cache;
	int           cache_level;
	Рисунок         wheel_cache;
	Прям          wheel_rect;
	Прям          column_rect;

	int           firstclick;

	struct WheelBuff
	{
		int arg;
		int l;
	};
};

struct ColorWheelCtrl : public WheelRampCtrl {
	ColorWheelCtrl() : WheelRampCtrl(false) {}
};

struct ColorRampCtrl : public WheelRampCtrl {
	ColorRampCtrl() : WheelRampCtrl(true) {}
};

class ColorSelector : public Ктрл
{
public:
	ColorSelector(bool not_null = true);

	ColorSelector& неПусто(bool nn = true)                  { impl->неПусто(nn); return *this; }
	ColorSelector& NoNotNull()                              { return неПусто(false); }
	bool           неПусто_ли() const                        { return impl->неПусто_ли(); }

	Цвет          дай() const                              { return impl->дай(); }
	void           уст(Цвет c)                             { impl->уст(c); }

	Вектор<Цвет>  GetPalette() const                       { return impl->GetPalette(); }
	void           SetPalette(const Вектор<Цвет>& palette) { impl->SetPalette(palette); }

	void           SerializeConfig(Поток& stream)          { impl->SerializeConfig(stream); }

	virtual void   устДанные(const Значение& color)              { уст(color); }
	virtual Значение  дайДанные() const                          { return дай(); }

	Ктрл&          GetImplCtrl()                            { return impl->дайКтрл(); }

public:
	Событие<>        WhenSetColor;

public:
	class Impl
	{
	public:
		virtual ~Impl() {}

		virtual void          уст(Цвет c) = 0;
		virtual Цвет         дай() const = 0;
		virtual void          неПусто(bool nn) = 0;
		virtual bool          неПусто_ли() const = 0;
		virtual Вектор<Цвет> GetPalette() const = 0;
		virtual void          SetPalette(const Вектор<Цвет>& pal) = 0;
		virtual void          SerializeConfig(Поток& stream) = 0;
		virtual Ктрл&         дайКтрл() = 0;

	protected:
		Impl() {}
	};

protected:
	Один<Impl>      impl;
};

class ColorCtrl : public DataPusher
{
public:
	typedef ColorCtrl ИМЯ_КЛАССА;
	ColorCtrl(bool not_null = true);
	virtual ~ColorCtrl();

	ColorCtrl&     неПусто(bool _nn = true) { empty.покажи(!_nn); return *this; }
	ColorCtrl&     NoNotNull()              { return неПусто(false); }
	bool           неПусто_ли() const        { return !empty.видим_ли(); }

protected:
	virtual void   DoAction();
	void           OnClear()                { SetDataAction(Null); }

protected:
	ФреймПраво<Кнопка> empty;
};

Цвет RunDlgSelectColor(Цвет init_color = чёрный, bool not_null = true, const char *title = 0, bool *ok = 0);
const Дисплей& StdColorDisplayNull();

class ColorPopUp : public Ктрл {
public:
	virtual  void рисуй(Draw& w);
	virtual  void леваяВверху(Точка p, dword);
	virtual  void леваяВнизу(Точка p, dword);
	virtual  void двигМыши(Точка p, dword);
	virtual  void выходМыши();
	virtual  bool Ключ(dword ключ, int count);
	virtual  void Выкладка();

private:
	void PopupDeactivate();

	struct Popup : Ктрл {
		ColorPopUp *color;
		
		virtual void откл() { color->PopupDeactivate(); }
	};

	int      дай(Точка p);
	int      GetCy();
	void     настрой(Цвет c);
	void     финиш();
	void     Ramp();
	void     Wheel();
	int      GetColorCount() const;
	Цвет    дайЦвет(int i) const;
	void     выдели();

	void     DrawFilledFrame(Draw &w, int x, int y, int cx, int cy, Цвет fcol, Цвет bcol);
	void     DrawFilledFrame(Draw &w, Прям &r, Цвет fcol, Цвет bcol);

	int      colori;
	bool     notnull;
	bool     scolors;
	bool     norampwheel;
	bool     animating;
	bool     hints;
	bool     open;
	bool     withvoid;
	Ткст   nulltext;
	Ткст   voidtext;
	Цвет    color;

	ColorRampCtrl  ramp;
	ColorWheelCtrl wheel;
	Кнопка         settext;
	Один<Popup>     popup;

	static Цвет   hint[18];
	
	friend void ColorPopUp_InitHint();

public:
	Событие<>  WhenCancel;
	Событие<>  WhenSelect;

	static void Hint(Цвет c);

	typedef ColorPopUp ИМЯ_КЛАССА;

	void     PopUp(Ктрл *owner, Цвет c = белый);
	Цвет    дай() const;
	
	ColorPopUp& неПусто(bool b = true)               { notnull = b; return *this; }
	ColorPopUp& SColors(bool b = true)               { scolors = b; return *this; }//Deprecated, BUT NEEDED IN THEIDE
	ColorPopUp& NullText(const char *s)              { nulltext = s; освежи(); return *this; }
	ColorPopUp& WithVoid(bool b = true)              { withvoid = b; освежи(); return *this; }
	ColorPopUp& VoidText(const char *s)              { voidtext = s; освежи(); return *this; }
	ColorPopUp& NoRampWheel(bool b = true)           { norampwheel = b; return *this; }
	ColorPopUp& Hints(bool b = true)                 { hints = b; return *this; }

	ColorPopUp();
	virtual ~ColorPopUp();
};

class ColorPusher : public Ктрл {
public:
	virtual void  рисуй(Draw& w);
	virtual void  леваяВнизу(Точка p, dword);
	virtual bool  Ключ(dword ключ, int);
	virtual void  сфокусирован()                  { освежи(); }
	virtual void  расфокусирован()                 { освежи(); }
	virtual void  устДанные(const Значение& v);
	virtual Значение дайДанные() const;

protected:
	bool       push;
	bool       withtext;
	bool       withhex;
	bool       track;
	Цвет      color, saved_color;
	ColorPopUp colors;
	Ткст     nulltext;
	Ткст     voidtext;

	void AcceptColors();
	void CloseColors();
	void NewColor();
	void сбрось();

public:
	typedef ColorPusher ИМЯ_КЛАССА;

	ColorPusher& NullText(const char *s)    { nulltext = s; colors.NullText(s); освежи(); return *this; }
	ColorPusher& неПусто(bool b = true)     { colors.неПусто(b); return *this; }
	ColorPusher& WithVoid(bool b = true)    { colors.WithVoid(b); return *this; }
	ColorPusher& VoidText(const char *s)    { voidtext = s; colors.VoidText(s); освежи(); return *this; }
	ColorPusher& SColors(bool b = true)     { colors.SColors(b); return *this; }
	ColorPusher& WithText(bool b = true)    { withtext = b; освежи(); return *this; }
	ColorPusher& WithHex(bool b = true)     { withhex = b; освежи(); return *this; }
	ColorPusher& Track(bool b = true)       { track = b; return *this; }
	ColorPusher& NoTrack()                  { return Track(false); }
	ColorPusher& NoRampWheel(bool b = true) { colors.NoRampWheel(b); return *this; }

	ColorPusher();
	virtual ~ColorPusher();
};

class ColorButton : public ColorPusher {
public:
	virtual void  рисуй(Draw& w);
	virtual void  входМыши(Точка p, dword keyflags);
	virtual void  выходМыши();
	virtual Размер  дайМинРазм() const;

protected:
	Рисунок      image, nullimage, staticimage;
	const ToolButton::Стиль *style;

public:
	ColorButton& ColorImage(const Рисунок& img)             { image = img; освежи(); return *this; }
	ColorButton& NullImage(const Рисунок& img)              { nullimage = img; освежи(); return *this; }
	ColorButton& StaticImage(const Рисунок& img)            { staticimage = img; освежи(); return *this; }
	ColorButton& устСтиль(const ToolButton::Стиль& s)     { style = &s; освежи(); return *this; }

	ColorButton();
	virtual ~ColorButton();
};

Ткст FormatColor(Цвет c);
Цвет  ReadColor(СиПарсер& p);
Цвет  RealizeColor(Цвет c);

#endif//__TCtrlLib_DlgColor__
