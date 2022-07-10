#include "Core.h"

Ткст GetLocalDir()
{
	return конфигФайл("UppLocal");
}

Ткст LocalPath(const Ткст& filename)
{
	return приставьИмяф(GetLocalDir(), filename);
}

Вектор<Ткст> IgnoreList()
{
	Вектор<Ткст> ignore;
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		for(int j = 0; j < pk.дайСчёт(); j++)
			if(!pk[j].separator && pk[j] == "ignorelist") {
				ФайлВвод in(SourcePath(wspc[i], pk[j]));
				while(in && !in.кф_ли()) {
					Ткст s = in.дайСтроку();
					if(!s.пустой())
						ignore.добавь(s);
				}
			}
	}
	return ignore;
}

Ткст FollowCygwinSymlink(const Ткст& file) {
	for(Ткст фн = file;;) {
		if(фн.пустой())
			return фн;
		ФайлВвод fi(фн);
		if(!fi.открыт())
			return фн;
		char буфер[10];
		if(!fi.дайВсе(буфер, 10) || memcmp(буфер, "!<symlink>", 10))
			return фн;
		фн = нормализуйПуть(загрузиПоток(fi), дайДиректориюФайла(фн));
	}
}

Вектор<Ткст> SplitDirs(const char *s) {
#ifdef PLATFORM_POSIX
	return разбей(s, [](int c) { return findarg(c, ';', ':') >= 0 ? c : 0; });
#else
	return разбей(s, ';');
#endif
}

static Ткст varsname = "default";

Ткст GetVarsName()
{
	return varsname;
}

Ткст VarFilePath(Ткст имя) {
	return конфигФайл(имя + ".var");
}

Ткст VarFilePath() {
	Ткст p = varsname;
	if(полнпуть_ли(varsname))
		return varsname;
	return VarFilePath(GetVarsName());
}

bool SaveVarFile(const char *filename, const ВекторМап<Ткст, Ткст>& var)
{
	ФайлВывод out(filename);
	for(int i = 0; i < var.дайСчёт(); i++)
		out << var.дайКлюч(i) << " = " << какТкстСи(var[i]) << ";\n";
	out.закрой();
	return !out.ошибка_ли();
}

bool LoadVarFile(const char *имя, ВекторМап<Ткст, Ткст>& _var)
{
	try {
		ВекторМап<Ткст, Ткст> var;
		Ткст env = загрузиФайл(имя);
		try {
			СиПарсер p(env);
			while(!p.кф_ли()) {
				Ткст v = p.читайИд();
				p.сим('=');
				if(p.ткст_ли())
					var.дайДобавь(v) = p.читайТкст();
				else {
					Ткст ln;
					while(p.подбериСим() != '\r' && p.подбериСим() != '\n' && p.подбериСим() != ';')
						ln.конкат(p.дайСим());
					var.дайДобавь(v) = ln;
					p.пробелы();
				}
				p.сим(';');
			}
		}
		catch(СиПарсер::Ошибка) {}
		_var = pick(var);
		return true;
	}
	catch(...) {
		return false;
	}
}

bool Гнездо::сохрани(const char *path)
{
	return SaveVarFile(path, var);
}

bool Гнездо::грузи(const char *path)
{
	InvalidatePackageCache();
	return LoadVarFile(path, var);
}

Ткст Гнездо::дай(const Ткст& id)
{
	return var.дай(id, Ткст());
}

void Гнездо::уст(const Ткст& id, const Ткст& val)
{
	var.дайДобавь(id) = val;
	InvalidatePackageCache();
}

Ткст Гнездо::путьПакета0(const Ткст& имя)
{
	Ткст uppfile = NativePath(имя);
	if(полнпуть_ли(uppfile)) return нормализуйПуть(uppfile);
	Вектор<Ткст> d = GetUppDirs();
	Ткст p;
	for(int i = 0; i < d.дайСчёт(); i++) {
		p = нормализуйПуть(приставьИмяф(приставьИмяф(d[i], uppfile),
		                  дайИмяф(uppfile)) + ".upp");
		if(файлЕсть(p)) return p;
	}
	return d.дайСчёт() ? нормализуйПуть(приставьИмяф(приставьИмяф(d[0], uppfile),
		                                дайИмяф(uppfile)) + ".upp") : "";
}

