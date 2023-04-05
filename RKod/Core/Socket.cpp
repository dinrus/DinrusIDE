#include "Core.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef PLATFORM_POSIX
#include <arpa/inet.h>
#endif

namespace РНЦПДинрус {

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#define LLOG(x)  // LOG("TCP " << x)

IpAddrInfo::Entry IpAddrInfo::пул[IpAddrInfo::COUNT];

Стопор IpAddrInfoPoolMutex;

void IpAddrInfo::EnterPool()
{
	IpAddrInfoPoolMutex.войди();
}

void IpAddrInfo::LeavePool()
{
	IpAddrInfoPoolMutex.выйди();
}

int sGetAddrInfo(const char *host, const char *port, int family, addrinfo **result)
{
	if(!host || !*host)
		return EAI_NONAME;
	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	const static int FamilyToAF[] = { AF_UNSPEC, AF_INET, AF_INET6 };
	hints.ai_family = FamilyToAF[(family > 0 && family < __countof(FamilyToAF)) ? family : 0];
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	return getaddrinfo(host, port, &hints, result);
}

auxthread_t auxthread__ IpAddrInfo::Нить(void *ptr)
{
	Entry *entry = (Entry *)ptr;
	EnterPool();
	if(entry->status == WORKING) {
		char host[1025];
		char port[257];
		int family = entry->family;
		strcpy(host, entry->host);
		strcpy(port, entry->port);
		LeavePool();
		addrinfo *result;
		if(sGetAddrInfo(host, port, family, &result) == 0 && result) {
			EnterPool();
			if(entry->status == WORKING) {
				entry->addr = result;
				entry->status = RESOLVED;
			}
			else {
				freeaddrinfo(result);
				entry->status = EMPTY;
			}
		}
		else {
			EnterPool();
			if(entry->status == CANCELED)
				entry->status = EMPTY;
			else
				entry->status = FAILED;
		}
	}
	LeavePool();
	return 0;
}

bool IpAddrInfo::выполни(const Ткст& host, int port, int family)
{
	очисть();
	entry = exe;
	addrinfo *result;
	entry->addr = sGetAddrInfo(~host, ~какТкст(port), family, &result) == 0 ? result : NULL;
	entry->status = entry->addr ? RESOLVED : FAILED;
	return entry->addr;
}

void IpAddrInfo::старт()
{
	if(entry)
		return;
	EnterPool();
	for(int i = 0; i < COUNT; i++) {
		Entry *e = пул + i;
		if(e->status == EMPTY) {
			entry = e;
			e->addr = NULL;
			if(host.дайСчёт() > 1024 || port.дайСчёт() > 256)
				e->status = FAILED;
			else {
				e->status = WORKING;
				e->host = host;
				e->port = port;
				e->family = family;
				стартВспомНити(&IpAddrInfo::Нить, e);
			}
			break;
		}
	}
	LeavePool();
}

void IpAddrInfo::старт(const Ткст& host_, int port_, int family_)
{
	очисть();
	port = какТкст(port_);
	host = host_;
	family = family_;
	старт();
}

bool IpAddrInfo::InProgress()
{
	if(!entry) {
		старт();
		return true;
	}
	EnterPool();
	int s = entry->status;
	LeavePool();
	return s == WORKING;
}

addrinfo *IpAddrInfo::дайРез() const
{
	EnterPool();
	addrinfo *ai = entry ? entry->addr : NULL;
	LeavePool();
	return ai;
}

void IpAddrInfo::очисть()
{
	EnterPool();
	if(entry) {
		if(entry->status == RESOLVED && entry->addr)
			freeaddrinfo(entry->addr);
		if(entry->status == WORKING)
			entry->status = CANCELED;
		else
			entry->status = EMPTY;
		entry = NULL;
	}
	LeavePool();
}

IpAddrInfo::IpAddrInfo()
{
	TcpSocket::иниц();
	entry = NULL;
}

#ifdef PLATFORM_POSIX

#define SOCKERR(x) x

const char *TcpSocketErrorDesc(int код_)
{
	return strerror(код_);
}

int TcpSocket::GetErrorCode()
{
	return errno;
}

#else

#define SOCKERR(x) WSA##x

const char *TcpSocketErrorDesc(int код_)
{
	static Кортеж<int, const char *> err[] = {
		{ WSAEINTR,                 "Interrupted ФУНКЦИЯ call." },
		{ WSAEACCES,                "Permission denied." },
		{ WSAEFAULT,                "Bad address." },
		{ WSAEINVAL,                "Invalid argument." },
		{ WSAEMFILE,                "Too many open files." },
		{ WSAEWOULDBLOCK,           "Resource temporarily unavailable." },
		{ WSAEINPROGRESS,           "Operation now in progress." },
		{ WSAEALREADY,              "Operation already in progress." },
		{ WSAENOTSOCK,              "TcpSocket operation on nonsocket." },
		{ WSAEDESTADDRREQ,          "Destination address required." },
		{ WSAEMSGSIZE,              "Message too long." },
		{ WSAEPROTOTYPE,            "Protocol wrong тип for socket." },
		{ WSAENOPROTOOPT,           "Bad protocol option." },
		{ WSAEPROTONOSUPPORT,       "Protocol not supported." },
		{ WSAESOCKTNOSUPPORT,       "TcpSocket тип not supported." },
		{ WSAEOPNOTSUPP,            "Operation not supported." },
		{ WSAEPFNOSUPPORT,          "Protocol family not supported." },
		{ WSAEAFNOSUPPORT,          "Address family not supported by protocol family." },
		{ WSAEADDRINUSE,            "Address already in use." },
		{ WSAEADDRNOTAVAIL,         "Cannot assign requested address." },
		{ WSAENETDOWN,              "Network is down." },
		{ WSAENETUNREACH,           "Network is unreachable." },
		{ WSAENETRESET,             "Network dropped connection on reset." },
		{ WSAECONNABORTED,          "Software caused connection abort." },
		{ WSAECONNRESET,            "Connection reset by peer." },
		{ WSAENOBUFS,               "No буфер space available." },
		{ WSAEISCONN,               "TcpSocket is already connected." },
		{ WSAENOTCONN,              "TcpSocket is not connected." },
		{ WSAESHUTDOWN,             "Cannot send after socket shutdown." },
		{ WSAETIMEDOUT,             "Connection timed out." },
		{ WSAECONNREFUSED,          "Connection refused." },
		{ WSAEHOSTDOWN,             "Хост is down." },
		{ WSAEHOSTUNREACH,          "No route to host." },
		{ WSAEPROCLIM,              "Too many processes." },
		{ WSASYSNOTREADY,           "Network subsystem is unavailable." },
		{ WSAVERNOTSUPPORTED,       "Winsock.dll версия out of range." },
		{ WSANOTINITIALISED,        "Successful WSAStartup not yet performed." },
		{ WSAEDISCON,               "Graceful shutdown in progress." },
		{ WSATYPE_NOT_FOUND,        "Класс тип not found." },
		{ WSAHOST_NOT_FOUND,        "Хост not found." },
		{ WSATRY_AGAIN,             "Nonauthoritative host not found." },
		{ WSANO_RECOVERY,           "This is a nonrecoverable Ошибка." },
		{ WSANO_DATA,               "Valid имя, no данные record of requested тип." },
		{ WSASYSCALLFAILURE,        "System call failure." },
	};
	const Кортеж<int, const char *> *x = найдиКортеж(err, __countof(err), код_);
	return x ? x->b : "Unknown Ошибка код_.";
}

int TcpSocket::GetErrorCode()
{
	return WSAGetLastError();
}

#endif

void TcpSocketИниt()
{
#ifdef PLATFORM_WIN32
	ONCELOCK {
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}
#endif
#ifdef PLATFORM_POSIX
	signal(SIGPIPE, SIG_IGN);
#endif
}

void TcpSocket::иниц()
{
	TcpSocketИниt();
}

void TcpSocket::переустанов()
{
	LLOG("переустанов");
	is_eof = false;
	socket = INVALID_SOCKET;
	ipv6 = false;
	ptr = end = буфер;
	is_error = false;
	is_abort = false;
	is_timeout = false;
	mode = NONE;
	ssl.очисть();
	sslinfo.очисть();
	cert = pkey = Null;
#if defined(PLATFORM_WIN32) || defined(PLATFORM_BSD)
	connection_start = Null;
#endif
	ssl_start = Null;
}

TcpSocket::TcpSocket()
{
	сотриОш();
	переустанов();
	timeout = global_timeout = start_time = Null;
	waitstep = 10;
	asn1 = false;
}

bool TcpSocket::SetupSocket()
{
#ifdef PLATFORM_WIN32
	connection_start = msecs();
	u_long арг = 1;
	if(ioctlsocket(socket, FIONBIO, &арг)) {
		SetSockError("ioctlsocket(FIO[N]BIO)");
		return false;
	}
#else
	#ifdef PLATFORM_BSD
		connection_start = msecs();
	#endif
	if(fcntl(socket, F_SETFL, (fcntl(socket, F_GETFL, 0) | O_NONBLOCK))) {
		SetSockError("fcntl(O_[NON]BLOCK)");
		return false;
	}
#endif
	return true;
}

bool TcpSocket::открой(int family, int тип, int protocol)
{
	иниц();
	открой();
	сотриОш();
	if((socket = ::socket(family, тип, protocol)) == INVALID_SOCKET) {
		SetSockError("open");
		return false;
	}
	LLOG("TcpSocket::Данные::открой() -> " << (int)socket);
	return SetupSocket();
}

bool TcpSocket::Listen(int port, int listen_count, bool ipv6_, bool reuse, void *addr)
{
	открой();
	иниц();
	переустанов();

	ipv6 = ipv6_;
	if(!открой(ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0))
		return false;
	sockaddr_in sin;
#ifdef PLATFORM_WIN32
	SOCKADDR_IN6 sin6;
	if(ipv6 && IsWinVista())
#else
	sockaddr_in6 sin6;
	if(ipv6)
#endif
	{
		обнули(sin6);
		sin6.sin6_family = AF_INET6;
		sin6.sin6_port = htons(port);
		sin6.sin6_addr = addr?(*(in6_addr*)addr):in6addr_any;
	}
	else {
		обнули(sin);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = addr?(*(uint32*)addr):htonl(INADDR_ANY);
	}
	if(reuse) {
		int optval = 1;
		setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
	}
	if(bind(socket, ipv6 ? (const sockaddr *)&sin6 : (const sockaddr *)&sin,
	        ipv6 ? sizeof(sin6) : sizeof(sin))) {
		SetSockError(фмт("bind(port=%d)", port));
		return false;
	}
	if(listen(socket, listen_count)) {
		SetSockError(фмт("listen(port=%d, count=%d)", port, listen_count));
		return false;
	}
	return true;
}

bool TcpSocket::Listen(const IpAddrInfo& addr, int port, int listen_count, bool ipv6, bool reuse)
{
	addrinfo *a = addr.дайРез();
	return Listen(port, listen_count, ipv6, reuse, &(((sockaddr_in*)a->ai_addr)->sin_addr.s_addr));
}

bool TcpSocket::прими(TcpSocket& ls)
{
	открой();
	иниц();
	переустанов();
	ПРОВЕРЬ(ls.открыт());
	int et = GetEndTime();
	for(;;) {
		int h = ls.GetTimeout();
		bool b = ls.Timeout(timeout).жди(WAIT_READ, et);
		ls.Timeout(h);
		if(!b) // timeout
			return false;
		socket = accept(ls.GetSOCKET(), NULL, NULL);
		if(socket != INVALID_SOCKET)
			break;
		if(!WouldBlock() && GetErrorCode() != SOCKERR(EINTR)) { // In prefork condition, жди is not enough, as other process can accept
			SetSockError("accept");
			return false;
		}
	}
	mode = ACCEPT;
	return SetupSocket();
}

Ткст TcpSocket::GetPeerAddr() const
{
	if(!открыт())
		return Null;
	sockaddr_in addr;
	socklen_t l = sizeof(addr);
	if(getpeername(socket, (sockaddr *)&addr, &l) != 0)
		return Null;
	if(l > sizeof(addr))
		return Null;
#ifdef PLATFORM_WIN32
	return inet_ntoa(addr.sin_addr);
#else
	char h[200];
	return inet_ntop(AF_INET, &addr.sin_addr, h, 200);
#endif
}

void TcpSocket::NoDelay()
{
	ПРОВЕРЬ(открыт());
	int __true = 1;
	LLOG("NoDelay(" << (int)socket << ")");
	if(setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&__true, sizeof(__true)))
		SetSockError("setsockopt(TCP_NODELAY)");
}

