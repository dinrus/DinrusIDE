#include "Debuggers.h"

#define LLOG(x)   // DLOG(x)

#define IMAGECLASS DbgImg
#define IMAGEFILE  <DinrusIDE/Debuggers/Debuggers.iml>
#include <Draw/iml_source.h>

const char *найдиТэг(const char *txt, const char *tag)
{
	while(*txt) {
		const char *q = txt;
		const char *t = tag;
		while(*q == *t) {
			q++;
			t++;
			if(*t == '\0') return txt;
		}
		txt++;
	}
	return NULL;
}

const char *AfterTag(const char *txt, const char *tag)
{
	while(*txt) {
		const char *q = txt;
		const char *t = tag;
		while(*q == *t) {
			q++;
			t++;
			if(*t == '\0') return q;
		}
		txt++;
	}
	return NULL;
}

const char *AfterHeading(const char *txt, const char *heading)
{
	while(*heading)
		if(*txt++ != *heading++)
			return NULL;
	return txt;
}

void RedDisplay::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword s) const {
	w.DrawRect(r, paper);
	DrawSmartText(w, r.left, r.top, r.устШирину(), Ткст(q), StdFont(), светлоКрасный());
}

ВекторМап<Ткст, Ткст> DataMap(const КтрлМассив& data)
{
	ЗамкниГип __;
	
	ВекторМап<Ткст, Ткст> m;
	for(int i = 0; i < data.дайСчёт(); i++)
		m.добавь(data.дай(i, 0), data.дай(i, 1));
	return m;
}

void MarkChanged(const ВекторМап<Ткст, Ткст>& m, КтрлМассив& data)
{
	ЗамкниГип __;
	
	for(int i = 0; i < data.дайСчёт(); i++) {
		int q = m.найди(data.дай(i, 0));
		if(q >= 0 && m[q] != data.дай(i, 1))
			data.устДисплей(i, 1, Single<RedDisplay>());
		else
			data.устДисплей(i, 1, StdDisplay());
	}
}

void Gdb::SyncFrameButtons()
{
	int ii = frame.дайИндекс();
	bool lock = исрОтладБлокировка_ли();
	frame_down.вкл(!lock && ii >= 0 && (ii < frame.дайСчёт() - 1 || frame.дайСчёт() == 1));
	frame_up.вкл(!lock && ii > 0);
}

void Gdb::блокируй()
{
	исрОтладБлокируй();
	watches.откл();
	locals.откл();
	frame.откл();
	SyncFrameButtons();
	dlock.покажи();
}

void Gdb::разблокируй()
{
	if(исрОтладРазблокируй()) {
		watches.вкл();
		locals.вкл();
		frame.вкл();
		SyncFrameButtons();
		dlock.скрой();
	}
}

bool Gdb::результат(Ткст& result, const Ткст& s)
{
	result.конкат(s);
	int l = result.дайДлину();
	int q = result.найди(GDB_PROMPT, max(0, l - 50));
	if(q >= 0) {
		result.обрежь(q);
		return true;
	}
	return false;
}

Ткст Gdb::Cmd(const char *command, bool start)
{
	if(!dbg.пущен() || исрОтладБлокировка_ли()) return Null;
	ТаймСтоп ts;
	блокируй();
	if(command) {
		LLOG("========= Cmd: " << command);
		dbg.пиши(Ткст(command) + "\n");
		PutVerbose(Ткст() << "Команда: " << command);
	}
	Ткст result;
	int ms0 = msecs();
	while(dbg.пущен()) {
		Ткст s;
		if(!dbg.читай(s)) {
			PutVerbose(result);
			PutVerbose("Отладчит остановлен");
			LLOG("Выполняется: " << dbg.пущен());
			break;
		}
		if(!s.пустой() && результат(result, s)) {
			LLOG(result);
			PutVerbose(result);
			if(start) {
				start = false;
				result = s.середина(result.дайСчёт());
			}
			else
				break;
		}
		if(ms0 != msecs()) {
			обработайСобытия();
			ms0 = msecs();
		}
		
		гипСпи(50);
		
		if(TTYQuit())
			стоп();
	}
	разблокируй();
	if(command) {
		PutVerbose(Ткст() << "Время у `" << command <<"` " << ts);
	}
	PutVerbose("=========== Результат:");
	PutVerbose(result);
	PutVerbose("===================");
#ifdef PLATFORM_POSIX
	system("xdotool ключ XF86Ungrab"); // force X11 to relese the mouse capture
#endif
	return result;
}

Ткст Gdb::FastCmd(const char *command)
{
	if(!dbg.пущен() || исрОтладБлокировка_ли()) return Null;
	bool lock = false;
	if(command) {
		dbg.пиши(Ткст(command) + "\n");
		PutVerbose(Ткст() << "Быстрая команда: " << command);
	}
	Ткст result;
	ТаймСтоп ts;
	while(dbg.пущен()) {
		Ткст s;
		if(TTYQuit()) {
			LLOG("TTYQuit");
			стоп();
		}
		if(!dbg.читай(s)) {
			LLOG(result);
			PutVerbose(result);
			PutVerbose("dbg terminated");
			LLOG("Выполняется: " << dbg.пущен());
			break;
		}
		if(!s.пустой() && результат(result, s)) {
			LLOG(result);
			LLOG("Длина результата: " << result.дайДлину());
			if(result.дайДлину() < 1000)
				PutVerbose(result);
			break;
		}
		if(s.дайСчёт() == 0)
			Sleep(0);
		if(ts.прошло() > 500) {
			if(!lock) {
				lock = true;
				блокируй();
			}
			Ктрл::обработайСобытия();
		}
	}
	if(lock)
		разблокируй();
#ifdef _ОТЛАДКА
	if(command) {
		PutVerbose(Ткст() << "Время у `" << command <<"` " << ts);
	}
#endif
	PutVerbose("Результат: " + result);
	return result;
}

void Gdb::стоп()
{
	LLOG("Стоп");
	if(dbg.пущен())
		dbg.глуши();
}

bool Gdb::окончен()
{
	return !dbg.пущен() && !исрОтладБлокировка_ли();
}
