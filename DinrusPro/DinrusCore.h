#ifndef _DinrusPro_DinrusCore_h_
#define _DinrusPro_DinrusCore_h_

#include <DinrusPro/DinrusPro.h>
#include <DinrusPro/Funcs.h>
#include <DinrusPro/Tpl.h>
#include <DinrusPro/Ops.h>

/////////////////////////////
ПИ_РНЦП//Пространство имён//
///////////////////////////
///////////
class Дата;
class Время;
class Поток;
class ТкстБуф;
class ШТкстБуф;
class   Ид;
class   Значение;
class   МассивЗнач;
class   МапЗнач;
class   РярВВ;
class   ДжейсонВВ;

class   Реф;
struct  РефНаТипЗнач;

template <class T>
проц вДжейсон(ДжейсонВВ& io, T& var);

template <class T>
проц вРяр(РярВВ& xml, T& var);

проц  освободиПам(ук укз);
цел  цпбЯдра();
//////////////

#ifdef _ОТЛАДКА
проц __LOGF__(кткст0 формат, ...);
#define LOGF             РНЦП::__LOGF__
#else
inline проц __LOGF__(кткст0 формат, ...);
#endif

template <class T>
проц IGNORE_RESULT(const T&) {}

#ifdef flagCHECKINIT
проц начниИницБлок__(кткст0 фн, цел line);
проц завершиИницБлок__(кткст0 фн, цел line);
#else
inline проц начниИницБлок__(const char *, цел) {}
inline проц завершиИницБлок__(const char *, цел) {}
#endif

struct ИницВызов {
	ИницВызов(проц (*фн)(), кткст0 cpp, цел line) { начниИницБлок__(cpp, line); фн(); завершиИницБлок__(cpp, line); }
	ИницВызов(проц (*фн)())                            { фн(); }
};

struct ВыходИзВызова {
	ВыходИзВызова(проц (*фн)()) { atexit(фн); }
};

//////////////////////////////////
//Topt.h
struct ПустойКласс {};

template <class T, class B = ПустойКласс>
class ОпыРеляций : public B
{
public:
	friend бул operator >  (const T& a, const T& b)  { return b < a; }
	friend бул operator != (const T& a, const T& b)  { return !(a == b); }
	friend бул operator <= (const T& a, const T& b)  { return !(b < a); }
	friend бул operator >= (const T& a, const T& b)  { return !(a < b); }
};

template <class U, class V, class B = ПустойКласс>
class ОпыСложения : public B
{
public:
	friend U& operator -= (U& a, const V& b)          { a += -b; return a; }
	friend U  operator +  (const U& a, const V& b)    { U x(a); x += b; return x; }
	friend U  operator -  (const U& a, const V& b)    { U x(a); x += -b; return x; }
};

template <class T, class B = ПустойКласс>
class ОпыПостфикс : public B
{
public:
	friend T operator ++ (T& i, цел)                  { T x = i; ++i; return x; }
	friend T operator -- (T& i, цел)                  { T x = i; --i; return x; }
};

template <class T, цел (*сравни)(T a, T b), class B = ПустойКласс>
class ОпыРелСравнения : public B
{
public:
	friend бул operator <  (T a, T b) { return (*сравни)(a, b) <  0; }
	friend бул operator >  (T a, T b) { return (*сравни)(a, b) >  0; }
	friend бул operator == (T a, T b) { return (*сравни)(a, b) == 0; }
	friend бул operator != (T a, T b) { return (*сравни)(a, b) != 0; }
	friend бул operator <= (T a, T b) { return (*сравни)(a, b) <= 0; }
	friend бул operator >= (T a, T b) { return (*сравни)(a, b) >= 0; }
};

template <class T, class B = ПустойКласс>
struct Сравнимое : public B
{
public:
	friend бул operator <  (const T& a, const T& b) { return a.сравни(b) < 0; }
	friend бул operator >  (const T& a, const T& b) { return a.сравни(b) > 0; }
	friend бул operator == (const T& a, const T& b) { return a.сравни(b) == 0; }
	friend бул operator != (const T& a, const T& b) { return a.сравни(b) != 0; }
	friend бул operator <= (const T& a, const T& b) { return a.сравни(b) <= 0; }
	friend бул operator >= (const T& a, const T& b) { return a.сравни(b) >= 0; }
	friend цел  сравниЗнак(const T& a, const T& b)   { return a.сравни(b); }
};


struct КомбСравни {
	цел result;

	template <class T>
	КомбСравни& operator()(const T& a, const T& b) { if(!result) result = сравниЗнак(a, b); return *this; }

	operator цел() const                               { return result; }

	КомбСравни()                                   { result = 0; }

	template <class T>
	КомбСравни(const T& a, const T& b)             { result = 0; operator()(a, b); }
};

#ifdef NO_MOVEABLE_CHECK

template <class T>
inline проц проверьДвиж(T *) {}

#define MoveableTemplate(T)

template <class T, class B = ПустойКласс>
class Движ : public B
{
};

template <class T>
struct Движ_ {
};

#define NTL_MOVEABLE(T)

#else

template <class T>
inline проц подтвердиДвижУкз(T, T) {}

template <class T>
inline проц подтвердиДвиж0(T *t) { подтвердиДвижУкз(&**t, *t); }
// COMPILATION Ошибка HERE MEANS TYPE T WAS NOT MARKED AS Движ

template <class T, class B = ПустойКласс>
struct Движ : public B {
	friend проц подтвердиДвиж0(T *) {}
};

template <class T>
struct Движ_ {
	friend проц подтвердиДвиж0(T *) {}
};

template <class T>
inline проц проверьДвиж(T *t = 0) { if(t) подтвердиДвиж0(t); }

#if defined(COMPILER_MSC) || defined(COMPILER_GCC) && (__GNUC__ < 4 || __GNUC_MINOR__ < 1)
	#define NTL_MOVEABLE(T) inline проц подтвердиДвиж0(T *) {}
#else
	#define NTL_MOVEABLE(T) template<> inline проц проверьДвиж<T>(T *) {}
#endif

#endif

NTL_MOVEABLE(бул)
NTL_MOVEABLE(char)
NTL_MOVEABLE(signed char)
NTL_MOVEABLE(unsigned char)
NTL_MOVEABLE(short)
NTL_MOVEABLE(unsigned short)
NTL_MOVEABLE(цел)
NTL_MOVEABLE(бцел)
NTL_MOVEABLE(long)
NTL_MOVEABLE(unsigned long)
NTL_MOVEABLE(дол)
NTL_MOVEABLE(бдол)
NTL_MOVEABLE(float)
NTL_MOVEABLE(дво)
NTL_MOVEABLE(ук)
NTL_MOVEABLE(const ук)

#if defined(_NATIVE_WCHAR_T_DEFINED) || defined(COMPILER_GCC)
NTL_MOVEABLE(wchar_t)
#endif

template <class T, class B = ПустойКласс>
class СКлоном : public B {
public:
	friend T клонируй(const T& ист) { T c(ист, 1); return c; }
};

template <class T, class B = ПустойКласс>
class ОпцияГлубокойКопии : public B {
public:
#ifdef DEPRECATED
	friend T& operator<<=(T& dest, const T& ист)
	{ if(&dest != &ист) { (&dest)->~T(); ::new(&dest) T(ист, 1); } return dest; }
#endif
	friend T  клонируй(const T& ист) { T c(ист, 1); return c; }
};

template <class T, class B = ПустойКласс>
class ОпцияДвижимогоИГлубКопии : public B {
	friend проц подтвердиДвиж0(T *) {}
#ifdef DEPRECATED
	friend T& operator<<=(T& dest, const T& ист)
	{ if(&dest != &ист) { (&dest)->~T(); ::new(&dest) T(ист, 1); } return dest; }
#endif
	friend T  клонируй(const T& ист) { T c(ист, 1); return c; }
};

template <class T>
class СГлубокойКопией : public T {
public:
	СГлубокойКопией(const T& a) : T(a, 1)                 {}
	СГлубокойКопией(const T& a, цел) : T(a, 1)            {}

	СГлубокойКопией(const СГлубокойКопией& a) : T(a, 1)      {}
	СГлубокойКопией& operator=(const СГлубокойКопией& a)     { (T&)*this = пикуй(T(a, 1)); return *this; }

	СГлубокойКопией(T&& a) : T(пикуй(a))                   {}
	СГлубокойКопией& operator=(T&& a)                     { (T&)*this = пикуй(a); return *this; }

	СГлубокойКопией(СГлубокойКопией&& a) : T(пикуй(a))        {}
	СГлубокойКопией& operator=(СГлубокойКопией&& a)          { (T&)*this = пикуй(a); return *this; }

	СГлубокойКопией()                                     {}
};

// compatibility hacks

#define STL_ITERATOR_COMPATIBILITY \
	typedef ptrdiff_t                        difference_type; \
    typedef std::random_access_iterator_tag  iterator_category; \
    typedef T                                value_type; \
    typedef T                                pointer; \
    typedef T                                reference; \

