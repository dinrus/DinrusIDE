class КтрлИнфо : public FrameLR<Ктрл> {
public:
	virtual void рисуй(Draw& w);
	virtual void выложиФрейм(Прям& r);

private:
	struct Вкладка {
		РисПрям             info;
		int                   width;

		Вкладка() { width = 0; }
	};

	Массив<Вкладка>   tab;
	РисПрям    temp;
	bool         right;
	Ткст       defaulttext;
	ОбрвызВремени temptime;

public:
	void уст(int tab, const РисПрям& info, int width);
	void уст(int tab, const Значение& info, int width);
	void уст(const РисПрям& info);
	void уст(const Значение& info);
	void Temporary(const РисПрям& info, int timeoout = 2000);
	void Temporary(const Значение& info, int timeout = 2000);
	void EndTemporary();

	int  дайСчётВкладок() const              { return tab.дайСчёт(); }
	int  дайСмещВкладки(int t) const;
	int  GetRealTabWidth(int tabi, int width) const;

	void operator=(const Ткст& s)       { уст(s); }

	КтрлИнфо& устДефолт(const Ткст& d) { defaulttext = d; освежи(); return *this; }
	КтрлИнфо& лево(int w);
	КтрлИнфо& право(int w);

	typedef КтрлИнфо ИМЯ_КЛАССА;

	КтрлИнфо();
};

class СтатусБар : public КтрлИнфо {
public:
	virtual void выложиФрейм(Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);
	virtual void рисуй(Draw& w);

public:
	struct Стиль : public ChStyle<Стиль> {
		Значение look;
	};

private:
	int      cy;
	SizeGrip grip;
	
	struct ТопФрейм : public КтрлФрейм {
		virtual void выложиФрейм(Прям& r);
		virtual void рисуйФрейм(Draw& draw, const Прям& r);
		virtual void добавьРазмФрейма(Размер& sz);
		
		const Стиль *style;
	};
	
	ТопФрейм frame;

	void    устТекст(const Ткст& s)          { уст(s); }

public:
	void operator=(const Ткст& s)           { уст(s); }

	operator Событие<const Ткст&>()       { return pteback(this, &СтатусБар::устТекст); }

	Событие<const Ткст&> operator~()      { return pteback(this, &СтатусБар::устТекст); }

	СтатусБар&  устВысоту(int _cy);
	СтатусБар&  NoSizeGrip()                  { Ктрл::удалиФрейм(grip); return *this; }

	static const Стиль& дефСтиль();

	КтрлИнфо& устСтиль(const Стиль& s)        { frame.style = &s; освежи(); return *this; }

	СтатусБар();
	~СтатусБар();
};


Дисплей& ProgressDisplay();

class ИнфОПрогрессе {
	КтрлИнфо *info;
	Ткст    text;
	int       tw;
	int       tabi;
	int       cx;
	int       total;
	int       pos;
	
	int       granularity;
	dword     set_time;

	void освежи();
	void переустанов();

public:
	ИнфОПрогрессе& устТекст(const Ткст& s)         { text = s; освежи(); return *this; }
	ИнфОПрогрессе& устШирТекста(int cx)             { tw = cx; освежи(); return *this; }
	ИнфОПрогрессе& устШирину(int _cx)                { cx = _cx; освежи(); return *this;  }
	ИнфОПрогрессе& Placement(int _tabi)          { tabi = _tabi; освежи(); return *this; }
	ИнфОПрогрессе& Info(КтрлИнфо& _info)         { info = &_info; info->EndTemporary(); освежи(); return *this; }
	ИнфОПрогрессе& Total(int _total)             { total = _total; освежи(); return *this; }

	ИнфОПрогрессе& уст(int _pos, int _total);

	void          уст(int _pos)                 { уст(_pos, total); }
	int           дай() const                   { return pos; }
	int           дайВсего() const              { return total; }

	void operator=(int p)                       { уст(p); }
	void operator++()                           { уст(pos + 1, total); }
	operator int()                              { return pos; }

	ИнфОПрогрессе()                              { переустанов(); }
	ИнфОПрогрессе(КтрлИнфо& f)                   { переустанов(); Info(f); }
	~ИнфОПрогрессе();
};
