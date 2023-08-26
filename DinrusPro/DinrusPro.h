#ifndef _DinrusPro_DinrusPro_h_
#define _DinrusPro_DinrusPro_h_

#ifdef __cplusplus
#include <cstddef>
#include <cstdlib>
#else
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#endif

#include <sys/types.h>

//Базовые типы (примитивы)

	typedef void                проц;
	typedef void*               ук;
	typedef  const ук           кук;

	typedef bool                бул;

	typedef  signed char        байт;   ///int8_t, int8
	typedef  unsigned char      ббайт;  ///uint8_t, byte, uint8

	typedef  short              крат;  ///int16_t
	typedef  unsigned short     бкрат; ///uint16_t, word

	typedef  int                цел;  ///int32_t
	typedef  unsigned int       бцел; ///uint32_t
	typedef unsigned short      сим16;

	typedef  long long int      дол;   ///int64_t
	typedef  unsigned long long бдол;  ///uint64_t

	typedef  size_t             т_мера;

	typedef const char*         кткст0;
	typedef char*               ткст0;
	typedef char                сим;
	
	typedef бцел                шим;
	typedef бдол                дим;
	
	typedef double              дво;
	typedef float               плав;
	
	
	
struct m128 {
	дол i64[2];
	
	static m128 обнули()             { m128 a; a.i64[0] = a.i64[1] = 0; return a; }
};

#ifdef CPU_64
#define ХЭШ64

#define ХЭШ_КОНСТ1 Ц64(0xf7c21089bee7c0a5)
#define ХЭШ_КОНСТ2 Ц64(0xc85abc8da7534a4d)
#define ХЭШ_КОНСТ3 Ц64(0x8642b0fe3e86671b)
typedef дим т_хэш;
#else
#define ХЭШ_КОНСТ1 0xbee7c0a5
#define ХЭШ_КОНСТ2 0xa7534a4d
#define ХЭШ_КОНСТ3 0x8e86671b
typedef бцел т_хэш;
#endif

//Включения
#include <DinrusPro/Defs.h>
#ifdef PLATFORM_WIN32
#include <DinrusPro/Windows.h>
#elif defined PLATFORM_POSIX
#include <DinrusPro/Posix.h>
#endif

typedef std::atomic<цел> Атомар;

template <typename T> T клонируй(const T& x) { return x; }

template<class T> class Функция;

// USRLOG

struct ИниБул;

namespace Ини {
extern ИниБул user_log;
}

inline constexpr бцел
FD_TOLERANCE(цел x) // сколько нулей допускается справа от '.' перед преобразованием в формат E
{ return x << 16; }


struct ОпцииПамяти { // размеры в КБ
	цел мастер_блок; // размер мастер-блока
	цел сис_блок_лимит; // > that this: allocate directly from the system
	цел мастер_резерв; // free master blocks kept in reserve
	цел большой_резерв; // free large blocks kept in reserve
	цел малый_резерв; // free formatted small block pages kept in reserve

	ОпцииПамяти(); // loads default options
	~ОпцииПамяти(); // sets options
};
///////////////////////
#ifdef КУЧА_РНЦП
struct ПрофильПамяти {
	цел    allocated[1024]; // active small blocks (индекс is size in bytes)
	цел    fragments[1024]; // unallocated small blocks (индекс is size in bytes)
	цел    freepages; // empty 4KB pages (can be recycled)
	цел    large_count; // count of large (~ 1 - 64KB) active blocks
	т_мера large_total; // ^ total size
	цел    large_fragments_count; // count of unused large blocks
	т_мера large_fragments_total; // ^ total size
	цел    large_fragments[2048]; // * 256
	цел    huge_count; // bigger blocks managed by U++ heap (<= 32MB)
	т_мера huge_total; // ^ total size
	цел    huge_fragments_count; // count of unused large blocks
	т_мера huge_fragments_total; // ^ total size
	цел    huge_fragments[65536]; // * 256
	цел    sys_count; // blocks directly allocated from the system (>32MB
	т_мера sys_total; // ^total size
	цел    master_chunks; // master blocks

