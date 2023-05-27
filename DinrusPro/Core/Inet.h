Ткст WwwFormat(Время tm);
бул   ScanWwwTime(кткст0 s, Время& tm);
Время   ScanWwwTime(кткст0 s);

Ткст MIMECharsetName(ббайт charset);

Ткст UrlEncode(кткст0 s, кткст0 end);
Ткст UrlEncode(кткст0 s, цел len);
Ткст UrlEncode(const Ткст& s);
Ткст UrlDecode(кткст0 s, кткст0 end);
Ткст UrlDecode(кткст0 s, цел len);
Ткст UrlDecode(const Ткст& s);

Ткст QPEncode(const char* s);
Ткст QPDecode(кткст0 s, бул undescore_to_space = false);

Ткст Base64Encode(кткст0 s, кткст0 end);
Ткст Base64Encode(кткст0 s, цел len);
Ткст Base64Encode(const Ткст& данные);
Ткст Base64Decode(кткст0 s, кткст0 end);
Ткст Base64Decode(кткст0 s, цел len);
Ткст Base64Decode(const Ткст& данные);

Ткст DeHtml(кткст0 s);

проц   HMAC_SHA1(const ббайт *text, цел text_len, const ббайт *ключ, цел key_len, ббайт *digest);
Ткст HMAC_SHA1(const Ткст& text, const Ткст& ключ);
Ткст HMAC_SHA1_Hex(const Ткст& text, const Ткст& ключ);

const Индекс<Ткст>& GetMIMETypes();
Ткст FileExtToMIME(const Ткст& ext);
Ткст MIMEToFileExt(const Ткст& mime);

class IpAddrInfo {
	enum { COUNT = 128 };
	struct Entry {
		кткст0 host;
		кткст0 port;
		цел         family;
		цел         status;
		addrinfo   *addr;
	};
	static Entry     пул[COUNT];
	
	enum {
		EMPTY = 0, WORKING, CANCELED, RESOLVED, FAILED
	};

	Ткст host, port;
	цел    family;
	Entry *entry;
	Entry  exe[1];

	static проц EnterPool();
	static проц LeavePool();
	static auxthread_t auxthread__ Нить(ук укз);

	проц старт();
	
	IpAddrInfo(const IpAddrInfo&);

public:
	enum IpAddrFamily {
		FAMILY_ANY = 0, FAMILY_IPV4, FAMILY_IPV6
	};
	проц      старт(const Ткст& host, цел port, цел family = FAMILY_ANY);
	бул      InProgress();
	бул      выполни(const Ткст& host, цел port, цел family = FAMILY_ANY);
	addrinfo *дайРез() const;
	проц      очисть();

	IpAddrInfo();
	~IpAddrInfo()           { очисть(); }
};

struct SSLInfo {
	Ткст  cipher;
	бул    cert_avail;
	бул    cert_verified; // Peer verification not yet working - this is always false
	Ткст  cert_subject;
	Ткст  cert_issuer;
	Дата    cert_notbefore;
	Дата    cert_notafter;
	цел     cert_version;
	Ткст  cert_serial;
};

enum { WAIT_READ = 1, WAIT_WRITE = 2, WAIT_IS_EXCEPTION = 4 };

class TcpSocket : БезКопий {
	enum { BUFFERSIZE = 512 };
	enum { NONE, CONNECT, ACCEPT, SSL_CONNECTED };
	SOCKET                  socket;
	цел                     mode;
	char                    буфер[BUFFERSIZE];
	char                   *укз;
	char                   *end;
	бул                    is_eof;
	бул                    is_error;
	бул                    is_timeout;
	бул                    is_abort;
	бул                    ipv6;

	цел                     timeout;
	цел                     waitstep;
	цел                     done;

	цел                     global_timeout;
	цел                     start_time;
#if defined(PLATFORM_WIN32) || defined(PLATFORM_BSD)
	цел                     connection_start;
#endif
	цел                     ssl_start;

	цел                     errorcode;
	Ткст                  errordesc;
	
	struct SSL {
		virtual бул  старт() = 0;
		virtual бул  жди(бцел flags, цел end_time) = 0;
		virtual цел   Send(const ук буфер, цел maxlen) = 0;
		virtual цел   Recv(ук буфер, цел maxlen) = 0;
		virtual проц  открой() = 0;
		virtual бцел Handshake() = 0;
		
