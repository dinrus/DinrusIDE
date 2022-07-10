class Сплиттер : public Ктрл {
public:
	virtual void   Выкладка();
	virtual void   рисуй(Draw& draw);
	virtual void   двигМыши(Точка p, dword keyflags);
	virtual void   леваяВнизу(Точка p, dword keyflags);
	virtual void   леваяВверху(Точка p, dword keyflags);
	virtual Рисунок  рисКурсора(Точка p, dword keyflags);
	virtual void   сериализуй(Поток& s);

public:
	struct Стиль : ChStyle<Стиль> {
		Значение  vert[2], horz[2];
		int    width;
		bool   dots;
	};

protected: // Because of docking... (will be private)
	Вектор<int> pos;
	Вектор<int> mins;
	Вектор<int> minpx;
	int         style;
	int         mouseindex;
	bool        vert;
	int         inset;
	const Стиль *chstyle;

	int         найдиИндекс(Точка client) const;
	int         GetMins(int i) const;
	int         GetBarWidth() const;
	void        SyncMin();

	static void PaintDots(Draw& w, const Прям& r, bool vert);
	
	friend class SplitterFrame;

public:
	Событие<>   WhenSplitFinish;

	void      уст(Ктрл& l, Ктрл& r);

	Сплиттер& устПоз(int newpos, int Индекс = 0);
	int       дайПоз(int Индекс = 0) const          { return Индекс < pos.дайСчёт() ? pos[Индекс] : 10000; }

	int       дайСчёт() const                     { return GetChildCount(); }

	int       PosToClient(int pos) const;
	int       ClientToPos(Точка client) const;

	void      Zoom(int i);
	void      NoZoom()                             { Zoom(-1); }
	int       GetZoom() const                      { return style; }

	void      SetMin(int i, int w)                 { mins.по(i, 0) = w; SyncMin(); }
	void      SetMinPixels(int i, int w)           { minpx.по(i, 0) = w; SyncMin(); }
	
	int       GetSplitWidth() const;

	void      добавь(Ктрл& pane);
	Сплиттер& operator<<(Ктрл& pane)               { добавь(pane); return *this; }
	void      вставь(int pos, Ктрл& pane);
	void      удали(Ктрл& pane);
	void      разверни(Ктрл& pane, Ктрл& newpane);

	static const Стиль& дефСтиль();

	Сплиттер& верт(Ктрл& top, Ктрл& bottom);
	Сплиттер& гориз(Ктрл& left, Ктрл& right);
	Сплиттер& верт()                               { vert = true; Выкладка(); return *this; }
	Сплиттер& гориз()                               { vert = false; Выкладка(); return *this; }
	bool      горизонтален() const                       { return !vert; }
	bool      вертикален() const                       { return vert; }
	Сплиттер& устСтиль(const Стиль& s);

	void      очисть();
	void      переустанов();

	Сплиттер();
	virtual ~Сплиттер();
};

class SplitterFrame : public КтрлФрейм, private Ктрл {
public:
	virtual void добавьКФрейму(Ктрл& parent);
	virtual void удалиФрейм();
	virtual void добавьРазмФрейма(Размер& sz);
	virtual void выложиФрейм(Прям& r);

	virtual void рисуй(Draw& draw);
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void двигМыши(Точка p, dword keyflags);
	virtual void леваяВверху(Точка p, dword keyflags);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);

private:
	Точка ref;
	Размер  parentsize;
	int   тип, minsize, sizemin;
	int   size, size0;
	const Сплиттер::Стиль *style;

	int   BoundSize();

public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	void сериализуй(Поток& s);

	SplitterFrame& уст(Ктрл& c, int size, int тип);
	SplitterFrame& лево(Ктрл& c, int size)    { return уст(c, size, LEFT); }
	SplitterFrame& верх(Ктрл& c, int size)     { return уст(c, size, TOP); }
	SplitterFrame& право(Ктрл& c, int size)   { return уст(c, size, RIGHT); }
	SplitterFrame& низ(Ктрл& c, int size)  { return уст(c, size, BOTTOM); }

	SplitterFrame& минРазм(int sz)            { minsize = sz; return *this; }
	SplitterFrame& SizeMin(int sz)            { sizemin = sz; return *this; }
	SplitterFrame& устСтиль(const Сплиттер::Стиль& s);

	int  дайТип() const                      { return тип; }
	int  дайРазм() const                      { return size; }
	void устРазм(int sz)                      { size = sz; освежиВыкладкуРодителя(); }

	void покажи(bool show = true)				  { Ктрл::покажи(show); }
	void скрой()								  { Ктрл::скрой(); }
	bool показан_ли()							  { return Ктрл::показан_ли(); }

	SplitterFrame();
};
