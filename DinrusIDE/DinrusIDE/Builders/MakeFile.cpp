#include "Builders.h"

Ткст MakeIdent(const char *имя)
{
	Ткст out;
	for(; *имя; имя++)
		out << (iscid(*имя) ? *имя : '_');
	return out;
}

static Ткст MakeSourcePath(const Вектор<Ткст>& dirs, Ткст фн, bool raw, bool exporting)
{
	фн = UnixPath(фн);
	for(int i = 0; i < dirs.дайСчёт(); i++)
	{
		int dl = dirs[i].дайДлину();
		if(фн.дайДлину() >= dl + 2 && !memcmp(фн, dirs[i], dl) && фн[dl] == '/') {
			Ткст s;
			if(!exporting)
				s << "$(UPPDIR" << (i + 1) << ")";
			s << AdjustMakePath(фн.дайОбх(dl + 1));
			return s;
		}
	}
	return raw ? Ткст() : AdjustMakePath(фн);
}

Ткст ПостроительСиПП::GetMakePath(Ткст фн) const
{
	return ::GetMakePath(фн, естьФлаг("WIN32"));
}

void ПостроительСиПП::AddMakeFile(MakeFile& makefile, Ткст package,
	const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
	const Индекс<Ткст>& common_config, bool exporting)
{
	Ткст packagepath = PackagePath(package);
	Пакет pkg;
	pkg.грузи(packagepath);
	Ткст packagedir = дайПапкуФайла(packagepath);
	Вектор<Ткст> src = GetUppDirs();
	for(int i = 0; i < src.дайСчёт(); i++)
		src[i] = UnixPath(src[i]);

	bool main = естьФлаг("MAIN");
	bool is_shared = естьФлаг("SO");
	bool libout = !main && !естьФлаг("NOLIB");
	bool win32 = естьФлаг("WIN32");

	Ткст pack_ident = MakeIdent(package);
	Ткст outdir = "OutDir_" + pack_ident;
	Ткст macros = "Macro_" + pack_ident;
	Ткст macdef = "$(Macro)";
	Ткст objext = (естьФлаг("MSC") || естьФлаг("EVC") ? ".obj" : ".o");

	Вектор<Ткст> x(config.дайКлючи(), 1);
	сортируй(x);
	for(int i = 0; i < x.дайСчёт(); i++) {
		if(common_config.найди(x[i]) < 0)
			macdef << " -Dflag" << x[i];
		x[i] = иницШапки(x[i]);
	}

	makefile.outdir << "$(" << outdir << ")";
	makefile.outfile << AdjustMakePath(дайТитулф(NativePath(package)));
	if(main)
		makefile.outfile << GetTargetExt();
	else if(is_shared)
		makefile.outfile << (win32 ? ".dll" : ".so");
	else
		makefile.outfile << (win32 && естьФлаг("MSC") ? ".lib" : ".a");
	makefile.output << (main ? Ткст("$(OutDir)") : makefile.outdir) << makefile.outfile;

	if(main) {
		makefile.config << "CXX = c++\n"
			"LINKER = $(CXX)\n";
		Ткст flags;
		if(естьФлаг("DEBUG"))
			flags << " -D_DEBUG " << debug_options;
		else
			flags << ' ' << release_options;
		if(естьФлаг("DEBUG_MINIMAL"))
			flags << " -ggdb -g1";
		if(естьФлаг("DEBUG_FULL"))
			flags << " -ggdb -g2";
		if(is_shared && !win32)
			flags << " -fPIC ";
		flags << ' ' << Gather(pkg.option, config.дайКлючи());
		makefile.config << "CFLAGS =" << Merge(" ", flags, c_options) << "\n"
			"CXXFLAGS =" << Merge(" ", flags, cpp_options) << "\n"
			"LDFLAGS = " << Merge(" ", common_link, естьФлаг("DEBUG") ? debug_link : release_link)
			             << " $(LINKOPTIONS)\n"
			"LIBPATH =";
		for(int i = 0; i < libpath.дайСчёт(); i++)
			makefile.config << " -L" << GetMakePath(AdjustMakePath(GetPathQ(libpath[i])));
		makefile.config << "\n"
			"AR = ar -sr\n\n";
		Вектор<Ткст> lib;
		Ткст lnk;
		lnk << "$(LINKER)";
		if(!естьФлаг("SHARED"))
			lnk << " -static";
		if(естьФлаг("WIN32")) {
			lnk << " -mwindows";
			if(!естьФлаг("GUI"))
				makefile.linkfiles << " -mconsole";
		}
		lnk << " -o \"$(OutFile)\"";
		if(естьФлаг("DEBUG") || естьФлаг("DEBUG_MINIMAL") || естьФлаг("DEBUG_FULL"))
			lnk << " -ggdb";
		else
			lnk << (!естьФлаг("OSX11") ? " -Wl,-s" : "");

		lnk << " $(LIBPATH)";
		if (!естьФлаг("OSX11"))
			lnk << " -Wl,-O,2";
		lnk << " $(LDFLAGS) -Wl,--start-группа ";

		makefile.linkfiles = lnk;
	}

	makefile.config << outdir << " = $(UPPOUT)"
		<< GetMakePath(AdjustMakePath(Ткст().конкат() << package << '/' << method << '-' << Join(x, "-") << '/')) << "\n"
		<< macros << " = " << macdef << "\n";

	makefile.install << " \\\n\t$(" << outdir << ")";
	makefile.rules << "$(" << outdir << "):\n\tmkdir -p $(" << outdir << ")\n\n";

	Ткст libdep, libfiles;

	libdep << makefile.output << ":";
	if(is_shared)
	{
		libfiles = "c++ -shared -fPIC"; // -v";
		Точка p = ExtractVersion();
		if(!пусто_ли(p.x)) {
			libfiles << " -Xlinker --major-image-version -Xlinker " << p.x;
			if(!пусто_ли(p.y))
				libfiles << " -Xlinker --minor-image-version -Xlinker " << p.y;
		}
		libfiles << " -o ";
	}
	else
		libfiles = "$(AR) ";
	libfiles << makefile.output;

	Вектор<Ткст> libs = разбей(Gather(pkg.library, config.дайКлючи()), ' ');
	for(int i = 0; i < libs.дайСчёт(); i++) {
		Ткст ln = libs[i];
		Ткст ext = впроп(дайРасшф(ln));
		if(ext == ".a" || ext == ".so" || ext == ".dll")
			makefile.linkfileend << " \\\n\t\t\t" << GetPathQ(FindInDirs(libpath, ln));
		else
			makefile.linkfileend << " \\\n\t\t\t-l" << ln;
	}

	for(int i = 0; i < pkg.дайСчёт(); i++)
		if(!pkg[i].separator) {
			Ткст gop = Gather(pkg[i].option, config.дайКлючи());
			Ткст фн = SourcePath(package, pkg[i]);
			Ткст ext = впроп(дайРасшф(фн));
			bool isc = ext == ".c";
			bool isrc = (ext == ".rc" && естьФлаг("WIN32"));
			bool iscpp = (ext == ".cpp" || ext == ".cc" || ext == ".cxx");
			bool isicpp = (ext == ".icpp");
			if(ext == ".brc") {
				isc = true;
				фн << "c";
			}
			if(isc || isrc || iscpp || isicpp) {
				Ткст outfile;
				outfile << makefile.outdir << AdjustMakePath(дайТитулф(фн)) << (isrc ? "_rc" : "") << objext;
				Ткст srcfile = GetMakePath(MakeSourcePath(src, фн, false, exporting));
				makefile.rules << outfile << ": " << srcfile;
				Вектор<Ткст> dep = HdependGetDependencies(фн, false);
				сортируй(dep, GetLanguageInfo());
				for(int d = 0; d < dep.дайСчёт(); d++) {
					Ткст dfn = MakeSourcePath(src, dep[d], true, exporting);
					if(!пусто_ли(dfn))
						makefile.rules << " \\\n\t" << GetMakePath(dfn);
				}
				makefile.rules << "\n"
					"\t$(CXX) -c " << (isc ? "-x c $(CFLAGS)" : "-x c++ $(CXXFLAGS)") << " $(CINC) $(" << macros << ") "
						<< gop << " " << srcfile << " -o " << outfile << "\n\n";
				if(!libout || isicpp) {
					makefile.linkdep << " \\\n\t" << outfile;
					makefile.linkfiles << " \\\n\t\t" << outfile;
				}
				else {
					libdep << " \\\n\t" << outfile;
					libfiles << " \\\n\t\t" << outfile;
				}
			}
			else
			if(ext == ".o" || ext == ".obj" || ext == ".a" || ext == ".so" || ext == ".lib" || ext == ".dll") {
				makefile.linkdep << " \\\n\t" << фн;
				makefile.linkfiles << ' ' << фн;
			}
		}

	if(libout) {
		makefile.rules << libdep << "\n\t" << libfiles << "\n\n";
		makefile.linkdep << " \\\n\t" << makefile.output;
		makefile.linkfiles << " \\\n\t\t\t" << makefile.output;
	}
/*
	if(main) {
		if(!естьФлаг("SOLARIS")&&!естьФлаг("OSX11"))
			makefile.linkfiles << " \\\n\t\t-Wl,--start-группа ";
		DDUMPC(all_libraries);
		for(int i = 0; i < all_libraries.дайСчёт(); i++) {
			Ткст ln = all_libraries[i];
			Ткст ext = впроп(дайРасшф(ln));
			if(ext == ".a" || ext == ".so" || ext == ".dll")
				makefile.linkfileend << " \\\n\t\t\t" << GetHostPathQ(FindInDirs(libpath, ln));
			else
				makefile.linkfileend << " \\\n\t\t\t-l" << ln;
		}
		if(!естьФлаг("SOLARIS")&&!естьФлаг("OSX11"))
			makefile.linkfileend << " \\\n\t\t-Wl,--end-группа\n\n";
	}
*/
}

