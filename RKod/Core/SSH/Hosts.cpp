#include "SSH.h"

namespace РНЦПДинрус {
	
#define LLOG(x)       do { if(SSH::sTrace) RLOG("SshHosts: " << x); } while(false)
	
bool SshHosts::добавь(const Ткст& host, int port, const Info& инфо, const Ткст& comment)
{
	return добавь(фмт("[%s]:%d", host, port), инфо, comment);
}

bool SshHosts::добавь(const Ткст& host, const Info& инфо, const Ткст& comment)
{
	ПРОВЕРЬ(ssh_session);
	очисть();
	bool b = handle &&
		libssh2_knownhost_addc(
			handle,
			~host,
			nullptr,
			~инфо.ключ, инфо.ключ.дайДлину(),
			~comment, comment.дайДлину(),
			инфо.тип |
			LIBSSH2_KNOWNHOST_TYPE_PLAIN |
			LIBSSH2_KNOWNHOST_KEYENC_RAW,
			nullptr
		) == 0;
	return b ? b : Ошибка();
}

bool SshHosts::удали(SshHost* host)
{
	ПРОВЕРЬ(ssh_session);
	очисть();
	auto b = handle && libssh2_knownhost_del(handle, host) == 0;
	return b ? b : Ошибка();
}

bool SshHosts::грузи(const Ткст& имяф)
{
	ПРОВЕРЬ(ssh_session);
	очисть();
	file_path = имяф;
	auto b = libssh2_knownhost_readfile(handle, ~file_path, LIBSSH2_KNOWNHOST_FILE_OPENSSH) >= 0;
	return b ? b : Ошибка();
}

bool SshHosts::сохрани()
{
	return SaveAs(file_path);
}

bool SshHosts::SaveAs(const Ткст& имяф)
{
	ПРОВЕРЬ(ssh_session);
	очисть();
	auto b = handle && libssh2_knownhost_writefile(handle, ~имяф, LIBSSH2_KNOWNHOST_FILE_OPENSSH) == 0;
	return b ? b : Ошибка();
}

SshHosts::Info SshHosts::Check(const Ткст& host, int port)
{
	ПРОВЕРЬ(ssh_session);
	очисть();
	Info инфо;
	if(handle) {
		int	   тип   = 0;
		size_t length = 0;
		auto*  p = libssh2_session_hostkey(ssh_session, &length, &тип);
		if(!p) {
			Ошибка();
			инфо.status = LIBSSH2_KNOWNHOST_CHECK_FAILURE;
			return pick(инфо);
		}
		инфо.status = libssh2_knownhost_checkp(
			handle,
			~host,
			port,
			p,
			length,
			LIBSSH2_KNOWNHOST_TYPE_PLAIN |
			LIBSSH2_KNOWNHOST_KEYENC_RAW,
			nullptr
		);
		инфо.ключ.уст(p, length);
		switch(тип) {
			case LIBSSH2_HOSTKEY_TYPE_RSA:
				инфо.тип = LIBSSH2_KNOWNHOST_KEY_SSHRSA;
				break;
			case LIBSSH2_HOSTKEY_TYPE_DSS:
				инфо.тип = LIBSSH2_KNOWNHOST_KEY_SSHDSS;
				break;
			case LIBSSH2_HOSTKEY_TYPE_ECDSA_256:
				инфо.тип = LIBSSH2_KNOWNHOST_KEY_ECDSA_256;
				break;
			case LIBSSH2_HOSTKEY_TYPE_ECDSA_384:
				инфо.тип = LIBSSH2_KNOWNHOST_KEY_ECDSA_384;
				break;
			case LIBSSH2_HOSTKEY_TYPE_ECDSA_521:
				инфо.тип = LIBSSH2_KNOWNHOST_KEY_ECDSA_521;
				break;
			case LIBSSH2_HOSTKEY_TYPE_ED25519:
				инфо.тип = LIBSSH2_KNOWNHOST_KEY_ED25519;
				break;
			case LIBSSH2_HOSTKEY_TYPE_UNKNOWN:
			default:
				инфо.тип = LIBSSH2_KNOWNHOST_KEY_UNKNOWN;
				break;
		}
	}
	return pick(инфо);
}

Вектор<SshHost*> SshHosts::GetHosts()
{
	ПРОВЕРЬ(ssh_session);
	очисть();
	Вектор<SshHost*> v;
	SshHost *prev = nullptr, *next = nullptr;
	int rc = libssh2_knownhost_get(handle, &prev, nullptr);
	if(rc >= 0) {
		v.добавь(prev);
		if(rc == 0)
			while(rc < 1) {
				rc = libssh2_knownhost_get(handle, &next, prev);
				if(rc < 0) break;
				v.добавь(next);
				prev = next;
			}
	}
	if(rc < 0) Ошибка();
	return pick(v);
}

bool SshHosts::Ошибка()
{
	ПРОВЕРЬ(ssh_session);
	Буфер<char*> libmsg(256, 0);
	int rc = libssh2_session_last_error(ssh_session, libmsg, nullptr, 0);
	Ошибка.a = rc;
	Ошибка.b = Ткст(*libmsg);
	LLOG("Failed. " << Ошибка.b);
	return false;
}

SshHosts::SshHosts(SshSession& session)
{
	ПРОВЕРЬ(session.дайУк());
	ssh_session   = session.дайУк();
	handle = libssh2_knownhost_init(ssh_session);
}

SshHosts::~SshHosts()
{
	if(ssh_session && handle)
		libssh2_knownhost_free(handle);
}
 
bool SshHosts::Info::IsUnknown() const
{
	return findarg(тип,
		LIBSSH2_KNOWNHOST_KEY_SSHRSA,
		LIBSSH2_KNOWNHOST_KEY_SSHDSS,
		LIBSSH2_KNOWNHOST_KEY_ECDSA_256,
		LIBSSH2_KNOWNHOST_KEY_ECDSA_384,
		LIBSSH2_KNOWNHOST_KEY_ECDSA_521,
		LIBSSH2_KNOWNHOST_KEY_ED25519) < 0;
}
}