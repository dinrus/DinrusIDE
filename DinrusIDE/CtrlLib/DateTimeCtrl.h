enum
{
	SUNDAY    = 0,
	MONDAY    = 1,
	TUESDAY   = 2,
	WEDNESDAY = 3,
	THURSDAY  = 4,
	FRIDAY    = 5,
	SATURDAY  = 6
};

class FlatButton : public Толкач {
public:
	Рисунок img;
	Цвет fg, bg, hl;
	bool left;
	bool drawedge;
	bool highlight;

	FlatButton();

	void DrawFrame(Draw &w, const Прям &r, Цвет lc, Цвет tc, Цвет rc, Цвет bc);
	virtual void рисуй(Draw &w);
	virtual void входМыши(Точка p, dword kflags) {
		освежи();
		Толкач::входМыши(p, kflags);
	}
	virtual void выходМыши() {
		освежи();
		Толкач::выходМыши();
	}

	const Рисунок& GetImage() { return img; }

	FlatButton& устРисунок(const Рисунок &_img) {
		img = _img;
		освежи();
		return *this;
	}

	FlatButton& устЛев()           { left = true;  return *this; }
	FlatButton& устПрав()          { left = false;	return *this; }
	FlatButton& DrawEdge(bool b)    { drawedge = b;	return *this; }
	FlatButton& Highlight(bool b)   { highlight = b;return *this; }
};

class FlatSpin : public Ктрл
{
private:
	FlatButton left;
	FlatButton right;
	Ткст text;
	Размер tsz;
	Шрифт font;
	bool selected;
	bool selectable;

public:
	FlatSpin();
	void устТекст(const Ткст& s);
	void SetTips(const char *tipl, const char *tipr);
	void SetCallbacks(const Событие<>& cbl, const Событие<>& cbr);

	FlatSpin& Selectable(bool b = true);
	int дайШирину(const Ткст& s, bool with_buttons = true);
	int дайВысоту();
	void устШрифт(const Шрифт& fnt);
	void SetLeftImage(const Рисунок &img)		{ left.устРисунок(img);                    }
	void SetRightImage(const Рисунок &img)	{ right.устРисунок(img);                   }
	void SetHighlight(bool b)               { left.Highlight(b); right.Highlight(b); }

	virtual void двигМыши(Точка p, dword keyflags);
	virtual void выходМыши();
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void Выкладка();
	virtual void рисуй(Draw& w);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);

	typedef FlatSpin ИМЯ_КЛАССА;
};

class PopUpCtrl : public Ктрл
{
protected:
	Рисунок nbg;
	bool popup;
public:
	PopUpCtrl() : popup(false) {}

	Событие<>  WhenPopDown;
	Событие<>  WhenDeactivate;
	virtual void откл();
	virtual Размер вычислиРазм() = 0;
	virtual void переустанов() {}

	void PopUp(Ктрл *owner, const Прям& rt);
	Размер GetPopUpSize() { return вычислиРазм(); }

	bool IsPopUp() const;
	void SetPopUp(bool b = true);
};

class Calendar : public PopUpCtrl
{
public:
	struct Стиль : ChStyle<Стиль> {
		Цвет header;

		Цвет bgmain;
		Цвет bgtoday;
		Цвет bgselect;

		Цвет fgmain;
		Цвет fgtoday;
		Цвет fgselect;
		Цвет outofmonth;
		Цвет curdate;
		Цвет today;
		Цвет selecttoday;
		Цвет cursorday;
		Цвет selectday;
		Цвет line;
		Цвет dayname;
		Цвет week;
		Шрифт  font;
		Рисунок spinleftimg;
		Рисунок spinrightimg;
		bool  spinhighlight;
	};

protected:
	const Стиль *style;
	const Стиль *St() const;

private:
	typedef Calendar ИМЯ_КЛАССА;

	FlatSpin spin_year;
	FlatSpin spin_month;
	FlatSpin spin_all;

	static const int cols = 7;
	static const int rows = 6;

	static const Точка nullday;
	int bs; //border size
	int hs; //header vertical size
	int ts; //today vertical size
	float colw;
	float rowh;
	int cw, rh;

	int col;
	int row;
	int lastrow;

	int fh;

