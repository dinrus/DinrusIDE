// #define MEMORY_SHRINK

#if !defined(flagUSEMALLOC) && !defined(flagSO) && !defined(flagHEAPOVERRIDE)
#define КУЧА_РНЦП
#endif

#define __ВСЁ__        (*(volatile int *)0 = 0) // kill(getpid(), SIGTRAP)

#ifdef COMPILER_MSC
	#pragma warning(disable : 4800)
	#pragma warning(disable : 4129)
	#pragma warning(disable : 4290)
	#pragma warning(disable : 4068)
	#pragma warning(disable : 4005)
	#pragma warning(disable : 4675)
	#pragma warning(disable : 4996)
	#pragma warning(disable : 4180)

	#pragma setlocale("C")
#endif

bool    режимПаники_ли();

void    паника(const char *msg);

void    неуспехПроверки(const char *file, int line, const char *cond);

void    устБоксСообПаники(void (*mb)(const char *title, const char *text));
void    боксСообПаники(const char *title, const char *text);

#define УБЕДИСЬ(x)        ((x) ? (void)0 : ::РНЦПДинрус::неуспехПроверки(__FILE__, __LINE__, #x))

#ifdef _ОТЛАДКА

#define ПРОВЕРЬ_(x, msg)  ((x) ? (void)0 : ::РНЦПДинрус	::неуспехПроверки(__FILE__, __LINE__, msg))
#define ПРОВЕРЬ(x)        ПРОВЕРЬ_(x, #x)

#else

inline  void LOGNOP__() {}

#define ЛОГ_НОП          РНЦПДинрус::LOGNOP__()

#define ПРОВЕРЬ_(x, msg)  ЛОГ_НОП
#define ПРОВЕРЬ(x)        ЛОГ_НОП

#endif

#define _cm_  ,

#define __countof(a)          int(sizeof(a) / sizeof(a[0]) )

#define КАКТКСТ__(x)  #x
#define КАКТКСТ(x)   КАКТКСТ__(x)

#define КОМБИНИРУЙ__(a, b)            a##b
#define КОМБИНИРУЙ(a, b)              КОМБИНИРУЙ__(a, b)

#define COMBINE3__(a, b, c)        a##b##c
#define COMBINE3(a, b, c)          COMBINE3__(a, b, c)

#define COMBINE4__(a, b, c, d)     a##b##c##d
#define COMBINE4(a, b, c, d)       COMBINE4__(a, b, c, d)

#define COMBINE5__(a, b, c, d, e)  a##b##c##d##e
#define COMBINE5(a, b, c, d, e)    COMBINE5__(a, b, c, d, e)

#define MK__s__(x)       s__s##x
#define MK__s_(x)        MK__s__(x)

#ifdef  BLITZ_INDEX__
#define MK__s            MK__s_(COMBINE3(BLITZ_INDEX__, _, __LINE__))
#else
#define MK__s            MK__s_(__LINE__)
#endif

#ifdef flagCHECKINIT
void начниИницБлок__(const char *фн, int line);
void завершиИницБлок__(const char *фн, int line);
#else
inline void начниИницБлок__(const char *, int) {}
inline void завершиИницБлок__(const char *, int) {}
#endif

struct ИницВызов {
	ИницВызов(void (*фн)(), const char *cpp, int line) { начниИницБлок__(cpp, line); фн(); завершиИницБлок__(cpp, line); }
	ИницВызов(void (*фн)())                            { фн(); }
};

struct ВыходИзВызова {
	ВыходИзВызова(void (*фн)()) { atexit(фн); }
};


#define ИНИЦБЛОК \
static void КОМБИНИРУЙ(MK__s, _fn)(); static РНЦП::ИницВызов MK__s(КОМБИНИРУЙ(MK__s, _fn), __FILE__, __LINE__); \
static void КОМБИНИРУЙ(MK__s, _fn)()

#define ИНИЦБЛОК_(x) \
static void КОМБИНИРУЙ(x, _fn)(); static РНЦП::ИницВызов x(КОМБИНИРУЙ(x, _fn), __FILE__, __LINE__); \
static void КОМБИНИРУЙ(x, _fn)()


#define ЭКЗИТБЛОК \
static void КОМБИНИРУЙ(MK__s, _fn)(); static РНЦП::ВыходИзВызова MK__s(КОМБИНИРУЙ(MK__s, _fn)); \
static void КОМБИНИРУЙ(MK__s, _fn)()

