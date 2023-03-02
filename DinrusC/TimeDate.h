class Обнул;
class Поток;
struct ФВремя;

struct Дата : RelOps< Дата, Движимое<Дата> > {
	byte   day;
	byte   month;
	int16  year;

	void     сериализуй(Поток& s);

	bool     пригоден() const;
	void     уст(int scalar);
	int      дай() const;

	static Дата наименьш()                    { return Дата(-4000, 1, 1); }
	static Дата наибольш()                   { return Дата(4000, 1, 1); }
	
	int      сравни(Дата b) const;

	Дата& operator++()                   { if(day < 28) day++; else уст(дай() + 1); return *this; }
	Дата& operator--()                   { if(day > 1) day--; else уст(дай() - 1); return *this; }

	Дата  operator++(int)                { Дата d = *this; operator++(); return d; }
	Дата  operator--(int)                { Дата d = *this; operator--(); return d; }

	Дата()                               { year = -32768; day = month = 0; }
	Дата(const Обнул&)                  { year = -32768; day = month = 0; }
	Дата(int y, int m, int d)            { day = d; month = m; year = y; }
};

inline hash_t дайХэшЗнач(Дата t) {
	return 512 * t.year + 32 * t.month + t.day;
}

inline bool operator==(Дата a, Дата b) {
	return a.day == b.day && a.month == b.month && a.year == b.year;
}

template<> inline bool  пусто_ли(const Дата& d)    { return d.year == -32768; }

bool operator<(Дата a, Дата b);

int   operator-(Дата a, Дата b);
Дата  operator+(Дата a, int b);
Дата  operator+(int a, Дата b);
Дата  operator-(Дата a, int b);
Дата& operator+=(Дата& a, int b);
Дата& operator-=(Дата& a, int b);

bool високосенГод(int year);

int  дайДниМесяца(int month, int year);

int  деньНедели(Дата date);
Дата последнДеньМесяца(Дата d);
Дата первДеньМесяца(Дата d);
Дата последнДеньГода(Дата d);
Дата первДеньГода(Дата d);
int  деньГода(Дата d);

Дата добавьМесяцы(Дата date, int months);
int  дайМесяцы(Дата since, Дата till);
int  GetMonthsP(Дата since, Дата till);
Дата добавьГоды(Дата date, int years);

Дата дайДатуНедели(int year, int week);
int  дайНеделю(Дата d, int& year);

Дата пасха(int year);

Дата дайСисДату();

Ткст имяДня(int i, int lang = 0);
Ткст DyName(int i, int lang = 0);
Ткст имяМесяца(int i, int lang = 0);
Ткст MonName(int i, int lang = 0);

void   устФорматДаты(const char *fmt);
void   устСканДат(const char *scan);
void   устФильтрДат(const char *seps);

const char *тктВДату(const char *fmt, Дата& d, const char *s, Дата опр = Null);
const char *тктВДату(Дата& d, const char *s, Дата опр);
const char *тктВДату(Дата& d, const char *s);
Дата        сканДату(const char *fmt, const char *s, Дата опр = Null);
Дата        сканДату(const char *s, Дата опр = Null);
Ткст        фмт(Дата date);
int         CharFilterDate(int c);

template<>
inline Ткст какТкст(const Дата& date) { return фмт(date); }

struct Время : Дата, RelOps< Время, Движимое<Время> > {
	byte   hour;
	byte   minute;
	byte   second;

	void     сериализуй(Поток& s);

	static Время наибольш()       { return Время(4000, 1, 1); }
	static Время наименьш()        { return Время(-4000, 1, 1); }

	void   уст(int64 scalar);
	int64  дай() const;

	bool   пригоден() const;

	int    сравни(Время b) const;

	Время()                   { hour = minute = second = 0; }
	Время(const Обнул&)      { hour = minute = second = 0; }
	Время(int y, int m, int d, int h = 0, int n = 0, int s = 0)
		{ day = d; month = m; year = y; hour = h; minute = n; second = s; }

	Время(ФВремя filetime);
	ФВремя какФВремя() const;
};

inline Время воВремя(const Дата& d) {
	return Время(d.year, d.month, d.day);
}

inline hash_t дайХэшЗнач(Время t) {
	return t.second +
		   32 * (t.minute + 32 * (t.hour + 16 * (t.day + 32 * (t.month + 8 * t.year))));
}

template<> inline bool  пусто_ли(const Время& t)    { return t.year == -32768; }

bool operator==(Время a, Время b);
bool operator<(Время a, Время b);

int64  operator-(Время a, Время b);
Время   operator+(Время a, int64 seconds);
Время   operator+(int64 seconds, Время a);
Время   operator-(Время a, int64 secs);
Время&  operator+=(Время& a, int64 secs);
Время&  operator-=(Время& a, int64 secs);

inline Время   operator+(Время a, int i)   { return a +  int64(i); }
inline Время   operator-(Время a, int i)   { return a -  int64(i); }
inline Время&  operator+=(Время& a, int i) { return a += int64(i); }
inline Время&  operator-=(Время& a, int i) { return a -= int64(i); }

inline Время   operator+(Время a, double i)   { return a +  int64(i); }
inline Время   operator-(Время a, double i)   { return a -  int64(i); }
inline Время&  operator+=(Время& a, double i) { return a += int64(i); }
inline Время&  operator-=(Время& a, double i) { return a -= int64(i); }

Время  дайСисВремя();
Время  GetUtcTime();

Ткст фмт(Время time, bool seconds = true);
const char *тктВоВремя(const char *datefmt, Время& d, const char *s);
const char *тктВоВремя(Время& d, const char *s);
Время        сканВремя(const char *datefmt, const char *s, Время опр = Null);
Время        сканВремя(const char *s, Время опр = Null);

template<>
inline Ткст какТкст(const Время& time) { return фмт(time); }

bool устСисВремя(Время time); // only root/sysadmin can do this...

int    дайЧПояс();
Ткст дайТкстЧПояса();
int    сканТкстЧПояса(const char *s);
int    сканЧПояс(const char *s);

int   GetLeapSeconds(Дата dt);
int64 GetUTCSeconds(Время tm);
Время  TimeFromUTC(int64 seconds);
