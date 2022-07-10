#include "Core.h"

const char *SkipSpc(const char *term) {
	while(*term == '\t' || *term == ' ')
		term++;
	return term;
}

Ткст FindIncludeFile(const char *s, const Ткст& filedir, const Вектор<Ткст>& incdir)
{
	s = SkipSpc(s);
	int тип = *s;
	if(тип == '<' || тип == '\"' || тип == '?') {
		s++;
		Ткст имя;
		if(тип == '<') тип = '>';
		while(*s != '\r' && *s != '\n') {
			if(*s == тип) {
				if(тип == '\"') {
					Ткст фн = нормализуйПуть(имя, filedir);
					if(файлЕсть(фн))
						return фн;
				}
				for(int i = 0; i < incdir.дайСчёт(); i++) {
					Ткст фн = CatAnyPath(incdir[i], имя);
					if(файлЕсть(фн))
						return фн;
				}
				break;
			}
			имя.конкат(*s++);
		}
	}
	return Ткст();
}

class Hdepend {
	struct Info {
		Время                      time;
		Вектор<int>               depend;
		Вектор<bool>              bydefine;
		Индекс<Ткст>             macroinclude;
		Вектор<Ткст>            define;
		bool                      flag;
		bool                      macroflag;
		bool                      timedirty;
		bool                      guarded;
		bool                      blitzprohibit;

		bool                      CanBlitz() { return guarded && !blitzprohibit; }

		Info()                   { time = Null; flag = false; timedirty = true; guarded = false; }
	};

	МассивМап<Ткст, Info>            map;
	Вектор<Ткст>                    incdir;
	ВекторМап<Ткст, Индекс<Ткст> > depends;

	void   Include(const char *trm, Info& info, const Ткст& filedir, bool bydefine);
	void   ScanFile(const Ткст& path, int map_index);
	int    Файл(const Ткст& path);
	Время   ФВремя(int i);
	void   ClearFlag();
	void   ClearMacroFlag();
	void   GetMacroIndex(Индекс<Ткст>& dest, int ix);

public:
	void  SetDirs(Вектор<Ткст> pick_ id)  { incdir = pick(id); map.очисть(); }
	void  TimeDirty();

	void  ClearDependencies()  { depends.очисть(); }
	void  AddDependency(const Ткст& file, const Ткст& depends);

	Время                  ФВремя(const Ткст& path);
	bool                  BlitzApproved(const Ткст& path);
	Ткст                FindIncludeFile(const char *s, const Ткст& filedir) { return ::FindIncludeFile(s, filedir, incdir); }
	const Вектор<Ткст>& GetDefines(const Ткст& path);
	Вектор<Ткст>        GetDependencies(const Ткст& path, bool bydefine_too = true);
	const Вектор<Ткст>& GetAllFiles()                           { return map.дайКлючи(); }
};

void Hdepend::AddDependency(const Ткст& file, const Ткст& dep)
{
	depends.дайДобавь(нормализуйПуть(file)).найдиДобавь(нормализуйПуть(dep));
}

const char *RestOfLine(const char *term, Ткст& val) {
	while(*term && *term != '\r' && *term != '\n')
		val.конкат(*term++);
	return term;
}

void Hdepend::Include(const char *s, Hdepend::Info& info, const Ткст& filedir, bool bydefine) {
	s = SkipSpc(s);
	if(IsAlpha(*s) || *s == '_') {
		const char *macid = s;
		while(IsAlNum(*++s) || *s == '_')
			;
		info.macroinclude.найдиДобавь(Ткст(macid, s));
	}
	else {
		Ткст фн = FindIncludeFile(s, filedir);
		if(!пусто_ли(фн)) {
			info.depend.добавь(Файл(фн));
			info.bydefine.добавь(bydefine);
		}
	}
}

static const char *SkipComment(const char *s) {
	if(*s == '/') {
		if(s[1] == '/')
			for(s += 2; *s && *s != '\n';)
				s++;
		else if(s[1] == '*') {
			for(s += 2; *s && (*s != '*' || s[1] != '/'); s++)
				;
			if(*s)
				s += 2;
		}
	}
	return s;
}

