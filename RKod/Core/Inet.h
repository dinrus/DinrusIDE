Ткст WwwFormat(Время tm);
bool   ScanWwwTime(const char *s, Время& tm);
Время   ScanWwwTime(const char *s);

Ткст MIMECharsetName(byte charset);

Ткст UrlEncode(const char *s, const char *end);
Ткст UrlEncode(const char *s, int len);
Ткст UrlEncode(const Ткст& s);
Ткст UrlDecode(const char *s, const char *end);
Ткст UrlDecode(const char *s, int len);
Ткст UrlDecode(const Ткст& s);

Ткст QPEncode(const char* s);
Ткст QPDecode(const char *s, bool undescore_to_space = false);

Ткст Base64Encode(const char *s, const char *end);
Ткст Base64Encode(const char *s, int len);
Ткст Base64Encode(const Ткст& данные);
Ткст Base64Decode(const char *s, const char *end);
Ткст Base64Decode(const char *s, int len);
Ткст Base64Decode(const Ткст& данные);

Ткст DeHtml(const char *s);

void   HMAC_SHA1(const byte *text, int text_len, const byte *ключ, int key_len, byte *digest);
Ткст HMAC_SHA1(const Ткст& text, const Ткст& ключ);
Ткст HMAC_SHA1_Hex(const Ткст& text, const Ткст& ключ);

const Индекс<Ткст>& GetMIMETypes();
Ткст FileExtToMIME(const Ткст& ext);
Ткст MIMEToFileExt(const Ткст& mime);

class IpAddrInfo {
	enum { COUNT = 128 };
	struct Entry {
		const char *host;
		const char *port;
		int         family;
		int         status;
		addrinfo   *addr;
	};
	static Entry     пул[COUNT];
	
	enum {
		EMPTY = 0, WORKING, CANCELED, RESOLVED, FAILED
	};

	Ткст host, port;
	int    family;
	Entry *entry;
	Entry  exe[1];

	static void EnterPool();
	static void LeavePool();
	static auxthread_t auxthread__ Нить(void *ptr);

	void старт();
	
	IpAddrInfo(const IpAddrInfo&);

public:
	enum IpAddrFamily {
		FAMILY_ANY = 0, FAMILY_IPV4, FAMILY_IPV6
	};
	void      старт(const Ткст& host, int port, int family = FAMILY_ANY);
	bool      InProgress();
	bool      выполни(const Ткст& host, int port, int family = FAMILY_ANY);
	addrinfo *дайРез() const;
	void      очисть();

	IpAddrInfo();
	~IpAddrInfo()           { очисть(); }
};

struct SSLInfo {
	Ткст  cipher;
	bool    cert_avail;
	bool    cert_verified; // Peer verification not yet working - this is always false
	Ткст  cert_subject;
	Ткст  cert_issuer;
	Дата    cert_notbefore;
	Дата    cert_notafter;
	int     cert_version;
	Ткст  cert_serial;
};

enum { WAIT_READ = 1, WAIT_WRITE = 2, WAIT_IS_EXCEPTION = 4 };

class TcpSocket : БезКопий {
	enum { BUFFERSIZE = 512 };
	enum { NONE, CONNECT, ACCEPT, SSL_CONNECTED };
	SOCKET                  socket;
	int                     mode;
	char                    буфер[BUFFERSIZE];
	char                   *ptr;
	char                   *end;
	bool                    is_eof;
	bool                    is_error;
	bool                    is_timeout;
	bool                    is_abort;
	bool                    ipv6;

	int                     timeout;
	int                     waitstep;
	int                     done;

	int                     global_timeout;
	int                     start_time;
#if defined(PLATFORM_WIN32) || defined(PLATFORM_BSD)
	int                     connection_start;
#endif
	int                     ssl_start;

	int                     errorcode;
	Ткст                  errordesc;
	
