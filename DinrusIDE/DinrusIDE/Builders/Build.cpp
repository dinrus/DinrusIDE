#include "Builders.h"
#include "AndroidBuilder.h"

#include <plugin/bz2/bz2.h>

#define LDUMP(x) // DUMP(x)

MakeBuild::MakeBuild()
{
	targetmode = 0;
	stoponerrors = true;
	use_target = false;
}

const TargetMode& MakeBuild::GetTargetMode()
{
	return (targetmode == 0 ? debug : release);
}

Индекс<Ткст> MakeBuild::PackageConfig(const РОбласть& wspc, int package,
                                 const ВекторМап<Ткст, Ткст>& bm, Ткст mainparam,
                                 Хост& host, Построитель& b, Ткст *target)
{
	Ткст packagepath = PackagePath(wspc[package]);
	const Пакет& pkg = wspc.package[package];
	cfg.очисть();
	MergeWith(mainparam, " ", bm.дай(targetmode ? "RELEASE_FLAGS" : "DEBUG_FLAGS", Ткст()),
	                          bm.дай("COMMON_FLAGS", Ткст()));
	cfg = SplitFlags(mainparam, package == 0, wspc.GetAllAccepts(package));
	cfg.найдиДобавь(bm.дай("BUILDER", "GCC"));
	const TargetMode& m = GetTargetMode();
	if(targetmode == 0)
		cfg.найдиДобавь("DEBUG");
	switch(m.linkmode) {
	case 2:
		cfg.найдиДобавь("SO");
	case 1:
		cfg.найдиДобавь("SHARED");
	}
	int q = m.package.найди(wspc[package]);
	if(q >= 0) {
		const PackageMode& p = m.package[q];
		switch(p.debug >= 0 ? p.debug : m.def.debug) {
		case 1:  cfg.найдиДобавь("DEBUG_MINIMAL"); break;
		case 2:  cfg.найдиДобавь("DEBUG_FULL"); break;
		}
		if(!pkg.noblitz && (p.blitz >= 0 ? p.blitz : m.def.blitz) && bm.дай("DISABLE_BLITZ", "") != "1")
			cfg.найдиДобавь("BLITZ");
	}
	else {
		switch(m.def.debug) {
		case 1:  cfg.найдиДобавь("DEBUG_MINIMAL"); break;
		case 2:  cfg.найдиДобавь("DEBUG_FULL"); break;
		}
		if(!pkg.noblitz && m.def.blitz && bm.дай("DISABLE_BLITZ", "") != "1")
			cfg.найдиДобавь("BLITZ");
	}
	b.добавьФлаги(cfg);
	host.добавьФлаги(cfg);
	for(int i = 0; i < pkg.flag.дайСчёт(); i++) {
		if(MatchWhen(pkg.flag[i].when, cfg.дайКлючи())) {
			Вектор<Ткст> h = разбей(pkg.flag[i].text, ' ');
			for(int i = 0; i < h.дайСчёт(); i++)
				if(*h[i] == '-')
					cfg.удалиКлюч(h[i].середина(1));
				else
					cfg.найдиДобавь(h[i]);
		}
	}
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.package[i];
		for(int j = 0; j < pk.дайСчёт(); j++)
			if(pk[j] == "main.conf")
				cfg.найдиДобавь(фильтруй(wspc.package.дайКлюч(i), [](int c) { return iscid(c) ? c : '_'; }) + "_conf");
	}
	Индекс<Ткст> h;
	h = clone(cfg); // Retain deep copy (h will be picked)
	return h;
}

Ткст NoCr(const char *s)
{
	Ткст out;
	while(*s)
	{
		const char *b = s;
		while(*s && *s != '\r')
			s++;
		out.конкат(b, int(s - b));
		if(*s == '\r')
			s++;
	}
	return out;
}

