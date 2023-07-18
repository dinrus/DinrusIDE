#include "SSL.h"

#define LLOG(x)  // DLOG(x)

struct TcpSocket::SSLImp : TcpSocket::SSL {
	virtual бул  старт();
	virtual бул  жди(бцел flags, цел end_time);
	virtual цел   Send(кук буфер, цел maxlen);
	virtual цел   Recv(ук буфер, цел maxlen);
	virtual проц  открой();
	virtual бцел Handshake();

	TcpSocket&     socket;
	SslContext     context;
	::SSL         *ssl;
	SslCertificate cert;

	цел            GetErrorCode(цел res);
	Ткст         дайТекстОш(цел код_) const;
	проц           SetSSLError(кткст0 context);
	проц           SetSSLResError(кткст0 context, цел res);
	бул           IsAgain(цел res) const;
	
	SSLImp(TcpSocket& socket) : socket(socket) { ssl = NULL; LLOG("SSLImp(" << socket.GetSOCKET() << ")"); }
	~SSLImp();
};

TcpSocket::SSL *TcpSocket::CreateSSLImp(TcpSocket& socket)
{
	return new TcpSocket::SSLImp(socket);
}

проц ИниtCreateSSL()
{
	TcpSocket::CreateSSL = TcpSocket::CreateSSLImp;
}

ИНИЦИАЛИЗАТОР(SSLSocket) {
	ИниtCreateSSL();
}

TcpSocket::SSLImp::~SSLImp()
{
	if(ssl)
		SSL_free(ssl);
}

проц TcpSocket::SSLImp::SetSSLError(кткст0 context)
{
	цел код_;
	Ткст text = SslGetLastError(код_);
	socket.SetSockError(context, код_, text);
}

кткст0 TcpSocketErrorDesc(цел код_);

цел TcpSocket::SSLImp::GetErrorCode(цел res)
{
	return SSL_get_error(ssl, res);
}

Ткст TcpSocket::SSLImp::дайТекстОш(цел код_) const
{
	Ткст out;
	switch(код_) {
#define SSLERR(c) case c: out = #c; break;
		SSLERR(SSL_ERROR_NONE)
		SSLERR(SSL_ERROR_SSL)
		SSLERR(SSL_ERROR_WANT_READ)
		SSLERR(SSL_ERROR_WANT_WRITE)
		SSLERR(SSL_ERROR_WANT_X509_LOOKUP)
		SSLERR(SSL_ERROR_SYSCALL)
		SSLERR(SSL_ERROR_ZERO_RETURN)
		SSLERR(SSL_ERROR_WANT_CONNECT)
#ifdef PLATFORM_WIN32
		SSLERR(SSL_ERROR_WANT_ACCEPT)
#endif
		default: out = "unknown код_"; break;
	}
	return out;
}

проц TcpSocket::SSLImp::SetSSLResError(кткст0 context, цел res)
{
	цел код_ = GetErrorCode(res);
	if(код_ == SSL_ERROR_SYSCALL) {
		socket.SetSockError(context);
		return;
	}
	Ткст txt = дайТекстОш(код_);
	цел err = ERR_get_error();
	if(err) {
		сим h[260];
		ERR_error_string(err, h);
		txt << "; " << h;
	}
	socket.SetSockError(context, код_, txt);
}

бул TcpSocket::SSLImp::IsAgain(цел res) const
{
	res = SSL_get_error(ssl, res);
	return res == SSL_ERROR_WANT_READ ||
	       res == SSL_ERROR_WANT_WRITE ||
	       res == SSL_ERROR_WANT_CONNECT ||
	       res == SSL_ERROR_WANT_ACCEPT;
}

