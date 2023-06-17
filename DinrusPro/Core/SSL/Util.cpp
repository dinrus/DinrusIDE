#include "SSL.h"

Ткст SslBuffer::дай() const
{
	if(пустой())
		return Ткст::дайПроц();
	return Ткст(buf_mem->data, (цел)buf_mem->length);
}

бул SslBuffer::Grow(цел length)
{
	return !пустой() && BUF_MEM_grow(buf_mem, length);
}

бул SslBuffer::уст(const Ткст& d)
{
	if(!buf_mem && !создай())
		return false;
	цел len = d.дайДлину();
	if((цел)buf_mem->макс < len && !Grow(len))
		return false;
	ПРОВЕРЬ((цел)buf_mem->макс >= len);
	buf_mem->length = len;
	memcpy(buf_mem, d, len);
	return true;
}

бул SslStream::OpenBuffer(кткст0 данные, цел length)
{
	return уст(BIO_new_mem_buf(const_cast<сим *>(данные), length));
}

бул SslStream::CreateBuffer()
{
	очисть();
	SslBuffer buf;
	if(!buf.создай() || !создай(BIO_s_mem()))
		return false;
	BIO_set_mem_buf(bio, buf.открепи(), BIO_CLOSE);
	return true;
}

Ткст SslStream::дайРез() const
{
	if(пустой())
		return Ткст::дайПроц();
	BUF_MEM *bm = NULL;
	BIO_get_mem_ptr(bio, &bm);
	if(!bm)
		return Ткст::дайПроц();
	return Ткст(bm->data, (цел)bm->length);
}

бул SslKey::грузи(const Ткст& данные)
{
	очисть();
	SslStream strm;
	if(!strm.OpenBuffer(данные.старт(), данные.дайДлину()))
		return false;
	return уст(PEM_read_bio_PrivateKey(strm, NULL, NULL, NULL));
}

бул SslCertificate::грузи(const Ткст& данные, бул asn1)
{
	очисть();
	SslStream in, pem, *sio = &in;
	if(!in.OpenBuffer(данные, данные.дайДлину()))
		return false;
	if(!asn1)
	{
		if(!pem.создай(BIO_f_base64()))
			return false;
		BIO_push(pem, in);
		sio = &pem;
	}
	return уст(d2i_X509_bio(*sio, NULL));
}

Ткст SslCertificate::сохрани(бул asn1) const
{
	if(пустой())
		return Ткст::дайПроц();
	SslStream out, pem, *sio = &out;
	if(!out.CreateBuffer())
		return Ткст::дайПроц();
	if(!asn1)
	{
		if(!pem.создай(BIO_f_base64()))
			return Ткст::дайПроц();
		BIO_push(pem, out);
		sio = &pem;
	}
	i2d_X509_bio(*sio, cert);
	return out.дайРез();
}

Ткст SslCertificate::GetSubjectName() const
{
	ПРОВЕРЬ(!пустой());
	return SslToString(X509_get_subject_name(cert));
}

Ткст SslCertificate::GetIssuerName() const
{
	ПРОВЕРЬ(!пустой());
	return SslToString(X509_get_issuer_name(cert));
}

Дата SslCertificate::GetNotBefore() const
{
	ПРОВЕРЬ(!пустой());
	return Asn1ToDate(X509_get_notBefore(cert));
}

Дата SslCertificate::GetNotAfter() const
{
	ПРОВЕРЬ(!пустой());
	return Asn1ToDate(X509_get_notAfter(cert));
}

цел SslCertificate::дайВерсию() const
{
	ПРОВЕРЬ(!пустой());
	return X509_get_version(cert);
}

Ткст SslCertificate::GetSerialNumber() const
{
	ПРОВЕРЬ(!пустой());
	return Asn1ToString(X509_get_serialNumber(cert));
}

SslContext::SslContext(SSL_CTX *c)
: ssl_ctx(c)
{
	SslИниtThread();
}

бул SslContext::CipherList(кткст0 list)
{
	ПРОВЕРЬ(ssl_ctx);
	return SSL_CTX_set_cipher_list(ssl_ctx, list);
}

бул SslContext::UseCertificate(Ткст certdata, Ткст pkeydata, бул cert_asn1)
{
	ПРОВЕРЬ(ssl_ctx);
	if(пусто_ли(certdata) || пусто_ли(pkeydata))
		return false;
	SslCertificate cert;
	SslKey pkey;
	if(!cert.грузи(certdata, cert_asn1) || !pkey.грузи(pkeydata))
		return false;
	if(!SSL_CTX_use_certificate(ssl_ctx, cert) || !SSL_CTX_use_PrivateKey(ssl_ctx, pkey))
		return false;
	if(!SSL_CTX_check_private_key(ssl_ctx))
		return false;
	return true;
}

проц SslContext::VerifyPeer(бул verify, цел depth)
{
	ПРОВЕРЬ(ssl_ctx);
	SSL_CTX_set_verify(ssl_ctx, verify ? SSL_VERIFY_PEER : SSL_VERIFY_NONE, NULL);
	SSL_CTX_set_verify_depth(ssl_ctx, depth);
}

Ткст SslGetLastError(цел& код_)
{
	сим errbuf[150];
	ERR_error_string(код_ = ERR_get_error(), errbuf);
	return errbuf;
}

Ткст SslGetLastError()
{
	цел dummy;
	return SslGetLastError(dummy);
}

Ткст SslToString(X509_NAME *имя)
{
	сим буфер[500];
	return X509_NAME_oneline(имя, буфер, sizeof(буфер));
}

Дата Asn1ToDate(ASN1_STRING *time)
{
	if(!time) return Null;
	цел digit = 0;
	while(digit < time->length && цифра_ли(time->data[digit]))
		digit++;
	if(digit < 6)
		return Null;
	цел year2 = time->data[0] * 10 + time->data[1] - 11 * '0';
	цел month = time->data[2] * 10 + time->data[3] - 11 * '0';
	цел day   = time->data[4] * 10 + time->data[5] - 11 * '0';
	return Дата(year2 + (year2 < 90 ? 2000 : 1900), month, day);
}

Ткст Asn1ToString(ASN1_STRING *s)
{
	return Ткст(s->data, s->length);
}
