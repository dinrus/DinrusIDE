#include "Core.h"

void Построитель::ChDir(const Ткст& path)
{
	host->ChDir(path);
}

Ткст TrimSlash(Ткст s)
{
	while(findarg(*s.последний(), '/', '\\') >= 0)
		s.обрежь(s.дайСчёт() - 1);
	return s;
}

Ткст Построитель::GetPathQ(const Ткст& path) const
{
	return '\"' + TrimSlash(path) + '\"';
}

Вектор<Хост::ИнфОФайле> Построитель::дайИнфОФайле(const Вектор<Ткст>& path) const
{
	return host->дайИнфОФайле(path);
}

Хост::ИнфОФайле Построитель::дайИнфОФайле(const Ткст& path) const
{
	return дайИнфОФайле(Вектор<Ткст>() << path)[0];
}

Время Построитель::дайФВремя(const Ткст& path) const
{
	return дайИнфОФайле(path);
}

Ткст Построитель::CmdX(const char *s)
{ // expand ` character delimited sections by executing them as commands
	Ткст r, cmd;
	bool cmdf = false;
	for(; *s; s++)
		if(*s == '`') {
			if(cmdf) {
				r << Sys(cmd);
				cmd.очисть();
			}
			cmdf = !cmdf;
		}
		else
			(cmdf ? cmd : r).конкат(*s);
	int q = r.найди(' ');
	if(r.дайСчёт() > 8000 && q >= 0) {
		Ткст rn = CatAnyPath(outdir, какТкст(tmpfilei.дайДобавь(outdir, 0)++) + ".cmd");
		PutVerbose("Генерируется респонс-файл: " << rn);
		PutVerbose(r);
		r.замени("\\", "/"); // clang win32 needs this
		сохраниФайл(rn, r.середина(q + 1));
		r = r.середина(0, q) + " @" + rn;
	}
	return r;
}

int Построитель::выполни(const char *cmdline)
{
	return host->выполни(CmdX(cmdline));
}

int Построитель::выполни(const char *cl, Поток& out)
{
	return host->выполни(CmdX(cl), out);
}
