#include "SSL.h"

#define LLOG(x)  // DLOG(x)

namespace РНЦПДинрус {

struct TcpSocket::SSLImp : TcpSocket::SSL {
	virtual bool  старт();
	virtual bool  жди(dword flags, int end_time);
	virtual int   Send(const void *буфер, int maxlen);
	virtual int   Recv(void *буфер, int maxlen);
	virtual void  открой();
	virtual dword Handshake();

	TcpSocket&     socket;
	SslContext     context;
	::SSL         *ssl;
	SslCertificate cert;

	int            GetErrorCode(int res);
	Ткст         дайТекстОш(int code) const;
	void           SetSSLError(const char *context);
	void           SetSSLResError(const char *context, int res);
	bool           IsAgain(int res) const;
	
	SSLImp(TcpSocket& socket) : socket(socket) { ssl = NULL; LLOG("SSLImp(" << socket.GetSOCKET() << ")"); }
	~SSLImp();
};

TcpSocket::SSL *TcpSocket::CreateSSLImp(TcpSocket& socket)
{
	return new TcpSocket::SSLImp(socket);
}

void ИниtCreateSSL()
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

void TcpSocket::SSLImp::SetSSLError(const char *context)
{
	int code;
	Ткст text = SslGetLastError(code);
	socket.SetSockError(context, code, text);
}

const char *TcpSocketErrorDesc(int code);

int TcpSocket::SSLImp::GetErrorCode(int res)
{
	return SSL_get_error(ssl, res);
}

Ткст TcpSocket::SSLImp::дайТекстОш(int code) const
{
	Ткст out;
	switch(code) {
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
		default: out = "unknown code"; break;
	}
	return out;
}

void TcpSocket::SSLImp::SetSSLResError(const char *context, int res)
{
	int code = GetErrorCode(res);
	if(code == SSL_ERROR_SYSCALL) {
		socket.SetSockError(context);
		return;
	}
	Ткст txt = дайТекстОш(code);
	int err = ERR_get_error();
	if(err) {
		char h[260];
		ERR_error_string(err, h);
		txt << "; " << h;
	}
	socket.SetSockError(context, code, txt);
}

bool TcpSocket::SSLImp::IsAgain(int res) const
{
	res = SSL_get_error(ssl, res);
	return res == SSL_ERROR_WANT_READ ||
	       res == SSL_ERROR_WANT_WRITE ||
	       res == SSL_ERROR_WANT_CONNECT ||
	       res == SSL_ERROR_WANT_ACCEPT;
}

bool TcpSocket::SSLImp::старт()
{
	LLOG("SSL старт");

#if 0 // bug hunting
	int n = socket.GetTimeout(); _DBG_
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
		Буфер<char> h(socket.sni.дайСчёт() + 1);
		strcpy(~h, ~socket.sni);
		SSL_set_tlsext_host_name(ssl, h);
	}

	if(!SSL_set_fd(ssl, (int)socket.GetSOCKET())) {
		SetSSLError("старт: SSL_set_fd");
		return false;
	}
	return true;
}

dword TcpSocket::SSLImp::Handshake()
{
	int res;
	ERR_clear_error();
	if(socket.mode == ACCEPT)
		res = SSL_accept(ssl);
	else
	if(socket.mode == CONNECT)
		res = SSL_connect(ssl);
	else
		return 0;
	if(res <= 0) {
		int code = GetErrorCode(res);
		if(code == SSL_ERROR_WANT_READ)
			return WAIT_READ;
		if(code == SSL_ERROR_WANT_WRITE)
			return WAIT_WRITE;
	#ifdef PLATFORM_WIN32
		if(code == SSL_ERROR_SYSCALL && socket.GetErrorCode() == WSAENOTCONN)
	#else
		if(code == SSL_ERROR_SYSCALL && socket.GetErrorCode() == ENOTCONN)
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

bool TcpSocket::SSLImp::жди(dword flags, int end_time)
{
	LLOG("SSL жди");
	if((flags & WAIT_READ) && SSL_pending(ssl) > 0)
		return true;
	return socket.RawWait(flags, end_time);
}

int TcpSocket::SSLImp::Send(const void *буфер, int maxlen)
{
	LLOG("SSL Send " << maxlen);
	ERR_clear_error();
	int res = SSL_write(ssl, (const char *)буфер, maxlen);
	if(res > 0)
		return res;
	if(res == 0)
		socket.is_eof = true;
	else
	if(!IsAgain(res))
		SetSSLResError("SSL_write", res);
	return 0;
}

int TcpSocket::SSLImp::Recv(void *буфер, int maxlen)
{
	LLOG("SSL Recv " << maxlen);
	ERR_clear_error();
	int res = SSL_read(ssl, (char *)буфер, maxlen);
	if(res > 0)
		return res;
	if(res == 0)
		socket.is_eof = true;
	else
	if(!IsAgain(res))
		SetSSLResError("SSL_read", res);
	return 0;
}

void TcpSocket::SSLImp::открой()
{
	LLOG("SSL открой");
	SSL_shutdown(ssl);
	socket.RawClose();
	SSL_free(ssl);
	ssl = NULL;
}

}
