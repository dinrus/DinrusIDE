#include "CtrlCore.h"

namespace РНЦП {

// #define LOG_QUEUE
#define LLOG(x) // LOG(x)
// #define DELAY_WATCH  1000 _DBG_

int MemoryProbeInt;

struct СобытиеВремени : public Линк<СобытиеВремени> {
	dword      time;
	int        delay;
	Событие<>    cb;
	void      *id;
	bool       rep;
};

static dword sTClick;

static СтатическаяКритСекция sTimerLock;

struct CtrlTimerOwner__ : public ЛинкВлад<СобытиеВремени> {
	CtrlTimerOwner__();
	~CtrlTimerOwner__();
};

static СобытиеВремени *tevents() {
	static ЛинкВлад<СобытиеВремени> t;
	return t.дайУк();
}

static void sTimeCallback(dword time, int delay, Событие<>  cb, void *id) {
	СобытиеВремени *ne = tevents()->вставьПредш();
	ne->time = time;
	ne->cb = cb;
	ne->delay = delay;
	ne->id = id;
	ne->rep = false;
	LLOG("sTimeCalllback " << ne->time << " " << ne->delay << " " << ne->id);
}

void устОбрвызВремени(int delay_ms, Функция<void ()> cb, void *id) {
	Стопор::Замок __(sTimerLock);
	ПРОВЕРЬ(абс(delay_ms) < 0x40000000);
	LLOG("устОбрвызВремени " << delay_ms << " " << id);
	sTimeCallback(msecs() + абс(delay_ms), delay_ms, Событие<> () << cb, id);
}

void KillTimeCallbacks(void *id, void *idlim) {
	Стопор::Замок __(sTimerLock);
	СобытиеВремени *list = tevents();
	for(СобытиеВремени *e = list->дайСледщ(); e != list;)
		if(e->id >= id && e->id < idlim) {
			e = e->дайСледщ();
			delete e->дайПредш();
		}
		else
			e = e->дайСледщ();
}

ЭКЗИТБЛОК
{
	Стопор::Замок __(sTimerLock);
	while(tevents()->дайСледщ() != tevents())
		delete tevents()->дайСледщ();
}

bool естьОбрвызВремени(void *id) {
	Стопор::Замок __(sTimerLock);
	СобытиеВремени *list = tevents();
	for(СобытиеВремени *e = list->дайСледщ(); e != list; e = e->дайСледщ())
		if(e->id == id)
			return true;
	return false;
}

void глушиОбрвызВремени(void *id) {
	KillTimeCallbacks(id, (byte *)id + 1);
}

void Ктрл::TimerProc(dword time)
{
	if(режимПаники_ли())
		return;
	sTimerLock.войди();
	СобытиеВремени *list = tevents();
	if(time == sTClick) {
		sTimerLock.выйди();
		return;
	}
	sTClick = time;
	sTimerLock.выйди();
	Ктрл::CheckMouseCtrl();
	Ктрл::SyncCaret();
	sTimerLock.войди();

	#ifdef LOG_QUEUE
		LLOG("--- Timer queue at " << time);
		for(СобытиеВремени *e = list->дайСледщ(); e != list; e = e->дайСледщ())
			LLOG("TP " << e->time << " " << e->delay << " " << e->id << " " << e->rep);
		LLOG("----");
	#endif

	for(;;) {
		СобытиеВремени *todo = NULL;
		int maxtm = -1;
		for(СобытиеВремени *e = list->дайСледщ(); e != list; e = e->дайСледщ()) {
			int tm = (int)(time - e->time);
			if(!e->rep && tm >= 0 && tm > maxtm) {
				maxtm = tm;
				todo = e;
			}
		}
		if(!todo)
			break;
		LLOG("Performing " << todo->time << " " << todo->delay << " " << todo->id);
		Событие<>  cb = todo->cb;
		if(todo->delay < 0)
			todo->rep = true;
		else
			delete todo;
		sTimerLock.выйди();
	#if DELAY_WATCH
		int tm = msecs();
	#endif
		cb();
	#if DELAY_WATCH
		if(msecs() - tm > DELAY_WATCH)
			паника("Long timer procedure detected!");
	#endif
		sTimerLock.войди();
	}
	time = msecs();
	LLOG("--- Rescheduling at " << time);
	СобытиеВремени *e = list->дайСледщ();
	while(e != list) {
		СобытиеВремени *w = e;
		e = e->дайСледщ();
		if(w->rep) {
			LLOG("Rescheduling " << e->id);
			sTimeCallback(time - w->delay, w->delay, w->cb, w->id);
			delete w;
		}
	}
	LLOG("----");
	sTimerLock.выйди();
}

void  Ктрл::иницТаймер()
{
	Стопор::Замок __(sTimerLock);
	tevents();
}

void  Ктрл::устОбрвызВремени(int delay_ms, Функция<void ()> cb, int id) {
	ПРОВЕРЬ(id >= 0 && (size_t)id < (int)sizeof(Ктрл));
	РНЦП::устОбрвызВремени(delay_ms, cb, (byte *)this + id);
}

void  Ктрл::глушиОбрвызВремени(int id) {
	ПРОВЕРЬ(id >= 0 && (size_t)id < sizeof(Ктрл));
	РНЦП::глушиОбрвызВремени((byte *)this + id);
}

void  Ктрл::KillSetTimeCallback(int delay_ms, Функция<void ()> cb, int id)
{
	глушиОбрвызВремени(id);
	устОбрвызВремени(delay_ms, cb, id);
}

void  Ктрл::постОбрвыз(Функция<void ()> cb, int id)
{
	устОбрвызВремени(0, cb, id);
}

void  Ктрл::KillPostCallback(Функция<void ()> cb, int id)
{
	KillSetTimeCallback(0, cb, id);
}

bool  Ктрл::естьОбрвызВремени(int id) const {
	ПРОВЕРЬ(id >= 0 && (size_t)id < sizeof(Ктрл));
	return РНЦП::естьОбрвызВремени((byte *)this + id);
}

dword GetTimeClick()
{
	return sTClick;
}

}
