#include "Core.h"

namespace РНЦПДинрус {

const Обнул Null;

#define LTIMING(x) // RTIMING(x)

ОшибкаТипаЗначения::ОшибкаТипаЗначения(const Ткст& text, const Значение& ист, int target)
:	Искл(text), ист(ист), target(target) {}

hash_t Значение::дайДрЗначХэш() const {
	if(пусто_ли())
		return 0;
	byte st = данные.GetSt();
	if(st == REF)
		return ptr()->дайХэшЗнач();
	return svo[st]->дайХэшЗнач(&данные);
}

void Значение::отпустиРеф()
{
	ПРОВЕРЬ(реф_ли()); // Check that svo тип is not registered as Реф
	ptr()->отпусти();
}

void Значение::RefRetain()
{
	ptr()->Retain();
}

Значение& Значение::operator=(const Значение& v) {
	if(this == &v) return *this;
	Значение h = v; // сделай copy a 'v' can be reference to МапЗнач/Массив contained element
	освободиРеф();   // e.g. json = json["foo"]
	данные = h.данные;
	if(реф_ли())
		ptr()->Retain();
	return *this;
}

void Значение::устЛардж(const Значение& v)
{
	if(v.реф_ли()) {
		данные.устСмолл(v.данные);
		RefRetain();
	}
	else
		данные.LSet(v.данные);
}

dword Значение::дайТип() const
{
	if(ткст_ли())
		return STRING_V;
	byte st = данные.GetSt();
	return st == REF ? дайРефТип() : st == VOIDV ? VOID_V : st;
}

bool Значение::пусто_ли() const
{
	if(ткст_ли())
		return данные.дайСчёт() == 0;
	int st = данные.GetSt();
	if(st == VOIDV)
		return true;
	if(st == REF)
		return ptr()->пусто_ли();
	return svo[st]->пусто_ли(&данные);
}

bool Значение::полиРавны(const Значение& v) const
{
	int st = данные.дайОсобый();
	if(st == REF)
		return ptr()->полиРавны(v);
	if(svo[st] && svo[st]->полиРавны(&данные, v))
		return true;
	return пусто_ли() && v.пусто_ли();
}

bool Значение::operator==(const Значение& v) const {
	if(ткст_ли() && v.ткст_ли())
		return данные == v.данные;
	if(дайТип() != v.дайТип()) {
		if(полиРавны(v) || v.полиРавны(*this))
			return true;
	}
	else {
		int st = данные.дайОсобый();
		if(st == REF) {
			if(ptr()->равен(v.ptr()))
				return true;
		}
		else
		if(st != VOIDV) {
			if(svo[st]->равен(&данные, &v.данные))
				return true;
		}
	}
	return пусто_ли() && v.пусто_ли();
}

int Значение::полиСравни(const Значение& v) const
{
	int st = данные.дайОсобый();
	if(st == REF)
		return ptr()->полиСравни(v);
	if(st != VOIDV)
		return svo[st]->полиСравни(&данные, v);
	return 0;
}

int Значение::сравни2(const Значение& v) const
{
	if(ткст_ли() && v.ткст_ли())
		return сравниЗнак(данные, v.данные);
	dword stw = данные.GetStW();
	if(stw == v.данные.GetStW()) {
		if(stw == Ткст::StW(INT64_V))
			return сравниЗнак(дайСыройСмолл<int64>(), v.дайСыройСмолл<int64>());
		if(stw == Ткст::StW(DATE_V))
			return сравниЗнак(дайСыройСмолл<Дата>(), v.дайСыройСмолл<Дата>());
		if(stw == Ткст::StW(TIME_V))
			return сравниЗнак(дайСыройСмолл<Время>(), v.дайСыройСмолл<Время>());
	}
	bool a = пусто_ли();
	bool b = v.пусто_ли();
	if(a || b)
		return сравниЗнак(b, a);
	int st = данные.дайОсобый();
	if(дайТип() == v.дайТип()) {
		if(st == REF)
			return ptr()->сравни(v.ptr());
		if(st != VOIDV)
			return svo[st]->сравни(&данные, &v.данные);
	}
	if(st != VOIDV) {
		int q = полиСравни(v);
		if(q) return q;
		return -v.полиСравни(*this);
	}
	return 0;
}

bool Значение::одинаково(const Значение& b) const
{
	const Значение& a = *this;
	if(a.является<МапЗнач>() && b.является<МапЗнач>())
		return МапЗнач(a).одинаково(МапЗнач(b));
	else
	if(a.является<МассивЗнач>() && b.является<МассивЗнач>()) {
		if(a.дайСчёт() != b.дайСчёт())
			return false;
		for(int i = 0; i < a.дайСчёт(); i++)
			if(!a[i].одинаково(b[i]))
				return false;
		return true;
	}
	else
		return a == b;
}

Значение::Значение(const ШТкст& s) { иницРеф(new RichValueRep<ШТкст>(s), WSTRING_V); Magic(); }

Значение::operator ШТкст() const
{
	if(пусто_ли()) return Null;
	return дайТип() == WSTRING_V ? To<ШТкст>() : ((Ткст)(*this)).вШТкст();
}

Дата Значение::дайДрДату() const
{
	if(пусто_ли()) return Null;
	return дайСмолл<Время>();
}

Время Значение::дайДрВремя() const
{
	if(пусто_ли()) return Null;
	return воВремя(дайСмолл<Дата>());
}

Ткст Значение::дайДрТкст() const
{
	if(пусто_ли()) return Null;
	if(является<Ткст>())
		return To<Ткст>();
	return To<ШТкст>().вТкст();
}

int Значение::дайДрЦел() const
{
	if(пусто_ли()) return Null;
	return данные.особый_ли(BOOL_V) ? (int)дайСмолл<bool>() :
	       данные.особый_ли(INT64_V) ? (int)дайСмолл<int64>() :
	       (int)дайСмолл<double>();
}

int64 Значение::дайДрЦел64() const
{
	if(пусто_ли()) return Null;
	return данные.особый_ли(BOOL_V) ? (int64)дайСмолл<bool>() :
	       данные.особый_ли(INT_V) ? (int64)дайСмолл<int>() :
	       (int64)дайСмолл<double>();
}

double Значение::дайДрДво() const
{
	if(пусто_ли()) return Null;
	return данные.особый_ли(BOOL_V) ? (double)дайСмолл<bool>() :
	       данные.особый_ли(INT_V) ? (double)дайСмолл<int>() :
	       (double)дайСмолл<int64>();
}

bool Значение::дайДрБул() const
{
	if(пусто_ли()) return Null;
	return данные.особый_ли(DOUBLE_V) ? (bool)дайСмолл<double>() :
	       данные.особый_ли(INT_V) ? (bool)дайСмолл<int>() :
	       (bool)дайСмолл<int64>();
}

ВекторМап<dword, Значение::Проц *(*)()>& Значение::Typemap()
{
	static ВекторМап<dword, Значение::Проц *(*)()> x;
	return x;
}

Индекс<Ткст>& Значение::индексИмени()
{
	static Индекс<Ткст> x;
	return x;
}

Индекс<dword>& Значение::индексТипа()
{
	static Индекс<dword> x;
	return x;
}

void Значение::добавьИмя(dword тип, const char *имя)
{
	индексИмени().добавь(имя);
	индексТипа().добавь(тип);
}

int Значение::дайТип(const char *имя)
{
	int q = индексИмени().найди(имя);
	if(q < 0)
		return Null;
	return индексТипа()[q];
}

Ткст Значение::дайИмя(dword тип)
{
	int q = индексТипа().найди(тип);
	if(q < 0)
		return Null;
	return индексИмени()[q];
}

SVO_FN(s_String, Ткст);
SVO_FN(s_int, int);
SVO_FN(s_double, double);
SVO_FN(s_int64, int64);
SVO_FN(s_bool, bool);
SVO_FN(s_date, Дата);
SVO_FN(s_time, Время);

struct SvoVoidFn {
	static bool       пусто_ли(const void *p)                      { return true; }
	static void       сериализуй(void *p, Поток& s)              {}
	static void       вРяр(void *p, РярВВ& xio)               {}
	static void       вДжейсон(void *p, ДжейсонВВ& jio)             {}
	static hash_t     дайХэшЗнач(const void *p)                { return 0; }
	static bool       равен(const void *p1, const void *p2)    { return true; }
	static bool       полиРавны(const void *p, const Значение& v) { return false; }
	static Ткст     какТкст(const void *p)                    { return Ткст(); }
};

static Значение::Sval s_void = {
	SvoVoidFn::пусто_ли, SvoVoidFn::сериализуй,SvoVoidFn::вРяр, SvoVoidFn::вДжейсон,
	SvoVoidFn::дайХэшЗнач, SvoVoidFn::равен,
	SvoVoidFn::полиРавны, SvoVoidFn::какТкст
};

Значение::Sval *Значение::svo[256] = {
	&s_String, // STRING_V
	&s_int, // INT_V

	&s_double, //DOUBLE_V  = 2;
	&s_void, //VOIDV_V  = 3;
	&s_date, //DATE_V    = 4;
	&s_time, //TIME_V    = 5;

	NULL, //ERROR_V   = 6;

	NULL, //VALUE_V   = 7;

	NULL, //WSTRING_V = 8;

	NULL, //VALUEARRAY_V = 9;

	&s_int64, //INT64_V  = 10;
	&s_bool, //BOOL_V   = 11;

	NULL, //VALUEMAP_V   = 12;
};

Значение::Проц *создайДанМассивЗач()
{
	return new МассивЗнач::Данные;
}

Значение::Проц *создайДанМапЗач()
{
	return new МапЗнач::Данные;
}

void Значение::регистрируйСтд()
{
	ONCELOCK {
		Значение::регистрируй<ШТкст>("ШТкст");
		Значение::регистрируй<Комплекс>("Комплекс");
		Значение::регистрируй(VALUEARRAY_V, создайДанМассивЗач, "МассивЗнач");
		Значение::регистрируй(VALUEMAP_V, создайДанМапЗач, "МапЗнач");
		Значение::добавьИмя(STRING_V, "Ткст");
		Значение::добавьИмя(INT_V, "int");
		Значение::добавьИмя(DOUBLE_V, "double");
		Значение::добавьИмя(VOID_V, "void");
		Значение::добавьИмя(DATE_V, "Дата");
		Значение::добавьИмя(TIME_V, "Время");
		Значение::добавьИмя(INT64_V, "int64");
		Значение::добавьИмя(BOOL_V, "bool");
		Значение::добавьИмя(ERROR_V, "Ошибка");
	};
}

void ValueRegisterHelper()
{
	Значение::регистрируйСтд();
}

ИНИЦБЛОК {
	ValueRegisterHelper();
}

void Значение::сериализуй(Поток& s) {
	регистрируйСтд();
	dword тип;
	if(s.грузится()) {
		s / тип;
		if(тип >= 0x8000000)
			s.загрузиОш();
		освободи();
		int st = тип == VOID_V ? VOIDV : тип == STRING_V ? STRING : тип;
		if(st == STRING)
			s % данные;
		else
		if(st >= 0 && st < 255 && svo[st]) {
			данные.устОсобо((byte)тип);
			svo[st]->сериализуй(&данные, s);
		}
		else {
			typedef Проц* (*vp)();
			vp *cr = Typemap().найдиУк(тип);
			if(cr) {
				Проц *p = (**cr)();
				p->сериализуй(s);
				иницРеф(p, тип);
			}
			else {
				освободи();
				данные.устОсобо(3);
				if(тип != VOID_V && тип != ERROR_V)
					s.загрузиОш();
			}
		}
	}
	else {
		тип = дайТип();
		ПРОВЕРЬ(тип < 0x8000000); // only Values with assigned real тип ИД can be serialized
		s / тип;
		int st = данные.дайОсобый();
		ПРОВЕРЬ_(!тип || тип == ERROR_V || тип == UNKNOWN_V || st == STRING ||
		        (реф_ли() ? Typemap().найди(тип) >= 0 : st < 255 && svo[st]),
		        дайИмя() + " не зарегистрировано для сериализации");
		if(st == VOIDV)
			return;
		if(st == STRING)
			s % данные;
		else
		if(реф_ли())
			ptr()->сериализуй(s);
		else
			svo[st]->сериализуй(&данные, s);
	}
}

static Ткст s_binary("serialized_binary");

void Значение::вРяр(РярВВ& xio)
{
	регистрируйСтд();
	if(xio.сохраняется()) {
		if(ошибка_ли()) {
			Значение v = Null;
			v.вРяр(xio);
			return;
		}
		dword тип = дайТип();
		Ткст имя = дайИмя(тип);
		if(имя.дайСчёт() == 0) {
			xio.устАтр("тип", s_binary);
			Ткст s = гексТкст(сохраниКакТкст(*this));
			РНЦПДинрус::вРяр(xio, s);
		}
		else {
			xio.устАтр("тип", имя);
			int st = данные.дайОсобый();
			ПРОВЕРЬ_(!тип || тип == ERROR_V || тип == UNKNOWN_V || st == STRING ||
			        (реф_ли() ? Typemap().найди(тип) >= 0 : st < 255 && svo[st]),
			        дайИмя() + " не зарегистрировано для xml-изации");
			if(st == VOIDV)
				return;
			if(st == STRING)
				РНЦПДинрус::вРяр(xio, данные);
			else
			if(реф_ли())
				ptr()->вРяр(xio);
			else
				svo[st]->вРяр(&данные, xio);
		}
	}
	else {
		Ткст имя = xio.дайАтр("тип");
		if(РНЦПДинрус::пусто_ли(имя))
			*this = Значение();
		else
		if(имя == s_binary) {
			Ткст s;
			РНЦПДинрус::вРяр(xio, s);
			try {
				грузиИзТкст(*this, сканГексТкст(s));
			}
			catch(LoadingError) {
				throw ОшибкаРяр("xmlize serialized_binary Ошибка");
			}
		}
		else {
			int тип = дайТип(имя);
			if(РНЦПДинрус::пусто_ли(тип))
				throw ОшибкаРяр("неверное значение типа");
			освободи();
			int st = (dword)тип == VOID_V ? VOIDV : (dword)тип == STRING_V ? STRING : тип;
			if(st == STRING)
				РНЦПДинрус::вРяр(xio, данные);
			else
			if(st < 255 && svo[st]) {
				данные.устОсобо((byte)тип);
				svo[st]->вРяр(&данные, xio);
			}
			else {
				typedef Проц* (*vp)();
				vp *cr = Typemap().найдиУк(тип);
				if(cr) {
					Проц *p = (**cr)();
					p->вРяр(xio);
					иницРеф(p, тип);
				}
				else
					throw ОшибкаРяр("неверное значение типа");
			}
		}
	}
}

void Значение::вДжейсон(ДжейсонВВ& jio)
{
	регистрируйСтд();
	if(jio.сохраняется()) {
		if(пусто_ли())
			jio.уст(Null);
		else {
			dword тип = дайТип();
			Ткст имя = дайИмя(тип);
			if(имя.дайСчёт() == 0) {
				Ткст s = гексТкст(сохраниКакТкст(*this));
				jio("тип", s_binary)
				   ("значение", s);
			}
			else {
				int st = данные.дайОсобый();
				ПРОВЕРЬ_(!тип || тип == ERROR_V || тип == UNKNOWN_V || st == STRING ||
				        (реф_ли() ? Typemap().найди(тип) >= 0 : st < 255 && svo[st]),
				        дайИмя() + " не зарегистрировано для json-изации");
				if(st == VOIDV)
					return;
				ДжейсонВВ hio;
				if(st == STRING) {
					Ткст h = данные;
					РНЦПДинрус::вДжейсон(hio, h);
				}
				else {
					if(реф_ли())
						ptr()->вДжейсон(hio);
					else
						svo[st]->вДжейсон(&данные, hio);
				}
				МапЗнач m;
				m.добавь("тип", имя);
				m.добавь("значение", hio.дайРез());
				jio.уст(m);
			}
		}
	}
	else {
		Значение g = jio.дай();
		if(g.пусто_ли())
			*this = Null;
		else {
			Ткст имя = g["тип"];
			Значение  val = g["значение"];
			if(имя == s_binary) {
				if(!РНЦПДинрус::ткст_ли(val))
					throw JsonizeError("serialized_binary Ошибка");
				Ткст s = val;
				try {
					грузиИзТкст(*this, сканГексТкст(s));
				}
				catch(LoadingError) {
					throw JsonizeError("serialized_binary Ошибка");
				}
			}
			else {
				int тип = дайТип(имя);
				if(РНЦПДинрус::пусто_ли(тип))
					throw JsonizeError("invalid Значение тип");
				освободи();
				int st = (dword)тип == VOID_V ? VOIDV : (dword)тип == STRING_V ? STRING : тип;
				if(st == STRING) {
					if(!РНЦПДинрус::ткст_ли(val))
						throw JsonizeError("serialized_binary Ошибка");
					данные = val;
				}
				else {
					ДжейсонВВ hio(val);
					if(st < 255 && svo[st]) {
						данные.устОсобо((byte)тип);
						svo[st]->вДжейсон(&данные, hio);
					}
					else {
						typedef Проц* (*vp)();
						vp *cr = Typemap().найдиУк(тип);
						if(cr) {
							Проц *p = (**cr)();
							p->вДжейсон(hio);
							иницРеф(p, тип);
						}
						else
							throw JsonizeError("invalid Значение тип");
					}
				}
			}
		}
	}
}

void Значение::регистрируй(dword w, Проц* (*c)(), const char *имя) {
#ifdef flagCHECKINIT
	RLOG("регистрируй valuetype " << w);
#endif
	проверьНаОН(); // all needs to be registered at file level scope
	ПРОВЕРЬ(w != UNKNOWN_V);
	ПРОВЕРЬ(w < 0x8000000);
	CHECK(Typemap().дайДобавь(w, c) == c);
	добавьИмя(w, имя);
}

Ткст  Значение::вТкст() const {
	if(пусто_ли())
		return Null;
	if(ткст_ли())
		return данные;
	if(реф_ли())
		return ptr()->какТкст();
	int st = данные.дайОсобый();
	return svo[st]->какТкст(&данные);
}

int Значение::дайСчёт() const
{
	if(реф_ли()) {
		dword t = дайРефТип();
		if(t == VALUEARRAY_V)
			return ((МассивЗнач::Данные *)ptr())->данные.дайСчёт();
		if(t == VALUEMAP_V)
			return ((МапЗнач::Данные *)ptr())->значение.дайСчёт();
	}
	return 0;
}

const Значение& Значение::operator[](int i) const
{
	if(реф_ли()) {
		dword t = дайРефТип();
		if(t == VALUEARRAY_V)
			return ((МассивЗнач::Данные *)ptr())->данные[i];
		if(t == VALUEMAP_V)
			return ((МапЗнач::Данные *)ptr())->значение[i];
	}
	return значОш();
}

const Вектор<Значение>& Значение::GetVA() const
{
	if(реф_ли()) {
		if(является<МассивЗнач>())
			return ((МассивЗнач::Данные *)ptr())->данные;
		if(является<МапЗнач>())
			return ((МапЗнач::Данные *)ptr())->значение.данные->данные;
	}
	return МассивЗнач::VoidData;
}

force_inline
Вектор<Значение>& Значение::UnShareArray()
{
	МассивЗнач::Данные *данные = (МассивЗнач::Данные *)ptr();
	if(данные->GetRefCount() != 1) {
		МассивЗнач::Данные *d = new МассивЗнач::Данные;
		d->данные = clone(данные->данные);
		данные->отпусти();
		ptr() = d;
		данные = d;
	}
	return данные->данные;
}

Значение& Значение::по(int i)
{
	if(пусто_ли())
		*this = МассивЗнач();
	ПРОВЕРЬ(i >= 0 && реф_ли());
	dword t = дайРефТип();
	if(t == VALUEMAP_V) {
		МассивЗнач& va = МапЗнач::UnShare((МапЗнач::Данные*&)ptr()).значение;
		ПРОВЕРЬ(i < va.дайСчёт());
		return va.по(i);
	}
	ПРОВЕРЬ(t == VALUEARRAY_V);
	return UnShareArray().по(i);
}

void Значение::добавь(const Значение& s)
{
	if(пусто_ли()) {
		if(реф_ли()) отпустиРеф();
		МассивЗнач::Данные *d = new МассивЗнач::Данные;
		d->данные.добавь(s);
		иницРеф(d, VALUEARRAY_V);
		Magic();
		return;
	}
	ПРОВЕРЬ(реф_ли() && дайРефТип() == VALUEARRAY_V);
	UnShareArray().добавь(s);
}

const Значение& Значение::operator[](const Ткст& ключ) const
{
	if(реф_ли() && дайРефТип() == VALUEMAP_V)
		return ((МапЗнач::Данные *)ptr())->дай(ключ);
	return значОш();
}

Значение& Значение::дайДобавь(const Значение& ключ)
{
	if(пусто_ли()) {
		if(реф_ли()) отпустиРеф();
		МапЗнач::Данные *d = new МапЗнач::Данные;
		Значение& h = d->дайДобавь(ключ);
		иницРеф(d, VALUEMAP_V);
		Magic();
		return h;
	}
	if(дайТип() == VALUEARRAY_V) {
		МапЗнач m = *this;
		*this = m;
	}
	ПРОВЕРЬ(дайТип() == VALUEMAP_V);
	return МапЗнач::UnShare((МапЗнач::Данные*&)ptr()).дайДобавь(ключ);
}

Значение& Значение::operator()(const Ткст& ключ)
{
	return дайДобавь(ключ);
}

Значение& Значение::operator()(const char *ключ)
{
	return дайДобавь(ключ);
}

Значение& Значение::operator()(const Ид& ключ)
{
	return дайДобавь(~ключ);
}

Ткст Значение::дайИмя() const
{
	if(реф_ли()) {
		Проц *p = ptr(); // supress CLANG warning
		return typeid(*p).name();
	}
	if(ткст_ли())
		return "Ткст";
	static Кортеж<byte, const char *> tp[] = {
		{ (byte)INT_V, "int" },
		{ (byte)DOUBLE_V, "double" },
		{ (byte)VOIDV, "void" },
		{ (byte)DATE_V, "Дата" },
		{ (byte)TIME_V, "Время" },
		{ (byte)INT64_V, "int64" },
		{ (byte)BOOL_V, "bool" },
	};
	Кортеж<byte, const char *> *x = найдиКортеж(tp, __countof(tp), данные.дайОсобый());
	return x ? Ткст(x->b) : какТкст(дайТип());
}

class ValueErrorCls : public RichValueRep<Ткст> {
public:
	virtual dword      дайТип() const             { return ERROR_V; }
	virtual bool       пусто_ли() const              { return true; }
	virtual void       сериализуй(Поток& s)        {}
	virtual Ткст     какТкст() const            { return "<Ошибка: \'" + v + "\'>"; }

	ValueErrorCls(const Ткст& s) : RichValueRep<Ткст>(s)  {}
};

Значение значОш(const Ткст& s) {
	return Значение(new ValueErrorCls(s), ERROR_V);
}

Значение значОш(const char *s) {
	return значОш(Ткст(s));
}

const Значение& значОш() {
	static Значение v = значОш(Ткст());
	return v;
}

Ткст дайТекстОш(const Значение& v) {
	ПРОВЕРЬ(ошибка_ли(v));
	return ((RichValueRep<Ткст> *)v.дайПроцУк())->дай();
}

}