void TcpSocket::Linger(int msecs)
{
	ПРОВЕРЬ(открыт());
	linger ls;
	ls.l_onoff = !пусто_ли(msecs) ? 1 : 0;
	ls.l_linger = !пусто_ли(msecs) ? (msecs + 999) / 1000 : 0;
	if(setsockopt(socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char *>(&ls), sizeof(ls)))
		SetSockError("setsockopt(SO_LINGER)");
}

void TcpSocket::прикрепи(SOCKET s)
{
	открой();
	socket = s;
}

bool TcpSocket::RawConnect(addrinfo *arp)
{
	if(!arp) {
		SetSockError("connect", -1, "not found");
		return false;
	}
	Ткст err;
	for(int pass = 0; pass < 2; pass++) {
		addrinfo *rp = arp;
		while(rp) {
			if(rp->ai_family == AF_INET == !pass && // Try to connect IPv4 in the first pass
			   открой(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) {
				if(connect(socket, rp->ai_addr, (int)rp->ai_addrlen) == 0 ||
				   GetErrorCode() == SOCKERR(EINPROGRESS) || GetErrorCode() == SOCKERR(EWOULDBLOCK)
				) {
					mode = CONNECT;
					return true;
				}
				if(err.дайСчёт())
					err << '\n';
				err << TcpSocketErrorDesc(GetErrorCode());
				открой();
			}
			rp = rp->ai_next;
		}
    }
	SetSockError("connect", -1, Nvl(err, "failed"));
	return false;
}


bool TcpSocket::Connect(IpAddrInfo& инфо)
{
	LLOG("Connect addrinfo");
	иниц();
	переустанов();
	addrinfo *result = инфо.дайРез();
	return RawConnect(result);
}

bool TcpSocket::Connect(const char *host, int port)
{
	LLOG("Connect(" << host << ':' << port << ')');
	открой();
	иниц();
	переустанов();
	IpAddrInfo инфо;
	if(!инфо.выполни(host, port)) {
		SetSockError(фмт("getaddrinfo(%s) failed", host));
		return false;
	}
	return Connect(инфо);
}

bool TcpSocket::WaitConnect()
{
	if(WaitWrite()) {
		int optval = 0;
		socklen_t optlen = sizeof(optval);
		if (getsockopt(GetSOCKET(), SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen) == 0) {
			if (optval == 0)
				return true;
			else {
				SetSockError("wait connect", -1, Nvl(Ткст(TcpSocketErrorDesc(optval)), "failed"));
				return false;
			}
		}
	}
	return false;
}

void TcpSocket::RawClose()
{
	LLOG("close " << (int)socket);
	if(socket != INVALID_SOCKET) {
		int res;
#if defined(PLATFORM_WIN32)
		res = closesocket(socket);
#elif defined(PLATFORM_POSIX)
		res = close(socket);
#else
	#error Unsupported platform
#endif
		if(res && !ошибка_ли())
			SetSockError("close");
		socket = INVALID_SOCKET;
	}
}

void TcpSocket::открой()
{
	if(ssl)
		ssl->открой();
	else
		RawClose();
	ssl.очисть();
}

bool TcpSocket::WouldBlock()
{
	int c = GetErrorCode();
#ifdef PLATFORM_POSIX
#ifdef PLATFORM_BSD
		if(c == SOCKERR(ENOTCONN) && !пусто_ли(connection_start) && msecs(connection_start) < 20000)
			return true;
#endif
	return c == SOCKERR(EWOULDBLOCK) || c == SOCKERR(EAGAIN);
#endif
#ifdef PLATFORM_WIN32
	if(c == SOCKERR(ENOTCONN) && !пусто_ли(connection_start) && msecs(connection_start) < 20000) {
		LLOG("ENOTCONN issue");
		return true;
	}
	return c == SOCKERR(EWOULDBLOCK);	       
#endif
}

int TcpSocket::RawSend(const void *buf, int amount)
{
#ifdef PLATFORM_LINUX
	int res = send(socket, (const char *)buf, amount, MSG_NOSIGNAL);
#else
	int res = send(socket, (const char *)buf, amount, 0);
#endif
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res == 0 || res < 0)
		SetSockError("send");
	return res;
}

