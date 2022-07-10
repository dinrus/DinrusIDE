#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // DTIMING(x)

void Parser::выведиОш(const Ткст& e)
{
	err(дайСтроку(lex.Поз()), e);
#ifdef _ОТЛАДКА
	int i = 0;
	while(i < 40 && lex[i] != t_eof)
		i++;
	LLOG("Ошибка: (" << дайСтроку(lex.Поз()) << ") " << e << ", scope: " << current_scope <<
	     ", code:  " << какТкстСи(Ткст(lex.Поз(), lex.Поз(i))));
#endif
	throw Ошибка();
}

inline const char *bew(const char *s, const char *t)
{
	while(*s)
		if(*s++ != *t++)
			return NULL;
	return t;
}

// Ткст == Ткст comparison is likely faster than Ткст == const char * comparison
static Ткст s_operator("operator");
static Ткст s_virtual("virtual");
static Ткст s_inline("inline");
static Ткст s_static("static");

static inline bool sSpaces(Ткст& res, const char *& s)
{
	if((byte)*s <= ' ') {
		char c = *s++;
		if(c != '\2' && c != '\x1f') {
			res.конкат(' ');
			while((byte)*s <= ' ' && *s)
				s++;
		}
		return true;
	}
	return false;
}

Ткст FnItem(const char *s, const char *pname, const char *qname, const Ткст& имя, bool oper)
{ // Converts function natural text to (unqualified) элт
	Ткст res;
	while(*s && (byte)*s <= ' ') s++;
	while(*s) { // дай the имя of function into res
		while(*s && !iscid(*s) && *s != '~')
			if(*s == '[') { // пропусти MSVC attributes
				while(*s)
					if(*s++ == '[')
						break;
			}
			else
				s++;
		int lvl = 0;
		int plvl = 0;
		for(;;) {
			if(*s == '<' && plvl == 0 && !oper) { // resolve template params, like Фн<int, true>
				res.конкат(*s++);
				lvl++;
			}
			if(*s == '>' && plvl == 0 && !oper) {
				res.конкат(*s++);
				lvl--;
			}
			if(*s == '(' && lvl) {
				res.конкат(*s++);
				plvl++;
			}
			if(*s == ')') {
				res.конкат(*s++);
				plvl--;
			}
			if(iscid(*s) || *s == '~' || *s && lvl)
				res.конкат(*s++);
			else
				break;
		}
		if(res == s_operator) {
			while(*s && *s != '(') {
				if((byte)*s >= ' ')
					res.конкат(*s);
				s++;
			}
			break;
		}
		while(*s && (byte)*s <= ' ')
			s++;
		if(*s == '(' && (res == имя || res == '~' + имя))
			break;
		res.очисть();
	}
	bool wasid = false;
	while(*s) {
		const char *w = bew(qname, s);
		byte c = *s;
		if(w && w > s && !iscid(*w)) {
			if(iscid(*res.последний()))
				res.конкат(' ');
			res.конкат(имя);
			s = w;
			wasid = true;
		}
		else
		if(iscid(c)) {
			const char *b = s++;
			while(iscid(*s)) s++;
			Ткст q(b, s);
			if(q != s_virtual && q != s_inline && q != s_static && !InScList(q, pname)) {
				if(iscid(*res.последний()))
					res.конкат(' ');
				res.конкат(q);
			}
			else
				while((byte)*s <= ' ' && *s) s++;
			wasid = true;
		}
		else
		if(c == '=') {
			s++;
			int l = 0;
			int tl = 0;
			while(*s && !(l == 0 && (*s == ',' && tl == 0 || *s == ')'))) {
				if(*s == '(' || *s == '[')
					l++;
				else
				if(*s == ')' || *s == ']')
					l--;
				if(*s == '<') // we always consider < > to be template bracket, not operator here
					tl++;
				else
				if(*s == '>')
					tl--;
				s++;
			}
		}
		else
		if((byte)*s <= ' ') {
			s++;
			while((byte)*s <= ' ' && *s)
				s++;
		}
		else
		if(c == '[' && !wasid) { // пропусти MSVC attribute
			while(*s)
				if(*s++ == ']')
					break;
		}
		else
		if(c == '-' && s[1] == '>')
			break; // trailing return тип
		else {
			res.конкат(c);
			if(c == ',')
				wasid = false;
			s++;
		}
	}
	return res;
}

Ткст Purify(const char *s, const char *qname, const Ткст& имя) {
	Ткст res;
	while(*s && (byte)*s <= ' ') s++;
	bool wasid = false;
	bool firstpar = true;
	while(*s) {
		const char *w = bew(qname, s);
		if(w && w > s) {
			res.конкат(имя);
			s = w;
			wasid = true;
		}
		else
		if(iscid(*s)) {
			const char *b = s++;
			while(iscid(*s)) s++;
			Ткст q(b, s);
			if(q != s_virtual && q != s_inline && q != s_static)
				res.конкат(q);
			else
				while((byte)*s <= ' ' && *s) s++;
			wasid = true;
		}
		else
		if(*s == '[' && !wasid) { // skip MSVC attribute
			do {
				while(*s)
					if(*s++ == ']')
						break;
				while(*s == ' ' || *s == '\2' || *s == '\x1f')
					s++;
			}
			while(*s == '[');
		}
		else
		if(!sSpaces(res, s)) {
			if(*s == ',')
				wasid = false;
			if(*s == '(' && firstpar)
				wasid = firstpar = false;
			res.конкат(*s++);
		}
	}
	return обрежьПраво(res);
}

Ткст Purify(const char *s) {
	Ткст res;
	while(*s && (byte)*s <= ' ') s++;
	while(*s) {
		if(iscid(*s)) {
			const char *b = s++;
			while(iscid(*s)) s++;
			Ткст q(b, s);
			if(q != s_virtual && q != s_inline)
				res.конкат(q);
			else
				while((byte)*s <= ' ' && *s) s++;
		}
		else
		if(!sSpaces(res, s))
			res.конкат(*s++);
	}
	return обрежьПраво(res);
}

Ткст Gpurify(const char *s)
{
	Ткст res;
	while(*s)
		if(!sSpaces(res, s))
			res.конкат(*s++);
	return res;
}

void ScAdd(Ткст& s, const Ткст& a)
{
	if(a.пустой()) return;
	if(!s.пустой())
		s << ';';
	s << a;
}


