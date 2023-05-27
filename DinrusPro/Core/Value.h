
template <class T, бцел тип = UNKNOWN_V, class B = ПустойКласс>
class ТипЗнач : public B {
public:
	static бцел номерТипаЗнач()                      { return тип == UNKNOWN_V ? StaticTypeNo<T>() + 0x8000000 : тип; }
	friend бцел номерТипаЗнач(const T*)              { return T::номерТипаЗнач(); }
	
	бул     экзПусто_ли() const                 { return false; }
	проц     сериализуй(Поток& s)                   { НИКОГДА(); }
	проц     вРяр(РярВВ& xio)                     { НИКОГДА(); }
	проц     вДжейсон(ДжейсонВВ& jio)                   { НИКОГДА(); }
	т_хэш   дайХэшЗнач() const                   { return 0; }
	бул     operator==(const T&) const             { НИКОГДА(); return false; }
	Ткст   вТкст() const                       { return typeid(T).name(); }
	цел      сравни(const T&) const                { НИКОГДА(); return 0; }
	цел      полиСравни(const Значение&) const        { НИКОГДА(); return 0; }
	
	operator РефНаТипЗнач();
};

template <class T, бцел тип, class B = ПустойКласс> // Backward compatiblity
class ПрисвойНомТипаЗнач : public ТипЗнач<T, тип, B> {};

template <class T>
бцел дайНомТипаЗнач() { return номерТипаЗнач((T*)NULL); }

class Значение : Движ_<Значение> {
public:
	class Проц {
	protected:
		Атомар  refcount;

	public:
		проц               Retain()                    { атомнИнк(refcount); }
		проц               отпусти()                   { if(атомнДек(refcount) == 0) delete this; }
		цел                GetRefCount() const         { return refcount; }

		virtual бул       пусто_ли() const              { return true; }
		virtual проц       сериализуй(Поток& s)        {}
		virtual проц       вРяр(РярВВ& xio)          {}
		virtual проц       вДжейсон(ДжейсонВВ& jio)        {}
		virtual т_хэш     дайХэшЗнач() const        { return 0; }
		virtual бул       равен(const Проц *p)      { return false; }
		virtual бул       полиРавны(const Значение& v) { return false; }
		virtual Ткст     какТкст() const            { return ""; }
		virtual цел        сравни(const Проц *p)      { return 0; }
		virtual цел        полиСравни(const Значение& p) { return 0; }

		Проц()                                         { refcount = 1; }
		virtual ~Проц()                                {}

		friend class Значение;
	};

	struct Sval {
		бул       (*пусто_ли)(const ук p);
		проц       (*сериализуй)(ук p, Поток& s);
		проц       (*вРяр)(ук p, РярВВ& xio);
		проц       (*вДжейсон)(ук p, ДжейсонВВ& jio);
		т_хэш     (*дайХэшЗнач)(const ук p);
		бул       (*равен)(const ук p1, const ук p2);
		бул       (*полиРавны)(const ук p, const Значение& v);
		Ткст     (*какТкст)(const ук p);
		цел        (*сравни)(const ук p1, const ук p2);
		цел        (*полиСравни)(const ук p1, const Значение& p2);
	};
	
protected:
	enum { STRING = 0, REF = 255, VOIDV = 3 };

	static ВекторМап<бцел, Проц* (*)()>& Typemap();
	static Sval *svo[256];
	static Индекс<Ткст>& индексИмени();
	static Индекс<бцел>&  индексТипа();
	
	static проц   добавьИмя(бцел тип, кткст0 имя);
	static цел    дайТип(кткст0 имя);
	static Ткст дайИмя(бцел тип);
	static проц   регистрируйСтд();

	friend проц ValueRegisterHelper();

	Ткст   данные;
	Проц    *&укз()                  { ПРОВЕРЬ(реф_ли()); return *(Проц **)&данные; }
	Проц     *укз() const            { ПРОВЕРЬ(реф_ли()); return *(Проц **)&данные; }

