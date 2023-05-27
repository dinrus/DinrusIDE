#ifndef _DinrusPro_DinrusPro_h_
#define _DinrusPro_DinrusPro_h_

#ifdef __cplusplus
#include <cstddef>
#include <cstdlib>
#else
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#endif

#include <sys/types.h>

//Базовые типы (примитивы)

	typedef void                проц;
	typedef void*               ук;

	typedef bool                бул;

	typedef  signed char        байт;   ///int8_t, int8
	typedef  unsigned char      ббайт;  ///uint8_t, byte, uint8

	typedef  short              крат;  ///int16_t
	typedef  unsigned short     бкрат; ///uint16_t, word

	typedef  int                цел;  ///int32_t
	typedef  unsigned int       бцел; ///uint32_t
	typedef бкрат               char16;

	typedef  long long int      дол;   ///int64_t
	typedef  unsigned long long бдол;  ///uint64_t

	typedef  size_t             т_мера;

	typedef const char*         кткст0;
	typedef char*               ткст0;
	
	typedef бцел                шим;
	typedef бдол                qword;
	
	typedef double              дво;
	
	
	
struct m128 {
	дол i64[2];
	
	static m128 обнули()             { m128 a; a.i64[0] = a.i64[1] = 0; return a; }
};
	

//Определения и переопределения по-русски
#define ПИ_РНЦП     namespace ДинрусРНЦП {
#define КОНЕЦ_ПИ_РНЦП }
#define РНЦП               ДинрусРНЦП

ПИ_РНЦП
///////////
class Ткст;
class ШТкст;
class Дата;
class Время;
class ТкстБуф;
class ШТкстБуф;
class Поток;
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
//////////////
КОНЕЦ_ПИ_РНЦП

//Включения
#include <DinrusPro/Defs.h>

#ifdef CPU_64

typedef qword               т_хэш;

#define HASH64

#define HASH_CONST1 I64(0xf7c21089bee7c0a5)
#define HASH_CONST2 I64(0xc85abc8da7534a4d)
#define HASH_CONST3 I64(0x8642b0fe3e86671b)

#else

#define HASH_CONST1 0xbee7c0a5
#define HASH_CONST2 0xa7534a4d
#define HASH_CONST3 0x8e86671b

typedef бцел т_хэш;

#endif

#include <DinrusPro/Funcs.h>
#include <DinrusPro/Tpl.h>
#include <DinrusPro/DinrusCore.h>
#include <DinrusPro/Ops.h>

#endif
