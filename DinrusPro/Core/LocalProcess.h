struct ПроцессИнк : БезКопий {
public:
	virtual проц глуши() = 0;
	virtual бул пущен() = 0;
	virtual проц пиши(Ткст s) = 0;
	virtual бул читай(Ткст& s) = 0;
	virtual бул читай2(Ткст& os, Ткст& es) { НИКОГДА(); return false; }
	virtual цел  дайКодВыхода() = 0;
	virtual Ткст дайСообВыхода() { return Ткст(); };
	virtual проц закройЧтен()        {}
	virtual проц закройЗап()       {}
	virtual проц открепи()           {};
	
	Ткст  дай()                  { Ткст x; if(читай(x)) return x; return Ткст::дайПроц(); }

	ПроцессИнк() {}
	virtual ~ПроцессИнк() {}
};

class ЛокальнПроцесс : public ПроцессИнк {
public:
	virtual проц глуши();
	virtual бул пущен();
	virtual проц пиши(Ткст s);
	virtual бул читай(Ткст& s);
	virtual бул читай2(Ткст& os, Ткст &es);
	virtual Ткст дайСообВыхода();
	virtual цел  дайКодВыхода();
	virtual проц закройЧтен();
	virtual проц закройЗап();
	virtual проц открепи();

private:
	проц         иниц();
	проц         освободи();
#ifdef PLATFORM_POSIX
	бул         DecodeExitCode(цел код_);
#endif

private:
	бул         convertcharset;

#ifdef PLATFORM_WIN32
	HANDLE       hProcess;
	HANDLE       hOutputRead;
	HANDLE       hErrorRead;
	HANDLE       hInputWrite;
	DWORD        dwProcessId;
#endif
#ifdef PLATFORM_POSIX
	pid_t        pid;
	цел          rpipe[2], wpipe[2], epipe[2];
	Ткст       exit_string;
	бул         doublefork;
#endif
	цел          exit_code;
	Ткст       wreso, wrese; // Output fetched during пиши

	typedef ЛокальнПроцесс ИМЯ_КЛАССА;

	бул DoStart(кткст0 cmdline, const Вектор<Ткст> *арг, бул spliterr, кткст0 envptr, кткст0 cd);

public:
	бул старт(кткст0 cmdline, кткст0 envptr = NULL, кткст0 cd = NULL)                         { return DoStart(cmdline, NULL, false, envptr, cd); }
	бул Start2(кткст0 cmdline, кткст0 envptr = NULL, кткст0 cd = NULL)                        { return DoStart(cmdline, NULL, true, envptr, cd); }

	бул старт(кткст0 cmd, const Вектор<Ткст>& арг, кткст0 envptr = NULL, кткст0 cd = NULL)  { return DoStart(cmd, &арг, false, envptr, cd); }
	бул Start2(кткст0 cmd, const Вектор<Ткст>& арг, кткст0 envptr = NULL, кткст0 cd = NULL) { return DoStart(cmd, &арг, true, envptr, cd); }
	
#ifdef PLATFORM_POSIX
	ЛокальнПроцесс& DoubleFork(бул b = true)                           { doublefork = b; return *this; }

	цел  GetPid()  const                                              { return pid; }
#endif

#ifdef PLATFORM_WIN32
	HANDLE  GetProcessHandle()  const                                 { return hProcess; }
#endif

	цел  финиш(Ткст& out);
		
	ЛокальнПроцесс& преобразуйНабСим(бул b = true)                       { convertcharset = b; return *this; }
	ЛокальнПроцесс& NoConvertCharset()                                  { return преобразуйНабСим(false); }

	ЛокальнПроцесс()                                                                          { иниц(); }
	ЛокальнПроцесс(кткст0 cmdline, кткст0 envptr = NULL)                            { иниц(); старт(cmdline, envptr); }
	ЛокальнПроцесс(кткст0 cmdline, const Вектор<Ткст>& арг, кткст0 envptr = NULL) { иниц(); старт(cmdline, арг, envptr); }
	virtual ~ЛокальнПроцесс()                                                                 { глуши(); }
};

цел    Sys(кткст0 cmdline, Ткст& out, бул convertcharset = true);
Ткст Sys(кткст0 cmdline, бул convertcharset = true);

цел    Sys(кткст0 cmd, const Вектор<Ткст>& арг, Ткст& out, бул convertcharset = true);
Ткст Sys(кткст0 cmd, const Вектор<Ткст>& арг, бул convertcharset = true);
