class HeaderCtrl : public Ктрл, public КтрлФрейм {
public:
	virtual void  режимОтмены();
	virtual void  рисуй(Draw& draw);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual void  леваяВнизу(Точка p, dword keyflags);
	virtual void  леваяДКлик(Точка p, dword keyflags);
	virtual void  леваяТяг(Точка p, dword keyflags);
	virtual void  двигМыши(Точка p, dword keyflags);
	virtual void  выходМыши();
	virtual void  леваяВверху(Точка, dword);
	virtual void  праваяВнизу(Точка, dword);
	virtual void  сериализуй(Поток& s);
	virtual void  Выкладка();

	virtual void  добавьКФрейму(Ктрл& parent);
	virtual void  удалиФрейм();
	virtual void  выложиФрейм(Прям& r);
	virtual void  добавьРазмФрейма(Размер& sz);

public:
	struct Стиль : ChStyle<Стиль> {
		Значение look[4];
		int   gridadjustment;
		bool  pressoffset;
	};

	class Колонка : public НадписьОснова {
	protected:
		virtual void  обновиНадпись();

		HeaderCtrl *header;
		double      ratio;
		bool        visible;
		int         min, max;
		int         margin;
		Цвет       paper;
		int         Индекс;
		Ткст      tip;

		void        рисуй(bool& first, Draw& w,
		                  int x, int y, int cx, int cy, bool disabled, bool push, bool hl);

		friend class HeaderCtrl;

	public:
		Событие<>     ПриЛевКлике;
		Событие<>     WhenLeftDouble;
		Событие<>     WhenAction;
		Событие<Бар&> WhenBar;

		Колонка&  мин(int _min)                     { min = _min; return *this; }
		Колонка&  макс(int _max)                     { max = _max; return *this; }
		Колонка&  минмакс(int m, int n)              { return мин(m).макс(n); }
		Колонка&  фиксирован(int f)                      { return минмакс(f, f); }
		Колонка&  Подсказка(const char *s)                { tip = s; return *this; }
		Колонка&  SetPaper(Цвет c)                 { paper = c; return *this; }
		Колонка&  SetRatio(double ratio);
		Колонка&  SetMargin(int m);

		void     покажи(bool b = true);
		void     скрой()                            { покажи(false); }

		int      GetMargin() const                 { return margin + 2; }
		Цвет    GetPaper() const                  { return paper; }
		int      дайИндекс() const                  { return Индекс; }
		bool     видим_ли() const                 { return visible; }
		double   GetRatio() const                  { return ratio; }

		Колонка();
	};

	friend class Колонка;

	enum { PROPORTIONAL, REDUCELAST, REDUCENEXT, SCROLL, FIXED };

protected:

	Массив<Колонка> col;
	ГПромотБар    sb;

	mutable int          oszcx;
	mutable Вектор<Прям> tabrect;

	int   split, pushi, li, ti;
	bool  isdrag;
	Рисунок dragtab;
	int   dragd, dragx;
	Прям  colRect;
	bool  push:1;
	bool  track:1;
	bool  invisible:1;
	bool  moving:1;
	byte  mode;
	int   light;
	int   height;
	const Стиль *style;
	bool  autohidesb;

	double Denominator() const;
	void   Reduce(int q, double& delta, double rs, int szcx, bool checkmin);
	int    GetNextTabWidth(int i);
	int    GetLastVisibleTab();
	void   WScroll();
	void   промотай();
	void   ScrollVisibility();
	void   DoSbTotal();
	void   SbTotal();
	void   SetTabWidth0(int i, int cx);
	int    суммаMin(int from);
	int    суммаMax(int from);
	void   Distribute(const Вектор<int>& sci, double delta);
	void   ReCompute();
	void   InvalidateDistribution()                              { oszcx = -1; }
	void   RefreshDistribution();
	Вектор<int> GetVisibleCi(int from);

public:
	Событие<>       WhenLayout;
	Событие<>       ПриПромоте;
	Событие<>       WhenScrollVisibility;

	Прям          GetTabRect(int i);

	const Колонка& Вкладка(int i) const                        { return col[i]; }
	Колонка&       Вкладка(int i);

	Колонка&       добавь(const char *text, double ratio = 0);
	Колонка&       добавь();
	const Колонка& operator[](int i) const                 { return col[i]; }
	int           дайСчёт() const                        { return col.дайСчёт(); }
	void          переустанов();

	void          ShowTab(int i, bool show = true);
	void          HideTab(int i)                          { ShowTab(i, false); }
	bool          IsTabVisible(int i)                     { return col[i].visible; }
	void          SetTabRatio(int i, double ratio);
	double        GetTabRatio(int i) const                { return InvZxf(col[i].ratio); }
	void          SetTabWidth(int i, int cx);
	int           GetTabWidth(int i);
	
	void          SwapTabs(int first, int second);
	void          MoveTab(int from, int to);
	int           GetTabIndex(int i) const                { return col[i].Индекс; }
	int           найдиИндекс(int ndx) const;

	void          StartSplitDrag(int s);
	int           GetSplit(int x);

	int           дайПромотку() const                       { return sb; }
	bool          IsScroll() const                        { return sb.показан_ли(); }

	void          SetHeight(int cy);
	int           дайВысоту() const;
	
	int           дайРежим() const                         { return mode; }
	
	static const Стиль& дефСтиль();

	HeaderCtrl&   Invisible(bool inv);
	HeaderCtrl&   Track(bool _track = true)               { track = _track; sb.Track(track); return *this; }
	HeaderCtrl&   NoTrack()                               { return Track(false); }
	HeaderCtrl&   Proportional();
	HeaderCtrl&   ReduceNext();
	HeaderCtrl&   ReduceLast();
	HeaderCtrl&   Absolute();
	HeaderCtrl&   фиксирован();
	HeaderCtrl&   устСтиль(const Стиль& s)                { style = &s; освежи(); return *this; }
	HeaderCtrl&   Moving(bool b = true)                   { moving = b; return *this; }
	HeaderCtrl&   AutoHideSb(bool b = true)               { autohidesb = b; SbTotal(); return *this; }
	HeaderCtrl&   NoAutoHideSb()                          { return AutoHideSb(false); }
	HeaderCtrl&   HideSb(bool b = true)                   { sb.покажи(!b); return *this; }

	HeaderCtrl&   устСтильПромотБара(const ПромотБар::Стиль& s)   { sb.устСтиль(s); return *this; }

	static int GetStdHeight()                             { return Draw::GetStdFontCy() + 4; }

	typedef HeaderCtrl ИМЯ_КЛАССА;

	HeaderCtrl();
	virtual ~HeaderCtrl();
};