		virtual ~SSL() {}
	};
	
	Один<SSL>                ssl;
	Один<SSLInfo>            sslinfo;
	Ткст                  cert, pkey, sni;
	бул                    asn1;

	struct SSLImp;
	friend struct SSLImp;

	static SSL *(*CreateSSL)(TcpSocket& socket);
	static SSL *CreateSSLImp(TcpSocket& socket);

	friend проц  ИниtCreateSSL();
	friend class IpAddrInfo;

	цел                     GetEndTime() const;
	бул                    RawWait(бцел flags, цел end_time);
	бул                    жди(бцел events, цел end_time);
	SOCKET                  AcceptRaw(бцел *ipaddr, цел timeout_msec);
	бул                    SetupSocket();
	бул                    открой(цел family, цел тип, цел protocol);
	цел                     RawRecv(ук буфер, цел maxlen);
	цел                     Recv(ук буфер, цел maxlen);
	цел                     RawSend(const ук буфер, цел maxlen);
	цел                     Send(const ук буфер, цел maxlen);
	бул                    RawConnect(addrinfo *arp);
	проц                    RawClose();

	проц                    ReadBuffer(цел end_time);
	цел                     Get_();
	цел                     Peek_();
	цел                     Peek_(цел end_time);
	цел                     подбери(цел end_time)          { return укз < end ? (ббайт)*укз : Peek_(end_time); }
	бул                    IsGlobalTimeout();

	проц                    переустанов();

	проц                    SetSockError(кткст0 context, кткст0 errdesc);
	проц                    SetSockError(кткст0 context);

	бул                    WouldBlock();

	static цел              GetErrorCode();
	static проц             иниц();
	
	TcpSocket(const TcpSocket&);

public:
	Событие<>         WhenWait;
	
	проц            SetSockError(кткст0 context, цел код_, кткст0 errdesc);

	enum { ERROR_GLOBAL_TIMEOUT = -1000000, ERROR_SSLHANDSHAKE_TIMEOUT, ERROR_LAST };

	static Ткст   GetHostName();
	
	цел             GetDone() const                          { return done; }

	бул            открыт() const                           { return socket != INVALID_SOCKET; }
	бул            кф_ли() const;

	бул            ошибка_ли() const                          { return is_error; }
	проц            сотриОш()                             { is_error = false; errorcode = 0; errordesc.очисть(); }
	цел             дайОш() const                         { return errorcode; }
	Ткст          GetErrorDesc() const                     { return errordesc; }

	проц            Abort()                                  { is_abort = true; }
	бул            аборт_ли() const                          { return is_abort; }
	проц            ClearAbort()                             { is_abort = false; }
	
	бул            таймаут_ли() const                        { return is_timeout; }
	
	SOCKET          GetSOCKET() const                        { return socket; }
	Ткст          GetPeerAddr() const;

	проц            прикрепи(SOCKET socket);
	бул            Connect(кткст0 host, цел port);
	бул            Connect(IpAddrInfo& инфо);
	бул            WaitConnect();
	бул            Listen(цел port, цел listen_count = 5, бул ipv6 = false, бул reuse = true, ук addr = NULL);
	бул            Listen(const IpAddrInfo& addr, цел port, цел listen_count = 5, бул ipv6 = false, бул reuse = true);
	бул            прими(TcpSocket& listen_socket);
	проц            открой();
	проц            Shutdown();

	проц            NoDelay();
	проц            Linger(цел msecs);
	проц            NoLinger()                               { Linger(Null); }
	
	бул            жди(бцел events);
	бул            WaitRead()                               { return жди(WAIT_READ); }
	бул            WaitWrite()                              { return жди(WAIT_WRITE); }

	цел             подбери()                                   { return укз < end ? (ббайт)*укз : Peek_(); }
	цел             прекрати()                                   { return подбери(); }
	цел             дай()                                    { return укз < end ? (ббайт)*укз++ : Get_(); }
	цел             дай(ук буфер, цел len);
	Ткст          дай(цел len);

	цел             помести(const ук s, цел len);
	цел             помести(const Ткст& s)                     { return помести(s.старт(), s.дайДлину()); }

	бул            дайВсе(ук буфер, цел len);
	Ткст          дайВсе(цел len);
	Ткст          дайСтроку(цел maxlen = 65536);

