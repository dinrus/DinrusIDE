template <class I>
inline void IterSwap(I a, I b) { разверни(*a, *b); }

struct EmptyClass {};

template <class T, class B = EmptyClass>
class RelOps : public B
{
public:
	friend bool operator >  (const T& a, const T& b)  { return b < a; }
	friend bool operator != (const T& a, const T& b)  { return !(a == b); }
	friend bool operator <= (const T& a, const T& b)  { return !(b < a); }
	friend bool operator >= (const T& a, const T& b)  { return !(a < b); }
};

template <class U, class V, class B = EmptyClass>
class AddOps : public B
{
public:
	friend U& operator -= (U& a, const V& b)          { a += -b; return a; }
	friend U  operator +  (const U& a, const V& b)    { U x(a); x += b; return x; }
	friend U  operator -  (const U& a, const V& b)    { U x(a); x += -b; return x; }
};

template <class T, class B = EmptyClass>
class PostfixOps : public B
{
public:
	friend T operator ++ (T& i, int)                  { T x = i; ++i; return x; }
	friend T operator -- (T& i, int)                  { T x = i; --i; return x; }
};

template <class T, int (*compare)(T a, T b), class B = EmptyClass>
class CompareRelOps : public B
{
public:
	friend bool operator <  (T a, T b) { return (*compare)(a, b) <  0; }
	friend bool operator >  (T a, T b) { return (*compare)(a, b) >  0; }
	friend bool operator == (T a, T b) { return (*compare)(a, b) == 0; }
	friend bool operator != (T a, T b) { return (*compare)(a, b) != 0; }
	friend bool operator <= (T a, T b) { return (*compare)(a, b) <= 0; }
	friend bool operator >= (T a, T b) { return (*compare)(a, b) >= 0; }
};

template <class T, class B = EmptyClass>
struct Comparable : public B
{
public:
	friend bool operator <  (const T& a, const T& b) { return a.сравни(b) < 0; }
	friend bool operator >  (const T& a, const T& b) { return a.сравни(b) > 0; }
	friend bool operator == (const T& a, const T& b) { return a.сравни(b) == 0; }
	friend bool operator != (const T& a, const T& b) { return a.сравни(b) != 0; }
	friend bool operator <= (const T& a, const T& b) { return a.сравни(b) <= 0; }
	friend bool operator >= (const T& a, const T& b) { return a.сравни(b) >= 0; }
	friend int  сравниЗнак(const T& a, const T& b)   { return a.сравни(b); }
};

template <class T>
int NumberCompare__(const T& a, const T& b)
{
	if(a < b) return -1;
	if(a > b) return 1;
	return 0;
}

inline int сравниЗнак(const char& a, const char& b)                     { return NumberCompare__(a, b); }
inline int сравниЗнак(const signed char& a, const signed char& b)       { return NumberCompare__(a, b); }
inline int сравниЗнак(const unsigned char& a, const unsigned char& b)   { return NumberCompare__(a, b); }
inline int сравниЗнак(const short& a, const short& b)                   { return NumberCompare__(a, b); }
inline int сравниЗнак(const unsigned short& a, const unsigned short& b) { return NumberCompare__(a, b); }
inline int сравниЗнак(const int& a, const int& b)                       { return NumberCompare__(a, b); }
inline int сравниЗнак(const unsigned int& a, const unsigned int& b)     { return NumberCompare__(a, b); }
inline int сравниЗнак(const long& a, const long& b)                     { return NumberCompare__(a, b); }
inline int сравниЗнак(const unsigned long& a, const unsigned long& b)   { return NumberCompare__(a, b); }
inline int сравниЗнак(const bool& a, const bool& b)                     { return NumberCompare__(a, b); }
inline int сравниЗнак(const int64& a, const int64& b)                   { return NumberCompare__(a, b); }
inline int сравниЗнак(const uint64& a, const uint64& b)                 { return NumberCompare__(a, b); }
inline int сравниЗнак(const float& a, const float& b)                   { return NumberCompare__(a, b); }
inline int сравниЗнак(const double& a, const double& b)                 { return NumberCompare__(a, b); }

template<class T>
inline int сравниЗнак(const T& a, const T& b)
{
	return a.сравни(b);
}

struct CombineCompare {
	int result;

	template <class T>
	CombineCompare& operator()(const T& a, const T& b) { if(!result) result = сравниЗнак(a, b); return *this; }

	operator int() const                               { return result; }

	CombineCompare()                                   { result = 0; }

	template <class T>
	CombineCompare(const T& a, const T& b)             { result = 0; operator()(a, b); }
};