	ПрофильПамяти();
};
#endif
////////////////////
class БезКопий {
private:
	БезКопий(const БезКопий&);
	проц operator=(const БезКопий&);
public:
	БезКопий() {}
};
//////////////////
class Обнул {
public:
	operator цел() const                { return ЦЕЛ_НУЛЛ; }
	operator дол() const              { return ЦЕЛ64_НУЛЛ; }
	operator дво() const             { return ДВО_НУЛЛ; }
	operator бул() const               { return false; }

	Обнул() {}
};

extern const Обнул Null;
/////////////////////////////
#define НБСИМ_(ид, т) НАБСИМ_##ид,

enum {
	ДЕФНАБСИМ,
#include "CharSet.i"
};

#undef НБСИМ_

#define  НАБСИМ_ВАСКИ      253
#define  НАБСИМ_УТФ32        254 // auxilary
#define  НАБСИМ_УТФ8         255

enum {
	ДЕФСИМ = 0x1f
};

#define НБСИМ_(ид, т) extern бкрат СИМТАБ_##ид[128];

#include "CharSet.i"

#undef НБСИМ_

#include "Utf.hpp"
///////////////
struct ПустойКласс{};


#ifdef NO_MOVEABLE_CHECK

template <class T>
inline проц проверьДвиж(T *) {}

#define MoveableTemplate(T)

template <class T, class B = ПустойКласс>
class Движ : public B
{
};

template <class T>
struct Движ_ {
};

#define NTL_MOVEABLE(T)

#else

template <class T>
inline проц подтвердиДвижУкз(T, T) {}

template <class T>
inline проц подтвердиДвиж0(T *t) { подтвердиДвижУкз(&**t, *t); }
// Ошибка компиляции в этом месте означает, что тип T не отмечен как Движ

template <class T, class B = ПустойКласс>
struct Движ : public B {
	friend проц подтвердиДвиж0(T *) {}
};

template <class T>
struct Движ_ {
	friend проц подтвердиДвиж0(T *) {}
};

template <class T>
inline проц проверьДвиж(T *t = 0) { if(t) подтвердиДвиж0(t); }

#if defined(COMPILER_MSC) || defined(COMPILER_GCC) && (__GNUC__ < 4 || __GNUC_MINOR__ < 1)
	#define NTL_MOVEABLE(T) inline проц подтвердиДвиж0(T *) {}
#else
	#define NTL_MOVEABLE(T) template<> inline проц проверьДвиж<T>(T *) {}
#endif

#endif

NTL_MOVEABLE(бул)
NTL_MOVEABLE(сим)
NTL_MOVEABLE(байт)
NTL_MOVEABLE(ббайт)
NTL_MOVEABLE(крат)
NTL_MOVEABLE(unsigned short)
NTL_MOVEABLE(цел)
NTL_MOVEABLE(бцел)
NTL_MOVEABLE(long)
NTL_MOVEABLE(unsigned long)
NTL_MOVEABLE(дол)
NTL_MOVEABLE(бдол)
NTL_MOVEABLE(плав)
NTL_MOVEABLE(дво)
NTL_MOVEABLE(ук)
NTL_MOVEABLE(кук)

#define СОВМЕСТИМОСТЬ_С_ОБХОДЧИКОМ_СТЛ \
	typedef ptrdiff_t                        difference_type; \
    typedef std::random_access_iterator_tag  iterator_category; \
    typedef T                                value_type; \
    typedef T                                pointer; \
    typedef T                                reference; \


#if defined(_NATIVE_WCHAR_T_DEFINED) || defined(COMPILER_GCC)
NTL_MOVEABLE(wchar_t)
#endif

////////////////////
#ifdef флагПРОФИЛЬМП
class Стопор;
class СтатическийСтопор;

struct МпИнспектор {
	кткст0 имя;
	цел   number;
	цел   locked;
	цел   blocked;

	static МпИнспектор *Dumi();

	МпИнспектор(кткст0 s, цел n = -1) { имя = s; number = n; locked = blocked = 0; }
	~МпИнспектор();
};

#define ПРОФИЛЬМП(mutex) \
	{ static МпИнспектор MK__s(__FILE__, __LINE__); mutex.уст(MK__s); }

#define ПРОФИЛЬМП_(mutex, ид) \
	{ static МпИнспектор MK__s(ид); mutex.уст(MK__s); }

#else

#define ПРОФИЛЬМП(mutex)
#define ПРОФИЛЬМП_(mutex, ид)

#endif

