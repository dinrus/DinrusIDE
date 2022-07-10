unsigned      stou(const char *ptr, void *endptr = NULL, unsigned radix = 10);
inline unsigned stou(const byte *ptr, void *endptr = NULL, unsigned radix = 10) { return stou((const char *)ptr, endptr, radix); }
unsigned      stou(const wchar *ptr, void *endptr = NULL, unsigned radix = 10);

uint64        stou64(const char *s, void *endptr = NULL, unsigned radix = 10);
uint64        stou64(const wchar *s, void *endptr = NULL, unsigned radix = 10);

#include "Convert.hpp"

int           сканЦел(const wchar *ptr, const wchar **endptr = NULL, int radix = 10);

int           сканЦел(const char *ptr, const char **endptr, int radix);
int           сканЦел(const char *ptr, const char **endptr);
int           сканЦел(const char *ptr);

int64         сканЦел64(const char *ptr, const char **endptr, int radix);
int64         сканЦел64(const char *ptr, const char **endptr);
int64         сканЦел64(const char *ptr);

double        сканДво(const char *ptr, const char **endptr, bool accept_comma);
double        сканДво(const wchar *ptr, const wchar **endptr, bool accept_comma);
double        сканДво(const char *ptr, const char **endptr);
double        сканДво(const wchar *ptr, const wchar **endptr);
double        сканДво(const char *ptr);
double        сканДво(const wchar *ptr);

double        Atof(const char *s);

Значение       тктЦелЗнач(const char *s);

inline int     тктЦел(const char* s)   { return сканЦел(s); }
inline Ткст    целТкт(int i)           { return фмтЦел(i); }

inline int64   тктЦел64(const char *s) { return сканЦел64(s); }
inline Ткст    целТкт64(int64 i)       { return фмтЦел64(i); }

inline double  тктДво(const char* s)   { return сканДво(s); }
inline Ткст    двоТкт(double d)        { return пусто_ли(d) ? Ткст() : фмтДво(d); }

inline double  целДво(int i)           { return пусто_ли(i) ? double(Null) : double(i); }
inline int     двоЦел(double d)        { return пусто_ли(d) ? int(Null) : fround(d); }

Значение       тктДвоЗнач(const char* s);


Значение ОшибкаНеПусто();

class Преобр {
public:
	Преобр();
	virtual ~Преобр();

	virtual Значение  фмт(const Значение& q) const;
	virtual Значение  скан(const Значение& text) const;
	virtual int    фильтруй(int chr) const;

	Значение operator()(const Значение& q) const              { return фмт(q); }
};

const Преобр& стдПреобр();

Ткст стдФормат(const Значение& q);

class ПреобрЦел : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;
	virtual int   фильтруй(int chr) const;

protected:
	int64 minval, maxval;
	bool  notnull;

public:
	ПреобрЦел& минмакс(int _min, int _max)        { minval = _min; maxval = _max; return *this; }
	ПреобрЦел& мин(int _min)                     { minval = _min; return *this; }
	ПреобрЦел& макс(int _max)                     { maxval = _max; return *this; }
	ПреобрЦел& неПусто(bool b = true)            { notnull = b; return *this; }
	ПреобрЦел& безНеПусто()                       { return неПусто(false); }
	int         дайМин() const                    { return (int)minval; }
	int         дайМакс() const                    { return (int)maxval; }
	bool        неПусто_ли() const                 { return notnull; }
	
	static int  дайДефМин()                   { return -INT_MAX; }
	static int  дайДефМакс()                   { return INT_MAX; }

	ПреобрЦел(int minval = -INT_MAX, int maxval = INT_MAX, bool notnull = false)
		: minval(minval), maxval(maxval), notnull(notnull) {}
};

const ПреобрЦел& стдПреобрЦел();
const ПреобрЦел& стдПреобрЦелНеПусто();

struct ПреобрЦел64 : public ПреобрЦел {
	ПреобрЦел64& минмакс(int64 _min, int64 _max)    { minval = _min; maxval = _max; return *this; }
	ПреобрЦел64& мин(int64 _min)                   { minval = _min; return *this; }
	ПреобрЦел64& макс(int64 _max)                   { maxval = _max; return *this; }
	int64         дайМин() const                    { return minval; }
	int64         дайМакс() const                    { return maxval; }

	static int64  дайДефМин()                   { return -INT64_MAX; }
	static int64  дайДефМакс()                   { return INT64_MAX; }

