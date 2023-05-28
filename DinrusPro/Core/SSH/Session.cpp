#include "SSH.h"
#include "разместпам.cpp"

namespace ДинрусРНЦП {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)   do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

// sKeyboardCallback: Authenticates a session, using keyboard-interactive authentication.

static проц sKeyboardCallback(кткст0 имя, цел name_len, кткст0 instruction,
	цел instruction_len, цел num_prompts, const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
	LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, проц **abstract)
{
	SshSession *session = reinterpret_cast<SshSession*>(*abstract);
	for(auto i = 0; i < num_prompts; i++) {
		auto response = session->WhenKeyboard(
			Ткст(имя, name_len),
			Ткст(instruction, instruction_len),
			Ткст(prompts[i].text, prompts[i].length)
		);
#ifdef КУЧА_РНЦП
		auto *r = (char*) ssh_malloc(response.дайДлину(), abstract);
		memcpy(r, response.старт(), response.дайДлину());
#else
		auto *r = strdup(~response);
#endif
		if(r) {
			responses[i].text   = r;
			responses[i].length = response.дайДлину();
		}
	}
}

// sChangePasswordCallback: Requests that the client's password be changed.

static проц sChangePasswordCallback(LIBSSH2_SESSION *session, char **pwd, цел *len, проц **abstract)
{
	Ткст newpwd = reinterpret_cast<SshSession*>(*abstract)->WhenPasswordChange();
#ifdef КУЧА_РНЦП
		*pwd = (char*) ssh_malloc(newpwd.дайДлину(), abstract);
		memcpy(*pwd, ~newpwd, newpwd.дайДлину());
#else
		*pwd = strdup(~newpwd);
#endif
}

// sX11RequestCallback: Dispatches incoming X11 requests.

static проц sX11RequestCallback(LIBSSH2_SESSION *session, LIBSSH2_CHANNEL *channel, char *shost, цел sport, проц **abstract)
{
	reinterpret_cast<SshSession*>(*abstract)->WhenX11((SshX11Handle) channel);
}

// slibssh2DebugCallback: Allows full-level logging (redirection) of libsssh2 diagnostic messages.

#ifdef flagLIBSSH2TRACE
static проц slibssh2DebugCallback(LIBSSH2_SESSION *session, ук context, кткст0 данные, т_мера length)
{
	if(!session  || !SSH::sTraceVerbose)
		return;
	auto* ssh_obj = reinterpret_cast<SshSession*>(context);
	RLOG(SSH::дайИмя(ssh_obj->дайТип(), ssh_obj->дайИд()) << Ткст(данные, дол(length)));
}
#endif

проц SshSession::выход()
{
	if(!session)
		return;

	пуск([=]() mutable {
		if(!ssh->session)
			return true;
		цел rc = libssh2_session_disconnect(ssh->session, "Disconnecting...");
		if(WouldBlock(rc))
			return false;
		LLOG("Successfully disconnected from the server.");
		return true;
	});

	пуск([=]() mutable {
		if(!ssh->session)
			return true;
		цел rc = libssh2_session_free(ssh->session);
		if(WouldBlock(rc))
			return false;
		ssh->init    = false;
		ssh->socket  = nullptr;
		ssh->session = nullptr;
		session->connected = false;
		LLOG("Session handles freed.");
		return true;
	}, false);
}

бул SshSession::Connect(const Ткст& url)
{
	UrlInfo u(url);

	auto b = найдиарг(u.scheme, "ssh", "sftp", "scp", "exec") >= 0 || (u.scheme.пустой() && !u.host.пустой());
	цел port = (u.port.пустой() || !b) ? 22 : тктЦел(u.port);
	if(b) return Connect(u.host, port, u.username, u.password);
	устОш(-1, "Malformed secure shell URL.");
	return false;
}

