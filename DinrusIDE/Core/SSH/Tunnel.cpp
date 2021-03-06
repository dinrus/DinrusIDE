#include "SSH.h"

namespace РНЦПДинрус {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool SshTunnel::пригоден()
{
	bool b = false;
	
	switch(mode) {
		case CONNECT:	b = channel || listener; break;
		case LISTEN:	b = channel || listener; break;
		case ACCEPT:	b = channel; break;
		default: NEVER();
	}
	if(b)
		устОш(-1, "Invalid channel instance.");
	return !b;
}

void SshTunnel::выход()
{
	if(!listener)
		return;
	
	пуск([=]() mutable{
		int rc = libssh2_channel_forward_cancel(*listener);
		if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
		if(rc == 0)	listener.очисть();
		return !rc;
	});
}

bool SshTunnel::Connect(const Ткст& host, int port)
{
	mode = CONNECT;
	
	if(!пригоден())
		return false;

	return пуск([=]() mutable{
		LIBSSH2_CHANNEL *ch = libssh2_channel_direct_tcpip(ssh->session, host , port);
		if(!ch && !WouldBlock()) выведиОш(-1);
		if(ch) {
			channel = сделайОдин<LIBSSH2_CHANNEL*>(ch);
			LLOG("Direct tcp-ip connection to " << host << ":" << port << " is established.");
		}
		return ch;
	});
}

bool SshTunnel::Connect(const Ткст& url)
{
	UrlInfo u(url);
	if(!u.host.пустой() && u.port.пустой())
		return Connect(u.host, тктЦел(u.port));
	устОш(-1, "Malformed proxy connection URL.");
	return false;
}

bool SshTunnel::Listen(const Ткст& host, int port, int* bound_port, int listen_count)
{
	mode = LISTEN;

	if(!пригоден())
		return false;
	
	return пуск([=]() mutable {
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

bool SshTunnel::прими(SshTunnel& listener)
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
}