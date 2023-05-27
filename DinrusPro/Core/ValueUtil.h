Значение скан(бцел stdtype, const Ткст& text, const Значение& опр = Null, бул *hastime = NULL);

inline const Ткст&  Nvl(const Ткст& a, const Ткст& b)    { return пусто_ли(a) ? b : a; }
inline цел            Nvl(цел a, цел b)                        { return пусто_ли(a) ? b : a; }
inline дол          Nvl(дол a, дол b)                    { return пусто_ли(a) ? b : a; }
inline дво         Nvl(дво a, дво b)                  { return пусто_ли(a) ? b : a; }
inline Дата           Nvl(Дата a, Дата b)                      { return пусто_ли(a) ? b : a; }
inline Время           Nvl(Время a, Время b)                      { return пусто_ли(a) ? b : a; }

inline цел            Nvl(цел a)                               { return Nvl(a, 0); }
inline дол          Nvl(дол a)                             { return Nvl(a, (дол)0); }
inline дво         Nvl(дво a)                            { return Nvl(a, 0.0); }

template <class T>
inline T              Nvl(T a, T b, T c)                       { return Nvl(Nvl(a, b), c); }

template <class T>
inline T              Nvl(T a, T b, T c, T d)                  { return Nvl(Nvl(a, b), c, d); }

template <class T>
inline T              Nvl(T a, T b, T c, T d, T e)             { return Nvl(Nvl(a, b), c, d, e); }

цел сравниСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f);
цел сравниСтдЗнач(const Значение& a, const Значение& b, цел язык);
цел сравниСтдЗнач(const Значение& a, const Значение& b);

цел сравниДескСтдЗнач(const Значение& a, const Значение& b, const LanguageInfo& f);
цел сравниДескСтдЗнач(const Значение& a, const Значение& b, цел язык);
цел сравниДескСтдЗнач(const Значение& a, const Значение& b);

struct ПорядокЗнач {
	virtual бул operator()(const Значение& a, const Значение& b) const = 0;
	virtual ~ПорядокЗнач() {}
};

struct StdValueOrder : ПорядокЗнач {
	цел язык;

	virtual бул operator()(const Значение& a, const Значение& b) const { return сравниСтдЗнач(a, b, язык) < 0; }

	StdValueOrder(цел l = -1) : язык(l) {}
};

struct FnValueOrder : ПорядокЗнач {
	цел (*фн)(const Значение& a, const Значение& b);

	virtual бул operator()(const Значение& a, const Значение& b) const { return (*фн)(a, b) < 0; }

	FnValueOrder(цел (*фн)(const Значение& a, const Значение& b)) : фн(фн) {}
};

struct ValuePairOrder {
	virtual бул operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const = 0;
	virtual ~ValuePairOrder() {}
};

struct StdValuePairOrder : ValuePairOrder {
	цел язык;

	virtual бул operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const;

	StdValuePairOrder(цел l = -1);
};

struct FnValuePairOrder : ValuePairOrder {
	цел (*фн)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2);

	virtual бул operator()(const Значение& keya, const Значение& valuea, const Значение& keyb, const Значение& valueb) const;

	FnValuePairOrder(цел (*фн)(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2)) : фн(фн) {}
};

цел CompareStrings(const Значение& a, const Значение& b, const LanguageInfo& f); // used by StdCompareValue

class Ид : Движ<Ид> {
	Ткст ид;

public:
	const Ткст&  вТкст() const              { return ид; }
	т_хэш         дайХэшЗнач() const          { return РНЦП::дайХэшЗнач(ид); }
	бул           пусто_ли() const                { return РНЦП::пусто_ли(ид); }

	operator const Ткст&() const               { return вТкст(); }
	const Ткст&  operator~() const             { return вТкст(); }
	бул           operator==(const Ид& b) const { return ид == b.ид; }
	бул           operator!=(const Ид& b) const { return ид != b.ид; }

	operator бул() const                        { return ид.дайСчёт(); }

	Ид()                                         {}
	Ид(const Ткст& s)                          { ид = s; }
	Ид(кткст0 s)                            { ид = s; }
};