	бул            PutAll(const ук s, цел len);
	бул            PutAll(const Ткст& s);
	
	бул            StartSSL();
	бул            IsSSL() const                            { return ssl; }
	бцел           SSLHandshake();
	проц            SSLCertificate(const Ткст& cert, const Ткст& pkey, бул asn1);
	проц            SSLServerNameIndication(const Ткст& имя);
	const SSLInfo  *GetSSLInfo() const                       { return ~sslinfo; }
	
	проц            очисть();

	TcpSocket&      Timeout(цел ms)                          { timeout = ms; return *this; }
	цел             GetTimeout() const                       { return timeout; }
	TcpSocket&      GlobalTimeout(цел ms);
	TcpSocket&      NoGlobalTimeout()                        { return GlobalTimeout(Null); }
	TcpSocket&      Blocking()                               { return Timeout(Null); }
	бул            IsBlocking()                             { return пусто_ли(GetTimeout()); }
	TcpSocket&      WaitStep(цел ms)                         { waitstep = ms; return *this; }
	цел             GetWaitStep() const                      { return waitstep; }

	TcpSocket();
	~TcpSocket()                                             { открой(); }
};

class SocketWaitСобытие {
	Вектор<Кортеж<цел, бцел>> socket;
	fd_set read[1], write[1], exception[1];
	SocketWaitСобытие(const SocketWaitСобытие &);

public:
	проц  очисть()                                            { socket.очисть(); }
	проц  добавь(SOCKET s, бцел events)                        { socket.добавь(сделайКортеж((цел)s, events)); }
	проц  добавь(TcpSocket& s, бцел events)                    { добавь(s.GetSOCKET(), events); }
	цел   жди(цел timeout);
	бцел дай(цел i) const;
	бцел operator[](цел i) const                            { return дай(i); }
	
	SocketWaitСобытие();
};

struct UrlInfo {
	Ткст                            url;

	Ткст                            scheme;
	Ткст                            host;
	Ткст                            port;
	Ткст                            username;
	Ткст                            password;
	Ткст                            path;
	Ткст                            query;
	Ткст                            fragment;

	ВекторМап<Ткст, Ткст>         parameters;
	ВекторМап<Ткст, Вектор<Ткст>> array_parameters;

	проц очисть()                      { *this = UrlInfo(); }
	проц Parse(const Ткст& url);

	Ткст operator[](кткст0 ид) const;
	const Вектор<Ткст>& дайМассив(кткст0 ид) const;
	
	UrlInfo() {}
	UrlInfo(const Ткст& url)        { Parse(url); }
};

struct HttpCookie : Движ<HttpCookie> {
	Ткст ид;
	Ткст значение;
	Ткст domain;
	Ткст path;
	Ткст raw;

	проц очисть();
	бул Parse(const Ткст& cookie);
};

struct HttpHeader {
	Ткст                        first_line;
	Ткст                        f1, f2, f3;
	ВекторМап<Ткст, Ткст>     fields;
	ВекторМап<Ткст, HttpCookie> cookies;
	бул                          scgi;
	
	Ткст operator[](кткст0 ид) const                  { return fields.дай(ид, Null); }
	Ткст GetCookie(кткст0 ид) const;

	бул   Response(Ткст& protocol, цел& код_, Ткст& reason) const;
	бул   Request(Ткст& method, Ткст& uri, Ткст& версия) const;
	
	Ткст GetProtocol() const                               { return f1; }
	цел    дайКод() const;
	Ткст GetReason() const                                 { return f3; }
	
	Ткст GetMethod() const                                 { return f1; }
	Ткст GetURI() const                                    { return f2; }
	Ткст дайВерсию() const                                { return f3; }

	бул   HasContentLength() const;
	дол  GetContentLength() const;

	проц   очисть();
	бул   ParseAdd(const Ткст& hdrs);
	бул   Parse(const Ткст& hdrs);
	бул   ParseSCGI(const Ткст& scgi_hdr);

	бул   читай(TcpSocket& socket);
	
	HttpHeader()                                             { scgi = false; }

private:
	проц   добавь(const Ткст& ид, const Ткст& значение);
	HttpHeader(const HttpHeader&);
};

class HttpRequest : public TcpSocket {
	цел          phase;
	бцел        waitevents;
	Ткст       данные;
	дол        count;

	HttpHeader   header;

