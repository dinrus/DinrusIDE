#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

#define LTIMING(x)  // DTIMING(x)
#define LLOG(x)     // DLOG(x)

void Cpp::ParamAdd(Вектор<Ткст>& param, const char *s, const char *e)
{
	while(s < e && (byte)*s <= ' ') s++;
	while(e > s && (byte)*(e - 1) <= ' ') e--;
	Ткст h;
	while(s < e) {
		if((byte)*s <= ' ') {
			h.конкат(' ');
			s++;
			while(s < e && (byte)*s <= ' ')
				s++;
		}
		else
		if(*s == '\"' || *s == '\'') {
			const char *q = SkipString(s);
			h.конкат(Ткст(s, q));
			s = q;
		}
		else
			h.конкат(*s++);
	}
	param.добавь(h);
}

Ткст Cpp::расширь(const char *s)
{
	ТкстБуф r;
	const char *l0 = s;
	while(*s) {
		if(incomment) {
			if(s[0] == '*' && s[1] == '/') {
				incomment = false;
				s += 2;
				r.конкат("*/");
			}
			else
				r.конкат(*s++);
		}
		else
		if(iscib(*s)) {
			LTIMING("расширь ИД");
			const char *bid = s;
			s++;
			while(iscid(*s))
				s++;
			Ткст id(bid, s);
			LTIMING("расширь ID2");
			if(notmacro.найди(id) < 0) {
				const PPMacro *pp = macro.найдиПоследнУк(id);
				int segmenti = pp ? segment_id.найди(pp->segment_id) : -1;
				const CppMacro *m = FindMacro(id, segment_id, segmenti);
				if(!m && pp)
					m = &pp->macro;
				if(m && m->IsUndef())
					m = NULL;
				if(m) {
					LTIMING("расширь macro");
					Вектор<Ткст> param;
					bool function_like = false;
					const char *s0 = s;
					if(m->param.дайСчёт()) {
						while(*s && (byte)*s <= ' ')
							s++;
						if(*s == '(') {
							function_like = true;
							s++;
							const char *b = s;
							int level = 0;
							for(;;)
								if(*s == ',' && level == 0) {
									ParamAdd(param, b, s);
									s++;
									b = s;
								}
								else
								if(*s == ')') {
									s++;
									if(level == 0) {
										ParamAdd(param, b, s - 1);
										break;
									}
									level--;
								}
								else
								if(*s == '(') {
									s++;
									level++;
								}
								else
								if(*s == '\0') { // macro use spread into more lines
									if(bid == l0) // begin of line
										prefix_macro = bid;
									else
										prefix_macro = Ткст(' ', 1) + bid; // do not want to emit grounding in body
									return Ткст(r);
								}
								else
								if(*s == '\"' || *s == '\'')
									s = SkipString(s);
								else
									s++;
						}
					}
					if(!!m->param.дайСчёт() == function_like) {
						int ti = notmacro.дайСчёт();
						Вектор<Ткст> eparam;
						eparam.устСчёт(param.дайСчёт());
						for(int i = 0; i < param.дайСчёт(); i++)
							eparam[i] = расширь(param[i]);
						notmacro.добавь(id);
						id = '\x1f' + расширь(m->расширь(param, eparam)); // \x1f is info for Pre that there was a macro expansion
						notmacro.обрежь(ti);
					}
					else
						s = s0;
				}
				else
					notmacro.добавь(id);
			}
			r.конкат(id);
		}
		else
		if(*s == '\"') {
			const char *e = SkipString(s);
			r.конкат(s, e);
			s = e;
		}
		else
		if(s[0] == '/' && s[1] == '*') {
			incomment = true;
			s += 2;
			r.конкат("/*");
		}
		else
		if(s[0] == '/' && s[1] == '/') {
			r.конкат(s);
			break;
		}
		else
			r.конкат(*s++);
	}
	return Ткст(r);
}

