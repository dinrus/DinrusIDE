#include "ide.h"

void Ide::UpgradeTheIDE()
{
	String idepath = GetExeFilePath();
	String newpath = idepath + ".new";
	String bakpath(~idepath, GetFileExtPos(idepath));
	bakpath << ".bak" << GetExeExt();
	
	int tbak = targetmode;
	String tmbak = StoreAsString(release);

	release.target_override = true;
	release.target = newpath;
#ifdef PLATFORM_WIN32
	release.linkmode = 0;
#else
	release.linkmode = 1;
#endif
	release.createmap = false;
	release.version.Clear();
	release.def.blitz = false;
	release.def.debug = false;
	release.package.Clear();
	
	targetmode = 1;

	if(Build()) {
		FileDelete(bakpath);
		if(FileExists(bakpath))
			Exclamation("Не удаётся удалить&" + bakpath);
		else {
			PutVerbose("Перемещается " + idepath + " в " + bakpath);
			FileMove(idepath, bakpath);
			PutVerbose("Перемещается " + newpath + " в " + idepath);
			FileMove(newpath, idepath);
			PutConsole("Апгрейд завершён, пожалуйста, перезапустите ИСР.");
		}
	}

	LoadFromString(release, tmbak);
	targetmode = tbak;
}

void Ide::InstallDesktop()
{
	String apps = GetHomeDirFile(".local/share/applications");
	if(PromptYesNo("Записать theide.desktop в&[* \1" + apps + "\1]?&"
	               "После этого ИСР РНЦП Динрус будет доступна через приложения рабочего стола.")) {
		String icon = ConfigFile("theide.png");
		PNGEncoder().SaveFile(icon, IdeImg::PackageLarge());
		String path = apps + "/theide.desktop";
		RealizePath(path);
		Upp::SaveFile(path,
			"[Desktop Entry]\n"
			"Encoding=UTF-8\n"
			"Name=TheIDE\n"
			"GenericName=TheIDE\n"
			"Comment=U++ IDE\n"
			"MimeType=application/x-upp;\n"
			"Exec=" + GetExeFilePath() + "\n"
			"Icon=" + icon + "\n"
			"Terminal=false\n"
			"Type=Application\n"
			"Categories=Development;IDE\n"
			"StartupNotify=false\n"
		);
	}
}