Ткст Гнездо::PackagePath(const Ткст& имя)
{
	int q = package_cache.найди(имя);
	if(q < 0) {
		Ткст h = путьПакета0(имя);
		package_cache.добавь(имя, h);
		return h;
	}
	return package_cache[q];
}

Гнездо& MainNest()
{
	return Single<Гнездо>();
}

bool SaveVars(const char *имя)
{
	if(!MainNest().сохрани(VarFilePath(имя)))
		return false;
	varsname = имя;
	return true;
}

Ткст DefaultHubFilePath()
{
	return конфигФайл("UppHub.path");
}

bool hubdir_resolved;

void SetHubDir(const Ткст& path)
{
	сохраниФайл(DefaultHubFilePath(), path);
	hubdir_resolved = false;
}

Ткст GetHubDir()
{
	static Ткст d;
	if(hubdir_resolved)
		return d;
	hubdir_resolved = true;
	d = GetVar("UPPHUB");
	if(d.дайСчёт() == 0 || !дирЕсть(d)) {
		d = загрузиФайл(DefaultHubFilePath());
		if(d.дайСчёт() == 0 || !дирЕсть(d)) {
			d = конфигФайл("UppHub");
			реализуйДир(d);
		}
	}
	d = нормализуйПуть(d);
	return d;
}

bool InUppHub(const Ткст& p)
{
	return p.начинаетсяС(GetHubDir());
}

bool   LoadVars(const char *имя) {
	varsname = (имя && *имя ? имя : "default");
	return MainNest().грузи(VarFilePath());
}

Ткст GetVar(const Ткст& var) {
	return MainNest().дай(var);
}

void SetVar(const Ткст& var, const Ткст& val, bool save) {
	MainNest().уст(var, val);
	if(save)
		SaveVars(GetVarsName());
}

bool hub_loaded;

Ткст main_nest;

void SetMainNest(const Ткст& n)
{
	main_nest = n;
}

Ткст GetAssemblyId()
{
	Ткст id = GetVarsName();
	Вектор<Ткст> s = SplitDirs(GetVar("РНЦП"));
	if(s.дайСчёт() && main_nest.дайСчёт() && s[0] != main_nest)
		id << "_" << дайИмяф(main_nest);
	return id;
}

Вектор<Ткст> GetUppDirsRaw()
{
	Вектор<Ткст> s = SplitDirs(GetVar("РНЦП"));
	static Вектор<Ткст> hub_dirs;
	if(!hub_loaded) {
		hub_dirs.очисть();
		for(const ФайлПоиск& ff : ФайлПоиск(GetHubDir() + "/*.*"))
			if(ff.папка_ли())
				hub_dirs.добавь(ff.дайПуть());
		hub_loaded = true;
	}
	s.приставь(hub_dirs);
	return s;
}

Вектор<Ткст> GetUppDirs()
{
	Вектор<Ткст> s = GetUppDirsRaw();
	if(main_nest.дайСчёт() && (s.дайСчёт() == 0 || main_nest != s[0]))
		s.вставь(0, main_nest);
	return s;
}

bool IsHubDir(const Ткст& path)
{
	return нормализуйПуть(path).начинаетсяС(GetHubDir());
}

void Гнездо::InvalidatePackageCache()
{
	package_cache.очисть();
	hub_loaded = false;
}

Ткст GetUppDir() {
	Вектор<Ткст> s = GetUppDirs();
#ifdef PLATFORM_WIN32
	return s.дайСчёт() == 0 ? дайПапкуФайла(дайФПутьИсп()) : s[0];
#endif
#ifdef PLATFORM_POSIX
	return s.дайСчёт() == 0 ? дайДомПапку() : s[0];
#endif
}
