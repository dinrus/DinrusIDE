#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // DTIMING(x)

bool DoQualify(ScopeInfo& nf, const Ткст& тип, const Ткст& usings, Ткст& qt);

bool Qualify0(ScopeInfo& nf, const Ткст& тип, const Ткст& usings, Ткст& qt)
{ // Qualify single тип based on scoping information
	const Вектор<Ткст>& nd = nf.GetScopes(usings);
	if(nd.дайСчёт()) {
		LTIMING("First test");
		qt = nd[0] + тип;
		if(nf.base.найди(qt) >= 0)
			return true;
	}
	if(nf.GetScope() >= 0) {
		int q = тип.найдирек(':');
		if(q > 0) {
			LTIMING("Qualifying qualification");
			ScopeInfo hnf(nf);
			hnf.NoBases();
			Ткст qn;
			Ткст qs = тип.середина(0, q - 1);
			if(цифра_ли(*qs)) {
				qt = тип;
				return true;
			}
			if(DoQualify(hnf, qs, usings, qn)) {
				Ткст tp = тип.середина(q + 1);
				if(nf.base.найди(qn) >= 0) {
					qt = qn + "::" + tp;
					return true;
				}
				int scopei = nf.base.найди(qn);
				if(scopei >= 0) {
					ScopeInfo nnf(nf.base, scopei);
					const Вектор<Ткст>& bs = nnf.GetBases();
					for(int i = 0; i < bs.дайСчёт(); i++) {
						qt = bs[i] + tp;
						if(nf.base.найди(qt) >= 0)
							return true;
					}
				}
			}
			if(nf.base.найди(qs) >= 0) {
				qt = qs;
				return true;
			}
			qt = тип;
			if(nf.base.найди(qt) >= 0) // e.g. std::string
				return true;
			qt = тип.середина(q + 1);
			return true;
		}
		else {
			LTIMING("Bases");
			const Вектор<Ткст>& bs = nf.GetBases();
			for(int i = 0; i < bs.дайСчёт(); i++) {
				qt = bs[i] + тип;
				if(nf.base.найди(qt) >= 0)
					return true;
			}
		}
	}
	if(тип[0] != ':') {
		LTIMING("Testing scopes");
		for(int i = 1; i < nd.дайСчёт(); i++) {
			qt = nd[i] + тип;
			if(nf.base.найди(qt) >= 0)
				return true;
		}
	}
	int q = тип.найди(':');
	if(q < 0)
		return false;
	return Qualify0(nf, тип.середина(q + 1), usings, qt);
}

bool DoQualify(ScopeInfo& nf, const Ткст& тип, const Ткст& usings, Ткст& qt)
{
	LTIMING("Qualify");
	int q = nf.cache.найди(тип);
	if(q >= 0) {
		qt = nf.cache[q];
		return true;
	}
	LTIMING("Qualify0");
	if(!Qualify0(nf, тип, usings, qt))
		return false;
	nf.cache.добавь(тип, qt);
	return true;
}

Ткст DoQualify(ScopeInfo& nf, const Ткст& тип, const Ткст& usings)
{
	Ткст qt;
	return DoQualify(nf, тип, usings, qt) ? qt : тип;
}

static Ткст s_int("int");
static Ткст s_void("void");
static Ткст s_double("double");
static Ткст s_char("char");
static Ткст s_float("float");
static Ткст s_long("long");
static Ткст s_const("const");
static Ткст s_struct("struct");
static Ткст s_class("class");
static Ткст s_unsigned("unsigned");

inline void Qualify(Ткст& r, ScopeInfo& nf, const char *b, const char *s, const Ткст& usings)
{
	Ткст тип(b, s);
	if(тип.дайСчёт() == 0 || тип == s_const ||
	   тип == s_int || тип == s_double || тип == s_char ||
	   тип == s_long || тип == s_unsigned || тип == s_struct || тип == s_class ||
	   тип == s_float) {
		r << тип;
		return;
	}
	r << DoQualify(nf, тип, usings);
}

Ткст QualifyIds(ScopeInfo& nf, const Ткст& k, const Ткст& usings, bool all)
{
	LTIMING("QualifyIds");
	Ткст r;
	const char *s = k;
	Вектор<Ткст> empty;
	while(*s) {
		int c = *s;
		if(c == ':') {
			const char *b = s++;
			while(*s == ':' || iscid(*s)) s++;
/*			if(all) {
				if(iscid(*r.последний()))
					r << ' ';
				ScopeInfo nnf(nf.GetScope(), nf.base);
				Qualify(r, nnf, b, s, usings);
			}
			else*/
				r.конкат(b, s);
		}
		else
		if(iscid(c)) {
			if(iscid(*r.последний()))
				r << ' ';
			if(s[0] == 'c' && s[1] == 'o' && s[2] == 'n' && s[3] == 's' && s[4] == 't' && !iscid(s[5])) {
				r << s_const;
				s += 5;
			}
			else
			if(s[0] == 'm' && s[1] == 'u' && s[2] == 't' && s[3] == 'a' && s[4] == 'b' && s[5] == 'l' && s[6] == 'e' && !iscid(s[7])) {
				r << "mutable";
				s += 7;
			}
			else
			if(s[0] == 'v' && s[1] == 'o' && s[2] == 'l' && s[3] == 'a' && s[4] == 't' && s[5] == 'i' && s[6] == 'l' && s[7] == 'e' && !iscid(s[8])) {
				r << "volatile";
				s += 8;
			}
			else {
				const char *b = s++;
				while(*s == ':' || iscid(*s)) s++;
				if(all)
					Qualify(r, nf, b, s, usings);
				else
					r.конкат(b, s);
			}
		}
		else {
			if(c == '(')
				all = true;
			if(c != ' ')
				r.конкат(c);
			s++;
		}
	}
	return r;
}

