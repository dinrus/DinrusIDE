#include "CtrlCore.h"

#define LLOG(x)   //  DLOG(x)

namespace РНЦП {

static СтатическийСтопор sGLock;

// #define DELAY_WATCH 1000 _DBG_

static thread_local int sGLockLevel = 0;

void войдиВСтопорГип()
{
	LLOG(">войдиВСтопорГип " << sGLockLevel << ' ' << главнаяНить_ли());
	if(sGLockLevel++ == 0) {
#if DELAY_WATCH
		for(int i = 0; i < DELAY_WATCH; i++) {
			if(sGLock.пробуйВойти()) return;
			спи(1);
		}
		паника("Long timer procedure detected!");
#else
		sGLock.войди();
#endif
	}
	LLOG("войдиВСтопорГип " << sGLockLevel << ' ' << главнаяНить_ли());
}

void войдиВСтопорГип(int n)
{
	LLOG(">войдиВСтопорГип: " << n << ' ' << sGLockLevel << ' ' << главнаяНить_ли());
	if(n > 0) {
		if(sGLockLevel == 0)
			sGLock.войди();
		sGLockLevel += n;
	}
	LLOG("войдиВСтопорГип " << sGLockLevel << ' ' << главнаяНить_ли());
}

bool пробуйВойтиВСтопорГип()
{
	if(sGLockLevel == 0 && sGLock.пробуйВойти()) {
		sGLockLevel++;
		return true;
	}
	return false;
}

void покиньСтопорГип()
{
	LLOG(">покиньСтопорГип " << sGLockLevel << ' ' << главнаяНить_ли());
	ПРОВЕРЬ(sGLockLevel > 0);
	if(--sGLockLevel == 0)
		sGLock.выйди();
	LLOG("покиньСтопорГип " << sGLockLevel << ' ' << главнаяНить_ли());
}

int покиньВсеСтопорыГип()
{
	LLOG(">покиньВсеСтопорыГип " << sGLockLevel << ' ' << главнаяНить_ли());
	int q = sGLockLevel;
	if(q) {
		sGLock.выйди();
		sGLockLevel = 0;
	}
	LLOG("покиньВсеСтопорыГип " << q << ' ' << главнаяНить_ли());
	return q;
}

bool уНитиЕстьЗамокГип()
{
	return Нить::главная_ли() || sGLockLevel;
}

int  дайУровеньЗамкаГип()
{
	return sGLockLevel;
}

void WakeUpGuiThread();

struct Ктрл::CallBox {
	Семафор sem;
	Событие<>   cb;
};

void Ктрл::PerformCall(Ктрл::CallBox *cbox)
{
	cbox->cb();
	cbox->sem.отпусти();
}

void Ктрл::Call(Функция<void ()> cb)
{
	if(главнаяНить_ли())
		cb();
	else {
		CallBox cbox;
		cbox.cb << cb;
		РНЦП::постОбрвыз(callback1(PerformCall, &cbox));
		WakeUpGuiThread();
		int level = покиньВсеСтопорыГип(); // разблокируй GUI to give main thread chance to handle Call
		cbox.sem.жди();
		войдиВСтопорГип(level);
	}
}

void Ктрл::шатдаунНитей()
{
	Нить::начниШатдаунНитей();
	while(Нить::дайСчёт()) {
		обработайСобытия();
		Sleep(0);
	}
	Нить::завершиШатдаунНитей();
}

}