	struct SSL {
		virtual bool  старт() = 0;
		virtual bool  жди(dword flags, int end_time) = 0;
		virtual int   Send(const void *буфер, int maxlen) = 0;
		virtual int   Recv(void *буфер, int maxlen) = 0;
		virtual void  открой() = 0;
		virtual dword Handshake() = 0;
		
		virtual ~SSL() {}
	};
	
	Один<SSL>                ssl;
	Один<SSLInfo>            sslinfo;
	Ткст                  cert, pkey, sni;
	bool                    asn1;

	struct SSLImp;
	friend struct SSLImp;

	static SSL *(*CreateSSL)(TcpSocket& socket);
	static SSL *CreateSSLImp(TcpSocket& socket);

	friend void  ИниtCreateSSL();
	friend class IpAddrInfo;

	int                     GetEndTime() const;
	bool                    RawWait(dword flags, int end_time);
	bool                    жди(dword events, int end_time);
	SOCKET                  AcceptRaw(dword *ipaddr, int timeout_msec);
	bool                    SetupSocket();
	bool                    открой(int family, int тип, int protocol);
	int                     RawRecv(void *буфер, int maxlen);
	int                     Recv(void *буфер, int maxlen);
	int                     RawSend(const void *буфер, int maxlen);
	int                     Send(const void *буфер, int maxlen);
	bool                    RawConnect(addrinfo *arp);
	void                    RawClose();

	void                    ReadBuffer(int end_time);
	int                     Get_();
	int                     Peek_();
	int                     Peek_(int end_time);
	int                     подбери(int end_time)          { return ptr < end ? (byte)*ptr : Peek_(end_time); }
	bool                    IsGlobalTimeout();

	void                    переустанов();

	void                    SetSockError(const char *context, const char *errdesc);
	void                    SetSockError(const char *context);

	bool                    WouldBlock();

	static int              GetErrorCode();
	static void             иниц();
	
	TcpSocket(const TcpSocket&);

public:
	Событие<>         WhenWait;
	
	void            SetSockError(const char *context, int код_, const char *errdesc);

	enum { ERROR_GLOBAL_TIMEOUT = -1000000, ERROR_SSLHANDSHAKE_TIMEOUT, ERROR_LAST };

	static Ткст   GetHostName();
	
	int             GetDone() const                          { return done; }

	bool            открыт() const                           { return socket != INVALID_SOCKET; }
	bool            кф_ли() const;

	bool            ошибка_ли() const                          { return is_error; }
	void            сотриОш()                             { is_error = false; errorcode = 0; errordesc.очисть(); }
	int             дайОш() const                         { return errorcode; }
	Ткст          GetErrorDesc() const                     { return errordesc; }

	void            Abort()                                  { is_abort = true; }
	bool            IsAbort() const                          { return is_abort; }
	void            ClearAbort()                             { is_abort = false; }
	
	bool            таймаут_ли() const                        { return is_timeout; }
	
	SOCKET          GetSOCKET() const                        { return socket; }
	Ткст          GetPeerAddr() const;

	void            прикрепи(SOCKET socket);
	bool            Connect(const char *host, int port);
	bool            Connect(IpAddrInfo& инфо);
	bool            WaitConnect();
	bool            Listen(int port, int listen_count = 5, bool ipv6 = false, bool reuse = true, void* addr = NULL);
	bool            Listen(const IpAddrInfo& addr, int port, int listen_count = 5, bool ipv6 = false, bool reuse = true);
	bool            прими(TcpSocket& listen_socket);
	void            открой();
	void            Shutdown();

	void            NoDelay();
	void            Linger(int msecs);
	void            NoLinger()                               { Linger(Null); }
	
	bool            жди(dword events);
	bool            WaitRead()                               { return жди(WAIT_READ); }
	bool            WaitWrite()                              { return жди(WAIT_WRITE); }