struct РефМенеджер {
	virtual цел   дайТип() = 0;
	virtual Значение дайЗначение(const проц *)            { return Null; }
	virtual бул  пусто_ли(const проц *)              { return false; }
	virtual проц  устЗначение(проц *, const Значение& v)  { НИКОГДА(); }
	virtual проц  устПусто(проц *)                   { НИКОГДА(); }
	virtual ~РефМенеджер() {}
};

template <class T>
struct StdRef : public РефМенеджер {
	virtual проц  устЗначение(ук p, const Значение& v) { *(T *) p = (T)v; }
	virtual Значение дайЗначение(const ук p)           { return *(const T *) p; }
	virtual цел   дайТип()                         { return дайНомТипаЗнач<T>(); }
	virtual бул  пусто_ли(const ук p)             { return РНЦП::пусто_ли(*(T *) p); }
	virtual проц  устПусто(ук p)                  { РНЦП::устПусто(*(T *)p); }
	virtual ~StdRef() {}
};

class Реф : Движ<Реф> {
protected:
	РефМенеджер *m;
	проц       *укз;
	struct      ValueRef;

public:
	бцел    дайТип() const           { return m ? m->дайТип() : VOID_V; }
	бул     пусто_ли() const            { return m ? m->пусто_ли(укз) : true; }

	проц       *дайПроцУк() const     { return укз; }
	РефМенеджер *GetManager() const     { return m; }

	template <class T>
	бул     является() const                { return дайТип() == дайНомТипаЗнач<T>(); } // VALUE_V!!!
	template <class T>
	T&       дай() const               { ПРОВЕРЬ(дайНомТипаЗнач<T>() == дайТип()); return *(T *)дайПроцУк(); }

	проц     устПусто()                 { if(m) m->устПусто(укз); }
	Значение    дайЗначение() const          { return m ? m->дайЗначение(укз) : Значение(); }
	проц     устЗначение(const Значение& v)  { ПРОВЕРЬ(m); m->устЗначение(укз, v); }

	operator Значение() const             { return дайЗначение(); }
	Значение    operator~() const         { return дайЗначение(); }
	Реф&     operator=(const Значение& v) { устЗначение(v); return *this; }

	Реф(Ткст& s);
	Реф(ШТкст& s);
	Реф(цел& i);
	Реф(дол& i);
	Реф(дво& d);
	Реф(бул& b);
	Реф(Дата& d);
	Реф(Время& t);
	Реф(Значение& v);
	Реф(ук _ptr, РефМенеджер *_m)    { укз = _ptr, m = _m; }
	Реф(const РефНаТипЗнач& r);
	Реф()                              { укз = m = NULL; }
};

template <class T>
T& дайРеф(Реф r, T *x = NULL) {
	ПРОВЕРЬ(дайНомТипаЗнач<T>() == r.дайТип());
	return *(T *) r.дайПроцУк();
}

inline Ткст&  рефТкст(Реф f)  { return дайРеф<Ткст>(f); }
inline ШТкст& рефШТкст(Реф f) { return дайРеф<ШТкст>(f); }
inline цел&     рефЦел(Реф f)     { return дайРеф<цел>(f); }
inline дол&   рефЦел64(Реф f)   { return дайРеф<дол>(f); }
inline дво&  рефДво(Реф f)  { return дайРеф<дво>(f); }
inline бул&    рефБул(Реф f)    { return дайРеф<бул>(f); }
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
	проц       *укз;
};

inline
Реф::Реф(const РефНаТипЗнач& r)
{
	укз = r.укз;
	m = r.m;
}

template <class T, бцел тип, class B>
ТипЗнач<T, тип, B>::operator РефНаТипЗнач()
{
	РефНаТипЗнач h;
	h.укз = this;
	h.m = &Single< StdRef<T> >();
	return h;
}

#define E__Value(I)   Значение p##I
#define E__Ref(I)     Реф p##I

// ---------------------- Значение Массив

