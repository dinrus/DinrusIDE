#include "Builders.h"

void поместиВремяКомпиляции(int time, int count)
{
	вКонсоль(Ткст().конкат() << count << " файлов(а) скомпилировано за " << GetPrintTime(time)
	           << " " << int(msecs() - time) / count << " мсек/файл");
}

Ткст ПостроительСиПП::GetTargetExt() const
{
	if(!естьФлаг("WIN32") && !естьФлаг("WINCE"))
		return естьФлаг("DLL") ? ".so" : "";
	else
		return естьФлаг("DLL") ? ".dll" : ".exe";
}

void ПостроительСиПП::очистьПакет(const Ткст& package, const Ткст& outdir)
{
	DeleteFolderDeep(outdir);
}

// POSIX lib files have names in form of libXXXXXX.so.ver.minver(.rel)
// so we can't simply get file extension
Ткст ПостроительСиПП::GetSrcType(Ткст фн) const
{
	фн = впроп(фн);
	Ткст ext = дайРасшф(фн);
	if(!естьФлаг("POSIX") || ext == ".so")
		return ext;
	int soPos = фн.найдирек(".so");
	if(soPos < 0)
		return ext;
	фн = фн.середина(soPos + 3);
	const char *c = ~фн;
	while(*c)
	{
		if(*c != '.' && !цифра_ли(*c))
			return ext;
		c++;
	}
	return ".so";
}

// from complete lib имя/path (libXXX.so.ver.minVer) gets the soname (libXXX.so.ver)
Ткст ПостроительСиПП::GetSoname(Ткст libName) const
{
	
	Ткст soname = дайИмяф(libName);
	int soLen = soname.дайСчёт();
	int soPos = впроп(soname).найдирек(".so");
	if(soPos < 0)
		soPos = soLen;
	else
		soPos += 3;
	if(soname.середина(soPos, 1) == ".")
	{
		soPos++;
		while(soPos < soLen && цифра_ли(soname[soPos]))
			soPos++;
	}
	return soname.лево(soPos);
}

// from complete lib имя/path (libXXX.so.ver.minVer) gets the link имя (libXXX.so)
Ткст ПостроительСиПП::GetSoLinkName(Ткст libName) const
{
	
	Ткст linkName = дайИмяф(libName);
	int soPos = впроп(linkName).найдирек(".so");
	if(soPos < 0)
		soPos = linkName.дайСчёт();
	else
		soPos += 3;
	return linkName.лево(soPos);
}

Ткст ПостроительСиПП::GetSharedLibPath(const Ткст& package) const
{
	Ткст outfn;
	if(естьФлаг("POSIX"))
	   outfn << "lib";
	for(const char *p = package; *p; p++)
		outfn.конкат(IsAlNum(*p) || *p == '-' ? *p : '_');
	if(!пусто_ли(version) && !естьФлаг("POSIX"))
		outfn << version;
	outfn << (естьФлаг("WIN32") || естьФлаг("WINCE") ? ".dll" : ".so");
	if(естьФлаг("POSIX"))
	{
		Точка p = ExtractVersion();
		int ver = пусто_ли(p.x) ? 1 : p.x;
		int minver = пусто_ли(p.y) ? 0 : p.y;
		outfn << '.' << ver << '.' << minver;
	}
	return CatAnyPath(дайПапкуФайла(target), outfn);
}

int ПостроительСиПП::разместиСлот()
{
	return host->разместиСлот();
}

bool ПостроительСиПП::пуск(const char *cmdline, int slot, Ткст ключ, int blitz_count)
{
	return host->пуск(CmdX(cmdline), slot, ключ, blitz_count);
}

bool ПостроительСиПП::пуск(const char *cmdline, Поток& out, int slot, Ткст ключ, int blitz_count)
{
	return host->пуск(CmdX(cmdline), out, slot, ключ, blitz_count);
}

bool ПостроительСиПП::жди()
{
	return host->жди();
}

bool ПостроительСиПП::жди(int slot)
{
	return host->жди(slot);
}