	int             подбери()                                   { return ptr < end ? (byte)*ptr : Peek_(); }
	int             прекрати()                                   { return подбери(); }
	int             дай()                                    { return ptr < end ? (byte)*ptr++ : Get_(); }
	int             дай(void *буфер, int len);
	Ткст          дай(int len);

	int             помести(const void *s, int len);
	int             помести(const Ткст& s)                     { return помести(s.старт(), s.дайДлину()); }

	bool            дайВсе(void *буфер, int len);
	Ткст          дайВсе(int len);
	Ткст          дайСтроку(int maxlen = 65536);

	bool            PutAll(const void *s, int len);
	bool            PutAll(const Ткст& s);
	
	bool            StartSSL();
	bool            IsSSL() const                            { return ssl; }
	dword           SSLHandshake();
	void            SSLCertificate(const Ткст& cert, const Ткст& pkey, bool asn1);
	void            SSLServerNameIndication(const Ткст& имя);
	const SSLInfo  *GetSSLInfo() const                       { return ~sslinfo; }
	
	void            очисть();

	TcpSocket&      Timeout(int ms)                          { timeout = ms; return *this; }
	int             GetTimeout() const                       { return timeout; }
	TcpSocket&      GlobalTimeout(int ms);
	TcpSocket&      NoGlobalTimeout()                        { return GlobalTimeout(Null); }
	TcpSocket&      Blocking()                               { return Timeout(Null); }
	bool            IsBlocking()                             { return пусто_ли(GetTimeout()); }
	TcpSocket&      WaitStep(int ms)                         { waitstep = ms; return *this; }
	int             GetWaitStep() const                      { return waitstep; }

	TcpSocket();
	~TcpSocket()                                             { открой(); }
};

class SocketWaitСобытие {
	Вектор<Кортеж<int, dword>> socket;
	fd_set read[1], write[1], exception[1];
	SocketWaitСобытие(const SocketWaitСобытие &);

public:
	void  очисть()                                            { socket.очисть(); }
	void  добавь(SOCKET s, dword events)                        { socket.добавь(сделайКортеж((int)s, events)); }
	void  добавь(TcpSocket& s, dword events)                    { добавь(s.GetSOCKET(), events); }
	int   жди(int timeout);
	dword дай(int i) const;
	dword operator[](int i) const                            { return дай(i); }
	
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

	void очисть()                      { *this = UrlInfo(); }
	void Parse(const Ткст& url);

	Ткст operator[](const char *ид) const;
	const Вектор<Ткст>& дайМассив(const char *ид) const;
	
	UrlInfo() {}
	UrlInfo(const Ткст& url)        { Parse(url); }
};

struct HttpCookie : Движимое<HttpCookie> {
	Ткст ид;
	Ткст значение;
	Ткст domain;
	Ткст path;
	Ткст raw;

	void очисть();
	bool Parse(const Ткст& cookie);
};

struct HttpHeader {
	Ткст                        first_line;
	Ткст                        f1, f2, f3;
	ВекторМап<Ткст, Ткст>     fields;
	ВекторМап<Ткст, HttpCookie> cookies;
	bool                          scgi;
	
	Ткст operator[](const char *ид) const                  { return fields.дай(ид, Null); }
	Ткст GetCookie(const char *ид) const;

	bool   Response(Ткст& protocol, int& код_, Ткст& reason) const;
	bool   Request(Ткст& method, Ткст& uri, Ткст& версия) const;
	
	Ткст GetProtocol() const                               { return f1; }
	int    дайКод() const;
	Ткст GetReason() const                                 { return f3; }
	
	Ткст GetMethod() const                                 { return f1; }
	Ткст GetURI() const                                    { return f2; }
	Ткст дайВерсию() const                                { return f3; }

	bool   HasContentLength() const;
	int64  GetContentLength() const;

	void   очисть();
	bool   ParseAdd(const Ткст& hdrs);
	bool   Parse(const Ткст& hdrs);
	bool   ParseSCGI(const Ткст& scgi_hdr);

