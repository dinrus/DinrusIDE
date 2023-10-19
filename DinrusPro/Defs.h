#ifndef _DinrusPro_Defs_h_
#define _DinrusPro_Defs_h_

#ifdef PLATFORM_WIN32
#include "Windows.h"
#include <plugin/z/lib/zlib.h>
#else
#include <zlib.h>
#endif

#if defined(PLATFORM_POSIX) || defined(PLATFORM_LINUX)
#include "Posix.h"
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

проц   неуспехПроверки(кткст0 file, цел line, кткст0 cond);
#define УБЕДИСЬ(x)        ((x) ? (проц)0 : неуспехПроверки(__FILE__, __LINE__, #x))

#ifdef _ОТЛАДКА

#define ПРОВЕРЬ_(x, msg)  ((x) ? (проц)0 : неуспехПроверки(__FILE__, __LINE__, msg))
#define ПРОВЕРЬ(x)        ПРОВЕРЬ_(x, #x)

#else

void LOGNOP__();

#define ЛОГ_НОП          LOGNOP__()

#define ПРОВЕРЬ_(x, msg)  ЛОГ_НОП
#define ПРОВЕРЬ(x)        ЛОГ_НОП

#endif

#define _cm_  ,

#define __количество(a)          цел(sizeof(a) / sizeof(a[0]) )

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
static проц КОМБИНИРУЙ(MK__s, _fn)(); static ИницВызов MK__s(КОМБИНИРУЙ(MK__s, _fn), __FILE__, __LINE__); \
static проц КОМБИНИРУЙ(MK__s, _fn)()

#define ИНИЦБЛОК_(x) \
static проц КОМБИНИРУЙ(x, _fn)(); static ИницВызов x(КОМБИНИРУЙ(x, _fn), __FILE__, __LINE__); \
static проц КОМБИНИРУЙ(x, _fn)()


#define ЭКЗИТБЛОК \
static проц КОМБИНИРУЙ(MK__s, _fn)(); static ВыходИзВызова MK__s(КОМБИНИРУЙ(MK__s, _fn)); \
static проц КОМБИНИРУЙ(MK__s, _fn)()

#define ЭКЗИТБЛОК_(x) \
static проц КОМБИНИРУЙ(x, _fn)(); static ВыходИзВызова x(КОМБИНИРУЙ(x, _fn)); \
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
	#define Ц64(c) ((дол)КОМБИНИРУЙ(c, i64))
#else
	#define Ц64(c) ((дол)КОМБИНИРУЙ(c, LL))
#endif

#define ЦЕЛ64_МИН          Ц64(-0x8000000000000000)
#define ЦЕЛ64_МАКС          Ц64(+0x7FFFFFFFFFFFFFFF)

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

#define СДЕЛАЙБДОЛ(l, h)  ((дим) (((бцел) (l)) | ((дим) ((бцел) (h))) << 32))
#define СДЕЛАЙБЦЕЛ(a)       (бцел)(((бдол)a) >> 32)
#define МЛБЦЕЛ(a)       бцел(a)

#define __смещение(clss, mbr) ((цел)(uintptr_t)&(((clss *)1)->mbr) - 1)

#ifdef CPU_LITTLE_ENDIAN
#define СДЕЛАЙ2Б(b0, b1)                            MAKEWORD(b0, b1)
#define СДЕЛАЙ4Б(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b0, b1), MAKEWORD(b2, b3))
#define СДЕЛАЙ8Б(b0, b1, b2, b3, b4, b5, b6, b7)    СДЕЛАЙБДОЛ(СДЕЛАЙ4Б(b0, b1, b2, b3), СДЕЛАЙ4Б(b4, b5, b6, b7))
#else
#define СДЕЛАЙ2Б(b0, b1)                            MAKEWORD(b1, b0);
#define СДЕЛАЙ4Б(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b2, b3), MAKEWORD(b0, b1))
#define СДЕЛАЙ8Б(b0, b1, b2, b3, b4, b5, b6, b7)    СДЕЛАЙБДОЛ(СДЕЛАЙ4Б(b7, b6, b5, b4), СДЕЛАЙ4Б(b3, b2, b1, b0))
#endif

#define pick_
#define rval_default(T) T(T&&) = default; T& operator=(T&&) = default;