int TcpSocket::Send(const void *buf, int amount)
{
	if(SSLHandshake())
		return 0;
	return ssl ? ssl->Send(buf, amount) : RawSend(buf, amount);
}

void TcpSocket::Shutdown()
{
	ПРОВЕРЬ(открыт());
	if(shutdown(socket, SD_SEND))
		SetSockError("shutdown(SD_SEND)");
}

Ткст TcpSocket::GetHostName()
{
	иниц();
	char буфер[256];
	gethostname(буфер, __countof(буфер));
	return буфер;
}

bool TcpSocket::IsGlobalTimeout()
{
	if(!пусто_ли(global_timeout) && msecs() - start_time > global_timeout) {
		SetSockError("wait", ERROR_GLOBAL_TIMEOUT, "Timeout");
		return true;
	}
	return false;
}

bool TcpSocket::RawWait(dword flags, int end_time)
{ // wait till end_time
	LLOG("RawWait end_time: " << end_time << ", текущ time " << msecs() << ", to wait: " << end_time - msecs());
	is_timeout = false;
	if((flags & WAIT_READ) && ptr != end)
		return true;
	if(socket == INVALID_SOCKET)
		return false;
	for(;;) {
		if(ошибка_ли() || IsAbort())
			return false;
		int to = end_time - msecs();
		if(WhenWait)
			to = waitstep;
		timeval *tvalp = NULL;
		timeval tval;
		if(end_time != INT_MAX || WhenWait) {
			to = max(to, 0);
			tval.tv_sec = to / 1000;
			tval.tv_usec = 1000 * (to % 1000);
			tvalp = &tval;
			LLOG("RawWait timeout: " << to);
		}
		fd_set fdsetr[1], fdsetw[1], fdsetx[1];;
		FD_ZERO(fdsetr);
		if(flags & WAIT_READ)
			FD_SET(socket, fdsetr);
		FD_ZERO(fdsetw);
		if(flags & WAIT_WRITE)
			FD_SET(socket, fdsetw);
		FD_ZERO(fdsetx);
		FD_SET(socket, fdsetx);
		int avail = select((int)socket + 1, fdsetr, fdsetw, fdsetx, tvalp);
		LLOG("жди select avail: " << avail);
		if(avail < 0 && GetErrorCode() != SOCKERR(EINTR)) {
			SetSockError("wait");
			return false;
		}
		if(avail > 0) {
		#if defined(PLATFORM_WIN32) || defined(PLATFORM_BSD)
			connection_start = Null;
		#endif
			return true;
		}
		if(IsGlobalTimeout() || to <= 0 && timeout) {
			is_timeout = true;
			return false;
		}
		WhenWait();
		if(timeout == 0) {
			is_timeout = true;
			return false;
		}
	}
}