	bool   читай(TcpSocket& socket);
	
	HttpHeader()                                             { scgi = false; }

private:
	void   добавь(const Ткст& ид, const Ткст& значение);
	HttpHeader(const HttpHeader&);
};

class HttpRequest : public TcpSocket {
	int          phase;
	dword        waitevents;
	Ткст       данные;
	int64        count;

	HttpHeader   header;

	Ткст       Ошибка;
	Ткст       body;
	int64        content_length;
	bool         has_content_length;
	bool         chunked_encoding;

	enum {
		DEFAULT_HTTP_PORT        = 80,
		DEFAULT_HTTPS_PORT       = 443
	};

	int          max_header_size;
	int          max_content_size;
	int          max_redirects;
	int          max_retries;
	int          timeout;

	Ткст       host;
	int          port;
	Ткст       proxy_host;
	int          proxy_port;
	Ткст       proxy_username;
	Ткст       proxy_password;
	Ткст       ssl_proxy_host;
	int          ssl_proxy_port;
	Ткст       ssl_proxy_username;
	Ткст       ssl_proxy_password;
	Ткст       path;
	Ткст       phost;
	bool         ssl;
	bool         ssl_get_proxy;

	int          method;
	Ткст       custom_method;
	Ткст       accept;
	Ткст       agent;
	bool         force_digest;
	bool         is_post;
	bool         std_headers;
	bool         hasurlvar;
	bool		 keep_alive;
	bool         all_content;
	Ткст       contenttype;
	Ткст       username;
	Ткст       password;
	Ткст       authorization;
	Ткст       request_headers;
	Ткст       postdata;
	Ткст       multipart;
	ВекторМап<Ткст, HttpCookie> cookies;

	Ткст       protocol;
	int          status_code;
	Ткст       reason_phrase;
	
	int          start_time;
	int          retry_count;
	int          redirect_count;
	
	int          chunk;

	IpAddrInfo   addrinfo;
	bool         gzip;
	Zlib         z;

	Поток      *poststream;
	int64        postlen;
	
	Ткст       chunk_crlf;

	void         иниц();

	void         StartPhase(int s);
	void         старт();
	void         Dns();
	void         StartConnect();
	void         ProcessSSLProxyResponse();
	void         AfterConnect();
	void         StartRequest();
	bool         SendingData(bool request = false);

	bool         ReadingHeader();
	bool         ReadingTrailer();
	void         StartBody();
	bool         ReadingBody();
	void         ReadingChunkHeader();
	void         финиш();
	bool         IsRequestTimeout();
	void         CopyCookies();

	void         HttpError(const char *s);
	void         выведи(const void *ptr, int size);

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

	Событие<const void *, int>  WhenContent;
	Событие<>                   WhenStart;
	Событие<>                   WhenDo;
	Врата<>                    WhenAuthenticate;

	HttpRequest&  MaxHeaderSize(int m)                   { max_header_size = m; return *this; }
	HttpRequest&  MaxContentSize(int m)                  { max_content_size = m; return *this; }
	HttpRequest&  MaxRedirect(int n)                     { max_redirects = n; return *this; }
	HttpRequest&  MaxRetries(int n)                      { max_retries = n; return *this; }
	HttpRequest&  RequestTimeout(int ms)                 { timeout = ms; return *this; }
	HttpRequest&  ChunkSize(int n)                       { chunk = n; return *this; }
	HttpRequest&  AllContent(bool b = true)              { all_content = b; return *this; }

