#include "CppBase.h"
#include "Internal.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

#define LTIMING(x) // DTIMING(x)

namespace РНЦП {

const Массив<CppItem>& GetTypeItems(const CppBase& codebase, const Ткст& тип)
{
	static Массив<CppItem> sEmpty;
	int q = codebase.найди(тип);
	if(q < 0)
		return sEmpty;
	return codebase[q];
}

Ткст ParseTemplatedType(const Ткст& тип, Вектор<Ткст>& tparam)
{
	const char *s = тип;
	Ткст r;
	while(*s) {
		if(*s == '<') {
			s++;
			int lvl = 0;
			Ткст t;
			while(*s) {
				int c = *s++;
				if(c == ',' && lvl == 0) {
					tparam.добавь(t);
					t.очисть();
				}
				else {
					if(c == '>') {
						if(lvl == 0)
							break;
						lvl--;
					}
					if(c == '<')
						lvl++;
					t.конкат(c);
				}
			}
			tparam.добавь(t);
		}
		else
			r.конкат(*s++);
	}
	LLOG("ParseTemplatedType " << тип << " -> " << r);
	LDUMPC(tparam);
	return r;
}

Ткст ResolveTParam(const CppBase& codebase, const Ткст& тип, const Вектор<Ткст>& tparam)
{
	LLOG("ResolveTParam " << тип << ' ' << tparam);
	Ткст r;
	const char *s = тип;
	while(*s) {
		if(цифра_ли(*s)) {
			int i = *s++ - '0';
			if(i >= 0 && i < tparam.дайСчёт())
				r.конкат(tparam[i]);
		}
		else
		if(iscib(*s))
			while(iscid(*s))
				r.конкат(*s++);
		else
			r.конкат(*s++);
	}
	LLOG("Resolved " << тип << " -> " << r);
	const Массив<CppItem>& x = GetTypeItems(codebase, r);
	if(x.дайСчёт() && x[0].kind == TYPEDEF) {
		LLOG("является typedef " << x[0].qtype << ", unqualified " << x[0].тип << ", natural " << x[0].natural);
		Ткст h = x[0].qtype;
		if(h != тип && h != r)
			return ResolveTParam(codebase, h, tparam);
		return h;
	}
	return r;
}

void ResolveTParam(const CppBase& codebase, Вектор<Ткст>& тип, const Вектор<Ткст>& tparam)
{
	for(int i = 0; i < тип.дайСчёт(); i++)
		тип[i] = ResolveTParam(codebase, тип[i], tparam);
}

struct ExpressionTyper {
	const CppBase&        codebase;
	int                   scan_counter; // limit permutations
	Ткст                context_type;
	Ткст                usings;
	const Вектор<Ткст>& xp;
	Индекс<Ткст>         typeset;
	const ParserContext&  parser;

	Вектор<Ткст>        GetTypeBases(const Ткст& тип);
	Ткст                ResolveReturnType(const CppItem& m, const Вектор<Ткст>& tparam);

	void                  ExpressionType(bool isptr, const Ткст& ttype, int ii,
	                                     bool variable, bool can_shortcut_operator,
	                                     Индекс<Ткст>& visited_bases, int lvl);
	void                  ExpressionType(bool isptr, const Ткст& ttype, int ii, bool variable, int lvl);

	Индекс<Ткст>         ExpressionType();
	
	enum { MAX_COUNT = 1000 };

