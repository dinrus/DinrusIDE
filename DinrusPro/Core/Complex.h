const бцел COMPLEX_V   = 20;

struct Комплекс : std::complex<дво>
{
	typedef std::complex<дво> C;

	Комплекс()                               {}
	Комплекс(дво r) : C(r)                {}
	Комплекс(дво r, дво i) : C(r,i)    {}
	Комплекс(const C& c) : C(c)              {}

	Комплекс(const Обнул&) : C(DOUBLE_NULL, DOUBLE_NULL) {}
	operator Значение() const                  { return богатыйВЗнач(*this); }
	Комплекс(const Значение& v) : C(число_ли(v) ? Комплекс((дво)v) : v.дай<Комплекс>()) {}

	operator Реф()                          { return какРеф(*this); }

	бул operator==(const Комплекс& c) const { return (const C&)(*this) == (const C&)c; }
	бул operator!=(const Комплекс& c) const { return (const C&)(*this) != (const C&)c; }
	
	цел  сравни(const Комплекс& c) const    { НИКОГДА(); return 0; }

	проц сериализуй(Поток& s);
	проц вРяр(РярВВ& xio);
	проц вДжейсон(ДжейсонВВ& jio);
};

template<> inline бул пусто_ли(const Комплекс& r) { return пусто_ли(r.real()) || пусто_ли(r.imag()); }
template<> inline т_хэш дайХэшЗнач(const Комплекс& x) { return комбинируйХэш(x.real(), x.imag()); }
template<> inline Ткст какТкст(const std::complex<дво>& x) { return Ткст().кат() << "(" << x.real() << "," << x.imag() << ")"; }
template<> inline Ткст какТкст(const Комплекс& x) { return какТкст((const std::complex<дво>&)x); }

template<> inline бцел номерТипаЗнач(const Комплекс*) { return COMPLEX_V; }

inline дво абс(const Комплекс& c) { return std::abs(c); }

inline const Комплекс& Nvl(const Комплекс& a, const Комплекс& b)  { return пусто_ли(a) ? b : a; }

template<>
inline бул полиРавны(const Комплекс& x, const Значение& v) {
	return число_ли(v) && x.imag() == 0 && x.real() == (дво)v;
}

template<>
inline цел  полиСравни(const Комплекс& a, const Значение& b)
{
	НИКОГДА(); return 0;
}

inline бул нч_ли(const Комплекс& x)        { return нч_ли(x.real()) || нч_ли(x.imag()); }
inline бул беск_ли(const Комплекс& x)        { return беск_ли(x.real()) || беск_ли(x.imag()); }
inline бул конечен(const Комплекс& x)        { return конечен(x.real()) && конечен(x.imag()); }

СРАВНИ_ЗНАЧЕНИЕ(Комплекс)
NTL_MOVEABLE(Комплекс)
