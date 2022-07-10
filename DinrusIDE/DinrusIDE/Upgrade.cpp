#include "DinrusIDE.h"

void Иср::UpgradeTheIDE()
{
	Ткст idepath = дайФПутьИсп();
	Ткст newpath = idepath + ".new";
	Ткст bakpath(~idepath, дайПозРасшф(idepath));
	bakpath << ".bak" << GetExeExt();
	
	int tbak = targetmode;
	Ткст tmbak = сохраниКакТкст(release);

	release.target_override = true;
	release.target = newpath;
#ifdef PLATFORM_WIN32
	release.linkmode = 0;
#else
	release.linkmode = 1;
#endif
	release.createmap = false;
	release.version.очисть();
	release.def.blitz = false;
	release.def.debug = false;
	release.package.очисть();
	
	targetmode = 1;

	if(Build()) {
		удалифл(bakpath);
		if(файлЕсть(bakpath))
			Exclamation("Не удаётся удалить&" + bakpath);
		else {
			PutVerbose("Перемещается " + idepath + " в " + bakpath);
			переместифл(idepath, bakpath);
			PutVerbose("Перемещается " + newpath + " в " + idepath);
			переместифл(newpath, idepath);
			вКонсоль("Апгрейд завершён, пожалуйста, перезапустите ИСР.");
		}
	}

	грузиИзТкст(release, tmbak);
	targetmode = tbak;
}

void Иср::InstallDesktop()
{
	Ткст apps = дайФайлИзДомПапки(".local/share/applications");
	if(PromptYesNo("Записть theide.desktop в&[* \1" + apps + "\1]?&"
	               "После этого ИСР РНЦП Динрус будет доступна через приложения рабочего стола.")) {
		Ткст icon = конфигФайл("theide.png");
		PNGEncoder().сохраниФайл(icon, IdeImg::PackageLarge());
		Ткст path = apps + "/theide.desktop";
		RealizePath(path);
		РНЦП::сохраниФайл(path,
			"[Desktop Entry]\n"
			"Encoding=UTF-8\n"
			"Имя=TheIDE\n"
			"GenericName=TheIDE\n"
			"Comment=U++ IDE\n"
			"MimeType=application/x-upp;\n"
			"Exec=" + дайФПутьИсп() + "\n"
			"Иконка=" + icon + "\n"
			"Terminal=false\n"
			"Type=Application\n"
			"Categories=Development;IDE\n"
			"StartupNotify=false\n"
		);
	}
}
