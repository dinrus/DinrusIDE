#include "Core.h"

namespace РНЦПДинрус {

#ifdef PLATFORM_POSIX
//#BLITZ_APPROVE
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#define LLOG(x) // DLOG(x)

void ЛокальнПроцесс::иниц() {
#ifdef PLATFORM_WIN32
	hProcess = hOutputRead = hErrorRead = hInputWrite = NULL;
#endif
#ifdef PLATFORM_POSIX
	pid = 0;
	doublefork = false;
	rpipe[0] = rpipe[1] = wpipe[0] = wpipe[1] = epipe[0] = epipe[1] = -1;
#endif
	exit_code = Null;
	convertcharset = true;
}

void ЛокальнПроцесс::освободи() {
#ifdef PLATFORM_WIN32
	if(hProcess) {
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	if(hOutputRead) {
		CloseHandle(hOutputRead);
		hOutputRead = NULL;
	}
	if(hErrorRead) {
		CloseHandle(hErrorRead);
		hErrorRead = NULL;
	}
	if(hInputWrite) {
		CloseHandle(hInputWrite);
		hInputWrite = NULL;
	}
#endif
#ifdef PLATFORM_POSIX
	LLOG("\nLocalProcess::освободи, pid = " << (int)getpid());
	LLOG("rpipe[" << rpipe[0] << ", " << rpipe[1] << "]");
	LLOG("wpipe[" << wpipe[0] << ", " << wpipe[1] << "]");
	if(rpipe[0] >= 0) { close(rpipe[0]); rpipe[0] = -1; }
	if(rpipe[1] >= 0) { close(rpipe[1]); rpipe[1] = -1; }
	if(wpipe[0] >= 0) { close(wpipe[0]); wpipe[0] = -1; }
	if(wpipe[1] >= 0) { close(wpipe[1]); wpipe[1] = -1; }
	if(epipe[0] >= 0) { close(epipe[0]); epipe[0] = -1; }
	if(epipe[1] >= 0) { close(epipe[1]); epipe[1] = -1; }
	if(pid) waitpid(pid, 0, WNOHANG | WUNTRACED);
	pid = 0;
#endif
}

#ifdef PLATFORM_POSIX
static void sNoBlock(int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
#endif

#ifdef PLATFORM_WIN32
bool Win32CreateProcess(const char *command, const char *envptr, STARTUPINFOW& si, PROCESS_INFORMATION& pi, const char *cd)
{ // provides conversion of charset for cmdline
	Вектор<WCHAR> cmd = вСисНабсимШ(command);
	cmd.добавь(0);
	return CreateProcessW(NULL, cmd, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, (void *)envptr,
	                      cd ? вСисНабсимШ(cd).begin() : NULL, &si, &pi);
}
#endif

bool ЛокальнПроцесс::DoStart(const char *command, const Вектор<Ткст> *арг, bool spliterr, const char *envptr, const char *cd)
{
	LLOG("ЛокальнПроцесс::старт(\"" << command << "\")");

	глуши();
	exit_code = Null;

	while(*command && (byte)*command <= ' ')
		command++;

#ifdef PLATFORM_WIN32
	HANDLE hOutputReadTmp, hOutputWrite;
	HANDLE hInputWriteTmp, hInputRead;
	HANDLE hErrorReadTmp, hErrorWrite;

	HANDLE hp = GetCurrentProcess();

	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 0);
	DuplicateHandle(hp, hInputWriteTmp, hp, &hInputWrite, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hInputWriteTmp);

	CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0);
	DuplicateHandle(hp, hOutputReadTmp, hp, &hOutputRead, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hOutputReadTmp);

	if(spliterr) {
		CreatePipe(&hErrorReadTmp, &hErrorWrite, &sa, 0);
		DuplicateHandle(hp, hErrorReadTmp, hp, &hErrorRead, 0, FALSE, DUPLICATE_SAME_ACCESS);
		CloseHandle(hErrorReadTmp);
	}
	else
		DuplicateHandle(hp, hOutputWrite, hp, &hErrorWrite, 0, TRUE, DUPLICATE_SAME_ACCESS);

	PROCESS_INFORMATION pi;
	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput  = hInputRead;
	si.hStdOutput = hOutputWrite;
	si.hStdError  = hErrorWrite;
	Ткст cmdh;
	if(арг) {
		cmdh = command;
		for(int i = 0; i < арг->дайСчёт(); i++) {
			cmdh << ' ';
			Ткст argument = (*арг)[i];
			if(argument.дайСчёт() && argument.найдиПервыйИз(" \t\n\v\"") < 0)
				cmdh << argument;
			else {
				cmdh << '\"';
				const char *s = argument;
				for(;;) {
					int num_backslashes = 0;
					while(*s == '\\') {
						s++;
						num_backslashes++;
					}
					if(*s == '\0') {
						cmdh.конкат('\\', 2 * num_backslashes);
						break;
					}
					else
					if(*s == '\"') {
						cmdh.конкат('\\', 2 * num_backslashes + 1);
						cmdh << '\"';
					}
					else {
						cmdh.конкат('\\', num_backslashes);
						cmdh.конкат(*s);
					}
					s++;
				}
				cmdh << '\"';
			}
	    }
		command = cmdh;
	}
	bool h = Win32CreateProcess(command, envptr, si, pi, cd);
	LLOG("CreateProcess " << (h ? "succeeded" : "failed"));
	CloseHandle(hErrorWrite);
	CloseHandle(hInputRead);
	CloseHandle(hOutputWrite);
	if(h) {
		hProcess = pi.hProcess;
		CloseHandle(pi.hThread);
	}
	else {
		освободи();
		return false;
	}
	return true;
#endif
#ifdef PLATFORM_POSIX
	Буфер<char> cmd_buf;
	Вектор<char *> арги;

	Ткст app;
	if(арг) {
		app = command;
		int n = strlen(command) + 1;
		for(int i = 0; i < арг->дайСчёт(); i++)
			n += (*арг)[i].дайСчёт() + 1;
		cmd_buf.размести(n + 1);
		char *p = cmd_buf;
		арги.добавь(p);
		int l = strlen(command) + 1;
		memcpy(p, command, l);
		p += l;
		for(int i = 0; i < арг->дайСчёт(); i++) {
			арги.добавь(p);
			l = (*арг)[i].дайСчёт() + 1;
			memcpy(p, ~(*арг)[i], l);
			p += l;
		}
	}
	else { // parse command line for execve
		cmd_buf.размести(strlen(command) + 1);
		char *cmd_out = cmd_buf;
		const char *p = command;
		while(*p)
			if((byte)*p <= ' ')
				p++;
			else {
				арги.добавь(cmd_out);
				while(*p && (byte)*p > ' ') {
					int c = *p;
					if(c == '\\') {
						if(*++p)
							*cmd_out++ = *p++;
					}
					else if(c == '\"' || c == '\'') {
						p++;
						while(*p && *p != c)
							if(*p == '\\') {
								if(*++p)
									*cmd_out++ = *p++;
							}
							else
								*cmd_out++ = *p++;
						if(*p == c)
							p++;
					}
					else
						*cmd_out++ = *p++;
				}
				*cmd_out++ = '\0';
			}
	}
	
	if(арги.дайСчёт() == 0)
		return false;

	арги.добавь(NULL);

	Ткст app_full = дайФайлПоПути(арги[0], getenv("PATH"), true);
	if(пусто_ли(app_full))
		return false;
	
	Буфер<char> arg0(app_full.дайСчёт() + 1);
	memcpy(~arg0, ~app_full, app_full.дайСчёт() + 1);
	арги[0] = ~arg0;

	if(pipe(rpipe) || pipe(wpipe))
		return false;

	if(spliterr && pipe(epipe))
		return false;
	
	LLOG("\nLocalProcess::старт");
	LLOG("rpipe[" << rpipe[0] << ", " << rpipe[1] << "]");
	LLOG("wpipe[" << wpipe[0] << ", " << wpipe[1] << "]");
	LLOG("epipe[" << epipe[0] << ", " << epipe[1] << "]");

#ifdef CPU_BLACKFIN
	pid = vfork(); //we *can* use vfork here, since exec is done later or the parent will exit
#else
	pid = fork();
#endif
	LLOG("\tfork, pid = " << (int)pid << ", getpid = " << (int)getpid());
	if(pid < 0)
		return false;
//		throw Искл(фмтЧ(t_("fork() Ошибка; Ошибка code = %d"), errno));

	if(pid) {
		LLOG("parent process - continue");
		close(rpipe[0]); rpipe[0]=-1;
		close(wpipe[1]); wpipe[1]=-1;
		sNoBlock(rpipe[1]);
		sNoBlock(wpipe[0]);
		if (spliterr) {
			sNoBlock(epipe[0]);
			close(epipe[1]); epipe[1]=-1;
		}
		if (doublefork)
			pid = 0;
		return true;
	}

	if (doublefork) {
		pid_t pid2 = fork();
		LLOG("\tfork2, pid2 = " << (int)pid2 << ", getpid = " << (int)getpid());
		if (pid2 < 0) {
			LLOG("fork2 failed");
			выход(1);
		}
		if (pid2) {
			LLOG("exiting intermediary process");
			close(rpipe[0]); rpipe[0]=-1;
			close(wpipe[1]); wpipe[1]=-1;
			sNoBlock(rpipe[1]);
			sNoBlock(wpipe[0]);
			if (spliterr) {
				sNoBlock(epipe[0]);
				close(epipe[1]); epipe[1]=-1;
			}
			// we call exec instead of выход, because exit doesn't behave nicelly with threads
			execl("/usr/bin/true", "[closing fork]", (char*)NULL);
			// only call выход when execl fails
			выход(0);
		}
	}

	LLOG("child process - execute application");
//	rpipe[1] = wpipe[0] = -1;
	dup2(rpipe[0], 0);
	dup2(wpipe[1], 1);
	dup2(spliterr ? epipe[1] : wpipe[1], 2);
	close(rpipe[0]);
	close(rpipe[1]);
	close(wpipe[0]);
	close(wpipe[1]);
	if (spliterr) {
		close(epipe[0]);
		close(epipe[1]);
	}
	rpipe[0] = rpipe[1] = wpipe[0] = wpipe[1] = epipe[0] = epipe[1] = -1;
#if DO_LLOG
	LLOG(арги.дайСчёт() << "arguments:");
	for(int a = 0; a < арги.дайСчёт(); a++)
		LLOG("[" << a << "]: <" << (арги[a] ? арги[a] : "NULL") << ">");
#endif

	if(cd)
		(void)!chdir(cd); // that (void)! strange thing is to silence GCC warning

	LLOG("running execve, app = " << app << ", #арги = " << арги.дайСчёт());
	if(envptr) {
		const char *from = envptr;
		Вектор<const char *> env;
		while(*from) {
			env.добавь(from);
			from += strlen(from) + 1;
		}
		env.добавь(NULL);
		execve(app_full, арги.старт(), (char *const *)env.старт());
	}
	else
		execv(app_full, арги.старт());
	LLOG("execve failed, errno = " << errno);
//	printf("Ошибка running '%s', Ошибка code %d\n", command, errno);
	exit(-errno);
	return true;
#endif
}

#ifdef PLATFORM_POSIX
bool ЛокальнПроцесс::DecodeExitCode(int status)
{
	if(WIFEXITED(status)) {
		exit_code = (byte)WEXITSTATUS(status);
		return true;
	}
	else if(WIFSIGNALED(status) || WIFSTOPPED(status)) {
		static const struct {
			const char *имя;
			int         code;
		}
		signal_map[] = {
#define SIGDEF(s) { #s, s },
SIGDEF(SIGHUP) SIGDEF(SIGINT) SIGDEF(SIGQUIT) SIGDEF(SIGILL) SIGDEF(SIGABRT)
SIGDEF(SIGFPE) SIGDEF(SIGKILL) SIGDEF(SIGSEGV) SIGDEF(SIGPIPE) SIGDEF(SIGALRM)
SIGDEF(SIGPIPE) SIGDEF(SIGTERM) SIGDEF(SIGUSR1) SIGDEF(SIGUSR2) SIGDEF(SIGTRAP)
SIGDEF(SIGURG) SIGDEF(SIGVTALRM) SIGDEF(SIGXCPU) SIGDEF(SIGXFSZ) SIGDEF(SIGIOT)
SIGDEF(SIGIO) SIGDEF(SIGWINCH)
#ifndef PLATFORM_BSD
//SIGDEF(SIGCLD) SIGDEF(SIGPWR)
#endif
//SIGDEF(SIGSTKFLT) SIGDEF(SIGUNUSED) // not in Solaris, make conditional if needed
#undef SIGDEF
		};

		int sig = (WIFSIGNALED(status) ? WTERMSIG(status) : WSTOPSIG(status));
		exit_code = (WIFSIGNALED(status) ? 1000 : 2000) + sig;
		exit_string << "\nProcess " << (WIFSIGNALED(status) ? "terminated" : "stopped") << " on signal " << sig;
		for(int i = 0; i < __countof(signal_map); i++)
			if(signal_map[i].code == sig)
			{
				exit_string << " (" << signal_map[i].имя << ")";
				break;
			}
		exit_string << "\n";
		return true;
	}
	return false;
}
#endif//PLATFORM_POSIX

void ЛокальнПроцесс::глуши() {
#ifdef PLATFORM_WIN32
	if(hProcess && пущен()) {
		TerminateProcess(hProcess, (DWORD)-1);
		exit_code = 255;
	}
#endif
#ifdef PLATFORM_POSIX
	if(пущен()) {
		LLOG("\nLocalProcess::глуши, pid = " << (int)pid);
		exit_code = 255;
		kill(pid, SIGTERM);
		дайКодВыхода();
		int status;
		if(pid && waitpid(pid, &status, 0) == pid)
			DecodeExitCode(status);
		exit_string = "Child process has been killed.\n";
	}
#endif
	освободи();
}

void ЛокальнПроцесс::открепи()
{
#ifdef PLATFORM_POSIX
	if (doublefork)
		waitpid(pid, 0, WUNTRACED);
#endif
	освободи();
}

bool ЛокальнПроцесс::пущен() {
#ifdef PLATFORM_WIN32
	dword exitcode;
	if(!hProcess)
		return false;
	if(GetExitCodeProcess(hProcess, &exitcode) && exitcode == STILL_ACTIVE)
		return true;
	dword n;
	if(PeekNamedPipe(hOutputRead, NULL, 0, NULL, &n, NULL) && n)
		return true;
	exit_code = exitcode;
	return false;
#endif
#ifdef PLATFORM_POSIX
	if(!pid || !пусто_ли(exit_code)) {
		LLOG("пущен() -> no");
		return false;
	}
	int status = 0, wp;
	if(!( (wp = waitpid(pid, &status, WNOHANG | WUNTRACED)) == pid &&
	      DecodeExitCode(status) ))
		return true;
	LLOG("пущен() -> no, just exited, exit code = " << exit_code);
	return false;
#endif
}

int  ЛокальнПроцесс::дайКодВыхода() {
#ifdef PLATFORM_WIN32
	return пущен() ? (int)Null : exit_code;
#endif
#ifdef PLATFORM_POSIX
	if(!пущен())
		return Nvl(exit_code, -1);
	int status;
	if(!( waitpid(pid, &status, WNOHANG | WUNTRACED) == pid && 
	      DecodeExitCode(status) ))
		return -1;
	LLOG("дайКодВыхода() -> " << exit_code << " (just exited)");
	return exit_code;
#endif
}

Ткст ЛокальнПроцесс::дайСообВыхода() {
#ifdef PLATFORM_POSIX
	if (!пущен() && дайКодВыхода() == -1)
		return exit_string;
	else
#endif
		return Ткст();
}

bool ЛокальнПроцесс::читай(Ткст& res) {
	Ткст dummy;
	return читай2(res, dummy);
}

bool ЛокальнПроцесс::читай2(Ткст& reso, Ткст& rese)
{
	LLOG("ЛокальнПроцесс::читай2");
	reso = wreso;
	rese = wrese;
	wreso.очисть();
	wrese.очисть();

#ifdef PLATFORM_WIN32
	LLOG("ЛокальнПроцесс::читай");
	bool was_running = пущен();
	char буфер[1024];
	dword n;
	if(hOutputRead && PeekNamedPipe(hOutputRead, NULL, 0, NULL, &n, NULL) && n &&
	   ReadFile(hOutputRead, буфер, sizeof(буфер), &n, NULL) && n)
		reso.конкат(буфер, n);

	if(hErrorRead && PeekNamedPipe(hErrorRead, NULL, 0, NULL, &n, NULL) && n &&
	   ReadFile(hErrorRead, буфер, sizeof(буфер), &n, NULL) && n)
		rese.конкат(буфер, n);

	if(convertcharset) {
		reso = изНабсимаОЕМ(reso);
		rese = изНабсимаОЕМ(rese);
	}
	
	return reso.дайСчёт() || rese.дайСчёт() || was_running;
#endif
#ifdef PLATFORM_POSIX
	Ткст res[2];
	bool was_running = пущен() || wpipe[0] >= 0 || epipe[0] >= 0;
	for (int wp=0; wp<2;wp++) {
		int *pipe = wp ? epipe : wpipe;
		if (pipe[0] < 0) {
			LLOG("пайп["<<wp<<"] closed");
			continue;
		}
		fd_set set[1];
		FD_ZERO(set);
		FD_SET(pipe[0], set);
		timeval tval = { 0, 0 };
		int sv;
		if((sv = select(pipe[0]+1, set, NULL, NULL, &tval)) > 0) {
			LLOG("читай() -> select");
			char буфер[1024];
			int done = read(pipe[0], буфер, sizeof(буфер));
			LLOG("читай(), read -> " << done);
			if(done > 0)
				res[wp].конкат(буфер, done);
			else if (done == 0) {
				close(pipe[0]);
				pipe[0] = -1;
			}
		}
		LLOG("пайп["<<wp<<"]=="<<pipe[0]<<" sv:"<<sv);
		if(sv < 0) {
			LLOG("select -> " << sv);
		}
	}
	if(convertcharset) {
		reso << изСисНабсима(res[0]);
		rese << изСисНабсима(res[1]);
	} else {
		reso << res[0];
		rese << res[1];
	}
	return !пусто_ли(res[0]) || !пусто_ли(res[1]) || was_running;
#endif
}

void ЛокальнПроцесс::пиши(Ткст s)
{
	if(convertcharset)
		s = вСисНабсим(s);
#ifdef PLATFORM_WIN32
	if (hInputWrite) {
		bool ret = true;
		dword n;
		for(int wn = 0; ret && wn < s.дайДлину(); wn += n) {
			ret = WriteFile(hInputWrite, ~s + wn, s.дайДлину(), &n, NULL);
			Ткст ho = wreso;
			Ткст he = wrese;
			wreso = wrese = Null;
			читай2(wreso, wrese);
			wreso = ho + wreso;
			wrese = he + wrese;
		}
	}
#endif
#ifdef PLATFORM_POSIX
	if (rpipe[1] >= 0) {
		int ret=1;
		for(int wn = 0; (ret > 0 || errno == EINTR) && wn < s.дайДлину(); wn += ret) {
			Ткст ho = wreso;
			Ткст he = wrese;
			wreso = wrese = Null;
			читай2(wreso, wrese);
			wreso = ho + wreso;
			wrese = he + wrese;
			ret = write(rpipe[1], ~s + wn, s.дайДлину() - wn);
		}
	}
#endif
}

void ЛокальнПроцесс::закройЧтен()
{
#ifdef PLATFORM_WIN32
	if(hOutputRead) {
		CloseHandle(hOutputRead);
		hOutputRead = NULL;
	}
#endif
#ifdef PLATFORM_POSIX
	if (wpipe[0] >= 0) {
		close(wpipe[0]);
		wpipe[0]=-1;
	}
#endif
}

void ЛокальнПроцесс::закройЗап()
{
#ifdef PLATFORM_WIN32
	if(hInputWrite) {
		CloseHandle(hInputWrite);
		hInputWrite = NULL;
	}
#endif
#ifdef PLATFORM_POSIX
	if (rpipe[1] >= 0) {
		close(rpipe[1]);
		rpipe[1]=-1;
	}
#endif
}

int ЛокальнПроцесс::финиш(Ткст& out)
{
	out.очисть();
	while(пущен()) {
		Ткст h = дай();
		if(пусто_ли(h))
			Sleep(1); // p.жди would be much better here!
		else
			out.конкат(h);
	}
	LLOG("финиш: About to read the rest of output");
	for(;;) {
		Ткст h = дай();
		if(h.проц_ли())
			break;
		out.конкат(h);
	}
	return дайКодВыхода();
}

int Sys(const char *cmd, Ткст& out, bool convertcharset)
{
	ЛокальнПроцесс p;
	p.преобразуйНабСим(convertcharset);
	if(!p.старт(cmd))
		return -1;
	return p.финиш(out);
}

Ткст Sys(const char *cmd, bool convertcharset)
{
	Ткст r;
	return Sys(cmd, r, convertcharset) ? Ткст::дайПроц() : r;
}

int Sys(const char *cmd, const Вектор<Ткст>& арг, Ткст& out, bool convertcharset)
{
	ЛокальнПроцесс p;
	p.преобразуйНабСим(convertcharset);
	if(!p.старт(cmd, арг))
		return -1;
	return p.финиш(out);
}

Ткст Sys(const char *cmd, const Вектор<Ткст>& арг, bool convertcharset)
{
	Ткст r;
	return Sys(cmd, арг, r, convertcharset) ? Ткст::дайПроц() : r;
}

}
