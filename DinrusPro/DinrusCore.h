#ifndef _DinrusPro_DinrusCore_h_
#define _DinrusPro_DinrusCore_h_

#include <DinrusPro/DinrusPro.h>

/////////////////////////////
ПИ_РНЦП//Пространство имён//
///////////////////////////


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
class RelOps : public B
{
public:
	friend бул operator >  (const T& a, const T& b)  { return b < a; }
	friend бул operator != (const T& a, const T& b)  { return !(a == b); }
	friend бул operator <= (const T& a, const T& b)  { return !(b < a); }
	friend бул operator >= (const T& a, const T& b)  { return !(a < b); }
};

template <class U, class V, class B = ПустойКласс>
class AddOps : public B
{
public:
	friend U& operator -= (U& a, const V& b)          { a += -b; return a; }
	friend U  operator +  (const U& a, const V& b)    { U x(a); x += b; return x; }
	friend U  operator -  (const U& a, const V& b)    { U x(a); x += -b; return x; }
};

template <class T, class B = ПустойКласс>
class PostfixOps : public B
{
public:
	friend T operator ++ (T& i, цел)                  { T x = i; ++i; return x; }
	friend T operator -- (T& i, цел)                  { T x = i; --i; return x; }
};

template <class T, цел (*compare)(T a, T b), class B = ПустойКласс>
class CompareRelOps : public B
{
public:
	friend бул operator <  (T a, T b) { return (*compare)(a, b) <  0; }
	friend бул operator >  (T a, T b) { return (*compare)(a, b) >  0; }
	friend бул operator == (T a, T b) { return (*compare)(a, b) == 0; }
	friend бул operator != (T a, T b) { return (*compare)(a, b) != 0; }
	friend бул operator <= (T a, T b) { return (*compare)(a, b) <= 0; }
	friend бул operator >= (T a, T b) { return (*compare)(a, b) >= 0; }
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


struct CombineCompare {
	цел result;

	template <class T>
	CombineCompare& operator()(const T& a, const T& b) { if(!result) result = сравниЗнак(a, b); return *this; }

	operator цел() const                               { return result; }

	CombineCompare()                                   { result = 0; }

	template <class T>
	CombineCompare(const T& a, const T& b)             { result = 0; operator()(a, b); }
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
	СГлубокойКопией& operator=(const СГлубокойКопией& a)     { (T&)*this = pick(T(a, 1)); return *this; }

	СГлубокойКопией(T&& a) : T(pick(a))                   {}
	СГлубокойКопией& operator=(T&& a)                     { (T&)*this = pick(a); return *this; }

	СГлубокойКопией(СГлубокойКопией&& a) : T(pick(a))        {}
	СГлубокойКопией& operator=(СГлубокойКопией&& a)          { (T&)*this = pick(a); return *this; }

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
T *DeclPtr__();

template <class Диапазон>
using ТипЗначенияМассива = typename std::remove_reference<decltype((*DeclPtr__<Диапазон>())[0])>::type;

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
	комбинируйХэш& помести(т_хэш h)                                      { hash = HASH_CONST2 * hash + h; return *this; }

	operator т_хэш() const                                         { return hash; }

	комбинируйХэш()                                                   { hash = HASH_CONST1; }
	template <class T>
	комбинируйХэш(const T& h1)                                        { hash = HASH_CONST1; делай(h1); }
	template <class T, class U>
	комбинируйХэш(const T& h1, const U& h2)                           { hash = HASH_CONST1; делай(h1); делай(h2); }
	template <class T, class U, class V>
	комбинируйХэш(const T& h1, const U& h2, const V& h3)              { hash = HASH_CONST1; делай(h1); делай(h2); делай(h3); }
	template <class T, class U, class V, class W>
	комбинируйХэш(const T& h1, const U& h2, const V& h3, const W& h4)	{ hash = HASH_CONST1; делай(h1); делай(h2); делай(h3); делай(h4); }

