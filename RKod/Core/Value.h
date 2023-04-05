class   Ид;
class   Значение;
class   МассивЗнач;
class   МапЗнач;
class   РярВВ;
class   ДжейсонВВ;

class   Реф;
struct  РефНаТипЗнач;

template <class T>
void вДжейсон(ДжейсонВВ& io, T& var);

template <class T>
void вРяр(РярВВ& xml, T& var);

const dword VOID_V    = 0;

const dword INT_V     = 1;
const dword DOUBLE_V  = 2;
const dword STRING_V  = 3;
const dword DATE_V    = 4;
const dword TIME_V    = 5;

const dword ERROR_V   = 6;

const dword VALUE_V   = 7;

const dword WSTRING_V = 8;

const dword VALUEARRAY_V = 9;

const dword INT64_V  = 10;
const dword BOOL_V   = 11;

const dword VALUEMAP_V   = 12;

const dword UNKNOWN_V = (dword)0xffffffff;

template <class T>
inline dword номерТипаЗнач(const T *)                 { return StaticTypeNo<T>() + 0x8000000; }

template<> inline dword номерТипаЗнач(const int*)     { return INT_V; }
template<> inline dword номерТипаЗнач(const int64*)   { return INT64_V; }
template<> inline dword номерТипаЗнач(const double*)  { return DOUBLE_V; }
template<> inline dword номерТипаЗнач(const bool*)    { return BOOL_V; }
template<> inline dword номерТипаЗнач(const Ткст*)  { return STRING_V; }
template<> inline dword номерТипаЗнач(const ШТкст*) { return WSTRING_V; }
template<> inline dword номерТипаЗнач(const Дата*)    { return DATE_V; }
template<> inline dword номерТипаЗнач(const Время*)    { return TIME_V; }
template<> inline dword номерТипаЗнач(const Значение*)   { return VALUE_V; }

template <class T, dword тип = UNKNOWN_V, class B = EmptyClass>
class ТипЗнач : public B {
public:
	static dword номерТипаЗнач()                      { return тип == UNKNOWN_V ? StaticTypeNo<T>() + 0x8000000 : тип; }
	friend dword номерТипаЗнач(const T*)              { return T::номерТипаЗнач(); }
	
	bool     экзПусто_ли() const                 { return false; }
	void     сериализуй(Поток& s)                   { НИКОГДА(); }
	void     вРяр(РярВВ& xio)                     { НИКОГДА(); }
	void     вДжейсон(ДжейсонВВ& jio)                   { НИКОГДА(); }
	hash_t   дайХэшЗнач() const                   { return 0; }
	bool     operator==(const T&) const             { НИКОГДА(); return false; }
	Ткст   вТкст() const                       { return typeid(T).name(); }
	int      сравни(const T&) const                { НИКОГДА(); return 0; }
	int      полиСравни(const Значение&) const        { НИКОГДА(); return 0; }
	
	operator РефНаТипЗнач();
};

template <class T, dword тип, class B = EmptyClass> // Backward compatiblity
class ПрисвойНомТипаЗнач : public ТипЗнач<T, тип, B> {};

template <class T>
dword дайНомТипаЗнач() { return номерТипаЗнач((T*)NULL); }

class Значение : Движимое_<Значение> {
public:
	class Проц {
	protected:
		Атомар  refcount;

	public:
		void               Retain()                    { атомнИнк(refcount); }
		void               отпусти()                   { if(атомнДек(refcount) == 0) delete this; }
		int                GetRefCount() const         { return refcount; }

		virtual bool       пусто_ли() const              { return true; }
		virtual void       сериализуй(Поток& s)        {}
		virtual void       вРяр(РярВВ& xio)          {}
		virtual void       вДжейсон(ДжейсонВВ& jio)        {}
		virtual hash_t     дайХэшЗнач() const        { return 0; }
		virtual bool       равен(const Проц *p)      { return false; }
		virtual bool       полиРавны(const Значение& v) { return false; }
		virtual Ткст     какТкст() const            { return ""; }
		virtual int        сравни(const Проц *p)      { return 0; }
		virtual int        полиСравни(const Значение& p) { return 0; }

		Проц()                                         { refcount = 1; }
		virtual ~Проц()                                {}

		friend class Значение;
	};

