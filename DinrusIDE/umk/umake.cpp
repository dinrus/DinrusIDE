#include "umake.h"

bool SilentMode;

Ткст GetUmkFile(const char *фн)
{
	if(дирЕсть(фн) || *фн == '.')
		return Null;
	Ткст h = конфигФайл(фн);
	if(файлЕсть(h))
		return h;
	Ткст cfgdir = дайПапкуФайла(дайПапкуФайла(конфигФайл("x")));
	ONCELOCK
		PutVerbose("Конфиг directory: " << cfgdir);
	return дайФайлПоПути(фн,
	                     cfgdir + "/umk" + ';' +
	                     cfgdir + "/theide" + ';' +
	                     cfgdir + "/ide" + ';' +
	                     дайДомПапку() + ';' +
	                     дайПапкуФайла(дайФПутьИсп()));
}

Ткст GetBuildMethodPath(Ткст method)
{
	if(дайРасшф(method) != ".bm")
		method << ".bm";
	return GetUmkFile(method);
}

Ткст Иср::GetDefaultMethod()
{
	return "GCC";
}

ВекторМап<Ткст, Ткст> Иср::GetMethodVars(const Ткст& method)
{
	ВекторМап<Ткст, Ткст> map;
	LoadVarFile(GetMethodName(method), map);
	return map;
}

Ткст Иср::GetMethodName(const Ткст& method)
{
	return GetBuildMethodPath(method);
}

void поместиТ(const char *s)
{
	if(!SilentMode)
		Cout() << s;
}

Ткст GetAndroidSDKPath()
{
	return Ткст();
}

#ifdef flagMAIN

