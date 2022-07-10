#include "DinrusIDE.h"
#include "CommandLineHandler.h"

#define FUNCTION_NAME UPP_FUNCTION_NAME << "(): "

void DelTemps()
{
	ФайлПоиск ff(конфигФайл("*.tmp"));
	while(ff) {
		DeleteFile(конфигФайл(ff.дайИмя()));
		ff.следщ();
	}
}

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )
#endif

extern int MemoryProbeFlags;

void Uninstall();

bool SilentMode;

#if defined(PLATFORM_WIN32)
#include <wincon.h>

void поместиТ(const char *s)
{
	dword dummy;
	if(!SilentMode)
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), s, (int)strlen(s), &dummy, NULL);
}
#endif


#ifdef PLATFORM_POSIX
void поместиТ(const char *s)
{
	if(!SilentMode)
		puts(s);
}
#endif

bool splash_screen;

int CommaSpace(int c)
{
	return c == ',' ? ' ' : c;
}

void ReduceCache()
{
	Ткст cfgdir = конфигФайл("cfg");
	ФайлПоиск ff(приставьИмяф(cfgdir, "*.*"));
	while(ff) {
		if(ff.файл_ли()) {
			Ткст фн = ff.дайИмя();
			Ткст ext = дайРасшф(фн);
			if(ext != ".aux" && ext != ".cfg")
				if((Дата)Время(ff.дайВремяПоследнДоступа()) < дайСисДату() - 14)
					DeleteFile(приставьИмяф(cfgdir, фн));
		}
		ff.следщ();
	}
}

bool IsAssembly(const Ткст& s)
{
	Вектор<Ткст> varlist;
	for(ФайлПоиск ff(конфигФайл("*.var")); ff; ff.следщ())
		if(ff.файл_ли())
			if(дайТитулф(ff.дайИмя()) == s)
				return true;
	Вектор<Ткст> l = разбей(s, ',');
	for(int i = 0; i < l.дайСчёт(); i++)
		if(ФайлПоиск(нормализуйПуть(l[i])).папка_ли())
			return true;
	return false;
}

void StartEditorMode(const Вектор<Ткст>& args, Иср& ide, bool& clset)
{
	if(args.пустой() || clset) {
		return;
	}
	
	Вектор<Ткст> dir = разбей(загрузиФайл(дайФайлИзДомПапки("usc.path")), ';');
	for(int i = 0; i < dir.дайСчёт(); i++)
		ide.UscProcessDirDeep(dir[i]);
	for(int i = 0; i < args.дайСчёт(); i++) {
		if(args[i] != "-f") {
			Ткст file_path = нормализуйПуть(args[i]);
			
			Logd() << FUNCTION_NAME << "Открывается файл \"" << file_path << "\".";
			
			ide.EditFile(file_path);
			ide.FileSelected();
		}
	}
	
	clset = true;
	ide.EditorMode();
}

// TODO: I do not like that we need to define macro here.
// I opt for std::function version. We need to fix that API
// in 2018.2 release. #1901

#undef  GUI_APP_MAIN_HOOK
#define GUI_APP_MAIN_HOOK \
{ \
	BaseCommandLineHandler cmd_handler(комСтрока()); \
	if (cmd_handler.Handle()) \
		return РНЦП::дайКодВыхода(); \
}

