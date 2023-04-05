// Backward compatibility; use Функция/Событие in the new код_

template <class... ТипыАрг>
class ОбрВызН : Движимое<ОбрВызН<ТипыАрг...>> {
	typedef Функция<void (ТипыАрг...)> Фн;

	Фн фн;

public:
	ОбрВызН() {}
	ОбрВызН(const ОбрВызН& ист) : фн(ист.фн)       {}
	ОбрВызН& operator=(const ОбрВызН& ист)         { фн = ист.фн; return *this; }

	ОбрВызН(Фн&& ист, int) : фн(pick(ист))       {} // Helper for обрвыз compatibility код_
	template <class F>
	ОбрВызН(F ист, int) : фн(ист)                {} // Helper for обрвыз compatibility код_
	
	ОбрВызН(ОбрВызН&& ист) : фн(pick(ист.фн))      {}
	ОбрВызН& operator=(ОбрВызН&& ист)              { фн = pick(ист.фн); return *this; }

	ОбрВызН(CNULLer)                             {}
	ОбрВызН& operator=(CNULLer)                  { фн.очисть(); return *this; }

	ОбрВызН прокси() const                        { return ОбрВызН(фн.прокси(), 1); }

	template <class F>
	ОбрВызН& operator<<(const F& f)              { фн << f; return *this; }

	template <class F>
	ОбрВызН& operator<<(F&& f)                   { фн << pick(f); return *this; }
	
	void operator()(ТипыАрг... арги) const    { return фн(арги...); }

	operator Фн() const                        { return фн; }
	operator bool() const                      { return фн; }
	void очисть()                               { фн.очисть(); }
	
	friend ОбрВызН прокси(const ОбрВызН& a)   { return a.прокси(); }
	friend void разверни(ОбрВызН& a, ОбрВызН& b) { РНЦП::разверни(a.фн, b.фн); }
};

// we need "isolation level" to avoid overloading issues
template <class... ТипыАрг>
class ЛазН : Движимое<ЛазН<ТипыАрг...>> {
	typedef Функция<bool (ТипыАрг...)> Фн;

	Фн фн;
	
	void уст(bool b) { if(b) фн = [](ТипыАрг...) { return true; }; else фн.очисть(); }

public:
	ЛазН()                                {}

	ЛазН(bool b)                          { уст(b); }
	ЛазН& operator=(bool b)               { уст(b); return *this; }

	ЛазН(const ЛазН& a) : фн(a.фн)   {}
	ЛазН& operator=(const ЛазН& a)   { фн = a.фн; return *this; }

	ЛазН(const Фн& ист) : фн(ист)   {}

	ЛазН(Фн&& ист, int) : фн(pick(ист))   {}
	ЛазН& operator=(ЛазН&& a)        { фн = pick(a.фн); return *this; }

	ЛазН(CNULLer)                         {}
	ЛазН& operator=(CNULLer)              { фн.очисть(); return *this; }

	ЛазН прокси() const                    { return фн.прокси(); }

	template <class F>
	ЛазН& operator<<(const F& f)          { фн << f; return *this; }

	template <class F>
	ЛазН& operator<<(F&& f)               { фн << pick(f); return *this; }
	
	bool operator()(ТипыАрг... арги) const    { return фн(арги...); }

	operator Фн() const                        { return фн; }
	operator bool() const                      { return фн; }
	void очисть()                               { фн.очисть(); }

	friend ЛазН прокси(const ЛазН& a)     { return a.прокси(); }
	friend void разверни(ЛазН& a, ЛазН& b)   { РНЦП::разверни(a.фн, b.фн); }
};

// backward compatibility
typedef ОбрВызН<> Обрвыз;
template <class P1> using Обрвыз1 = ОбрВызН<P1>;
template <class P1, class P2> using Обрвыз2 = ОбрВызН<P1, P2>;
template <class P1, class P2, class P3> using Обрвыз3 = ОбрВызН<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Обрвыз4 = ОбрВызН<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Обрвыз5 = ОбрВызН<P1, P2, P3, P4, P5>;

#define  Рез void
#define  Ов_ ОбрВызН
#include "CallbackR.i"

using Лаз0 = ЛазН<>;
template <class P1> using Лаз5 = ЛазН<P1>;
template <class P1, class P2> using Лаз2 = ЛазН<P1, P2>;
template <class P1, class P2, class P3> using Лаз3 = ЛазН<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Лаз4 = ЛазН<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Gate5 = ЛазН<P1, P2, P3, P4, P5>;

#define  Рез bool
#define  Ов_ ЛазН
#include "CallbackR.i"

#define САМОБР(x)                  обрвыз(this, &ИМЯ_КЛАССА::x)
#define САМОБР1(x, арг)            обрвыз1(this, &ИМЯ_КЛАССА::x, арг)
#define САМОБР2(m, a, b)           обрвыз2(this, &ИМЯ_КЛАССА::m, a, b)
#define САМОБР3(m, a, b, c)        обрвыз3(this, &ИМЯ_КЛАССА::m, a, b, c)
#define САМОБР4(m, a, b, c, d)     обрвыз4(this, &ИМЯ_КЛАССА::m, a, b, c, d)
#define САМОБР5(m, a, b, c, d, e)  обрвыз5(this, &ИМЯ_КЛАССА::m, a, b, c, d,e)

#define ПТЕОБР(x)                   птеобр(this, &ИМЯ_КЛАССА::x)
#define ПТЕОБР1(x, арг)             птеобр1(this, &ИМЯ_КЛАССА::x, арг)
#define ПТЕОБР2(m, a, b)            птеобр2(this, &ИМЯ_КЛАССА::m, a, b)
#define ПТЕОБР3(m, a, b, c)         птеобр3(this, &ИМЯ_КЛАССА::m, a, b, c)
#define ПТЕОБР4(m, a, b, c, d)      птеобр4(this, &ИМЯ_КЛАССА::m, a, b, c, d)
#define ПТЕОБР5(m, a, b, c, d, e)   птеобр5(this, &ИМЯ_КЛАССА::m, a, b, c, d, e)

#define СТДОБР(x)                   обрвыз(&x)
#define СТДОБР1(x, арг)             обрвыз1(&x, арг)
#define СТДОБР2(m, a, b)            обрвыз2(&m, a, b)
#define СТДОБР3(m, a, b, c)         обрвыз3(&m, a, b, c)
#define СТДОБР4(m, a, b, c, d)      обрвыз4(&m, a, b, c, d)
#define СТДОБР5(m, a, b, c, d, e)   обрвыз5(&m, a, b, c, d, e)


template <class T>
class ЦельАргаОбрвызН
{
	T result;

	void устРезульт(const T& значение)      { result = значение; }
	void уст(T значение)                   { result = значение; }

public:
	typedef ЦельАргаОбрвызН ИМЯ_КЛАССА;

	operator const T&() const           { return result; }
	bool экзПусто_ли() const         { return пусто_ли(result); }

	Обрвыз operator[](const T& значение) { return САМОБР1(устРезульт, значение); }
	operator Обрвыз1<const T&>()      { return САМОБР(устРезульт); }
	operator Обрвыз1<T>()             { return САМОБР(уст); }

	ЦельАргаОбрвызН()                    { result = Null; }
};

template <class T>
using ЦельАргаОбрвыз = ЦельАргаОбрвызН<T>;