void TpSkip(СиПарсер& p)
{
	int lvl = 0;
	for(;;) {
		if(lvl == 0 && (p.сим_ли(',') || p.сим_ли('>')) || p.кф_ли())
			break;
		if(p.сим('<'))
			lvl++;
		else
		if(p.сим('>'))
			lvl--;
		else
			p.пропустиТерм();
	}
}

Ткст Subst(const Ткст& s, const Вектор<Ткст>& tpar)
{
	if(tpar.дайСчёт() == 0)
		return s;
	Ткст r;
	СиПарсер p(s);
	while(!p.кф_ли()) {
		if(p.ид_ли()) {
			Ткст id = p.читайИд();
			int q = найдиИндекс(tpar, id);
			if(q >= 0)
				r << какТкст(q);
			else
				r << id;
		}
		else
			r << p.дайСим();
	}
	return r;
}

Ткст CleanTp(const Ткст& tp)
{
	int q = tp.найди('<');
	int w = tp.найдирек('>');
	if(q < 0 || w < 0) return tp;
	Ткст a = обрежьЛево(обрежьПраво(tp.середина(q + 1, w - q - 1)));
	const char *s = a;
	Ткст r;
	while(*s) {
		if(*s == ',') {
			r.конкат(';');
			s++;
			while(*s && *s <= ' ')
				s++;
		}
		else
			r.конкат(*s++);
	}
	return r;
}

Ткст Parser::Контекст::Dump() const
{
	return "Scopeing: " + scope;
}

static Ткст s_dblcln("::");

inline void ScopeCat(Ткст& scope, const Ткст& s)
{
	if(scope.дайСчёт())
		scope << s_dblcln;
	scope << s;
}

void Parser::Контекст::operator<<=(const Контекст& t)
{
	ns = t.ns;
	scope = t.scope;
	typenames = clone(t.typenames);
	tparam = clone(t.tparam);
	access = t.access;
	ctname = t.ctname;
	namespace_using = t.namespace_using;
}

bool Parser::IsNamespace(const Ткст& scope)
{
	int l = scope.дайСчёт();
	return memcmp(~context.ns, ~scope, l) == 0 && findarg(context.ns[l], '\0', ':') >= 0;
}

bool Parser::Ключ(int code)
{
	if(lex == code) {
		++lex;
		return true;
	}
	return false;
}

int  Parser::дайСтроку(const char *p)
{
	int pos = int(p - ~file.text);
	int l = 0;
	int h = file.linepos.дайСчёт();
	while(l < h) {
		int q = (l + h) / 2;
		if(file.linepos[q] < pos)
			l = q + 1;
		else
			h = q;
	}
	return l;
}

void Parser::Строка()
{
	int pos = int(lex.Поз() - ~file.text);
	while(line + 1 < file.linepos.дайСчёт() && file.linepos[line + 1] <= pos)
		line++;
}

void Parser::Check(bool b, const char *err)
{
	if(!b) выведиОш(err);
}

void Parser::CheckKey(int c)
{
	if(!Ключ(c)) выведиОш(фмт("Missing %c", c));
}

Ткст Parser::TemplateParams(Ткст& param)
{
	Ткст r;
	do {
		const char *pos = lex.Поз();
		CheckKey('<');
		int level = 1;
		Ткст id;
		bool gp = true;
		while(lex != t_eof) {
			if(lex.ид_ли() && gp)
				id = lex.дайИд();
			else
			if(Ключ(',')) {
				ScAdd(param, id);
				id.очисть();
				gp = true;
			}
			else
			if(Ключ('=')) {
				if(!id.пустой()) {
					ScAdd(param, id);
					id.очисть();
				}
				gp = false;
			}
			else
			if(Ключ('>')) {
				level--;
				if(level <= 0) {
					ScAdd(param, id);
					break;
				}
			}
			else
			if(Ключ(t_shr) && level >= 2) {
				level -= 2;
				if(level <= 0) {
					ScAdd(param, id);
					break;
				}
			}
			else
			if(Ключ('<'))
				level++;
			else
			if(Ключ('('))
				level++;
			else
			if(Ключ(')'))
				level--;
			else
				++lex;
		}
		MergeWith(r, ",", Ткст(pos, lex.Поз()));
	}
	while(Ключ(tk_template));
	return r;
}

Ткст Parser::TemplateParams()
{
	Ткст dummy;
	return TemplateParams(dummy);
}

Ткст Parser::TemplatePnames()
{
	Ткст pnames;
	TemplateParams(pnames);
	return pnames;
}

Ткст Parser::ReadOper(bool& castoper) {
	const char *p = lex.Поз();
	Ключ(tk_operator);
	int level = 0;
	if(Ключ('('))
		level++;
	for(;;) {
		if(lex == t_eof || level <= 0 && lex == '(') break;
		if(Ключ('(') || Ключ('[')) level++;
		else
		if(Ключ(')')  || Ключ(']')) level--;
		else
			++lex;
	}
	ТкстБуф r;
	bool spc = false;
	while(p < lex.Поз()) {
		if((byte)*p > ' ') {
			if(spc && iscid(*p)) {
				castoper = true;
				r.конкат(' ');
			}
			r.конкат(*p);
			spc = false;
		}
		else
			spc = true;
		p++;
	}
	return Ткст(r);
}

Ткст Parser::Имя(Ткст& имя, bool& castoper, bool& oper)
{
	Ткст s;
	if(Ключ(t_dblcolon)) {
		s = "::";
		имя = s;
	}
	Check(lex.ид_ли() || lex == tk_operator, "Имя expected");
	for(;;) {
		if(lex.ид_ли()) {
			имя << lex.Ид();
			s << lex.дайИд();
		}
		else {
			Ткст h = ReadOper(castoper);
			oper = true;
			имя << h;
			s << h;
			break;
		}
		if(lex == '<') // void Фн<byte>(); situation
			s << TemplateParams();
		if(Ключ(t_dblcolon)) {
			s << "::";
			имя << "::";
			if(Ключ('~')) {
				s << "~";
				имя << "~";
			}
		}
		else
			break;
	}
	return s;
}

Ткст Parser::Имя(bool& castoper, bool& oper)
{
	Ткст h;
	return Имя(h, castoper, oper);
}

