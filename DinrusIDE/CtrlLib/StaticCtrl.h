class СтатичТекст : public Ктрл, public НадписьОснова {
public:
	virtual void   рисуй(Draw& w);
	virtual Размер   дайМинРазм() const;
	virtual void   обновиНадпись();

public:
	СтатичТекст& устШрифт(Шрифт font)                      { НадписьОснова::устШрифт(font); return *this; }
	СтатичТекст& устЧернила(Цвет color)                     { НадписьОснова::устЧернила(color); return *this; }
	СтатичТекст& устЛин(int align)                     { НадписьОснова::устЛин(align); return *this; }
	СтатичТекст& устРисунок(const Рисунок& img, int spc = 0) { НадписьОснова::устРисунок(img, spc); return *this; }
	СтатичТекст& устТекст(const char *text)               { НадписьОснова::устТекст(text); return *this; }

	СтатичТекст& operator=(const char *s)                { устТекст(s); return *this; }

	СтатичТекст();
};

class Надпись : public СтатичТекст {
public:
	virtual bool   горячаяКлав(dword ключ);
	virtual dword  дайКлючиДоступа() const;
	virtual void   присвойКлючиДоступа(dword used);

private:
	bool   noac;

public:
	Надпись& устТекст(const char *text);
	Надпись& устНадпись(const char *lbl);

	Надпись& operator=(const char *s)                     { устТекст(s); return *this; }

	Надпись();
	virtual ~Надпись();
};

class БоксНадпись : public Надпись {
public:
	virtual void   рисуй(Draw& w);
	virtual void   присвойКлючиДоступа(dword used);
	virtual Прям   дайПроцПрям() const;


	Цвет color;

	static Значение LabelBoxLook;
	static void  SetLook(const Значение& v) { LabelBoxLook = v; }
	static Значение GetLook()               { return LabelBoxLook; }

public:

	БоксНадпись&    устЦвет(Цвет c)       { color = c; return *this; }

	БоксНадпись();
	virtual ~БоксНадпись();
	БоксНадпись& operator=(const char *s)   { устТекст(s); return *this; }
};

void рисуйБоксНадпись(Draw& w, Размер sz, Цвет color, int d);

Цвет цветТекстаБоксНадписи();
Цвет цветБоксНадписи();

void LabelBoxTextColor_Write(Цвет c);
void LabelBoxColor_Write(Цвет c);

class КтрлРодитель : public Ктрл {
	Размер minsize;

public:
	virtual Прям   дайПроцПрям() const;
	virtual Размер   дайСтдРазм() const;
	virtual Размер   дайМинРазм() const;
	virtual void   устМинРазм(Размер sz)         { minsize = sz; }

	КтрлРодитель();
};

class СтатичПрям : public Ктрл {
public:
	virtual void   рисуй(Draw& w);

protected:
	Значение bg;

public:
	СтатичПрям& фон(const Значение& chvalue);
	СтатичПрям& Цвет(class Цвет c)                   { фон(c); return *this; }

	Значение дайФон() const                        { return bg; }

	СтатичПрям();
	virtual ~СтатичПрям();
};

class КтрлРисунок : public Ктрл {
public:
	virtual void   рисуй(Draw& w);
	virtual Размер   дайСтдРазм() const;
	virtual Размер   дайМинРазм() const;

protected:
	Рисунок   img;

public:
	КтрлРисунок&   устРисунок(const Рисунок& _img)          { img = _img; освежи(); return *this; }

	КтрлРисунок()                                       { Transparent(); NoWantFocus(); }
};

class КтрлДисплей : public Ктрл {
public:
	virtual void   рисуй(Draw& w);
	virtual Размер   дайМинРазм() const;
	virtual void   устДанные(const Значение& v);
	virtual Значение  дайДанные() const;

private:
	РисПрям pr;

public:
	void устДисплей(const Дисплей& d);
};

class КтрлЧертёж : public Ктрл {
public:
	virtual void   рисуй(Draw& w);

protected:
	Чертёж picture;
	Цвет   background;
	bool    ratio;

public:
	Чертёж  дай() const                             { return picture; }

	КтрлЧертёж& фон(Цвет color);
	КтрлЧертёж& KeepRatio(bool keep = true)         { ratio = keep; освежи(); return *this; }
	КтрлЧертёж& NoKeepRatio()                       { return KeepRatio(false); }
	КтрлЧертёж& уст(const Чертёж& _picture)        { picture = _picture; освежи(); return *this; }

	КтрлЧертёж& operator=(const Чертёж& _picture)  { return уст(_picture); }
	КтрлЧертёж& operator=(const Painting& _picture) { return уст(AsDrawing(_picture)); }

	КтрлЧертёж();
};

// BWC
typedef КтрлРисунок Иконка;
typedef КтрлЧертёж Picture;

class КтрлСепаратор : public Ктрл {
public:
	virtual Размер дайМинРазм() const;
	virtual void рисуй(Draw& w);

	struct Стиль : ChStyle<Стиль> {
		Значение l1, l2;
	};

private:
	int          lmargin, rmargin;
	int          size;
	const Стиль *style;

public:
	static const Стиль& дефСтиль();

	КтрлСепаратор& Margin(int l, int r);
	КтрлСепаратор& Margin(int w)                { return Margin(w, w); }
	КтрлСепаратор& устРазм(int w);
	КтрлСепаратор& устСтиль(const Стиль& s);

	КтрлСепаратор();
};
