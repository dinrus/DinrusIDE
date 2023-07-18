#include "SSL.h"

Ткст GetP7Signature_imp(кук данные, цел length, const Ткст& cert_pem, const Ткст& pkey_pem)
{
	SslCertificate x509;
	SslKey pkey;
	if(!x509.грузи(cert_pem))
		return Null;
	if(!pkey.грузи(pkey_pem))
		return Null;

	SslStream in;
	in.OpenBuffer((кткст0 )данные, length);

    PKCS7 *p7 = PKCS7_sign(x509, pkey, NULL, in, PKCS7_DETACHED|PKCS7_BINARY|PKCS7_STREAM);

	if (!p7)
		return Null;

	SslStream out;
	out.CreateBuffer();

	Ткст r;
    if (SMIME_write_PKCS7(out, p7, in, PKCS7_DETACHED|PKCS7_BINARY|PKCS7_STREAM)) {
		SslStream out1;
		out1.CreateBuffer();
		i2d_PKCS7_bio(out1, p7);
		r = out1.дайРез();
    }
  
    PKCS7_free(p7);
    
	return r;
}

extern Ткст (*GetP7Signature__)(кук данные, цел length, const Ткст& cert_pem, const Ткст& pkey_pem);

ИНИЦИАЛИЗАТОР(P7S)
{
	GetP7Signature__ = GetP7Signature_imp;
}