бул SshSession::Connect(const Ткст& host, цел port, const Ткст& user, const Ткст& password)
{
	IpAddrInfo ipinfo;

	if(!пуск([=, &ipinfo] () mutable {
		if(host.пустой())
			выведиОш(-1, "Хост не указан.");
		ssh->session = nullptr;
		session->socket.Timeout(0);
		if(!WhenProxy) {
			ipinfo.старт(host, port);
			LLOG(фмт("Starting DNS sequence locally for %s:%d", host, port));
		}
		else
			LLOG("прокси plugin found. Attempting to connect via proxy...");
		WhenPhase(WhenProxy ? PHASE_CONNECTION : PHASE_DNS);
		return true;
	})) goto Bailout;

	if(!WhenProxy) {
		if(!пуск([=, &ipinfo] () mutable {
			if(ipinfo.InProgress())
				return false;
			if(!ipinfo.дайРез())
				выведиОш(-1, "DNS lookup failed.");
			WhenPhase(PHASE_CONNECTION);
			return true;
		})) goto Bailout;

		if(!пуск([=, &ipinfo] () mutable {
			if(!session->socket.Connect(ipinfo))
				return false;
			ipinfo.очисть();
			return true;
		})) goto Bailout;

		if(!пуск([=, &ipinfo] () mutable {
			if(!session->socket.WaitConnect())
				return false;
			LLOG("Successfully connected to " << host <<":" << port);
			return true;
		})) goto Bailout;
	}
	else {
		if(!пуск([=] () mutable {
			if(!WhenProxy())
				выведиОш(-1, "Неудачная попытка подключения к прокси.");
			LLOG("прокси connection to " << host << ":" << port << " is successful.");
			return true;
		})) goto Bailout;
	}

	if(!пуск([=] () mutable {
#ifdef КУЧА_РНЦП
			LLOG("Using ДинрусРНЦП's memory managers.");
			ssh->session = libssh2_session_init_ex((*ssh_malloc), (*ssh_free), (*ssh_realloc), this);
#else
			LLOG("Using libssh2's memory managers.");
			ssh->session = libssh2_session_init_ex(nullptr, nullptr, nullptr, this);
#endif
			if(!ssh->session)
				выведиОш(-1, "Failed to initalize libssh2 session.");
#ifdef flagLIBSSH2TRACE
			if(libssh2_trace_sethandler(ssh->session, this, &slibssh2DebugCallback))
				LLOG("Warning: Unable to set trace (debug) handler for libssh2.");
			else {
				libssh2_trace(ssh->session, SSH::sTraceVerbose);
				LLOG("Verbose debugging mode enabled.");
			}
#endif
			libssh2_session_set_blocking(ssh->session, 0);
			ssh->socket = &session->socket;
			LLOG("Session successfully initialized.");
			WhenConfig();
			libssh2_session_flag(ssh->session, LIBSSH2_FLAG_COMPRESS, (цел) session->compression);
			LLOG("Compression is " << (session->compression ? "enabled." : "disabled."));
			WhenPhase(PHASE_HANDSHAKE);
			return true;
	})) goto Bailout;

	while(!session->iomethods.пустой()) {
		if(!пуск([=] () mutable {
			цел    method = session->iomethods.дайКлюч(0);
			Ткст mnames = GetMethodNames(method);
			цел rc = libssh2_session_method_pref(ssh->session, method, ~mnames);
			if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
			if(!rc && !session->iomethods.пустой()) {
				LLOG("Transport method: #" << method << " is set to [" << mnames << "]");
				session->iomethods.удали(0);
			}
			return !rc;
		})) goto Bailout;
	}

	if(!пуск([=] () mutable {
			цел rc = libssh2_session_handshake(ssh->session, session->socket.GetSOCKET());
			if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
			if(!rc) {
				LLOG("Handshake successful.");
				WhenPhase(PHASE_AUTHORIZATION);
			}
			return !rc;
	})) goto Bailout;

	if(!пуск([=] () mutable {
			switch(session->hashtype) {  // TODO: удали this block along with the deprecated Hashtype()
			case HASH_MD5:               //       and  GetFingerprint() methods, in the future versions.
				session->fingerprint = GetMD5Fingerprint();
				LLOG("MD5 fingerprint of " << host << ": " << гексТкст(session->fingerprint, 1, ':'));
				break;
			case HASH_SHA1:
				session->fingerprint = GetSHA1Fingerprint();
				LLOG("SHA1 fingerprint of " << host << ": " << гексТкст(session->fingerprint, 1, ':'));
				break;
			case HASH_SHA256:
				session->fingerprint = GetSHA256Fingerprint();
				LLOG("SHA256 fingerprint of " << host << ": " << Base64Encode(session->fingerprint));
				break;
			default:
				break;
			}
			if(WhenVerify && !WhenVerify(host, port))
				выведиОш(-1);
			return true;
	})) goto Bailout;

	if(!пуск([=] () mutable {
			session->authmethods = libssh2_userauth_list(ssh->session, user, user.дайДлину());
			if(пусто_ли(session->authmethods)) {
				if(libssh2_userauth_authenticated(ssh->session)) {
					LLOG("Server @" << host << " does not require authentication!");
					WhenPhase(PHASE_SUCCESS);
					session->connected = true;
					return session->connected;
				}
				else
				if(!WouldBlock())
					выведиОш(-1);
				return false;
			}
			LLOG("Authentication methods list successfully retrieved: [" << session->authmethods << "]");
			WhenAuth();
			return true;
	})) goto Bailout;

	if(session->connected)
		goto Finalize;

	if(!пуск([=] () mutable {
			цел rc = -1;
			switch(session->authmethod) {
				case PASSWORD:
					rc = libssh2_userauth_password_ex(
							ssh->session,
							~user,
							 user.дайДлину(),
							~password,
							 password.дайДлину(),
							 WhenPasswordChange
								? &sChangePasswordCallback
									: nullptr);
					break;
				case PUBLICKEY:
					rc = session->keyfile
					?	libssh2_userauth_publickey_fromfile(
							ssh->session,
							~user,
							~session->pubkey,
							~session->prikey,
							~session->phrase)
					:	libssh2_userauth_publickey_frommemory(
							ssh->session,
							~user,
							 user.дайДлину(),
							~session->pubkey,
							 session->pubkey.дайДлину(),
							~session->prikey,
							 session->prikey.дайДлину(),
							~session->phrase);
					break;
				case HOSTBASED:
					if(!session->keyfile)
						выведиОш(-1, "Keys cannot be loaded from memory.");
					else
					rc = libssh2_userauth_hostbased_fromfile(
							ssh->session,
							~user,
							~session->pubkey,
							~session->prikey,
							~session->phrase,
							~host);
					break;
				case KEYBOARD:
					rc = libssh2_userauth_keyboard_interactive(
						ssh->session,
						~user,
						&sKeyboardCallback);
					break;
				case SSHAGENT:
					rc = TryAgent(user);
					break;
				default:
					НИКОГДА();

			}
			if(rc != 0 && !WouldBlock(rc))
				выведиОш(rc);
			if(rc == 0 && libssh2_userauth_authenticated(ssh->session)) {
				LLOG("Client succesfully authenticated.");
				WhenPhase(PHASE_SUCCESS);
				session->connected = true;
			}
			return	session->connected;
	})) goto Bailout;

Finalize:
#ifdef PLATFORM_POSIX
	libssh2_session_обрвыз_set(ssh->session, LIBSSH2_CALLBACK_X11, (проц*) sX11RequestCallback);
	LLOG("X11 dispatcher is set.");
#endif
	return true;

Bailout:
	LLOG("Connection attempt failed. Bailing out...");
	выход();
	return false;
}

