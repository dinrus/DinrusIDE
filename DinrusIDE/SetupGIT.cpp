#include "DinrusIDE.h"

#ifndef bmYEAR
#include <build_info.h>
#endif

struct Dialog: WithSetupGITLayout<TopWindow>
{
	Dialog(){
		ok.Ok() << Acceptor(IDOK);
		cancel.Cancel() << Rejector(IDCANCEL);
	}

};

//Клонировать исходники U++
bool SetupUppGITMaster()
{
	bool all;
	String prefix, postfix;
	Dialog dlg;
	CtrlLayoutOKCancel(dlg, "Клонировать исходники U++ с github");
	SelectDirButton dir_browse("Целевая папка");
	dir_browse.Attach(dlg.dir);
		
#ifdef PLATFORM_WIN32
	dlg.dir <<= GetExeDirFile("upp");
#else
	dlg.dir <<= GetHomeDirFile("dinrus/upp");
#endif
	static MapConvert revcv;

	UrepoConsole console;
	String dir;
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		console.Clear();
		dir = ~dlg.dir;
		all = dlg.all;
		prefix = ~~dlg.prefix;
		postfix = ~~dlg.postfix;
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
		console.Log("Готово", Gray());
		console.Perform();
	}
	
#ifdef PLATFORM_WIN32
	String out = GetExeDirFile("DinrusIDE_output");
	String ass = GetExeFolder();
#else
	String out = GetHomeDirFile(".cache/DinrusIDE_output");
	String ass = GetConfigFolder();
#endif
	RealizeDirectory(out);

	String uppsrc = AppendFileName(dir, "upp");

	auto MakeAssembly = [&](String b) {
		String name = GetFileTitle(b);
		String a = ass + '/' + prefix + name + postfix + ".var";
		if(name != "upp")
			b << ';' << uppsrc;
		console.Log("Создаётся сборка " + a);
		SaveFile(a,
			"UPP = " + AsCString(b) + ";\r\n"
			"OUTPUT = " + AsCString(out) + ";\r\n"
		);
	};

	for(const char *nest : { "uppsrc", "reference", "examples", "tutorial" })
		MakeAssembly(AppendFileName(dir, nest));

	if(all)
		for(const char *nest : { "autotest", "benchmarks", "uppbox", "upptst" })
			MakeAssembly(AppendFileName(dir, nest));

	console.Log("Готово", Gray());
	console.Perform();
	return true;
}

//Клонировать исходники Dinrus
bool SetupDinrusGITMaster()
{
	bool all;
    Dialog dlg;
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
		all = dlg.all;
		dlg.Close();
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
		console.Log("Готово", Gray());
		console.Perform();
	}
		
  if(all)
  {
    CtrlLayoutOKCancel(dlg, "Клонировать примеры приложений с github");
    SelectDirButton dir_browse("Целевая папка");
	dir_browse.Attach(dlg.dir);
	#ifdef PLATFORM_WIN32
		dlg.dir = GetExeDirFile("examples");
	#else
		dlg.dir = GetHomeDirFile("dinrus/examples");
	#endif
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		dir = ~dlg.dir;
		dlg.Close();
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
		  console.Log("Готово", Gray());
		  console.Perform();
	}
	//////////////////
	CtrlLayoutOKCancel(dlg, "Клонировать уроки по программированию с github");
	#ifdef PLATFORM_WIN32
		dlg.dir = GetExeDirFile("tutorial");
	#else
		dlg.dir = GetHomeDirFile("dinrus/tutorial");
	#endif
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		dir = ~dlg.dir;
		dlg.Close();
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
		   console.Log("Готово", Gray());
           console.Perform();
	}
	///////////////////
    CtrlLayoutOKCancel(dlg, "Клонировать уроки по программированию с github");
	#ifdef PLATFORM_WIN32
		dlg.dir = GetExeDirFile("reference");
	#else
		dlg.dir = GetHomeDirFile("dinrus/reference");
	#endif
	for(;;) {
		if(dlg.Run() != IDOK)
			return false;
		dir = ~dlg.dir;
		dlg.Close();
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
		   console.Log("Готово", Gray());
	       console.Perform();
	}
  }
  
#ifdef PLATFORM_WIN32
	String out = GetExeDirFile("DinrusIDE_output");
	String ass = GetExeFolder();
	String myapps = GetExeDirFile("dev");
#else
	String out = GetHomeDirFile(".cache/DinrusIDE_output");
	String ass = GetConfigFolder();
	String myapps = GetHomeDirFile("dinrus/dev");
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
	console.Log("Готово", Gray());
	console.Perform();
	return true;
}