	проц     устРефТип(бцел тип)  { ПРОВЕРЬ(реф_ли()); ((цел *)&данные)[2] = тип; }
	бцел    дайРефТип() const      { ПРОВЕРЬ(реф_ли()); return ((цел *)&данные)[2]; }

	бул     ткст_ли() const          { return !данные.особый_ли(); }
	бул     является(ббайт v) const          { return данные.особый_ли(v); }
	бул     реф_ли() const             { return является(REF); }
	проц     иницРеф(Проц *p, бцел t) { данные.устОсобо(REF); укз() = p; устРефТип(t); }
	проц     отпустиРеф();
	проц     RefRetain();
	проц     освободиРеф()                 { if(реф_ли()) отпустиРеф(); }
	проц     освободи()                    { освободиРеф(); данные.очисть(); }
	проц     устЛардж(const Значение& v);

	template <class T>
	проц     иницСмолл(const T& init);
	template <class T>
	T&       дайСыройСмолл() const;
	template <class T>
	T&       дайСмолл() const;
	
	цел      дайДрЦел() const;
	дол    дайДрЦел64() const;
	дво   дайДрДво() const;
	бул     дайДрБул() const;
	Дата     дайДрДату() const;
	Время     дайДрВремя() const;
	Ткст   дайДрТкст() const;
	т_хэш   дайДрЗначХэш() const;

	бул     полиРавны(const Значение& v) const;
	
	enum VSMALL { SMALL };

	template <class T>
	Значение(const T& значение, VSMALL);
	
	template <class T> friend Значение SvoToValue(const T& x);

	Ткст  дайИмя() const;
	
	цел     полиСравни(const Значение& v) const;
	цел     сравни2(const Значение& v) const;

	Вектор<Значение>&  разшарьArray();

	const Вектор<Значение>& GetVA() const;
	
#if defined(_ОТЛАДКА) && defined(COMPILER_GCC)
	бцел  magic[4];
	проц    Magic()               { magic[0] = 0xc436d851; magic[1] = 0x72f67c76; magic[2] = 0x3e5e10fd; magic[3] = 0xc90d370b; }
	проц    ClearMagic()          { magic[0] = magic[1] = magic[2] = magic[3] = 0; }
#else
	проц    Magic()               {}
	проц    ClearMagic()          {}
#endif

#ifndef DEPRECATED
	static  проц регистрируй(бцел w, Проц* (*c)(), кткст0 имя = NULL);
#endif

	Значение(Проц *p) = delete;

public:
	template <class T>
	static  проц регистрируй(кткст0 имя = NULL);
	template <class T>
	static  проц SvoRegister(кткст0 имя = NULL);
	
	бцел    дайТип() const;
	бул     ошибка_ли() const         { return дайТип() == ERROR_V; }
	бул     проц_ли() const          { return является(VOIDV) || ошибка_ли(); }
	бул     пусто_ли() const;

	template <class T>
	бул     является() const;
	template <class T>
	const T& To() const;
	template <class T>
	const T& дай() const;

	operator Ткст() const          { return ткст_ли() ? данные : дайДрТкст(); }
	operator ШТкст() const;
	operator Дата() const            { return является(DATE_V) ? дайСыройСмолл<Дата>() : дайДрДату(); }
	operator Время() const            { return является(TIME_V) ? дайСыройСмолл<Время>() : дайДрВремя(); }
	operator дво() const          { return является(DOUBLE_V) ? дайСыройСмолл<дво>() : дайДрДво(); }
	operator цел() const             { return является(INT_V) ? дайСыройСмолл<цел>() : дайДрЦел(); }
	operator дол() const           { return является(INT64_V) ? дайСыройСмолл<дол>() : дайДрЦел64(); }
	operator бул() const            { return является(BOOL_V) ? дайСыройСмолл<бул>() : дайДрБул(); }
	std::string  вСтд() const       { return operator Ткст().вСтд(); }
	std::wstring вСтдШ() const      { return operator ШТкст().вСтд(); }
	