проц SshSession::Disconnect()
{
	выход();
}

SFtp SshSession::CreateSFtp()
{
	ПРОВЕРЬ(ssh && ssh->session);
	return пикуй(SFtp(*this));
}

SshChannel SshSession::CreateChannel()
{
	ПРОВЕРЬ(ssh && ssh->session);
	return пикуй(SshChannel(*this));
}

SshExec SshSession::CreateExec()
{
	ПРОВЕРЬ(ssh && ssh->session);
	return пикуй(SshExec(*this));
}

Scp SshSession::CreateScp()
{
	ПРОВЕРЬ(ssh && ssh->session);
	return пикуй(Scp(*this));
}

SshTunnel SshSession::CreateTunnel()
{
	ПРОВЕРЬ(ssh && ssh->session);
	return пикуй(SshTunnel(*this));
}

SshShell SshSession::CreateShell()
{
	ПРОВЕРЬ(ssh && ssh->session);
	return пикуй(SshShell(*this));
}

Ткст SshSession::GetHostKeyHash(цел тип, цел length) const
{
	Ткст hash;
	if(ssh->session) {
		hash = libssh2_hostkey_hash(ssh->session, тип);
		if(hash.дайДлину() > length)
			hash.обрежьПоследн(hash.дайДлину() - length);
	}
	return hash;
}