void ПостроительСиПП::OnFinish(Событие<>  cb)
{
	host->OnFinish(cb);
}

int CasFilter(int c) {
	return c == '\n' ? '\n' : c == '\t' ? ' ' : c >= ' ' ? c : 0;
}

int CharFilterEol(int c) {
	return c == '\n' || c == '\r' ? c : 0;
}

Вектор<Ткст> Cuprep(const Ткст& m, const ВекторМап<Ткст, Ткст>& mac,
                      const Вектор<Ткст>& inc) {
	Ткст r;
	Ткст q = фильтруй(m, CasFilter);
	const char *s = q;
	while(*s)
		if(*s == '$') {
			Ткст v;
			s++;
			if(*s == '$') {
				r.конкат('$');
				s++;
			}
			else
			if(*s == '(') {
				s++;
				bool pinc = false;
				if(*s == '!') {
					pinc = true;
					s++;
				}
				for(;;) {
					if(*s == ')') {
						s++;
						break;
					}
					if(*s == '\0')
						break;
					v.конкат(*s++);
				}
				if(pinc)
					for(int i = 0; i < inc.дайСчёт(); i++)
						if(inc[i].найди(' '))
							r << v << '"' << inc[i] << "\" ";
						else
							r << v << inc[i] << ' ';
				else
					r.конкат(mac.дай(v, Null));
			}
			else
				r.конкат('$');
		}
		else
			r.конкат(*s++);
	return разбей(r, CharFilterEol);
}

bool ПостроительСиПП::Cd(const Ткст& cmd) {
	if(cmd.дайДлину() > 2 && впроп(cmd.середина(0, 3)) == "cd ") {
		Ткст path = cmd.середина(3);
	#ifdef PLATFOTM_POSIX
		chdir(path);
	#endif
	#ifdef PLATFORM_WIN32
		SetCurrentDirectory(path);
	#endif
		return true;
	}
	return false;
}

bool ПостроительСиПП::Cp(const Ткст& cmd, const Ткст& package, bool& Ошибка) {
	if(cmd.дайДлину() > 2 && впроп(cmd.середина(0, 3)) == "cp ") {
		Вектор<Ткст> path = разбей(cmd.середина(3), ' ');
		if(path.дайСчёт() == 2) {
			Ткст p = дайПапкуФайла(PackagePath(package));
			Ткст p1 = нормализуйПуть(path[0], p);
			Ткст p2 = нормализуйПуть(path[1], p);
			RealizePath(p2);
			if(!файлЕсть(p1)) {
				вКонсоль("FAILED: " + cmd);
				Ошибка = true;
			}
			сохраниФайл(p2, загрузиФайл(p1));
		}
		return true;
	}
	return false;
}

static void AddPath(ВекторМап<Ткст, Ткст>& out, Ткст ключ, Ткст path)
{
	out.добавь(ключ, path);
	out.добавь(ключ + "_WIN", WinPath(path));
	out.добавь(ключ + "_UNIX", UnixPath(path));
}

void sGatherAllExt(Вектор<Ткст>& files, Вектор<Ткст>& dirs, const Ткст& pp, const Ткст& p)
{
	dirs.добавь(p);
	ФайлПоиск ff(pp + "/" + p + "/*.*");
	while(ff) {
		Ткст n = Merge("/", p, ff.дайИмя());
		if(ff.файл_ли())
			files.добавь(n);
		else
		if(ff.папка_ли()) {
			sGatherAllExt(files, dirs, pp, n);
		}
		ff.следщ();
	}
}

// gather files based on pattern -- pattern should be normalized
// if pattern doesn't contain wildcards is taken as a file имя
static void sGatherFiles(Индекс<Ткст> &files, const Ткст &pattern)
{
	if(!HasWildcards(pattern))
	{
		if(файлЕсть(pattern))
			files.найдиДобавь(pattern);
		return;
	}
	ФайлПоиск ff(pattern);
	while(ff)
	{
		if(ff.файл_ли())
			files.найдиДобавь(ff.дайПуть());
		ff.следщ();
	}
	ff.ищи(приставьИмяф(дайПапкуФайла(pattern), "*.*"));
	while(ff)
	{
		if(ff.папка_ли())
			sGatherFiles(files, приставьИмяф(ff.дайПуть(), дайИмяф(pattern)));
		ff.следщ();
	}
}

