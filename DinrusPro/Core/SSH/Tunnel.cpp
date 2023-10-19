#include "SSH.h"

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

бул SshTunnel::пригоден()
{
	бул b = false;
	
	switch(mode) {
		case CONNECT:	b = channel || listener; break;
		case LISTEN:	b = channel || listener; break;
		case ACCEPT:	b = channel; break;
		default: НИКОГДА();
	}
	if(b)
		устОш(-1, "Invalid channel instance.");
	return !b;
}

проц SshTunnel::выход()
{
	if(!listener)
		return;
	
	пуск([=, this]() mutable{
		цел rc = libssh2_channel_forward_cancel(*listener);
		if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
		if(rc == 0)	listener.очисть();
		return !rc;
	});
}

бул SshTunnel::Connect(const Ткст& host, цел port)
{
	mode = CONNECT;
	
	if(!пригоден())
		return false;

	return пуск([=, this]() mutable{
		LIBSSH2_CHANNEL *ch = libssh2_channel_direct_tcpip(ssh->session, host , port);
		if(!ch && !WouldBlock()) выведиОш(-1);
		if(ch) {
			channel = сделайОдин<LIBSSH2_CHANNEL*>(ch);
			LLOG("Direct tcp-ip connection to " << host << ":" << port << " is established.");
		}
		return ch;
	});
}

бул SshTunnel::Connect(const Ткст& url)
{
	UrlInfo u(url);
	if(!u.host.пустой() && u.port.пустой())
		return Connect(u.host, тктЦел(u.port));
	устОш(-1, "Malformed proxy connection URL.");
	return false;
}

бул SshTunnel::Listen(const Ткст& host, цел port, цел* bound_port, цел listen_count)
{
	mode = LISTEN;

	if(!пригоден())
		return false;
	
	return пуск([=, this]() mutable {
		LIBSSH2_LISTENER *lsn = libssh2_channel_forward_listen_ex(
			ssh->session,
			host.пустой() ? nullptr : ~host,
			port,
			bound_port ? bound_port : nullptr,
			listen_count
		);
		if(!lsn && !WouldBlock())
			выведиОш(-1);
		if(lsn) {
			listener = сделайОдин<LIBSSH2_LISTENER*>(lsn);
			LLOG("Started listening on port #" << port);
		}
		return lsn;
	});
}

бул SshTunnel::прими(SshTunnel& listener)
{
	mode = ACCEPT;

	if(!пригоден())
		return false;
	
	if(!listener.listener) {
		устОш(-1, "Invalid listener.");
		return false;
	}

	return пуск([=, &listener]() mutable {
		LIBSSH2_CHANNEL *ch = libssh2_channel_forward_accept(*listener.listener);
		if(!ch && !WouldBlock()) выведиОш(-1);
		if(ch) {
			channel = сделайОдин<LIBSSH2_CHANNEL*>(ch);
			LLOG("Connection accepted.");
		}
		return ch;
	});
}
