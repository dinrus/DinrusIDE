#include "Core.h"

namespace РНЦПДинрус {
/*
 * SHA256
 *
 * The author (Brad Conte) has released this file "into the public domain free
 * of any restrictions".  This file is unchanged except for some style
 * clean-up.
 */

// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds
// c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

struct SHA256_CTX
{
   byte  данные[64];
   dword datalen;
   dword bitlen[2];
   dword state[8];
};

dword k[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void sha256_transform(SHA256_CTX *ctx, const byte *данные)
{
	dword a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
	
	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (данные[j] << 24) | (данные[j+1] << 16) | (данные[j+2] << 8) | (данные[j+3]);
	for (; i < 64; ++i)
		m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];
 
	for (i = 0; i < 64; ++i)
	{
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}
 
	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

static void sha256_init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen[0] = 0;
	ctx->bitlen[1] = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

static void sha256_update(SHA256_CTX *ctx, const byte *данные, dword len)
{
	dword i;
 
	for (i=0; i < len; ++i)
	{
		ctx->данные[ctx->datalen] = данные[i];
		ctx->datalen++;
		if (ctx->datalen == 64)
		{
			sha256_transform(ctx,ctx->данные);
			DBL_INT_ADD(ctx->bitlen[0],ctx->bitlen[1],512);
			ctx->datalen = 0;
		}
	}
}

static void sha256_final(SHA256_CTX *ctx, byte *hash)
{
	dword i;
 
	i = ctx->datalen;
 
	// Pad whatever данные is left in the буфер.
	if (ctx->datalen < 56)
	{
		ctx->данные[i++] = 0x80;
		while (i < 56)
			ctx->данные[i++] = 0x00;
	}
	else
	{
		ctx->данные[i++] = 0x80;
		while (i < 64)
			ctx->данные[i++] = 0x00;
		sha256_transform(ctx,ctx->данные);
		memset(ctx->данные,0,56);
	}
 
	// приставь to the padding the total message's length in bits and transform.
	DBL_INT_ADD(ctx->bitlen[0],ctx->bitlen[1],ctx->datalen * 8);
	ctx->данные[63] = byte(ctx->bitlen[0]);
	ctx->данные[62] = byte(ctx->bitlen[0] >> 8);
	ctx->данные[61] = byte(ctx->bitlen[0] >> 16);
	ctx->данные[60] = byte(ctx->bitlen[0] >> 24);
	ctx->данные[59] = byte(ctx->bitlen[1]);
	ctx->данные[58] = byte(ctx->bitlen[1] >> 8);
	ctx->данные[57] = byte(ctx->bitlen[1] >> 16);
	ctx->данные[56] = byte(ctx->bitlen[1] >> 24);
	sha256_transform(ctx,ctx->данные);
 
	// Since this implementation uses little endian byte ordering and SHA uses
	// big endian, reverse all the bytes when copying the final state to the
	// output hash.
	for (i=0; i < 4; ++i)
	{
		hash[i]	= (ctx->state[0] >> (24-i*8)) & 0x000000ff;
		hash[i+4]  = (ctx->state[1] >> (24-i*8)) & 0x000000ff;
		hash[i+8]  = (ctx->state[2] >> (24-i*8)) & 0x000000ff;
		hash[i+12] = (ctx->state[3] >> (24-i*8)) & 0x000000ff;
		hash[i+16] = (ctx->state[4] >> (24-i*8)) & 0x000000ff;
		hash[i+20] = (ctx->state[5] >> (24-i*8)) & 0x000000ff;
		hash[i+24] = (ctx->state[6] >> (24-i*8)) & 0x000000ff;
		hash[i+28] = (ctx->state[7] >> (24-i*8)) & 0x000000ff;
   }
}

void Sha256Stream::Cleanup()
{
	STATIC_ASSERT(sizeof(SHA256_CTX) < 128);

	memset(буфер, 0, sizeof(буфер));
}

void Sha256Stream::выведи(const void *данные, dword length)
{
	sha256_update((SHA256_CTX *)буфер, (const byte *)данные, length);
}

void Sha256Stream::финиш(byte *hash32)
{
	слей();
	sha256_final((SHA256_CTX *)буфер, hash32);
	Cleanup();
}

Ткст Sha256Stream::FinishString()
{
	byte hash[32];
	финиш(hash);
	return гексТкст(hash, 32);
}

Ткст Sha256Stream::FinishStringS()
{
	byte hash[32];
	финиш(hash);
	return гексТкст(hash, 32, 4);
}

void Sha256Stream::переустанов() {
	sha256_init((SHA256_CTX *)буфер);
}

Sha256Stream::Sha256Stream()
{
	переустанов();
}

Sha256Stream::~Sha256Stream()
{
	Cleanup();
}

void SHA256(byte *hash20, const void *данные, dword size)
{
	Sha256Stream sha1;
	sha1.помести(данные, size);
	sha1.финиш(hash20);
}

void SHA256(byte *hash20, const Ткст& s)
{
	return SHA256(hash20, s, s.дайДлину());
}

Ткст SHA256String(const void *данные, dword size)
{
	Sha256Stream sha1;
	sha1.помести(данные, size);
	return sha1.FinishString();
}

Ткст SHA256String(const Ткст& данные)
{
	return SHA256String(~данные, данные.дайДлину());
}

Ткст  SHA256StringS(const void *данные, dword size)
{
	Sha256Stream sha1;
	sha1.помести(данные, size);
	return sha1.FinishStringS();
}

Ткст  SHA256StringS(const Ткст& данные)
{
	return SHA256StringS(~данные, данные.дайДлину());
}

}
