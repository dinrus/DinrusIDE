#include "Debuggers.h"

#ifdef PLATFORM_POSIX

void Do_ps(ВекторМап<Ткст, Ткст>& pids, Индекс<Ткст>& ttys, const Ткст& cmd)
{
	pids.очисть();
	ttys.очисть();
	Вектор<Ткст> ps = разбей(Sys(cmd), '\n');
	for(int i = 1; i < ps.дайСчёт(); i++) {
		Вектор<Ткст> ss = разбей(ps[i], ' ');
		if(ss.дайСчёт() >= 2) {
			if(цифра_ли(*ss[0]) && IsAlNum(*ss[1])) {
				pids.добавь(ss[1], ss[0]);
				ttys.найдиДобавь(ss[1]);
			}
		}
	}
}

static ВекторМап<Ткст, Ткст> sPid;
static Ткст sTTY;

Ткст CreateDebugTTY()
{ // The method is far from ideal and reliable, but should work most of time
	sTTY.очисть();

	Индекс<Ткст> tty0, tty;
	Do_ps(sPid, tty0, "ps -A");

	IGNORE_RESULT(chdir(дайДомПапку()));

	static const char *term[] = {
		"/usr/bin/mate-terminal -e 'tail -f /dev/null'",
		"/usr/bin/gnome-terminal -e 'tail -f /dev/null'",
		"/usr/bin/konsole -e 'tail -f /dev/null'",
		"/usr/bin/lxterminal -e 'tail -f /dev/null'",
		"/usr/bin/xterm -e 'tail -f /dev/null'",
	};
	
	for(int i = 0; i < __countof(term); i++) {
		if(файлЕсть(разбей(term[i], ' ')[0])) {
			ЛокальнПроцесс(term[i]).открепи();
			for(int i = 0; i < 10; i++) { // жди for the new TTY to appear
				Do_ps(sPid, tty, "ps -A");
				for(int i = 0; i < tty.дайСчёт(); i++)
					if(tty0.найди(tty[i]) < 0) {
						sTTY = tty[i];
						return "/dev/" + sTTY;
					}
				спи(200);
			}
		}
	}
	return Null;
}

void KillDebugTTY()
{
	Индекс<Ткст> tty;
	Do_ps(sPid, tty, "ps -A");
	if(sTTY.дайСчёт())
		for(int q = sPid.найди(sTTY); q >= 0; q = sPid.найдиСледщ(q)) {
			Sys("kill -9 " + sPid[q]);
		}
	sTTY.очисть();
}

bool TTYQuit()
{
	if(sTTY.дайСчёт()) {
		int fd = open("/dev/" + sTTY, O_RDWR | O_NONBLOCK);
		if(fd < 0)
			return true;
		close(fd);
		return false;
	}
	return false;
}
#else

Ткст CreateDebugTTY() { return Null; }
void KillDebugTTY()     {}
bool TTYQuit()          { return false; }

#endif

Ткст GdbCommand(bool console)
{
#ifdef PLATFORM_LLDB
	Ткст gdb = "/usr/bin/lldb -X ";
#else
	Ткст gdb = "gdb ";
#ifdef PLATFORM_POSIX
	if(console) {
		Ткст tty = CreateDebugTTY();
		if(tty.дайСчёт())
			gdb << "-tty=" << tty << ' ';
	}
#endif
#endif
	return gdb;
}

