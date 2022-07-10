#include "DinrusIDE.h"

// DEPRECATED

extern FileSel& sSD();

Ткст NormalizePathNN(const Ткст& path)
{
	return пусто_ли(path) ? path : нормализуйПуть(path);
}

#ifdef PLATFORM_WIN32
bool ExistProgram(Ткст& bin, const char *dir, const char *file)
{
	Ткст win = нормализуйПуть(GetWindowsDirectory());
	if(файлЕсть(приставьИмяф(win.середина(0, 3) + dir, file))) {
		bin = win.середина(0, 3) + dir;
		return true;
	}
	return false;
}

class CtrlList : public КтрлМассив {
public:
	typedef CtrlList ИМЯ_КЛАССА;
	CtrlList() { AutoHideSb().NoGrid().NoHeader(); добавьКолонку(Ткст()); }

	template <class T>
	CtrlList& AddLayout(T& ctrl) {
		добавь();
		int Индекс = дайСчёт() - 1;
		дезактивируйСтроку(Индекс);
		устКтрл(Индекс, 0, ctrl.SizePos());
		SetLineCy(Индекс, ctrl.GetLayoutSize().cy);
		добавьСепаратор();
		return *this;
	}
};

void AutoSetup()
{
	WithAutoSetupLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Автонастройка методов Ultimate++");
	dlg.Sizeable().Zoomable().устПрям(Размер(640, 480));

	WithAutoSetupMSC12<КтрлРодитель> msc12; CtrlLayout(msc12);
	WithAutoSetupMSC11<КтрлРодитель> msc11; CtrlLayout(msc11);
	WithAutoSetupMSC10<КтрлРодитель> msc10; CtrlLayout(msc10);
	WithAutoSetupMSC9<КтрлРодитель> msc9; CtrlLayout(msc9);
	WithAutoSetupMSC8<КтрлРодитель> msc8; CtrlLayout(msc8);
	WithAutoSetupMSC7_1<КтрлРодитель> msc7_1; CtrlLayout(msc7_1);
	WithAutoSetupMinGW<КтрлРодитель> mingw; CtrlLayout(mingw);
	WithAutoSetupOWC<КтрлРодитель> owc; CtrlLayout(owc);
	WithAutoSetupOther<КтрлРодитель> other; CtrlLayout(other);

	CtrlList msclist, mingwlist, owclist, otherlist;

	Ткст dir = дайПапкуФайла(дайФПутьИсп());

	msclist.AddLayout(msc12)
		.AddLayout(msc11)
		.AddLayout(msc10)
		.AddLayout(msc9)
		.AddLayout(msc8)
		.AddLayout(msc7_1);

	mingwlist.AddLayout(mingw);
	owclist.AddLayout(owc);
	otherlist.AddLayout(other);

	dlg.tab.добавь(msclist.SizePos(), "Visual C++");
	dlg.tab.добавь(mingwlist.SizePos(), "MinGW GCC");
	dlg.tab.добавь(owclist.SizePos(), "открой Watcom");
	dlg.tab.добавь(otherlist.SizePos(), "Другой");

	Ткст mw = дайФайлИзПапкиИсп("mingw");
	ФайлПоиск ff(mw);
	if(ff && ff.директория_ли())
		mingw.dir <<= mw;
	else
		mingw.dir <<=
			NormalizePathNN(дайТкстВинРега("Inno настрой: App Path",
		       "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW_is1"));
	if (пусто_ли(mingw.dir))
		mingw.dir <<= NormalizePathNN(дайТкстВинРега("InstallLocation",
						"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\TDM-GCC" ));
	mingw.method <<= "MINGW";
	mingw.create <<= !пусто_ли(mingw.dir) && !файлЕсть(приставьИмяф(dir, "MINGW.bm"));

	Ткст owcdir = дайСреду("WATCOM");
	if(owcdir.пустой())
		owcdir = "C:\\WATCOM";

	if(дирЕсть(owcdir))
		owc.dir <<= owcdir;

	owc.method <<= "OWC";
	owc.create <<= !пусто_ли(owc.dir)  && !файлЕсть(приставьИмяф(dir, "OWC.bm"));


	Ткст vs = дайТкстВинРега("ProductDir", "SOFTWARE\\Microsoft\\VisualStudio\\7.1\\настрой\\VC");
	msc7_1.dir <<=
		NormalizePathNN(
			Nvl(
				дайТкстВинРега("InstallLocation",
	                            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
	                            "{362882AE-E40A-4435-B214-6420634C401F}"),
	            vs)
	    );
	msc7_1.method <<= "MSC71";
	msc7_1.create <<= !пусто_ли(msc7_1.dir) && !файлЕсть(приставьИмяф(dir, "MSC71.bm"));

	Ткст sdk8 = дайТкстВинРега("InstallationFolder",
	                             "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.0",
	                             HKEY_CURRENT_USER);
	Ткст bin8;
	if(!пусто_ли(sdk8)) {
		sdk8 = нормализуйПуть(sdk8);
		msc8.sdk <<= sdk8;
		msc7_1.sdk <<= sdk8;
		msc8.dir <<= bin8 = sdk8;
	}
	else {
		msc8.dir <<= bin8 = NormalizePathNN(
			дайТкстВинРега("8.0", "SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7")
		);
		msc8.sdk <<= NormalizePathNN(дайТкстВинРега("Install Dir", "SOFTWARE\\Microsoft\\MicrosoftSDK\\InstalledSDKs\\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3"));
	}
	msc8.method <<= "MSC8";
	msc8.create <<= !пусто_ли(msc8.dir) && !файлЕсть(приставьИмяф(dir, "MSC8.bm"));
	if(bin8.дайДлину() && файлЕсть(приставьИмяф(bin8, "VC\\Bin\\x64\\cl.exe"))
	   && !файлЕсть(приставьИмяф(dir, "MSC8x64.bm"))) {
		msc8.create64 = true;
	}
	msc8.method64 <<= "MSC8x64";
	Ткст sdk9 = NormalizePathNN(дайТкстВинРега("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0",
		                                       HKEY_LOCAL_MACHINE));
	if(пусто_ли(sdk9))
		sdk9 = NormalizePathNN(дайТкстВинРега("InstallationFolder",
	                                           "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.1",
	                                           HKEY_CURRENT_USER));
	if(пусто_ли(sdk9))
		sdk9 = NormalizePathNN(дайТкстВинРега("InstallationFolder",
		                                       "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.0A",
		                                       HKEY_CURRENT_USER));  
	Ткст bin9;
	if(!пусто_ли(sdk9) && файлЕсть(приставьИмяф(sdk9, "VC\\Bin\\cl.exe")))
		bin9 = sdk9;
	else
		ExistProgram(bin9, "Program Files (x86)\\Microsoft Visual Studio 9.0", "VC\\Bin\\cl.exe")
		|| ExistProgram(bin9, "Program Files\\Microsoft Visual Studio 9.0", "VC\\Bin\\cl.exe");
	msc9.sdk <<= sdk9;
	msc9.dir <<= bin9;
	msc9.method <<= "MSC9";
	msc9.create <<= !пусто_ли(msc9.dir) && !файлЕсть(приставьИмяф(dir, "MSC9.bm"));
	Ткст vc9_64 = приставьИмяф(bin9, "VC\\Bin\\x64");
	if(!файлЕсть(приставьИмяф(vc9_64, "cl.exe")))
		vc9_64 = приставьИмяф(bin9, "VC\\Bin\\amd64");
	if(bin9.дайДлину() && файлЕсть(приставьИмяф(vc9_64, "cl.exe"))
	   && !файлЕсть(приставьИмяф(dir, "MSC9x64.bm")))
		msc9.create64 = true;
	msc9.method64 <<= "MSC9x64";


	Ткст sdk10 = NormalizePathNN(дайТкстВинРега("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.1",
		                                       HKEY_LOCAL_MACHINE));
	if (пусто_ли(sdk10))
		sdk10 = NormalizePathNN(дайТкстВинРега("InstallationFolder",
		                                        "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A",
		                                        HKEY_LOCAL_MACHINE));
	Ткст bin10;
	if(!пусто_ли(sdk10) && файлЕсть(приставьИмяф(sdk10, "VC\\Bin\\cl.exe")))
		bin10 = sdk10;
	else
		ExistProgram(bin10, "Program Files (x86)\\Microsoft Visual Studio 10.0", "VC\\Bin\\cl.exe")
		|| ExistProgram(bin10, "Program Files\\Microsoft Visual Studio 10.0", "VC\\Bin\\cl.exe");
	msc10.sdk <<= sdk10;
	msc10.dir <<= bin10;
	msc10.method <<= "MSC10";
	msc10.create <<= !пусто_ли(msc10.dir) && !файлЕсть(приставьИмяф(dir, "MSC10.bm"));
	Ткст vc10_64 = приставьИмяф(bin10, "VC\\Bin\\x64");
	if(!файлЕсть(приставьИмяф(vc10_64, "cl.exe")))
		vc10_64 = приставьИмяф(bin10, "VC\\Bin\\amd64");
	if(bin10.дайДлину() && файлЕсть(приставьИмяф(vc10_64, "cl.exe"))
	   && !файлЕсть(приставьИмяф(dir, "MSC10x64.bm")))
		msc10.create64 = true;
	msc10.method64 <<= "MSC10x64";


	Ткст sdk11 = NormalizePathNN(дайТкстВинРега("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.0",
		                                       HKEY_LOCAL_MACHINE));
	Ткст bin11;
	if(!пусто_ли(sdk11) && файлЕсть(приставьИмяф(sdk11, "VC\\bin\\cl.exe")))
		bin11 = sdk11;
	else
		ExistProgram(bin11, "Program Files (x86)\\Microsoft Visual Studio 11.0", "VC\\bin\\cl.exe")
		|| ExistProgram(bin11, "Program Files\\Microsoft Visual Studio 11.0", "VC\\bin\\cl.exe");
	msc11.sdk <<= sdk11;
	msc11.dir <<= bin11;
	msc11.method <<= "MSC11";
	msc11.create <<= !пусто_ли(msc11.dir) && !файлЕсть(приставьИмяф(dir, "MSC11.bm"));
	Ткст vc11_64 = приставьИмяф(bin11, "VC\\bin\\x64");
	if(!файлЕсть(приставьИмяф(vc11_64, "cl.exe")))
		vc11_64 = приставьИмяф(bin11, "VC\\bin\\x86_amd64");
	if(bin11.дайДлину() && файлЕсть(приставьИмяф(vc11_64, "cl.exe"))
	   && !файлЕсть(приставьИмяф(dir, "MSC11x64.bm")))
		msc11.create64 = true;
	msc11.method64 <<= "MSC11x64";


	Ткст sdk12 = NormalizePathNN(дайТкстВинРега("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.1",
		                                       HKEY_LOCAL_MACHINE));
	Ткст bin12;
	if(!пусто_ли(sdk12) && файлЕсть(приставьИмяф(sdk12, "VC\\bin\\cl.exe")))
		bin12 = sdk12;
	else
		ExistProgram(bin12, "Program Files (x86)\\Microsoft Visual Studio 12.0", "VC\\bin\\cl.exe")
		|| ExistProgram(bin12, "Program Files\\Microsoft Visual Studio 12.0", "VC\\bin\\cl.exe");
	msc12.sdk <<= sdk12;
	msc12.dir <<= bin12;
	msc12.method <<= "MSC12";
	msc12.create <<= !пусто_ли(msc12.dir) && !файлЕсть(приставьИмяф(dir, "MSC12.bm"));
	Ткст vc12_64 = приставьИмяф(bin12, "VC\\bin\\x64");
	if(!файлЕсть(приставьИмяф(vc12_64, "cl.exe")))
		vc12_64 = приставьИмяф(bin12, "VC\\bin\\x86_amd64");
	if(bin12.дайДлину() && файлЕсть(приставьИмяф(vc12_64, "cl.exe"))
	   && !файлЕсть(приставьИмяф(dir, "MSC12x64.bm")))
		msc12.create64 = true;
	msc12.method64 <<= "MSC12x64";


	other.mysql <<= NormalizePathNN(дайТкстВинРега("Location", "SOFTWARE\\MySQL AB\\MySQL Server 4.1"));

	Ткст sdl = NormalizePathNN(конфигФайл("sdl"));
	if(дирЕсть(sdl))
		other.sdl <<= sdl;

	Массив< ФреймПраво<Кнопка> > bd;
	выбДир(mingw.dir, bd.добавь());
	выбДир(owc.dir, bd.добавь());
	выбДир(msc7_1.dir, bd.добавь());
	выбДир(msc8.dir, bd.добавь());
	выбДир(msc9.dir, bd.добавь());
	выбДир(msc10.dir, bd.добавь());
	выбДир(msc11.dir, bd.добавь());
	выбДир(msc12.dir, bd.добавь());
	выбДир(msc7_1.sdk, bd.добавь());
	выбДир(msc8.sdk, bd.добавь());
	выбДир(msc9.sdk, bd.добавь());
	выбДир(msc10.sdk, bd.добавь());
	выбДир(msc11.sdk, bd.добавь());
	выбДир(msc12.sdk, bd.добавь());
	выбДир(other.sdl, bd.добавь());
	выбДир(other.mysql, bd.добавь());
	if(dlg.пуск() != IDOK)
		return;

	Ткст exe;
	Ткст include;
	Ткст lib;

	sdl = ~other.sdl;
	if(!пусто_ли(sdl)) {
		exe << ";" << приставьИмяф(sdl, "lib");
		include << ";" << приставьИмяф(sdl, "include");
		lib << ";" << приставьИмяф(sdl, "lib");
	}

	Ткст mysql = ~other.mysql;
	Ткст mlib;
	if(!пусто_ли(mysql)) {
		exe << ";" << приставьИмяф(mysql, "bin");
		include << ";" << приставьИмяф(mysql, "include");
		mlib << ";" << приставьИмяф(mysql, "bin");
	}

	Ткст mingwdir = ~mingw.dir;
	if(!пусто_ли(mingwdir) && mingw.create) {
		Ткст m = ~mingw.method;
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"GCC\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-O0 -gstabs\";\n"
			"RELEASE_BLITZ = \"1\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O3 -ffunction-sections\";\n" // -funroll-loops ?
			"DEBUGGER = \"gdb\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + какТкстСи(приставьИмяф(mingwdir, "bin") + exe) + ";\n"
			"INCLUDE = " + какТкстСи(приставьИмяф(mingwdir, "include") + include) + ";\n"
			"LIB = " + какТкстСи(приставьИмяф(mingwdir, "lib") + lib + mlib) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"-msse2\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	owcdir = ~owc.dir;
	if(!пусто_ли(owcdir) && owc.create) {
		Ткст m = ~owc.method;
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"OWC\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-d2\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-ot\";\n"
			"DEBUGGER = \"gdb\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + какТкстСи(
				приставьИмяф(owcdir, "binnt") + ';' +
				приставьИмяф(owcdir, "binw") +
				exe) + ";\n"
			"INCLUDE = " + какТкстСи(приставьИмяф(owcdir, "h") + ';' +
				приставьИмяф(owcdir, "h\\nt") +
				include) + ";\n"
			"LIB = " + какТкстСи(приставьИмяф(owcdir, "lib386") + ';' +
				приставьИмяф(owcdir, "lib386\\nt") +
				lib + mlib) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	if(!пусто_ли(mysql)) {
		lib << ";" << приставьИмяф(mysql, "lib");
	}

	Ткст vs12 = ~msc12.dir;
	if(!пусто_ли(vs12) && msc12.create64) {
		Ткст vc = приставьИмяф(vs12, "VC");
		Ткст m = ~msc12.method64;
		Ткст sdk = ~msc12.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		Ткст vc_lib = приставьИмяф(vc, "lib\\x64");
		if(!дирЕсть(vc_lib))
			vc_lib = приставьИмяф(vc, "lib\\amd64");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC12X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs12, "Common7\\IDE") + ';' +
							vc12_64 + ';' +
							приставьИмяф(sdk, "bin\\x64") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "include") + ';' +
							приставьИмяф(sdk, "Include\\um") + ';' +
							приставьИмяф(sdk, "Include\\shared") + ';' +
							приставьИмяф(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							vc_lib + ';' +
							приставьИмяф(sdk, "Lib\\winv6.3\\um\\x64") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	if(!пусто_ли(vs12) && msc12.create) {
		Ткст vc = приставьИмяф(vs12, "VC");
		Ткст m = ~msc12.method;
		Ткст sdk = ~msc12.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC12\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs12, "Common7\\IDE") + ';' +
							приставьИмяф(vc, "bin") + ';' +
							приставьИмяф(sdk, "bin\\x86") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "include") + ';' +
							приставьИмяф(sdk, "Include\\um") + ';' +
							приставьИмяф(sdk, "Include\\shared") + ';' +
							приставьИмяф(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							приставьИмяф(vc, "lib") + ';' +
							приставьИмяф(sdk, "Lib\\winv6.3\\um\\x86") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	Ткст vs11 = ~msc11.dir;
	if(!пусто_ли(vs11) && msc11.create64) {
		Ткст vc = приставьИмяф(vs11, "VC");
		Ткст m = ~msc11.method64;
		Ткст sdk = ~msc11.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		Ткст vc_lib = приставьИмяф(vc, "Lib\\x64");
		if(!дирЕсть(vc_lib))
			vc_lib = приставьИмяф(vc, "Lib\\amd64");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC11X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs11, "Common7\\Иср") + ';' +
							vc11_64 + ';' +
							приставьИмяф(sdk, "bin\\x64") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include\\um") + ';' +
							приставьИмяф(sdk, "Include\\shared") + ';' +
							приставьИмяф(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							vc_lib + ';' +
							приставьИмяф(sdk, "Lib\\win8\\um\\x64") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	if(!пусто_ли(vs11) && msc11.create) {
		Ткст vc = приставьИмяф(vs11, "VC");
		Ткст m = ~msc11.method;
		Ткст sdk = ~msc11.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC11\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs11, "Common7\\Иср") + ';' +
							приставьИмяф(vc, "bin") + ';' +
							приставьИмяф(sdk, "bin\\x86") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include\\um") + ';' +
							приставьИмяф(sdk, "Include\\shared") + ';' +
							приставьИмяф(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							приставьИмяф(vc, "Lib") + ';' +
							приставьИмяф(sdk, "Lib\\win8\\um\\x86") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	Ткст vs10 = ~msc10.dir;
	if(!пусто_ли(vs10) && msc10.create64) {
		Ткст vc = приставьИмяф(vs10, "Vc");
		Ткст m = ~msc10.method64;
		Ткст sdk = ~msc10.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		Ткст vc_lib = приставьИмяф(vc, "Lib\\x64");
		if(!дирЕсть(vc_lib))
			vc_lib = приставьИмяф(vc, "Lib\\amd64");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC10X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs10, "Common7\\Иср") + ';' +
							vc10_64 + ';' +
							приставьИмяф(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							vc_lib + ';' +
							приставьИмяф(sdk, "Lib\\x64") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	if(!пусто_ли(vs10) && msc10.create) {
		Ткст vc = приставьИмяф(vs10, "Vc");
		Ткст m = ~msc10.method;
		Ткст sdk = ~msc10.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC10\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs10, "Common7\\Иср") + ';' +
							приставьИмяф(vc, "Bin") + ';' +
							приставьИмяф(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							приставьИмяф(vc, "Lib") + ';' +
							приставьИмяф(sdk, "Lib") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	Ткст vs9 = ~msc9.dir;
	if(!пусто_ли(vs9) && msc9.create64) {
		Ткст vc = приставьИмяф(vs9, "Vc");
		Ткст m = ~msc9.method64;
		Ткст sdk = ~msc9.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		Ткст vc_lib = приставьИмяф(vc, "Lib\\x64");
		if(!дирЕсть(vc_lib))
			vc_lib = приставьИмяф(vc, "Lib\\amd64");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC9X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs9, "Common7\\Иср") + ';' +
							vc9_64 + ';' +
							приставьИмяф(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							vc_lib + ';' +
							приставьИмяф(sdk, "Lib\\x64") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	if(!пусто_ли(vs9) && msc9.create) {
		Ткст vc = приставьИмяф(vs9, "Vc");
		Ткст m = ~msc9.method;
		Ткст sdk = ~msc9.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC9\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs9, "Common7\\Иср") + ';' +
							приставьИмяф(vc, "Bin") + ';' +
							приставьИмяф(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							приставьИмяф(vc, "Lib") + ';' +
							приставьИмяф(sdk, "Lib") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	Ткст vs8 = ~msc8.dir;
	if(!пусто_ли(vs8) && msc8.create64) {
		Ткст vc = приставьИмяф(vs8, "Vc");
		Ткст m = ~msc8.method64;
		Ткст sdk = ~msc8.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc, "PlatformSDK");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC8X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs8, "Common7\\Иср") + ';' +
							приставьИмяф(vc, "Bin\\x64") + ';' +
							приставьИмяф(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							приставьИмяф(vc, "Lib\\x64") + ';' +
							приставьИмяф(sdk, "Lib\\x64") +
							lib
			           ) + ";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	if(!пусто_ли(vs8) && msc8.create) {
		Ткст vc = приставьИмяф(vs8, "Vc");
		Ткст m = ~msc8.method;
		Ткст sdk = ~msc8.sdk;
		if(пусто_ли(sdk8))
			sdk = приставьИмяф(vc, "PlatformSDK");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC8\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                приставьИмяф(vs8, "Common7\\Иср") + ';' +
							приставьИмяф(vc, "Bin") + ';' +
							приставьИмяф(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc, "Include") + ';' +
							приставьИмяф(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							приставьИмяф(vc, "Lib") + ';' +
							приставьИмяф(sdk, "Lib") +
							lib
			           ) + ";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}

	Ткст vc7 = ~msc7_1.dir;
	if(путиРавны(vc7, vs))
		vs = приставьИмяф(дайПапкуФайла(нормализуйПуть(vs)), "Common7\\ide") + ";";
	else
		vs.очисть();
	if(!пусто_ли(vc7) && msc7_1.create) {
		Ткст m = ~msc7_1.method;
		Ткст sdk = ~msc7_1.sdk;
		if(пусто_ли(sdk))
			sdk = приставьИмяф(vc7, "PlatformSDK");
		сохраниФайл(
			приставьИмяф(dir, m + ".bm"),
			"BUILDER = \"MSC71\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + какТкстСи(
			                vs +
							приставьИмяф(vc7, "Bin") + ';' +
							приставьИмяф(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + какТкстСи(
							приставьИмяф(vc7, "Include") + ';' +
							приставьИмяф(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + какТкстСи(
							приставьИмяф(vc7, "Lib") + ';' +
							приставьИмяф(sdk, "Lib") +
							lib
			           ) + ";\n"
		);
		сохраниФайл(приставьИмяф(dir, "default_method"), m);
	}
}

void Иср::AutoSetup()
{
	::AutoSetup();
	SyncBuildMode();
	SetBar();
}

#endif

