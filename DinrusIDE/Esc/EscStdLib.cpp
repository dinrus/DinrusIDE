#include <Esc/Esc.h>


namespace РНЦП {

#define LTIMING(x)  // RTIMING(x)

void ESC_count(EscEscape& e)
{
	e = e[0].дайСчёт();
}

void ESC_is_void(EscEscape& e)
{
	e = e[0].проц_ли();
}

void ESC_is_number(EscEscape& e)
{
	e = e[0].число_ли();
}

void ESC_is_array(EscEscape& e)
{
	e = e[0].IsArray();
}

void ESC_is_map(EscEscape& e)
{
	e = e[0].IsMap();
}

void ESC_int(EscEscape& e)
{
	if(e[0].число_ли())
		e = (int)e[0].GetNumber();
}

void ESC_to_string(EscEscape& e)
{
	e = e[0].вТкст();
}

void ESC_to_number(EscEscape& e)
{
	if(e[0].IsArray()) {
		double d = сканДво((Ткст)e[0]);
		if(!пусто_ли(d)) {
			if(FitsInInt64(d))
				e = сканЦел64((Ткст)e[0]);
			else
				e = d;
		}
	}
}

void ESC_rand(EscEscape& e)
{
	e = rand();
}

void ESC_keys(EscEscape& e)
{
	e.CheckMap(0);
	EscValue v;
	const ВекторМап<EscValue, EscValue>& m = e[0].дайМап();
	for(int i = 0; i < m.дайСчёт(); i++)
		if(!m.отлинкован(i))
			v.ArrayAdd(m.дайКлюч(i));
	e = v;
}

void ESC_values(EscEscape& e)
{
	e.CheckMap(0);
	EscValue v;
	const ВекторМап<EscValue, EscValue>& m = e[0].дайМап();
	for(int i = 0; i < m.дайСчёт(); i++)
		if(!m.отлинкован(i))
			v.ArrayAdd(m[i]);
	e = v;
}

void ESC_reverse(EscEscape& e)
{
	e.CheckArray(0);
	const Вектор<EscValue>& a = e[0].дайМассив();
	EscValue r;
	for(int i = a.дайСчёт() - 1; i >= 0; i--)
		r.ArrayAdd(a[i]);
	e = r;
}

struct EscCmp {
	Esc *esc;
	bool operator()(const EscValue& a, const EscValue& b) const {
		return esc->DoCompare(a, b, "< (sort)") < 0;
	}
};

Вектор<int> EscGetSortOrder(EscEscape& e)
{
	e.CheckArray(0);
	const Вектор<EscValue>& va = e[0].дайМассив();
	EscCmp сравни;
	сравни.esc = &e.esc;
	return GetSortOrder(va, сравни);
}

void ESC_sort(EscEscape& e)
{
	Вектор<int> so = EscGetSortOrder(e);
	EscValue r;
	for(int i = 0; i < so.дайСчёт(); i++)
		r.ArrayAdd(e[0].ArrayGet(so[i]));
	e = r;
}

void ESC_order(EscEscape& e)
{
	Вектор<int> so = EscGetSortOrder(e);
	EscValue r;
	for(int i = 0; i < so.дайСчёт(); i++)
		r.ArrayAdd(so[i]);
	e = r;
}

// ---------------------------

void ESC_mid(EscEscape& e)
{
	e.CheckArray(0);
	int pos = e.Цел(1);
	int count = e.Цел(2);
	if(pos < 0 || pos + count > e[0].дайСчёт())
		e.выведиОш("out of bounds in call to 'mid'");
	e.ret_val = e[0].ArrayGet(pos, count);
}

void ESC_exists(EscEscape& e)
{
	e.CheckMap(0);
	e = !e[0].MapGet(e[1]).проц_ли();
}

struct ESC_FileOut : public EscHandle {
	ФайлПоток file;
	void помести(EscEscape& e)         { if(file) file.помести(e.Цел(0)); }
	void PutLine(EscEscape& e)     { if(file) file.PutLine(Ткст(e[0])); }
	void закрой(EscEscape& e)       { if(file) file.закрой(); }

	typedef ESC_FileOut ИМЯ_КЛАССА;

	ESC_FileOut(EscEscape& e, EscValue& v, int style) {
		file.открой(Ткст(e[0]), style);
		v.Escape("помести(a)", this, THISBACK(помести));
		v.Escape("PutLine(a)", this, THISBACK(PutLine));
		v.Escape("закрой()", this, THISBACK(закрой));
	}
};

void ESC_OpenFileOut(EscEscape& e)
{
	EscValue v;
	ESC_FileOut *f = new ESC_FileOut(e, v, ФайлПоток::CREATE);
	if(f->file)
		e = v;
}

void ESC_OpenFileAppend(EscEscape& e)
{
	EscValue v;
	ESC_FileOut *f = new ESC_FileOut(e, v, ФайлПоток::APPEND);
	if(f->file)
		e = v;
}

struct ESC_FileIn : public EscHandle {
	ФайлВвод file;
	void кф_ли(EscEscape& e)       { e = file.кф_ли(); }
	void дай(EscEscape& e)         { e = file.дай(); }
	void дайСтроку(EscEscape& e)     { e = file.дайСтроку(); }
	void закрой(EscEscape& e)       { if(file) file.закрой(); }