#define ЭКЗИТБЛОК_(x) \
static void КОМБИНИРУЙ(x, _fn)(); static РНЦП::ВыходИзВызова x(КОМБИНИРУЙ(x, _fn)); \
static void КОМБИНИРУЙ(x, _fn)()

#define ИНИЦИАЛИЗУЙ(x) \
void x##__initializer(); \
static struct x##__initialize_struct { x##__initialize_struct() { x##__initializer(); } } x##__initialize_dummy;

#define ИНИЦИАЛИЗАТОР(x) \
void x##__initializer_fn(); \
void x##__initializer() \
{ \
	ONCELOCK { \
		x##__initializer_fn(); \
	} \
} \
void x##__initializer_fn()



#ifdef DEPRECATED
#define ИНИЦКОД(x) \
static void КОМБИНИРУЙ(MK__s, _fn)() { x } static РНЦП::ИницВызов MK__s(КОМБИНИРУЙ(MK__s, _fn), __FILE__, __LINE__);

#define КОД_ВЫХОДА(x) \
static void КОМБИНИРУЙ(MK__s, _fn)() { x } static РНЦП::ВыходИзВызова MK__s(КОМБИНИРУЙ(MK__s, _fn));
#endif

typedef unsigned char      byte;
typedef signed char        int8;
typedef unsigned char      uint8;

typedef short unsigned     word;
typedef short int          int16;
typedef short unsigned     uint16;

#ifdef PLATFORM_WIN32
typedef unsigned long      dword;
typedef long               int32;
typedef unsigned long      uint32;
typedef WCHAR              char16;

#else
typedef unsigned int       dword;
typedef int                int32;
typedef unsigned int       uint32;
typedef word               char16;
#endif

typedef uint32             wchar;

#ifdef COMPILER_MSC
typedef __int64            int64;
typedef unsigned __int64   uint64;
#else
typedef long long int      int64;
typedef long long unsigned uint64;
#endif

typedef uint64             qword;

struct m128 {
	int64 i64[2];
	
	static m128 обнули()             { m128 a; a.i64[0] = a.i64[1] = 0; return a; }
};

inline bool НЧ_ли(double d)        { return std::isnan(d); }
inline bool БЕСК_ли(double d)        { return std::isinf(d); }
inline bool конечен(double d)        { return !НЧ_ли(d) && !БЕСК_ли(d); }

#ifdef COMPILER_MSC
	#define I64(c) ((int64)КОМБИНИРУЙ(c, i64))
#else
	#define I64(c) ((int64)КОМБИНИРУЙ(c, LL))
#endif

#ifndef INT64_MIN
#define INT64_MIN          I64(-0x8000000000000000)
#endif
#ifndef INT64_MAX
#define INT64_MAX          I64(+0x7FFFFFFFFFFFFFFF)
#endif

#if !defined(PLATFORM_WIN32)

#define HIBYTE(a)        (byte)((a) >> 8)
#define LOBYTE(a)        byte(a)
#define HIWORD(a)        (word)((a) >> 16)
#define LOWORD(a)        word(a)

#define MAKEWORD(l, h)   ((word)  (((byte) (l)) | ((word) ((byte) (h))) << 8))
#define MAKELONG(l, h)   ((dword) (((word) (l)) | ((dword) ((word) (h))) << 16))

#endif

#define MAKEQWORD(l, h)  ((qword) (((dword) (l)) | ((qword) ((dword) (h))) << 32))
#define HIDWORD(a)       (dword)(((uint64)a) >> 32)
#define LODWORD(a)       dword(a)

#define OFFSETOF(clss, mbr) ((int)(uintptr_t)&(((clss *)1)->mbr) - 1)

template <typename T> T clone(const T& x) { return x; }

#define pick_
#define rval_default(T) T(T&&) = default; T& operator=(T&&) = default;

template <typename T>
auto pick(T&& x) noexcept -> decltype(std::move(x)) { return std::move(x); }

template<class T> class Функция;

#ifdef DEPRECATED
#define rval_ &&
#define init_
#endif

#ifdef COMPILER_MSC
#define force_inline __forceinline
#define never_inline __declspec(noinline)
#elif defined(COMPILER_GCC)
#define force_inline __attribute__((always_inline)) inline
#define never_inline __attribute__((noinline))
#else
#define force_inline inline
#define never_inline
#endif