TcpSocket& TcpSocket::GlobalTimeout(int ms)
{
	start_time = msecs();
	global_timeout = ms;
	return *this;
}

bool TcpSocket::жди(dword flags, int end_time)
{
	return ssl ? ssl->жди(flags, end_time) : RawWait(flags, end_time);
}

int  TcpSocket::GetEndTime() const
{ // Compute time limit for operation, based on global timeout and per-operation timeout настройки
	int o = min(пусто_ли(global_timeout) ? INT_MAX : start_time + global_timeout,
	            пусто_ли(timeout) ? INT_MAX : msecs() + timeout);
#if defined(PLATFORM_WIN32) || defined(PLATFORM_BSD)
	if(GetErrorCode() == SOCKERR(ENOTCONN) && !пусто_ли(connection_start))
		if(msecs(connection_start) < 20000)
			o = connection_start + 20000;
#endif
	return o;
}

bool TcpSocket::жди(dword flags)
{
	return жди(flags, GetEndTime());
}

int TcpSocket::помести(const void *s_, int length)
{
	LLOG("помести " << socket << ": " << length);
	ПРОВЕРЬ(открыт());
	const char *s = (const char *)s_;
	if(length < 0 && s)
		length = (int)strlen(s);
	if(!s || length <= 0 || ошибка_ли() || IsAbort())
		return 0;
	done = 0;
	bool подбери = false;
	int end_time = GetEndTime();
	while(done < length) {
		if(подбери && !жди(WAIT_WRITE, end_time))
			return done;
		подбери = false;
		int count = Send(s + done, length - done);
		if(ошибка_ли() || timeout == 0 && count == 0 && подбери)
			return done;
		if(count > 0)
			done += count;
		else
			подбери = true;
	}
	LLOG("//помести() -> " << done);
	return done;
}

