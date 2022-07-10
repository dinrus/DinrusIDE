#include "Core.h"

bool IsCSourceFile(const char *path)
{
	Ткст ext = впроп(дайРасшф(path));
	return ext == ".cpp" || ext == ".c" || ext == ".cc" || ext == ".cxx"
	                     || ext == ".m" || ext == ".mm" || ext == ".icpp";
}

bool IsCHeaderFile(const char *path)
{
	Ткст ext = впроп(дайРасшф(path));
	return ext == ".h" || ext == ".hpp" || ext == ".hh" || ext == ".hxx";
}

bool IsFullDirectory(const Ткст& d) {
	if(полнпуть_ли(d)) {
		ФайлПоиск ff(d);
		if(ff) return ff.директория_ли();
	}
	return false;
}

bool папка_ли(const Ткст& path)
{
	if(пусто_ли(path) || *path.последний() == '\\' || *path.последний() == '/' || *path.последний() == ':')
		return true;
	if(path.найди('?') >= 0 || path.найди('*') >= 0)
		return false;
	ФайлПоиск ff(path);
	return ff && ff.директория_ли();
}

void InvalidatePackageCache()
{
	MainNest().InvalidatePackageCache();
}

Ткст PackagePath(const Ткст& имя)
{
	return MainNest().PackagePath(имя);
}

Ткст GetPackagePathNest(const Ткст& path)
{
	Ткст h = UnixPath(нормализуйПуть(path));
	for(auto dir : GetUppDirs())
		if(h.начинаетсяС(UnixPath(нормализуйПуть(dir)) + '/'))
			return dir;
	return Null;
}

Ткст SourcePath(const Ткст& package, const Ткст& file) {
	if(полнпуть_ли(file)) return NativePath(file);
	return нормализуйПуть(приставьИмяф(дайПапкуФайла(PackagePath(package)), file));
}

bool IsNestReadOnly(const Ткст& path)
{
	Вектор<Ткст> d = GetUppDirs();
	for(int i = 0; i < d.дайСчёт(); i++)
		if(path.начинаетсяС(d[i]) && файлЕсть(приставьИмяф(d[i], "readonly")))
			return true;
	return false;
}

Ткст GetAnyFileName(const char *path)
{
	const char *p = path;
	for(char c; (c = *path++) != 0;)
		if(c == '\\' || c == '/' || c == ':')
			p = path;
	return p;
}

Ткст GetAnyFileTitle(const char *path)
{
	Ткст фн = GetAnyFileName(path);
	const char *p = фн;
	const char *e = p;
	while(*e && *e != '.')
		e++;
	return Ткст(p, e);
}

Ткст CatAnyPath(Ткст path, const char *more)
{
	if(!more || !*more)
		return path;
	if(!path.пустой() && *path.последний() != '\\' && *path.последний() != '/' &&
	*more != '\\' && *more != '/')
#ifdef PLATFORM_WIN32
		path.конкат('\\');
#else
		path.конкат('/');
#endif
	path.конкат(more);
	return path;
}


void SplitHostName(const char *hostname, Ткст& host, int& port) {
	enum { DEFAULT_REMOTE_PORT = 2346 };
	const char *p = hostname;
	while(p && *p != ':')
		p++;
	host = Ткст(hostname, p);
	if(*p++ == ':' && цифра_ли(*p))
		port = stou(p);
	else
		port = DEFAULT_REMOTE_PORT;
}

Вектор<Ткст> SplitFlags0(const char *flags) {
	return разбей(flags, CharFilterWhitespace);
}

Вектор<Ткст> SplitFlags(const char *flags, bool main, const Вектор<Ткст>& accepts)
{
	Вектор<Ткст> v = SplitFlags0(flags);
	Вектор<Ткст> h;
	for(int i = 0; i < v.дайСчёт(); i++)
		if(v[i][0] == '.') {
			Ткст f = v[i].середина(1);
			if(main || найдиИндекс(accepts, f) >= 0)
				h.добавь(v[i].середина(1));
		}
		else
			h.добавь(v[i]);
	if(main)
		h.добавь("MAIN");
	return h;
}

Вектор<Ткст> SplitFlags(const char *flags, bool main)
{
	Вектор<Ткст> dummy;
	return SplitFlags(flags, main, dummy);
}

bool естьФлаг(const Вектор<Ткст>& conf, const char *flag) {
	return найдиИндекс(conf, flag) >= 0;
}

