#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП{
////////////////////////////

#if defined(CPU_UNALIGNED) && defined(CPU_LE)
NOUBSAN  цел    подбери16лэ(const ук укз)  { return *(const бкрат *)укз; }
NOUBSAN  цел    подбери32лэ(const ук укз)  { return *(const бцел *)укз; }
NOUBSAN  дол  подбери64лэ(const ук укз)  { return *(const дол *)укз; }

NOUBSAN  проц   помести16лэ(const ук укз, цел знач)    { *(бкрат *)укз = знач; }
NOUBSAN  проц   помести32лэ(const ук укз, цел знач)    { *(бцел *)укз = знач; }
NOUBSAN  проц   помести64лэ(const ук укз, дол знач)  { *(дол *)укз = знач; }
#else
 цел    подбери16лэ(const ук укз)  { return MAKEWORD(((ббайт *)укз)[0], ((ббайт *)укз)[1]); }
 цел    подбери32лэ(const ук укз)  { return MAKELONG(подбери16лэ(укз), подбери16лэ((ббайт *)укз + 2)); }
 дол  подбери64лэ(const ук укз)  { return MAKEQWORD(подбери32лэ(укз), подбери32лэ((ббайт *)укз + 4)); }

 проц   помести16лэ(const ук укз, цел знач)    { ((ббайт *)укз)[0] = LOBYTE(знач); ((ббайт *)укз)[1] = HIBYTE(знач); }
 проц   помести32лэ(const ук укз, цел знач)    { помести16лэ(укз, LOWORD(знач)); помести16лэ((ббайт *)укз + 2, HIWORD(знач)); }
 проц   помести64лэ(const ук укз, дол знач)  { помести32лэ(укз, LODWORD(знач)); помести32лэ((ббайт *)укз + 4, HIDWORD(знач)); }
#endif

 цел    подбери16бэ(const ук укз)  { return MAKEWORD(((ббайт *)укз)[1], ((ббайт *)укз)[0]); }
 цел    подбери32бэ(const ук укз)  { return MAKELONG(подбери16бэ((ббайт *)укз + 2), подбери16бэ(укз)); }
 дол  подбери64бэ(const ук укз)  { return MAKEQWORD(подбери32бэ((ббайт *)укз + 4), подбери32бэ(укз)); }

 проц   помести16бэ(const ук укз, цел знач)    { ((ббайт *)укз)[1] = LOBYTE(знач); ((ббайт *)укз)[0] = HIBYTE(знач); }
 проц   помести32бэ(const ук укз, цел знач)    { помести16бэ(укз, HIWORD(знач)); помести16бэ((ббайт *)укз + 2, LOWORD(знач)); }
 проц   помести64бэ(const ук укз, дол знач)  { помести32бэ(укз, HIDWORD(знач)); помести32бэ((ббайт *)укз + 4, LODWORD(знач)); }

#ifdef CPU_LITTLE_ENDIAN

#define MAKE2B(b0, b1)                            MAKEWORD(b0, b1)
#define MAKE4B(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b0, b1), MAKEWORD(b2, b3))
#define MAKE8B(b0, b1, b2, b3, b4, b5, b6, b7)    MAKEQWORD(MAKE4B(b0, b1, b2, b3), MAKE4B(b4, b5, b6, b7))

 цел    подбери16(const ук укз)  { return подбери16лэ(укз); }
 цел    подбери32(const ук укз)  { return подбери32лэ(укз); }
 дол  подбери64(const ук укз)  { return подбери64лэ(укз); }

 проц   помести16(const ук укз, цел знач)    { помести16лэ(укз, знач); }
 проц   помести32(const ук укз, цел знач)    { помести32лэ(укз, знач); }
 проц   помести64(const ук укз, дол знач)  { помести64лэ(укз, знач); }

#else

#define MAKE2B(b0, b1)                            MAKEWORD(b1, b0);
#define MAKE4B(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b2, b3), MAKEWORD(b0, b1))
#define MAKE8B(b0, b1, b2, b3, b4, b5, b6, b7)    MAKEQWORD(MAKE4B(b7, b6, b5, b4), MAKE4B(b3, b2, b1, b0))

 цел    подбери16(const ук укз)  { return подбери16бэ(укз); }
 цел    подбери32(const ук укз)  { return подбери32бэ(укз); }
 дол  подбери64(const ук укз)  { return подбери64бэ(укз); }

 проц   помести16(const ук укз, цел знач)    { помести16бэ(укз, знач); }
 проц   помести32(const ук укз, цел знач)    { помести32бэ(укз, знач); }
 проц   помести64(const ук укз, дол знач)  { помести64бэ(укз, знач); }