	Ткст       Ошибка;
	Ткст       body;
	дол        content_length;
	бул         has_content_length;
	бул         chunked_encoding;

	enum {
		DEFAULT_HTTP_PORT        = 80,
		DEFAULT_HTTPS_PORT       = 443
	};

	цел          max_header_size;
	цел          max_content_size;
	цел          max_redirects;
	цел          max_retries;
	цел          timeout;

	Ткст       host;
	цел          port;
	Ткст       proxy_host;
	цел          proxy_port;
	Ткст       proxy_username;
	Ткст       proxy_password;
	Ткст       ssl_proxy_host;
	цел          ssl_proxy_port;
	Ткст       ssl_proxy_username;
	Ткст       ssl_proxy_password;
	Ткст       path;
	Ткст       phost;
	бул         ssl;
	бул         ssl_get_proxy;

	цел          method;
	Ткст       custom_method;
	Ткст       accept;
	Ткст       agent;
	бул         force_digest;
	бул         is_post;
	бул         std_headers;
	бул         hasurlvar;
	бул		 keep_alive;
	бул         all_content;
	Ткст       contenttype;
	Ткст       username;
	Ткст       password;
	Ткст       authorization;
	Ткст       request_headers;
	Ткст       postdata;
	Ткст       multipart;
	ВекторМап<Ткст, HttpCookie> cookies;

	Ткст       protocol;
	цел          status_code;
	Ткст       reason_phrase;
	
	цел          start_time;
	цел          retry_count;
	цел          redirect_count;
	
	цел          chunk;

	IpAddrInfo   addrinfo;
	бул         gzip;
	Zlib         z;

	Поток      *poststream;
	дол        postlen;
	
	Ткст       chunk_crlf;

	проц         иниц();

	проц         StartPhase(цел s);
	проц         старт();
	проц         Dns();
	проц         StartConnect();
	проц         ProcessSSLProxyResponse();
	проц         AfterConnect();
	проц         StartRequest();
	бул         SendingData(бул request = false);

	бул         ReadingHeader();
	бул         ReadingTrailer();
	проц         StartBody();
	бул         ReadingBody();
	проц         ReadingChunkHeader();
	проц         финиш();
	бул         IsRequestTimeout();
	проц         CopyCookies();

	проц         HttpError(кткст0 s);
	проц         выведи(const ук укз, цел size);

	Ткст       CalculateDigest(const Ткст& authenticate) const;

public:
	enum {
		METHOD_GET     = 0,
		METHOD_POST    = 1,
		METHOD_HEAD    = 2,
		METHOD_PUT     = 3,
		METHOD_DELETE  = 4,
		METHOD_TRACE   = 5,
		METHOD_OPTIONS = 6,
		METHOD_CONNECT = 7,
		METHOD_PATCH   = 8,
	};

	Событие<const проц *, цел>  WhenContent;
	Событие<>                   WhenStart;
	Событие<>                   WhenDo;
	Врата<>                    WhenAuthenticate;

	HttpRequest&  MaxHeaderSize(цел m)                   { max_header_size = m; return *this; }
	HttpRequest&  MaxContentSize(цел m)                  { max_content_size = m; return *this; }
	HttpRequest&  MaxRedirect(цел n)                     { max_redirects = n; return *this; }
	HttpRequest&  MaxRetries(цел n)                      { max_retries = n; return *this; }
	HttpRequest&  RequestTimeout(цел ms)                 { timeout = ms; return *this; }
	HttpRequest&  ChunkSize(цел n)                       { chunk = n; return *this; }
	HttpRequest&  AllContent(бул b = true)              { all_content = b; return *this; }

	HttpRequest&  Method(цел m, кткст0 custom_name = NULL);
	HttpRequest&  GET()                                  { return Method(METHOD_GET); }
	HttpRequest&  POST()                                 { return Method(METHOD_POST); }
	HttpRequest&  HEAD()                                 { return Method(METHOD_HEAD); }
	HttpRequest&  PUT()                                  { return Method(METHOD_PUT); }
	HttpRequest&  DEL()                                  { return Method(METHOD_DELETE); }
	HttpRequest&  TRACE()                                { return Method(METHOD_TRACE); }
	HttpRequest&  OPTIONS()                              { return Method(METHOD_OPTIONS); }
	HttpRequest&  CONNECT()                              { return Method(METHOD_CONNECT); }
	HttpRequest&  PATCH()                                { return Method(METHOD_PATCH); }