Ткст Qualify(const CppBase& base, const Ткст& scope, const Ткст& тип, const Ткст& usings)
{
	if(тип.начинаетсяС("::"))
		return тип.середина(2);
	ScopeInfo nf(base, scope);
	return QualifyIds(nf, тип, usings, true);
}

Ткст QualifyKey(const CppBase& base, const Ткст& scope, const Ткст& тип, const Ткст& usings)
{
	ScopeInfo nf(base, scope);
	return QualifyIds(nf, тип, usings, false);
}

void QualifyTypes(CppBase& base, const Ткст& scope, CppItem& m)
{
	ScopeInfo nf(base, scope);
	m.qtype = QualifyIds(nf, m.тип, m.using_namespaces, true);
	m.qptype = QualifyIds(nf, m.ptype, m.using_namespaces, true);
}

void QualifyPass0(CppBase& base)
{ // Move scopes - solve: namespace X { struct C { void Foo(); }; using namespace X; void C::Foo() {}
	Вектор<int> remove_scope;
	for(int ni = 0; ni < base.дайСчёт(); ni++) {
		Массив<CppItem>& n = base[ni];
		ScopeInfo nf(base);
		Ткст scope = base.дайКлюч(ni);
		Ткст usings;
		int    toscopei = -1;
		Вектор<int> remove_item;
		for(int i = 0; i < n.дайСчёт(); i++) {
			CppItem& m = n[i];
			if(m.qualify && m.impl) {
				if(usings != m.using_namespaces) {
					usings = m.using_namespaces;
					toscopei = -1;
					Вектор<Ткст> h = разбей(m.using_namespaces, ';');
					for(int i = 0; i < h.дайСчёт(); i++) {
						Ткст ns = h[i] + "::" + scope;
						toscopei = base.найди(ns);
						if(toscopei >= 0)
							break;
					}
				}
				if(toscopei >= 0 && toscopei != ni) {
					base[toscopei].добавь(m);
					remove_item.добавь(i);
				}
			}
		}
		n.удали(remove_item);
		if(scope.дайСчёт() && n.дайСчёт() == 0)
			remove_scope.добавь(ni);
	}
	base.удали(remove_scope);
}

void QualifyPass1(CppBase& base)
{ // Qualify types
	LTIMING("QualifyPass1");
	for(int ni = 0; ni < base.дайСчёт(); ni++) {
		Массив<CppItem>& n = base[ni];
		ScopeInfo nf(base, ni);
		for(int i = 0; i < n.дайСчёт(); i++) {
			CppItem& m = n[i];
			if(m.kind == NAMESPACE)
				base.namespaces.найдиДобавь(base.дайКлюч(ni));
			if(m.IsType() && m.qualify) {
				m.qualify = false;
				m.qtype = QualifyIds(nf, m.тип, m.using_namespaces, true); // тип of элт, now qualified (probaly already was)
				m.qptype = QualifyIds(nf, m.ptype, m.using_namespaces, true); // base classes
				m.qitem = m.элт; // имя of тип (struct) элт is already qualified
			}
		}
	}
}

void QualifyPass2(CppBase& base)
{ // Qualify function parameters
	LTIMING("QualifyPass2");
	for(int ni = 0; ni < base.дайСчёт(); ni++) {
		Массив<CppItem>& n = base[ni];
		ScopeInfo nf(base, ni);
		for(int i = 0; i < n.дайСчёт(); i++) {
			CppItem& m = n[i];
			if(m.uname.дайСчёт() == 0 && m.имя.дайСчёт())
				m.uname = взаг(m.имя);
			if(!m.IsType() && m.qualify) {
				m.qualify = false;
				m.qtype = QualifyIds(nf, m.тип, m.using_namespaces, true);
				m.qptype = QualifyIds(nf, m.ptype, m.using_namespaces, true);
				m.qitem = m.IsCode() ? QualifyIds(nf, m.элт, m.using_namespaces, false)
				                     : m.элт;
			}
		}
	}
}

void Qualify(CppBase& base)
{
	Md5Stream md5;
	Вектор<int> no = GetSortOrder(base.дайКлючи());
	for(int q = 0; q < base.дайСчёт(); q++) {
		int ni = no[q];
		md5 << base.дайКлюч(ni);
		const Массив<CppItem>& n = base[ni];
		for(int i = 0; i < n.дайСчёт(); i++) {
			const CppItem& m = n[i];
			if(m.IsType())
				md5 << ';' << m.ptype;
		}
		md5 << '\n';
	}
	Ткст c5 = md5.FinishString();
	if(c5 != base.types_md5) { // set of types changed, requalify everything
		base.types_md5 = c5;
		for(int ni = 0; ni < base.дайСчёт(); ni++) {
			Массив<CppItem>& n = base[ni];
			for(int i = 0; i < n.дайСчёт(); i++)
				n[i].qualify = true;
		}
	}
	base.namespaces.очисть();
	QualifyPass0(base);
	QualifyPass1(base);
	QualifyPass2(base);
}

}
