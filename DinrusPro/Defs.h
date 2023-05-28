#ifndef _DinrusPro_Defs_h_
#define _DinrusPro_Defs_h_

#ifdef PLATFORM_WIN32
#include <DinrusPro/Windows.h>
#include <plugin/z/lib/zlib.h>
#else
#include <zlib.h>
#endif

#if defined(PLATFORM_POSIX) || defined(PLATFORM_LINUX)
#include <DinrusPro/Posix.h>
#endif

#include "config.h"

#include <typeinfo>
#include <stddef.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

#ifdef CPU_X86
#include <immintrin.h>
#ifdef COMPILER_GCC
#include <x86intrin.h>
#endif
#endif

#include <functional>
#include <algorithm>
#include <string>
#include <complex>
#include <type_traits>
#include <atomic>
#include <chrono>
#include <utility>

#if !defined(flagUSEMALLOC) && !defined(flagSO) && !defined(flagHEAPOVERRIDE)
#define КУЧА_РНЦП
#endif

#define __ВСЁ__        (*(volatile цел *)0 = 0) // kill(getpid(), SIGTRAP)

#define УБЕДИСЬ(x)        ((x) ? (проц)0 : ::ДинрусРНЦП::неуспехПроверки(__FILE__, __LINE__, #x))

#ifdef _ОТЛАДКА

#define ПРОВЕРЬ_(x, msg)  ((x) ? (проц)0 : ::ДинрусРНЦП	::неуспехПроверки(__FILE__, __LINE__, msg))
#define ПРОВЕРЬ(x)        ПРОВЕРЬ_(x, #x)

#else

namespace ДинрусРНЦП{
inline  void LOGNOP__() {}

#define ЛОГ_НОП          LOGNOP__()

#define ПРОВЕРЬ_(x, msg)  ЛОГ_НОП
#define ПРОВЕРЬ(x)        ЛОГ_НОП
}

#endif

#define _cm_  ,

#define __countof(a)          цел(sizeof(a) / sizeof(a[0]) )

#define КАКТКСТ__(x)  #x
#define КАКТКСТ(x)   КАКТКСТ__(x)

#define КОМБИНИРУЙ__(a, b)            a##b
#define КОМБИНИРУЙ(a, b)              КОМБИНИРУЙ__(a, b)

#define КОМБИНИРУЙ3__(a, b, c)        a##b##c
#define КОМБИНИРУЙ3(a, b, c)          КОМБИНИРУЙ3__(a, b, c)

#define КОМБИНИРУЙ4__(a, b, c, d)     a##b##c##d
#define КОМБИНИРУЙ4(a, b, c, d)       КОМБИНИРУЙ4__(a, b, c, d)

#define КОМБИНИРУЙ5__(a, b, c, d, e)  a##b##c##d##e
#define КОМБИНИРУЙ5(a, b, c, d, e)    КОМБИНИРУЙ5__(a, b, c, d, e)

#define MK__s__(x)       s__s##x
#define MK__s_(x)        MK__s__(x)

#ifdef  BLITZ_INDEX__
#define MK__s            MK__s_(КОМБИНИРУЙ3(BLITZ_INDEX__, _, __LINE__))
#else
#define MK__s            MK__s_(__LINE__)
#endif

#define ИНИЦБЛОК \
static проц КОМБИНИРУЙ(MK__s, _fn)(); static РНЦП::ИницВызов MK__s(КОМБИНИРУЙ(MK__s, _fn), __FILE__, __LINE__); \
static проц КОМБИНИРУЙ(MK__s, _fn)()

#define ИНИЦБЛОК_(x) \
static проц КОМБИНИРУЙ(x, _fn)(); static РНЦП::ИницВызов x(КОМБИНИРУЙ(x, _fn), __FILE__, __LINE__); \
static проц КОМБИНИРУЙ(x, _fn)()


#define ЭКЗИТБЛОК \
static проц КОМБИНИРУЙ(MK__s, _fn)(); static РНЦП::ВыходИзВызова MK__s(КОМБИНИРУЙ(MK__s, _fn)); \
static проц КОМБИНИРУЙ(MK__s, _fn)()

#define ЭКЗИТБЛОК_(x) \
static проц КОМБИНИРУЙ(x, _fn)(); static РНЦП::ВыходИзВызова x(КОМБИНИРУЙ(x, _fn)); \
static проц КОМБИНИРУЙ(x, _fn)()

