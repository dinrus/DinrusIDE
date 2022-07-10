#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x)  // DLOG(x)

#ifdef _ОТЛАДКА
Ткст Pdb::Val::вТкст() const
{
	Ткст r;
	r << "Pdb::Val тип: " << тип << ", ref: " << ref << ", array: " << array
	  << ", rvalue: " << rvalue;
	return r;
}
#endif

void Pdb::выведиОш(const char *s)
{
	throw СиПарсер::Ошибка(s);
}

int Pdb::размТипа(int ti)
{
	if(ti >= 0)
		return дайТип(ti).size;
	switch(ti) {
	case BOOL1:
	case SINT1:
	case UINT1:
		return 1;
	case SINT2:
	case UINT2:
		return 2;
	case SINT4:
	case UINT4:
	case FLT:
		return 4;
	case SINT8:
	case UINT8:
	case DBL:
		return 8;
	}
	return 0;
}

int Pdb::размТипа(const Ткст& имя)
{
	TypeInfo f = GetTypeInfo(имя);
	if(f.ref)
		return win64 ? 8 : 4;
	return размТипа(f.тип);
}

#define READINT0(тип) { \
	тип x; \
	if(v.address < 10000) \
		x = (тип)дайРегистрЦпб(*v.context, (int)v.address); \
	else \
	if(!копируй(v.address, &x, sizeof(x))) \
		выведиОш("??"); \
	v.ival = x; \
	break; \
} \

#define READINT(q, тип) \
case q: READINT0(тип)

#define READFLT(q, тип) \
case q: { \
	тип x; \
	if(!копируй(v.address, &x, sizeof(x))) \
		выведиОш("??"); \
	v.fval = x; \
	break; \
} \

adr_t Pdb::подбериУк(adr_t address)
{
	adr_t r = 0;
	if(!копируй(address, &r, win64 ? 8 : 4))
		выведиОш("??");
	return r;
}

byte Pdb::PeekByte(adr_t address)
{
	byte b;
	if(!копируй(address, &b, 1))
		выведиОш("??");
	return b;
}

word Pdb::PeekWord(adr_t address)
{
	word w;
	if(!копируй(address, &w, 2))
		выведиОш("??");
	return w;
}

dword Pdb::PeekDword(adr_t address)
{
	dword w;
	if(!копируй(address, &w, 4))
		выведиОш("??");
	return w;
}

Pdb::Val Pdb::GetRVal(Pdb::Val v)
{ // read data from debugee
	if(v.rvalue)
		return v;
	v.rvalue = true;
	if(v.array) {
		v.array = false;
		v.ref++;
	}
	else
	if(v.ref) { // Fetch pointer from the debugee memory
		v.address = подбериУк(v.address);
	}
	else
	if(v.bitcnt) { // Fetch bitfield from debugee memory
		dword w;
		if(!копируй(v.address, &w, win64 ? 8 : 4))
			выведиОш("??");
		w = (w >> v.bitpos) & (0xffffffff >> (32 - v.bitcnt));
		if(v.тип == SINT1 || v.тип == SINT2 || v.тип == SINT4 || v.тип == SINT8) {
			if(w & (1 << (v.bitcnt - 1)))
				w |= (0xffffffff << v.bitcnt);
			v.ival = (int32)w;
		}
		else
			v.ival = w;
	}
	else {
		if(v.address < 10000 && !v.context)
			выведиОш("??"); // регистрируй значение is missing context
		switch(v.тип) { // resolve primitive значение
		READINT(BOOL1, bool)
		READINT(UINT1, byte);
		READINT(SINT1, int8);
		READINT(UINT2, uint16);
		READINT(SINT2, int16);
		READINT(UINT4, uint32);
		READINT(SINT4, int32);
		READINT(UINT8, uint64);
		READINT(SINT8, int64);
		READFLT(FLT, float);
		READFLT(DBL, double);
		case PFUNC:
			if(win64) {
				READINT0(uint64);
			}
			else {
				READINT0(uint32)
			}
			break;
		case UNKNOWN:
			break;
		default:
			выведиОш("Неверный операнд");
		}
	}
	return v;
}

#define GETINT(q, тип) case q: return (тип)v.ival;

