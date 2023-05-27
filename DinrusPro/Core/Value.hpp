inline
Значение::Значение(const Значение& v)
:	данные(Ткст::SPECIAL)
{
	if(v.реф_ли() || v.данные.IsLarge())
		устЛардж(v);
	else
		данные.устСмолл(v.данные);
	Magic();
}

template<>
inline бул полиРавны(const бул& x, const Значение& v) {
	return v.является<дво>() && цел(x) == дво(v)
	    || v.является<дол>() && цел(x) == дол(v)
	    || v.является<цел>() && цел(x) == цел(v);
}

template<>
inline бул полиРавны(const цел& x, const Значение& v) {
	return v.является<дво>() && x == дво(v)
	    || v.является<дол>() && x == дол(v);
}

template<>
inline бул полиРавны(const дол& x, const Значение& v) {
	return v.является<дво>() && дво(x) == дво(v);
}

template<>
inline бул полиРавны(const Дата& x, const Значение& v) {
	return v.является<Время>() && воВремя(x) == Время(v);
}

template<>
inline бул полиРавны(const ШТкст& x, const Значение& v) {
	return v.дайТип() == STRING_V && ШТкст(v) == x;
}

template<class T>
inline цел  полиСравни(const T& a, const Значение& b)
{
	return a.полиСравни(b);
}

template<>
inline цел полиСравни(const ШТкст& x, const Значение& v) {
	return ткст_ли(v) && сравниЗнак(x, ШТкст(v));
}

template<>
inline цел полиСравни(const Ткст& x, const Значение& v) {
	return ткст_ли(v) && сравниЗнак((ШТкст)x, (ШТкст)v);
}

template<>
inline цел полиСравни(const бул& x, const Значение& v) {
	return v.является<дол>() ? сравниЗнак((дол)x, (дол)v)
	     : число_ли(v) ? сравниЗнак((дво)x, (дво)v)
	     : 0;
}

template<>
inline цел полиСравни(const цел& x, const Значение& v) {
	return v.является<дол>() ? сравниЗнак((дол)x, (дол)v)
	     : число_ли(v) ? сравниЗнак((дво)x, (дво)v)
	     : 0;
}

template<>
inline цел полиСравни(const дол& x, const Значение& v) {
	return v.является<дво>() ? сравниЗнак((дво)x, (дво)v)
	     : число_ли(v) ? сравниЗнак((дол)x, (дол)v)
	     : 0;
}

template<>
inline цел полиСравни(const дво& x, const Значение& v) {
	return число_ли(v) ? сравниЗнак((дво)x, (дво)v) : 0;
}

template<>
inline цел полиСравни(const Дата& x, const Значение& v) {
	return v.является<Время>() && сравниЗнак(воВремя(x), Время(v));
}

template<>
inline цел полиСравни(const Время& x, const Значение& v) {
	return v.является<Дата>() && сравниЗнак(воВремя(x), Время(v));
}

template<>
inline т_хэш ValueGetHashValue(const бул& x) {
	return РНЦП::дайХэшЗнач((дол)x);
}

template<>
inline т_хэш ValueGetHashValue(const цел& x) {
	return РНЦП::дайХэшЗнач((дол)x);
}

template<>
inline т_хэш ValueGetHashValue(const дво& x) {
	if(x >= (дво)INT64_MIN && x <= (дво)INT64_MAX && (дол)x == x)
		return РНЦП::дайХэшЗнач((дол)x); // we want this to be equal to other number types
	return РНЦП::дайХэшЗнач(x);
}

template<>
inline т_хэш ValueGetHashValue(const Дата& x) {
	return РНЦП::дайХэшЗнач(воВремя(x));
}

template<>
inline т_хэш ValueGetHashValue(const ШТкст& x) {
	return РНЦП::дайХэшЗнач(x.вТкст());
}

template <class T>
class RawValueRep : public Значение::Проц {
public:
	virtual бул  пусто_ли() const              { return false; }

	T v;

	enum VPICK { PICK };
	enum VDEEP { DEEP };
	
	const T& дай() const                      { return v; }
	T&       дай()                            { return v; }
	
	RawValueRep(const T& v) : v(v)             {}
	RawValueRep(T&& v, VPICK) : v(pick(v))     {}
	RawValueRep(const T& v, VDEEP) : v(v, 1)   {}
	RawValueRep()                              {}
};