#ifdef COMPILER_MSC
#define форс_инлайн __forceinline
#define не_инлайн __declspec(noinline)
#elif defined(COMPILER_GCC)
#define форс_инлайн __attribute__((always_inline)) inline
#define не_инлайн __attribute__((noinline))
#else
#define форс_инлайн inline
#define не_инлайн
#endif
//#ifdef _ОТЛАДКА
#undef форс_инлайн
#define форс_инлайн inline
//#endif

#define БИНАР(i, f) \
extern "C" ббайт *i; \
extern "C" цел КОМБИНИРУЙ(i, _length);

#define БИНМАССИВ(i, x, f) \
extern "C" ббайт *i[]; \
extern "C" цел КОМБИНИРУЙ(i, _length)[]; \
extern "C" цел КОМБИНИРУЙ(i, _count);

#define БИНМАСКА(i, m) \
extern "C" ббайт *i[]; \
extern "C" цел КОМБИНИРУЙ(i, _length)[]; \
extern "C" цел КОМБИНИРУЙ(i, _count); \
extern "C" сим *КОМБИНИРУЙ(i, _files)[];

// Backward compatibility
/*
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
*/
#if __GNUC__ > 6
	#define НЕСАННЕОПР __attribute__((no_sanitize_undefined))
#elif __clang_major__ > 6
	#define НЕСАННЕОПР __attribute__((no_sanitize("undefined")))
#else
	#define НЕСАННЕОПР
#endif

#endif

const цел    ЦЕЛ_НУЛЛ           =    INT_MIN;
const дол  ЦЕЛ64_НУЛЛ         =    ЦЕЛ64_МИН;

constexpr дво ДВО_НУЛЛ    =    -std::numeric_limits<дво>::infinity();

enum { MAX_DECOMPOSED = 18 };

const сим LOG_BEGIN = '\x1e';
const сим LOG_END = '\x1f';

enum LogOptions {
	LOG_FILE = 1, LOG_COUT = 2, LOG_CERR = 4, LOG_DBG = 8, LOG_SYS = 16, LOG_ELAPSED = 128,
	LOG_TIMESTAMP = 256, LOG_TIMESTAMP_UTC = 512, LOG_APPEND = 1024, LOG_ROTATE_GZIP = 2048,
	LOG_COUTW = 4096, LOG_CERRW = 8192
};

#define EOL "\r\n"

#define STATIC_ASSERT( expr ) { struct __static_assert { unsigned __static_assert_tst:(expr); }; }

#ifdef _ОТЛАДКА

#define LOG(a)           VppLog() << a << EOL
#define LOGF             __LOGF__
#define LOGBEGIN()       VppLog() << LOG_BEGIN
#define LOGEND()         VppLog() << LOG_END
#define LOGBLOCK(n)      RLOGBLOCK(n)
#define LOGHEXDUMP(s, a) гексДамп(VppLog(), s, a)
#define LOGHEX(x)        логГекс(x)
#define QUOTE(a)         { LOG(#a); a; }
#define LOGSRCPOS()      VppLog() << __FILE__ << '#' << __LINE__ << EOL
#define DUMP(a)          VppLog() << #a << " = " << (a) << EOL
#define DUMPC(c)         дампКонтейнер(VppLog() << #c << ':' << EOL, (c))
#define DUMPCC(c)        дампКонтейнер2(VppLog() << #c << ':' << EOL, (c))
#define DUMPCCC(c)       дампКонтейнер3(VppLog() << #c << ':' << EOL, (c))
#define DUMPM(c)         дампМап(VppLog() << #c << ':' << EOL, (c))
#define DUMPHEX(x)       VppLog() << #x << " = ", логГекс(x)
#define XASSERT(c, d)    if(!бул(c)) { LOG("XASSERT не удался"); LOGSRCPOS(); LOG(d); ПРОВЕРЬ(0); } else
#define НИКОГДА()          ПРОВЕРЬ(0)
#define НИКОГДА_(msg)      ПРОВЕРЬ_(0, msg)
#define XНИКОГДА(d)        if(1) { LOG("НИКОГДА не удался"); LOGSRCPOS(); LOG(d); ПРОВЕРЬ(0); } else
#define CHECK(c)         if(!бул(c)) { ПРОВЕРЬ(0); } else
#define XCHECK(c, d)     if(!бул(c)) { LOG("XCHECK не удался"); LOGSRCPOS(); LOG(d); ПРОВЕРЬ(0); } else

