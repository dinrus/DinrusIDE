/*
* Вся основная инициализация и запуск программы,
* а также применение прочих дополнительных действий,
* выполняется в этом - главном - модуле.
*/
#include "DinrusIDE.h"
#include "CommandLineHandler.h"

#define FUNCTION_NAME UPP_FUNCTION_NAME << "(): "

//Удаление временных файлов в папке конфигурации.
void DelTemps()
{
	FindFile ff(ConfigFile("*.tmp"));
	while(ff) {
		DeleteFile(ConfigFile(ff.GetName()));
		ff.Next();
	}
}

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )
#endif

extern int MemoryProbeFlags;

void Uninstall();

//Тихий режим...
bool SilentMode;

#if defined(PLATFORM_WIN32)
#include <wincon.h>
//Помещает строку в стандартный вывод Windows.
void Puts(const char *s)
{
	dword dummy;
	if(!SilentMode)
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), s, (int)strlen(s), &dummy, NULL);
}
#endif


#ifdef PLATFORM_POSIX
//Помещает строку в стандартный вывод Posix.
void Puts(const char *s)
{
	if(!SilentMode)
		puts(s);
}
#endif

//Вывод сплэш-скрина...
bool splash_screen;

//Заменяет запятую на пробел.
int CommaSpace(int c)
{
	return c == ',' ? ' ' : c;
}

//Удаляет устарелые файлы в кэше.
void ReduceCache()
{
	String cfgdir = ConfigFile("cfg");
	FindFile ff(AppendFileName(cfgdir, "*.*"));
	while(ff) {
		if(ff.IsFile()) {
			String fn = ff.GetName();
			String ext = GetFileExt(fn);
			if(ext != ".aux" && ext != ".cfg")
				if((Date)Time(ff.GetLastAccessTime()) < GetSysDate() - 14)
					DeleteFile(AppendFileName(cfgdir, fn));
		}
		ff.Next();
	}
}

//Проверка, является ли указанный путь путём к сборке.
bool IsAssembly(const String& s)
{
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile())
			if(GetFileTitle(ff.GetName()) == s)
				return true;
	Vector<String> l = Split(s, ',');
	for(int i = 0; i < l.GetCount(); i++)
		if(FindFile(NormalizePath(l[i])).IsFolder())
			return true;
	return false;
}

//Запуск в режиме редактирования.
void StartEditorMode(const Vector<String>& args, Ide& ide, bool& clset)
{
	if(args.IsEmpty() || clset) {
		return;
	}
	
	Vector<String> dir = Split(LoadFile(GetHomeDirFile("usc.path")), ';');
	for(int i = 0; i < dir.GetCount(); i++)
		ide.UscProcessDirDeep(dir[i]);
	for(int i = 0; i < args.GetCount(); i++) {
		if(args[i] != "-f") {
			String file_path = NormalizePath(args[i]);
			
			Logd() << FUNCTION_NAME << "Открывается файл \"" << file_path << "\".";
			
			ide.EditFile(file_path);
			ide.FileSelected();
		}
	}
	
	clset = true;
	ide.EditorMode();
}

// СДЕЛАТЬ: Этот макрос желательно заменить версией
// с std::function.
#undef  GUI_APP_MAIN_HOOK
#define GUI_APP_MAIN_HOOK \
{ \
	BaseCommandLineHandler cmd_handler(CommandLine()); \
	if (cmd_handler.Handle()) \
		return Upp::GetExitCode(); \
}

