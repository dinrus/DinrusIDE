Значение скан(dword stdtype, const Ткст& text, const Значение& опр = Null, bool *hastime = NULL);

inline const Ткст&  Nvl(const Ткст& a, const Ткст& b)    { return пусто_ли(a) ? b : a; }
inline int            Nvl(int a, int b)                        { return пусто_ли(a) ? b : a; }
inline int64          Nvl(int64 a, int64 b)                    { return пусто_ли(a) ? b : a; }
inline double         Nvl(double a, double b)                  { return пусто_ли(a) ? b : a; }
inline Дата           Nvl(Дата a, Дата b)                      { return пусто_ли(a) ? b : a; }
inline Время           Nvl(Время a, Время b)                      { return пусто_ли(a) ? b : a; }

inline int            Nvl(int a)                               { return Nvl(a, 0); }
inline int64          Nvl(int64 a)                             { return Nvl(a, (int64)0); }
inline double         Nvl(double a)                            { return Nvl(a, 0.0); }

template <class T>
inline T              Nvl(T a, T b, T c)                       { return Nvl(Nvl(a, b), c); }

template <class T>
inline T              Nvl(T a, T b, T c, T d)                  { return Nvl(Nvl(a, b), c, d); }

template <class T>
inline T              Nvl(T a, T b, T c, T d, T e)             { return Nvl(Nvl(a, b), c, d, e); }

int сравниСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f);
int сравниСтдЗнач(const Значение& a, const Значение& b, int язык);
int сравниСтдЗнач(const Значение& a, const Значение& b);

int сравниДескСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f);
int сравниДескСтдЗнач(const Значение& a, const Значение& b, int язык);
int сравниДескСтдЗнач(const Значение& a, const Значение& b);

struct ПорядокЗнач {
	virtual bool operator()(const Значение& a, const Значение& b) const = 0;
	virtual ~ПорядокЗнач() {}
};

struct StdValueOrder : ПорядокЗнач {
	int язык;

	virtual bool operator()(const Значение& a, const Значение& b) const { return сравниСтдЗнач(a, b, язык) < 0; }

	StdValueOrder(int l = -1) : язык(l) {}
};

struct FnValueOrder : ПорядокЗнач {
	int (*фн)(const Значение& a, const Значение& b);

	virtual bool operator()(const Значение& a, const Значение& b) const { return (*фн)(a, b) < 0; }

	FnValueOrder(int (*фн)(const Значение& a, const Значение& b)) : фн(фн) {}
};

struct ValuePairOrder {
	virtual bool operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const = 0;
	virtual ~ValuePairOrder() {}
};

struct StdValuePairOrder : ValuePairOrder {
	int язык;

	virtual bool operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const;

	StdValuePairOrder(int l = -1);
};

struct FnValuePairOrder : ValuePairOrder {
	int (*фн)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2);

	virtual bool operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const;

	FnValuePairOrder(int (*фн)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2)) : фн(фн) {}
};

int CompareStrings(const Значение& a, const Значение& b, const LanguageInfo& f); // used by StdCompareValue

class Ид : Движимое<Ид> {
	Ткст ид;

public:
	const Ткст&  вТкст() const              { return ид; }
	hash_t         дайХэшЗнач() const          { return РНЦП::дайХэшЗнач(ид); }
	bool           пусто_ли() const                { return РНЦП::пусто_ли(ид); }

	operator const Ткст&() const               { return вТкст(); }
	const Ткст&  operator~() const             { return вТкст(); }
	bool           operator==(const Ид& b) const { return ид == b.ид; }
	bool           operator!=(const Ид& b) const { return ид != b.ид; }

	operator bool() const                        { return ид.дайСчёт(); }

	Ид()                                         {}
	Ид(const Ткст& s)                          { ид = s; }
	Ид(const char *s)                            { ид = s; }
};

struct РефМенеджер {
	virtual int   дайТип() = 0;
	virtual Значение дайЗначение(const void *)            { return Null; }
	virtual bool  пусто_ли(const void *)              { return false; }
	virtual void  устЗначение(void *, const Значение& v)  { НИКОГДА(); }
	virtual void  устПусто(void *)                   { НИКОГДА(); }
	virtual ~РефМенеджер() {}
};

