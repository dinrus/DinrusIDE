#include "Common.h"

bool Отладчик::Подсказка(const Ткст&, РедакторКода::MouseTip&)
{
	return false;
}

bool deactivation_save;

void DeactivationSave(bool b) // On deactivation, make no prompts, ignore save errors
{
	deactivation_save = b;
}

bool IsDeactivationSave()
{
	return deactivation_save;
}

bool FinishSave(Ткст tmpfile, Ткст outfile)
{
	if(IsDeactivationSave()) {
		переместифл(tmpfile, outfile);
		return true;
	}
	Progress progress;
	int time = msecs();
	for(;;) {
		progress.устВсего(10000);
		progress.устТекст("Сохраняется '" + дайИмяф(outfile) + "'");
		if(!файлЕсть(tmpfile))
			return false;
		удалифл(outfile);
		if(переместифл(tmpfile, outfile))
			return true;
		слейКонсИср();
		Sleep(200);
		if(progress.SetPosCanceled((msecs() - time) % progress.дайВсего())) {
			int art = Prompt(Ктрл::дайИмяПрил(), CtrlImg::exclamation(),
				"Не удаётся сохранить текущий файл.&"
				"Повторить попытку, проигнорировать или сохранить его в другом месте?",
				"Сохранить как...", "Повторить", "Игнорировать");
			if(art < 0)
				return false;
			if(art && AnySourceFs().ExecuteSaveAs())
				outfile = AnySourceFs();
			progress.устПоз(0);
		}
	}
}

bool FinishSave(Ткст outfile)
{
	return FinishSave(outfile + ".$tmp", outfile);
}

bool SaveFileFinish(const Ткст& filename, const Ткст& data)
{
	if(!сохраниФайл(filename + ".$tmp", data)) {
		if(IsDeactivationSave())
			return true;
		Exclamation("Ошибка при создании временного файла " + filename);
		return false;
	}
	return FinishSave(filename);
}

bool SaveChangedFileFinish(const Ткст& filename, const Ткст& data)
{
	if(data == загрузиФайл(filename))
		return true;
	return SaveFileFinish(filename, data);
}

ВекторМап<Ткст, Ткст>& sWorkspaceCfg()
{
	static ВекторМап<Ткст, Ткст> h;
	return h;
}

Вектор<Событие<> >& sWorkspaceCfgFlush()
{
	static Вектор<Событие<> > h;
	return h;
}

void    RegisterWorkspaceConfig(const char *имя)
{
	ПРОВЕРЬ(sWorkspaceCfg().найди(имя) < 0);
	sWorkspaceCfg().добавь(имя);
}

void    RegisterWorkspaceConfig(const char *имя, Событие<>  WhenFlush)
{
	RegisterWorkspaceConfig(имя);
	sWorkspaceCfgFlush().добавь(WhenFlush);
}


Ткст& WorkspaceConfigData(const char *имя)
{
	return sWorkspaceCfg().дайДобавь(имя);
}

void  SerializeWorkspaceConfigs(Поток& s)
{
	int i;
	for(i = 0; i < sWorkspaceCfgFlush().дайСчёт(); i++)
		sWorkspaceCfgFlush()[i]();
	ВекторМап<Ткст, Ткст>& cfg = sWorkspaceCfg();
	int version = 0;
	s / version;
	int count = cfg.дайСчёт();
	s / count;
	for(i = 0; i < count; i++) {
		Ткст имя;
		if(s.сохраняется())
			имя = cfg.дайКлюч(i);
		s % имя;
		int q = cfg.найди(имя);
		if(q >= 0)
			s % cfg[q];
		else
		{
			Ткст dummy;
			s % dummy;
		}
	}
	s.Magic();
}

bool GuiPackageResolver(const Ткст& Ошибка, const Ткст& path, int line)
{
prompt:
	switch(Prompt(Ктрл::дайИмяПрил(), CtrlImg::exclamation(),
	              Ошибка + "&При разборе пакета " + DeQtf(path),
		          "Редактировать \\& Повторить", "Игнорировать",  "Стоп")) {
	case 0:
		if(!PromptYesNo("Игнорирование повредит пакет. Всё, что после "
			            "точки ошибки будет потеряно.&Продолжить ?"))
			goto prompt;
		return false;
	case 1: {
			ТопОкно win;
			СтрокРедакт edit;
			edit.уст(загрузиФайл(path));
			edit.устКурсор(edit.дайПоз(line));
			win.Титул(path);
			win.добавь(edit.SizePos());
			win.пуск();
			сохраниФайл(path, edit.дай());
		}
		return true;;
	case -1:
		exit(1);
	}
	return false;
}

