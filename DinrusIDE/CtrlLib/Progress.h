class ProgressIndicator : public Ктрл {
public:
	virtual void   рисуй(Draw& draw);
	virtual void   Выкладка();

public:
	struct Стиль : ChStyle<Стиль> {
		bool  classic;
		bool  bound;
		bool  nomargins;
		Значение vlook, vchunk, hlook, hchunk;
	};

protected:
	int   total, actual;
	int   pxp;
	bool  percent:1;
	const Стиль *style;
	Цвет color;

	Прям GetMargins();
	Размер GetMsz();

public:
	void  уст(int actual, int total);
	void  уст(int _actual)                      { уст(_actual, total); }

	void  operator=(int i)                      { уст(i); }
	int   operator++()                          { уст(actual + 1); return actual; }
	int   operator++(int)                       { int i = actual; уст(actual + 1); return i; }
	int   operator+=(int i)                     { уст(actual + i); return actual; }

	int   дай() const                           { return actual; }
	int   дайВсего() const                      { return total; }

	operator int()                              { return actual; }

	static const Стиль& дефСтиль();

	ProgressIndicator& устВсего(int _total)     { уст(actual, _total); return *this; }
	ProgressIndicator& Percent(bool b = true)   { percent = b; освежи(); return *this; }
	ProgressIndicator& NoPercent()              { return Percent(false); }
	ProgressIndicator& устЦвет(Цвет c)        { color = c; освежи(); return *this; }
	ProgressIndicator& устСтиль(const Стиль& s) { style = &s; освежи(); return *this; }

	ProgressIndicator();
	virtual ~ProgressIndicator();
};

class Progress : public WithProgressLayout<ТопОкно> {
public:
	ProgressIndicator pi;
	virtual void закрой();
	virtual void Выкладка();

protected:
	bool     cancel;
	int      total;
	int      pos;
	Ткст   text;
	int      granularity;
	int      show_delay;
	dword    set_time;
	dword    show_time;
	Ктрл    *owner;
	Modality modality;
	Стопор    mtx;

	void     Setxt0();
	void     Setxt();
	void     иниц();
	void     Process();

public:
	void     SetOwner(Ктрл *_owner)        { owner = _owner; }

	void     создай();
	void     Cancel();

	void     уст(int pos, int total);
	void     устПоз(int pos);
	void     устТекст(const char *s)        { устТекст((Ткст)s); }
	void     устТекст(const Ткст& s);
	void     устВсего(int total);
	void     Step(int steps = 1);
	int      дайПоз() const                { return pos; }
	int      дайВсего() const              { return total; }
	Ткст   дайТекст() const               { return text; }

	bool     Canceled();
	bool     SetCanceled(int pos, int total);
	bool     SetPosCanceled(int pos);
	bool     StepCanceled(int steps = 1);

	void     переустанов();

	void     Granularity(int ms)           { granularity = ms; }
	void     Delay(int ms)                 { show_delay = ms; }

	void     AlignText(int align)          { info.устЛин(align); }

	operator Врата<int, int>()             { return callback(this, &Progress::SetCanceled); }

	Progress();
	Progress(Ктрл *_owner);
	Progress(const char *txt, int total = 0);
	Progress(Ктрл *_owner, const char *txt, int total = 0);
	virtual ~Progress();
};
