unsigned      stou(кткст0 укз, ук endptr = NULL, unsigned radix = 10);
inline unsigned stou(const ббайт *укз, ук endptr = NULL, unsigned radix = 10) { return stou((const char *)укз, endptr, radix); }
unsigned      stou(const шим *укз, ук endptr = NULL, unsigned radix = 10);

бдол        stou64(кткст0 s, ук endptr = NULL, unsigned radix = 10);
бдол        stou64(const шим *s, ук endptr = NULL, unsigned radix = 10);

#include "Convert.hpp"

цел           сканЦел(const шим *укз, const шим **endptr = NULL, цел radix = 10);

цел           сканЦел(кткст0 укз, const char **endptr, цел radix);
цел           сканЦел(кткст0 укз, const char **endptr);
цел           сканЦел(кткст0 укз);

дол         сканЦел64(кткст0 укз, const char **endptr, цел radix);
дол         сканЦел64(кткст0 укз, const char **endptr);
дол         сканЦел64(кткст0 укз);

дво        сканДво(кткст0 укз, const char **endptr, бул accept_comma);
дво        сканДво(const шим *укз, const шим **endptr, бул accept_comma);
дво        сканДво(кткст0 укз, const char **endptr);
дво        сканДво(const шим *укз, const шим **endptr);
дво        сканДво(кткст0 укз);
дво        сканДво(const шим *укз);

дво        Atof(кткст0 s);

Значение       тктЦелЗнач(кткст0 s);

inline цел     тктЦел(const char* s)   { return сканЦел(s); }
inline Ткст    целТкт(цел i)           { return фмтЦел(i); }

inline дол   тктЦел64(кткст0 s) { return сканЦел64(s); }
inline Ткст    целТкт64(дол i)       { return фмтЦел64(i); }

inline дво  тктДво(const char* s)   { return сканДво(s); }
inline Ткст    двоТкт(дво d)        { return пусто_ли(d) ? Ткст() : фмтДво(d); }

inline дво  целДво(цел i)           { return пусто_ли(i) ? дво(Null) : дво(i); }
inline цел     двоЦел(дво d)        { return пусто_ли(d) ? цел(Null) : fround(d); }

Значение       тктДвоЗнач(const char* s);


Значение ОшибкаНеПусто();

class Преобр {
public:
	Преобр();
	virtual ~Преобр();

	virtual Значение  фмт(const Значение& q) const;
	virtual Значение  скан(const Значение& text) const;
	virtual цел    фильтруй(цел chr) const;

	Значение operator()(const Значение& q) const              { return фмт(q); }
};

const Преобр& стдПреобр();

Ткст стдФормат(const Значение& q);

class ПреобрЦел : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;

protected:
	дол минзнач, максзнач;
	бул  notnull;

public:
	ПреобрЦел& минмакс(цел _min, цел _max)        { минзнач = _min; максзнач = _max; return *this; }
	ПреобрЦел& мин(цел _min)                     { минзнач = _min; return *this; }
	ПреобрЦел& макс(цел _max)                     { максзнач = _max; return *this; }
	ПреобрЦел& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрЦел& безНеПусто()                       { return неПусто(false); }
	цел         дайМин() const                    { return (цел)минзнач; }
	цел         дайМакс() const                    { return (цел)максзнач; }
	бул        неПусто_ли() const                 { return notnull; }
	
	static цел  дайДефМин()                   { return -INT_MAX; }
	static цел  дайДефМакс()                   { return INT_MAX; }

	ПреобрЦел(цел минзнач = -INT_MAX, цел максзнач = INT_MAX, бул notnull = false)
		: минзнач(минзнач), максзнач(максзнач), notnull(notnull) {}
};

const ПреобрЦел& стдПреобрЦел();
const ПреобрЦел& стдПреобрЦелНеПусто();

struct ПреобрЦел64 : public ПреобрЦел {
	ПреобрЦел64& минмакс(дол _min, дол _max)    { минзнач = _min; максзнач = _max; return *this; }
	ПреобрЦел64& мин(дол _min)                   { минзнач = _min; return *this; }
	ПреобрЦел64& макс(дол _max)                   { максзнач = _max; return *this; }
	дол         дайМин() const                    { return минзнач; }
	дол         дайМакс() const                    { return максзнач; }

	static дол  дайДефМин()                   { return -INT64_MAX; }
	static дол  дайДефМакс()                   { return INT64_MAX; }

	ПреобрЦел64(дол минзнач = -INT64_MAX, дол максзнач = INT64_MAX, бул notnull = false) {
		минмакс(минзнач, максзнач); неПусто(notnull);
	}
};