template <class T>
struct StdRef : public РефМенеджер {
	virtual void  устЗначение(void *p, const Значение& v) { *(T *) p = (T)v; }
	virtual Значение дайЗначение(const void *p)           { return *(const T *) p; }
	virtual int   дайТип()                         { return дайНомТипаЗнач<T>(); }
	virtual bool  пусто_ли(const void *p)             { return РНЦП::пусто_ли(*(T *) p); }
	virtual void  устПусто(void *p)                  { РНЦП::устПусто(*(T *)p); }
	virtual ~StdRef() {}
};

class Реф : Движимое<Реф> {
protected:
	РефМенеджер *m;
	void       *ptr;
	struct      ValueRef;

public:
	dword    дайТип() const           { return m ? m->дайТип() : VOID_V; }
	bool     пусто_ли() const            { return m ? m->пусто_ли(ptr) : true; }

	void       *дайПроцУк() const     { return ptr; }
	РефМенеджер *GetManager() const     { return m; }

	template <class T>
	bool     является() const                { return дайТип() == дайНомТипаЗнач<T>(); } // VALUE_V!!!
	template <class T>
	T&       дай() const               { ПРОВЕРЬ(дайНомТипаЗнач<T>() == дайТип()); return *(T *)дайПроцУк(); }

	void     устПусто()                 { if(m) m->устПусто(ptr); }
	Значение    дайЗначение() const          { return m ? m->дайЗначение(ptr) : Значение(); }
	void     устЗначение(const Значение& v)  { ПРОВЕРЬ(m); m->устЗначение(ptr, v); }

	operator Значение() const             { return дайЗначение(); }
	Значение    operator~() const         { return дайЗначение(); }
	Реф&     operator=(const Значение& v) { устЗначение(v); return *this; }

	Реф(Ткст& s);
	Реф(ШТкст& s);
	Реф(int& i);
	Реф(int64& i);
	Реф(double& d);
	Реф(bool& b);
	Реф(Дата& d);
	Реф(Время& t);
	Реф(Значение& v);
	Реф(void *_ptr, РефМенеджер *_m)    { ptr = _ptr, m = _m; }
	Реф(const РефНаТипЗнач& r);
	Реф()                              { ptr = m = NULL; }
};

template <class T>
T& дайРеф(Реф r, T *x = NULL) {
	ПРОВЕРЬ(дайНомТипаЗнач<T>() == r.дайТип());
	return *(T *) r.дайПроцУк();
}

inline Ткст&  рефТкст(Реф f)  { return дайРеф<Ткст>(f); }
inline ШТкст& рефШТкст(Реф f) { return дайРеф<ШТкст>(f); }
inline int&     рефЦел(Реф f)     { return дайРеф<int>(f); }
inline int64&   рефЦел64(Реф f)   { return дайРеф<int64>(f); }
inline double&  рефДво(Реф f)  { return дайРеф<double>(f); }
inline bool&    рефБул(Реф f)    { return дайРеф<bool>(f); }
inline Дата&    рефДата(Реф f)    { return дайРеф<Дата>(f); }
inline Время&    рефВремя(Реф f)    { return дайРеф<Время>(f); }
inline Значение&   рефЗнач(Реф f)   { ПРОВЕРЬ(f.дайТип() == VALUE_V);
                                    return *(Значение *)f.дайПроцУк(); }

template <class T>
Реф какРеф(T& x) {
	return Реф(&x, &Single< StdRef<T> >());
}

struct РефНаТипЗнач {
	РефМенеджер *m;
	void       *ptr;
};

inline
Реф::Реф(const РефНаТипЗнач& r)
{
	ptr = r.ptr;
	m = r.m;
}

template <class T, dword тип, class B>
ТипЗнач<T, тип, B>::operator РефНаТипЗнач()
{
	РефНаТипЗнач h;
	h.ptr = this;
	h.m = &Single< StdRef<T> >();
	return h;
}

#define E__Value(I)   Значение p##I
#define E__Ref(I)     Реф p##I

// ---------------------- Значение Массив

