#include <DinrusPro/DinrusCore.h>

#ifdef PLATFORM_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef PLATFORM_POSIX
#include <arpa/inet.h>
#endif

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#define LLOG(x)  // LOG("TCP " << x)

IpAddrInfo::Entry IpAddrInfo::пул[IpAddrInfo::COUNT];

Стопор IpAddrInfoPoolMutex;

проц IpAddrInfo::EnterPool()
{
	IpAddrInfoPoolMutex.войди();
}

проц IpAddrInfo::LeavePool()
{
	IpAddrInfoPoolMutex.выйди();
}

цел sGetAddrInfo(кткст0 host, кткст0 port, цел family, addrinfo **result)
{
	if(!host || !*host)
		return EAI_NONAME;
	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	const static цел FamilyToAF[] = { AF_UNSPEC, AF_INET, AF_INET6 };
	hints.ai_family = FamilyToAF[(family > 0 && family < __количество(FamilyToAF)) ? family : 0];
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	return getaddrinfo(host, port, &hints, result);
}

auxthread_t auxthread__ IpAddrInfo::Нить(ук укз)
{
	Entry *entry = (Entry *)укз;
	EnterPool();
	if(entry->status == WORKING) {
		сим host[1025];
		сим port[257];
		цел family = entry->family;
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

бул IpAddrInfo::выполни(const Ткст& host, цел port, цел family)
{
	очисть();
	entry = exe;
	addrinfo *result;
	entry->addr = sGetAddrInfo(~host, ~какТкст(port), family, &result) == 0 ? result : NULL;
	entry->status = entry->addr ? RESOLVED : FAILED;
	return entry->addr;
}

проц IpAddrInfo::старт()
{
	if(entry)
		return;
	EnterPool();
	for(цел i = 0; i < COUNT; i++) {
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

проц IpAddrInfo::старт(const Ткст& host_, цел port_, цел family_)
{
	очисть();
	port = какТкст(port_);
	host = host_;
	family = family_;
	старт();
}

бул IpAddrInfo::InProgress()
{
	if(!entry) {
		старт();
		return true;
	}
	EnterPool();
	цел s = entry->status;
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

проц IpAddrInfo::очисть()
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

кткст0 TcpSocketErrorDesc(цел код_)
{
	return strerror(код_);
}

цел TcpSocket::GetErrorCode()
{
	return errno;
}

#else

#define SOCKERR(x) WSA##x

кткст0 TcpSocketErrorDesc(цел код_)
{
	static Кортеж<цел, кткст0 > err[] = {
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
		{ WSAECONNABORTED,          "Software caused connection аборт." },
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
	const Кортеж<цел, кткст0 > *x = найдиКортеж(err, __количество(err), код_);
	return x ? x->b : "Unknown Ошибка код_.";
}

цел TcpSocket::GetErrorCode()
{
	return WSAGetLastError();
}

#endif

проц TcpSocketИниt()
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

проц TcpSocket::иниц()
{
	TcpSocketИниt();
}

проц TcpSocket::переустанов()
{
	LLOG("переустанов");
	is_eof = false;
	socket = INVALID_SOCKET;
	ipv6 = false;
	укз = end = буфер;
	is_error = false;
	is_аборт = false;
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

бул TcpSocket::SetupSocket()
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

бул TcpSocket::открой(цел family, цел тип, цел protocol)
{
	иниц();
	открой();
	сотриОш();
	if((socket = ::socket(family, тип, protocol)) == INVALID_SOCKET) {
		SetSockError("open");
		return false;
	}
	LLOG("TcpSocket::Данные::открой() -> " << (цел)socket);
	return SetupSocket();
}

бул TcpSocket::Listen(цел port, цел listen_count, бул ipv6_, бул reuse, ук addr)
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
		sin.sin_addr.s_addr = addr?(*(бцел*)addr):htonl(INADDR_ANY);
	}
	if(reuse) {
		цел optval = 1;
		setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (кткст0 )&optval, sizeof(optval));
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

бул TcpSocket::Listen(const IpAddrInfo& addr, цел port, цел listen_count, бул ipv6, бул reuse)
{
	addrinfo *a = addr.дайРез();
	return Listen(port, listen_count, ipv6, reuse, &(((sockaddr_in*)a->ai_addr)->sin_addr.s_addr));
}

бул TcpSocket::прими(TcpSocket& ls)
{
	открой();
	иниц();
	переустанов();
	ПРОВЕРЬ(ls.открыт());
	цел et = GetEndTime();
	for(;;) {
		цел h = ls.GetTimeout();
		бул b = ls.Timeout(timeout).жди(WAIT_READ, et);
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
	сим h[200];
	return inet_ntop(AF_INET, &addr.sin_addr, h, 200);
#endif
}

проц TcpSocket::NoDelay()
{
	ПРОВЕРЬ(открыт());
	цел __true = 1;
	LLOG("NoDelay(" << (цел)socket << ")");
	if(setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (кткст0 )&__true, sizeof(__true)))
		SetSockError("setsockopt(TCP_NODELAY)");
}

проц TcpSocket::Linger(цел msecs)
{
	ПРОВЕРЬ(открыт());
	linger ls;
	ls.l_onoff = !пусто_ли(msecs) ? 1 : 0;
	ls.l_linger = !пусто_ли(msecs) ? (msecs + 999) / 1000 : 0;
	if(setsockopt(socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<кткст0 >(&ls), sizeof(ls)))
		SetSockError("setsockopt(SO_LINGER)");
}

проц TcpSocket::прикрепи(SOCKET s)
{
	открой();
	socket = s;
}

бул TcpSocket::RawConnect(addrinfo *arp)
{
	if(!arp) {
		SetSockError("connect", -1, "not found");
		return false;
	}
	Ткст err;
	for(цел pass = 0; pass < 2; pass++) {
		addrinfo *rp = arp;
		while(rp) {
			if(rp->ai_family == AF_INET == !pass && // Try to connect IPv4 in the first pass
			   открой(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) {
				if(connect(socket, rp->ai_addr, (цел)rp->ai_addrlen) == 0 ||
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


бул TcpSocket::Connect(IpAddrInfo& инфо)
{
	LLOG("Connect addrinfo");
	иниц();
	переустанов();
	addrinfo *result = инфо.дайРез();
	return RawConnect(result);
}

бул TcpSocket::Connect(кткст0 host, цел port)
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

бул TcpSocket::WaitConnect()
{
	if(WaitWrite()) {
		цел optval = 0;
		socklen_t optlen = sizeof(optval);
		if (getsockopt(GetSOCKET(), SOL_SOCKET, SO_ERROR, (сим*)&optval, &optlen) == 0) {
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

проц TcpSocket::RawClose()
{
	LLOG("close " << (цел)socket);
	if(socket != INVALID_SOCKET) {
		цел res;
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

проц TcpSocket::открой()
{
	if(ssl)
		ssl->открой();
	else
		RawClose();
	ssl.очисть();
}

бул TcpSocket::WouldBlock()
{
	цел c = GetErrorCode();
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

цел TcpSocket::RawSend(кук buf, цел amount)
{
#ifdef PLATFORM_LINUX
	цел res = send(socket, (кткст0 )buf, amount, MSG_NOSIGNAL);
#else
	цел res = send(socket, (кткст0 )buf, amount, 0);
#endif
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res == 0 || res < 0)
		SetSockError("send");
	return res;
}

цел TcpSocket::Send(кук buf, цел amount)
{
	if(SSLHandshake())
		return 0;
	return ssl ? ssl->Send(buf, amount) : RawSend(buf, amount);
}

проц TcpSocket::Shutdown()
{
	ПРОВЕРЬ(открыт());
	if(shutdown(socket, SD_SEND))
		SetSockError("shutdown(SD_SEND)");
}

Ткст TcpSocket::GetHostName()
{
	иниц();
	сим буфер[256];
	gethostname(буфер, __количество(буфер));
	return буфер;
}

бул TcpSocket::IsGlobalTimeout()
{
	if(!пусто_ли(global_timeout) && msecs() - start_time > global_timeout) {
		SetSockError("wait", ERROR_GLOBAL_TIMEOUT, "Timeout");
		return true;
	}
	return false;
}

бул TcpSocket::RawWait(бцел flags, цел end_time)
{ // wait till end_time
	LLOG("RawWait end_time: " << end_time << ", текущ time " << msecs() << ", to wait: " << end_time - msecs());
	is_timeout = false;
	if((flags & WAIT_READ) && укз != end)
		return true;
	if(socket == INVALID_SOCKET)
		return false;
	for(;;) {
		if(ошибка_ли() || аборт_ли())
			return false;
		цел to = end_time - msecs();
		if(WhenWait)
			to = waitstep;
		timeval *tvalp = NULL;
		timeval tval;
		if(end_time != INT_MAX || WhenWait) {
			to = макс(to, 0);
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
		цел avail = select((цел)socket + 1, fdsetr, fdsetw, fdsetx, tvalp);
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

TcpSocket& TcpSocket::GlobalTimeout(цел ms)
{
	start_time = msecs();
	global_timeout = ms;
	return *this;
}

бул TcpSocket::жди(бцел flags, цел end_time)
{
	return ssl ? ssl->жди(flags, end_time) : RawWait(flags, end_time);
}

цел  TcpSocket::GetEndTime() const
{ // Compute time limit for operation, based on global timeout and per-operation timeout настройки
	цел o = мин(пусто_ли(global_timeout) ? INT_MAX : start_time + global_timeout,
	            пусто_ли(timeout) ? INT_MAX : msecs() + timeout);
#if defined(PLATFORM_WIN32) || defined(PLATFORM_BSD)
	if(GetErrorCode() == SOCKERR(ENOTCONN) && !пусто_ли(connection_start))
		if(msecs(connection_start) < 20000)
			o = connection_start + 20000;
#endif
	return o;
}

бул TcpSocket::жди(бцел flags)
{
	return жди(flags, GetEndTime());
}

цел TcpSocket::помести(кук s_, цел length)
{
	LLOG("помести " << socket << ": " << length);
	ПРОВЕРЬ(открыт());
	кткст0 s = (кткст0 )s_;
	if(length < 0 && s)
		length = (цел)strlen(s);
	if(!s || length <= 0 || ошибка_ли() || аборт_ли())
		return 0;
	done = 0;
	бул подбери = false;
	цел end_time = GetEndTime();
	while(done < length) {
		if(подбери && !жди(WAIT_WRITE, end_time))
			return done;
		подбери = false;
		цел count = Send(s + done, length - done);
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

бул TcpSocket::PutAll(кук s, цел len)
{
	if(помести(s, len) != len) {
		if(!ошибка_ли())
			SetSockError("GePutAll", -1, "timeout");
		return false;
	}
	return true;
}

бул TcpSocket::PutAll(const Ткст& s)
{
	if(помести(s) != s.дайСчёт()) {
		if(!ошибка_ли())
			SetSockError("GePutAll", -1, "timeout");
		return false;
	}
	return true;
}

цел TcpSocket::RawRecv(ук buf, цел amount)
{
	цел res = recv(socket, (сим *)buf, amount, 0);
	if(res == 0)
		is_eof = true;
	else
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res < 0)
		SetSockError("recv");
	LLOG("recv(" << socket << "): " << res << " bytes: "
	     << какТкстСи((сим *)buf, (сим *)buf + мин(res, 16))
	     << (res ? "" : кф_ли() ? ", EOF" : ", WOULDBLOCK"));
	return res;
}

цел TcpSocket::Recv(ук буфер, цел maxlen)
{
	if(SSLHandshake())
		return 0;
	return ssl ? ssl->Recv(буфер, maxlen) : RawRecv(буфер, maxlen);
}

проц TcpSocket::ReadBuffer(цел end_time)
{
	укз = end = буфер;
	if(жди(WAIT_READ, end_time))
		end = буфер + Recv(буфер, BUFFERSIZE);
}

бул TcpSocket::кф_ли() const
{
	return is_eof && укз == end || аборт_ли() || !открыт() || ошибка_ли();
}

цел TcpSocket::Get_()
{
	if(!открыт() || ошибка_ли() || кф_ли() || аборт_ли())
		return -1;
	ReadBuffer(GetEndTime());
	return укз < end ? (ббайт)*укз++ : -1;
}

цел TcpSocket::Peek_(цел end_time)
{
	if(!открыт() || ошибка_ли() || кф_ли() || аборт_ли())
		return -1;
	ReadBuffer(end_time);
	return укз < end ? (ббайт)*укз : -1;
}

цел TcpSocket::Peek_()
{
	return Peek_(GetEndTime());
}

цел TcpSocket::дай(ук буфер, цел count)
{
	LLOG("дай " << count);

	if(!открыт() || ошибка_ли() || кф_ли() || аборт_ли())
		return 0;
	
	цел l = (цел)(end - укз);
	done = 0;
	if(l > 0) {
		if(l < count) {
			memcpy(буфер, укз, l);
			done += l;
			укз = end;
		}
		else {
			memcpy(буфер, укз, count);
			укз += count;
			return count;
		}
	}
	цел end_time = GetEndTime();
	while(done < count && !ошибка_ли() && !кф_ли()) {
		if(!жди(WAIT_READ, end_time))
			break;
		цел part = Recv((сим *)буфер + done, count - done);
		if(part > 0)
			done += part;
		if(timeout == 0)
			break;
	}
	return done;
}

Ткст TcpSocket::дай(цел count)
{
	if(count == 0)
		return Null;
	ТкстБуф out(count);
	цел done = дай(out, count);
	if(!done && кф_ли())
		return Ткст::дайПроц();
	out.устДлину(done);
	return Ткст(out);
}

бул  TcpSocket::дайВсе(ук буфер, цел len)
{
	if(дай(буфер, len) == len)
		return true;
	if(!ошибка_ли())
		SetSockError("дайВсе", -1, "timeout");
	return false;
}

Ткст TcpSocket::дайВсе(цел len)
{
	Ткст s = дай(len);
	if(s.дайСчёт() != len) {
		if(!ошибка_ли())
			SetSockError("дайВсе", -1, "timeout");
		return Ткст::дайПроц();
	}
	return s;
}

Ткст TcpSocket::дайСтроку(цел maxlen)
{
	LLOG("дайСтроку " << maxlen << ", iseof " << кф_ли());
	Ткст ln;
	цел end_time = GetEndTime();
	for(;;) {
		if(кф_ли())
			return Ткст::дайПроц();
		цел c = подбери(end_time);
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
			ln.кат(c);
	}
}

проц TcpSocket::SetSockError(кткст0 context, цел код_, кткст0 errdesc)
{
	errorcode = код_;
	errordesc.очисть();
	if(socket != INVALID_SOCKET)
		errordesc << "socket(" << (цел)socket << ") / ";
	errordesc << context << ": " << errdesc;
	is_error = true;
	LLOG("Ошибка " << errordesc);
}

проц TcpSocket::SetSockError(кткст0 context, кткст0 errdesc)
{
	SetSockError(context, GetErrorCode(), errdesc);
}

проц TcpSocket::SetSockError(кткст0 context)
{
	SetSockError(context, TcpSocketErrorDesc(GetErrorCode()));
}

TcpSocket::SSL *(*TcpSocket::CreateSSL)(TcpSocket& socket);

бул TcpSocket::StartSSL()
{
	ПРОВЕРЬ(открыт());
	if(!CreateSSL) {
		SetSockError("StartSSL", -1, "Missing SSL support (RKod/SSL)");
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

бцел TcpSocket::SSLHandshake()
{
	if(ssl && (mode == CONNECT || mode == ACCEPT)) {
		бцел w = ssl->Handshake();
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

проц TcpSocket::SSLCertificate(const Ткст& cert_, const Ткст& pkey_, бул asn1_)
{
	cert = cert_;
	pkey = pkey_;
	asn1 = asn1_;
}

проц TcpSocket::SSLServerNameIndication(const Ткст& имя)
{
	sni = имя;
}

проц TcpSocket::очисть()
{
	сотриОш();
	if(открыт())
		открой();
	переустанов();
}

цел SocketWaitСобытие::жди(цел timeout)
{
	FD_ZERO(read);
	FD_ZERO(write);
	FD_ZERO(exception);
	цел maxindex = -1;
	for(цел i = 0; i < socket.дайСчёт(); i++) {
		const Кортеж<цел, бцел>& s = socket[i];
		if(s.a >= 0) {
			const Кортеж<цел, бцел>& s = socket[i];
			if(s.b & WAIT_READ)
				FD_SET(s.a, read);
			if(s.b & WAIT_WRITE)
				FD_SET(s.a, write);
			FD_SET(s.a, exception);
			maxindex = макс(s.a, maxindex);
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

бцел SocketWaitСобытие::дай(цел i) const
{
	цел s = socket[i].a;
	if(s < 0)
		return 0;
	бцел events = 0;
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