Ткст Parser::Constant()
{
	const char *p = lex.Поз();
	const char *p1 = p;
	int level = 0;
	for(;;) {
		p1 = lex.Поз();
		if(lex == t_eof) break;
		if(level <= 0 && (lex == ',' || lex == ';' || lex == ')' || lex == '}' || lex == ']'))
			break;
		if(Ключ('(') || Ключ('[') || Ключ('{')) level++;
		else
		if(Ключ(')') || Ключ(']') || Ключ('}')) level--;
		else
			++lex;
	}
	return Ткст(p, p1);
}

Ткст Parser::TType()
{
	int q = найдиИндекс(context.tparam, lex[0]);
	if(q >= 0) return какТкст(q);
	return lex.Ид();
}

Ткст Parser::StructDeclaration(const Ткст& tn, const Ткст& tp)
{
	int t = lex.дайКод(); // t is now struct/class/union
	context.typenames.найдиДобавь(lex);
	Контекст cc;
	cc <<= context;
	СиПарсер p(tp);
	Вектор<Ткст> tpar;
	if(p.сим('<')) {
		while(!p.кф_ли() && !p.сим('>')) {
			if((p.ид("class") || p.ид("typename") || p.ид("struct")) && p.ид_ли()) {
				tpar.добавь(p.читайИд());
				context.tparam.добавь(lex.Ид(tpar.верх()));
			}
			else
				context.tparam.добавь(0);
			TpSkip(p);
			p.сим(',');
		}
	}
	if(Ключ(t_dblcolon))
		context.scope = Null;
	Ткст имя;
	Ткст new_scope = context.scope;
	if(lex.ид_ли())
		do {
			context.typenames.найдиДобавь(lex);
			имя = lex.дайИд(); // имя of structure
			if(lex == '<')
				имя << TemplateParams();
			ScopeCat(new_scope, имя);
		}
		while(Ключ(t_dblcolon));
	else {
		имя = AnonymousName();
		ScopeCat(new_scope, имя);
	}
	if(lex.ид_ли() && findarg(lex.Ид(), "override", "final") >= 0)
		++lex;
	if(lex.ид_ли() || lex == '*') { // struct My { struct My *p; }
		return имя;
	}
	context.scope = new_scope;
	context.access = t == tk_class ? PRIVATE : PUBLIC;
	if(tn.дайСчёт()) {
		if(context.ctname.дайСчёт())
			context.ctname << ';';
		context.ctname << tn;
	}
	Ткст nn;
	if(!tp.пустой())
		nn = "template " + tp + " ";
	Ткст ключ = (t == tk_class ? "class" : t == tk_union ? "union" : "struct");
	nn << ключ << ' ' << имя;
	LLOG("Struct "  << context.scope << " using " << context.namespace_using);
	CppItem& im = Элемент(context.scope, context.namespace_using, ключ, имя, lex != ';');
	im.kind = tp.пустой() ? STRUCT : STRUCTTEMPLATE;
	im.тип = имя;
	im.access = cc.access;
	im.tname = tn;
	im.ctname = context.ctname;
	im.tparam = CleanTp(tp);
	im.ptype.очисть();
	im.pname.очисть();
	im.param.очисть();
	if(lex == ';') { // TODO: perhaps could be united with following code
		context = pick(cc);
		im.natural = Gpurify(nn);
		SetScopeCurrent();
		return имя;
	}
	if(Ключ(':')) {
		nn << " : ";
		bool c = false;
		do {
			Ткст access = t == tk_class ? "private" : "public";
			bool virt = Ключ(tk_virtual);
			if(Ключ(tk_public)) access = "public";
			else
			if(Ключ(tk_protected)) access = "protected";
			else
			if(Ключ(tk_private)) access = "private";
			if(Ключ(tk_virtual) || virt) access << " virtual";
			Ткст h;
			bool dummy;
			Ткст n = Имя(h, dummy, dummy);
			ScAdd(im.pname, h);
			if(c)
				im.ptype << ';';
			im.ptype << Subst(n, tpar);
			ScAdd(im.param, access + ' ' + n);
			if(c)
				nn << ", ";
			nn << access + ' ' + n;
			c = true;
		}
		while(Ключ(','));
	}
	if(Ключ('{')) {
		struct_level++;
		ScopeBody();
		struct_level--;
		im.natural = Gpurify(nn);
		im.decla = true;
	}
	else
		if(пусто_ли(im.natural))
			im.natural = Gpurify(nn);
	context = pick(cc);
	SetScopeCurrent();
	return имя;
}

Ткст Parser::ReadType(Decla& d, const Ткст& tname, const Ткст& tparam)
{ // returns the имя of constructor
	if(findarg((int)lex, tk_struct, tk_class, tk_union) >= 0 && !d.isfriend) {
		d.тип = StructDeclaration(tname, tparam);
		return Ткст();
	}
	Ключ(tk_typename) || Ключ(tk_struct) || Ключ(tk_class) || Ключ(tk_union) || Ключ(tk_enum) || Ключ(tk_template);
	if(Ключ(tk_bool) || Ключ(tk_float) || Ключ(tk_double) || Ключ(tk_void))
		return Null;
	bool зн = Ключ(tk_signed) || Ключ(tk_unsigned);
	if(Ключ(tk_long)) {
		Ключ(tk_long);
		Ключ(tk_int) || Ключ(tk_double);
		return Null;
	}
	if(Ключ(tk_short)) {
		Ключ(tk_unsigned);
		Ключ(tk_int);
		return Null;
	}
	if(Ключ(tk_int) || Ключ(tk_char) ||
	   Ключ(tk___int8) || Ключ(tk___int16) || Ключ(tk___int32) || Ключ(tk___int64) || Ключ(tk___int128) ||
	   Ключ(tk_char16_t) || Ключ(tk_char32_t)) return Null;
	if(зн) return Null;
	const char *p = lex.Поз();
	bool cs = false;
	Индекс<int> cix;
	if(Ключ(tk_decltype) && Ключ('(')) {
		const char *p = lex.Поз();
		int lvl = 1;
		for(;;) {
			if(lex == t_eof)
				break;
			if(lex == '(')
				lvl++;
			else
			if(lex == ')' && --lvl == 0) {
				d.тип = "@" + Ткст(p, lex.Поз());
				++lex;
				break;
			}
			++lex;
		}
	}
	else
	if(Ключ(tk_auto))
		d.тип = "*";
	else {
		if(Ключ(t_dblcolon))
			d.тип << "::";
		Ключ(tk_typename) || Ключ(tk_template);
		Check(lex.ид_ли(), "Имя expected");
		while(lex.ид_ли()) {
			d.тип << TType();
			if(cix.найди(lex) >= 0)
				cs = true;
			else
				cix.добавь(lex);
			++lex;
			if(lex == '<')
				d.тип << TemplateParams();
			if(Ключ(t_dblcolon)) {
				d.тип << "::";
				if(Ключ('~'))
					cs = true;
				Ключ(tk_typename) || Ключ(tk_template);
			}
			else
				break;
		}
	}
	return cs ? Ткст(p, lex.Поз()) : Ткст();
}

