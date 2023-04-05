class SshChannel : public Ssh {
public:
    SshChannel&         Timeout(int ms)                                                     { ssh->timeout = ms; return *this; }
    SshChannel&         ChunkSize(int sz)                                                   { ssh->chunk_size = clamp(sz, 1, INT_MAX); return *this; }

    LIBSSH2_CHANNEL*    дайУк() const                                                   { return channel ? *channel : nullptr; }
    int                 GetDone() const                                                     { return done; }

    bool                открыт() const                                                      { return channel; }

    bool                открой();
    bool                открой();
    bool                WaitClose();

    bool                Request(const Ткст& request, const Ткст& params = Null);
    bool                RequestExec(const Ткст& cmdline)                                  { return Request("exec", cmdline); }
    bool                RequestShell()                                                      { return Request("shell", Null); }
    bool                RequestSubsystem(const Ткст& subsystem)                           { return Request("subsystem", subsystem); }
    bool                RequestTerminal(const Ткст& term, int width, int height, const Ткст& tmodes);
    bool                RequestTerminal(const Ткст& term, размер sz, const Ткст& tmodes)   { return RequestTerminal(term, sz.cx, sz.cy, tmodes); }
    bool                SetEnv(const Ткст& ПЕРЕМЕННАЯ, const Ткст& значение);

    int                 дай(void *ptr, int size, int sid = 0);
    Ткст              дай(int size, int sid = 0);
    Ткст              дайСтроку(int maxlen = 65536, int sid = 0);
    Ткст              GetStdErr(int size)                                                 { return дай(size, SSH_EXTENDED_DATA_STDERR); }
    int                 помести(const void *ptr, int size, int sid = 0);
    int                 помести(const Ткст& s, int sid = 0)                                   { return помести(~s, s.дайДлину(), sid); }
    int                 PutStdErr(const Ткст& err)                                        { return помести(err, SSH_EXTENDED_DATA_STDERR); }

    bool                PutEof();
    bool                GetEof();
    bool                PutGetEof()                                                         { return PutEof() && GetEof(); }
    bool                кф_ли();

    bool                SetTerminalSize(int width, int height);
    bool                SetTerminalSize(размер sz)                                            { return SetTerminalSize(sz.cx, sz.cy); }
    bool                SetReadWindowSize(uint32 size, bool force = false);
    uint32              GetReadWindowSize()                                                 { return libssh2_channel_window_read(*channel); }
    uint32              GetWriteWindowSize()                                                { return libssh2_channel_window_write(*channel); }
    int                 дайКодВыхода();
    Ткст              GetExitSignal();

    SshChannel(SshSession& session);
    virtual ~SshChannel();

    SshChannel(SshChannel&&) = default;

protected:
    bool                иниц() override;
    void                выход() override;
    int                 читай(void *ptr, int size, int sid = 0);
    int                 читай(int sid = 0);
    int                 пиши(const void *ptr, int size, int sid = 0);
    bool                пиши(char c, int sid = 0);
    bool                SetWndSz(uint32 size, bool force = false);
    int                 SetPtySz(int w, int h);
    int                 SetPtySz(размер sz)                                                   { return SetPtySz(sz.cx, sz.cy); }


    dword               СобытиеWait(int fd, dword events, int tv = 10);
    bool                обработайСобытия(Ткст& input);
    virtual void        ReadWrite(Ткст& in, const void* out, int out_len)                 {}

    bool                Shut(const Ткст& msg, bool nowait = true);
    
    Один<LIBSSH2_CHANNEL*>  channel;
    int                    done;
};

class SshExec : public SshChannel {
public:
    int выполни(const Ткст& cmd, Ткст& out, Ткст& err);
    int operator()(const Ткст& cmd, Ткст& out, Ткст& err)                             { return выполни(cmd, out, err); }

    SshExec(SshSession& session) : SshChannel(session)                                      { ssh->otype = EXEC; };
private:
	void ReadStdOut(Ткст& out);
	void ReadStdErr(Ткст& err);
};

int     SshExecute(SshSession& session, const Ткст& cmd, Ткст& out, Ткст& err);
int     SshExecute(SshSession& session, const Ткст& cmd, Ткст& out);
Ткст  SshExecute(SshSession& session, const Ткст& cmd);