	HttpRequest&  Method(int m, const char *custom_name = NULL);
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
	HttpRequest&  Port(int p)                            { port = p; return *this; }
	HttpRequest&  SSL(bool b = true)                     { ssl = b; return *this; }
	HttpRequest&  Path(const Ткст& p)                  { path = p; return *this; }
	HttpRequest&  Authorization(const Ткст& h)         { authorization = h; return *this; }
	HttpRequest&  User(const Ткст& u, const Ткст& p) { username = u; password = p; return *this; }
	HttpRequest&  Digest()                               { force_digest = true; return *this; }
	HttpRequest&  Digest(const Ткст& u, const Ткст& p) { User(u, p); return Digest(); }
	HttpRequest&  SetDigest(const Ткст& d)             { return Authorization("Digest " + d); }
	HttpRequest&  Url(const char *url);
	HttpRequest&  UrlVar(const char *ид, const Ткст& данные);
	HttpRequest&  operator()(const char *ид, const Ткст& данные) { return UrlVar(ид, данные); }
	HttpRequest&  PostData(const Ткст& pd)              { postdata = pd; poststream = NULL; return *this; }
	HttpRequest&  PostStream(Поток& s, int64 len = Null);
	Ткст        GetPostData() const                     { return postdata; }

	bool          ResolveDigestAuthentication();

	HttpRequest&  PostUData(const Ткст& pd)             { return PostData(UrlEncode(pd)); }
	HttpRequest&  пост(const Ткст& данные)                { POST(); return PostData(данные); }
	HttpRequest&  пост(const char *ид, const Ткст& данные);
	HttpRequest&  Part(const char *ид, const Ткст& данные,
	                   const char *content_type = NULL, const char *имяф = NULL);

	HttpRequest&  ClearPost()                             { PostData(Null); poststream = NULL; ; multipart.очисть(); GET(); return *this; }

	HttpRequest&  Headers(const Ткст& h)                { request_headers = h; return *this; }
	HttpRequest&  ClearHeaders()                          { return Headers(Null); }
	HttpRequest&  AddHeaders(const Ткст& h)             { request_headers.конкат(h); return *this; }
	HttpRequest&  Header(const char *ид, const Ткст& данные);
	
	HttpRequest&  Cookie(const HttpCookie& c);
	HttpRequest&  Cookie(const Ткст& ид, const Ткст& значение,
	                     const Ткст& domain = Null, const Ткст& path = Null);
	HttpRequest&  CopyCookies(const HttpRequest& r);
	HttpRequest&  ClearCookies()                          { cookies.очисть(); return *this; }

	HttpRequest&  StdHeaders(bool sh)                     { std_headers = sh; return *this; }
	HttpRequest&  NoStdHeaders()                          { return StdHeaders(false); }
	HttpRequest&  прими(const Ткст& a)                 { accept = a; return *this; }
	HttpRequest&  UserAgent(const Ткст& a)              { agent = a; return *this; }
	HttpRequest&  ContentType(const Ткст& a)            { contenttype = a; return *this; }
	HttpRequest&  KeepAlive(bool ka = true)               { keep_alive = ka; return *this;}

	HttpRequest&  прокси(const Ткст& host, int port)            { proxy_host = host; proxy_port = port; return *this; }
	HttpRequest&  прокси(const char *p);
	HttpRequest&  ProxyAuth(const Ткст& u, const Ткст& p)    { proxy_username = u; proxy_password = p; return *this; }

	HttpRequest&  SSLProxy(const Ткст& host, int port)         { ssl_proxy_host = host; ssl_proxy_port = port; return *this; }
	HttpRequest&  SSLProxy(const char *p);
	HttpRequest&  SSLProxyAuth(const Ткст& u, const Ткст& p) {  ssl_proxy_username = u; ssl_proxy_password = p; return *this; }
	HttpRequest&  SSLProxyGET(bool b = true)                     { ssl_get_proxy = b; return *this; }

	HttpRequest&  CommonProxy(const Ткст& host, int port)         { прокси(host, port); return SSLProxy(host, port); }
	HttpRequest&  CommonProxy(const char *p)                        { прокси(p); return SSLProxy(p); }
	HttpRequest&  CommonProxyAuth(const Ткст& u, const Ткст& p) { ProxyAuth(u, p); return SSLProxyAuth(u, p); }

