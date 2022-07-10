const dword COMPLEX_V   = 20;

struct Комплекс : std::complex<double>
{
	typedef std::complex<double> C;

	Комплекс()                               {}
	Комплекс(double r) : C(r)                {}
	Комплекс(double r, double i) : C(r,i)    {}
	Комплекс(const C& c) : C(c)              {}

	Комплекс(const Обнул&) : C(DOUBLE_NULL, DOUBLE_NULL) {}
	operator Значение() const                  { return богатыйВЗнач(*this); }
	Комплекс(const Значение& v) : C(число_ли(v) ? Комплекс((double)v) : v.дай<Комплекс>()) {}

	operator Реф()                          { return какРеф(*this); }

	bool operator==(const Комплекс& c) const { return (const C&)(*this) == (const C&)c; }
	bool operator!=(const Комплекс& c) const { return (const C&)(*this) != (const C&)c; }
	
	int  сравни(const Комплекс& c) const    { NEVER(); return 0; }

	void сериализуй(Поток& s);
	void вРяр(РярВВ& xio);
	void вДжейсон(ДжейсонВВ& jio);
};

template<> inline bool пусто_ли(const Комплекс& r) { return пусто_ли(r.real()) || пусто_ли(r.imag()); }
template<> inline hash_t дайХэшЗнач(const Комплекс& x) { return комбинируйХэш(x.real(), x.imag()); }
template<> inline Ткст какТкст(const std::complex<double>& x) { return Ткст().конкат() << "(" << x.real() << "," << x.imag() << ")"; }
template<> inline Ткст какТкст(const Комплекс& x) { return какТкст((const std::complex<double>&)x); }

template<> inline dword номерТипаЗнач(const Комплекс*) { return COMPLEX_V; }

inline double абс(const Комплекс& c) { return std::abs(c); }

inline const Комплекс& Nvl(const Комплекс& a, const Комплекс& b)  { return пусто_ли(a) ? b : a; }

template<>
inline bool полиРавны(const Комплекс& x, const Значение& v) {
	return число_ли(v) && x.imag() == 0 && x.real() == (double)v;
}

template<>
inline int  полиСравни(const Комплекс& a, const Значение& b)
{
	NEVER(); return 0;
}

inline bool НЧ_ли(const Комплекс& x)        { return НЧ_ли(x.real()) || НЧ_ли(x.imag()); }
inline bool БЕСК_ли(const Комплекс& x)        { return БЕСК_ли(x.real()) || БЕСК_ли(x.imag()); }
inline bool конечен(const Комплекс& x)        { return конечен(x.real()) && конечен(x.imag()); }

СРАВНИ_ЗНАЧЕНИЕ(Комплекс)
NTL_MOVEABLE(Комплекс)