bool TcpSocket::PutAll(const void *s, int len)
{
	if(помести(s, len) != len) {
		if(!ошибка_ли())
			SetSockError("GePutAll", -1, "timeout");
		return false;
	}
	return true;
}

bool TcpSocket::PutAll(const Ткст& s)
{
	if(помести(s) != s.дайСчёт()) {
		if(!ошибка_ли())
			SetSockError("GePutAll", -1, "timeout");
		return false;
	}
	return true;
}

int TcpSocket::RawRecv(void *buf, int amount)
{
	int res = recv(socket, (char *)buf, amount, 0);
	if(res == 0)
		is_eof = true;
	else
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res < 0)
		SetSockError("recv");
	LLOG("recv(" << socket << "): " << res << " bytes: "
	     << какТкстСи((char *)buf, (char *)buf + min(res, 16))
	     << (res ? "" : кф_ли() ? ", EOF" : ", WOULDBLOCK"));
	return res;
}

int TcpSocket::Recv(void *буфер, int maxlen)
{
	if(SSLHandshake())
		return 0;
	return ssl ? ssl->Recv(буфер, maxlen) : RawRecv(буфер, maxlen);
}

void TcpSocket::ReadBuffer(int end_time)
{
	ptr = end = буфер;
	if(жди(WAIT_READ, end_time))
		end = буфер + Recv(буфер, BUFFERSIZE);
}

