#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

#define LTIMING(x)  // DTIMING(x)
#define LLOG(x)     // DLOG(x)

static std::atomic<int> s_PPserial;
static ВекторМап<Ткст, PPMacro>  sAllMacros; // Only MakePP can write to this
static МассивМап<Ткст, PPFile>    sPPfile; // Only MakePP can write to this

static ВекторМап<Ткст, Время>     s_PathFileTime;
static СтатическийСтопор                 s_PathFileTimeMutex;

static ВекторМап<Ткст, Ткст>   s_IncludePath;
static Ткст                      s_Include_Path;
static СтатическийСтопор                 s_IncludePathMutex;

static СтатическийСтопор                 s_FlatPPMutex;
static МассивМап<Ткст, FlatPP>    s_FlatPP; // МассивМап to allow read access

int  NextPPSerial()
{
	return ++s_PPserial;
}

void SweepPPFiles(const Индекс<Ткст>& keep)
{
	Индекс<int> pp_segment_id;
	int unlinked_count = 0;
	for(int i = 0; i < sPPfile.дайСчёт(); i++)
		if(sPPfile.отлинкован(i))
			unlinked_count++;
		else
			if(keep.найди(sPPfile.дайКлюч(i)) < 0) {
				unlinked_count++;
				sPPfile.отлинкуй(i);
			}
			else {
				const PPFile& p = sPPfile[i];
				for(int j = 0; j < p.элт.дайСчёт(); j++)
					pp_segment_id.найдиДобавь(p.элт[j].segment_id);
			}
	if(unlinked_count > sPPfile.дайСчёт() / 2) {
		CleanPP();
		return;
	}
	unlinked_count = 0;
	for(int i = 0; i < sAllMacros.дайСчёт(); i++) {
		if(sAllMacros.отлинкован(i))
			unlinked_count++;
		else
		if(sAllMacros[i].segment_id && pp_segment_id.найди(sAllMacros[i].segment_id) < 0) {
			sAllMacros.отлинкуй(i);
			unlinked_count++;
		}
		if(unlinked_count > sAllMacros.дайСчёт() / 2) {
			CleanPP();
			return;
		}
	}
}

Ткст GetSegmentFile(int segment_id)
{
	for(int i = 0; i < sPPfile.дайСчёт(); i++) {
		const Массив<PPItem>& m = sPPfile[i].элт;
		for(int j = 0; j < m.дайСчёт(); j++)
			if(m[j].тип == PP_DEFINES && m[j].segment_id == segment_id)
				return sPPfile.дайКлюч(i);
	}
	return "<not found>";
}

PPMacro *FindPPMacro(const Ткст& id, Индекс<int>& segment_id, int& segmenti)
{
	Индекс<int> undef;
	PPMacro *r;
	int best;
	for(int pass = 0; pass < 2; pass++) {
		r = NULL;
		best = segmenti;
		int line = -1;
		int q = sAllMacros.найди(id);
		while(q >= 0) {
			PPMacro& m = sAllMacros[q];
			if(m.macro.IsUndef()) {
				if(pass == 0 && segment_id.найди(m.segment_id) >= 0)
					undef.найдиДобавь(m.segment_id); // cancel out undefined macro...
			}
			else
			if(pass == 0 || m.segment_id == 0 || undef.найди(m.undef_segment_id) < 0) {
				int si = m.segment_id == 0 ? INT_MAX : segment_id.найди(m.segment_id); // defs macros always override
				if(si > best || si >= 0 && si == best && m.line > line) {
					best = si;
					line = m.line;
					r = &m;
				}
			}
			q = sAllMacros.найдиСледщ(q);
		}
		if(undef.дайСчёт() == 0)
			break;
	}
	segmenti = best;
	return r;
}

const CppMacro *FindMacro(const Ткст& id, Индекс<int>& segment_id, int& segmenti)
{
	PPMacro *m = FindPPMacro(id, segment_id, segmenti);
	return m ? &m->macro : NULL;
}

void PPFile::CheckEndNamespace(Вектор<int>& namespace_block, int level, Md5Stream& md5)
{
	if(namespace_block.дайСчёт() && namespace_block.верх() == level) {
		namespace_block.сбрось();
		элт.добавь().тип = PP_NAMESPACE_END;
		md5.помести('.');
	}
}

