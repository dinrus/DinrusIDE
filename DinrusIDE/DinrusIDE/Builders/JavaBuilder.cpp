#include "Builders.h"

Ткст AdjustLines(const Ткст& file)
{
	Ткст out;
	const char *p = file;
	while(*p)
	{
		const char *b = p;
		while(*p && (byte)*p <= ' ' && *p != '\r' && *p != '\n')
			p++;
		if(*p == '#')
		{
			out.конкат("//");
			b = p;
			while(*p && *p != '\n' && *p != '\"')
				p++;
			out.конкат(b, (int)(p - b));
			b = p;
			if(*p == '\"')
			{
				out.конкат('\"');
				b = ++p;
				while(*p && *p++ != '\n')
					;
				const char *e = p;
				while(e > b && (byte)e[-1] <= ' ')
					e--;
				if(e[-1] == '\"')
					e--;
				out.конкат(UnixPath(Ткст(b, e)));
				out.конкат("\"\r\n");
				b = p;
				continue;
			}
		}
		out.конкат(b, (int)(p - b));
		while(*p && *p != '\n')
		{
			b = p;
			while(*p && *p != '\n' && *p != '\r')
				p++;
			out.конкат(b, (int)(p - b));
			while(*p == '\r')
				p++;
		}
		if(*p == '\n')
		{
			p++;
			out.конкат("\r\n");
		}
	}
	return out;
}

Ткст JavaBuilder::JavaLine()
{
	return "javac";
}

Ткст JavaBuilder::JarLine()
{
	return "jar";
}

enum { MAINCLASS, MAINDIR, MANIFEST, ITEMCOUNT };

bool JavaBuilder::постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>&, Ткст& linkoptions,
	const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int)
{
	int time = msecs();
	int i;
	int manifest = -1;
	Ткст packagepath = PackagePath(package);
	Пакет pkg;
	pkg.грузи(packagepath);
	Ткст packagedir = дайПапкуФайла(packagepath);
	ChDir(packagedir);
	PutVerbose("cd " + packagedir);
	Вектор<Ткст> pkgsfile;
	Вектор<Ткст> sfile;
	Вектор<Ткст> sobjfile;
	Вектор<Ткст> soptions;
	bool           Ошибка = false;
	bool main = естьФлаг("MAIN");

	for(i = 0; i < pkg.дайСчёт(); i++) {
		if(!строитсяИср())
			return false;
		if(!pkg[i].separator) {
			Ткст gop = Gather(pkg[i].option, config.дайКлючи());
			Вектор<Ткст> srcfile = CustomStep(pkg[i], package, Ошибка);
			if(srcfile.дайСчёт() == 0)
				Ошибка = true;
			for(int j = 0; j < srcfile.дайСчёт(); j++)
			{
				Ткст фн = srcfile[j];
				Ткст ext = впроп(дайРасшф(фн));
				bool ismf = false;
				if(ext == ".java" || main && ext == ".mf")
				{
					if(ext == ".mf")
					{
						ismf = true;
						if(manifest >= 0)
						{
							вКонсоль(фмт("%s(1): duplicate manifest file", фн));
							вКонсоль(фмт("%s(1): (previous manifest file)", sfile[manifest]));
						}
						manifest = sfile.дайСчёт();
					}
					Ткст pkgfile = приставьИмяф(package, pkg[i]);
					pkgsfile.добавь(pkgfile);
					sfile.добавь(фн);
					soptions.добавь(gop);
					Ткст objfile = NativePath(CatAnyPath(outdir, ismf ? Ткст("manifest.mf") : pkgfile));
					sobjfile.добавь(objfile);
				}
			}
		}
	}

	Вектор<Хост::ИнфОФайле> sobjinfo = host->дайИнфОФайле(sobjfile);
	int ccount = 0;
	for(i = 0; i < sfile.дайСчёт(); i++) {
		if(!строитсяИср())
			return false;
		if(HdependFileTime(sfile[i]) > sobjinfo[i]) {
			ccount++;
			if(!PreprocessJava(sfile[i], sobjfile[i], soptions[i], package, pkg))
				Ошибка = true;
		}
	}
	linkfile.добавь(outdir);
	if(ccount > 0)
		вКонсоль(Ткст().конкат() << ccount << " file(s) preprocessed in " << GetPrintTime(time) <<
		           " " << int(msecs() - time) / ccount << " msec/file");
	linkoptions << ' ' << Gather(pkg.link, config.дайКлючи());

	if(!Ошибка && естьФлаг("MAIN") && !sfile.пустой())
	{
		Ткст mainfile = sfile.верх();
		Ткст mainobj = sobjfile.верх();
		Ткст maincls = форсируйРасш(mainobj, ".class");
		Ткст libs;
		int i;
		for(i = 0; i < libpath.дайСчёт(); i++)
			libs << (i ? ";" : " -classpath ") << '\"' << libpath[i] << '\"';
		Ткст linkcmd;
		linkcmd << "javac";
		linkcmd << (естьФлаг("DEBUG") ? " -g" : " -g:none");
		if(!естьФлаг("DEBUG")) {
			if(!пусто_ли(release_options))
				linkcmd << ' ' << release_options;
			else
				linkcmd << " -O";
		}
		linkcmd << " -deprecation" << linkoptions << " -sourcepath ";
		bool win32 = естьФлаг("WIN32");
		for(i = 0; i < linkfile.дайСчёт(); i++) {
			linkcmd << (i ? (win32 ? ";" : ":") : "");
			if(linkfile[i].найди(' ') >= 0)
				linkcmd << '\"' << linkfile[i] << '\"';
			else
				linkcmd << linkfile[i];
		}
		linkfile.вставьН(0, ITEMCOUNT);
		linkfile[MAINCLASS] = maincls;
		linkfile[MAINDIR] = outdir;
		linkfile[MANIFEST] = (manifest >= 0 ? sobjfile[manifest] : Ткст());
		linkcmd << ' ' << mainobj;
		linkoptions = linkcmd;
	}
	return !Ошибка;
}

