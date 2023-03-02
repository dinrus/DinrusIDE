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
inline bool полиРавны(const bool& x, const Значение& v) {
	return v.является<double>() && int(x) == double(v)
	    || v.является<int64>() && int(x) == int64(v)
	    || v.является<int>() && int(x) == int(v);
}

template<>
inline bool полиРавны(const int& x, const Значение& v) {
	return v.является<double>() && x == double(v)
	    || v.является<int64>() && x == int64(v);
}

template<>
inline bool полиРавны(const int64& x, const Значение& v) {
	return v.является<double>() && double(x) == double(v);
}

template<>
inline bool полиРавны(const Дата& x, const Значение& v) {
	return v.является<Время>() && воВремя(x) == Время(v);
}

template<>
inline bool полиРавны(const ШТкст& x, const Значение& v) {
	return v.дайТип() == STRING_V && ШТкст(v) == x;
}

template<class T>
inline int  полиСравни(const T& a, const Значение& b)
{
	return a.полиСравни(b);
}

template<>
inline int полиСравни(const ШТкст& x, const Значение& v) {
	return ткст_ли(v) && сравниЗнак(x, ШТкст(v));
}

template<>
inline int полиСравни(const Ткст& x, const Значение& v) {
	return ткст_ли(v) && сравниЗнак((ШТкст)x, (ШТкст)v);
}

template<>
inline int полиСравни(const bool& x, const Значение& v) {
	return v.является<int64>() ? сравниЗнак((int64)x, (int64)v)
	     : число_ли(v) ? сравниЗнак((double)x, (double)v)
	     : 0;
}

template<>
inline int полиСравни(const int& x, const Значение& v) {
	return v.является<int64>() ? сравниЗнак((int64)x, (int64)v)
	     : число_ли(v) ? сравниЗнак((double)x, (double)v)
	     : 0;
}

template<>
inline int полиСравни(const int64& x, const Значение& v) {
	return v.является<double>() ? сравниЗнак((double)x, (double)v)
	     : число_ли(v) ? сравниЗнак((int64)x, (int64)v)
	     : 0;
}

template<>
inline int полиСравни(const double& x, const Значение& v) {
	return число_ли(v) ? сравниЗнак((double)x, (double)v) : 0;
}

template<>
inline int полиСравни(const Дата& x, const Значение& v) {
	return v.является<Время>() && сравниЗнак(воВремя(x), Время(v));
}

template<>
inline int полиСравни(const Время& x, const Значение& v) {
	return v.является<Дата>() && сравниЗнак(воВремя(x), Время(v));
}

template<>
inline hash_t ValueGetHashValue(const bool& x) {
	return РНЦП::дайХэшЗнач((int64)x);
}

template<>
inline hash_t ValueGetHashValue(const int& x) {
	return РНЦП::дайХэшЗнач((int64)x);
}

template<>
inline hash_t ValueGetHashValue(const double& x) {
	if(x >= (double)INT64_MIN && x <= (double)INT64_MAX && (int64)x == x)
		return РНЦП::дайХэшЗнач((int64)x); // we want this to be equal to other number types
	return РНЦП::дайХэшЗнач(x);
}

template<>
inline hash_t ValueGetHashValue(const Дата& x) {
	return РНЦП::дайХэшЗнач(воВремя(x));
}

template<>
inline hash_t ValueGetHashValue(const ШТкст& x) {
	return РНЦП::дайХэшЗнач(x.вТкст());
}

template <class T>
class RawValueRep : public Значение::Проц {
public:
	virtual bool  пусто_ли() const              { return false; }

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
	virtual bool       пусто_ли() const                { return РНЦП::пусто_ли(this->v); }
	virtual void       сериализуй(Поток& s)          { s % this->v; }
	virtual void       вРяр(РярВВ& xio)            { РНЦПДинрус::вРяр(xio, this->v); }
	virtual void       вДжейсон(ДжейсонВВ& jio)          { РНЦПДинрус::вДжейсон(jio, this->v); }
	virtual hash_t     дайХэшЗнач() const          { return РНЦП::ValueGetHashValue(this->v); }
	virtual bool       равен(const Значение::Проц *p) { ПРОВЕРЬ(dynamic_cast<const RawValueRep<T> *>(p));
	                                                   return static_cast<const RawValueRep<T> *>(p)->дай() == this->v; }
	virtual bool       полиРавны(const Значение& b)   { return РНЦП::полиРавны(this->v, b); }
	virtual Ткст     какТкст() const              { return РНЦП::какТкст(this->v); }
	virtual int        сравни(const Значение::Проц *p) { ПРОВЕРЬ(dynamic_cast<const RawValueRep<T> *>(p));
	                                                   return сравниЗнак(this->v, static_cast<const RawValueRep<T> *>(p)->дай()); }
	virtual int        полиСравни(const Значение& b)   { return РНЦПДинрус::полиСравни(this->v, b); }

	RichValueRep(const T& v) : RawValueRep<T>(v)     {}
	RichValueRep()                                   {}

	static Значение::Проц *создай()                     { return new RichValueRep; }
};

