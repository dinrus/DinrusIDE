#ifndef _Core_Hash_h_
#define _Core_Hash_h_

/* MD5 context. */
typedef struct {
	бцел state[4];                                   /* state (ABCD) */
	бцел count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char буфер[64];                         /* input буфер */
} РНЦП_MD5_CTX;

class Md5Stream : public ПотокВывода {
	РНЦП_MD5_CTX context;

	virtual  проц  выведи(const ук данные, бцел size);

public:
	проц   финиш(ббайт *hash16);
	Ткст FinishString();
	Ткст FinishStringS();
	проц   переустанов();
	
	Md5Stream();
	~Md5Stream();
};

проц    MD5(ббайт *hash16, const ук данные, бцел size);
проц    MD5(ббайт *hash16, const Ткст& данные);
Ткст  MD5String(const ук данные, бцел size);
Ткст  MD5String(const Ткст& данные);
Ткст  MD5StringS(const ук данные, бцел size);
Ткст  MD5StringS(const Ткст& данные);

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char буфер[64];
} РНЦП_SHA1_CTX;

class Sha1Stream : public ПотокВывода {
	РНЦП_SHA1_CTX ctx[1];

	virtual  проц  выведи(const ук данные, бцел size);

	проц  Cleanup()                      { memset(ctx, 0, sizeof(ctx)); }

public:
	проц   финиш(ббайт *hash20);
	Ткст FinishString();
	Ткст FinishStringS();

	проц   переустанов();
	проц   нов()                         { переустанов(); }
	
	Sha1Stream();
	~Sha1Stream();
};

проц    SHA1(ббайт *hash20, const ук данные, бцел size);
проц    SHA1(ббайт *hash20, const Ткст& s);
Ткст  SHA1String(const ук данные, бцел size);
Ткст  SHA1String(const Ткст& данные);
Ткст  SHA1StringS(const ук данные, бцел size);
Ткст  SHA1StringS(const Ткст& данные);

class Sha256Stream : public ПотокВывода {
	ббайт  буфер[128];

	virtual  проц  выведи(const ук данные, бцел size);

	проц  Cleanup();

public:
	проц   финиш(ббайт *hash32);
	Ткст FinishString();
	Ткст FinishStringS();

	проц   переустанов();
	проц   нов()                         { переустанов(); }
	
	Sha256Stream(); 
	~Sha256Stream();
};

проц    SHA256(ббайт *hash32, const ук данные, бцел size);
проц    SHA256(ббайт *hash32, const Ткст& s);
Ткст  SHA256String(const ук данные, бцел size);
Ткст  SHA256String(const Ткст& данные);
Ткст  SHA256StringS(const ук данные, бцел size);
Ткст  SHA256StringS(const Ткст& данные);

class ххХэшПоток : public ПотокВывода {
	ббайт context[8 * 8];
	
	virtual  проц  выведи(const ук данные, бцел size);

public:
	цел финиш();
	
	проц переустанов(бцел seed = 0);
	
	ххХэшПоток(бцел seed = 0);
};

цел xxHash(const ук данные, т_мера len);
цел xxHash(const Ткст& s);

class ххХэш64Поток : public ПотокВывода {
	ббайт context[12 * 8];
	
	virtual  проц  выведи(const ук данные, бцел size);

public:
	дол финиш();
	
	проц переустанов(бцел seed = 0);
	
	ххХэш64Поток(бцел seed = 0);
};

дол xxHash64(const ук данные, т_мера len);
дол xxHash64(const Ткст& s);

#endif
