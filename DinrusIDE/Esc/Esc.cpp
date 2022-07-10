#include <Esc/Esc.h>


namespace РНЦП {

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif

#define LTIMING(x)  // RTIMING(x)

void Esc::OutOfMemory()
{
	выведиОш("Вне памяти");
}

void Esc::TestLimit()
{
	LTIMING("TestLimit");
	if(!пусто_ли(op_limit))
		if(op_limit < 0)
			выведиОш("out of operations limit - considered frozen");
	if(EscValue::GetTotalCount() >= EscValue::GetMaxTotalCount())
		OutOfMemory();
}

EscValue Esc::дай(const SRVal& val)
{
	LTIMING("дай");
	if(skipexp)
		return (int64)1;
	EscValue v = val.lval ? *val.lval : val.rval;
	if(val.sbs.IsArray()) {
		const Вектор<EscValue>& sbs = val.sbs.дайМассив();
		for(int i = 0; i < sbs.дайСчёт(); i++) {
			const EscValue& ss = sbs[i];
			if(v.IsMap()) //!!!! (problem with a[1, 2]
				v = v.MapGet(ss);
			else
			if(v.IsArray()) {
				int count = v.дайСчёт();
				if(ss.IsArray() && ss.дайМассив().дайСчёт() >= 2) {
					EscValue v1 = ss.ArrayGet(0);
					EscValue v2 = ss.ArrayGet(1);
					int i = v1.дайЦел();
					int n = count - i;
					if(ss.дайСчёт() == 2)
						n = v2.цел_ли() ? v2.дайЦел() : n;
					else {
						if(v2.цел_ли()) {
							n = v2.дайЦел();
							if(n < 0)
								n += count;
							n -= i;
						}
					}
					if(i >= 0 && n >= 0 && i + n <= count)
						v = v.ArrayGet(i, n);
					else
						выведиОш("slice out of range");
				}
				else {
					int64 i = Цел(ss, "Индекс");
					if(i < 0)
						i += count;
					if(i >= 0 && i < count)
						v = v.ArrayGet((int)i);
					else
						выведиОш("Индекс out of range");
				}
			}
			else
				выведиОш("invalid indirection");
			TestLimit();
		}
	}
	return v;
}

void Esc::присвой(EscValue& val, const Вектор<EscValue>& sbs, int si, const EscValue& src)
{
	LTIMING("присвой");
	const EscValue& ss = sbs[si++];
	if(val.проц_ли())
		val.SetEmptyMap();
	if(val.IsMap()) {
		if(si < sbs.дайСчёт()) {
			EscValue x = val.MapGet(ss);
			val.MapSet(ss, 0.0);
			присвой(x, sbs, si, src);
			val.MapSet(ss, x);
		}
		else
			val.MapSet(ss, src);
		return;
	}
	else
	if(val.IsArray()) {
		if(si < sbs.дайСчёт()) {
			if(ss.IsArray())
				выведиОш("slice must be last subscript");
			int64 i = Цел(ss, "Индекс");
			if(i >= 0 && i < val.дайСчёт()) {
				EscValue x = val.ArrayGet((int)i);
				val.ArraySet((int)i, 0.0);
				присвой(x, sbs, si, src);
				if(!val.ArraySet((int)i, x))
					OutOfMemory();
				return;
			}
		}
		else {
			int count = val.дайСчёт();
			if(ss.IsArray()) {
				if(!src.IsArray() || ss.дайМассив().дайСчёт() < 2)
					выведиОш("only array can be assigned to the slice");
				EscValue v1 = ss.ArrayGet(0);
				EscValue v2 = ss.ArrayGet(1);
				int i = v1.цел_ли() ? v1.дайЦел() : 0;
				int n = count - i;
				if(ss.дайСчёт() == 2)
					n = v2.цел_ли() ? v2.дайЦел() : n;
				else {
					if(v2.цел_ли()) {
						n = v2.дайЦел();
						if(n < 0)
							n += count;
						n -= i;
					}
				}
				if(i >= 0 && n >= 0 && i + n <= count) {
					val.замени(i, n, src);
					return;
				}
				else
					выведиОш("slice out of range");
			}
			else {
				int64 i = ss.проц_ли() ? val.дайСчёт() : Цел(ss, "Индекс");
				if(i < 0)
					i = count + i;
				if(i >= 0 && i < INT_MAX) {
					if(!val.ArraySet((int)i, src))
						выведиОш("out of memory");
					return;
				}
			}
		}
	}
	выведиОш("invalid indirection");
}

void Esc::присвой(const SRVal& val, const EscValue& src)
{
	if(skipexp)
		return;
	if(!val.lval)
		выведиОш("l-значение required");
	if(val.sbs.IsArray() && val.sbs.дайСчёт())
		присвой(*val.lval, val.sbs.дайМассив(), 0, src);
	else
		*val.lval = src;
}

EscValue Esc::ExecuteLambda(const Ткст& id, EscValue lambda, SRVal self, Вектор<SRVal>& arg)
{
	LTIMING("ExecuteLambda");
	if(!lambda.IsLambda())
		выведиОш(фмт("'%s' is not a lambda", id));
	const EscLambda& l = lambda.GetLambda();
	if(!l.varargs && arg.дайСчёт() > l.arg.дайСчёт()
	   || arg.дайСчёт() < l.arg.дайСчёт() - l.def.дайСчёт())
		выведиОш("invalid number of arguments in call to '" + id + "'");
	Esc sub(global, l.code, op_limit, l.filename, l.line);
	sub.self = дай(self);
	for(int i = 0; i < l.arg.дайСчёт(); i++) {
		sub.var.дайДобавь(l.arg[i]) =
			i < arg.дайСчёт() ? дай(arg[i])
		                       : Evaluatex(l.def[i - (l.arg.дайСчёт() - l.def.дайСчёт())], global, op_limit);
		TestLimit();
	}
	EscValue retval;
	Массив<EscValue> argvar;
	if(l.escape) {
		argvar = sub.var.подбериЗначения();
		for(int i = l.arg.дайСчёт(); i < arg.дайСчёт(); i++) {
			argvar.добавь(дай(arg[i]));
		}
		EscValue v = дай(self);
		EscEscape e(*this, v, argvar);
		e.id = id;
		l.escape(e);
		retval = e.ret_val;
		self = e.self;
	}
	else {
		if(l.varargs) {
			EscValue& argv = sub.var.дайДобавь("argv");
			argv.SetEmptyArray();
			for(int i = l.arg.дайСчёт(); i < arg.дайСчёт(); i++)
				argv.ArrayAdd(дай(arg[i]));
		}
		sub.пуск();
		retval = sub.return_value;
		argvar = sub.var.подбериЗначения();
	}
	for(int i = 0; i < l.inout.дайСчёт(); i++)
		if(l.inout[i] && i < arg.дайСчёт() && arg[i].lval)
			присвой(arg[i], argvar[i]);
	if(self.lval)
		присвой(self, sub.self);
	return retval;
}

void Esc::Subscript(Esc::SRVal& r, Esc::SRVal _self, Ткст id)
{
	LTIMING("Subscript");
	for(;;) {
		TestLimit();
		if(char('['))
			if(char(']'))
				r.sbs.ArrayAdd(EscValue());
			else {
				EscValue v1, v2;
				if(!сим_ли(',') && !сим_ли(':'))
					v1 = GetExp();
				if(char(',')) {
					if(!сим_ли(']'))
						v2 = GetExp();
					EscValue x;
					x.ArrayAdd(v1);
					x.ArrayAdd(v2);
					r.sbs.ArrayAdd(x);
				}
				else
				if(char(':')) {
					if(!сим_ли(']'))
						v2 = GetExp();
					EscValue x;
					x.ArrayAdd(v1);
					x.ArrayAdd(v2);
					x.ArrayAdd(EscValue());
					r.sbs.ArrayAdd(x);
				}
				else
					r.sbs.ArrayAdd(v1);
				передайСим(']');
			}
		else
		if(char('.')) {
			_self = r;
			r.sbs.ArrayAdd(id = читайИд());
		}
		else
		if(char('(')) {
			LTIMING("call фн");
			Вектор<SRVal> arg;
			if(!char(')'))
				for(;;) {
					LTIMING("make args");
					Exp(arg.добавь());
					if(char(')')) break;
					передайСим(',');
				}
			if(!сим2_ли('!', '=') && char('!')) {
				прекрати(_self);
				EscValue g = дай(_self);
				if(!_self.lval || (!g.проц_ли() && !g.IsMap()))
					выведиОш("l-значение map or l-значение void expected on the right side of !");
				if(g.проц_ли()) {
					EscValue v;
					v.SetEmptyMap();
					присвой(_self, v);
				}
			}
			if(!skipexp)
				try {
					r = ExecuteLambda(id, дай(r), _self, arg);
				}
				catch(Искл e) {
					throw Ошибка(фмт("%s.%s(): %s", дай(r).GetTypeName(), id, e));
				}
		}
		else
			return;
	}
}

void Esc::Subscript(Esc::SRVal& r)
{
	Subscript(r, SRVal(), Ткст());
}

void Esc::прекрати(SRVal& r)
{
	r.sbs = EscValue();

	op_limit--;
	TestLimit();
	if(сим2('0', 'x') || сим2('0', 'X')) {
		r = читайЧисло64(16);
		return;
	}
	if(сим2('0', 'b') || сим2('0', 'B')) {
		r = читайЧисло64(2);
		return;
	}
	if(сим2_ли('0', '.')) {
		r = читайДво();
		return;
	}
	if(char('0')) {
		r = число_ли() ? читайЧисло64(8) : 0;
		return;
	}
	if(число_ли()) {
		// TODO: int64 !
		r = читайДво();
		return;
	}
	if(ткст_ли()) {
		r = EscValue(вУтф32(читайТкст()));
		return;
	}
	if(сим_ли('\'')) {
		ШТкст s = вУтф32(читайТкст('\''));
		if(s.дайДлину() != 1)
			выведиОш("invalid character literal");
		r = (int64)s[0];
		return;
	}
	if(char('@')) {
		r = ReadLambda(*this);
		Subscript(r);
		return;
	}
	if(Ид("void")) {
		r = EscValue();
		return;
	}
	if(char('{')) {
		EscValue map;
		map.SetEmptyMap();
		if(!char('}'))
			for(;;) {
				EscValue v = GetExp();
				передайСим(':');
				map.MapSet(v, GetExp());
				if(char('}'))
					break;
				передайСим(',');
				TestLimit();
			}
		r = map;
		Subscript(r);
		return;
	}
	if(char('[')) {
		EscValue array;
		array.SetEmptyArray();
		if(!char(']'))
			for(;;) {
				array.ArrayAdd(GetExp());
				if(char(']'))
					break;
				передайСим(',');
				TestLimit();
			}
		r = array;
		Subscript(r);
		return;
	}
	if(char('(')) {
		Exp(r);
		передайСим(')');
		Subscript(r);
		return;
	}

	SRVal _self;
	bool  _global = false;
	if(char('.')) {
		if(!self.IsMap())
			выведиОш("member-access in non-member code");
		_self.lval = &self;
	}
	else
	if(char(':'))
		_global = true;
	if(ид_ли()) {
		Ткст id = читайИд();
		EscValue method;
		int locali = var.найди(id);
		int ii;

		if(id == "self") {
			if(!self.IsMap())
				выведиОш("self in non-member code");
			_self.lval = &self;
			r = self;
		}
		else
		if(!_self.lval && !_global && locali < 0 && сим_ли('(') &&
		   self.IsMap() && (method = self.MapGet(id)).IsLambda()) {
		    _self.lval = &self;
			r = method;
		}
		else
		if(!_self.lval && !_global && locali < 0 && сим_ли('(') &&
		   (ii = global.найди(id)) >= 0 && global[ii].IsLambda()) {
			r = global[ii];
		}
		else
		if(_self.lval) {
			r = _self;
			r.sbs.ArrayAdd(id);
		}
		else
		if(_global)
			r.lval = &global.дайПомести(id);
		else
			r.lval = &var.дайПомести(id);

		try {
			Subscript(r, _self, id);
		}
		catch(СиПарсер::Ошибка e) {
			throw СиПарсер::Ошибка(id + ": " + e);
		}
	}
	else
		выведиОш("invalid expression");
}

Ткст Lims(const Ткст& s)
{
	return s.дайДлину() > 80 ? s.середина(0, 80) : s;
}

double Esc::Number(const EscValue& a, const char *oper)
{
	if(!a.число_ли())
		выведиОш(Ткст().конкат() << "number expected for '" << oper << "', encountered " << Lims(a.вТкст()));
	return a.GetNumber();
}

int64 Esc::Цел(const EscValue& a, const char *oper)
{
	if(!a.число_ли())
		выведиОш(Ткст().конкат() << "integer expected for '" << oper << "', encountered " << Lims(a.вТкст()));
	return a.дайЦел64();
}

double Esc::Number(const Esc::SRVal& a, const char *oper)
{
	return Number(дай(a), oper);
}

int64 Esc::Цел(const Esc::SRVal& a, const char *oper)
{
	return Цел(дай(a), oper);
}

void Esc::Unary(Esc::SRVal& r)
{
	if(сим2('+', '+')) {
		Unary(r);
		EscValue v = дай(r);
		if(v.IsInt64())
			присвой(r, Цел(v, "++") + 1);
		else
			присвой(r, Number(v, "++") + 1);
	}
	else
	if(сим2('-', '-')) {
		Unary(r);
		EscValue v = дай(r);
		if(v.IsInt64())
			присвой(r, Цел(v, "--") - 1);
		else
			присвой(r, Number(v, "--") - 1);
	}
	else
	if(char('-')) {
		Unary(r);
		EscValue v = дай(r);
		if(v.IsInt64())
			r = -Цел(v, "-");
		else
			r = -Number(v, "-");
	}
	else
	if(char('+')) {
		Unary(r);
		EscValue v = дай(r);
		if(v.IsInt64())
			r = Цел(v, "+");
		else
			r = Number(v, "+");
	}
	else
	if(char('!')) {
		Unary(r);
		r = (int64)!IsTrue(дай(r));
	}
	else
	if(char('~')) {
		Unary(r);
		r = ~Цел(дай(r), "~");
	}
	else
		прекрати(r);

	if(сим2('+', '+')) {
		EscValue v = дай(r);
		if(v.IsInt64())
			присвой(r, Цел(v, "++") + 1);
		else
			присвой(r, Number(v, "++") + 1);
		r = v;
	}
	if(сим2('-', '-')) {
		EscValue v = дай(r);
		if(v.IsInt64())
			присвой(r, Цел(v, "--") - 1);
		else
			присвой(r, Number(v, "--") - 1);
		r = v;
	}
}

EscValue Esc::MulArray(EscValue array, EscValue times)
{
	EscValue r;
	r.SetEmptyArray();
	for(int n = times.дайЦел(); n > 0; n >>= 1) {
		if(n & 1)
			if(!r.приставь(array))
				OutOfMemory();
		if(!array.приставь(array))
			OutOfMemory();
		TestLimit();
	}
	return r;
}

void Esc::Mul(Esc::SRVal& r)
{
	Unary(r);
	for(;;)
		if(!сим2_ли('*', '=') && char('*')) {
			EscValue x = дай(r);
			SRVal w;
			Unary(w);
			EscValue y = дай(w);
			if(x.IsArray() && y.цел_ли())
				r = MulArray(x, y);
			else
			if(y.IsArray() && x.цел_ли())
				r = MulArray(y, x);
			else
			if(x.IsInt64() && y.IsInt64())
				r = Цел(x, "*") * Цел(y, "*");
			else
				r = Number(x, "*") * Number(y, "*");
		}
		else
		if(!сим2_ли('/', '=') && char('/')) {
			SRVal w;
			Unary(w);
			EscValue x = дай(r);
			EscValue y = дай(w);
			double b = Number(y, "/");
			if(b == 0)
				выведиОш("divide by zero");
			r = Number(x, "/") / b;
		}
		else
		if(!сим2_ли('%', '=') && char('%')) {
			SRVal w;
			Unary(w);
			int64 b = Цел(w, "%");
			if(b == 0)
				выведиОш("divide by zero");
			r = Цел(r, "%") % b;
		}
		else
			return;
}

void Esc::добавь(Esc::SRVal& r)
{
	Mul(r);
	for(;;)
		if(!сим2_ли('+', '=') && char('+')) {
			EscValue v = дай(r);
			SRVal w;
			Mul(w);
			EscValue b = дай(w);
			if(v.IsArray() && b.IsArray()) {
				if(!v.замени(v.дайСчёт(), 0, b))
					OutOfMemory();
				r = v;
			}
			else
			if(!(v.IsArray() && b.проц_ли())) {
				if(v.IsInt64() && b.IsInt64())
					r = Цел(v, "+") + Цел(b, "+");
				else
					r = Number(v, "+") + Number(b, "+");
			}
		}
		else
		if(!сим2_ли('-', '=') && char('-')) {
			SRVal w;
			Mul(w);
			EscValue v = дай(r);
			EscValue b = дай(w);
			if(v.IsInt64() && b.IsInt64())
				r = Цел(v, "-") - Цел(b, "-");
			else
				r = Number(v, "-") - Number(b, "-");
		}
		else
			return;
}

void Esc::Shift(Esc::SRVal& r)
{
	добавь(r);
	for(;;)
		if(сим2('<', '<')) {
			EscValue v = дай(r);
			SRVal w;
			добавь(w);
			EscValue b = дай(w);
			if(v.IsArray() && b.IsArray()) {
				if(!v.замени(v.дайСчёт(), 0, b))
					OutOfMemory();
				присвой(r, v);
			}
			else
			if(!(v.IsArray() && b.проц_ли()))
				r = Цел(v, "<<") << Цел(b, "<<");
		}
		else
		if(сим2('>', '>')) {
			SRVal w;
			добавь(w);
			r = Цел(r, ">>") >> Цел(w,  ">>");
		}
		else
			return;
}

double Esc::DoCompare(const EscValue& a, const EscValue& b, const char *op)
{
	LTIMING("DoCompare");
	if(a.IsInt64() && b.IsInt64())
		return сравниЗнак(a.дайЦел64(), b.дайЦел64());
	if(a.число_ли() && b.число_ли())
		return сравниЗнак(a.GetNumber(), b.GetNumber());
	if(a.IsArray() && b.IsArray()) {
		const Вектор<EscValue>& x = a.дайМассив();
		const Вектор<EscValue>& y = b.дайМассив();
		int i = 0;
		for(;;) {
			if(i >= x.дайСчёт())
				return i < y.дайСчёт() ? -1 : 0;
			if(i >= y.дайСчёт())
				return i < x.дайСчёт() ? 1 : 0;
			double q = DoCompare(x[i], y[i], op);
			if(q) return q;
			i++;
		}
	}
	if(a.проц_ли() && b.проц_ли())
		return 0;
	if(!a.проц_ли() && b.проц_ли())
		return 1;
	if(a.проц_ли() && !b.проц_ли())
		return -1;
	выведиОш("invalid values for comparison " + a.GetTypeName() + ' ' + op + ' ' + b.GetTypeName());
	return 0;
}

double Esc::DoCompare(const SRVal& a, const char *op)
{
	SRVal w;
	Shift(w);
	return DoCompare(дай(a), дай(w), op);
}

void Esc::сравни(Esc::SRVal& r)
{
	Shift(r);
	for(;;)
		if(сим2('>', '='))
			r = DoCompare(r, ">=") >= 0;
		else
		if(сим2('<', '='))
			r = DoCompare(r, "<=") <= 0;
		else
		if(char('>'))
			r = DoCompare(r, ">") > 0;
		else
		if(char('<'))
			r = DoCompare(r, "<") < 0;
		else
			return;
}

void Esc::Equal(Esc::SRVal& r)
{
	сравни(r);
	for(;;)
		if(сим2('=', '=')) {
			SRVal w;
			сравни(w);
			r = дай(r) == дай(w);
		}
		else
		if(сим2('!', '=')) {
			SRVal w;
			сравни(w);
			r = дай(r) != дай(w);
		}
		else
			return;
}

void Esc::BinAnd(Esc::SRVal& r)
{
	Equal(r);
	while(!сим2_ли('&', '&') && char('&')) {
		SRVal w;
		Equal(w);
		r = Цел(r, "&") & Цел(w, "&");
	}
}

void Esc::BinXor(Esc::SRVal& r)
{
	BinAnd(r);
	while(char('^')) {
		SRVal w;
		BinAnd(w);
		r = Цел(r, "^") ^ Цел(w, "^");
	}
}

void Esc::BinOr(Esc::SRVal& r)
{
	BinXor(r);
	while(!сим2_ли('|', '|') && char('|')) {
		SRVal w;
		BinXor(w);
		r = Цел(r, "|") | Цел(w, "|");
	}
}

void Esc::And(Esc::SRVal& r)
{
	BinOr(r);
	if(сим2_ли('&', '&')) {
		bool b = IsTrue(дай(r));
		while(сим2('&', '&')) {
			SRVal w;
			if(b) {
				BinOr(w);
				b = b && IsTrue(дай(w));
			}
			else {
				skipexp++;
				BinOr(w);
				skipexp--;
			}
		}
		r = b;
	}
}

void Esc::Or(Esc::SRVal& r)
{
	And(r);
	if(сим2_ли('|', '|')) {
		bool b = IsTrue(дай(r));
		while(сим2('|', '|')) {
			SRVal w;
			if(b) {
				skipexp++;
				And(w);
				skipexp--;
			}
			else {
				And(w);
				b = b || IsTrue(дай(w));
			}
		}
		r = b;
	}
}

void Esc::Cond(Esc::SRVal& r)
{
	Or(r);
	if(char('?')) {
		bool t = IsTrue(дай(r));
		SRVal dummy;
		if(t) {
			Cond(r);
			передайСим(':');
			skipexp++;
			Cond(dummy);
			skipexp--;
		}
		else {
			skipexp++;
			Cond(dummy);
			skipexp--;
			передайСим(':');
			Cond(r);
		}
	}
}

void Esc::присвой(Esc::SRVal& r)
{
	Cond(r);
	if(char('=')) {
		SRVal w;
		присвой(w);
		присвой(r, дай(w));
	}
	else
	if(сим2('+', '=')) {
		EscValue v = дай(r);
		SRVal w;
		Cond(w);
		EscValue b = дай(w);
		if(v.IsArray() && b.IsArray()) {
			if(!v.замени(v.дайСчёт(), 0, b))
				OutOfMemory();
			присвой(r, v);
		}
		else
		if(!(v.IsArray() && b.проц_ли())) {
			if(v.IsInt64() && b.IsInt64())
				присвой(r, Цел(v, "+=") + Цел(b, "+="));
			else
				присвой(r, Number(v, "+=") + Number(b, "+="));
		}
	}
	else
	if(сим2('-', '=')) {
		SRVal w;
		Cond(w);
		EscValue v = дай(r);
		EscValue b = дай(w);
		if(v.IsInt64() && b.IsInt64())
			присвой(r, Цел(v, "-=") - Цел(b, "-="));
		else
			присвой(r, Number(v, "-=") - Number(b, "-="));
	}
	else
	if(сим2('*', '=')) {
		SRVal w;
		Cond(w);
		EscValue x = дай(r);
		EscValue y = дай(w);
		if(x.IsInt64() && y.IsInt64())
			присвой(r, Цел(x, "*=") * Цел(y, "*="));
		else
			присвой(r, Number(x, "*=") * Number(y, "*="));
	}
	else
	if(сим2('/', '=')) {
		SRVal w;
		Cond(w);
		EscValue v = дай(r);
		EscValue b = дай(w);
		double q = Number(v, "/=");
		if(q == 0)
			выведиОш("divide by zero");
		присвой(r, Number(b, "/=") / q);
	}
	else
	if(сим2('%', '=')) {
		SRVal w;
		Cond(w);
		int64 a = Цел(r, "%=");
		int64 b = Цел(w, "%=");
		if(b == 0)
			выведиОш("divide by zero");
		присвой(r, a % b);
	}
}

int Esc::stack_level = 50;

void Esc::Exp(Esc::SRVal& r)
{
	LTIMING("Exp");
	пробелы();
	stack_level--;
	if(stack_level <= 0)
		выведиОш("stack overflow");
	присвой(r);
	stack_level++;
}

EscValue Esc::GetExp() {
	SRVal r;
	Exp(r);
	return дай(r);
}

void Esc::пропустиТерм()
{
	if(кф_ли())
		выведиОш("unexpected end of file");
	СиПарсер::пропустиТерм();
	пробелы();
}

void Esc::SkipExp()
{
	int level = 0;
	for(;;) {
		if(сим_ли(';'))
			return;
		if(сим_ли(')') && level == 0)
			return;
		if(char(')'))
			level--;
		else
		if(char('('))
			level++;
		else
			пропустиТерм();
		if(кф_ли())
			выведиОш("unexpected end of file");
	}
}

void SkipBlock(СиПарсер& p)
{
	int level = 1;
	while(level > 0 && !p.кф_ли()) {
		if(p.сим('{')) level++;
		else
		if(p.сим('}')) level--;
		else
			p.пропустиТерм();
	}
}

void Esc::SkipStatement()
{
	stack_level--;
	if(stack_level <= 0)
		выведиОш("stack overflow");
	if(Ид("if")) {
		передайСим('(');
		SkipExp();
		передайСим(')');
		SkipStatement();
		if(Ид("else"))
			SkipStatement();
	}
	else
	if(Ид("for")) {
		передайСим('(');
		if(!сим_ли(';'))
			SkipExp();
		передайСим(';');
		if(!сим_ли(';'))
			SkipExp();
		передайСим(';');
		if(!сим_ли(')'))
			SkipExp();
		передайСим(')');
		SkipStatement();
	}
	else
	if(Ид("while") || Ид("switch")) {
		передайСим('(');
		SkipExp();
		передайСим(')');
		SkipStatement();
	}
	else
	if(Ид("do")) {
		SkipBlock(*this);
		передайИд("while");
		передайСим('(');
		SkipExp();
		передайСим(')');
		передайСим(';');
	}
	else
	if(char('{'))
		SkipBlock(*this);
	else {
		SkipExp();
		передайСим(';');
	}
	stack_level++;
}

bool  Esc::PCond()
{
	передайСим('(');
	bool c = IsTrue(GetExp());
	передайСим(')');
	return c;
}

void Esc::FinishSwitch()
{
	while(no_break && no_return && no_continue) {
		if(Ид("case")) {
			SRVal r;
			Exp(r);
			передайСим(':');
		}
		else
		if(Ид("default"))
			передайСим(':');
		else
			if(char('}'))
				return;
		DoStatement();
	}
	while(!char('}'))
		SkipStatement();
}

void  Esc::DoStatement()
{
	op_limit--;
	TestLimit();
	if(Ид("if"))
		if(PCond()) {
			DoStatement();
			if(Ид("else"))
				SkipStatement();
		}
		else {
			SkipStatement();
			if(Ид("else"))
				DoStatement();
		}
	else
	if(Ид("do")) {
		loop++;
		Поз pos = дайПоз();
		do {
			устПоз(pos);
			DoStatement();
			передайИд("while");
			no_continue = true;
		}
		while(PCond() && no_break && no_return);
		передайСим(';');
		no_break = true;
		loop--;
	}
	else
	if(Ид("while")) {
		loop++;
		Поз pos = дайПоз();
		for(;;) {
			устПоз(pos);
			if(!PCond() || !no_break || !no_return || !no_continue) {
				SkipStatement();
				break;
			}
			DoStatement();
			no_continue = true;
		}
		no_break = true;
		loop--;
	}
	else
	if(Ид("for")) {
		loop++;
		передайСим('(');
		SRVal var;
		if(!сим_ли(';'))
			Exp(var);
		if(Ид("in") || char(':')) {
			EscValue range = GetExp();
			передайСим(')');
			Поз stmt = дайПоз();
			int i = 0;
			for(;;) {
				устПоз(stmt);
				if(range.IsArray()) {
					if(i >= range.дайСчёт())
						break;
					присвой(var, (int64)i);
				}
				else
				if(range.IsMap()) {
					const ВекторМап<EscValue, EscValue>& map = range.дайМап();
					if(i >= map.дайСчёт())
						break;
					if(map.отлинкован(i)) {
						i++;
						continue;
					}
					присвой(var, map.дайКлюч(i));
				}
				if(!no_break || !no_return || !no_continue) {
					SkipStatement();
					break;
				}
				DoStatement();
				no_continue = true;
				i++;
			}
			SkipStatement();
		}
		else {
			передайСим(';');
			Поз cond;
			if(!сим_ли(';')) {
				cond = дайПоз();
				SkipExp();
			}
			передайСим(';');
			Поз after;
			if(!сим_ли(')')) {
				after = дайПоз();
				SkipExp();
			}
			передайСим(')');
			Поз stmt = дайПоз();
			for(;;) {
				bool c = true;
				if(cond.ptr) {
					устПоз(cond);
					c = IsTrue(GetExp());
				}
				устПоз(stmt);
				if(!c || !no_break || !no_return || !no_continue) {
					SkipStatement();
					break;
				}
				DoStatement();
				no_continue = true;
				if(after.ptr) {
					устПоз(after);
					SRVal r;
					Exp(r);
				}
			}
		}
		no_break = true;
		loop--;
	}
	else
	if(Ид("break")) {
		if(!loop)
			выведиОш("misplaced 'break'");
		no_break = false;
		передайСим(';');
	}
	else
	if(Ид("continue")) {
		if(!loop)
			выведиОш("misplaced 'continue'");
		no_continue = false;
		передайСим(';');
	}
	else
	if(Ид("case"))
		выведиОш("misplaced 'case'");
	else
	if(Ид("default"))
		выведиОш("misplaced 'default'");
	else
	if(Ид("else"))
		выведиОш("misplaced 'else'");
	else
	if(Ид("return")) {
		no_return = false;
		if(!char(';')) {
			return_value = GetExp();
			передайСим(';');
		}
		else
			return_value = EscValue();
	}
	else
	if(Ид("switch")) {
		loop++;
		передайСим('(');
		EscValue a = GetExp();
		передайСим(')');
		передайСим('{');
		while(!char('}')) {
			if(Ид("case")) {
				EscValue b = GetExp();
				передайСим(':');
				if(a == b) {
					FinishSwitch();
					break;
				}
			}
			else
			if(Ид("default")) {
				передайСим(':');
				FinishSwitch();
				break;
			}
			else
				SkipStatement();
		}
		loop--;
		no_break = true;
	}
	else
	if(char('#')) {
		int тип = 0;
		if(char('.'))
			тип = 1;
		else
		if(char(':'))
			тип = 2;
		Ткст id = читайИд();
		EscValue l = ReadLambda(*this);
		if(тип == 1) {
			if(self.проц_ли())
				выведиОш("no instance");
			self.MapSet(id, l);
		}
		else
		if(тип == 2)
			global.дайДобавь(id) = l;
		else
			var.дайДобавь(id) = l;
	}
	else
	if(char('{')) {
		while(!char('}') && no_break && no_return && no_continue)
			DoStatement();
	}
	else
	if(!char(';')) {
		SRVal v;
		Exp(v);
		передайСим(';');
	}
}

void  Esc::пуск()
{
	no_return = no_break = no_continue = true;
	loop = 0;
	skipexp = 0;
	while(!кф_ли() && no_return && no_break && no_continue)
		DoStatement();
}

}