template <class T>
class RichValueRep : public RawValueRep<T> {
public:
	virtual бул       пусто_ли() const                { return РНЦП::пусто_ли(this->v); }
	virtual проц       сериализуй(Поток& s)          { s % this->v; }
	virtual проц       вРяр(РярВВ& xio)            { ДинрусРНЦП::вРяр(xio, this->v); }
	virtual проц       вДжейсон(ДжейсонВВ& jio)          { ДинрусРНЦП::вДжейсон(jio, this->v); }
	virtual т_хэш     дайХэшЗнач() const          { return РНЦП::ValueGetHashValue(this->v); }
	virtual бул       равен(const Значение::Проц *p) { ПРОВЕРЬ(dynamic_cast<const RawValueRep<T> *>(p));
	                                                   return static_cast<const RawValueRep<T> *>(p)->дай() == this->v; }
	virtual бул       полиРавны(const Значение& b)   { return РНЦП::полиРавны(this->v, b); }
	virtual Ткст     какТкст() const              { return РНЦП::какТкст(this->v); }
	virtual цел        сравни(const Значение::Проц *p) { ПРОВЕРЬ(dynamic_cast<const RawValueRep<T> *>(p));
	                                                   return сравниЗнак(this->v, static_cast<const RawValueRep<T> *>(p)->дай()); }
	virtual цел        полиСравни(const Значение& b)   { return ДинрусРНЦП::полиСравни(this->v, b); }

	RichValueRep(const T& v) : RawValueRep<T>(v)     {}
	RichValueRep()                                   {}

	static Значение::Проц *создай()                     { return new RichValueRep; }
};

template <class T>
struct SvoFn {
	static бул       пусто_ли(const ук p)                      { return РНЦП::пусто_ли(*(T *)p); }
	static проц       сериализуй(ук p, Поток& s)              { s % *(T*)p; }
	static проц       вРяр(ук p, РярВВ& xio)                { ДинрусРНЦП::вРяр(xio, *(T*)p); }
	static проц       вДжейсон(ук p, ДжейсонВВ& jio)              { ДинрусРНЦП::вДжейсон(jio, *(T*)p); }
	static т_хэш     дайХэшЗнач(const ук p)                { return РНЦП::ValueGetHashValue(*(T*)p); }
	static бул       равен(const ук p1, const ук p2)    { return *(T*)p1 == *(T*)p2; }
	static бул       полиРавны(const ук p, const Значение& v) { return РНЦП::полиРавны(*(T*)p, v); }
	static Ткст     какТкст(const ук p)                    { return РНЦП::какТкст(*(T*)p); }
	static цел        сравни(const ук p1, const ук p2)    { return сравниЗнак(*(T*)p1, *(T*)p2); }
	static цел        полиСравни(const ук p1, const Значение& p2) { return РНЦП::полиСравни(*(T*)p1, p2); }
};

#define SVO_FN(ид, T) \
	static Значение::Sval ид = { \
		SvoFn<T>::пусто_ли, SvoFn<T>::сериализуй, SvoFn<T>::вРяр, SvoFn<T>::вДжейсон, \
		SvoFn<T>::дайХэшЗнач, SvoFn<T>::равен, \
		SvoFn<T>::полиРавны, SvoFn<T>::какТкст, \
		SvoFn<T>::сравни, SvoFn<T>::полиСравни \
	};

template <class T>
проц Значение::иницСмолл(const T& init)
{
	ПРОВЕРЬ(sizeof(T) <= 8);
	SVO_FN(sval, T)
	цел typeno = дайНомТипаЗнач<T>();
	ПРОВЕРЬ(typeno >= 0 && typeno < 256);
	svo[typeno] = &sval;
	данные.устОсобо(typeno);
	new(&данные) T(init);
}

template <class T>
inline T& Значение::дайСыройСмолл() const
{
	return *(T*)&данные; // Silence compiler warning
}

template <class T>
T& Значение::дайСмолл() const
{
	бцел t = дайНомТипаЗнач<T>();
	ПРОВЕРЬ(t < 255);
	if(является((ббайт)t))
		return дайСыройСмолл<T>();
	Ткст h = Ткст() << "Неверное преобразование значения: "
	                    << дайИмя() << " -> " << typeid(T).name();
	throw ОшибкаТипаЗначения(h, *this, t);
	return *(T*)&данные; // Silence compiler warning
}

template <class T>
проц Значение::SvoRegister(кткст0 имя)
{
	бцел t = дайНомТипаЗнач<T>();
	ПРОВЕРЬ(t < 255);
	SVO_FN(sval, T)
	svo[t] = &sval;
	добавьИмя(t, имя);
}

