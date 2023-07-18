#include <DinrusPro/DinrusCore.h>

дво ipow10_table[601];

unsigned stou(кткст0 s, ук endptr, unsigned base)
{
	ПРОВЕРЬ(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // Ошибка
		if(endptr)
			*(кткст0 *)endptr = s;
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
		*(кткст0 *)endptr = s;
	return значение;
}

unsigned stou(const шим *s, ук endptr, unsigned base)
{
	ПРОВЕРЬ(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // Ошибка
		if(endptr)
			*(const шим **)endptr = s;
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
		*(const шим **)endptr = s;
	return значение;
}


бдол stou64(кткст0 s, ук endptr, unsigned base)
{
	ПРОВЕРЬ(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // Ошибка
		if(endptr)
			*(кткст0 *)endptr = s;
		return ~0;
	}
	бдол значение = digit;
	while((digit = ctoi(*++s)) < base) {
		бдол v0 = значение;
		значение = значение * base + digit;
		if(v0 > значение) // overflow
			return ~0;
	}
	if(endptr)
		*(кткст0 *)endptr = s;
	return значение;
}

цел сканЦел(кткст0 укз, кткст0 *endptr, цел base)
{
	кткст0 s = укз;
	бул minus = false;
	while(*s && (ббайт)*s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	unsigned u = stou(s, endptr, base);
	if(~u)
		return (minus ? -(цел)u : (цел)u);
	else
		return Null;
}

цел сканЦел(const шим *укз, const шим **endptr, цел base)
{
	const шим *s = укз;
	бул minus = false;
	while(*s && *s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	unsigned u = stou(s, endptr, base);
	if(~u)
		return (minus ? -(цел)u : (цел)u);
	else
		return Null;
}

дол сканЦел64(кткст0 укз, кткст0 *endptr, цел base)
{
	кткст0 s = укз;
	бул minus = false;
	while(*s && *s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	бдол u = stou64(s, endptr, base);
	if(~u)
		return (minus ? -(дол)u : (дол)u);
	else
		return Null;
}

цел сканЦел(кткст0 укз, кткст0 *endptr)
{
	цел x;
	бул overflow = false;
	укз = сканЦел<сим, ббайт, бцел, цел, 10>(x, укз, overflow);
	if(укз && endptr)
		*endptr = укз;
	return !overflow && укз ? x : Null;
}

цел сканЦел(кткст0 укз)
{
	цел x;
	бул overflow = false;
	return сканЦел<сим, ббайт, бцел, цел, 10>(x, укз, overflow) && !overflow ? x : Null;
}

дол сканЦел64(кткст0 укз, кткст0 *endptr)
{
	дол x;
	бул overflow = false;
	укз = сканЦел<сим, ббайт, бдол, дол, 10>(x, укз, overflow);
	if(укз && endptr)
		*endptr = укз;
	return !overflow && укз ? x : Null;
}

дол сканЦел64(кткст0 укз)
{
	дол x;
	бул overflow = false;
	return сканЦел<сим, ббайт, бдол, дол, 10>(x, укз, overflow) && !overflow ? x : Null;
}

Значение тктЦелЗнач(кткст0 s)
{
	if(s && *s) {
		кткст0 p;
		дол q = сканЦел64(s, &p);
		if(!пусто_ли(q))
			while(*p) {
				if((ббайт)*p > ' ')
					return значОш(t_("Неверное число !"));
				p++;
			}
		return пусто_ли(q) ? значОш(t_("Неверное число !")) : Значение(q);
	}
	return (цел)Null;
}

Значение тктДвоЗнач(кткст0 s)
{
	if(s && *s) {
		кткст0 p;
		дво q = сканДво(s, &p);
		if(!пусто_ли(q))
			while(*p) {
				if((ббайт)*p > ' ')
					return значОш(t_("Неверное число !"));
				p++;
			}
		return пусто_ли(q) ? значОш(t_("Неверное число !")) : Значение(q);
	}
	return (дво)Null;
}

Значение скан(бцел qtype, const Ткст& text, const Значение& опр, бул *hastime) {
	Дата date;
	кткст0 s;
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
				цел q = p.читайЦел();
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
		return целТкт64((дол)q);
	case INT_V:
	case BOOL_V:
		return целТкт((цел)q);
	case DOUBLE_V:
		return двоТкт((дво)q);
	case DATE_V:
		return фмт(Дата(q));
	case TIME_V:
		return фмт(Время(q));
	case STRING_V:
	case WSTRING_V:
		return q;
	}
	return q.вТкст();
}

Значение  Преобр::скан(const Значение& text) const {
	return text;
};

цел    Преобр::фильтруй(цел chr) const {
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
	Значение v = скан(INT_V, text);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	дол m = v;
	if(m >= минзнач && m <= максзнач) {
		if(m >= INT_MIN && m <= INT_MAX)
			return (цел)m;
		else
			return v;
	}
	return значОш(фмт(t_("Number must be between %d and %d."), минзнач, максзнач));
}

цел   ПреобрЦел::фильтруй(цел chr) const {
	return минзнач >= 0 ? СимФильтрЦифра(chr) : СимФильтрЦел(chr);
}

Значение ПреобрДво::фмт(const Значение& q) const
{
	if(пусто_ли(q))
		return Null;
	return фмт(pattern, (дво)q);
}

Значение ПреобрДво::скан(const Значение& txt) const {
	Ткст text = txt;
	if(pattern.дайСчёт() && pattern != "%.10g") { // фиксируй text with patterns like "%2.!n EUR" (e.g. 1.2 EUR)
		text = фильтруй(text, СимФильтрДво);
		while(взаг(*text.последний()) == 'E')
			text.обрежь(text.дайСчёт() - 1);
	}
	Значение v = скан(DOUBLE_V, text);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	дво m = v;
	if(m >= минзнач && m <= максзнач) return v;
	return значОш(фмт(t_("Number must be between %g and %g."), минзнач, максзнач));
}

цел   ПреобрДво::фильтруй(цел chr) const {
	chr = СимФильтрДво(chr);
	return comma && chr == '.' ? ',' : chr;
}

ПреобрДво::ПреобрДво(дво минзнач, дво максзнач, бул notnull)
  : минзнач(минзнач), максзнач(максзнач), notnull(notnull)
{
	pattern = "%.10g";
	comma = false;
}

ПреобрДво& ПреобрДво::образец(кткст0 p)
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

проц ПреобрДату::устДефМинМакс(Дата мин, Дата макс)
{
	default_min() = мин;
	default_max() = макс;
}

ПреобрДату::ПреобрДату(Дата минзнач, Дата максзнач, бул notnull)
: минзнач(минзнач), максзнач(максзнач), notnull(notnull) {
	defaultval = Null;
}

Значение ПреобрДату::фмт(const Значение& q) const
{
	if(датаВремя_ли(q))
		return Преобр::фмт((Дата)q);
	return Преобр::фмт(q);
}

Значение ПреобрДату::скан(const Значение& text) const {
	Значение v = скан(DATE_V, text, defaultval);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	Дата m = v;
	Дата минзнач = дайМин();
	Дата максзнач = дайМакс();
	if(m >= минзнач && m <= максзнач) return v;
	return значОш(t_("Дата должна быть между ") + фмт(минзнач) + t_("диапазоном\v и ") + фмт(максзнач) + ".");
}

цел   ПреобрДату::фильтруй(цел chr) const {
	return CharFilterDate(chr);
}

ПреобрВремя::ПреобрВремя(Время минзнач, Время максзнач, бул notnull)
: минзнач(минзнач), максзнач(максзнач), notnull(notnull), seconds(true) {
	defaultval = Null;
	timealways = false;
	dayend = false;
}

ПреобрВремя::~ПреобрВремя()
{
}

Значение ПреобрВремя::скан(const Значение& text) const
{
	бул hastime;
	Значение v = скан(TIME_V, text, defaultval, &hastime);
	if(ошибка_ли(v)) return v;
	if(пусто_ли(v)) return notnull ? ОшибкаНеПусто() : v;
	Время m = v;
	if(!hastime && dayend) {
		m.hour = 23;
		m.minute = 59;
		m.second = 59;
		v = m;
	}
	Время минзнач = дайМин();
	Время максзнач = дайМакс();
	if(m >= минзнач && m <= максзнач) return v;
	return значОш(t_("Время must be between ") + фмт(минзнач) + t_("range\v and ") + фмт(максзнач) + ".");
}

цел ПреобрВремя::фильтруй(цел chr) const
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
		return воВремя((Дата)q) != (Время)q || timealways ? фмт(Время(q), seconds) : фмт(Дата(q));
	else
		return Преобр::фмт(q);
}

Значение ПреобрТкст::скан(const Значение& text) const {
	if(ошибка_ли(text)) return text;
	if(text.дайТип() == STRING_V) {
		Ткст s = text;
		if(trimleft)
			s = обрежьЛево(s);
		if(trimright)
			s = обрежьПраво(s);
		if(пусто_ли(s)) return notnull ? ОшибкаНеПусто() : Значение(s);
		if(s.дайДлину() <= maxlen) return s;
	}
	if(text.дайТип() == WSTRING_V) {
		ШТкст s = text;
		if(trimleft)
			s = обрежьЛево(s);
		if(trimright)
			s = обрежьПраво(s);
		if(пусто_ли(s)) return notnull ? ОшибкаНеПусто() : Значение(s);
		if(s.дайДлину() <= maxlen) return s;
	}
	return значОш(фмт(t_("Please enter no more than %d characters."), maxlen));
}

const ПреобрЦел& стдПреобрЦел() { static ПреобрЦел h; return h; }
const ПреобрЦел& стдПреобрЦелНеПусто() { static ПреобрЦел h(-INT_MAX, INT_MAX, true); return h; }

const ПреобрДво& стдПреобрДво() { static ПреобрДво h; return h; }
const ПреобрДво& стдПреобрДвоНеПусто() { static ПреобрДво h(-std::numeric_limits<дво>::макс(), std::numeric_limits<дво>::макс(), true); return h; }

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
	return Сингл<КлассНеПреобр>();
}

Значение КлассОшибкаПреобр::скан(const Значение& v) const
{
	return значОш();
}

const КлассОшибкаПреобр& ошибкаПреобр()
{
	return Сингл<КлассОшибкаПреобр>();
}

Значение ПреобрСоед::фмт(const Значение& v) const {
	Ткст r;
	МассивЗнач a = v;
	for(цел i = 0; i < элт.дайСчёт(); i++) {
		const Элемент& m = элт[i];
		if(m.pos < 0)
			r << m.text;
		else
			r << (Ткст) стдПреобр().фмт(m.convert->фмт(a[m.pos]));
	}
	return r;
}

цел ПреобрСоед::следщПоз() const {
	for(цел i = элт.дайСчёт() - 1; i >= 0; i--)
		if(элт[i].pos >= 0) return элт[i].pos + 1;
	return 0;
}

ПреобрСоед& ПреобрСоед::добавь(кткст0 text) {
	Элемент& m = элт.добавь();
	m.pos = -1;
	m.text = text;
	return *this;
}

ПреобрСоед& ПреобрСоед::добавь(цел pos, const Преобр& cv) {
	ПРОВЕРЬ(pos >= 0);
	Элемент& m = элт.добавь();
	m.pos = pos;
	m.convert = &cv;
	return *this;
}

ПреобрСоед& ПреобрСоед::добавь(цел pos) {
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
	return фмт(формат, va.дай());
}