class МассивЗнач : public ТипЗнач<МассивЗнач, VALUEARRAY_V, Движ<МассивЗнач> > {
	struct Данные : Значение::Проц {
		virtual бул         пусто_ли() const;
		virtual проц         сериализуй(Поток& s);
		virtual проц         вРяр(РярВВ& xio);
		virtual проц         вДжейсон(ДжейсонВВ& jio);
		virtual т_хэш       дайХэшЗнач() const;
		virtual бул         равен(const Значение::Проц *p);
		virtual Ткст       какТкст() const;
		virtual цел          сравни(const Значение::Проц *p);

		цел GetRefCount() const     { return refcount; }
		Вектор<Значение>& клонируй();

		Вектор<Значение> данные;
	};
	struct ДанныеПусто : Данные {};
	Данные *данные;
	
	static Вектор<Значение> VoidData;

	Вектор<Значение>& создай();
	Вектор<Значение>& клонируй();
	проц  иниц0();

	friend Значение::Проц *создайДанМассивЗач();
	friend class Значение;
	friend class МапЗнач;

public:
	МассивЗнач()                              { иниц0(); }
	МассивЗнач(const МассивЗнач& v);
	МассивЗнач(МассивЗнач&& v);
	МассивЗнач(Вектор<Значение>&& values);
	explicit МассивЗнач(const Вектор<Значение>& values, цел deep);
	МассивЗнач(std::initializer_list<Значение> init)    { иниц0(); for(const auto& i : init) { добавь(i); }}
	~МассивЗнач();

	МассивЗнач& operator=(const МассивЗнач& v);
	МассивЗнач& operator=(Вектор<Значение>&& values)  { *this = МассивЗнач(pick(values)); return *this; }

	operator Значение() const;
	МассивЗнач(const Значение& ист);

	МассивЗнач(const Обнул&)                 { иниц0(); }
	бул экзПусто_ли() const               { return пустой(); }
	
	проц очисть();
	проц устСчёт(цел n);
	проц устСчёт(цел n, const Значение& v);
	цел  дайСчёт() const                     { return данные->данные.дайСчёт(); }
	бул пустой() const                      { return данные->данные.пустой(); }

	проц добавь(const Значение& v);
	МассивЗнач& operator<<(const Значение& v)    { добавь(v); return *this; }
	проц уст(цел i, const Значение& v);
	const Значение& дай(цел i) const;
	Значение дайИОчисть(цел i);
	const Вектор<Значение>& дай() const          { return данные ? данные->данные : VoidData; }
	Вектор<Значение> подбери();

	проц удали(цел i, цел count = 1);
	проц удали(const Вектор<цел>& ii);
	проц вставь(цел i, const МассивЗнач& va);
	проц приставь(const МассивЗнач& va)         { вставь(дайСчёт(), va); }

	const Значение& operator[](цел i) const      { return дай(i); }
	
	Значение& по(цел i);

	т_хэш   дайХэшЗнач() const             { return данные->дайХэшЗнач(); }
	проц     сериализуй(Поток& s);
	проц     вДжейсон(ДжейсонВВ& jio);
	проц     вРяр(РярВВ& xio);
	Ткст   вТкст() const;

	бул     operator==(const МассивЗнач& v) const;
	бул     operator!=(const МассивЗнач& v) const  { return !operator==(v); }
	
	цел      сравни(const МассивЗнач& b) const;
	бул     operator<=(const МассивЗнач& x) const { return сравни(x) <= 0; }
	бул     operator>=(const МассивЗнач& x) const { return сравни(x) >= 0; }
	бул     operator<(const МассивЗнач& x) const  { return сравни(x) < 0; }
	бул     operator>(const МассивЗнач& x) const  { return сравни(x) > 0; }

	typedef КонстОбходчикУ<Вектор<Значение>> const_iterator;

#ifdef DEPRECATED
	typedef Значение          value_type;
	typedef const_iterator КонстОбходчик;
	typedef const Значение&   const_reference;
	typedef цел            size_type;
	typedef цел            difference_type;
#endif

	const_iterator         begin() const                      { return дай().begin(); }
	const_iterator         end() const                        { return дай().end(); }
};

template<>
Ткст какТкст(const МассивЗнач& v);