	int days[rows][cols];

	Точка newday;
	Точка oldday;
	Точка prevday;
	Точка curday, firstday;
	int   newweek, oldweek;

	Ткст stoday;
	Размер sztoday;
	Ткст curdate;

	bool selall;
	bool istoday;
	bool wastoday;
	bool time_mode;
	bool swap_month_year;
	bool one_button;

	Время view;
	Время today;
	Время sel;

	int first_day;

	void OnMonthLeft();
	void OnMonthRight();
	void OnYearLeft();
	void OnYearRight();

	void UpdateFields();

	bool MouseOnToday(Точка p);
	bool MouseOnHeader(Точка p);

	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void двигМыши(Точка p, dword keyflags);
	virtual void рисуй(Draw &w);
	virtual bool Ключ(dword ключ, int count);
	virtual void выходМыши();
	virtual void State(int reason);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);

	int& Day(int x, int y) { return days[y][x]; }
	int& Day(Точка p)      { return Day(p.x, p.y); }
	Точка GetDay(Точка p);
	int GetWeek(Точка p);

	virtual Размер вычислиРазм();

public:
	Calendar();
	Событие<Время &> WhenTime;
	Событие<Дата>   WhenWeek;

	static const Стиль& дефСтиль();

	void переустанов();

	int	 деньНедели(int day, int month, int year, int zelleroffset = 2);
	int  WeekOfYear(int day, int month, int year);

	virtual Значение дайДанные() const			{ return time_mode ? (Время) sel : (Дата) sel; }
	virtual void  устДанные(const Значение& v)   { time_mode ? SetTime((Время) v) : SetDate((Дата) v); }

	Дата GetDate() const;
	void SetDate(int y, int m, int d);
	void SetDate(const Дата &dt);

	Дата дайВремя() const;
	void SetTime(int y, int m, int d, int h, int n, int s);
	void SetTime(const Время &tm);

	Дата дайКурсор() const			   { return view; 				  }
	bool естьКурсор() const			   { return view.day != 0;        }

	Дата GetView() const			   { return Дата(view.year, view.month, view.day ? view.day : 1); }
	void SetView(const Время &v);

	Calendar& устСтиль(const Стиль& s);
	Calendar& выбериВсе(bool b = true)     { selall = b; return *this;       }
	Calendar& NoSelectAll()                { selall = false; return *this;   }
	Calendar& FirstDay(int n = MONDAY)     { first_day = n; return *this;    }
	Calendar& TimeMode(bool b = true)      { time_mode = b; return *this;    }
	Calendar& SwapMonthYear(bool b = true) { swap_month_year = b; return *this; }
	Calendar& OneButton(bool b = true)     { one_button = b; return *this;   }
	Calendar& NoOneButton()                { one_button = false; return *this;   }

	void PopUp(Ктрл *owner, Прям &rt);

	Событие<>  WhenSelect;
};

struct LineCtrl : Ктрл
{
	int pos, real_pos;

	virtual void рисуй(Draw& w);
	void устПоз(Точка p);
	int дайПоз();
	void устПоз(int p);

	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void двигМыши(Точка p, dword keyflags);
	virtual void леваяВверху(Точка p, dword keyflags);
	LineCtrl();
};

class Clock : public PopUpCtrl
{
public:
	struct Стиль : ChStyle<Стиль> {
		Цвет header;
		Цвет bgmain;
		Цвет fgmain;

		Цвет arrowhl;
		Цвет arrowhour;
		Цвет arrowminute;
		Цвет arrowsecond;

		Шрифт font;
	};

protected:
	const Стиль *style;
	const Стиль *St() const;

private:
	FlatSpin spin_hour;
	FlatSpin spin_minute;
	EditIntSpin ed_hour, ed_minute;
	LineCtrl ln_hour, ln_minute;

	//int hour, minute, second;
	Время sel;
	int hs;
	int dir;
	bool accept_time;

	struct Строка {
		ТочкаПЗ s, e;
	};

	struct минмакс {
		int diff;
		int значение;
	};

	Строка lines[3];
	Размер sz;
	ТочкаПЗ cm; //circle middle
	ТочкаПЗ cf; //circle factor