class Scp : public SshChannel {
public:
    bool   сохраниФайл(const char *path, const Ткст& данные);
    Ткст загрузиФайл(const char *path);
    bool   сохраниФайл(const char *path, Поток& in);
    bool   загрузиФайл(Поток& out, const char *path);

    Врата<int64, int64> WhenProgress;
    
    Scp(SshSession& session) : SshChannel(session)                                          { ssh->otype = SCP; ssh->init = true; }

private:
    bool   OpenRead(const Ткст& path, ScpAttrs& attrs);
    bool   OpenWrite(const Ткст& path, int64 size, long mode = 0644);
    bool   грузи(Поток& s, ScpAttrs a, int64 maxsize = INT64_MAX);
    bool   сохрани(Поток& s);
};

class SshTunnel : public SshChannel {
public:
    bool Connect(const Ткст& host, int port);
    bool Connect(const Ткст& url);
    bool Listen(int port, int listen_count = 5)                                             { return Listen(Null, port, nullptr, listen_count); }
    bool Listen(const Ткст& host, int port, int* bound_port, int listen_count = 5);
    bool прими(SshTunnel& listener);

    SshTunnel(SshSession& session) : SshChannel(session)                                    { ssh->otype = TUNNEL; mode = NONE; ssh->init = true; }
    virtual ~SshTunnel()                                                                    { выход(); }

    SshTunnel(SshTunnel&&) = default;

private:
    void выход() override;
    bool пригоден();

    int  mode;
    Один<LIBSSH2_LISTENER*>  listener;

    enum Режимы { NONE, CONNECT, LISTEN, ACCEPT };
};


class SshShell : public SshChannel {
public:
    bool        пуск(const Ткст& terminal, размер pagesize, const Ткст& tmodes = Null)                 { return пуск(GENERIC, terminal, pagesize, tmodes);  }
    bool        пуск(const Ткст& terminal, int width, int height, const Ткст& tmodes = Null)         { return пуск(GENERIC, terminal, {width, height}, tmodes); }
    
    bool        Консоль(const Ткст& terminal, const Ткст& tmodes = Null)                            { return пуск(CONSOLE, terminal, GetConsolePageSize(), tmodes); }

    SshShell&   ForwardX11(const Ткст& host = Null, int дисплей = 0, int screen = 0, int bufsize = 1024 * 1024);
    bool        AcceptX11(SshX11Handle xhandle);

    void        Send(int c)                     { queue.конкат(c);   }
    void        Send(const char* s)             { Send(Ткст(s));}
    void        Send(const Ткст& s)           { queue.конкат(s);   }

    SshShell&   PageSize(размер sz)               { resized = sz != psize; if(resized) psize = sz; return *this;}
    размер        GetPageSize() const             { return psize; }
    размер        GetConsolePageSize();

    Событие<>                  WhenInput;
    Событие<const void*, int>  WhenOutput;

    SshShell(SshSession& session);
    virtual ~SshShell();

    SshShell(SshShell&&) = default;

protected:
    void    ReadWrite(Ткст& in, const void* out, int out_len) override;
    virtual bool пуск(int mode_, const Ткст& terminal, размер pagesize, const Ткст& tmodes);

    void    Resize();
    bool    консИниц();
    void    консЧмтай();
    void    консПиши(const void* буфер, int len);
    void    ConsoleRawMode(bool b = true);

    bool    х11Иниц();
    void    х11Цикл();

    enum Режимы { GENERIC, CONSOLE };

private:
    Ткст  queue;
    размер    psize;
    int     mode;
    bool    rawmode;
    bool    resized;
    bool    xenabled;
#ifdef PLATFORM_WIN32
    DWORD   tflags;
    HANDLE  stdinput;
    HANDLE  stdoutput;
#elif  PLATFORM_POSIX
    termios tflags;
    byte    xdisplay;
    byte    xscreen;
    Ткст  xhost;
    Буфер<char> xbuffer;
    int          xbuflen;
    Вектор<Кортеж<SshX11Handle, SOCKET>> xrequests;
#endif
};