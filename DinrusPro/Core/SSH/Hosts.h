class SshHosts {
public:
    struct Info {
        Ткст          ключ;
        цел             тип;
        цел             status;

        бул            IsRSA() const                   { return тип == LIBSSH2_KNOWNHOST_KEY_SSHRSA;       }
        бул            IsDSS() const                   { return тип == LIBSSH2_KNOWNHOST_KEY_SSHDSS;       }
        бул            IsECDSA256() const              { return тип == LIBSSH2_KNOWNHOST_KEY_ECDSA_256;    }
        бул            IsECDSA384() const              { return тип == LIBSSH2_KNOWNHOST_KEY_ECDSA_384;    }
        бул            IsECDSA521() const              { return тип == LIBSSH2_KNOWNHOST_KEY_ECDSA_521;    }
        бул            IsED25519() const               { return тип == LIBSSH2_KNOWNHOST_KEY_ED25519;      }
        бул            IsUnknown() const;

        бул            IsFailure() const               { return status == LIBSSH2_KNOWNHOST_CHECK_FAILURE;  }
        бул            IsNotFound() const              { return status == LIBSSH2_KNOWNHOST_CHECK_NOTFOUND; }
        бул            IsMismatch() const              { return status == LIBSSH2_KNOWNHOST_CHECK_MISMATCH; }
        бул            IsMatch() const                 { return status == LIBSSH2_KNOWNHOST_CHECK_MATCH;    }
    };

public:
    бул                добавь(const Ткст& host, цел port, const Info& инфо, const Ткст& comment);
    бул                добавь(const Ткст& host, const Info& инфо, const Ткст& comment);
    бул                удали(SshHost* host);
    бул                грузи(const Ткст& имяф);
    бул                сохрани();
    бул                SaveAs(const Ткст& имяф);
    Info                Check(const Ткст& host, цел port);

    Вектор<SshHost*>    GetHosts();

    цел                 дайОш() const                { return Ошибка.a; }
    Ткст              GetErrorDesc() const            { return Ошибка.b; }

    SshHosts(SshSession& session);
    virtual ~SshHosts();

private:
    бул                Ошибка();
    проц                очисть()                         { Ошибка.a = 0; Ошибка.b = Null; }

    Ткст              file_path;
    Кортеж<цел,Ткст>   Ошибка;
    LIBSSH2_SESSION*    ssh_session;
    LIBSSH2_KNOWNHOSTS* handle;
};
