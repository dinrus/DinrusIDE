#ifdef DEPRECATED
#define thread__ thread_local
#endif

#ifdef флагПРОФИЛЬМП
class Стопор;
class СтатическийСтопор;

struct МпИнспектор {
	const char *имя;
	int   number;
	int   locked;
	int   blocked;
	
	static МпИнспектор *Dumi();

	МпИнспектор(const char *s, int n = -1) { имя = s; number = n; locked = blocked = 0; }
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
	bool       пуск(Функция<void ()> cb, bool noshutdown = false);
	bool       выполниХорошо(Функция<void ()> cb, bool noshutdown = false);
	bool       выполниКрит(Функция<void ()> cb, bool noshutdown = false);

	void       открепи();
	int        жди();

	bool       открыт() const     { return handle; }

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
	
	bool        приоритет(int percent); // 0 = lowest, 100 = normal
	
	void        Nice()                         { приоритет(25); }
	void        критически()                     { приоритет(150); }

	static void старт(Функция<void ()> cb, bool noshutdown = false);
	static void стартХорошо(Функция<void ()> cb, bool noshutdown = false);
	static void стартКрит(Функция<void ()> cb, bool noshutdown = false);

	static void спи(int ms);

	static bool ОН_ли();
	static bool главная_ли();
	static bool рнцп_ли();
	static int  дайСчёт();
	static void начниШатдаунНитей();
	static void завершиШатдаунНитей();
	static void шатдаунНитей();
	static bool шатдаунНитей_ли();
	static void (*приВыходе(void (*exitfn)()))();

	static void выход();

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
	void operator=(const Нить&);
	Нить(const Нить&);
};

#ifdef _ОТЛАДКА
inline void проверьНаОН() { ПРОВЕРЬ(Нить::ОН_ли()); }
#else
inline void проверьНаОН() {}
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
	bool       жди(int timeout_ms = -1);
	void       отпусти();
#ifdef PLATFORM_WIN32
	void       отпусти(int n);
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

	Стопор(int)         {}

	friend class ПеременнаяУсловия;

public:
	bool  пробуйВойти();
	void  выйди()                { LeaveCriticalSection(&section); }

#ifdef флагПРОФИЛЬМП
	void  войди()                { if(!пробуйВойти()) { mti->blocked++; EnterCriticalSection(&section); }; mti->locked++; }
	void  уст(МпИнспектор& m)    { mti = &m; }

	Стопор()                      { mti = МпИнспектор::Dumi(); InitializeCriticalSection(&section); }
#else
	void  войди()                { EnterCriticalSection(&section); }

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
	void войдиЗ();
	void выйдиЗ();

	void войдиЧ();
	void выйдиЧ();

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
	bool жди(Стопор& m, int timeout_ms = -1);
	void Signal();
	void Broadcast();
	
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
	bool  пробуйВойти()          { bool b = pthread_mutex_trylock(mutex) == 0; mti->locked += b; return b; }
	void  войди()             { if(pthread_mutex_trylock(mutex) != 0) { mti->blocked++; pthread_mutex_lock(mutex); } mti->locked++; }
	void  уст(МпИнспектор& m) { mti = &m; }
#else
	bool  пробуйВойти()          { return pthread_mutex_trylock(mutex) == 0; }
	void  войди()             { pthread_mutex_lock(mutex); }
#endif
	void  выйди()             { pthread_mutex_unlock(mutex); }

	class Замок;

	Стопор();
	~Стопор()           { pthread_mutex_destroy(mutex); }
};

class ЧЗСтопор : БезКопий {
	pthread_rwlock_t rwlock[1];

public:
	void войдиЗ()  { pthread_rwlock_wrlock(rwlock); }
	void выйдиЗ()  { pthread_rwlock_unlock(rwlock); }
	void войдиЧ()   { pthread_rwlock_rdlock(rwlock); }
	void выйдиЧ()   { pthread_rwlock_unlock(rwlock); }

	ЧЗСтопор();
	~ЧЗСтопор();

	class ЧЗамок;
	class ЗЗамок;
};

