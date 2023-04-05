#if defined(CPU_UNALIGNED) && defined(CPU_LE)
NOUBSAN inline int    Peek16le(const void *ptr)  { return *(const word *)ptr; }
NOUBSAN inline int    Peek32le(const void *ptr)  { return *(const dword *)ptr; }
NOUBSAN inline int64  Peek64le(const void *ptr)  { return *(const int64 *)ptr; }

NOUBSAN inline void   Poke16le(const void *ptr, int val)    { *(word *)ptr = val; }
NOUBSAN inline void   Poke32le(const void *ptr, int val)    { *(dword *)ptr = val; }
NOUBSAN inline void   Poke64le(const void *ptr, int64 val)  { *(int64 *)ptr = val; }
#else
inline int    Peek16le(const void *ptr)  { return MAKEWORD(((byte *)ptr)[0], ((byte *)ptr)[1]); }
inline int    Peek32le(const void *ptr)  { return MAKELONG(Peek16le(ptr), Peek16le((byte *)ptr + 2)); }
inline int64  Peek64le(const void *ptr)  { return MAKEQWORD(Peek32le(ptr), Peek32le((byte *)ptr + 4)); }

inline void   Poke16le(const void *ptr, int val)    { ((byte *)ptr)[0] = LOBYTE(val); ((byte *)ptr)[1] = HIBYTE(val); }
inline void   Poke32le(const void *ptr, int val)    { Poke16le(ptr, LOWORD(val)); Poke16le((byte *)ptr + 2, HIWORD(val)); }
inline void   Poke64le(const void *ptr, int64 val)  { Poke32le(ptr, LODWORD(val)); Poke32le((byte *)ptr + 4, HIDWORD(val)); }
#endif

inline int    Peek16be(const void *ptr)  { return MAKEWORD(((byte *)ptr)[1], ((byte *)ptr)[0]); }
inline int    Peek32be(const void *ptr)  { return MAKELONG(Peek16be((byte *)ptr + 2), Peek16be(ptr)); }
inline int64  Peek64be(const void *ptr)  { return MAKEQWORD(Peek32be((byte *)ptr + 4), Peek32be(ptr)); }

inline void   Poke16be(const void *ptr, int val)    { ((byte *)ptr)[1] = LOBYTE(val); ((byte *)ptr)[0] = HIBYTE(val); }
inline void   Poke32be(const void *ptr, int val)    { Poke16be(ptr, HIWORD(val)); Poke16be((byte *)ptr + 2, LOWORD(val)); }
inline void   Poke64be(const void *ptr, int64 val)  { Poke32be(ptr, HIDWORD(val)); Poke32be((byte *)ptr + 4, LODWORD(val)); }

#ifdef CPU_LITTLE_ENDIAN

#define MAKE2B(b0, b1)                            MAKEWORD(b0, b1)
#define MAKE4B(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b0, b1), MAKEWORD(b2, b3))
#define MAKE8B(b0, b1, b2, b3, b4, b5, b6, b7)    MAKEQWORD(MAKE4B(b0, b1, b2, b3), MAKE4B(b4, b5, b6, b7))

inline int    подбери16(const void *ptr)  { return Peek16le(ptr); }
inline int    подбери32(const void *ptr)  { return Peek32le(ptr); }
inline int64  подбери64(const void *ptr)  { return Peek64le(ptr); }

inline void   Poke16(const void *ptr, int val)    { Poke16le(ptr, val); }
inline void   Poke32(const void *ptr, int val)    { Poke32le(ptr, val); }
inline void   Poke64(const void *ptr, int64 val)  { Poke64le(ptr, val); }

#else

#define MAKE2B(b0, b1)                            MAKEWORD(b1, b0);
#define MAKE4B(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b2, b3), MAKEWORD(b0, b1))
#define MAKE8B(b0, b1, b2, b3, b4, b5, b6, b7)    MAKEQWORD(MAKE4B(b7, b6, b5, b4), MAKE4B(b3, b2, b1, b0))

inline int    подбери16(const void *ptr)  { return Peek16be(ptr); }
inline int    подбери32(const void *ptr)  { return Peek32be(ptr); }
inline int64  подбери64(const void *ptr)  { return Peek64be(ptr); }

inline void   Poke16(const void *ptr, int val)    { Poke16be(ptr, val); }
inline void   Poke32(const void *ptr, int val)    { Poke32be(ptr, val); }
inline void   Poke64(const void *ptr, int64 val)  { Poke64be(ptr, val); }

#endif

