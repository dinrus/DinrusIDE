#include <DinrusPro/DinrusCore.h>

проц ТкстБуф::длинтекс()
{
	устДлину((цел)длинтекс__(pbegin));
}

проц ТкстБуф::кат(кткст0 s, цел l)
{
	if(pend + l > limit)
		переразместиД(s, l);
	else {
		копирпам8(pend, (кук)s, l);
		pend += l;
	}
}

проц ТкстБуф::кат(цел c, цел l)
{
	if(pend + l > limit)
		переразместиД(NULL, l);
	устпам8(pend, c, l);
	pend += l;
}

проц ТкстБуф::кат(кткст0 s)
{
	кат(s, (цел)длинтекс__(s));
}


проц Ткст0::уст(кткст0 s, цел len)
{
	очисть();
	if(len <= 14) {
		копирпам8(chr, s, len);
		мДлин() = len;
		Dsyn();
		return;
	}
	устБ(s, len);
	Dsyn();
}

проц Ткст0::уст0(кткст0 s, цел len)
{
	обнули();
	if(len <= 14) {
		мДлин() = len;
		копирпам8(chr, s, len);
		Dsyn();
		return;
	}
	устБ(s, len);
	Dsyn();
}

бул Ткст0::равен(кткст0 s) const
{ // This is optimized for comparison with string literals...
	т_мера len = strlen(s);
	кук p;
	if(малый()) {
		if(len > 14 || len != (т_мера)chr[МДЛИН]) return false; // len > 14 fixes issue with GCC warning, might improves performance too
		p = chr;
	}
	else {
		if(len != (т_мера)w[БДЛИН]) return false;
		p = укз;
	}
	return memcmp(p, s, len) == 0; // compiler is happy to optimize memcmp out with up to 64bit comparisons for literals...
}

