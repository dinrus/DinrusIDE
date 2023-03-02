class Ssh {
public:
    void                Abort()                                 { ssh->status = ABORTED; }
    Ssh&                WaitStep(int ms)                        { ssh->waitstep = clamp(ms, 0, INT_MAX); return *this; }
    int                 GetTimeout() const                      { return ssh->timeout; }
    int                 GetWaitStep() const                     { return ssh->waitstep; }
    bool                InProgress() const                      { return ssh->status == WORKING; }
    bool                ошибка_ли() const                         { return ssh->status == FAILED || ssh->status == ABORTED; }
    int                 дайОш() const                        { return ssh->Ошибка.дай<int>(); }
    Ткст              GetErrorDesc() const                    { return ssh->Ошибка.дай<Ткст>(); }
    int64               дайИд() const                           { return ssh->oid;   }
    int                 дайТип() const                         { return ssh->otype; }
    template <class T>  T&   To()                               { auto* t = dynamic_cast<T*>(this); ПРОВЕРЬ(t); return *t; }
    template <class T>  bool является() const                         { return dynamic_cast<const T*>(this); }

    operator bool() const                                       { return ssh; }

    Событие<>             WhenWait;
    
    static void         Trace(bool b = true)                    { SSH::sTrace = b; }
    static void         TraceVerbose(int level)                 { Trace((bool)level); SSH::sTraceVerbose = level; }

    Ssh();
    virtual ~Ssh();

    Ssh(Ssh&&) = default;

    struct Ошибка : Искл {
        int code;
        Ошибка() : Искл(Null), code(-1) {}
        Ошибка(const Ткст& reason) : Искл(reason), code(-1) {}
        Ошибка(int rc, const Ткст& reason) : Искл(reason), code(rc) {}
    };
    enum Type  { CORE, SESSION, SFTP, CHANNEL, SCP, EXEC, SHELL, TUNNEL };

protected:
    struct CoreData {
        LIBSSH2_SESSION*    session;
        TcpSocket*          socket;
        Кортеж<int, Ткст>  Ошибка;
        Событие<>             whenwait;
        bool                init;
        int64               oid;
        int                 otype;
        int                 timeout;
        int                 start_time;
        int                 waitstep;
        int                 chunk_size;
        int                 status;
    };
    Один<CoreData> ssh;

    const int           CHUNKSIZE = 1024 * 64;

    enum Статус         { IDLE, WORKING, FAILED, ABORTED };

    virtual bool        иниц()                                  { return true; }
    virtual void        выход()                                  {}
    void                жди();
    bool                пуск(Врата<>&& фн, bool abortable = true);
    bool                WouldBlock(int rc)                      { return rc == LIBSSH2_ERROR_EAGAIN; }
    bool                WouldBlock()                            { return ssh->session && WouldBlock(libssh2_session_last_errno(ssh->session)); }
    bool                таймаут_ли() const                       { return !пусто_ли(ssh->timeout) && ssh->timeout > 0 &&  msecs(ssh->start_time) >= ssh->timeout; }
    void                выведиОш(int rc, const Ткст& reason = Null);
    void                устОш(int rc, const Ткст& reason);
    void                UpdateClient()                          { WhenWait  ? WhenWait() : ssh->whenwait(); }
    
private:
    static int64        GetNewId();

};