	bool         IsSocketError() const                    { return TcpSocket::ошибка_ли(); }
	bool         IsHttpError() const                      { return !пусто_ли(Ошибка) ; }
	bool         ошибка_ли() const                          { return IsSocketError() || IsHttpError(); }
	Ткст       GetErrorDesc() const                     { return IsSocketError() ? TcpSocket::GetErrorDesc() : Ошибка; }
	void         сотриОш()                             { TcpSocket::сотриОш(); Ошибка.очисть(); }

	Ткст       GetHeader(const char *ид)                { return header[ид]; }
	Ткст       operator[](const char *ид)               { return GetHeader(ид); }
	Ткст       GetRedirectUrl();
	bool         HasContentLength();
	int64        GetContentLength();
	int          GetStatusCode() const                    { return status_code; }
	Ткст       GetReasonPhrase() const                  { return reason_phrase; }

	const HttpHeader& GetHttpHeader() const               { return header; }
	Ткст       GetCookie(const char *ид)                { return header.GetCookie(ид); }

	Ткст       GetContent() const                       { return body; }
	Ткст       operator~() const                        { return GetContent(); }
	operator     Ткст() const                           { return GetContent(); }
	void         ClearContent()                           { body.очисть(); }

	enum Phase {
		BEGIN, START, DNS,
		SSLPROXYREQUEST, SSLPROXYRESPONSE, SSLHANDSHAKE,
		REQUEST, HEADER, BODY,
		CHUNK_HEADER, CHUNK_BODY, CHUNK_CRLF, TRAILER,
		FINISHED, FAILED,
	};

	bool    делай();
	dword   GetWaitСобытиеs()                       { return waitevents; }
	int     GetPhase() const                      { return phase; }
	Ткст  GetPhaseName() const;
	bool    InProgress() const                    { return phase != FAILED && phase != FINISHED; }
	bool    IsFailure() const                     { return phase == FAILED; }
	bool    IsSuccess() const                     { return phase == FINISHED && status_code >= 200 && status_code < 300; }

	Ткст  выполни();

	void    нов();
	void    NewRequest();
	void    очисть();

	HttpRequest();
	HttpRequest(const char *url);
	
	static void  Trace(bool b = true);
	static void  TraceHeader(bool b = true);
	static void  TraceBody(bool b = true);
	static void  TraceShort(bool b = true);
};

bool HttpResponse(TcpSocket& socket, bool scgi, int код_, const char *phrase,
                  const char *content_type = NULL, const Ткст& данные = Null,
                  const char *server = NULL, bool gzip = false);

#include <RKod/Core.h>

class WebSocket {
	Ткст     Ошибка;

	TcpSocket  std_socket;
	TcpSocket *socket;
	
	Ткст     uri;
	Ткст     host;
	IpAddrInfo addrinfo;
	bool       ssl;
	Ткст     request_headers;

	Ткст     данные;
	int        data_pos;

	int        opcode;
	int64      length;
	bool       mask;
	int        ключ[4];

	struct Input : Движимое<Input> {
		dword  opcode;
		Ткст данные;
	};
	
	БиВектор<Input>  in_queue;
	
	БиВектор<Ткст> out_queue;
	int              out_at;
	
	bool             close_sent;
	bool             close_received;
	
	dword            current_opcode;
	
	bool             client;
	
	int              redirect = 0;

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

	void очисть();
	void ошибка(const Ткст& Ошибка);

	void выведи(const Ткст& s);

	void Output();

	void StartConnect();
	void Dns();
	void SSLHandshake();
	void SendRequest();
	bool ReadHttpHeader();
	void ResponseHeader();
	void RequestHeader();
	void FrameHeader();
	void FrameData();

	int GetFinIndex() const;

	void   SendRaw(int hdr, const Ткст& данные, dword mask = 0);
	void   делай0();
	
	static Ткст FormatBlock(const Ткст& s);

public:
	WebSocket& NonBlocking(bool b = true)               { socket->Timeout(b ? 0 : Null); return *this; }