цел Ткст0::сравни(const Ткст0& s) const
{
#ifdef CPU_LE
	if((chr[РОД] | s.chr[РОД]) == 0) { // both strings are small, len is the MSB
	#ifdef CPU_64
		бдол a = эндианРазворот64(q[0]);
		бдол b = эндианРазворот64(s.q[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот64(q[1]);
		b = эндианРазворот64(s.q[1]);
		return a == b ? 0 : a < b ? -1 : 1;
	#else
		бдол a = эндианРазворот32(w[0]);
		бдол b = эндианРазворот32(s.w[0]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[1]);
		b = эндианРазворот32(s.w[1]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[2]);
		b = эндианРазворот32(s.w[2]);
		if(a != b)
			return a < b ? -1 : 1;
		a = эндианРазворот32(w[3]);
		b = эндианРазворот32(s.w[3]);
		return a == b ? 0 : a < b ? -1 : 1;
	#endif
	}
#endif
	return сравниБ(s);
}

Ткст& Ткст::operator=(кткст0 s)
{
	присвойДлин(s, длинтекс__(s));
	return *this;
}

форс_инлайн
Ткст::Ткст(кткст0 s)
{
	Ткст0::уст0(s, длинтекс__(s));
}

проц Ткст0::разверни(Ткст0& b)
{
	дим a0 = q[0]; // Explicit код_ (instead of разверни) seems to emit some SSE2 код_ with CLANG
	дим a1 = q[1];
	дим b0 = b.q[0];
	дим b1 = b.q[1];
	q[0] = b0;
	q[1] = b1;
	b.q[0] = a0;
	b.q[1] = a1;
	Dsyn(); b.Dsyn();
}
///////////////

проц соДелай(Функция<проц ()>&& фн)
{
	СоРабота co;
	co * фн;
}


проц соДелай_ОН(Функция<проц ()>&& фн)
{
	фн();
}


проц соДелай(бул co, Функция<проц ()>&& фн)
{
	if(co)
		соДелай(пикуй(фн));
	else
		соДелай_ОН(пикуй(фн));
}

 бул iscib(сим c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '$';
}

 бул iscid(сим c) {
	return iscib(c) || c >= '0' && c <= '9';
}

 бул СиПарсер::Сим(сим c)
{
	if(сим_ли(c)) {
		term++;
		сделайПробелы();
		return true;
	}
	return false;
}

 бул СиПарсер::сим2(сим c1, сим c2)
{
	if(сим2_ли(c1, c2)) {
		term += 2;
		сделайПробелы();
		return true;
	}
	return false;
}

 бул СиПарсер::сим3(сим c1, сим c2, сим c3)
{
	if(сим3_ли(c1, c2, c3)) {
		term += 3;
		сделайПробелы();
		return true;
	}
	return false;
}

Ткст дайИмяТипа(const ::std::type_info& tinfo)   { return дайИмяТипа(tinfo.name()); }

#ifndef КУЧА_РНЦП
Ткст какТкст(const ПрофильПамяти&) { return "Используется malloc - профили памяти отсутствуют"; }
#endif

цел LOG_ROTATE(цел x) { return x << 24; }

Ткст какДжейсон(цел i)             { return пусто_ли(i) ? "null" : какТкст(i); }
Ткст какДжейсон(дво n)          { return пусто_ли(n) ? "null" : какТкст(n); }
Ткст какДжейсон(бул b)            { return b ? "true" : "false"; }
Ткст какДжейсон(const Ткст& s)   { return какТкстСи(s, INT_MAX, NULL, ASCSTRING_JSON); }
Ткст какДжейсон(const ШТкст& s)  { return какТкстСи(s.вТкст(), INT_MAX, NULL, ASCSTRING_JSON); }
Ткст какДжейсон(кткст0 s)     { return какТкстСи(s, INT_MAX, NULL, ASCSTRING_JSON); }

бул номТипЗначПроц_ли(цел q)     { return (бцел)q == VOID_V; }
бул номТипЗначОш_ли(цел q)    { return (бцел)q == ERROR_V; }
бул номТипЗначТкст_ли(цел q)   { return (бцел)q == STRING_V || (бцел)q == WSTRING_V; }

бул номТипЗначЦел_ли(цел q)  { return (бцел)q == INT_V || (бцел)q == INT64_V || (бцел)q == BOOL_V; }
бул номТипЗначПлав_ли(цел q)    { return (бцел)q == DOUBLE_V; }

бул номТипЗначЧисло_ли(цел q)   { return номТипЗначЦел_ли(q) || номТипЗначПлав_ли(q); }
бул номТипЗначДатаВремя_ли(цел q) { return (бцел)q == DATE_V || (бцел)q == TIME_V; }

бул проц_ли(const Значение& v)       { return v.проц_ли(); }
бул ошибка_ли(const Значение& v)      { return v.ошибка_ли(); }
бул ткст_ли(const Значение& v)     { return v.является<Ткст>() || v.является<ШТкст>(); }
бул число_ли(const Значение& v)     { return v.является<дво>() || v.является<цел>() || v.является<дол>() || v.является<бул>(); }
бул датаВремя_ли(const Значение& v)   { return v.является<Дата>() || v.является<Время>(); }
бул массивЗнач_ли(const Значение& v) { return v.дайТип() == VALUEARRAY_V || v.дайТип() == VALUEMAP_V; }
бул мапЗнач_ли(const Значение& v)   { return массивЗнач_ли(v); }

Ткст      дайТекстОш(const Значение& v);

бул          пусто_ли(const Значение& v)               { return v.пусто_ли(); }
const Значение&  Nvl(const Значение& a, const Значение& b)  { return пусто_ли(a) ? b : a; }

бул          нч_ли(const Значение& v)                { return v.является<дво>() && нч_ли((дво)v); }

КЗСА обнулиКЗСА() { КЗСА c; c.r = c.g = c.b = c.a = 0; return c; }

//#ifndef PLATFORM_WIN32
 цел        дайЗнК(бцел c)             { return (ббайт)(c >> 0); }
 цел        дайЗнЗ(бцел c)             { return (ббайт)(c >> 8); }
 цел        дайЗнС(бцел c)             { return (ббайт)(c >> 16); }
 бцел      дайКЗС(ббайт r, ббайт g, ббайт b)    { return r | (g << 8) | (b << 16); }
//#endif
 Цвет прямойЦвет(КЗСА rgba)  { return Цвет(rgba.r, rgba.g, rgba.b); }
 т_хэш   дайХэшЗнач(Цвет c)  { return c.дайХэшЗнач(); }
 Цвет    Nvl(Цвет a, Цвет b)  { return пусто_ли(a) ? b : a; }
 Цвет серыйЦвет(цел a = 128)    { return Цвет(a, a, a); }

 Цвет чёрный()     { return Цвет(0, 0, 0); }
 Цвет серый()      { return Цвет(128, 128, 128); }
 Цвет светлоСерый()    { return Цвет(192, 192, 192); }
 Цвет белоСерый() { return Цвет(224, 224, 224); }
 Цвет белый()     { return Цвет(255, 255, 255); }

 Цвет красный()       { return Цвет(128, 0, 0); }
 Цвет зелёный()     { return Цвет(0, 128, 0); }
 Цвет коричневый()     { return Цвет(128, 128, 0); }
 Цвет синий()      { return Цвет(0, 0, 128); }
 Цвет магента()   { return Цвет(128, 0, 255); }
 Цвет цыан()      { return Цвет(0, 128, 128); }
 Цвет жёлтый()    { return Цвет(255, 255, 0); }
 Цвет светлоКрасный()     { return Цвет(255, 0, 0); }
 Цвет светлоЗелёный()   { return Цвет(0, 255, 0); }
 Цвет светлоЖёлтый()  { return Цвет(255, 255, 192); }
 Цвет светлоСиний()    { return Цвет(0, 0, 255); }
 Цвет светлоМагента() { return Цвет(255, 0, 255); }
 Цвет светлоЦыан()    { return Цвет(0, 255, 255); }

дво абс(const Комплекс& c) { return std::abs(c); }
const Комплекс& Nvl(const Комплекс& a, const Комплекс& b)  { return пусто_ли(a) ? b : a; }
бул нч_ли(const Комплекс& x)        { return нч_ли(x.real()) || нч_ли(x.imag()); }
бул беск_ли(const Комплекс& x)        { return беск_ли(x.real()) || беск_ли(x.imag()); }
бул конечен(const Комплекс& x)        { return конечен(x.real()) && конечен(x.imag()); }

 const Ткст&  Nvl(const Ткст& a, const Ткст& b)    { return пусто_ли(a) ? b : a; }
 цел            Nvl(цел a, цел b)                        { return пусто_ли(a) ? b : a; }
 дол          Nvl(дол a, дол b)                    { return пусто_ли(a) ? b : a; }
 дво         Nvl(дво a, дво b)                  { return пусто_ли(a) ? b : a; }
 Дата           Nvl(Дата a, Дата b)                      { return пусто_ли(a) ? b : a; }
 Время           Nvl(Время a, Время b)                      { return пусто_ли(a) ? b : a; }

 цел            Nvl(цел a)                               { return Nvl(a, 0); }
 дол          Nvl(дол a)                             { return Nvl(a, (дол)0); }
 дво         Nvl(дво a)                            { return Nvl(a, 0.0); }
 
Ткст&  рефТкст(Реф f)  { return дайРеф<Ткст>(f); }
ШТкст& рефШТкст(Реф f) { return дайРеф<ШТкст>(f); }
цел&     рефЦел(Реф f)     { return дайРеф<цел>(f); }
дол&   рефЦел64(Реф f)   { return дайРеф<дол>(f); }
дво&  рефДво(Реф f)  { return дайРеф<дво>(f); }
бул&    рефБул(Реф f)    { return дайРеф<бул>(f); }
Дата&    рефДата(Реф f)    { return дайРеф<Дата>(f); }
Время&    рефВремя(Реф f)    { return дайРеф<Время>(f); }
Значение&   рефЗнач(Реф f)   { ПРОВЕРЬ(f.дайТип() == VALUE_V);
                                    return *(Значение *)f.дайПроцУк(); }
								
Реф::Реф(const РефНаТипЗнач& r)
{
	укз = r.укз;
	m = r.m;
}

unsigned stou(const ббайт *укз, ук endptr = NULL, unsigned radix = 10) { return stou((кткст0 )укз, endptr, radix); }

цел     тктЦел(const сим* s)   { return сканЦел(s); }
Ткст    целТкт(цел i)           { return фмтЦел(i); }

дол   тктЦел64(кткст0 s) { return сканЦел64(s); }
Ткст    целТкт64(дол i)       { return фмтЦел64(i); }

дво  тктДво(const сим* s)   { return сканДво(s); }
Ткст    двоТкт(дво d)        { return пусто_ли(d) ? Ткст() : фмтДво(d); }

дво  целДво(цел i)           { return пусто_ли(i) ? дво(Null) : дво(i); }
цел     двоЦел(дво d)        { return пусто_ли(d) ? цел(Null) : fround(d); }

#ifdef CPU_64
Ткст  фмтЦелГекс(кук укз) { return фмт64Гекс((дол)(uintptr_t)укз); }
Ткст  фмтГекс(кук укз)    { return фмт64Гекс((дол)(uintptr_t)укз); }
#else
Ткст  фмтЦелГекс(кук укз) { return фмтЦелГекс((цел)(uintptr_t)укз); }
Ткст  фмтГекс(кук укз)    { return фмтЦелГекс((цел)(uintptr_t)укз); }
#endif	

Ткст         фмтЦелое(цел a)                  { return фмтЦел(a); }
Ткст         фмт64(бдол a)                    { return фмтБцел64(a); }

Ткст         фмтДвоФикс(дво x, цел digits) { return фмтФ(x, digits); }
Ткст         фмтДвоЭксп(дво x, цел digits) { return фмтЭ(x, digits); }

Ткст фмтБцел(бцел w)
{
	return Ткст::сделай(12, [&](сим *s) { return utoa32(w, s); });
}

Ткст фмтЦел(цел i)
{
	return Ткст::сделай(12, [&](сим *s) {
		if(пусто_ли(i))
			return 0;
		if(i < 0) {
			*s++ = '-';
			return utoa32(-i, s) + 1;
		}
		return utoa32(i, s);
	});
}

цел  атомнИнк(volatile Атомар& t)             { return ++t; }
цел  атомнДек(volatile Атомар& t)             { return --t; }

цел классТини__(цел sz) { // we suppose that this gets resolved at compile time....
	if(sz <= 8) return КЛАСС_8;
	if(sz <= 16) return КЛАСС_16;
	if(sz <= 24) return КЛАСС_24;
	if(sz <= 32) return КЛАСС_32;
	if(sz <= 40) return КЛАСС_40;
	if(sz <= 48) return КЛАСС_48;
	if(sz <= 56) return КЛАСС_56;
	return -1;
}

extern цел  ini_version__;
бул иниИзменён__(цел версия) { return версия != ini_version__; }

Ткст кодируйГекс(кткст0 s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return кодируйГекс((ббайт *)s, count, sep, sepchr); }
Ткст кодируйГекс(кук s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return кодируйГекс((ббайт *)s, count, sep, sepchr); }

Ткст декодируйГекс(кткст0 s, цел len) { return декодируйГекс(s, s + len); }
Ткст декодируйГекс(const Ткст& s)        { return декодируйГекс(~s, s.дайСчёт()); }

#ifdef PLATFORM_POSIX
цел GetLastError() { return errno; }
#endif

проц памустХ(ук t, кук элт, цел item_size, цел count) {
	ПРОВЕРЬ(count >= 0);
	ббайт *q = (ббайт *)t;
	while(count--) {
		копирпам8(q, элт, item_size);
		q += item_size;
	}
}

дво  sqr          (дво a)                      { return a * a; }
дво  argsinh      (дво s)                      { return log(s + sqrt(s * s + 1)); }
дво  argcosh      (дво c)                      { ПРОВЕРЬ(c >= 1); return log(c + sqrt(c * c - 1)); }
дво  argtanh      (дво t)                      { ПРОВЕРЬ(fabs(t) < 1); return log((1 + t) / (1 - t)) / 2; }

т_хэш дайХэшЗнач(Дата t) {
	return 512 * t.year + 32 * t.month + t.day;
}

бул operator==(Дата a, Дата b) {
	return a.day == b.day && a.month == b.month && a.year == b.year;
}

Время воВремя(const Дата& d) {
	return Время(d.year, d.month, d.day);
}

т_хэш дайХэшЗнач(Время t) {
	return t.second +
		   32 * (t.minute + 32 * (t.hour + 16 * (t.day + 32 * (t.month + 8 * t.year))));
}

Время   operator+(Время a, цел i)   { return a +  дол(i); }
Время   operator-(Время a, цел i)   { return a -  дол(i); }
Время&  operator+=(Время& a, цел i) { return a += дол(i); }
Время&  operator-=(Время& a, цел i) { return a -= дол(i); }

Время   operator+(Время a, дво i)   { return a +  дол(i); }
Время   operator-(Время a, дво i)   { return a -  дол(i); }
Время&  operator+=(Время& a, дво i) { return a += дол(i); }
Время&  operator-=(Время& a, дво i) { return a -= дол(i); }

Поток& operator%(Поток& s, КЗСА& c)
{
	return s % c.r % c.g % c.b % c.a;
}

бул operator==(const КЗСА& a, const КЗСА& b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

бул operator!=(const КЗСА& a, const КЗСА& b)
{
	return !(a == b);
}

бул operator==(const Значение& v, Цвет x)        { return v == x.operator Значение(); }
бул operator==(Цвет x, const Значение& v)        { return v == x.operator Значение(); }
бул operator!=(const Значение& v, Цвет x)        { return v != x.operator Значение(); }
бул operator!=(Цвет x, const Значение& v)        { return v != x.operator Значение(); }

бул operator==(const Значение& v, Цвет (*x)())   { return v == (*x)(); }
бул operator==(Цвет (*x)(), const Значение& v)   { return v == (*x)(); }
бул operator!=(const Значение& v, Цвет (*x)())   { return v != (*x)(); }
бул operator!=(Цвет (*x)(), const Значение& v)   { return v != (*x)(); }

бул operator==(Цвет c, Цвет (*x)())          { return c == (*x)(); }
бул operator==(Цвет (*x)(), Цвет c)          { return c == (*x)(); }
бул operator!=(Цвет c, Цвет (*x)())          { return c != (*x)(); }
бул operator!=(Цвет (*x)(), Цвет c)          { return c != (*x)(); }

Ткст& operator<<(Ткст& s, кткст0 x)
{
	s.кат(x, длинтекс__(x));
	return s;
}

Ткст& operator<<(Ткст& s, сим *x)
{
	s.кат(x);
	return s;
}

Ткст& operator<<(Ткст& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

Ткст& operator<<(Ткст& s, сим x)
{
	s.кат((цел) x);
	return s;
}

Ткст& operator<<(Ткст& s, кук x)
{
	s << фмтУк(x);
	return s;
}

Ткст& operator<<(Ткст&& s, кткст0 x)
{
	s.кат(x, длинтекс__(x));
	return s;
}

Ткст& operator<<(Ткст&& s, сим *x)
{
	s.кат(x);
	return s;
}

Ткст& operator<<(Ткст&& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

Ткст& operator<<(Ткст&& s, сим x)
{
	s.кат((цел) x);
	return s;
}

Ткст& operator<<(Ткст&& s, кук x)
{
	s << фмтУк(x);
	return s;
}

ТкстБуф& operator<<(ТкстБуф& s, сим *x)
{
	s.кат(x);
	return s;
}

ТкстБуф& operator<<(ТкстБуф& s, const Ткст &x)
{
	s.кат(x);
	return s;
}

ТкстБуф& operator<<(ТкстБуф& s, сим x)
{
	s.кат((цел) x);
	return s;
}

ТкстБуф& operator<<(ТкстБуф& s, кук x)
{
	s << фмтУк(x);
	return s;
}

ТкстБуф& operator<<(ТкстБуф& s, кткст0 x)
{
	s.кат(x);
	return s;
}

////////

Поток& operator<<(Поток& s, кткст0 x)
{
	s.помести(x);
	return s;
}

Поток& operator<<(Поток& s, сим *x)
{
	s.помести(x);
	return s;
}

Поток& operator<<(Поток& s, const Ткст &x)
{
	s.помести(x);
	return s;
}

Поток& operator<<(Поток& s, сим x)
{
	s.помести((цел) x);
	return s;
}

Поток& operator<<(Поток& s, const ук x)
{
	s << фмтУк(x);
	return s;
}

/*
Поток& operator<<(Поток& s, ук x)
{
	s << фмтУк(x);
	return s;
}
*/

Джейсон& Джейсон::operator()(кткст0 ключ, const МассивДжейсон& array)
{
	return CatRaw(ключ, array);
}

#ifdef PLATFORM_WINCE
проц аборт() { TerminateProcess(NULL, -1); }
#endif

Ткст сканГексТкст(кткст0 s, цел len) { return сканГексТкст(s, s + len); }
Ткст сканГексТкст(const Ткст& s)        { return сканГексТкст(~s, s.дайСчёт()); }
Ткст гексТкст(кук s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return гексТкст((ббайт *)s, count, sep, sepchr); }
Ткст гексТкст(кткст0 s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return гексТкст((ббайт *)s, count, sep, sepchr); }

#ifdef  PLATFORM_WIN32
Ткст  NativePath(кткст0 path) { return WinPath(path); }
#endif

#ifdef  PLATFORM_POSIX
Ткст  NativePath(кткст0 path) { return UnixPath(path); }
бул удалифл(кткст0 фн)      { return unlink(фн) == 0; }
#endif

бул  operator==(const Ууид& u, const Ууид& w) {
	return ((u.v[0] ^ w.v[0]) | (u.v[1] ^ w.v[1])) == 0;
}

бул  operator!=(const Ууид& u, const Ууид& w) {
	return !(u == w);
}