	HttpRequest&  Хост(const Ткст& h)                  { host = h; return *this; }
	HttpRequest&  Port(цел p)                            { port = p; return *this; }
	HttpRequest&  SSL(бул b = true)                     { ssl = b; return *this; }
	HttpRequest&  Path(const Ткст& p)                  { path = p; return *this; }
	HttpRequest&  Authorization(const Ткст& h)         { authorization = h; return *this; }
	HttpRequest&  User(const Ткст& u, const Ткст& p) { username = u; password = p; return *this; }
	HttpRequest&  Digest()                               { force_digest = true; return *this; }
	HttpRequest&  Digest(const Ткст& u, const Ткст& p) { User(u, p); return Digest(); }
	HttpRequest&  SetDigest(const Ткст& d)             { return Authorization("Digest " + d); }
	HttpRequest&  Url(кткст0 url);
	HttpRequest&  UrlVar(кткст0 ид, const Ткст& данные);
	HttpRequest&  operator()(кткст0 ид, const Ткст& данные) { return UrlVar(ид, данные); }
	HttpRequest&  PostData(const Ткст& pd)              { postdata = pd; poststream = NULL; return *this; }
	HttpRequest&  PostStream(Поток& s, дол len = Null);
	Ткст        GetPostData() const                     { return postdata; }

	бул          ResolveDigestAuthentication();

	HttpRequest&  PostUData(const Ткст& pd)             { return PostData(UrlEncode(pd)); }
	HttpRequest&  пост(const Ткст& данные)                { POST(); return PostData(данные); }
	HttpRequest&  пост(кткст0 ид, const Ткст& данные);
	HttpRequest&  Part(кткст0 ид, const Ткст& данные,
	                   кткст0 content_type = NULL, кткст0 имяф = NULL);

	HttpRequest&  ClearPost()                             { PostData(Null); poststream = NULL; ; multipart.очисть(); GET(); return *this; }

	HttpRequest&  Headers(const Ткст& h)                { request_headers = h; return *this; }
	HttpRequest&  ClearHeaders()                          { return Headers(Null); }
	HttpRequest&  AddHeaders(const Ткст& h)             { request_headers.кат(h); return *this; }
	HttpRequest&  Header(кткст0 ид, const Ткст& данные);
	
	HttpRequest&  Cookie(const HttpCookie& c);
	HttpRequest&  Cookie(const Ткст& ид, const Ткст& значение,
	                     const Ткст& domain = Null, const Ткст& path = Null);
	HttpRequest&  CopyCookies(const HttpRequest& r);
	HttpRequest&  ClearCookies()                          { cookies.очисть(); return *this; }

	HttpRequest&  StdHeaders(бул sh)                     { std_headers = sh; return *this; }
	HttpRequest&  NoStdHeaders()                          { return StdHeaders(false); }
	HttpRequest&  прими(const Ткст& a)                 { accept = a; return *this; }
	HttpRequest&  UserAgent(const Ткст& a)              { agent = a; return *this; }
	HttpRequest&  ContentType(const Ткст& a)            { contenttype = a; return *this; }
	HttpRequest&  KeepAlive(бул ka = true)               { keep_alive = ka; return *this;}

	HttpRequest&  прокси(const Ткст& host, цел port)            { proxy_host = host; proxy_port = port; return *this; }
	HttpRequest&  прокси(кткст0 p);
	HttpRequest&  ProxyAuth(const Ткст& u, const Ткст& p)    { proxy_username = u; proxy_password = p; return *this; }

	HttpRequest&  SSLProxy(const Ткст& host, цел port)         { ssl_proxy_host = host; ssl_proxy_port = port; return *this; }
	HttpRequest&  SSLProxy(кткст0 p);
	HttpRequest&  SSLProxyAuth(const Ткст& u, const Ткст& p) {  ssl_proxy_username = u; ssl_proxy_password = p; return *this; }
	HttpRequest&  SSLProxyGET(бул b = true)                     { ssl_get_proxy = b; return *this; }

	HttpRequest&  CommonProxy(const Ткст& host, цел port)         { прокси(host, port); return SSLProxy(host, port); }
	HttpRequest&  CommonProxy(кткст0 p)                        { прокси(p); return SSLProxy(p); }
	HttpRequest&  CommonProxyAuth(const Ткст& u, const Ткст& p) { ProxyAuth(u, p); return SSLProxyAuth(u, p); }

