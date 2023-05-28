// Backward compatibility; use Функция/Событие in the new код_

template <class... ТипыАрг>
class ОбрВызН : Движ<ОбрВызН<ТипыАрг...>> {
	typedef Функция<проц (ТипыАрг...)> Фн;

	Фн фн;

public:
	ОбрВызН() {}
	ОбрВызН(const ОбрВызН& ист) : фн(ист.фн)       {}
	ОбрВызН& operator=(const ОбрВызН& ист)         { фн = ист.фн; return *this; }

	ОбрВызН(Фн&& ист, цел) : фн(пикуй(ист))       {} // Helper for обрвыз compatibility код_
	template <class F>
	ОбрВызН(F ист, цел) : фн(ист)                {} // Helper for обрвыз compatibility код_
	
	ОбрВызН(ОбрВызН&& ист) : фн(пикуй(ист.фн))      {}
	ОбрВызН& operator=(ОбрВызН&& ист)              { фн = пикуй(ист.фн); return *this; }

	ОбрВызН(CNULLer)                             {}
	ОбрВызН& operator=(CNULLer)                  { фн.очисть(); return *this; }

	ОбрВызН прокси() const                        { return ОбрВызН(фн.прокси(), 1); }

	template <class F>
	ОбрВызН& operator<<(const F& f)              { фн << f; return *this; }

	template <class F>
	ОбрВызН& operator<<(F&& f)                   { фн << пикуй(f); return *this; }
	
	проц operator()(ТипыАрг... арги) const    { return фн(арги...); }

	operator Фн() const                        { return фн; }
	operator бул() const                      { return фн; }
	проц очисть()                               { фн.очисть(); }
	
	friend ОбрВызН прокси(const ОбрВызН& a)   { return a.прокси(); }
	friend проц разверни(ОбрВызН& a, ОбрВызН& b) { РНЦП::разверни(a.фн, b.фн); }
};

// we need "isolation level" to avoid overloading issues
template <class... ТипыАрг>
class ЛазН : Движ<ЛазН<ТипыАрг...>> {
	typedef Функция<бул (ТипыАрг...)> Фн;

	Фн фн;
	
	проц уст(бул b) { if(b) фн = [](ТипыАрг...) { return true; }; else фн.очисть(); }

public:
	ЛазН()                                {}

	ЛазН(бул b)                          { уст(b); }
	ЛазН& operator=(бул b)               { уст(b); return *this; }

	ЛазН(const ЛазН& a) : фн(a.фн)   {}
	ЛазН& operator=(const ЛазН& a)   { фн = a.фн; return *this; }

	ЛазН(const Фн& ист) : фн(ист)   {}

	ЛазН(Фн&& ист, цел) : фн(пикуй(ист))   {}
	ЛазН& operator=(ЛазН&& a)        { фн = пикуй(a.фн); return *this; }

	ЛазН(CNULLer)                         {}
	ЛазН& operator=(CNULLer)              { фн.очисть(); return *this; }

	ЛазН прокси() const                    { return фн.прокси(); }

	template <class F>
	ЛазН& operator<<(const F& f)          { фн << f; return *this; }

	template <class F>
	ЛазН& operator<<(F&& f)               { фн << пикуй(f); return *this; }
	
	бул operator()(ТипыАрг... арги) const    { return фн(арги...); }

	operator Фн() const                        { return фн; }
	operator бул() const                      { return фн; }
	проц очисть()                               { фн.очисть(); }

	friend ЛазН прокси(const ЛазН& a)     { return a.прокси(); }
	friend проц разверни(ЛазН& a, ЛазН& b)   { РНЦП::разверни(a.фн, b.фн); }
};

// backward compatibility
typedef ОбрВызН<> Обрвыз;
template <class P1> using Обрвыз1 = ОбрВызН<P1>;
template <class P1, class P2> using Обрвыз2 = ОбрВызН<P1, P2>;
template <class P1, class P2, class P3> using Обрвыз3 = ОбрВызН<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Обрвыз4 = ОбрВызН<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Обрвыз5 = ОбрВызН<P1, P2, P3, P4, P5>;

#define  Рез проц
#define  Ов_ ОбрВызН
#include "CallbackR.i"

using Лаз0 = ЛазН<>;
template <class P1> using Лаз5 = ЛазН<P1>;
template <class P1, class P2> using Лаз2 = ЛазН<P1, P2>;
template <class P1, class P2, class P3> using Лаз3 = ЛазН<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Лаз4 = ЛазН<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Gate5 = ЛазН<P1, P2, P3, P4, P5>;

#define  Рез бул
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

	проц устРезульт(const T& значение)      { result = значение; }
	проц уст(T значение)                   { result = значение; }

public:
	typedef ЦельАргаОбрвызН ИМЯ_КЛАССА;

	operator const T&() const           { return result; }
	бул экзПусто_ли() const         { return пусто_ли(result); }

	Обрвыз operator[](const T& значение) { return САМОБР1(устРезульт, значение); }
	operator Обрвыз1<const T&>()      { return САМОБР(устРезульт); }
	operator Обрвыз1<T>()             { return САМОБР(уст); }

	ЦельАргаОбрвызН()                    { result = Null; }
};

template <class T>
using ЦельАргаОбрвыз = ЦельАргаОбрвызН<T>;