void Parser::Qualifier(bool override_final)
{
	for(;;)
		if(Ключ(tk_const) || Ключ(tk_volatile) || Ключ(tk_constexpr) || Ключ(tk_thread_local) || VCAttribute())
			;
		else
		if(Ключ(tk_noexcept)) {
			if(Ключ('(')) {
				int lvl = 0;
				while(lex != t_eof && lex != ';' && !(lvl == 0 && Ключ(')')))
					if(Ключ('('))
						lvl++;
					else
					if(Ключ(')'))
						lvl--;
					else
						++lex;
			}
		}
		else
		if(override_final && lex.ид_ли() && findarg(lex.Ид(), "override", "final") >= 0)
			++lex;
		else
		if(Ключ(tk_throw) || Ключ(tk_alignas)) {
			while(lex != t_eof && lex != ';' && !Ключ(')'))
				++lex;
		}
		else
		if(lex[0] == '=' && findarg(lex[1], tk_delete, tk_default) >= 0) {
			++lex;
			++lex;
		}
		else
			break;
}

void Parser::Elipsis(Decl& d)
{
	Decl& q = d.param.добавь();
	q.имя = "...";
	if(lex.ид_ли()) // bool emplace(_Args&&... __args);
		++lex;
	CheckKey(')');
}

void Parser::ParamList(Decl& d) {
	if(!Ключ(')'))
		for(;;) {
			if(Ключ(t_elipsis)) {
				Elipsis(d);
				break;
			}
			else {
				Массив<Parser::Decl> decl = Declaration(false, false, Null, Null);
				if(decl.дайСчёт()) {
					d.param.добавь() = pick(decl.верх());
					if(dobody) { // put arguments to the list of local variables
						Decl& p = d.param.верх();
						Local& l = local.добавь(p.имя);
						l.тип = p.тип;
						l.isptr = p.isptr;
						l.line = line + 1;
					}
				}
			}
			if(Ключ(t_elipsis)) {
				Elipsis(d);
				break;
			}
			if(Ключ(')')) break;
			CheckKey(',');
		}
}

int Parser::RPtr()
{
	int n = 0;
	int tlevel = 0;
	for(;;) {
		int t = lex[n];
		if(t == '*') return n + 1;
		if(t == '<') {
			tlevel++;
			n++;
		}
		else
		if(t == '>') {
			tlevel--;
			n++;
		}
		else
		if(t == t_shr && tlevel >= 2) {
			tlevel -= 2;
			n++;
		}
		else
		if(t == t_dblcolon || lex.ид_ли(n) || t == ',' && tlevel > 0)
			n++;
		else
			return 0;
	}
}

void Parser::EatInitializers()
{
	if(Ключ(':')) {
		int lvl = 0;
		for(;;) {
			if(lex == t_eof)
				break;
			if(lvl == 0) {
				Qualifier(false);
				if(lex == '{')
					break;
				if(lex.ид_ли() && lex[1] == '{') { // : X{123} { case
					lvl++;
					++lex;
					++lex;
				}
			}
			else
				if(lex == '{')
					lvl++;
			if(lex == '(')
				lvl++;
			if(lex == ')' || lex == '}')
				lvl--;
			++lex;
		}
	}
}

void Parser::Declarator(Decl& d, const char *p)
{
	int n = RPtr();
	if(n) {
		while(n--) lex.дай();
		Declarator(d, p);
		d.isptr = true;
		return;
	}
	if(Ключ('&') || Ключ(t_and) || Ключ(tk_const) || Ключ(tk_volatile)) { // t_and is r-значение here
		Declarator(d, p);
		return;
	}
	if(Ключ('(')) {
		Declarator(d, p);
		if(d.isptr)
			d.nofn = true;
		CheckKey(')');
	}
//	if(lex == tk_operator)
//		d.имя = ReadOper();
//	else
	if(lex.ид_ли() || lex == t_dblcolon || lex == tk_operator) {
		d.имя = Имя(d.castoper, d.oper);
		if(lex == ':' && lex[1] == t_integer) { // Bitfield, like 'unsigned x:5'
			++lex;
			++lex;
		}
	}
	if(Ключ('(')) {
		if(inbody || (lex < 256 || lex == tk_true || lex == tk_false)
		   && lex != ')' && lex != '[' && lex != t_dblcolon) {
			int level = 0;
			for(;;) {
				if(lex == t_eof) break;
				if(Ключ('(')) level++;
				else
				if(Ключ(')')) {
					if(--level < 0) break;
				}
				else
					++lex;
			}
			return;
		}
		else {
			d.function = !d.nofn;
			ParamList(d);
			p = lex.Поз();
			Qualifier(true);

			if(d.function && Ключ(t_arrow)) { // C++11 trailing return тип
				d.тип.очисть();
				ReadType(d, Null, Null);
			}
			
			if(filetype == FILE_C && lex != '{' && lex != ';') // K&R style function header
				while(lex != '{' && lex != t_eof)
					++lex;
		}
	}
	if(*d.тип == '*') // C++11 auto declaration
		d.тип = ResolveAutoType();
	else
		EatInitializers();
	while(Ключ('[')) {
		d.isptr = true;
		int level = 1;
		while(level && lex != t_eof) {
			if(Ключ('[')) level++;
			else
			if(Ключ(']')) level--;
			else
				++lex;
		}
	}
	if(Ключ('=') || (inbody && lex == '(')) { // TODO: добавь C++11 initializers here (?)
		int level = 0;
		int tlevel = 0;
		for(;;) {
			TryLambda();
			if(lex == t_eof  || lex == ';'
			   || level == 0 && ((lex == ',' && tlevel == 0) || lex == ')'))
				break;
			if(Ключ('<')) // we ignore < > as operators, always consider them template bracket
				tlevel++;
			else
			if(Ключ('>'))
				tlevel--;
			else
			if(Ключ('(') || Ключ('{'))
				level++;
			else
			if(Ключ(')') || Ключ('}'))
				level--;
			else
				++lex;
		}
	}
}

