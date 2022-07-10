#include "DinrusIDE.h"

IdeCalc::IdeCalc()
{
	Highlight("калькулятор");
	NoHorzScrollbar();
	HideBar();
}

int LfToSpaceFilter(int c)
{
	return c == '\n' ? ' ' : c;
}

void IdeCalc::выполни()
{
	int li = дайСчётСтрок() - 1;
	if(выделение_ли()) {
		Ткст s = дайВыделение();
		if(s.дайДлину() < 80) {
			устКурсор(дайДлину64());
			Paste(фильтруй(s, LfToSpaceFilter).вШТкст());
		}
		return;
	}
	if(дайСтроку(дайКурсор64()) != li) {
		ШТкст s = дайШСтроку(дайСтроку(дайКурсор64()));
		if(s[0] == '$') s = s.середина(1);
		устКурсор(дайДлину64());
		Paste(s);
		return;
	}
	Ткст txt;
	try {
		МассивМап<Ткст, EscValue>& g = UscGlobal();
		for(int i = 0; i < g.дайСчёт(); i++)
			vars.дайДобавь(g.дайКлюч(i)) = g[i];
		Ткст s = дайУтф8Строку(li);
		if(пусто_ли(s))
			return;
		EscValue v = Evaluatex(s, vars);
		txt = v.вТкст(дайРазм().cx / max(1, дайШрифт().Info()['x']), 4, true);
		vars.дайДобавь("_") = v;
	}
	catch(СиПарсер::Ошибка e) {
		const char *x = strchr(e, ':');
		txt << "ОШИБКА: " << (x ? x + 1 : ~e);
	}
	устКурсор(дайПоз64(li));
	Paste("$");
	устКурсор(дайДлину64());
	Paste("\n");
	Paste(вУтф32(txt));
	Paste("\n");
}

void IdeCalc::леваяДКлик(Точка p, dword flags)
{
	РедакторКода::леваяДКлик(p, flags);
	if(выделение_ли())
		выполни();
}

bool IdeCalc::Ключ(dword ключ, int count)
{
	switch(ключ) {
	case K_ENTER:
		выполни();
		break;
	default:
		return РедакторКода::Ключ(ключ, count);
	}
	return true;
}