#define ИНИЦИАЛИЗУЙ(x) \
проц x##__initializer(); \
static struct x##__initialize_struct { x##__initialize_struct() { x##__initializer(); } } x##__initialize_dummy;

#define ИНИЦИАЛИЗАТОР(x) \
проц x##__initializer_fn(); \
проц x##__initializer() \
{ \
	ONCELOCK { \
		x##__initializer_fn(); \
	} \
} \
проц x##__initializer_fn()

#ifdef COMPILER_MSC
	#define I64(c) ((дол)КОМБИНИРУЙ(c, i64))
#else
	#define I64(c) ((дол)КОМБИНИРУЙ(c, LL))
#endif

#ifndef INT64_MIN
#define INT64_MIN          I64(-0x8000000000000000)
#endif
#ifndef INT64_MAX
#define INT64_MAX          I64(+0x7FFFFFFFFFFFFFFF)
#endif

#if !defined(PLATFORM_WIN32)

#define HIBYTE(a)        (ббайт)((a) >> 8)
#define LOBYTE(a)        ббайт(a)
#define HIWORD(a)        (бкрат)((a) >> 16)
#define LOWORD(a)        бкрат(a)

#define MAKEWORD(l, h)   ((бкрат)  (((ббайт) (l)) | ((бкрат) ((ббайт) (h))) << 8))
#define MAKELONG(l, h)   ((бцел) (((бкрат) (l)) | ((бцел) ((бкрат) (h))) << 16))

#else

#include <windef.h>

#endif

#define MAKEQWORD(l, h)  ((qword) (((бцел) (l)) | ((qword) ((бцел) (h))) << 32))
#define HIDWORD(a)       (бцел)(((бдол)a) >> 32)
#define LODWORD(a)       бцел(a)

#define OFFSETOF(clss, mbr) ((цел)(uintptr_t)&(((clss *)1)->mbr) - 1)

#ifdef CPU_LITTLE_ENDIAN
#define MAKE2B(b0, b1)                            MAKEWORD(b0, b1)
#define MAKE4B(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b0, b1), MAKEWORD(b2, b3))
#define MAKE8B(b0, b1, b2, b3, b4, b5, b6, b7)    MAKEQWORD(MAKE4B(b0, b1, b2, b3), MAKE4B(b4, b5, b6, b7))
#else
#define MAKE2B(b0, b1)                            MAKEWORD(b1, b0);
#define MAKE4B(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b2, b3), MAKEWORD(b0, b1))
#define MAKE8B(b0, b1, b2, b3, b4, b5, b6, b7)    MAKEQWORD(MAKE4B(b7, b6, b5, b4), MAKE4B(b3, b2, b1, b0))
#endif

template <typename T> T клонируй(const T& x) { return x; }

#define pick_
#define rval_default(T) T(T&&) = default; T& operator=(T&&) = default;

template <typename T>
auto пикуй(T&& x) noexcept -> decltype(std::move(x)) { return std::move(x); }

template<class T> class Функция;

#ifdef DEPRECATED
#define rval_ &&
#define иниц_
#endif

//#ifdef _ОТЛАДКА
#undef force_inline
#define force_inline inline
//#endif

#define BINARY(i, f) \
extern "C" ббайт *i; \
extern "C" цел КОМБИНИРУЙ(i, _length);

#define BINARY_ARRAY(i, x, f) \
extern "C" ббайт *i[]; \
extern "C" цел КОМБИНИРУЙ(i, _length)[]; \
extern "C" цел КОМБИНИРУЙ(i, _count);

#define BINARY_MASK(i, m) \
extern "C" ббайт *i[]; \
extern "C" цел КОМБИНИРУЙ(i, _length)[]; \
extern "C" цел КОМБИНИРУЙ(i, _count); \
extern "C" char *КОМБИНИРУЙ(i, _files)[];

// Backward compatibility

#define GLOBAL_VP(тип, имя, парам) \
имя() \
{ \
	static тип x парам; \
	return x; \
}

#define GLOBAL_VARP(тип, имя, парам) \
тип& GLOBAL_VP(тип, имя, парам)

#define GLOBAL_V(тип, имя)   GLOBAL_VP(тип, имя, иниц_)
#define GLOBAL_VAR(тип, имя) тип& GLOBAL_V(тип, имя)

#define GLOBAL_VP_INIT(тип, имя, парам) \
имя() \
{ \
	static тип x парам; \
	return x; \
} \
ИНИЦБЛОК { имя(); }