void PPFile::Parse(Поток& in)
{
	LTIMING("PPFile::Parse");
	for(int i = 0; i < ppmacro.дайСчёт(); i++)
		sAllMacros.отлинкуй(ppmacro[i]);
	ppmacro.очисть();
	элт.очисть();
	includes.очисть();
	bool was_using = false;
	bool was_namespace = false;
	int  level = 0;
	bool incomment = false;
	bool do_pp = true;
	Вектор<int> namespace_block;
	bool next_segment = true;
	Индекс<int> local_segments;
	keywords.очисть();
	int linei = 0;
	Md5Stream md5;
	int current_serial = 0;
	
	ВекторМап<Ткст, PPMacro> local_macro; // gather all macros first to reduce locking
	
	while(!in.кф_ли()) {
		Ткст l = in.дайСтроку();
		const char *ll = l;
		while(*ll == ' ' || *ll == '\t')
			ll++;
		if(ll[0] == '/' && ll[1] == '/' && ll[2] == '$')
			do_pp = decode(ll[3], '+', true, '-', false, do_pp);
		while(*l.последний() == '\\' && !in.кф_ли()) {
			l.обрежь(l.дайДлину() - 1);
			l.конкат(in.дайСтроку());
		}
		RemoveComments(l, incomment);
		try {
			СиПарсер p(l);
			if(p.сим('#')) {
				if(do_pp) {
					if(p.ид("define")) {
						if(next_segment) {
							PPItem& m = элт.добавь();
							m.тип = PP_DEFINES;
							m.segment_id = current_serial = NextPPSerial();
							next_segment = false;
							local_segments.добавь(current_serial);
						}
						CppMacro def;
						Ткст   id = def.Define(p.дайУк());
						if(id.дайСчёт()) {
							PPMacro& l = local_macro.добавь(id);
							l.segment_id = current_serial;
							l.line = linei;
							l.macro = def;
						/*
							PPMacro m;
							m.segment_id = current_serial;
							m.line = linei;
							m.macro = def;
							ppmacro.добавь(sAllMacros.помести(id, m));
						*/
							md5.помести("#", 1);
							md5.помести(id);
							md5.помести(0);
							md5.помести(l.macro.md5, 16);
						}
					}
					else
					if(p.ид("undef")) {
						if(p.ид_ли()) {
							Ткст id = p.читайИд();
							if(id.дайСчёт()) {
								md5.помести("#", 1);
								md5.помести(id);
								md5.помести(1);
								int q = local_macro.найдиПоследн(id); // heuristic: only local undefs are allowed
								while(q >= 0) {
									PPMacro& um = local_macro[q];
									if(!um.macro.IsUndef()) { // found corresponding macro to undef
										PPItem& m = элт.добавь();
										m.тип = PP_DEFINES;
										m.segment_id = current_serial = NextPPSerial();
										um.undef_segment_id = m.segment_id;
										next_segment = true;
										local_segments.добавь(current_serial);
										PPMacro& l = local_macro.добавь(id);
										l.segment_id = current_serial;
										l.line = linei;
										l.macro.SetUndef();
									}
									q = local_macro.найдиПредш(q);
								}
							}
						/*
							int segmenti = -1;
							PPMacro *um = FindPPMacro(id, local_segments, segmenti);
							if(um && segmenti) {
								PPItem& m = элт.добавь();
								m.тип = PP_DEFINES;
								m.segment_id = current_serial = NextPPSerial();
								um->undef_segment_id = m.segment_id;
								next_segment = true;
								local_segments.добавь(current_serial);
								if(id.дайСчёт()) {
									PPMacro m;
									m.segment_id = current_serial;
									m.line = linei;
									m.macro.SetUndef();
									ppmacro.добавь(sAllMacros.помести(id, m));
								}
							}
						*/
						}
					}
					else
					if(p.ид("include")) {
						PPItem& m = элт.добавь();
						next_segment = true;
						m.тип = PP_INCLUDE;
						m.text = обрежьОба(p.дайУк());
						if(пусто_ли(m.text))
							элт.сбрось();
						else
							includes.найдиДобавь(m.text);
						md5.помести('@');
						md5.помести(m.text);
					}
				}
			}
			else {
				while(!p.кф_ли()) {
					if(was_namespace) {
						int тип = was_using ? PP_USING : PP_NAMESPACE;
						Ткст id;
						while(p.сим2(':', ':'))
							id = "::";
						if(p.ид_ли()) {
							id << p.читайИд();
							while(p.сим2(':', ':') && p.ид_ли())
								id << "::" << p.читайИд();
							if(!was_using)
								namespace_block.добавь(level);
							if(!was_using || level == 0) {
								PPItem& m = элт.добавь();
								next_segment = true;
								m.тип = тип;
								m.text = id;
							}
							md5.помести('$');
							md5.помести(тип);
							md5.помести(id);
						}
						was_namespace = was_using = false;
					}
					else
					if(p.ид("using"))
						was_using = true;
					else
					if(p.ид("namespace"))
						was_namespace = true;
					else {
						was_using = was_namespace = false;
						if(p.ид_ли()) {
							static const ВекторМап<Ткст, Ткст>& namespace_macro = GetNamespaceMacros();
							static const Индекс<Ткст>& namespace_end_macro = GetNamespaceEndMacros();

							Ткст id = p.читайИд();
							int q = namespace_macro.найди(id);
							if(q > 0) {
								PPItem& m = элт.добавь();
								next_segment = true;
								m.тип = PP_NAMESPACE;
								m.text = namespace_macro[q];
								namespace_block.добавь(level);
								level++;
								md5.помести('%');
								md5.помести(id);
							}
							else {
								q = namespace_end_macro.найди(id);
								if(q >= 0) {
									level--;
									CheckEndNamespace(namespace_block, level, md5);
								}
							}
							keywords.добавь(id);
						}
						else
						if(p.сим('}')) {
							if(level > 0) {
								level--;
								CheckEndNamespace(namespace_block, level, md5);
							}
						}
						else
						if(p.сим('{'))
							level++;
						else
							p.пропустиТерм();
					}
				}
			}
		}
		catch(...) {}
		linei++;
	}
	md5sum = md5.FinishString();
	сортируй(keywords);
	Вектор<int> remove;
	int i = 0;
	while(i < keywords.дайСчёт()) { // удали identical items
		int ii = i;
		i++;
		while(i < keywords.дайСчёт() && keywords[ii] == keywords[i])
			remove.добавь(i++);
	}
	keywords.удали(remove);
	INTERLOCKED { // this is the only place that is allowed to write to sAllMacros
		for(int i = 0; i < local_macro.дайСчёт(); i++)
			ppmacro.добавь(sAllMacros.помести(local_macro.дайКлюч(i), local_macro[i]));
	}
}

