class SshChannel : public Ssh {
public:
    SshChannel&         Timeout(цел ms)                                                     { ssh->timeout = ms; return *this; }
    SshChannel&         ChunkSize(цел sz)                                                   { ssh->chunk_size = clamp(sz, 1, INT_MAX); return *this; }

    LIBSSH2_CHANNEL*    дайУк() const                                                   { return channel ? *channel : nullptr; }
    цел                 GetDone() const                                                     { return done; }

    бул                открыт() const                                                      { return channel; }

    бул                открой();
    бул                открой();
    бул                WaitClose();

    бул                Request(const Ткст& request, const Ткст& params = Null);
    бул                RequestExec(const Ткст& cmdline)                                  { return Request("exec", cmdline); }
    бул                RequestShell()                                                      { return Request("shell", Null); }
    бул                RequestSubsystem(const Ткст& subsystem)                           { return Request("subsystem", subsystem); }
    бул                RequestTerminal(const Ткст& term, цел width, цел height, const Ткст& tmodes);
    бул                RequestTerminal(const Ткст& term, размер sz, const Ткст& tmodes)   { return RequestTerminal(term, sz.cx, sz.cy, tmodes); }
    бул                SetEnv(const Ткст& ПЕРЕМЕННАЯ, const Ткст& значение);

    цел                 дай(ук укз, цел size, цел sid = 0);
    Ткст              дай(цел size, цел sid = 0);
    Ткст              дайСтроку(цел maxlen = 65536, цел sid = 0);
    Ткст              GetStdErr(цел size)                                                 { return дай(size, SSH_EXTENDED_DATA_STDERR); }
    цел                 помести(кук укз, цел size, цел sid = 0);
    цел                 помести(const Ткст& s, цел sid = 0)                                   { return помести(~s, s.дайДлину(), sid); }
    цел                 PutStdErr(const Ткст& err)                                        { return помести(err, SSH_EXTENDED_DATA_STDERR); }

    бул                PutEof();
    бул                GetEof();
    бул                PutGetEof()                                                         { return PutEof() && GetEof(); }
    бул                кф_ли();

    бул                SetTerminalSize(цел width, цел height);
    бул                SetTerminalSize(размер sz)                                            { return SetTerminalSize(sz.cx, sz.cy); }
    бул                SetReadWindowSize(бцел size, бул force = false);
    бцел              GetReadWindowSize()                                                 { return libssh2_channel_window_read(*channel); }
    бцел              GetWriteWindowSize()                                                { return libssh2_channel_window_write(*channel); }
    цел                 дайКодВыхода();
    Ткст              GetExitSignal();

    SshChannel(SshSession& session);
    virtual ~SshChannel();

    SshChannel(SshChannel&&) = default;

protected:
    бул                иниц() override;
    проц                выход() override;
    цел                 читай(ук укз, цел size, цел sid = 0);
    цел                 читай(цел sid = 0);
    цел                 пиши(кук укз, цел size, цел sid = 0);
    бул                пиши(сим c, цел sid = 0);
    бул                SetWndSz(бцел size, бул force = false);
    цел                 SetPtySz(цел w, цел h);
    цел                 SetPtySz(размер sz)                                                   { return SetPtySz(sz.cx, sz.cy); }


    бцел               СобытиеWait(цел fd, бцел events, цел tv = 10);
    бул                обработайСобытия(Ткст& input);
    virtual проц        ReadWrite(Ткст& in, кук out, цел out_len)                 {}

    бул                Shut(const Ткст& msg, бул nowait = true);
    
    Один<LIBSSH2_CHANNEL*>  channel;
    цел                    done;
};

class SshExec : public SshChannel {
public:
    цел выполни(const Ткст& cmd, Ткст& out, Ткст& err);
    цел operator()(const Ткст& cmd, Ткст& out, Ткст& err)                             { return выполни(cmd, out, err); }

    SshExec(SshSession& session) : SshChannel(session)                                      { ssh->otype = EXEC; };
private:
	проц ReadStdOut(Ткст& out);
	проц ReadStdErr(Ткст& err);
};

цел     SshExecute(SshSession& session, const Ткст& cmd, Ткст& out, Ткст& err);
цел     SshExecute(SshSession& session, const Ткст& cmd, Ткст& out);
Ткст  SshExecute(SshSession& session, const Ткст& cmd);

