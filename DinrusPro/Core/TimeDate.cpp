#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

static цел s_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static цел s_month_off[] = {
	  0,  31,  59,  90, 120, 151,
	181, 212, 243, 273, 304, 334
};

бул високосенГод(цел year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

проц Дата::сериализуй(Поток& s)
{
	s % day % month % year;
}

цел  дайДниМесяца(цел m, цел y) {
	ПРОВЕРЬ(m >= 1 && m <= 12);
	return s_month[m - 1] + (m == 2) * високосенГод(y);
}

бул Дата::пригоден() const {
	return year == -32768 || month >= 1 && month <= 12 && day >= 1 && day <= дайДниМесяца(month, year);
}

Ткст имяДня(цел i, цел lang)
{
	static кткст0 day[] = {
		tt_("date\vSunday"), tt_("date\vMonday"), tt_("date\vTuesday"),
		tt_("date\vWednesday"), tt_("date\vThursday"), tt_("date\vFriday"), tt_("date\vSaturday")
	};
	return i >= 0 && i < 7 ? Nvl(GetLngString(lang, day[i]), GetENUS(day[i])) : Ткст();
}

Ткст DyName(цел i, цел lang)
{
	static кткст0 day[] = {
		tt_("date\vSu"), tt_("date\vMo"), tt_("date\vTu"),
		tt_("date\vWe"), tt_("date\vTh"), tt_("date\vFr"), tt_("date\vSa")
	};
	return i >= 0 && i < 7 ? Nvl(GetLngString(lang, day[i]), GetENUS(day[i])) : Ткст();
}

Ткст имяМесяца(цел i, цел lang)
{
	static кткст0 month[] = {
		tt_("date\vJanuary"), tt_("date\vFebruary"), tt_("date\vMarch"), tt_("date\vApril"), tt_("date\vMay"),
		tt_("date\vJune"), tt_("date\vJuly"), tt_("date\vAugust"), tt_("date\vSeptember"), tt_("date\vOctober"),
		tt_("date\vNovember"), tt_("date\vDecember")
	};
	return i >= 0 && i < 12 ? Nvl(GetLngString(lang, month[i]), GetENUS(month[i])) : Ткст();
}

Ткст MonName(цел i, цел lang)
{
	static кткст0 month[] = {
		tt_("sdate\vJan"), tt_("sdate\vFeb"), tt_("sdate\vMar"), tt_("sdate\vApr"), tt_("sdate\vMay"),
		tt_("sdate\vJun"), tt_("sdate\vJul"), tt_("sdate\vAug"), tt_("sdate\vSep"), tt_("sdate\vOct"),
		tt_("sdate\vNov"), tt_("sdate\vDec")
	};
	return i >= 0 && i < 12 ? Nvl(GetLngString(lang, month[i]), GetENUS(month[i])) : Ткст();
}

static thread_local char s_date_format_thread[64];
static char s_date_format_main[64] = "%2:02d/%3:02d/%1:4d";

проц   устФорматДаты(кткст0 fmt)
{
	strncpy(s_date_format_thread, fmt, 63);
	if(Нить::главная_ли())
		strncpy(s_date_format_main, fmt, 63);
}

Ткст   фмт(Дата date) {
	Ткст  s;
	if(пусто_ли(date))
		return Ткст();
	return фмт(*s_date_format_thread ? s_date_format_thread : s_date_format_main, date.year, date.month, date.day, деньНедели(date));
}

static thread_local char s_date_scan_thread[64];
static char s_date_scan_main[64] = "mdy";

проц   устСканДат(кткст0 scan)
{
	strncpy(s_date_scan_thread, scan, 63);
	if(Нить::главная_ли())
		strncpy(s_date_scan_main, scan, 63);
}

кткст0 тктВДату(Дата& d, кткст0 s, Дата опр)
{
	return тктВДату(*s_date_scan_thread ? s_date_scan_thread : s_date_scan_main, d, s, опр);
}

кткст0 тктВДату(кткст0 fmt, Дата& d, кткст0 s, Дата опр)
{
	if(*s == 0) {
		d = Null;
		return s;
	}
	d = Nvl(опр, дайСисДату());
	while(*fmt) {
		while(*s && !цифра_ли(*s) && !альфа_ли(*s) && (ббайт)*s < 128)
			s++;
		цел n;
		if(цифра_ли(*s)) {
			char *q;
			n = strtoul(s, &q, 10);
			s = q;
		}
		else
		if(альфа_ли(*s) || (ббайт)*s >= 128) {
			if(*fmt != 'm')
				return NULL;
			Ткст m;
			while(альфа_ли(*s) || (ббайт)*s >= 128)
				m.кат(*s++);
			m = взаг(m);
			for(цел i = 0; i < 12; i++)
				if(m == взаг(имяМесяца(i)) || m == взаг(MonName(i))) {
					n = i + 1;
					goto found;
				}
			return NULL;
		found:
			;
		}
		else
			break;

		switch(*fmt) {
		case 'd':
			if(n < 1 || n > 31)
				return NULL;
			d.day = n;
			break;
		case 'm':
			if(n < 1 || n > 12)
				return NULL;
			d.month = n;
			break;
		case 'y':
			d.year = n;
			if(d.year < 35) d.year += 2000; // Check again in 2030.... // TODO: сделай this automatic
			else
			if(d.year < 100) d.year += 1900;
			break;
		default:
			НИКОГДА();
		}
		fmt++;
	}
	return d.пригоден() ? s : NULL;
}

кткст0 тктВДату(Дата& d, кткст0 s)
{
	return тктВДату(d, s, Null);
}

Дата сканДату(кткст0 fmt, кткст0 s, Дата опр)
{
	Дата d;
	if(тктВДату(fmt, d, s, опр))
		return d;
	return опр;
}

Дата сканДату(кткст0 s, Дата опр)
{
	Дата d;
	if(тктВДату(d, s, опр))
		return d;
	return опр;
}

static thread_local char s_date_seps_thread[64];
static char s_date_seps_main[64] = "A/\a .-";

проц   устФильтрДат(кткст0 seps)
{
	strncpy(s_date_seps_thread, seps, 63);
	if(Нить::главная_ли())
		strncpy(s_date_seps_main, seps, 63);
}

цел  CharFilterDate(цел c)
{
	if(цифра_ли(c))
		return c;
	кткст0 s = *s_date_seps_thread ? s_date_seps_thread : s_date_seps_main;
	бул letters = false;
	бул upper = false;
	if(*s == 'a') {
		letters = true;
		s++;
	}
	else
	if(*s == 'A') {
		letters = true;
		upper = true;
		s++;
	}
	if(letters && буква_ли(c))
		return upper ? взаг(c) : c;
	;
	цел r = 0;
	while(*s) {
		if(c == (ббайт)*s)
			return c;
		if(*s == '\r') {
			s++;
			while(*s != '\r') {
				if((ббайт)*s == c)
					return r;
				if(*s == '\0')
					return 0;
				s++;
			}
			s++;
		}
		else
			r = *s++;
	}
	return 0;
}

цел Дата::дай() const
{
	if(пусто_ли(*this))
		return Null;
	цел y400 = (year / 400 ) - 2;
	цел ym = year - y400 * 400;
	return y400 * (400 * 365 + 100 - 3) +
	        ym * 365 + s_month_off[month - 1] + (day - 1) +
	       (ym - 1) / 4 - (ym - 1) / 100 + (ym - 1) / 400 + 1 +
	       (month > 2) * високосенГод(ym);
}

проц Дата::уст(цел d)
{
	if(пусто_ли(d)) {
		*this = Null;
		return;
	}
	цел q, leap;
	year = 0;
	q = d / (400 * 365 + 100 - 3);
	year += 400 * q;
	d -= q * (400 * 365 + 100 - 3);
	if(d < 0) {
		year -= 400;
		d += 400 * 365 + 100 - 3;
	}
 	leap = 1;
	if(d >= 100 * 365 + 24 + 1) {
		d--;
		q = d / (100 * 365 + 24);
		year += 100 * q;
		d -= q * (100 * 365 + 24);
		leap = 0;
	}
	if(d >= 365 * 4 + leap) {
		q = (d + 1 - leap) / (365 * 4 + 1);
		year += 4 * q;
		d -= q * (365 * 4 + 1) - 1 + leap;
		leap = 1;
	}
	if(d >= 365 + leap) {
		q = (d - leap) / 365;
		year += q;
		d -= q * 365 + leap;
		leap = 0;
	}
	цел i;
	for(i = 0; i < 12; i++) {
		цел q = s_month[i] + (i == 1) * leap;
		if(q > d) break;
		d -= q;
	}
	month = i + 1;
	day = d + 1;
}

цел Дата::сравни(Дата b) const
{
	цел q = сравниЗнак(year, b.year);
	if(q) return q;
	q = сравниЗнак(month, b.month);
	return q ? q : сравниЗнак(day, b.day);
}

бул operator<(Дата a, Дата b) {
	return a.сравни(b) < 0;
}

цел   operator-(Дата a, Дата b)    { return a.дай() - b.дай(); }
Дата  operator+(Дата a, цел b)     { Дата q; q.уст(a.дай() + b); return q; }
Дата  operator+(цел a, Дата b)     { Дата q; q.уст(b.дай() + a); return q; }
Дата  operator-(Дата a, цел b)     { Дата q; q.уст(a.дай() - b); return q; }
Дата& operator+=(Дата& a, цел b)   { a.уст(a.дай() + b); return a; }
Дата& operator-=(Дата& a, цел b)   { a.уст(a.дай() - b); return a; }

цел деньНедели(Дата date) {
	return (date.дай() + 6) % 7;
}

Дата последнДеньМесяца(Дата d) {
	d.day = дайДниМесяца(d.month, d.year);
	return d;
}

Дата первДеньМесяца(Дата d) {
	d.day = 1;
	return d;
}

Дата последнДеньГода(Дата d)
{
	d.day = 31;
	d.month = 12;
	return d;
}

Дата первДеньГода(Дата d)
{
	d.day = 1;
	d.month = 1;
	return d;
}

цел деньГода(Дата d) {
	return 1 + d - первДеньГода(d);
}

Дата добавьМесяцы(Дата date, цел months) {
	months += date.month - 1;
	цел year = idivfloor(months, 12);
	months -= 12 * year;
	year += date.year;
	date.year = year;
	date.month = months + 1;
	date.day = мин(date.day, последнДеньМесяца(date).day);
	return date;
}

цел дайМесяцы(Дата since, Дата till)
{
	return 12 * till.year + till.month - (12 * since.year + since.month) + (till.day >= since.day) - 1;
}

цел GetMonthsP(Дата since, Дата till)
{
	return 12 * till.year + till.month - (12 * since.year + since.month) + (till.day > since.day);
}

Дата добавьГоды(Дата date, цел years) {
	date.year += years;
	date.day = мин(date.day, последнДеньМесяца(date).day);
	return date;
}

Дата дайДатуНедели(цел year, цел week)
{
	цел jan1 = Дата(year, 1, 1).дай();
	цел start = (jan1 - 584390) / 7 * 7 + 584390; // 584390 = 03.01.1600 Monday
	if(jan1 - start > 3)
		start += 7;
	Дата d;
	d.уст(start + 7 * (week - 1));
	return d;
}

цел дайНеделю(Дата d, цел& year)
{
	year = d.year;
	Дата d0 = дайДатуНедели(year + 1, 1);
	if(d >= d0)
		year++;
	else {
		d0 = дайДатуНедели(year, 1);
		if(d < d0)
			d0 = дайДатуНедели(--year, 1);
	}
	return (d - d0) / 7 + 1;
}

Дата пасха(цел year)
{
    цел a = year % 19;
    цел b = year >> 2;
    цел c = b / 25 + 1;
    цел d = (c * 3) >> 2;
    цел e = ((a * 19) - ((c * 8 + 5) / 25) + d + 15) % 30;
    e += (29578 - a - e * 32) >> 10;
    e -= ((year % 7) + b - d + e + 2) % 7;
    d = e >> 5;
    return Дата(year, d + 3, e - d * 31);
}

проц Время::сериализуй(Поток& s)
{
	s % day % month % year % hour % minute % second;
}

#ifdef PLATFORM_WIN32
Время::Время(ФВремя filetime)
{
	SYSTEMTIME tm, tml;
	ФВремя ft;
	FileTimeToSystemTime(&filetime, &tm);
	SystemTimeToTzSpecificLocalTime(NULL, &tm, &tml);
	*this = Время(tml.wYear, tml.wMonth, tml.wDay, tml.wHour, tml.wMinute, tml.wSecond);
}

ФВремя Время::какФВремя() const
{
	SYSTEMTIME tm;
	tm.wYear = year;
	tm.wMonth = month;
	tm.wDay = day;
	tm.wHour = hour;
	tm.wMinute = minute;
	tm.wSecond = second;
	tm.wMilliseconds = 0;
	ФВремя ft, filetime;
	SystemTimeToFileTime(&tm, &ft);
	LocalFileTimeToFileTime(&ft, &filetime);
	return filetime;
}
#endif

#ifdef PLATFORM_POSIX
Время::Время(ФВремя filetime) {
	struct tm time_r;
	struct tm *time = localtime_r(&filetime.ft, &time_r);
	if(time)
		*this = Время(time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
	                 time->tm_hour, time->tm_min, time->tm_sec);
}

ФВремя Время::какФВремя() const {
	struct tm time;
	time.tm_year = year - 1900;
	time.tm_mon = month - 1;
	time.tm_mday = day;
	time.tm_hour = hour;
	time.tm_min = minute;
	time.tm_sec = second;
	time.tm_isdst = -1;
	return mktime(&time);
}
#endif

бул operator==(Время a, Время b) {
	return a.day == b.day && a.month == b.month && a.year == b.year &&
		   a.hour == b.hour && a.minute == b.minute && a.second == b.second;
}

цел Время::сравни(Время b) const
{
	цел q = Дата::сравни(b);
	if(q) return q;
	q = сравниЗнак(hour, b.hour);
	if(q) return q;
	q = сравниЗнак(minute, b.minute);
	if(q) return q;
	return сравниЗнак(second, b.second);
}

бул operator<(Время a, Время b) {
	return a.сравни(b) < 0;
}

проц Время::уст(дол scalar)
{
	цел q = (цел)(scalar / (24 * 3600));
	Дата::уст(q);
	цел n = цел(scalar - (дол)q * 24 * 3600);
	hour = n / 3600;
	n %= 3600;
	minute = n / 60;
	second = n % 60;
}

дол Время::дай() const
{
	return Дата::дай() * (дол)24 * 3600 + hour * 3600 + minute * 60 + second;
}

бул Время::пригоден() const
{
	return year == -32768 ||
	       Дата::пригоден() && hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60;
}

дол operator-(Время a, Время b) {
	return a.дай() - b.дай();
}

Время operator+(Время time, дол secs) {
	time.уст(time.дай() + secs);
	return time;
}

Время  operator+(дол seconds, Время a)          { return a + seconds; }
Время  operator-(Время a, дол secs)             { return a + (-secs); }
Время& operator+=(Время& a, дол secs)           { a = a + secs; return a; }
Время& operator-=(Время& a, дол secs)           { a = a - secs; return a; }

Ткст фмт(Время time, бул seconds)
{
	if(пусто_ли(time)) return Ткст();
	return фмт(Дата(time)) + (seconds ? фмт(" %02d:%02d:%02d", time.hour, time.minute, time.second)
	                                     : фмт(" %02d:%02d", time.hour, time.minute));
}

кткст0 тктВоВремя(кткст0 datefmt, Время& d, кткст0 s)
{
	s = тктВДату(datefmt, d, s);
	if(!s)
		return NULL;
	d.hour = d.minute = d.second = 0;
	for(цел i = 0; i < 3; i++) {
		while(*s == ' ' || *s == ':')
			s++;
		цел n;
		if(цифра_ли(*s)) {
			char *q;
			n = strtoul(s, &q, 10);
			s = q;
		} else
			break;
		if(n < 0 || n > (i ? 59 : 23))
			return NULL;
		(i == 0 ? d.hour : i == 1 ? d.minute : d.second) = n;
	}
	return s;
}

кткст0 тктВоВремя(Время& d, кткст0 s)
{
	return тктВоВремя(*s_date_scan_thread ? s_date_scan_thread : s_date_scan_main, d, s);
}

Время сканВремя(кткст0 datefmt, кткст0 s, Время опр)
{
	Время tm;
	if(тктВоВремя(datefmt, tm, s))
		return tm;
	return опр;
}

Время сканВремя(кткст0 s, Время опр)
{
	Время tm;
	if(тктВоВремя(tm, s))
		return tm;
	return опр;
}

#ifdef PLATFORM_WIN32

Время дайСисВремя() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	return Время(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

Время GetUtcTime() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return Время(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

#endif

#ifdef PLATFORM_POSIX

Время дайСисВремя() {
	return Время(time(NULL));
}

Время GetUtcTime()
{
	time_t gmt = time(NULL);
	struct tm timer;
	struct tm *utc = gmtime_r(&gmt, &timer);
	return Время(utc->tm_year + 1900, utc->tm_mon + 1, utc->tm_mday,
	            utc->tm_hour, utc->tm_min, utc->tm_sec);
}

#endif

Дата дайСисДату() {
	return дайСисВремя();
}

бул устСисВремя(Время time)
{
#ifdef PLATFORM_POSIX
	struct tm      tmp_time;
	tmp_time.tm_sec  = time.second;
	tmp_time.tm_min  = time.minute;
	tmp_time.tm_hour = time.hour;
	tmp_time.tm_mday = time.day;
	tmp_time.tm_mon  = time.month-1;
	tmp_time.tm_year = time.year-1900;
	time_t raw_time  = mktime(&tmp_time);

	struct ::timespec sys_time;
	sys_time.tv_sec  = raw_time;
	sys_time.tv_nsec = 0;

	цел result = clock_settime(CLOCK_REALTIME, &sys_time);
	return (result == 0);
#endif
#ifdef PLATFORM_WIN32
	SYSTEMTIME systime;
	systime.wYear	= time.year;
	systime.wMonth	= time.month;
	systime.wDay	= time.day;
	systime.wHour	= time.hour;
	systime.wMinute	= time.minute;
	systime.wSecond	= time.second;
	systime.wDayOfWeek = 0;
	systime.wMilliseconds = 0;
	return SetLocalTime( &systime );
#endif
}

цел дайЧПояс()
{
	static цел zone;
	ONCELOCK {
		for(;;) { // This is somewhat ugly, but unified approach to get time zone offset
			Время t0 = дайСисВремя();
			Время gmtime = GetUtcTime();
			Время ltime = дайСисВремя();
			if(дайСисВремя() - t0 < 1) { // сделай sure that there is not much time between calls
				zone = (цел)(ltime - gmtime) / 60; // Round to minutes
				break;
			}
		}
	}
	return zone;
}

Ткст FormatTimeZone(цел n)
{
	return (n < 0 ? "-" : "+") + фмт("%02.2d%02.2d", абс(n) / 60, абс(n) % 60);
}

Ткст дайТкстЧПояса()
{
	return FormatTimeZone(дайЧПояс());
}

цел сканЧПояс(кткст0 s)
{
	цел n = atoi(s);
	цел hour = абс(n) / 100;
	цел minutes = абс(n) % 100;
	if(hour >= 0 && hour <= 12 && minutes >= 0 && minutes < 60)
		return зн(n) * (hour * 60 + minutes);
	return 0;
}

цел GetLeapSeconds(Дата dt)
{
	static Кортеж<цел, цел> sLeapSeconds[] = {
		{ 1972,6 }, { 1972,12 }, { 1973,12 }, { 1974,12 }, { 1975,12 }, { 1976,12 }, { 1977,12 },
		{ 1978,12 }, { 1979,12 }, { 1981,6 }, { 1982,6 }, { 1983,6 }, { 1985,6 }, { 1987,12 },
		{ 1989,12 }, { 1990,12 }, { 1992,6 }, { 1993,6 }, { 1994,6 }, { 1995,12 }, { 1997,6 },
		{ 1998,12 }, { 2005,12 }, { 2008,12 }, { 2012,6 }, { 2015,6 }
	};
	static ббайт ls[1200]; // 100 years of leap seconds per month
	ONCELOCK {
		for(цел i = 0; i < __countof(sLeapSeconds); i++) {
			цел l = (sLeapSeconds[i].a - 1970) * 12 + sLeapSeconds[i].b - 1;
			memset(ls + l, i + 1, __countof(ls) - l);
		}
	}
	return ls[минмакс(12 * (dt.year - 1970) + dt.month - 1, 0, __countof(ls) - 1)];
}

дол GetUTCSeconds(Время tm)
{
	return tm - Время(1970, 1, 1) + GetLeapSeconds(tm);
}

Время TimeFromUTC(дол seconds)
{
	Время tm = Время(1970, 1, 1) + seconds;
	return tm - GetLeapSeconds(tm);
}

}