#ifdef flagMAIN
ГЛАВНАЯ_ГИП_ПРИЛ
#else
void AppMain___()
#endif
{
//	Ктрл::ShowRepaint(50);

#ifdef flagPEAKMEM
	PeakMemoryProfile();
#endif

	Logi() << UPP_FUNCTION_NAME << "(): " << SplashCtrl::GenerateVersionInfo(' ');

	Ктрл::SetUHDEnabled();
	Ктрл::SetDarkThemeEnabled();
	Ктрл::устИмяПрил("ИСР РНЦП Динрус");

	SetLanguage(LNG_ENGLISH);
	устДефНабСим(CHARSET_UTF8);

	MainCommandLineHandler cmd_handler(комСтрока());
	if (cmd_handler.Handle())
		return;
	auto arg = clone(cmd_handler.GetArgs());

	SetVppLogSizeLimit(200000000);
	
	if(!CheckLicense())
		return;
#ifdef PLATFORM_WIN32
	AutoInstantSetup();
#endif
	bool hasvars = ФайлПоиск(конфигФайл("*.var"));
#ifdef PLATFORM_POSIX
	RemoveConsoleScripts(); // remove old console-script files

	Ткст home = Environment().дай("UPP_HOME", Null);
	if(!пусто_ли(home))
		устДомПапку(home);
	if(!hasvars) {
		if(!Install(hasvars))
			return;
		сохраниФайл(конфигФайл("version"), IDE_VERSION);
	}
#endif

	if(!hasvars)
		SetupGITMaster();

	if(!файлЕсть(BlitzBaseFile()))
		ResetBlitz();
	
	if(!файлЕсть(SearchEnginesFile()))
		SearchEnginesDefaultSetup();

	bool dosplash = true;
	for(int i = 0; i < arg.дайСчёт(); i++)
		if(arg[i] == "--nosplash") {
			dosplash = false;
			arg.удали(i);
			break;
		}
	for(int i = 0; i < arg.дайСчёт(); i++) {
	#ifdef PLATFORM_WIN32
		if(arg[i] == "!") {
			Ткст cmdline;
			for(++i; i < arg.дайСчёт(); i++) {
				if(!пусто_ли(cmdline))
					cmdline << ' ';
				Ткст h = arg[i];
				if(h.найди(' ') >= 0)
					cmdline << '\"' << arg[i] << '\"';
				else
					cmdline << arg[i];
			}
			int n = cmdline.дайДлину() + 1;
			Буфер<char> cmd(n);
			memcpy(cmd, cmdline, n);
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			AllocConsole();
			SetConsoleTitle(cmdline);
			int time = msecs();
			if(CreateProcess(NULL, cmd, &sa, &sa, TRUE,
				             NORMAL_PRIORITY_CLASS,
			                 NULL, NULL, &si, &pi)) {
				WaitForSingleObject(pi.hProcess, INFINITE);
				dword exitcode = 255;
				GetExitCodeProcess(pi.hProcess, &exitcode);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				поместиТ("<--- Завершено за " + GetPrintTime(time) + ", код выхода: " + какТкст(exitcode) + " --->");
			}
			else
				поместиТ("Не удаётся запустить " + cmdline);
			char h[1];
			dword dummy;
			ReadFile(GetStdHandle(STD_INPUT_HANDLE), h, 1, &dummy, NULL);
			return;
		}
	#endif
	/*
		if(arg[i] == "#git_ask_pass") {
			Ткст s;
			for(;;) {
				int c = дайСим();
				if(c == EOF)
					break;
				s.конкат(c);
			}
			PromptOK("ASK_PASS:&\1" + s);
			return;
		}
	*/
	}

#ifdef _DEBUG0
#ifdef PLATFORM_WIN32
	InstantSetup();
#endif
#endif

#ifndef _ОТЛАДКА
	try {
#endif
		void RegisterLayDes(); RegisterLayDes();
		void RegisterIconDes(); RegisterIconDes();

		splash_screen = true;
		
		{ // setup skin and font before the 'real' Иср gets constructed
			Иср h;
			грузиИзФайла(h);
			if(h.gui_font_override)
				SetStdFont(h.gui_font);
			auto v = GetAllChSkins();
			Ктрл::SetSkin(v[clamp(h.chstyle, 0, v.дайСчёт() - 1)].a);
		}

		Иср ide;
		ide.разверни();
		bool clset = false;
		
		ide.LoadConfig();
		
		if(arg.дайСчёт() == 1) {
			if(arg[0].заканчиваетсяНа(".upp")) {
				Вектор<Ткст> names = разбей(arg[0], DIR_SEP);
				int last = names.дайСчёт() - 1;
				for(int i = last; i >= 0; --i) {
					if (IsAssembly(names[i])) {
						Ткст package = names[last];
						package.удали(package.дайСчёт() - 4, 4);
						LoadVars(names[i]);
						ide.SetMain(package);
						clset = true;
						break;
					}
				}
			}
		} else {
			if(arg.дайСчёт() == 2 && IsAssembly(arg[0])) {
				LoadVars(arg[0]);
				ide.SetMain(arg[1]);
				clset=true;
			}
		}
		
		ide.LoadAbbr();

		DelTemps();

#ifndef PLATFORM_COCOA // MacOS sends some weird things to app bundle - disable editor mode for now
		StartEditorMode(arg, ide, clset);
#endif

		if(splash_screen && dosplash && !ide.IsEditorMode()) {
			ShowSplash();
			Ктрл::обработайСобытия();
		}

		ide.editor_bottom.Zoom(0);
		ide.right_split.Zoom(0);
		if(файлЕсть(конфигФайл("developide"))) {
	#ifdef PLATFORM_WIN32
			установиКрашДамп();
	#endif
			Ини::user_log = true;
		}
		
		Ткст ppdefs = конфигФайл("global.defs");
	#ifndef _ОТЛАДКА
		if(!файлЕсть(ppdefs))
	#endif
			сохраниФайл(ppdefs, GetStdDefs());

		SetPPDefs(загрузиФайл(ppdefs));
		
		if(!clset)
			ide.LoadLastMain();
		do {
			IdeAgain = false;
			if(clset || ide.OpenMainPackage()) {
				ide.SaveLastMain();
				ide.isscanning++;
				ide.MakeTitle();
				if(!ide.IsEditorMode())
					SyncRefs();
				ide.FileSelected();
				ide.isscanning--;
				ide.MakeTitle();
				if(!IdeExit)
					ide.пуск();
				ide.SaveConfigOnTime();
				ide.SaveLastMain();
				ide.закрой();
			}
		}
		while(IdeAgain);

		SaveCodeBase();
		DelTemps();
		DeletePCHFiles();
		ReduceCache();
#ifndef _ОТЛАДКА
	}
	catch(const СиПарсер::Ошибка& e) {
		ErrorOK("Ошибка парсера " + e);
		LOG("!!!!! Ошибка парсера " + e);
	}
	catch(const Искл& e) {
		ErrorOK("Исключение " + e);
		LOG("!!!!! Исключение " << e);
	}
#ifdef PLATFORM_POSIX
	catch(...) {
		ErrorOK("Неизвестное исключение !");
		LOG("!!!!! Неизвестное исключение");
	}
#endif
#endif
}

#ifdef flagPEAKMEM
ЭКЗИТБЛОК {
	RDUMP(*PeakMemoryProfile());
	RDUMP(ПрофильПамяти());
	MemoryDump();
}
#endif