	struct Sval {
		bool       (*пусто_ли)(const void *p);
		void       (*сериализуй)(void *p, Поток& s);
		void       (*вРяр)(void *p, РярВВ& xio);
		void       (*вДжейсон)(void *p, ДжейсонВВ& jio);
		hash_t     (*дайХэшЗнач)(const void *p);
		bool       (*равен)(const void *p1, const void *p2);
		bool       (*полиРавны)(const void *p, const Значение& v);
		Ткст     (*какТкст)(const void *p);
		int        (*сравни)(const void *p1, const void *p2);
		int        (*полиСравни)(const void *p1, const Значение& p2);
	};
	
protected:
	enum { STRING = 0, REF = 255, VOIDV = 3 };

	static ВекторМап<dword, Проц* (*)()>& Typemap();
	static Sval *svo[256];
	static Индекс<Ткст>& индексИмени();
	static Индекс<dword>&  индексТипа();
	
	static void   добавьИмя(dword тип, const char *имя);
	static int    дайТип(const char *имя);
	static Ткст дайИмя(dword тип);
	static void   регистрируйСтд();

	friend void ValueRegisterHelper();

	Ткст   данные;
	Проц    *&ptr()                  { ПРОВЕРЬ(реф_ли()); return *(Проц **)&данные; }
	Проц     *ptr() const            { ПРОВЕРЬ(реф_ли()); return *(Проц **)&данные; }

	void     устРефТип(dword тип)  { ПРОВЕРЬ(реф_ли()); ((int *)&данные)[2] = тип; }
	dword    дайРефТип() const      { ПРОВЕРЬ(реф_ли()); return ((int *)&данные)[2]; }

	bool     ткст_ли() const          { return !данные.особый_ли(); }
	bool     является(byte v) const          { return данные.особый_ли(v); }
	bool     реф_ли() const             { return является(REF); }
	void     иницРеф(Проц *p, dword t) { данные.устОсобо(REF); ptr() = p; устРефТип(t); }
	void     отпустиРеф();
	void     RefRetain();
	void     освободиРеф()                 { if(реф_ли()) отпустиРеф(); }
	void     освободи()                    { освободиРеф(); данные.очисть(); }
	void     устЛардж(const Значение& v);

	template <class T>
	void     иницСмолл(const T& init);
	template <class T>
	T&       дайСыройСмолл() const;
	template <class T>
	T&       дайСмолл() const;
	
	int      дайДрЦел() const;
	int64    дайДрЦел64() const;
	double   дайДрДво() const;
	bool     дайДрБул() const;
	Дата     дайДрДату() const;
	Время     дайДрВремя() const;
	Ткст   дайДрТкст() const;
	hash_t   дайДрЗначХэш() const;

	bool     полиРавны(const Значение& v) const;
	
	enum VSMALL { SMALL };

	template <class T>
	Значение(const T& значение, VSMALL);
	
	template <class T> friend Значение SvoToValue(const T& x);

	Ткст  дайИмя() const;
	
	int     полиСравни(const Значение& v) const;
	int     сравни2(const Значение& v) const;

	Вектор<Значение>&  UnShareArray();

	const Вектор<Значение>& GetVA() const;
	
#if defined(_ОТЛАДКА) && defined(COMPILER_GCC)
	uint32  magic[4];
	void    Magic()               { magic[0] = 0xc436d851; magic[1] = 0x72f67c76; magic[2] = 0x3e5e10fd; magic[3] = 0xc90d370b; }
	void    ClearMagic()          { magic[0] = magic[1] = magic[2] = magic[3] = 0; }
#else
	void    Magic()               {}
	void    ClearMagic()          {}
#endif

#ifndef DEPRECATED
	static  void регистрируй(dword w, Проц* (*c)(), const char *имя = NULL);
#endif

	Значение(Проц *p) = delete;

public:
	template <class T>
	static  void регистрируй(const char *имя = NULL);
	template <class T>
	static  void SvoRegister(const char *имя = NULL);
	
	dword    дайТип() const;
	bool     ошибка_ли() const         { return дайТип() == ERROR_V; }
	bool     проц_ли() const          { return является(VOIDV) || ошибка_ли(); }
	bool     пусто_ли() const;

	template <class T>
	bool     является() const;
	template <class T>
	const T& To() const;
	template <class T>
	const T& дай() const;