#if defined(CPU_X86) && defined(COMPILER_MSC)
#ifdef COMPILER_GCC
#ifdef CPU_64
inline word   эндианРазворот16(word v)    { __asm__("xchgb %b0,%h0" : "=Q" (v) :  "0" (v)); return v; }
inline int16  эндианРазворот16(int16 v)   { __asm__("xchgb %b0,%h0" : "=Q" (v) :  "0" (v)); return v; }
#else
inline word   эндианРазворот16(word v)    { __asm__("xchgb %b0,%h0" : "=q" (v) :  "0" (v)); return v; }
inline int16  эндианРазворот16(int16 v)   { __asm__("xchgb %b0,%h0" : "=q" (v) :  "0" (v)); return v; }
#endif
inline dword  эндианРазворот32(dword v)   { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
inline int    эндианРазворот32(int v)     { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
#endif

#ifdef COMPILER_MSC
#pragma intrinsic (_byteswap_ushort, _byteswap_ulong, _byteswap_uint64, strlen)

inline word   эндианРазворот16(word v)    { return _byteswap_ushort(v); }
inline int16  эндианРазворот16(int16 v)   { return _byteswap_ushort(v); }
inline dword  эндианРазворот32(dword v)   { return _byteswap_ulong(v); }
inline int    эндианРазворот32(int v)     { return _byteswap_ulong(v); }
#endif

inline void   эндианРазворот(word& v)     { v = эндианРазворот16(v); }
inline void   эндианРазворот(int16& v)    { v = эндианРазворот16(v); }
inline void   эндианРазворот(dword& v)    { v = эндианРазворот32(v); }
inline void   эндианРазворот(int& v)      { v = эндианРазворот32(v); }

#else

#ifdef COMPILER_GCC

inline dword  эндианРазворот32(dword v)   { return __builtin_bswap32(v); }
inline int    эндианРазворот32(int v)     { return __builtin_bswap32(v); }

inline word   эндианРазворот16(word v)    { return эндианРазворот32(v) >> 16; } // GCC bug workaround
inline int16  эндианРазворот16(int16 v)   { return эндианРазворот32(v) >> 16; }

inline void   эндианРазворот(word& v)     { v = эндианРазворот16(v); }
inline void   эндианРазворот(int16& v)    { v = эндианРазворот16(v); }
inline void   эндианРазворот(dword& v)    { v = эндианРазворот32(v); }
inline void   эндианРазворот(int& v)      { v = эндианРазворот32(v); }

#else

inline void   эндианРазворот(word& v)     { byte *x = (byte *)(&v); разверни(x[0], x[1]); }
inline void   эндианРазворот(int16& v)    { эндианРазворот(*(word *)&v); }
inline void   эндианРазворот(dword& v)    { byte *x = (byte *)&v; разверни(x[0], x[3]); разверни(x[1], x[2]); }
inline void   эндианРазворот(int& v)      { эндианРазворот(*(dword *)&v); }
inline word   эндианРазворот16(word v)    { эндианРазворот(v); return v; }
inline int16  эндианРазворот16(int16 v)   { эндианРазворот(v); return v; }
inline dword  эндианРазворот32(dword v)   { эндианРазворот(v); return v; }
inline int    эндианРазворот32(int v)     { эндианРазворот(v); return v; }

#endif

#endif

#if defined(CPU_AMD64) && (defined(COMPILER_GCC) || defined(COMPILER_MSC))
#ifdef COMPILER_GCC
inline uint64  SwapEndian64(uint64 v) { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
inline int64   SwapEndian64(int64 v)  { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
#endif
#ifdef COMPILER_MSC
inline uint64  SwapEndian64(uint64 v) { return _byteswap_uint64(v); }
inline int64   SwapEndian64(int64 v)  { return _byteswap_uint64(v); }
#endif

inline void   эндианРазворот(int64& v)    { v = SwapEndian64(v); }
inline void   эндианРазворот(uint64& v)   { v = SwapEndian64(v); }

#else

#ifdef COMPILER_GCC

inline uint64  SwapEndian64(uint64 v) { return __builtin_bswap64(v); }
inline int64   SwapEndian64(int64 v)  { return __builtin_bswap64(v); }

inline void    эндианРазворот(int64& v)   { v = SwapEndian64(v); }
inline void    эндианРазворот(uint64& v)  { v = SwapEndian64(v); }

#else
inline void   эндианРазворот(int64& v)    { byte *x = (byte *)&v; разверни(x[0], x[7]); разверни(x[1], x[6]); разверни(x[2], x[5]); разверни(x[3], x[4]); }
inline void   эндианРазворот(uint64& v)   { эндианРазворот(*(int64 *)&v); }
inline int64  SwapEndian64(int64 v)   { эндианРазворот(v); return v; }
inline uint64 SwapEndian64(uint64 v)  { эндианРазворот(v); return v; }
#endif
#endif

inline word   эндианРазворот16(int w)     { return эндианРазворот16((word)w); }
inline word   эндианРазворот16(dword w)   { return эндианРазворот16((word)w); }

void эндианРазворот(word *v, size_t count);
void эндианРазворот(int16 *v, size_t count);
void эндианРазворот(dword *v, size_t count);
void эндианРазворот(int *v, size_t count);
void эндианРазворот(int64 *v, size_t count);
void эндианРазворот(uint64 *v, size_t count);

#ifdef CPU_64

#define HASH64

#define HASH_CONST1 I64(0xf7c21089bee7c0a5)
#define HASH_CONST2 I64(0xc85abc8da7534a4d)
#define HASH_CONST3 I64(0x8642b0fe3e86671b)

typedef qword hash_t;

inline dword FoldHash(qword h)
{
	return (dword)SwapEndian64(HASH_CONST3 * h);
}

#else

#define HASH_CONST1 0xbee7c0a5
#define HASH_CONST2 0xa7534a4d
#define HASH_CONST3 0x8e86671b

typedef dword hash_t;

inline dword FoldHash(dword h)
{
	return эндианРазворот32(HASH_CONST3 * h);
}

#endif

force_inline
int SignificantBits(dword x)
{ // basically log2(x) + 1 except that for 0 this is 0, number of significant bits of x
#ifdef COMPILER_MSC
	DWORD индекс;
	return _BitScanReverse(&индекс, x) ? индекс + 1 : 0;
#else
	return x ? 32 - __builtin_clz(x) : 0;
#endif
}

force_inline
int SignificantBits64(uint64 x)
{ // basically log2(x) + 1 except that for 0 this is 0, number of significant bits of x
#ifdef COMPILER_MSC
#ifdef CPU_64
	DWORD индекс;
	return _BitScanReverse64(&индекс, x) ? индекс + 1 : 0;
#else
	if(x & 0xffffffff00000000)
		return SignificantBits(HIDWORD(x)) + 32;
	else
		return SignificantBits((DWORD)x);
#endif
#else
	return x ? 64 - __builtin_clzll(x) : 0;
#endif
}

inline bool FitsInInt64(double x)
{
	return x >= -9223372036854775808.0 && x < 9223372036854775808.0;
}

#if defined(__SIZEOF_INT128__) && (__GNUC__ > 5 || __clang_major__ >= 5)

#ifdef CPU_X86

inline
byte addc64(uint64& result, const uint64& значение, byte carry) {
	return _addcarry_u64(carry, result, значение, &result);
}

#else

force_inline
byte addc64(uint64& r, uint64 a, byte carry)
{
	uint64_t r1 = r;
	r += a + carry;
	return carry ? r <= a : r < a;
}

#endif

inline
uint64 mul64(uint64 a, uint64 b, uint64& hi)
{
	unsigned __int128 prod =  (unsigned __int128)a * b;
	hi = prod >> 64;
	return prod;
}

#elif defined(COMPILER_MSC) && defined(CPU_64)

inline
uint64 mul64(uint64 a, uint64 b, uint64& hi)
{
	return _umul128(a, b, &hi);
}

inline
byte addc64(uint64& result, const uint64& значение, byte carry) {
	return _addcarry_u64(carry, result, значение, &result);
}

#else

force_inline
byte addc64(uint64& r, uint64 a, byte carry)
{
	uint64_t r1 = r;
	r += a + carry;
	return carry ? r <= a : r < a;
}

force_inline
uint64 mul64(uint64 a, uint64 b, uint64& hi)
{
	uint64 lo_lo = (a & 0xFFFFFFFF) * (b & 0xFFFFFFFF);
	uint64 hi_lo = (a >> 32)        * (b & 0xFFFFFFFF);
	uint64 lo_hi = (a & 0xFFFFFFFF) * (b >> 32);
	uint64 hi_hi = (a >> 32)        * (b >> 32);
	
	uint64 cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
	hi = (hi_lo >> 32) + (cross >> 32)        + hi_hi;

	return (cross << 32) | (lo_lo & 0xFFFFFFFF);
}

#endif