bool TcpSocket::кф_ли() const
{
	return is_eof && ptr == end || IsAbort() || !открыт() || ошибка_ли();
}

int TcpSocket::Get_()
{
	if(!открыт() || ошибка_ли() || кф_ли() || IsAbort())
		return -1;
	ReadBuffer(GetEndTime());
	return ptr < end ? (byte)*ptr++ : -1;
}

int TcpSocket::Peek_(int end_time)
{
	if(!открыт() || ошибка_ли() || кф_ли() || IsAbort())
		return -1;
	ReadBuffer(end_time);
	return ptr < end ? (byte)*ptr : -1;
}

int TcpSocket::Peek_()
{
	return Peek_(GetEndTime());
}

int TcpSocket::дай(void *буфер, int count)
{
	LLOG("дай " << count);

	if(!открыт() || ошибка_ли() || кф_ли() || IsAbort())
		return 0;
	
	int l = (int)(end - ptr);
	done = 0;
	if(l > 0) {
		if(l < count) {
			memcpy(буфер, ptr, l);
			done += l;
			ptr = end;
		}
		else {
			memcpy(буфер, ptr, count);
			ptr += count;
			return count;
		}
	}
	int end_time = GetEndTime();
	while(done < count && !ошибка_ли() && !кф_ли()) {
		if(!жди(WAIT_READ, end_time))
			break;
		int part = Recv((char *)буфер + done, count - done);
		if(part > 0)
			done += part;
		if(timeout == 0)
			break;
	}
	return done;
}

Ткст TcpSocket::дай(int count)
{
	if(count == 0)
		return Null;
	ТкстБуф out(count);
	int done = дай(out, count);
	if(!done && кф_ли())
		return Ткст::дайПроц();
	out.устДлину(done);
	return Ткст(out);
}

bool  TcpSocket::дайВсе(void *буфер, int len)
{
	if(дай(буфер, len) == len)
		return true;
	if(!ошибка_ли())
		SetSockError("дайВсе", -1, "timeout");
	return false;
}

Ткст TcpSocket::дайВсе(int len)
{
	Ткст s = дай(len);
	if(s.дайСчёт() != len) {
		if(!ошибка_ли())
			SetSockError("дайВсе", -1, "timeout");
		return Ткст::дайПроц();
	}
	return s;
}

Ткст TcpSocket::дайСтроку(int maxlen)
{
	LLOG("дайСтроку " << maxlen << ", iseof " << кф_ли());
	Ткст ln;
	int end_time = GetEndTime();
	for(;;) {
		if(кф_ли())
			return Ткст::дайПроц();
		int c = подбери(end_time);
		if(c < 0) {
			if(!ошибка_ли()) {
				if(msecs() > end_time)
					SetSockError("дайСтроку", -1, "timeout");
				else
					continue;
			}
			return Ткст::дайПроц();
		}
		дай();
		if(c == '\n')
			return ln;
		if(ln.дайСчёт() >= maxlen) {
			if(!ошибка_ли())
				SetSockError("дайСтроку", -1, "maximal length exceeded");
			return Ткст::дайПроц();
		}
		if(c != '\r')
			ln.конкат(c);
	}
}