Parser::Decl& Parser::финиш(Decl& d, const char *s)
{
	d.natural = Ткст(s, lex.Поз());
	return d;
}

bool Parser::IsParamList(int q)
{
	return true;
}

void Parser::ReadMods(Decla& d)
{
	for(;;) {
		if(Ключ(tk_static))
			d.s_static = true;
		else
		if(Ключ(tk_extern))
			d.s_extern = true;
		else
		if(Ключ(tk_register))
			d.s_register = true;
		else
		if(Ключ(tk_mutable))
			d.s_mutable = true;
		else
		if(Ключ(tk_explicit))
			d.s_explicit = true;
		else
		if(Ключ(tk_virtual))
			d.s_virtual = true;
		else
		if(!(Ключ(tk_inline) || Ключ(tk_force_inline) || Ключ(tk_never_inline) || Ключ(tk___inline) || VCAttribute()))
			break;
	}
}

Массив<Parser::Decl> Parser::Declaration0(bool l0, bool more, const Ткст& tname, const Ткст& tparam)
{
	Массив<Decl> r;
	Decla d;
	const char *p = lex.Поз();
	if(Ключ(tk_friend))
		d.isfriend = true;
	ReadMods(d);
	Qualifier();
	if(l0) {
		if(lex == tk_SKYLARK && lex[1] == '(' && lex.ид_ли(2)) {
			++lex;
			++lex;
			Decl& a = r.добавь();
			a.имя = lex.дайИд();
			a.function = true;
			a.natural = Ткст().конкат() << "void " << a.имя << "(Http& http)";
			Decl& p = a.param.добавь();
			p.имя = "http";
			p.тип = "Http";
			p.natural = "Http& http";
			Ключ(',');
			lex.дайТекст();
			Ключ(')');
			return r;
		}
		else
		if((lex == tk_RPC_METHOD || lex == tk_RPC_GMETHOD) && lex[1] == '(' && lex.ид_ли(2)) {
			++lex;
			++lex;
			Decl& a = r.добавь();
			a.имя = lex.дайИд();
			a.function = true;
			a.natural = Ткст().конкат() << "void " << a.имя << "(RpcData& rpc)";
			Decl& p = a.param.добавь();
			p.имя = "rpc";
			p.тип = "RpcData";
			p.natural = "RpcData& rpc";
			if (Ключ(','))
				lex.дайТекст();
			Ключ(')');
			return r;
		}
	}
	bool isdestructor = Ключ('~');
	if(l0 && context.typenames.найди(lex) >= 0 && lex[1] == '(' && lex.ид_ли()) {
		Decl& a = r.добавь();
		a.имя = lex.дайИд();
		a.isdestructor = isdestructor;
		a.function = true;
		a.istructor = true;
		++lex;
		ParamList(a);
		Qualifier();
		a.natural = Ткст(p, lex.Поз());
		EatInitializers();
		return r;
	}
	if(lex == tk_operator) {
		Decl& a = r.добавь();
		(Decla&)a = d;
		a.имя = ReadOper(a.castoper);
		Ключ('(');
		ParamList(a);
		Qualifier();
		a.function = true;
		a.natural = Ткст(p, lex.Поз());
		a.oper = true;
		return r;
	}
	Ткст st = ReadType(d, tname, tparam);
	if(!lex.IsGrounded()) // 'static' etc.. can be after тип too... (but not allow it on start of line)
		ReadMods(d);
	if(!st.пустой()) {
		Decl& a = r.добавь();
		int q = st.найди('~');
		if(q >= 0)
			st.удали(q, 1);
		a.имя = st;
		a.isdestructor = q >= 0;
		a.function = true;
		a.istructor = true;
		if(Ключ('('))
			ParamList(a);
		Qualifier();
		a.natural = Ткст(p, lex.Поз());
		EatInitializers();
		return r;
	}
	Ткст natural1 = Ткст(p, lex.Поз());
	if(lex != ';') // struct/class declaration without defining variable
		do {
			const char *p1 = lex.Поз();
			Decl& a = r.добавь();
			(Decla&)a = d;
			Declarator(a, p);
			if(a.castoper)
				a.имя = фильтруй(natural1, CharFilterNotWhitespace) + a.имя;
			a.natural = natural1 + Ткст(p1, lex.Поз());
			p = lex.Поз();
		}
		while(more && Ключ(','));
	return r;
}

Массив<Parser::Decl> Parser::Declaration(bool l0, bool more, const Ткст& tname, const Ткст& tparam)
{
	if(Ключ(tk_typedef)) {
		Массив<Decl> r = Declaration0(false, true, tname, tparam);
		for(int i = 0; i < r.дайСчёт(); i++) {
			r[i].type_def = true;
			r[i].natural = "typedef " + r[i].natural;
		}
		return r;
	}
	const char *b = lex.Поз();
	if(Ключ(tk_using) && lex.ид_ли()) {
		Ткст имя = lex.дайИд();
		Ключ('=');
		Массив<Decl> r;
		Decl& d = r.добавь();
		ReadType(d, tname, tparam);
		while(Ключ('*') || Ключ(tk_volatile) || Ключ(tk_const));
		d.имя = имя;
		d.natural = Ткст(b, lex.Поз());
		d.type_def = true;
		return r;
	}
	return Declaration0(l0, more, tname, tparam);
}

Ткст Parser::Tparam(int& q)
{
	if(lex[q] != '<')
		return Null;
	const char *p = lex.Поз(q);
	int level = 1;
	q++;
	while(lex[q] != t_eof && level) {
		if(lex[q] == '<')
			level++;
		if(lex[q] == '>')
			level--;
		else
		if(lex[q] == t_shr && level >= 2)
			level -= 2;
		q++;
	}
	return Ткст(p, lex.Поз(q));
}

