#include "Core.h"

namespace РНЦПДинрус {

Значение ParseJSON(СиПарсер& p)
{
	p.искейпЮникод();
	if(p.дво_ли())
		return p.читайДво();
	if(p.ткст_ли()) {
		bool dt = p.сим2_ли('\"', '\\');
		Ткст s = p.читайТкст();
		if(dt) {
			СиПарсер p(s);
			if(p.сим('/') && p.ид("Дата") && p.сим('(') && p.цел_ли()) {
				int64 n = p.читайЦел64();
				if(!пусто_ли(n))
					return Время(1970, 1, 1) + n / 1000;
			}
		}
		return s;
	}
	if(p.ид("null"))
		return Null;
	if(p.ид("true"))
		return true;
	if(p.ид("false"))
		return false;
	if(p.сим('{')) {
		МапЗнач m;
		while(!p.сим('}')) {
			Ткст ключ = p.читайТкст();
			p.передайСим(':');
			m.добавь(ключ, ParseJSON(p));
			if(p.сим('}')) // Stray ',' at the end of list is allowed...
				break;
			p.передайСим(',');
		}
		return m;
	}
	if(p.сим('[')) {
		МассивЗнач va;
		while(!p.сим(']')) {
			va.добавь(ParseJSON(p));
			if(p.сим(']')) // Stray ',' at the end of list is allowed...
				break;
			p.передайСим(',');
		}
		return va;
	}
	p.выведиОш("Unrecognized JSON element");
	return Null;
}

Значение ParseJSON(const char *s)
{
	try {
		СиПарсер p(s);
		return ParseJSON(p);
	}
	catch(СиПарсер::Ошибка e) {
		return значОш(e);
	}
}

Ткст AsJSON(Время tm)
{
	return пусто_ли(tm) ? "null" : "\"\\/Дата(" + какТкст(1000 * (tm - Время(1970, 1, 1))) + ")\\/\"";
}

Ткст AsJSON(Дата dt)
{
	return AsJSON(воВремя(dt));
}

Json& Json::CatRaw(const char *ключ, const Ткст& val)
{
	if(text.дайСчёт())
		text << ',';
	text << AsJSON(ключ) << ":" << val;
	return *this;
}

JsonArray& JsonArray::CatRaw(const Ткст& val)
{
	if(text.дайСчёт())
		text << ',';
	text << val;
	return *this;
}

Ткст AsJSON(const Значение& v, const Ткст& sep, bool pretty)
{
	Ткст r;
	if(v.дайТип() == VALUEMAP_V) {
		r << "{";
		Ткст sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		МапЗнач m = v;
		МассивЗнач va = m.дайЗначения();
		for(int i = 0; i < m.дайСчёт(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << AsJSON((Ткст)m.дайКлюч(i)) << (pretty ? ": " : ":")
			  << AsJSON(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "}";
		return r;
	}
	if(v.дайТип() == VALUEARRAY_V) {
		r << "[";
		Ткст sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		МассивЗнач va = v;
		for(int i = 0; i < va.дайСчёт(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << AsJSON(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "]";
		return r;
	}
	if(число_ли(v) && (пусто_ли(v) || НЧ_ли((double)v)))
		return "null";
	if(v.дайТип() == INT_V)
		return фмт("%d", (int)v);
	if(v.дайТип() == BOOL_V)
		return (bool)v ? "true" : "false";
	if(число_ли(v))
		return фмтГ((double)v, 17);
	if(ткст_ли(v))
		return какТкстСи((Ткст)v, INT_MAX, NULL, ASCSTRING_JSON);
	if(датаВремя_ли(v))
		return AsJSON((Время)v);
	if(пусто_ли(v))
		return "null";
	NEVER_("Non-JSON значение in AsJSON: " + v.дайИмяТипа());
	return "null";
}

void ДжейсонВВ::уст(const char *ключ, const Значение& v)
{
	ПРОВЕРЬ(сохраняется());
	if(!map)
		map.создай();
	map->добавь(ключ, v);
}

Ткст AsJSON(const Значение& v, bool pretty)
{
	return AsJSON(v, Ткст(), pretty);
}

template<> void вДжейсон(ДжейсонВВ& io, double& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(пусто_ли(v)) {
			var = Null;
			return;
		}
		if(число_ли(v)) {
			var = io.дай();
			return;
		}
		if(ткст_ли(v)) {
			double h = сканДво((Ткст)v);
			if(!пусто_ли(h)) {
				var = h;
				return;
			}
		}
		throw JsonizeError("number expected");
	}
	else
		io.уст(var);
}

template<> void вДжейсон(ДжейсонВВ& io, int& var)
{
	double v = целДво(var);
	вДжейсон(io, v);
	if(io.грузится()) {
		if(пусто_ли(v))
			var = Null;
		else
		if(v >= INT_MIN && v <= INT_MAX && (int)v == v)
			var = (int)v;
		else
			throw JsonizeError("number is not integer");
	}
}

template<> void вДжейсон(ДжейсонВВ& io, byte& var)
{
	double v = var;
	вДжейсон(io, v);
	if(io.грузится()) {
		if(v >= 0 && v <= 255 && (int)v == v)
			var = (byte)v;
		else
			throw JsonizeError("integer 0-255 expected");
	}
}

template<> void вДжейсон(ДжейсонВВ& io, int16& var)
{
	double v = var;
	вДжейсон(io, v);
	if(io.грузится()) {
		if(v >= -32768 && v <= 32767 && (int)v == v)
			var = (int16)v;
		else
			throw JsonizeError("16-bit integer expected");
	}
}

template<> void вДжейсон(ДжейсонВВ& io, bool& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(число_ли(v) && !пусто_ли(v))
			var = (bool)v;
		else
			throw JsonizeError("boolean expected");
	}
	else
		io.уст(var);
}

template<> void вДжейсон(ДжейсонВВ& io, int64& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(пусто_ли(v)) {
			var = Null;
			return;
		}
		if(v.является<int64>() || v.является<int>()) {
			var = v;
			return;
		}
		if(число_ли(v)) {
			double d = v;
			if(FitsInInt64(d)) {
				var = (int64)d;
				return;
			}
		}
		else
		if(ткст_ли(v)) {
			int64 h = сканЦел64((Ткст)v);
			if(!пусто_ли(h)) {
				var = h;
				return;
			}
		}
		throw JsonizeError("invalid int64 значение");
	}
	else
		if(пусто_ли(var))
			io.уст(Null);
		else
		if(var >= I64(-9007199254740992) && var <= I64(9007199254740991))
			io.уст(var);
		else
			io.уст(какТкст(var));
}

template<> void вДжейсон(ДжейсонВВ& io, Ткст& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(пусто_ли(v))
			var = Null;
		else
		if(ткст_ли(v))
			var = v;
		else
			throw JsonizeError("string expected");
	}
	else
		io.уст(var);
}

template<> void вДжейсон(ДжейсонВВ& io, ШТкст& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(пусто_ли(v))
			var = Null;
		else
		if(ткст_ли(v))
			var = v;
		else
			throw JsonizeError("string expected");
	}
	else
		io.уст(var);
}

template<> void вДжейсон(ДжейсонВВ& io, Дата& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(пусто_ли(v)) {
			var = Null;
			return;
		}
		if(ткст_ли(v)) {
			Ткст text = фильтруй(~v, CharFilterDigit);
			if(text.дайСчёт() >= 8) {
				Дата d;
				d.year = сканЦел(text.лево(4));
				d.month = сканЦел(text.середина(4, 2));
				d.day = сканЦел(text.середина(6, 2));
				if(d.пригоден()) {
					var = d;
					return;
				}
			}
		}
		throw JsonizeError("string expected for Дата значение");
	}
	else
		if(пусто_ли(var))
			io.уст(Null);
		else
			io.уст(фмт("%04d-%02d-%02d", var.year, var.month, var.day));
}

template<> void вДжейсон(ДжейсонВВ& io, Время& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(пусто_ли(v)) {
			var = Null;
			return;
		}
		if(ткст_ли(v)) {
			Ткст text = фильтруй(~v, CharFilterDigit);
			if(text.дайСчёт() >= 12) { //seconds may be missing
				Время tm;
				tm.year = сканЦел(text.лево(4));
				tm.month = сканЦел(text.середина(4, 2));
				tm.day = сканЦел(text.середина(6, 2));
				tm.hour = сканЦел(text.середина(8, 2));
				tm.minute = сканЦел(text.середина(10, 2));
				tm.second = text.дайСчёт() > 12 ? сканЦел(text.середина(12)) : 0;
				if(tm.пригоден()) {
					var = tm;
					return;
				}
			}
		}
		throw JsonizeError("string expected for Время значение");
	}
	else
		if(пусто_ли(var))
			io.уст(Null);
		else
			io.уст(фмт("%04d-%02d-%02d`T%02d:%02d:%02d",
				          var.year, var.month, var.day, var.hour, var.minute, var.second));
}

Ткст sJsonFile(const char *file)
{
	return file ? Ткст(file) : конфигФайл(дайТитулИсп() + ".json");
}

}