template <class T>
inline void Fill(T *dst, const T *lim, const T& x) {
	while(dst < lim)
		*dst++ = x;
}

template <class T>
inline void копируй(T *dst, const T *ист, const T *lim) {
	while(ист < lim)
		*dst++ = *ист++;
}

template <class T>
inline void копируй(T *dst, const T *ист, int n) {
	копируй(dst, ист, ист + n);
}

inline void Fill(char *t, const char *lim, const char& x)
{ memset8(t, x, size_t(lim - t)); }
inline void копируй(char *dst, const char *ист, const char *lim)
{ memcpy8(dst, ист, size_t((byte *)lim - (byte *)ист)); }

inline void Fill(signed char *t, const signed char *lim, const signed char& x)
{ memset8(t, x, size_t(lim - t)); }
inline void копируй(signed char *dst, const signed char *ист, const signed char *lim)
{ memcpy8(dst, ист, size_t((byte *)lim - (byte *)ист)); }

inline void Fill(unsigned char *t, const unsigned char *lim, const unsigned char& x)
{ memset8(t, x, size_t(lim - t)); }
inline void копируй(unsigned char *dst, const unsigned char *ист, const unsigned char *lim)
{ memcpy8(dst, ист, size_t((byte *)lim - (byte *)ист)); }

#ifdef NO_MOVEABLE_CHECK

template <class T>
inline void проверьДвижимое(T *) {}

#define MoveableTemplate(T)

template <class T, class B = EmptyClass>
class Движимое : public B
{
};

template <class T>
struct Движимое_ {
};

#define NTL_MOVEABLE(T)

#else

template <class T>
inline void AssertMoveablePtr(T, T) {}

template <class T>
inline void AssertMoveable0(T *t) { AssertMoveablePtr(&**t, *t); }
// COMPILATION Ошибка HERE MEANS TYPE T WAS NOT MARKED AS Движимое

template <class T, class B = EmptyClass>
struct Движимое : public B {
	friend void AssertMoveable0(T *) {}
};

template <class T>
struct Движимое_ {
	friend void AssertMoveable0(T *) {}
};

template <class T>
inline void проверьДвижимое(T *t = 0) { if(t) AssertMoveable0(t); }

#if defined(COMPILER_MSC) || defined(COMPILER_GCC) && (__GNUC__ < 4 || __GNUC_MINOR__ < 1)
	#define NTL_MOVEABLE(T) inline void AssertMoveable0(T *) {}
#else
	#define NTL_MOVEABLE(T) template<> inline void проверьДвижимое<T>(T *) {}
#endif

#endif

NTL_MOVEABLE(bool)
NTL_MOVEABLE(char)
NTL_MOVEABLE(signed char)
NTL_MOVEABLE(unsigned char)
NTL_MOVEABLE(short)
NTL_MOVEABLE(unsigned short)
NTL_MOVEABLE(int)
NTL_MOVEABLE(unsigned int)
NTL_MOVEABLE(long)
NTL_MOVEABLE(unsigned long)
NTL_MOVEABLE(int64)
NTL_MOVEABLE(uint64)
NTL_MOVEABLE(float)
NTL_MOVEABLE(double)
NTL_MOVEABLE(void *)
NTL_MOVEABLE(const void *)

#if defined(_NATIVE_WCHAR_T_DEFINED) || defined(COMPILER_GCC)
NTL_MOVEABLE(wchar_t)
#endif

template <class T, class B = EmptyClass>
class WithClone : public B {
public:
	friend T clone(const T& ист) { T c(ист, 1); return c; }
};

template <class T, class B = EmptyClass>
class DeepCopyOption : public B {
public:
#ifdef DEPRECATED
	friend T& operator<<=(T& dest, const T& ист)
	{ if(&dest != &ист) { (&dest)->~T(); ::new(&dest) T(ист, 1); } return dest; }
#endif
	friend T  clone(const T& ист) { T c(ист, 1); return c; }
};

template <class T, class B = EmptyClass>
class ОпцияДвижимогоИГлубКопии : public B {
	friend void AssertMoveable0(T *) {}
#ifdef DEPRECATED
	friend T& operator<<=(T& dest, const T& ист)
	{ if(&dest != &ист) { (&dest)->~T(); ::new(&dest) T(ист, 1); } return dest; }
#endif
	friend T  clone(const T& ист) { T c(ист, 1); return c; }
};

template <class T>
class WithDeepCopy : public T {
public:
	WithDeepCopy(const T& a) : T(a, 1)                 {}
	WithDeepCopy(const T& a, int) : T(a, 1)            {}