void PPFile::Dump() const
{
	for(int i = 0; i < элт.дайСчёт(); i++) {
		const PPItem& m = элт[i];
		Ткст ll;
		ll << decode(m.тип, PP_DEFINES, "#defines ", PP_INCLUDE, "#include ",
		                     PP_USING, "using namespace ", PP_NAMESPACE, "namespace ",
		                     PP_NAMESPACE_END, "}", "");
		if(m.тип == PP_DEFINES)
			ll << m.segment_id;
		else
			ll << m.text;
		if(m.тип == PP_NAMESPACE)
			ll << " {";
		LOG(ll);
	}
	LOG("----- includes:");
	DUMPC(includes);
}

void InvalidatePPCache()
{
	{
		Стопор::Замок __(s_IncludePathMutex);
		s_IncludePath.очисть();
	}
	{
		Стопор::Замок __(s_FlatPPMutex);
		s_FlatPP.очисть();
	}
}

void PPSync(const Ткст& include_path)
{
	LLOG("* PPSync");
	bool update = false;
	{
		Стопор::Замок __(s_IncludePathMutex);
		if(s_Include_Path != include_path) {
			s_IncludePath.очисть();
			s_Include_Path = include_path;
			update = true;
		}
	}
	if(update) {
		Стопор::Замок __(s_FlatPPMutex);
		s_FlatPP.очисть();
	}
}

void InvalidateFileTimeCache()
{
	Стопор::Замок __(s_PathFileTimeMutex);
	s_PathFileTime.очисть();
}

void InvalidateFileTimeCache(const Ткст& path)
{
	LLOG("InvalidateFileTimeCache " << path);
	Стопор::Замок __(s_PathFileTimeMutex);
	s_PathFileTime.отлинкуйКлюч(path);
}