// gather folders based on pattern -- pattern should be normalized
// if pattern doesn't contain wildcards is taken as a folder имя
static void sGatherFolders(Индекс<Ткст> &folders, const Ткст &pattern)
{
	if(pattern.найди("*") < 0)
	{
		if(дирЕсть(pattern))
			folders.найдиДобавь(pattern);
		return;
	}
	Ткст pth = дайПапкуФайла(pattern);
	Ткст pat = дайИмяф(pattern);
	if(дирЕсть(pth))
		folders.найдиДобавь(pth);
	ФайлПоиск ff(pattern);
	while(ff)
	{
		if(ff.папка_ли())
			sGatherFolders(folders, приставьИмяф(ff.дайПуть(), pat));
		ff.следщ();
	}
}

Вектор<Ткст> ReadPatterns(СиПарсер& p)
{
	Вектор<Ткст> out;
	while(!p.кф_ли() && !p.сим(';')) {
		out << читайЗнач(p);
		p.сим(',');
	}
	return out;
}

static bool CheckImportCondition(СиПарсер &p, Вектор<Ткст> const &flag)
{
	// no condition == true
	if(!p.сим_ли('('))
		return true;
	Ткст s = ReadWhen(p);
	return MatchWhen(s, flag);
}

static void ExtExclude(СиПарсер& p, Ткст const &packageFolder, Индекс<Ткст>& x, Вектор<Ткст> const &flag)
{
	bool apply = CheckImportCondition(p, flag);
	Вектор<Ткст> e = ReadPatterns(p);
	if(apply) {
		for(int i = 0; i < e.дайСчёт(); i++)
			e[i] = нормализуйПуть(e[i], packageFolder);
		Вектор<int> remove;
		for(int i = 0; i < x.дайСчёт(); i++)
			for(int j = 0; j < e.дайСчёт(); j++) {
				if(PatternMatch(e[j], x[i])) {
					remove.добавь(i);
					break;
				}
			}
		x.удали(remove);
	}
}

