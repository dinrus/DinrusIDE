#include "Builders.h"

inline
Ткст SString(const Ткст& s) {
	if (s.найди(' ') >= 0)
		return "\"" << s << "\"";
	return s;
}

inline
Ткст SAppend(Ткст l, const Ткст& r) {
	return l << SString(r);
}

void ПостроительСиПП::AddCCJ(
	MakeFile& makefile,
	Ткст package,
	const Индекс<Ткст>& common_config,
	bool exporting,
	bool last_ws)
{
	const char* eol = "\n";
	const char* tab = "\t";

	Ткст packagepath = PackagePath(package);
	Пакет pkg;
	pkg.грузи(packagepath);
	Ткст packagedir = дайПапкуФайла(packagepath);
	Вектор<Ткст> src = GetUppDirs();
	for(int i = 0; i < src.дайСчёт(); i++)
		src[i] = UnixPath(src[i]);

	bool main = естьФлаг("MAIN");
	bool is_shared = естьФлаг("SO");
	bool win32 = естьФлаг("WIN32");

	Ткст pack_ident = MakeIdent(package);
	Вектор<Ткст> macdef;
	Ткст objext = (естьФлаг("MSC") || естьФлаг("EVC") ? ".obj" : ".o");

	macdef.приставь(Macro);

	Вектор<Ткст> x(config.дайКлючи(), 1);
	сортируй(x);
	for(int i = 0; i < x.дайСчёт(); i++) {
		if(common_config.найди(x[i]) < 0) {
			Ткст str;
			str << "-Dflag" << x[i];
			macdef << str;
		}
		x[i] = иницШапки(x[i]);
	}

	makefile.outdir << outdir << "/";

	if(main) {
		Вектор<Ткст> flags;
		if(естьФлаг("DEBUG"))
			flags << "-D_DEBUG" << debug_options;
		else
			flags << release_options;
		if(естьФлаг("DEBUG_MINIMAL"))
			flags << "-ggdb" << "-g1";
		if(естьФлаг("DEBUG_FULL"))
			flags << "-ggdb" << "-g2";
		if(is_shared && !win32)
			flags << "-fPIC";
		flags.приставь(GatherV(pkg.option, config.дайКлючи()));

		CFLAGS = clone(flags);
		CXXFLAGS = pick(flags);

		CFLAGS << c_options;
		CXXFLAGS << cpp_options;
	}

//	Ткст cmplr = Sys("which " + compiler);
//	cmplr.обрежьКонец("\n");
	Ткст cmplr = compiler;

	const Ткст inc_outdir = SAppend("-I", makefile.outdir);
	for(int count = pkg.дайСчёт(), last = count - 1, i = 0; i < count; i++)
		if(!pkg[i].separator) {
			Ткст gop = Gather(pkg[i].option, config.дайКлючи());
			Ткст фн = SourcePath(package, pkg[i]);
			Ткст ext = впроп(дайРасшф(фн));
			bool isc = ext == ".c";
			const bool ish = ext == ".h";
			const bool isrc = (ext == ".rc" && естьФлаг("WIN32"));
			const bool iscpp = (ext == ".cpp" || ext == ".cc" || ext == ".cxx");
			const bool ishpp = (ext == ".hpp" || ext == ".hh" || ext == ".hxx");
			const bool isicpp = (ext == ".icpp");
			if(ext == ".brc") {
				isc = true;
				фн << "c";
			}
			if(isc || ish || isrc || iscpp || ishpp || isicpp) {
				Ткст outfile;
				const Ткст srcfile = SString(фн);
				outfile << makefile.outdir << AdjustMakePath(дайТитулф(фн));
				if (isrc)
					outfile << "_rc";
				else if (ish || ishpp)
					outfile << "_hpp";
				else
					outfile << "_cpp";
				outfile << objext;
				outfile = SString(outfile);

#if 0
				Json j;
				JsonArray ja;
				ja << cmplr;
				ja << "-c";
				ja << "-x";
				if (isc)
					ja << "c" << CFLAGS;
				else
					ja << "c++" << CXXFLAGS;
				ja << CINC ;
				ja << inc_outdir;
				ja << macdef;
				// ja << gop; // ???
				ja << srcfile << "-o" << outfile;
				// j("arguments", ja)("directory", дайПапкуФайла(фн))("file", дайИмяф(фн));
				// j("arguments", ja)("directory", дайПапкуФайла(фн))("file", фн);
				j("arguments", ja)("directory", SString(дайПапкуФайла(фн)))("file", srcfile);
				makefile.rules << tab << j;
				if (i == last && last_ws)
					makefile.rules << eol;
				else
					makefile.rules << "," << eol;
#else
				Json j;
				Ткст ja;
				ja << cmplr;
				ja << " -c";
				ja << " -x";
				if (isc)
					ja << " c " << Join(CFLAGS, " ");
				else
					ja << " c++ " << Join(CXXFLAGS, " ");
				MergeWith(ja, " ", Join(CINC, " "));
				MergeWith(ja, " ", inc_outdir);
				MergeWith(ja, " ", Join(macdef, " "));
				ja << ' ' << srcfile << " -o" << outfile;
				j("command", ja)("directory", SString(дайПапкуФайла(фн)))("file", srcfile);
				makefile.rules << tab << j;
				if (i == last && last_ws)
					makefile.rules << eol;
				else
					makefile.rules << "," << eol;
#endif
			}
		}
}