Ткст NoTemplatePars(const Ткст& s)
{
	int q = s.найди('<');
	return q >= 0 ? s.середина(0, q) : s;
}

bool Parser::VCAttribute()
{
	if(lex[0] == '[') // пропусти Visual C++ attribute
		for(;;) {
			if(lex[0] == ']') {
				++lex;
				return true;
			}
			if(lex[0] == t_eof)
				return false;
			++lex;
		}
	return false;
}

void Parser::SetScopeCurrent()
{
	current_scope = context.scope;
}

CppItem& Parser::Элемент(const Ткст& scope, const Ткст& using_namespace, const Ткст& элт,
                      const Ткст& имя, bool impl)
{
	current_scope = scope;
	if(dobody)
		current = CppItem();
	current_key = элт;
	current_name = имя;
	CppItem& im = dobody ? current : base->дайДобавь(current_scope).добавь();
	im.элт = элт;
	im.имя = имя;
	im.file = filei;
	im.line = line + 1;
	im.impl = impl;
	im.filetype = filetype;
	im.using_namespaces = using_namespace;
	LLOG("нов элт " << line + 1 << "    " << scope << "::" << элт);
	return im;
}

void Parser::AddMacro(int lineno, const Ткст& macro, int kind)
{
	Ткст имя;
	const char *s = macro;
	while(*s && iscid(*s))
		имя.конкат(*s++);
	CppItem& im = Элемент("", "", macro, имя);
	im.kind = kind;
	im.line = lineno;
	im.access = PUBLIC;
}

CppItem& Parser::Элемент(const Ткст& scope, const Ткст& using_namespace, const Ткст& элт,
                      const Ткст& имя)
{
	Ткст h = Purify(элт);
	CppItem& im = Элемент(scope, using_namespace, h, имя, false);
	im.natural = h;
	return im;
}

void Parser::Resume(int bl)
{
	for(;;) {
		if(lex == t_eof || lex.GetBracesLevel() == bl && lex == ';')
			break;
		++lex;
	}
}

void Parser::ScopeBody()
{
	int bl = lex.GetBracesLevel();
	while(!Ключ('}')) {
		if(lex == t_eof)
			выведиОш("Unexpected end of file");
		try {
			Do();
		}
		catch(Ошибка) {
			Resume(bl);
			Ключ(';');
		}
	}
}

Ткст Parser::AnonymousName()
{
	int lvl = 0;
	for(int i = 0; lex[i] != t_eof; i++) {
		if(lex[i] == '{') lvl++;
		else
		if(lex[i] == '}')
			if(--lvl == 0) {
				if(lex.ид_ли(i + 1))
					return "." + lex.Ид(i + 1);
				break;
			}
	}

	dword x[4];
	x[0] = случ();
	x[1] = случ();
	x[2] = случ();
	x[3] = случ();
	
	return "@" + Base64Encode(Ткст((const char *)&x, sizeof(x))) + "/" + title;
}

void Parser::AddNamespace(const Ткст& n, const Ткст& имя)
{
	Ткст h = "namespace " + n;
	CppItem& m = Элемент(n, Null, h, имя);
	m.kind = NAMESPACE;
	m.natural = h;
}

bool Parser::Scope(const Ткст& tp, const Ткст& tn) {
	if(Ключ(tk_namespace)) {
		Check(lex.ид_ли(), "Expected имя of namespace");
		Ткст имя = lex.дайИд();
		LLOG("namespace " << имя);
		namespace_info << ';' << имя;
		Контекст c0;
		c0 <<= context;
		int struct_level0 = struct_level;
		ScopeCat(context.scope, имя);
		ScopeCat(context.ns, имя);
		AddNamespace(context.scope, имя);
		if(Ключ('{')) {
			Контекст cc;
			cc <<= context;
			while(!Ключ('}')) {
				if(lex == t_eof)
					выведиОш("Unexpected end of file");
				try {
					Do();
				}
				catch(Ошибка) {
					if(struct_level0)
						throw;
					context <<= cc;
					struct_level = struct_level0;
					LLOG("---- Recovery to namespace level");
					++lex;
					lex.SkipToGrounding();
					lex.ClearBracesLevel();
					LLOG("Grounding skipped to " << дайСтроку(lex.Поз()));
				}
				catch(Lex::Grounding) {
					LLOG("---- Grounding to namespace level");
					context <<= cc;
					struct_level = struct_level0;
					lex.ClearBracesLevel();
					lex.ClearBody();
				}
			}
		}
		LLOG("стоп namespace");
		Ключ(';');
		context <<= c0;
		SetScopeCurrent();
		namespace_info << ";}";
		return true;
	}
	return false;
}

CppItem& Parser::Фн(const Decl& d, const Ткст& templ, bool body,
                    const Ткст& tname, const Ткст& tparam)
{
	Ткст param;
	Ткст pname;
	Ткст ptype;
	for(int i = 0; i < d.param.дайСчёт(); i++) {
		const Decla& p = d.param[i];
		ScAdd(param, p.natural);
		if(i)
			ptype << ';';
		ptype << p.тип;
		ScAdd(pname, p.имя);
	}
	Ткст nn0;
	Ткст nm = d.имя;
	int q;
	if(d.castoper) {
		q = d.имя.найдирек(' ');
		q = q > 0 ? d.имя.найдирек(':', q) : d.имя.найдирек(':');
	}
	else
		q = d.имя.найдирек(':');
	if(q >= 0) {
		nm = d.имя.середина(q + 1);
		if(q > 0)
			nn0 = d.имя.середина(0, q - 1);
	}
	Ткст элт = FnItem(d.natural, pname, d.имя, nm, d.oper);
	Ткст scope = context.scope;
	Ткст nn;
	const char *s = nn0;
	int l = 0;
	while(*s) {
		if(*s == '<')
			l++;
		else
		if(*s == '>')
			l--;
		else
		if(l == 0)
			nn.конкат(*s);
		s++;
	}
	s = nn;
	while(*s == ':') s++;
	if(*s)
		ScopeCat(scope, s);
	CppItem& im = Элемент(scope, context.namespace_using, элт, nm, body);
	im.natural.очисть();
	if(!пусто_ли(templ)) {
		im.natural = обрежьПраво(Gpurify(templ)) + ' ';
		im.at = im.natural.дайДлину();
	}
	im.natural << Purify(d.natural, d.имя, nm);
	im.kind = templ.дайСчёт() ? IsNamespace(scope) ? FUNCTIONTEMPLATE
	                                           : d.s_static ? CLASSFUNCTIONTEMPLATE
	                                                        : INSTANCEFUNCTIONTEMPLATE
	                           : d.istructor ? (d.isdestructor ? DESTRUCTOR : CONSTRUCTOR)
	                                         : d.isfriend ? INLINEFRIEND
	                                                      : IsNamespace(scope) ? FUNCTION
	                                                                      : d.s_static ? CLASSFUNCTION
	                                                                                   : INSTANCEFUNCTION;
	im.param = param;
	im.pname = pname;
	im.ptype = ptype;
	im.access = context.access;
	im.virt = d.s_virtual;
	im.тип = d.тип;
	im.decla = true;
	im.tname = tname;
	im.tparam = tparam;
	im.ctname = context.ctname;
	LLOG("FnItem: " << scope << "::" << элт << ", natural: " << im.natural
	                << ", ctname: " << im.ctname);
	return im;
}

