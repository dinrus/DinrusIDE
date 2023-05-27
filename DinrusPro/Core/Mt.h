#ifdef DEPRECATED
#define thread__ thread_local
#endif

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

template<typename Рез, typename... ТипыАрг>
class Функция<Рез(ТипыАрг...)>;

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
	CRITICAL_SECTION section;
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
    CRITICAL_SECTION m_wrlock;

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
for(static ::ДинрусРНЦП::Стопор o_ss_; !o_b_.load(std::memory_order_acquire);) \
	for(::ДинрусРНЦП::Стопор::Замок o_ss_lock__(o_ss_); !o_b_.load(std::memory_order_acquire); o_b_.store(true, std::memory_order_release))

#define ONCELOCK \
for(static ::ДинрусРНЦП::ФлагЕдиножды o_b_; !o_b_.load(std::memory_order_acquire);) ONCELOCK_(o_b_)


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
	for(static РНЦП::Стопор i_ss_; i_b_;) \
		for(РНЦП::Стопор::Замок i_ss_lock__(i_ss_); i_b_; i_b_ = false)

struct H_l_ : Стопор::Замок {
	бул b;
	H_l_(Стопор& cs) : Стопор::Замок(cs) { b = true; }
};

#define ВЗАИМОЗАМК_(cs) \
for(РНЦП::H_l_ i_ss_lock__(cs); i_ss_lock__.b; i_ss_lock__.b = false)

#ifdef DEPRECATED
typedef Стопор КритСекция;
typedef СтатическийСтопор СтатическаяКритСекция;
#endif

// Auxiliary multithreading - this is not using/cannot use U++ heap, so does not need cleanup.
// Used to resolve some host platform issues.

#ifdef PLATFORM_WIN32
#define auxthread_t DWORD
#define auxthread__ WINAPI
#else
#define auxthread_t ук 
#define auxthread__
#endif

бул стартВспомНити(auxthread_t (auxthread__ *фн)(ук укз), ук укз);