void MakeBuild::SaveCCJ(const Ткст& фн, bool exporting)
{
	const char* eol = "\n";

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

	makefile << "[" << eol;

	Индекс<Ткст> oi;
	if(!exporting)
		for(int i = 0; i < uppdirs.дайСчёт(); i++) {
			Ткст s;
			s << "-I" << uppdirs[i];
			oi.найдиДобавь(s);
		}

	for(const Ткст& s: pkg_config) {
		Ткст str;
		Ткст out;
		str << "pkg-config --cflags " << s;
		if (Sys(str, out) < 0)
			continue;
		out.обрежьКонец("\n");
		if (out.пустой())
			continue;
		Вектор<Ткст> l = разбей(out, ' ');
		for(const Ткст& s1: l)
			oi.найдиДобавь(s1);
	}

	Вектор<Ткст> includes = SplitDirs(bm.дай("INCLUDE",""));
	for(int i = 0; i < includes.дайСчёт(); i++) {
		Ткст str;
		str << SAppend("-I", includes[i]);
		oi.найдиДобавь(str);
	}

	for (const Ткст& s: allconfig) {
		Ткст str;
		str << "-Dflag" << s;
		b->Macro << str;
	}

	Ткст UPPOUT("-I");
	if (exporting)
		UPPOUT << "_out/";
	else
		UPPOUT << GetMakePath(AdjustMakePath(приставьИмяф(uppout, "")), win32);
	oi.найдиДобавь(pick(UPPOUT));

	b->CINC.приставь(oi.подбериКлючи());

	Ткст config, rules;

	for(int count = wspc.дайСчёт(), last = count - 1, i = 0; i < count; i++) {
		const Ткст package = wspc[i];
		b->config = PackageConfig(wspc, i, bm, mainconfigparam, host, *b);
		b->version = tm.version;
		b->method = method;
		b->outdir = OutDir(b->config, package, bm);
		MakeFile mf;
		b->AddCCJ(mf, package, allconfig, exporting, i == last);
		config << mf.config;
		rules << mf.rules;
		if(i == 0) // main package
			b->SaveBuildInfo(package);
	}

	makefile
		<< config
		<< rules
		<< "]"
	;

	bool sv = ::сохраниФайл(фн, makefile);
	if(!exporting) {
		if(sv)
			вКонсоль(фмтЧ("%s(1): генерация compile_commands.json завершена", фн));
		else
			вКонсоль(фмтЧ("%s: ошибка при записи compile_commands.json", фн));
	}
}
