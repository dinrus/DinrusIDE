#include "CppBase.h"
#include "Internal.h"

#define LLOG(x)  // DLOG(x)

namespace РНЦП {

void Parser::Locals(const Ткст& тип)
{
	Строка();
	Массив<Parser::Decl> d = Declaration(true, true, Null, Null);
	for(int i = 0; i < d.дайСчёт(); i++) {
		Local& l = local.добавь(d[i].имя);
		l.тип = *тип == '*' ? d[i].тип : тип;
		l.isptr = d[i].isptr;
		l.line = line + 1;
		LLOG("== Local variable " << d[i].имя << ": " << l.тип);
	}
}

bool Parser::TryDecl()
{ // attempt to interpret code as local variable declaration
	for(;;) {
		if(findarg(lex[0], tk_static, tk_const, tk_register, tk_volatile) >= 0)
			++lex;
		else
		if(!VCAttribute())
			break;
	}
	int t = lex[0];
	int q = 0;
	if(t == tk_int || t == tk_bool || t == tk_float || t == tk_double || t == tk_void ||
	   t == tk_long || t == tk_signed || t == tk_unsigned || t == tk_short ||
	   t == tk_char || t == tk___int8 || t == tk___int16 || t == tk___int32 || t == tk___int64 ||
	   t == tk_auto) {
		q++;
		if(findarg(lex[q], tk_long, tk_int, tk_char) >= 0)
			q++;
		if(findarg(lex[q], tk_long, tk_int, tk_char) >= 0)
			q++;
		while(findarg(lex[q], '*', '&', t_and, tk_const) >= 0) // t_and is r-значение here
			q++;
		if(!lex.ид_ли(q))
			return false;
		static Ткст aut("*"), empty;
		Locals(t == tk_auto ? aut : empty);
		return true;
	}
	Ткст тип;
	if(t == tk_decltype && lex[q + 1] == '(') {
		q += 2;
		int q0 = q;
		int lvl = 1;
		for(;;) {
			if(lex[q] == t_eof)
				break;
			if(lex[q] == '(')
				lvl++;
			else
			if(lex[q] == ')' && --lvl == 0) {
				Locals("@" + Ткст(lex.Поз(q0), lex.Поз(q)));
				return true;
			}
			++q;
		}
		return false;
	}
	if(lex[q] == t_dblcolon) {
		тип << "::";
		q++;
	}
	if(lex.ид_ли(q)) {
		тип << lex.Ид(q++);
		тип << Tparam(q);
	}
	else
		return false;
	while(lex[q] == t_dblcolon) {
		тип << "::";
		if(lex.ид_ли(++q))
			тип << lex.Ид(q++);
		else
			return false;
		тип << Tparam(q);
	}
	while(lex[q] == '*' || lex[q] == '&' || lex[q] == t_and) // t_and is r-значение here
		q++;
	if(!lex.ид_ли(q))
		return false;
	тип = qualify(current_scope, тип, context.namespace_using);
	if(тип.дайСчёт()) {
		Locals(тип);
		return true;
	}
	return false;
}

void Parser::MatchPars()
{
	int level = 1;
	while(level && lex != t_eof) {
		if(Ключ('(')) level++;
		else
		if(Ключ(')')) level--;
		else
			++lex;
	}
}

void Parser::Statement()
{
	RecursionCounter recursionCounter(currentScopeDepth, lex == '{' ? 0 : 1);
	maxScopeDepth = max(maxScopeDepth, currentScopeDepth);

	if(Ключ(tk_case)) {
		if(lex.ид_ли())
			++lex;
		Ключ(':');
	}
	if(Ключ(tk_default))
		Ключ(':');
	if(lex.ид_ли() && lex[1] == ':') {
		++lex;
		++lex;
	}
	if(Ключ('{')) {
		Контекст cc;
		cc <<= context;
		int l = local.дайСчёт();
		while(!Ключ('}')) {
			if(lex == t_eof)
				выведиОш("eof");
			Statement();
		}
		context <<= cc;
		local.обрежь(l);
	}
	else
	if(Ключ(tk_if)) {
		int l = local.дайСчёт();
		Ключ('(');
		TryDecl();
		MatchPars();
		Statement();
		if(Ключ(tk_else))
			Statement();
		local.обрежь(l);
	}
	else
	if(Ключ(tk_for)) {
		int l = local.дайСчёт();
		Ключ('(');
		TryDecl();
		MatchPars();
		Statement();
		local.обрежь(l);
	}
	else
	if(Ключ(tk_while)) {
		int l = local.дайСчёт();
		Ключ('(');
		TryDecl();
		MatchPars();
		Statement();
		local.обрежь(l);
	}
	else
	if(Ключ(tk_try))
		Statement();
	else
	if(Ключ(tk_catch)) {
		Ключ('(');
		MatchPars();
		Statement();
	}
	else
	if(Ключ(tk_do)) {
		Statement();
		Ключ(tk_while);
		Ключ('(');
		MatchPars();
	}
	else
	if(Ключ(tk_switch)) {
		int l = local.дайСчёт();
		Ключ('(');
		TryDecl();
		MatchPars();
		Statement();
		local.обрежь(l);
	}
	else
	if(UsingNamespace())
		;
	else
	if(TryDecl())
		Ключ(';');
	else
		for(;;) {
			if(lex == t_eof)
				выведиОш("");
			TryLambda();
			if(Ключ(';') || lex == '{' || lex == '}' || lex >= tk_if && lex <= tk_do)
				break;
			++lex;
		}
}

bool Parser::Skipars(int& q)
{
	int par = lex[q];
	q++;
	int lvl = 1;
	while(lex[q] != t_eof) {
		int c = lex[q++];
		if(c == '(' || c == '[' || c == '{')
			lvl++;
		else
		if(c == ')' || c == ']' || c == '}') {
			lvl--;
			if(lvl == 0)
				return c == decode(par, '(', ')', '[', ']', '{', '}', 0);
		}
	}
	return false;
}

void Parser::TryLambda()
{
	if(lex != '[' || !dobody)
		return;
	int q = 0;
	if(!Skipars(q))
		return;
	int params = q;
	if(lex[q] == '(')
		if(!Skipars(q))
			return;
	if(lex[q] == tk_mutable)
		q++;
	if(lex[q] == t_arrow) {
		q++; // TODO: auto declaration could assign a return тип here
		for(;;) {
			if(lex[q] == ';' || lex[q] == t_eof)
				return;
			if(lex[q] == '{')
				break;
			q++;
		}
	}
	if(lex[q] == '{') {
		int n = local.дайСчёт();
		lex.дай(params);
		if(Ключ('(')) {
			Decl d;
			Строка();
			ParamList(d);
		}
		while(lex != '{' && lex != t_eof)
			++lex;
		Statement();
		local.обрежь(n);
	}
}

bool Parser::EatBody()
{
	if(lex == t_eof)
		return false;
	if(lex != '{') {
		local.очисть();
		return false;
	}
	lex.BeginBody();
	maxScopeDepth = currentScopeDepth = dobody ? 0 : 1;
	if(dobody) {
		inbody = true;
		Statement();
		inbody = false;
		local.очисть();
	}
	else {
		Ключ('{');
		int level = 1;
		while(level && lex != t_eof) {
			if(Ключ('{')) level++;
			else
			if(Ключ('}')) level--;
			else
				++lex;
			maxScopeDepth = max(level, maxScopeDepth);
		}
	}
	lex.EndBody();
	return true;
}

Ткст Parser::ResolveAutoType()
{
	Вектор<Ткст> xp = MakeXP(lex.Поз());
	if(lex == ':') // resolve for declaration, like 'for(auto i: vector)'
		xp << "." << "begin" << "()" << "->"; // incorrect, should rather use operator*(), but sufficient for now
	Индекс<Ткст> s = GetExpressionType(*base, *this, xp);
	int i = найдиМакс(s); // Ugly hack: we are not resolving overloading at all, so just choose stable тип if there are more, not Null
	return i < 0 ? Ткст() : s[i];
}
	
}
