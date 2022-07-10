#include "CtrlLib.h"

namespace РНЦП {

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#define LLOG(x)

static Ткст s_updatedir;
static Ткст s_updater;

void UpdateSetDir(const char *path)
{
	s_updatedir = path;
}

void UpdateSetUpdater(const char *exename)
{
	s_updater = exename;
}

Ткст UpdateGetDir()
{
	return Nvl(s_updatedir, дайКлючИни("UPDATE"));
}

Время FileTimeToTime(const FILETIME& time) {
	SYSTEMTIME t;
	FileTimeToSystemTime(&time, &t);
	return Время(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
}

bool NoUpdate(const char *dsf, const char *srf, int& len) {
	ФайлПоиск dst(dsf);
	ФайлПоиск src(srf);
	if(!src) return true;
	len = (int)src.дайДлину();
	if(!dst) return false;
	return абс(FileTimeToTime(dst.дайВремяПоследнЗаписи()) - FileTimeToTime(src.дайВремяПоследнЗаписи()))
		    < 100 && len == dst.дайДлину();
}

void обновиФайл(Ткст dst, Ткст src)
{
	if(src.пустой()) return;
	Ткст filename = дайИмяф(dst);
	int len;
	if(NoUpdate(dst, src, len)) return;
	Ткст tmp = dst + ".tmp";
	Progress pi;
	pi.Титул((t_("Обновление ") + filename).вШТкст());
	pi.устТекст(t_("Обновляется ") + filename);
//	pi.открой();
	ФайлВвод in(src);
	if(!in)
	{
		Exclamation(фмт(t_("Ошибка при открытии файла [* \1%s\1]."), src));
		return;
	}
	удалифл(tmp);
	ФайлВывод out(tmp);
	dword n = 0;
	char буфер[2048];
	if(!out)
	{
		Exclamation(фмт(t_("Ошибка при создании файла [* \1%s\1]."), tmp));
		return;
	}
	for(;;)
	{
		dword i = in.дай(буфер, 2048);
		if(i == 0) break;
		out.помести(буфер, i);
		n += i;
		pi.уст(n, len);
		if(pi.Canceled() && PromptYesNo(t_("Aborting update is likely to cause trouble when running the application for the next time.\nContinue?")))
		{
			out.закрой();
			удалифл(tmp);
			return;
		}
	}
	out.устВремя(in.дайВремя());
	out.закрой();
	if(out.ошибка_ли())
	{
		Exclamation(фмт(t_("Ошибка при записи файла [* \1%s\1]."), tmp));
		удалифл(tmp);
		return;
	}
	Ткст old = dst + ".old";
	pi.устТекст(фмт(t_("Переписывается %s"), dst));
	pi.устВсего(10000);
	int start = msecs();
	for(;;)
	{
		SetFileAttributes(dst, 0);
		удалифл(old);
		переместифл(dst, old);
		if(переместифл(tmp, dst))
			return;
		if(pi.SetPosCanceled(msecs(start) % 10000) && PromptYesNo(t_("Aborting update is likely to cause trouble when running the application for the next time.\nContinue?")))
			return;
		Sleep(500);
	}
}

void обновиФайл(const char *filename)
{
	Ткст dst = дайФайлИзПапкиИсп(filename);
	Ткст src = UpdateGetDir();
	if(пусто_ли(src))
		return;
	обновиФайл(dst, приставьИмяф(src, filename));
}

static Ткст FixArg(Ткст s)
{
	if(s.найди(' ') < 0 && s.найди('\"') < 0)
		return s;
	Ткст ucmd;
	ucmd << '\"';
	for(const char *p = s; *p; p++)
		if(*p == '\"')
			ucmd << "\"\"";
		else
			ucmd << *p;
	ucmd << '\"';
	return ucmd;
}

void SelfUpdate() {
	char dst[512];
	обновиФайл(Nvl(s_updater, Ткст("UPDATER.EXE")));
	::GetModuleFileName(NULL, dst, 512);
	{
		Ткст src = UpdateGetDir();
		if(src.пустой()) return;
		src = приставьИмяф(src, дайПозИмяф(dst));
		int dummy;
		if(NoUpdate(dst, src, dummy)) return;
		Ткст commandline;
		const Вектор<Ткст>& cmd = комСтрока();
		for(int i = 0; i < cmd.дайСчёт(); i++) {
			commandline += ' ';
			commandline += FixArg(cmd[i]);
		}
		if(WinExec(дайФайлИзПапкиИсп("updater.exe") + " " + дайИмяф(dst) + commandline, SW_SHOWNORMAL) <= 31)
			return;
	}
	exit(0);
}

bool SelfUpdateSelf()
{
	const Вектор<Ткст>& cmdline = комСтрока();
	Ткст exe = дайФПутьИсп();

	if(cmdline.дайСчёт() >= 2 && cmdline[0] == "-update")
	{ // updater
		Ткст exec = FixArg(cmdline[1]);
		обновиФайл(exec, exe);
		for(int i = 2; i < cmdline.дайСчёт(); i++)
			exec << ' ' << FixArg(cmdline[i]);
		WinExec(exec, SW_SHOWNORMAL);
		return true;
	}

	Ткст src = UpdateGetDir();
	if(src.пустой()) return false;
	src = приставьИмяф(src, дайПозИмяф(exe));
	int dummy;
	if(NoUpdate(exe, src, dummy)) return false;

	Ткст commandline;
	commandline << src << " -update " << FixArg(exe);
	for(int i = 0; i < cmdline.дайСчёт(); i++)
		commandline << ' ' << FixArg(cmdline[i]);
	LLOG("SelfUpdateSelf (running updater): " << commandline);
	return WinExec(commandline, SW_SHOWNORMAL) >= 32;
}

#endif
#endif

}
