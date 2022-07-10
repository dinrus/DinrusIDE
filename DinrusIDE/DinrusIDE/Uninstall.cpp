#include "ide.h"

#ifdef PLATFORM_WIN32

Ткст GetShellFolder(const char *имя, HKEY тип)
{
	return дайТкстВинРега(имя,
	         "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
		     тип);
}

void DelKey(const char *dir, const char *ключ)
{
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return;
	RegDeleteKey(hkey, ключ);
	RegCloseKey(hkey);
}

void RemoveWindowsItems()
{
	DeleteFile(приставьИмяф(GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE), "Ultimate++ IDE.lnk"));
	DeleteFile(приставьИмяф(GetShellFolder("Desktop", HKEY_CURRENT_USER), "TheIde.lnk"));

	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Ultimate++",
	       "DisplayName");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Ultimate++",
	       "UninstallString");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
	       "Ultimate++");
}

void Uninstall()
{
	Ткст path = дайФПутьИсп();
	Ткст bat = приставьИмяф(GetShellFolder("Desktop", HKEY_CURRENT_USER), "removepp.bat");
	Ткст dir = дайПапкуФайла(path);
	if(!PromptYesNo("[*3 Do you wish to uninstall Ultimate`+`+ development system ?&&]"
	                "Uninstall will remove [* " + DeQtf(dir) + "] directory and all "
	                "registry and desktop items associated with Ultimate`+`+.")) return;
	DeleteFolderDeep(dir);
	RemoveWindowsItems();
	сохраниФайл(bat,
		":повтори\r\n"
		"del \"" + path + "\"\r\n"
		"if exist \"" + path + "\" goto повтори\r\n"
		"del \"" + дайФайлИзПапкиИсп("theide.log") + "\"\r\n"
		"rmdir \"" + dir + "\"\r\n"
		"del \"" + bat + "\"\r\n"
	);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	char h[512];
	strcpy(h, bat);
	if(!CreateProcess(NULL, h, NULL, NULL, FALSE,
	                  IDLE_PRIORITY_CLASS, NULL, GetShellFolder("Desktop", HKEY_CURRENT_USER),
	                  &si, &pi))
		Exclamation("Uninstall failed to remove some files...&" + DeQtf(дайПоследнОшСооб()));
}

#else

void Uninstall() {}

#endif