void MakeBuild::CreateHost(Хост& host, bool darkmode, bool disable_uhd)
{
	SetupDefaultMethod();
	ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	{
		ВекторМап<Ткст, Ткст> env = clone(систСреда());
		host.exedirs = SplitDirs(bm.дай("PATH", "") + ';' + env.дай("PATH", ""));
#ifdef PLATFORM_WIN32
		Ткст p = дайФайлИзПапкиИсп("bin/mingit/cmd");
		if(файлЕсть(p + "/git.exe"))
			host.exedirs.добавь(p);
		env.дайДобавь("PATH") = Join(host.exedirs, ";");
#else
		env.дайДобавь("PATH") = Join(host.exedirs, ":");
#endif
		env.дайДобавь("UPP_MAIN__") = дайДиректориюФайла(PackagePath(GetMain()));
		env.дайДобавь("UPP_ASSEMBLY__") = GetVar("РНЦП");
		if(disable_uhd)
			env.дайДобавь("UPP_DISABLE_UHD__") = "1";
		if(darkmode)
			env.дайДобавь("UPP_DARKMODE__") = "1";
		// setup LD_LIBRARY_PATH on target dir, needed for all shared builds on posix
#ifdef PLATFORM_POSIX
		if(target != "")
		{
			Ткст ldPath = дайПапкуФайла(target) + ";" + env.дай("LD_LIBRARY_PATH", "");
			env.дайДобавь("LD_LIBRARY_PATH") = ldPath;
		}
#endif
#ifdef PLATFORM_COCOA
		host.exedirs.приставь(SplitDirs("/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin")); // sometimes some of these are missing..
#endif
		for(int i = 0; i < env.дайСчёт(); i++) {
			LDUMP(env.дайКлюч(i));
			LDUMP(env[i]);
			host.environment << env.дайКлюч(i) << '=' << env[i] << '\0';
		}
		host.environment.конкат(0);
		host.cmdout = &cmdout;
	}
}

Один<Построитель> MakeBuild::CreateBuilder(Хост *host)
{
	SetupDefaultMethod();
	ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	Ткст builder = bm.дай("BUILDER", "GCC");
	int q = BuilderMap().найди(builder);
	if(q < 0) {
		вКонсоль("Неверный построитель " + builder);
		ConsoleShow();
		return NULL;
	}
	Построитель* b = (*BuilderMap().дай(builder))();
	b->host = host;
	b->script = bm.дай("SCRIPT", "");
	if(AndroidBuilder::GetBuildersNames().найди(builder) > -1) {
		AndroidBuilder* ab = dynamic_cast<AndroidBuilder*>(b);
		ab->sdk.SetPath((bm.дай("SDK_PATH", "")));
		ab->ndk.SetPath((bm.дай("NDK_PATH", "")));
		ab->SetJdk(Один<Jdk>(new Jdk(bm.дай("JDK_PATH", ""), host)));
		
		Ткст platformVersion = bm.дай("SDK_PLATFORM_VERSION", "");
		if(!platformVersion.пустой())
			ab->sdk.SetPlatform(platformVersion);
		else
			ab->sdk.DeducePlatform();
		Ткст buildToolsRelease = bm.дай("SDK_BUILD_TOOLS_RELEASE", "");
		if(!buildToolsRelease.пустой())
			ab->sdk.SetBuildToolsRelease(buildToolsRelease);
		else
			ab->sdk.DeduceBuildToolsRelease();
		
		ab->ndk_blitz = bm.дай("NDK_BLITZ", "") == "1";
		if(bm.дай("NDK_ARCH_ARMEABI_V7A", "") == "1")
			ab->ndkArchitectures.добавь("armeabi-v7a");
		if(bm.дай("NDK_ARCH_ARM64_V8A", "") == "1")
			ab->ndkArchitectures.добавь("arm64-v8a");
		if(bm.дай("NDK_ARCH_X86", "") == "1")
			ab->ndkArchitectures.добавь("x86");
		if(bm.дай("NDK_ARCH_X86_64", "") == "1")
			ab->ndkArchitectures.добавь("x86_64");
		ab->ndkToolchain = bm.дай("NDK_TOOLCHAIN", "");
		ab->ndkCppRuntime = bm.дай("NDK_CPP_RUNTIME", "");
		ab->ndkCppFlags = bm.дай("NDK_COMMON_CPP_OPTIONS", "");
		ab->ndkCFlags = bm.дай("NDK_COMMON_C_OPTIONS", "");
		
		b = ab;
	}
	else {
		// TODO: cpp builder variables only!!!
		b->compiler = bm.дай("COMPILER", "");
		b->include = SplitDirs(Join(GetUppDirs(), ";") + ';' + bm.дай("INCLUDE", "") + ';' + add_includes);
		const РОбласть& wspc = GetIdeWorkspace();
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			const Пакет& pkg = wspc.дайПакет(i);
			for(int j = 0; j < pkg.include.дайСчёт(); j++)
				b->include.добавь(SourcePath(wspc[i], pkg.include[j].text));
		}
		b->libpath = SplitDirs(bm.дай("LIB", ""));
		b->cpp_options = bm.дай("COMMON_CPP_OPTIONS", "");
		b->c_options = bm.дай("COMMON_C_OPTIONS", "");
		b->debug_options = Join(bm.дай("COMMON_OPTIONS", ""), bm.дай("DEBUG_OPTIONS", ""));
		b->release_options = Join(bm.дай("COMMON_OPTIONS", ""), bm.дай("RELEASE_OPTIONS", ""));
		b->common_link = bm.дай("COMMON_LINK", "");
		b->debug_link = bm.дай("DEBUG_LINK", "");
		b->release_link = bm.дай("RELEASE_LINK", "");
		
		b->main_conf = !!main_conf.дайСчёт();
		b->allow_pch = bm.дай("ALLOW_PRECOMPILED_HEADERS", "") == "1";
		b->start_time = start_time;
	}
	return b;
}