void Cpp::DoFlatInclude(const Ткст& header_path)
{
	LTIMING("DoFlatInclude");
	LLOG("Flat include " << header_path);
	if(header_path.дайСчёт()) {
		md5.помести(header_path);
		const FlatPP& pp = GetFlatPPFile(header_path);
		LLOG("DoFlatInclude " << header_path << ", " << pp.segment_id.дайСчёт() << " segments");
		for(int i = 0; i < pp.segment_id.дайСчёт(); i++)
			segment_id.найдиДобавь(pp.segment_id[i]);
		for(int i = 0; i < pp.usings.дайСчёт(); i++) {
			namespace_using.найдиДобавь(pp.usings[i]);
			LLOG(" Flat usings " << pp.usings[i]);
			md5.помести('$');
			md5.помести(pp.usings[i]);
		}
	}
}

#define IGNORE_ELSE

void Cpp::Do(const Ткст& sourcefile, Поток& in, const Ткст& currentfile, bool get_macros)
{
	LLOG("Cpp::Do " << sourcefile << ", current: " << currentfile);
	if(visited.найди(currentfile) >= 0 || visited.дайСчёт() > 20000)
		return;
	visited.добавь(currentfile);
	Ткст current_folder = дайПапкуФайла(currentfile);
	bool notthefile = sourcefile != currentfile;
	if(notthefile || get_macros) {
		LTIMING("DO2");
		const PPFile& pp = GetPPFile(currentfile);
		for(int i = 0; i < pp.элт.дайСчёт() && !done; i++) {
			const PPItem& m = pp.элт[i];
			if(m.тип == PP_DEFINES) {
				LTIMING("PP_DEFINES");
				if(notthefile) // if getting macros, we are interested in included macros only
					segment_id.найдиДобавь(m.segment_id);
			}
			else
			if(m.тип == PP_INCLUDE) {
				Ткст s = GetIncludePath(m.text, current_folder);
				if(s.дайСчёт()) {
					if(notthefile && (s == sourcefile || IncludesFile(s, sourcefile))) {
						LLOG("Include IN " << s);
						md5.помести(s);
						Do(sourcefile, in, s, get_macros);
					}
					else {
						LLOG("Include FLAT " << s);
						DoFlatInclude(s);
					}
				}
			}
			else
			if(m.тип == PP_NAMESPACE) {
				namespace_stack.добавь(m.text);
				LLOG("pp namespace " << m.text << " " << namespace_stack);
			}
			else
			if(m.тип == PP_NAMESPACE_END && namespace_stack.дайСчёт()) {
				namespace_stack.сбрось();
				LLOG("pp end namespace " << namespace_stack);
			}
			else
			if(m.тип == PP_USING) {
				namespace_using.найдиДобавь(m.text);
				md5.помести('$');
				md5.помести(m.text);
			}
		}
		if(sourcefile != currentfile)
			return;
	}

	md5.помести('@');
	md5.помести(Join(namespace_stack, ";"));
	
	done = true;
	
	if(get_macros)
		return;
	
	if(in.подбери() == 0xef) { // пропусти UTF-8 BOM
		int64 pos = in.дайПоз();
		in.дай();
		if(in.дай() != 0xbb || in.дай() != 0xbf)
			in.перейди(pos); // Was not UTF-8 BOM after all
	}

	LTIMING("расширь");
	incomment = false;
	prefix_macro.очисть();
	ТкстБуф result;
	result.очисть();
	result.резервируй(16384);
	int lineno = 0;
	bool incomment = false;
	bool do_pp = true;
	int segment_serial = 0;
	segment_id.добавь(--segment_serial);
#ifdef IGNORE_ELSE
	int ignore_else = 0;
#endif
	while(!in.кф_ли()) {
		Ткст l = prefix_macro + in.дайСтроку();
		Ткст ll = обрежьЛево(l);
		if(ll.начинаетсяС("//$")) { // Do not remove assist++ parser directives
			result.конкат(l + "\n");
			do_pp = decode(ll[3], '+', true, '-', false, do_pp);
			continue;
		}
		prefix_macro.очисть();
		lineno++;
		int el = 0;
		while(*l.последний() == '\\' && !in.кф_ли()) {
			el++;
			l.обрежь(l.дайДлину() - 1);
			l.конкат(in.дайСтроку());
		}
		RemoveComments(l, incomment);
		СиПарсер p(l);
		if(p.сим('#')) {
			if(do_pp) {
				if(p.ид("define")) {
					result.конкат(l + "\n");
					CppMacro m;
					Ткст id = m.Define(p.дайУк());
					if(id.дайСчёт()) {
						PPMacro& pp = macro.добавь(id);
						pp.macro = m;
						pp.segment_id = segment_serial;
						notmacro.обрежь(kw.дайСчёт());
					}
				}
				else
				if(p.ид("undef")) {
					result.конкат(l + "\n");
					if(p.ид_ли()) {
						segment_id.добавь(--segment_serial);
						PPMacro& m = macro.добавь(p.читайИд());
						m.segment_id = segment_serial;
						m.macro.SetUndef();
						notmacro.обрежь(kw.дайСчёт());
						segment_id.добавь(--segment_serial);
					}
				}
				else
				if(p.ид("if") || p.ид("ifdef") || p.ид("ifndef")) // FLAGTEST support
					result.конкат(l + "\n");
				else {
					result.конкат('\n');
				#ifdef IGNORE_ELSE
					if(ignore_else) {
						if(p.ид("if") || p.ид("ifdef") || p.ид("ifndef"))
							ignore_else++;
						else
						if(p.ид("endif"))
							ignore_else--;
					}
					else {
						if(p.ид("else") || p.ид("elif"))
							ignore_else = 1;
					}
				#endif
					if(p.ид("include")) {
						LTIMING("расширь include");
						Ткст s = GetIncludePath(p.дайУк(), current_folder);
						DoFlatInclude(s);
						segment_id.добавь(--segment_serial);
					}
				}
			}
			else
				result.конкат('\n');
		}
		else {
			LTIMING("расширь expand");
		#ifdef IGNORE_ELSE
			if(ignore_else)
				result.конкат('\n');
			else
		#endif
				result.конкат(расширь(l) + "\n");
		}
		while(el--)
			result.конкат("\n");
	}
	output = result;
}