Вектор<Ткст> ПостроительСиПП::CustomStep(const Ткст& pf, const Ткст& package_, bool& Ошибка)
{
	Ткст package = Nvl(package_, mainpackage);
	Ткст path = (*pf == '.' && pf[1] != '.') ? target : SourcePath(package, pf);
	Ткст file = path;
	Ткст ext = впроп(дайРасшф(pf));
	if(ext == ".ext") {
		Вектор<Ткст> const &flags = config.дайКлючи();

		Ткст packageFolder = дайПапкуФайла(path);
		
		Индекс<Ткст> pkg_files;
		Пакет pkg;
		pkg.грузи(PackagePath(package));
		for(int i = 0; i < pkg.дайСчёт(); i++)
			pkg_files.добавь(pkg[i]);
		
		Индекс<Ткст> out;
		Индекс<Ткст> include_path;
		Ткст f = загрузиФайл(path);
		try {
			СиПарсер p(f);
			while(!p.кф_ли()) {
				if(p.ид("files")) {
					bool apply = CheckImportCondition(p, flags);
					Вектор<Ткст> e = ReadPatterns(p);
					if(apply) {
						Индекс<Ткст> files;
						for(int iPat = 0; iPat < e.дайСчёт(); iPat++)
							sGatherFiles(files, нормализуйПуть(e[iPat], packageFolder));
						for(int i = 0; i < files.дайСчёт(); i++)
						{
							Ткст f = files[i];
							if(pkg_files.найди(f) < 0)
								out.найдиДобавь(f);
						}
					}
				}
				if(p.ид("exclude")) {
					ExtExclude(p, packageFolder, out, flags);
				}
				if(p.ид("include_path")) {
					bool apply = CheckImportCondition(p, flags);
					Вектор<Ткст> e = ReadPatterns(p);
					if(apply) {
						Индекс<Ткст> dirs;
						for(int iPat = 0; iPat < e.дайСчёт(); iPat++)
							sGatherFolders(dirs, нормализуйПуть(e[iPat], packageFolder));
						for(int i = 0; i < dirs.дайСчёт(); i++) {
							Ткст d = dirs[i];
							include_path.найдиДобавь(d);
						}
					}
				}
				if(p.ид("exclude_path")) {
					ExtExclude(p, packageFolder, include_path, flags);
				}
				else {
					p.передайИд("includes");
					bool apply = CheckImportCondition(p, flags);
					Вектор<Ткст> e = ReadPatterns(p);
					if(apply) {
						Индекс<Ткст> files;
						for(int iPat = 0; iPat < e.дайСчёт(); iPat++)
							sGatherFiles(files, нормализуйПуть(e[iPat], packageFolder));
						for(int i = 0; i < files.дайСчёт(); i++)
							include_path.найдиДобавь(дайПапкуФайла(files[i]));
					}
				}
			}
		}
		catch(СиПарсер::Ошибка) {
			вКонсоль("Неверный файл .ext");
			Ошибка = true;
			return Вектор<Ткст>();
		}
		
		for(int i = 0; i < include_path.дайСчёт(); i++)
			include.добавь(include_path[i]);
		
		Вектор<Ткст> o;
		for(int i = 0; i < out.дайСчёт(); i++)
			o.добавь(SourcePath(package, out[i]));
		return o;
	}
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Массив< ::CustomStep >& mv = wspc.дайПакет(i).custom;
		for(int j = 0; j < mv.дайСчёт(); j++) {
			const ::CustomStep& m = mv[j];
			if(MatchWhen(m.when, config.дайКлючи()) && m.MatchExt(ext)) {
				ВекторМап<Ткст, Ткст> mac;
				AddPath(mac, "PATH", file);
				AddPath(mac, "RELPATH", pf);
				AddPath(mac, "DIR", дайПапкуФайла(PackagePath(package)));
				AddPath(mac, "FILEDIR", дайПапкуФайла(file));
				AddPath(mac, "ПАКЕТ", package);
				mac.добавь("FILE", дайИмяф(file));
				mac.добавь("TITLE", дайТитулф(file));
				AddPath(mac, "EXEPATH", target);
				AddPath(mac, "EXEDIR", дайПапкуФайла(target));
				mac.добавь("EXEFILE", дайИмяф(target));
				mac.добавь("EXETITLE", дайТитулф(target));
				AddPath(mac, "OUTDIR", outdir);
				//BW
				AddPath(mac, "OUTDIR", дайПапкуФайла(target));
				AddPath(mac, "OUTFILE", дайИмяф(target));
				AddPath(mac, "OUTTITLE", дайТитулф(target));

				mac.добавь("INCLUDE", Join(include, ";"));

				Вектор<Ткст> out = Cuprep(m.output, mac, include);
				bool dirty = out.пустой();
				for(int i = 0; !dirty && i < out.дайСчёт(); i++)
					dirty = (дайФВремя(file) > дайФВремя(out[i]));
				if(dirty) {
					HdependTimeDirty();
					вКонсоль(дайИмяф(file));
					Вектор<Ткст> cmd = Cuprep(m.command, mac, include);
					Ткст cmdtext;
					for(int c = 0; c < cmd.дайСчёт(); c++) {
						PutVerbose(cmd[c]);
						if(!Cd(cmd[c]) && !Cp(cmd[c], package, Ошибка)) {
							Ткст ctext = cmd[c];
							const char *cm = ctext;
							if(*cm == '?')
								cm++;
							if(*ctext != '?' && выполни(cm)) {
								for(int t = 0; t < out.дайСчёт(); t++)
									DeleteFile(out[t]);
								вКонсоль("ПРОВАЛ: " + ctext);
								Ошибка = true;
								return Вектор<Ткст>();
							}
						}
					}
				}
				return out;
			}
		}
	}
	Вектор<Ткст> out;
	out.добавь(path);
	return out;
}