int CharFilterSlash(int c)
{
	return c == '\\' ? '/' : c;
}

bool output_per_assembly;

Ткст MakeBuild::OutDir(const Индекс<Ткст>& cfg, const Ткст& package, const ВекторМап<Ткст, Ткст>& bm,
                   bool use_target)
{
	Индекс<Ткст> excl;
	excl.добавь(bm.дай("BUILDER", "GCC"));
	excl.добавь("MSC");
	Хост().добавьФлаги(excl);
	Вектор<Ткст> x;
	bool dbg = cfg.найди("DEBUG_FULL") >= 0 || cfg.найди("DEBUG_MINIMAL") >= 0;
	if(cfg.найди("DEBUG") >= 0) {
		excl.добавь("BLITZ");
		if(cfg.найди("BLITZ") < 0)
			x.добавь("NOBLITZ");
	}
	else
		if(dbg)
			x.добавь("RELEASE");
	if(use_target)
		excl.добавь("MAIN");
	for(int i = 0; i < cfg.дайСчёт(); i++)
		if(excl.найди(cfg[i]) < 0)
			x.добавь(cfg[i]);
	сортируй(x);
	for(int i = 0; i < x.дайСчёт(); i++)
		x[i] = иницШапки(x[i]);
	Ткст outdir = GetVar("OUTPUT");
	if(output_per_assembly)
		outdir = приставьИмяф(outdir, GetAssemblyId());
	if(!use_target)
		outdir = приставьИмяф(outdir, package);
	outdir = приставьИмяф(outdir, дайТитулф(method) + "." + Join(x, "."));
	outdir = фильтруй(outdir, CharFilterSlash);
	return outdir;
}

void MakeBuild::PkgConfig(const РОбласть& wspc, const Индекс<Ткст>& config, Индекс<Ткст>& pkg_config)
{
	for(int i = 0; i < wspc.дайСчёт(); i++)
		for(Ткст h : разбей(Gather(wspc.дайПакет(i).pkg_config, config.дайКлючи()), ' '))
			pkg_config.найдиДобавь(h);
}