#define TIMING(x)        RTIMING(x)
#define HITCOUNT(x)      RHITCOUNT(x)
#define ACTIVATE_TIMING()    ТаймингИнспектор::активируй(true);
#define DEACTIVATE_TIMING()  ТаймингИнспектор::активируй(false);
#define TIMESTOP(x)      RTIMESTOP(x)

#else

inline проц LOGF(кткст0 формат, ...) {}

#define LOG(a)           ЛОГ_НОП
#define LOGBEGIN()       ЛОГ_НОП
#define LOGEND()         ЛОГ_НОП
#define LOGBLOCK(n)      ЛОГ_НОП
#define LOGHEXDUMP(s, a) ЛОГ_НОП
#define LOGHEX(x)        ЛОГ_НОП
#define QUOTE(a)         a
#define LOGSRCPOS()      ЛОГ_НОП
#define DUMP(a)          ЛОГ_НОП
#define DUMPC(a)         ЛОГ_НОП
#define DUMPCC(a)        ЛОГ_НОП
#define DUMPCCC(a)       ЛОГ_НОП
#define DUMPM(a)         ЛОГ_НОП
#define DUMPHEX(nx)      ЛОГ_НОП
#define XASSERT(c, d)    ЛОГ_НОП
#define НИКОГДА()          ЛОГ_НОП
#define НИКОГДА_(msg)      ЛОГ_НОП
#define XНИКОГДА(d)        ЛОГ_НОП
#define CHECK(c)         (проц)(c)
#define XCHECK(c, d)     (проц)(c)

#define TIMING(x)           ЛОГ_НОП
#define HITCOUNT(x)         ЛОГ_НОП
#define ACTIVATE_TIMING()   ЛОГ_НОП
#define DEACTIVATE_TIMING() ЛОГ_НОП

#define TIMESTOP(x)      ЛОГ_НОП

#endif

#define RLOG(a)           VppLog() << a << EOL
#define RLOGBEGIN()       VppLog() << LOG_BEGIN
#define RLOGEND()         VppLog() << LOG_END
#define RLOGBLOCK(n)      DebugLogBlock MK__s(n)
#define RLOGHEXDUMP(s, a) гексДамп(VppLog(), s, a)
#define RQUOTE(a)         { LOG(#a); a; }
#define RLOGSRCPOS()      VppLog() << __FILE__ << '#' << __LINE__ << EOL
#define RDUMP(a)          VppLog() << #a << " = " << (a) << EOL
#define RDUMPC(c)         дампКонтейнер(VppLog() << #c << ':' << EOL, (c))
#define RDUMPM(c)         дампМап(VppLog() << #c << ':' << EOL, (c))
#define RLOGHEX(x)        логГекс(x)
#define RDUMPHEX(x)       VppLog() << #x << " = ", логГекс(x)

#if defined(_ОТЛАДКА) || defined(flagDEBUGCODE)

#define DLOG(x)          RLOG(x)
#define DDUMP(x)         RDUMP(x)
#define DDUMPC(x)        RDUMPC(x)
#define DDUMPM(x)        RDUMPM(x)
#define DTIMING(x)       RTIMING(x)
#define DLOGHEX(x)       RLOGHEX(x)
#define DDUMPHEX(x)      RDUMPHEX(x)
#define DTIMESTOP(x)     RTIMESTOP(x)
#define DHITCOUNT(x)     RHITCOUNT(x)

#define DEBUGCODE(x)     x

#define _DBG_

#else

#define DLOG(x)          @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMP(x)         @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMPC(x)        @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMPM(x)        @ // To clean logs after debugging, this produces Ошибка in release mode
#define DTIMING(x)       @ // To clean logs after debugging, this produces Ошибка in release mode
#define DLOGHEX(x)       @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMPHEX(nx)     @ // To clean logs after debugging, this produces Ошибка in release mode
#define DTIMESTOP(x)     @ // To clean logs after debugging, this produces Ошибка in release mode
#define DHITCOUNT(x)     @ // To clean logs after debugging, this produces Ошибка in release mode

#define DEBUGCODE(x)     ЛОГ_НОП

#endif

// Условное логгирование

#define LOG_(flag, x)       do { if(flag) RLOG(x); } while(false)
#define LOGBEGIN_(flag)     do { if(flag) RLOGBEGIN(x); } while(false)
#define LOGEND_(flag)       do { if(flag) RLOGEND(x); } while(false)
#define DUMP_(flag, x)      do { if(flag) RDUMP(x); } while(false)
#define LOGHEX_(flag, x)    do { if(flag) RLOGHEX(x); } while(false)
#define DUMPHEX_(flag, x)   do { if(flag) RDUMPHEX(x); } while(false)
	
