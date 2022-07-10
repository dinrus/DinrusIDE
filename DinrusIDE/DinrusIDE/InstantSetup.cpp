#include "DinrusIDE.h"

#ifdef PLATFORM_WIN32

struct DirFinder {
	Progress pi;

	Вектор<Ткст> dir;

	Ткст дай(const Ткст& substring, const char *files);
	Ткст GetVisualStudioFolder(int drive, Ткст programFolder);
	void   GatherDirs(Индекс<Ткст>& path, const Ткст& dir);
	
	DirFinder();
};

void DirFinder::GatherDirs(Индекс<Ткст>& path, const Ткст& dir)
{
	pi.Step();
	path.найдиДобавь(dir);
	ФайлПоиск ff(dir + "/*");
	while(ff) {
		if(ff.папка_ли())
			GatherDirs(path, ff.дайПуть());
		ff.следщ();
	}
}

DirFinder::DirFinder()
{
	Индекс<Ткст> path;
	
	pi.устТекст("Настраиваются методы построения");

	for(int i = 0; i < 3; i++) {
		HKEY ключ = 0;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		                "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\Folders", 0,
		                KEY_READ|decode(i, 0, KEY_WOW64_32KEY, 1, KEY_WOW64_64KEY, 0),
		                &ключ) == ERROR_SUCCESS) {
			int i = 0;
			for(;;) {
				char  значение[255];
				unsigned long valueSize = 250;
				*значение = 0;
		
				if(RegEnumValue(ключ, i++, значение, &valueSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
					break;
				
				path.найдиДобавь(значение);
				pi.Step();
			}
			RegCloseKey(ключ);
		}
	}
	
	Массив<ИнфОФС::ИнфОФайле> root = стдИнфОФС().найди(Null);
	for(int i = 0; i < root.дайСчёт(); i++) {
		if(root[i].root_style == ИнфОФС::ROOT_FIXED) {
			int drive = *root[i].filename;
			Ткст x86pf = GetVisualStudioFolder(drive, дайПапкуПрограмХ86());
			if(дирЕсть(x86pf)) {
				GatherDirs(path, x86pf);
			}
			Ткст pf = GetVisualStudioFolder(drive, дайПапкуПрограм());
			if(дирЕсть(pf)) {
				GatherDirs(path, pf);
			}
		}
	}

	for(Ткст s : path) {
		s = впроп(s);
		s.замени("\\", "/");
		while(s.обрежьКонец("/"));
		dir.добавь(s);
	}
}

Ткст DirFinder::дай(const Ткст& substring, const char *files)
{
	Ткст path;
	Вектор<int> versions;
	Вектор<Ткст> фн = разбей(files, ';');
	for(const auto& d : dir) {
		pi.Step();
		int p = d.найди(substring);
		if(p >= 0) {
			Ткст cp = d;
			for(const auto& f : фн)
				if(!файлЕсть(приставьИмяф(d, f))) {
					cp.очисть();
					break;
				}
			if(cp.дайСчёт()) {
				Вектор<Ткст> h = разбей(cp.середина(p), [] (int c) { return цифра_ли(c) ? 0 : c; });
				Вектор<int> vers;
				for(const auto& hh : h)
					vers.добавь(atoi(hh));
				if(сравниДиапазоны(vers, versions) > 0) {
					path = cp;
					versions = clone(vers);
				}
			}
		}
	}
	return path;
}

Ткст DirFinder::GetVisualStudioFolder(int drive, Ткст pf)
{
	constexpr auto VISUAL_STUDIO_FOLDER = "Microsoft Visual Studio";
	
	pf.уст(0, drive);
	pf = приставьИмяф(pf, VISUAL_STUDIO_FOLDER);
	return pf;
}

bool CheckDirs(const Вектор<Ткст>& d, int count)
{
	if(d.дайСчёт() < count)
		return false;
	for(int i = 0; i < count; i++)
		if(!дирЕсть(d[i]))
			return false;
	return true;
}

void bmSet(ВекторМап<Ткст, Ткст>& bm, const char *id, const Ткст& val)
{
	Ткст& t = bm.дайДобавь(id);
	t = Nvl(t, val);
}