bool MakeBuild::постройПакет(const РОбласть& wspc, int pkindex, int pknumber, int pkcount,
	Ткст mainparam, Ткст outfile, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
	Ткст& linkopt, bool link)
{
	Ткст package = wspc[pkindex];
	Ткст mainpackage = wspc[0];
	const Пакет& pkg = wspc.package[pkindex];
	ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	if(bm.дайСчёт() == 0) {
		вКонсоль(GetInvalidBuildMethodError(method));
		ConsoleShow();
		return false;
	}
	Хост host;
	CreateHost(host, false, false);
	host.onefile = onefile;
	Один<Построитель> b = CreateBuilder(&host);
	if(!b)
		return false;
	b->config = PackageConfig(wspc, pkindex, bm, mainparam, host, *b);
	PkgConfig(wspc, b->config, b->pkg_config);
	const TargetMode& m = targetmode == 0 ? debug : release;
	b->version = m.version;
	b->method = method;
	b->outdir = OutDir(b->config, package, bm);
	host.RealizeDir(b->outdir);
	Ткст mainfn = Null;
	Индекс<Ткст> mcfg = PackageConfig(wspc, 0, bm, mainparam, host, *b, &mainfn);
	HdependClearDependencies();
	for(int i = 0; i < pkg.дайСчёт(); i++) {
		const Массив<OptItem>& f = pkg[i].depends;
		for(int j = 0; j < f.дайСчёт(); j++)
			if(MatchWhen(f[j].when, mcfg.дайКлючи()))
				HdependAddDependency(SourcePath(package, pkg[i]), SourcePath(package, f[j].text));
	}
	Ткст tout = OutDir(mcfg, mainpackage, bm, use_target);
	host.RealizeDir(tout);
	if(пусто_ли(mainfn))
		mainfn = дайТитулф(mainpackage) + b->GetTargetExt();
	if(!пусто_ли(outfile))
		target = нормализуйПуть(outfile, tout);
	else {
	#ifdef PLATFORM_COCOA
		if(m.target_override && !пусто_ли(m.target)
		   && папка_ли(m.target) && дайРасшф(m.target) == ".app")
			target = нормализуйПуть(m.target);
		else
	#endif
		if(m.target_override && !пусто_ли(m.target) && папка_ли(m.target))
			target = нормализуйПуть(приставьИмяф(m.target, mainfn));
		else
		if(m.target_override && (полнпуть_ли(m.target) || *m.target == '/' || *m.target == '\\'))
			target = m.target;
		else
		if(m.target_override && !пусто_ли(m.target))
			target = нормализуйПуть(приставьИмяф(tout, m.target));
		else
		if(полнпуть_ли(mainfn))
			target = mainfn;
		else
			target = нормализуйПуть(приставьИмяф(tout, mainfn));
	}
	b->target = target;
	b->mainpackage = mainpackage;
	if(пусто_ли(onefile)) {
		Ткст out;
		out << "----- " << package << " ( " << Join(b->config.дайКлючи(), " ") << " )";
		if(pkcount > 1)
			out << " (" << (pknumber + 1) << " / " << pkcount << ')';
		вКонсоль(out);
	}
	else
		b->config.найдиДобавь("NOLIB");
	bool ok = b->постройПакет(package, linkfile, immfile, linkopt,
		                      GetAllUses(wspc, pkindex, bm, mainparam, host, *b),
		                      GetAllLibraries(wspc, pkindex, bm, mainparam, host, *b),
		                      targetmode - 1);
	target = b->target; // apple app bundle can change target
	Вектор<Ткст> errors = PickErrors();
	for(Ткст p : errors)
		DeleteFile(p);
	if(!ok || !errors.пустой())
		return false;
	if(link) {
		ok = b->Link(linkfile, linkopt, GetTargetMode().createmap);
		PutLinkingEnd(ok);
		errors = PickErrors();
		for(Ткст p : errors)
			DeleteFile(p);
		if(!ok || !errors.пустой())
			return false;
	}
	return true;
}

void MakeBuild::SetHdependDirs()
{
	Вектор<Ткст> include = SplitDirs(GetVar("РНЦП") + ';'
		+ GetMethodVars(method).дай("INCLUDE", "") + ';'
		+ систСреда().дай("INCLUDE", "")
#ifdef PLATFORM_POSIX
		+ ";/usr/include;/usr/local/include;"
#endif
		+ add_includes
		);
	// Also adding internal includes
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pkg = wspc.дайПакет(i);
		for(int j = 0; j < pkg.include.дайСчёт(); j++)
			include.добавь(SourcePath(wspc[i], pkg.include[j].text));
	}

	HdependSetDirs(pick(include));
}