	WebSocket&  Headers(const Ткст& h)                { request_headers = h; return *this; }
	WebSocket&  ClearHeaders()                          { return Headers(Null); }
	WebSocket&  AddHeaders(const Ткст& h)             { request_headers.конкат(h); return *this; }
	WebSocket&  Header(const char *ид, const Ткст& данные);

	Ткст      GetHeaders()                            { return request_headers; }
	
	bool   IsBlocking() const                           { return пусто_ли(socket->GetTimeout()); }
	
	bool   ошибка_ли() const                              { return socket->ошибка_ли() || Ошибка.дайСчёт(); }
	Ткст дайОш() const                             { return Nvl(socket->GetErrorDesc(), Ошибка); }
	
	bool   прими(TcpSocket& listener_socket);
	bool   Connect(const Ткст& uri, const Ткст& host, bool ssl, int port);
	bool   Connect(const Ткст& uri, const Ткст& host, bool ssl) { return Connect(uri, host, ssl, ssl ? 440 : 80); }
	bool   Connect(const Ткст& url);
	
	void   делай();

	Ткст Receive();
	bool   конечен() const                                { return current_opcode & FIN; }
	bool   текст_ли() const                               { return current_opcode & TEXT; }
	bool   IsBinary() const                             { return current_opcode & BINARY; }

	void   SendText(const Ткст& данные)                 { SendRaw(FIN|TEXT, данные); }
	void   SendTextMasked(const Ткст& данные)           { SendRaw(FIN|TEXT, данные, MASK); }
	void   SendBinary(const Ткст& данные)               { SendRaw(FIN|BINARY, данные); }
	void   Ping(const Ткст& данные)                     { SendRaw(FIN|PING, данные); }

	void   BeginText(const Ткст& данные)                { SendRaw(TEXT, данные); }
	void   BeginBinary(const Ткст& данные)              { SendRaw(BINARY, данные); }
	void   Continue(const Ткст& данные)                 { SendRaw(0, данные); }
	void   Fin(const Ткст& данные)                      { SendRaw(FIN, данные); }

	void   открой(const Ткст& msg = Null, bool wait_reply = false);
	bool   открыт() const                               { return socket->открыт(); }
	bool   IsClosed() const                             { return !открыт(); }

	dword  GetWaitСобытиеs() const                        { return WAIT_READ|(!!out_queue.дайСчёт() * WAIT_WRITE); }
	SOCKET GetSOCKET() const                            { return socket ? socket->GetSOCKET() : INVALID_SOCKET; }
	Ткст GetPeerAddr() const                          { return socket ? socket->GetPeerAddr() : ""; }
	void   добавьК(SocketWaitСобытие& e)                    { e.добавь(*socket, GetWaitСобытиеs()); }

	static void Trace(bool b = true);

	WebSocket();

// backward compatibility:
	bool   WebAccept(TcpSocket& socket, HttpHeader& hdr);
	bool   WebAccept(TcpSocket& socket);

	int    GetOpCode() const { return current_opcode & 15; }

	bool   SendText(const Ткст& данные, bool fin)                   { SendRaw((fin ? 0x80 : 0)|TEXT, данные); return !ошибка_ли(); }
	bool   SendText(const void *данные, int len, bool fin = true)     { return SendText(Ткст((char *)данные, len), fin); }

	bool   SendBinary(const Ткст& данные, bool fin)                 { SendRaw((fin ? 0x80 : 0)|BINARY, данные); return !ошибка_ли(); }
	bool   SendBinary(const void *данные, int len, bool fin = true)   { return SendText(Ткст((char *)данные, len), fin); }

	Ткст GetErrorDesc() const                                     { return дайОш(); }

// keep mispeled method names
	Ткст Recieve()    { return Receive(); }
};

void ParseProxyUrl(const char *p, Ткст& proxy_host, int& proxy_port);