Индекс<Ткст> Cpp::kw;

bool Cpp::Preprocess(const Ткст& sourcefile, Поток& in, const Ткст& currentfile,
                     bool get_macros)
{
	LLOG("===== Preprocess " << sourcefile << " <- " << currentfile);
	LTIMING("Cpp::Preprocess");
	macro.очисть();
	macro.резервируй(1000);
	segment_id.очисть();
	segment_id.добавь(0);
	segment_id.резервируй(100);
	
	ONCELOCK {
		const char **h = CppKeyword();
		while(*h) {
			kw.добавь(*h);
			h++;
		}
	}
	notmacro = clone(kw);

	done = false;
	incomment = false;
	output.очисть();
	visited.очисть();
	Do(нормализуйПуть(sourcefile), in, нормализуйПуть(currentfile), get_macros);
	if(!done)
		output.очисть();
	return done;
}

void GetAllMacros(Md5Stream& md5, const Ткст& id, Индекс<int>& segment_id);

Ткст Cpp::GetDependeciesMd5(const Вектор<Ткст>& m)
{
	Ткст r;
	Md5Stream md5;
	md5.помести(Join(namespace_stack, ";"));
	md5.помести('@');
	md5.помести(Join(namespace_using.дайКлючи(), ";"));
	md5.помести('@');
	for(int i = 0; i < m.дайСчёт(); i++)
		GetAllMacros(md5, m[i], segment_id);
	return md5.FinishString();
}

}
