#include "Core.h"

#ifdef PLATFORM_OSX
#include <sys/time.h>
#endif

namespace РНЦПДинрус {

#define LLOG(x)  // DLOG(x)

static Стопор& sMutexLock()
{ // this is Стопор intended to synchronize initialization of other primitives
	static Стопор m;
	return m;
}

ИНИЦБЛОК {
	sMutexLock();
}

Нить::Нить()
{
	sMutexLock();
#ifdef PLATFORM_WIN32
	handle = 0;
	thread_id = 0;
#endif
#ifdef PLATFORM_POSIX
	handle = 0;
#endif
}

void Нить::открепи()
{
#if defined(PLATFORM_WIN32)
	if(handle) {
		CloseHandle(handle);
		handle = 0;
		thread_id = 0;
	}
#elif defined(PLATFORM_POSIX)
	if(handle) {
		CHECK(!pthread_detach(handle));
		handle = 0;
	}
#endif
}

static Атомар sThreadCount;

static thread_local void (*sExit)(void);

void (*Нить::приВыходе(void (*exitfn)()))()
{
	void (*prev)() = sExit;
	sExit = exitfn;
	return prev;
}

struct sThreadExitExc__ {};

void Нить::выход()
{
	throw sThreadExitExc__();
}

struct sThreadParam {
	Функция<void ()> cb;
	bool              noshutdown;
};

static thread_local bool sUppThread = false;

static
#ifdef PLATFORM_WIN32
	#ifdef CPU_64
		unsigned int
	#else
		uintptr_t __stdcall
	#endif
#else
	void *
#endif
sThreadRoutine(void *арг)
{
	LLOG("sThreadRoutine");
	sUppThread = true;
	auto p = (sThreadParam *)арг;
	try {
		p->cb();
	}
	catch(Искл e) {
		паника(e);
	}
	catch(sThreadExitExc__) {}
	catch(РНЦПДинрус::ИсклВыхода) {}
	if(!p->noshutdown)
		атомнДек(sThreadCount);
	delete p;
	if(sExit)
		(*sExit)();
	return 0;
}

static bool threadr; //indicates if *any* Нить instance is running (having called its выполни()), upon first call of выполни
#ifndef CPU_BLACKFIN
static thread_local  bool sMain;
#else
#ifdef PLATFORM_POSIX
static Индекс<pthread_t> threadsv; //a threads ид vector, sMain=true ==>> 'pthread_self() pthread_t beeing present in vector, problem, wont be cleared when thread exits
Стопор vm; //a common access synchronizer
#endif
#endif

//to sMain: an Application can start more than one thread, without having *any* one of them called выполни() of any Нить instace
//when выполни() is called *anytime*, it means, the term of *MainThread* has to be running anyway,
//otherwise no child threads could run. they are created by main.
//now each thread, having any Нить instance can start a first выполни()

bool Нить::пуск(Функция<void ()> _cb, bool noshutdown)
{
	LLOG("Нить::пуск");
	if(!noshutdown)
		атомнИнк(sThreadCount);
	if(!threadr)
#ifndef CPU_BLACKFIN
		threadr = sMain = true;
#else
	{
		threadr = true;
		//the sMain replacement
#ifdef PLATFORM_POSIX
		pthread_t thid = pthread_self();
		vm.войди();
		if(threadsv.найди(thid) < 0){
			//thread not yet present, mark present
			threadsv.добавь(thid);
		}
		else
			RLOG("BUG: Multiple добавь in Mt.cpp");
		vm.выйди();
#endif
	}
#endif
	открепи();
	auto p = new sThreadParam;
	p->cb = _cb;
	p->noshutdown = noshutdown;
#ifdef PLATFORM_WIN32
#ifdef CPU_32 // in 32-bit, reduce stack size to 1MB to fit more threads into address space
	handle = (HANDLE)_beginthreadex(0, 1024*1024, sThreadRoutine, p, STACK_SIZE_PARAM_IS_A_RESERVATION, ((unsigned int *)(&thread_id)));
#else
	handle = (HANDLE)_beginthreadex(0, 0, sThreadRoutine, p, 0, ((unsigned int *)(&thread_id)));
#endif
#endif
#ifdef PLATFORM_POSIX
	if(pthread_create(&handle, 0, sThreadRoutine, p))
		handle = 0;
#endif
	return handle;
}

bool Нить::выполниХорошо(Функция<void ()> cb, bool noshutdown)
{
	if(пуск(cb, noshutdown)) {
		Nice();
		return true;
	}
	return false;
}

bool Нить::выполниКрит(Функция<void ()> cb, bool noshutdown)
{
	if(пуск(cb, noshutdown)) {
		критически();
		return true;
	}
	return false;
}

Нить::~Нить()
{
	открепи();
#ifdef CPU_BLACKFIN
#ifdef PLATFORM_POSIX
	//the static destruction replacement
	pthread_t thid = pthread_self();
	vm.войди();
	int ид = threadsv.найди(thid);
	if(ид >= 0)
		threadsv.удали(ид);
	vm.выйди();
#endif
#endif
}

bool Нить::рнцп_ли()
{
	return sUppThread;
}

bool Нить::ОН_ли() //the containing thread (of wich there may be multiple) has not run its пуск() yet
{
	return !threadr;
}

bool Нить::главная_ли() //the calling thread is the Main Нить or the only one in App
{
#ifndef CPU_BLACKFIN
	return !threadr || sMain;
#else
	if(!threadr) 
		return true;
#ifdef PLATFORM_POSIX
	//the sMain replacement
	pthread_t thid = pthread_self();
	vm.войди();
	if(threadsv.найди(thid) >= 0)
	{
		vm.выйди();
		return true;
	}
	vm.выйди();
#endif
	return false;
#endif
}

int Нить::дайСчёт()
{
	return sThreadCount;
}

static int sShutdown;

void Нить::начниШатдаунНитей()
{
	sShutdown++;
}

void Нить::завершиШатдаунНитей()
{
	sShutdown--;
}

void Нить::шатдаунНитей()
{
	начниШатдаунНитей();
	while(дайСчёт())
		спи(100);
	завершиШатдаунНитей();
}

bool Нить::шатдаунНитей_ли()
{
	return sShutdown;
}

int Нить::жди()
{
	if(!открыт())
		return -1;
	int out;
#ifdef PLATFORM_WIN32
	dword exit;
	if(!GetExitCodeThread(handle, &exit))
		return -1;
	if(exit != STILL_ACTIVE)
		out = (int)exit;
	else
	{
		if(WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
			return Null;
		out = GetExitCodeThread(handle, &exit) ? int(exit) : int(Null);
	}
	открепи();
#endif
#ifdef PLATFORM_POSIX
	void *thread_return;
	if(pthread_join(handle, &thread_return))
		out = Null;
	else
		out = (int)(intptr_t)thread_return;
	handle = 0;
#endif
	return out;
}

bool Нить::приоритет(int percent)
{
	ПРОВЕРЬ(открыт());
#ifdef PLATFORM_WIN32
	int prior;
	if(percent <= 25)
		prior = THREAD_PRIORITY_LOWEST;
	else if(percent <= 75)
		prior = THREAD_PRIORITY_BELOW_NORMAL;
	else if(percent <= 125)
		prior = THREAD_PRIORITY_NORMAL;
	else if(percent <= 175)
		prior = THREAD_PRIORITY_ABOVE_NORMAL;
	else
		prior = THREAD_PRIORITY_HIGHEST;
	return SetThreadPriority(handle, prior);
#endif
#ifdef PLATFORM_POSIX
	int policy;
	struct sched_param param;
	
	if(pthread_getschedparam(handle, &policy, &param))
		return false;
	int percent_min = 0, percent_max = 200;
	if(percent <= 25) {
		#if defined(SCHED_IDLE)
			policy = SCHED_IDLE;
			percent_min = 0;
			percent_max = 25;
		#elif defined(SCHED_BATCH)
			policy = SCHED_BATCH;
			percent_min = 0;
			percent_max = 75;
		#else
			policy = SCHED_OTHER;
			percent_min = 0;
			percent_max = 125;
		#endif
	}
	else
	if(percent <= 75){
		#if defined(SCHED_IDLE)
			policy = SCHED_BATCH;
			percent_min = 25;
			percent_max = 75;
		#elif defined(SCHED_BATCH)
			policy = SCHED_BATCH;
			percent_min = 0;
			percent_max = 75;
		#else
			policy = SCHED_OTHER;
			percent_min = 0;
			percent_max = 125;
		#endif
	}
	else
	if(percent <= 125){
		policy = SCHED_OTHER;
		#if defined(SCHED_IDLE)
			percent_min = 75;
			percent_max = 125;
		#elif defined(SCHED_BATCH)
			percent_min = 25;
			percent_max = 125;
		#else
			percent_min = 0;
			percent_max = 125;
		#endif
	}
	else
	if(percent <= 175){ // should be the root
		policy = SCHED_FIFO;
		percent_min = 125;
		percent_max = 175;
	}
	else
		policy = SCHED_RR;

	param.sched_priority = (sched_get_priority_max(policy) - sched_get_priority_min(policy))*(minmax(percent, percent_min, percent_max)-percent_min)/(percent_max - percent_min);
	
	if (pthread_setschedparam(handle, policy, &param)) {
		// No privileges? Try maximum possible! делай not use EPERM as not all os support this one
		policy = SCHED_OTHER;
		percent_max = 125;
		percent_min = minmax(percent_min, 0, percent_max);
		param.sched_priority = (sched_get_priority_max(policy) - sched_get_priority_min(policy))
		                       * (minmax(percent, percent_min, percent_max) - percent_min)
		                       / max(percent_max - percent_min, 1);
		if(pthread_setschedparam(handle, policy, &param))
			return false;
	}
	return true;
#endif
}

void Нить::старт(Функция<void ()> cb, bool noshutdown)
{
	Нить t;
	t.пуск(cb);
	t.открепи();
}

void Нить::стартХорошо(Функция<void ()> cb, bool noshutdown)
{
	Нить t;
	t.пуск(cb);
	t.Nice();
	t.открепи();
}

void Нить::стартКрит(Функция<void ()> cb, bool noshutdown)
{
	Нить t;
	t.пуск(cb);
	t.критически();
	t.открепи();
}

void Нить::спи(int msec)
{
#ifdef PLATFORM_WIN32
	::Sleep(msec);
#endif
#ifdef PLATFORM_POSIX
	::timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
#endif
}

#ifdef флагПРОФИЛЬМП
МпИнспектор *МпИнспектор::Dumi()
{
	static МпИнспектор h(NULL);
	return &h;
}

МпИнспектор::~МпИнспектор()
{
	if(имя)
		RLOG("Стопор " << имя << '(' << number << ") " << blocked << "/" << locked <<
		     " = " << спринтф("%.4f", locked ? (double)blocked / locked : 0) << " blocked/locked times");
}
#endif

#ifdef PLATFORM_WIN32

void Семафор::отпусти()
{
	ReleaseSemaphore(handle, 1, NULL);
}

void Семафор::отпусти(int n)
{
	ReleaseSemaphore(handle, n, NULL);
}

bool Семафор::жди(int timeout_ms)
{
	return WaitForSingleObject(handle, timeout_ms < 0 ? INFINITE : timeout_ms) == WAIT_OBJECT_0;
}

Семафор::Семафор()
{
	handle = CreateSemaphore(NULL, 0, INT_MAX, NULL);
}

Семафор::~Семафор()
{
	CloseHandle(handle);
}

Стопор& sMutexLock();

bool Стопор::пробуйВойти()
{
	return TryEnterCriticalSection(&section);
}

/* Win32 ЧЗСтопор implementation by Chris Thomasson, cristom@comcast.net */

void ЧЗСтопор::войдиЗ()
{
	EnterCriticalSection ( &m_wrlock );
	LONG count = InterlockedExchangeAdd(&m_count, -LONG_MAX);
	if(count < LONG_MAX)
		if(InterlockedExchangeAdd ( &m_rdwake, LONG_MAX - count ) + LONG_MAX - count )
			WaitForSingleObject ( m_wrwset, INFINITE );
}

void ЧЗСтопор::выйдиЗ()
{
	LONG count = InterlockedExchangeAdd ( &m_count, LONG_MAX );
	if (count < 0)
	    ReleaseSemaphore ( m_rdwset, count * -1, 0 );
	LeaveCriticalSection ( &m_wrlock );
}

void ЧЗСтопор::войдиЧ()
{
	LONG count = InterlockedDecrement ( &m_count );
	if(count < 0)
		WaitForSingleObject ( m_rdwset, INFINITE );
}

void ЧЗСтопор::выйдиЧ()
{
	LONG count = InterlockedIncrement ( &m_count );
	if ( count < 1 )
		if ( ! InterlockedDecrement ( &m_rdwake ) )
			SetEvent ( m_wrwset );
}

ЧЗСтопор::ЧЗСтопор()
:	m_count ( LONG_MAX ),
	m_rdwake ( 0 ),
	m_wrwset ( CreateEvent ( 0, FALSE, FALSE, 0 ) ),
	m_rdwset ( CreateSemaphore ( 0, 0, LONG_MAX, 0 ) )
{
	InitializeCriticalSection ( &m_wrlock );
}

ЧЗСтопор::~ЧЗСтопор()
{
	DeleteCriticalSection ( &m_wrlock );
	CloseHandle ( m_rdwset );
	CloseHandle ( m_wrwset );
}

VOID (WINAPI *ПеременнаяУсловия::InitializeConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
VOID (WINAPI *ПеременнаяУсловия::WakeConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
VOID (WINAPI *ПеременнаяУсловия::WakeAllConditionVariable)(PCONDITION_VARIABLE ПеременнаяУсловия);
BOOL (WINAPI *ПеременнаяУсловия::SleepConditionVariableCS)(PCONDITION_VARIABLE ПеременнаяУсловия, PCRITICAL_SECTION КритСекция, DWORD dwMilliseconds);

bool ПеременнаяУсловия::жди(Стопор& m, int timeout_ms)
{
	if(InitializeConditionVariable)
		return SleepConditionVariableCS(cv, &m.section, timeout_ms < 0 ? INFINITE : timeout_ms);
	else { // WindowsXP implementation
		static thread_local byte буфер[sizeof(ЖдущаяНить)]; // only one жди per thread is possible
		ЖдущаяНить *w = new(буфер) ЖдущаяНить;
		{
			Стопор::Замок __(mutex);
			w->next = NULL;
			if(head)
				tail->next = w;
			else
				head = w;
			tail = w;
		}
		m.выйди();
		bool r = w->sem.жди(timeout_ms);
		m.войди();
		w->ЖдущаяНить::~ЖдущаяНить();
		return r;
	}
}

void ПеременнаяУсловия::Signal()
{
	if(InitializeConditionVariable)
		WakeConditionVariable(cv);
	else { // WindowsXP implementation
		Стопор::Замок __(mutex);
		if(head) {
			head->sem.отпусти();
			head = head->next;
		}
	}
}

void ПеременнаяУсловия::Broadcast()
{
	if(InitializeConditionVariable)
		WakeAllConditionVariable(cv);
	else { // WindowsXP implementation
		Стопор::Замок __(mutex);
		while(head) {
			head->sem.отпусти();
			head = head->next;
		}
	}
}

ПеременнаяУсловия::ПеременнаяУсловия()
{
#ifndef flagTESTXPCV
	ONCELOCK {
		if(IsWinVista()) {
			фнДлл(InitializeConditionVariable, "kernel32", "InitializeConditionVariable");
			фнДлл(WakeConditionVariable, "kernel32", "WakeConditionVariable");
			фнДлл(WakeAllConditionVariable, "kernel32", "WakeAllConditionVariable");
			фнДлл(SleepConditionVariableCS, "kernel32", "SleepConditionVariableCS");
		}
	}
#endif
	if(InitializeConditionVariable)
		InitializeConditionVariable(cv);
	else
		head = tail = NULL;
}

ПеременнаяУсловия::~ПеременнаяУсловия()
{
	Broadcast();
}

#endif

#ifdef PLATFORM_POSIX

Стопор::Стопор()
{
	pthread_mutexattr_t mutex_attr[1];
	pthread_mutexattr_init(mutex_attr);
	pthread_mutexattr_settype(mutex_attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(mutex, mutex_attr);
#ifdef флагПРОФИЛЬМП
	mti = МпИнспектор::Dumi();
#endif
}

ЧЗСтопор::ЧЗСтопор()
{
	pthread_rwlock_init(rwlock, NULL);
}

ЧЗСтопор::~ЧЗСтопор()
{
	pthread_rwlock_destroy(rwlock);
}

bool ПеременнаяУсловия::жди(Стопор& m, int timeout_ms)
{
	if(timeout_ms < 0) {
		pthread_cond_wait(cv, m.mutex);
		return true;
	}
	::timespec until;
	clock_gettime(CLOCK_REALTIME, &until);
	
	until.tv_sec += timeout_ms / 1000;
	timeout_ms %= 1000;
	until.tv_nsec += timeout_ms * 1000000;
	until.tv_sec += until.tv_nsec / 1000000000;
	until.tv_nsec %= 1000000000;

	return pthread_cond_timedwait(cv, m.mutex, &until) == 0;
}

#ifdef PLATFORM_OSX

void Семафор::отпусти()
{
	dispatch_semaphore_signal(sem);
}

bool Семафор::жди(int timeout_ms)
{
	return dispatch_semaphore_wait(sem, timeout_ms < 0 ? DISPATCH_TIME_FOREVER
	                                    : dispatch_time(DISPATCH_TIME_NOW, 1000000 * timeout_ms)) == 0;
}

Семафор::Семафор()
{
	sem = dispatch_semaphore_create(0);
}

Семафор::~Семафор()
{
	dispatch_release(sem);
}

#else

void Семафор::отпусти()
{
	sem_post(&sem);
}

bool Семафор::жди(int timeout_ms)
{
	if(timeout_ms < 0) {
		sem_wait(&sem);
		return true;
	}
	struct timespec until;
	clock_gettime(CLOCK_REALTIME, &until);
	
	until.tv_sec += timeout_ms / 1000;
	timeout_ms %= 1000;
	until.tv_nsec += timeout_ms * 1000000;
	until.tv_sec += until.tv_nsec / 1000000000;
	until.tv_nsec %= 1000000000;
	
	return sem_timedwait(&sem,&until) != -1;
}

Семафор::Семафор()
{
	sem_init(&sem, 0, 0);
}

Семафор::~Семафор()
{
	sem_destroy(&sem);
}

#endif

#endif

void ФоновыйОбнов::инивалидируй()
{
	dirty.store(true, std::memory_order_release);
	dirty = true;
}

bool ФоновыйОбнов::начниОбнов() const
{
	bool b = dirty.load(std::memory_order_acquire);
	if(b) {
		mutex.войди();
		if(dirty) return true;
		mutex.выйди();
	}
	return false;
}

void ФоновыйОбнов::завершиОбнов() const
{
	dirty.store(false, std::memory_order_release);
	mutex.выйди();
}

ФоновыйОбнов::ФоновыйОбнов()
{
	dirty = true;
}

void СпинЗамок::жди()
{
	volatile int n = 0;
	while(locked) {
	#ifdef CPU_X86
		_mm_pause();
	#endif
		if(n++ > 500)
			Sleep(0);
	}
}

bool стартВспомНити(auxthread_t (auxthread__ *фн)(void *ptr), void *ptr)
{
#ifdef PLATFORM_WIN32
	HANDLE handle;
	handle = CreateThread(NULL, 512*1024, фн, ptr, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL);
	if(handle) {
		CloseHandle(handle);
		return true;
	}
#endif
#ifdef PLATFORM_POSIX
	pthread_t handle;
	if(pthread_create(&handle, 0, фн, ptr) == 0) {
		pthread_detach(handle);
		return true;
	}
#endif
	return false;
}

}
