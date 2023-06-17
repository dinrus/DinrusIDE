#include <DinrusPro/DinrusCore.h>

#ifdef PLATFORM_OSX
#include <sys/time.h>
#endif

#define LLOG(x)  // DLOG(x)

static Стопор& sMutexLock()
{ // этот Стопор служит для синхронизованной инициализации других примитивов
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

проц Нить::открепи()
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

static thread_local проц (*sExit)(проц);

проц (*Нить::приВыходе(проц (*exitfn)()))()
{
	проц (*prev)() = sExit;
	sExit = exitfn;
	return prev;
}

struct sThreadExitExc__ {};

проц Нить::выход()
{
	throw sThreadExitExc__();
}

struct sThreadParam {
	Функция<проц ()> cb;
	бул              noshutdown;
};

static thread_local бул sUppThread = false;

static
#ifdef PLATFORM_WIN32
	#ifdef CPU_64
		бцел
	#else
		uintptr_t __stdcall
	#endif
#else
	ук 
#endif
sThreadRoutine(ук арг)
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
	catch(ИсклВыхода) {}
	if(!p->noshutdown)
		атомнДек(sThreadCount);
	delete p;
	if(sExit)
		(*sExit)();
	return 0;
}

static бул threadr; //indicates if *any* Нить instance is running (having called its выполни()), upon first call of выполни
#ifndef CPU_BLACKFIN
static thread_local  бул sMain;
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

бул Нить::пуск(Функция<проц ()> _cb, бул noshutdown)
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
	handle = (HANDLE)_beginthreadex(0, 1024*1024, sThreadRoutine, p, STACK_SIZE_PARAM_IS_A_RESERVATION, ((бцел *)(&thread_id)));
#else
	handle = (HANDLE)_beginthreadex(0, 0, sThreadRoutine, p, 0, ((бцел *)(&thread_id)));
#endif
#endif
#ifdef PLATFORM_POSIX
	if(pthread_create(&handle, 0, sThreadRoutine, p))
		handle = 0;
#endif
	return handle;
}

бул Нить::выполниХорошо(Функция<проц ()> cb, бул noshutdown)
{
	if(пуск(cb, noshutdown)) {
		Nice();
		return true;
	}
	return false;
}

бул Нить::выполниКрит(Функция<проц ()> cb, бул noshutdown)
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
	цел ид = threadsv.найди(thid);
	if(ид >= 0)
		threadsv.удали(ид);
	vm.выйди();
#endif
#endif
}

бул Нить::рнцп_ли()
{
	return sUppThread;
}

бул Нить::ОН_ли() //the containing thread (of wich there may be multiple) has not run its пуск() yet
{
	return !threadr;
}

бул Нить::главная_ли() //the calling thread is the Main Нить or the only one in App
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

цел Нить::дайСчёт()
{
	return sThreadCount;
}

static цел sShutdown;

проц Нить::начниШатдаунНитей()
{
	sShutdown++;
}

проц Нить::завершиШатдаунНитей()
{
	sShutdown--;
}

проц Нить::шатдаунНитей()
{
	начниШатдаунНитей();
	while(дайСчёт())
		спи(100);
	завершиШатдаунНитей();
}

бул Нить::шатдаунНитей_ли()
{
	return sShutdown;
}

цел Нить::жди()
{
	if(!открыт())
		return -1;
	цел out;
#ifdef PLATFORM_WIN32
	бцел exit;
	if(!GetExitCodeThread(handle, &exit))
		return -1;
	if(exit != STILL_ACTIVE)
		out = (цел)exit;
	else
	{
		if(WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
			return Null;
		out = GetExitCodeThread(handle, &exit) ? цел(exit) : цел(Null);
	}
	открепи();
#endif
#ifdef PLATFORM_POSIX
	ук thread_return;
	if(pthread_join(handle, &thread_return))
		out = Null;
	else
		out = (цел)(intptr_t)thread_return;
	handle = 0;
#endif
	return out;
}

бул Нить::приоритет(цел percent)
{
	ПРОВЕРЬ(открыт());
#ifdef PLATFORM_WIN32
	цел prior;
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
	цел policy;
	struct sched_param param;
	
	if(pthread_getschedparam(handle, &policy, &param))
		return false;
	цел percent_min = 0, percent_max = 200;
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

	param.sched_priority = (sched_get_priority_max(policy) - sched_get_priority_min(policy))*(минмакс(percent, percent_min, percent_max)-percent_min)/(percent_max - percent_min);
	
	if (pthread_setschedparam(handle, policy, &param)) {
		// No privileges? Try maximum possible! делай not use EPERM as not all os support this one
		policy = SCHED_OTHER;
		percent_max = 125;
		percent_min = минмакс(percent_min, 0, percent_max);
		param.sched_priority = (sched_get_priority_max(policy) - sched_get_priority_min(policy))
		                       * (минмакс(percent, percent_min, percent_max) - percent_min)
		                       / макс(percent_max - percent_min, 1);
		if(pthread_setschedparam(handle, policy, &param))
			return false;
	}
	return true;
#endif
}

проц Нить::старт(Функция<проц ()> cb, бул noshutdown)
{
	Нить t;
	t.пуск(cb);
	t.открепи();
}

проц Нить::стартХорошо(Функция<проц ()> cb, бул noshutdown)
{
	Нить t;
	t.пуск(cb);
	t.Nice();
	t.открепи();
}

проц Нить::стартКрит(Функция<проц ()> cb, бул noshutdown)
{
	Нить t;
	t.пуск(cb);
	t.критически();
	t.открепи();
}

проц Нить::спи(цел msec)
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
		     " = " << спринтф("%.4f", locked ? (дво)blocked / locked : 0) << " blocked/locked times");
}
#endif