	int64 cur_time;
	int   cur_line;
	int   prv_line;
	int   cur_hour;
	int   cur_minute;
	int   cur_second;
	int   cur_point;
	int   prv_point;

	bool seconds;
	bool colon;

	void PaintPtr(int n, Draw& w, ТочкаПЗ p, double pos, double m, double rd, int d, Цвет color, Точка cf);
	void PaintCenteredText(Draw& w, int x, int y, const char *text, const Шрифт& fnt, Цвет c);
	void PaintCenteredImage(Draw &w, int x, int y, const Рисунок& img);

	void SetHourEdit();
	void SetMinuteEdit();
	void SetHourLine();
	void SetMinuteLine();
	void SetHourLeft();
	void SetHourRight();
	void SetMinuteLeft();
	void SetMinuteRight();

	минмакс SetMinMax(int v, int min, int max);
	void UpdateFields();
	void UpdateTime();
	int  IncFactor(int dir, int pp, int cp);

	int  GetDir(int prev_point, int cur_point);
	int  GetPointedLine(Точка p);
	int  GetPoint(ТочкаПЗ p, double tolerance = 4.0);
	bool IsCircle(ТочкаПЗ p, ТочкаПЗ s, double r);
	bool IsLine(ТочкаПЗ s, ТочкаПЗ e, ТочкаПЗ p, double tolerance = 3.0);
	void CalcSizes();

	void Timer();

public:
	virtual bool Ключ(dword ключ, int count);
	virtual void рисуй(Draw& w);
	virtual void State(int reason);
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void леваяВверху(Точка p, dword keyflags);
	virtual void двигМыши(Точка p, dword keyflags);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual Размер вычислиРазм();

	virtual Значение дайДанные() const;
	virtual void  устДанные(const Значение& v);

	Время дайВремя() const;
	void SetTime(int h, int n, int s);
	void SetTime(const Время& tm);

	static const Стиль& дефСтиль();

	void переустанов();

	int GetHour() const   { return sel.hour;   }
	int GetMinute() const { return sel.minute; }
	int GetSecond() const { return sel.second; }

	Clock& устСтиль(const Стиль& s);
	Clock& секунды(bool b = true)    { seconds = b; return *this; }
	Clock& безСекунд()               { return секунды(false); }
	Clock& Colon(bool b = true)      { colon = b; return *this; }
	Clock& NoColon()                 { return Colon(false); }

	Clock();

	typedef Clock ИМЯ_КЛАССА;
};

class CalendarClock : public Ктрл
{
private:
	int mode;

	Размер calendar_size;
	Размер clock_size;
	Размер sz;

public:
	enum {
		MODE_DATE = 0,
		MODE_TIME = 1
	};

	typedef CalendarClock ИМЯ_КЛАССА;

	Calendar calendar;
	Clock clock;

	CalendarClock(int m = MODE_TIME);
	Событие<>  WhenPopDown;

	virtual void откл();
	virtual bool Ключ(dword ключ, int count);
	virtual void Выкладка();

	Размер вычислиРазм();
	Размер GetCalendarClockSize() { return вычислиРазм(); }
	void UpdateTime(Время &tm);

	void PopUp(Ктрл *owner, const Прям& rt);
};

template<class T>
class DateTimeCtrl : public T {
	MultiButtonFrame drop;
	CalendarClock cc;

	int mode;

	void OnCalendarChoice() {
		Дата dt = ~cc.calendar;
		ПреобрВремя *cv = dynamic_cast<ПреобрВремя *>(this);
		if(cv && cv->конецДня_ли()) {
			Время tm = воВремя(dt);
			tm.hour = 23;
			tm.minute = tm.second = 59;
			this->устДанные(tm);
		}
		else
			this->устДанные(dt);
		this->WhenAction();
	}

	void OnClockChoice() {
		this->устДанные(~cc.clock);
		this->WhenAction();
	}

	void OnClose() {
		this->устФокус();
	}