int64 Pdb::дайЦел64(Pdb::Val v)
{
	v = GetRVal(v);
	switch(v.тип) {
	GETINT(BOOL1, bool)
	GETINT(UINT1, byte);
	GETINT(SINT1, int8);
	GETINT(UINT2, uint16);
	GETINT(SINT2, int16);
	GETINT(UINT4, uint32);
	GETINT(SINT4, int32);
	GETINT(UINT8, uint64);
	GETINT(SINT8, int64);
	case FLT:
	case DBL:
		if(FitsInInt64(v.fval))
			return (int64)v.fval;
	}
	выведиОш("Неверный операнд");
	return 0;
}

int64 Pdb::GetInt64Attr(Pdb::Val v, const char *a)
{
	return дайЦел64(дайАтр(v, a));
}

#define GETINT(q, тип) case q: return (тип)v.ival;

double Pdb::GetFlt(Pdb::Val v)
{
	v = GetRVal(v);
	if(v.тип == DBL || v.тип == FLT)
		return v.fval;
	return (double)дайЦел64(v);
}

void Pdb::ZeroDiv(double x)
{
	if(x == 0)
		выведиОш("Деление на ноль");
}

Pdb::Val Pdb::Compute(Pdb::Val v1, Pdb::Val v2, int oper)
{
	LLOG("Compute " << char(oper));
	LLOG("v1: " << v1);
	LLOG("v2: " << v2);
	if(v1.ref || v1.array) {
		int q = (int)дайЦел64(v2) * (v1.ref > 1 ? 4 : размТипа(v1.тип));
		v1 = GetRVal(v1);
		switch(oper) {
		case '+': v1.address += q; break;
		case '-': v1.address -= q; break;
		default: выведиОш("Неверная арифметика указателей");
		}
		return v1;
	}
	if(v2.ref || v2.array) {
		int q = (int)дайЦел64(v1) * (v2.ref ? 4 : размТипа(v2.тип));
		v2 = GetRVal(v2);
		if(oper == '+')
			v2.address += q;
		else
			выведиОш("Неверная арифметика указателей");
		return v2;
	}
	Pdb::Val res;
	res.тип = max(v1.тип, v2.тип);
	res.rvalue = true;
	if(res.тип == DBL || res.тип == FLT) {
		double a = GetFlt(v1);
		double b = GetFlt(v2);
		switch(oper) {
		case '+': res.fval = a + b; break;
		case '-': res.fval = a - b; break;
		case '*': res.fval = a * b; break;
		case '/': ZeroDiv(b); res.fval = a / b; break;
		default: выведиОш("Неверные операнды для операции");
		}
	}
	else {
		int64 a = дайЦел64(v1);
		int64 b = дайЦел64(v2);
		switch(oper) {
		case '+': res.ival = a + b; break;
		case '-': res.ival = a - b; break;
		case '*': res.ival = a * b; break;
		case '/': ZeroDiv((double)b); res.ival = a / b; break;
		case '%': ZeroDiv((double)b); res.ival = a % b; break;
		default: выведиОш("Неверные операнды для операции");
		}
	}
	return res;
}

Pdb::Val Pdb::RValue(int64 i)
{
	Val v;
	v.rvalue = true;
	v.ival = i;
	v.тип = i >= -128 && i <= 127 ? SINT1 :
	         i >= -32768 && i <= 32767 ? SINT2 :
	         i >= INT_MIN && i <= INT_MAX ? SINT4 :
	         SINT8;
	return v;
}

Pdb::Val Pdb::DeRef(Pdb::Val v)
{
	if(v.ref <= 0)
		выведиОш("Only pointer can be dereferenced");
	v = GetRVal(v);
	v.ref--;
	v.rvalue = false;
	return v;
}

Pdb::Val Pdb::Реф(Pdb::Val v)
{
	if(v.rvalue)
		выведиОш("R-значение cannot be referenced");
	if(v.reference)
		v.reference = false;
	else {
		v.rvalue = true;
		v.ref++;
	}
	return v;
}

Pdb::Val Pdb::Field0(Pdb::Val v, const Ткст& field)
{
	Val v1;
	v1.тип = Null;
	if(v.rvalue || v.ref || v.array || v.тип < 0)
		return v1;
	const Type& t = дайТип(v.тип);
	int q = t.member.найди(field);
	if(q >= 0) {
		Val r = t.member[q];
		r.address += v.address;
		return r;
	}
	q = t.static_member.найди(field);
	if(q >= 0)
		return t.static_member[q];
	for(int i = 0; i < t.base.дайСчёт(); i++) {
		v1 = t.base[i];
		v1.address += v.address;
		if(v1.тип >= 0) {
			if(дайТип(v1.тип).имя == field)
				return v1;
			v1 = Field0(v1, field);
			if(!пусто_ли(v1.тип))
				return v1;
		}
	}
	return v1;
}