void TcpSocket::SetSockError(const char *context, int код_, const char *errdesc)
{
	errorcode = код_;
	errordesc.очисть();
	if(socket != INVALID_SOCKET)
		errordesc << "socket(" << (int)socket << ") / ";
	errordesc << context << ": " << errdesc;
	is_error = true;
	LLOG("Ошибка " << errordesc);
}

void TcpSocket::SetSockError(const char *context, const char *errdesc)
{
	SetSockError(context, GetErrorCode(), errdesc);
}

void TcpSocket::SetSockError(const char *context)
{
	SetSockError(context, TcpSocketErrorDesc(GetErrorCode()));
}

TcpSocket::SSL *(*TcpSocket::CreateSSL)(TcpSocket& socket);

bool TcpSocket::StartSSL()
{
	ПРОВЕРЬ(открыт());
	if(!CreateSSL) {
		SetSockError("StartSSL", -1, "Missing SSL support (DinrusC/SSL)");
		return false;
	}
	if(!открыт()) {
		SetSockError("StartSSL", -1, "Socket is not open");
		return false;
	}
	if(mode != CONNECT && mode != ACCEPT) {
		SetSockError("StartSSL", -1, "Socket is not connected");
		return false;
	}
	ssl = (*CreateSSL)(*this);
	if(!ssl->старт()) {
		ssl.очисть();
		return false;
	}
	ssl_start = msecs();
	SSLHandshake();
	return true;
}

dword TcpSocket::SSLHandshake()
{
	if(ssl && (mode == CONNECT || mode == ACCEPT)) {
		dword w = ssl->Handshake();
		if(w) {
			if(msecs(ssl_start) > 20000) {
				SetSockError("ssl handshake", ERROR_SSLHANDSHAKE_TIMEOUT, "Timeout");
				return false;
			}
			if(IsGlobalTimeout())
				return false;
			жди(w);
			return ssl->Handshake();
		}
	}
	return 0;
}

void TcpSocket::SSLCertificate(const Ткст& cert_, const Ткст& pkey_, bool asn1_)
{
	cert = cert_;
	pkey = pkey_;
	asn1 = asn1_;
}

void TcpSocket::SSLServerNameIndication(const Ткст& имя)
{
	sni = имя;
}

void TcpSocket::очисть()
{
	сотриОш();
	if(открыт())
		открой();
	переустанов();
}

int SocketWaitСобытие::жди(int timeout)
{
	FD_ZERO(read);
	FD_ZERO(write);
	FD_ZERO(exception);
	int maxindex = -1;
	for(int i = 0; i < socket.дайСчёт(); i++) {
		const Кортеж<int, dword>& s = socket[i];
		if(s.a >= 0) {
			const Кортеж<int, dword>& s = socket[i];
			if(s.b & WAIT_READ)
				FD_SET(s.a, read);
			if(s.b & WAIT_WRITE)
				FD_SET(s.a, write);
			FD_SET(s.a, exception);
			maxindex = max(s.a, maxindex);
		}
	}
	timeval *tvalp = NULL;
	timeval tval;
	if(!пусто_ли(timeout)) {
		tval.tv_sec = timeout / 1000;
		tval.tv_usec = 1000 * (timeout % 1000);
		tvalp = &tval;
	}
	return select(maxindex + 1, read, write, exception, tvalp);
}

dword SocketWaitСобытие::дай(int i) const
{
	int s = socket[i].a;
	if(s < 0)
		return 0;
	dword events = 0;
	if(FD_ISSET(s, read))
		events |= WAIT_READ;
	if(FD_ISSET(s, write))
		events |= WAIT_WRITE;
	if(FD_ISSET(s, exception))
		events |= WAIT_IS_EXCEPTION;
	return events;
}

SocketWaitСобытие::SocketWaitСобытие()
{
	FD_ZERO(read);
	FD_ZERO(write);
	FD_ZERO(exception);
}

}
