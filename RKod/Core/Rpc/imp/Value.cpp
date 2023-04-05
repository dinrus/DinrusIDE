#include <DinrusC/Core.h>
#include <DinrusC/Rpc/Rpc.h>

namespace РНЦПДинрус {

Ткст FormatIso8601(Время t)
{
	return фмт("%04.4d%02.2d%02.2d`T%02.2d`:%02.2d`:%02.2d",
	              t.year, t.month, t.day, t.hour, t.minute, t.second);
}

Время ScanIso8601(const Ткст& p)
{
	Ткст s = обрежьОба(p);
	// 19980717T14:08:55
	// 01234567890123456
	if(s.дайСчёт() != 17 || s[8] != 'T' || s[11] != ':' || s[14] != ':')
		return Null;
	Время tm;
	tm.year = atoi(s.середина(0, 4));
	tm.month = atoi(s.середина(4, 2));
	tm.day = atoi(s.середина(6, 2));
	tm.hour = atoi(s.середина(9, 2));
	tm.minute = atoi(s.середина(12, 2));
	tm.second = atoi(s.середина(15, 2));
	if(!tm.пригоден())
		return Null;
	return tm;
}

void ValueCheck(bool b)
{
	if(!b)
		throw ValueTypeMismatch();
}

void ValueGet(int& n, const Значение& v)
{
	ValueCheck(пусто_ли(v) || число_ли(v));
	n = v;
}

void ValueGet(int64& n, const Значение& v)
{
	ValueCheck(пусто_ли(v) || число_ли(v));
	n = v;
}

void ValueGet(Ткст& s, const Значение& v)
{
	ValueCheck(пусто_ли(v) || ткст_ли(v));
	s = v;
}

void ValueGet(double& x, const Значение& v)
{
	ValueCheck(пусто_ли(v) || число_ли(v));
	x = v;
}

void ValueGet(bool& x, const Значение& v)
{
	ValueCheck(пусто_ли(v) || число_ли(v));
	x = v;
}

Время IsoTime(const Значение& v)
{
	Время tm = ScanIso8601((Ткст)v);
	if(пусто_ли(tm))
		throw ValueTypeMismatch();
	return tm;
}

void ValueGet(Дата& x, const Значение& v)
{
	if(ткст_ли(v)) {
		x = IsoTime(v);
		return;
	}
	ValueCheck(пусто_ли(v) || датаВремя_ли(v));
	x = v;
}

void ValueGet(Время& x, const Значение& v)
{
	if(ткст_ли(v)) {
		x = IsoTime(v);
		return;
	}
	ValueCheck(пусто_ли(v) || датаВремя_ли(v));
	x = v;
}

void ValueGet(Значение& t, const Значение& v)
{
	t = v;
}
	
void ValueGet(МассивЗнач& va, const Значение& v)
{
	ValueCheck(пусто_ли(v) || массивЗнач_ли(v));
	va = v;
}

void ValueGet(МапЗнач& vm, const Значение& v)
{
	ValueCheck(пусто_ли(v) || мапЗнач_ли(v));
	vm = v;
}

void ValuePut(Значение& v, int n)
{
	v = n;
}

void ValuePut(Значение& v, int64 n)
{
	v = n;
}

void ValuePut(Значение& v, const Ткст& s)
{
	v = s;
}

void ValuePut(Значение& v, const char *s)
{
	ValuePut(v, Ткст(s));
}

void ValuePut(Значение& v, double x)
{
	v = x;
}

void ValuePut(Значение& v, bool x)
{
	v = x;
}

void ValuePut(Значение& v, const Дата& x)
{
	v = x;
}

void ValuePut(Значение& v, const Время& x)
{
	v = x;
}

void ValuePut(Значение& v, const Значение& t)
{
	v = t;
}

void ValuePut(Значение& v, const МассивЗнач& va)
{
	v = va;
}

void ValuePut(Значение& v, const МапЗнач& vm)
{
	v = vm;
}

void ValuePut(Значение& v, const Json& json)
{
	RawJsonText x;
	x.json = json;
	v = RawToValue(x);
}

}