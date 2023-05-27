#include <DinrusPro/DinrusPro.h>

#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/engine.h>

namespace ДинрусРНЦП {
	
ИНИЦИАЛИЗУЙ(SSL);
ИНИЦИАЛИЗУЙ(SSLSocket);
ИНИЦИАЛИЗУЙ(P7S);

проц SslИниtThread();

class SslBuffer
{
public:
	SslBuffer(BUF_MEM *m = NULL) : buf_mem(m) {}
	~SslBuffer()                              { очисть(); }

	бул     пустой() const                  { return !buf_mem; }

	бул     уст(BUF_MEM *b)                  { очисть(); return !!(buf_mem = b); }
	бул     создай()                         { return уст(BUF_MEM_new()); }
	проц     очисть()                          { if(buf_mem) { BUF_MEM_free(buf_mem); buf_mem = NULL; } }
	BUF_MEM *открепи()                         { BUF_MEM *b = buf_mem; buf_mem = NULL; return b; }

	бул     Grow(цел length);

	Ткст   дай() const;
	бул     уст(const Ткст& d);

	operator BUF_MEM * () const { return buf_mem; }

private:
	BUF_MEM *buf_mem;
};

class SslStream
{
public:
	SslStream(BIO *b = NULL) : bio(b)        {}
	~SslStream()                             { очисть(); }

	бул     пустой() const                 { return !bio; }

	бул     уст(BIO *b)                     { очисть(); return !!(bio = b); }
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	бул     создай(const BIO_METHOD *meth)  { return уст(BIO_new(meth)); }
#else
	бул     создай(BIO_METHOD *meth)        { return уст(BIO_new(meth)); }
#endif
	проц     очисть()                         { if(bio) { BIO_free(bio); bio = NULL; } }

	бул     OpenBuffer(кткст0 данные, цел length);
	бул     CreateBuffer();
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

	бул      пустой() const            { return !ключ; }

	бул      уст(EVP_PKEY *k)           { очисть(); return !!(ключ = k); }
	проц      очисть()                    { if(ключ) { EVP_PKEY_free(ключ); ключ = NULL; } }
	EVP_PKEY *открепи()                   { EVP_PKEY *k = ключ; ключ = NULL; return k; }

	operator  EVP_PKEY * () const        { return ключ; }

	бул      грузи(const Ткст& данные);

private:
	EVP_PKEY *ключ;
};

class SslCertificate
{
public:
	SslCertificate(X509 *c = NULL) : cert(c) {}
	~SslCertificate()                        { очисть(); }

	бул     пустой() const                 { return !cert; }

	бул     уст(X509 *c)                    { очисть(); return !!(cert = c); }
	бул     создай()                        { return уст(X509_new()); }
	проц     очисть()                         { if(cert) { X509_free(cert); cert = NULL; } }
	X509    *открепи()                        { X509 *c = cert; cert = NULL; return c; }

	бул     грузи(const Ткст& данные, бул asn1 = false);
	Ткст   сохрани(бул asn1 = false) const;

	Ткст   GetSubjectName() const;
	Ткст   GetIssuerName() const;
	Дата     GetNotBefore() const;
	Дата     GetNotAfter() const;
	цел      дайВерсию() const;
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

	бул     пустой() const                   { return !ssl_ctx; }

	бул     уст(SSL_CTX *c)                   { очисть(); return !!(ssl_ctx = c); }
	бул     создай(SSL_METHOD *meth)          { return уст(SSL_CTX_new(meth)); }
	проц     очисть()                           { if(ssl_ctx) { SSL_CTX_free(ssl_ctx); ssl_ctx = NULL; } }
	SSL_CTX *открепи()                          { SSL_CTX *c = ssl_ctx; ssl_ctx = NULL; return c; }

	operator SSL_CTX * () const                { return ssl_ctx; }

	бул     CipherList(кткст0 list);
	бул     UseCertificate(Ткст certificate, Ткст private_key, бул cert_asn1 = false);
	проц     VerifyPeer(бул verify = true, цел depth = 2);

private:
	SSL_CTX *ssl_ctx;
};

Ткст SslGetLastError(цел& код_);
Ткст SslGetLastError();
Ткст SslToString(X509_NAME *имя);
Дата   Asn1ToDate(ASN1_STRING *time);
Ткст Asn1ToString(ASN1_STRING *s);

}
