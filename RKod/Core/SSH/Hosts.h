class SshHosts {
public:
    struct Info {
        Ткст          ключ;
        int             тип;
        int             status;

        bool            IsRSA() const                   { return тип == LIBSSH2_KNOWNHOST_KEY_SSHRSA;       }
        bool            IsDSS() const                   { return тип == LIBSSH2_KNOWNHOST_KEY_SSHDSS;       }
        bool            IsECDSA256() const              { return тип == LIBSSH2_KNOWNHOST_KEY_ECDSA_256;    }
        bool            IsECDSA384() const              { return тип == LIBSSH2_KNOWNHOST_KEY_ECDSA_384;    }
        bool            IsECDSA521() const              { return тип == LIBSSH2_KNOWNHOST_KEY_ECDSA_521;    }
        bool            IsED25519() const               { return тип == LIBSSH2_KNOWNHOST_KEY_ED25519;      }
        bool            IsUnknown() const;

        bool            IsFailure() const               { return status == LIBSSH2_KNOWNHOST_CHECK_FAILURE;  }
        bool            IsNotFound() const              { return status == LIBSSH2_KNOWNHOST_CHECK_NOTFOUND; }
        bool            IsMismatch() const              { return status == LIBSSH2_KNOWNHOST_CHECK_MISMATCH; }
        bool            IsMatch() const                 { return status == LIBSSH2_KNOWNHOST_CHECK_MATCH;    }
    };

public:
    bool                добавь(const Ткст& host, int port, const Info& инфо, const Ткст& comment);
    bool                добавь(const Ткст& host, const Info& инфо, const Ткст& comment);
    bool                удали(SshHost* host);
    bool                грузи(const Ткст& имяф);
    bool                сохрани();
    bool                SaveAs(const Ткст& имяф);
    Info                Check(const Ткст& host, int port);

    Вектор<SshHost*>    GetHosts();

    int                 дайОш() const                { return Ошибка.a; }
    Ткст              GetErrorDesc() const            { return Ошибка.b; }

    SshHosts(SshSession& session);
    virtual ~SshHosts();

private:
    bool                Ошибка();
    void                очисть()                         { Ошибка.a = 0; Ошибка.b = Null; }

    Ткст              file_path;
    Кортеж<int,Ткст>   Ошибка;
    LIBSSH2_SESSION*    ssh_session;
    LIBSSH2_KNOWNHOSTS* handle;
};