#ifdef _ОТЛАДКА
#undef force_inline
#define force_inline inline
#endif

#define BINARY(i, f) \
extern "C" byte *i; \
extern "C" int КОМБИНИРУЙ(i, _length);

#define BINARY_ARRAY(i, x, f) \
extern "C" byte *i[]; \
extern "C" int КОМБИНИРУЙ(i, _length)[]; \
extern "C" int КОМБИНИРУЙ(i, _count);

#define BINARY_MASK(i, m) \
extern "C" byte *i[]; \
extern "C" int КОМБИНИРУЙ(i, _length)[]; \
extern "C" int КОМБИНИРУЙ(i, _count); \
extern "C" char *КОМБИНИРУЙ(i, _files)[];

class БезКопий {
private:
	БезКопий(const БезКопий&);
	void operator=(const БезКопий&);
public:
	БезКопий() {}
};

const int    INT_NULL           =    INT_MIN;
const int64  INT64_NULL         =    INT64_MIN;

constexpr double DOUBLE_NULL    =    -std::numeric_limits<double>::infinity();

class Обнул {
public:
	operator int() const                { return INT_NULL; }
	operator int64() const              { return INT64_NULL; }
	operator double() const             { return DOUBLE_NULL; }
	operator bool() const               { return false; }

	Обнул() {}
};

extern const Обнул Null;

template <class T> void устПусто(T& x) { x = Null; }

template <class T> bool пусто_ли(const T& x)       { return x.экзПусто_ли(); }

template<> inline bool  пусто_ли(const int& i)     { return i == INT_NULL; }
template<> inline bool  пусто_ли(const int64& i)   { return i == INT64_NULL; }
template<> inline bool  пусто_ли(const double& r)  { return !(std::abs(r) < std::numeric_limits<double>::infinity()); }
template<> inline bool  пусто_ли(const bool& r  )  { return false; }

#include "Heap.h"
#ifdef CPU_X86
bool цпбММХ();
bool цпбССЕ();
bool цпбССЕ2();
bool цпбССЕ3();
bool цпбГипервизор();
bool цпбАВХ();
#endif

int  цпбЯдра();

void дайСтатусСисПамяти(uint64& total, uint64& available);

template <class T>
inline void разверни(T& a, T& b) { T tmp = pick(a); a = pick(b); b = pick(tmp); }

//Quick fix....
#ifdef PLATFORM_WINCE
const char *изСисНабСима(const wchar *s);
const wchar *вСисНабСим(const char *s);
#else
inline const char *изСисНабСима(const char *s) { return s; }
inline const char *вСисНабСим(const char *s) { return s; }
#endif

#ifdef _ОТЛАДКА
void __LOGF__(const char *формат, ...);
#define LOGF             РНЦП::__LOGF__
#else
inline void __LOGF__(const char *формат, ...);
#endif

template <class T>
void IGNORE_RESULT(const T&) {}

// Backward compatibility

#define GLOBAL_VP(тип, имя, param) \
имя() \
{ \
	static тип x param; \
	return x; \
}

#define GLOBAL_VARP(тип, имя, param) \
тип& GLOBAL_VP(тип, имя, param)

#define GLOBAL_V(тип, имя)   GLOBAL_VP(тип, имя, init_)
#define GLOBAL_VAR(тип, имя) тип& GLOBAL_V(тип, имя)

#define GLOBAL_VP_INIT(тип, имя, param) \
имя() \
{ \
	static тип x param; \
	return x; \
} \
ИНИЦБЛОК { имя(); }

#define GLOBAL_VARP_INIT(тип, имя, param) \
тип& GLOBAL_VP_INIT(тип, имя, param)

#define GLOBAL_V_INIT(тип, имя)   GLOBAL_VP_INIT(тип, имя, init_)
#define GLOBAL_VAR_INIT(тип, имя) тип& GLOBAL_V_INIT(тип, имя)

#if __GNUC__ > 6
	#define NOUBSAN __attribute__((no_sanitize_undefined))
#elif __clang_major__ > 6
	#define NOUBSAN __attribute__((no_sanitize("undefined")))
#else
	#define NOUBSAN
#endif

// DEPRECATED:

// these are pre-c++ 11 tools to achieve something like variadic templates
#define __Expand1(x) x(1)
#define __Expand2(x)  __Expand1(x) x(2)
#define __Expand3(x)  __Expand2(x) x(3)
#define __Expand4(x)  __Expand3(x) x(4)
#define __Expand5(x)  __Expand4(x) x(5)
#define __Expand6(x)  __Expand5(x) x(6)
#define __Expand7(x)  __Expand6(x) x(7)
#define __Expand8(x)  __Expand7(x) x(8)
#define __Expand9(x)  __Expand8(x) x(9)
#define __Expand10(x) __Expand9(x) x(10)
#define __Expand11(x) __Expand10(x) x(11)
#define __Expand12(x) __Expand11(x) x(12)
#define __Expand13(x) __Expand12(x) x(13)
#define __Expand14(x) __Expand13(x) x(14)
#define __Expand15(x) __Expand14(x) x(15)
#define __Expand16(x) __Expand15(x) x(16)
#define __Expand17(x) __Expand16(x) x(17)
#define __Expand18(x) __Expand17(x) x(18)
#define __Expand19(x) __Expand18(x) x(19)
#define __Expand20(x) __Expand19(x) x(20)
#define __Expand21(x) __Expand20(x) x(21)
#define __Expand22(x) __Expand21(x) x(22)
#define __Expand23(x) __Expand22(x) x(23)
#define __Expand24(x) __Expand23(x) x(24)
#define __Expand25(x) __Expand24(x) x(25)
#define __Expand26(x) __Expand25(x) x(26)
#define __Expand27(x) __Expand26(x) x(27)
#define __Expand28(x) __Expand27(x) x(28)
#define __Expand29(x) __Expand28(x) x(29)
#define __Expand30(x) __Expand29(x) x(30)
#define __Expand31(x) __Expand30(x) x(31)
#define __Expand32(x) __Expand31(x) x(32)
#define __Expand33(x) __Expand32(x) x(33)
#define __Expand34(x) __Expand33(x) x(34)
#define __Expand35(x) __Expand34(x) x(35)
#define __Expand36(x) __Expand35(x) x(36)
#define __Expand37(x) __Expand36(x) x(37)
#define __Expand38(x) __Expand37(x) x(38)
#define __Expand39(x) __Expand38(x) x(39)
#define __Expand40(x) __Expand39(x) x(40)

#define __Expand(x)   __Expand40(x)

#define __List1(x) x(1)
#define __List2(x)  __List1(x), x(2)
#define __List3(x)  __List2(x), x(3)
#define __List4(x)  __List3(x), x(4)
#define __List5(x)  __List4(x), x(5)
#define __List6(x)  __List5(x), x(6)
#define __List7(x)  __List6(x), x(7)
#define __List8(x)  __List7(x), x(8)
#define __List9(x)  __List8(x), x(9)
#define __List10(x) __List9(x), x(10)
#define __List11(x) __List10(x), x(11)
#define __List12(x) __List11(x), x(12)
#define __List13(x) __List12(x), x(13)
#define __List14(x) __List13(x), x(14)
#define __List15(x) __List14(x), x(15)
#define __List16(x) __List15(x), x(16)
#define __List17(x) __List16(x), x(17)
#define __List18(x) __List17(x), x(18)
#define __List19(x) __List18(x), x(19)
#define __List20(x) __List19(x), x(20)
#define __List21(x) __List20(x), x(21)
#define __List22(x) __List21(x), x(22)
#define __List23(x) __List22(x), x(23)
#define __List24(x) __List23(x), x(24)
#define __List25(x) __List24(x), x(25)
#define __List26(x) __List25(x), x(26)
#define __List27(x) __List26(x), x(27)
#define __List28(x) __List27(x), x(28)
#define __List29(x) __List28(x), x(29)
#define __List30(x) __List29(x), x(30)
#define __List31(x) __List30(x), x(31)
#define __List32(x) __List31(x), x(32)
#define __List33(x) __List32(x), x(33)
#define __List34(x) __List33(x), x(34)
#define __List35(x) __List34(x), x(35)
#define __List36(x) __List35(x), x(36)
#define __List37(x) __List36(x), x(37)
#define __List38(x) __List37(x), x(38)
#define __List39(x) __List38(x), x(39)
#define __List40(x) __List39(x), x(40)

#define E__p(I)       p##I