Вектор<Ткст> MakeBuild::GetAllUses(const РОбласть& wspc, int f,
	const ВекторМап<Ткст, Ткст>& bm, Ткст mainparam, Хост& host, Построитель& builder)
{ // Gathers all uses, including subpackages, to support SO builds
	Ткст package = wspc[f];
	Индекс<Ткст> all_uses;
	bool warn = true;
	int n = 0;
	while(f >= 0) {
		const Пакет& p = wspc.package[f];
		Индекс<Ткст> config = PackageConfig(wspc, f, bm, mainparam, host, builder);
		for(int fu = 0; fu < p.uses.дайСчёт(); fu++) {
			if(MatchWhen(p.uses[fu].when, config.дайКлючи())) {
				if(p.uses[fu].text != package)
					all_uses.найдиДобавь(p.uses[fu].text);
				else if(warn) {
					вКонсоль(фмт("%s: циркулярная цепечка 'использований'", package));
					warn = false;
				}
			}
		}
		f = -1;
		while(n < all_uses.дайСчёт() && (f = wspc.package.найди(all_uses[n++])) < 0)
			;
	}
	return all_uses.подбериКлючи();
}

Вектор<Ткст> MakeBuild::GetAllLibraries(const РОбласть& wspc, int Индекс,
	const ВекторМап<Ткст, Ткст>& bm, Ткст mainparam,
	Хост& host, Построитель& builder)
{
	Вектор<Ткст> uses = GetAllUses(wspc, Индекс, bm, mainparam, host, builder);
	uses.добавь(wspc[Индекс]);
	::Индекс<Ткст> libraries;
	
	for(int i = 0; i < uses.дайСчёт(); i++) {
		int f = wspc.package.найди(UnixPath(uses[i]));
		if(f >= 0) {
			const Пакет& pk = wspc.package[f];
			::Индекс<Ткст> config = PackageConfig(wspc, f, bm, mainparam, host, builder);
			Вектор<Ткст> pklibs = разбей(Gather(pk.library, config.дайКлючи()), ' ');
			FindAppend(libraries, pklibs);
		}
	}
	return libraries.подбериКлючи();
}

bool MakeBuild::Build(const РОбласть& wspc, Ткст mainparam, Ткст outfile, bool clear_console)
{
	InitBlitz();

	Ткст hfile = outfile + ".xxx";
	сохраниФайл(hfile, "");
	start_time = дайФВремя(hfile); // Defensive way to get correct filetime of start
	DeleteFile(hfile);
	
	ClearErrorEditor();
	BeginBuilding(clear_console);
	bool ok = true;
	main_conf.очисть();
	add_includes.очисть();
	if(wspc.дайСчёт()) {
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			const Пакет& pk = wspc.package[i];
			for(int j = 0; j < pk.дайСчёт(); j++)
				if(pk[j] == "main.conf") {
					Ткст pn = wspc[i];
					Ткст p = SourcePath(pn, "main.conf");
					main_conf << "// " << pn << "\r\n" << загрузиФайл(p) << "\r\n";
					вКонсоль("Found " + p);
				}
		}

		if(main_conf.дайСчёт()) {
			ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
			Хост host;
			CreateHost(host, false, false);
			Один<Построитель> b = CreateBuilder(&host);
			if(b) {
				Индекс<Ткст> mcfg = PackageConfig(wspc, 0, bm, mainparam, host, *b, NULL);
				Ткст outdir = OutDir(mcfg, wspc[0], bm, false);
				Ткст path = приставьИмяф(outdir, "main.conf.h");
				RealizePath(path);
				сохраниИзменёнФайл(path, main_conf);
				вКонсоль("Сохраняется " + path);
				PutVerbose(main_conf);
				add_includes << outdir << ';';
			}
		}

		Вектор<int> build_order;
		if(cfg.найди("SO") < 0) {
			for(int i = 1; i < wspc.дайСчёт(); i++)
				build_order.добавь(i);
		}
		else {
			Индекс<int> remaining;
			for(int i = 1; i < wspc.дайСчёт(); i++)
				remaining.добавь(i);
			while(!remaining.пустой()) {
				int t;
				for(t = 0; t < remaining.дайСчёт(); t++) {
					const Пакет& pk = wspc.package[remaining[t]];
					bool delay = false;
					for(int u = 0; u < pk.uses.дайСчёт(); u++) {
						if(remaining.найди(wspc.package.найди(UnixPath(pk.uses[u].text))) >= 0) {
							delay = true;
							break;
						}
					}
					if(!delay)
						break;
				}
				if(t >= remaining.дайСчёт()) // Progress even if circular references present
					t = 0;
				build_order.добавь(remaining[t]);
				remaining.удали(t);
			}
		}

		Ткст mainpackage = wspc[0];
		Вектор<Ткст> linkfile, immfile;
		Ткст linkopt = Merge(" ", GetMethodVars(method).дай("COMMON_LINK", Null),
		                       GetMethodVars(method).дай(targetmode ? "RELEASE_LINK" : "DEBUG_LINK", Null));
		if(linkopt.дайСчёт())
			linkopt << ' ';
		ok = true;
		int ms = msecs();
		for(int i = 0; i < build_order.дайСчёт() && (ok || !stoponerrors); i++) {
			int px = build_order[i];
			ok = постройПакет(wspc, px, i, build_order.дайСчёт() + 1,
				              mainparam, Null, linkfile, immfile, linkopt) && ok;
			if(msecs() - ms >= 200) {
				DoProcessСобытиеs();
				ms = msecs();
			}
		}
		if(ok || !stoponerrors) {
			ok = постройПакет(wspc, 0, build_order.дайСчёт(), build_order.дайСчёт() + 1,
			                  mainparam, outfile, linkfile, immfile, linkopt, ok) && ok;
		}
	}
	EndBuilding(ok);
	ReQualifyCodeBase();
	SetErrorEditor();
	return ok;
}