#endif

#if defined(CPU_X86) && defined(COMPILER_MSC)
#ifdef COMPILER_GCC
#ifdef CPU_64
 бкрат   эндианРазворот16(бкрат v)    { __asm__("xchgb %b0,%h0" : "=Q" (v) :  "0" (v)); return v; }
 крат  эндианРазворот16(крат v)   { __asm__("xchgb %b0,%h0" : "=Q" (v) :  "0" (v)); return v; }
#else
 бкрат   эндианРазворот16(бкрат v)    { __asm__("xchgb %b0,%h0" : "=q" (v) :  "0" (v)); return v; }
 крат  эндианРазворот16(крат v)   { __asm__("xchgb %b0,%h0" : "=q" (v) :  "0" (v)); return v; }
#endif
 бцел  эндианРазворот32(бцел v)   { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
 цел    эндианРазворот32(цел v)     { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
#endif

#ifdef COMPILER_MSC
#pragma intrinsic (_byteswap_ushort, _byteswap_ulong, _byteswap_uint64, strlen)

 бкрат   эндианРазворот16(бкрат v)    { return _byteswap_ushort(v); }
 крат  эндианРазворот16(крат v)   { return _byteswap_ushort(v); }
 бцел  эндианРазворот32(бцел v)   { return _byteswap_ulong(v); }
 цел    эндианРазворот32(цел v)     { return _byteswap_ulong(v); }
#endif

 проц   эндианРазворот(бкрат& v)     { v = эндианРазворот16(v); }
 проц   эндианРазворот(крат& v)    { v = эндианРазворот16(v); }
 проц   эндианРазворот(бцел& v)    { v = эндианРазворот32(v); }
 проц   эндианРазворот(цел& v)      { v = эндианРазворот32(v); }

#else

#ifdef COMPILER_GCC

 бцел  эндианРазворот32(бцел v)   { return __builtin_bswap32(v); }
 цел    эндианРазворот32(цел v)     { return __builtin_bswap32(v); }

 бкрат   эндианРазворот16(бкрат v)    { return эндианРазворот32(v) >> 16; } // GCC bug workaround
 крат  эндианРазворот16(крат v)   { return эндианРазворот32(v) >> 16; }

 проц   эндианРазворот(бкрат& v)     { v = эндианРазворот16(v); }
 проц   эндианРазворот(крат& v)    { v = эндианРазворот16(v); }
 проц   эндианРазворот(бцел& v)    { v = эндианРазворот32(v); }
 проц   эндианРазворот(цел& v)      { v = эндианРазворот32(v); }

#else

 проц   эндианРазворот(бкрат& v)     { ббайт *x = (ббайт *)(&v); разверни(x[0], x[1]); }
 проц   эндианРазворот(крат& v)    { эндианРазворот(*(бкрат *)&v); }
 проц   эндианРазворот(бцел& v)    { ббайт *x = (ббайт *)&v; разверни(x[0], x[3]); разверни(x[1], x[2]); }
 проц   эндианРазворот(цел& v)      { эндианРазворот(*(бцел *)&v); }
 бкрат   эндианРазворот16(бкрат v)    { эндианРазворот(v); return v; }
 крат  эндианРазворот16(крат v)   { эндианРазворот(v); return v; }
 бцел  эндианРазворот32(бцел v)   { эндианРазворот(v); return v; }
 цел    эндианРазворот32(цел v)     { эндианРазворот(v); return v; }

#endif

#endif

#if defined(CPU_AMD64) && (defined(COMPILER_GCC) || defined(COMPILER_MSC))
#ifdef COMPILER_GCC
 бдол  эндианРазворот64(бдол v) { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
 дол   эндианРазворот64(дол v)  { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
#endif
#ifdef COMPILER_MSC
 бдол  эндианРазворот64(бдол v) { return _byteswap_uint64(v); }
 дол   эндианРазворот64(дол v)  { return _byteswap_uint64(v); }
#endif

 проц   эндианРазворот(дол& v)    { v = эндианРазворот64(v); }
 проц   эндианРазворот(бдол& v)   { v = эндианРазворот64(v); }

#else

#ifdef COMPILER_GCC

 бдол  эндианРазворот64(бдол v) { return __builtin_bswap64(v); }
 дол   эндианРазворот64(дол v)  { return __builtin_bswap64(v); }

 проц    эндианРазворот(дол& v)   { v = эндианРазворот64(v); }
 проц    эндианРазворот(бдол& v)  { v = эндианРазворот64(v); }

#else
 проц   эндианРазворот(дол& v)    { ббайт *x = (ббайт *)&v; разверни(x[0], x[7]); разверни(x[1], x[6]); разверни(x[2], x[5]); разверни(x[3], x[4]); }
 проц   эндианРазворот(бдол& v)   { эндианРазворот(*(дол *)&v); }
 дол  эндианРазворот64(дол v)   { эндианРазворот(v); return v; }
 бдол эндианРазворот64(бдол v)  { эндианРазворот(v); return v; }
#endif
#endif

 бкрат   эндианРазворот16(цел w)     { return эндианРазворот16((бкрат)w); }
 бкрат   эндианРазворот16(бцел w)   { return эндианРазворот16((бкрат)w); }

проц эндианРазворот(бкрат *v, т_мера count);
проц эндианРазворот(крат *v, т_мера count);
проц эндианРазворот(бцел *v, т_мера count);
проц эндианРазворот(цел *v, т_мера count);
проц эндианРазворот(дол *v, т_мера count);
проц эндианРазворот(бдол *v, т_мера count);

#ifdef CPU_64

#define HASH64

#define HASH_CONST1 I64(0xf7c21089bee7c0a5)
#define HASH_CONST2 I64(0xc85abc8da7534a4d)
#define HASH_CONST3 I64(0x8642b0fe3e86671b)

 бцел складиХэш(qword h)
{
	return (бцел)эндианРазворот64(HASH_CONST3 * h);
}

#else

#define HASH_CONST1 0xbee7c0a5
#define HASH_CONST2 0xa7534a4d
#define HASH_CONST3 0x8e86671b

typedef бцел т_хэш;

 бцел складиХэш(бцел h)
{
	return эндианРазворот32(HASH_CONST3 * h);
}

#endif


цел значимыеБиты(бцел x)
{ // basically log2(x) + 1 except that for 0 this is 0, number of significant bits of x
#ifdef COMPILER_MSC
	DWORD индекс;
	return _BitScanReverse(&индекс, x) ? индекс + 1 : 0;
#else
	return x ? 32 - __builtin_clz(x) : 0;
#endif
}


цел значимыеБиты64(бдол x)
{ // basically log2(x) + 1 except that for 0 this is 0, number of significant bits of x
#ifdef COMPILER_MSC
#ifdef CPU_64
	DWORD индекс;
	return _BitScanReverse64(&индекс, x) ? индекс + 1 : 0;
#else
	if(x & 0xffffffff00000000)
		return значимыеБиты(HIDWORD(x)) + 32;
	else
		return значимыеБиты((DWORD)x);
#endif
#else
	return x ? 64 - __builtin_clzll(x) : 0;
#endif
}

 бул вмещаетсяВЦел64(дво x)
{
	return x >= -9223372036854775808.0 && x < 9223372036854775808.0;
}

#if defined(__SIZEOF_INT128__) && (__GNUC__ > 5 || __clang_major__ >= 5)

#ifdef CPU_X86

ббайт сложик64(бдол& result, const бдол& значение, ббайт carry) {
	return _addcarry_u64(carry, result, значение, &result);
}

#else


ббайт сложик64(бдол& r, бдол a, ббайт carry)
{
	uint64_t r1 = r;
	r += a + carry;
	return carry ? r <= a : r < a;
}

#endif


бдол умножь64(бдол a, бдол b, бдол& hi)
{
	unsigned __int128 prod =  (unsigned __int128)a * b;
	hi = prod >> 64;
	return prod;
}

#elif defined(COMPILER_MSC) && defined(CPU_64)


бдол умножь64(бдол a, бдол b, бдол& hi)
{
	return _umul128(a, b, &hi);
}


ббайт сложик64(бдол& result, const бдол& значение, ббайт carry) {
	return _addcarry_u64(carry, result, значение, &result);
}

#else


ббайт сложик64(бдол& r, бдол a, ббайт carry)
{
	uint64_t r1 = r;
	r += a + carry;
	return carry ? r <= a : r < a;
}


бдол умножь64(бдол a, бдол b, бдол& hi)
{
	бдол lo_lo = (a & 0xFFFFFFFF) * (b & 0xFFFFFFFF);
	бдол hi_lo = (a >> 32)        * (b & 0xFFFFFFFF);
	бдол lo_hi = (a & 0xFFFFFFFF) * (b >> 32);
	бдол hi_hi = (a >> 32)        * (b >> 32);
	
	бдол cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
	hi = (hi_lo >> 32) + (cross >> 32)        + hi_hi;

	return (cross << 32) | (lo_lo & 0xFFFFFFFF);
}

#endif

///////////////////////////
} ns end