class МапЗнач : public ТипЗнач<МапЗнач, VALUEMAP_V, Движ<МапЗнач> >{
	struct Данные : Значение::Проц {
		virtual бул       пусто_ли() const;
		virtual проц       сериализуй(Поток& s);
		virtual проц       вРяр(РярВВ& xio);
		virtual проц       вДжейсон(ДжейсонВВ& jio);
		virtual т_хэш     дайХэшЗнач() const;
		virtual бул       равен(const Значение::Проц *p);
		virtual Ткст     какТкст() const;
		virtual цел        сравни(const Значение::Проц *p);

		const Значение& дай(const Значение& k) const {
			цел q = ключ.найди(k);
			return q >= 0 ? значение[q] : значОш();
		}
		Значение& дайДобавь(const Значение& k) {
			цел i = ключ.найди(k);
			if(i < 0) {
				i = значение.дайСчёт();
				ключ.добавь(k);
			}
			return значение.по(i);
		}
		Значение& по(цел i) {
			ПРОВЕРЬ(i < значение.дайСчёт());
			return значение.по(i);
		}

		Индекс<Значение> ключ;
		МассивЗнач   значение;
	};

	struct ДанныеПусто : Данные {};
	Данные *данные;

	Данные& создай();
	static проц клонируй(Данные *&укз);
	force_inline
	static МапЗнач::Данные& разшарь(МапЗнач::Данные *&укз) { if(укз->GetRefCount() != 1) клонируй(укз); return *укз; }
	Данные& разшарь() { return разшарь(данные); }
	проц  иниц0();
	проц  изМассива(const МассивЗнач& va);

	friend Значение::Проц *создайДанМапЗач();
	friend class Значение;

public:
	МапЗнач()                                      { иниц0(); }
	МапЗнач(const МапЗнач& v);
	МапЗнач(const МассивЗнач& va)                  { изМассива(va); }
	МапЗнач(Индекс<Значение>&& k, Вектор<Значение>&& v);
	МапЗнач(ВекторМап<Значение, Значение>&& m);
	МапЗнач(const Индекс<Значение>& k, const Вектор<Значение>& v, цел deep);
	МапЗнач(const ВекторМап<Значение, Значение>& m, цел deep);
	МапЗнач(std::initializer_list<std::pair<Значение, Значение>> init) { иниц0(); for(const auto& i : init) { добавь(i.first, i.second); }}
	~МапЗнач();

	МапЗнач& operator=(const МапЗнач& v);
	МапЗнач& operator=(ВекторМап<Значение, Значение>&& m) { *this = МапЗнач(pick(m)); return *this; }

	operator Значение() const;
	МапЗнач(const Значение& ист);

	МапЗнач(const Обнул&)                         { иниц0(); }
	бул экзПусто_ли() const                     { return пустой(); }

	проц очисть();
	цел  дайСчёт() const                           { return данные->значение.дайСчёт(); }
	бул пустой() const                            { return данные->значение.пустой(); }
	const Значение& дайКлюч(цел i) const                { return данные->ключ[i]; }
	const Значение& дайЗначение(цел i) const              { return данные->значение[i]; }
	цел  найди(const Значение& ключ) const               { return данные ? данные->ключ.найди(ключ) : -1; }
	цел  найдиСледщ(цел ii) const                     { return данные ? данные->ключ.найдиСледщ(ii) : -1; }

	проц добавь(const Значение& ключ, const Значение& значение);
	проц добавь(const Ткст& ключ, const Значение& значение) { добавь(Значение(ключ), значение); }
	проц добавь(кткст0 ключ, const Значение& значение)   { добавь(Значение(ключ), значение); }
	проц добавь(цел ключ, const Значение& значение)           { добавь(Значение(ключ), значение); }
	проц добавь(Ид ключ, const Значение& значение)            { добавь(Значение(ключ.вТкст()), значение); }
	
