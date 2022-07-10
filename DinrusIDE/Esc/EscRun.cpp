#include "Esc.h"


namespace РНЦП {

#define LTIMING(x) // RTIMING(x)

void LambdaArgs(СиПарсер& p, EscLambda& l)
{
	p.передайСим('(');
	if(!p.сим(')'))
		for(;;) {
			if(p.сим3('.', '.', '.')) {
				l.varargs = true;
				p.передайСим(')');
				break;
			}
			l.inout.добавь(p.сим('&'));
			l.arg.добавь(p.читайИд());
			if(p.сим('=')) {
				const char *s = p.дайУк();
				int level = 0;
				for(;;) {
					if((p.сим_ли(')') || p.сим_ли(',')) && level == 0)
						break;
					if(p.сим(')'))
						level--;
					else
					if(p.сим('('))
						level++;
					else
						p.пропустиТерм();
					if(p.кф_ли())
						p.выведиОш("неожиданный конец файла при чтении дефолтного значения аргумента "
						             + l.arg.верх());
				}
				l.def.добавь(Ткст(s, p.дайУк()));
			}
			else
				if(l.def.дайСчёт())
					p.выведиОш("отсутствие дефолтного значения аргумента " + l.arg.верх());
			if(p.сим(')'))
				break;
			p.передайСим(',');
		}
	l.inout.сожми();
	l.arg.сожми();
}

EscValue ReadLambda(СиПарсер& p)
{
	EscValue lambda;
	EscLambda& l = lambda.CreateLambda();
	LambdaArgs(p, l);
	const char *t = p.дайУк();
	l.filename = p.дайИмяф();
	l.line = p.дайСтроку();
	if(!p.сим('{'))
		p.выведиОш("отсутствует '{'");
	SkipBlock(p);
	l.code = Ткст(t, p.дайУк());
	return lambda;
}

EscValue ReadLambda(const char *s)
{
	СиПарсер p(s);
	return ReadLambda(p);
}

Ткст EscEscape::InCall()
{
	return пусто_ли(id) ? Ткст() : " при вызове '" + id + "'";
}

Ткст EscEscape::DumpType(int i)
{
	if(i < arg.дайСчёт())
		return Ткст().конкат() << " (" << arg[i].GetTypeName() << " присутствует)";
	else
		return " (не хватает аргументов)";
}

void  EscEscape::CheckNumber(int i)
{
	if(i < arg.дайСчёт() && arg[i].число_ли())
		return;
	выведиОш(Ткст().конкат() << "ожидалось число как параметр " << i + 1 << InCall()
	           << DumpType(i));
}

double EscEscape::Number(int i)
{
	if(i >= arg.дайСчёт())
		выведиОш("слишком мало параметров" + InCall());
	return esc.Number(arg[i], "parameter" + InCall());
}

int EscEscape::Цел(int i)
{
	if(i >= arg.дайСчёт())
		выведиОш("слишком мало параметров" + InCall());
	return (int)esc.Цел(arg[i], "parameter" + InCall());
}

void  EscEscape::CheckArray(int i)
{
	if(i < arg.дайСчёт() && arg[i].IsArray())
		return;
	выведиОш(Ткст().конкат() << "array expected as parameter " << i + 1 << InCall()
	           << DumpType(i));
}

void  EscEscape::CheckMap(int i)
{
	if(i < arg.дайСчёт() && arg[i].IsMap())
		return;
	выведиОш(Ткст().конкат() << "map expected as parameter " << i + 1 << InCall());
}

void Escape(МассивМап<Ткст, EscValue>& globals, const char *function, Событие<EscEscape&> escape)
{
	СиПарсер p(function);
	EscValue& v = globals.дайПомести(p.читайИд());
	EscLambda& l = v.CreateLambda();
	l.escape = escape;
	LambdaArgs(p, l);
}

void Escape(МассивМап<Ткст, EscValue>& globals, const char *function, void (*escape)(EscEscape& e))
{
	Escape(globals, function, callback(escape));
}

void  EscValue::Escape(const char *method, Событие<EscEscape&> escape)
{
	СиПарсер p(method);
	Ткст id = p.читайИд();
	EscValue v;
	EscLambda& l = v.CreateLambda();
	l.escape = escape;
	LambdaArgs(p, l);
	MapSet(id, v);
}

void  EscValue::Escape(const char *method, EscHandle *h, Событие<EscEscape&> escape)
{
	СиПарсер p(method);
	Ткст id = p.читайИд();
	EscValue v;
	EscLambda& l = v.CreateLambda();
	l.escape = escape;
	l.handle = h;
	h->Retain();
	LambdaArgs(p, l);
	if(проц_ли())
		SetEmptyMap();
	MapSet(id, v);
}

void скан(МассивМап<Ткст, EscValue>& global, const char *file, const char *filename)
{
	LTIMING("скан");
	СиПарсер p(file, filename);
	while(!p.кф_ли()) {
		EscValue& v = global.дайПомести(p.читайИд());;
		v = ReadLambda(p);
	}
}

EscValue выполни(МассивМап<Ткст, EscValue>& global, EscValue *self,
                 const EscValue& lambda, Вектор<EscValue>& arg, int op_limit)
{
	const EscLambda& l = lambda.GetLambda();
	if(arg.дайСчёт() != l.arg.дайСчёт()) {
		Ткст argnames;
		for(int i = 0; i < l.arg.дайСчёт(); i++)
			argnames << (i ? ", " : "") << l.arg[i];
		throw СиПарсер::Ошибка(фмт("invalid number of arguments (%d passed, expected: %s)", arg.дайСчёт(), argnames));
	}
	EscValue ret;
	{
		Esc sub(global, l.code, op_limit, l.filename, l.line);
		if(self)
			sub.self = *self;
		for(int i = 0; i < l.arg.дайСчёт(); i++)
			sub.var.дайПомести(l.arg[i]) = arg[i];
		sub.пуск();
		if(self)
			*self = sub.self;
		ret = sub.return_value;
	}
	return ret;
}

EscValue выполни(МассивМап<Ткст, EscValue>& global, EscValue *self,
                 const char *имя, Вектор<EscValue>& arg, int op_limit)
{
	if(!self->IsMap())
		return EscValue();
	const ВекторМап<EscValue, EscValue>& m = self->дайМап();
	int ii = m.найди(Ткст(имя));
	if(ii >= 0 && m[ii].IsLambda())
		return выполни(global, self, m[ii], arg, op_limit);
	return EscValue();
}

EscValue выполни(МассивМап<Ткст, EscValue>& global, const char *имя, int op_limit)
{
	int ii = global.найди(Ткст(имя));
	Вектор<EscValue> arg;
	if(ii >= 0 && global[ii].IsLambda())
		return выполни(global, NULL, global[ii], arg, op_limit);
	return EscValue();
}

EscValue Evaluatex(const char *expression, МассивМап<Ткст, EscValue>& global, int oplimit)
{
	Esc sub(global, expression, oplimit, "", 0);
	for(int i = 0; i < global.дайСчёт(); i++)
		sub.var.добавь(global.дайКлюч(i), global[i]);
	EscValue v;
	v = sub.GetExp();
	for(int i = 0; i < sub.var.дайСчёт(); i++)
		global.дайДобавь(sub.var.дайКлюч(i)) = sub.var[i];
	return v;
}

EscValue Evaluate(const char *expression, МассивМап<Ткст, EscValue>& global, int oplimit)
{
	try {
		return Evaluatex(expression, global, oplimit);
	}
	catch(СиПарсер::Ошибка&) {}
	return EscValue();
}

Ткст   расширь(const Ткст& doc, МассивМап<Ткст, EscValue>& global,
                int oplimit, Ткст (*формат)(const Значение& v))
{
	Ткст out;
	const char *term = doc;
	bool cond = true;
	while(*term)
		if(term[0] == '<' && term[1] == ':') {
			term += 2;
			try {
				Esc sub(global, term, oplimit, "", 0);
				for(int i = 0; i < global.дайСчёт(); i++)
					sub.var.добавь(global.дайКлюч(i), global[i]);
				EscValue v;
				if(*term == '{') {
					sub.пуск();
					v = sub.return_value;
				}
				else
				if(sub.сим('!')) {
					EscValue& v = global.дайПомести(sub.читайИд());
					v = ReadLambda(sub);
				}
				else
				if(sub.сим('?'))
					cond = IsTrue(sub.GetExp());
				else
				if(sub.сим('/'))
					cond = !cond;
				else
				if(sub.сим('.'))
					cond = true;
				else
					v = sub.GetExp();
				if(cond)
					out << формат(StdValueFromEsc(v));
				sub.пробелы();
				term = sub.дайУк();
				if(term[0] != ':' || term[1] != '>')
					throw СиПарсер::Ошибка("отсутствие :>" + Ткст(term));
				term += 2;
			}
			catch(СиПарсер::Ошибка& e) {
				out << "(#Ошибка: " << e << "#)";
			}
		}
		else {
			if(cond)
				out.конкат(*term);
			term++;
		}
	return out;
}

}