	ПреобрЦел64(int64 minval = -INT64_MAX, int64 maxval = INT64_MAX, bool notnull = false) {
		минмакс(minval, maxval); неПусто(notnull);
	}
};

class ПреобрДво : public Преобр {
public:
	virtual Значение фмт(const Значение& q) const;
	virtual Значение скан(const Значение& text) const;
	virtual int   фильтруй(int chr) const;

protected:
	double      minval, maxval;
	bool        notnull, comma;
	Ткст      pattern;

public:
	ПреобрДво& образец(const char *p);
	ПреобрДво& минмакс(double _min, double _max)  { minval = _min; maxval = _max; return *this; }
	ПреобрДво& мин(double _min)                  { minval = _min; return *this; }
	ПреобрДво& макс(double _max)                  { maxval = _max; return *this; }
	ПреобрДво& неПусто(bool b = true)            { notnull = b; return *this; }
	ПреобрДво& безНеПусто()                       { return неПусто(false); }
	double         дайМин() const                    { return minval; }
	double         дайМакс() const                    { return maxval; }
	bool           неПусто_ли() const                 { return notnull; }

	static double  дайДефМин()                   { return -std::numeric_limits<double>::max(); }
	static double  дайДефМакс()                   { return std::numeric_limits<double>::max(); }

	ПреобрДво(double minval = дайДефМин(), double maxval = дайДефМакс(),
		          bool notnull = false);
};

const ПреобрДво& стдПреобрДво();
const ПреобрДво& стдПреобрДвоНеПусто();

class ПреобрДату : public Преобр {
public:
	virtual Значение фмт(const Значение& q) const;
	virtual Значение скан(const Значение& text) const;
	virtual int   фильтруй(int chr) const;

protected:
	Дата minval, maxval, defaultval;
	bool notnull;
	
	static Дата& default_min();
	static Дата& default_max();

public:
	ПреобрДату& минмакс(Дата _min, Дата _max)      { minval = _min; maxval = _max; return *this; }
	ПреобрДату& мин(Дата _min)                    { minval = _min; return *this; }
	ПреобрДату& макс(Дата _max)                    { maxval = _max; return *this; }
	ПреобрДату& неПусто(bool b = true)            { notnull = b; return *this; }
	ПреобрДату& безНеПусто()                       { return неПусто(false); }
	ПреобрДату& дефолт(Дата d)                   { defaultval = d; return *this; }
	bool         неПусто_ли() const                 { return notnull; }
	Дата         дайМин() const                    { return max(дайДефМин(), minval); }
	Дата         дайМакс() const                    { return min(дайДефМакс(), maxval); }

	static void  устДефМинМакс(Дата min, Дата max);
	static Дата  дайДефМин()                   { return default_min(); }
	static Дата  дайДефМакс()                   { return default_max(); }

	ПреобрДату(Дата minval = Дата::наименьш(), Дата maxval = Дата::наибольш(), bool notnull = false);
};

const ПреобрДату& стдПреобрДату();
const ПреобрДату& стдПреобрДатуНеПусто();

class ПреобрВремя : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;
	virtual int   фильтруй(int chr) const;
	virtual Значение фмт(const Значение& q) const;

protected:
	Время minval, maxval, defaultval;
	bool notnull;
	bool seconds;
	bool timealways;
	bool dayend;

public:
	ПреобрВремя& минмакс(Время _min, Время _max)      { minval = _min; maxval = _max; return *this; }
	ПреобрВремя& мин(Время _min)                    { minval = _min; return *this; }
	ПреобрВремя& макс(Время _max)                    { maxval = _max; return *this; }
	ПреобрВремя& неПусто(bool b = true)            { notnull = b; return *this; }
	ПреобрВремя& безНеПусто()                       { return неПусто(false); }
	ПреобрВремя& секунды(bool b = true)            { seconds = b; return *this; }
	ПреобрВремя& безСекунд()                       { return секунды(false); }
	bool         секунды_ли() const                 { return seconds; }
	ПреобрВремя& времяВсегда(bool b = true)         { timealways = b; return *this; }
	bool         времяВсегда_ли() const              { return timealways; }
	ПреобрВремя& конецДня(bool b = true)             { dayend = b; return *this; }
	bool         конецДня_ли() const                  { return dayend; }
	ПреобрВремя& дефолт(Время d)                   { defaultval = d; return *this; }
	bool         неПусто_ли() const                 { return notnull; }

	Время         дайМин() const                    { return max(minval, дайДефМин()); }
	Время         дайМакс() const                    { return min(maxval, дайДефМакс()); }

	static Время  дайДефМин()                   { return воВремя(ПреобрДату::дайДефМин()); }
	static Время  дайДефМакс()                   { return воВремя(ПреобрДату::дайДефМакс()); }

	ПреобрВремя(Время minval = воВремя(Дата::наименьш()), Время maxval = воВремя(Дата::наибольш()), bool notnull = false);
	virtual ~ПреобрВремя();
};