Время GetFileTimeCached(const Ткст& p)
{
	LTIMING("GetFileTimeCached");
	{
		Стопор::Замок __(s_PathFileTimeMutex);
		int q = s_PathFileTime.найди(p);
		if(q >= 0)
			return s_PathFileTime[q];
	}
	Время m = дайВремяф(p);
	{
		Стопор::Замок __(s_PathFileTimeMutex);
		s_PathFileTime.помести(p, m);
	}
	return m;
}

Ткст GetIncludePath()
{
	Стопор::Замок __(s_IncludePathMutex);
	return s_Include_Path;
}

Ткст GetIncludePath0(const char *s, const char *filedir)
{
	LTIMING("GetIncludePath0");
	while(IsSpace(*s))
		s++;
	int тип = *s;
	if(тип == '<' || тип == '\"' || тип == '?') {
		s++;
		Ткст имя;
		if(тип == '<') тип = '>';
		while(*s != '\r' && *s != '\n' && *s) {
			if(*s == тип) {
				if(тип == '\"') {
					Ткст фн = NormalizeSourcePath(имя, filedir);
					if(файлЕсть(фн))
						return фн;
				}
				Ткст p = дайФайлПоПути(имя, GetIncludePath(), false);
				if(p.дайСчёт())
					return NormalizeSourcePath(p);
				return Null;
			}
			имя.конкат(*s++);
		}
	}
	return Null;
}

Ткст GetIncludePath(const Ткст& s, const Ткст& filedir)
{
	LTIMING("GetIncludePath");
	Стопор::Замок __(s_IncludePathMutex);
	Ткст ключ;
	ключ << s << "#" << filedir;
	int q = s_IncludePath.найди(ключ);
	if(q >= 0)
		return s_IncludePath[q];
	LTIMING("GetIncludePath 2");
	Ткст p = GetIncludePath0(s, filedir);
	s_IncludePath.добавь(ключ, p);
	LLOG("GetIncludePath " << s << " " << filedir << ": " << p);
	return p;
}

void MakePP(const Индекс<Ткст>& paths)
{
	LLOG("MakePP " << paths);
	Вектор<Ткст> todo;
	Вектор<PPFile *> pp;
	for(int i = 0; i < paths.дайСчёт(); i++) {
		Ткст path = paths[i];
		PPFile& f = sPPfile.дайПомести(path);
		Время tm = GetFileTimeCached(path);
		if(f.filetime != tm) {
			f.filetime = tm;
			pp.добавь(&f);
			todo.добавь(path);
		}
	}
	соФор(todo.дайСчёт(), [&](int i) {
		ФайлВвод in(todo[i]);
		pp[i]->Parse(in);
	});
}

const PPFile& GetPPFile(const char *path)
{
	LTIMING("GetPPFile");
	LLOG("GetPPFile " << path);
	static PPFile zero;
	return sPPfile.дай(path, zero);
}

bool IsSameFile(const Ткст& f1, const Ткст& f2)
{
	return нормализуйПуть(f1) == нормализуйПуть(f2);
}

const FlatPP& GetFlatPPFile(const char *path, Индекс<Ткст>& visited)
{
	LTIMING("GetFlatPPFile");
	LLOG("GetFlatPPFile " << path);
	Стопор::Замок __(s_FlatPPMutex);
	int q = s_FlatPP.найди(path);
	if(q >= 0) {
		LLOG("From cache");
		return s_FlatPP[q];
	}
	FlatPP& fp = s_FlatPP.добавь(path);
	const PPFile& pp = GetPPFile(path);
	int n = visited.дайСчёт();
	visited.найдиДобавь(path);
	for(int i = 0; i < pp.элт.дайСчёт(); i++) {
		const PPItem& m = pp.элт[i];
		if(m.тип == PP_INCLUDE) {
			Ткст s = GetIncludePath(m.text, дайПапкуФайла(path));
			LLOG("#include " << m.text << " -> " << s);
			if(s.дайСчёт() && visited.найди(s) < 0) {
				visited.добавь(s);
				const FlatPP& pp = GetFlatPPFile(s, visited);
				for(int i = 0; i < pp.segment_id.дайСчёт(); i++)
					fp.segment_id.найдиДобавь(pp.segment_id[i]);
				for(int i = 0; i < pp.usings.дайСчёт(); i++)
					fp.usings.найдиДобавь(pp.usings[i]);
			}
		}
		else
		if(m.тип == PP_DEFINES)
			fp.segment_id.найдиДобавь(m.segment_id);
		else
		if(m.тип == PP_USING)
			fp.usings.найдиДобавь(m.text);
	}
	visited.обрежь(n);
	return fp;
}