void Hdepend::ScanFile(const Ткст& path, int map_index) {
	Info& info = map[map_index];
	Ткст src = загрузиФайл(path);
	const char *term = src;
	info.depend.очисть();
	info.bydefine.очисть();
	info.macroinclude.очисть();;
	info.define.очисть();;
	info.guarded = false;
	info.blitzprohibit = false;
	Ткст filedir = дайДиректориюФайла(path);
	bool testg = true;
	bool defines = IsCSourceFile(path);
	goto begin;
	while(*term) {
		if(term[0] == '/' && term[1] == '*') {
			while(*term) {
				if(term[0] == '*' && term[1] == '/') {
					term += 2;
					break;
				}
				term++;
			}
		}
		else
		if(term[0] == '/' && term[1] == '/') {
			if(term[2] == '#') {
				try {
					СиПарсер p(term + 3);
					if(p.ид("BLITZ_APPROVE") || p.ид("once"))
						info.guarded = true;
					else
					if(p.ид("BLITZ_PROHIBIT"))
						info.blitzprohibit = true;
				}
				catch(СиПарсер::Ошибка) {}
			}
			while(*term) {
				if(*term == '\n') break;
				term++;
			}
		}
		else
		if(*term == '\n') {
			term++;
		begin:
			while(*term == '\r' || *term == ' ' || *term == '\t') term++;
			if(*term == '#') {
				term++;
				while(*term == ' ' || *term == '\t') term++;
				if(term[0] == 'i' && term[1] == 'n' && term[2] == 'c' && term[3] == 'l' &&
				   term[4] == 'u' && term[5] == 'd' && term[6] == 'e' &&
				   (term[7] == ' ' || term[7] == '\t')) {
					term = SkipSpc(term + 7);
					Ткст val;
					term = RestOfLine(term, val);
					val = обрежьПраво(val);
					Include(val, info, filedir, false);
				}
				else
				if(testg && term[0] == 'i' && term[1] == 'f' && term[2] == 'n' &&
				   term[3] == 'd' && term[4] == 'e' && term[5] == 'f' &&
				   (term[6] == ' ' || term[6] == '\t')) {
					testg = false;
					try {
						СиПарсер p(term + 6);
						if(p.ид_ли()) {
							Ткст id = p.читайИд();
							if(p.сим('#') && p.ид("define") && p.ид_ли() && id == p.читайИд())
								info.guarded = true;
						}
					}
					catch(СиПарсер::Ошибка) {}
				}
				else
				if(defines && term[0] == 'd' && term[1] == 'e' && term[2] == 'f' &&
				   term[3] == 'i' && term[4] == 'n' && term[5] == 'e' &&
				   (term[6] == ' ' || term[6] == '\t')) {
				       try {
					       СиПарсер p(term + 6);
					       if(p.ид_ли())
					          info.define.добавь(p.читайИд());
					       term = p.дайУк();
				       }
				       catch(СиПарсер::Ошибка) {}
				}
				else
				if(term[0] == 'p' && term[1] == 'r' && term[2] == 'a' &&
				   term[3] == 'g' && term[4] == 'm' && term[5] == 'a' &&
				   (term[6] == ' ' || term[6] == '\t')) {
				    try {
						СиПарсер p(term + 6);
						if(p.ид("BLITZ_APPROVE") || p.ид("once"))
							info.guarded = true;
						else
						if(p.ид("BLITZ_PROHIBIT"))
							info.blitzprohibit = true;
						term = p.дайУк();
				    }
				    catch(СиПарсер::Ошибка) {}
				}
			}
			else if(IsAlpha(*term) || *term == '_') {
				const char *id = term;
				while(IsAlNum(*++term) || *term == '_')
					;
				bool is_bin = (term - id == 6 && !memcmp(id, "BINARY", 6));
				bool is_mask = (term - id == 11 && !memcmp(id, "BINARY_MASK", 11));
				bool is_array = (term - id == 12 && !memcmp(id, "BINARY_ARRAY", 12));
				if(is_bin || is_mask || is_array) {
					while(*term && (byte)*term <= ' ')
						term++;
					if(*term == '(') {
						while(*term && *term != ')' && *term != ',')
							term++;
						if(is_array && *term == ',') {
							while(*++term && *term != ')' && *term != ',')
								;
						}
						if(*term == ',') {
							while(*++term && *term != ')' && (byte)*term <= ' ')
								;
							if(*term == '\"')
								Include(term, info, filedir, false);
						}
					}
				}
			}
		}
		else {
			if((byte)*term > ' ')
				testg = false;
			term++;
		}
	}
	Индекс<Ткст> minc;
	for(;;) {
		ClearMacroFlag();
		Индекс<Ткст> new_minc;
		GetMacroIndex(new_minc, map_index);
		if(new_minc.дайСчёт() <= minc.дайСчёт())
			return;
		minc = pick(new_minc);
		term = src;
		while(*term) {
			while(*term && (byte)*term <= ' ')
				term++;
			if(*term == '#') {
				term++;
				while(*term && *term != '\n' && (byte)*term <= ' ')
					term++;
				if(Peek32le(term) == 'd' + ('e' << 8) + ('f' << 16) + ('i' << 24)
				&& Peek16le(term + 4) == 'n' + ('e' << 8)) {
					term += 6;
					while(*term && *term != '\n' && (byte)*term <= ' ')
						term++;
					const char *id = term;
					if(IsAlpha(*term) || *term == '_')
						while(IsAlNum(*++term) || *term == '_')
							;
					Ткст ident(id, term);
					if(minc.найди(ident) >= 0) {
						term = SkipSpc(term);
						Ткст incfn;
						id = term;
						while(*term && *term != '\n')
							term++;
						const char *e = term;
						while(e > id && (byte)e[-1] <= ' ')
							e--;
						Include(Ткст(id, e), info, filedir, true);
					}
				}
			}
			while(*term && *term != '\n')
				if(*term == '/' && (term[1] == '*' || term[1] == '/'))
					term = SkipComment(term);
				else
					term++;
			if(*term)
				term++;
		}
	}
}