	МапЗнач& operator()(const Значение& ключ, const Значение& значение)  { добавь(ключ, значение); return *this; }
	МапЗнач& operator()(const Ткст& ключ, const Значение& значение) { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(кткст0 ключ, const Значение& значение)   { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(цел ключ, const Значение& значение)           { добавь(Значение(ключ), значение); return *this; }
	МапЗнач& operator()(Ид ключ, const Значение& значение)            { добавь(Значение(ключ.вТкст()), значение); return *this; }

	проц уст(const Значение& ключ, const Значение& значение);
	проц уст(const Ткст& ключ, const Значение& значение) { уст(Значение(ключ), значение); }
	проц уст(кткст0 ключ, const Значение& значение)   { уст(Значение(ключ), значение); }
	проц уст(цел ключ, const Значение& значение)           { уст(Значение(ключ), значение); }
	проц уст(Ид ключ, const Значение& значение)            { уст(Значение(ключ.вТкст()), значение); }

	проц устПо(цел i, const Значение& v);
	проц устКлюч(цел i, const Значение& ключ);
	проц устКлюч(цел i, const Ткст& ключ)           { устКлюч(i, Значение(ключ)); }
	проц устКлюч(цел i, const char* ключ)             { устКлюч(i, Значение(ключ)); }
	проц устКлюч(цел i, цел ключ)                     { устКлюч(i, Значение(ключ)); }
	проц устКлюч(цел i, Ид ключ)                      { устКлюч(i, Значение(ключ.вТкст())); }

	цел  удалиКлюч(const Значение& ключ);
	цел  удалиКлюч(const Ткст& ключ)               { return удалиКлюч(Значение(ключ)); }
	цел  удалиКлюч(const char* ключ)                 { return удалиКлюч(Значение(ключ)); }
	цел  удалиКлюч(цел ключ)                         { return удалиКлюч(Значение(ключ)); }
	цел  удалиКлюч(Ид ключ)                          { return удалиКлюч(Значение(ключ.вТкст())); }
	проц удали(цел i);

	const Индекс<Значение>& дайКлючи() const             { return данные->ключ; }
	МассивЗнач дайЗначения() const                    { return данные->значение; }

	operator МассивЗнач() const                     { return дайЗначения(); }
	
	ВекторМап<Значение, Значение> подбери();

	const Значение& operator[](const Значение& ключ) const  { return данные->дай(ключ); }
	const Значение& operator[](const Ткст& ключ) const { return operator[](Значение(ключ)); }
	const Значение& operator[](кткст0 ключ) const   { return operator[](Значение(ключ)); }
	const Значение& operator[](const цел ключ) const     { return operator[](Значение(ключ)); }
	const Значение& operator[](const Ид& ключ) const     { return operator[](Значение(ключ.вТкст())); }

	Значение& дайДобавь(const Значение& ключ)                  { return разшарь().дайДобавь(ключ); }
	Значение& operator()(const Значение& ключ)              { return дайДобавь(ключ); }
	Значение& operator()(const Ткст& ключ)             { return operator()(Значение(ключ)); }
	Значение& operator()(кткст0 ключ)               { return operator()(Значение(ключ)); }
	Значение& operator()(const цел ключ)                 { return operator()(Значение(ключ)); }
	Значение& operator()(const Ид& ключ)                 { return operator()(Значение(ключ.вТкст())); }
	Значение& по(цел i)                                 { return разшарь().по(i); }
	
	Значение дайИОчисть(const Значение& ключ);

	т_хэш   дайХэшЗнач() const                   { return данные->дайХэшЗнач(); }
	проц     сериализуй(Поток& s);
	проц     вДжейсон(ДжейсонВВ& jio);
	проц     вРяр(РярВВ& xio);
	Ткст   вТкст() const                       { return данные->какТкст(); }

	бул operator==(const МапЗнач& v) const;
	бул operator!=(const МапЗнач& v) const        { return !operator==(v); }

	цел      сравни(const МапЗнач& b) const;
	бул     operator<=(const МапЗнач& x) const    { return сравни(x) <= 0; }
	бул     operator>=(const МапЗнач& x) const    { return сравни(x) >= 0; }
	бул     operator<(const МапЗнач& x) const     { return сравни(x) < 0; }
	бул     operator>(const МапЗнач& x) const     { return сравни(x) > 0; }

	бул     одинаково(const МапЗнач& b) const;
};

class ГенЗначения {
public:
	virtual Значение  дай() = 0;
	Значение operator++()  { return дай(); }
	virtual ~ГенЗначения() {}
};


#include "Value.hpp"
#include "ValueUtil.hpp"