#define USRLOG(x)         LOG_(Ини::user_log, x)

#if 0 // редкие случаи, когда требуютсяn DLOG/DDUMP режима выпуска

#undef  DLOG
#undef  DDUMP
#define DLOG(x)  RLOG(x)
#define DDUMP(x) RDUMP(x)

#endif

enum
{
	FD_SIGN     = 0x01,  // всегда предпоставить знак (+10)
	FD_MINUS0   = 0x02,  // показывать знак отрицательного числа (-0)
	FD_SIGN_EXP = 0x04,  // всегла предпоставлять знак к экспоненте (1e+2)
	FD_CAP_E    = 0x08,  // заглавное E для экспоненты (1E10)
	FD_ZEROS    = 0x10,  // сохранять завершающие нули (1.25000)
	FD_FIX      = 0x20,  // всегда использовать фиксированную нотацию (фмтФ)
	FD_EXP      = 0x40,  // всегда использовать экспоненциальную нотацию (фмтГ)
	FD_COMMA       = 0x80,  // использовать ',' вместо '.'
	FD_NOTHSEPS    = 0x100, // не использовать разделитель тысяч в i18n
	FD_MINIMAL_EXP = 0x1000, // использовать минимальную эксп (1e5 вместо 1e+05)
	FD_SPECIAL     = 0x2000, // выводить nan, inf (если не указан пустой вывод для nan/inf)
	FD_SIGN_SPACE  = 0x4000, // добавлять простанство всесто знака для положительных чисел ( 10)
	FD_POINT       = 0x8000, // всегда добавлять десятичную точку
};

//Определения и переопределения по-русски
#define ПИ_РНЦП     namespace ДинрусРНЦП {
#define КОН_ПИ_РНЦП }
#define РНЦП               ДинрусРНЦП


enum {
	РНЦП_РАСКЛАД_КУЧИ = 16,
	РНЦП_МИНБЛОК_КУЧИ = 32,
};

enum {
	КЛАСС_8 = 17,
	КЛАСС_16 = 18,
	КЛАСС_24 = 19,
	КЛАСС_32 = 0,
	КЛАСС_40 = 20,
	КЛАСС_48 = 21,
	КЛАСС_56 = 22,
};

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

#ifdef PLATFORM_WIN32
#define auxthread_t DWORD
#define auxthread__ WINAPI
#else
#define auxthread_t ук
#define auxthread__
#endif

enum {
	КО_ПАРТИЦ_МИН = 128,
	КО_ПАРТИЦ_МАКС = 1024*1024,
};

#ifdef  _ОТЛАДКА
#define NEWBLOCKSTREAM
#endif

enum {
	STRM_ERROR   =  0x20,
	STRM_READ    =  0x10,
	STRM_WRITE   =  0x08,
	STRM_SEEK    =  0x04,
	STRM_LOADING =  0x02,
	STRM_THROW   =  0x01,

	ERROR_NOT_ENOUGH_SPACE = -1,
	ERROR_LOADING_FAILED   = -2
};

enum {
	ASCSTRING_SMART     = 0x01,
	ASCSTRING_OCTALHI   = 0x02,
	ASCSTRING_JSON      = 0x04,
};

const цел COLOR_V = 39;
const бцел COMPLEX_V   = 20;

//enum EOLenum { EOL };

#ifdef PLATFORM_WIN32
#ifndef _MAX_PATH
static const цел _MAX_PATH = MAX_PATH;
#endif
#endif

#ifdef PLATFORM_POSIX
static const цел _MAX_PATH = PATH_MAX;
бцел  GetTickCount();
#endif

#define LNG_(a, b, c, d)   ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                             (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) )

#define LNGC_(a, b, c, d, cs) ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                                (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) | \
                                ((cs & 255) << 20) )

#define ТЕКЯЗ       0

#define s_(x) txt##x

#define обнулиМассив(x)       memset((x), 0, sizeof(x))

// Constants rounded for 21 decimals.

#ifndef M_E

#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_1_SQRTPI  0.564189583547756286948
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT_2    0.707106781186547524401

#endif

#define M_2PI       (2 * M_PI)

enum {
	НАЙТИВСЕФАЙЛЫ = 1,
	НАЙТИВСЕПАПКИ = 2,
};
// ------