	бул         IsSocketError() const                    { return TcpSocket::ошибка_ли(); }
	бул         IsHttpError() const                      { return !пусто_ли(Ошибка) ; }
	бул         ошибка_ли() const                          { return IsSocketError() || IsHttpError(); }
	Ткст       GetErrorDesc() const                     { return IsSocketError() ? TcpSocket::GetErrorDesc() : Ошибка; }
	проц         сотриОш()                             { TcpSocket::сотриОш(); Ошибка.очисть(); }

	Ткст       GetHeader(кткст0 ид)                { return header[ид]; }
	Ткст       operator[](кткст0 ид)               { return GetHeader(ид); }
	Ткст       GetRedirectUrl();
	бул         HasContentLength();
	дол        GetContentLength();
	цел          GetStatusCode() const                    { return status_code; }
	Ткст       GetReasonPhrase() const                  { return reason_phrase; }

	const HttpHeader& GetHttpHeader() const               { return header; }
	Ткст       GetCookie(кткст0 ид)                { return header.GetCookie(ид); }

	Ткст       GetContent() const                       { return body; }
	Ткст       operator~() const                        { return GetContent(); }
	operator     Ткст() const                           { return GetContent(); }
	проц         ClearContent()                           { body.очисть(); }

	enum Phase {
		BEGIN, START, DNS,
		SSLPROXYREQUEST, SSLPROXYRESPONSE, SSLHANDSHAKE,
		REQUEST, HEADER, BODY,
		CHUNK_HEADER, CHUNK_BODY, CHUNK_CRLF, TRAILER,
		FINISHED, FAILED,
	};

	бул    делай();
	бцел   GetWaitСобытиеs()                       { return waitevents; }
	цел     GetPhase() const                      { return phase; }
	Ткст  GetPhaseName() const;
	бул    InProgress() const                    { return phase != FAILED && phase != FINISHED; }
	бул    IsFailure() const                     { return phase == FAILED; }
	бул    IsSuccess() const                     { return phase == FINISHED && status_code >= 200 && status_code < 300; }

	Ткст  выполни();

	проц    нов();
	проц    NewRequest();
	проц    очисть();

	HttpRequest();
	HttpRequest(кткст0 url);
	
	static проц  Trace(бул b = true);
	static проц  TraceHeader(бул b = true);
	static проц  TraceBody(бул b = true);
	static проц  TraceShort(бул b = true);
};

бул HttpResponse(TcpSocket& socket, бул scgi, цел код_, кткст0 phrase,
                  кткст0 content_type = NULL, const Ткст& данные = Null,
                  кткст0 server = NULL, бул gzip = false);

#include <DinrusPro/DinrusPro.h>

class WebSocket {
	Ткст     Ошибка;

	TcpSocket  std_socket;
	TcpSocket *socket;
	
	Ткст     uri;
	Ткст     host;
	IpAddrInfo addrinfo;
	бул       ssl;
	Ткст     request_headers;

	Ткст     данные;
	цел        data_pos;

	цел        opcode;
	дол      length;
	бул       mask;
	цел        ключ[4];

	struct Input : Движ<Input> {
		бцел  opcode;
		Ткст данные;
	};
	
	БиВектор<Input>  in_queue;
	
	БиВектор<Ткст> out_queue;
	цел              out_at;
	
	бул             close_sent;
	бул             close_received;
	
	бцел            current_opcode;
	
	бул             client;
	
	цел              redirect = 0;

	enum {
		HTTP_REQUEST_HEADER = -100,
		HTTP_RESPONSE_HEADER = -101,
		READING_FRAME_HEADER = -102,
		DNS = -103,
		SSL_HANDSHAKE = -104,

		FIN = 0x80,
		TEXT = 0x1,
		BINARY = 0x2,
		CLOSE = 0x8,
		PING = 0x9,
		PONG = 0xa,
		
		MASK = 0x80,
	};

	проц очисть();
	проц ошибка(const Ткст& Ошибка);

	проц выведи(const Ткст& s);

	проц Output();

	проц StartConnect();
	проц Dns();
	проц SSLHandshake();
	проц SendRequest();
	бул ReadHttpHeader();
	проц ResponseHeader();
	проц RequestHeader();
	проц FrameHeader();
	проц FrameData();