const ПреобрВремя& стдПреобрВремя();
const ПреобрВремя& стдПреобрВремяНеПусто();

class ПреобрТкст : public Преобр {
public:
	virtual Значение скан(const Значение& text) const;

protected:
	int  maxlen;
	bool notnull, trimleft, trimright;

public:
	ПреобрТкст& максдлин(int _maxlen)             { maxlen = _maxlen; return *this; }
	int            дайМаксДлин() const            { return maxlen; }
	ПреобрТкст& неПусто(bool b = true)          { notnull = b; return *this; }
	ПреобрТкст& безНеПусто()                     { return неПусто(false); }
	bool           неПусто_ли() const               { return notnull; }
	ПреобрТкст& обрежьЛево(bool b = true)         { trimleft = b; return *this; }
	ПреобрТкст& обрежьПраво(bool b = true)        { trimright = b; return *this; }
	ПреобрТкст& обрежьОба(bool b = true)         { return обрежьЛево(b).обрежьПраво(b); }
	bool           обрежьЛево_ли() const              { return trimleft; }
	bool           обрежьПраво_ли() const             { return trimright; }

	ПреобрТкст(int maxlen = INT_MAX, bool notnull = false)
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
	void         очисть()                                 { map.очисть(); }
	МапПреобр&  добавь(const Значение& a, const Значение& b)     { map.добавь(a, b); return *this; }
	МапПреобр&  дефолт(const Значение& v)                 { default_value = v; return *this; }

	int          дайСчёт() const                        { return map.дайСчёт(); }
	int          найди(const Значение& v) const              { return map.найди(v); }
	const Значение& дайКлюч(int i) const                     { return map.дайКлюч(i); }
	const Значение& дайЗначение(int i) const                   { return map[i]; }
	const Значение& operator[](int i) const                 { return map[i]; }

	virtual ~МапПреобр() {}
};

class ПреобрСоед : public Преобр {
public:
	virtual Значение фмт(const Значение& v) const;

protected:
	struct Элемент {
		int            pos;
		const Преобр *convert;
		Ткст         text;
	};
	Массив<Элемент> элт;

	int следщПоз() const;

public:
	ПреобрСоед& добавь(const char *text);
	ПреобрСоед& добавь(int pos, const Преобр& cv);
	ПреобрСоед& добавь(int pos);
	ПреобрСоед& добавь(const Преобр& cv);
	ПреобрСоед& добавь();
};

class ПреобрФормата : public Преобр {
public:
	virtual Значение фмт(const Значение& v) const;

private:
	Ткст формат;

public:
	void   устФормат(const char *fmt)           { формат = fmt; }
};

Преобр& преобрЯЗ();

template <typename F, typename S, class R>
struct КлассЛямбдаПреобр : Преобр {
	Функция<Значение(const Значение& w)> формат;
	Функция<Значение(const Значение& text)> scan;
	Функция<int(int)> filter;

	virtual Значение фмт(const Значение& q) const { return формат(q); }
	virtual Значение скан(const Значение& text) const { return scan(text); }
	virtual int фильтруй(int chr) const { return filter(chr); }
	
	КлассЛямбдаПреобр(F формат, S scan, R filter) : формат(формат), scan(scan), filter(filter) {}
};

template <typename F, typename S, class R>
const auto& лямбдаПреобр(F формат, S scan, R filter)
{
	static КлассЛямбдаПреобр<F, S, R> x(формат, scan, filter);
	return x;
}

template <typename F, typename S>
const auto& лямбдаПреобр(F формат, S scan)
{
	return лямбдаПреобр(формат, scan, [](int ch) { return ch; });
}

template <typename F>
const auto& лямбдаПреобр(F формат)
{
	return лямбдаПреобр(формат, [](const Значение& v) { return v; });
}