	void OnDrop() {
		if(!this->редактируем_ли())
			return;

		Размер sz = cc.GetCalendarClockSize();

		int width = sz.cx;
		int height = sz.cy;

		Прям rw = this->Ктрл::GetWorkArea();
		Прям rs = this->дайПрямЭкрана();
		Прям r;
		r.left   = rs.left;
		r.right  = rs.left + width;
		r.top    = rs.bottom;
		r.bottom = rs.bottom + height;

		if(r.bottom > rw.bottom)
		{
			r.top = rs.top - height;
			r.bottom = rs.top;
		}
		if(r.right > rw.right)
		{
			int diff;
			if(rs.right <= rw.right)
				diff = rs.right - r.right;
			else
				diff = rw.right - r.right;

			r.left += diff;
			r.right += diff;
		}
		if(r.left < rw.left)
		{
			int diff = rw.left - r.left;
			r.left += diff;
			r.right += diff;

		}
		if(WhenWeek)
			cc.calendar.WhenWeek = WhenWeek.прокси();
		else
			cc.calendar.WhenWeek.очисть();
		cc.PopUp(this, r);
		cc.calendar <<= this->дайДанные();
		cc.clock <<= this->дайДанные();
	}

public:
	typedef DateTimeCtrl ИМЯ_КЛАССА;
	
	Событие<Дата> WhenWeek;

	DateTimeCtrl(int m) : cc(m) {
		drop.добавьК(*this);
		drop.AddButton().Main() <<= THISBACK(OnDrop);
		drop.NoDisplay();
		drop.устСтиль(drop.StyleFrame());
		drop.GetButton(0).SetMonoImage(Grayscale(CtrlsImg::DA()));
		cc.calendar   <<= THISBACK(OnCalendarChoice);
		cc.clock      <<= THISBACK(OnClockChoice);
		cc.WhenPopDown  = THISBACK(OnClose);
		cc.calendar.WhenSelect = WhenSelect.прокси();
		this->WhenPaper = [=](Цвет c) {
			drop.SetPaper(c);
		};
	}

	virtual void сфокусирован()  { T::сфокусирован(); drop.освежиФрейм(); }
	virtual void расфокусирован() { T::расфокусирован(); drop.освежиФрейм(); }
	virtual Размер дайМинРазм() const { return drop.дайМинРазм(); }

	DateTimeCtrl& SetCalendarStyle(const Calendar::Стиль& style)   { cc.calendar.устСтиль(style); return *this;  }
	DateTimeCtrl& SetClockStyle(const Clock::Стиль& style)         { cc.clock.устСтиль(style); return *this;  }
	DateTimeCtrl& SetButtonStyle(const MultiButton::Стиль& style)  { drop.устСтиль(style); return *this; }
	DateTimeCtrl& выбериВсе(bool b = true)                         { cc.calendar.выбериВсе(b); return *this;     }
	DateTimeCtrl& NoSelectAll()                                    { cc.calendar.выбериВсе(false); return *this; }
	DateTimeCtrl& секунды(bool b = true)                           { cc.clock.секунды(b); return *this; }
	DateTimeCtrl& безСекунд()                                      { cc.clock.секунды(false); return *this; }
	DateTimeCtrl& Colon(bool b = true)                             { cc.clock.Colon(b); return *this; }
	DateTimeCtrl& NoColon()                                        { cc.clock.Colon(false); return *this; }
	DateTimeCtrl& SwapMonthYear(bool b = true)                     { cc.calendar.SwapMonthYear(b); return *this; }
	DateTimeCtrl& OneButton(bool b = true)                         { cc.calendar.OneButton(true); return *this; }
	DateTimeCtrl& NoOneButton()                                    { cc.calendar.OneButton(false); return *this; }
	DateTimeCtrl& конецДня(bool b = true)                            { ПреобрВремя::конецДня(b); return *this; }
	DateTimeCtrl& времяВсегда(bool b = true)                        { ПреобрВремя::времяВсегда(b); return *this; }

	Событие<>  WhenSelect;
};

class DropDate : public DateTimeCtrl<EditDate>
{
public:
	DropDate();
	DropDate& SetDate(int y, int m, int d);
};

class DropTime : public DateTimeCtrl<EditTime>
{
public:
	DropTime();
	DropTime& SetTime(int y, int m, int d, int h, int n, int s);
	DropTime& секунды(bool b = true)                           { DateTimeCtrl<EditTime>::секунды(b); EditTime::секунды(b); return *this; }
	DropTime& безСекунд()                                      { return секунды(false); }
};