ГЛАВНАЯ_КОНСОЛЬН_ПРИЛ
{
#ifdef PLATFORM_POSIX
	setlinebuf(stdout);
	CreateBuildMethods();
#endif

	Иср ide;
	TheIde(&ide);
	ide.console.устСлоты(цпбЯдра());
	ide.console.console = true;

	ide.debug.def.blitz = ide.release.def.blitz = 0;
	ide.debug.def.debug = 2;
	ide.release.def.debug = 0;
	ide.debug.package.очисть();
	ide.release.package.очисть();
	ide.debug.linkmode = ide.release.linkmode = 0;
	ide.release.createmap = ide.debug.createmap = false;
	ide.targetmode = 0;
	ide.use_target = false;
	ide.makefile_svn_revision = false;
	bool clean = false;
	bool makefile = false;
	bool ccfile = false;
	bool deletedir = true;
	int  exporting = 0;
	bool run = false;
	Ткст mkf;

	Вектор<Ткст> param, runargs;

	const Вектор<Ткст>& args = комСтрока();
	for(int i = 0; i < args.дайСчёт(); i++) {
		Ткст a = args[i];
		if(*a == '-') {
			for(const char *s = ~a + 1; *s; s++)
				switch(*s) {
				case 'a': clean = true; break;
				case 'r': ide.targetmode = 1; break;
				case 'm': ide.release.createmap = ide.debug.createmap = true; break;
				case 'b': ide.release.def.blitz = ide.debug.def.blitz = 1; break;
				case 's': ide.debug.linkmode = ide.release.linkmode = 1; break;
				case 'd': ide.debug.def.debug = 0; break;
				case 'S': ide.debug.linkmode = ide.release.linkmode = 2; break;
				case 'v': ide.console.verbosebuild = true; break;
				case 'l': SilentMode = true; break;
				case 'x': exporting = 1; break;
				case 'X': exporting = 2; break;
				case 'k': deletedir = false; break;
				case 'u': ide.use_target = true; break;
				case 'j': ccfile = true; break;
				case 'M': {
					makefile = true;
					if(s[1] == '=') {
						mkf = нормализуйПуть(s + 2);
						PutVerbose("Generating Makefile: " + mkf);
						goto endopt;
					}
					else
						PutVerbose("Generating Makefile");
					break;
				}
				case 'H': {
					int n = 0;
					while(цифра_ли(s[1])) {
						n = 10 * n + s[1] - '0';
						s++;
					}
					n = minmax(n, 1, 32);
					PutVerbose("Hydra threads: " + какТкст(n));
					ide.console.устСлоты(n);
					break;
				}
				default:
					SilentMode = false;
					поместиТ("Invalid build option(s)");
					устКодВыхода(3);
					return;
				}
		endopt:;
		}
		else
		if(*a == '+')
			ide.mainconfigparam = фильтруй(~a + 1, [](int c) { return c == ',' ? ' ' : c; });
		else
		if(*a == '!') {
			run = true;
			for(int j = i + 1; j < args.дайСчёт(); j++)
				runargs.добавь(args[j]);
			if(runargs)
				PutVerbose("уст to execute the result with args: " << Join(runargs, " "));
			else
				PutVerbose("уст to execute the result");
			break;
		}
		else
			param.добавь(a);
	}

	if(param.дайСчёт() >= 2) {
		Ткст v = GetUmkFile(param[0] + ".var");
		if(пусто_ли(v)) {
		#ifdef PLATFORM_POSIX
			Вектор<Ткст> h = разбей(param[0], [](int c) { return c == ':' || c == ',' ? c : 0; });
		#else
			Вектор<Ткст> h = разбей(param[0], ',');
		#endif
			for(int i = 0; i < h.дайСчёт(); i++)
				h[i] = дайПолнПуть(обрежьОба(h[i]));
			Ткст x = Join(h, ";");
			SetVar("РНЦП", x, false);
			PutVerbose("Inline assembly: " + x);
			Ткст outdir = GetDefaultUppOut();
			реализуйДир(outdir);
			SetVar("OUTPUT", outdir, false);
		}
		else {
			if(!LoadVars(v)) {
				поместиТ("Invalid assembly\n");
				устКодВыхода(2);
				return;
			}
			PutVerbose("Assembly file: " + v);
			PutVerbose("Assembly: " + GetVar("РНЦП"));
		}
		PutVerbose("Output directory: " + GetVar("OUTPUT"));
		v = SourcePath(param[1], дайТитулф(param[1]) + ".upp");
		PutVerbose("Main package: " + v);
		if(!файлЕсть(v)) {
			поместиТ("Пакет does not exist\n");
			устКодВыхода(2);
			return;
		}
		ide.main = param[1];
		ide.wspc.скан(ide.main);
		const РОбласть& wspc = ide.роблИср();
		if(!wspc.дайСчёт()) {
			поместиТ("Empty assembly\n");
			устКодВыхода(4);
			return;
		}
		Индекс<Ткст> missing;
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			Ткст p = wspc[i];
			if(!файлЕсть(PackagePath(p)))
				missing.найдиДобавь(p);
		}
		if(missing.дайСчёт()) {
			поместиТ("Missing package(s): " << Join(missing.дайКлючи(), " ") << "\n");
			устКодВыхода(5);
			return;
		}
		if(пусто_ли(ide.mainconfigparam)) {
			const Массив<Пакет::Конфиг>& f = wspc.дайПакет(0).config;
			if(f.дайСчёт())
				ide.mainconfigparam = f[0].param;
		}
		PutVerbose("Build flags: " << ide.mainconfigparam);
		Ткст m = 2 < param.дайСчёт() ? param[2] : "CLANG";
		Ткст bp = GetBuildMethodPath(m);
		PutVerbose("Build method: " + bp);
		if(bp.дайСчёт() == 0) {
			SilentMode = false;
			поместиТ("Invalid build method\n");
			устКодВыхода(3);
			return;
		}

		if(3 < param.дайСчёт()) {
			ide.debug.target_override = ide.release.target_override = true;
			ide.debug.target = ide.release.target = нормализуйПуть(param[3]);
			PutVerbose("Target override: " << ide.debug.target);
		}

		ide.method = m;

		if(ccfile) {
			ide.SaveCCJ(дайДиректориюФайла(PackagePath(ide.main)) + "compile_commands.json", false);
			устКодВыхода(0);
			return;
		}

		if(clean)
			ide.Clean();
		if(exporting) {
			mkf = дайПолнПуть(mkf);
			Cout() << mkf << '\n';
			реализуйДир(mkf);
			if(makefile)
				ide.ExportMakefile(mkf);
			else
				ide.ExportProject(mkf, exporting == 2, deletedir);
		}
		else
		if(makefile) {
			ide.SaveMakeFile(пусто_ли(mkf) ? "Makefile" : mkf, false);
			устКодВыхода(0);
		}
		else
		if(ide.Build()) {
			устКодВыхода(0);
			if(run) {
				Вектор<char *> args;
				Вектор<Буфер<char>> буфер;
				auto добавь = [&](const Ткст& s) {
					auto& b = буфер.добавь();
					b.размести(s.дайСчёт() + 1);
					memcpy(b, s, s.дайСчёт() + 1);
					args.добавь(b);
				};
				добавь(ide.target);
				for(const Ткст& s : runargs)
					добавь(s);
				args.добавь(NULL);
				устКодВыхода(execv(ide.target, args.begin()));
			}
		}
		else
			устКодВыхода(1);
	}
	else
		поместиТ("Usage: [-options] umk assembly main_package [build_method] [+flags] [output]\n\n"
		     "Examples: umk examples Bombs GCC -ab +GUI,SHARED ~/bombs\n"
		     "          umk examples,uppsrc Bombs ~/GCC.bm -rv +GUI,SHARED ~/bin\n\n"
		     "See https://www.ultimatepp.org/app$ide$umk$en-us.html for details\n");
}

#endif