JsonArray& operator<<(JsonArray& array, const Вектор<Ткст>& v) {
	for (const Ткст& s: v)
		array << s;
	return array;
}

Вектор<Ткст>& operator<<(Вектор<Ткст>& array, const Вектор<Ткст>& v) {
	array.приставь(v);
	return array;
}

Точка ПостроительСиПП::ExtractVersion() const
{
	Точка v = Точка(Null, Null);
	try {
		СиПарсер p(version);
		while(!p.кф_ли()) {
			if(p.число_ли()) {
				v.x = p.читайЧисло();
				break;
			}
			p.дайСим();
			p.пробелы();
		}
		while(!p.кф_ли()) {
			if(p.число_ли()) {
				v.y = p.читайЧисло();
				break;
			}
			p.дайСим();
			p.пробелы();
		}
	}
	catch(СиПарсер::Ошибка) {}
	return v;
}

void ПостроительСиПП::ShowTime(int count, int start_time)
{
	if(count)
		вКонсоль(фмт("%d файла(ов)скомпилировано за %s %d мсек/файл",
			count, GetPrintTime(start_time), msecs(start_time) / count));
}

void MakeBuild::SaveMakeFile(const Ткст& фн, bool exporting)
{
	BeginBuilding(true);

	ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	Хост host;
	CreateHost(host, false, false);
	Один<Построитель> b = CreateBuilder(&host);

	if(!b)
		return;

	const TargetMode& tm = GetTargetMode();

	Ткст makefile;

	Вектор<Ткст> uppdirs = GetUppDirs();
	Ткст uppout = exporting ? GetVar("OUTPUT") : ".cache/upp.out";
	Ткст inclist;

	Индекс<Ткст> allconfig = PackageConfig(GetIdeWorkspace(), 0, bm, mainconfigparam, host, *b);
	bool win32 = allconfig.найди("WIN32") >= 0;

	РОбласть wspc;
	wspc.скан(GetMain(), allconfig.дайКлючи());

	Индекс<Ткст> pkg_config;
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		Индекс<Ткст> modconfig = PackageConfig(wspc, i, bm, mainconfigparam, host, *b);
		PkgConfig(wspc, modconfig, pkg_config);
		if(i)
			for(int a = allconfig.дайСчёт(); --a >= 0;)
				if(modconfig.найди(allconfig[a]) < 0)
					allconfig.удали(a);
	}

	if(!exporting)
		for(int i = 0; i < uppdirs.дайСчёт(); i++) {
			Ткст srcdir = GetMakePath(AdjustMakePath(приставьИмяф(uppdirs[i], "")), win32);
			makefile << "UPPDIR" << (i + 1) << " = " << srcdir << "\n";
			inclist << " -I$(UPPDIR" << (i + 1) << ")";
		}

	for(Ткст s : pkg_config)
		inclist << " `pkg-config --cflags " << s << "`";

	Вектор<Ткст> includes = SplitDirs(bm.дай("INCLUDE",""));
	for(int i = 0; i < includes.дайСчёт(); i++)
		inclist << " -I" << includes[i];

	inclist << " -I./";
	inclist << " -I$(UPPOUT)"; // build_info.h is created there

	makefile << "\n"
		"UPPOUT = " << (exporting ? "_out/" : GetMakePath(AdjustMakePath(приставьИмяф(uppout, "")), win32)) << "\n"
		"CINC   = " << inclist << "\n"
		"Macro  = ";

	for(int i = 0; i < allconfig.дайСчёт(); i++)
		makefile << " -Dflag" << allconfig[i];
	makefile << "\n";

	Ткст output, config, install, rules, linkdep, linkfiles, linkfileend;

	for(Ткст s : pkg_config)
		linkfileend << " \\\n\t\t\t`pkg-config --libs " << s << "`";

	for(int i = 0; i < wspc.дайСчёт(); i++) {
		b->config = PackageConfig(wspc, i, bm, mainconfigparam, host, *b);
		b->version = tm.version;
		b->method = method;
		MakeFile mf;
		b->AddMakeFile(mf, wspc[i], GetAllUses(wspc, i, bm, mainconfigparam, host, *b),
		               GetAllLibraries(wspc, i, bm, mainconfigparam, host, *b), allconfig,
		               exporting);
		if(i == 0) { // main package
			Ткст tdir = mf.outdir;
			Ткст trg;
			if(tm.target_override) {
				trg = GetMakePath(AdjustMakePath(tm.target), win32);
				if(!trg.пустой() && *trg.последний() == (win32 ? '\\' : '/'))
					trg << mf.outfile;
				else if(trg.найди(win32 ? '\\' : '/') < 0)
					trg.вставь(0, "$(OutDir)");
			}
			else
			if(exporting)
				trg = wspc[i] + ".out";
			else
				trg = "./" + wspc[0];
			output = Nvl(trg, mf.output);
			while(дирЕсть(output))
				output << ".out";
			ТкстПоток ss;
			Ткст svn_info;
			Ткст build_info = "\"$(UPPOUT)/build_info.h\"";
			if(makefile_svn_revision) {
				Вектор<Ткст> bi = RepoInfo(wspc[i]);
				for(int i = 0; i < bi.дайСчёт(); i++)
					svn_info << "	echo '" << bi[i] << "' >> " << build_info << "\n";
			}
			install << "\n"
				"OutDir = " << tdir << "\n"
				"OutFile = " << output << "\n"
				"\n"
				".PHONY: all\n"
				"all: prepare $(OutFile)\n"
				"\n"
				".PHONY: build_info\n"
				"build_info:\n"
				"	(date '+#define bmYEAR    %y%n"
				"#define бмМЕСЯЦ   %m%n"
				"#define бмДЕНЬ     %d%n"
				"#define бмЧАС    %H%n"
				"#define бмМИНУТА  %M%n"
				"#define бмСЕКУНДА  %S%n"
				"#define бмВРЕМЯ    Время( %y, %m, %d, %H, %M, %S )' | sed 's| 0\\([[:digit:]]\\)| \\1|g' && \\\n"
				"	echo '#define bmMACHINE \"'`hostname`'\"' && \\\n"
				"	echo '#define bmUSER    \"'`whoami`'\"') > " << build_info << "\n"
				<< svn_info <<
				"\n"
				".PHONY: prepare\n"
				"prepare:";
		}
		config << mf.config;
		install << mf.install;
		rules << mf.rules;
		linkdep << mf.linkdep;
		linkfiles << mf.linkfiles;
		linkfileend << mf.linkfileend;
	}

	makefile
		<< config
		<< install
		<< "\n\n"
		"$(OutFile): build_info " << linkdep << "\n\t" << linkfiles << linkfileend << " -Wl,--end-группа\n\n"
		<< rules
		<< ".PHONY: clean\n"
		<< "clean:\n"
		<< "\tif [ \"$(UPPOUT)\" != \"\" -a \"$(UPPOUT)\" != \"/\" -a -d \"$(UPPOUT)\" ] ; then rm -fr \"$(UPPOUT)\" ; fi\n"
	;

	bool sv = ::сохраниФайл(фн, makefile);
	if(!exporting) {
		if(sv)
			вКонсоль(фмт("%s(1): генерация makefile завершена", фн));
		else
			вКонсоль(фмт("%s: ошибки при записи makefile", фн));
	}
	EndBuilding(true);
}