class Нить : БезКопий {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
	DWORD	   thread_id;
#endif
#ifdef PLATFORM_POSIX
	pthread_t  handle;
#endif
public:
	бул       пуск(Функция<проц ()> cb, бул noshutdown = false);
	бул       выполниХорошо(Функция<проц ()> cb, бул noshutdown = false);
	бул       выполниКрит(Функция<проц ()> cb, бул noshutdown = false);

	проц       открепи();
	цел        жди();

	бул       открыт() const     { return handle; }

#ifdef PLATFORM_WIN32
	typedef HANDLE Handle;
	typedef DWORD  Ид;

	Ид          дайИд() const                  { return thread_id; }
#endif
#ifdef PLATFORM_POSIX
	typedef pthread_t Handle;
	typedef pthread_t Ид;

	Ид          дайИд() const                  { return handle; }
#endif

	Handle      дайУк() const              { return handle; }

	бул        приоритет(цел percent); // 0 = lowest, 100 = normal

	проц        Nice()                         { приоритет(25); }
	проц        критически()                     { приоритет(150); }

	static проц старт(Функция<проц ()> cb, бул noshutdown = false);
	static проц стартХорошо(Функция<проц ()> cb, бул noshutdown = false);
	static проц стартКрит(Функция<проц ()> cb, бул noshutdown = false);

	static проц спи(цел ms);

	static бул ОН_ли();
	static бул главная_ли();
	static бул рнцп_ли();
	static цел  дайСчёт();
	static проц начниШатдаунНитей();
	static проц завершиШатдаунНитей();
	static проц шатдаунНитей();
	static бул шатдаунНитей_ли();
	static проц (*приВыходе(проц (*exitfn)()))();

	static проц выход();

#ifdef PLATFORM_WIN32
	static Handle дайУкТекущ()          { return GetCurrentThread(); }
	static inline Ид дайИдТекущ()           { return ::GetCurrentThreadId(); };
#endif
#ifdef PLATFORM_POSIX
	static Handle дайУкТекущ()          { return pthread_self(); }
	static inline Ид дайИдТекущ()           { return pthread_self(); };
#endif

	Нить();
	~Нить();

private:
	проц operator=(const Нить&);
	Нить(const Нить&);
};

#ifdef _ОТЛАДКА
inline проц проверьНаОН() { ПРОВЕРЬ(Нить::ОН_ли()); }
#else
inline проц проверьНаОН() {}
#endif


class Семафор : БезКопий {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
#elif PLATFORM_OSX
	dispatch_semaphore_t    sem;
#else
typedef int __atomic_lock_t;

    struct _pthread_fastlock
    {
        long            __status;
      __atomic_lock_t   __spinlock;
    };

    struct sem_t
    {
      _pthread_fastlock __sem_lock;
      int               __sem_value;
      void*             __sem_waiting;
    };

	sem_t      sem;
#endif

public:
	бул       жди(цел timeout_ms = -1);
	проц       отпусти();
#ifdef PLATFORM_WIN32
	проц       отпусти(цел n);
#endif

	Семафор();
	~Семафор();
};

struct МпИнспектор;

#ifdef PLATFORM_WIN32

class Стопор : БезКопий {
protected:
	КРИТСЕКЦ section;
	МпИнспектор        *mti;

	Стопор(цел)         {}

	friend class ПеременнаяУсловия;

public:
	бул  пробуйВойти();
	проц  выйди()                { LeaveCriticalSection(&section); }

#ifdef флагПРОФИЛЬМП
	проц  войди()                { if(!пробуйВойти()) { mti->blocked++; EnterCriticalSection(&section); }; mti->locked++; }
	проц  уст(МпИнспектор& m)    { mti = &m; }

	Стопор()                      { mti = МпИнспектор::Dumi(); InitializeCriticalSection(&section); }
#else
	проц  войди()                { EnterCriticalSection(&section); }

	Стопор()                      { InitializeCriticalSection(&section); }
#endif

	~Стопор()                     { DeleteCriticalSection(&section); }

	class Замок;
};

/* Win32 ЧЗСтопор implementation by Chris Thomasson, cristom@comcast.net */

class ЧЗСтопор : БезКопий {
    LONG   m_count, m_rdwake;
    HANDLE m_wrwset, m_rdwset;
    КРИТСЕКЦ m_wrlock;

public:
	проц войдиЗ();
	проц выйдиЗ();