class Scp : public SshChannel {
public:
    бул   сохраниФайл(кткст0 path, const Ткст& данные);
    Ткст загрузиФайл(кткст0 path);
    бул   сохраниФайл(кткст0 path, Поток& in);
    бул   загрузиФайл(Поток& out, кткст0 path);

    Врата<дол, дол> WhenProgress;
    
    Scp(SshSession& session) : SshChannel(session)                                          { ssh->otype = SCP; ssh->init = true; }

private:
    бул   OpenRead(const Ткст& path, ScpAttrs& attrs);
    бул   OpenWrite(const Ткст& path, дол size, long mode = 0644);
    бул   грузи(Поток& s, ScpAttrs a, дол maxsize = ЦЕЛ64_МАКС);
    бул   сохрани(Поток& s);
};

class SshTunnel : public SshChannel {
public:
    бул Connect(const Ткст& host, цел port);
    бул Connect(const Ткст& url);
    бул Listen(цел port, цел listen_count = 5)                                             { return Listen(Null, port, nullptr, listen_count); }
    бул Listen(const Ткст& host, цел port, цел* bound_port, цел listen_count = 5);
    бул прими(SshTunnel& listener);

    SshTunnel(SshSession& session) : SshChannel(session)                                    { ssh->otype = TUNNEL; mode = NONE; ssh->init = true; }
    virtual ~SshTunnel()                                                                    { выход(); }

    SshTunnel(SshTunnel&&) = default;

private:
    проц выход() override;
    бул пригоден();

    цел  mode;
    Один<LIBSSH2_LISTENER*>  listener;

    enum Режимы { NONE, CONNECT, LISTEN, ACCEPT };
};


class SshShell : public SshChannel {
public:
    бул        пуск(const Ткст& terminal, размер pagesize, const Ткст& tmodes = Null)                 { return пуск(GENERIC, terminal, pagesize, tmodes);  }
    бул        пуск(const Ткст& terminal, цел width, цел height, const Ткст& tmodes = Null)         { return пуск(GENERIC, terminal, {width, height}, tmodes); }
    
    бул        Консоль(const Ткст& terminal, const Ткст& tmodes = Null)                            { return пуск(CONSOLE, terminal, GetConsolePageSize(), tmodes); }

    SshShell&   ForwardX11(const Ткст& host = Null, цел дисплей = 0, цел screen = 0, цел bufsize = 1024 * 1024);
    бул        AcceptX11(SshX11Handle xhandle);

    проц        Send(цел c)                     { queue.кат(c);   }
    проц        Send(const сим* s)             { Send(Ткст(s));}
    проц        Send(const Ткст& s)           { queue.кат(s);   }

    SshShell&   PageSize(размер sz)               { resized = sz != psize; if(resized) psize = sz; return *this;}
    размер        GetPageSize() const             { return psize; }
    размер        GetConsolePageSize();

    Событие<>                  WhenInput;
    Событие<const проц*, цел>  WhenOutput;

    SshShell(SshSession& session);
    virtual ~SshShell();

    SshShell(SshShell&&) = default;

protected:
    проц    ReadWrite(Ткст& in, кук out, цел out_len) override;
    virtual бул пуск(цел mode_, const Ткст& terminal, размер pagesize, const Ткст& tmodes);

    проц    Resize();
    бул    консИниц();
    проц    консЧмтай();
    проц    консПиши(кук буфер, цел len);
    проц    ConsoleRawMode(бул b = true);

    бул    х11Иниц();
    проц    х11Цикл();

    enum Режимы { GENERIC, CONSOLE };

private:
    Ткст  queue;
    размер    psize;
    цел     mode;
    бул    rawmode;
    бул    resized;
    бул    xenabled;
#ifdef PLATFORM_WIN32
    DWORD   tflags;
    HANDLE  stdinput;
    HANDLE  stdoutput;
#elif  PLATFORM_POSIX
    termios tflags;
    ббайт    xdisplay;
    ббайт    xscreen;
    Ткст  xhost;
    Буфер<сим> xbuffer;
    цел          xbuflen;
    Вектор<Кортеж<SshX11Handle, SOCKET>> xrequests;
#endif
};