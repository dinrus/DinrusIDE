namespace ДинрусРНЦП{
class Обнул;
class Поток;
struct ФВремя;

struct Дата : ОпыРеляций< Дата, Движ<Дата> > {
	ббайт   day;
	ббайт   month;
	крат  year;

	проц     сериализуй(Поток& s);

	бул     пригоден() const;
	проц     уст(цел scalar);
	цел      дай() const;

	static Дата наименьш()                    { return Дата(-4000, 1, 1); }
	static Дата наибольш()                   { return Дата(4000, 1, 1); }
	
	цел      сравни(Дата b) const;

	Дата& operator++()                   { if(day < 28) day++; else уст(дай() + 1); return *this; }
	Дата& operator--()                   { if(day > 1) day--; else уст(дай() - 1); return *this; }

	Дата  operator++(цел)                { Дата d = *this; operator++(); return d; }
	Дата  operator--(цел)                { Дата d = *this; operator--(); return d; }

	Дата()                               { year = -32768; day = month = 0; }
	Дата(const Обнул&)                  { year = -32768; day = month = 0; }
	Дата(цел y, цел m, цел d)            { day = d; month = m; year = y; }
};

inline т_хэш дайХэшЗнач(Дата t) {
	return 512 * t.year + 32 * t.month + t.day;
}

inline бул operator==(Дата a, Дата b) {
	return a.day == b.day && a.month == b.month && a.year == b.year;
}

template<> inline бул  пусто_ли(const Дата& d)    { return d.year == -32768; }

бул operator<(Дата a, Дата b);

цел   operator-(Дата a, Дата b);
Дата  operator+(Дата a, цел b);
Дата  operator+(цел a, Дата b);
Дата  operator-(Дата a, цел b);
Дата& operator+=(Дата& a, цел b);
Дата& operator-=(Дата& a, цел b);

бул високосенГод(цел year);

цел  дайДниМесяца(цел month, цел year);

цел  деньНедели(Дата date);
Дата последнДеньМесяца(Дата d);
Дата первДеньМесяца(Дата d);
Дата последнДеньГода(Дата d);
Дата первДеньГода(Дата d);
цел  деньГода(Дата d);

Дата добавьМесяцы(Дата date, цел months);
цел  дайМесяцы(Дата since, Дата till);
цел  GetMonthsP(Дата since, Дата till);
Дата добавьГоды(Дата date, цел years);

Дата дайДатуНедели(цел year, цел week);
цел  дайНеделю(Дата d, цел& year);

Дата пасха(цел year);

Дата дайСисДату();

Ткст имяДня(цел i, цел lang = 0);
Ткст DyName(цел i, цел lang = 0);
Ткст имяМесяца(цел i, цел lang = 0);
Ткст MonName(цел i, цел lang = 0);

проц   устФорматДаты(кткст0 fmt);
проц   устСканДат(кткст0 scan);
проц   устФильтрДат(кткст0 seps);

кткст0 тктВДату(кткст0 fmt, Дата& d, кткст0 s, Дата опр = Null);
кткст0 тктВДату(Дата& d, кткст0 s, Дата опр);
кткст0 тктВДату(Дата& d, кткст0 s);
Дата        сканДату(кткст0 fmt, кткст0 s, Дата опр = Null);
Дата        сканДату(кткст0 s, Дата опр = Null);
Ткст        фмт(Дата date);
цел         CharFilterDate(цел c);

template<>
inline Ткст какТкст(const Дата& date) { return фмт(date); }

struct Время : Дата, ОпыРеляций< Время, Движ<Время> > {
	ббайт   hour;
	ббайт   minute;
	ббайт   second;

	проц     сериализуй(Поток& s);

	static Время наибольш()       { return Время(4000, 1, 1); }
	static Время наименьш()        { return Время(-4000, 1, 1); }

	проц   уст(дол scalar);
	дол  дай() const;

	бул   пригоден() const;

	цел    сравни(Время b) const;

	Время()                   { hour = minute = second = 0; }
	Время(const Обнул&)      { hour = minute = second = 0; }
	Время(цел y, цел m, цел d, цел h = 0, цел n = 0, цел s = 0)
		{ day = d; month = m; year = y; hour = h; minute = n; second = s; }

	Время(ФВремя filetime);
	ФВремя какФВремя() const;
};

inline Время воВремя(const Дата& d) {
	return Время(d.year, d.month, d.day);
}

inline т_хэш дайХэшЗнач(Время t) {
	return t.second +
		   32 * (t.minute + 32 * (t.hour + 16 * (t.day + 32 * (t.month + 8 * t.year))));
}

template<> inline бул  пусто_ли(const Время& t)    { return t.year == -32768; }

бул operator==(Время a, Время b);
бул operator<(Время a, Время b);

дол  operator-(Время a, Время b);
Время   operator+(Время a, дол seconds);
Время   operator+(дол seconds, Время a);
Время   operator-(Время a, дол secs);
Время&  operator+=(Время& a, дол secs);
Время&  operator-=(Время& a, дол secs);

inline Время   operator+(Время a, цел i)   { return a +  дол(i); }
inline Время   operator-(Время a, цел i)   { return a -  дол(i); }
inline Время&  operator+=(Время& a, цел i) { return a += дол(i); }
inline Время&  operator-=(Время& a, цел i) { return a -= дол(i); }

inline Время   operator+(Время a, дво i)   { return a +  дол(i); }
inline Время   operator-(Время a, дво i)   { return a -  дол(i); }
inline Время&  operator+=(Время& a, дво i) { return a += дол(i); }
inline Время&  operator-=(Время& a, дво i) { return a -= дол(i); }

Время  дайСисВремя();
Время  GetUtcTime();

Ткст фмт(Время time, бул seconds = true);
кткст0 тктВоВремя(кткст0 datefmt, Время& d, кткст0 s);
кткст0 тктВоВремя(Время& d, кткст0 s);
Время        сканВремя(кткст0 datefmt, кткст0 s, Время опр = Null);
Время        сканВремя(кткст0 s, Время опр = Null);

template<>
inline Ткст какТкст(const Время& time) { return фмт(time); }

бул устСисВремя(Время time); // only root/sysadmin can do this...

цел    дайЧПояс();
Ткст дайТкстЧПояса();
цел    сканТкстЧПояса(кткст0 s);
цел    сканЧПояс(кткст0 s);

цел   GetLeapSeconds(Дата dt);
дол GetUTCSeconds(Время tm);
Время  TimeFromUTC(дол seconds);
}