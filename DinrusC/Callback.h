// Backward compatibility; use Функция/Событие in the new code

template <class... ТипыАрг>
class CallbackN : Движимое<CallbackN<ТипыАрг...>> {
	typedef Функция<void (ТипыАрг...)> Фн;

	Фн фн;

public:
	CallbackN() {}
	CallbackN(const CallbackN& ист) : фн(ист.фн)       {}
	CallbackN& operator=(const CallbackN& ист)         { фн = ист.фн; return *this; }

	CallbackN(Фн&& ист, int) : фн(pick(ист))       {} // Helper for callback compatibility code
	template <class F>
	CallbackN(F ист, int) : фн(ист)                {} // Helper for callback compatibility code
	
	CallbackN(CallbackN&& ист) : фн(pick(ист.фн))      {}
	CallbackN& operator=(CallbackN&& ист)              { фн = pick(ист.фн); return *this; }

	CallbackN(CNULLer)                             {}
	CallbackN& operator=(CNULLer)                  { фн.очисть(); return *this; }

	CallbackN прокси() const                        { return CallbackN(фн.прокси(), 1); }

	template <class F>
	CallbackN& operator<<(const F& f)              { фн << f; return *this; }

	template <class F>
	CallbackN& operator<<(F&& f)                   { фн << pick(f); return *this; }
	
	void operator()(ТипыАрг... арги) const    { return фн(арги...); }

	operator Фн() const                        { return фн; }
	operator bool() const                      { return фн; }
	void очисть()                               { фн.очисть(); }
	
	friend CallbackN прокси(const CallbackN& a)   { return a.прокси(); }
	friend void разверни(CallbackN& a, CallbackN& b) { РНЦП::разверни(a.фн, b.фн); }
};

// we need "isolation level" to avoid overloading issues
template <class... ТипыАрг>
class GateN : Движимое<GateN<ТипыАрг...>> {
	typedef Функция<bool (ТипыАрг...)> Фн;

	Фн фн;
	
	void уст(bool b) { if(b) фн = [](ТипыАрг...) { return true; }; else фн.очисть(); }

public:
	GateN()                                {}

	GateN(bool b)                          { уст(b); }
	GateN& operator=(bool b)               { уст(b); return *this; }

	GateN(const GateN& a) : фн(a.фн)   {}
	GateN& operator=(const GateN& a)   { фн = a.фн; return *this; }

	GateN(const Фн& ист) : фн(ист)   {}

	GateN(Фн&& ист, int) : фн(pick(ист))   {}
	GateN& operator=(GateN&& a)        { фн = pick(a.фн); return *this; }

	GateN(CNULLer)                         {}
	GateN& operator=(CNULLer)              { фн.очисть(); return *this; }

	GateN прокси() const                    { return фн.прокси(); }

	template <class F>
	GateN& operator<<(const F& f)          { фн << f; return *this; }

	template <class F>
	GateN& operator<<(F&& f)               { фн << pick(f); return *this; }
	
	bool operator()(ТипыАрг... арги) const    { return фн(арги...); }

	operator Фн() const                        { return фн; }
	operator bool() const                      { return фн; }
	void очисть()                               { фн.очисть(); }

	friend GateN прокси(const GateN& a)     { return a.прокси(); }
	friend void разверни(GateN& a, GateN& b)   { РНЦП::разверни(a.фн, b.фн); }
};

// backward compatibility
typedef CallbackN<> Callback;
template <class P1> using Обрвыз1 = CallbackN<P1>;
template <class P1, class P2> using Обрвыз2 = CallbackN<P1, P2>;
template <class P1, class P2, class P3> using Callback3 = CallbackN<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Callback4 = CallbackN<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Callback5 = CallbackN<P1, P2, P3, P4, P5>;

#define  Рез void
#define  Cb_ CallbackN
#include "CallbackR.i"

using Gate0 = GateN<>;
template <class P1> using Gate1 = GateN<P1>;
template <class P1, class P2> using Gate2 = GateN<P1, P2>;
template <class P1, class P2, class P3> using Gate3 = GateN<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Gate4 = GateN<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Gate5 = GateN<P1, P2, P3, P4, P5>;

#define  Рез bool
#define  Cb_ GateN
#include "CallbackR.i"

#define THISBACK(x)                  callback(this, &ИМЯ_КЛАССА::x)
#define THISBACK1(x, арг)            callback1(this, &ИМЯ_КЛАССА::x, арг)
#define THISBACK2(m, a, b)           callback2(this, &ИМЯ_КЛАССА::m, a, b)
#define THISBACK3(m, a, b, c)        callback3(this, &ИМЯ_КЛАССА::m, a, b, c)
#define THISBACK4(m, a, b, c, d)     callback4(this, &ИМЯ_КЛАССА::m, a, b, c, d)
#define THISBACK5(m, a, b, c, d, e)  callback5(this, &ИМЯ_КЛАССА::m, a, b, c, d,e)

#define PTEBACK(x)                   pteback(this, &ИМЯ_КЛАССА::x)
#define PTEBACK1(x, арг)             pteback1(this, &ИМЯ_КЛАССА::x, арг)
#define PTEBACK2(m, a, b)            pteback2(this, &ИМЯ_КЛАССА::m, a, b)
#define PTEBACK3(m, a, b, c)         pteback3(this, &ИМЯ_КЛАССА::m, a, b, c)
#define PTEBACK4(m, a, b, c, d)      pteback4(this, &ИМЯ_КЛАССА::m, a, b, c, d)
#define PTEBACK5(m, a, b, c, d, e)   pteback5(this, &ИМЯ_КЛАССА::m, a, b, c, d, e)

#define STDBACK(x)                   callback(&x)
#define STDBACK1(x, арг)             callback1(&x, арг)
#define STDBACK2(m, a, b)            callback2(&m, a, b)
#define STDBACK3(m, a, b, c)         callback3(&m, a, b, c)
#define STDBACK4(m, a, b, c, d)      callback4(&m, a, b, c, d)
#define STDBACK5(m, a, b, c, d, e)   callback5(&m, a, b, c, d, e)


template <class T>
class CallbackNArgTarget
{
	T result;

	void SetResult(const T& значение)      { result = значение; }
	void уст(T значение)                   { result = значение; }

public:
	typedef CallbackNArgTarget ИМЯ_КЛАССА;

	operator const T&() const           { return result; }
	bool экзПусто_ли() const         { return пусто_ли(result); }

	Callback operator[](const T& значение) { return THISBACK1(SetResult, значение); }
	operator Обрвыз1<const T&>()      { return THISBACK(SetResult); }
	operator Обрвыз1<T>()             { return THISBACK(уст); }

	CallbackNArgTarget()                    { result = Null; }
};

template <class T>
using CallbackArgTarget = CallbackNArgTarget<T>;