#define GLOBAL_VARP_INIT(тип, имя, парам) \
тип& GLOBAL_VP_INIT(тип, имя, парам)

#define GLOBAL_V_INIT(тип, имя)   GLOBAL_VP_INIT(тип, имя, иниц_)
#define GLOBAL_VAR_INIT(тип, имя) тип& GLOBAL_V_INIT(тип, имя)

#if __GNUC__ > 6
	#define NOUBSAN __attribute__((no_sanitize_undefined))
#elif __clang_major__ > 6
	#define NOUBSAN __attribute__((no_sanitize("undefined")))
#else
	#define NOUBSAN
#endif

#endif

const цел    INT_NULL           =    INT_MIN;
const дол  INT64_NULL         =    INT64_MIN;

constexpr дво DOUBLE_NULL    =    -std::numeric_limits<дво>::infinity();

namespace ДинрусРНЦП {

struct ОпцииПамяти { // sizes are in KB
	цел мастер_блок; // master block size
	цел сис_блок_лимит; // > that this: allocate directly from the system
	цел мастер_резерв; // free master blocks kept in reserve
	цел большой_резерв; // free large blocks kept in reserve
	цел малый_резерв; // free formatted small block pages kept in reserve

	ОпцииПамяти(); // loads default options
	~ОпцииПамяти(); // sets options
};

enum {
	РНЦП_РАСКЛАД_КУЧИ = 16,
	РНЦП_МИНБЛОК_КУЧИ = 32,
};

#ifdef КУЧА_РНЦП

бул MemoryTryRealloc__(ук укз, т_мера& newsize);

struct ПрофильПамяти {
	цел    allocated[1024]; // active small blocks (индекс is size in bytes)
	цел    fragments[1024]; // unallocated small blocks (индекс is size in bytes)
	цел    freepages; // empty 4KB pages (can be recycled)
	цел    large_count; // count of large (~ 1 - 64KB) active blocks
	т_мера large_total; // ^ total size
	цел    large_fragments_count; // count of unused large blocks
	т_мера large_fragments_total; // ^ total size
	цел    large_fragments[2048]; // * 256
	цел    huge_count; // bigger blocks managed by U++ heap (<= 32MB)
	т_мера huge_total; // ^ total size
	цел    huge_fragments_count; // count of unused large blocks
	т_мера huge_fragments_total; // ^ total size
	цел    huge_fragments[65536]; // * 256
	цел    sys_count; // blocks directly allocated from the system (>32MB
	т_мера sys_total; // ^total size
	цел    master_chunks; // master blocks

	ПрофильПамяти();
};

ПрофильПамяти *PeakMemoryProfile();

enum {
	KLASS_8 = 17,
	KLASS_16 = 18,
	KLASS_24 = 19,
	KLASS_32 = 0,
	KLASS_40 = 20,
	KLASS_48 = 21,
	KLASS_56 = 22,
};

inline цел TinyKlass__(цел sz) { // we suppose that this gets resolved at compile time....
	if(sz <= 8) return KLASS_8;
	if(sz <= 16) return KLASS_16;
	if(sz <= 24) return KLASS_24;
	if(sz <= 32) return KLASS_32;
	if(sz <= 40) return KLASS_40;
	if(sz <= 48) return KLASS_48;
	if(sz <= 56) return KLASS_56;
	return -1;
}

ук MemoryAllok__(цел klass);
проц  освободиПамk__(цел klass, ук укз);

#endif

const бцел VOID_V    = 0;

const бцел INT_V     = 1;
const бцел DOUBLE_V  = 2;
const бцел STRING_V  = 3;
const бцел DATE_V    = 4;
const бцел TIME_V    = 5;

const бцел ERROR_V   = 6;

const бцел VALUE_V   = 7;

const бцел WSTRING_V = 8;

const бцел VALUEARRAY_V = 9;

const бцел INT64_V  = 10;
const бцел BOOL_V   = 11;

const бцел VALUEMAP_V   = 12;

const бцел UNKNOWN_V = (бцел)0xffffffff;

class БезКопий {
private:
	БезКопий(const БезКопий&);
	проц operator=(const БезКопий&);
public:
	БезКопий() {}
};

class Обнул {
public:
	operator цел() const                { return INT_NULL; }
	operator дол() const              { return INT64_NULL; }
	operator дво() const             { return DOUBLE_NULL; }
	operator бул() const               { return false; }

	Обнул() {}
};

extern const Обнул Null;

}//end ns