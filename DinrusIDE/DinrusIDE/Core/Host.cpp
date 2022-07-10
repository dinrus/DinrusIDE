#include "Core.h"

#define LLOG(x)

#include <plugin/bz2/bz2.h>

Хост::Хост()
{
}

Ткст Хост::GetEnvironment()
{
	return environment;
}

void Хост::AddEnvironment(const char *id, const char *значение)
{
	bool b = *environment;
	if(b)
		environment.обрежь(environment.дайСчёт() - 1);
	environment << id << '=' << значение << '\0';
	if(b)
		environment << '\0';
}

Вектор<Хост::ИнфОФайле> Хост::дайИнфОФайле(const Вектор<Ткст>& path)
{
	Вектор<ИнфОФайле> fi;
	for(int i = 0; i < path.дайСчёт(); i++) {
		ФайлПоиск ff(path[i]);

		ИнфОФайле& f = fi.добавь();
		if(ff) {
			(Время&)f = ff.дайВремяПоследнЗаписи();
			f.length = ff.файл_ли() ? (int)ff.дайДлину() : -1;
		}
		else {
			(Время&)f = Время::наименьш();
			f.length = Null;
		}

		if(onefile.дайСчёт()) {
			if(path[i] == onefile)
				(Время &)f = дайСисВремя();
			else
				(Время &)f = Время::наименьш();
		}
	}
	return fi;
}

void Хост::ChDir(const Ткст& path)
{
#ifdef PLATFORM_WIN32
	SetCurrentDirectory(path);
#endif
#ifdef PLATFORM_POSIX
	IGNORE_RESULT( chdir(path) );
#endif
	if(cmdout)
		*cmdout << "cd \"" << path << "\"\n";
}

void Хост::DoDir(const Ткст& dir)
{
	if(dir.дайДлину() > 3) {
		DoDir(дайПапкуФайла(dir));
		*cmdout << "mkdir \"" << dir << "\"\n";
	}
}

bool Хост::RealizeDir(const Ткст& path)
{
	bool realized = реализуйДир(path);
	if(cmdout)
		DoDir(path);
	return realized;
}

int Хост::выполни(const char *cmdline)
{
	if(cmdout)
		*cmdout << cmdline << '\n';
	Log(cmdline);
	int q = выполниВКонсИср(FindCommand(exedirs, cmdline), NULL, environment, false);
	Log(фмт("Exitcode: %d", q));
	return q;
}

int Хост::ExecuteWithInput(const char *cmdline, bool noconvert)
{
	if(cmdout)
		*cmdout << cmdline << '\n';
	Log(cmdline);
	int q = выполниВКонсИсрСВводом(FindCommand(exedirs, cmdline), NULL, environment, false, noconvert);
	Log(фмт("Exitcode: %d", q));
	return q;
}

int Хост::выполни(const char *cmdline, Поток& out, bool noconvert)
{
	Log(cmdline);
	int q = выполниВКонсИср(FindCommand(exedirs, cmdline), &out, environment, true, noconvert);
	Log(фмт("Exitcode: %d", q));
	return q;
}

int Хост::разместиСлот()
{
	return разместиСлотКонсИср();
}

bool Хост::пуск(const char *cmdline, int slot, Ткст ключ, int blitz_count)
{
	return пускКонсИср(FindCommand(exedirs, cmdline), NULL, environment, false, slot, ключ, blitz_count);
}

bool Хост::пуск(const char *cmdline, Поток& out, int slot, Ткст ключ, int blitz_count)
{
	return пускКонсИср(FindCommand(exedirs, cmdline), &out, environment, true, slot, ключ, blitz_count);
}

bool Хост::жди()
{
	return ждиКонсИср();
}

bool Хост::жди(int slot)
{
	return ждиКонсИср(slot);
}

void Хост::OnFinish(Событие<>  cb)
{
	приФинишеКонсИср(cb);
}

bool Хост::StartProcess(ЛокальнПроцесс& p, const char *cmdline)
{
	try {
		if(canlog) Log(cmdline);
		p.NoConvertCharset();
		if(p.старт(FindCommand(exedirs, cmdline), environment))
			return true;
	}
	catch(...) {
	}
	return false;
}

#ifdef PLATFORM_POSIX
//#BLITZ_APPROVE
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static Вектор<int>& sPid()
{
	static Вектор<int> q;
	return q;
}

void sCleanZombies(int signal_number)
{
	Вектор<int>& pid = sPid();
	int i = 0;
	while(i < pid.дайСчёт())
		if(pid[i] && waitpid(pid[i], 0, WNOHANG | WUNTRACED) > 0)
			pid.удали(i);
		else
			i++;
}
#endif

#ifdef PLATFORM_WIN32
Ткст HostConsole = "powershell.exe";
#else
Ткст HostConsole = "/usr/bin/xterm -e";
#endif

#ifdef PLATFORM_POSIX
void RemoveConsoleScripts()
{
	ФВремя tm = (дайСисВремя() - 24*3600).какФВремя();
	for(ФайлПоиск ff(конфигФайл("console-script-*")); ff; ff++) {
		if(ff.дайВремяПоследнЗаписи() < tm)
			удалифл(ff.дайПуть());
	}
}
#endif