void Parser::Enum(bool vars)
{
	Ткст имя;
	if(lex.ид_ли())
		имя = lex.дайИд();
	while(lex != '{' && lex != ';' && lex != t_eof)
		++lex;
	if(Ключ('{')) {
		for(;;) {
			Строка();
			Ткст val;
			Check(lex.ид_ли(), "Expected identifier");
			Ткст id = lex.дайИд();
			CppItem& im = Элемент(context.scope, context.namespace_using, id, id);
			im.natural = "enum ";
			if(!пусто_ли(имя))
				im.natural << имя << ' ';
			im.natural << id;
			if(Ключ('='))
				im.natural << " = " << Constant();
			im.kind = ENUM;
			im.access = context.access;
			Ключ(',');
			if(Ключ('}')) break;
		}
	}
	while(!Ключ(';')) {
		if(lex.ид_ли()) {
			if(vars) { // typedef имя ignored here
				Ткст scope = context.scope;
				Ткст имя = lex.дайИд();
				CppItem& im = Элемент(scope, context.namespace_using, имя, имя);
				im.natural = "enum " + имя;
				im.access = context.access;
				im.kind = IsNamespace(scope) ? VARIABLE : INSTANCEVARIABLE;
			}
			++lex;
		}
		else
		if(lex == ',' || lex == '*')
			++lex;
		else
			break;
	}
	Ключ(';');
	SetScopeCurrent();
}

bool Parser::UsingNamespace()
{
	if(lex == tk_using && !(lex.ид_ли(1) && lex[2] == '=')) {
		++lex;
		if(Ключ(tk_namespace))
			while(lex.ид_ли()) {
				Вектор<Ткст> h = разбей(context.namespace_using, ';');
				Ткст u;
				do {
					u << lex.дайИд();
					if(Ключ(t_dblcolon))
						u << "::";
				}
				while(lex.ид_ли());
				if(найдиИндекс(h, u) < 0)
					h.добавь(u);
				context.namespace_using = Join(h, ";");
				Ключ(',');
			}
		while(!Ключ(';') && lex != t_eof)
			++lex;
		namespace_info << ";using " << context.namespace_using;
		return true;
	}
	return false;
}

bool Parser::IsEnum(int i)
{
	for(;;) {
		if(lex[i] == '{')
			return true;
		else
		if(lex.ид_ли(i) || lex[i] == ':' ||
		   findarg(lex[i], tk_bool, tk_signed, tk_unsigned, tk_long, tk_int, tk_short,
		                       tk_char, tk___int8, tk___int16, tk___int32, tk___int64,
		                       tk_char16_t, tk_char32_t) >= 0)
			i++;
		else
			return false;
	}
}

void Parser::ClassEnum(const Ткст& clss)
{
	context.typenames.найдиДобавь(lex);
	Контекст cc;
	cc <<= context;
	Ткст имя = lex.дайИд();
	Ткст new_scope = context.scope;
	ScopeCat(new_scope, имя);
	context.scope = new_scope;
	context.access = PUBLIC;
	Ткст ключ = "class";
	LLOG("enum class "  << context.scope << " using " << context.namespace_using);
	CppItem& im = Элемент(context.scope, context.namespace_using, ключ, имя, lex != ';');
	im.kind = STRUCT;
	im.natural = clss + " enum " + имя;
	im.тип = имя;
	im.access = cc.access;
	im.tname.очисть();
	im.ctname.очисть();
	im.tparam.очисть();
	im.ptype.очисть();
	im.pname.очисть();
	im.param.очисть();
	Enum(true);
	context = pick(cc);
	SetScopeCurrent();
}

void Parser::DoNamespace()
{
	if(Ключ('{'))
		while(!Ключ('}')) {
			if(lex == t_eof)
				выведиОш("Unexpected end of file");
			Do();
		}
	Ключ(';');
}