class ПеременнаяУсловия {
	pthread_cond_t cv[1];
	
public:
	bool жди(Стопор& m, int timeout_ms = -1);

	void Signal()        { pthread_cond_signal(cv); }
	void Broadcast()     { pthread_cond_broadcast(cv); }
	
	ПеременнаяУсловия()  { pthread_cond_init(cv, NULL); }
	~ПеременнаяУсловия() { pthread_cond_destroy(cv); }
};

#endif

typedef std::atomic<bool> ФлагЕдиножды;

#define ONCELOCK_(o_b_) \
for(static ::РНЦПДинрус::Стопор o_ss_; !o_b_.load(std::memory_order_acquire);) \
	for(::РНЦПДинрус::Стопор::Замок o_ss_lock__(o_ss_); !o_b_.load(std::memory_order_acquire); o_b_.store(true, std::memory_order_release))

#define ONCELOCK \
for(static ::РНЦПДинрус::ФлагЕдиножды o_b_; !o_b_.load(std::memory_order_acquire);) ONCELOCK_(o_b_)


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
	byte       буфер[sizeof(Примитив)];
	ФлагЕдиножды   once;
	
	void инициализуй() { primitive = new(буфер) Примитив; }

public:
	Примитив& дай()  { ONCELOCK_(once) инициализуй(); return *primitive; }
};

class СтатическийСтопор : СтатическийПримитив_<Стопор> {
public:
	operator Стопор&()          { return дай(); }
	bool пробуйВойти()            { return дай().пробуйВойти();}
	void войди()               { дай().войди();}
	void выйди()               { дай().выйди(); }
#ifdef флагПРОФИЛЬМП
	void уст(МпИнспектор& mti) { дай().уст(mti); }
#endif
};

class СтатическийСемафор : СтатическийПримитив_<Семафор> {
public:
	operator Семафор&()        { return дай(); }
	void жди()                  { дай().жди(); }
	void отпусти()               { дай().отпусти(); }
};

class СтатическийЧЗСтопор : СтатическийПримитив_<ЧЗСтопор> {
public:
	operator ЧЗСтопор&()  { return дай(); }
	void войдиЧ()     { дай().войдиЧ();}
	void выйдиЧ()     { дай().выйдиЧ(); }
	void войдиЗ()    { дай().войдиЗ();}
	void выйдиЗ()    { дай().выйдиЗ(); }
};

class СтатическаяПеременнаяУсловия : СтатическийПримитив_<ПеременнаяУсловия> {
public:
	operator ПеременнаяУсловия&() { return дай(); }
	void жди(Стопор& m)  { дай().жди(m); }
	void Signal()        { дай().Signal(); }
	void Broadcast()     { дай().Broadcast(); }
};

class ФоновыйОбнов {
	mutable Стопор              mutex;
	mutable std::atomic<bool>  dirty;

public:
	void инивалидируй();
	bool начниОбнов() const;
	void завершиОбнов() const;

	ФоновыйОбнов();
};

inline bool главнаяНить_ли() { return Нить::главная_ли(); }

struct СпинЗамок : Движимое<СпинЗамок> {
#ifdef COMPILER_MSC
	volatile LONG locked;

	bool пробуйВойти() { return InterlockedCompareExchange(&locked, 1, 0) == 0; }
	void выйди()    { _ReadWriteBarrier(); locked = 0; }
#else
	volatile int locked;
	
	bool пробуйВойти() { return  __sync_bool_compare_and_swap (&locked, 0, 1); }
	void выйди()    { __sync_lock_release(&locked); }
#endif

	void войди()    { while(!пробуйВойти()) жди(); }
	
	void жди();
	
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
for(bool i_b_ = true; i_b_;) \
	for(static РНЦП::Стопор i_ss_; i_b_;) \
		for(РНЦП::Стопор::Замок i_ss_lock__(i_ss_); i_b_; i_b_ = false)

struct H_l_ : Стопор::Замок {
	bool b;
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
#define auxthread_t void *
#define auxthread__
#endif

bool стартВспомНити(auxthread_t (auxthread__ *фн)(void *ptr), void *ptr);
