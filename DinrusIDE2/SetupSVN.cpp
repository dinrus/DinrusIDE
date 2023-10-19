#include "DinrusIDE2.h"

#ifndef bmYEAR
#include <build_info.h>
#endif

bool SetupGITMaster()
{
	WithSetupGITLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Клонировать исходники U++ с github");
	SelectDirButton dir_browse("Целевая папка");
	dir_browse.Attach(dlg.dir);
#ifdef PLATFORM_WIN32
	dlg.dir <<= GetExeDirFile("upp.src");
#else
	dlg.dir <<= GetHomeDirFile("upp.src");
#endif
	static MapConvert revcv;

	UrepoConsole console;
	String dir;
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		console.Clear();
		dir = ~dlg.dir;
		bool direxists = DirectoryExists(dir);
		bool fileexists = FileExists(dir);
		if((direxists || fileexists)) {
			if(!PromptYesNo("Целевой путь уже существует, перезаписать?"))
				continue;
			DeleteFolderDeep(dir, true);
			FileDelete(dir);
		}
		if(console.System("git clone https://github.com/ultimatepp/ultimatepp.git " + dir) == 0)
			break;
		console.Perform();
	}

#ifdef PLATFORM_WIN32
	String out = GetExeDirFile("out");
	String ass = GetExeFolder();
	String myapps = GetExeDirFile("Projects");
#else
	String out = GetHomeDirFile("out");
	String ass = GetConfigFolder();
	String myapps = GetHomeDirFile("Projects");
#endif
	RealizeDirectory(out);

	String src = AppendFileName(dir, "src");

	auto MakeAssembly = [&](String b) {
		String name = GetFileTitle(b);
		String a = ass + '/' + ~~dlg.prefix + name + ~~dlg.postfix + ".var";
		if(name != "src")
			b << ';' << src;
		console.Log("Создаётся сборка " + a);
		SaveFile(a,
			"UPP = " + AsCString(b) + ";\r\n"
			"OUTPUT = " + AsCString(out) + ";\r\n"
		);
	};

	for(const char *nest : { "src", "reference", "examples", "tutorial" })
		MakeAssembly(AppendFileName(dir, nest));

	if(dlg.all)
		for(const char *nest : { "autotest", "benchmarks", "uppbox", "upptst" })
			MakeAssembly(AppendFileName(dir, nest));

	MakeAssembly(myapps);

	console.Perform();
	return true;
}