	typedef ESC_FileIn ИМЯ_КЛАССА;

	ESC_FileIn(EscEscape& e, EscValue& v) {
		file.открой(Ткст(e[0]));
		v.Escape("кф_ли()", this, THISBACK(кф_ли));
		v.Escape("дай()", this, THISBACK(дай));
		v.Escape("дайСтроку()", this, THISBACK(дайСтроку));
		v.Escape("закрой()", this, THISBACK(закрой));
	}
};

void ESC_OpenFileIn(EscEscape& e)
{
	EscValue v;
	ESC_FileIn *f = new ESC_FileIn(e, v);
	if(f->file)
		e = v;
}

// ---------------------------

bool IsDate(const EscValue& v)
{
	return v.HasNumberField("year") && v.HasNumberField("month") && v.HasNumberField("day");
}

bool IsTime(const EscValue& v)
{
	return IsDate(v) && v.HasNumberField("hour") && v.HasNumberField("minute") && v.HasNumberField("second");
}

void SIC_IsDate(EscEscape& e)
{
	e = IsDate(e[0]);
}

void SIC_IsTime(EscEscape& e)
{
	e = IsTime(e[0]);
}

void SIC_GetSysTime(EscEscape& e)
{
	Время tm = дайСисВремя();
	EscValue v;
	v.MapSet("year", (int)tm.year);
	v.MapSet("month", (int)tm.month);
	v.MapSet("day", (int)tm.day);
	v.MapSet("hour", (int)tm.hour);
	v.MapSet("minute", (int)tm.minute);
	v.MapSet("second", (int)tm.second);
	e = v;
}

void ESC_ToLower(EscEscape& e)
{
	Ткст s = e[0];
	e = впроп(s);
}

void ESC_ToUpper(EscEscape& e)
{
	Ткст s = e[0];
	e = взаг(s);
}

// ---------------------------

void ESC_replace(EscEscape& e)
{
	Ткст str = e[0];
	str.замени(e[1], e[2]);
	e = str;
}

// ---------------------------

void StdLib(МассивМап<Ткст, EscValue>& global)
{
	Escape(global, "is_number(значение)", ESC_is_number);
	Escape(global, "is_array(значение)", ESC_is_array);
	Escape(global, "is_map(значение)", ESC_is_map);
	Escape(global, "is_void(значение)", ESC_is_void);
	Escape(global, "int(значение)", ESC_int);
	Escape(global, "to_string(значение)", ESC_to_string);
	Escape(global, "to_number(значение)", ESC_to_number);
	Escape(global, "count(значение)", ESC_count);
	Escape(global, "keys(map)", ESC_keys);
	Escape(global, "values(map)", ESC_values);
	Escape(global, "rand()", ESC_rand);
	Escape(global, "reverse(array)", ESC_reverse);
	Escape(global, "sort(array)", ESC_sort);
	Escape(global, "order(array)", ESC_order);
	
	Escape(global, "replace(str, find, replace)", ESC_replace);
	
	Escape(global, "взаг(значение)", ESC_ToUpper);
	Escape(global, "впроп(значение)", ESC_ToLower);
	
	Escape(global, "len(x)", ESC_count);
	Escape(global, "mid(array, pos, count)", ESC_mid);
	Escape(global, "exists(map, ключ)", ESC_exists);

	Escape(global, "OpenFileOut(x)", ESC_OpenFileOut);
	Escape(global, "OpenFileAppend(x)", ESC_OpenFileOut);
	Escape(global, "OpenFileIn(x)", ESC_OpenFileIn);

	Escape(global, "дайСисВремя()", SIC_GetSysTime);
	Escape(global, "IsDate(x)", SIC_IsDate);
	Escape(global, "IsTime(x)", SIC_IsTime);

	#define FN(фн) Escfn(global, #фн "(значение)", [](EscEscape& e) { e = фн(e[0].GetNumber()); });
	
	FN(sin)
	FN(cos)
	FN(tan)
	FN(asin)
	FN(acos)
	FN(atan)

	FN(sinh)
	FN(cosh)
	FN(tanh)
	FN(asinh)
	FN(acosh)
	FN(atanh)
	
	FN(exp)
	FN(log)
	FN(log10)
	FN(exp2)
	FN(log2)
	FN(sqrt)
	FN(cbrt)
	
	#undef FN
	
	Escfn(global, "pow(base, exponent)", [](EscEscape& e) { e = pow(e[0].GetNumber(), e[1].GetNumber()); });
	Escfn(global, "atan2(a, b)", [](EscEscape& e) { e = atan2(e[0].GetNumber(), e[1].GetNumber()); });
}

}
