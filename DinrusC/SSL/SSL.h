#include <Core/Core.h>

#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/engine.h>

namespace РНЦПДинрус {
	
ИНИЦИАЛИЗУЙ(SSL);
ИНИЦИАЛИЗУЙ(SSLSocket);
ИНИЦИАЛИЗУЙ(P7S);

void SslИниtThread();

class SslBuffer
{
public:
	SslBuffer(BUF_MEM *m = NULL) : buf_mem(m) {}
	~SslBuffer()                              { очисть(); }

	bool     пустой() const                  { return !buf_mem; }

	bool     уст(BUF_MEM *b)                  { очисть(); return !!(buf_mem = b); }
	bool     создай()                         { return уст(BUF_MEM_new()); }
	void     очисть()                          { if(buf_mem) { BUF_MEM_free(buf_mem); buf_mem = NULL; } }
	BUF_MEM *открепи()                         { BUF_MEM *b = buf_mem; buf_mem = NULL; return b; }

	bool     Grow(int length);

	Ткст   дай() const;
	bool     уст(const Ткст& d);

	operator BUF_MEM * () const { return buf_mem; }

private:
	BUF_MEM *buf_mem;
};

class SslStream
{
public:
	SslStream(BIO *b = NULL) : bio(b)        {}
	~SslStream()                             { очисть(); }

	bool     пустой() const                 { return !bio; }

	bool     уст(BIO *b)                     { очисть(); return !!(bio = b); }
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	bool     создай(const BIO_METHOD *meth)  { return уст(BIO_new(meth)); }
#else
	bool     создай(BIO_METHOD *meth)        { return уст(BIO_new(meth)); }
#endif
	void     очисть()                         { if(bio) { BIO_free(bio); bio = NULL; } }

	bool     OpenBuffer(const char *данные, int length);
	bool     CreateBuffer();
	Ткст   дайРез() const;

	operator BIO * () const                  { return bio; }

private:
	BIO     *bio;
};

class SslKey
{
public:
	SslKey(EVP_PKEY *k = NULL) : ключ(k) {}
	~SslKey()                           { очисть(); }

	bool      пустой() const            { return !ключ; }

	bool      уст(EVP_PKEY *k)           { очисть(); return !!(ключ = k); }
	void      очисть()                    { if(ключ) { EVP_PKEY_free(ключ); ключ = NULL; } }
	EVP_PKEY *открепи()                   { EVP_PKEY *k = ключ; ключ = NULL; return k; }

	operator  EVP_PKEY * () const        { return ключ; }

	bool      грузи(const Ткст& данные);

private:
	EVP_PKEY *ключ;
};

class SslCertificate
{
public:
	SslCertificate(X509 *c = NULL) : cert(c) {}
	~SslCertificate()                        { очисть(); }

	bool     пустой() const                 { return !cert; }

	bool     уст(X509 *c)                    { очисть(); return !!(cert = c); }
	bool     создай()                        { return уст(X509_new()); }
	void     очисть()                         { if(cert) { X509_free(cert); cert = NULL; } }
	X509    *открепи()                        { X509 *c = cert; cert = NULL; return c; }

	bool     грузи(const Ткст& данные, bool asn1 = false);
	Ткст   сохрани(bool asn1 = false) const;

	Ткст   GetSubjectName() const;
	Ткст   GetIssuerName() const;
	Дата     GetNotBefore() const;
	Дата     GetNotAfter() const;
	int      дайВерсию() const;
	Ткст   GetSerialNumber() const;

	operator X509 * () const                 { return cert; }

private:
	X509    *cert;
};

class SslContext
{
public:
	SslContext(SSL_CTX *c = NULL);
	~SslContext()                              { очисть(); }

	bool     пустой() const                   { return !ssl_ctx; }

	bool     уст(SSL_CTX *c)                   { очисть(); return !!(ssl_ctx = c); }
	bool     создай(SSL_METHOD *meth)          { return уст(SSL_CTX_new(meth)); }
	void     очисть()                           { if(ssl_ctx) { SSL_CTX_free(ssl_ctx); ssl_ctx = NULL; } }
	SSL_CTX *открепи()                          { SSL_CTX *c = ssl_ctx; ssl_ctx = NULL; return c; }

	operator SSL_CTX * () const                { return ssl_ctx; }

	bool     CipherList(const char *list);
	bool     UseCertificate(Ткст certificate, Ткст private_key, bool cert_asn1 = false);
	void     VerifyPeer(bool verify = true, int depth = 2);

private:
	SSL_CTX *ssl_ctx;
};

Ткст SslGetLastError(int& code);
Ткст SslGetLastError();
Ткст SslToString(X509_NAME *имя);
Дата   Asn1ToDate(ASN1_STRING *time);
Ткст Asn1ToString(ASN1_STRING *s);

}