	проц войдиЧ();
	проц выйдиЧ();

	ЧЗСтопор();
	~ЧЗСтопор();

	class ЧЗамок;
	class ЗЗамок;
};

class ПеременнаяУсловия {
	static VOID (WINAPI *InitializeConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
	static VOID (WINAPI *WakeConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
	static VOID (WINAPI *WakeAllConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
	static BOOL (WINAPI *SleepConditionVariableCS)(PCONDITION_VARIABLE ПеременнаяУсловия, PCRITICAL_SECTION КритСекция, DWORD dwMilliseconds);

	CONDITION_VARIABLE cv[1];

	struct ЖдущаяНить { // Windows XP does not provide ПеременнаяУсловия, implement using Semaphores
		Семафор      sem;
		ЖдущаяНить *next;
	};
	Стопор          mutex;
	ЖдущаяНить *head, *tail;

public:
	бул жди(Стопор& m, цел timeout_ms = -1);
	проц Signal();
	проц Broadcast();

	ПеременнаяУсловия();
	~ПеременнаяУсловия();
};

#endif

#ifdef PLATFORM_POSIX

class Стопор : БезКопий {
	pthread_mutex_t  mutex[1];
#ifdef флагПРОФИЛЬМП
	МпИнспектор     *mti;
#endif
	friend class ПеременнаяУсловия;

public:
#ifdef флагПРОФИЛЬМП
	бул  пробуйВойти()          { бул b = pthread_mutex_trylock(mutex) == 0; mti->locked += b; return b; }
	проц  войди()             { if(pthread_mutex_trylock(mutex) != 0) { mti->blocked++; pthread_mutex_lock(mutex); } mti->locked++; }
	проц  уст(МпИнспектор& m) { mti = &m; }
#else
	бул  пробуйВойти()          { return pthread_mutex_trylock(mutex) == 0; }
	проц  войди()             { pthread_mutex_lock(mutex); }
#endif
	проц  выйди()             { pthread_mutex_unlock(mutex); }

	class Замок;

	Стопор();
	~Стопор()           { pthread_mutex_destroy(mutex); }
};

class ЧЗСтопор : БезКопий {
	pthread_rwlock_t rwlock[1];

public:
	проц войдиЗ()  { pthread_rwlock_wrlock(rwlock); }
	проц выйдиЗ()  { pthread_rwlock_unlock(rwlock); }
	проц войдиЧ()   { pthread_rwlock_rdlock(rwlock); }
	проц выйдиЧ()   { pthread_rwlock_unlock(rwlock); }

	ЧЗСтопор();
	~ЧЗСтопор();

	class ЧЗамок;
	class ЗЗамок;
};

class ПеременнаяУсловия {
	pthread_cond_t cv[1];

public:
	бул жди(Стопор& m, цел timeout_ms = -1);

	проц Signal()        { pthread_cond_signal(cv); }
	проц Broadcast()     { pthread_cond_broadcast(cv); }

	ПеременнаяУсловия()  { pthread_cond_init(cv, NULL); }
	~ПеременнаяУсловия() { pthread_cond_destroy(cv); }
};

#endif

typedef std::atomic<бул> ФлагЕдиножды;

#define ONCELOCK_(o_b_) \
for(static ::Стопор o_ss_; !o_b_.load(std::memory_order_acquire);) \
	for(::Стопор::Замок o_ss_lock__(o_ss_); !o_b_.load(std::memory_order_acquire); o_b_.store(true, std::memory_order_release))

#define ONCELOCK \
for(static ::ФлагЕдиножды o_b_; !o_b_.load(std::memory_order_acquire);) ONCELOCK_(o_b_)


class Стопор::Замок : БезКопий {
	Стопор& s;

public:
	Замок(Стопор& s) : s(s) { s.войди(); }
	~Замок()               { s.выйди(); }
};

class ЧЗСтопор::ЧЗамок : БезКопий {
	ЧЗСтопор& s;

public:
	ЧЗамок(ЧЗСтопор& s) : s(s) { s.войдиЧ(); }
	~ЧЗамок()                 { s.выйдиЧ(); }
};

class ЧЗСтопор::ЗЗамок : БезКопий {
	ЧЗСтопор& s;

public:
	ЗЗамок(ЧЗСтопор& s) : s(s) { s.войдиЗ(); }
	~ЗЗамок()                 { s.выйдиЗ(); }
};

template <class Примитив>
class СтатическийПримитив_ : БезКопий {
	Примитив *primitive;
	ббайт       буфер[sizeof(Примитив)];
	ФлагЕдиножды   once;

	проц инициализуй() { primitive = new(буфер) Примитив; }

public:
	Примитив& дай()  { ONCELOCK_(once) инициализуй(); return *primitive; }
};

class СтатическийСтопор : СтатическийПримитив_<Стопор> {
public:
	operator Стопор&()          { return дай(); }
	бул пробуйВойти()            { return дай().пробуйВойти();}
	проц войди()               { дай().войди();}
	проц выйди()               { дай().выйди(); }
#ifdef флагПРОФИЛЬМП
	проц уст(МпИнспектор& mti) { дай().уст(mti); }
#endif
};

class СтатическийСемафор : СтатическийПримитив_<Семафор> {
public:
	operator Семафор&()        { return дай(); }
	проц жди()                  { дай().жди(); }
	проц отпусти()               { дай().отпусти(); }
};

class СтатическийЧЗСтопор : СтатическийПримитив_<ЧЗСтопор> {
public:
	operator ЧЗСтопор&()  { return дай(); }
	проц войдиЧ()     { дай().войдиЧ();}
	проц выйдиЧ()     { дай().выйдиЧ(); }
	проц войдиЗ()    { дай().войдиЗ();}
	проц выйдиЗ()    { дай().выйдиЗ(); }
};

class СтатическаяПеременнаяУсловия : СтатическийПримитив_<ПеременнаяУсловия> {
public:
	operator ПеременнаяУсловия&() { return дай(); }
	проц жди(Стопор& m)  { дай().жди(m); }
	проц Signal()        { дай().Signal(); }
	проц Broadcast()     { дай().Broadcast(); }
};

class ФоновыйОбнов {
	mutable Стопор              mutex;
	mutable std::atomic<бул>  dirty;

public:
	проц инивалидируй();
	бул начниОбнов() const;
	проц завершиОбнов() const;

	ФоновыйОбнов();
};

inline бул главнаяНить_ли() { return Нить::главная_ли(); }

struct СпинЗамок : Движ<СпинЗамок> {
#ifdef COMPILER_MSC
	volatile LONG locked;

	бул пробуйВойти() { return InterlockedCompareExchange(&locked, 1, 0) == 0; }
	проц выйди()    { _ReadWriteBarrier(); locked = 0; }
#else
	volatile цел locked;

	бул пробуйВойти() { return  __sync_bool_compare_and_swap (&locked, 0, 1); }
	проц выйди()    { __sync_lock_release(&locked); }
#endif

	проц войди()    { while(!пробуйВойти()) жди(); }

	проц жди();

	class Замок;

	СпинЗамок()         { locked = 0; }
};

class СпинЗамок::Замок : БезКопий {
	СпинЗамок& s;

public:
	Замок(СпинЗамок& s) : s(s) { s.войди(); }
	~Замок()                  { s.выйди(); }
};

#define INTERLOCKED \
for(бул i_b_ = true; i_b_;) \
	for(static Стопор i_ss_; i_b_;) \
		for(Стопор::Замок i_ss_lock__(i_ss_); i_b_; i_b_ = false)

struct H_l_ : Стопор::Замок {
	бул b;
	H_l_(Стопор& cs) : Стопор::Замок(cs) { b = true; }
};

#define ВЗАИМОЗАМК_(cs) \
for(H_l_ i_ss_lock__(cs); i_ss_lock__.b; i_ss_lock__.b = false)
	
//////////////////////////////////////

template <class B>
class АТкст : public B {
	
	typedef typename B::tchar  tchar;
	typedef typename B::bchar  bchar;
	typedef typename B::Буфер буфер;
	typedef typename B::Ткст  Ткст;

public:
	проц очисть()                                              { B::освободи(); B::обнули(); }
	цел  дайДлину() const                                    { return B::дайСчёт(); }
	бул пустой() const                                      { return B::дайСчёт() == 0; }

	const tchar *стоп() const                                  { return B::старт() + дайДлину(); }
	const tchar *end() const                                  { return стоп(); }
	const tchar *последний() const                                 { return стоп() - !!B::дайСчёт(); }
	const tchar *дайОбх(цел i) const                         { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт() + i; }

	цел operator*() const                                     { return *B::старт(); }
	цел operator[](цел i) const                               { ПРОВЕРЬ(i >= 0 && i <= B::дайСчёт()); return B::старт()[i]; }

	operator const tchar *() const                            { return B::старт(); }
	const tchar *operator~() const                            { return B::старт(); }
	operator const bchar *() const                            { return (bchar *)B::старт(); }
	operator const проц *() const                             { return B::старт(); }

	проц вставь(цел pos, цел c)                               { *B::вставь(pos, 1, NULL) = c; }
	проц вставь(цел pos, const tchar *s, цел count)           { B::вставь(pos, count, s); }
	проц вставь(цел pos, const Ткст& s)                     { вставь(pos, s, s.дайСчёт()); }
	проц вставь(цел pos, кткст0 s);

	проц обрежьПоследн(цел count = 1)                              { B::обрежь(B::дайСчёт() - count); }

	проц  кат(цел c)                                          { B::кат(c); }
	проц  кат(const tchar *s, цел len)                        { B::кат(s, len); }
	проц  кат(const tchar *s);
	проц  кат(const Ткст& s)                                { кат(~s, s.дайДлину()); }
	проц  кат(цел c, цел count);
	проц  кат(const tchar *s, const tchar *lim)               { ПРОВЕРЬ(s <= lim); кат(s, цел(lim - s)); }
	проц  кат(const Ткст& s, цел len)                       { B::кат(~s, len); }
	проц  кат(const bchar *s, цел len)                        { кат((const tchar *) s, len); }

	Ткст& кат()                                             { return *(Ткст *)this; }

	цел    сравни(const Ткст& s) const                     { return B::сравни(s); }
	цел    сравни(const tchar *s) const;

	бул   равен(const Ткст& s) const                     { return B::равен(s); }
	бул   равен(const tchar *s) const                      { return B::равен(s); }

	Ткст середина(цел pos, цел length) const;
	Ткст середина(цел pos) const                                 { return середина(pos, дайДлину() - pos); }
	Ткст право(цел count) const                             { return середина(дайДлину() - count); }
	Ткст лево(цел count) const                              { return середина(0, count); }

	цел    найди(цел chr, цел from = 0) const;
	цел    найдирек(цел chr, цел from) const;
	цел    найдирек(цел chr) const;

	цел    найди(цел len, const tchar *s, цел from) const      { return найди(B::старт(), B::дайСчёт(), s, len, from); }
	цел    найди(const tchar *s, цел from = 0) const           { return найди(длинтекс__(s), s, from); }
	цел    найди(const Ткст& s, цел from = 0) const          { return найди(s.дайСчёт(), ~s, from); }

	цел    найдиПосле(const tchar *s, цел from = 0) const      { цел n = длинтекс__(s); цел q = найди(n, s, from); return q < 0 ? -1 : q + n; }
	цел    найдиПосле(const Ткст& s, цел from = 0) const     { цел n = s.дайСчёт(); цел q = найди(n, ~s, from); return q < 0 ? -1 : q + n; }

	цел    найдирек(цел len, const tchar *s, цел from) const;
	цел    найдирек(const tchar *s, цел from) const;
	цел    найдирек(const Ткст& s, цел from) const       { return найдирек(s.дайСчёт(), ~s, from); }
	цел    найдирек(const tchar *s) const                  { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}
	цел    найдирек(const Ткст& s) const                 { return дайДлину() ? найдирек(s, дайДлину()-1) : -1;}

	цел    найдирекПосле(цел len, const tchar *s, цел from) const;
	цел    найдирекПосле(const tchar *s, цел from) const;
	цел    найдирекПосле(const Ткст& s, цел from) const  { return найдирекПосле(s.дайСчёт(), ~s, from); }
	цел    найдирекПосле(const tchar *s) const             { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}
	цел    найдирекПосле(const Ткст& s) const            { return дайДлину() ? найдирекПосле(s, дайДлину()-1) : -1;}

	проц   замени(const tchar *найди, цел findlen, const tchar *replace, цел replacelen);
	проц   замени(const Ткст& найди, const Ткст& replace);
	проц   замени(const tchar *найди, const tchar *replace);
	проц   замени(const Ткст& найди, const tchar *replace);
	проц   замени(const tchar *найди, const Ткст& replace);

	бул   начинаетсяС(const tchar *s, цел len) const;
	бул   начинаетсяС(const tchar *s) const;
	бул   начинаетсяС(const Ткст& s) const                  { return начинаетсяС(~s, s.дайДлину()); }

	бул   обрежьСтарт(const tchar *s, цел len)                 { if(!начинаетсяС(s, len)) return false; B::удали(0, len); return true; }
	бул   обрежьСтарт(const tchar *s)                          { return обрежьСтарт(s, длинтекс__(s)); }
	бул   обрежьСтарт(const Ткст& s)                         { return обрежьСтарт(~s, s.дайДлину()); }

	бул   заканчиваетсяНа(const tchar *s, цел len) const;
	бул   заканчиваетсяНа(const tchar *s) const;
	бул   заканчиваетсяНа(const Ткст& s) const                    { return заканчиваетсяНа(~s, s.дайДлину()); }

	бул   обрежьКонец(const tchar *s, цел len)                   { if(!заканчиваетсяНа(s, len)) return false; обрежьПоследн(len); return true; }
	бул   обрежьКонец(const tchar *s)                            { return обрежьКонец(s, длинтекс__(s)); }
	бул   обрежьКонец(const Ткст& s)                           { return обрежьКонец(~s, s.дайДлину()); }

	цел    найдиПервыйИз(цел len, const tchar *set, цел from = 0) const;
	цел    найдиПервыйИз(const tchar *set, цел from = 0) const  { return найдиПервыйИз(длинтекс__(set), set, from); }
	цел    найдиПервыйИз(const Ткст& set, цел from = 0) const { return найдиПервыйИз(set.дайСчёт(), ~set, from); }

	friend бул operator<(const Ткст& a, const Ткст& b)   { return a.сравни(b) < 0; }
	friend бул operator<(const Ткст& a, const tchar *b)    { return a.сравни(b) < 0; }
	friend бул operator<(const tchar *a, const Ткст& b)    { return b.сравни(a) > 0; }

	friend бул operator<=(const Ткст& a, const Ткст& b)  { return a.сравни(b) <= 0; }
	friend бул operator<=(const Ткст& a, const tchar *b)   { return a.сравни(b) <= 0; }
	friend бул operator<=(const tchar *a, const Ткст& b)   { return b.сравни(a) >= 0; }

	friend бул operator>(const Ткст& a, const Ткст& b)   { return a.сравни(b) > 0; }
	friend бул operator>(const Ткст& a, const tchar *b)    { return a.сравни(b) > 0; }
	friend бул operator>(const tchar *a, const Ткст& b)    { return b.сравни(a) < 0; }

	friend бул operator>=(const Ткст& a, const Ткст& b)   { return a.сравни(b) >= 0; }
	friend бул operator>=(const Ткст& a, const tchar *b)    { return a.сравни(b) >= 0; }
	friend бул operator>=(const tchar *a, const Ткст& b)    { return b.сравни(a) <= 0; }

	friend бул operator==(const Ткст& a, const Ткст& b)   { return a.равен(b); }
	friend бул operator!=(const Ткст& a, const Ткст& b)   { return !a.равен(b); }
	friend бул operator==(const Ткст& a, const tchar *b)    { return a.равен(b); }
	friend бул operator==(const tchar *a, const Ткст& b)    { return b.равен(a); }
	friend бул operator!=(const Ткст& a, const tchar *b)    { return !a.равен(b); }
	friend бул operator!=(const tchar *a, const Ткст& b)    { return !b.равен(a); }

	friend Ткст operator+(const Ткст& a, const Ткст& b)  { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, const tchar *b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(const Ткст& a, tchar b)          { Ткст c(a); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const Ткст& b)       { Ткст c(пикуй(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, const tchar *b)        { Ткст c(пикуй(a)); c += b; return c; }
	friend Ткст operator+(Ткст&& a, tchar b)               { Ткст c(пикуй(a)); c += b; return c; }
	friend Ткст operator+(const tchar *a, const Ткст& b)   { Ткст c(a); c += b; return c; }
	friend Ткст operator+(tchar a, const Ткст& b)          { Ткст c(a, 1); c += b; return c; }

};

#endif
