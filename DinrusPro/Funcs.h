#ifndef _DinrusPro_Funcs_h_
#define _DinrusPro_Funcs_h_
#include <DinrusPro/DinrusPro.h>
//////////////////////
namespace ДинрусРНЦП {
/////////////////////
class Ткст;
class ШТкст;
class Дата;
class Время;
class Поток;
class   Ид;
class   Значение;
//Defs.h
проц LOGNOP__();
бул    режимПаники_ли();
проц   паника(кткст0 msg);
проц   неуспехПроверки(кткст0 file, цел line, кткст0 cond);
проц   устБоксСообПаники(проц (*mb)(кткст0 title, кткст0 text));
проц   боксСообПаники(кткст0 title, кткст0 text);

бул    нч_ли(дво d);
бул    беск_ли(дво d);
бул    конечен(дво d);

#ifdef CPU_X86
бул цпбММХ();
бул цпбССЕ();
бул цпбССЕ2();
бул цпбССЕ3();
бул цпбГипервизор();
бул цпбАВХ();
#endif

цел  цпбЯдра();
проц дайСтатусСисПамяти(бдол& total, бдол& available);

#ifdef PLATFORM_WINCE
кткст0 изСисНабСима(const шим *s);
const шим *вСисНабСим(кткст0 s);
#else
inline кткст0 изСисНабСима(кткст0 s) { return s; }
inline кткст0 вСисНабСим(кткст0 s) { return s; }
#endif

//Mem.h

проц устпам8(ук укз, ббайт данные, т_мера счёт);
проц устпам16(ук укз, бкрат данные, т_мера счёт);
проц устпам32(ук укз, бцел данные, т_мера счёт);
проц устпам64(ук укз, qword данные, т_мера счёт);
проц устпам128(ук укз, m128 данные, т_мера счёт);

проц копирпам8(ук p, const ук q, т_мера счёт);
проц копирпам16(ук p, const ук q, т_мера счёт);
проц копирпам32(ук p, const ук q, т_мера счёт);
проц копирпам64(ук p, const ук q, т_мера счёт);
проц копирпам128(ук p, const ук q, т_мера счёт);

бул равнпам8(const ук p, const ук q, т_мера счёт);
бул равнпам16(const ук p, const ук q, т_мера счёт);
бул равнпам32(const ук p, const ук q, т_мера счёт);
бул равнпам64(const ук p, const ук q, т_мера счёт);
бул равнпам128(const ук p, const ук q, т_мера счёт);

т_хэш хэшпам(const ук укз, т_мера счёт);

проц заполни(char *t, кткст0 lim, const char& x);
проц заполни(signed char *t, const signed char *lim, const signed char& x);
проц заполни(unsigned char *t, const unsigned char *lim, const unsigned char& x);

проц копируй(char *dst, кткст0 ист, кткст0 lim);
проц копируй(signed char *dst, const signed char *ист, const signed char *lim);
проц копируй(unsigned char *dst, const unsigned char *ист, const unsigned char *lim);

бул  пусто_ли(const цел& i);
бул  пусто_ли(const дол& i) ;
бул  пусто_ли(const дво& r);
бул  пусто_ли(const бул& r  );

цел сравниЗнак(const char& a, const char& b);
цел сравниЗнак(const signed char& a, const signed char& b);
цел сравниЗнак(const unsigned char& a, const unsigned char& b) ;
цел сравниЗнак(const short& a, const short& b);
цел сравниЗнак(const unsigned short& a, const unsigned short& b);
цел сравниЗнак(const цел& a, const цел& b);
цел сравниЗнак(const бцел& a, const бцел& b);
цел сравниЗнак(const long& a, const long& b) ;
цел сравниЗнак(const unsigned long& a, const unsigned long& b);
цел сравниЗнак(const бул& a, const бул& b);
цел сравниЗнак(const дол& a, const дол& b) ;
цел сравниЗнак(const бдол& a, const бдол& b)  ;
цел сравниЗнак(const float& a, const float& b) ;
цел сравниЗнак(const дво& a, const дво& b)    ;

unsigned Pow2Bound(unsigned i);
unsigned PrimeBound(unsigned i);

т_хэш дайХэшЗнач(const char& a) ;
т_хэш дайХэшЗнач(const signed char& a) ;
т_хэш дайХэшЗнач(const unsigned char& a);
т_хэш дайХэшЗнач(const short& a) ;
т_хэш дайХэшЗнач(const unsigned short& a);
т_хэш дайХэшЗнач(const цел& a) ;
т_хэш дайХэшЗнач(const бцел& a) ;
т_хэш дайХэшЗнач(const long& a)  ;
т_хэш дайХэшЗнач(const unsigned long& a);
т_хэш дайХэшЗнач(const бул& a);
т_хэш дайХэшЗнач(const wchar_t& a) ;
т_хэш дайХэшЗнач(const дол& a);
т_хэш дайХэшЗнач(const бдол& a);
т_хэш дайХэшЗнач(const дво& a) ;
т_хэш дайХэшЗнач(const float& a) ;
т_хэш дайХэшЗначУкз(const ук a) ;

ук разместиПамПерманентно(т_мера size);

ук разместиПамТн(т_мера& size);
ук разместиПам(т_мера size);
проц  освободиПам(ук укз);
ук разместиПам32();
проц  освободиПам32(ук укз);
проц  проверьПам();
проц  MemoryDumpLarge();
проц  MemoryDumpHuge();
цел   MemoryUsedKb();
цел   MemoryUsedKbMax();
проц  MemoryLimitKb(цел kb);

т_мера дайРазмБлокаПам(ук укз);
бул  пробуйПереместПам(ук укз, т_мера& newsize);

проц  остановТчкПам(бцел serial);

проц  диагнозИницПам();
проц  MemoryDumpLeaks();

проц  MemoryIgnoreLeaksBegin();
проц  MemoryIgnoreLeaksEnd();
проц  MemoryCheckDebug();

ук TinyAlloc(цел size);
проц TinyFree(цел size, ук укз);

бцел MemoryGetCurrentSerial();

проц  MemoryIgnoreNonMainLeaks();
проц  MemoryIgnoreNonUppThreadsLeaks();


бцел номерТипаЗнач(const цел*) ;
бцел номерТипаЗнач(const дол*) ;
бцел номерТипаЗнач(const дво*) ;
бцел номерТипаЗнач(const бул*)   ;
бцел номерТипаЗнач(const Ткст*)  ;
бцел номерТипаЗнач(const ШТкст*) ;
бцел номерТипаЗнач(const Дата*)   ;
бцел номерТипаЗнач(const Время*)    ;
бцел номерТипаЗнач(const Значение*)  ;

цел длинтекс8(кткст0 s);
цел длинтекс16(const char16 *s);
цел длинтекс32(const шим *s);

#ifdef PLATFORM_POSIX
цел stricmp(кткст0 a, кткст0 b) ;
цел strnicmp(кткст0 a, кткст0 b, цел n);
#endif

#ifdef PLATFORM_WINCE
цел stricmp(кткст0 a, кткст0 b);
цел strnicmp(кткст0 a, кткст0 b, цел n) ;
#endif

цел длинтекс__(кткст0 s) ;
цел длинтекс__(const шим *s) ;

цел сравнзн__(char x);
цел сравнзн__(шим x);

цел найди(кткст0 text, цел len, кткст0 needle, цел nlen, цел from);
цел найди(const шим *text, цел len, const шим *needle, цел nlen, цел from);

//////////////////////////
}//конец пи
#endif
//////////////////////////