	Значение(const Ткст& s) : данные(s) { Magic(); }
	Значение(const ШТкст& s);
	Значение(кткст0 s) : данные(s)   { Magic(); }
	Значение(цел i)                     : данные(i, INT_V, Ткст::SPECIAL) { Magic(); }
	Значение(дол i)                   : данные(i, INT64_V, Ткст::SPECIAL) { Magic(); }
	Значение(дво d)                  : данные(d, DOUBLE_V, Ткст::SPECIAL) { Magic(); }
	Значение(бул b)                    : данные(b, BOOL_V, Ткст::SPECIAL) { Magic(); }
	Значение(Дата d)                    : данные(d, DATE_V, Ткст::SPECIAL) { Magic(); }
	Значение(Время t)                    : данные(t, TIME_V, Ткст::SPECIAL) { Magic(); }
	Значение(const Обнул&)             : данные((цел)Null, INT_V, Ткст::SPECIAL) { Magic(); }
	Значение(const std::string& s) : Значение(Ткст(s)) {}
	Значение(const std::wstring& s) : Значение(ШТкст(s)) {}

	бул operator==(const Значение& v) const;
	бул operator!=(const Значение& v) const { return !operator==(v); }
	бул одинаково(const Значение& v) const;
	
	цел  сравни(const Значение& v) const;
	бул operator<=(const Значение& x) const { return сравни(x) <= 0; }
	бул operator>=(const Значение& x) const { return сравни(x) >= 0; }
	бул operator<(const Значение& x) const  { return сравни(x) < 0; }
	бул operator>(const Значение& x) const  { return сравни(x) > 0; }

	Ткст вТкст() const;
	Ткст operator ~() const             { return вТкст(); }
	Ткст дайИмяТипа() const            { return дайИмя(); }

	проц  сериализуй(Поток& s);
	проц  вРяр(РярВВ& xio);
	проц  вДжейсон(ДжейсонВВ& jio);

	т_хэш дайХэшЗнач() const;

	Значение& operator=(const Значение& v);
	Значение(const Значение& v);
	
	цел   дайСчёт() const;
	const Значение& operator[](цел i) const;
	const Значение& operator[](const Ткст& ключ) const;
	const Значение& operator[](кткст0 ключ) const;
	const Значение& operator[](const Ид& ключ) const;
	
	Значение& по(цел i);
	Значение& operator()(цел i)              { return по(i); }
	проц   добавь(const Значение& ист);
	template <typename T>
	Значение& operator<<(const T& ист)       { добавь(ист); return *this; }

	Значение& дайДобавь(const Значение& ключ);
	Значение& operator()(const Ткст& ключ);
	Значение& operator()(кткст0 ключ);
	Значение& operator()(const Ид& ключ);

	Значение()                               : данные((цел)Null, VOIDV, Ткст::SPECIAL) { Magic(); }
	~Значение()                              { ClearMagic(); if(реф_ли()) отпустиРеф(); }

	Значение(Проц *p, бцел тип)            { иницРеф(p, тип); Magic(); }
	const Проц *дайПроцУк() const        { ПРОВЕРЬ(реф_ли()); return укз(); }

	friend проц разверни(Значение& a, Значение& b)  { разверни(a.данные, b.данные); }

	typedef КонстОбходчикУ<Вектор<Значение>> const_iterator;

	const_iterator         begin() const                      { return GetVA().begin(); }
	const_iterator         end() const                        { return GetVA().end(); }

#ifdef DEPRECATED
	typedef Значение          value_type;
	typedef const_iterator КонстОбходчик;
	typedef const Значение&   const_reference;
	typedef цел            size_type;
	typedef цел            difference_type;

	static  проц регистрируй(бцел w, Проц* (*c)(), кткст0 имя = NULL);
#endif
};

struct ОшибкаТипаЗначения : Искл {
	Значение ист;
	цел   target;

	ОшибкаТипаЗначения(const Ткст& text, const Значение& ист, цел target);
};

template <class T> бул  FitsSvoValue()                    { return sizeof(T) <= 8; }
template <class T> Значение SvoToValue(const T& x)            { return Значение(x, Значение::SMALL); }