void InstantSetup()
{
	bool dirty = false;
	Ткст default_method;
	
	Ткст bin = дайФайлИзПапкиИсп("bin");

	if(дирЕсть(bin + "/clang"))
		for(int x64 = 0; x64 < 2; x64++) {
			Ткст method = x64 ? "CLANGx64" : "CLANG";
		#ifdef INSTANT_TESTING
			method << "Test";
		#endif
			ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	
			Вектор<Ткст> bins = разбей(bm.дай("PATH", ""), ';');
			Вектор<Ткст> incs = разбей(bm.дай("INCLUDE", ""), ';');
			Вектор<Ткст> libs = разбей(bm.дай("LIB", ""), ';');
		#ifdef INSTANT_TESTING
			if(CheckDirs(bins, 3) && CheckDirs(incs, 2) && CheckDirs(libs, 2)) {
				if(!x64)
					default_method = Nvl(default_method, method);
				continue;
			}
		#endif
	
			bmSet(bm, "BUILDER", "CLANG");
			bmSet(bm, "COMPILER", x64 ? "" : "i686-w64-mingw32-c++");
			bmSet(bm, "COMMON_OPTIONS", "");
			bmSet(bm, "COMMON_CPP_OPTIONS", "");
			bmSet(bm, "COMMON_C_OPTIONS", "");
			bmSet(bm, "COMMON_LINK", "");
			bmSet(bm, "COMMON_FLAGS", "");
			bmSet(bm, "DEBUG_INFO", "2");
			bmSet(bm, "DEBUG_BLITZ", "1");
			bmSet(bm, "DEBUG_LINKMODE", "0");
			bmSet(bm, "DEBUG_OPTIONS", "");
			bmSet(bm, "DEBUG_FLAGS", "");
			bmSet(bm, "DEBUG_LINK", "-Wl,--stack,20000000");
			bmSet(bm, "RELEASE_BLITZ", "1");
			bmSet(bm, "RELEASE_LINKMODE", "0");
			bmSet(bm, "RELEASE_OPTIONS", "-O3 ");
			bmSet(bm, "RELEASE_FLAGS", "");
			bmSet(bm, "RELEASE_LINK", "-Wl,--stack,20000000");
			bmSet(bm, "DEBUGGER", "gdb");
			bmSet(bm, "ALLOW_PRECOMPILED_HEADERS", "1");
			bmSet(bm, "DISABLE_BLITZ", "");
			
	//		bmSet(bm, "LINKMODE_LOCK", "0");
	
			Ткст clang = bin + "/clang";

			bins.по(0) = clang + "/bin";
			bins.по(1) = clang + (x64 ? "/x86_64-w64-mingw32/bin" : "/i686-w64-mingw32/bin");
			bins.по(2) = дайФайлИзПапкиИсп(x64 ? "bin/SDL2/lib/x64" : "bin/SDL2/lib/x86");
			bins.по(3) = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/bin" : "bin/pgsql/x86/bin");
			bins.по(4) = дайФайлИзПапкиИсп(x64 ? "bin/mysql/lib64" : "bin/mysql/lib32");

			incs.по(0) = дайФайлИзПапкиИсп("bin/SDL2/include");
			incs.по(1) = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/include" : "bin/pgsql/x86/include");
			incs.по(2) = дайФайлИзПапкиИсп(x64 ? "bin/mysql/include" : "bin/mysql/include");

			libs.по(0) = дайФайлИзПапкиИсп(x64 ? "bin/SDL2/lib/x64" : "bin/SDL2/lib/x86");
			libs.по(1) = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/lib" : "bin/pgsql/x86/lib");
			libs.по(2) = дайФайлИзПапкиИсп(x64 ? "bin/mysql/lib64" : "bin/mysql/lib32");
	
			bm.дайДобавь("PATH") = Join(bins, ";");
			bm.дайДобавь("INCLUDE") = Join(incs, ";");
			bm.дайДобавь("LIB") = Join(libs, ";");
			
			SaveVarFile(конфигФайл(method + ".bm"), bm);
			dirty = true;
	
			if(x64)
				default_method = Nvl(default_method, method);
		}

	enum { VS_2015, VS_2017, BT_2017, VS_2019, VSP_2019, BT_2019, VS_2022, VSP_2022, BT_2022 };
	DirFinder df;

	for(int version = VS_2015; version <= BT_2022; version++)
		for(int x64 = 0; x64 < 2; x64++) {
			Ткст x86method = decode(version, VS_2015, "MSVS15",
			                                   VS_2017, "MSVS17", BT_2017, "MSBT17",
			                                   VS_2019, "MSVS19", VSP_2019, "MSVSP19", BT_2019, "MSBT19",
			                                   VS_2022, "MSVS22", VSP_2022, "MSVSP22", BT_2022, "MSBT22",
			                                   "MSBT");
			Ткст x64s = x64 ? "x64" : "";
			Ткст method = x86method + x64s;
			Ткст builder = decode(version, VS_2015, "MSC15",
			                                 VS_2017, "MSC17", BT_2017, "MSC17",
			                                 VS_2019, "MSC19", VSP_2019, "MSC19", BT_2019, "MSC19",
			                                 VS_2022, "MSC22", VSP_2022, "MSC22", BT_2022, "MSC22",
			                                 "MSC22"
			                 ) + взаг(x64s);
		
		#ifdef INSTANT_TESTING
			method << "Test";
		#endif
	
			Ткст vc, bin, inc, lib, kit81;
		
			ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
			Вектор<Ткст> bins = разбей(bm.дай("PATH", ""), ';');
			Вектор<Ткст> incs = разбей(bm.дай("INCLUDE", ""), ';');
			Вектор<Ткст> libs = разбей(bm.дай("LIB", ""), ';');
		#ifdef INSTANT_TESTING
			if(CheckDirs(bins, 2) && CheckDirs(incs, 4) && CheckDirs(libs, 3)) {
				if(x64)
					default_method = Nvl(default_method, x86method);
			
				continue;
			}
		#endif

			if(version == VS_2015)
				vc = df.дай("/microsoft visual studio 14.0/vc", "bin/cl.exe;bin/lib.exe;bin/link.exe;bin/mspdb140.dll");
			else
				vc = df.дай(decode(version, BT_2017, "/microsoft visual studio/2017/buildtools/vc/tools/msvc",
				                            VS_2017, "/microsoft visual studio/2017/community/vc/tools/msvc",
				                            BT_2019, "/microsoft visual studio/2019/buildtools/vc/tools/msvc",
				                            VS_2019, "/microsoft visual studio/2019/community/vc/tools/msvc",
				                            VSP_2019, "/microsoft visual studio/2019/professional/vc/tools/msvc",
				                            BT_2022, "/microsoft visual studio/2022/buildtools/vc/tools/msvc",
				                            VS_2022, "/microsoft visual studio/2022/community/vc/tools/msvc",
				                            VSP_2022, "/microsoft visual studio/2022/professional/vc/tools/msvc",
				                            ""),
				            x64 ? "bin/hostx64/x64/cl.exe;bin/hostx64/x64/mspdb140.dll"
				                : "bin/hostx86/x86/cl.exe;bin/hostx86/x86/mspdb140.dll");

			bin = df.дай("/windows kits/10/bin", "x86/makecat.exe;x86/accevent.exe");
			inc = df.дай("/windows kits/10", "um/adhoc.h");
			lib = df.дай("/windows kits/10", "um/x86/kernel32.lib");
			
			bool ver17 = version >= VS_2017;
	
			if(inc.дайСчёт() == 0 || lib.дайСчёт() == 0) // workaround for situation when 8.1 is present, but 10 just partially
				kit81 = df.дай("/windows kits/8.1", "include");
			
			LOG("=============");
			DUMP(method);
			DUMP(vc);
			DUMP(bin);
			DUMP(inc);
			DUMP(kit81);
			DUMP(lib);
	
			if(vc.дайСчёт() && bin.дайСчёт() && (inc.дайСчёт() && lib.дайСчёт() || kit81.дайСчёт())) {
				bins.по(0) = vc + (ver17 ? (x64 ? "/bin/hostx64/x64" : "/bin/hostx86/x86") : (x64 ? "/bin/amd64" : "/bin"));
				bins.по(1) = bin + (x64 ? "/x64" : "/x86");

				int ii = 2;

				Ткст& sdlbin = bins.по(ii++);
				if(пусто_ли(sdlbin) || впроп(sdlbin).найди("sdl2") >= 0)
					sdlbin = дайФайлИзПапкиИсп(x64 ? "bin/SDL2/lib/x64" : "bin/SDL2/lib/x86");
				
				Ткст& pgbin = bins.по(ii++);
				if(пусто_ли(pgbin) || впроп(pgbin).найди("pgsql") >= 0)
					pgbin = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/bin" : "bin/pgsql/x86/bin");

				Ткст& mybin = bins.по(ii++);
				if(пусто_ли(mybin) || впроп(mybin).найди("pgsql") >= 0)
					mybin = дайФайлИзПапкиИсп(x64 ? "bin/mysql/lib64" : "bin/mysql/lib32");
				
				incs.по(0) = vc + "/include";
				ii = 1;
				if(inc.дайСчёт()) {
					incs.по(ii++) = inc + "/um";
					incs.по(ii++) = inc + "/ucrt";
					incs.по(ii++) = inc + "/shared";
				}
				if(kit81.дайСчёт()) {
					incs.по(ii++) = kit81 + "/include/um";
					incs.по(ii++) = kit81 + "/include/ucrt";
					incs.по(ii++) = kit81 + "/include/shared";
				}
	
				Ткст& sslinc = incs.по(ii++);
				if(пусто_ли(sslinc) || впроп(sslinc).найди("openssl") >= 0)
					sslinc = дайФайлИзПапкиИсп("bin/openssl/include");

				Ткст& sdlinc = incs.по(ii++);
				if(пусто_ли(sdlinc) || впроп(sdlinc).найди("sdl2") >= 0)
					sdlinc = дайФайлИзПапкиИсп("bin/SDL2/include");

				Ткст& pginc = incs.по(ii++);
				if(пусто_ли(pginc) || впроп(pginc).найди("pgsql") >= 0)
					pginc = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/include" : "bin/pgsql/x86/include");

				Ткст& myinc = incs.по(ii++);
				if(пусто_ли(myinc) || впроп(myinc).найди("mysql") >= 0)
					myinc = дайФайлИзПапкиИсп(x64 ? "bin/mysql/include" : "bin/mysql/include");
				
				libs.по(0) = vc + (ver17 ? (x64 ? "/lib/x64" : "/lib/x86") : (x64 ? "/lib/amd64" : "/lib"));
				ii = 1;
				if(lib.дайСчёт()) {
					libs.по(ii++) = lib + (x64 ? "/ucrt/x64" : "/ucrt/x86");
					libs.по(ii++) = lib + (x64 ? "/um/x64" : "/um/x86");
				}
				if(kit81.дайСчёт()) {
					libs.по(ii++) = kit81 + (x64 ? "/lib/winv6.3/um/x64" : "/lib/winv6.3/um/x86");
				}
				Ткст& ssllib = libs.по(ii++);
				if(пусто_ли(ssllib) || впроп(ssllib).найди("openssl") >= 0)
					ssllib = дайФайлИзПапкиИсп(x64 ? "bin/openssl/lib64" : "bin/openssl/lib32");
				
				Ткст& sdllib = libs.по(ii++);
				if(пусто_ли(sdllib) || впроп(sdllib).найди("sdl2") >= 0)
					sdllib = дайФайлИзПапкиИсп(x64 ? "bin/SDL2/lib/x64" : "bin/SDL2/lib/x86");

				Ткст& pglib = libs.по(ii++);
				if(пусто_ли(pglib) || впроп(pglib).найди("pgsql") >= 0)
					pglib = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/lib" : "bin/pgsql/x86/lib");

				Ткст& mylib = libs.по(ii++);
				if(пусто_ли(mylib) || впроп(mylib).найди("pgsql") >= 0)
					mylib = дайФайлИзПапкиИсп(x64 ? "bin/mysql/lib64" : "bin/mysql/lib32");
			
				bm.дайДобавь("BUILDER") = builder;
				bmSet(bm, "COMPILER", "");
				bmSet(bm, "COMMON_OPTIONS", x64 ? "/bigobj" : "/bigobj /D_ATL_XP_TARGETING");
				bmSet(bm, "COMMON_CPP_OPTIONS", "");
				bmSet(bm, "COMMON_C_OPTIONS", "");
				bmSet(bm, "COMMON_FLAGS", "");
				bmSet(bm, "DEBUG_INFO", "2");
				bmSet(bm, "DEBUG_BLITZ", "1");
				bmSet(bm, "DEBUG_LINKMODE", "0");
				bmSet(bm, "DEBUG_OPTIONS", "-Od");
				bmSet(bm, "DEBUG_FLAGS", "");
				bmSet(bm, "DEBUG_LINK", x64 ? "/STACK:20000000" : "/STACK:10000000");
				bmSet(bm, "RELEASE_BLITZ", "0");
				bmSet(bm, "RELEASE_LINKMODE", "0");
				bmSet(bm, "RELEASE_OPTIONS", "-O2");
				bmSet(bm, "RELEASE_FLAGS", "");
				bmSet(bm, "RELEASE_LINK", x64 ? "/STACK:20000000" : "/STACK:10000000");
				bmSet(bm, "DISABLE_BLITZ", "");
				bmSet(bm, "ALLOW_PRECOMPILED_HEADERS", "1");
				bmSet(bm, "DEBUGGER", findarg(version, BT_2017, BT_2019, BT_2022) >= 0 ? Ткст()
				                      : дайПапкуФайла(vc) +  "/Common7/IDE/devenv.exe");

				bm.дайДобавь("PATH") = Join(bins, ";");
				bm.дайДобавь("INCLUDE") = Join(incs, ";");
				bm.дайДобавь("LIB") = Join(libs, ";");
				
				SaveVarFile(конфигФайл(method + ".bm"), bm);
				dirty = true;
	
				if(!x64)
					default_method = Nvl(default_method, x86method);
	
				DUMP(конфигФайл(method + ".bm"));
				DUMPC(incs);
				DUMPC(libs);
				DUMPM(bm);
			}
		}

	if(дирЕсть(bin + "/mingw64")) // hopefully deprecated, but keep it for now
		for(int x64 = 0; x64 < 2; x64++) {
			Ткст method = x64 ? "MINGWx64" : "MINGW";
		#ifdef INSTANT_TESTING
			method << "Test";
		#endif
			ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	
			Вектор<Ткст> bins = разбей(bm.дай("PATH", ""), ';');
			Вектор<Ткст> incs = разбей(bm.дай("INCLUDE", ""), ';');
			Вектор<Ткст> libs = разбей(bm.дай("LIB", ""), ';');
		#ifdef INSTANT_TESTING
			if(CheckDirs(bins, 3) && CheckDirs(incs, 2) && CheckDirs(libs, 2)) {
				if(!x64)
					default_method = Nvl(default_method, method);
				continue;
			}
		#endif
	
			bmSet(bm, "BUILDER", "GCC");
			bmSet(bm, "COMPILER", "");
			bmSet(bm, "COMMON_OPTIONS", "-msse2 -D__CRT__NO_INLINE");
			bmSet(bm, "COMMON_CPP_OPTIONS", "-std=c++14");
			bmSet(bm, "COMMON_C_OPTIONS", "");
			bmSet(bm, "COMMON_LINK", "");
			bmSet(bm, "COMMON_FLAGS", "");
			bmSet(bm, "DEBUG_INFO", "2");
			bmSet(bm, "DEBUG_BLITZ", "1");
			bmSet(bm, "DEBUG_LINKMODE", "0");
			bmSet(bm, "DEBUG_OPTIONS", "-O0");
			bmSet(bm, "DEBUG_FLAGS", "");
			bmSet(bm, "DEBUG_LINK", "-Wl,--stack,20000000");
			bmSet(bm, "RELEASE_BLITZ", "1");
			bmSet(bm, "RELEASE_LINKMODE", "0");
			bmSet(bm, "RELEASE_OPTIONS", "-O2 -ffunction-sections");
			bmSet(bm, "RELEASE_FLAGS", "");
			bmSet(bm, "RELEASE_LINK", "-Wl,--stack,20000000");
			bmSet(bm, "DEBUGGER", "gdb");
			bmSet(bm, "ALLOW_PRECOMPILED_HEADERS", "1");
			bmSet(bm, "DISABLE_BLITZ", "");
			
	//		bmSet(bm, "LINKMODE_LOCK", "0");
	
			Ткст m = x64 ? "64" : "32";
			Ткст binx = bin + (x64 ? "/mingw64/64" : "/mingw64/32");
			Ткст mingw = binx + (x64 ? "/x86_64-w64-mingw32" : "/i686-w64-mingw32");

			bins.по(0) = binx + "/bin";
			bins.по(1) = binx + "/opt/bin";
			bins.по(2) = binx + "/gdb/bin";
			bins.по(3) = дайФайлИзПапкиИсп(x64 ? "bin/SDL2/lib/x64" : "bin/SDL2/lib/x86");
			bins.по(4) = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/bin" : "bin/pgsql/x86/bin");
			bins.по(5) = дайФайлИзПапкиИсп(x64 ? "bin/mysql/lib64" : "bin/mysql/lib32");

			incs.по(0) = mingw + "/include";
			incs.по(1) = binx + "/opt/include";
			incs.по(2) = дайФайлИзПапкиИсп("bin/SDL2/include");
			incs.по(3) = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/include" : "bin/pgsql/x86/include");
			incs.по(4) = дайФайлИзПапкиИсп(x64 ? "bin/mysql/include" : "bin/mysql/include");

			libs.по(0) = mingw + "/lib";
			libs.по(1) = binx + "/opt/lib";
			libs.по(2) = дайФайлИзПапкиИсп(x64 ? "bin/SDL2/lib/x64" : "bin/SDL2/lib/x86");
			libs.по(3) = дайФайлИзПапкиИсп(x64 ? "bin/pgsql/x64/lib" : "bin/pgsql/x86/lib");
			libs.по(4) = дайФайлИзПапкиИсп(x64 ? "bin/mysql/lib64" : "bin/mysql/lib32");
	
			bm.дайДобавь("PATH") = Join(bins, ";");
			bm.дайДобавь("INCLUDE") = Join(incs, ";");
			bm.дайДобавь("LIB") = Join(libs, ";");
			
			SaveVarFile(конфигФайл(method + ".bm"), bm);
			dirty = true;
	
			if(!x64)
				default_method = Nvl(default_method, method);
		}

	if(default_method.дайСчёт())
		сохраниФайл(дайФайлИзПапкиИсп("default_method"), default_method);
	
	static Кортеж<const char *, const char *> ass[] = {
		{ "uppsrc", "#/uppsrc" },
		{ "reference", "#/reference;#/uppsrc" },
		{ "examples", "#/examples;#/uppsrc" },
		{ "tutorial", "#/tutorial;#/uppsrc" },
		{ "MyApps", "#/MyApps;#/uppsrc" },
	};

	Ткст exe = дайФПутьИсп();
	Ткст dir = дайПапкуФайла(exe);
	Ткст out = дайФайлИзПапкиИсп("out");
	реализуйДир(out);

	for(int i = 0; i < __countof(ass); i++) {
		Ткст vf = дайФайлИзПапкиИсп(Ткст(ass[i].a) + ".var");
		ВекторМап<Ткст, Ткст> map;
		bool ok = LoadVarFile(vf, map);
		if(ok) {
			Вектор<Ткст> dir = разбей(map.дай("РНЦП", Ткст()), ';');
			if(dir.дайСчёт() == 0)
				ok = false;
			else
				for(int j = 0; j < dir.дайСчёт(); j++) {
					if(!дирЕсть(dir[j])) {
						ok = false;
						break;
					}
				}
		}
		if(!ok) {
			Ткст b = ass[i].b;
			b.замени("#", dir);
			сохраниФайл(vf,
				"РНЦП = " + какТкстСи(b) + ";\r\n"
				"OUTPUT = " + какТкстСи(out) + ";\r\n"
			);
			dirty = true;
		}
	}

	Иср *ide = dynamic_cast<Иср *>(TheIde());
	if(dirty && ide) {
		ide->SyncBuildMode();
		ide->CodeBaseSync();
		ide->SetBar();
	}
}

void AutoInstantSetup()
{
	Ткст зн = впроп(дайПапкуФайла(дайФПутьИсп())) + "\n" +
	             дайТкстВинРега("MachineGuid", "SOFTWARE\\Microsoft\\Cryptography", HKEY_LOCAL_MACHINE, KEY_WOW64_64KEY);
	Ткст cf = дайФайлИзПапкиИсп("setup-path");
	Ткст sgn0 =  загрузиФайл(cf);
	if(зн != sgn0) {
		InstantSetup();
		сохраниФайл(cf, зн);
		сохраниФайл(cf + ".old", sgn0); // forensics
	}
}

#endif