	template <class T> комбинируйХэш& operator<<(const T& x)          { делай(x); return *this; }
};


template <цел size>
struct Data_S_ : Движ< Data_S_<size> >
{
	ббайт filler[size];
};
///////////////////////////////
#include <DinrusPro/TextCore.h>
///////////////////////////////
//Vcont.h
#ifdef _ОТЛАДКА
проц всёКогдаПодобрано(ук укз);
проц ВСЁ_КОГДА_ПОДОБРАНО__(ук укз);

template <class T>
проц ВСЁ_КОГДА_ПОДОБРАНО(T& x)
{
	ВСЁ_КОГДА_ПОДОБРАНО__(&x);
}
#endif

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
	проц        operator=(Один<TT>&& d)     { if((проц *)this != (проц *)&d) { освободи(); подбери(pick(d)); }}

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
	Один(Один<TT>&& p)                       { подбери(pick(p)); }
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
	T&       добавь(T&& x)                      { return items < alloc ? *(::new(Rdd()) T(pick(x))) : нарастиДобавь(pick(x)); }
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
	T&       уст(цел i, T&& x)          { return по(i) = pick(x); }
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
	проц     приставь(Вектор&& x)                    { вставь(дайСчёт(), pick(x)); }
	template <class Диапазон>
	проц     приставьДиапазон(const Диапазон& r)           { вставьДиапазон(дайСчёт(), r); }

	проц     вставьРазбей(цел i, Вектор<T>& v, цел from);

	проц     разверни(цел i1, цел i2)    { РНЦП::разверни(дай(i1), дай(i2)); }