	ExpressionTyper(const CppBase& codebase, const ParserContext& parser, const Вектор<Ткст>& xp);
};

ExpressionTyper::ExpressionTyper(const CppBase& codebase, const ParserContext& parser, const Вектор<Ткст>& xp)
:	codebase(codebase), xp(xp), parser(parser)
{
	scan_counter = 0;
	context_type = parser.current_scope;
	usings = parser.context.namespace_using;
}

Вектор<Ткст> ExpressionTyper::GetTypeBases(const Ткст& тип)
{
	const Массив<CppItem>& n = GetTypeItems(codebase, тип);
	Ткст bases;
	for(int i = 0; i < n.дайСчёт(); i++) {
		const CppItem& im = n[i];
		if(im.IsType())
			bases << im.qptype << ';';
	}
	Индекс<Ткст> r;
	Вектор<Ткст> h = разбей(bases, ';');
	for(int i = 0; i < h.дайСчёт(); i++)
		r.найдиДобавь(h[i]);
	return r.подбериКлючи();
}

Ткст ExpressionTyper::ResolveReturnType(const CppItem& m, const Вектор<Ткст>& tparam)
{
	if(m.tparam.дайСчёт()) {
		int q = InScListIndex(m.qtype, m.tname);
		if(q >= 0 && q < tparam.дайСчёт())
			return tparam[q];
	}
	return m.qtype;
}

Вектор<Ткст> SplitTScope(const char *s)
{
	Вектор<Ткст> r;
	const char *b = s;
	while(*s) {
		if(*s == '<') {
			s++;
			int lvl = 0;
			while(*s) {
				int c = *s++;
				if(c == '>') {
					if(lvl == 0)
						break;
					lvl--;
				}
				if(c == '<')
					lvl++;
			}
		}
		else
		if(*s == ':') {
			if(s > b)
				r.добавь(Ткст(b, s));
			while(*s == ':')
				s++;
			b = s;
		}
		else
			s++;
	}
	if(s > b)
		r.добавь(Ткст(b, s));
	return r;
}

Ткст AddTParams(const Ткст& тип, const Ткст& ttype)
{ // РНЦП::Вектор::Foo, РНЦП::Вектор<РНЦП::Ткст>::Бар -> РНЦП::Вектор<РНЦП::Ткст>::Foo
	if(ttype.найди('<') < 0)
		return тип;
	Вектор<Ткст> t = SplitTScope(тип);
	Вектор<Ткст> tt = SplitTScope(ttype);
	int i = 0;
	Ткст r;
	while(i < t.дайСчёт() && i < tt.дайСчёт()) {
		int q = tt[i].найди('<');
		if(q < 0)
			q = tt[i].дайДлину();
		if(tt[i].середина(0, q) != t[i])
			break;
		if(i)
			r << "::";
		r << tt[i];
		i++;
	}
	while(i < t.дайСчёт()) {
		if(i)
			r << "::";
		r << t[i];
		i++;
	}
	LLOG("AddTParam " << тип << ", " << ttype << " -> " << r);
	return r;
}

void ExpressionTyper::ExpressionType(bool isptr, const Ткст& ttype, int ii,
                                     bool variable, bool can_shortcut_operator,
                                     Индекс<Ткст>& visited_bases, int lvl)
{
	LLOG("--- ExpressionType " << scan_counter << ", lvl " << lvl << ", ttype " << ttype << ", isptr " << isptr
	     << ", ii: " << ii << ", " << (ii < xp.дайСчёт() ? xp[ii] : "<end>"));
	if(++scan_counter >= MAX_COUNT || lvl > 100) // sort of ugly limitation of parsing permutations
		return;
	LDUMP(can_shortcut_operator);
	Вектор<Ткст> tparam;
	Ткст тип = ParseTemplatedType(ttype, tparam);
	int c0 = typeset.дайСчёт();
	const Массив<CppItem>& n = GetTypeItems(codebase, тип);
	LDUMP(тип);
	LDUMP(tparam);
	
	// STL/NTL is too much to parse for now, so until we have better expression resolver, let us play dirty
	static Индекс<Ткст> std_container;
	static Индекс<Ткст> std_pair_container;
	static Индекс<Ткст> std_container_iterator;
	static Индекс<Ткст> std_pair_container_iterator;
	static Индекс<Ткст> upp_container;
	static Индекс<Ткст> upp_container_iterator;
	static Индекс<Ткст> upp_map_container;
	static Индекс<Ткст> upp_map_container_iterator;
	static Индекс<Ткст> upp_map_container_key_iterator;
	ONCELOCK {
		Вектор<Ткст> a = разбей("array;vector;deque;forward_list;list;stack;queue;priority_queue;"
		                         "set;multiset;unordered_set;unordered_multiset", ';');
		for(int i = 0; i < a.дайСчёт(); i++) {
			std_container.добавь("std::" + a[i]);
			std_container_iterator.добавь("std::" + a[i] + "::iterator");
		}
		a = разбей("map;multimap;unordered_map;unordered_multimap", ';');
		for(int i = 0; i < a.дайСчёт(); i++) {
			std_container.добавь("std::" + a[i]);
			std_pair_container_iterator.добавь("std::" + a[i] + "::iterator");
		}
		a = разбей("ВекторМап;МассивМап;SortedVectorMap;SortedArrayMap", ';');
		for(int i = 0; i < a.дайСчёт(); i++) {
			upp_map_container.добавь("РНЦП::" + a[i]);
			upp_map_container_iterator.добавь("РНЦП::" + a[i] + "::iterator");
			upp_map_container_iterator.добавь("РНЦП::" + a[i] + "::Обходчик");
			upp_map_container_key_iterator.добавь("РНЦП::" + a[i] + "::KeyIterator");
		}
		a = разбей("Индекс;ArrayIndex;InVector;InArray;SortedIndex", ';');
		for(int i = 0; i < a.дайСчёт(); i++) {
			upp_container.добавь("РНЦП::" + a[i]);
			upp_container_iterator.добавь("РНЦП::" + a[i] + "::iterator");
			upp_container_iterator.добавь("РНЦП::" + a[i] + "::Обходчик");
		}
	}
	if(tparam.дайСчёт() > 0 && std_container_iterator.найди(тип) >= 0) {
		LLOG("# nasty iterator");
		typeset.очисть();
		ExpressionType(false, tparam[0], ii, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}
	if(tparam.дайСчёт() > 1 && std_pair_container_iterator.найди(тип) >= 0) {
		LLOG("# nasty pair iterator");
		typeset.очисть();
		ExpressionType(false, "std::pair<" + tparam[0] + "," + tparam[1] + ">",
		               ii, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}
	if(tparam.дайСчёт() > 0 && upp_container_iterator.найди(тип) >= 0) {
		LLOG("# РНЦП nasty iterator");
		typeset.очисть();
		ExpressionType(false, tparam[0], ii, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}
	if(tparam.дайСчёт() > 1 && upp_map_container_iterator.найди(тип) >= 0) {
		LLOG("# РНЦП map nasty iterator");
		typeset.очисть();
		ExpressionType(false, tparam[1], ii, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}
	if(tparam.дайСчёт() > 1 && upp_map_container_key_iterator.найди(тип) >= 0) {
		LLOG("# РНЦП map nasty ключ iterator");
		typeset.очисть();
		ExpressionType(false, tparam[0], ii, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}

	if(codebase.namespaces.найди(ttype) < 0 && ttype.дайСчёт()) // do not search for namespace typedefs
		for(int i = 0; i < n.дайСчёт(); i++)
			if(n[i].kind == TYPEDEF) {
				LLOG("typedef -> " << n[i].qtype);
				ExpressionType(false, AddTParams(ResolveTParam(codebase, n[i].qtype, tparam), ttype), ii, variable, can_shortcut_operator, visited_bases, lvl + 1);
				return;
			}
	if(ii >= xp.дайСчёт()) {
		LLOG("==== Final тип: " << ttype);
		typeset.найдиДобавь(ttype);
		return;
	}
	Ткст id = xp[ii];

	// ещё nasty tricks to make things work with containers (hopefully temporary)
	if((id == "begin" || id == "end") && std_container.найди(тип) >= 0) {
		LLOG("# nasty begin/end");
		typeset.очисть();
		ExpressionType(false, ttype + "::iterator", ii + 1, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}
	if(findarg(id, "begin", "end", "старт", "стоп") >= 0 &&
	   (upp_map_container.найди(тип) >= 0 || upp_container.найди(тип) >= 0)) {
		LLOG("# nasty РНЦП begin/end");
		typeset.очисть();
		ExpressionType(false, ttype + "::Обходчик", ii + 1, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}
	if(findarg(id, "KeyBegin", "KeyEnd") >= 0 && upp_map_container.найди(тип) >= 0) {
		LLOG("# nasty РНЦП KeyBegin/KeyEnd");
		typeset.очисть();
		ExpressionType(false, ttype + "::KeyIterator", ii + 1, variable, can_shortcut_operator, visited_bases, lvl + 1);
		scan_counter = MAX_COUNT;
		return;
	}
	
	int q = id.найдирек(':');
	if(q > 0 && id[q - 1] == ':') {
		тип = ResolveTParam(codebase, Qualify(codebase, ttype, id.середина(0, q - 1), usings), tparam);
		id = id.середина(q + 1);
	}
	if(id.найди('<') >= 0) // as in Single<Дисплей>
		id = ParseTemplatedType(id, tparam);
	LLOG("ExpressionType " << тип << " ii: " << ii << " id:" << id << " variable:" << variable);
	
	for(int i = 0; i < tparam.дайСчёт(); i++) // need to qualify template parameters
		tparam[i] = Qualify(codebase, context_type, tparam[i], usings);

	bool shortcut_oper = false;
	if(!iscid(*id) && *id != '.' && !isptr) {
		shortcut_oper = can_shortcut_operator;
		id = "operator" + id;
		LLOG("id as: " << id);
	}
	if(*id == '.' || (!variable && !iscid(*id))) {
		LLOG(". " << ttype);
		ExpressionType(isptr, ttype, ii + 1, false, lvl + 1);
		return;
	}
	LDUMP(id);
	for(int i = 0; i < n.дайСчёт(); i++) {
		const CppItem& m = n[i];
		if(m.имя == id) {
			Ткст t = AddTParams(ResolveReturnType(m, tparam), ttype);
			bool skipfnpars = m.IsCode() && ii + 1 < xp.дайСчёт() && xp[ii + 1] == "()";
			ExpressionType(m.isptr, ResolveTParam(codebase, t, tparam), ii + skipfnpars + 1,
			               m.IsData() && !m.isptr, lvl + 1);
		}
	}
	
	if(typeset.дайСчёт() == c0 && тип.дайСчёт()) { // try to find id in тип - Class::Method case
		Ткст type2 = ParseTemplatedType(тип, tparam);
		const Массив<CppItem>& n = GetTypeItems(codebase, type2);
		for(int i = 0; i < n.дайСчёт(); i++) {
			const CppItem& m = n[i];
			if(m.имя == id) {
				Ткст t = AddTParams(ResolveReturnType(m, tparam), тип);
				bool skipfnpars = m.IsCode() && ii + 1 < xp.дайСчёт() && xp[ii + 1] == "()";
				ExpressionType(m.isptr, ResolveTParam(codebase, t, tparam), ii + skipfnpars + 1,
				               m.IsData() && !m.isptr, lvl + 1);
			}
		}
	}

	if(typeset.дайСчёт() != c0 || пусто_ли(тип))
		return;
	Вектор<Ткст> base = GetTypeBases(тип);
	LDUMPC(base);
	ResolveTParam(codebase, base, tparam);
	LDUMPC(base);
	for(int i = 0; i < base.дайСчёт(); i++)
		if(visited_bases.найди(base[i]) < 0) {
			visited_bases.добавь(base[i]);
			ExpressionType(isptr, base[i], ii, variable, false, visited_bases, lvl + 1);
			if(typeset.дайСчёт() != c0)
				return;
		}

	if(shortcut_oper) {
		LLOG("Shortcut " << xp[ii] << ", ttype " << ttype);
		ExpressionType(false, ttype, ii + 1, false, lvl + 1);
	}
}

void ExpressionTyper::ExpressionType(bool isptr, const Ткст& ttype, int ii, bool variable, int lvl)
{
	Индекс<Ткст> bases;
	ExpressionType(isptr, ttype, ii, false, true, bases, 0);
}

Индекс<Ткст> ExpressionTyper::ExpressionType()
{
	LLOG("------------------------------------------------------");
	LLOG("**** ExpressionType " << xp);
	Ткст тип;
	if(xp.дайСчёт() == 0)
		return pick(typeset);
	if(xp[0] == "this") {
		LLOG("this: " << тип);
		ExpressionType(false, context_type, 1, false, 0);
		return pick(typeset);
	}
	int q = parser.local.найдиПоследн(xp[0]);
	if(q >= 0) {
		Ткст тип = Qualify(codebase, context_type, parser.local[q].тип, parser.context.namespace_using);
		LLOG("Found тип local: " << тип << " in scope: " << context_type << ", isptr: " << parser.local[q].isptr);
		ExpressionType(parser.local[q].isptr, тип, 1, !parser.local[q].isptr, 0);
		return pick(typeset);
	}
	if(context_type.дайСчёт()) {
		ExpressionType(false, context_type, 0, false, 0);
		if(typeset.дайСчёт())
			return pick(typeset);
	}
	if(xp.дайСчёт() >= 2 && xp[1] == "()") {
		Вектор<Ткст> usings = разбей(parser.context.namespace_using, ';');
		usings.добавь("");
		for(int i = 0; i < usings.дайСчёт(); i++) {
			Ткст qtype = Qualify(codebase, context_type, Merge("::", usings[i], xp[0]), parser.context.namespace_using);
			Вектор<Ткст> tparam;
			if(codebase.найди(ParseTemplatedType(qtype, tparam)) >= 0) {
				LLOG("является constructor " << qtype);
				ExpressionType(false, qtype, 2, false, 0);
			}
		}
		if(typeset.дайСчёт())
			return pick(typeset);
	}
	Вектор<Ткст> ns = parser.GetNamespaces();
	for(int i = 0; i < ns.дайСчёт(); i++)
		ExpressionType(false, ns[i], 0, false, 0);
	return pick(typeset);
}

Индекс<Ткст> GetExpressionType(const CppBase& codebase, const ParserContext& parser, const Вектор<Ткст>& xp)
{ // xp is a list of meaningful parts like "foo", "." , "Фн", "()", "->", "m", "[]"
	return ExpressionTyper(codebase, parser, xp).ExpressionType();
}

void SkipPars(СиПарсер& p)
{
	int lvl = 1;
	while(lvl && !p.кф_ли())
		if(p.сим('(') || p.сим('['))
			lvl++;
		else
		if(p.сим(')') || p.сим(']'))
			lvl--;
		else
			p.пропустиТерм();
}

Вектор<Ткст> MakeXP(const char *s)
{
	Вектор<Ткст> xp;
	try {
		СиПарсер p(s);
		while(!p.сим2_ли(':', ':') && !p.ид_ли() && !p.кф_ли())
			p.пропустиТерм();
		while(!p.кф_ли()) {
			Ткст id;
			if(p.сим2_ли(':', ':') || p.ид_ли()) {
				for(;;)
					if(p.сим2(':', ':'))
						id.конкат("::");
					else
					if(p.ид_ли())
						id.конкат(p.читайИд());
					else
						break;
				const char *s = p.дайУк();
				if(p.сим('<')) {
					int lvl = 1;
					while(lvl && !p.кф_ли()) {
						if(p.сим('<'))
							lvl++;
						else
						if(p.сим('>'))
							lvl--;
						else
							p.пропустиТерм();
					}
					while(s < p.дайУк()) {
						if((byte)*s > ' ')
							id.конкат(*s);
						s++;
					}
				}
				xp.добавь(id);
			}
			else
			if(p.сим('(')) {
				xp.добавь("()");
				SkipPars(p);
			}
			else
			if(p.сим('[')) {
				xp.добавь("[]");
				SkipPars(p);
			}
			else
			if(p.сим2('-', '>'))
				xp.добавь("->");
			else
			if(p.сим('.'))
				xp.добавь(".");
			else
				break;
		}
	}
	catch(СиПарсер::Ошибка) {}
	return xp;
}

};