void CleanModules()
{
	for(int i = 0; i < дайСчётМодульИСР(); i++)
		дайМодульИСР(i).CleanUsc();
}

bool IdeModuleUsc(СиПарсер& p)
{
	for(int i = 0; i < дайСчётМодульИСР(); i++)
		if(дайМодульИСР(i).ParseUsc(p))
			return true;
	return false;
}

static void ReadMacro(СиПарсер& p)
{
	IdeMacro macro;
	if(p.ткст_ли()) {
		macro.menu = p.читайТкст();
		if(p.сим(':'))
			macro.submenu = p.читайТкст();
	}
	if(!p.сим_ли('{'))
		macro.hotkey = разбериОписКлюча(p);
	EscLambda& l = macro.code.CreateLambda();
	const char *t = p.дайУк();
	l.filename = p.дайИмяф();
	l.line = p.дайСтроку();
	if(!p.сим('{'))
		p.выведиОш("отсутствует '{'");
	SkipBlock(p);
	l.code = Ткст(t, p.дайУк());
	Массив<IdeMacro>& mlist = UscMacros();
	if(macro.hotkey) {
		int f = найдиИндексПоля(mlist, &IdeMacro::hotkey, macro.hotkey);
		if(f >= 0) {
			вКонсоль(фмт("%s(%d): дубликат горячей клавиши макроса %s\n", l.filename, l.line, GetKeyDesc(macro.hotkey)));
			const EscLambda& lambda = UscMacros()[f].code.GetLambda();
			вКонсоль(фмт("%s(%d): ранее определено здесь\n", lambda.filename, lambda.line));
		}
	}
	if(!пусто_ли(macro.menu)) {
		for(int i = 0; i < mlist.дайСчёт(); i++)
			if(mlist[i].menu == macro.menu && mlist[i].submenu == macro.submenu) {
				вКонсоль(фмт("%s(%d): дубликат элемента меню макроса (%s:%s)\n",
					l.filename, l.line, macro.menu, macro.submenu));
				const EscLambda& lambda = UscMacros()[i].code.GetLambda();
				вКонсоль(фмт("%s(%d): ранее определено здесь\n", lambda.filename, lambda.line));
				break;
			}
	}
	mlist.добавь(macro);
}

ИНИЦБЛОК {
	Пакет::SetPackageResolver(GuiPackageResolver);
	UscSetCleanModules(CleanModules);
	SetIdeModuleUsc(IdeModuleUsc);
	UscSetReadMacro(ReadMacro);
}

bool IdeExit;
bool IdeAgain;

bool копируйПапку(const char *dst, const char *src, Progress *pi)
{
	if(strcmp(src, dst) == 0)
		return true;
	реализуйДир(dst);
	if(pi)
		pi->устТекст(dst);
	ФайлПоиск ff(приставьИмяф(src, "*"));
	while(ff) {
		if(pi && pi->StepCanceled())
			return false;
		Ткст s = приставьИмяф(src, ff.дайИмя());
		Ткст d = приставьИмяф(dst, ff.дайИмя());
		if(ff.папка_ли())
			if(!копируйПапку(d, s, pi))
				return false;
		if(ff.файл_ли())
			if(!сохраниФайл(d, загрузиФайл(s)))
				return false;
		ff.следщ();
	}
	return true;
}

bool HasSvn()
{
	Ткст dummy;
	static bool b = Sys("svn", dummy) >= 0;
	return b;
}

#ifdef PLATFORM_WIN32

Ткст GetInternalGitPath()
{
	return дайФайлИзПапкиИсп("bin/mingit/cmd/git.exe");
}

bool HasGit()
{
	Ткст dummy;
	static bool b = файлЕсть(GetInternalGitPath()) || Sys("git", dummy) >= 0;
	return b;
}

#else

bool HasGit()
{
	Ткст dummy;
	static bool b = Sys("git", dummy) >= 0;
	return b;
}

#endif

int MaxAscent(Шрифт f)
{
	return max(f.GetAscent(), f().Bold().GetAscent(),
	           f().Italic().GetAscent(), f().Bold().Italic().GetAscent());
}
