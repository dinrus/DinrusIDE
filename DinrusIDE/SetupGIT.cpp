#include "DinrusIDE.h"

#ifndef bmYEAR
#include <build_info.h>
#endif
//Клонировать исходники U++
bool SetupUppGITMaster()
{
	WithSetupGITLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Клонировать исходники U++ с github");
	SelectDirButton dir_browse("Целевая папка");
	dir_browse.Attach(dlg.dir);
#ifdef PLATFORM_WIN32
	dlg.dir <<= GetExeDirFile("uppsrc");
#else
	dlg.dir <<= GetHomeDirFile("dinrus/uppsrc");
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
	String myapps = GetExeDirFile("dev");
#else
	String out = GetHomeDirFile("out");
	String ass = GetConfigFolder();
	String myapps = GetHomeDirFile("dev");
#endif
	RealizeDirectory(out);

	String uppsrc = AppendFileName(dir, "uppsrc");

	auto MakeAssembly = [&](String b) {
		String name = GetFileTitle(b);
		String a = ass + '/' + ~~dlg.prefix + name + ~~dlg.postfix + ".var";
		if(name != "uppsrc")
			b << ';' << uppsrc;
		console.Log("Создаётся сборка " + a);
		SaveFile(a,
			"UPP = " + AsCString(b) + ";\r\n"
			"OUTPUT = " + AsCString(out) + ";\r\n"
		);
	};

	for(const char *nest : { "uppsrc", "reference", "examples", "tutorial" })
		MakeAssembly(AppendFileName(dir, nest));

	if(dlg.all)
		for(const char *nest : { "autotest", "benchmarks", "uppbox", "upptst" })
			MakeAssembly(AppendFileName(dir, nest));

	MakeAssembly(myapps);

	console.Perform();
	return true;
}

//Клонировать исходники Dinrus
bool SetupDinrusGITMaster()
{
	WithSetupGITLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Клонировать исходники Dinrus с github");
	SelectDirButton dir_browse("Целевая папка");
	dir_browse.Attach(dlg.dir);
#ifdef PLATFORM_WIN32
	dlg.dir <<= GetExeDirFile("src");
#else
	dlg.dir <<= GetHomeDirFile("dinrus/src");
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
		
		if(console.System("git clone https://github.com/dinrus/DinrusIDE.git " + dir) == 0)
			break;
		console.Perform();
	}
		
  if(dlg.all)
  {
	CtrlLayoutOKCancel(dlg, "Клонировать примеры приложений с github");
	//SelectDirButton dir_browse("Целевая папка");
	//dir_browse.Attach(dlg.dir);
	#ifdef PLATFORM_WIN32
		dlg.dir = GetExeDirFile("examples");
	#else
		dlg.dir = GetHomeDirFile("dinrus/examples");
	#endif
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		dir = ~dlg.dir;
		bool direxists = DirectoryExists(dir);
		bool fileexists = FileExists(dir);
		if((direxists || fileexists)) {
			if(!PromptYesNo("Целевой путь уже существует, перезаписать?"))
				continue;
			DeleteFolderDeep(dir, true);
			FileDelete(dir);
		}
		   if(console.System("git clone https://github.com/dinrus/examples.git " + dir) == 0)
		      break;
		 console.Perform();
	}
	//////////////////
	CtrlLayoutOKCancel(dlg, "Клонировать уроки по программированию с github");
	//SelectDirButton dir_browse("Целевая папка");
	//dir_browse.Attach(dlg.dir);
	#ifdef PLATFORM_WIN32
		dlg.dir = GetExeDirFile("tutorial");
	#else
		dlg.dir = GetHomeDirFile("dinrus/tutorial");
	#endif
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		dir = ~dlg.dir;
		bool direxists = DirectoryExists(dir);
		bool fileexists = FileExists(dir);
		if((direxists || fileexists)) {
			if(!PromptYesNo("Целевой путь уже существует, перезаписать?"))
				continue;
			DeleteFolderDeep(dir, true);
			FileDelete(dir);
		}
		   if(console.System("git clone https://github.com/dinrus/tutorial.git " + dir) == 0)
		    break;
	console.Perform();
	}
	///////////////////
		   
    CtrlLayoutOKCancel(dlg, "Клонировать уроки по программированию с github");
	//SelectDirButton dir_browse("Целевая папка");
	//dir_browse.Attach(dlg.dir);
	#ifdef PLATFORM_WIN32
		dlg.dir = GetExeDirFile("reference");
	#else
		dlg.dir = GetHomeDirFile("dinrus/reference");
	#endif
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		dir = ~dlg.dir;
		bool direxists = DirectoryExists(dir);
		bool fileexists = FileExists(dir);
		if((direxists || fileexists)) {
			if(!PromptYesNo("Целевой путь уже существует, перезаписать?"))
				continue;
			DeleteFolderDeep(dir, true);
			FileDelete(dir);
		}
		
		   if(console.System("git clone https://github.com/dinrus/reference.git " + dir) == 0)
			break;
		console.Perform();
	}
  }
  
#ifdef PLATFORM_WIN32
	String out = GetExeDirFile("out");
	String ass = GetExeFolder();
	String myapps = GetExeDirFile("dev");
#else
	String out = GetHomeDirFile("out");
	String ass = GetConfigFolder();
	String myapps = GetHomeDirFile("dev");
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

	MakeAssembly(AppendFileName(dir, "src"));

	if(dlg.all)
		for(const char *nest : { "reference", "examples", "tutorial" })
			MakeAssembly(AppendFileName(dir, nest));

	MakeAssembly(myapps);

	console.Perform();
	return true;
}