template <class T>
struct SvoFn {
	static bool       пусто_ли(const void *p)                      { return РНЦП::пусто_ли(*(T *)p); }
	static void       сериализуй(void *p, Поток& s)              { s % *(T*)p; }
	static void       вРяр(void *p, РярВВ& xio)                { РНЦПДинрус::вРяр(xio, *(T*)p); }
	static void       вДжейсон(void *p, ДжейсонВВ& jio)              { РНЦПДинрус::вДжейсон(jio, *(T*)p); }
	static hash_t     дайХэшЗнач(const void *p)                { return РНЦП::ValueGetHashValue(*(T*)p); }
	static bool       равен(const void *p1, const void *p2)    { return *(T*)p1 == *(T*)p2; }
	static bool       полиРавны(const void *p, const Значение& v) { return РНЦП::полиРавны(*(T*)p, v); }
	static Ткст     какТкст(const void *p)                    { return РНЦП::какТкст(*(T*)p); }
	static int        сравни(const void *p1, const void *p2)    { return сравниЗнак(*(T*)p1, *(T*)p2); }
	static int        полиСравни(const void *p1, const Значение& p2) { return РНЦП::полиСравни(*(T*)p1, p2); }
};

#define SVO_FN(ид, T) \
	static Значение::Sval ид = { \
		SvoFn<T>::пусто_ли, SvoFn<T>::сериализуй, SvoFn<T>::вРяр, SvoFn<T>::вДжейсон, \
		SvoFn<T>::дайХэшЗнач, SvoFn<T>::равен, \
		SvoFn<T>::полиРавны, SvoFn<T>::какТкст, \
		SvoFn<T>::сравни, SvoFn<T>::полиСравни \
	};

template <class T>
void Значение::иницСмолл(const T& init)
{
	ПРОВЕРЬ(sizeof(T) <= 8);
	SVO_FN(sval, T)
	int typeno = дайНомТипаЗнач<T>();
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
	dword t = дайНомТипаЗнач<T>();
	ПРОВЕРЬ(t < 255);
	if(является((byte)t))
		return дайСыройСмолл<T>();
	Ткст h = Ткст() << "Неверное преобразование значения: "
	                    << дайИмя() << " -> " << typeid(T).name();
	throw ОшибкаТипаЗначения(h, *this, t);
	return *(T*)&данные; // Silence compiler warning
}

template <class T>
void Значение::SvoRegister(const char *имя)
{
	dword t = дайНомТипаЗнач<T>();
	ПРОВЕРЬ(t < 255);
	SVO_FN(sval, T)
	svo[t] = &sval;
	добавьИмя(t, имя);
}

template <class T>
inline const T& Значение::To() const
{
	dword t = дайНомТипаЗнач<T>();
	ПРОВЕРЬ(t != VALUEARRAY_V && t != VALUEMAP_V);
#ifndef _ОТЛАДКА
	if(t == VALUEARRAY_V) {
		ПРОВЕРЬ(ptr()->дайТип() == VALUEARRAY_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
	if(t == VALUEMAP_V) {
		ПРОВЕРЬ(ptr()->дайТип() == VALUEMAP_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
#endif
	if(реф_ли()) {
		const RawValueRep<T> *x = dynamic_cast<const RawValueRep<T>*>(ptr());
		if(x)
			return x->дай();
	}
	else
	if(t == STRING_V) {
		ПРОВЕРЬ(ткст_ли());
		return *reinterpret_cast<const T*>(&данные); // Only active when T is Ткст
	}
	else
	if(t < 255 && является((byte)t))
		return дайСыройСмолл<T>();
	throw ОшибкаТипаЗначения(Ткст().конкат() << "Неверное преобразование значения: "
	                     << дайИмя() << " -> " << typeid(T).name(),
	                     *this, t);
}

template <class T>
inline bool Значение::является() const
{
	dword t = дайНомТипаЗнач<T>();
	if(реф_ли() && дайРефТип() == t)
		return true;
	if(t == STRING_V)
		return ткст_ли();
	if(t == VOID_V)
		return проц_ли();
	return t < 255 && является((byte)t);
}

inline
int Значение::сравни(const Значение& v) const
{
	if(является(INT_V) && v.является(INT_V))
		return сравниЗнак(дайСыройСмолл<int>(), v.дайСыройСмолл<int>());
	if(является(DOUBLE_V) && v.является(DOUBLE_V))
		return сравниЗнак(дайСыройСмолл<double>(), v.дайСыройСмолл<double>());
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
	dword t = дайНомТипаЗнач<T>();
	if(t == VALUEARRAY_V) {
		ПРОВЕРЬ(ptr()->дайТип() == VALUEARRAY_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
	if(t == VALUEMAP_V) {
		ПРОВЕРЬ(ptr()->дайТип() == VALUEMAP_V);
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
hash_t Значение::дайХэшЗнач() const
{
	return ткст_ли() ? данные.дайСчёт() ? данные.дайХэшЗнач() : 0
	                  : дайДрЗначХэш();
}

template <class T>
void Значение::регистрируй(const char *имя)
{
	dword t = дайНомТипаЗнач<T>();
	Значение::регистрируй(t, RichValueRep<T>::создай, имя);
}

inline
const Значение& Значение::operator[](const char *ключ) const
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
bool IsTypeRaw(const Значение& значение, T * = 0)                { return значение.является<T>(); }

template <class T> // use Значение::является
bool IsType(const Значение& x, T* = 0)                        { return x.является<T>(); }

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
	static void регистрируй() init_                           { Значение::регистрируй<T>(); }
	static const T& извлеки(const Значение& v)                { return v.дай<T>(); }
};

template <class T> // use Значение::To
inline const T& ValueTo(const Значение& v)                    { return v.To<T>(); }

template <class T> // use Значение::To
inline const T& ValueTo(const Значение& v, const T& dflt)     { return v.является<T>() ? v.To<T>() : dflt; }
#endif