const FlatPP& GetFlatPPFile(const char *path)
{
	Индекс<Ткст> visited;
	visited.добавь(path);
	return GetFlatPPFile(path, visited);
}

void GetAllMacros(Md5Stream& md5, const Ткст& id, Индекс<int>& segment_id)
{
	Вектор< Tuple2<int, int> > pos;
	Вектор<const CppMacro *> def;
	Ткст r;
	int q = sAllMacros.найди(id);
	while(q >= 0) {
		const PPMacro& m = sAllMacros[q];
		int si = segment_id.найди(m.segment_id);
		if(si >= 0) {
			pos.добавь(сделайКортеж(si, m.line));
			def.добавь(&m.macro);
		}
		q = sAllMacros.найдиСледщ(q);
	}
	IndexSort(pos, def);
	int n = def.дайСчёт();
	if(n) {
		md5.помести(&n, sizeof(int));
		md5.помести(id);
		for(int i = 0; i < n; i++)
			md5.помести(def[i]->md5, 16);
	}
}

static ВекторМап<Ткст, Ткст> s_namespace_macro;
static Индекс<Ткст> s_namespace_end_macro;

static Ткст sDefs;

void LoadPPConfig()
{
	for(int i = 0; i < sAllMacros.дайСчёт(); i++)
		if(sAllMacros[i].segment_id == 0 && !sAllMacros.отлинкован(i))
			sAllMacros.отлинкуй(i);

	s_namespace_macro.очисть();
	s_namespace_end_macro.очисть();

	ТкстПоток ss(sDefs);
	int linei = 0;
	while(!ss.кф_ли()) {
		Ткст l = ss.дайСтроку();
		try {
			СиПарсер p(l);
			if(p.сим('#')) {
				if(p.ид("define")) {
					CppMacro def;
					Ткст   id = def.Define(p.дайУк());
					if(id.дайСчёт()) {
						PPMacro m;
						m.segment_id = 0;
						m.line = linei;
						m.macro = def;
						sAllMacros.помести(id, m);
						if(findarg(обрежьОба(def.body), "}", "};") >= 0)
							s_namespace_end_macro.добавь(id);
						try {
							СиПарсер p(def.body);
							if(p.ид("namespace") && p.ид_ли()) {
								Ткст n = p.читайИд();
								if(p.сим('{') && p.кф_ли())
									s_namespace_macro.добавь(id, n);
							}
						}
						catch(СиПарсер::Ошибка) {}
					}
				}
			}
		}
		catch(СиПарсер::Ошибка) {}
		linei++;
	}
}

const ВекторМап<Ткст, Ткст>& GetNamespaceMacros()
{
	return s_namespace_macro;
}

const Индекс<Ткст>& GetNamespaceEndMacros()
{
	return s_namespace_end_macro;
}

void SetPPDefs(const Ткст& defs)
{
	sDefs = defs;
	LoadPPConfig();
}

void CleanPP()
{
	sAllMacros.очисть();
	sPPfile.очисть();
	s_PPserial = 0;
	LoadPPConfig();
}

void SerializePPFiles(Поток& s)
{
	int sPPserial = s_PPserial;
	s % sAllMacros % sPPfile % sPPserial;
	s_PPserial = sPPserial;
	if(s.грузится())
		LoadPPConfig();

#if 0
	if(s.грузится()) { _DBG_
		DDUMP(sPPfile.дайСчёт());
		DDUMP(sAllMacros.дайСчёт());
		DDUMP(sPPserial);
		
		Индекс<int> psegment;
		for(int i = 0; i < sPPfile.дайСчёт(); i++) {
			const PPFile& p = sPPfile[i];
			for(int j = 0; j < p.элт.дайСчёт(); j++)
				psegment.найдиДобавь(p.элт[j].segment_id);
		}
		DDUMP(psegment.дайСчёт());
			
		int n = 0; _DBG_
		Индекс<int> msegment;
		for(int i = 0; i < sAllMacros.дайСчёт(); i++) { _DBG_
			if(sAllMacros.отлинкован(i))
				n++;
			else
				msegment.найдиДобавь(sAllMacros[i].segment_id);
		}
		DLOG("UNLINKED " << n);
		DLOG("Segments " << msegment.дайСчёт());
	}
#endif
}

}