#define STL_BI_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	проц                  clear()                { очисть(); } \
	size_type             size() const           { return дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	бул                  empty() const          { return пустой(); } \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \

#define STL_MAP_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return B::begin(); } \
	const_iterator        стоп() const            { return B::end(); } \
	проц                  clear()                { B::очисть(); } \
	size_type             size() const           { return B::дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	бул                  empty() const          { return B::пустой(); } \
	iterator              старт()                { return B::begin(); } \
	iterator              стоп()                  { return B::end(); } \

#define STL_SORTED_MAP_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	проц                  clear()                { B::очисть(); } \
	size_type             size() const           { return B::дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	бул                  empty() const          { return B::дайСчёт() == 0; } \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \

#define STL_VECTOR_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	проц                  clear()                { очисть(); } \
	size_type             size() const           { return дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \
	reference             front()                { return (*this)[0]; } \
	const_reference       front() const          { return (*this)[0]; } \
	reference             back()                 { return верх(); } \
	const_reference       back() const           { return верх(); } \
	бул                  empty() const          { return пустой(); } \
	проц                  push_back(const T& x)  { добавь(x); } \
	проц                  pop_back()             { сбрось(); } \


template <class T>
T *ДеклУкз__();

template <class Диапазон>
using ТипЗначенияМассива = typename std::remove_reference<decltype((*ДеклУкз__<Диапазон>())[0])>::type;

template <class V>
class ОбходчикКонстИ {
	typedef ТипЗначенияМассива<V> T;

	const V       *cont;
	цел            ii;
	struct NP { цел dummy; };

public:
	const T&       operator*() const       { return (*cont)[ii]; }
	const T       *operator->() const      { return &(*cont)[ii]; }
	const T&       operator[](цел i) const { return (*cont)[ii + i]; }

	ОбходчикКонстИ& operator++()           { ++ii; return *this; }
	ОбходчикКонстИ& operator--()           { --ii; return *this; }
	ОбходчикКонстИ  operator++(цел)        { ОбходчикКонстИ t = *this; ++ii; return t; }
	ОбходчикКонстИ  operator--(цел)        { ОбходчикКонстИ t = *this; --ii; return t; }

	ОбходчикКонстИ& operator+=(цел d)      { ii += d; return *this; }
	ОбходчикКонстИ& operator-=(цел d)      { ii -= d; return *this; }

	ОбходчикКонстИ  operator+(цел d) const { return ОбходчикКонстИ(*cont, ii + d); }
	ОбходчикКонстИ  operator-(цел d) const { return ОбходчикКонстИ(*cont, ii - d); }

	цел  operator-(const ОбходчикКонстИ& b) const   { return ii - b.ii; }

	бул operator==(const ОбходчикКонстИ& b) const  { return ii == b.ii; }
	бул operator!=(const ОбходчикКонстИ& b) const  { return ii != b.ii; }
	бул operator<(const ОбходчикКонстИ& b) const   { return ii < b.ii; }
	бул operator>(const ОбходчикКонстИ& b) const   { return ii > b.ii; }
	бул operator<=(const ОбходчикКонстИ& b) const  { return ii <= b.ii; }
	бул operator>=(const ОбходчикКонстИ& b) const  { return ii >= b.ii; }

	operator бул() const     { return ii < 0; }

	ОбходчикКонстИ()          {}
	ОбходчикКонстИ(NP *null)  { ПРОВЕРЬ(null == NULL); ii = -1; }
	ОбходчикКонстИ(const V& _cont, цел ii) : cont(&_cont), ii(ii) {}

	STL_ITERATOR_COMPATIBILITY
};

template <class V>
class ОбходчикИ {
	typedef ТипЗначенияМассива<V> T;

	V             *cont;
	цел            ii;
	struct NP { цел dummy; };

public:
	T&       operator*()              { return (*cont)[ii]; }
	T       *operator->()             { return &(*cont)[ii]; }
	T&       operator[](цел i)        { return (*cont)[ii + i]; }

	const T& operator*() const        { return (*cont)[ii]; }
	const T *operator->() const       { return &(*cont)[ii]; }
	const T& operator[](цел i) const  { return (*cont)[ii + i]; }

	ОбходчикИ& operator++()           { ++ii; return *this; }
	ОбходчикИ& operator--()           { --ii; return *this; }
	ОбходчикИ  operator++(цел)        { ОбходчикИ t = *this; ++ii; return t; }
	ОбходчикИ  operator--(цел)        { ОбходчикИ t = *this; --ii; return t; }

	ОбходчикИ& operator+=(цел d)      { ii += d; return *this; }
	ОбходчикИ& operator-=(цел d)      { ii -= d; return *this; }

	ОбходчикИ  operator+(цел d) const { return ОбходчикИ(*cont, ii + d); }
	ОбходчикИ  operator-(цел d) const { return ОбходчикИ(*cont, ii - d); }

	цел  operator-(const ОбходчикИ& b) const   { return ii - b.ii; }

	бул operator==(const ОбходчикИ& b) const  { return ii == b.ii; }
	бул operator!=(const ОбходчикИ& b) const  { return ii != b.ii; }
	бул operator<(const ОбходчикИ& b) const   { return ii < b.ii; }
	бул operator>(const ОбходчикИ& b) const   { return ii > b.ii; }
	бул operator<=(const ОбходчикИ& b) const  { return ii <= b.ii; }
	бул operator>=(const ОбходчикИ& b) const  { return ii >= b.ii; }

	operator бул() const                      { return ii < 0; }

	ОбходчикИ()          {}
	ОбходчикИ(NP *null)  { ПРОВЕРЬ(null == NULL); ii = -1; }
	ОбходчикИ(V& _cont, цел ii) : cont(&_cont), ii(ii) {}

	STL_ITERATOR_COMPATIBILITY
};

struct комбинируйХэш {
	т_хэш hash;

	template <class T> комбинируйХэш& делай(const T& x)                  { помести(дайХэшЗнач(x)); return *this; }

public:
	комбинируйХэш& помести(т_хэш h)                                      { hash = ХЭШ_КОНСТ2 * hash + h; return *this; }

	operator т_хэш() const                                         { return hash; }

	комбинируйХэш()                                                   { hash = ХЭШ_КОНСТ1; }
	template <class T>
	комбинируйХэш(const T& h1)                                        { hash = ХЭШ_КОНСТ1; делай(h1); }
	template <class T, class U>
	комбинируйХэш(const T& h1, const U& h2)                           { hash = ХЭШ_КОНСТ1; делай(h1); делай(h2); }
	template <class T, class U, class V>
	комбинируйХэш(const T& h1, const U& h2, const V& h3)              { hash = ХЭШ_КОНСТ1; делай(h1); делай(h2); делай(h3); }
	template <class T, class U, class V, class W>
	комбинируйХэш(const T& h1, const U& h2, const V& h3, const W& h4)	{ hash = ХЭШ_КОНСТ1; делай(h1); делай(h2); делай(h3); делай(h4); }

	template <class T> комбинируйХэш& operator<<(const T& x)          { делай(x); return *this; }
};


template <цел size>
struct Data_S_ : Движ< Data_S_<size> >
{
	ббайт filler[size];
};
///////////////////////////////
class Ткст0 : Движ<Ткст0> {
	enum { //
		РОД = 14,    // chr[РОД] is Ткст tier flag, 0 - small, 31 - medium, 32..254 ref alloc, 255 - read alloc from Реф
		МДЛИН = 15,    // chr[МДЛИН] stores the length of small tier strings (up to 14 bytes)
		БДЛИН = 2,     // chr[БДЛИН] stores the length of medium (< 32) and large tier strings
		СПЕЦ = 13, // chr[СПЕЦ]: stores Значение тип
	};
	enum { // chr[РОД] predefined values, if > СРЕДНИЙ, it is Rc тип; if <255, also stores alloc
		МАЛЫЙ = 0, // SVO ending zero, also has to be 0 because of дайСпец, also flag of small
	    СРЕДНИЙ = 31 // medium, укз points to 32 bytes block on heap
	};

	struct Rc {
		Атомар refcount;
		цел    alloc;

		char *дайУк() const  { return (char*)(this + 1); }
	};

	union {
		char   chr[16];
		ткст0 укз; // medium and large tier, in large tier (Rc *)укз - 1 points to Rc
		бцел *wptr;
		qword *qptr;
		бкрат   v[8];
		бцел  w[4];
		qword  q[2];
	};


#if defined(_ОТЛАДКА) && defined(COMPILER_GCC)
	цел          len;
	кткст0 s;
#endif

#ifdef _ОТЛАДКА
	проц Dsyn();
#else
	проц Dsyn() {}
#endif

	char&  мДлин()                { return chr[МДЛИН]; }
	char   мДлин() const          { return chr[МДЛИН]; }
	бцел& бДлин()                { return w[БДЛИН]; }
	бцел  бДлин() const          { return w[БДЛИН]; }
	бул   малый() const       { return chr[РОД] == МАЛЫЙ; }
	бул   большой() const       { return chr[РОД] != МАЛЫЙ; }
	бул   средний() const      { return chr[РОД] == СРЕДНИЙ; }
	бул   реф_ли() const         { return (ббайт)chr[РОД] > СРЕДНИЙ; }
	Rc    *Реф() const           { return (Rc *)укз - 1; }
	бул   совместный() const      { return Реф()->refcount != 1; }
	бул   шарссыл_ли() const   { return реф_ли() && совместный(); }
	цел    бРазмести() const        { цел b = (ббайт)chr[РОД]; return b == 255 ? Реф()->alloc : b; }
	бцел  бРавен(const Ткст0& s) const;

	проц бУст(const Ткст0& s);
	проц бОсвободи();
	проц бКат(цел c);
	т_хэш бЗначХэша() const;

	проц разшарь();
	проц устМДлин(цел l);

	char *Ук()                   { return малый() ? chr : укз; }
	char *размести_(цел count, char& kind);
	char *размести(цел count, char& kind);

	static Ткст0::Rc voidptr[2];

	проц разверни(Ткст0& b);

	// интерфейс к Значение
	static бцел StW(ббайт st)     { return MAKE4B(0, st, 0, 0); }
	проц устСпец0(ббайт st)     { w[3] = StW(st); }
	проц устСпец(ббайт st)      { ПРОВЕРЬ(малый() && дайСчёт() == 0); устСпец0(st); }
	ббайт дайСпец() const       { return (chr[МДЛИН] | chr[РОД]) == 0 ? chr[СПЕЦ] : 0; }
	ббайт GetSt() const            { return chr[СПЕЦ]; }
	бцел GetStW() const          { return w[3]; }
	бул спец_ли() const        { return !v[7] && v[6]; }
	бул спец_ли(ббайт st) const { return w[3] == StW(st); }

	friend class Ткст;
	friend class ТкстБуф;
	friend class Значение;
	friend class ТекстКтрл;

protected:
	проц обнули()                     { q[0] = q[1] = 0; Dsyn(); }
	проц устМалый(const Ткст0& s) { q[0] = s.q[0]; q[1] = s.q[1]; }
	проц освободи()                     { if(большой()) бОсвободи(); }
	проц пикуй0(Ткст0&& s) {
		устМалый(s);
		s.обнули();
	}
	проц уст0(const Ткст0& s) {
		if(s.малый()) устМалый(s); else бУст(s);
		Dsyn();
	}
	проц присвой(const Ткст0& s) {
		if(s.малый()) {
			освободи();
			устМалый(s);
		}
		else
			if(this != &s) {
				освободи();
				бУст(s);
			}
		Dsyn();
	}
	проц  уст0(кткст0 s, цел len);
	проц  устБ(кткст0 s, цел len);
	char *вставь(цел pos, цел count, кткст0 str);

	typedef char         tchar;
	typedef ббайт         bchar;
	typedef ТкстБуф Буфер;
	typedef Ткст  Ткст;

	цел    сравниБ(const Ткст0& s) const;

public:
	бул бРавн(const Ткст0& s) const;
	бул равен(const Ткст0& s) const {
		бдол q1 = q[1];
		бдол sq1 = s.q[1];
		return q1 == sq1 && q[0] == s.q[0] || ((q1 | sq1) & MAKE8B(0,0,0,0,0,0,255,0)) && бРавн(s);
	}
	бул равен(кткст0 s) const;

	цел    сравни(const Ткст0& s) const;

	т_хэш дайХэшЗнач() const {
#ifdef HASH64
		return chr[РОД] ? бЗначХэша() : (т_хэш)комбинируйХэш(q[0], q[1]);
#else
		return chr[РОД] ? бЗначХэша() : (т_хэш)комбинируйХэш(w[0], w[1], w[2], w[3]);
#endif
	}

	проц кат(цел c) {
		if(мДлин() < 14)
			chr[цел(мДлин()++)] = c;
		else
			бКат(c);
		Dsyn();
	}

	проц кат(кткст0 s, цел len);
	проц уст(кткст0 s, цел len);

	проц уст(цел i, цел chr);
	проц обрежь(цел pos);

	кткст0 старт() const   { return малый() ? chr : укз; }
	кткст0 begin() const   { return старт(); }
	кткст0 стоп() const     { return старт() + дайДлину(); }
	кткст0 end() const     { return стоп(); }

	цел operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i <= дайСчёт()); return старт()[i]; }

	operator const char *() const   { return старт(); }
	кткст0 operator~() const   { return старт(); }

	проц удали(цел pos, цел count = 1);
	проц очисть()                { освободи(); обнули(); }

	цел дайСчёт() const        { return малый() ? chr[МДЛИН] : w[БДЛИН]; }
	цел дайДлину() const       { return дайСчёт(); }
	цел дайРазмест() const        { return малый() ? 14 : бРазмести(); }

	проц резервируй(цел r);

	Ткст0()                   {}
	~Ткст0()                  { освободи(); }
};

class Ткст : public Движ<Ткст, АТкст<Ткст0> > {
	проц разверни(Ткст& b)                                   { Ткст0::разверни(b); }

#ifdef _ОТЛАДКА
#ifndef COMPILER_GCC
	цел          len;
	кткст0 s;
#endif
	friend class Ткст0;
#endif

	проц присвойДлин(кткст0 s, цел МДЛИН);

	enum SSPECIAL { СПЕЦ };

	template <class T>
	Ткст(const T& x, ббайт st, SSPECIAL) {
		*(T*)chr = x;
		устСпец0(st);
	}
	Ткст(SSPECIAL) {}

	friend class Значение;

public:
	const Ткст& operator+=(char c)                       { кат(c); return *this; }
	const Ткст& operator+=(кткст0 s)                { кат(s); return *this; }
	const Ткст& operator+=(const Ткст& s)              { кат(s); return *this; }

	Ткст& operator=(кткст0 s);
	Ткст& operator=(const Ткст& s)                     { Ткст0::присвой(s); return *this; }
	Ткст& operator=(Ткст&& s)                          { if(this != &s) { освободи(); пикуй0(пикуй(s)); } return *this; }
	Ткст& operator=(ТкстБуф& b)                     { *this = Ткст(b); return *this; }

	проц   сожми()                                        { *this = Ткст(старт(), дайДлину()); }
	цел    дайСчётСим() const;

	Ткст()                                               { обнули(); }
	Ткст(const Обнул&)                                  { обнули(); }
	Ткст(const Ткст& s)                                { Ткст0::уст0(s); }
	Ткст(Ткст&& s)                                     { Ткст0::пикуй0(пикуй(s)); }
	Ткст(кткст0 s);
	Ткст(const Ткст& s, цел n)                         { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); Ткст0::уст0(~s, n); }
	Ткст(кткст0 s, цел n)                           { Ткст0::уст0(s, n); }
	Ткст(const ббайт *s, цел n)                           { Ткст0::уст0((const char *)s, n); }
	Ткст(кткст0 s, кткст0 lim)                 { Ткст0::уст0(s, (цел)(lim - s)); }
	Ткст(цел chr, цел count)                             { Ткст0::обнули(); кат(chr, count); }
	Ткст(ТкстБуф& b);


	Ткст(char16 *s);
	Ткст(шим *s);

	ШТкст вШТкст() const;
	const Ткст& вТкст() const                         { return *this; }

	static Ткст дайПроц();
	бул   проц_ли() const;

	friend проц разверни(Ткст& a, Ткст& b)                 { a.разверни(b); }

	Ткст(const std::string& s)                           { Ткст0::уст0(s.c_str(), (цел)s.length()); }
	std::string вСтд() const                              { return std::string(старт(), стоп()); }

	template <class Делец>
	static Ткст сделай(цел alloc, Делец m);
};

inline std::string to_string(const Ткст& s)              { return std::string(s.старт(), s.стоп()); }

class ТкстБуф : БезКопий {
	char   *pbegin;
	char   *pend;
	char   *limit;
	char    буфер[256];

	friend class Ткст;

	typedef Ткст0::Rc Rc;

	char *размести(цел len, цел& alloc);
	проц  переразмести(бцел n, кткст0 cat = NULL, цел l = 0);
	проц  переразместиД(кткст0 s, цел l);
	проц  расширь();
	проц  обнули()                    { pbegin = pend = буфер; limit = pbegin + 255; }
	проц  освободи();
	проц  уст(Ткст& s);

public:
	char *старт()                   { *pend = '\0'; return pbegin; }
	char *begin()                   { return старт(); }
	char *стоп()                     { *pend = '\0'; return pend; }
	char *end()                     { return стоп(); }

	char& operator*()               { return *старт(); }
	char& operator[](цел i)         { return старт()[i]; }
	operator char*()                { return старт(); }
	operator ббайт*()                { return (ббайт *)старт(); }
	operator проц*()                { return старт(); }
	char *operator~()               { return старт(); }

	проц устДлину(цел l);
	проц устСчёт(цел l)            { устДлину(l); }
	цел  дайДлину() const          { return (цел)(pend - pbegin); }
	цел  дайСчёт() const           { return дайДлину(); }
	проц длинтекс();
	проц очисть()                    { освободи(); обнули(); }
	проц резервируй(цел r)             { цел l = дайДлину(); устДлину(l + r); устДлину(l); }
	проц сожми();

	проц кат(цел c)                        { if(pend >= limit) расширь(); *pend++ = c; }
	проц кат(цел c, цел count);
	проц кат(кткст0 s, цел l);
	проц кат(кткст0 s, кткст0 e) { кат(s, цел(e - s)); }
	проц кат(кткст0 s);
	проц кат(const Ткст& s)              { кат(s, s.дайДлину()); }

	цел  дайРазмест() const           { return (цел)(limit - pbegin); }

	проц operator=(Ткст& s)       { освободи(); уст(s); }

	ТкстБуф()                  { обнули(); }
	ТкстБуф(Ткст& s)         { уст(s); }
	ТкстБуф(цел len)           { обнули(); устДлину(len); }
	~ТкстБуф()                 { if(pbegin != буфер) освободи(); }
};

inline бул  пустой(const Ткст& s)      { return s.пустой(); }

Ткст фмтУк(const ук p);

template <class T>
inline Ткст какТкст(const T& x)
{
	return x.вТкст();
}

template <class T>
inline Ткст какТкст(T *x)
{
	return фмтУк(x);
}

inline Ткст& operator<<(Ткст& s, кткст0 x)
{
	s.кат(x, длинтекс__(x));
	return s;
}

inline Ткст& operator<<(Ткст& s, char *x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст& s, char x)
{
	s.кат((цел) x);
	return s;
}

inline Ткст& operator<<(Ткст& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline Ткст& operator<<(Ткст& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст& s, const char& x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, кткст0 x)
{
	s.кат(x, длинтекс__(x));
	return s;
}

inline Ткст& operator<<(Ткст&& s, char *x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, char x)
{
	s.кат((цел) x);
	return s;
}

inline Ткст& operator<<(Ткст&& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline Ткст& operator<<(Ткст&& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline Ткст& operator<<(Ткст&& s, const char& x)
{
	s.кат(x);
	return s;
}

template<>
inline бул  пусто_ли(const Ткст& s)       { return s.пустой(); }

template<>
inline Ткст какТкст(const Ткст& s)     { return s; }

template<>
inline т_хэш дайХэшЗнач(const Ткст& s) { return s.дайХэшЗнач(); }

цел сравнилюб(const Ткст& a, const Ткст& b);
цел сравнилюб(const Ткст& a, кткст0 b);

inline
цел сравнилюб(кткст0 a, const Ткст& b) {
	return -сравнилюб(b, a);
}

Ткст обрежьЛево(const Ткст& s);
Ткст обрежьПраво(const Ткст& s);
Ткст обрежьОба(const Ткст& s);

Ткст обрежьЛево(кткст0 prefix, цел len, const Ткст& s);
force_inline Ткст обрежьЛево(кткст0 prefix, const Ткст& s)    { return обрежьЛево(prefix, (цел)strlen(prefix), s); }
force_inline Ткст обрежьЛево(const Ткст& prefix, const Ткст& s)  { return обрежьЛево(~prefix, prefix.дайСчёт(), s); }

Ткст обрежьПраво(кткст0 suffix, цел len, const Ткст& s);
force_inline Ткст обрежьПраво(кткст0 suffix, const Ткст& s)   { return обрежьПраво(suffix, (цел)strlen(suffix), s); }
force_inline Ткст обрежьПраво(const Ткст& suffix, const Ткст& s) { return обрежьПраво(~suffix, suffix.дайСчёт(), s); }

inline ТкстБуф& operator<<(ТкстБуф& s, кткст0 x)
{
	s.кат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, char *x)
{
	s.кат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, char x)
{
	s.кат((цел) x);
	return s;
}

inline ТкстБуф& operator<<(ТкстБуф& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

template <class T>
inline ТкстБуф& operator<<(ТкстБуф& s, const T& x)
{
	s.кат(какТкст(x));
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, кткст0  const &x)
{
	s.кат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

template<>
inline ТкстБуф& operator<<(ТкстБуф& s, const char& x)
{
	s.кат(x);
	return s;
}

template <class T>
проц сырКат(Ткст& s, const T& x)
{
	s.кат((const char *)&x, sizeof(x));
}

template <class T>
проц сырКат(ТкстБуф& s, const T& x)
{
	s.кат((const char *)&x, sizeof(x));
}

class ШТкст0 {
	enum { МАЛЫЙ = 23 };

	шим *укз;
	цел    length;
	цел    alloc;

#ifdef _ОТЛАДКА
	проц Dsyn();
#else
	проц Dsyn() {}
#endif

	static Атомар voidptr[2];

	бул    IsRc() const  { return alloc > МАЛЫЙ; }
	Атомар& Rc()          { return *((Атомар *)укз - 1); }
	бул    совместный()    { return IsRc() && Rc() > 1; }

	шим  *размести(цел& count);
	проц    бКат(цел c);
	проц    разшарь();

	friend class ШТкстБуф;
	friend class ШТкст;

protected:
	typedef шим         tchar;
	typedef крат         bchar;
	typedef ШТкстБуф Буфер;
	typedef ШТкст       Ткст;

	проц    обнули()                       { static шим e[2]; length = alloc = 0; укз = e; Dsyn(); ПРОВЕРЬ(*укз == 0); }
	проц    уст0(const шим *s, цел length);
	проц    уст0(const ШТкст0& s);
	проц    пикуй0(ШТкст0&& s)          { укз = s.укз; length = s.length; alloc = s.alloc; s.обнули(); Dsyn(); }
	проц    освободи();
	проц    FFree()                      { if(alloc > 0) освободи(); }
	проц    разверни(ШТкст0& b)            { ДинрусРНЦП::разверни(укз, b.укз); ДинрусРНЦП::разверни(length, b.length); ДинрусРНЦП::разверни(alloc, b.alloc); Dsyn(); b.Dsyn(); }
	шим  *вставь(цел pos, цел count, const шим *данные);

public:
	const шим *старт() const           { return укз; }
	const шим *стоп() const             { return старт() + дайДлину(); }
	const шим *begin() const           { return старт(); }
	const шим *end() const             { return стоп(); }
	цел   operator[](цел i) const        { return укз[i]; }

	operator const шим *() const       { return старт(); }
	const шим *operator~() const       { return старт(); }

	проц кат(цел c)                      { if(!IsRc() && length < alloc) { укз[length++] = c; укз[length] = 0; } else бКат(c); Dsyn(); }
	проц кат(const шим *s, цел length);
	проц уст(const шим *s, цел length);

	цел  дайСчёт() const                { return length; }
	цел  дайДлину() const               { return length; }
	цел  дайРазмест() const                { return alloc; }

	т_хэш   дайХэшЗнач() const             { return хэшпам(укз, length * sizeof(шим)); }
	бул     равен(const ШТкст0& s) const { return s.length == length && т_равнпам(укз, s.укз, length); }
	бул     равен(const шим *s) const    { цел l = длинтекс__(s); return l == дайСчёт() && т_равнпам(begin(), s, l); }
	цел      сравни(const ШТкст0& s) const;

	проц удали(цел pos, цел count = 1);
	проц вставь(цел pos, const шим *s, цел count);
	проц очисть()                         { освободи(); обнули(); }

	проц уст(цел pos, цел ch);
	проц обрежь(цел pos);

	ШТкст0()                           { обнули(); }
	~ШТкст0()                          { освободи(); }

//	ШТкст0& operator=(const ШТкст0& s) { освободи(); уст0(s); return *this; }
};

class ШТкст : public Движ<ШТкст, АТкст<ШТкст0> >
{
	проц разверни(ШТкст& b)                                   { ШТкст0::разверни(b); }

#ifdef _ОТЛАДКА
	цел          len;
	const шим *s;
	friend class ШТкст0;
#endif

public:
	РНЦП::Ткст вТкст() const;

	const ШТкст& operator+=(шим c)                      { кат(c); return *this; }
	const ШТкст& operator+=(const шим *s)               { кат(s); return *this; }
	const ШТкст& operator+=(const ШТкст& s)             { кат(s); return *this; }

	ШТкст& operator<<(шим c)                            { кат(c); return *this; }
	ШТкст& operator<<(const ШТкст& s)                   { кат(s); return *this; }
	ШТкст& operator<<(const шим *s)                     { кат(s); return *this; }

	ШТкст& operator=(const шим *s);
	ШТкст& operator=(const ШТкст& s)                    { if(this != &s) { ШТкст0::FFree(); ШТкст0::уст0(s); } return *this; }
	ШТкст& operator=(ШТкст&& s)                         { if(this != &s) { ШТкст0::FFree(); ШТкст0::пикуй0(пикуй(s)); } return *this; }
	ШТкст& operator=(ШТкстБуф& b)                    { *this = ШТкст(b); return *this; }
	ШТкст& operator<<=(const ШТкст& s)                  { if(this != &s) { ШТкст0::освободи(); ШТкст0::уст0(s, s.дайСчёт()); } return *this; }

	проц   сожми()                                         { *this = ШТкст(старт(), дайДлину()); }

	ШТкст()                                               {}
	ШТкст(const Обнул&)                                  {}
	ШТкст(const ШТкст& s)                               { ШТкст0::уст0(s); }
	ШТкст(ШТкст&& s)                                    { ШТкст0::пикуй0(пикуй(s)); }
	ШТкст(const шим *s)                                 { ШТкст0::уст0(s, длинтекс__(s)); }
	ШТкст(const ШТкст& s, цел n)                        { ПРОВЕРЬ(n >= 0 && n <= s.дайДлину()); ШТкст0::уст0(~s, n); }
	ШТкст(const шим *s, цел n)                          { ШТкст0::уст0(s, n); }
	ШТкст(const шим *s, const шим *lim)               { ШТкст0::уст0(s, (цел)(lim - s)); }
	ШТкст(цел chr, цел count)                             { ШТкст0::обнули(); кат(chr, count); }
	ШТкст(ШТкстБуф& b);

	ШТкст(кткст0 s);
	ШТкст(кткст0 s, цел n);
	ШТкст(кткст0 s, кткст0 lim);

	ШТкст(const char16 *s);

	static ШТкст дайПроц();
	бул   проц_ли() const                                   { return alloc < 0; }

	friend проц разверни(ШТкст& a, ШТкст& b)                { a.разверни(b); }
	friend ШТкст operator+(const ШТкст& a, char b)      { ШТкст c(a); c += b; return c; }
	friend ШТкст operator+(ШТкст&& a, char b)           { ШТкст c(пикуй(a)); c += b; return c; }
	friend ШТкст operator+(char a, const ШТкст& b)      { ШТкст c(a, 1); c += b; return c; }

#ifndef _HAVE_NO_STDWSTRING
	ШТкст(const std::wstring& s);
	operator std::wstring() const                           { return вСтд(); }
	std::wstring вСтд() const;
#endif
};

#ifndef _HAVE_NO_STDWSTRING
inline std::wstring to_string(const ШТкст& s)             { return s.вСтд(); }
#endif

class ШТкстБуф : БезКопий {
	шим   *pbegin;
	шим   *pend;
	шим   *limit;

	friend class ШТкст;

	шим *размести(цел len, цел& alloc);
	проц   расширь(бцел n, const шим *cat = NULL, цел l = 0);
	проц   расширь();
	проц   обнули();
	проц   освободи();
	проц   уст(ШТкст& s);

public:
	шим *старт()                   { *pend = '\0'; return pbegin; }
	шим *begin()                   { return старт(); }
	шим *стоп()                     { *pend = '\0'; return pend; }
	шим *end()                     { return стоп(); }

	шим& operator*()               { return *старт(); }
	шим& operator[](цел i)         { return старт()[i]; }
	operator шим*()                { return старт(); }
	operator крат*()                { return (крат *)старт(); }
	operator проц*()                 { return старт(); }
	шим *operator~()               { return старт(); }

	проц  устДлину(цел l);
	проц  устСчёт(цел l)            { устДлину(l); }
	цел   дайДлину() const          { return (цел)(pend - pbegin); }
	цел   дайСчёт() const           { return дайДлину(); }
	проц  длинтекс()                   { устДлину(длинтекс__(pbegin)); }
	проц  очисть()                    { освободи(); обнули(); }
	проц  резервируй(цел r)             { цел l = дайДлину(); устДлину(l + r); устДлину(l); }

	проц  кат(цел c)                          { if(pend >= limit) расширь(); *pend++ = c; }
	проц  кат(цел c, цел count);
	проц  кат(const шим *s, цел l);
	проц  кат(const шим *s, const шим *e) { кат(s, цел(e - s)); }
	проц  кат(const шим *s)                 { кат(s, длинтекс__(s)); }
	проц  кат(const ШТкст& s)               { кат(s, s.дайДлину()); }
	проц  кат(кткст0 s)                  { кат(ШТкст(s)); }

	цел   дайРазмест() const           { return (цел)(limit - pbegin); }

	проц operator=(ШТкст& s)       { освободи(); уст(s); }

	ШТкстБуф()                  { обнули(); }
	ШТкстБуф(ШТкст& s)        { уст(s); }
	ШТкстБуф(цел len)           { обнули(); устДлину(len); }
	~ШТкстБуф()                 { освободи(); }
};

inline бул  пустой(const ШТкст& s)      { return s.пустой(); }

template<>
inline бул  пусто_ли(const ШТкст& s)       { return s.пустой(); }

//template<>
//inline Ткст какТкст(const ШТкст& s)     { return s; }

template<>
inline т_хэш дайХэшЗнач(const ШТкст& s) { return хэшпам((const ук)~s, 2 * s.дайДлину()); }

ШТкст обрежьЛево(const ШТкст& str);
ШТкст обрежьПраво(const ШТкст& s);

цел сравнилюб(const ШТкст& a, const ШТкст& b);
цел сравнилюб(const ШТкст& a, const шим *b);

inline
цел сравнилюб(const шим *a, const ШТкст& b) {
	return -сравнилюб(b, a);
}

template<> inline Ткст какТкст(кткст0  const &s)    { return s; }
template<> inline Ткст какТкст(char * const &s)          { return s; }
template<> inline Ткст какТкст(кткст0 s)            { return s; }
template<> inline Ткст какТкст(char *s)                  { return s; }
template<> inline Ткст какТкст(const char& a)            { return Ткст(a, 1); }
template<> inline Ткст какТкст(const signed char& a)     { return Ткст(a, 1); }
template<> inline Ткст какТкст(const unsigned char& a)   { return Ткст(a, 1); }
template<> inline Ткст какТкст(const бул& a)            { return a ? "true" : "false"; }
template<> inline Ткст какТкст(const std::string& s)     { return Ткст(s); }
template<> inline Ткст какТкст(const std::wstring& s)    { return ШТкст(s).вТкст(); }

unsigned ctoi(цел c);

typedef цел (*CharFilter)(цел);

цел CharFilterAscii(цел c);
цел CharFilterAscii128(цел c);
цел CharFilterUnicode(цел c);
цел CharFilterDigit(цел c);
цел CharFilterWhitespace(цел c);
цел CharFilterNotWhitespace(цел c);
цел CharFilterAlpha(цел c);
цел CharFilterToUpper(цел c);
цел CharFilterToLower(цел c);
цел CharFilterToUpperAscii(цел c);
цел CharFilterAlphaToUpper(цел c);
цел CharFilterAlphaToLower(цел c);
цел CharFilterInt(цел c);
цел CharFilterDouble(цел c);
цел CharFilterDefaultToUpperAscii(цел c);
цел CharFilterCrLf(цел c);
цел CharFilterNoCrLf(цел c);

Ткст фильтруй(кткст0 s, цел (*фильтр)(цел));
Ткст FilterWhile(кткст0 s, цел (*фильтр)(цел));

ШТкст фильтруй(const шим *s, цел (*фильтр)(цел));
ШТкст FilterWhile(const шим *s, цел (*фильтр)(цел));

//#include "AString.hpp"
template <class B>
force_inline
проц АТкст<B>::вставь(цел pos, кткст0 s)
{
	вставь(pos, s, длинтекс__(s));
}

template <class B>
проц АТкст<B>::кат(цел c, цел count)
{
	tchar *s = B::вставь(дайДлину(), count, NULL);
	while(count--)
		*s++ = c;
}

template <class B>
force_inline
проц АТкст<B>::кат(const tchar *s)
{
	кат(s, длинтекс__(s));
}

template <class B>
цел АТкст<B>::сравни(const tchar *b) const
{
	const tchar *a = B::старт();
	const tchar *ae = стоп();
	for(;;) {
		if(a >= ae)
			return *b == 0 ? 0 : -1;
		if(*b == 0)
			return 1;
		цел q = сравнзн__(*a++) - сравнзн__(*b++);
		if(q)
			return q;
	}
}

template <class B>
typename АТкст<B>::Ткст АТкст<B>::середина(цел from, цел count) const
{
	цел l = дайДлину();
	if(from > l) from = l;
	if(from < 0) from = 0;
	if(count < 0)
		count = 0;
	if(from + count > l)
		count = l - from;
	return Ткст(B::старт() + from, count);
}

template <class B>
цел АТкст<B>::найди(цел chr, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *e = стоп();
	const tchar *укз = B::старт();
	for(const tchar *s = укз + from; s < e; s++)
		if(*s == chr)
			return (цел)(s - укз);
	return -1;
}

template <class B>
цел АТкст<B>::найдирек(цел chr, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *укз = B::старт();
		for(const tchar *s = укз + from; s >= укз; s--)
			if(*s == chr)
				return (цел)(s - укз);
	}
	return -1;
}

template <class B>
цел АТкст<B>::найдирек(цел len, const tchar *s, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	if(from < дайДлину()) {
		const tchar *укз = B::старт();
		const tchar *p = укз + from - len + 1;
		len *= sizeof(tchar);
		while(p >= укз) {
			if(*s == *p && memcmp(s, p, len) == 0)
				return (цел)(p - укз);
			p--;
		}
	}
	return -1;
}

template <class B>
цел АТкст<B>::найдирекПосле(цел len, const tchar *s, цел from) const
{
	цел q = найдирек(len, s, from);
	return q >= 0 ? q + len : -1;
}

template <class B>
проц АТкст<B>::замени(const tchar *найди, цел findlen, const tchar *replace, цел replacelen)
{
	if(findlen == 0)
		return;
	Ткст r;
	цел i = 0;
	const tchar *p = B::старт();
	for(;;) {
		цел j = найди(findlen, найди, i);
		if(j < 0)
			break;
		r.кат(p + i, j - i);
		r.кат(replace, replacelen);
		i = j + findlen;
	}
	r.кат(p + i, B::дайСчёт() - i);
	B::освободи();
	B::уст0(r);
}

template <class B>
цел АТкст<B>::найдирек(const tchar *s, цел from) const
{
	return найдирек(длинтекс__(s), s, from);
}

template <class B>
цел АТкст<B>::найдирекПосле(const tchar *s, цел from) const
{
	return найдирекПосле(длинтекс__(s), s, from);
}

template <class B>
цел АТкст<B>::найдирек(цел chr) const
{
	return B::дайСчёт() ? найдирек(chr, B::дайСчёт() - 1) : -1;
}

template <class B>
проц АТкст<B>::замени(const Ткст& найди, const Ткст& replace)
{
	замени(~найди, найди.дайСчёт(), ~replace, replace.дайСчёт());
}

template <class B>
force_inline
проц АТкст<B>::замени(const tchar *найди, const tchar *replace)
{
	замени(найди, (цел)длинтекс__(найди), replace, (цел)длинтекс__(replace));
}

template <class B>
force_inline
проц АТкст<B>::замени(const Ткст& найди, const tchar *replace)
{
	замени(~найди, найди.дайСчёт(), replace, (цел)длинтекс__(replace));
}

template <class B>
force_inline
проц АТкст<B>::замени(const tchar *найди, const Ткст& replace)
{
	замени(найди, (цел)длинтекс__(найди), ~replace, replace.дайСчёт());
}

template <class B>
бул АТкст<B>::начинаетсяС(const tchar *s, цел len) const
{
	if(len > дайДлину()) return false;
	return memcmp(s, B::старт(), len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
бул АТкст<B>::начинаетсяС(const tchar *s) const
{
	return начинаетсяС(s, длинтекс__(s));
}

template <class B>
бул АТкст<B>::заканчиваетсяНа(const tchar *s, цел len) const
{
	цел l = дайДлину();
	if(len > l) return false;
	return memcmp(s, B::старт() + l - len, len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
бул АТкст<B>::заканчиваетсяНа(const tchar *s) const
{
	return заканчиваетсяНа(s, длинтекс__(s));
}

template <class B>
цел АТкст<B>::найдиПервыйИз(цел len, const tchar *s, цел from) const
{
	ПРОВЕРЬ(from >= 0 && from <= дайДлину());
	const tchar *укз = B::старт();
	const tchar *e = B::стоп();
	const tchar *se = s + len;
	if(len == 1) {
		tchar c1 = s[0];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			if(*bs == c1)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 2) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 3) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3)
				return (цел)(bs - укз);
		}
		return -1;
	}
	if(len == 4) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		tchar c4 = s[3];
		for(const tchar *bs = укз + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3 || ch == c4)
				return (цел)(bs - укз);
		}
		return -1;
	}
	for(const tchar *bs = укз + from; bs < e; bs++)
		for(const tchar *ss = s; ss < se; ss++)
			if(*bs == *ss)
				return (цел)(bs - укз);
	return -1;
}

force_inline
проц Ткст0::уст(кткст0 s, цел len)
{
	очисть();
	if(len <= 14) {
		копирпам8(chr, (const ук) s, len);
		мДлин() = len;
		Dsyn();
		return;
	}
	устБ(s, len);
	Dsyn();
}

force_inline
проц Ткст0::уст0(кткст0 s, цел len)
{
	обнули();
	if(len <= 14) {
		мДлин() = len;
		копирпам8(chr, (const ук)s, len);
		Dsyn();
		return;
	}
	устБ(s, len);
	Dsyn();
}

inline
бул Ткст0::равен(кткст0 s) const
{ // This is optimized for comparison with string literals...
	т_мера len = strlen(s);
	ук p;
	if(малый()) {
		if(len > 14 || len != (т_мера)chr[МДЛИН]) return false; // len > 14 fixes issue with GCC warning, might improves performance too
		p = (const ук)chr;
	}
	else {
		if(len != (т_мера)w[БДЛИН]) return false;
		p = укз;
	}
	return memcmp(p, s, len) == 0; // compiler is happy to optimize memcmp out with up to 64bit comparisons for literals...
}

inline
цел Ткст0::сравни(const Ткст0& s) const
{
#ifdef CPU_LE
	if((chr[РОД] | s.chr[РОД]) == 0) { // both strings are small, len is the MSB
	#ifdef CPU_64
		бдол a = эндианРазворот64(q[0]);
		бдол b = эндианРазворот64(s.q[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот64(q[1]);
		b = эндианРазворот64(s.q[1]);
		return a == b ? 0 : a < b ? -1 : 1;
	#else
		бдол a = эндианРазворот32(w[0]);
		бдол b = эндианРазворот32(s.w[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[1]);
		b = эндианРазворот32(s.w[1]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[2]);
		b = эндианРазворот32(s.w[2]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[3]);
		b = эндианРазворот32(s.w[3]);
		return a == b ? 0 : a < b ? -1 : 1;
	#endif
	}
#endif
	return сравниБ(s);
}

force_inline
Ткст& Ткст::operator=(кткст0 s)
{
	присвойДлин(s, длинтекс__(s));
	return *this;
}

force_inline
Ткст::Ткст(кткст0 s)
{
	Ткст0::уст0(s, длинтекс__(s));
}

force_inline
проц Ткст0::разверни(Ткст0& b)
{
	qword a0 = q[0]; // Explicit код_ (instead of разверни) seems to emit some SSE2 код_ with CLANG
	qword a1 = q[1];
	qword b0 = b.q[0];
	qword b1 = b.q[1];
	q[0] = b0;
	q[1] = b1;
	b.q[0] = a0;
	b.q[1] = a1;
	Dsyn(); b.Dsyn();
}

template <class Делец>
inline
Ткст Ткст::сделай(цел alloc, Делец m)
{
	Ткст s;
	if(alloc <= 14) {
		цел len = m(s.chr);
		ПРОВЕРЬ(len <= alloc);
		s.мДлин() = len;
		s.Dsyn();
	}
	else {
		if(alloc < 32) {
			s.chr[РОД] = СРЕДНИЙ;
			s.укз = (char *)разместиПам32();
		}
		else
			s.укз = s.размести(alloc, s.chr[РОД]);
		цел len = m(s.укз);
		ПРОВЕРЬ(len <= alloc);
		s.укз[len] = 0;
		s.бДлин() = len;
		s.мДлин() = 15;
		if(alloc >= 32 && alloc > 2 * len)
			s.сожми();
	}
	return s;
}

force_inline
проц ТкстБуф::длинтекс()
{
	устДлину((цел)длинтекс__(pbegin));
}

inline
проц ТкстБуф::кат(кткст0 s, цел l)
{
	if(pend + l > limit)
		переразместиД(s, l);
	else {
		копирпам8(pend, (кук)s, l);
		pend += l;
	}
}

inline
проц ТкстБуф::кат(цел c, цел l)
{
	if(pend + l > limit)
		переразместиД(NULL, l);
	устпам8(pend, c, l);
	pend += l;
}

force_inline
проц ТкстБуф::кат(кткст0 s)
{
	кат(s, (цел)длинтекс__(s));
}
///////////////////////////////
//Vcont.h

template <class T>
class Буфер : Движ< Буфер<T> > {
	T *укз;
	
	проц разместпам(т_мера size) {
		if(std::is_trivially_destructible<T>::value)
			укз = (T *)разместиПам(size * sizeof(T));
		else {
			ук p = разместиПам(size * sizeof(T) + 16);
			*(т_мера *)p = size;
			укз = (T *)((ббайт *)p + 16);
		}
	}
	проц нов(т_мера size) {
		разместпам(size);
		строй(укз, укз + size);
	}
	проц нов(т_мера size, const T& in) {
		разместпам(size);
		стройЗаполниГлубКопию(укз, укз + size, in);
	}
	проц освободи() {
		if(укз) {
			if(std::is_trivially_destructible<T>::value)
				освободиПам(укз);
			else {
				ук p = (ббайт *)укз - 16;
				т_мера size = *(т_мера *)p;
				разрушь(укз, укз + size);
				освободиПам(p);
			}
		}
	}

public:
	operator T*()                        { return укз; }
	operator const T*() const            { return укз; }
	T *operator~()                       { return укз; }
	const T *operator~() const           { return укз; }
	T          *дай()                    { return укз; }
	const T    *дай() const              { return укз; }
	T          *begin()                  { return укз; }
	const T    *begin() const            { return укз; }

	проц размести(т_мера size)              { очисть(); нов(size); }
	проц размести(т_мера size, const T& in) { очисть(); нов(size, in); }

	проц очисть()                         { освободи(); укз = NULL; }
	бул пустой() const                 { return укз == NULL; }

	Буфер()                             { укз = NULL; }
	Буфер(т_мера size)                  { нов(size); }
	Буфер(т_мера size, const T& init)   { нов(size, init); }
	~Буфер()                            { освободи(); }

	проц operator=(Буфер&& v)           { if(&v != this) { очисть(); укз = v.укз; v.укз = NULL; } }
	Буфер(Буфер&& v)                   { укз = v.укз; v.укз = NULL; }

	Буфер(т_мера size, std::initializer_list<T> init) {
		разместпам(size); T *t = укз; for(const auto& i : init) new (t++) T(i);
	}
	Буфер(std::initializer_list<T> init) : Буфер(init.size(), init) {}
};

template <class T>
class Один : ОпцияДвижимогоИГлубКопии< Один<T> > {
	mutable T  *укз;

	проц        освободи()                     { if(укз && укз != (T*)1) delete укз; }
	template <class TT>
	проц        подбери(Один<TT>&& данные)       { укз = данные.открепи(); }

public:
	проц        прикрепи(T *данные)            { освободи(); укз = данные; }
	T          *открепи()                   { T *t = укз; укз = NULL; return t; }
	проц        очисть()                    { освободи(); укз = NULL; }

	проц        operator=(T *данные)         { прикрепи(данные); }
	
	template <class TT>
	проц        operator=(Один<TT>&& d)     { if((проц *)this != (проц *)&d) { освободи(); подбери(пикуй(d)); }}

	const T    *operator->() const         { ПРОВЕРЬ(укз); return укз; }
	T          *operator->()               { ПРОВЕРЬ(укз); return укз; }
	const T    *operator~() const          { return укз; }
	T          *operator~()                { return укз; }
	const T    *дай() const                { return укз; }
	T          *дай()                      { return укз; }
	const T&    operator*() const          { ПРОВЕРЬ(укз); return *укз; }
	T&          operator*()                { ПРОВЕРЬ(укз); return *укз; }

	template <class TT, class... Арги>
	TT&         создай(Арги&&... арги)     { TT *q = new TT(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	template <class TT> // with C++ conforming compiler, this would not be needed - GCC bug workaround
	TT&         создай()                   { TT *q = new TT; прикрепи(q); return *q; }
	template <class... Арги>
	T&          создай(Арги&&... арги)     { T *q = new T(std::forward<Арги>(арги)...); прикрепи(q); return *q; }
	T&          создай()                   { T *q = new T; прикрепи(q); return *q; }

	template <class TT>
	бул        является() const                 { return dynamic_cast<const TT *>(укз); }

	бул        пустой() const            { return !укз; }

	operator бул() const                  { return укз; }
	
	Ткст вТкст() const                { return укз ? какТкст(*укз) : "<empty>"; }

	Один()                                  { укз = NULL; }
	Один(T *newt)                           { укз = newt; }
	template <class TT>
	Один(Один<TT>&& p)                       { подбери(пикуй(p)); }
	Один(const Один<T>& p, цел)              { укз = p.пустой() ? NULL : new T(клонируй(*p)); }
	Один(const Один<T>& p) = delete;
	проц operator=(const Один<T>& p) = delete;
	~Один()                                 { освободи(); }
};

template <class T, class... Арги>
Один<T> сделайОдин(Арги&&... арги) {
	Один<T> r;
	r.создай(std::forward<Арги>(арги)...);
	return r;
}

template <class U> class Индекс;

template <class T>
class Вектор : public ОпцияДвижимогоИГлубКопии< Вектор<T> > {
	T       *vector;
	цел      items;
	цел      alloc;

	static проц    освободиРяд(T *укз)            { if(укз) освободиПам(укз); }
	static T      *разместиРяд(цел& n);

	проц     обнули()                          { vector = NULL; items = alloc = 0; }
	проц     подбери(Вектор<T>&& v);

	T       *Rdd()                           { return vector + items++; }

	проц     освободи();
	проц     __глубКопия(const Вектор& ист);
	T&       дай(цел i) const                { ПРОВЕРЬ(i >= 0 && i < items); return vector[i]; }
	бул     перемест(цел alloc);
	проц     ReAllocF(цел alloc);
	бул     GrowSz();
	проц     GrowF();
	T&       нарастиДобавь(const T& x);
	T&       нарастиДобавь(T&& x);
	проц     вставьРяд(цел q, цел count);

	template <class U> friend class Индекс;

public:
	T&       добавь()                           { if(items >= alloc) GrowF(); return *(::new(Rdd()) T); }
	T&       добавь(const T& x)                 { return items < alloc ? *(new(Rdd()) T(x)) : нарастиДобавь(x); }
	T&       добавь(T&& x)                      { return items < alloc ? *(::new(Rdd()) T(пикуй(x))) : нарастиДобавь(пикуй(x)); }
	template <class... Арги>
	T&       создай(Арги&&... арги)          { if(items >= alloc) GrowF(); return *(::new(Rdd()) T(std::forward<Арги>(арги)...)); }
	проц     добавьН(цел n);
	const T& operator[](цел i) const         { return дай(i); }
	T&       operator[](цел i)               { return дай(i); }
	const T& дай(цел i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(цел i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	цел      дайСчёт() const                { return items; }
	бул     пустой() const                 { return items == 0; }
	проц     обрежь(цел n);
	проц     устСчёт(цел n);
	проц     устСчёт(цел n, const T& init);
	проц     устСчётР(цел n);
	проц     устСчётР(цел n, const T& init);
	проц     очисть();

	T&       по(цел i)                  { if(i >= items) устСчётР(i + 1); return (*this)[i]; }
	T&       по(цел i, const T& x)      { if(i >= items) устСчётР(i + 1, x); return (*this)[i]; }

	проц     сожми()                   { if(items != alloc) ReAllocF(items); }
	проц     резервируй(цел n);
	цел      дайРазмест() const           { return alloc; }

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(цел i, T&& x)          { return по(i) = пикуй(x); }
	template <class Диапазон>
	проц     устДиапазон(цел i, const Диапазон& r);

	проц     удали(цел i, цел count = 1);
	проц     удали(const цел *sorted_list, цел n);
	проц     удали(const Вектор<цел>& sorted_list);
	template <class Условие>
	проц     удалиЕсли(Условие c);

	проц     вставьН(цел i, цел count = 1);
	T&       вставь(цел i)              { вставьН(i); return дай(i); }
	проц     вставь(цел i, const T& x, цел count);
	T&       вставь(цел i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(цел i, T&& x);
	проц     вставь(цел i, const Вектор& x);
	проц     вставь(цел i, const Вектор& x, цел offset, цел count);
	проц     вставь(цел i, Вектор&& x);
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r);
	проц     приставь(const Вектор& x)               { вставь(дайСчёт(), x); }
	проц     приставь(const Вектор& x, цел o, цел c) { вставь(дайСчёт(), x, o, c); }
	проц     приставь(Вектор&& x)                    { вставь(дайСчёт(), пикуй(x)); }
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)           { вставьДиапазон(дайСчёт(), r); }

	проц     вставьРазбей(цел i, Вектор<T>& v, цел from);

	проц     разверни(цел i1, цел i2)    { РНЦП::разверни(дай(i1), дай(i2)); }

	проц     сбрось(цел n = 1)         { ПРОВЕРЬ(n <= дайСчёт()); обрежь(items - n); }
	T&       верх()                   { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	const T& верх() const             { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	T        вынь()                   { T h = пикуй(верх()); сбрось(); return h; }

	operator T*()                    { return (T*)vector; }
	operator const T*() const        { return (T*)vector; }

	Вектор&  operator<<(const T& x)  { добавь(x); return *this; }
	Вектор&  operator<<(T&& x)            { добавь(пикуй(x)); return *this; }

#ifdef РНЦП
	проц     сериализуй(Поток& s)                        { StreamContainer(s, *this); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "элт");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                        { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }
#endif

	Вектор()                                  { обнули(); }
	explicit Вектор(цел n)                    { items = n; vector = разместиРяд(n); alloc = n; строй(vector, vector + n); }
	explicit Вектор(цел n, const T& init)     { items = n; vector = разместиРяд(n); alloc = n; стройЗаполниГлубКопию(vector, vector + n, init); }
	~Вектор() {
		освободи();
		return; // Constraints:
		проверьДвиж((T *)0);  // T must be moveable
	}

// подбери assignment & copy. Picked source can only do очисть(), ~Вектор(), operator=, operator <<=
	Вектор(Вектор&& v)               { подбери(пикуй(v)); }
	проц operator=(Вектор&& v)       { if(this != &v) { освободи(); подбери(пикуй(v)); } }

	проц     вставь(цел i, std::initializer_list<T> init);
	проц     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Вектор(std::initializer_list<T> init) { vector = NULL; items = alloc = 0; вставь(0, init); }

// Deep copy
	Вектор(const Вектор& v, цел)     { __глубКопия(v); }

// Standard container interface
	const T         *begin() const          { return (T*)vector; }
	const T         *end() const            { return (T*)vector + items; }
	T               *begin()                { return (T*)vector; }
	T               *end()                  { return (T*)vector + items; }

// Optimizations
	friend проц разверни(Вектор& a, Вектор& b)  { РНЦП::разверни(a.items, b.items); РНЦП::разверни(a.alloc, b.alloc); РНЦП::разверни(a.vector, b.vector); }

#ifdef DEPRECATED
	T&       индексируй(цел i)             { return по(i); }
	T&       индексируй(цел i, const T& x) { return по(i, x); }
	T&       добавьПодбор(T&& x)             { return items < alloc ? *(::new(Rdd()) T(пикуй(x))) : нарастиДобавь(пикуй(x)); }
	цел      дайИндекс(const T& элт) const;
	T&       вставьПодбор(цел i, T&& x)   { return вставь(i, пикуй(x)); }
	проц     вставьПодбор(цел i, Вектор&& x) { вставь(i, пикуй(x)); }
	проц     приставьПодбор(Вектор&& x)                { вставьПодбор(дайСчёт(), пикуй(x)); }
	typedef T       *Обходчик;
	typedef const T *КонстОбходчик;
	КонстОбходчик    дайОбх(цел i) const   { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	Обходчик         дайОбх(цел i)         { ПРОВЕРЬ(i >= 0 && i <= items); return begin() + i; }
	typedef T        ТипЗнач;
	friend проц приставь(Вектор& dst, const Вектор& ист)         { dst.приставь(ист); }

	STL_VECTOR_COMPATIBILITY(Вектор<T>)
#endif
};

template <class T>
class Массив : public ОпцияДвижимогоИГлубКопии< Массив<T> > {
protected:
#ifdef _ОТЛАДКА
	typedef T *ТипУказатель;
#else
	typedef ук ТипУказатель;
#endif
	Вектор<ТипУказатель> vector;

	проц     освободи();
	проц     __глубКопия(const Массив& v);
	T&       дай(цел i) const                           { return *(T *)vector[i]; }
	T      **дайУк(цел i) const                        { return (T **)vector.begin() + i; }

	проц     уд(ТипУказатель *укз, ТипУказатель *lim)              { while(укз < lim) delete (T *) *укз++; }
	проц     иниц(ТипУказатель *укз, ТипУказатель *lim)             { while(укз < lim) *укз++ = new T; }
	проц     иниц(ТипУказатель *укз, ТипУказатель *lim, const T& x) { while(укз < lim) *укз++ = new T(клонируй(x)); }

public:
	T&       добавь()                           { T *q = new T; vector.добавь(q); return *q; }
	T&       добавь(const T& x)                 { T *q = new T(x); vector.добавь(q); return *q; }
	T&       добавь(T&& x)                      { T *q = new T(пикуй(x)); vector.добавь(q); return *q; }
	T&       добавь(T *newt)                    { vector.добавь(newt); return *newt; }
	T&       добавь(Один<T>&& one)               { ПРОВЕРЬ(one); return добавь(one.открепи()); }
	template<class... Арги>
	T&       создай(Арги&&... арги)          { T *q = new T(std::forward<Арги>(арги)...); добавь(q); return *q; }
	template<class TT, class... Арги>
	TT&      создай(Арги&&... арги)          { TT *q = new TT(std::forward<Арги>(арги)...); добавь(q); return *q; }
	const T& operator[](цел i) const         { return дай(i); }
	T&       operator[](цел i)               { return дай(i); }
	const T& дай(цел i, const T& опр) const  { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	T&       дай(цел i, T& опр)              { return i >= 0 && i < дайСчёт() ? дай(i) : опр; }
	цел      дайСчёт() const                { return vector.дайСчёт(); }
	бул     пустой() const                 { return vector.пустой(); }
	проц     обрежь(цел n);
	проц     устСчёт(цел n);
	проц     устСчёт(цел n, const T& init);
	проц     устСчётР(цел n);
	проц     устСчётР(цел n, const T& init);
	проц     очисть()                    { освободи(); vector.очисть(); }

	T&       по(цел i)                  { if(i >= дайСчёт()) устСчётР(i + 1); return дай(i); }
	T&       по(цел i, const T& x)      { if(i >= дайСчёт()) устСчётР(i + 1, x); return дай(i); }

	проц     сожми()                   { vector.сожми(); }
	проц     резервируй(цел xtra)          { vector.резервируй(xtra); }
	цел      дайРазмест() const           { return vector.дайРазмест(); }

	проц     уст(цел i, const T& x, цел count);
	T&       уст(цел i, const T& x)     { уст(i, x, 1); return дай(i); }
	T&       уст(цел i, T&& x)          { return по(i) = пикуй(x); }
	проц     удали(цел i, цел count = 1);
	проц     удали(const цел *sorted_list, цел n);
	проц     удали(const Вектор<цел>& sorted_list);
	template <class Условие>
	проц     удалиЕсли(Условие c);
	проц     вставьН(цел i, цел count = 1);
	T&       вставь(цел i)              { вставьН(i); return дай(i); }
	проц     вставь(цел i, const T& x, цел count);
	T&       вставь(цел i, const T& x)  { вставь(i, x, 1); return дай(i); }
	T&       вставь(цел i, T&& x);
	проц     вставь(цел i, const Массив& x);
	проц     вставь(цел i, const Массив& x, цел offset, цел count);
	template <class Диапазон>
	проц     вставьДиапазон(цел i, const Диапазон& r);
	проц     вставь(цел i, Массив&& x)             { vector.вставь(i, пикуй(x.vector)); }
	проц     приставь(const Массив& x)               { вставь(дайСчёт(), x); }
	проц     приставь(const Массив& x, цел o, цел c) { вставь(дайСчёт(), x, o, c); }
	проц     приставь(Массив&& x)                    { вставьПодбор(дайСчёт(), пикуй(x)); }
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)          { вставьДиапазон(дайСчёт(), r); }

	проц     разверни(цел i1, цел i2)                 { РНЦП::разверни(vector[i1], vector[i2]); }
	проц     перемести(цел i1, цел i2);

	T&       уст(цел i, T *newt)        { delete (T *)vector[i]; vector[i] = newt; return *newt; }
	T&       вставь(цел i, T *newt);

	проц     сбрось(цел n = 1)            { обрежь(дайСчёт() - n); }
	T&       верх()                      { return дай(дайСчёт() - 1); }
	const T& верх() const                { return дай(дайСчёт() - 1); }

	T       *открепи(цел i)              { T *t = &дай(i); vector.удали(i); return t; }
	T       *разверни(цел i, T *newt)       { T *tmp = (T*)vector[i]; vector[i] = newt; return tmp; }
	T       *выньОткрепи()                { return (T *) vector.вынь(); }

	проц     разверни(Массив& b)             { разверни(vector, b.vector); }

	Массив& operator<<(const T& x)       { добавь(x); return *this; }
	Массив& operator<<(T&& x)            { добавь(пикуй(x)); return *this; }
	Массив& operator<<(T *newt)          { добавь(newt); return *this; }

	проц     сериализуй(Поток& s)       { StreamContainer(s, *this); }
	проц     вРяр(РярВВ& xio, кткст0 itemtag = "элт");
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const       { return хэшПоСериализу(*this); }

	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	Массив()                                          {}
	explicit Массив(цел n) : vector(n)                { иниц(vector.begin(), vector.end()); }
	explicit Массив(цел n, const T& init) : vector(n) { иниц(vector.begin(), vector.end(), init); }
	~Массив()                                         { освободи(); }

// подбери assignment & copy. Picked source can only очисть(), ~Вектор(), operator=, operator<<=
	Массив(Массив&& v) : vector(пикуй(v.vector))  {}
	проц operator=(Массив&& v)           { if(this != &v) { освободи(); vector = пикуй(v.vector); } }

// Deep copy
	Массив(const Массив& v, цел)          { __глубКопия(v); }

	проц     вставь(цел i, std::initializer_list<T> init);
	проц     приставь(std::initializer_list<T> init) { вставь(дайСчёт(), init); }
	Массив(std::initializer_list<T> init) { вставь(0, init); }

	class Обходчик;

	class КонстОбходчик {
	protected:
		T **укз;
		КонстОбходчик(T **p)                    { укз = p; }

		friend class Массив<T>;
		struct NP { цел dummy; };

	public:
		const T& operator*() const              { return **укз; }
		const T *operator->() const             { return *укз; }
		const T& operator[](цел i) const        { return *укз[i]; }

		КонстОбходчик& operator++()             { укз++; return *this; }
		КонстОбходчик& operator--()             { укз--; return *this; }
		КонстОбходчик  operator++(цел)          { КонстОбходчик t = *this; ++*this; return t; }
		КонстОбходчик  operator--(цел)          { КонстОбходчик t = *this; --*this; return t; }

		КонстОбходчик& operator+=(цел i)        { укз += i; return *this; }
		КонстОбходчик& operator-=(цел i)        { укз -= i; return *this; }

		КонстОбходчик operator+(цел i) const    { return укз + i; }
		КонстОбходчик operator-(цел i) const    { return укз - i; }

		цел  operator-(КонстОбходчик x) const   { return (цел)(укз - x.укз); }

		бул operator==(КонстОбходчик x) const  { return укз == x.укз; }
		бул operator!=(КонстОбходчик x) const  { return укз != x.укз; }
		бул operator<(КонстОбходчик x) const   { return укз < x.укз; }
		бул operator>(КонстОбходчик x) const   { return укз > x.укз; }
		бул operator<=(КонстОбходчик x) const  { return укз <= x.укз; }
		бул operator>=(КонстОбходчик x) const  { return укз >= x.укз; }

		operator бул() const                   { return укз; }

		КонстОбходчик()                         {}
		КонстОбходчик(NP *null)                 { ПРОВЕРЬ(null == NULL); укз = NULL; }
		
		STL_ITERATOR_COMPATIBILITY
	};

	class Обходчик : public КонстОбходчик {
		friend class Массив<T>;
		Обходчик(T **p) : КонстОбходчик(p)      {}
		typedef КонстОбходчик B;
		struct NP { цел dummy; };

	public:
		T& operator*()                          { return **B::укз; }
		T *operator->()                         { return *B::укз; }
		T& operator[](цел i)                    { return *B::укз[i]; }

		const T& operator*() const              { return **B::укз; }
		const T *operator->() const             { return *B::укз; }
		const T& operator[](цел i) const        { return *B::укз[i]; }

		Обходчик& operator++()                  { B::укз++; return *this; }
		Обходчик& operator--()                  { B::укз--; return *this; }
		Обходчик  operator++(цел)               { Обходчик t = *this; ++*this; return t; }
		Обходчик  operator--(цел)               { Обходчик t = *this; --*this; return t; }

		Обходчик& operator+=(цел i)             { B::укз += i; return *this; }
		Обходчик& operator-=(цел i)             { B::укз -= i; return *this; }

		Обходчик operator+(цел i) const         { return B::укз + i; }
		Обходчик operator-(цел i) const         { return B::укз - i; }

		цел      operator-(Обходчик x) const    { return B::operator-(x); }

		Обходчик()                               {}
		Обходчик(NP *null) : КонстОбходчик(null) {}

		STL_ITERATOR_COMPATIBILITY
	};

// Standard container interface
	Обходчик         begin()                    { return (T **)vector.begin(); }
	Обходчик         end()                      { return (T **)vector.end(); }
	КонстОбходчик    begin() const              { return (T **)vector.begin(); }
	КонстОбходчик    end() const                { return (T **)vector.end(); }

// Optimalization
	friend проц разверни(Массив& a, Массив& b)                   { РНЦП::разверни(a.vector, b.vector); }
	//GCC forced move from Обходчик, ugly workaround
private:
	static проц IterSwap0(Обходчик a, Обходчик b)          { РНЦП::разверни(*a.укз, *b.укз); }
public:
	friend проц IterSwap(Обходчик a, Обходчик b)           { Массив<T>::IterSwap0(a, b); }

#ifdef DEPRECATED
	цел      дайИндекс(const T& элт) const;
	проц     вставьПодбор(цел i, Массив&& x)       { vector.вставьПодбор(i, пикуй(x.vector)); }
	проц     приставьПодбор(Массив&& x)              { вставьПодбор(дайСчёт(), пикуй(x)); }
	T&       индексируй(цел i)                     { return по(i); }
	T&       индексируй(цел i, const T& x)         { return по(i, x); }
	T&       добавьПодбор(T&& x)                     { T *q = new T(пикуй(x)); vector.добавь(q); return *q; }
	T&       вставьПодбор(цел i, T&& x)           { return вставь(i, пикуй(x)); }
	typedef T        ТипЗнач;
	Обходчик         дайОбх(цел pos)           { return (T **)vector.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const     { return (T **)vector.дайОбх(pos); }
#endif
	STL_VECTOR_COMPATIBILITY(Массив<T>)
};

////////////////////////
//BiCont.h
template <class T>
class БиВектор : ОпцияДвижимогоИГлубКопии< БиВектор<T> > {
protected:
	T       *vector;
	цел      start;
	цел      items;
	цел      alloc;

	цел      Ix(цел i) const         { return i + start < alloc ? i + start : i + start - alloc; }

	цел      EI() const              { return Ix(items - 1); }
	проц     перемест(цел newalloc);
	проц     добавь0();
	проц     глубокаяКопия0(const БиВектор& ист);
	T       *добавьГолову0()              { проверьДвиж<T>(); добавь0(); return &vector[start = Ix(alloc - 1)/*(start + alloc - 1) % alloc*/]; }
	T       *добавьХвост0()              { проверьДвиж<T>(); добавь0(); return &vector[EI()]; }
	проц     обнули()                  { start = items = alloc = 0; vector = NULL; }
	проц     освободи();
	проц     подбери(БиВектор&& x)      { vector = пикуй(x.vector); start = x.start; items = x.items;
	                                   alloc = x.alloc; ((БиВектор&)x).items = -1; }
	проц     копируй(T *dst, цел start, цел count) const;

public:
	цел      дайСчёт() const        { return items; }
	бул     пустой() const         { return items == 0; }
	проц     очисть();

	T&       добавьГолову()               { return *new(добавьГолову0()) T; }
	T&       добавьХвост()               { return *new(добавьХвост0()) T; }
	проц     добавьГолову(const T& x)     { new(добавьГолову0()) T(x); }
	проц     добавьХвост(const T& x)     { new(добавьХвост0()) T(x); }
	проц     добавьГолову(T&& x)          { new(добавьГолову0()) T(пикуй(x)); }
	проц     добавьХвост(T&& x)          { new(добавьХвост0()) T(пикуй(x)); }
	T&       дайГолову()                  { ПРОВЕРЬ(items > 0); return vector[start]; }
	T&       дайХвост()                  { ПРОВЕРЬ(items > 0); return vector[EI()]; }
	const T& дайГолову() const            { ПРОВЕРЬ(items > 0); return vector[start]; }
	const T& дайХвост() const            { ПРОВЕРЬ(items > 0); return vector[EI()]; }
	проц     сбросьГолову()              { (&дайГолову())->~T(); items--; start = Ix(1); }
	проц     сбросьХвост()              { (&дайХвост())->~T(); items--; }
	T        выньГолову()               { T x = дайГолову(); сбросьГолову(); return x; }
	T        выньХвост()               { T x = дайХвост(); сбросьХвост(); return x; }
	проц     сбросьГолову(цел n)         { while(n-- > 0) БиВектор<T>::сбросьГолову(); }
	проц     сбросьХвост(цел n)         { while(n-- > 0) БиВектор<T>::сбросьХвост(); }
	const T& operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i < items); return vector[Ix(i)]; }
	T&       operator[](цел i)       { ПРОВЕРЬ(i >= 0 && i < items); return vector[Ix(i)]; }
	проц     сожми();
	проц     резервируй(цел n);
	цел      дайРазмест() const        { return alloc; }

	проц     сериализуй(Поток& s);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const    { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	БиВектор(const БиВектор& ист, цел)          { глубокаяКопия0(ист); }
	БиВектор(БиВектор&& ист)                    { подбери(пикуй(ист)); }
	проц operator=(БиВектор&& ист)              { if(this != &ист) { освободи(); подбери(пикуй(ист)); } }
	БиВектор()                                  { обнули(); }
	~БиВектор()                                 { освободи(); } // gcc4.0 workaround!!

	БиВектор(std::initializer_list<T> init);

	typedef ОбходчикКонстИ<БиВектор> КонстОбходчик;
	typedef ОбходчикИ<БиВектор>      Обходчик;

	КонстОбходчик    begin() const              { return КонстОбходчик(*this, 0); }
	КонстОбходчик    end() const                { return КонстОбходчик(*this, дайСчёт()); }
	Обходчик         begin()                    { return Обходчик(*this, 0); }
	Обходчик         end()                      { return Обходчик(*this, дайСчёт()); }

	friend проц разверни(БиВектор& a, БиВектор& b)  { разверни(a.vector, b.vector);
	                                              разверни(a.start, b.start);
	                                              разверни(a.items, b.items);
	                                              разверни(a.alloc, b.alloc); }
};

template <class T>
class БиМассив : ОпцияДвижимогоИГлубКопии< БиМассив<T> > {
protected:
	БиВектор<проц *> bv;

	проц     освободи();
	проц     глубокаяКопия0(const БиМассив<T>& v);

public:
	цел      дайСчёт() const              { return bv.дайСчёт(); }
	бул     пустой() const               { return дайСчёт() == 0; }
	проц     очисть()                       { освободи(); bv.очисть(); }

	T&       добавьГолову()                     { T *q = new T; bv.добавьГолову(q); return *q; }
	T&       добавьХвост()                     { T *q = new T; bv.добавьХвост(q); return *q; }
	проц     добавьГолову(const T& x)           { bv.добавьГолову(new T(x)); }
	проц     добавьХвост(const T& x)           { bv.добавьХвост(new T(x)); }
	T&       добавьГолову(T *newt)              { bv.добавьГолову(newt); return *newt; }
	T&       добавьХвост(T *newt)              { bv.добавьХвост(newt); return *newt; }
	template <class TT> TT& создайГоолову()   { TT *q = new TT; bv.добавьГолову(q); return *q; }
	template <class TT> TT& создайХвост()   { TT *q = new TT; bv.добавьХвост(q); return *q; }
	T&       добавьГолову(Один<T>&& one)         { ПРОВЕРЬ(one); return добавьГолову(one.открепи()); }
	T&       добавьХвост(Один<T>&& one)         { ПРОВЕРЬ(one); return добавьХвост(one.открепи()); }
	T&       дайГолову()                        { return *(T *) bv.дайГолову(); }
	T&       дайХвост()                        { return *(T *) bv.дайХвост(); }
	const T& дайГолову() const                  { return *(const T *) bv.дайГолову(); }
	const T& дайХвост() const                  { return *(const T *) bv.дайХвост(); }
	проц     сбросьГолову()                    { delete (T*) bv.дайГолову(); bv.сбросьГолову(); }
	проц     сбросьХвост()                    { delete (T*) bv.дайХвост(); bv.сбросьХвост(); }
	T       *открепиГолову()                  { T *q = (T*) bv.дайГолову(); bv.сбросьГолову(); return q; }
	T       *открепиХвост()                  { T *q = (T*) bv.дайХвост(); bv.сбросьХвост(); return q; }

	T&       operator[](цел i)             { return *(T *) bv[i]; }
	const T& operator[](цел i) const       { return *(const T *) bv[i]; }

	проц     сожми()                      { bv.сожми(); }
	проц     резервируй(цел n)                { bv.резервируй(n); }
	цел      дайРазмест() const              { return bv.дайРазмест(); }

	проц     сериализуй(Поток& s);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const    { return хэшПоСериализу(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	БиМассив(const БиМассив& v, цел)           { глубокаяКопия0(v); }

	БиМассив(БиМассив&& ист) : bv(пикуй(ист.bv)){}
	проц operator=(БиМассив&& ист)            { if(this != &ист) { освободи(); bv = пикуй(ист.bv); } }
	БиМассив()                                {}
	~БиМассив()                               { освободи(); }

	БиМассив(std::initializer_list<T> init);

	typedef ОбходчикКонстИ<БиМассив> КонстОбходчик;
	typedef ОбходчикИ<БиМассив>      Обходчик;

	КонстОбходчик    begin() const              { return КонстОбходчик(*this, 0); }
	КонстОбходчик    end() const                { return КонстОбходчик(*this, дайСчёт()); }
	Обходчик         begin()                    { return Обходчик(*this, 0); }
	Обходчик         end()                      { return Обходчик(*this, дайСчёт()); }

	friend проц разверни(БиМассив& a, БиМассив& b)    { разверни(a.bv, b.bv); }
};

//////////
//Range.h

template <class T>
T *ДеклУкз__();

template <class Диапазон>
using типЗначУ = typename std::remove_reference<decltype(*ДеклУкз__<typename std::remove_reference<Диапазон>::type>()->begin())>::type;

template <class Диапазон>
using ОбходчикУ = decltype(ДеклУкз__<typename std::remove_reference<Диапазон>::type>()->begin());

template <class Диапазон>
using КонстОбходчикУ = decltype(ДеклУкз__<const typename std::remove_reference<Диапазон>::type>()->begin());

template <class I>
class КлассСубДиапазон {
	I   l;
	цел count;

public:
	typedef typename std::remove_reference<decltype(*l)>::type value_type;

	цел дайСчёт() const { return count; }

	КлассСубДиапазон& пиши()                   { return *this; }

	value_type& operator[](цел i) const { ПРОВЕРЬ(i >= 0 && i < count); return l[i]; }
	I  begin() const { return l; }
	I  end() const { return l + count; }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассСубДиапазон(I begin, цел count) : l(begin), count(count)   {}
	КлассСубДиапазон(I begin, I end) : l(begin), count(цел(end - begin)) {}
	КлассСубДиапазон() {} // MSC bug workaround
};

template <class I>
КлассСубДиапазон<I> СубДиапазон(I l, I h)
{
	return КлассСубДиапазон<I>(l, h);
}

template <class I>
КлассСубДиапазон<I> СубДиапазон(I l, цел count)
{
	return КлассСубДиапазон<I>(l, count);
}

template <class C>
auto СубДиапазон(C&& c, цел pos, цел count) -> decltype(СубДиапазон(c.begin() + pos, count))
{
	return СубДиапазон(c.begin() + pos, count);
}

template <class C> using СубДиапазонУ = decltype(СубДиапазон(((C *)0)->begin(), ((C *)0)->end()));

template <class T>
struct КлассКонстДиапазон {
	T   значение;
	цел count;

	typedef T value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](цел i) const { return значение; }
	цел дайСчёт() const                      { return count; }

	typedef ОбходчикКонстИ<КлассКонстДиапазон> Обходчик;

	Обходчик begin() const { return Обходчик(*this, 0); }
	Обходчик end() const { return Обходчик(*this, count); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассКонстДиапазон(const T& значение, цел count) : значение(значение), count(count) {}
	КлассКонстДиапазон(цел count) : count(count) {}
	КлассКонстДиапазон() {} // MSC bug workaround
};

template <class T>
КлассКонстДиапазон<T> КонстДиапазон(const T& значение, цел count)
{
	return КлассКонстДиапазон<T>(значение, count);
}

template <class T>
КлассКонстДиапазон<T> КонстДиапазон(цел count)
{
	return КлассКонстДиапазон<T>(count);
}

template <class ДиапазонОснова>
struct КлассРеверсДиапазон {
	typename std::remove_reference<ДиапазонОснова>::type& r;

	typedef типЗначУ<ДиапазонОснова>                       value_type;
	typedef value_type                                   ТипЗнач;

	const value_type& operator[](цел i) const            { return r[r.дайСчёт() - i - 1]; }
	value_type& operator[](цел i)                        { return r[r.дайСчёт() - i - 1]; }
	цел дайСчёт() const                                 { return r.дайСчёт(); }

	typedef ОбходчикИ<КлассРеверсДиапазон>                  Обходчик;
	typedef ОбходчикКонстИ<КлассРеверсДиапазон>             КонстОбходчик;

	КлассРеверсДиапазон& пиши()                           { return *this; }

	КонстОбходчик begin() const                          { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const                            { return КонстОбходчик(*this, r.дайСчёт()); }

	Обходчик begin()                                     { return Обходчик(*this, 0); }
	Обходчик end()                                       { return Обходчик(*this, r.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассРеверсДиапазон(ДиапазонОснова& r) : r(r) {}
	КлассРеверсДиапазон() {} // MSC bug workaround
};

template <class ДиапазонОснова>
КлассРеверсДиапазон<ДиапазонОснова> РеверсДиапазон(ДиапазонОснова&& r)
{
	return КлассРеверсДиапазон<ДиапазонОснова>(r);
}

template <class ДиапазонОснова>
struct КлассДиапазонОбзора {
	typename std::remove_reference<ДиапазонОснова>::тип *r;
	Вектор<цел> ndx;

	typedef типЗначУ<ДиапазонОснова> value_type;
	typedef value_type ТипЗнач;

	const value_type& operator[](цел i) const { return (*r)[ndx[i]]; }
	value_type& operator[](цел i)             { return (*r)[ndx[i]]; }
	цел дайСчёт() const                      { return ndx.дайСчёт(); }

	typedef ОбходчикИ<КлассДиапазонОбзора> Обходчик;
	typedef ОбходчикКонстИ<КлассДиапазонОбзора> КонстОбходчик;

	КлассДиапазонОбзора& пиши()                   { return *this; }

	КонстОбходчик begin() const { return КонстОбходчик(*this, 0); }
	КонстОбходчик end() const  { return КонстОбходчик(*this, ndx.дайСчёт()); }

	Обходчик begin() { return Обходчик(*this, 0); }
	Обходчик end()   { return Обходчик(*this, ndx.дайСчёт()); }

	Ткст   вТкст() const                            { return AsStringArray(*this); }
	template <class B> бул operator==(const B& b) const { return диапазоныРавны(*this, b); }
	template <class B> бул operator!=(const B& b) const { return !operator==(b); }
	template <class B> цел  сравни(const B& b) const    { return сравниДиапазоны(*this, b); }
	template <class B> бул operator<=(const B& x) const { return сравни(x) <= 0; }
	template <class B> бул operator>=(const B& x) const { return сравни(x) >= 0; }
	template <class B> бул operator<(const B& x) const  { return сравни(x) < 0; }
	template <class B> бул operator>(const B& x) const  { return сравни(x) > 0; }

	КлассДиапазонОбзора(ДиапазонОснова& r, Вектор<цел>&& ndx) : r(&r), ndx(пикуй(ndx)) {}
	КлассДиапазонОбзора() {} // MSC bug workaround
};

template <class ДиапазонОснова>
КлассДиапазонОбзора<ДиапазонОснова> ДиапазонОбзора(ДиапазонОснова&& r, Вектор<цел>&& ndx)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, пикуй(ndx));
}

template <class ДиапазонОснова, class Предикат>
КлассДиапазонОбзора<ДиапазонОснова> ДиапазонФильтр(ДиапазонОснова&& r, Предикат p)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, найдиВсе(r, p));
}

template <class ДиапазонОснова, class Предикат>
КлассДиапазонОбзора<ДиапазонОснова> СортированныйДиапазон(ДиапазонОснова&& r, Предикат p)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, дайСортПорядок(r, p));
}

template <class ДиапазонОснова>
КлассДиапазонОбзора<ДиапазонОснова> СортированныйДиапазон(ДиапазонОснова&& r)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, дайСортПорядок(r));
}
/////////
//Algo.h
template <class Диапазон>
проц реверс(Диапазон&& r)
{
	auto start = r.begin();
	auto end = r.end();
	if(start != end && --end != start)
		do
			IterSwap(start, end);
		while(++start != end && --end != start);
}

template <class Диапазон, class Накопитель>
проц накопи(const Диапазон& r, Накопитель& a)
{
	for(const auto& e : r)
		a(e);
}

template <class Диапазон>
типЗначУ<Диапазон> сумма(const Диапазон& r, const типЗначУ<Диапазон>& ноль)
{
	типЗначУ<Диапазон> сум = ноль;
	for(const auto& e : r)
		сум += e;
	return сум;
}

template <class T>
типЗначУ<T> сумма(const T& c)
{
	return сумма(c, (типЗначУ<T>)0);
}

template <class Диапазон, class V>
цел счёт(const Диапазон& r, const V& знач)
{
	цел count = 0;
	for(const auto& x : r)
		if(x == знач)
			count++;
	return count;
}

template <class Диапазон, class Предикат>
цел счётЕсли(const Диапазон& r, const Предикат& p)
{
	цел count = 0;
	for(const auto& x : r)
		if(p(x))
			count++;
	return count;
}

template <class Диапазон, class Предикат>
цел найдиЛучший(const Диапазон& r, const Предикат& pred)
{
	цел count = r.дайСчёт();
	if(count == 0)
		return -1;
	auto *m = &r[0];
	цел mi = 0;
	for(цел i = 1; i < count; i++)
		if(pred(r[i], *m)) {
			m = &r[i];
			mi = i;
		}
	return mi;
}

template <class Диапазон>
цел найдиМин(const Диапазон& r)
{
	return найдиЛучший(r, std::less<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& мин(const Диапазон& r)
{
	return r[найдиМин(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& мин(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	цел q = найдиМин(r);
	return q < 0 ? опр : r[q];
}


template <class Диапазон>
цел найдиМакс(const Диапазон& r)
{
	return найдиЛучший(r, std::greater<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& макс(const Диапазон& r)
{
	return r[найдиМакс(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& макс(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	цел q = найдиМакс(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон1, class Диапазон2>
бул диапазоныРавны(const Диапазон1& a, const Диапазон2& b)
{
	if(a.дайСчёт() != b.дайСчёт())
		return false;
	for(цел i = 0; i < a.дайСчёт(); i++)
		if(!(a[i] == b[i]))
			return false;
	return true;
}

template <class Диапазон1, class Диапазон2, class Сравн>
цел сравниДиапазоны(const Диапазон1& a, const Диапазон2& b, Сравн сравни)
{
	цел n = min(a.дайСчёт(), b.дайСчёт());
	for(цел i = 0; i < n; i++) {
		цел q = сравни(a[i], b[i]);
		if(q)
			return q;
	}
	return сравни(a.дайСчёт(), b.дайСчёт());
}

template <class Диапазон1, class Диапазон2>
цел сравниДиапазоны(const Диапазон1& a, const Диапазон2& b)
{
	цел n = min(a.дайСчёт(), b.дайСчёт());
	for(цел i = 0; i < n; i++) {
		цел q = сравниЗнак(a[i], b[i]);
		if(q)
			return q;
	}
	return сравниЗнак(a.дайСчёт(), b.дайСчёт());
}

template <class Диапазон, class C>
цел найдиСовпадение(const Диапазон& r, const C& match, цел from = 0)
{
	for(цел i = from; i < r.дайСчёт(); i++)
		if(match(r[i])) return i;
	return -1;
}

template <class Диапазон, class V>
цел найдиИндекс(const Диапазон& r, const V& значение, цел from = 0)
{
	for(цел i = from; i < r.дайСчёт(); i++)
		if(r[i] == значение) return i;
	return -1;
}

template <class Диапазон, class Предикат>
Вектор<цел> найдиВсе(const Диапазон& r, Предикат match, цел from = 0)
{
	Вектор<цел> ndx;
	for(цел i = from; i < r.дайСчёт(); i++)
		if(match(r[i]))
			ndx.добавь(i);
	return ndx;
}

template <class Диапазон, class Предикат>
Вектор<цел> найдиВсеи(const Диапазон& r, Предикат match, цел from = 0)
{
	Вектор<цел> ndx;
	for(цел i = from; i < r.дайСчёт(); i++)
		if(match(i))
			ndx.добавь(i);
	return ndx;
}

template <class Диапазон, class T, class Less>
цел найдиНижнГран(const Диапазон& r, const T& знач, const Less& less)
{
	цел pos = 0;
	цел count = r.дайСчёт();
	while(count > 0) {
		цел half = count >> 1;
		цел m = pos + half;
		if(less(r[m], знач)) {
			pos = m + 1;
			count = count - half - 1;
		}
		else
			count = half;
	}
	return pos;
}

template <class Диапазон, class T>
цел найдиНижнГран(const Диапазон& r, const T& знач)
{
	return найдиНижнГран(r, знач, std::less<T>());
}

template <class Диапазон, class T, class L>
цел найдиВерхнГран(const Диапазон& r, const T& знач, const L& less)
{
	цел pos = 0;
	цел count = r.дайСчёт();
	while(count > 0) {
		цел half = count >> 1;
		цел m = pos + half;
		if(less(знач, r[m]))
			count = half;
		else {
			pos = m + 1;
			count = count - half - 1;
		}
	}
	return pos;
}

template <class Диапазон, class T>
цел найдиВерхнГран(const Диапазон& r, const T& знач)
{
	return найдиВерхнГран(r, знач, std::less<T>());
}

template <class Диапазон, class T, class L>
цел найдиБинар(const Диапазон& r, const T& знач, const L& less)
{
	цел i = найдиНижнГран(r, знач, less);
	return i < r.дайСчёт() && !less(знач, r[i]) ? i : -1;
}

template <class Диапазон, class T>
цел найдиБинар(const Диапазон& r, const T& знач)
{
	return найдиБинар(r, знач, std::less<T>());
}

template <class Контейнер, class T>
проц добавьЛру(Контейнер& lru, T значение, цел limit = 10)
{
	цел q = найдиИндекс(lru, значение);
	if(q >= 0)
		lru.удали(q);
	lru.вставь(0, значение);
	if(lru.дайСчёт() > limit)
		lru.устСчёт(limit);
}

template <class C = Вектор<цел>, class V>
C сделайЙоту(V end, V start = 0, V step = 1)
{
	C x;
	x.резервируй((end - start) / step);
	for(V i = start; i < end; i += step)
		x.добавь(i);
	return x;
}
////////////////////////////////
///Other.h
template <class T, class... Арги>
T& Single(Арги... арги) {
	static T o(арги...);
	return o;
}

template <class T> // Workaround for GCC bug - specialization needed...
T& Single() {
	static T o;
	return o;
}

цел RegisterTypeNo__(кткст0 тип);

template <class T>
цел StaticTypeNo() {
	static цел typeno = RegisterTypeNo__(typeid(T).name());
	return typeno;
}

class Любое : Движ<Любое> {
	struct ДанныеОснова {
		цел      typeno;
		
		virtual ~ДанныеОснова() {}
	};

	template <class T>
	struct Данные : ДанныеОснова {
		T        данные;

		template <class... Арги>
		Данные(Арги&&... арги) : данные(std::forward<Арги>(арги)...) { typeno = StaticTypeNo<T>(); }
	};

	ДанныеОснова *укз;

	проц Chk() const                              { ПРОВЕРЬ(укз != (проц *)1); }
	проц подбери(Любое&& s)                            { укз = s.укз; const_cast<Любое&>(s).укз = NULL; }

public:
	template <class T, class... Арги> T& создай(Арги&&... арги) { очисть(); Данные<T> *x = new Данные<T>(std::forward<Арги>(арги)...); укз = x; return x->данные; }
	template <class T> бул является() const            { return укз && укз->typeno == StaticTypeNo<T>(); }
	template <class T> T& дай()                   { ПРОВЕРЬ(является<T>()); Chk(); return ((Данные<T>*)укз)->данные; }
	template <class T> const T& дай() const       { ПРОВЕРЬ(является<T>()); Chk(); return ((Данные<T>*)укз)->данные; }

	проц очисть()                                  { if(укз) delete укз; укз = NULL; }

	бул пустой() const                          { return укз == NULL; }

	проц operator=(Любое&& s)                       { if(this != &s) { очисть(); подбери(пикуй(s)); } }
	Любое(Любое&& s)                                  { подбери(пикуй(s)); }
	
	Любое(const Любое& s) = delete;
	проц operator=(const Любое& s) = delete;

	Любое()                                         { укз = NULL; }
	~Любое()                                        { очисть(); }
};

class Биты : Движ<Биты> {
	цел         alloc;
	бцел      *bp;
	
	проц расширь(цел q);
	проц переразмести(цел nalloc);
	цел  дайПоследн() const;

public:
	проц   очисть();
	проц   уст(цел i, бул b = true) { ПРОВЕРЬ(i >= 0 && alloc >= 0); цел q = i >> 5; if(q >= alloc) расширь(q);
	                                   i &= 31; bp[q] = (bp[q] & ~(1 << i)) | (b << i); }
	бул   дай(цел i) const        { ПРОВЕРЬ(i >= 0 && alloc >= 0); цел q = i >> 5;
	                                 return q < alloc ? bp[q] & (1 << (i & 31)) : false; }
	бул   operator[](цел i) const { return дай(i); }

	проц   уст(цел i, бцел bits, цел count);
	бцел  дай(цел i, цел count);
	проц   уст64(цел i, бдол bits, цел count);
	бдол дай64(цел i, цел count);

	проц   устН(цел i, бул b, цел count);
	проц   устН(цел i, цел count)         { устН(i, true, count); }
	
	проц   резервируй(цел nbits);
	проц   сожми();
	
	Ткст вТкст() const;

	бцел       *создайРяд(цел n_dwords);
	const бцел *дайРяд(цел& n_dwords) const { n_dwords = alloc; return bp; }
	бцел       *дайРяд(цел& n_dwords)       { n_dwords = alloc; return bp; }
	
	проц         сериализуй(Поток& s);

	Биты()                                { bp = NULL; alloc = 0; }
	~Биты()                               { очисть(); }

	Биты(Биты&& b)                        { alloc = b.alloc; bp = b.bp; b.bp = NULL; }
	проц operator=(Биты&& b)              { if(this != &b) { очисть(); alloc = b.alloc; bp = b.bp; b.bp = NULL; } }
	
	Биты(const Биты&) = delete;
	проц operator=(const Биты&) = delete;
};

//# System dependent
template <class T>
class Mitor : Движ< Mitor<T> > {
	union {
		mutable unsigned   count;
		mutable Вектор<T> *vector;
	};
	ббайт elem0[sizeof(T)];

	T&        дай(цел i) const;
	проц      подбери(Mitor&& m);
	проц      копируй(const Mitor& m);
	проц      Chk() const               { ПРОВЕРЬ(count != 2); }

public:
	T&        operator[](цел i)         { return дай(i); }
	const T&  operator[](цел i) const   { return дай(i); }
	цел       дайСчёт() const;
	T&        добавь();
	проц      добавь(const T& x);
	проц      очисть();
	проц      сожми();

	Mitor(Mitor&& m)                    { подбери(пикуй(m)); }
	проц operator=(Mitor&& m)           { if(this != &m) { очисть(); подбери(пикуй(m)); } }

	Mitor(Mitor& m, цел)                { копируй(m); }

	Mitor()                             { count = 0; }
	~Mitor()                            { очисть(); }
};

template <class T>
T& Mitor<T>::дай(цел i) const
{
	ПРОВЕРЬ(i >= 0 && i < дайСчёт());
	return i == 0 ? *(T*)elem0 : (*const_cast<Вектор<T>*>(vector))[i - 1];
}

template <class T>
проц Mitor<T>::подбери(Mitor&& m)
{
	m.Chk();
	vector = m.vector;
	memcpy(&elem0, &m.elem0, sizeof(T));
	m.count = 2;
}

template <class T>
проц Mitor<T>::копируй(const Mitor& m)
{
	m.Chk();
	if(m.count > 0)
		стройГлубКопию(elem0, (const T*)m.elem0);
	if(m.count > 1)
		vector = new Вектор<T>(*m.vector, 1);
}

template <class T>
цел Mitor<T>::дайСчёт() const
{
	Chk();
	return count > 1 ? vector->дайСчёт() + 1 : count;
}

template <class T>
T& Mitor<T>::добавь()
{
	Chk();
	if(count == 0) {
		count = 1;
		return *new(elem0) T;
	}
	if(count == 1)
		vector = new Вектор<T>;
	return vector->добавь();
}

template <class T>
проц Mitor<T>::добавь(const T& x)
{
	Chk();
	if(count == 0) {
		count = 1;
		new((T*) elem0) T(x);
	}
	else {
		if(count == 1)
			vector = new Вектор<T>;
		vector->добавь(x);
	}
}

template <class T>
проц Mitor<T>::очисть()
{
	if(count > 2)
		delete vector;
	if(count && count != 2)
		((T*)elem0)->~T();
	count = 0;
}

template <class T>
проц Mitor<T>::сожми()
{
	if(count > 2)
		vector->сожми();
}

//#
template <class T, цел N = 1>
struct Линк {
	T *link_prev[N];
	T *link_next[N];

protected:
	проц LPN(цел i)                      { link_prev[i]->link_next[i] = link_next[i]->link_prev[i] = (T *)this; }

public:
	NOUBSAN	T *дайУк()                  { return (T *) this; }
	const T *дайУк() const              { return (const T *) this; }
	T       *дайСледщ(цел i = 0)          { return link_next[i]; }
	T       *дайПредш(цел i = 0)          { return link_prev[i]; }
	const T *дайСледщ(цел i = 0) const    { return link_next[i]; }
	const T *дайПредш(цел i = 0) const    { return link_prev[i]; }

	NOUBSAN	проц линкуйся(цел i = 0)     { link_next[i] = link_prev[i] = (T *)this; }
	проц линкуйсяВесь()                   { for(цел i = 0; i < N; i++) линкуйся(i); }
	проц отлинкуй(цел i = 0)               { link_next[i]->link_prev[i] = link_prev[i]; link_prev[i]->link_next[i] = link_next[i];
	                                       линкуйся(i); }
	проц отлинкуйВсе()                     { for(цел i = 0; i < N; i++) отлинкуй(i); }
	NOUBSAN	проц линкПеред(Линк *n, цел i = 0)  { link_next[i] = (T *)n; link_prev[i] = link_next[i]->link_prev[i]; LPN(i); }
	NOUBSAN	проц линкПосле(Линк *p, цел i = 0)   { link_prev[i] = (T *)p; link_next[i] = link_prev[i]->link_next[i]; LPN(i); }

	T   *вставьСледщ(цел i = 0)           { T *x = new T; x->линкПосле(this, i); return x; }
	T   *вставьПредш(цел i = 0)           { T *x = new T; x->линкПеред(this, i); return x; }

	проц удалиСписок(цел i = 0)           { while(link_next[i] != дайУк()) delete link_next[i]; }

	бул вСписке(цел i = 0) const         { return link_next[i] != дайУк(); }
	бул пустой(цел i = 0) const        { return !вСписке(i); }

	Линк()                               { линкуйсяВесь(); }
	~Линк()                              { отлинкуйВсе(); }

private:
	Линк(const Линк&);
	проц operator=(const Линк&);

};

template <class T, цел N = 1>
class ЛинкВлад : public Линк<T, N> {
public:
	~ЛинкВлад()                         { Линк<T, N>::удалиСписок(); }
};

template <class T, class K = Ткст>
class LRUCache {
public:
	struct Делец {
		virtual K      Ключ() const = 0;
		virtual цел    сделай(T& object) const = 0;
		virtual ~Делец() {}
	};

private:
	struct Элемент : Движ<Элемент> {
		цел    prev, next;
		цел    size;
		Один<T> данные;
		бул   flag;
	};
	
	struct Ключ : Движ<Ключ> {
		K            ключ;
		Ткст       тип;
		
		бул operator==(const Ключ& b) const { return ключ == b.ключ && тип == b.тип; }
		т_хэш дайХэшЗнач() const { return комбинируйХэш(ключ, тип); }
	};

	Индекс<Ключ>   ключ;
	Вектор<Элемент> данные;
	цел  head;

	цел  size;
	цел  count;

	цел  foundsize;
	цел  newsize;
	бул flag = false;
	
	const цел InternalSize = 3 * (sizeof(Элемент) + sizeof(Ключ) + 24) / 2;

	проц отлинкуй(цел i);
	проц линкуйГолову(цел i);

public:
	цел  дайРазм() const            { return size; }
	цел  дайСчёт() const           { return count; }

	template <class P> проц настройРазм(P getsize);

	T&       GetLRU();
	const K& GetLRUKey();
	проц     DropLRU();
	проц     сожми(цел maxsize, цел maxcount = 30000);
	проц     сожмиСчёт(цел maxcount = 30000)          { сожми(INT_MAX, maxcount); }

	template <class P> цел  удали(P predicate);
	template <class P> бул удалиОдин(P predicate);

	T&   дай(const Делец& m);

	проц очисть();

	проц очистьСчётчики();
	цел  дайНайденРазм() const       { return foundsize; }
	цел  дайНовРазм() const         { return newsize; }

	LRUCache() { head = -1; size = 0; count = 0; очистьСчётчики(); }
};

template <class T, class K>
проц LRUCache<T, K>::линкуйГолову(цел i)
{
	Элемент& m = данные[i];
	if(head >= 0) {
		цел tail = данные[head].prev;
		m.next = head;
		m.prev = tail;
		данные[head].prev = i;
		данные[tail].next = i;
	}
	else
		m.prev = m.next = i;
	head = i;
	count++;
}


template <class T, class K>
проц LRUCache<T, K>::отлинкуй(цел i)
{
	Элемент& m = данные[i];
	if(m.prev == i)
		head = -1;
	else {
		if(head == i)
			head = m.next;
		данные[m.next].prev = m.prev;
		данные[m.prev].next = m.next;
	}
	count--;
}

template <class T, class K>
T& LRUCache<T, K>::GetLRU()
{
	цел tail = данные[head].prev;
	return *данные[tail].данные;
}

template <class T, class K>
const K& LRUCache<T, K>::GetLRUKey()
{
	цел tail = данные[head].prev;
	return ключ[tail].ключ;
}

template <class T, class K>
проц LRUCache<T, K>::DropLRU()
{
	if(head >= 0) {
		цел tail = данные[head].prev;
		size -= данные[tail].size;
		данные[tail].данные.очисть();
		отлинкуй(tail);
		ключ.отлинкуй(tail);
	}
}

template <class T, class K>
template <class P>
проц LRUCache<T, K>::настройРазм(P getsize)
{
	size = 0;
	count = 0;
	for(цел i = 0; i < данные.дайСчёт(); i++)
		if(!ключ.отлинкован(i)) {
			цел sz = getsize(*данные[i].данные);
			if(sz >= 0)
				данные[i].size = sz + InternalSize;
			size += данные[i].size;
			count++;
		}
}

template <class T, class K>
template <class P>
цел LRUCache<T, K>::удали(P predicate)
{
	цел n = 0;
	цел i = 0;
	while(i < данные.дайСчёт())
		if(!ключ.отлинкован(i) && predicate(*данные[i].данные)) {
			size -= данные[i].size;
			отлинкуй(i);
			ключ.отлинкуй(i);
			n++;
		}
		else
			i++;
	return n;
}

template <class T, class K>
template <class P>
бул LRUCache<T, K>::удалиОдин(P predicate)
{
	цел i = head;
	if(i >= 0)
		for(;;) {
			цел next = данные[i].next;
			if(predicate(*данные[i].данные)) {
				size -= данные[i].size;
				отлинкуй(i);
				ключ.отлинкуй(i);
				return true;
			}
			if(i == next || next == head || next < 0)
				break;
			i = next;
		}
	return false;
}

template <class T, class K>
проц LRUCache<T, K>::сожми(цел maxsize, цел maxcount)
{
	if(maxsize >= 0 && maxcount >= 0)
		while(count > maxcount || size > maxsize)
			DropLRU();
}

template <class T, class K>
проц LRUCache<T, K>::очисть()
{
	head = -1;
	size = 0;
	count = 0;
	newsize = foundsize = 0;
	ключ.очисть();
	данные.очисть();
}

template <class T, class K>
проц LRUCache<T, K>::очистьСчётчики()
{
	flag = !flag;
	newsize = foundsize = 0;
}

template <class T, class K>
T& LRUCache<T, K>::дай(const Делец& m)
{
	Ключ k;
	k.ключ = m.Ключ();
	k.тип = typeid(m).name();
	цел q = ключ.найди(k);
	if(q < 0) {
		q = ключ.помести(k);
		Элемент& t = данные.по(q);
		t.size = m.сделай(t.данные.создай()) + InternalSize;
		size += t.size;
		newsize += t.size;
		t.flag = flag;
	}
	else {
		Элемент& t = данные[q];
		отлинкуй(q);
		if(t.flag != flag) {
			t.flag = flag;
			foundsize += t.size;
		}
	}
	линкуйГолову(q);
	return *данные[q].данные;
}
///////////////////////
КОН_ПИ_РНЦП///////////
//////////////////////
//Function.h
enum CNULLer { CNULL }; // Deprecated, use plain Null

template<typename Рез, typename... ТипыАрг>
class Функция<Рез(ТипыАрг...)> : РНЦП::Движ<Функция<Рез(ТипыАрг...)>> {
	struct ОбёрткаОснова {
		Атомар  refcount;

		virtual Рез выполни(ТипыАрг... арги) = 0;
		
		ОбёрткаОснова() { refcount = 1; }
		virtual ~ОбёрткаОснова() {}
	};

	template <class F>
	struct Обёртка : ОбёрткаОснова {
		F фн;
		virtual Рез выполни(ТипыАрг... арги) { return фн(арги...); }

		Обёртка(F&& фн) : фн(пикуй(фн)) {}
	};

	template <class F>
	struct Обёртка2 : ОбёрткаОснова {
		Функция l;
		F        фн;

		virtual Рез выполни(ТипыАрг... арги) { l(арги...); return фн(арги...); }

		Обёртка2(const Функция& l, F&& фн) : l(l), фн(пикуй(фн)) {}
		Обёртка2(const Функция& l, const F& фн) : l(l), фн(фн) {}
	};

	ОбёрткаОснова *укз;
	
	static проц освободи(ОбёрткаОснова *укз) {
		if(укз && атомнДек(укз->refcount) == 0)
			delete укз;
	}
	
	проц копируй(const Функция& a) {
		укз = a.укз;
		if(укз)
			атомнИнк(укз->refcount);
	}
	
	проц подбери(Функция&& ист) {
		укз = ист.укз;
		ист.укз = NULL;
	}

public:
	Функция()                                 { укз = NULL; }
	Функция(CNULLer)                          { укз = NULL; }
	Функция(const РНЦП::Обнул&)                    { укз = NULL; }
	
	template <class F> Функция(F фн)          { укз = new Обёртка<F>(пикуй(фн)); }
	
	Функция(const Функция& ист)              { копируй(ист); }
	Функция& operator=(const Функция& ист)   { auto b = укз; копируй(ист); освободи(b); return *this; }

	Функция(Функция&& ист)                   { подбери(пикуй(ист)); }
	Функция& operator=(Функция&& ист)        { if(&ист != this) { освободи(укз); укз = ист.укз; ист.укз = NULL; } return *this; }
	
	Функция прокси() const                     { return [=] (ТипыАрг... арги) { return (*this)(арги...); }; }

	template <class F>
	Функция& operator<<(F фн)                 { if(!укз) { подбери(пикуй(фн)); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<F>(*this, пикуй(фн)); освободи(b); return *this; }

	Функция& operator<<(const Функция& фн)   { if(!укз) { копируй(фн); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<Функция>(*this, фн); освободи(b); return *this; }

	Функция& operator<<(Функция&& фн)        { if(!укз) { подбери(пикуй(фн)); return *this; }
	                                             ОбёрткаОснова *b = укз; укз = new Обёртка2<Функция>(*this, пикуй(фн)); освободи(b); return *this; }

	Рез operator()(ТипыАрг... арги) const     { return укз ? укз->выполни(арги...) : Рез(); }
	
	operator бул() const                      { return укз; }
	проц очисть()                               { освободи(укз); укз = NULL; }

	~Функция()                                { освободи(укз); }

	friend Функция прокси(const Функция& a)   { return a.прокси(); }
	friend проц разверни(Функция& a, Функция& b) { РНЦП::разверни(a.укз, b.укз); }
};

template <typename... ТипыАрг>
using Событие = Функция<проц (ТипыАрг...)>;

template <typename... ТипыАрг>
using Врата = Функция<бул (ТипыАрг...)>;

template <class Ук, class Класс, class Рез, class... ТипыАрг>
Функция<Рез (ТипыАрг...)> памФн(Ук object, Рез (Класс::*method)(ТипыАрг...))
{
	return [=](ТипыАрг... арги) { return (object->*method)(арги...); };
}

#define THISFN(x)   памФн(this, &ИМЯ_КЛАССА::x)

template<typename Рез, typename... ТипыАрг>
class Функция<Рез(ТипыАрг...)>;

////////////////////////////
ПИ_РНЦП///////////////////
///////////////////////////
//Mt.h
#ifdef DEPRECATED
#define thread__ thread_local
#endif

#ifdef флагПРОФИЛЬМП
class Стопор;
class СтатическийСтопор;

struct МпИнспектор {
	кткст0 имя;
	цел   number;
	цел   locked;
	цел   blocked;
	
	static МпИнспектор *Dumi();

	МпИнспектор(кткст0 s, цел n = -1) { имя = s; number = n; locked = blocked = 0; }
	~МпИнспектор();
};

#define ПРОФИЛЬМП(mutex) \
	{ static МпИнспектор MK__s(__FILE__, __LINE__); mutex.уст(MK__s); }

#define ПРОФИЛЬМП_(mutex, ид) \
	{ static МпИнспектор MK__s(ид); mutex.уст(MK__s); }

#else

#define ПРОФИЛЬМП(mutex)
#define ПРОФИЛЬМП_(mutex, ид)

#endif

class Нить : БезКопий {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
	DWORD	   thread_id;
#endif
#ifdef PLATFORM_POSIX
	pthread_t  handle;
#endif
public:
	бул       пуск(Функция<проц ()> cb, бул noshutdown = false);
	бул       выполниХорошо(Функция<проц ()> cb, бул noshutdown = false);
	бул       выполниКрит(Функция<проц ()> cb, бул noshutdown = false);

	проц       открепи();
	цел        жди();

	бул       открыт() const     { return handle; }

#ifdef PLATFORM_WIN32
	typedef HANDLE Handle;
	typedef DWORD  Ид;

	Ид          дайИд() const                  { return thread_id; }
#endif
#ifdef PLATFORM_POSIX
	typedef pthread_t Handle;
	typedef pthread_t Ид;

	Ид          дайИд() const                  { return handle; }
#endif

	Handle      дайУк() const              { return handle; }
	
	бул        приоритет(цел percent); // 0 = lowest, 100 = normal
	
	проц        Nice()                         { приоритет(25); }
	проц        критически()                     { приоритет(150); }

	static проц старт(Функция<проц ()> cb, бул noshutdown = false);
	static проц стартХорошо(Функция<проц ()> cb, бул noshutdown = false);
	static проц стартКрит(Функция<проц ()> cb, бул noshutdown = false);

	static проц спи(цел ms);

	static бул ОН_ли();
	static бул главная_ли();
	static бул рнцп_ли();
	static цел  дайСчёт();
	static проц начниШатдаунНитей();
	static проц завершиШатдаунНитей();
	static проц шатдаунНитей();
	static бул шатдаунНитей_ли();
	static проц (*приВыходе(проц (*exitfn)()))();

	static проц выход();

#ifdef PLATFORM_WIN32
	static Handle дайУкТекущ()          { return GetCurrentThread(); }
	static inline Ид дайИдТекущ()           { return ::GetCurrentThreadId(); };
#endif
#ifdef PLATFORM_POSIX
	static Handle дайУкТекущ()          { return pthread_self(); }
	static inline Ид дайИдТекущ()           { return pthread_self(); };
#endif

	Нить();
	~Нить();

private:
	проц operator=(const Нить&);
	Нить(const Нить&);
};

#ifdef _ОТЛАДКА
inline проц проверьНаОН() { ПРОВЕРЬ(Нить::ОН_ли()); }
#else
inline проц проверьНаОН() {}
#endif


class Семафор : БезКопий {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
#elif PLATFORM_OSX
	dispatch_semaphore_t    sem;
#else
typedef int __atomic_lock_t;

    struct _pthread_fastlock
    {
        long            __status;
      __atomic_lock_t   __spinlock;
    };

    struct sem_t
    {
      _pthread_fastlock __sem_lock;
      int               __sem_value;
      void*             __sem_waiting;
    };
    
	sem_t      sem;
#endif

public:
	бул       жди(цел timeout_ms = -1);
	проц       отпусти();
#ifdef PLATFORM_WIN32
	проц       отпусти(цел n);
#endif

	Семафор();
	~Семафор();
};

struct МпИнспектор;

#ifdef PLATFORM_WIN32

class Стопор : БезКопий {
protected:
	CRITICAL_SECTION section;
	МпИнспектор        *mti;

	Стопор(цел)         {}

	friend class ПеременнаяУсловия;

public:
	бул  пробуйВойти();
	проц  выйди()                { LeaveCriticalSection(&section); }

#ifdef флагПРОФИЛЬМП
	проц  войди()                { if(!пробуйВойти()) { mti->blocked++; EnterCriticalSection(&section); }; mti->locked++; }
	проц  уст(МпИнспектор& m)    { mti = &m; }

	Стопор()                      { mti = МпИнспектор::Dumi(); InitializeCriticalSection(&section); }
#else
	проц  войди()                { EnterCriticalSection(&section); }

	Стопор()                      { InitializeCriticalSection(&section); }
#endif

	~Стопор()                     { DeleteCriticalSection(&section); }

	class Замок;
};

/* Win32 ЧЗСтопор implementation by Chris Thomasson, cristom@comcast.net */

class ЧЗСтопор : БезКопий {
    LONG   m_count, m_rdwake;
    HANDLE m_wrwset, m_rdwset;
    CRITICAL_SECTION m_wrlock;

public:
	проц войдиЗ();
	проц выйдиЗ();

	проц войдиЧ();
	проц выйдиЧ();

	ЧЗСтопор();
	~ЧЗСтопор();

	class ЧЗамок;
	class ЗЗамок;
};

class ПеременнаяУсловия {
	static VOID (WINAPI *InitializeConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
	static VOID (WINAPI *WakeConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
	static VOID (WINAPI *WakeAllConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
	static BOOL (WINAPI *SleepConditionVariableCS)(PCONDITION_VARIABLE ПеременнаяУсловия, PCRITICAL_SECTION КритСекция, DWORD dwMilliseconds);
	
	CONDITION_VARIABLE cv[1];

	struct ЖдущаяНить { // Windows XP does not provide ПеременнаяУсловия, implement using Semaphores
		Семафор      sem;
		ЖдущаяНить *next;
	};
	Стопор          mutex;
	ЖдущаяНить *head, *tail;
	
public:
	бул жди(Стопор& m, цел timeout_ms = -1);
	проц Signal();
	проц Broadcast();
	
	ПеременнаяУсловия();
	~ПеременнаяУсловия();
};

#endif

#ifdef PLATFORM_POSIX

class Стопор : БезКопий {
	pthread_mutex_t  mutex[1];
#ifdef флагПРОФИЛЬМП
	МпИнспектор     *mti;
#endif
	friend class ПеременнаяУсловия;

public:
#ifdef флагПРОФИЛЬМП
	бул  пробуйВойти()          { бул b = pthread_mutex_trylock(mutex) == 0; mti->locked += b; return b; }
	проц  войди()             { if(pthread_mutex_trylock(mutex) != 0) { mti->blocked++; pthread_mutex_lock(mutex); } mti->locked++; }
	проц  уст(МпИнспектор& m) { mti = &m; }
#else
	бул  пробуйВойти()          { return pthread_mutex_trylock(mutex) == 0; }
	проц  войди()             { pthread_mutex_lock(mutex); }
#endif
	проц  выйди()             { pthread_mutex_unlock(mutex); }

	class Замок;

	Стопор();
	~Стопор()           { pthread_mutex_destroy(mutex); }
};

class ЧЗСтопор : БезКопий {
	pthread_rwlock_t rwlock[1];

public:
	проц войдиЗ()  { pthread_rwlock_wrlock(rwlock); }
	проц выйдиЗ()  { pthread_rwlock_unlock(rwlock); }
	проц войдиЧ()   { pthread_rwlock_rdlock(rwlock); }
	проц выйдиЧ()   { pthread_rwlock_unlock(rwlock); }

	ЧЗСтопор();
	~ЧЗСтопор();

	class ЧЗамок;
	class ЗЗамок;
};

class ПеременнаяУсловия {
	pthread_cond_t cv[1];
	
public:
	бул жди(Стопор& m, цел timeout_ms = -1);

	проц Signal()        { pthread_cond_signal(cv); }
	проц Broadcast()     { pthread_cond_broadcast(cv); }
	
	ПеременнаяУсловия()  { pthread_cond_init(cv, NULL); }
	~ПеременнаяУсловия() { pthread_cond_destroy(cv); }
};

#endif

typedef std::atomic<бул> ФлагЕдиножды;

#define ONCELOCK_(o_b_) \
for(static ::ДинрусРНЦП::Стопор o_ss_; !o_b_.load(std::memory_order_acquire);) \
	for(::ДинрусРНЦП::Стопор::Замок o_ss_lock__(o_ss_); !o_b_.load(std::memory_order_acquire); o_b_.store(true, std::memory_order_release))

#define ONCELOCK \
for(static ::ДинрусРНЦП::ФлагЕдиножды o_b_; !o_b_.load(std::memory_order_acquire);) ONCELOCK_(o_b_)


class Стопор::Замок : БезКопий {
	Стопор& s;

public:
	Замок(Стопор& s) : s(s) { s.войди(); }
	~Замок()               { s.выйди(); }
};

class ЧЗСтопор::ЧЗамок : БезКопий {
	ЧЗСтопор& s;

public:
	ЧЗамок(ЧЗСтопор& s) : s(s) { s.войдиЧ(); }
	~ЧЗамок()                 { s.выйдиЧ(); }
};

class ЧЗСтопор::ЗЗамок : БезКопий {
	ЧЗСтопор& s;

public:
	ЗЗамок(ЧЗСтопор& s) : s(s) { s.войдиЗ(); }
	~ЗЗамок()                 { s.выйдиЗ(); }
};

template <class Примитив>
class СтатическийПримитив_ : БезКопий {
	Примитив *primitive;
	ббайт       буфер[sizeof(Примитив)];
	ФлагЕдиножды   once;
	
	проц инициализуй() { primitive = new(буфер) Примитив; }

public:
	Примитив& дай()  { ONCELOCK_(once) инициализуй(); return *primitive; }
};

class СтатическийСтопор : СтатическийПримитив_<Стопор> {
public:
	operator Стопор&()          { return дай(); }
	бул пробуйВойти()            { return дай().пробуйВойти();}
	проц войди()               { дай().войди();}
	проц выйди()               { дай().выйди(); }
#ifdef флагПРОФИЛЬМП
	проц уст(МпИнспектор& mti) { дай().уст(mti); }
#endif
};

class СтатическийСемафор : СтатическийПримитив_<Семафор> {
public:
	operator Семафор&()        { return дай(); }
	проц жди()                  { дай().жди(); }
	проц отпусти()               { дай().отпусти(); }
};

class СтатическийЧЗСтопор : СтатическийПримитив_<ЧЗСтопор> {
public:
	operator ЧЗСтопор&()  { return дай(); }
	проц войдиЧ()     { дай().войдиЧ();}
	проц выйдиЧ()     { дай().выйдиЧ(); }
	проц войдиЗ()    { дай().войдиЗ();}
	проц выйдиЗ()    { дай().выйдиЗ(); }
};

class СтатическаяПеременнаяУсловия : СтатическийПримитив_<ПеременнаяУсловия> {
public:
	operator ПеременнаяУсловия&() { return дай(); }
	проц жди(Стопор& m)  { дай().жди(m); }
	проц Signal()        { дай().Signal(); }
	проц Broadcast()     { дай().Broadcast(); }
};

class ФоновыйОбнов {
	mutable Стопор              mutex;
	mutable std::atomic<бул>  dirty;

public:
	проц инивалидируй();
	бул начниОбнов() const;
	проц завершиОбнов() const;

	ФоновыйОбнов();
};

inline бул главнаяНить_ли() { return Нить::главная_ли(); }

struct СпинЗамок : Движ<СпинЗамок> {
#ifdef COMPILER_MSC
	volatile LONG locked;

	бул пробуйВойти() { return InterlockedCompareExchange(&locked, 1, 0) == 0; }
	проц выйди()    { _ReadWriteBarrier(); locked = 0; }
#else
	volatile цел locked;
	
	бул пробуйВойти() { return  __sync_bool_compare_and_swap (&locked, 0, 1); }
	проц выйди()    { __sync_lock_release(&locked); }
#endif

	проц войди()    { while(!пробуйВойти()) жди(); }
	
	проц жди();
	
	class Замок;

	СпинЗамок()         { locked = 0; }
};

class СпинЗамок::Замок : БезКопий {
	СпинЗамок& s;

public:
	Замок(СпинЗамок& s) : s(s) { s.войди(); }
	~Замок()                  { s.выйди(); }
};

#define INTERLOCKED \
for(бул i_b_ = true; i_b_;) \
	for(static РНЦП::Стопор i_ss_; i_b_;) \
		for(РНЦП::Стопор::Замок i_ss_lock__(i_ss_); i_b_; i_b_ = false)

struct H_l_ : Стопор::Замок {
	бул b;
	H_l_(Стопор& cs) : Стопор::Замок(cs) { b = true; }
};

#define ВЗАИМОЗАМК_(cs) \
for(РНЦП::H_l_ i_ss_lock__(cs); i_ss_lock__.b; i_ss_lock__.b = false)

#ifdef DEPRECATED
typedef Стопор КритСекция;
typedef СтатическийСтопор СтатическаяКритСекция;
#endif

// Auxiliary multithreading - this is not using/cannot use U++ heap, so does not need cleanup.
// Used to resolve some host platform issues.

#ifdef PLATFORM_WIN32
#define auxthread_t DWORD
#define auxthread__ WINAPI
#else
#define auxthread_t ук 
#define auxthread__
#endif

бул стартВспомНити(auxthread_t (auxthread__ *фн)(ук укз), ук укз);
///////////////////////////
///CoWork.h
class СоРабота : БезКопий {
	struct МРабота : Движ<МРабота>, Линк<МРабота, 2> {
		Функция<проц ()> фн;
		СоРабота           *work = NULL;
		бул              looper = false;
	};

	enum { SCHEDULED_MAX = 2048 };

public:
	struct Пул {
		МРабота             *free;
		Линк<МРабота, 2>     jobs;
		МРабота              slot[SCHEDULED_MAX];
		цел               waiting_threads;
		Массив<Нить>     threads;
		бул              quit;

		Стопор             lock;
		ПеременнаяУсловия waitforjob;

		проц              освободи(МРабота& m);
		проц              работай(МРабота& m);
		проц              суньРаботу(Функция<проц ()>&& фн, СоРабота *work, бул looper = false);

		проц              иницНити(цел nthreads);
		проц              покиньНити();

		Пул();
		~Пул();

		static thread_local бул    финблок;

		бул работай();
		static проц пускНити(цел tno);
	};

	friend struct Пул;

	static Пул& дайПул();

	static thread_local цел индекс_трудяги;
	static thread_local СоРабота *текущ;

	ПеременнаяУсловия  waitforfinish;
	Линк<МРабота, 2>      jobs; // global stack and СоРабота stack as дво-linked lists
	цел                todo;
	бул               canceled;
	std::exception_ptr exc = nullptr; // workaround for sanitizer bug(?)
	Функция<проц ()>  looper_fn;
	цел                looper_count;

	проц делай0(Функция<проц ()>&& фн, бул looper);

	проц отмени0();
	проц финишируй0();

	Атомар             индекс;

// experimental pipe support
	Стопор stepmutex;
	Массив<БиВектор<Функция<проц ()>>> step;
	Вектор<бул> steprunning;

public:
	static бул пробуйПлан(Функция<проц ()>&& фн);
	static бул пробуйПлан(const Функция<проц ()>& фн)      { return пробуйПлан(клонируй(фн)); }
	static проц планируй(Функция<проц ()>&& фн);
	static проц планируй(const Функция<проц ()>& фн)         { return планируй(клонируй(фн)); }

	проц     делай(Функция<проц ()>&& фн)                       { делай0(пикуй(фн), false); }
	проц     делай(const Функция<проц ()>& фн)                  { делай(клонируй(фн)); }

	СоРабота&  operator&(const Функция<проц ()>& фн)           { делай(фн); return *this; }
	СоРабота&  operator&(Функция<проц ()>&& фн)                { делай(пикуй(фн)); return *this; }

	проц     цикл(Функция<проц ()>&& фн);
	проц     цикл(const Функция<проц ()>& фн)                { цикл(клонируй(фн)); }

	СоРабота&  operator*(const Функция<проц ()>& фн)           { цикл(фн); return *this; }
	СоРабота&  operator*(Функция<проц ()>&& фн)                { цикл(пикуй(фн)); return *this; }

	цел      следщ()                                           { return ++индекс - 1; }

	цел  дайСчётПланируемых() const                            { return todo; }
	проц пайп(цел stepi, Функция<проц ()>&& lambda); // experimental

	static проц финБлок();

	проц отмена();
	static бул отменён();

	проц финиш();

	бул финишировал();

	проц переустанов();

	static бул трудяга_ли()                                    { return дайИндексТрудяги() >= 0; }
	static цел  дайИндексТрудяги();
	static цел  дайРазмерПула();
	static проц устРазмерПула(цел n);

	СоРабота();
	~СоРабота() noexcept(false);
};

struct CoWorkNX : СоРабота {
	~CoWorkNX() noexcept(true) {}
};

inline
проц соДелай(Функция<проц ()>&& фн)
{
	СоРабота co;
	co * фн;
}

inline
проц соДелай_ОН(Функция<проц ()>&& фн)
{
	фн();
}

inline
проц соДелай(бул co, Функция<проц ()>&& фн)
{
	if(co)
		соДелай(пикуй(фн));
	else
		соДелай_ОН(пикуй(фн));
}

template <typename Фн>
проц соФор(цел n, Фн iterator)
{
	std::atomic<цел> ii(0);
	соДелай([&] {
		for(цел i = ii++; i < n; i = ii++)
			iterator(i);
	});
}

template <typename Фн>
проц соФор_ОН(цел n, Фн iterator)
{
	for(цел i = 0; i < n; i++)
		iterator(i);
}

template <typename Фн>
проц соФор(бул co, цел n, Фн iterator)
{
	if(co)
		соФор(n, iterator);
	else
		соФор_ОН(n, iterator);
}

template <class T>
class соРесурсыТрудяги {
	цел          workercount;
	Буфер<T>    res;

public:
	цел дайСчёт() const  { return workercount + 1; }
	T& operator[](цел i)  { return res[i]; }

	T& дай()              { цел i = СоРабота::дайИндексТрудяги(); return res[i < 0 ? workercount : i]; }
	T& operator~()        { return дай(); }

	T *begin()            { return ~res; }
	T *end()              { return ~res + дайСчёт(); }

	соРесурсыТрудяги()   { workercount = СоРабота::дайРазмерПула(); res.размести(дайСчёт()); }

	соРесурсыТрудяги(Событие<T&> initializer) : соРесурсыТрудяги() {
		for(цел i = 0; i < дайСчёт(); i++)
			initializer(res[i]);
	}
};

template <class Ret>
class АсинхРабота {
	template <class Ret2>
	struct Imp {
		СоРабота co;
		Ret2   ret;

		template<class Функция, class... Арги>
		проц        делай(Функция&& f, Арги&&... арги) { co.делай([=]() { ret = f(арги...); }); }
		const Ret2& дай()                            { return ret; }
		Ret2        подбери()                           { return пикуй(ret); }
	};

	struct ImpVoid {
		СоРабота co;

		template<class Функция, class... Арги>
		проц        делай(Функция&& f, Арги&&... арги) { co.делай([=]() { f(арги...); }); }
		проц        дай()                            {}
		проц        подбери()                           {}
	};

	using ImpType = typename std::conditional<std::is_void<Ret>::значение, ImpVoid, Imp<Ret>>::тип;

	Один<ImpType> imp;

public:
	template< class Функция, class... Арги>
	проц  делай(Функция&& f, Арги&&... арги)          { imp.создай().делай(f, арги...); }

	проц        отмена()                            { if(imp) imp->co.отмена(); }
	static бул отменён()                        { return СоРабота::отменён(); }
	бул        финишировал()                        { return imp && imp->co.финишировал(); }
	Ret         дай()                               { ПРОВЕРЬ(imp); imp->co.финиш(); return imp->дай(); }
	Ret         operator~()                         { return дай(); }
	Ret         подбери()                              { ПРОВЕРЬ(imp); imp->co.финиш(); return imp->подбери(); }

	АсинхРабота& operator=(АсинхРабота&&) = default;
	АсинхРабота(АсинхРабота&&) = default;

	АсинхРабота()                                     {}
	~АсинхРабота()                                    { if(imp) imp->co.отмена(); }
};

template< class Функция, class... Арги>
АсинхРабота<std::invoke_result_t<Функция, Арги...>>
Async(Функция&& f, Арги&&... арги)
{
	АсинхРабота<std::invoke_result_t<Функция, Арги...>> h;
	h.делай(f, арги...);
	return пикуй(h);
}

/////////////////////
//CoAlgo.h
enum {
	КО_ПАРТИЦ_МИН = 128,
	КО_ПАРТИЦ_МАКС = 1024*1024,
};

template <class C, class MC>
inline т_мера соЧанк__(C count, MC min_chunk = КО_ПАРТИЦ_МИН, MC max_chunk = КО_ПАРТИЦ_МАКС)
{
	цел n = min(count / цпбЯдра(), (C)max_chunk);
	return n < (C)min_chunk ? 0 : n;
}

template <class Обх, class Лямбда>
проц соОтсек(Обх begin, Обх end, const Лямбда& lambda, цел min_chunk = КО_ПАРТИЦ_МИН, цел max_chunk = КО_ПАРТИЦ_МАКС)
{
	т_мера chunk = соЧанк__(end - begin, min_chunk, max_chunk);
	if(chunk) {
		СоРабота co;
		while(begin < end) {
			Обх e = Обх(begin + мин(chunk, т_мера(end - begin)));
			co & [=] {
				lambda(begin, e);
			};
			begin = e;
		}
	}
	else
		lambda(begin, end);
}

template <class Диапазон, class Лямбда>
проц соОтсек(Диапазон&& r, const Лямбда& lambda)
{
	т_мера chunk = соЧанк__(r.дайСчёт(), КО_ПАРТИЦ_МИН);
	if(chunk) {
		СоРабота co;
		auto begin = r.begin();
		auto end = r.end();
		while(begin < end) {
			auto e = begin + мин(chunk, т_мера(end - begin));
			co & [=] {
				auto sr = СубДиапазон(begin, e); // we need l-значение
				lambda(sr);
			};
			begin = e;
		}
	}
	else
		lambda(СубДиапазон(r.begin(), r.end()));
}

template <class Диапазон, class Накопитель>
проц соНакопи(const Диапазон& r, Накопитель& result)
{
	typedef КонстОбходчикУ<Диапазон> I;
	соОтсек(r.begin(), r.end(),
		[=, &result](I i, I e) {
			Накопитель h;
			while(i < e)
				h(*i++);
			СоРабота::финБлок();
			result(h);
		}
	);
}

template <class Диапазон>
типЗначУ<Диапазон> соСумма(const Диапазон& r, const типЗначУ<Диапазон>& zero)
{
	typedef типЗначУ<Диапазон> VT;
	typedef КонстОбходчикУ<Диапазон> I;
	VT sum = zero;
	соОтсек(r.begin(), r.end(),
		[=, &sum](I i, I e) {
			VT h = zero;
			while(i < e)
				h += *i++;
			СоРабота::финБлок();
			sum += h;
		}
	);
	return sum;
}

template <class T>
типЗначУ<T> соСумма(const T& c)
{
	return соСумма(c, (типЗначУ<T>)0);
}

template <class Диапазон, class V>
цел соСчёт(const Диапазон& r, const V& знач)
{
	цел count = 0;
	соОтсек(r, [=, &знач, &count](const СубДиапазонУ<const Диапазон>& r) {
		цел n = счёт(r, знач);
		СоРабота::финБлок();
		count += n;
	});
	return count;
}

template <class Диапазон, class Предикат>
цел соСчётЕсли(const Диапазон& r, const Предикат& p)
{
	цел count = 0;
	соОтсек(r, [=, &p, &count](const СубДиапазонУ<const Диапазон>& r) {
		цел n = счётЕсли(r, p);
		СоРабота::финБлок();
		count += n;
	});
	return count;
}

template <class Диапазон, class Better>
цел соНайдиЛучш(const Диапазон& r, const Better& better)
{
	if(r.дайСчёт() <= 0)
		return -1;
	typedef КонстОбходчикУ<Диапазон> I;
	I best = r.begin();
	соОтсек(r.begin() + 1, r.end(),
		[=, &best](I i, I e) {
			I b = i++;
			while(i < e) {
				if(better(*i, *b))
					b = i;
				i++;
			}
			СоРабота::финБлок();
			if(better(*b, *best) || !better(*best, *b) && b < best)
				best = b;
		}
	);
	return best - r.begin();
}

template <class Диапазон>
цел соНайдиМин(const Диапазон& r)
{
	return соНайдиЛучш(r, std::less<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМин(const Диапазон& r)
{
	return r[соНайдиМин(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМин(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	цел q = соНайдиМин(r);
	return q < 0 ? опр : r[q];
}


template <class Диапазон>
цел соНайдиМакс(const Диапазон& r)
{
	return соНайдиЛучш(r, std::greater<типЗначУ<Диапазон>>());
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМакс(const Диапазон& r)
{
	return r[соНайдиМакс(r)];
}

template <class Диапазон>
const типЗначУ<Диапазон>& соМакс(const Диапазон& r, const типЗначУ<Диапазон>& опр)
{
	цел q = соНайдиМакс(r);
	return q < 0 ? опр : r[q];
}

template <class Диапазон, class Свер>
цел соНайдиСовпад(const Диапазон& r, const Свер& eq, цел from = 0)
{
	цел count = r.дайСчёт();
	std::atomic<цел> found;
	found = count;
	соОтсек(from, count,
		[=, &r, &found](цел i, цел e) {
			for(; i < e; i++) {
				if(found < i)
					break;
				if(eq(r[i])) {
					СоРабота::финБлок();
					if(i < found)
						found = i;
					return;
				}
			}
		}
	);
	return found < (цел)count ? (цел)found : -1;
}

template <class Диапазон, class V>
цел соНайдиИндекс(const Диапазон& r, const V& значение, цел from = 0)
{
	return соНайдиСовпад(r, [=, &значение](const типЗначУ<Диапазон>& m) { return m == значение; }, from);
}

template <class Диапазон1, class Диапазон2>
цел соДиапазоныРавны(const Диапазон1& r1, const Диапазон2& r2)
{
	if(r1.дайСчёт() != r2.дайСчёт())
		return false;
	std::atomic<бул> equal(true);
	соОтсек(0, r1.дайСчёт(),
		[=, &equal, &r1, &r2](цел i, цел e) {
			while(i < e && equal) {
				if(r1[i] != r2[i]) {
					СоРабота::финБлок();
					equal = false;
				}
				i++;
			}
		}
	);
	return equal;
}

template <class Диапазон, class Предикат>
Вектор<цел> соНайдиВсе(const Диапазон& r, Предикат match, цел from = 0)
{
	Вектор<Вектор<цел>> rs;
	цел total = 0;
	соОтсек(from, r.дайСчёт(), [=, &r, &rs, &total](цел begin, цел end) {
		Вектор<цел> v = найдиВсе(СубДиапазон(r, 0, end), match, begin);
		СоРабота::финБлок();
		if(v.дайСчёт()) {
			total += v.дайСчёт();
			rs.добавь(пикуй(v));
		}
	});
	соСортируй(rs, [](const Вектор<цел>& a, const Вектор<цел>& b) { return a[0] < b[0]; });
	Вектор<цел> result;
	for(const auto& s : rs)
		result.приставь(s);
	return result;
}
/////////////////////
///Map.h
template <class K, class V>
struct КлючЗначРеф {
	const K& ключ;
	V&       значение;
	
	КлючЗначРеф(const K& ключ, V& значение) : ключ(ключ), значение(значение) {}
};

template <class вКарту, class K, class V>
struct ДиапазонКЗМап {
	вКарту& map;
	
	struct Обходчик {
		вКарту& map;
		цел  ii;
		
		проц SkipUnlinked()               { while(ii < map.дайСчёт() && map.отлинкован(ii)) ii++; }
		
		проц operator++()                 { ++ii; SkipUnlinked(); }
		КлючЗначРеф<K, V> operator*()     { return КлючЗначРеф<K, V>(map.дайКлюч(ii), map[ii]); }
		бул operator!=(Обходчик b) const { return ii != b.ii; }
		
		Обходчик(вКарту& map, цел ii) : map(map), ii(ii) { SkipUnlinked(); }
	};

	Обходчик begin() const             { return Обходчик(map, 0); }
	Обходчик end() const               { return Обходчик(map, map.дайСчёт()); }
	
	ДиапазонКЗМап(вКарту& map) : map(map) {}
};

template <class K, class T, class V>
class АМап {
protected:
	Индекс<K> ключ;
	V        значение;
	
	template <class KK>           цел FindAdd_(KK&& k);
	template <class KK, class TT> цел FindAdd_(KK&& k, TT&& init);
	template <class KK>           T&  Put_(KK&& k);
	template <class KK, class TT> цел Put_(KK&& k, TT&& x);
	template <class KK>           цел PutDefault_(KK&& k);
	template <class KK>           цел FindPut_(KK&& k);
	template <class KK, class TT> цел FindPut_(KK&& k, TT&& init);
	template <class KK>           T&  GetAdd_(KK&& k);
	template <class KK, class TT> T&  GetAdd_(KK&& k, TT&& init);
	template <class KK>           T&  GetPut_(KK&& k);
	template <class KK, class TT> T&  GetPut_(KK&& k, TT&& init);

public:
	T&       добавь(const K& k, const T& x)            { ключ.добавь(k); return значение.добавь(x); }
	T&       добавь(const K& k, T&& x)                 { ключ.добавь(k); return значение.добавь(пикуй(x)); }
	T&       добавь(const K& k)                        { ключ.добавь(k); return значение.добавь(); }
	T&       добавь(K&& k, const T& x)                 { ключ.добавь(пикуй(k)); return значение.добавь(x); }
	T&       добавь(K&& k, T&& x)                      { ключ.добавь(пикуй(k)); return значение.добавь(пикуй(x)); }
	T&       добавь(K&& k)                             { ключ.добавь(пикуй(k)); return значение.добавь(); }

	цел      найди(const K& k) const                 { return ключ.найди(k); }
	цел      найдиСледщ(цел i) const                  { return ключ.найдиСледщ(i); }
	цел      найдиПоследн(const K& k) const             { return ключ.найдиПоследн(k); }
	цел      найдиПредш(цел i) const                  { return ключ.найдиПредш(i); }

	цел      найдиДобавь(const K& k)                    { return FindAdd_(k); }
	цел      найдиДобавь(const K& k, const T& init)     { return FindAdd_(k, init); }
	цел      найдиДобавь(const K& k, T&& init)          { return FindAdd_(k, пикуй(init)); }
	цел      найдиДобавь(K&& k)                         { return FindAdd_(пикуй(k)); }
	цел      найдиДобавь(K&& k, const T& init)          { return FindAdd_(пикуй(k), init); }
	цел      найдиДобавь(K&& k, T&& init)               { return FindAdd_(пикуй(k), пикуй(init)); }

	T&       помести(const K& k)                        { return Put_(k); }
	цел      помести(const K& k, const T& x)            { return Put_(k, x); }
	цел      помести(const K& k, T&& x)                 { return Put_(k, пикуй(x)); }
	T&       помести(K&& k)                             { return Put_(пикуй(k)); }
	цел      помести(K&& k, const T& x)                 { return Put_(пикуй(k), x); }
	цел      помести(K&& k, T&& x)                      { return Put_(пикуй(k), пикуй(x)); }

	цел      поместиДефолт(const K& k)                 { return PutDefault_(k); }
	цел      поместиДефолт(K&& k)                      { return PutDefault_(пикуй(k)); }

	цел      найдиПомести(const K& k)                    { return FindPut_(k); }
	цел      найдиПомести(const K& k, const T& init)     { return FindPut_(k, init); }
	цел      найдиПомести(const K& k, T&& init)          { return FindPut_(k, пикуй(init)); }
	цел      найдиПомести(K&& k)                         { return FindPut_(пикуй(k)); }
	цел      найдиПомести(K&& k, const T& init)          { return FindPut_(пикуй(k), init); }
	цел      найдиПомести(K&& k, T&& init)               { return FindPut_(пикуй(k), пикуй(init)); }

	T&       дай(const K& k)                        { return значение[найди(k)]; }
	const T& дай(const K& k) const                  { return значение[найди(k)]; }
	const T& дай(const K& k, const T& d) const      { цел i = найди(k); return i >= 0 ? значение[i] : d; }

	T&       дайДобавь(const K& k)                     { return GetAdd_(k); }
	T&       дайДобавь(const K& k, const T& x)         { return GetAdd_(k, x); }
	T&       дайДобавь(const K& k, T&& x)              { return GetAdd_(k, пикуй(x)); }
	T&       дайДобавь(K&& k)                          { return GetAdd_(пикуй(k)); }
	T&       дайДобавь(K&& k, const T& x)              { return GetAdd_(пикуй(k), x); }
	T&       дайДобавь(K&& k, T&& x)                   { return GetAdd_(пикуй(k), пикуй(x)); }

	T&       дайПомести(const K& k)                     { return GetPut_(k); }
	T&       дайПомести(const K& k, const T& x)         { return GetPut_(k, x); }
	T&       дайПомести(const K& k, T&& x)              { return GetPut_(k, пикуй(x)); }
	T&       дайПомести(K&& k)                          { return GetPut_(пикуй(k)); }
	T&       дайПомести(K&& k, const T& x)              { return GetPut_(пикуй(k), x); }
	T&       дайПомести(K&& k, T&& x)                   { return GetPut_(пикуй(k), пикуй(x)); }

	проц     устКлюч(цел i, const K& k)              { ключ.уст(i, k); }
	проц     устКлюч(цел i, K&& k)                   { ключ.уст(i, пикуй(k)); }

	T       *найдиУк(const K& k)                    { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиУк(const K& k) const              { цел i = найди(k); return i >= 0 ? &значение[i] : NULL; }

	T       *найдиПоследнУк(const K& k)                { цел i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }
	const T *найдиПоследнУк(const K& k) const          { цел i = найдиПоследн(k); return i >= 0 ? &значение[i] : NULL; }

	проц     отлинкуй(цел i)                          { ключ.отлинкуй(i); }
	цел      отлинкуйКлюч(const K& k, unsigned h)      { return ключ.отлинкуйКлюч(k, h); }
	цел      отлинкуйКлюч(const K& k)                  { return ключ.отлинкуйКлюч(k); }
	бул     отлинкован(цел i) const                { return ключ.отлинкован(i); }
	проц     смети();
	бул     HasUnlinked() const                    { return ключ.HasUnlinked(); }

	const T& operator[](цел i) const                { return значение[i]; }
	T&       operator[](цел i)                      { return значение[i]; }
	цел      дайСчёт() const                       { return значение.дайСчёт(); }
	бул     пустой() const                        { return значение.пустой(); }
	проц     очисть()                                { ключ.очисть(); значение.очисть(); }
	проц     сожми()                               { значение.сожми(); ключ.сожми(); }
	проц     резервируй(цел xtra)                      { значение.резервируй(xtra); ключ.резервируй(xtra); }
	цел      дайРазмест() const                       { return значение.дайРазмест(); }

	unsigned дайХэш(цел i) const                   { return ключ.дайХэш(i); }

	проц     сбрось(цел n = 1)                        { ключ.сбрось(n); значение.сбрось(n); }
	T&       верх()                                  { return значение.верх(); }
	const T& верх() const                            { return значение.верх(); }
	const K& TopKey() const                         { return ключ.верх(); }
//	T        вынь()                                  { T h = верх(); сбрось(); return h; }
	K        PopKey()                               { K h = TopKey(); сбрось(); return h; }
	проц     обрежь(цел n)                            { ключ.обрежь(n); значение.обрежь(n); }

	const K& дайКлюч(цел i) const                    { return ключ[i]; }

	проц     удали(const цел *sl, цел n)           { ключ.удали(sl, n); значение.удали(sl, n); }
	проц     удали(const Вектор<цел>& sl)          { удали(sl, sl.дайСчёт()); }
	template <typename P> проц удалиЕсли(P p)        { удали(найдиВсеи(p)); }

	проц     сериализуй(Поток& s);
	проц     вРяр(РярВВ& xio);
	проц     вДжейсон(ДжейсонВВ& jio);
	Ткст   вТкст() const;
	т_хэш   дайХэшЗнач() const                   { return хэшПоСериализу(*this); }
	бул     operator==(const АМап& b) const        { ПРОВЕРЬ(!HasUnlinked()); return равныМапы(*this, b); }
	бул     operator!=(const АМап& b) const        { return !operator==(b); }
	цел      сравни(const АМап& b) const           { ПРОВЕРЬ(!HasUnlinked()); return сравниМап(*this, b); }
	бул     operator<=(const АМап& x) const        { return сравни(x) <= 0; }
	бул     operator>=(const АМап& x) const        { return сравни(x) >= 0; }
	бул     operator<(const АМап& x) const         { return сравни(x) < 0; }
	бул     operator>(const АМап& x) const         { return сравни(x) > 0; }

	проц     разверни(АМап& x)                          { РНЦП::разверни(значение, x.значение); РНЦП::разверни(ключ, x.ключ); }
	const Индекс<K>&  дайИндекс() const               { return ключ; }
	Индекс<K>         PickIndex()                    { return пикуй(ключ); }

	const Вектор<K>& дайКлючи() const                { return ключ.дайКлючи(); }
	Вектор<K>        подбериКлючи()                     { return ключ.подбериКлючи(); }

	const V&         дайЗначения() const              { return значение; }
	V&               дайЗначения()                    { return значение; }
	V                подбериЗначения()                   { return пикуй(значение); }
	
	ДиапазонКЗМап<АМап, K, T> operator~()                   { return ДиапазонКЗМап<АМап, K, T>(*this); }
	ДиапазонКЗМап<const АМап, K, const T> operator~() const { return ДиапазонКЗМап<const АМап, K, const T>(*this); }
	
	АМап& operator()(const K& k, const T& v)        { добавь(k, v); return *this; }

	АМап()                                          {}
	АМап(const АМап& s, цел) : ключ(s.ключ, 0), значение(s.значение, 0) {}
	АМап(Индекс<K>&& ndx, V&& знач) : ключ(пикуй(ndx)), значение(пикуй(знач)) {}
	АМап(Вектор<K>&& ndx, V&& знач) : ключ(пикуй(ndx)), значение(пикуй(знач)) {}
	АМап(std::initializer_list<std::pair<K, T>> init) { for(const auto& i : init) добавь(клонируй(i.first), клонируй(i.second)); }

	typedef ОбходчикУ<V>           Обходчик;
	typedef КонстОбходчикУ<V>      КонстОбходчик;

	Обходчик         begin()                        { return значение.begin(); }
	Обходчик         end()                          { return значение.end(); }
	КонстОбходчик    begin() const                  { return значение.begin(); }
	КонстОбходчик    end() const                    { return значение.end(); }

#ifdef DEPRECATED
	typedef V                          КонтейнерЗначений;
	typedef T                          ТипЗнач;

	typedef Вектор<K> КонтейнерКлючей;
	typedef K         ТипКлюча;
	typedef КонстОбходчикУ<Индекс<K>> КонстОбходчикКлючей;

	friend цел     дайСчёт(const АМап& v)                        { return v.дайСчёт(); }
	цел      PutPick(const K& k, T&& x)                           { return помести(k, пикуй(x)); }
	T&       добавьПодбор(const K& k, T&& x)                           { return добавь(k, пикуй(x)); }
	цел      FindAddPick(const K& k, T&& init)                    { return найдиДобавь(k, пикуй(init)); }
	цел      FindPutPick(const K& k, T&& init)                    { return найдиПомести(k, пикуй(init)); }
	T&       GetAddPick(const K& k, T&& x)                        { return дайДобавь(k, пикуй(x)); }
	T&       GetPutPick(const K& k, T&& x)                        { return дайПомести(k, пикуй(x)); }

	КонстОбходчикКлючей дайОбхКлючей(цел pos) const                    { return ключ.дайОбх(pos); }
	Обходчик         дайОбх(цел pos)                             { return значение.дайОбх(pos); }
	КонстОбходчик    дайОбх(цел pos) const                       { return значение.дайОбх(pos); }

	КонстОбходчикКлючей стартКлючи() const                             { return ключ.begin(); }
	КонстОбходчикКлючей стопКлючи() const                               { return ключ.end(); }

	T&       вставь(цел i, const K& k)             { ключ.вставь(i, k); return значение.вставь(i); }
	T&       вставь(цел i, const K& k, const T& x) { ключ.вставь(i, k); return значение.вставь(i, x); }
	T&       вставь(цел i, const K& k, T&& x)      { ключ.вставь(i, k); return значение.вставь(i, пикуй(x)); }
	T&       вставь(цел i, K&& k)                  { ключ.вставь(i, пикуй(k)); return значение.вставь(i); }
	T&       вставь(цел i, K&& k, const T& x)      { ключ.вставь(i, пикуй(k)); return значение.вставь(i, x); }
	T&       вставь(цел i, K&& k, T&& x)           { ключ.вставь(i, пикуй(k)); return значение.вставь(i, пикуй(x)); }

	проц     удали(цел i)                         { ключ.удали(i); значение.удали(i); }
	проц     удали(цел i, цел count)              { ключ.удали(i, count); значение.удали(i, count); }
	цел      удалиКлюч(const K& k);
#endif
};

template <class K, class T>
class ВекторМап : public ОпцияДвижимогоИГлубКопии<ВекторМап<K, T>>,
                  public АМап<K, T, Вектор<T>> {
    typedef АМап<K, T, Вектор<T>> B;
public:
	T        вынь()                            { T h = пикуй(B::верх()); B::сбрось(); return h; }

	ВекторМап(const ВекторМап& s, цел) : АМап<K, T, Вектор<T>>(s, 1) {}
	ВекторМап(Индекс<K>&& ndx, Вектор<T>&& знач) : АМап<K, T, Вектор<T>>(пикуй(ndx), пикуй(знач)) {}
	ВекторМап(Вектор<K>&& ndx, Вектор<T>&& знач) : АМап<K, T, Вектор<T>>(пикуй(ndx), пикуй(знач)) {}
	ВекторМап()                                                       {}
	ВекторМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	ВекторМап& operator()(KK&& k, TT&& v)                { B::добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend проц    разверни(ВекторМап& a, ВекторМап& b)      { a.B::разверни(b); }

	typedef typename АМап< K, T, Вектор<T>>::КонстОбходчик КонстОбходчик;
	typedef typename АМап< K, T, Вектор<T>>::Обходчик      Обходчик;
	STL_MAP_COMPATIBILITY(ВекторМап<K _cm_ T _cm_ HashFn>)
};

template <class K, class T>
class МассивМап : public ОпцияДвижимогоИГлубКопии<МассивМап<K, T>>,
                 public АМап<K, T, Массив<T>> {
	typedef АМап<K, T, Массив<T>> B;
public:
	T&        добавь(const K& k, const T& x)          { return B::добавь(k, x); }
	T&        добавь(const K& k, T&& x)               { return B::добавь(k, пикуй(x)); }
	T&        добавь(const K& k)                      { return B::добавь(k); }
	T&        добавь(const K& k, T *newt)             { B::ключ.добавь(k); return B::значение.добавь(newt); }
	T&        добавь(K&& k, const T& x)               { return B::добавь(пикуй(k), x); }
	T&        добавь(K&& k, T&& x)                    { return B::добавь(пикуй(k), пикуй(x)); }
	T&        добавь(K&& k)                           { return B::добавь(пикуй(k)); }
	T&        добавь(K&& k, T *newt)                  { B::ключ.добавь(пикуй(k)); return B::значение.добавь(newt); }

	T&        вставь(цел i, const K& k, T *newt)   { B::ключ.вставь(i, k); return B::значение.вставь(i, newt); }
	T&        вставь(цел i, K&& k, T *newt)        { B::ключ.вставь(i, пикуй(k)); return B::значение.вставь(i, newt); }
	using B::вставь;

	template <class TT, class... Арги>
	TT& создай(const K& k, Арги&&... арги)         { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(k); return static_cast<TT&>(B::значение.добавь(q)); }
	template <class TT, class... Арги>
	TT& создай(K&& k, Арги&&... арги)              { TT *q = new TT(std::forward<Арги>(арги)...); B::ключ.добавь(пикуй(k)); return static_cast<TT&>(B::значение.добавь(q)); }

	T&        уст(цел i, T *укз)                   { return B::значение.уст(i, укз); }
	T        *выньОткрепи()                          { B::ключ.сбрось(); return B::значение.выньОткрепи(); }
	T        *открепи(цел i)                        { B::ключ.удали(i); return B::значение.открепи(i); }
	T        *разверни(цел i, T *newt)                 { return B::значение.разверни(i, newt); }

	МассивМап(const МассивМап& s, цел) : АМап<K, T, Массив<T>>(s, 1) {}
	МассивМап(Индекс<K>&& ndx, Массив<T>&& знач) : АМап<K, T, Массив<T>>(пикуй(ndx), пикуй(знач)) {}
	МассивМап(Вектор<K>&& ndx, Массив<T>&& знач) : АМап<K, T, Массив<T>>(пикуй(ndx), пикуй(знач)) {}
	МассивМап() {}

	МассивМап(std::initializer_list<std::pair<K, T>> init) : B::АМап(init) {}

	template <class KK, class TT>
	МассивМап& operator()(KK&& k, TT&& v)           { добавь(std::forward<KK>(k), std::forward<TT>(v)); return *this; }

	friend проц    разверни(МассивМап& a, МассивМап& b)        { a.B::разверни(b); }

	typedef typename АМап< K, T, Массив<T>>::КонстОбходчик КонстОбходчик;
	typedef typename АМап< K, T, Массив<T>>::Обходчик      Обходчик;
	STL_MAP_COMPATIBILITY(МассивМап<K _cm_ T _cm_ HashFn>)
};

////////////////////////////////
//App.h

Ткст  дайСреду(кткст0 ид);

Ткст  дайФПутьИсп();
Ткст  дайТитулИсп();
Ткст  дайПапкуИсп();
Ткст  дайФайлИзПапкиИсп(кткст0 fp);

#ifdef PLATFORM_COCOA
Ткст дайПапкуПрил();
#endif

Ткст дайИмяПрил();
проц устИмяАпп(const Ткст& имя);

Ткст  дайФайлИзДомПапки(кткст0 fp);
Ткст  дайДомПапку();

проц    устДомПапку(кткст0 dir);

проц    используйКонфигДомПапки(бул b = true);

Ткст  дайВремПапку();
Ткст  времФайл(кткст0 имяф);

Ткст  дайКонфигПапку();
Ткст  конфигФайл(кткст0 file);
Ткст  конфигФайл();

проц    устКонфигГруппу(кткст0 группа);
Ткст  дайКонфигГруппу();


const Вектор<Ткст>& комСтрока();
Ткст дайАргз0();
const ВекторМап<Ткст, Ткст>& систСреда();

проц    устКодВыхода(цел код_);
цел     дайКодВыхода();

бул    главнаяПущена();

struct ИсклВыхода {};

проц выход(цел код_ = 1);

проц выходПрил__();
проц выполниПрил__(проц (*app)());

#ifdef PLATFORM_WIN32

проц иницПрил__(цел argc, const char **argv);
проц иницПрилСреду__();

#define ГЛАВНАЯ_КОНСОЛЬН_ПРИЛ \
проц главФнКонсоли_(); \
 \
цел main(цел argc, char *argv[]) { \
	РНЦП::иницПрил__(argc, (const char **)argv); \
	РНЦП::выполниПрил__(главФнКонсоли_); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
 \
проц главФнКонсоли_()

#endif

#ifdef PLATFORM_POSIX

проц иницПрил__(цел argc, const char **argv, const char **envptr);

#define ГЛАВНАЯ_КОНСОЛЬН_ПРИЛ \
проц главФнКонсоли_(); \
 \
цел main(цел argc, const char **argv, const char **envptr) { \
	РНЦП::иницПрил__(argc, argv, envptr); \
	РНЦП::выполниПрил__(главФнКонсоли_); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
 \
проц главФнКонсоли_()

#endif

Ткст  дайФайлДан(кткст0 имяф);
Ткст  загрузиФайлДан(кткст0 имяф);
проц    устПутьДан(кткст0 path);

проц    запустиВебБраузер(const Ткст& url);

Ткст дайИмяКомпа();
Ткст дайИмяПользователя();
Ткст дайМенеджерРабСтола();

Ткст дайПапкуРабСтола();
Ткст дайПапкуПрограм();
#ifdef PLATFORM_WIN32
Ткст дайПапкуПрограмХ86();
#endif
Ткст дайПапкуДанныхПрил();
Ткст дайПапкуМузыки();
Ткст дайПапкуФото();
Ткст дайПапкуВидео();
Ткст дайПапкуДокументов();
Ткст дайПапкуШаблонов();
Ткст дайПапкуЗагрузок();

//////////////
КОН_ПИ_РНЦП

#include <DinrusPro/Core/TimeDate.h>
#include <DinrusPro/Core/Diag.h>
#include <DinrusPro/Core/Index.h>
#include <DinrusPro/Core/Sorted.h>
#include <DinrusPro/Core/Sort.h>
#include <DinrusPro/Core/Obsolete.h>
#include <DinrusPro/Core/FixedMap.h>
#include <DinrusPro/Core/InVector.h>
#include <DinrusPro/Core/CharSet.h>
#include <DinrusPro/Core/Path.h>
#include <DinrusPro/Core/SplitMerge.h>
#include <DinrusPro/Core/Lang.h>
#include <DinrusPro/Core/Value.h>
#include <DinrusPro/Core/ValueUtil.h>
#include <DinrusPro/Core/Tuple.h>
#include <DinrusPro/Core/Uuid.h>
#include <DinrusPro/Core/Ptr.h>
#include <DinrusPro/Core/Callback.h>
#include <DinrusPro/Core/Color.h>
#include <DinrusPro/Core/Complex.h>
#include <DinrusPro/Core/Hash.h>
#include <DinrusPro/Core/Util.h>
#include <DinrusPro/Core/Profile.h>
#include <DinrusPro/Core/FilterStream.h>
#include <DinrusPro/Core/Format.h>
#include <DinrusPro/Core/Convert.h>
#include <DinrusPro/Core/z.h>

#include <DinrusPro/Core/Parser.h>
#include <DinrusPro/Core/JSON.h>
#include <DinrusPro/Core/XML.h>
#include <DinrusPro/Core/Xmlize.h>

#include <DinrusPro/Core/Gtypes.h>
#include <DinrusPro/Core/i18n.h>
#include <DinrusPro/Core/Topic.h>


#include <DinrusPro/Core/LocalProcess.h>

#include <DinrusPro/Core/Inet.h>

#include <DinrusPro/Core/Win32Util.h>

#include <DinrusPro/Core/Vcont.hpp>
#include <DinrusPro/Core/Index.hpp>
#include <DinrusPro/Core/Map.hpp>
#include <DinrusPro/Core/InVector.hpp>
#include <DinrusPro/Core/InMap.hpp>

#include <DinrusPro/Core/Huge.h>

#include <DinrusPro/Core/ValueCache.h>
#endif