int Hdepend::Файл(const Ткст& f) {
	Ткст path = нормализуйПуть(f);
	int ii = map.найдиДобавь(path);
	Info& info = map[ii];
	if(info.flag) return ii;
	info.flag = true;
	ФайлПоиск ff(path);
	if(!ff || ff.дайВремяПоследнЗаписи() == info.time) {
		if(info.timedirty) {
			for(int i = 0; i < info.depend.дайСчёт(); i++)
				Файл(map.дайКлюч(info.depend[i]));
			info.timedirty = false;
		}
	}
	else {
		info.time = ff.дайВремяПоследнЗаписи();
		if(info.time > дайСисВремя() + 10)
			вКонсоль(Ткст().конкат() << "ПРЕДУПРЕЖДЕНИЕ: " << path << " имеет неверное (будущее) время " << info.time);
		ScanFile(path, ii);
	}
	return ii;
}

void Hdepend::GetMacroIndex(Индекс<Ткст>& dest, int ix) {
	Info& info = map[ix];
	if(!info.macroflag) {
		info.macroflag = true;
		int i;
		for(i = 0; i < info.macroinclude.дайСчёт(); i++)
			dest.найдиДобавь(info.macroinclude[i]);
		for(i = 0; i < info.depend.дайСчёт(); i++)
			GetMacroIndex(dest, info.depend[i]);
	}
}

Время Hdepend::ФВремя(int ii)
{
//	LOG("ФВремя " << map.дайКлюч(ii));
//	LOGBEGIN();
	Info& info = map[ii];
//	DUMP(info.time);
	Время time = info.time;
	if(!info.flag) {
		info.flag = true;
		for(int i = 0; i < info.depend.дайСчёт(); i++)
			time = max(time, ФВремя(info.depend[i]));
	}
//	LOGEND();
	return time;
}