template <class T> Значение богатыйВЗнач(const T& данные);

template <class T> Значение RawToValue(const T& данные);
template <class T> Значение RawPickToValue(T&& данные);
template <class T> Значение RawDeepToValue(const T& данные);
template <class T> T&    CreateRawValue(Значение& v);

Значение        значОш(кткст0 s);
Значение        значОш(const Ткст& s);
const Значение& значОш();

template <class T>
inline бул полиРавны(const T& x, const Значение& v) {
	return false;
}

template <class T>
inline т_хэш ValueGetHashValue(const T& x) {
	return РНЦП::дайХэшЗнач(x);
}

#define VALUE_COMPARE_V(T, VT) \
inline бул operator==(const Значение& v, T x)   { return v.является<VT>() ? (VT)v == x : v == Значение(x); } \
inline бул operator==(T x, const Значение& v)   { return v.является<VT>() ? (VT)v == x : v == Значение(x); } \
inline бул operator!=(const Значение& v, T x)   { return v.является<VT>() ? (VT)v != x : v != Значение(x); } \
inline бул operator!=(T x, const Значение& v)   { return v.является<VT>() ? (VT)v != x : v != Значение(x); } \

#define СРАВНИ_ЗНАЧЕНИЕ(T) VALUE_COMPARE_V(T, T)

СРАВНИ_ЗНАЧЕНИЕ(цел)
СРАВНИ_ЗНАЧЕНИЕ(дол)
СРАВНИ_ЗНАЧЕНИЕ(дво)
СРАВНИ_ЗНАЧЕНИЕ(бул)
СРАВНИ_ЗНАЧЕНИЕ(Дата)
СРАВНИ_ЗНАЧЕНИЕ(Время)
СРАВНИ_ЗНАЧЕНИЕ(Ткст)
СРАВНИ_ЗНАЧЕНИЕ(ШТкст)
VALUE_COMPARE_V(const char *, Ткст)
VALUE_COMPARE_V(const шим *, ШТкст)

inline бул номТипЗначПроц_ли(цел q)     { return (бцел)q == VOID_V; }
inline бул номТипЗначОш_ли(цел q)    { return (бцел)q == ERROR_V; }
inline бул номТипЗначТкст_ли(цел q)   { return (бцел)q == STRING_V || (бцел)q == WSTRING_V; }

inline бул номТипЗначЦел_ли(цел q)  { return (бцел)q == INT_V || (бцел)q == INT64_V || (бцел)q == BOOL_V; }
inline бул номТипЗначПлав_ли(цел q)    { return (бцел)q == DOUBLE_V; }

inline бул номТипЗначЧисло_ли(цел q)   { return номТипЗначЦел_ли(q) || номТипЗначПлав_ли(q); }
inline бул номТипЗначДатаВремя_ли(цел q) { return (бцел)q == DATE_V || (бцел)q == TIME_V; }

inline бул проц_ли(const Значение& v)       { return v.проц_ли(); }
inline бул ошибка_ли(const Значение& v)      { return v.ошибка_ли(); }
inline бул ткст_ли(const Значение& v)     { return v.является<Ткст>() || v.является<ШТкст>(); }
inline бул число_ли(const Значение& v)     { return v.является<дво>() || v.является<цел>() || v.является<дол>() || v.является<бул>(); }
inline бул датаВремя_ли(const Значение& v)   { return v.является<Дата>() || v.является<Время>(); }
inline бул массивЗнач_ли(const Значение& v) { return v.дайТип() == VALUEARRAY_V || v.дайТип() == VALUEMAP_V; }
inline бул мапЗнач_ли(const Значение& v)   { return массивЗнач_ли(v); }

Ткст      дайТекстОш(const Значение& v);

inline бул          пусто_ли(const Значение& v)               { return v.пусто_ли(); }
inline const Значение&  Nvl(const Значение& a, const Значение& b)  { return пусто_ли(a) ? b : a; }

inline бул          нч_ли(const Значение& v)                { return v.является<дво>() && нч_ли((дво)v); }