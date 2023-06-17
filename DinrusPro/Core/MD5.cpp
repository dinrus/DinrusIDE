#include <DinrusPro/DinrusCore.h>

#define MD5_CTX РНЦП_МД5_КОНТЕКСТ

/* MD5C.C - RSA Данные Security, Inc., мд5 message-digest algorithm
*/

/* Copyright (C) 1991-2, RSA Данные Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Данные Security, Inc. мд5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this ФУНКЦИЯ.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Данные
Security, Inc. мд5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Данные Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
*/

/* Constants for MD5Transform routine.
*/

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static проц MD5Transform (бцел [4], const ббайт [64]);
static проц Encode (ббайт *, const бцел *, бцел);
static проц Decode (бцел *, const ббайт *, бцел);
static проц MD5_memcpy (проц *, const проц *, бцел);
static проц MD5_memset (проц *, цел, бцел);

static ббайт PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic мд5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
		(a) += F ((b), (c), (d)) + (x) + (бцел)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
#define GG(a, b, c, d, x, s, ac) { \
		(a) += G ((b), (c), (d)) + (x) + (бцел)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
#define HH(a, b, c, d, x, s, ac) { \
		(a) += H ((b), (c), (d)) + (x) + (бцел)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
#define II(a, b, c, d, x, s, ac) { \
		(a) += I ((b), (c), (d)) + (x) + (бцел)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}

/* мд5 initialization. Begins an мд5 operation, writing a new context.
*/
проц MD5Иниt (MD5_CTX *context)
{
	context->count[0] = context->count[1] = 0;
	/* грузи magic initialization constants.
*/
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}

/* мд5 block update operation. Continues an мд5 message-digest
	operation, processing another message block, and updating the
	context.
*/
проц MD5Update (MD5_CTX *context, const ббайт *input, бцел inputLen)
//MD5_CTX *context;                                        /* context */
//ббайт *input;                                /* input block */
//бцел inputLen;                     /* length of input block */
{
	бцел i, индекс, partLen;

	/* Compute number of bytes mod 64 */
	индекс = (бцел)((context->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((context->count[0] += ((бцел)inputLen << 3)) < ((бцел)inputLen << 3))
		context->count[1]++;
	context->count[1] += ((бцел)inputLen >> 29);

	partLen = 64 - индекс;

	/* Transform as many times as possible.
*/
	if (inputLen >= partLen) {
		MD5_memcpy
			((проц *)&context->буфер[индекс], (проц *)input, partLen);
		MD5Transform (context->state, context->буфер);

		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform (context->state, &input[i]);

		индекс = 0;
	}
	else
		i = 0;

	/* Буфер remaining input */
	MD5_memcpy((проц *)&context->буфер[индекс], (проц *)&input[i],
	inputLen-i);
}

/* мд5 finalization. Ends an мд5 message-digest operation, writing the
	the message digest and zeroizing the context.
*/
проц MD5Final (ббайт *digest, MD5_CTX *context)
{
	ббайт bits[8];
	бцел индекс, padLen;

	/* сохрани number of bits */
	Encode (bits, context->count, 8);

	/* Pad out to 56 mod 64.
*/
	индекс = (бцел)((context->count[0] >> 3) & 0x3f);
	padLen = (индекс < 56) ? (56 - индекс) : (120 - индекс);
	MD5Update (context, PADDING, padLen);

	/* приставь length (before padding) */
	MD5Update (context, bits, 8);

	/* сохрани state in digest */
	Encode (digest, context->state, 16);

	/* Zeroize sensitive information.
*/
	MD5_memset ((проц *)context, 0, sizeof (*context));
}

/* мд5 basic transformation. Transforms state based on block.
*/
static проц MD5Transform (бцел state[4], const ббайт block[64])
{
	бцел a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode (x, block, 64);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */

	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information.

*/
	MD5_memset ((проц *)x, 0, sizeof (x));
}

/* Encodes input (бцел) into output (ббайт). Assumes len is
	a multiple of 4.
*/
static проц Encode (ббайт *output, const бцел *input, бцел len)
{
	бцел i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (ббайт)(input[i] & 0xff);
		output[j+1] = (ббайт)((input[i] >> 8) & 0xff);
		output[j+2] = (ббайт)((input[i] >> 16) & 0xff);
		output[j+3] = (ббайт)((input[i] >> 24) & 0xff);
	}
}

/* Decodes input (ббайт) into output (бцел). Assumes len is
	a multiple of 4.
*/
static проц Decode (бцел *output, const ббайт *input, бцел len)
{
	бцел i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((бцел)input[j]) | (((бцел)input[j+1]) << 8) |
		(((бцел)input[j+2]) << 16) | (((бцел)input[j+3]) << 24);
}

/* Note: замени "for loop" with standard memcpy if possible.
*/

static проц MD5_memcpy (ук  output, кук  input, бцел len)
{
	memcpy(output, input, len);
}

/* Note: замени "for loop" with standard memset if possible.
*/
static проц MD5_memset (ук  output, цел значение, бцел len)
{
	memset(output, значение, len);
}

// ------------------ U++ код_ starts here: ----------------------

проц ПотокМд5::выведи(кук данные, бцел size)
{
	MD5Update (&context, (const ббайт *)данные, size);
}

проц ПотокМд5::финиш(ббайт *hash16)
{
	слей();
	MD5Final(hash16, &context);
}

Ткст ПотокМд5::завершиТкст()
{
	ббайт hash[16];
	финиш(hash);
	return гексТкст(hash, 16);
}

Ткст ПотокМд5::завершиПТкст()
{
	ббайт hash[16];
	финиш(hash);
	return гексТкст(hash, 16, 4);
}

проц ПотокМд5::переустанов()
{
	MD5Иниt (&context);
}

ПотокМд5::ПотокМд5()
{
	переустанов();
}

ПотокМд5::~ПотокМд5()
{
	memset(&context, 0, sizeof(context));
}

проц мд5(ббайт *hash16, кук данные, бцел size)
{
	ПотокМд5 md5;
	md5.помести(данные, size);
	md5.финиш(hash16);
}

Ткст мд5Ткст(кук данные, бцел size)
{
	ПотокМд5 md5;
	md5.помести(данные, size);
	return md5.завершиТкст();
}

проц мд5(ббайт *hash16, const Ткст& данные)
{
	return мд5(hash16, ~данные, данные.дайСчёт());
}

Ткст мд5Ткст(const Ткст& данные)
{
	return мд5Ткст(~данные, данные.дайСчёт());
}

Ткст мд5ПТкст(кук данные, бцел size)
{
	ПотокМд5 md5;
	md5.помести(данные, size);
	return md5.завершиПТкст();
}

Ткст мд5ПТкст(const Ткст& данные)
{
	return мд5ПТкст(~данные, данные.дайСчёт());
}

