#include "Core.h"

namespace РНЦПДинрус {

double ipow10_table[601];

unsigned stou(const char *s, void *endptr, unsigned base)
{
	ПРОВЕРЬ(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // Ошибка
		if(endptr)
			*(const char **)endptr = s;
		return ~0;
	}
	unsigned значение = digit;
	while((digit = ctoi(*++s)) < base) {
		unsigned v0 = значение;
		значение = значение * base + digit;
		if(v0 > значение) // overflow
			return ~0;
	}
	if(endptr)
		*(const char **)endptr = s;
	return значение;
}

unsigned stou(const wchar *s, void *endptr, unsigned base)
{
	ПРОВЕРЬ(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // Ошибка
		if(endptr)
			*(const wchar **)endptr = s;
		return ~0;
	}
	unsigned значение = digit;
	while((digit = ctoi(*++s)) < base) {
		unsigned v0 = значение;
		значение = значение * base + digit;
		if(v0 > значение) // overflow
			return ~0;
	}
	if(endptr)
		*(const wchar **)endptr = s;
	return значение;
}


uint64 stou64(const char *s, void *endptr, unsigned base)
{
	ПРОВЕРЬ(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // Ошибка
		if(endptr)
			*(const char **)endptr = s;
		return ~0;
	}
	uint64 значение = digit;
	while((digit = ctoi(*++s)) < base) {
		uint64 v0 = значение;
		значение = значение * base + digit;
		if(v0 > значение) // overflow
			return ~0;
	}
	if(endptr)
		*(const char **)endptr = s;
	return значение;
}

int сканЦел(const char *ptr, const char **endptr, int base)
{
	const char *s = ptr;
	bool minus = false;
	while(*s && (byte)*s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	unsigned u = stou(s, endptr, base);
	if(~u)
		return (minus ? -(int)u : (int)u);
	else
		return Null;
}

int сканЦел(const wchar *ptr, const wchar **endptr, int base)
{
	const wchar *s = ptr;
	bool minus = false;
	while(*s && *s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	unsigned u = stou(s, endptr, base);
	if(~u)
		return (minus ? -(int)u : (int)u);
	else
		return Null;
}

int64 сканЦел64(const char *ptr, const char **endptr, int base)
{
	const char *s = ptr;
	bool minus = false;
	while(*s && *s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	uint64 u = stou64(s, endptr, base);
	if(~u)
		return (minus ? -(int64)u : (int64)u);
	else
		return Null;
}

int сканЦел(const char *ptr, const char **endptr)
{
	int x;
	bool overflow = false;
	ptr = сканЦел<char, byte, uint32, int, 10>(x, ptr, overflow);
	if(ptr && endptr)
		*endptr = ptr;
	return !overflow && ptr ? x : Null;
}

int сканЦел(const char *ptr)
{
	int x;
	bool overflow = false;
	return сканЦел<char, byte, uint32, int, 10>(x, ptr, overflow) && !overflow ? x : Null;
}

int64 сканЦел64(const char *ptr, const char **endptr)
{
	int64 x;
	bool overflow = false;
	ptr = сканЦел<char, byte, uint64, int64, 10>(x, ptr, overflow);
	if(ptr && endptr)
		*endptr = ptr;
	return !overflow && ptr ? x : Null;
}

int64 сканЦел64(const char *ptr)
{
	int64 x;
	bool overflow = false;
	return сканЦел<char, byte, uint64, int64, 10>(x, ptr, overflow) && !overflow ? x : Null;
}

Значение тктЦелЗнач(const char *s)
{
	if(s && *s) {
		const char *p;
		int64 q = сканЦел64(s, &p);
		if(!пусто_ли(q))
			while(*p) {
				if((byte)*p > ' ')
					return значОш(t_("Неверное число !"));
				p++;
			}
		return пусто_ли(q) ? значОш(t_("Неверное число !")) : Значение(q);
	}
	return (int)Null;
}

Значение тктДвоЗнач(const char *s)
{
	if(s && *s) {
		const char *p;
		double q = сканДво(s, &p);
		if(!пусто_ли(q))
			while(*p) {
				if((byte)*p > ' ')
					return значОш(t_("Неверное число !"));
				p++;
			}
		return пусто_ли(q) ? значОш(t_("Неверное число !")) : Значение(q);
	}
	return (double)Null;
}

Значение скан(dword qtype, const Ткст& text, const Значение& опр, bool *hastime) {
	Дата date;
	const char *s;
	if(hastime)
		*hastime = false;
	switch(qtype) {
	case INT64_V:
	case INT_V:
	case BOOL_V:
		return тктЦелЗнач(text);
	case DATE_V:
		if(text.пустой()) return (Дата) Null;
		s = тктВДату(date, text, (Дата)опр);
		if(s)
			for(;;) {
				if(цифра_ли(*s))
					break;
				if(*s == '\0')
					return date;
				s++;
			}
		return значОш(t_("Неверная дата !"));
	case TIME_V: {
		if(text.пустой()) return (Время) Null;
		s = тктВДату(date, text, (Дата)опр);
		if(s)
			try {
				СиПарсер p(s);
				Время tm = воВремя(date);
				Время d = (Время)опр;
				tm.hour = d.hour;
				tm.minute = d.minute;
				tm.second = d.second;
				if(p.кф_ли())
					return tm;
				if(hastime)
					*hastime = true;
				int q = p.читайЦел();
				if(q < 0 || q > 23)
					throw СиПарсер::Ошибка("");
				tm.hour = q;
				if(p.кф_ли())
					return tm;
				p.передайСим(':');
				q = p.читайЦел();
				if(q < 0 || q > 59)
					throw СиПарсер::Ошибка("");
				tm.minute = q;
				if(p.кф_ли())
					return tm;
				p.передайСим(':');
				q = p.читайЦел();
				if(q < 0 || q > 59)
					throw СиПарсер::Ошибка("");
				tm.second = q;
				if(p.кф_ли())
					return tm;
			}
			catch(СиПарсер::Ошибка) {}
		return значОш(t_("Неверное время !"));
	}
	case STRING_V:
	case WSTRING_V:
		return text;
	case DOUBLE_V:
		return тктДвоЗнач(text);
	default:
		ПРОВЕРЬ(0);
		break;
	}
	return Null;
}

Преобр::Преобр() {}
Преобр::~Преобр() {}

Значение  Преобр::фмт(const Значение& q) const {
	if(проц_ли(q) || q.пусто_ли()) return Ткст();
	switch(q.дайТип()) {
	case INT64_V:
		return целТкт64((int64)q);
	case INT_V:
	case BOOL_V:
		return целТкт((int)q);
	case DOUBLE_V:
		return двоТкт((double)q);
	case DATE_V:
		return РНЦП::фмт(Дата(q));
	case TIME_V:
		return РНЦП::фмт(Время(q));
	case STRING_V:
	case WSTRING_V:
		return q;
	}
	return q.вТкст();
}

Значение  Преобр::скан(const Значение& text) const {
	return text;
};

int    Преобр::фильтруй(int chr) const {
	return chr;
}

const Преобр& стдПреобр()
{
	static Преобр h;
	return h;
}

Ткст стдФормат(const Значение& q) {
	return стдПреобр().фмт(q);
}

Значение ОшибкаНеПусто() {
	return значОш(t_("Null значение not allowed."));
}

Значение ПреобрЦел::скан(const Значение& text) const {
	Значение v = РНЦП::скан(INT_V, text);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	int64 m = v;
	if(m >= minval && m <= maxval) {
		if(m >= INT_MIN && m <= INT_MAX)
			return (int)m;
		else
			return v;
	}
	return значОш(РНЦП::фмт(t_("Number must be between %d and %d."), minval, maxval));
}

int   ПреобрЦел::фильтруй(int chr) const {
	return minval >= 0 ? CharFilterDigit(chr) : CharFilterInt(chr);
}

Значение ПреобрДво::фмт(const Значение& q) const
{
	if(пусто_ли(q))
		return Null;
	return РНЦП::фмт(pattern, (double)q);
}

Значение ПреобрДво::скан(const Значение& txt) const {
	Ткст text = txt;
	if(pattern.дайСчёт() && pattern != "%.10g") { // фиксируй text with patterns like "%2.!n EUR" (e.g. 1.2 EUR)
		text = РНЦП::фильтруй(text, CharFilterDouble);
		while(взаг(*text.последний()) == 'E')
			text.обрежь(text.дайСчёт() - 1);
	}
	Значение v = РНЦП::скан(DOUBLE_V, text);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	double m = v;
	if(m >= minval && m <= maxval) return v;
	return значОш(РНЦП::фмт(t_("Number must be between %g and %g."), minval, maxval));
}

int   ПреобрДво::фильтруй(int chr) const {
	chr = CharFilterDouble(chr);
	return comma && chr == '.' ? ',' : chr;
}

ПреобрДво::ПреобрДво(double minval, double maxval, bool notnull)
  : minval(minval), maxval(maxval), notnull(notnull)
{
	pattern = "%.10g";
	comma = false;
}

ПреобрДво& ПреобрДво::образец(const char *p)
{
	pattern = p;
	comma = Ткст(фмт(1.1)).найди(',') >= 0;
	return *this;
}

Дата& ПреобрДату::default_min()
{
	static Дата v = Дата::наименьш();
	return v;
}

Дата& ПреобрДату::default_max()
{
	static Дата v = Дата::наибольш();
	return v;
}

void ПреобрДату::устДефМинМакс(Дата min, Дата max)
{
	default_min() = min;
	default_max() = max;
}

ПреобрДату::ПреобрДату(Дата minval, Дата maxval, bool notnull)
: minval(minval), maxval(maxval), notnull(notnull) {
	defaultval = Null;
}

Значение ПреобрДату::фмт(const Значение& q) const
{
	if(датаВремя_ли(q))
		return Преобр::фмт((Дата)q);
	return Преобр::фмт(q);
}

Значение ПреобрДату::скан(const Значение& text) const {
	Значение v = РНЦП::скан(DATE_V, text, defaultval);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	Дата m = v;
	Дата minval = дайМин();
	Дата maxval = дайМакс();
	if(m >= minval && m <= maxval) return v;
	return значОш(t_("Дата должна быть между ") + РНЦП::фмт(minval) + t_("диапазоном\v и ") + РНЦП::фмт(maxval) + ".");
}

int   ПреобрДату::фильтруй(int chr) const {
	return CharFilterDate(chr);
}

ПреобрВремя::ПреобрВремя(Время minval, Время maxval, bool notnull)
: minval(minval), maxval(maxval), notnull(notnull), seconds(true) {
	defaultval = Null;
	timealways = false;
	dayend = false;
}

ПреобрВремя::~ПреобрВремя()
{
}

Значение ПреобрВремя::скан(const Значение& text) const
{
	bool hastime;
	Значение v = РНЦП::скан(TIME_V, text, defaultval, &hastime);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	Время m = v;
	if(!hastime && dayend) {
		m.hour = 23;
		m.minute = 59;
		m.second = 59;
		v = m;
	}
	Время minval = дайМин();
	Время maxval = дайМакс();
	if(m >= minval && m <= maxval) return v;
	return значОш(t_("Время must be between ") + РНЦП::фмт(minval) + t_("range\v and ") + РНЦП::фмт(maxval) + ".");
}

int ПреобрВремя::фильтруй(int chr) const
{
	if(цифра_ли(chr) || chr == ' ' || chr == '.' || chr == ':')
		return chr;
	if(chr == ',')
		return '.';
	return CharFilterDate(chr);
}

Значение ПреобрВремя::фмт(const Значение& q) const
{
	if(проц_ли(q) || q.пусто_ли())
		return Ткст();
	else
	if(q.дайТип() == TIME_V || timealways)
		return воВремя((Дата)q) != (Время)q || timealways ? РНЦП::фмт(Время(q), seconds) : РНЦП::фмт(Дата(q));
	else
		return Преобр::фмт(q);
}

Значение ПреобрТкст::скан(const Значение& text) const {
	if(ошибка_ли(text)) return text;
	if(text.дайТип() == STRING_V) {
		Ткст s = text;
		if(trimleft)
			s = РНЦПДинрус::обрежьЛево(s);
		if(trimright)
			s = РНЦПДинрус::обрежьПраво(s);
		if(пусто_ли(s)) return notnull ? ОшибкаНеПусто() : Значение(s);
		if(s.дайДлину() <= maxlen) return s;
	}
	if(text.дайТип() == WSTRING_V) {
		ШТкст s = text;
		if(trimleft)
			s = РНЦПДинрус::обрежьЛево(s);
		if(trimright)
			s = РНЦПДинрус::обрежьПраво(s);
		if(пусто_ли(s)) return notnull ? ОшибкаНеПусто() : Значение(s);
		if(s.дайДлину() <= maxlen) return s;
	}
	return значОш(РНЦП::фмт(t_("Please enter no more than %d characters."), maxlen));
}

const ПреобрЦел& стдПреобрЦел() { static ПреобрЦел h; return h; }
const ПреобрЦел& стдПреобрЦелНеПусто() { static ПреобрЦел h(-INT_MAX, INT_MAX, true); return h; }

const ПреобрДво& стдПреобрДво() { static ПреобрДво h; return h; }
const ПреобрДво& стдПреобрДвоНеПусто() { static ПреобрДво h(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), true); return h; }

const ПреобрДату& стдПреобрДату() { static ПреобрДату h; return h; }
const ПреобрДату& стдПреобрДатуНеПусто() { static ПреобрДату h(Дата(0, 0, 0), Дата(3000, 12, 31), true); return h; }

const ПреобрВремя& стдПреобрВремя() { static ПреобрВремя h; return h; }
const ПреобрВремя& стдПреобрВремяНеПусто() { static ПреобрВремя h(Null, Null, true); return h; }

const ПреобрТкст& стдПреобрТкст() { static ПреобрТкст h; return h; }
const ПреобрТкст& стдПреобрТкстНеПусто() { static ПреобрТкст h(INT_MAX, true); return h; }

Значение  МапПреобр::фмт(const Значение& q) const {
	return map.дай(q, default_value);
}

КлассНеПреобр::КлассНеПреобр() {}

Значение КлассНеПреобр::фмт(const Значение& q) const {
	return q;
}

const КлассНеПреобр& NoConvert() {
	return Single<КлассНеПреобр>();
}

Значение КлассОшибкаПреобр::скан(const Значение& v) const
{
	return значОш();
}

const КлассОшибкаПреобр& ошибкаПреобр()
{
	return Single<КлассОшибкаПреобр>();
}

Значение ПреобрСоед::фмт(const Значение& v) const {
	Ткст r;
	МассивЗнач a = v;
	for(int i = 0; i < элт.дайСчёт(); i++) {
		const Элемент& m = элт[i];
		if(m.pos < 0)
			r << m.text;
		else
			r << (Ткст) стдПреобр().фмт(m.convert->фмт(a[m.pos]));
	}
	return r;
}

int ПреобрСоед::следщПоз() const {
	for(int i = элт.дайСчёт() - 1; i >= 0; i--)
		if(элт[i].pos >= 0) return элт[i].pos + 1;
	return 0;
}

ПреобрСоед& ПреобрСоед::добавь(const char *text) {
	Элемент& m = элт.добавь();
	m.pos = -1;
	m.text = text;
	return *this;
}

ПреобрСоед& ПреобрСоед::добавь(int pos, const Преобр& cv) {
	ПРОВЕРЬ(pos >= 0);
	Элемент& m = элт.добавь();
	m.pos = pos;
	m.convert = &cv;
	return *this;
}

ПреобрСоед& ПреобрСоед::добавь(int pos) {
	добавь(pos, стдПреобр());
	return *this;
}

ПреобрСоед& ПреобрСоед::добавь(const Преобр& cv) {
	добавь(следщПоз(), cv);
	return *this;
}

ПреобрСоед& ПреобрСоед::добавь() {
	добавь(следщПоз());
	return *this;
}

Значение ПреобрФормата::фмт(const Значение& v) const
{
	МассивЗнач va;
	if(массивЗнач_ли(v))
		va = v;
	else
		va.добавь(v);
	return РНЦП::фмт(формат, va.дай());
}

}