Вектор<Ткст> Hdepend::GetDependencies(const Ткст& path, bool bydefine_too)
{
	ClearFlag();
	Индекс<int> out;
	out.добавь(Файл(path));
	for(int i = 0; i < out.дайСчёт(); i++)
		if(out[i] >= 0) {
			Info& f = map[out[i]];
			for(int i = 0; i < f.depend.дайСчёт(); i++)
				if(bydefine_too || !f.bydefine[i])
					out.найдиДобавь(f.depend[i]);
		}
	Вектор<Ткст> outnames;
	for(int i = 1; i < out.дайСчёт(); i++)
		if(out[i] >= 0)
			outnames.добавь(map.дайКлюч(out[i]));
	return outnames;
}

void Hdepend::ClearFlag()
{
	for(int i = 0; i < map.дайСчёт(); i++)
		map[i].flag = false;
}

void Hdepend::ClearMacroFlag()
{
	for(int i = 0; i < map.дайСчёт(); i++)
		map[i].macroflag = false;
}

void Hdepend::TimeDirty()
{
	for(int i = 0; i < map.дайСчёт(); i++)
		map[i].timedirty = true;
}


Время Hdepend::ФВремя(const Ткст& path)
{
	Ткст p = нормализуйПуть(path);
	ClearFlag();
	int d = Файл(p);
	ClearFlag();
	Время h = ФВремя(d);
	d = depends.найди(p);
	if(d >= 0) {
		const Индекс<Ткст>& dep = depends[d];
		for(int i = 0; i < dep.дайСчёт(); i++) {
			ФайлПоиск ff(dep[i]);
			if(ff) {
				Время tm = ff.дайВремяПоследнЗаписи();
				if(tm > дайСисВремя() + 10)
					вКонсоль(Ткст().конкат() << "ПРЕДУПРЕЖДЕНИЕ: " << dep[i] << " имеет неверное (будущее) время " << tm);
				if(tm > h)
					h = tm;
			}
		}
	}
	return h;
}

bool Hdepend::BlitzApproved(const Ткст& path)
{
	ClearFlag();
	int d = Файл(path);
	ClearFlag();
	Info& info = map[d];
	if(info.blitzprohibit)
		return false;
	if(info.guarded)
		return true;
	for(int i = 0; i < info.depend.дайСчёт(); i++)
		if(!info.bydefine[i] && !map[info.depend[i]].CanBlitz()) {
			PutVerbose(Ткст().конкат() << map.дайКлюч(info.depend[i])
			           << "(1) : blocks BLITZ of " << path);
			return false;
		}
	return true;
}

const Вектор<Ткст>& Hdepend::GetDefines(const Ткст& path)
{
	ClearFlag();
	int d = Файл(path);
	ClearFlag();
	return map[d].define;
}

void HdependSetDirs(Вектор<Ткст> pick_ id)
{
	Single<Hdepend>().SetDirs(pick(id));
}

void HdependTimeDirty()
{
	Single<Hdepend>().TimeDirty();
}

Время HdependFileTime(const Ткст& path)
{
	Ткст of = исрДайОдинФайл();
	if(пусто_ли(of))
		return Single<Hdepend>().ФВремя(path);
	else
		return of == path ? дайСисВремя() : Время::наименьш();
}

Ткст FindIncludeFile(const char *s, const Ткст& filedir)
{
	return Single<Hdepend>().FindIncludeFile(s, filedir);
}

bool HdependBlitzApproved(const Ткст& path)
{
	return Single<Hdepend>().BlitzApproved(path);
}

const Вектор<Ткст>& HdependGetDefines(const Ткст& path)
{
	return Single<Hdepend>().GetDefines(path);
}

Вектор<Ткст> HdependGetDependencies(const Ткст& file, bool bydefine_too)
{
	return Single<Hdepend>().GetDependencies(file, bydefine_too);
}

void  HdependClearDependencies()
{
	Single<Hdepend>().ClearDependencies();
}

void  HdependAddDependency(const Ткст& file, const Ткст& depends)
{
	Single<Hdepend>().AddDependency(file, depends);
}

const Вектор<Ткст>& HdependGetAllFiles()
{
	return Single<Hdepend>().GetAllFiles();
}