bool JavaBuilder::Preprocess(const Ткст& package, const Ткст& file, const Ткст& target, bool)
{
	return Построитель::Preprocess(file, target, Null, false);
}

bool JavaBuilder::PreprocessJava(Ткст file, Ткст target, Ткст options,
                                 Ткст package, const Пакет& pkg)
{
	Ткст mscpp = GetVar("MSCPP_JDK");
	Ткст cc;
	if(!пусто_ли(mscpp))
		cc = mscpp + " /C /TP /P /nologo ";
	else
		cc = "cpp -C ";
	cc << IncludesDefinesTargetTime(package, pkg);
	int time = msecs();
	RealizePath(target);
	Ткст execpath;
	execpath << cc << ' ' << options << ' ';
	Ткст prepfile;
	bool Ошибка = false;
	if(!пусто_ли(mscpp))
	{
		prepfile = форсируйРасш(file, ".i");
		host->ChDir(дайПапкуФайла(prepfile));
		execpath << file;
	}
	else
	{
		вКонсоль(file);
		execpath << file << " " << target;
		prepfile = target;
	}
	if(выполни(execpath) != 0)
	{
		DeleteFile(target);
		Ошибка = true;
	}
	Ткст prep = загрузиФайл(prepfile);
	if(prep.пустой())
	{
		вКонсоль(фмт("Ошибка loading preprocessed file %s", prepfile));
		Ошибка = true;
	}
	DeleteFile(prepfile);
	if(!prep.пустой() && !::сохраниФайл(target, AdjustLines(prep)))
	{
		DeleteFile(target);
		Ошибка = true;
		вКонсоль(фмт("%s: Ошибка saving file.", target));
	}
	PutVerbose("preprocessed in " + GetPrintTime(time));
	return !Ошибка;
}

Время JavaBuilder::AddClassDeep(Ткст& link, Ткст dir, Ткст reldir)
{
	Время time = Время::наименьш();
	Вектор<Ткст> folders;
	for(ФайлПоиск ff(приставьИмяф(dir, приставьИмяф(reldir, "*"))); ff; ff.следщ()) {
		if(ff.папка_ли())
			folders.добавь(ff.дайИмя());
		else if(!stricmp(дайПозРасшф(ff.дайИмя()), ".class"))
		{
			link << " -C \"" << dir << "\" \"" << UnixPath(CatAnyPath(reldir, ff.дайИмя())) << '\"';
			time = max(time, Время(ff.дайВремяПоследнЗаписи()));
		}
	}
	for(int f = 0; f < folders.дайСчёт(); f++)
		time = max(time, AddClassDeep(link, dir, приставьИмяф(reldir, folders[f])));
	return time;
}

bool JavaBuilder::Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool)
{
	if(linkfile.дайСчёт() < ITEMCOUNT)
		return false;
	int time = msecs();
	Ткст mainclass = linkfile[MAINCLASS];
	Ткст maindir = linkfile[MAINDIR];
	Ткст manifest = linkfile[MANIFEST];
	вКонсоль("Compiling...");
	if(выполни(linkoptions) != 0) {
		DeleteFile(mainclass);
		return false;
	}
	PutVerbose("compiled in " + GetPrintTime(time));
	host->ChDir(maindir);

	вКонсоль("Archiving...");
	Ткст cmdline;
	cmdline << "cf";
	if(!manifest.пустой())
		cmdline << 'm';
	cmdline << ' ' << target;
	if(!manifest.пустой())
		cmdline << ' ' << manifest;
	Время tm = Время::наименьш();
	for(int i = ITEMCOUNT; i < linkfile.дайСчёт(); i++)
		tm = max(tm, AddClassDeep(cmdline, linkfile[i], Null));
	bool Ошибка = false;
	if(tm > targettime) {
		CustomStep(".pre-link", Null, Ошибка);
		Ткст link, response;
		link << "jar ";
		if(cmdline.дайДлину() < 32000)
			link << cmdline;
		else {
			response = дайВремИмяф("jar");
			link << '@' << response;
			if(!РНЦП::сохраниФайл(response, cmdline)) {
				вКонсоль(Ткст().конкат() << "Ошибка writing JAR response file '" << response << "'");
				return false;
			}
		}
		bool ok = (выполни(link) == 0);
		if(!пусто_ли(response))
			удалифл(response);
		if(!ok) {
			DeleteFile(target);
			return false;
		}
		CustomStep(".post-link", Null, Ошибка);
		вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
		           << " B) archived in " << GetPrintTime(time));
	}
	else
		вКонсоль(Ткст().конкат() << target << " (" << дайИнфОФайле(target).length
		           << " B) is up to date.");
	return true;
}

static Построитель *CreateJavaBuilder()
{
	return new JavaBuilder;
}

ИНИЦИАЛИЗАТОР(JavaBuilder)
{
	RegisterBuilder("JDK", &CreateJavaBuilder);
}