void Хост::Launch(const char *_cmdline, bool console)
{
	Ткст cmdline = FindCommand(exedirs, _cmdline);
	Log(cmdline);
#ifdef PLATFORM_WIN32
	if(console)
		cmdline = дайФПутьИсп() + " ! " + cmdline;
	PROCESS_INFORMATION pi;
	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFOW);
	if(Win32CreateProcess(cmdline, ~environment, si, pi, NULL)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
		вКонсоль("Не удаётся запустить " + Ткст(_cmdline));
#endif
#ifdef PLATFORM_POSIX
	Ткст script = конфигФайл("console-script-" + какТкст(getpid()));
	if(console) {
		ФайлПоток out(script, ФайлПоток::CREATE, 0777);
		out << "#!/bin/sh\n"
		    << cmdline << '\n'
		#ifndef PLATFORM_COCOA
		    << "echo \"<--- Finished, press [ENTER] to close the window --->\"\nread dummy\n"
		#endif
		;
	}
#ifdef PLATFORM_COCOA
	if(console)
		cmdline = "/usr/bin/open " + script;
#else
	Ткст lc;
	static const char *term[] = {
		"/usr/bin/mate-terminal -x",
		"/usr/bin/gnome-terminal --window -x",
		"/usr/bin/konsole -e",
		"/usr/bin/lxterminal -e",
		"/usr/bin/xterm -e",
	};
	int ii = 0;
	for(;;) { // If (pre)defined terminal emulator is not available, try to find one
		int c = HostConsole.найдиПервыйИз(" ");
		lc = c < 0 ? HostConsole : HostConsole.лево(c);
		if(ii >= __countof(term) || файлЕсть(lc))
			break;
		HostConsole = term[ii++];
	}
	if(файлЕсть(lc))
	{
		if(console)
			cmdline = HostConsole + " sh " + script;
	}
	else
	if(HostConsole.дайСчёт())
		вКонсоль("Предупреждение: Терминал '" + lc + "' не найден, выполнение в фоне.");
#endif

	Буфер<char> cmd_buf(strlen(cmdline) + 1);
	Вектор<char *> args;
	
	Log(cmdline);

	char *o = cmd_buf;
	const char *s = cmdline;
	while(*s) {
		char *arg = o;
		while((byte)*s > ' ') {
			if(*s == '\"') {
				s++;
				while(*s) {
					if(*s == '\"') {
						s++;
						break;
					}
					*o++ = *s++;
				}
			}
			else
				*o++ = *s++;
		}
		while(*s && (byte)*s <= ' ')
			s++;
		if(o > arg) {
			*o++ = '\0';
			args.добавь(arg);
		}
	}

	args.добавь(NULL);

	ONCELOCK {
		struct sigaction sigchld_action;
		memset(&sigchld_action, 0, sizeof(sigchld_action));
		sigchld_action.sa_handler = sCleanZombies;
		sigaction(SIGCHLD, &sigchld_action, NULL);
	}

	pid_t pid = fork();
	if(pid == 0)
	{
		const char *from = environment;
		Вектор<const char *> env;
		while(*from) {
			env.добавь(from);
			from += strlen(from) + 1;
		}
		env.добавь(NULL);
		const char **envp = env.старт();
		execve(args[0], args, (char *const *)envp);
		abort();
	}
	LLOG("Launch pid: " << pid);
	sPid().добавь(pid);
#endif
}

void Хост::добавьФлаги(Индекс<Ткст>& cfg)
{
	if(HasPlatformFlag(cfg))
		return;
	
#if   defined(PLATFORM_WIN32)
	cfg.добавь("WIN32");
#endif

#ifdef PLATFORM_POSIX
	cfg.добавь("POSIX");
#endif

#ifdef PLATFORM_LINUX
	cfg.добавь("LINUX");
#endif

#ifdef PLATFORM_ANDROID
	cfg.добавь("ANDROID");
#endif

#ifdef PLATFORM_BSD
	cfg.добавь("BSD");
#endif

#ifdef PLATFORM_OSX
	cfg.добавь("OSX");
#endif

#ifdef PLATFORM_FREEBSD
	cfg.добавь("FREEBSD");
#endif

#ifdef PLATFORM_OPENBSD
	cfg.добавь("OPENBSD");
#endif

#ifdef PLATFORM_NETBSD
	cfg.добавь("NETBSD");
#endif

#ifdef PLATFORM_DRAGONFLY
	cfg.добавь("DRAGONFLY");
#endif

#ifdef PLATFORM_SOLARIS
	cfg.добавь("SOLARIS");
#endif

#ifdef PLATFORM_OSX11
	cfg.добавь("OSX11");
#endif
}

const Вектор<Ткст>& Хост::GetExecutablesDirs() const
{
	return exedirs;
}

bool Хост::HasPlatformFlag(const Индекс<Ткст>& cfg)
{
	static const Индекс<Ткст> platformFlags = {
		"WIN32", "POSIX", "LINUX", "ANDROID",
		"BSD", "FREEBSD", "OPENBSD", "NETBSD",
		"DRAGONFLY", "SOLARIS", "OSX11", "OSX"
	};

	for(const Ткст& flag : cfg)
		if(platformFlags.найди(flag) >= 0)
			return true;

	return false;
}
