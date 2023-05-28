#include <DinrusPro/DinrusPro.h>
#include <Core/Core.h>
#include <DinrusPro/Tpl.h>
/////////////////////////
namespace ДинрусРНЦП{
//////////////////////////
проц LOGNOP__() {}

проц устпам8(ук укз, ббайт данные, т_мера счёт)
{
	Upp::memset8(укз, данные, счёт);
}

проц устпам16(ук укз, бкрат данные, т_мера счёт)
{
	Upp::memset16(укз, данные, счёт);
}

проц устпам32(ук укз, бцел данные, т_мера счёт)
{
	Upp::memset32(укз, данные, счёт);
}

проц устпам64(ук укз, бдол данные, т_мера счёт)
{
	Upp::memset64(укз, данные, счёт);
}

проц устпам128(ук укз, Upp::m128 данные, т_мера счёт)
{
	Upp::memset128(укз, данные, счёт);
}

/////////////////////////////////////////////

проц копирпам8(ук p, const ук q, т_мера счёт)
{
	Upp::memcpy8(p, q, счёт);
}

проц копирпам16(ук p, const ук q, т_мера счёт)
{
	Upp::memcpy16(p, q, счёт);
}

проц копирпам32(ук p, const ук q, т_мера счёт)
{
	Upp::memcpy32(p, q, счёт);
}

проц копирпам64(ук p, const ук q, т_мера счёт)
{
	Upp::memcpy64(p, q, счёт);
}

проц копирпам128(ук p, const ук q, т_мера счёт)
{
	Upp::memcpy128(p, q, счёт);
}

//////////////////////////////

бул равнпам8(const ук p, const ук q, т_мера счёт)
{
	return Upp::memeq8(p, q, счёт);
}

бул равнпам16(const ук p, const ук q, т_мера счёт)
{
	return Upp::memeq16(p, q, счёт);
}

бул равнпам32(const ук p, const ук q, т_мера счёт)
{
	return Upp::memeq32(p, q, счёт);
}

бул равнпам64(const ук p, const ук q, т_мера счёт)
{
	return Upp::memeq64(p, q, счёт);
}

бул равнпам128(const ук p, const ук q, т_мера счёт)
{
	return Upp::memeq128(p, q, счёт);
}

///////////////////
Upp::т_хэш хэшпам(const ук укз, т_мера счёт)
{
	return Upp::memhash(укз, счёт);
}
//////////////
бул нч_ли(дво d)        { return std::isnan(d); }
бул беск_ли(дво d)        { return std::isinf(d); }
бул конечен(дво d)        { return !нч_ли(d) && !беск_ли(d); }
/////////////////
проц заполни(char *t, кткст0 lim, const char& x)
{ устпам8(t, x, т_мера(lim - t)); }

проц копируй(char *dst, кткст0 ист, кткст0 lim)
{ копирпам8( (ук)dst, (ук)ист, т_мера((ббайт *)lim - (ббайт *)ист)); }

проц заполни(signed char *t, const signed char *lim, const signed char& x)
{ устпам8(t, x, т_мера(lim - t)); }

проц копируй(signed char *dst, const signed char *ист, const signed char *lim)
{ копирпам8((ук)dst,(ук) ист, т_мера((ббайт *)lim - (ббайт *)ист)); }

проц заполни(unsigned char *t, const unsigned char *lim, const unsigned char& x)
{ устпам8(t, x, т_мера(lim - t)); }

проц копируй(unsigned char *dst, const unsigned char *ист, const unsigned char *lim)
{ копирпам8((ук)dst, (ук)ист, т_мера((ббайт *)lim - (ббайт *)ист)); }

/////////////////////////
бул  пусто_ли(const цел& i)     { return i == INT_NULL; }
бул  пусто_ли(const дол& i)   { return i == INT64_NULL; }
бул  пусто_ли(const дво& r)  { return !(std::abs(r) < std::numeric_limits<дво>::infinity()); }
бул  пусто_ли(const бул& r  )  { return false; }

////////
цел сравниЗнак(const char& a, const char& b)                     { return сравниЧисла__(a, b); }
цел сравниЗнак(const signed char& a, const signed char& b)       { return сравниЧисла__(a, b); }
цел сравниЗнак(const unsigned char& a, const unsigned char& b)   { return сравниЧисла__(a, b); }
цел сравниЗнак(const short& a, const short& b)                   { return сравниЧисла__(a, b); }
цел сравниЗнак(const unsigned short& a, const unsigned short& b) { return сравниЧисла__(a, b); }
цел сравниЗнак(const цел& a, const цел& b)                       { return сравниЧисла__(a, b); }
цел сравниЗнак(const бцел& a, const бцел& b)     { return сравниЧисла__(a, b); }
цел сравниЗнак(const long& a, const long& b)                     { return сравниЧисла__(a, b); }
цел сравниЗнак(const unsigned long& a, const unsigned long& b)   { return сравниЧисла__(a, b); }
цел сравниЗнак(const бул& a, const бул& b)                     { return сравниЧисла__(a, b); }
цел сравниЗнак(const дол& a, const дол& b)                   { return сравниЧисла__(a, b); }
цел сравниЗнак(const бдол& a, const бдол& b)                 { return сравниЧисла__(a, b); }
цел сравниЗнак(const float& a, const float& b)                   { return сравниЧисла__(a, b); }
цел сравниЗнак(const дво& a, const дво& b)                 { return сравниЧисла__(a, b); }

т_хэш дайХэшЗнач(const char& a)           { return (т_хэш)a; }
т_хэш дайХэшЗнач(const signed char& a)    { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const unsigned char& a)  { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const short& a)          { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const unsigned short& a) { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const цел& a)            { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const бцел& a)   { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const long& a)           { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const unsigned long& a)  { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const бул& a)           { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const wchar_t& a)        { return (const т_хэш)a; }

#ifdef HASH64
т_хэш дайХэшЗнач(const дол& a)          { return (const т_хэш)a; }
т_хэш дайХэшЗнач(const бдол& a)         { return (const т_хэш)a; }
#else
т_хэш дайХэшЗнач(const дол& a)          { return комбинируйХэш((т_хэш)a, (т_хэш)(a >> 32)); }
т_хэш дайХэшЗнач(const бдол& a)         { return дайХэшЗнач((дол)a); }
#endif

т_хэш дайХэшЗнач(const дво& a)         { return хэшпам((ук) &a, sizeof(a)); }
т_хэш дайХэшЗнач(const float& a)          { дво хэшпам(&a, sizeof(a)); }

#ifdef CPU_32
т_хэш дайХэшЗначУкз(const ук a)                   { return (цел)a; }
#else
т_хэш дайХэшЗначУкз(const ук a)                   { return комбинируйХэш((т_хэш)(uintptr_t)a); }
#endif

//////////////////////////////////
#ifdef КУЧА_РНЦП

#ifdef _ОТЛАДКА
 // in DEBUG test for small block is moved inside, because debug adds diagnostics header
бул  пробуйПереместПам(ук укз, т_мера& newsize) {
	return MemoryTryRealloc__(укз, newsize);
}
#else
бул  пробуйПереместПам(ук укз, т_мера& newsize) {
	return (((бцел)(uintptr_t)укз) & 16) && MemoryTryRealloc__(укз, newsize);
}
#endif

#ifdef HEAPDBG
проц  MemoryIgnoreLeaksBegin();
проц  MemoryIgnoreLeaksEnd();

проц  MemoryCheckDebug();
#else
проц  MemoryIgnoreLeaksBegin() {}
проц  MemoryIgnoreLeaksEnd() {}
проц  MemoryCheckDebug() {}
#endif

ук TinyAlloc(цел size) {
	цел k = TinyKlass__(size);
	if(k < 0) return разместиПам(size);
	return MemoryAllok__(k);
}

проц TinyFree(цел size, ук укз)
{
	цел k = TinyKlass__(size);
	if(k < 0)
		освободиПам(укз);
	else
		освободиПамk__(k, укз);
}

#else

ОпцииПамяти::ОпцииПамяти() {}
ОпцииПамяти::~ОпцииПамяти() {}

#ifndef flagHEAPOVERRIDE