бул TcpSocket::SSLImp::старт()
{
	LLOG("SSL старт");

#if 0 // bug hunting
	цел n = socket.GetTimeout(); _DBG_
	socket.Timeout(Null);
	socket.жди(WAIT_WRITE);
	socket.Timeout(n);
#endif

	ERR_clear_error();

	if(!context.создай(socket.mode == CONNECT ? const_cast<SSL_METHOD *>(SSLv23_client_method())
	                                          : const_cast<SSL_METHOD *>(SSLv23_server_method()))) {
		SetSSLError("старт: SSL context.");
		return false;
	}
	if(socket.cert.дайСчёт())
		context.UseCertificate(socket.cert, socket.pkey, socket.asn1);
	if(!(ssl = SSL_new(context))) {
		SetSSLError("старт: SSL_new");
		return false;
	}

	if(socket.sni.дайСчёт()) {
		Буфер<сим> h(socket.sni.дайСчёт() + 1);
		strcpy(~h, ~socket.sni);
		SSL_set_tlsext_host_name(ssl, h);
	}

	if(!SSL_set_fd(ssl, (цел)socket.GetSOCKET())) {
		SetSSLError("старт: SSL_set_fd");
		return false;
	}
	return true;
}

бцел TcpSocket::SSLImp::Handshake()
{
	цел res;
	ERR_clear_error();
	if(socket.mode == ACCEPT)
		res = SSL_accept(ssl);
	else
	if(socket.mode == CONNECT)
		res = SSL_connect(ssl);
	else
		return 0;
	if(res <= 0) {
		цел код_ = GetErrorCode(res);
		if(код_ == SSL_ERROR_WANT_READ)
			return WAIT_READ;
		if(код_ == SSL_ERROR_WANT_WRITE)
			return WAIT_WRITE;
	#ifdef PLATFORM_WIN32
		if(код_ == SSL_ERROR_SYSCALL && socket.GetErrorCode() == WSAENOTCONN)
	#else
		if(код_ == SSL_ERROR_SYSCALL && socket.GetErrorCode() == ENOTCONN)
	#endif
			return WAIT_WRITE;
		SetSSLResError("SSL handshake", res);
		return 0;
	}
	socket.mode = SSL_CONNECTED;
	cert.уст(SSL_get_peer_certificate(ssl));
	SSLInfo& f = socket.sslinfo.создай();
	f.cipher = SSL_get_cipher(ssl);
	if(!cert.пустой()) {
		f.cert_avail = true;
		f.cert_subject = cert.GetSubjectName();
		f.cert_issuer = cert.GetIssuerName();
		f.cert_serial = cert.GetSerialNumber();
		f.cert_notbefore = cert.GetNotBefore();
		f.cert_notafter = cert.GetNotAfter();
		f.cert_version = cert.дайВерсию();
		f.cert_verified = SSL_get_verify_result(ssl) == X509_V_OK;
	}
	return 0;
}

бул TcpSocket::SSLImp::жди(бцел flags, цел end_time)
{
	LLOG("SSL жди");
	if((flags & WAIT_READ) && SSL_pending(ssl) > 0)
		return true;
	return socket.RawWait(flags, end_time);
}

цел TcpSocket::SSLImp::Send(кук буфер, цел maxlen)
{
	LLOG("SSL Send " << maxlen);
	ERR_clear_error();
	цел res = SSL_write(ssl, (кткст0 )буфер, maxlen);
	if(res > 0)
		return res;
	if(res == 0)
		socket.is_eof = true;
	else
	if(!IsAgain(res))
		SetSSLResError("SSL_write", res);
	return 0;
}

цел TcpSocket::SSLImp::Recv(ук буфер, цел maxlen)
{
	LLOG("SSL Recv " << maxlen);
	ERR_clear_error();
	цел res = SSL_read(ssl, (сим *)буфер, maxlen);
	if(res > 0)
		return res;
	if(res == 0)
		socket.is_eof = true;
	else
	if(!IsAgain(res))
		SetSSLResError("SSL_read", res);
	return 0;
}

проц TcpSocket::SSLImp::открой()
{
	LLOG("SSL открой");
	SSL_shutdown(ssl);
	socket.RawClose();
	SSL_free(ssl);
	ssl = NULL;
}