Pdb::Val Pdb::Field(Pdb::Val v, const Ткст& field)
{
	if(v.тип < 0)
		выведиОш("Must be struct, class or union");
	v = Field0(v, field);
	if(пусто_ли(v.тип))
		выведиОш("Unknown member \'" + field + "\'");
	return v;
}

Pdb::Val Pdb::прекрати(СиПарсер& p)
{
	if(p.сим2('0', 'x') || p.сим2('0', 'X'))
		return RValue(p.читайЧисло64(16));
	if(p.сим2_ли('0', '.')) {
		Val v;
		v.тип = DBL;
		v.rvalue = true;
		v.fval = p.читайДво();
		return v;
	}
	if(p.сим('0'))
		return RValue(p.число_ли() ? p.читайЧисло64(8) : 0);
	if(p.число_ли()) {
		double d = p.читайДво();
		if(d >= INT_MIN && d <= INT_MAX && (int)d == d) {
			return RValue((int)d);
		}
		else {
			Val v;
			v.тип = DBL;
			v.rvalue = true;
			v.fval = d;
			return v;
		}
	}
	if(p.сим_ли('\'')) {
		Ткст s = p.читайТкст('\'');
		if(s.дайДлину() != 1)
			выведиОш("Invalid character literal");
		return RValue(s[0]);
	}
	if(p.сим('(')) {
		Val v = Exp0(p);
		p.передайСим(')');
		return v;
	}
	if(!p.ид_ли())
		выведиОш("Missing id");
	Ткст id = p.читайИд();
	while(p.сим2(':', ':') && p.ид_ли())
		id << "::" << p.читайИд();
	if(current_frame) {
		Фрейм& f = *current_frame;
		int q = f.local.найди(id);
		if(q >= 0)
			return f.local[q];
		q = f.param.найди(id);
		if(q >= 0)
			return f.param[q];
		q = f.local.найди("this");
		if(q >= 0) {
			Val v = Field0(DeRef(f.local[q]), id);
			if(!пусто_ли(v.тип))
				return v;
		}
		Ткст scope = f.фн.имя;
		do {
			Ткст n = id;
			int q = scope.найдирек("::");
			if(q >= 0) {
				scope.обрежь(q);
				n = scope + "::" + id;
			}
			else
				scope.очисть();
			Val v = GetGlobal(n);
			if(v.address)
				return v;
		}
		while(scope.дайСчёт());
	}
	выведиОш("\'" + id + "\' undefined");
	return Val();
}

Ткст ReadType(СиПарсер& p)
{
	Ткст t = p.читайИд();
	if(p.сим('<')) {
		t << '<';
		int l = 1;
		while(l > 0) {
			if(p.сим('<')) {
				t << '<';
				l++;
			}
			else
			if(p.сим('>')) {
				t << '>';
				l--;
			}
			else
			if(p.число_ли())
				t << p.читайЦел();
			else
			if(p.сим(','))
				t << ',';
			else
				t << p.читайИд();
		}
	}
	return t;
}

