#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

Значение ParseJSON(СиПарсер& p)
{
	p.искейпЮникод();
	if(p.дво_ли())
		return p.читайДво();
	if(p.ткст_ли()) {
		бул dt = p.сим2_ли('\"', '\\');
		Ткст s = p.читайТкст();
		if(dt) {
			СиПарсер p(s);
			if(p.сим('/') && p.ид("Дата") && p.сим('(') && p.цел_ли()) {
				дол n = p.читайЦел64();
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

Значение ParseJSON(кткст0 s)
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

Json& Json::CatRaw(кткст0 ключ, const Ткст& знач)
{
	if(text.дайСчёт())
		text << ',';
	text << AsJSON(ключ) << ":" << знач;
	return *this;
}

JsonArray& JsonArray::CatRaw(const Ткст& знач)
{
	if(text.дайСчёт())
		text << ',';
	text << знач;
	return *this;
}

Ткст AsJSON(const Значение& v, const Ткст& sep, бул pretty)
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
		for(цел i = 0; i < m.дайСчёт(); i++) {
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
		for(цел i = 0; i < va.дайСчёт(); i++) {
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
	if(число_ли(v) && (пусто_ли(v) || нч_ли((дво)v)))
		return "null";
	if(v.дайТип() == INT_V)
		return фмт("%d", (цел)v);
	if(v.дайТип() == BOOL_V)
		return (бул)v ? "true" : "false";
	if(число_ли(v))
		return фмтГ((дво)v, 17);
	if(ткст_ли(v))
		return какТкстСи((Ткст)v, INT_MAX, NULL, ASCSTRING_JSON);
	if(датаВремя_ли(v))
		return AsJSON((Время)v);
	if(пусто_ли(v))
		return "null";
	НИКОГДА_("Non-JSON значение in AsJSON: " + v.дайИмяТипа());
	return "null";
}

проц ДжейсонВВ::уст(кткст0 ключ, const Значение& v)
{
	ПРОВЕРЬ(сохраняется());
	if(!map)
		map.создай();
	map->добавь(ключ, v);
}

Ткст AsJSON(const Значение& v, бул pretty)
{
	return AsJSON(v, Ткст(), pretty);
}

template<> проц вДжейсон(ДжейсонВВ& io, дво& var)
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
			дво h = сканДво((Ткст)v);
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

template<> проц вДжейсон(ДжейсонВВ& io, цел& var)
{
	дво v = целДво(var);
	вДжейсон(io, v);
	if(io.грузится()) {
		if(пусто_ли(v))
			var = Null;
		else
		if(v >= INT_MIN && v <= INT_MAX && (цел)v == v)
			var = (цел)v;
		else
			throw JsonizeError("number is not integer");
	}
}

template<> проц вДжейсон(ДжейсонВВ& io, ббайт& var)
{
	дво v = var;
	вДжейсон(io, v);
	if(io.грузится()) {
		if(v >= 0 && v <= 255 && (цел)v == v)
			var = (ббайт)v;
		else
			throw JsonizeError("integer 0-255 expected");
	}
}

template<> проц вДжейсон(ДжейсонВВ& io, крат& var)
{
	дво v = var;
	вДжейсон(io, v);
	if(io.грузится()) {
		if(v >= -32768 && v <= 32767 && (цел)v == v)
			var = (крат)v;
		else
			throw JsonizeError("16-bit integer expected");
	}
}

template<> проц вДжейсон(ДжейсонВВ& io, бул& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(число_ли(v) && !пусто_ли(v))
			var = (бул)v;
		else
			throw JsonizeError("boolean expected");
	}
	else
		io.уст(var);
}

template<> проц вДжейсон(ДжейсонВВ& io, дол& var)
{
	if(io.грузится()) {
		const Значение& v = io.дай();
		if(пусто_ли(v)) {
			var = Null;
			return;
		}
		if(v.является<дол>() || v.является<цел>()) {
			var = v;
			return;
		}
		if(число_ли(v)) {
			дво d = v;
			if(вмещаетсяВЦел64(d)) {
				var = (дол)d;
				return;
			}
		}
		else
		if(ткст_ли(v)) {
			дол h = сканЦел64((Ткст)v);
			if(!пусто_ли(h)) {
				var = h;
				return;
			}
		}
		throw JsonizeError("invalid дол значение");
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

template<> проц вДжейсон(ДжейсонВВ& io, Ткст& var)
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

template<> проц вДжейсон(ДжейсонВВ& io, ШТкст& var)
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

template<> проц вДжейсон(ДжейсонВВ& io, Дата& var)
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

template<> проц вДжейсон(ДжейсонВВ& io, Время& var)
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

Ткст sJsonFile(кткст0 file)
{
	return file ? Ткст(file) : конфигФайл(дайТитулИсп() + ".json");
}

}