class МассивЗнач : public ТипЗнач<МассивЗнач, VALUEARRAY_V, Движимое<МассивЗнач> > {
	struct Данные : Значение::Проц {
		virtual bool         пусто_ли() const;
		virtual void         сериализуй(Поток& s);
		virtual void         вРяр(РярВВ& xio);
		virtual void         вДжейсон(ДжейсонВВ& jio);
		virtual hash_t       дайХэшЗнач() const;
		virtual bool         равен(const Значение::Проц *p);
		virtual Ткст       какТкст() const;
		virtual int          сравни(const Значение::Проц *p);

		int GetRefCount() const     { return refcount; }
		Вектор<Значение>& клонируй();

		Вектор<Значение> данные;
	};
	struct ДанныеПусто : Данные {};
	Данные *данные;
	
	static Вектор<Значение> VoidData;

	Вектор<Значение>& создай();
	Вектор<Значение>& клонируй();
	void  иниц0();

	friend Значение::Проц *создайДанМассивЗач();
	friend class Значение;
	friend class МапЗнач;

public:
	МассивЗнач()                              { иниц0(); }
	МассивЗнач(const МассивЗнач& v);
	МассивЗнач(МассивЗнач&& v);
	МассивЗнач(Вектор<Значение>&& values);
	explicit МассивЗнач(const Вектор<Значение>& values, int deep);
	МассивЗнач(std::initializer_list<Значение> init)    { иниц0(); for(const auto& i : init) { добавь(i); }}
	~МассивЗнач();

	МассивЗнач& operator=(const МассивЗнач& v);
	МассивЗнач& operator=(Вектор<Значение>&& values)  { *this = МассивЗнач(pick(values)); return *this; }

	operator Значение() const;
	МассивЗнач(const Значение& ист);

	МассивЗнач(const Обнул&)                 { иниц0(); }
	bool экзПусто_ли() const               { return пустой(); }
	
	void очисть();
	void устСчёт(int n);
	void устСчёт(int n, const Значение& v);
	int  дайСчёт() const                     { return данные->данные.дайСчёт(); }
	bool пустой() const                      { return данные->данные.пустой(); }

	void добавь(const Значение& v);
	МассивЗнач& operator<<(const Значение& v)    { добавь(v); return *this; }
	void уст(int i, const Значение& v);
	const Значение& дай(int i) const;
	Значение дайИОчисть(int i);
	const Вектор<Значение>& дай() const          { return данные ? данные->данные : VoidData; }
	Вектор<Значение> подбери();

	void удали(int i, int count = 1);
	void удали(const Вектор<int>& ii);
	void вставь(int i, const МассивЗнач& va);
	void приставь(const МассивЗнач& va)         { вставь(дайСчёт(), va); }

	const Значение& operator[](int i) const      { return дай(i); }
	
	Значение& по(int i);

	hash_t   дайХэшЗнач() const             { return данные->дайХэшЗнач(); }
	void     сериализуй(Поток& s);
	void     вДжейсон(ДжейсонВВ& jio);
	void     вРяр(РярВВ& xio);
	Ткст   вТкст() const;

	bool     operator==(const МассивЗнач& v) const;
	bool     operator!=(const МассивЗнач& v) const  { return !operator==(v); }
	
	int      сравни(const МассивЗнач& b) const;
	bool     operator<=(const МассивЗнач& x) const { return сравни(x) <= 0; }
	bool     operator>=(const МассивЗнач& x) const { return сравни(x) >= 0; }
	bool     operator<(const МассивЗнач& x) const  { return сравни(x) < 0; }
	bool     operator>(const МассивЗнач& x) const  { return сравни(x) > 0; }

	typedef КонстОбходчикУ<Вектор<Значение>> const_iterator;

#ifdef DEPRECATED
	typedef Значение          value_type;
	typedef const_iterator КонстОбходчик;
	typedef const Значение&   const_reference;
	typedef int            size_type;
	typedef int            difference_type;
#endif

	const_iterator         begin() const                      { return дай().begin(); }
	const_iterator         end() const                        { return дай().end(); }
};

template<>
Ткст какТкст(const МассивЗнач& v);