Ткст ПостроительСиПП::Includes(const char *sep, const Ткст& package, const Пакет& pkg)
{
	Ткст cc;
	for(int i = 0; i < include.дайСчёт(); i++)
		cc << sep << GetPathQ(include[i]);
	cc << sep << GetPathQ(outdir);
	return cc;
}

Вектор<Ткст> RepoInfo(const Ткст& package)
{
	Вектор<Ткст> info;
	Ткст d = дайПапкуФайла(PackagePath(package));
	int repo = GetRepoKind(d);
	if(repo == SVN_DIR) {
		Ткст v = Sys("svnversion " + d);
		if(цифра_ли(*v))
			info.добавь("#define bmSVN_REVISION " + какТкстСи(обрежьОба(v)));
		v = Sys("svn info " + d);
		ТкстПоток in(v);
		while(!in.кф_ли()) {
			Ткст l = in.дайСтроку();
			if(l.начинаетсяС("URL: ")) {
				info.добавь("#define bmSVN_URL " + какТкстСи(обрежьОба(l.середина(5))));
				break;
			}
		}
	}
	if(repo == GIT_DIR) {
		Ткст v = Sys("git rev-list --count HEAD");
		if(цифра_ли(*v))
			info.добавь("#define bmGIT_REVCOUNT " + какТкстСи(обрежьОба(v)));
	}
	return info;
}

void ПостроительСиПП::SaveBuildInfo(const Ткст& package)
{
	Ткст path = приставьИмяф(outdir, "build_info.h");
	RealizePath(path);
	ФайлВывод info(path);
	Время t = дайСисВремя();
	info << "#define bmYEAR   " << (int)t.year << "\r\n";
	info << "#define бмМЕСЯЦ  " << (int)t.month << "\r\n";
	info << "#define бмДЕНЬ    " << (int)t.day << "\r\n";
	info << "#define бмЧАС   " << (int)t.hour << "\r\n";
	info << "#define бмМИНУТА " << (int)t.minute << "\r\n";
	info << "#define бмСЕКУНДА " << (int)t.second << "\r\n";
	info << фмт("#define бмВРЕМЯ   Время(%d, %d, %d, %d, %d, %d)\r\n",
	        (int)t.year, (int)t.month, (int)t.day, (int)t.hour, (int)t.minute, (int)t.second);
	info << "#define bmMACHINE " << какТкстСи(дайИмяКомпа()) << "\r\n";
	info << "#define bmUSER    " << какТкстСи(дайИмяПользователя()) << "\r\n";

	if(package == mainpackage)
		info << Join(RepoInfo(package), "\r\n");
}

Ткст ПостроительСиПП::DefinesTargetTime(const char *sep, const Ткст& package, const Пакет& pkg)
{
	Ткст cc;
	for(int i = 0; i < config.дайСчёт(); i++)
		cc << sep << "flag" << config[i];
	if(main_conf)
		cc << sep << "MAIN_CONF";
	targettime = дайФВремя(target);
	
	return cc;
}

Ткст ПостроительСиПП::IncludesDefinesTargetTime(const Ткст& package, const Пакет& pkg)
{
	Ткст cc = Includes(" -I", package, pkg);
	cc << DefinesTargetTime(" -D", package, pkg);
	return cc;
}

bool ПостроительСиПП::HasAnyDebug() const
{
	return естьФлаг("DEBUG") || естьФлаг("DEBUG_MINIMAL") || естьФлаг("DEBUG_FULL");
}

Ткст SourceToObjName(const Ткст& package, const Ткст& srcfile_)
{
	Ткст srcfile = srcfile_;
	srcfile.обрежьКонец(".cpp");
	int q = дайПапкуФайла(PackagePath(package)).дайСчёт() + 1;
	if(q >= srcfile.дайСчёт())
		return дайТитулф(srcfile);
	Ткст r;
	for(const char *s = ~srcfile + q; *s; s++)
		r.конкат(findarg(*s, '/', '\\') >= 0 ? '_' : *s);
	return r;
}