	цел GetFinIndex() const;

	проц   SendRaw(цел hdr, const Ткст& данные, бцел mask = 0);
	проц   делай0();
	
	static Ткст FormatBlock(const Ткст& s);

public:
	WebSocket& NonBlocking(бул b = true)               { socket->Timeout(b ? 0 : Null); return *this; }

	WebSocket&  Headers(const Ткст& h)                { request_headers = h; return *this; }
	WebSocket&  ClearHeaders()                          { return Headers(Null); }
	WebSocket&  AddHeaders(const Ткст& h)             { request_headers.кат(h); return *this; }
	WebSocket&  Header(кткст0 ид, const Ткст& данные);

	Ткст      GetHeaders()                            { return request_headers; }
	
	бул   IsBlocking() const                           { return пусто_ли(socket->GetTimeout()); }
	
	бул   ошибка_ли() const                              { return socket->ошибка_ли() || Ошибка.дайСчёт(); }
	Ткст дайОш() const                             { return Nvl(socket->GetErrorDesc(), Ошибка); }
	
	бул   прими(TcpSocket& listener_socket);
	бул   Connect(const Ткст& uri, const Ткст& host, бул ssl, цел port);
	бул   Connect(const Ткст& uri, const Ткст& host, бул ssl) { return Connect(uri, host, ssl, ssl ? 440 : 80); }
	бул   Connect(const Ткст& url);
	
	проц   делай();

	Ткст Receive();
	бул   конечен() const                                { return current_opcode & FIN; }
	бул   текст_ли() const                               { return current_opcode & TEXT; }
	бул   IsBinary() const                             { return current_opcode & BINARY; }

	проц   SendText(const Ткст& данные)                 { SendRaw(FIN|TEXT, данные); }
	проц   SendTextMasked(const Ткст& данные)           { SendRaw(FIN|TEXT, данные, MASK); }
	проц   SendBinary(const Ткст& данные)               { SendRaw(FIN|BINARY, данные); }
	проц   Ping(const Ткст& данные)                     { SendRaw(FIN|PING, данные); }

	проц   BeginText(const Ткст& данные)                { SendRaw(TEXT, данные); }
	проц   BeginBinary(const Ткст& данные)              { SendRaw(BINARY, данные); }
	проц   Continue(const Ткст& данные)                 { SendRaw(0, данные); }
	проц   Fin(const Ткст& данные)                      { SendRaw(FIN, данные); }

	проц   открой(const Ткст& msg = Null, бул wait_reply = false);
	бул   открыт() const                               { return socket->открыт(); }
	бул   IsClosed() const                             { return !открыт(); }

	бцел  GetWaitСобытиеs() const                        { return WAIT_READ|(!!out_queue.дайСчёт() * WAIT_WRITE); }
	SOCKET GetSOCKET() const                            { return socket ? socket->GetSOCKET() : INVALID_SOCKET; }
	Ткст GetPeerAddr() const                          { return socket ? socket->GetPeerAddr() : ""; }
	проц   добавьК(SocketWaitСобытие& e)                    { e.добавь(*socket, GetWaitСобытиеs()); }

	static проц Trace(бул b = true);

	WebSocket();

// backward compatibility:
	бул   WebAccept(TcpSocket& socket, HttpHeader& hdr);
	бул   WebAccept(TcpSocket& socket);

	цел    GetOpCode() const { return current_opcode & 15; }

	бул   SendText(const Ткст& данные, бул fin)                   { SendRaw((fin ? 0x80 : 0)|TEXT, данные); return !ошибка_ли(); }
	бул   SendText(const ук данные, цел len, бул fin = true)     { return SendText(Ткст((char *)данные, len), fin); }

	бул   SendBinary(const Ткст& данные, бул fin)                 { SendRaw((fin ? 0x80 : 0)|BINARY, данные); return !ошибка_ли(); }
	бул   SendBinary(const ук данные, цел len, бул fin = true)   { return SendText(Ткст((char *)данные, len), fin); }

	Ткст GetErrorDesc() const                                     { return дайОш(); }

// keep mispeled method names
	Ткст Recieve()    { return Receive(); }
};

проц ParseProxyUrl(кткст0 p, Ткст& proxy_host, цел& proxy_port);
