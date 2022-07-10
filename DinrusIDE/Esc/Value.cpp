#include <Esc/Esc.h>


namespace РНЦП {

EscValue EscFromStdValue(const Значение& v)
{
	EscValue r;
	Время t;
	if(!пусто_ли(v))
		switch(v.дайТип()) {
		case BOOL_V:
		case INT_V:
		case INT64_V:
		case DOUBLE_V:
			r = (double)v;
			break;
		case STRING_V:
		case WSTRING_V:
			r = (ШТкст)v;
			break;
		case TIME_V:
			t = v;
			r.MapSet("hour", t.hour);
			r.MapSet("minute", t.minute);
			r.MapSet("second", t.second);
		case DATE_V:
			t = v;
			r.MapSet("year", t.year);
			r.MapSet("month", t.month);
			r.MapSet("day", t.day);
			break;
		case VALUEARRAY_V:
			МассивЗнач va = v;
			r.SetEmptyArray();
			for(int i = 0; i < va.дайСчёт(); i++)
				r.ArrayAdd(EscFromStdValue(va[i]));
		}
	return r;
}

Значение StdValueFromEsc(const EscValue& v)
{
	if(v.число_ли())
		return v.GetNumber();
	if(v.IsArray())
		return (ШТкст)v;
	if(IsTime(v))
		return Время(v.GetFieldInt("year"), v.GetFieldInt("month"), v.GetFieldInt("day"),
			v.GetFieldInt("hour"), v.GetFieldInt("minute"), v.GetFieldInt("second"));
	if(IsDate(v))
		return Дата(v.GetFieldInt("year"), v.GetFieldInt("month"), v.GetFieldInt("day"));
	return Значение();
}

void SIC_StdFormat(EscEscape& e)
{
	e = стдФормат(StdValueFromEsc(e[0]));
}

void StdValueLib(МассивМап<Ткст, EscValue>& global)
{
	Escape(global, "стдФормат(x)", SIC_StdFormat);
}

}
