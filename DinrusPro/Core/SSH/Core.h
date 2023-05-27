class Ssh {
public:
    проц                Abort()                                 { ssh->status = ABORTED; }
    Ssh&                WaitStep(цел ms)                        { ssh->waitstep = clamp(ms, 0, INT_MAX); return *this; }
    цел                 GetTimeout() const                      { return ssh->timeout; }
    цел                 GetWaitStep() const                     { return ssh->waitstep; }
    бул                InProgress() const                      { return ssh->status == WORKING; }
    бул                ошибка_ли() const                         { return ssh->status == FAILED || ssh->status == ABORTED; }
    цел                 дайОш() const                        { return ssh->Ошибка.дай<цел>(); }
    Ткст              GetErrorDesc() const                    { return ssh->Ошибка.дай<Ткст>(); }
    дол               дайИд() const                           { return ssh->oid;   }
    цел                 дайТип() const                         { return ssh->otype; }
    template <class T>  T&   To()                               { auto* t = dynamic_cast<T*>(this); ПРОВЕРЬ(t); return *t; }
    template <class T>  бул является() const                         { return dynamic_cast<const T*>(this); }

    operator бул() const                                       { return ssh; }

    Событие<>             WhenWait;
    
    static проц         Trace(бул b = true)                    { SSH::sTrace = b; }
    static проц         TraceVerbose(цел level)                 { Trace((бул)level); SSH::sTraceVerbose = level; }

    Ssh();
    virtual ~Ssh();

    Ssh(Ssh&&) = default;

    struct Ошибка : Искл {
        цел код_;
        Ошибка() : Искл(Null), код_(-1) {}
        Ошибка(const Ткст& reason) : Искл(reason), код_(-1) {}
        Ошибка(цел rc, const Ткст& reason) : Искл(reason), код_(rc) {}
    };
    enum Type  { CORE, SESSION, SFTP, CHANNEL, SCP, EXEC, SHELL, TUNNEL };

protected:
    struct CoreData {
        LIBSSH2_SESSION*    session;
        TcpSocket*          socket;
        Кортеж<цел, Ткст>  Ошибка;
        Событие<>             whenwait;
        бул                init;
        дол               oid;
        цел                 otype;
        цел                 timeout;
        цел                 start_time;
        цел                 waitstep;
        цел                 chunk_size;
        цел                 status;
    };
    Один<CoreData> ssh;

    const цел           CHUNKSIZE = 1024 * 64;

    enum Статус         { IDLE, WORKING, FAILED, ABORTED };

    virtual бул        иниц()                                  { return true; }
    virtual проц        выход()                                  {}
    проц                жди();
    бул                пуск(Врата<>&& фн, бул abortable = true);
    бул                WouldBlock(цел rc)                      { return rc == LIBSSH2_ERROR_EAGAIN; }
    бул                WouldBlock()                            { return ssh->session && WouldBlock(libssh2_session_last_errno(ssh->session)); }
    бул                таймаут_ли() const                       { return !пусто_ли(ssh->timeout) && ssh->timeout > 0 &&  msecs(ssh->start_time) >= ssh->timeout; }
    проц                выведиОш(цел rc, const Ткст& reason = Null);
    проц                устОш(цел rc, const Ткст& reason);
    проц                UpdateClient()                          { WhenWait  ? WhenWait() : ssh->whenwait(); }
    
private:
    static дол        GetNewId();

};