	operator Ткст() const          { return ткст_ли() ? данные : дайДрТкст(); }
	operator ШТкст() const;
	operator Дата() const            { return является(DATE_V) ? дайСыройСмолл<Дата>() : дайДрДату(); }
	operator Время() const            { return является(TIME_V) ? дайСыройСмолл<Время>() : дайДрВремя(); }
	operator double() const          { return является(DOUBLE_V) ? дайСыройСмолл<double>() : дайДрДво(); }
	operator int() const             { return является(INT_V) ? дайСыройСмолл<int>() : дайДрЦел(); }
	operator int64() const           { return является(INT64_V) ? дайСыройСмолл<int64>() : дайДрЦел64(); }
	operator bool() const            { return является(BOOL_V) ? дайСыройСмолл<bool>() : дайДрБул(); }
	std::string  вСтд() const       { return operator Ткст().вСтд(); }
	std::wstring вСтдШ() const      { return operator ШТкст().вСтд(); }
	
	Значение(const Ткст& s) : данные(s) { Magic(); }
	Значение(const ШТкст& s);
	Значение(const char *s) : данные(s)   { Magic(); }
	Значение(int i)                     : данные(i, INT_V, Ткст::SPECIAL) { Magic(); }
	Значение(int64 i)                   : данные(i, INT64_V, Ткст::SPECIAL) { Magic(); }
	Значение(double d)                  : данные(d, DOUBLE_V, Ткст::SPECIAL) { Magic(); }
	Значение(bool b)                    : данные(b, BOOL_V, Ткст::SPECIAL) { Magic(); }
	Значение(Дата d)                    : данные(d, DATE_V, Ткст::SPECIAL) { Magic(); }
	Значение(Время t)                    : данные(t, TIME_V, Ткст::SPECIAL) { Magic(); }
	Значение(const Обнул&)             : данные((int)Null, INT_V, Ткст::SPECIAL) { Magic(); }
	Значение(const std::string& s) : Значение(Ткст(s)) {}
	Значение(const std::wstring& s) : Значение(ШТкст(s)) {}

	bool operator==(const Значение& v) const;
	bool operator!=(const Значение& v) const { return !operator==(v); }
	bool одинаково(const Значение& v) const;
	
	int  сравни(const Значение& v) const;
	bool operator<=(const Значение& x) const { return сравни(x) <= 0; }
	bool operator>=(const Значение& x) const { return сравни(x) >= 0; }
	bool operator<(const Значение& x) const  { return сравни(x) < 0; }
	bool operator>(const Значение& x) const  { return сравни(x) > 0; }

	Ткст вТкст() const;
	Ткст operator ~() const             { return вТкст(); }
	Ткст дайИмяТипа() const            { return дайИмя(); }

	void  сериализуй(Поток& s);
	void  вРяр(РярВВ& xio);
	void  вДжейсон(ДжейсонВВ& jio);

	hash_t дайХэшЗнач() const;

	Значение& operator=(const Значение& v);
	Значение(const Значение& v);
	
	int   дайСчёт() const;
	const Значение& operator[](int i) const;
	const Значение& operator[](const Ткст& ключ) const;
	const Значение& operator[](const char *ключ) const;
	const Значение& operator[](const Ид& ключ) const;
	
	Значение& по(int i);
	Значение& operator()(int i)              { return по(i); }
	void   добавь(const Значение& ист);
	template <typename T>
	Значение& operator<<(const T& ист)       { добавь(ист); return *this; }

	Значение& дайДобавь(const Значение& ключ);
	Значение& operator()(const Ткст& ключ);
	Значение& operator()(const char *ключ);
	Значение& operator()(const Ид& ключ);

	Значение()                               : данные((int)Null, VOIDV, Ткст::SPECIAL) { Magic(); }
	~Значение()                              { ClearMagic(); if(реф_ли()) отпустиРеф(); }

	Значение(Проц *p, dword тип)            { иницРеф(p, тип); Magic(); }
	const Проц *дайПроцУк() const        { ПРОВЕРЬ(реф_ли()); return ptr(); }

	friend void разверни(Значение& a, Значение& b)  { разверни(a.данные, b.данные); }

	typedef КонстОбходчикУ<Вектор<Значение>> const_iterator;

	const_iterator         begin() const                      { return GetVA().begin(); }
	const_iterator         end() const                        { return GetVA().end(); }

#ifdef DEPRECATED
	typedef Значение          value_type;
	typedef const_iterator КонстОбходчик;
	typedef const Значение&   const_reference;
	typedef int            size_type;
	typedef int            difference_type;