template <class T>
inline const T& Значение::To() const
{
	бцел t = дайНомТипаЗнач<T>();
	ПРОВЕРЬ(t != VALUEARRAY_V && t != VALUEMAP_V);
#ifndef _ОТЛАДКА
	if(t == VALUEARRAY_V) {
		ПРОВЕРЬ(укз()->дайТип() == VALUEARRAY_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
	if(t == VALUEMAP_V) {
		ПРОВЕРЬ(укз()->дайТип() == VALUEMAP_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
#endif
	if(реф_ли()) {
		const RawValueRep<T> *x = dynamic_cast<const RawValueRep<T>*>(укз());
		if(x)
			return x->дай();
	}
	else
	if(t == STRING_V) {
		ПРОВЕРЬ(ткст_ли());
		return *reinterpret_cast<const T*>(&данные); // Only active when T is Ткст
	}
	else
	if(t < 255 && является((ббайт)t))
		return дайСыройСмолл<T>();
	throw ОшибкаТипаЗначения(Ткст().кат() << "Неверное преобразование значения: "
	                     << дайИмя() << " -> " << typeid(T).name(),
	                     *this, t);
}

template <class T>
inline бул Значение::является() const
{
	бцел t = дайНомТипаЗнач<T>();
	if(реф_ли() && дайРефТип() == t)
		return true;
	if(t == STRING_V)
		return ткст_ли();
	if(t == VOID_V)
		return проц_ли();
	return t < 255 && является((ббайт)t);
}

inline
цел Значение::сравни(const Значение& v) const
{
	if(является(INT_V) && v.является(INT_V))
		return сравниЗнак(дайСыройСмолл<цел>(), v.дайСыройСмолл<цел>());
	if(является(DOUBLE_V) && v.является(DOUBLE_V))
		return сравниЗнак(дайСыройСмолл<дво>(), v.дайСыройСмолл<дво>());
	return сравни2(v);
}

template <class T>
const T& GetStaticNull()
{
	static T *q;
	ONCELOCK {
		static T x;
		устПусто(x);
		q = &x;
	}
	return *q;
}

template <class T>
inline const T& Значение::дай() const
{
#ifndef _ОТЛАДКА
	бцел t = дайНомТипаЗнач<T>();
	if(t == VALUEARRAY_V) {
		ПРОВЕРЬ(укз()->дайТип() == VALUEARRAY_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
	if(t == VALUEMAP_V) {
		ПРОВЕРЬ(укз()->дайТип() == VALUEMAP_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
#endif
	if(пусто_ли())
		return GetStaticNull<T>();
	return To<T>();
}

template <class T>
Значение::Значение(const T& x, VSMALL)
{
	иницСмолл(x);
}

inline
т_хэш Значение::дайХэшЗнач() const
{
	return ткст_ли() ? данные.дайСчёт() ? данные.дайХэшЗнач() : 0
	                  : дайДрЗначХэш();
}

template <class T>
проц Значение::регистрируй(кткст0 имя)
{
	бцел t = дайНомТипаЗнач<T>();
	Значение::регистрируй(t, RichValueRep<T>::создай, имя);
}

inline
const Значение& Значение::operator[](кткст0 ключ) const
{
	return operator[](Ткст(ключ));
}

inline
const Значение& Значение::operator[](const Ид& ключ) const
{
	return operator[](~ключ);
}

template <class T>
inline Значение RawToValue(const T& данные)
{
	return Значение(new RawValueRep<T>(данные), дайНомТипаЗнач<T>());
}

template <class T>
inline Значение RawPickToValue(T&& данные)
{
	typedef RawValueRep<T> R;
	return Значение(new R(pick(данные), R::PICK), дайНомТипаЗнач<T>());
}

template <class T>
inline Значение RawDeepToValue(const T& данные)
{
	typedef RawValueRep<T> R;
	return Значение(new R(данные, R::DEEP), дайНомТипаЗнач<T>());
}

template <class T>
inline T& CreateRawValue(Значение& v) {
	typedef RawValueRep<T> R;
	R *r = new R;
	v = Значение(r, дайНомТипаЗнач<T>());
	return r->дай();
}

template <class T>
inline Значение богатыйВЗнач(const T& данные)
{
	return Значение(new RichValueRep<T>(данные), дайНомТипаЗнач<T>());
}

template <>
inline Значение богатыйВЗнач(const Ткст& данные)
{
	return Значение(данные);
}

#ifdef DEPRECATED
template <class T> // use Значение::является
бул IsTypeRaw(const Значение& значение, T * = 0)                { return значение.является<T>(); }

template <class T> // use Значение::является
бул IsType(const Значение& x, T* = 0)                        { return x.является<T>(); }

template <class T>
struct СыроеЗначение : public Значение { // use RawToValue and Значение::To
	СыроеЗначение(const T& x) : Значение(RawToValue(x))            {}
	static const T& извлеки(const Значение& v)                { return v.To<T>(); }
	static const T& извлеки(const Значение& v, const T& dflt) { return v.является<T>() ? v.To<T>() : dflt; }
};

template <class T>
struct RichValue : public Значение { // use богатыйВЗнач and Значение::To
public:
	RichValue(const T& x) : Значение(богатыйВЗнач<T>(x))       {}
	static проц регистрируй() init_                           { Значение::регистрируй<T>(); }
	static const T& извлеки(const Значение& v)                { return v.дай<T>(); }
};

template <class T> // use Значение::To
inline const T& ValueTo(const Значение& v)                    { return v.To<T>(); }

template <class T> // use Значение::To
inline const T& ValueTo(const Значение& v, const T& dflt)     { return v.является<T>() ? v.To<T>() : dflt; }
#endif