МапЗнач SshSession::GetMethods() const
{
	МапЗнач methods;
	if(ssh->session) {
		for(цел i = METHOD_EXCHANGE; i <= METHOD_SLANGUAGE; i++) {
			const char **p = nullptr;
			auto rc = libssh2_session_supported_algs(ssh->session, i, &p);
			if(rc > 0) {
				auto& v = methods(i);
				for(цел j = 0; j < rc; j++) {
					v << p[j];
				}
				libssh2_free(ssh->session, p);
			}
		}
	}
	return пикуй(methods);
}

Ткст SshSession::GetMethodNames(цел тип) const
{
	Ткст names;
	const Значение& v = session->iomethods[тип];
	if(массивЗнач_ли(v)) {
		for(цел i = 0; i < v.дайСчёт(); i++)
			names << v[i].To<Ткст>() << (i < v.дайСчёт() - 1 ? "," : "");
	}
	else names << v;
	return пикуй(names);
}

цел SshSession::TryAgent(const Ткст& username)
{
	LLOG("Attempting to authenticate via ssh-agent...");
	auto agent = libssh2_agent_init(ssh->session);
	if(!agent)
		выведиОш(-1, "Couldn't initialize ssh-agent support.");
	if(libssh2_agent_connect(agent)) {
		libssh2_agent_free(agent);
		выведиОш(-1, "Couldn't connect to ssh-agent.");
	}
	if(libssh2_agent_list_identities(agent)) {
		FreeAgent(agent);
		выведиОш(-1, "Couldn't request identities to ssh-agent.");
	}
	libssh2_agent_publickey *ид = nullptr, *previd = nullptr;

	for(;;) {
		auto rc = libssh2_agent_get_identity(agent, &ид, previd);
		if(rc < 0) {
			FreeAgent(agent);
			выведиОш(-1, "Unable to obtain identity from ssh-agent.");
		}
		if(rc != 1) {
			if(libssh2_agent_userauth(agent, ~username, ид)) {
				LLOG(фмт("Authentication with username %s and public ключ %s failed.",
							username, ид->comment));
			}
			else {
				LLOG(фмт("Authentication with username %s and public ключ %s succeesful.",
							username, ид->comment));
				break;
			}
		}
		else {
			FreeAgent(agent);
			выведиОш(-1, "Couldn't authenticate via ssh-agent");
		}
		previd = ид;
	}
	FreeAgent(agent);
	return 0;
}

проц SshSession::FreeAgent(SshAgent* agent)
{
	libssh2_agent_disconnect(agent);
	libssh2_agent_free(agent);
}


SshSession& SshSession::Keys(const Ткст& prikey, const Ткст& pubkey, const Ткст& phrase, бул fromfile)
{
    session->prikey  = prikey;
    session->pubkey  = pubkey;
    session->phrase  = phrase;
    session->keyfile = fromfile;
    return *this;
}

SshSession::SshSession()
: Ssh()
{
    session.создай();
    ssh->otype           = SESSION;
    ssh->whenwait        = прокси(WhenWait);
    session->authmethod  = PASSWORD;
    session->connected   = false;
    session->keyfile     = true;
    session->compression = false;
    session->hashtype    = HASH_SHA256;
 }

SshSession::~SshSession()
{
	выход();
}
}