#ifdef flagMAIN
GUI_APP_MAIN
#else
void AppMain___()
#endif
{
#ifdef flagPEAKMEM
	PeakMemoryProfile();
#endif

	Logi() << UPP_FUNCTION_NAME << "(): " << SplashCtrl::GenerateVersionInfo(' ');

	Ctrl::SetUHDEnabled();
	Ctrl::SetDarkThemeEnabled();
	//Установка имени приложения.
	Ctrl::SetAppName("ИСР РНЦП Динрус");
    //Установка языка.
	SetLanguage(LNG_RUSSIAN);
	//Установка дефолтного набсима.
	SetDefaultCharset(CHARSET_UTF8);
    //Обработчик командной строки
	MainCommandLineHandler cmd_handler(CommandLine());
	//Если хэндл получен, возврат (выход)
	if (cmd_handler.Handle())
		return;
	//Получение аргументов командной строки в arg
	auto arg = clone(cmd_handler.GetArgs());
    //Установка лимитированного размера для лога
	SetVppLogSizeLimit(200000000);
	//Проверка лицензии.
	if(!CheckLicense())
		return;
#ifdef PLATFORM_WIN32
//Автосамонастройка на Windows.
	AutoInstantSetup();
#endif
//Проверка на наличие файлов .var в папке конфигурации.
	bool hasvars = FindFile(ConfigFile("*.var"));
#ifdef PLATFORM_POSIX
// Удаление старых файлов консольных сценариев.
	RemoveConsoleScripts();
//Установка домашней папки по переменной среды DINRUS_HOME.
	String home = Environment().Get("DINRUS_HOME", Null);
	if(!IsNull(home))
		SetHomeDirectory(home);
	//Устанавливается только, если такая переменная была задана пользователем.
//
	if(!hasvars) {
		if(!Install(hasvars))
			return;
	}
#endif

	if(!hasvars)
		SetupDinrusGITMaster();

	if(!FileExists(BlitzBaseFile()))
		ResetBlitz();
	
	if(!FileExists(SearchEnginesFile()))
		SearchEnginesDefaultSetup();

	bool dosplash = true;
	for(int i = 0; i < arg.GetCount(); i++)
		if(arg[i] == "--nosplash") {
			dosplash = false;
			arg.Remove(i);
			break;
		}
	for(int i = 0; i < arg.GetCount(); i++) {
	#ifdef PLATFORM_WIN32
		if(arg[i] == "!") {
			String cmdline;
			for(++i; i < arg.GetCount(); i++) {
				if(!IsNull(cmdline))
					cmdline << ' ';
				String h = arg[i];
				if(h.Find(' ') >= 0)
					cmdline << '\"' << arg[i] << '\"';
				else
					cmdline << arg[i];
			}
			int n = cmdline.GetLength() + 1;
			Buffer<char> cmd(n);
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

			SetConsoleCP(65001);
			SetConsoleOutputCP(65001);

			if(CreateProcess(NULL, cmd, &sa, &sa, TRUE,
				             NORMAL_PRIORITY_CLASS,
			                 NULL, NULL, &si, &pi)) {
				WaitForSingleObject(pi.hProcess, INFINITE);
				dword exitcode = 255;
				GetExitCodeProcess(pi.hProcess, &exitcode);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				Puts("<--- Завершено за " + GetPrintTime(time) + ", код выхода: " + AsString(exitcode) + " --->");
			}
			else
				Puts("Не удаётся запустить " + cmdline);
			char h[1];
			dword dummy;
			ReadFile(GetStdHandle(STD_INPUT_HANDLE), h, 1, &dummy, NULL);
			return;
		}
	#endif
	/*
		if(arg[i] == "#git_ask_pass") {
			String s;
			for(;;) {
				int c = getchar();
				if(c == EOF)
					break;
				s.Cat(c);
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

#ifndef _DEBUG
	try {
#endif
		void RegisterLayDes(); RegisterLayDes();
		void RegisterIconDes(); RegisterIconDes();

		splash_screen = true;
		
		{ // Установка скина и шрифта до конструирования 'реальной' ИСР
			Ide h;
			LoadFromFile(h);
			if(h.gui_font_override)
				SetStdFont(h.gui_font);
			auto v = GetAllChSkins();
			Ctrl::SetSkin(v[clamp(h.chstyle, 0, v.GetCount() - 1)].a);
		}

		Ide ide;
		ide.Maximize();
		bool clset = false;
		
		ide.LoadConfig();
		
		if(arg.GetCount() == 1) {
			if(arg[0].EndsWith(".upp")) {
				Vector<String> names = Split(arg[0], DIR_SEP);
				int last = names.GetCount() - 1;
				for(int i = last; i >= 0; --i) {
					if (IsAssembly(names[i])) {
						String package = names[last];
						package.Remove(package.GetCount() - 4, 4);
						LoadVars(names[i]);
						ide.SetMain(package);
						clset = true;
						break;
					}
				}
			}
		} else {
			if(arg.GetCount() == 2 && IsAssembly(arg[0])) {
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
			Ctrl::ProcessEvents();
		}

		ide.editor_bottom.Zoom(0);
		ide.right_split.Zoom(0);
		if(FileExists(ConfigFile("developide"))) {
	#ifdef PLATFORM_WIN32
			InstallCrashDump();
	#endif
			Ini::user_log = true;
		}
		
		String ppdefs = ConfigFile("global.defs");
	#ifndef _DEBUG
		if(!FileExists(ppdefs))
	#endif
			SaveFile(ppdefs, GetStdDefs());

		SetPPDefs(LoadFile(ppdefs));
		
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
					ide.Run();
				ide.SaveConfigOnTime();
				ide.SaveLastMain();
				ide.Close();
			}
		}
		while(IdeAgain);

		SaveCodeBase();
		DelTemps();
		DeletePCHFiles();
		ReduceCache();
#ifndef _DEBUG
	}
	catch(const CParser::Error& e) {
		ErrorOK("Ошибка парсера " + e);
		LOG("!!!!! Ошибка парсера " + e);
	}
	catch(const Exc& e) {
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
EXITBLOCK {
	RDUMP(*PeakMemoryProfile());
	RDUMP(MemoryProfile());
	MemoryDump();
}
#endif