	проц     сбрось(цел n = 1)         { ПРОВЕРЬ(n <= дайСчёт()); обрежь(items - n); }
	T&       верх()                   { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	const T& верх() const             { ПРОВЕРЬ(дайСчёт()); return дай(items - 1); }
	T        вынь()                   { T h = pick(верх()); сбрось(); return h; }

	operator T*()                    { return (T*)vector; }
	operator const T*() const        { return (T*)vector; }

	Вектор&  operator<<(const T& x)  { добавь(x); return *this; }
	Вектор&  operator<<(T&& x)            { добавь(pick(x)); return *this; }

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
	Вектор(Вектор&& v)               { подбери(pick(v)); }
	проц operator=(Вектор&& v)       { if(this != &v) { освободи(); подбери(pick(v)); } }

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
	T&       добавьПодбор(T&& x)             { return items < alloc ? *(::new(Rdd()) T(pick(x))) : нарастиДобавь(pick(x)); }
	цел      дайИндекс(const T& элт) const;
	T&       вставьПодбор(цел i, T&& x)   { return вставь(i, pick(x)); }
	проц     вставьПодбор(цел i, Вектор&& x) { вставь(i, pick(x)); }
	проц     приставьПодбор(Вектор&& x)                { вставьПодбор(дайСчёт(), pick(x)); }
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
	T&       добавь(T&& x)                      { T *q = new T(pick(x)); vector.добавь(q); return *q; }
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
	T&       уст(цел i, T&& x)          { return по(i) = pick(x); }
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
	проц     вставь(цел i, Массив&& x)             { vector.вставь(i, pick(x.vector)); }
	проц     приставь(const Массив& x)               { вставь(дайСчёт(), x); }
	проц     приставь(const Массив& x, цел o, цел c) { вставь(дайСчёт(), x, o, c); }
	проц     приставь(Массив&& x)                    { вставьПодбор(дайСчёт(), pick(x)); }
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
	Массив& operator<<(T&& x)            { добавь(pick(x)); return *this; }
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
	Массив(Массив&& v) : vector(pick(v.vector))  {}
	проц operator=(Массив&& v)           { if(this != &v) { освободи(); vector = pick(v.vector); } }

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
	проц     вставьПодбор(цел i, Массив&& x)       { vector.вставьПодбор(i, pick(x.vector)); }
	проц     приставьПодбор(Массив&& x)              { вставьПодбор(дайСчёт(), pick(x)); }
	T&       индексируй(цел i)                     { return по(i); }
	T&       индексируй(цел i, const T& x)         { return по(i, x); }
	T&       добавьПодбор(T&& x)                     { T *q = new T(pick(x)); vector.добавь(q); return *q; }
	T&       вставьПодбор(цел i, T&& x)           { return вставь(i, pick(x)); }
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
	проц     подбери(БиВектор&& x)      { vector = pick(x.vector); start = x.start; items = x.items;
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
	проц     добавьГолову(T&& x)          { new(добавьГолову0()) T(pick(x)); }
	проц     добавьХвост(T&& x)          { new(добавьХвост0()) T(pick(x)); }
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
	БиВектор(БиВектор&& ист)                    { подбери(pick(ист)); }
	проц operator=(БиВектор&& ист)              { if(this != &ист) { освободи(); подбери(pick(ист)); } }
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

	БиМассив(БиМассив&& ист) : bv(pick(ист.bv)){}
	проц operator=(БиМассив&& ист)            { if(this != &ист) { освободи(); bv = pick(ист.bv); } }
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
T *DeclPtr__();

template <class Диапазон>
using типЗначУ = typename std::remove_reference<decltype(*DeclPtr__<typename std::remove_reference<Диапазон>::type>()->begin())>::type;

template <class Диапазон>
using ОбходчикУ = decltype(DeclPtr__<typename std::remove_reference<Диапазон>::type>()->begin());

template <class Диапазон>
using КонстОбходчикУ = decltype(DeclPtr__<const typename std::remove_reference<Диапазон>::type>()->begin());

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

	КлассДиапазонОбзора(ДиапазонОснова& r, Вектор<цел>&& ndx) : r(&r), ndx(pick(ndx)) {}
	КлассДиапазонОбзора() {} // MSC bug workaround
};

template <class ДиапазонОснова>
КлассДиапазонОбзора<ДиапазонОснова> ДиапазонОбзора(ДиапазонОснова&& r, Вектор<цел>&& ndx)
{
	return КлассДиапазонОбзора<ДиапазонОснова>(r, pick(ndx));
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
			Обх e = Обх(begin + min(chunk, т_мера(end - begin)));
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
			auto e = begin + min(chunk, т_мера(end - begin));
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
			rs.добавь(pick(v));
		}
	});
	соСортируй(rs, [](const Вектор<цел>& a, const Вектор<цел>& b) { return a[0] < b[0]; });
	Вектор<цел> result;
	for(const auto& s : rs)
		result.приставь(s);
	return result;
}

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
КОНЕЦ_ПИ_РНЦП

#include <DinrusPro/Core/Mt.h>
#include <DinrusPro/Core/String.h>
#include <DinrusPro/Core/TimeDate.h>
#include <DinrusPro/Core/Stream.h>
#include <DinrusPro/Core/Diag.h>
#include <DinrusPro/Core/Index.h>
#include <DinrusPro/Core/Map.h>
#include <DinrusPro/Core/Sorted.h>
#include <DinrusPro/Core/Sort.h>
#include <DinrusPro/Core/Obsolete.h>
#include <DinrusPro/Core/FixedMap.h>
#include <DinrusPro/Core/InVector.h>
#include <DinrusPro/Core/CharSet.h>
#include <DinrusPro/Core/Path.h>
#include <DinrusPro/Core/SplitMerge.h>
#include <DinrusPro/Core/Other.h>
#include <DinrusPro/Core/Lang.h>
#include <DinrusPro/Core/Value.h>
#include <DinrusPro/Core/ValueUtil.h>
#include <DinrusPro/Core/Tuple.h>
#include <DinrusPro/Core/Uuid.h>
#include <DinrusPro/Core/Ptr.h>
#include <DinrusPro/Core/Function.h>
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


#include <DinrusPro/Core/CoWork.h>

#include <DinrusPro/Core/CoAlgo.h>
#include <DinrusPro/Core/CoSort.h>

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