	WithDeepCopy(const WithDeepCopy& a) : T(a, 1)      {}
	WithDeepCopy& operator=(const WithDeepCopy& a)     { (T&)*this = pick(T(a, 1)); return *this; }

	WithDeepCopy(T&& a) : T(pick(a))                   {}
	WithDeepCopy& operator=(T&& a)                     { (T&)*this = pick(a); return *this; }

	WithDeepCopy(WithDeepCopy&& a) : T(pick(a))        {}
	WithDeepCopy& operator=(WithDeepCopy&& a)          { (T&)*this = pick(a); return *this; }

	WithDeepCopy()                                     {}
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
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	void                  clear()                { очисть(); } \
	size_type             size() const           { return дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	bool                  empty() const          { return пустой(); } \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \

#define STL_MAP_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        старт() const          { return B::begin(); } \
	const_iterator        стоп() const            { return B::end(); } \
	void                  clear()                { B::очисть(); } \
	size_type             size() const           { return B::дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	bool                  empty() const          { return B::пустой(); } \
	iterator              старт()                { return B::begin(); } \
	iterator              стоп()                  { return B::end(); } \

#define STL_SORTED_MAP_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	void                  clear()                { B::очисть(); } \
	size_type             size() const           { return B::дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	bool                  empty() const          { return B::дайСчёт() == 0; } \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \

#define STL_VECTOR_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	void                  clear()                { очисть(); } \
	size_type             size() const           { return дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \
	reference             front()                { return (*this)[0]; } \
	const_reference       front() const          { return (*this)[0]; } \
	reference             back()                 { return верх(); } \
	const_reference       back() const           { return верх(); } \
	bool                  empty() const          { return пустой(); } \
	void                  push_back(const T& x)  { добавь(x); } \
	void                  pop_back()             { сбрось(); } \


template <class T>
T *DeclPtr__();

template <class Диапазон>
using ValueTypeOfArray = typename std::remove_reference<decltype((*DeclPtr__<Диапазон>())[0])>::type;

template <class V>
class ConstIIterator {
	typedef ValueTypeOfArray<V> T;

	const V       *cont;
	int            ii;
	struct NP { int dummy; };

public:
	const T&       operator*() const       { return (*cont)[ii]; }
	const T       *operator->() const      { return &(*cont)[ii]; }
	const T&       operator[](int i) const { return (*cont)[ii + i]; }

	ConstIIterator& operator++()           { ++ii; return *this; }
	ConstIIterator& operator--()           { --ii; return *this; }
	ConstIIterator  operator++(int)        { ConstIIterator t = *this; ++ii; return t; }
	ConstIIterator  operator--(int)        { ConstIIterator t = *this; --ii; return t; }

	ConstIIterator& operator+=(int d)      { ii += d; return *this; }
	ConstIIterator& operator-=(int d)      { ii -= d; return *this; }

	ConstIIterator  operator+(int d) const { return ConstIIterator(*cont, ii + d); }
	ConstIIterator  operator-(int d) const { return ConstIIterator(*cont, ii - d); }

	int  operator-(const ConstIIterator& b) const   { return ii - b.ii; }

	bool operator==(const ConstIIterator& b) const  { return ii == b.ii; }
	bool operator!=(const ConstIIterator& b) const  { return ii != b.ii; }
	bool operator<(const ConstIIterator& b) const   { return ii < b.ii; }
	bool operator>(const ConstIIterator& b) const   { return ii > b.ii; }
	bool operator<=(const ConstIIterator& b) const  { return ii <= b.ii; }
	bool operator>=(const ConstIIterator& b) const  { return ii >= b.ii; }

	operator bool() const     { return ii < 0; }

	ConstIIterator()          {}
	ConstIIterator(NP *null)  { ПРОВЕРЬ(null == NULL); ii = -1; }
	ConstIIterator(const V& _cont, int ii) : cont(&_cont), ii(ii) {}

	STL_ITERATOR_COMPATIBILITY
};

template <class V>
class IIterator {
	typedef ValueTypeOfArray<V> T;

	V             *cont;
	int            ii;
	struct NP { int dummy; };

public:
	T&       operator*()              { return (*cont)[ii]; }
	T       *operator->()             { return &(*cont)[ii]; }
	T&       operator[](int i)        { return (*cont)[ii + i]; }

	const T& operator*() const        { return (*cont)[ii]; }
	const T *operator->() const       { return &(*cont)[ii]; }
	const T& operator[](int i) const  { return (*cont)[ii + i]; }

	IIterator& operator++()           { ++ii; return *this; }
	IIterator& operator--()           { --ii; return *this; }
	IIterator  operator++(int)        { IIterator t = *this; ++ii; return t; }
	IIterator  operator--(int)        { IIterator t = *this; --ii; return t; }

	IIterator& operator+=(int d)      { ii += d; return *this; }
	IIterator& operator-=(int d)      { ii -= d; return *this; }

	IIterator  operator+(int d) const { return IIterator(*cont, ii + d); }
	IIterator  operator-(int d) const { return IIterator(*cont, ii - d); }

	int  operator-(const IIterator& b) const   { return ii - b.ii; }

	bool operator==(const IIterator& b) const  { return ii == b.ii; }
	bool operator!=(const IIterator& b) const  { return ii != b.ii; }
	bool operator<(const IIterator& b) const   { return ii < b.ii; }
	bool operator>(const IIterator& b) const   { return ii > b.ii; }
	bool operator<=(const IIterator& b) const  { return ii <= b.ii; }
	bool operator>=(const IIterator& b) const  { return ii >= b.ii; }

	operator bool() const                      { return ii < 0; }

	IIterator()          {}
	IIterator(NP *null)  { ПРОВЕРЬ(null == NULL); ii = -1; }
	IIterator(V& _cont, int ii) : cont(&_cont), ii(ii) {}

	STL_ITERATOR_COMPATIBILITY
};

unsigned Pow2Bound(unsigned i);
unsigned PrimeBound(unsigned i);

hash_t memhash(const void *ptr, size_t size);

template <class T>
inline hash_t дайХэшЗнач(const T& x)                              { return x.дайХэшЗнач(); }

struct комбинируйХэш {
	hash_t hash;

	template <class T> комбинируйХэш& делай(const T& x)                  { помести(дайХэшЗнач(x)); return *this; }

public:
	комбинируйХэш& помести(hash_t h)                                      { hash = HASH_CONST2 * hash + h; return *this; }

	operator hash_t() const                                         { return hash; }

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

template<> inline hash_t дайХэшЗнач(const char& a)           { return (hash_t)a; }
template<> inline hash_t дайХэшЗнач(const signed char& a)    { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const unsigned char& a)  { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const short& a)          { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const unsigned short& a) { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const int& a)            { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const unsigned int& a)   { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const long& a)           { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const unsigned long& a)  { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const bool& a)           { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const wchar_t& a)        { return (const hash_t)a; }

#ifdef HASH64
template<> inline hash_t дайХэшЗнач(const int64& a)          { return (const hash_t)a; }
template<> inline hash_t дайХэшЗнач(const uint64& a)         { return (const hash_t)a; }
#else
template<> inline hash_t дайХэшЗнач(const int64& a)          { return комбинируйХэш((hash_t)a, (hash_t)(a >> 32)); }
template<> inline hash_t дайХэшЗнач(const uint64& a)         { return дайХэшЗнач((int64)a); }
#endif

template<> inline hash_t дайХэшЗнач(const double& a)         { return memhash(&a, sizeof(a)); }
//template<> inline hash_t дайХэшЗнач(const float& a)          { double memhash(&a, sizeof(a)); }

#ifdef CPU_32
inline hash_t GetPtrHashValue(const void *a)                   { return (int)a; }
#else
inline hash_t GetPtrHashValue(const void *a)                   { return комбинируйХэш((hash_t)(uintptr_t)a); }
#endif

template <class T>
inline hash_t дайХэшЗнач(T *ptr)                             { return GetPtrHashValue(reinterpret_cast<const void *>(ptr)); }

template <int size>
struct Data_S_ : Движимое< Data_S_<size> >
{
	byte filler[size];
};

template <class C>
bool IsEqualMap(const C& a, const C& b)
{
	if(a.дайСчёт() != b.дайСчёт())
		return false;
	for(int i = 0; i < a.дайСчёт(); i++)
		if(a.дайКлюч(i) != b.дайКлюч(i) || a[i] != b[i])
			return false;
	return true;
}

template <class C>
int CompareMap(const C& a, const C& b)
{
	int n = min(a.дайСчёт(), b.дайСчёт());
	for(int i = 0; i < n; i++) {
		int q = сравниЗнак(a.дайКлюч(i), b.дайКлюч(i));
		if(q)
			return q;
		q = сравниЗнак(a[i], b[i]);
		if(q)
			return q;
	}
	return сравниЗнак(a.дайСчёт(), b.дайСчёт());
}
