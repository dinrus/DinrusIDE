#ifndef _Core_Hash_h_
#define _Core_Hash_h_

/* MD5 context. */
typedef struct {
	dword state[4];                                   /* state (ABCD) */
	dword count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char буфер[64];                         /* input буфер */
} РНЦП_MD5_CTX;

class Md5Stream : public ПотокВывода {
	РНЦП_MD5_CTX context;

	virtual  void  выведи(const void *данные, dword size);

public:
	void   финиш(byte *hash16);
	Ткст FinishString();
	Ткст FinishStringS();
	void   переустанов();
	
	Md5Stream();
	~Md5Stream();
};

void    MD5(byte *hash16, const void *данные, dword size);
void    MD5(byte *hash16, const Ткст& данные);
Ткст  MD5String(const void *данные, dword size);
Ткст  MD5String(const Ткст& данные);
Ткст  MD5StringS(const void *данные, dword size);
Ткст  MD5StringS(const Ткст& данные);

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char буфер[64];
} РНЦП_SHA1_CTX;

class Sha1Stream : public ПотокВывода {
	РНЦП_SHA1_CTX ctx[1];

	virtual  void  выведи(const void *данные, dword size);

	void  Cleanup()                      { memset(ctx, 0, sizeof(ctx)); }

public:
	void   финиш(byte *hash20);
	Ткст FinishString();
	Ткст FinishStringS();

	void   переустанов();
	void   нов()                         { переустанов(); }
	
	Sha1Stream();
	~Sha1Stream();
};

void    SHA1(byte *hash20, const void *данные, dword size);
void    SHA1(byte *hash20, const Ткст& s);
Ткст  SHA1String(const void *данные, dword size);
Ткст  SHA1String(const Ткст& данные);
Ткст  SHA1StringS(const void *данные, dword size);
Ткст  SHA1StringS(const Ткст& данные);

class Sha256Stream : public ПотокВывода {
	byte  буфер[128];

	virtual  void  выведи(const void *данные, dword size);

	void  Cleanup();

public:
	void   финиш(byte *hash32);
	Ткст FinishString();
	Ткст FinishStringS();

	void   переустанов();
	void   нов()                         { переустанов(); }
	
	Sha256Stream(); 
	~Sha256Stream();
};

void    SHA256(byte *hash32, const void *данные, dword size);
void    SHA256(byte *hash32, const Ткст& s);
Ткст  SHA256String(const void *данные, dword size);
Ткст  SHA256String(const Ткст& данные);
Ткст  SHA256StringS(const void *данные, dword size);
Ткст  SHA256StringS(const Ткст& данные);

class ххХэшПоток : public ПотокВывода {
	byte context[8 * 8];
	
	virtual  void  выведи(const void *данные, dword size);

public:
	int финиш();
	
	void переустанов(dword seed = 0);
	
	ххХэшПоток(dword seed = 0);
};

int xxHash(const void *данные, size_t len);
int xxHash(const Ткст& s);

class ххХэш64Поток : public ПотокВывода {
	byte context[12 * 8];
	
	virtual  void  выведи(const void *данные, dword size);

public:
	int64 финиш();
	
	void переустанов(dword seed = 0);
	
	ххХэш64Поток(dword seed = 0);
};

int64 xxHash64(const void *данные, size_t len);
int64 xxHash64(const Ткст& s);

#endif