Pdb::Val Pdb::пост(СиПарсер& p)
{
	Val v = прекрати(p);
	LLOG("пост: " << v);
	for(;;) {
		auto индексируй = [&](bool ключ) {
			int ech = ключ ? ')' : ']';
			int64 i = Null;
			if(!p.сим(ech))
				i = (int)дайЦел64(Exp0(p));
			p.сим(ech);
			Pretty p;
			if(PrettyVal(v, 0, 0, p) && p.kind != SINGLE_VALUE && p.data_type.дайСчёт() &&
			   (пусто_ли(i) && p.data_count || абс(i) < p.data_count)) {
			    if(пусто_ли(i))
			        i = p.data_count - 1;
			    else
			    if(i < 0)
			        i = p.data_count + i;
			    Pretty p;
				PrettyVal(v, i, 1, p);
				Val элт;
				int q = 0;
				if(p.data_type.дайСчёт() > 1) {
					if(!ключ)
						q = 1;
				}
				(TypeInfo &)элт = GetTypeInfo(p.data_type[q]);
				элт.context = v.context;
				элт.address = q < p.data_ptr.дайСчёт() ? p.data_ptr[q] : 0;
				v = элт;
			}
			else
				v = DeRef(Compute(v, RValue(Nvl(i)), '+'));
		};
		
		if(p.сим(':'))
			v = Field(v.ref ? DeRef(v) : v, ReadType(p));
		else
		if(p.сим('.') || p.сим2(':', ':') || p.сим2('-', '>'))
			v = Field(v.ref ? DeRef(v) : v, p.читайИд());
		else
		if(p.сим('['))
			индексируй(false);
		else
		if(p.сим('('))
			индексируй(true);
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Unary(СиПарсер& p)
{
	if(p.сим('-'))
		return Compute(RValue(0), Unary(p), '-');
	if(p.сим('+'))
		return GetRVal(Unary(p));
	if(p.сим('#')) {
		Pretty pp;
		if(PrettyVal(Unary(p), 0, 0, pp))
			return RValue((int)pp.data_count);
		выведиОш("Значение not recognized as high-level тип");
	}
	if(p.сим('*'))
		return DeRef(Unary(p));
	if(p.сим('&'))
		return Реф(Unary(p));
	if(p.сим('!')) {
		Val v = GetRVal(Unary(p));
		if(v.тип == FLT || v.тип == DBL)
			return RValue(!GetFlt(v));
		else
			return RValue(!дайЦел64(v));
	}
	return пост(p);
}

Pdb::Val Pdb::Multiplicative(СиПарсер& p)
{
	Val v = Unary(p);
	for(;;) {
		if(p.сим('*'))
			v = Compute(v, Unary(p), '*');
		else
		if(p.сим('/'))
			v = Compute(v, Unary(p), '/');
		else
		if(p.сим('%'))
			v = Compute(v, Unary(p), '%');
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Additive(СиПарсер& p)
{
	Val v = Multiplicative(p);
	for(;;) {
		if(p.сим('+'))
			v = Compute(v, Multiplicative(p), '+');
		else
		if(p.сим('-'))
			v = Compute(v, Multiplicative(p), '-');
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::сравни(Val v1, СиПарсер& p, int r1, int r2)
{
	int q;
	Val v2 = Additive(p);
	if(findarg(max(v1.тип, v2.тип), DBL, FLT) >= 0)
		q = сравниЗнак(GetFlt(v1), GetFlt(v2));
	else
		q = сравниЗнак(дайЦел64(v1), дайЦел64(v2));
	q = зн(q);
	return RValue(q == r1 || q == r2);
}

Pdb::Val Pdb::Comparison(СиПарсер& p)
{
	Val v = Additive(p);
	for(;;) {
		if(p.сим2('=', '='))
			v = сравни(v, p, 0, 0);
		else
		if(p.сим2('!', '='))
			v = сравни(v, p, -1, 1);
		else
		if(p.сим2('<', '='))
			v = сравни(v, p, -1, 0);
		else
		if(p.сим2('>', '='))
			v = сравни(v, p, 1, 0);
		else
		if(p.сим('<'))
			v = сравни(v, p, -1, -1);
		else
		if(p.сим('>'))
			v = сравни(v, p, 1, 1);
		else
			break;
	}
	return v;
}

void Pdb::GetBools(Val v1, Val v2, bool& a, bool& b)
{
	if(findarg(max(v1.тип, v2.тип), DBL, FLT) >= 0) {
		a = (bool)GetFlt(v1);
		b = (bool)GetFlt(v2);
	}
	else {
		a = (bool)дайЦел64(v1);
		b = (bool)дайЦел64(v2);
	}
}

Pdb::Val Pdb::LogAnd(СиПарсер& p)
{
	Val v = Comparison(p);
	for(;;) {
		if(p.сим2('&', '&')) {
			bool a, b;
			GetBools(v, Comparison(p), a, b);
			v = RValue(a && b);
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::LogOr(СиПарсер& p)
{
	Val v = LogAnd(p);
	for(;;) {
		if(p.сим2('|', '|')) {
			bool a, b;
			GetBools(v, LogAnd(p), a, b);
			v = RValue(a || b);
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Exp0(СиПарсер& p)
{
	DR_LOG("Evaluating Expression: " << p.дайУк());
	LLOG("Evaluating Expression: " << p.дайУк());
	return LogOr(p);
}

Pdb::Val Pdb::Exp(СиПарсер& p)
{
	Pdb::Val v = Exp0(p);
	if(!p.кф_ли())
		выведиОш("Неверное выражение");
	return v;
}

#endif
