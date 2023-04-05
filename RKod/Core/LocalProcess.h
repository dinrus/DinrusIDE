struct ПроцессИнк : БезКопий {
public:
	virtual void глуши() = 0;
	virtual bool пущен() = 0;
	virtual void пиши(Ткст s) = 0;
	virtual bool читай(Ткст& s) = 0;
	virtual bool читай2(Ткст& os, Ткст& es) { НИКОГДА(); return false; }
	virtual int  дайКодВыхода() = 0;
	virtual Ткст дайСообВыхода() { return Ткст(); };
	virtual void закройЧтен()        {}
	virtual void закройЗап()       {}
	virtual void открепи()           {};
	
	Ткст  дай()                  { Ткст x; if(читай(x)) return x; return Ткст::дайПроц(); }

	ПроцессИнк() {}
	virtual ~ПроцессИнк() {}
};

class ЛокальнПроцесс : public ПроцессИнк {
public:
	virtual void глуши();
	virtual bool пущен();
	virtual void пиши(Ткст s);
	virtual bool читай(Ткст& s);
	virtual bool читай2(Ткст& os, Ткст &es);
	virtual Ткст дайСообВыхода();
	virtual int  дайКодВыхода();
	virtual void закройЧтен();
	virtual void закройЗап();
	virtual void открепи();

private:
	void         иниц();
	void         освободи();
#ifdef PLATFORM_POSIX
	bool         DecodeExitCode(int код_);
#endif

private:
	bool         convertcharset;

#ifdef PLATFORM_WIN32
	HANDLE       hProcess;
	HANDLE       hOutputRead;
	HANDLE       hErrorRead;
	HANDLE       hInputWrite;
	DWORD        dwProcessId;
#endif
#ifdef PLATFORM_POSIX
	pid_t        pid;
	int          rpipe[2], wpipe[2], epipe[2];
	Ткст       exit_string;
	bool         doublefork;
#endif
	int          exit_code;
	Ткст       wreso, wrese; // Output fetched during пиши

	typedef ЛокальнПроцесс ИМЯ_КЛАССА;

	bool DoStart(const char *cmdline, const Вектор<Ткст> *арг, bool spliterr, const char *envptr, const char *cd);

public:
	bool старт(const char *cmdline, const char *envptr = NULL, const char *cd = NULL)                         { return DoStart(cmdline, NULL, false, envptr, cd); }
	bool Start2(const char *cmdline, const char *envptr = NULL, const char *cd = NULL)                        { return DoStart(cmdline, NULL, true, envptr, cd); }

	bool старт(const char *cmd, const Вектор<Ткст>& арг, const char *envptr = NULL, const char *cd = NULL)  { return DoStart(cmd, &арг, false, envptr, cd); }
	bool Start2(const char *cmd, const Вектор<Ткст>& арг, const char *envptr = NULL, const char *cd = NULL) { return DoStart(cmd, &арг, true, envptr, cd); }
	
#ifdef PLATFORM_POSIX
	ЛокальнПроцесс& DoubleFork(bool b = true)                           { doublefork = b; return *this; }

	int  GetPid()  const                                              { return pid; }
#endif

#ifdef PLATFORM_WIN32
	HANDLE  GetProcessHandle()  const                                 { return hProcess; }
#endif

	int  финиш(Ткст& out);
		
	ЛокальнПроцесс& преобразуйНабСим(bool b = true)                       { convertcharset = b; return *this; }
	ЛокальнПроцесс& NoConvertCharset()                                  { return преобразуйНабСим(false); }

	ЛокальнПроцесс()                                                                          { иниц(); }
	ЛокальнПроцесс(const char *cmdline, const char *envptr = NULL)                            { иниц(); старт(cmdline, envptr); }
	ЛокальнПроцесс(const char *cmdline, const Вектор<Ткст>& арг, const char *envptr = NULL) { иниц(); старт(cmdline, арг, envptr); }
	virtual ~ЛокальнПроцесс()                                                                 { глуши(); }
};

int    Sys(const char *cmdline, Ткст& out, bool convertcharset = true);
Ткст Sys(const char *cmdline, bool convertcharset = true);

int    Sys(const char *cmd, const Вектор<Ткст>& арг, Ткст& out, bool convertcharset = true);
Ткст Sys(const char *cmd, const Вектор<Ткст>& арг, bool convertcharset = true);