void Parser::Do()
{
	LLOG("Do, scope: " << current_scope);
	if(lex.IsGrounded() && struct_level)
		throw Lex::Grounding();
	Строка();
	if(UsingNamespace())
		;
	else
	if(Ключ(tk_static_assert))
		while(lex != t_eof && lex != ';')
			++lex;
	else
	if(lex == tk_inline && lex[1] == tk_namespace) { // for now, inline namespace is simply ignored
		while(lex != t_eof && lex != '{')
			++lex;
		DoNamespace();
	}
	else
	if(Ключ(';')) // 'empty' declaration, result of some ignores
		;
	else
	if(lex == tk_extern && lex[1] == tk_template) { // skip 'extern template void Foo<char>();'
		while(lex != t_eof) {
			if(lex == ';') {
				++lex;
				break;
			}
			++lex;
		}
	}
	else
	if(Ключ(tk_extern) && lex == t_string) { // extern "C++" kind
		++lex;
		DoNamespace();
	}
	else
	if(Ключ(tk_template)) {
		if(lex.ид_ли() || lex == tk_class && lex.ид_ли(1)) {
			Ключ(tk_class);
			for(;;) {
				if(lex.ид_ли())
					lex.дайИд();
				else
				if(!Ключ(t_dblcolon))
					break;
			}
			TemplateParams();
			Ключ(';');
		}
		else {
			Ткст tnames;
			Ткст tparam = TemplateParams(tnames);
			if(lex == tk_using) { //C++11 template alias template <...> using ИД =
				Массив<Decl> r = Declaration(true, true, tnames, tparam);
				for(int i = 0; i < r.дайСчёт(); i++) {
					Decla& d = r[i];
					if(d.type_def) {
						Ткст scope = context.scope;
						ScopeCat(scope, d.имя);
						CppItem& im = Элемент(scope, context.namespace_using, "typedef", d.имя);
						im.natural = Purify(d.natural);
						im.тип = d.тип;
						im.access = context.access;
						im.kind = TYPEDEF;
					}
				}
			}
			else {
				if(!Scope(tparam, tnames)) {
					Массив<Decl> r = Declaration(true, true, tnames, tparam);
					bool body = lex == '{';
					for(int i = 0; i < r.дайСчёт(); i++) {
						Decl& d = r[i];
						if(!d.isfriend && d.function)
							Фн(d, "template " + tparam + ' ', body, tnames, tparam);
					}
					EatBody();
					Ключ(';');
				}
				EatBody();
			}
		}
	}
	else
	if(lex == tk_enum && IsEnum(1)) {
		++lex;
		Enum(true);
	}
	else
	if(lex == tk_enum && lex[1] == tk_class && IsEnum(2)) {
		++lex;
		++lex;
		ClassEnum("class"); // like enum class Foo { A, B }
	}
	else
	if(lex == tk_enum && lex[1] == tk_struct && IsEnum(2)) {
		++lex;
		++lex;
		ClassEnum("struct"); // like enum struct Foo { A, B }
	}
	else
	if(lex == tk_typedef && lex[1] == tk_enum && IsEnum(2)) {
		++lex;
		++lex;
		Enum(false);
	}
	else
	if(!Scope(Ткст(), Ткст())) {
		if(Ключ(tk_public)) {
			context.access = PUBLIC;
			Ключ(':');
		}
		else
		if(Ключ(tk_private)) {
			context.access = PRIVATE;
			Ключ(':');
		}
		else
		if(Ключ(tk_protected)) {
			context.access = PROTECTED;
			Ключ(':');
		}
		else {
			Массив<Decl> r = Declaration(true, true, Null, Null);
			bool body = lex == '{';
			for(int i = 0; i < r.дайСчёт(); i++) {
				Decl& d = r[i];
				if(d.имя.дайСчёт()) {
					if(d.function) {
						if(!d.isfriend)
							Фн(d, Null, body, Ткст(), Ткст());
					}
					else {
						Ткст h = d.natural;
						int q = h.найди('=');
						if(q >= 0)
							h = обрежьПраво(h.середина(0, q));
						Ткст scope = context.scope;
						if(d.type_def)
							ScopeCat(scope, d.имя);
						Ткст имя = d.имя;
						int member_type = d.s_static ? CLASSVARIABLE : INSTANCEVARIABLE;
						q = d.имя.найдирек("::");
						if(q >= 0) { // class variable definition like: int Ктрл::циклСобытий;
							ScopeCat(scope, d.имя.середина(0, q));
							current_scope = scope; // temporary until ';'
							имя = d.имя.середина(q + 2);
							member_type = CLASSVARIABLE;
						}
						CppItem& im = Элемент(scope, context.namespace_using,
						                   d.isfriend ? "friend class"
						                   : d.type_def ? "typedef"
						                   : имя, имя);
						im.natural = Purify(h);
						im.тип = d.тип;
						im.access = context.access;
						im.kind = d.isfriend ? FRIENDCLASS :
						          d.type_def ? TYPEDEF :
						          IsNamespace(scope) ? VARIABLE :
						          member_type;
						if(im.IsData())
							im.isptr = d.isptr;
					}
				}
			}
			EatBody();
			if(Ключ(';'))
				SetScopeCurrent(); // need to be after ';' to make class variable definitions work
		}
	}
}

void  Parser::Do(Поток& in, CppBase& _base, int filei_, int filetype_,
                 const Ткст& title_, Событие<int, const Ткст&> _err,
                 const Вектор<Ткст>& typenames,
                 const Вектор<Ткст>& namespace_stack,
                 const Индекс<Ткст>& namespace_using)
{
	LTIMING("Parser::Do");
	LLOG("= C++ Parser ==================================== " << title_ << " " << namespace_stack << ", dobody: " << dobody);
	base = &_base;
	err = _err;
	filei = filei_;
	filetype = filetype_;
	title = title_;
	lpos = 0;
	line = 0;

	context.namespace_using = Join(namespace_using.дайКлючи(), ";");
	
	Ткст n;
	for(int i = 0; i < namespace_stack.дайСчёт(); i++) {
		MergeWith(n, "::", namespace_stack[i]);
		AddNamespace(n, namespace_stack[i]);
	}

	file = PreProcess(in, *this);
	lex.иниц(~file.text);

	while(lex != t_eof)
		try {
			try {
				current_scope.очисть();
				context.access = PUBLIC;
				context.typenames.очисть();
				context.tparam.очисть();
				context.ns = context.scope = Join(namespace_stack, "::");
				inbody = false;
				struct_level = 0;
				for(int i = 0; i < typenames.дайСчёт(); i++)
					context.typenames.добавь(lex.Ид(typenames[i]));
				Do();
			}
			catch(Ошибка) {
				if(lex.IsBody()) {
					LLOG("---- Recovery to next ';'");
					Resume(lex.GetBracesLevel());
					Ключ(';');
				}
				else {
					LLOG("---- Recovery to file level");
					++lex;
					lex.SkipToGrounding();
					lex.ClearBracesLevel();
					LLOG("Grounding skipped to " << дайСтроку(lex.Поз()));
				}
			}
		}
		catch(Lex::Grounding) {
			LLOG("---- Grounding to file level");
			lex.ClearBracesLevel();
			lex.ClearBody();
		}
}

Вектор<Ткст> ParserContext::GetNamespaces() const
{
	Вектор<Ткст> ns;
	Вектор<Ткст> h = разбей(current_scope, ':');
	while(h.дайСчёт()) {
		ns.добавь(Join(h, "::"));
		h.сбрось();
	}
	ns.приставь(разбей(context.namespace_using, ';'));
	ns.добавь(""); // добавь global namespace too
	return ns;
}

}