class ПреобрДво : public Преобр {
public:
	virtual Значение фмт(const Значение& q) const;
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;

protected:
	дво      минзнач, максзнач;
	бул        notnull, comma;
	Ткст      pattern;

public:
	ПреобрДво& образец(кткст0 p);
	ПреобрДво& минмакс(дво _min, дво _max)  { минзнач = _min; максзнач = _max; return *this; }
	ПреобрДво& мин(дво _min)                  { минзнач = _min; return *this; }
	ПреобрДво& макс(дво _max)                  { максзнач = _max; return *this; }
	ПреобрДво& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрДво& безНеПусто()                       { return неПусто(false); }
	дво         дайМин() const                    { return минзнач; }
	дво         дайМакс() const                    { return максзнач; }
	бул           неПусто_ли() const                 { return notnull; }

	static дво  дайДефМин()                   { return -std::numeric_limits<дво>::макс(); }
	static дво  дайДефМакс()                   { return std::numeric_limits<дво>::макс(); }

	ПреобрДво(дво минзнач = дайДефМин(), дво максзнач = дайДефМакс(),
		          бул notnull = false);
};

const ПреобрДво& стдПреобрДво();
const ПреобрДво& стдПреобрДвоНеПусто();

class ПреобрДату : public Преобр {
public:
	virtual Значение фмт(const Значение& q) const;
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;

protected:
	Дата минзнач, максзнач, defaultval;
	бул notnull;
	
	static Дата& default_min();
	static Дата& default_max();

public:
	ПреобрДату& минмакс(Дата _min, Дата _max)      { минзнач = _min; максзнач = _max; return *this; }
	ПреобрДату& мин(Дата _min)                    { минзнач = _min; return *this; }
	ПреобрДату& макс(Дата _max)                    { максзнач = _max; return *this; }
	ПреобрДату& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрДату& безНеПусто()                       { return неПусто(false); }
	ПреобрДату& дефолт(Дата d)                   { defaultval = d; return *this; }
	бул         неПусто_ли() const                 { return notnull; }
	Дата         дайМин() const                    { return макс(дайДефМин(), минзнач); }
	Дата         дайМакс() const                    { return мин(дайДефМакс(), максзнач); }

	static проц  устДефМинМакс(Дата мин, Дата макс);
	static Дата  дайДефМин()                   { return default_min(); }
	static Дата  дайДефМакс()                   { return default_max(); }

	ПреобрДату(Дата минзнач = Дата::наименьш(), Дата максзнач = Дата::наибольш(), бул notnull = false);
};

const ПреобрДату& стдПреобрДату();
const ПреобрДату& стдПреобрДатуНеПусто();

class ПреобрВремя : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;
	virtual цел   фильтруй(цел chr) const;
	virtual Значение фмт(const Значение& q) const;

protected:
	Время минзнач, максзнач, defaultval;
	бул notnull;
	бул seconds;
	бул timealways;
	бул dayend;

public:
	ПреобрВремя& минмакс(Время _min, Время _max)      { минзнач = _min; максзнач = _max; return *this; }
	ПреобрВремя& мин(Время _min)                    { минзнач = _min; return *this; }
	ПреобрВремя& макс(Время _max)                    { максзнач = _max; return *this; }
	ПреобрВремя& неПусто(бул b = true)            { notnull = b; return *this; }
	ПреобрВремя& безНеПусто()                       { return неПусто(false); }
	ПреобрВремя& секунды(бул b = true)            { seconds = b; return *this; }
	ПреобрВремя& безСекунд()                       { return секунды(false); }
	бул         секунды_ли() const                 { return seconds; }
	ПреобрВремя& времяВсегда(бул b = true)         { timealways = b; return *this; }
	бул         времяВсегда_ли() const              { return timealways; }
	ПреобрВремя& конецДня(бул b = true)             { dayend = b; return *this; }
	бул         конецДня_ли() const                  { return dayend; }
	ПреобрВремя& дефолт(Время d)                   { defaultval = d; return *this; }
	бул         неПусто_ли() const                 { return notnull; }

	Время         дайМин() const                    { return макс(минзнач, дайДефМин()); }
	Время         дайМакс() const                    { return мин(максзнач, дайДефМакс()); }

	static Время  дайДефМин()                   { return воВремя(ПреобрДату::дайДефМин()); }
	static Время  дайДефМакс()                   { return воВремя(ПреобрДату::дайДефМакс()); }

	ПреобрВремя(Время минзнач = воВремя(Дата::наименьш()), Время максзнач = воВремя(Дата::наибольш()), бул notnull = false);
	virtual ~ПреобрВремя();
};