	static  void регистрируй(dword w, Проц* (*c)(), const char *имя = NULL);
#endif
};

struct ОшибкаТипаЗначения : Искл {
	Значение ист;
	int   target;

	ОшибкаТипаЗначения(const Ткст& text, const Значение& ист, int target);
};

template <class T> bool  FitsSvoValue()                    { return sizeof(T) <= 8; }
template <class T> Значение SvoToValue(const T& x)            { return Значение(x, Значение::SMALL); }

template <class T> Значение богатыйВЗнач(const T& данные);

template <class T> Значение RawToValue(const T& данные);
template <class T> Значение RawPickToValue(T&& данные);
template <class T> Значение RawDeepToValue(const T& данные);
template <class T> T&    CreateRawValue(Значение& v);

Значение        значОш(const char *s);
Значение        значОш(const Ткст& s);
const Значение& значОш();

template <class T>
inline bool полиРавны(const T& x, const Значение& v) {
	return false;
}

template <class T>
inline hash_t ValueGetHashValue(const T& x) {
	return РНЦП::дайХэшЗнач(x);
}

#define VALUE_COMPARE_V(T, VT) \
inline bool operator==(const Значение& v, T x)   { return v.является<VT>() ? (VT)v == x : v == Значение(x); } \
inline bool operator==(T x, const Значение& v)   { return v.является<VT>() ? (VT)v == x : v == Значение(x); } \
inline bool operator!=(const Значение& v, T x)   { return v.является<VT>() ? (VT)v != x : v != Значение(x); } \
inline bool operator!=(T x, const Значение& v)   { return v.является<VT>() ? (VT)v != x : v != Значение(x); } \

#define СРАВНИ_ЗНАЧЕНИЕ(T) VALUE_COMPARE_V(T, T)

СРАВНИ_ЗНАЧЕНИЕ(int)
СРАВНИ_ЗНАЧЕНИЕ(int64)
СРАВНИ_ЗНАЧЕНИЕ(double)
СРАВНИ_ЗНАЧЕНИЕ(bool)
СРАВНИ_ЗНАЧЕНИЕ(Дата)
СРАВНИ_ЗНАЧЕНИЕ(Время)
СРАВНИ_ЗНАЧЕНИЕ(Ткст)
СРАВНИ_ЗНАЧЕНИЕ(ШТкст)
VALUE_COMPARE_V(const char *, Ткст)
VALUE_COMPARE_V(const wchar *, ШТкст)

inline bool номТипЗначПроц_ли(int q)     { return (dword)q == VOID_V; }
inline bool номТипЗначОш_ли(int q)    { return (dword)q == ERROR_V; }
inline bool номТипЗначТкст_ли(int q)   { return (dword)q == STRING_V || (dword)q == WSTRING_V; }

inline bool номТипЗначЦел_ли(int q)  { return (dword)q == INT_V || (dword)q == INT64_V || (dword)q == BOOL_V; }
inline bool номТипЗначПлав_ли(int q)    { return (dword)q == DOUBLE_V; }

inline bool номТипЗначЧисло_ли(int q)   { return номТипЗначЦел_ли(q) || номТипЗначПлав_ли(q); }
inline bool номТипЗначДатаВремя_ли(int q) { return (dword)q == DATE_V || (dword)q == TIME_V; }

inline bool проц_ли(const Значение& v)       { return v.проц_ли(); }
inline bool ошибка_ли(const Значение& v)      { return v.ошибка_ли(); }
inline bool ткст_ли(const Значение& v)     { return v.является<Ткст>() || v.является<ШТкст>(); }
inline bool число_ли(const Значение& v)     { return v.является<double>() || v.является<int>() || v.является<int64>() || v.является<bool>(); }
inline bool датаВремя_ли(const Значение& v)   { return v.является<Дата>() || v.является<Время>(); }
inline bool массивЗнач_ли(const Значение& v) { return v.дайТип() == VALUEARRAY_V || v.дайТип() == VALUEMAP_V; }
inline bool мапЗнач_ли(const Значение& v)   { return массивЗнач_ли(v); }

Ткст      дайТекстОш(const Значение& v);

inline bool          пусто_ли(const Значение& v)               { return v.пусто_ли(); }
inline const Значение&  Nvl(const Значение& a, const Значение& b)  { return пусто_ли(a) ? b : a; }

inline bool          НЧ_ли(const Значение& v)                { return v.является<double>() && НЧ_ли((double)v); }