Вектор<Ткст> Combine(const Вектор<Ткст>& conf, const char *flags) {
	Вектор<Ткст> cfg(conf, 1);
	try {
		СиПарсер p(flags);
		while(!p.кф_ли()) {
			bool isnot = p.сим('!');
			if(!p.ид_ли()) break;
			Ткст flg = p.читайИд();
			int i = найдиИндекс(cfg, flg);
			if(isnot) {
				if(i >= 0) cfg.удали(i);
			}
			else
				if(i < 0) cfg.добавь(flg);
		}
	}
	catch(СиПарсер::Ошибка) {}
	return cfg;
}

Ткст Gather(const Массив<OptItem>& set, const Вектор<Ткст>& conf, bool nospace) {
	Ткст s;
	for(int i = 0; i < set.дайСчёт(); i++)
		if(MatchWhen(set[i].when, conf)) {
			if(!nospace && !s.пустой()) s.конкат(' ');
			s.конкат(set[i].text);
		}
	return s;
}

Вектор<Ткст> GatherV(const Массив<OptItem>& set, const Вектор<Ткст>& conf) {
	Вектор<Ткст> s;
	for(int i = 0; i < set.дайСчёт(); i++)
		if(MatchWhen(set[i].when, conf))
			s << set[i].text;
	return s;
}

int    дайТип(const Вектор<Ткст>& conf, const char *flags) {
	Вектор<Ткст> f = SplitFlags(flags);
	int q = FLAG_UNDEFINED;
	for(int i = 0; i < f.дайСчёт(); i++)
		if(естьФлаг(conf, f[i])) {
			if(q == FLAG_UNDEFINED)
				q = i;
			else
				q = FLAG_MISMATCH;
		}
	return q;
}

Ткст удалиТип(Вектор<Ткст>& conf, const char *flags)
{
	Ткст old;
	Индекс<Ткст> f(SplitFlags(flags));
	for(int i = conf.дайСчёт(); --i >= 0;)
		if(f.найди(conf[i]) >= 0)
		{
			old = conf[i];
			conf.удали(i);
		}
	return old;
}

int    дайТип(const Вектор<Ткст>& conf, const char *flags, int def) {
	int q = дайТип(conf, flags);
	return q == FLAG_UNDEFINED ? def : q;
}

bool   дайФлаг(const Вектор<Ткст>& conf, const char *flag) {
	return найдиИндекс(conf, flag) >= 0;
}

void РОбласть::AddLoad(const Ткст& имя, bool match, const Вектор<Ткст>& flag)
{
	package.добавь(имя).грузи(PackagePath(имя));
}

void РОбласть::AddUses(Пакет& p, bool match, const Вектор<Ткст>& flag)
{
	int q = package.дайСчёт();
	for(int i = 0; i < p.uses.дайСчёт(); i++) {
		Ткст uses = UnixPath(p.uses[i].text);
		if((!match || MatchWhen(p.uses[i].when, flag)) && package.найди(uses) < 0)
			AddLoad(uses, match, flag);
	}
	for(int i = q; i < package.дайСчёт(); i++)
		AddUses(package[i], match, flag);
}

void РОбласть::скан(const char *prjname) {
	package.очисть();
	AddLoad(prjname, false, Вектор<Ткст>());
	AddUses(package[0], false, Вектор<Ткст>());
}

void РОбласть::скан(const char *prjname, const Вектор<Ткст>& flag) {
	package.очисть();
	AddLoad(prjname, true, flag);
	AddUses(package[0], true, flag);
}

void РОбласть::Dump() {
	for(int i = 0; i < package.дайСчёт(); i++) {
		Пакет& prj = package[i];
		LOG("Пакет " << package.дайКлюч(i));
		LOG(" file ");
		for(int i = 0; i < prj.file.дайСчёт(); i++)
			LOG("\t" << prj.file[i]);
	}
}

Вектор<Ткст> РОбласть::GetAllAccepts(int pk) const
{
	Индекс<Ткст> accepts;
	Индекс<int> pkg;
	pkg.добавь(pk);
	for(int i = 0; i < pkg.дайСчёт(); i++) {
		const Пакет& p = package[pkg[i]];
		FindAppend(accepts, p.accepts);
		for(int u = 0; u < p.uses.дайСчёт(); u++) {
			int f = package.найди(UnixPath(p.uses[u].text));
			if(f >= 0)
				pkg.найдиДобавь(f);
		}
	}
	return accepts.подбериКлючи();
}