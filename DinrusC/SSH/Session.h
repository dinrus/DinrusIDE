class SshSession : public Ssh {
public:
    enum Methods : int {
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

    enum Phase : int {
        PHASE_DNS,
        PHASE_CONNECTION,
        PHASE_HANDSHAKE,
        PHASE_AUTHORIZATION,
        PHASE_SUCCESS
    };
    
public:
    SshSession&         Timeout(int ms)                         { ssh->timeout = ms; return *this; }
 
    SshSession&         Compression(bool b = true)              { session->compression = b; return *this; }
    SshSession&         NoCompression()                         { return Compression(false); }

    SshSession&         Keys(const Ткст& prikey, const Ткст& pubkey, const Ткст& phrase, bool fromfile = true);
    SshSession&         Method(int тип, Значение method)          { session->iomethods(тип) << pick(method); return *this; }
    SshSession&         Methods(МапЗнач methods)               { session->iomethods = pick(methods); return *this; }
 
    SshSession&         PasswordAuth()                          { session->authmethod = PASSWORD;  return *this; }
    SshSession&         PublicKeyAuth()                         { session->authmethod = PUBLICKEY; return *this; }
    SshSession&         HostBasedAuth()                         { session->authmethod = HOSTBASED; return *this; }
    SshSession&         KeyboardAuth()                          { session->authmethod = KEYBOARD;  return *this; }
    SshSession&         AgentAuth()                             { session->authmethod = SSHAGENT;  return *this; }
 
    LIBSSH2_SESSION*    дайУк()                             { return ssh->session; }
    
    Ткст              GetBanner() const                       { return ssh->session ? pick(Ткст(libssh2_session_banner_get(ssh->session))) : Null; }
    Ткст              GetMD5Fingerprint() const               { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_MD5, 16);    }
    Ткст              GetSHA1Fingerprint() const              { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_SHA1, 20);   }
    Ткст              GetSHA256Fingerprint() const            { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_SHA256, 32); }
    Вектор<Ткст>      GetAuthMethods()                        { return pick(разбей(session->authmethods, ',')); }
    TcpSocket&          GetSocket()                             { return session->socket;  }
    МапЗнач            GetMethods() const;

    SFtp                CreateSFtp();
    SshChannel          CreateChannel();
    SshExec             CreateExec();
    Scp                 CreateScp();
    SshTunnel           CreateTunnel();
    SshShell            CreateShell();

    bool                Connect(const Ткст& url);
    bool                Connect(const Ткст& host, int port, const Ткст& user, const Ткст& password);
    void                Disconnect();
    
    Событие<>             WhenConfig;
    Событие<>             WhenAuth;
    Функция<Ткст()>  WhenPasswordChange;
    Событие<int>          WhenPhase;
    Врата<Ткст, int>   WhenVerify;
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
    void                выход() override;
    Ткст              GetHostKeyHash(int тип, int length) const;
    Ткст              GetMethodNames(int тип) const;
    int                 TryAgent(const Ткст& username);
    void                FreeAgent(SshAgent* agent);
    
    struct SessionData {
        TcpSocket       socket;
        Ткст          fingerprint;
        int             hashtype;
        Ткст          authmethods;
        int             authmethod;
        Ткст          prikey;
        Ткст          pubkey;
        bool            keyfile;
        Ткст          phrase;
        МапЗнач        iomethods;
        bool            connected;
        bool            compression;
    };
    Один<SessionData> session;

    enum AuthMethod     { PASSWORD, PUBLICKEY, HOSTBASED, KEYBOARD, SSHAGENT };
    enum HostkeyType    { RSAKEY, DSSKEY };
};