#ifdef PLATFORM_WIN32

проц Семафор::отпусти()
{
	ReleaseSemaphore(handle, 1, NULL);
}

проц Семафор::отпусти(цел n)
{
	ReleaseSemaphore(handle, n, NULL);
}

бул Семафор::жди(цел timeout_ms)
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

бул Стопор::пробуйВойти()
{
	return TryEnterCriticalSection(&section);
}

/* Win32 ЧЗСтопор implementation by Chris Thomasson, cristom@comcast.net */

проц ЧЗСтопор::войдиЗ()
{
	EnterCriticalSection ( &m_wrlock );
	LONG count = InterlockedExchangeAdd(&m_count, -LONG_MAX);
	if(count < LONG_MAX)
		if(InterlockedExchangeAdd ( &m_rdwake, LONG_MAX - count ) + LONG_MAX - count )
			WaitForSingleObject ( m_wrwset, INFINITE );
}

проц ЧЗСтопор::выйдиЗ()
{
	LONG count = InterlockedExchangeAdd ( &m_count, LONG_MAX );
	if (count < 0)
	    ReleaseSemaphore ( m_rdwset, count * -1, 0 );
	LeaveCriticalSection ( &m_wrlock );
}

проц ЧЗСтопор::войдиЧ()
{
	LONG count = InterlockedDecrement ( &m_count );
	if(count < 0)
		WaitForSingleObject ( m_rdwset, INFINITE );
}

проц ЧЗСтопор::выйдиЧ()
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

бул ПеременнаяУсловия::жди(Стопор& m, цел timeout_ms)
{
	if(InitializeConditionVariable)
		return SleepConditionVariableCS(cv, &m.section, timeout_ms < 0 ? INFINITE : timeout_ms);
	else { // WindowsXP implementation
		static thread_local ббайт буфер[sizeof(ЖдущаяНить)]; // only one жди per thread is possible
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
		бул r = w->sem.жди(timeout_ms);
		m.войди();
		w->ЖдущаяНить::~ЖдущаяНить();
		return r;
	}
}

проц ПеременнаяУсловия::Signal()
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

проц ПеременнаяУсловия::Broadcast()
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

бул ПеременнаяУсловия::жди(Стопор& m, цел timeout_ms)
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

проц Семафор::отпусти()
{
	dispatch_semaphore_signal(sem);
}

бул Семафор::жди(цел timeout_ms)
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

проц Семафор::отпусти()
{
	sem_post(&sem);
}

бул Семафор::жди(цел timeout_ms)
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

проц ФоновыйОбнов::инивалидируй()
{
	dirty.store(true, std::memory_order_release);
	dirty = true;
}

бул ФоновыйОбнов::начниОбнов() const
{
	бул b = dirty.load(std::memory_order_acquire);
	if(b) {
		mutex.войди();
		if(dirty) return true;
		mutex.выйди();
	}
	return false;
}

проц ФоновыйОбнов::завершиОбнов() const
{
	dirty.store(false, std::memory_order_release);
	mutex.выйди();
}

ФоновыйОбнов::ФоновыйОбнов()
{
	dirty = true;
}

проц СпинЗамок::жди()
{
	volatile цел n = 0;
	while(locked) {
	#ifdef CPU_X86
		_mm_pause();
	#endif
		if(n++ > 500)
			Sleep(0);
	}
}

бул стартВспомНити(auxthread_t (auxthread__ *фн)(ук укз), ук укз)
{
#ifdef PLATFORM_WIN32
	HANDLE handle;
	handle = CreateThread(NULL, 512*1024, фн, укз, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL);
	if(handle) {
		CloseHandle(handle);
		return true;
	}
#endif
#ifdef PLATFORM_POSIX
	pthread_t handle;
	if(pthread_create(&handle, 0, фн, укз) == 0) {
		pthread_detach(handle);
		return true;
	}
#endif
	return false;
}