class МапЗнач : public ТипЗнач<МапЗнач, VALUEMAP_V, Движимое<МапЗнач> >{
	struct Данные : Значение::Проц {
		virtual bool       пусто_ли() const;
		virtual void       сериализуй(Поток& s);
		virtual void       вРяр(РярВВ& xio);
		virtual void       вДжейсон(ДжейсонВВ& jio);
		virtual hash_t     дайХэшЗнач() const;
		virtual bool       равен(const Значение::Проц *p);
		virtual Ткст     какТкст() const;
		virtual int        сравни(const Значение::Проц *p);

		const Значение& дай(const Значение& k) const {
			int q = ключ.найди(k);
			return q >= 0 ? значение[q] : значОш();
		}
		Значение& дайДобавь(const Значение& k) {
			int i = ключ.найди(k);
			if(i < 0) {
				i = значение.дайСчёт();
				ключ.добавь(k);
			}
			return значение.по(i);
		}
		Значение& по(int i) {
			ПРОВЕРЬ(i < значение.дайСчёт());
			return значение.по(i);
		}

		Индекс<Значение> ключ;
		МассивЗнач   значение;
	};

	struct ДанныеПусто : Данные {};
	Данные *данные;

	Данные& создай();
	static void клонируй(Данные *&ptr);
	force_inline
	static МапЗнач::Данные& UnShare(МапЗнач::Данные *&ptr) { if(ptr->GetRefCount() != 1) клонируй(ptr); return *ptr; }
	Данные& UnShare() { return UnShare(данные); }
	void  иниц0();
	void  изМассива(const МассивЗнач& va);

	friend Значение::Проц *создайДанМапЗач();
	friend class Значение;

public:
	МапЗнач()                                      { иниц0(); }
	МапЗнач(const МапЗнач& v);
	МапЗнач(const МассивЗнач& va)                  { изМассива(va); }
	МапЗнач(Индекс<Значение>&& k, Вектор<Значение>&& v);
	МапЗнач(ВекторМап<Значение, Значение>&& m);
	МапЗнач(const Индекс<Значение>& k, const Вектор<Значение>& v, int deep);
	МапЗнач(const ВекторМап<Значение, Значение>& m, int deep);
	МапЗнач(std::initializer_list<std::pair<Значение, Значение>> init) { иниц0(); for(const auto& i : init) { добавь(i.first, i.second); }}
	~МапЗнач();

	МапЗнач& operator=(const МапЗнач& v);
	МапЗнач& operator=(ВекторМап<Значение, Значение>&& m) { *this = МапЗнач(pick(m)); return *this; }

	operator Значение() const;
	МапЗнач(const Значение& ист);

	МапЗнач(const Обнул&)                         { иниц0(); }
	bool экзПусто_ли() const                     { return пустой(); }

	void очисть();
	int  дайСчёт() const                           { return данные->значение.дайСчёт(); }
	bool пустой() const                            { return данные->значение.пустой(); }
	const Значение& дайКлюч(int i) const                { return данные->ключ[i]; }
	const Значение& дайЗначение(int i) const              { return данные->значение[i]; }
	int  найди(const Значение& ключ) const               { return данные ? данные->ключ.найди(ключ) : -1; }
	int  найдиСледщ(int ii) const                     { return данные ? данные->ключ.найдиСледщ(ii) : -1; }

	void добавь(const Значение& ключ, const Значение& значение);
	void добавь(const Ткст& ключ, const Значение& значение) { добавь(Значение(ключ), значение); }
	void добавь(const char *ключ, const Значение& значение)   { добавь(Значение(ключ), значение); }
	void добавь(int ключ, const Значение& значение)           { добавь(Значение(ключ), значение); }
	void добавь(Ид ключ, const Значение& значение)            { добавь(Значение(ключ.вТкст()), значение); }
	