 ук разместиПамПерманентно(т_мера size)                { return malloc(size); }
 ук разместиПам(т_мера size)     { return new ббайт[size]; }
 ук разместиПамТн(т_мера &size)  { return new ббайт[size]; }
 проц   освободиПам(ук p)          { delete[] (ббайт *) p; }
 ук разместиПам32()              { return new ббайт[32]; }
 ук разместиПам48()              { return new ббайт[48]; }
 проц   освободиПам32(ук укз)      { delete[] (ббайт *)укз; }
 проц   освободиПам48(ук укз)      { delete[] (ббайт *)укз; }
 проц   диагнозИницПам()      {}
 проц   проверьПам() {}
 проц   MemoryCheckDebug() {}
 цел    MemoryUsedKb() { return 0; }
 цел    MemoryUsedKbMax() { return 0; }

 проц   MemoryIgnoreLeaksBegin() {}
 проц   MemoryIgnoreLeaksEnd() {}

 т_мера дайРазмБлокаПам(ук укз) { return 0; }

 бул   пробуйПереместПам(ук укз, т_мера& newsize) { return false; }

struct ПрофильПамяти {
	цел empty__;
};

 ПрофильПамяти *PeakMemoryProfile() { return NULL; }

 ук TinyAlloc(цел size) { return разместиПам(size); }

 проц TinyFree(цел, ук укз) { return освободиПам(укз); }

#endif
#endif

бцел номерТипаЗнач(const цел*)     { return INT_V; }
бцел номерТипаЗнач(const дол*)   { return INT64_V; }
бцел номерТипаЗнач(const дво*)  { return DOUBLE_V; }
бцел номерТипаЗнач(const бул*)    { return BOOL_V; }
бцел номерТипаЗнач(const Ткст*)  { return STRING_V; }
бцел номерТипаЗнач(const ШТкст*) { return WSTRING_V; }
бцел номерТипаЗнач(const Дата*)    { return DATE_V; }
бцел номерТипаЗнач(const Время*)    { return TIME_V; }
бцел номерТипаЗнач(const Значение*)   { return VALUE_V; }
/////////////////////////////
}//ns end
//////////////////////////