void MakeBuild::BuildWorkspace(РОбласть& wspc, Хост& host, Построитель& builder)
{
	Индекс<Ткст> p = PackageConfig(GetIdeWorkspace(), 0, GetMethodVars(method), mainconfigparam,
	                                host, builder);
	wspc.скан(GetMain(), p.дайКлючи());
}

bool MakeBuild::Build()
{
	if(GetMethodVars(method).дайСчёт() == 0) {
		вКонсоль(GetInvalidBuildMethodError(method));
		ConsoleShow();
		return false;
	}
	Хост host;
	CreateHost(host, false, false);
	Один<Построитель> builder = CreateBuilder(&host);
	if(!builder)
		return false;
	РОбласть wspc;
	BuildWorkspace(wspc, host, *builder);
	return Build(wspc, mainconfigparam, Null);
}

void MakeBuild::очистьПакет(const РОбласть& wspc, int package)
{
	вКонсоль(фмт("Очищается %s", wspc[package]));
	Хост host;
	CreateHost(host, false, false);
	Один<Построитель> builder = CreateBuilder(&host);
	if(!builder)
		return;
	Ткст outdir = OutDir(PackageConfig(wspc, package, GetMethodVars(method), mainconfigparam,
	                       host, *builder), wspc[package], GetMethodVars(method));
	// TODO: almost perfect, but target will be detected after build. if build does not occur the target is empty :(
	// How to make sure we know target? Target directory is where android project sandbox is.
	builder->target = target;
	builder->очистьПакет(wspc[package], outdir);
}

void MakeBuild::Clean()
{
	ConsoleClear();

	Хост host;
	CreateHost(host, false, false);
	Один<Построитель> builder = CreateBuilder(&host);
	if(!builder)
		return;
	builder->target = target;
	
	РОбласть wspc;
	BuildWorkspace(wspc, host, *builder);
	for(int i = 0; i < wspc.дайСчёт(); i++)
		очистьПакет(wspc, i);
	
	builder->AfterClean();
	
	вКонсоль("...готово");
}

void MakeBuild::RebuildAll()
{
	Clean();
	Build();
}

Ткст MakeBuild::GetInvalidBuildMethodError(const Ткст& method)
{
	return "Неверный метод построения " + method + " (" + GetMethodPath(method) + ").";
}
