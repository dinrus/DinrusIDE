#include <DinrusPro/DinrusPro.h>
#include <RKod/Rpc/Rpc.h>

namespace ДинрусРНЦП {

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

проц ValueCheck(бул b)
{
	if(!b)
		throw ValueTypeMismatch();
}

проц ValueGet(цел& n, const Значение& v)
{
	ValueCheck(пусто_ли(v) || число_ли(v));
	n = v;
}

проц ValueGet(дол& n, const Значение& v)
{
	ValueCheck(пусто_ли(v) || число_ли(v));
	n = v;
}

проц ValueGet(Ткст& s, const Значение& v)
{
	ValueCheck(пусто_ли(v) || ткст_ли(v));
	s = v;
}

проц ValueGet(дво& x, const Значение& v)
{
	ValueCheck(пусто_ли(v) || число_ли(v));
	x = v;
}

проц ValueGet(бул& x, const Значение& v)
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

проц ValueGet(Дата& x, const Значение& v)
{
	if(ткст_ли(v)) {
		x = IsoTime(v);
		return;
	}
	ValueCheck(пусто_ли(v) || датаВремя_ли(v));
	x = v;
}

проц ValueGet(Время& x, const Значение& v)
{
	if(ткст_ли(v)) {
		x = IsoTime(v);
		return;
	}
	ValueCheck(пусто_ли(v) || датаВремя_ли(v));
	x = v;
}

проц ValueGet(Значение& t, const Значение& v)
{
	t = v;
}
	
проц ValueGet(МассивЗнач& va, const Значение& v)
{
	ValueCheck(пусто_ли(v) || массивЗнач_ли(v));
	va = v;
}

проц ValueGet(МапЗнач& vm, const Значение& v)
{
	ValueCheck(пусто_ли(v) || мапЗнач_ли(v));
	vm = v;
}

проц ValuePut(Значение& v, цел n)
{
	v = n;
}

проц ValuePut(Значение& v, дол n)
{
	v = n;
}

проц ValuePut(Значение& v, const Ткст& s)
{
	v = s;
}

проц ValuePut(Значение& v, кткст0 s)
{
	ValuePut(v, Ткст(s));
}

проц ValuePut(Значение& v, дво x)
{
	v = x;
}

проц ValuePut(Значение& v, бул x)
{
	v = x;
}

проц ValuePut(Значение& v, const Дата& x)
{
	v = x;
}

проц ValuePut(Значение& v, const Время& x)
{
	v = x;
}

проц ValuePut(Значение& v, const Значение& t)
{
	v = t;
}

проц ValuePut(Значение& v, const МассивЗнач& va)
{
	v = va;
}

проц ValuePut(Значение& v, const МапЗнач& vm)
{
	v = vm;
}

проц ValuePut(Значение& v, const Json& json)
{
	RawJsonText x;
	x.json = json;
	v = RawToValue(x);
}

}