const ПреобрВремя& стдПреобрВремя();
const ПреобрВремя& стдПреобрВремяНеПусто();

class ПреобрТкст : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;

protected:
	цел  maxlen;
	бул notnull, trimleft, trimright;

public:
	ПреобрТкст& максдлин(цел _maxlen)             { maxlen = _maxlen; return *this; }
	цел            дайМаксДлин() const            { return maxlen; }
	ПреобрТкст& неПусто(бул b = true)          { notnull = b; return *this; }
	ПреобрТкст& безНеПусто()                     { return неПусто(false); }
	бул           неПусто_ли() const               { return notnull; }
	ПреобрТкст& обрежьЛево(бул b = true)         { trimleft = b; return *this; }
	ПреобрТкст& обрежьПраво(бул b = true)        { trimright = b; return *this; }
	ПреобрТкст& обрежьОба(бул b = true)         { return обрежьЛево(b).обрежьПраво(b); }
	бул           обрежьЛево_ли() const              { return trimleft; }
	бул           обрежьПраво_ли() const             { return trimright; }

	ПреобрТкст(цел maxlen = INT_MAX, бул notnull = false)
		: maxlen(maxlen), notnull(notnull) { trimleft = trimright = false; }
};

const ПреобрТкст& стдПреобрТкст();
const ПреобрТкст& стдПреобрТкстНеПусто();

class КлассНеПреобр : public Преобр {
public:
	КлассНеПреобр();

	virtual Значение  фмт(const Значение& q) const;
};

const КлассНеПреобр& NoConvert();

class КлассОшибкаПреобр : public Преобр {
public:
	Значение скан(const Значение& v) const;
};

const КлассОшибкаПреобр& ошибкаПреобр();

class МапПреобр : public Преобр {
public:
	virtual Значение  фмт(const Значение& q) const;

protected:
	ВекторМап<Значение, Значение> map;
	Значение                   default_value;

public:
	проц         очисть()                                 { map.очисть(); }
	МапПреобр&  добавь(const Значение& a, const Значение& b)     { map.добавь(a, b); return *this; }
	МапПреобр&  дефолт(const Значение& v)                 { default_value = v; return *this; }

	цел          дайСчёт() const                        { return map.дайСчёт(); }
	цел          найди(const Значение& v) const              { return map.найди(v); }
	const Значение& дайКлюч(цел i) const                     { return map.дайКлюч(i); }
	const Значение& дайЗначение(цел i) const                   { return map[i]; }
	const Значение& operator[](цел i) const                 { return map[i]; }

	virtual ~МапПреобр() {}
};

class ПреобрСоед : public Преобр {
public:
	virtual Значение фмт(const Значение& v) const;

protected:
	struct Элемент {
		цел            pos;
		const Преобр *convert;
		Ткст         text;
	};
	Массив<Элемент> элт;

	цел следщПоз() const;

public:
	ПреобрСоед& добавь(кткст0 text);
	ПреобрСоед& добавь(цел pos, const Преобр& cv);
	ПреобрСоед& добавь(цел pos);
	ПреобрСоед& добавь(const Преобр& cv);
	ПреобрСоед& добавь();
};

class ПреобрФормата : public Преобр {
public:
	virtual Значение фмт(const Значение& v) const;

private:
	Ткст формат;

public:
	проц   устФормат(кткст0 fmt)           { формат = fmt; }
};

Преобр& преобрЯЗ();

template <typename F, typename S, class R>
struct КлассЛямбдаПреобр : Преобр {
	Функция<Значение(const Значение& w)> формат;
	Функция<Значение(const Значение& text)> scan;
	Функция<цел(цел)> фильтр;

	virtual Значение фмт(const Значение& q) const { return формат(q); }
	virtual Значение скан(const Значение& text) const { return scan(text); }
	virtual цел фильтруй(цел chr) const { return фильтр(chr); }
	
	КлассЛямбдаПреобр(F формат, S scan, R фильтр) : формат(формат), scan(scan), фильтр(фильтр) {}
};

template <typename F, typename S, class R>
const auto& лямбдаПреобр(F формат, S scan, R фильтр)
{
	static КлассЛямбдаПреобр<F, S, R> x(формат, scan, фильтр);
	return x;
}

template <typename F, typename S>
const auto& лямбдаПреобр(F формат, S scan)
{
	return лямбдаПреобр(формат, scan, [](цел ch) { return ch; });
}

template <typename F>
const auto& лямбдаПреобр(F формат)
{
	return лямбдаПреобр(формат, [](const Значение& v) { return v; });
}
