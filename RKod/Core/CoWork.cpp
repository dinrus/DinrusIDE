#include "Core.h"

namespace РНЦПДинрус {

#define LLOG(x)       // RLOG(x)
#define LDUMP(x)      // DDUMP(x)

#define LTIMING(x)    // RTIMING(x)
#define LHITCOUNT(x)  // RHITCOUNT(x)

thread_local bool    СоРабота::Пул::финблок;
thread_local int     СоРабота::индекс_трудяги = -1;
thread_local СоРабота *СоРабота::текущ;

СоРабота::Пул& СоРабота::дайПул()
{
	static СоРабота::Пул пул;
	return пул;
}

void СоРабота::Пул::освободи(МРабота& job)
{
	job.link_next[0] = free;
	free = &job;
}

void СоРабота::Пул::иницНити(int nthreads)
{
	LLOG("Пул::иницНити: " << nthreads);
	for(int i = 0; i < nthreads; i++)
		CHECK(threads.добавь().выполниХорошо([=] { индекс_трудяги = i; пускНити(i); }, true));
}

void СоРабота::Пул::покиньНити()
{
	lock.войди();
	quit = true;
	lock.выйди();
	waitforjob.Broadcast();
	for(int i = 0; i < threads.дайСчёт(); i++)
		threads[i].жди();
	threads.очисть();
	lock.войди();
	quit = false;
	lock.выйди();
}

int СоРабота::дайРазмерПула()
{
	int n = дайПул().threads.дайСчёт();
	return n ? n : цпбЯдра() + 2;
}

СоРабота::Пул::Пул()
{
	ПРОВЕРЬ(!трудяга_ли());

	иницНити(цпбЯдра() + 2);

	free = NULL;
	for(int i = 0; i < SCHEDULED_MAX; i++)
		освободи(slot[i]);
	
	quit = false;
}

СоРабота::Пул::~Пул()
{
	ПРОВЕРЬ(!трудяга_ли());
	LLOG("Quit");
	покиньНити();
	for(int i = 0; i < SCHEDULED_MAX; i++)
		slot[i].линкуйся();
	LLOG("Quit ended");
}

void СоРабота::финБлок()
{
	if(текущ && !Пул::финблок) {
		Пул::финблок = true;
		дайПул().lock.войди();
	}
}

void СоРабота::Пул::работай(МРабота& job)
{
	LLOG("работай (СоРабота " << фмтЦелГекс(job.work) << ")");
	финблок = false;

	СоРабота *work = job.work;
	СоРабота::текущ = work;
	bool looper = job.looper;
	Функция<void ()> фн;
	if(looper) {
		ПРОВЕРЬ(work);
		if(--work->looper_count <= 0) {
			job.отлинкуйВсе();
			освободи(job);
		}
	}
	else {
		job.отлинкуйВсе();
		фн = pick(job.фн);
		освободи(job); // using 'job' after this point is grave Ошибка....
	}

	lock.выйди();
	std::exception_ptr exc = nullptr;
	try {
		if(looper)
			work->looper_fn();
		else
			фн();
	}
	catch(...) {
		LLOG("работай захватил искл");
		exc = std::current_exception();
	}
	СоРабота::текущ = NULL;
	if(!финблок)
		lock.войди();
	if(!work)
		return;
	if(exc && !work->exc) {
		work->canceled = true;
		work->отмени0();
		work->exc = exc;
	}
	else
	if(looper)
		work->отмени0();
	if(--work->todo == 0) {
		LLOG("Releasing waitforfinish of (СоРабота " << фмтЦелГекс(work) << ")");
		work->waitforfinish.Signal();
	}
	LLOG("DoJobA, todo: " << work->todo << " (СоРабота " << фмтЦелГекс(work) << ")");
	ПРОВЕРЬ(work->todo >= 0);
	LLOG("Finished, remaining todo " << work->todo);
}

void СоРабота::Пул::пускНити(int tno)
{
	LLOG("СоРабота thread #" << tno << " started");
	Пул& p = дайПул();
	p.lock.войди();
	for(;;) {
		while(!p.jobs.вСписке()) {
			LHITCOUNT("СоРабота: Parking thread to жди");
			if(p.quit) {
				p.lock.выйди();
				return;
			}
			p.waiting_threads++;
			LLOG("#" << tno << " Waiting for job");
			p.waitforjob.жди(p.lock);
			LLOG("#" << tno << " Waiting ended");
			p.waiting_threads--;
		}
		LLOG("#" << tno << " Job acquired");
		LHITCOUNT("СоРабота: Running new job");
		p.работай(*p.jobs.дайСледщ());
		LLOG("#" << tno << " Job finished");
	}
	p.lock.выйди();
	LLOG("СоРабота thread #" << tno << " finished");
}

void СоРабота::Пул::суньРаботу(Функция<void ()>&& фн, СоРабота *work, bool looper)
{
	ПРОВЕРЬ(free);
	МРабота& job = *free;
	free = job.link_next[0];
	job.линкПосле(&jobs);
	if(work)
		job.линкПосле(&work->jobs, 1);
	job.work = work;
	job.looper = looper;
	if(looper) {
		work->looper_fn = pick(фн);
		work->looper_count = дайРазмерПула();
	}
	else
		job.фн = pick(фн);
	LLOG("Adding job");
	if(looper)
		waitforjob.Broadcast();
	else
	if(waiting_threads) {
		LTIMING("Releasing thread waiting for job");
		LLOG("Releasing thread waiting for job, waiting threads: " << waiting_threads);
		waitforjob.Signal();
	}
}

bool СоРабота::пробуйПлан(Функция<void ()>&& фн)
{
	Пул& p = дайПул();
	Стопор::Замок __(p.lock);
	if(!p.free)
		return false;
	p.суньРаботу(pick(фн), NULL);
	return true;
}

void СоРабота::планируй(Функция<void ()>&& фн)
{
	while(!пробуйПлан(pick(фн))) Sleep(0);
}

void СоРабота::делай0(Функция<void ()>&& фн, bool looper)
{
	LTIMING("Scheduling обрвыз");
	LHITCOUNT("СоРабота: Scheduling обрвыз");
	LLOG("делай0, looper: " << looper << ", previous todo: " << todo);
	Пул& p = дайПул();
	p.lock.войди();
	if(!p.free) {
		LLOG("Stack full: running in the originating thread");
		LHITCOUNT("СоРабота: Stack full: Running in originating thread");
		p.lock.выйди();
		Пул::финблок = false;
		фн();
		if(Пул::финблок)
			p.lock.выйди();
		return;
	}
	p.суньРаботу(pick(фн), this, looper);
	if(looper)
		todo += дайРазмерПула();
	else
		++todo;
	p.lock.выйди();
}

void СоРабота::цикл(Функция<void ()>&& фн)
{
	индекс = 0;
	делай0(pick(фн), true);
	финиш();
}

void СоРабота::отмени0()
{
	LLOG("СоРабота отмени0");
	Пул& p = дайПул();
	while(!jobs.пустой(1)) {
		LHITCOUNT("СоРабота::Canceling scheduled Job");
		МРабота& job = *jobs.дайСледщ(1);
		job.отлинкуйВсе();
		if(job.looper)
			todo -= job.work->looper_count;
		else
			--todo;
		p.освободи(job);
	}
}

void СоРабота::финишируй0()
{
	Пул& p = дайПул();
	while(todo) {
		LLOG("WaitForFinish (СоРабота " << фмтЦелГекс(this) << ")");
		waitforfinish.жди(p.lock);
	}
	canceled = false;
	if(exc) {
		LLOG("СоРабота rethrowing worker exception");
		auto e = exc;
		exc = nullptr;
		p.lock.выйди();
		std::rethrow_exception(e);
	}
}

void СоРабота::отмена()
{
	Пул& p = дайПул();
	p.lock.войди();
	canceled = true;
	отмени0();
	финишируй0();
	p.lock.выйди();
	LLOG("СоРабота " << фмтЦелГекс(this) << " canceled and finished");
}

void СоРабота::финиш() {
	Пул& p = дайПул();
	p.lock.войди();
	while(!jobs.пустой(1)) {
		LLOG("финиш: todo: " << todo << " (СоРабота " << фмтЦелГекс(this) << ")");
		p.работай(*jobs.дайСледщ(1));
	}
	финишируй0();
	p.lock.выйди();
	LLOG("СоРабота " << фмтЦелГекс(this) << " finished");
}

bool СоРабота::финишировал()
{
	Пул& p = дайПул();
	p.lock.войди();
	bool b = todo == 0;
	p.lock.выйди();
	return b;
}

void СоРабота::устРазмерПула(int n)
{
	Пул& p = дайПул();
	p.покиньНити();
	p.иницНити(n);
}

void СоРабота::пайп(int stepi, Функция<void ()>&& фн)
{
	Стопор::Замок __(stepmutex);
	auto& q = step.по(stepi);
	LLOG("Step " << stepi << ", count: " << q.дайСчёт() << ", running: " << steprunning.дайСчёт());
	q.добавьГолову(pick(фн));
	if(!steprunning.по(stepi, false)) {
		steprunning.по(stepi) = true;
		*this & [=]() {
			LLOG("Starting step " << stepi << " processor");
			stepmutex.войди();
			for(;;) {
				Функция<void ()> f;
				auto& q = step[stepi];
				LLOG("StepWork " << stepi << ", todo:" << q.дайСчёт());
				if(q.дайСчёт() == 0)
					break;
				f = pick(q.дайХвост());
				q.сбросьХвост();
				stepmutex.выйди();
				f();
				stepmutex.войди();
			}
			steprunning.по(stepi) = false;
			stepmutex.выйди();
			LLOG("Exiting step " << stepi << " processor");
		};
	}
}

void СоРабота::переустанов()
{
	try {
		отмена();
	}
	catch(...) {}
	todo = 0;
	canceled = false;
}

bool СоРабота::отменён()
{
	return текущ && текущ->canceled;
}

int СоРабота::дайИндексТрудяги()
{
	return индекс_трудяги;
}

СоРабота::СоРабота()
{
	LLOG("СоРабота конструировал " << фмтГекс(this));
	todo = 0;
	canceled = false;
}

СоРабота::~СоРабота() noexcept(false)
{
	финиш();
	LLOG("~СоРабота " << фмтЦелГекс(this));
}

}
