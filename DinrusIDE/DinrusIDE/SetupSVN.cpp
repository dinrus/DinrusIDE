#include "DinrusIDE.h"

#ifndef bmYEAR
#include <build_info.h>
#endif

bool SetupGITMaster()
{
	WithSetupGITLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Клонировать исходники U++ с github");
	SelectDirButton dir_browse("Целевая папка");
	dir_browse.прикрепи(dlg.dir);
#ifdef PLATFORM_WIN32
	dlg.dir <<= дайФайлИзПапкиИсп("upp.src");
#else
	dlg.dir <<= дайФайлИзДомПапки("upp.src");
#endif
	static МапПреобр revcv;

	UrepoConsole console;
	Ткст dir;
	for(;;) {
		if(dlg.пуск() != IDOK)
			return false;
		console.очисть();
		dir = ~dlg.dir;
		bool direxists = дирЕсть(dir);
		bool fileexists = файлЕсть(dir);
		if((direxists || fileexists)) {
			if(!PromptYesNo("Целевой путь уже существует, перезаписать?"))
				continue;
			DeleteFolderDeep(dir, true);
			удалифл(dir);
		}
		if(console.System("git clone https://github.com/ultimatepp/ultimatepp.git " + dir) == 0)
			break;
		console.Perform();
	}

#ifdef PLATFORM_WIN32
	Ткст out = дайФайлИзПапкиИсп("out");
	Ткст ass = дайПапкуИсп();
	Ткст myapps = дайФайлИзПапкиИсп("MyApps");
#else
	Ткст out = дайФайлИзДомПапки("out");
	Ткст ass = GetConfigFolder();
	Ткст myapps = дайФайлИзДомПапки("MyApps");
#endif
	реализуйДир(out);
	
	Ткст uppsrc = приставьИмяф(dir, "uppsrc");

	auto MakeAssembly = [&](Ткст b) {
		Ткст имя = дайТитулф(b);
		Ткст a = ass + '/' + ~~dlg.prefix + имя + ~~dlg.postfix + ".var";
		if(имя != "uppsrc")
			b << ';' << uppsrc;
		console.Log("Создаётся сборка " + a);
		сохраниФайл(a,
			"РНЦП = " + какТкстСи(b) + ";\r\n"
			"OUTPUT = " + какТкстСи(out) + ";\r\n"
		);
	};

	for(const char *nest : { "uppsrc", "reference", "examples", "tutorial" })
		MakeAssembly(приставьИмяф(dir, nest));
	
	if(dlg.all)
		for(const char *nest : { "autotest", "benchmarks", "uppbox", "upptst" })
			MakeAssembly(приставьИмяф(dir, nest));
	
	MakeAssembly(myapps);

	console.Perform();
	return true;
}
