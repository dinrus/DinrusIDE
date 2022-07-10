#include "Core.h"

namespace РНЦПДинрус {
	
static bool sTrace;

#define LLOG(x)  if(sTrace) RLOG((client ? "WS CLIENT " : "WS SERVER ") << x)

void WebSocket::Trace(bool b)
{
	sTrace = b;
}

Ткст WebSocket::FormatBlock(const Ткст& s)
{
	return какТкстСи(s.дайСчёт() < 500 ? s : s.середина(0, 500), INT_MAX, NULL, ASCSTRING_OCTALHI);
}

WebSocket::WebSocket()
{
	очисть();

	static Ткст request_headers_const =
	    "прими: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
		"прими-Language: cs,en-US;q=0.7,en;q=0.3\r\n"
		"Sec-WebSocket-Version: 13\r\n"
		"Sec-WebSocket-Extensions: permessage-deflate\r\n"
		"Connection: keep-alive, Upgrade\r\n"
		"Pragma: no-cache\r\n"
		"Кэш-Контрол: no-cache\r\n"
		"Upgrade: websocket\r\n";
    request_headers = request_headers_const;
}

void WebSocket::очисть()
{
	socket = &std_socket;
	opcode = 0;
	current_opcode = 0;
	данные.очисть();
	data_pos = 0;
	in_queue.очисть();
	out_queue.очисть();
	out_at = 0;
	Ошибка.очисть();
	socket->очисть();
	close_sent = close_received = false;
	client = false;
}

void WebSocket::ошибка(const Ткст& err)
{
	LLOG("Ошибка: " << err);
	Ошибка = err;
}

bool WebSocket::прими(TcpSocket& listen_socket)
{
	очисть();
	if(!socket->прими(listen_socket)) {
		ошибка("прими has failed");
		return false;
	}
	opcode = HTTP_REQUEST_HEADER;
	return true;
}

WebSocket& WebSocket::Header(const char *ид, const Ткст& данные)
{
	request_headers << ид << ": " << данные << "\r\n";
	return *this;
}

bool WebSocket::Connect(const Ткст& url)
{
	const char *u = url;
	bool ssl = memcmp(u, "wss", 3) == 0;
	const char *t = u;
	while(*t && *t != '?')
		if(*t++ == '/' && *t == '/') {
			u = ++t;
			break;
		}
	t = u;
	while(*u && *u != ':' && *u != '/' && *u != '?')
		u++;
	Ткст host = Ткст(t, u);
	int port = ssl ? 443 : 80;
	if(*u == ':')
		port = сканЦел(u + 1, &u);
	
	return Connect(url.начинаетсяС("wss:") ? "https:" + url.середина(4)
	               : url.начинаетсяС("ws:") ? "http:" + url.середина(3) : url,
	               host, ssl, port);
}

bool WebSocket::Connect(const Ткст& uri_, const Ткст& host_, bool ssl_, int port)
{
	очисть();
	
	client = true;
	redirect = 0;
	
	uri = uri_;
	host = host_;
	ssl = ssl_;
	
	if(socket->IsBlocking()) {
		if(!addrinfo.выполни(host, port)) {
			ошибка("Not found");
			return false;
		}
		LLOG("DNS resolved");
		StartConnect();
		while(opcode != READING_FRAME_HEADER) {
			делай0();
			if(ошибка_ли())
				return false;
		}
	}
	else {
		opcode = DNS;
		addrinfo.старт(host, port);
	}
	return true;
}

void WebSocket::SendRequest()
{
	LLOG("Sending connection request");
	Ткст h;
	for(int i = 0; i < 16; i++)
		h.конкат(случ());
	выведи( // needs to be the first thing to sent after the connection is established
	    "GET " + uri + " HTTP/1.1\r\n"
	    "Хост: " + host + "\r\n" +
		"Sec-WebSocket-Ключ: " + Base64Encode(h) + "\r\n" +
	    request_headers +
	    "\r\n"
	);
	opcode = HTTP_RESPONSE_HEADER;
}

void WebSocket::StartConnect()
{
	if(!socket->Connect(addrinfo)) {
		ошибка("Connect has failed");
		return;
	}
	
	LLOG("Connect issued");
	
	if(IsBlocking()) {
		if(ssl) {
			socket->StartSSL();
			socket->SSLHandshake();
			LLOG("Blocking SSL handshake finished");
		}
		SendRequest();
		return;
	}
	
	if(ssl) {
		if(!socket->StartSSL()) {
			ошибка("Unable to start SSL handshake");
			return;
		}
		LLOG("Started SSL handshake");
		opcode = SSL_HANDSHAKE;
	}
	else
		SendRequest();
}

void WebSocket::Dns()
{
	if(addrinfo.InProgress())
		return;
	if(!addrinfo.дайРез()) {
		ошибка("DNS lookup failed");
		return;
	}
	LLOG("DNS resolved");
	StartConnect();
}

void WebSocket::SSLHandshake()
{
	if(socket->SSLHandshake())
		return;
	LLOG("SSL handshake finished");
	SendRequest();
}

bool WebSocket::ReadHttpHeader()
{
	for(;;) {
		int c = socket->дай();
		if(c < 0)
			return false;
		else
			данные.конкат(c);
		if(данные.дайСчёт() == 2 && данные[0] == '\r' && данные[1] == '\n') { // header is empty
			ошибка("Empty HTTP header");
			return false;
		}
		if(данные.дайСчёт() >= 3) {
			const char *h = данные.последний();
			if(h[0] == '\n' && h[-1] == '\r' && h[-2] == '\n') { // empty ending line after non-empty header
				LLOG("HTTP header received: " << FormatBlock(данные));
				return true;
			}
		}
		if(данные.дайСчёт() > 100000) {
			ошибка("HTTP header size exceeded");
			return false;
		}
	}
}

void WebSocket::RequestHeader()
{
	if(ReadHttpHeader()) {
		HttpHeader hdr;
		if(!hdr.Parse(данные)) {
			ошибка("Invalid HTTP header");
			return;
		}
		Ткст dummy;
		hdr.Request(dummy, uri, dummy);
		Ткст ключ = hdr["sec-websocket-ключ"];
		if(пусто_ли(ключ)) {
			ошибка("Invalid HTTP header: missing sec-websocket-ключ");
			return;
		}
	
		byte sha1[20];
		SHA1(sha1, ключ + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
		
		выведи(
			"HTTP/1.1 101 Switching Protocols\r\n"
			"Upgrade: websocket\r\n"
			"Connection: Upgrade\r\n"
			"Sec-WebSocket-прими: " + Base64Encode((char *)sha1, 20) + "\r\n\r\n"
		);
		
		LLOG("HTTP request header received, sending response");
		данные.очисть();
		opcode = READING_FRAME_HEADER;
	}
}

void WebSocket::ResponseHeader()
{
	if(ReadHttpHeader()) {
		HttpHeader h;
		h.Parse(данные);
		int code = h.дайКод();
		if(code >= 300 && code < 400) {
			int r = redirect + 1;
			if(r++ < 5) {
				Connect(h["location"]);
				redirect = r;
				return;
			}
		}
		if(впроп(h["upgrade"]) != "websocket") {
			ошибка("Повреждённый заголовок ответа сервера HTTP");
			return;
		}
		LLOG("Получен заголовок ответа HTTP");
		opcode = READING_FRAME_HEADER;
		данные.очисть();
	}
}

void WebSocket::FrameHeader()
{
	for(;;) {
		int c = socket->дай();
		if(c < 0)
			return;
		данные.конкат(c);
		
		LLOG("Receiving frame header, текущ header len: " << данные.дайСчёт());

		int ii = 0;
		bool ok = true;
		auto дай = [&]() -> byte {
			if(ii < данные.дайСчёт())
				return данные[ii++];
			ok = false;
			return 0;
		};
		auto GetLen = [&](int count) -> int64 {
			int64 len = 0;
			while(count--)
				len = (len << 8) | дай();
			return len;
		};
		int new_opcode = дай();
		length = дай();
		mask = length & 128;
		length &= 127;
		if(length == 127)
			length = GetLen(8);
		if(length == 126)
			length = GetLen(2);
		if(mask) {
			ключ[0] = дай();
			ключ[1] = дай();
			ключ[2] = дай();
			ключ[3] = дай();
		}

		if(ok) {
			LLOG("Фрейм header received, len: " << length << ", code " << new_opcode);
			opcode = new_opcode;
			данные.очисть();
			data_pos = 0;
			return;
		}
	}
}

void WebSocket::открой(const Ткст& msg, bool wait_reply)
{
	LLOG("Sending CLOSE");
	SendRaw(CLOSE|FIN, msg, MASK);
	close_sent = true;
	if(IsBlocking())
		while((wait_reply ? открыт() : out_queue.дайСчёт()) && !ошибка_ли() && socket->открыт())
			делай0();
}

void WebSocket::FrameData()
{
	Буфер<char> буфер(32768);
	while(data_pos < length) {
		int n = socket->дай(~буфер, (int)min(length - data_pos, (int64)32768));
		if(n == 0)
			return;
		if(mask)
			for(int i = 0; i < n; i++) // TODO: Optimize
				буфер[i] ^= ключ[(i + data_pos) & 3];
		данные.конкат(~буфер, n); // TODO: разбей long данные
		data_pos += n;
		LLOG("Фрейм данные chunk received, chunk len: " << n);
	}
	LLOG("Фрейм данные received");
	int op = opcode & 15;
	switch(op) {
	case PING:
		LLOG("PING");
		SendRaw(PONG|FIN, данные, MASK);
		break;
	case CLOSE:
		LLOG("CLOSE received");
		close_received = true;
		if(!close_sent)
			открой(данные);
		socket->открой();
		break;
	default:
		Input& m = in_queue.добавьХвост();
		m.opcode = opcode;
		m.данные = данные;
		LLOG((m.opcode & TEXT ? "TEXT: " : "BINARY: ") << FormatBlock(данные));
		LLOG("Input queue count is now " << in_queue.дайСчёт());
		break;
	}
	данные.очисть();
	opcode = READING_FRAME_HEADER;
}

void WebSocket::делай0()
{
	int prev_opcode;
	do {
		prev_opcode = opcode;
		if(findarg(opcode, DNS, SSL_HANDSHAKE) < 0) {
			Output();
			if(socket->кф_ли() && !(close_sent || close_received))
				ошибка("Сокет неожиданно закрылся");
		}
		if(ошибка_ли())
			return;
		switch(opcode) {
		case DNS:
			Dns();
			break;
		case SSL_HANDSHAKE:
			SSLHandshake();
			break;
		case HTTP_RESPONSE_HEADER:
			ResponseHeader();
			break;
		case HTTP_REQUEST_HEADER:
			RequestHeader();
			break;
		case READING_FRAME_HEADER:
			FrameHeader();
			break;
		default:
			FrameData();
			break;
		}
	}
	while(!IsBlocking() && opcode != prev_opcode);
}

void WebSocket::делай()
{
	ПРОВЕРЬ(!IsBlocking());
	делай0();
}

Ткст WebSocket::Receive()
{
	current_opcode = 0;
	do {
		делай0();
		if(in_queue.дайСчёт()) {
			Ткст s = in_queue.дайГолову().данные;
			current_opcode = in_queue.дайГолову().opcode;
			in_queue.сбросьГолову();
			return s;
		}
	}
	while(IsBlocking() && socket->открыт() && !ошибка_ли());
	return Ткст::дайПроц();
}

void WebSocket::выведи(const Ткст& s)
{
	out_queue.добавьХвост(s);
	while((IsBlocking() || close_sent) && socket->открыт() && !ошибка_ли() && out_queue.дайСчёт())
		Output();
}

void WebSocket::Output()
{
	if(socket->открыт()) {
		while(out_queue.дайСчёт()) {
			const Ткст& s = out_queue.дайГолову();
			int n = socket->помести(~s + out_at, s.дайСчёт() - out_at);
			if(n == 0)
				break;
			LLOG("Sent " << n << " bytes: " << FormatBlock(s));
			out_at += n;
			if(out_at >= s.дайСчёт()) {
				out_at = 0;
				out_queue.сбросьГолову();
				LLOG("Block sent complete, " << out_queue.дайСчёт() << " remaining blocks in queue");
			}
		}
	}
}

void WebSocket::SendRaw(int hdr, const Ткст& данные, dword mask)
{
	if(ошибка_ли())
		return;
	
	ПРОВЕРЬ(!close_sent);
	LLOG("Send " << данные.дайСчёт() << " bytes, hdr: " << hdr);
	
	Ткст header;
	header.конкат(hdr);
	int len = данные.дайСчёт();
	if(len > 65535) {
		header.конкат(127 | mask);
		header.конкат(0);
		header.конкат(0);
		header.конкат(0);
		header.конкат(0);
		header.конкат(byte(len >> 24));
		header.конкат(byte(len >> 16));
		header.конкат(byte(len >> 8));
		header.конкат(byte(len));
	}
	else
	if(len > 125) {
		header.конкат(126 | mask);
		header.конкат(byte(len >> 8));
		header.конкат(byte(len));
	}
	else
		header.конкат((int)len | mask);

	if(mask) {
		byte Cle[4];
		for(int i = 0; i < 4; i++)
			header.конкат(Cle[i] = (byte)случ());

		выведи(header);

		if(данные.дайСчёт()) {
			ТкстБуф buf(данные.дайСчёт());
			int n = данные.дайСчёт();
			for(int i = 0; i < n; i++)
				buf[i] = данные[i] ^ Cle[i & 3];
			выведи(buf);
		}
	}
	else {
		выведи(header);
	
		if(данные.дайСчёт() == 0)
			return;
		выведи(данные);
	}
}

bool WebSocket::WebAccept(TcpSocket& socket_, HttpHeader& hdr)
{
	socket = &socket_;
	Ткст ключ = hdr["sec-websocket-ключ"];
	if(пусто_ли(ключ))
		return false;
	
	byte sha1[20];
	SHA1(sha1, ключ + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	
	выведи(
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-прими: " + Base64Encode((char *)sha1, 20) + "\r\n\r\n"
	);

	LLOG("HTTP request header received, sending response");
	данные.очисть();
	opcode = READING_FRAME_HEADER;
	return true;
}

bool WebSocket::WebAccept(TcpSocket& socket)
{
	HttpHeader hdr;
	if(!hdr.читай(socket))
		return false;
	return WebAccept(socket, hdr);
}

}
