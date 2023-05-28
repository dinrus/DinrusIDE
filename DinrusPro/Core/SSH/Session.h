class SshSession : public Ssh {
public:
    enum Methods : цел {
        METHOD_EXCHANGE = 0,
        METHOD_HOSTKEY,
        METHOD_CENCRYPTION,
        METHOD_SENCRYPTION,
        METHOD_CMAC,
        METHOD_SMAC,
        METHOD_CCOMPRESSION,
        METHOD_SCOMPRESSION,
        METHOD_CLANGUAGE,
        METHOD_SLANGUAGE
    };

    enum Phase : цел {
        PHASE_DNS,
        PHASE_CONNECTION,
        PHASE_HANDSHAKE,
        PHASE_AUTHORIZATION,
        PHASE_SUCCESS
    };
    
public:
    SshSession&         Timeout(цел ms)                         { ssh->timeout = ms; return *this; }
 
    SshSession&         Compression(бул b = true)              { session->compression = b; return *this; }
    SshSession&         NoCompression()                         { return Compression(false); }

    SshSession&         Keys(const Ткст& prikey, const Ткст& pubkey, const Ткст& phrase, бул fromfile = true);
    SshSession&         Method(цел тип, Значение method)          { session->iomethods(тип) << пикуй(method); return *this; }
    SshSession&         Methods(МапЗнач methods)               { session->iomethods = пикуй(methods); return *this; }
 
    SshSession&         PasswordAuth()                          { session->authmethod = PASSWORD;  return *this; }
    SshSession&         PublicKeyAuth()                         { session->authmethod = PUBLICKEY; return *this; }
    SshSession&         HostBasedAuth()                         { session->authmethod = HOSTBASED; return *this; }
    SshSession&         KeyboardAuth()                          { session->authmethod = KEYBOARD;  return *this; }
    SshSession&         AgentAuth()                             { session->authmethod = SSHAGENT;  return *this; }
 
    LIBSSH2_SESSION*    дайУк()                             { return ssh->session; }
    
    Ткст              GetBanner() const                       { return ssh->session ? пикуй(Ткст(libssh2_session_banner_get(ssh->session))) : Null; }
    Ткст              GetMD5Fingerprint() const               { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_MD5, 16);    }
    Ткст              GetSHA1Fingerprint() const              { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_SHA1, 20);   }
    Ткст              GetSHA256Fingerprint() const            { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_SHA256, 32); }
    Вектор<Ткст>      GetAuthMethods()                        { return пикуй(разбей(session->authmethods, ',')); }
    TcpSocket&          GetSocket()                             { return session->socket;  }
    МапЗнач            GetMethods() const;

    SFtp                CreateSFtp();
    SshChannel          CreateChannel();
    SshExec             CreateExec();
    Scp                 CreateScp();
    SshTunnel           CreateTunnel();
    SshShell            CreateShell();

    бул                Connect(const Ткст& url);
    бул                Connect(const Ткст& host, цел port, const Ткст& user, const Ткст& password);
    проц                Disconnect();
    
    Событие<>             WhenConfig;
    Событие<>             WhenAuth;
    Функция<Ткст()>  WhenPasswordChange;
    Событие<цел>          WhenPhase;
    Врата<Ткст, цел>   WhenVerify;
    Врата<>              WhenProxy;
    Событие<SshX11Handle> WhenX11;
    Функция<Ткст(Ткст, Ткст, Ткст)>  WhenKeyboard;
    
    SshSession();
    virtual ~SshSession();

    SshSession(SshSession&&) = default;

    // DEPRECATED stuff.
    enum Хэш           { HASH_MD5, HASH_SHA1, HASH_SHA256 };
    [[deprecated("Will be removed with U++ 2021.2. Use GetxxxFingerprint() methods instead.")]]
	Ткст      GetFingerprint() const              { return session->fingerprint; }
    [[deprecated("Will be removed with U++ 2021.2. Use GetxxxFingerprint() methods instead.")]]
    SshSession& HashType(Хэш h)                    { session->hashtype = h; return *this; }

private:
    проц                выход() override;
    Ткст              GetHostKeyHash(цел тип, цел length) const;
    Ткст              GetMethodNames(цел тип) const;
    цел                 TryAgent(const Ткст& username);
    проц                FreeAgent(SshAgent* agent);
    
    struct SessionData {
        TcpSocket       socket;
        Ткст          fingerprint;
        цел             hashtype;
        Ткст          authmethods;
        цел             authmethod;
        Ткст          prikey;
        Ткст          pubkey;
        бул            keyfile;
        Ткст          phrase;
        МапЗнач        iomethods;
        бул            connected;
        бул            compression;
    };
    Один<SessionData> session;

    enum AuthMethod     { PASSWORD, PUBLICKEY, HOSTBASED, KEYBOARD, SSHAGENT };
    enum HostkeyType    { RSAKEY, DSSKEY };
};
