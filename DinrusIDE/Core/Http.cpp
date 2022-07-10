#include "Core.h"

namespace РНЦПДинрус {

namespace Ини {
	INI_BOOL(HttpRequest_Trace, false, "Activates HTTP requests tracing")
	INI_BOOL(HttpRequest_TraceBody, false, "Activates HTTP requests body tracing")
	INI_BOOL(HttpRequest_TraceShort, false, "Activates HTTP requests short tracing")
};

#define LLOG(x)      LOG_(Ини::HttpRequest_Trace, x)
#define LLOGB(x)     LOG_(Ини::HttpRequest_TraceBody, x)
#define LLOGS(x)     LOG_( Ини::HttpRequest_Trace || Ини::HttpRequest_TraceShort, x)
#define LLOGSS(x)    LOG_(!Ини::HttpRequest_Trace && Ини::HttpRequest_TraceShort, x)
	
#ifdef _ОТЛАДКА
_DBG_
// #define ENDZIP // only activate if zip pipe is in the question
#endif

void HttpRequest::Trace(bool b)
{
	Ини::HttpRequest_Trace = b;
	Ини::HttpRequest_TraceBody = b;
}

void HttpRequest::TraceHeader(bool b)
{
	Ини::HttpRequest_Trace = b;
}

void HttpRequest::TraceBody(bool b)
{
	Ини::HttpRequest_TraceBody = b;
}

void HttpRequest::TraceShort(bool b)
{
	Ини::HttpRequest_TraceShort = b;
}

void HttpRequest::иниц()
{
	port = 0;
	proxy_port = 0;
	ssl_proxy_port = 0;
	max_header_size = 1000000;
	max_content_size = 10000000;
	max_redirects = 10;
	max_retries = 3;
	force_digest = false;
	std_headers = true;
	hasurlvar = false;
	keep_alive = false;
	method = METHOD_GET;
	phase = BEGIN;
	redirect_count = 0;
	retry_count = 0;
	gzip = false;
	all_content = false;
	WhenAuthenticate = callback(this, &HttpRequest::ResolveDigestAuthentication);
	chunk = 4096;
	timeout = 120000;
	ssl = false;
	poststream = NULL;
	postlen = Null;
	has_content_length = false;
	content_length = 0;
	chunked_encoding = false;
	waitevents = 0;
	ssl_get_proxy = false;
}

HttpRequest::HttpRequest()
{
	иниц();
}

HttpRequest::HttpRequest(const char *url)
{
	иниц();
	Url(url);
}

HttpRequest& HttpRequest::Method(int m, const char *custom_name)
{
	method = m;
	custom_method = custom_name;
	return *this;
}

HttpRequest& HttpRequest::Url(const char *u)
{
	ssl = memcmp(u, "https", 5) == 0;
	const char *t = u;
	while(*t && *t != '?')
		if(*t++ == '/' && *t == '/') {
			u = ++t;
			break;
		}
	t = u;
	while(*u && *u != ':' && *u != '/' && *u != '?')
		u++;
	hasurlvar = *u == '?' && u[1];
	host = Ткст(t, u);
	port = 0;
	if(*u == ':')
		port = сканЦел(u + 1, &u);
	path = u;
	int q = path.найди('#');
	if(q >= 0)
		path.обрежь(q);
	return *this;
}

void ParseProxyUrl(const char *p, Ткст& proxy_host, int& proxy_port)
{
	const char *t = p;
	while(*p && *p != ':')
		p++;
	proxy_host = Ткст(t, p);
	if(*p++ == ':' && цифра_ли(*p))
		proxy_port = сканЦел(p);
}

HttpRequest& HttpRequest::прокси(const char *url)
{
	proxy_port = 80;
	ParseProxyUrl(url, proxy_host, proxy_port);
	return *this;
}

HttpRequest& HttpRequest::SSLProxy(const char *url)
{
	ssl_proxy_port = 8080;
	ParseProxyUrl(url, ssl_proxy_host, ssl_proxy_port);
	return *this;
}

HttpRequest& HttpRequest::PostStream(Поток& s, int64 len)
{
	POST();
	poststream = &s;
	postlen = Nvl(len, s.GetLeft());
	postdata.очисть();
	return *this;
}

HttpRequest& HttpRequest::пост(const char *ид, const Ткст& данные)
{
	POST();
	if(postdata.дайСчёт())
		postdata << '&';
	postdata << ид << '=' << UrlEncode(данные);
	return *this;
}

HttpRequest& HttpRequest::Part(const char *ид, const Ткст& данные,
                               const char *content_type, const char *filename)
{
	if(пусто_ли(multipart)) {
		POST();
		multipart = какТкст(Uuid::создай());
		ContentType("multipart/form-данные; boundary=" + multipart);
	}
	postdata << "--" << multipart << "\r\n"
	         << "Content-Disposition: form-данные; имя=\"" << ид << "\"";
	if(filename && *filename)
		postdata << "; filename=\"" << filename << "\"";
	postdata << "\r\n";
	if(content_type && *content_type)
		postdata << "Content-Type: " << content_type << "\r\n";
	postdata << "\r\n" << данные << "\r\n";
	return *this;
}

HttpRequest& HttpRequest::UrlVar(const char *ид, const Ткст& данные)
{
	int c = *path.последний();
	if(hasurlvar && c != '&')
		path << '&';
	if(!hasurlvar && c != '?')
		path << '?';
	path << ид << '=' << UrlEncode(данные);
	hasurlvar = true;
	return *this;
}

Ткст HttpRequest::CalculateDigest(const Ткст& authenticate) const
{
	const char *p = authenticate;
	Ткст realm, qop, nonce, opaque;
	while(*p) {
		if(!IsAlNum(*p)) {
			p++;
			continue;
		}
		else {
			const char *b = p;
			while(IsAlNum(*p))
				p++;
			Ткст var = впроп(Ткст(b, p));
			Ткст значение;
			while(*p && (byte)*p <= ' ')
				p++;
			if(*p == '=') {
				p++;
				while(*p && (byte)*p <= ' ')
					p++;
				if(*p == '\"') {
					p++;
					while(*p && *p != '\"')
						if(*p != '\\' || *++p)
							значение.конкат(*p++);
					if(*p == '\"')
						p++;
				}
				else {
					b = p;
					while(*p && *p != ',' && (byte)*p > ' ')
						p++;
					значение = Ткст(b, p);
				}
			}
			if(var == "realm")
				realm = значение;
			else if(var == "qop")
				qop = значение;
			else if(var == "nonce")
				nonce = значение;
			else if(var == "opaque")
				opaque = значение;
		}
	}
	Ткст hv1, hv2;
	hv1 << username << ':' << realm << ':' << password;
	Ткст ha1 = MD5String(hv1);
	hv2 << (method == METHOD_GET ? "GET" : method == METHOD_PUT ? "PUT" : method == METHOD_POST ? "POST" : "READ")
	<< ':' << path;
	Ткст ha2 = MD5String(hv2);
	int nc = 1;
	Ткст cnonce = фмтЦелГекс(случ(), 8);
	Ткст hv;
	hv << ha1
	   << ':' << nonce
	   << ':' << фмтЦелГекс(nc, 8)
	   << ':' << cnonce
	   << ':' << qop << ':' << ha2;
	Ткст ha = MD5String(hv);
	Ткст auth;
	auth << "username=" << какТкстСи(username)
	     << ", realm=" << какТкстСи(realm)
	     << ", nonce=" << какТкстСи(nonce)
	     << ", uri=" << какТкстСи(path)
	     << ", qop=" << какТкстСи(qop)
	     << ", nc=" << какТкстСи(фмтЦелГекс(nc, 8))
	     << ", cnonce=" << cnonce
	     << ", response=" << какТкстСи(ha);
	if(!пусто_ли(opaque))
		auth << ", opaque=" << какТкстСи(opaque);
	return auth;
}

HttpRequest& HttpRequest::Header(const char *ид, const Ткст& данные)
{
	request_headers << ид << ": " << данные << "\r\n";
	return *this;
}

HttpRequest& HttpRequest::Cookie(const HttpCookie& c)
{
	cookies.дайДобавь(Ткст(c.ид).конкат() << '?' << c.domain << '?' << c.path) = c;
	return *this;
}

HttpRequest& HttpRequest::Cookie(const Ткст& ид, const Ткст& значение, const Ткст& domain, const Ткст& path)
{
	HttpCookie c;
	c.ид = ид;
	c.значение = значение;
	c.domain = domain;
	c.path = path;
	return Cookie(c);
}

HttpRequest& HttpRequest::CopyCookies(const HttpRequest& r)
{
	const HttpHeader& h = r.GetHttpHeader();
	for(int i = 0; i < h.cookies.дайСчёт(); i++)
		Cookie(h.cookies[i]);
	return *this;
}

void HttpRequest::HttpError(const char *s)
{
	if(ошибка_ли())
		return;
	Ошибка = фмт(t_("%s:%d: ") + Ткст(s), host, port);
	LLOGS("HTTP Ошибка: " << Ошибка);
	открой();
	phase = FAILED;
}

void HttpRequest::StartPhase(int s)
{
	waitevents = WAIT_READ;
	phase = s;
	LLOG("Starting status " << s << " '" << GetPhaseName() << "', url: " << host);
	данные.очисть();
}

void HttpRequest::нов()
{
	сотриОш();
	ClearAbort();
	waitevents = 0;
	phase = BEGIN;
}

void HttpRequest::NewRequest()
{
	нов();
	иниц();
	host = proxy_host = proxy_username = proxy_password = ssl_proxy_host =
	ssl_proxy_username = ssl_proxy_password = path =
	custom_method = accept = agent = contenttype = username = password =
	authorization = request_headers = postdata = multipart = Null;
}

void HttpRequest::очисть()
{
	TcpSocket::очисть();
	NewRequest();
	cookies.очисть();
}

bool HttpRequest::делай()
{
	switch(phase) {
	case BEGIN:
		retry_count = 0;
		redirect_count = 0;
		start_time = msecs();
		GlobalTimeout(timeout);
	case START:
		старт();
		break;
	case DNS:
		Dns();
		break;
	case SSLPROXYREQUEST:
		if(SendingData())
			break;
		StartPhase(SSLPROXYRESPONSE);
		break;
	case SSLPROXYRESPONSE:
		if(ReadingHeader())
			break;
		ProcessSSLProxyResponse();
		break;
	case SSLHANDSHAKE:
		waitevents = SSLHandshake();
		if(waitevents)
			break;
		StartRequest();
		break;
	case REQUEST:
		if(SendingData(true))
			break;
		StartPhase(HEADER);
		break;
	case HEADER:
		if(ReadingHeader())
			break;
		StartBody();
		break;
	case BODY:
		if(ReadingBody())
			break;
		финиш();
		break;
	case CHUNK_HEADER:
		ReadingChunkHeader();
		break;
	case CHUNK_BODY:
		if(ReadingBody())
			break;
		StartPhase(CHUNK_CRLF);
		break;
	case CHUNK_CRLF:
		if(chunk_crlf.дайСчёт() < 2)
			chunk_crlf.конкат(TcpSocket::дай(2 - chunk_crlf.дайСчёт()));
		if(chunk_crlf.дайСчёт() < 2)
			break;
		if(chunk_crlf != "\r\n")
			HttpError("missing ending CRLF in chunked transfer");
		StartPhase(CHUNK_HEADER);
		break;
	case TRAILER:
		if(ReadingHeader())
			break;
		header.ParseAdd(данные);
		финиш();
		break;
	case FINISHED:
	case FAILED:
		WhenDo();
		return false;
	default:
		NEVER();
	}

	if(phase != FAILED) {
		if(IsSocketError() || ошибка_ли())
			phase = FAILED;
		else
		if(msecs(start_time) >= timeout)
			HttpError("connection timed out");
		else
		if(IsAbort())
			HttpError("connection was aborted");
	}
	
	if(phase == FAILED) {
		if(retry_count++ < max_retries) {
			LLOGS("HTTP retry on Ошибка " << GetErrorDesc());
			start_time = msecs();
			GlobalTimeout(timeout);
			StartPhase(START);
		}
	}
	WhenDo();
	return phase != FINISHED && phase != FAILED;
}

void HttpRequest::старт()
{
	LLOG("HTTP START");
	открой();
	сотриОш();
	gzip = false;
	z.очисть();
	header.очисть();
	status_code = 0;
	reason_phrase.очисть();
	body.очисть();
	WhenStart();

	bool ssl_connect = ssl && !ssl_get_proxy;
	bool use_proxy = !пусто_ли(ssl_connect ? ssl_proxy_host : proxy_host);

	int p = use_proxy ? (ssl_connect ? ssl_proxy_port : proxy_port) : port;
	if(!p)
		p = ssl_connect ? DEFAULT_HTTPS_PORT : DEFAULT_HTTP_PORT;
	phost = use_proxy ? ssl_connect ? ssl_proxy_host : proxy_host : host;
	LLOG("Using " << (use_proxy ? "proxy " : "") << phost << ":" << p);

	SSLServerNameIndication(host);

	StartPhase(DNS);
	if(пусто_ли(GetTimeout()) && timeout == INT_MAX) {
		if(WhenWait) {
			addrinfo.старт(phost, p);
			while(addrinfo.InProgress()) {
				Sleep(GetWaitStep());
				WhenWait();
				if(msecs(start_time) >= timeout)
					break;
			}
		}
		else
			addrinfo.выполни(phost, p);
		StartConnect();
	}
	else
		addrinfo.старт(phost, p);
}

void HttpRequest::Dns()
{
	for(int i = 0; i <= Nvl(GetTimeout(), INT_MAX); i++) {
		if(!addrinfo.InProgress()) {
			StartConnect();
			return;
		}
		Sleep(1);
		if(msecs(start_time) >= timeout)
			break;
	}
}

void HttpRequest::StartConnect()
{
	LLOG("HTTP StartConnect");
	if(!Connect(addrinfo))
		return;
	addrinfo.очисть();
	if(ssl && ssl_proxy_host.дайСчёт() && !ssl_get_proxy) {
		StartPhase(SSLPROXYREQUEST);
		waitevents = WAIT_WRITE;
		Ткст host_port = host;
		if(port)
			host_port << ':' << port;
		else
			host_port << ":443";
		данные << "CONNECT " << host_port << " HTTP/1.1\r\n"
		     << "Хост: " << host_port << "\r\n";
		if(!пусто_ли(ssl_proxy_username))
			данные << "прокси-Authorization: Basic "
			        << Base64Encode(proxy_username + ':' + proxy_password) << "\r\n";
		данные << "\r\n";
		count = 0;
		LLOG("HTTPS proxy request:\n" << данные);
	}
	else
		AfterConnect();
}

void HttpRequest::ProcessSSLProxyResponse()
{
	LLOG("HTTPS proxy response:\n" << данные);
	int q = min(данные.найди('\r'), данные.найди('\n'));
	if(q >= 0)
		данные.обрежь(q);
	if(!данные.начинаетсяС("HTTP") || данные.найди(" 2") < 0) {
		HttpError("Invalid proxy reply: " + данные);
		return;
	}
	AfterConnect();
}

void HttpRequest::AfterConnect()
{
	LLOG("HTTP AfterConnect");
	if(ssl && !ssl_get_proxy && !StartSSL())
		return;
	if(ssl && !ssl_get_proxy)
		StartPhase(SSLHANDSHAKE);
	else
		StartRequest();
}

void HttpRequest::StartRequest()
{
	StartPhase(REQUEST);
	waitevents = WAIT_WRITE;
	count = 0;
	Ткст ctype = contenttype;
	if((method == METHOD_POST || method == METHOD_PUT) && пусто_ли(ctype))
		ctype = "application/x-www-form-urlencoded";
	static const char *smethod[] = {
		"GET", "POST", "HEAD", "PUT", "DELETE", "TRACE", "OPTIONS", "CONNECT", "PATCH",
	};
	ПРОВЕРЬ(method >= 0 && method <= 8);
	данные = Nvl(custom_method, smethod[method]);
	данные << ' ';
	Ткст host_port = host;
	if(port)
		host_port << ':' << port;
	Ткст url;
	url << (ssl && ssl_get_proxy ? "https://" : "http://") << host_port << Nvl(path, "/");
	if(!пусто_ли(proxy_host) && (!ssl || ssl_get_proxy))
		данные << url;
	else {
		if(*path != '/')
			данные << '/';
		данные << path;
	}
	данные << " HTTP/1.1\r\n";
	Ткст pd = postdata;

	if(!пусто_ли(multipart))
		pd << "--" << multipart << "--\r\n";
	if(method == METHOD_GET || method == METHOD_HEAD) {
		pd.очисть();
		poststream = NULL;
	}
	if(std_headers) {
		данные << "URL: " << url << "\r\n"
		     << "Хост: " << (ssl_get_proxy ? phost : host_port) << "\r\n"
		     << "Connection: " << (keep_alive ? "keep-alive\r\n" : "close\r\n")
		     << "прими: " << Nvl(accept, "*/*") << "\r\n"
		     << "прими-Encoding: gzip\r\n"
		     << "User-Agent: " << Nvl(agent, "U++ HTTP request") << "\r\n";
		int64 len = poststream ? postlen : pd.дайСчёт();
		if(len > 0 || method == METHOD_POST || method == METHOD_PUT)
			данные << "Content-длина: " << len << "\r\n";
		if(ctype.дайСчёт())
			данные << "Content-Type: " << ctype << "\r\n";
	}
	ВекторМап<Ткст, Кортеж<Ткст, int> > cms;
	for(int i = 0; i < cookies.дайСчёт(); i++) {
		const HttpCookie& c = cookies[i];
		if(host.заканчиваетсяНа(c.domain) && path.начинаетсяС(c.path)) {
			Кортеж<Ткст, int>& m = cms.дайДобавь(c.ид, сделайКортеж(Ткст(), -1));
			if(c.path.дайДлину() > m.b) {
				m.a = c.значение;
				m.b = c.path.дайДлину();
			}
		}
	}
	Ткст cs;
	for(int i = 0; i < cms.дайСчёт(); i++) {
		if(i)
			cs << "; ";
		cs << cms.дайКлюч(i) << '=' << cms[i].a;
	}
	if(cs.дайСчёт())
		данные << "Cookie: " << cs << "\r\n";
	if(!пусто_ли(proxy_host) && !пусто_ли(proxy_username))
		 данные << "прокси-Authorization: Basic " << Base64Encode(proxy_username + ':' + proxy_password) << "\r\n";
	if(!пусто_ли(authorization))
		данные << "Authorization: " << authorization << "\r\n";
	else
	if(!force_digest && (!пусто_ли(username) || !пусто_ли(password)))
		данные << "Authorization: Basic " << Base64Encode(username + ":" + password) << "\r\n";
	данные << request_headers;
	LLOG("HTTP REQUEST " << host << ":" << port);
	if (pd.дайСчёт() || method == METHOD_POST || method == METHOD_PUT)
	    LLOGSS("HTTP Request " << smethod[method] << " " << url << " данные:" << ctype << "(" << pd.дайСчёт() << ")");
	else
	    LLOGSS("HTTP Request " << smethod[method] << " " << url);
	LLOG("HTTP request:\n" << данные);
	данные << "\r\n" << pd;
	LLOGB("HTTP request body:\n" << pd);
}

bool HttpRequest::SendingData(bool request)
{
	const int upload_chunk =  64*1024;

	if(count < данные.дайДлину())
		for(;;) {
			int n = min(upload_chunk, данные.дайДлину() - (int)count);
			n = TcpSocket::помести(~данные + count, n);
			if(n == 0) {
				if(count < данные.дайДлину())
					return true;
				if(poststream && request)
					break;
				return false;
			}
			count += n;
		}
	if(poststream && request)
		for(;;) {
			Буфер<byte> буфер(upload_chunk);
			int n = poststream->дай(буфер, (int)min((int64)upload_chunk, postlen + данные.дайДлину() - count));
			if(n < 0) {
				HttpError("Ошибка reading input stream");
				return false;
			}
			if(n == 0)
				break;
			n = TcpSocket::помести(буфер, n);
			if(n == 0)
				break;
			count += n;
		}
	return count < данные.дайДлину() + postlen;
}

bool HttpRequest::ReadingHeader()
{
	for(;;) {
		int c = TcpSocket::дай();
		if(c < 0)
			return !кф_ли();
		else
			данные.конкат(c);
		if(данные.дайСчёт() == 2 && данные[0] == '\r' && данные[1] == '\n') // header is empty
			return false;
		if(данные.дайСчёт() >= 3) {
			const char *h = данные.последний();
			if(h[0] == '\n' && h[-1] == '\r' && h[-2] == '\n') // empty ending line after non-empty header
				return false;
		}
		if(данные.дайСчёт() > max_header_size) {
			HttpError("HTTP header exceeded " + какТкст(max_header_size));
			return true;
		}
	}
}

void HttpRequest::ReadingChunkHeader()
{
	for(;;) {
		int c = TcpSocket::дай();
		if(c < 0)
			break;
		else
		if(c == '\n') {
			int n = сканЦел(~данные, NULL, 16);
			LLOG("HTTP Chunk header: 0x" << данные << " = " << n);
			if(пусто_ли(n)) {
				HttpError("invalid chunk header");
				break;
			}
			if(n == 0) {
				StartPhase(TRAILER);
				break;
			}
			count += n;
			StartPhase(CHUNK_BODY);
			chunk_crlf.очисть();
			break;
		}
		if(c != '\r')
			данные.конкат(c);
	}
}

Ткст HttpRequest::GetRedirectUrl()
{
	Ткст redirect_url = обрежьЛево(header["location"]);
	if(redirect_url.начинаетсяС("http://") || redirect_url.начинаетсяС("https://"))
		return redirect_url;
	Ткст h = (ssl ? "https://" : "http://") + host;
	if(*redirect_url != '/')
		h << '/';
	h << redirect_url;
	return h;
}

bool HttpRequest::HasContentLength()
{
	return header.HasContentLength();
}

int64 HttpRequest::GetContentLength()
{
	return header.GetContentLength();
}

void HttpRequest::StartBody()
{
	LLOG("HTTP Header received: ");
	LLOG(данные);
	header.очисть();
	if(!header.Parse(данные)) {
		HttpError("invalid HTTP header");
		return;
	}
	
	if(!header.Response(protocol, status_code, reason_phrase)) {
		HttpError("invalid HTTP response");
		return;
	}
	
	LLOG("HTTP status code: " << status_code);

	content_length = count = GetContentLength();
	has_content_length = HasContentLength();
	
	
	if(method == METHOD_HEAD)
		phase = FINISHED;
	else
	if(header["transfer-encoding"] == "chunked") {
		count = 0;
		chunked_encoding = true;
		StartPhase(CHUNK_HEADER);
	}
	else
		StartPhase(BODY);
	body.очисть();
	gzip = GetHeader("content-encoding") == "gzip";
	if(gzip) {
		gzip = true;
		z.WhenOut = callback(this, &HttpRequest::выведи);
		z.ChunkSize(chunk).GZip().Decompress();
	}
}

void HttpRequest::выведи(const void *ptr, int size)
{
	LLOG("HTTP выведи " << size);
	if(z.ошибка_ли()) {
		HttpError("gzip формат Ошибка");
		return;
	}
	if(body.дайСчёт() + size > max_content_size) {
		HttpError("content length exceeded " + какТкст(max_content_size));
		return;
	}
	if(WhenContent && (status_code >= 200 && status_code < 300 || all_content))
		WhenContent(ptr, size);
	else
		body.конкат((const char *)ptr, size);
}


bool HttpRequest::ReadingBody()
{
	LLOG("HTTP reading body " << count);

	if(has_content_length && content_length == 0)
		return false;

	Ткст s = TcpSocket::дай(has_content_length && content_length > 0 || chunked_encoding ?
	                          (int)min((int64)chunk, count) : chunk);
	if(s.дайСчёт()) {
	#ifndef ENDZIP
		if(gzip)
			z.помести(~s, s.дайСчёт());
		else
	#endif
			выведи(~s, s.дайСчёт());
		if(count > 0) {
			count -= s.дайСчёт();
			return !кф_ли() && count > 0;
		}
	}
	return !кф_ли();
}
/*
bool HttpRequest::ReadingBody()
{
	LLOG("HTTP reading body " << count);
	Ткст s = TcpSocket::дай((int)min((int64)chunk, count));
	if(s.дайСчёт() == 0)
		return !кф_ли() && count;
#ifndef ENDZIP
	if(gzip)
		z.помести(~s, s.дайСчёт());
	else
#endif
		выведи(~s, s.дайСчёт());
	if(count > 0) {
		count -= s.дайСчёт();
		return !кф_ли() && count > 0;
	}
	return !кф_ли();
}
*/

void HttpRequest::CopyCookies()
{
	CopyCookies(*this);
}

bool HttpRequest::ResolveDigestAuthentication()
{
	Ткст authenticate = header["www-authenticate"];
	if(authenticate.начинаетсяС("Digest")) {
		SetDigest(CalculateDigest(authenticate));
		return true;
	}
	return false;
}

void HttpRequest::финиш()
{
	if(gzip) {
	#ifdef ENDZIP
		body = GZDecompress(body);
		if(body.проц_ли()) {
			HttpError("gzip decompress at finish Ошибка");
			return;
		}
	#else
		z.стоп();
		if(z.ошибка_ли()) {
			HttpError("gzip формат Ошибка (finish)");
			return;
		}
	#endif
	}
	CopyCookies();
	if(status_code == 401 && redirect_count++ < max_redirects && WhenAuthenticate()) {
		if(keep_alive)
			StartRequest();
		else
			старт();
		return;
	}
	открой();
	if(status_code >= 300 && status_code < 400) {
		Ткст url = GetRedirectUrl();
		GET();
		if(url.дайСчёт() && redirect_count++ < max_redirects) {
			LLOG("--- HTTP redirect " << url);
			Url(url);
			старт();
			retry_count = 0;
			return;
		}
	}
	phase = FINISHED;
}

Ткст HttpRequest::выполни()
{
	нов();
	while(делай())
		LLOG("HTTP выполни: " << GetPhaseName());
	LLOGSS("HTTP Reply: " << status_code << " " << reason_phrase <<" size:" << GetContent().дайСчёт() << " тип:" << GetHeader("content-тип"));
	return IsSuccess() ? GetContent() : Ткст::дайПроц();
}

Ткст HttpRequest::GetPhaseName() const
{
	static const char *m[] = {
		"Иниtial state",
		"старт",
		"Resolving host имя",
		"SSL proxy request",
		"SSL proxy response",
		"SSL handshake",
		"Sending request",
		"Receiving header",
		"Receiving content",
		"Receiving chunk header",
		"Receiving content chunk",
		"Receiving content chunk ending",
		"Receiving trailer",
		"Request with continue",
		"Waiting for continue header",
		"Finished",
		"Failed",
	};
	return phase >= 0 && phase <= FAILED ? m[phase] : "";
}

}