	МапЗнач& operator()(const Значение& ключ, const Значение& значение)  { добавь(ключ, значение); return *this; }
	МапЗнач& operator()(const Ткст& ключ, const Значение& значение) { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(const char *ключ, const Значение& значение)   { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(int ключ, const Значение& значение)           { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(Ид ключ, const Значение& значение)            { добавь(Значение(ключ.вТкст()), значение); return *this; }

	void уст(const Значение& ключ, const Значение& значение);
	void уст(const Ткст& ключ, const Значение& значение) { уст(Значение(ключ), значение); }
	void уст(const char *ключ, const Значение& значение)   { уст(Значение(ключ), значение); }
	void уст(int ключ, const Значение& значение)           { уст(Значение(ключ), значение); }
	void уст(Ид ключ, const Значение& значение)            { уст(Значение(ключ.вТкст()), значение); }

	void устПо(int i, const Значение& v);
	void устКлюч(int i, const Значение& ключ);
	void устКлюч(int i, const Ткст& ключ)           { устКлюч(i, Значение(ключ)); }
	void устКлюч(int i, const char* ключ)             { устКлюч(i, Значение(ключ)); }
	void устКлюч(int i, int ключ)                     { устКлюч(i, Значение(ключ)); }
	void устКлюч(int i, Ид ключ)                      { устКлюч(i, Значение(ключ.вТкст())); }

	int  удалиКлюч(const Значение& ключ);
	int  удалиКлюч(const Ткст& ключ)               { return удалиКлюч(Значение(ключ)); }
	int  удалиКлюч(const char* ключ)                 { return удалиКлюч(Значение(ключ)); }
	int  удалиКлюч(int ключ)                         { return удалиКлюч(Значение(ключ)); }
	int  удалиКлюч(Ид ключ)                          { return удалиКлюч(Значение(ключ.вТкст())); }
	void удали(int i);

	const Индекс<Значение>& дайКлючи() const             { return данные->ключ; }
	МассивЗнач дайЗначения() const                    { return данные->значение; }

	operator МассивЗнач() const                     { return дайЗначения(); }
	
	ВекторМап<Значение, Значение> подбери();

	const Значение& operator[](const Значение& ключ) const  { return данные->дай(ключ); }
	const Значение& operator[](const Ткст& ключ) const { return operator[](Значение(ключ)); }
	const Значение& operator[](const char *ключ) const   { return operator[](Значение(ключ)); }
	const Значение& operator[](const int ключ) const     { return operator[](Значение(ключ)); }
	const Значение& operator[](const Ид& ключ) const     { return operator[](Значение(ключ.вТкст())); }

	Значение& дайДобавь(const Значение& ключ)                  { return UnShare().дайДобавь(ключ); }
	Значение& operator()(const Значение& ключ)              { return дайДобавь(ключ); }
	Значение& operator()(const Ткст& ключ)             { return operator()(Значение(ключ)); }
	Значение& operator()(const char *ключ)               { return operator()(Значение(ключ)); }
	Значение& operator()(const int ключ)                 { return operator()(Значение(ключ)); }
	Значение& operator()(const Ид& ключ)                 { return operator()(Значение(ключ.вТкст())); }
	Значение& по(int i)                                 { return UnShare().по(i); }
	
	Значение дайИОчисть(const Значение& ключ);

	hash_t   дайХэшЗнач() const                   { return данные->дайХэшЗнач(); }
	void     сериализуй(Поток& s);
	void     вДжейсон(ДжейсонВВ& jio);
	void     вРяр(РярВВ& xio);
	Ткст   вТкст() const                       { return данные->какТкст(); }

	bool operator==(const МапЗнач& v) const;
	bool operator!=(const МапЗнач& v) const        { return !operator==(v); }

	int      сравни(const МапЗнач& b) const;
	bool     operator<=(const МапЗнач& x) const    { return сравни(x) <= 0; }
	bool     operator>=(const МапЗнач& x) const    { return сравни(x) >= 0; }
	bool     operator<(const МапЗнач& x) const     { return сравни(x) < 0; }
	bool     operator>(const МапЗнач& x) const     { return сравни(x) > 0; }

	bool     одинаково(const МапЗнач& b) const;
};

class ГенЗначения {
public:
	virtual Значение  дай() = 0;
	Значение operator++()  { return дай(); }
	virtual ~ГенЗначения() {}
};


#include "Value.hpp"
#include "ValueUtil.hpp"