#include <DinrusPro/DinrusCore.h>

// #define LOGAF

static бул  sIgnoreNonMainLeaks;
static бул  sIgnoreNonUppThreadsLeaks;

static бцел serial_number = 0;
static бцел serial_main_begin;
static бцел serial_main_end;

бцел MemoryGetCurrentSerial() { return serial_number; }

проц MemoryIgnoreNonMainLeaks()
{ // ignore leaks outside _APP_MAIN
	sIgnoreNonMainLeaks = true;
}

проц MemoryIgnoreNonUppThreadsLeaks()
{ // ignore leaks in threads not launched by U++ Нить
	sIgnoreNonUppThreadsLeaks = true;
}

проц MemorySetMainBegin__()
{
	serial_main_begin = serial_number;
}

проц MemorySetMainEnd__()
{
	serial_main_end = serial_number;
}

#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(КУЧА_РНЦП)

цел сНачСчётДиагПам;

#endif

extern бул AppNormalExit;

#if defined(КУЧА_РНЦП)

#include "HeapImp.h"

#if defined(HEAPDBG)

extern бул PanicMode;
проц паникаКучи(кткст0 text, ук pos, цел size);

static СтатическийСтопор sHeapLock2;

struct alignas(16) DbgЗагБлока {
	т_мера        size;
	DbgЗагБлока *prev;
	DbgЗагБлока *next;
	бцел         serial;

	проц линкуйся() {
		next = prev = this;
	}
	проц отлинкуй() {
		prev->next = next;
		next->prev = prev;
	}
	проц вставь(DbgЗагБлока *lnk) {
		lnk->prev = this;
		lnk->next = next;
		next->prev = lnk;
		next = lnk;
	}
};

static кткст0 DbgFormat(сим *b, DbgЗагБлока *p)
{
	sprintf(b, "--memory-breakpoint__ %u ", (бцел)~(p->serial ^ (uintptr_t)p));
	return b;
}

static проц DbgHeapPanic(кткст0 text, DbgЗагБлока *p)
{
	сим h[256];
	сим b[100];
	strcpy(h, text);
	strcat(h, DbgFormat(b, p));
	паникаКучи(h, p + 1, (цел)(uintptr_t)p->size);
}

static DbgЗагБлока dbg_live = { 0, &dbg_live, &dbg_live, 0 };

static бцел s_allocbreakpoint;
static thread_local бцел s_ignoreleaks;

проц MemoryIgnoreLeaksBegin()
{
	Стопор::Замок __(sHeapLock2);
	s_ignoreleaks++;
}

проц MemoryIgnoreLeaksEnd()
{
	Стопор::Замок __(sHeapLock2);
	s_ignoreleaks--;
}

проц остановТчкПам(бцел serial)
{
	s_allocbreakpoint = serial;
}

ук разместиПамТн_(т_мера& size);

проц  DbgSet(DbgЗагБлока *p, т_мера size)
{
	бул allow_leak = s_ignoreleaks ||
	                  sIgnoreNonUppThreadsLeaks && !Нить::рнцп_ли() && !Нить::главная_ли()
#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(КУЧА_РНЦП)
	                  || сНачСчётДиагПам == 0
#endif
	;

	p->serial = allow_leak ? 0 : ~ ++serial_number ^ (бцел)(uintptr_t) p;
	p->size = size;
	if(s_allocbreakpoint && s_allocbreakpoint == serial_number)
		__ВСЁ__;
	dbg_live.вставь(p);
	помести32лэ((ббайт *)(p + 1) + p->size, p->serial);
}

ук разместиПамТн(т_мера& size)
{
	if(PanicMode)
		return malloc(size);
	Стопор::Замок __(sHeapLock2);
	size += sizeof(DbgЗагБлока) + sizeof(бцел);
	DbgЗагБлока *p = (DbgЗагБлока *)разместиПамТн_(size);
	size -= sizeof(DbgЗагБлока) + sizeof(бцел);
	DbgSet(p, size);
#ifdef LOGAF
	сим h[200];
	sprintf(h, "ALLOCATED %d at %p - %p", size, p + 1, (ббайт *)(p + 1) + size);
	DLOG(h);
#endif
	return p + 1;
}

ук разместиПам(т_мера size)
{
	return разместиПамТн(size);
}

проц освободиПам_(ук укз);

проц DbgCheck(DbgЗагБлока *p)
{
	if((бцел)подбери32лэ((ббайт *)(p + 1) + p->size) != p->serial)
		DbgHeapPanic("Куча повреждена ", p);
}

проц освободиПам(ук укз)
{
#ifdef LOGAF
	сим h[200];
	sprintf(h, "FREE %p", укз);
	DLOG(h);
#endif
	if(PanicMode)
		return;
	if(!укз) return;
	Стопор::Замок __(sHeapLock2);
	DbgЗагБлока *p = (DbgЗагБлока *)укз - 1;
	DbgCheck(p);
	p->отлинкуй();
	освободиПам_(p);
}

бул MemoryпробуйПеремест_(ук укз, т_мера& newsize);

бул MemoryпробуйПеремест__(ук укз, т_мера& newsize)
{
	if(!укз || PanicMode) return false;
	Стопор::Замок __(sHeapLock2);
	DbgЗагБлока *p = (DbgЗагБлока *)укз - 1;
	DbgCheck(p);
	т_мера sz = newsize;
	sz += sizeof(DbgЗагБлока) + sizeof(бцел);
	if(MemoryпробуйПеремест_((DbgЗагБлока *)укз - 1, sz)) {
		newsize = sz - sizeof(DbgЗагБлока) - sizeof(бцел);
		p->отлинкуй();
		DbgSet(p, newsize);
		return true;
	}
	return false;
}

т_мера GetMemoryBlockSize_(ук укз);

т_мера дайРазмБлокаПам(ук укз)
{
	if(!укз) return 0;
	return ((DbgЗагБлока *)укз - 1)->size;
}

ук разместиПам32()             { return разместиПам(32); }
проц  освободиПам32(ук укз)     { return освободиПам(укз); }

проц MemoryCheckDebug()
{
	if(PanicMode)
		return;
	проверьПам();
	Стопор::Замок __(sHeapLock2);
	DbgЗагБлока *p = dbg_live.next;
	while(p != &dbg_live) {
		if((бцел)подбери32лэ((ббайт *)(p + 1) + p->size) != p->serial)
			DbgHeapPanic("ПРОВЕРКА КУЧИ: Куча повреждена ", p);
		p = p->next;
	}
	while(p != &dbg_live);
}

проц MemoryDumpLeaks()
{
	if(PanicMode)
		return;
#ifdef PLATFORM_MACOS
	return; // ignore leaks in macos
#endif
	if(главнаяПущена()) {
		VppLog() << "Application was terminated in a non-standard way (e.g. exit(x) call or Ктрл+C)\n";
	}
#ifndef PLATFORM_POSIX
	if(s_ignoreleaks)
		паника("Ignore leaks старт/стоп mismatch!");
#endif
	MemoryCheckDebug();
	DbgЗагБлока *p = dbg_live.next;
	бул leaks = false;
	цел n = 0;
	while(p != &dbg_live) {
		бцел serial = (бцел)~(p->serial ^ (uintptr_t)p);
		if(p->serial && (!sIgnoreNonMainLeaks || serial >= serial_main_begin && serial < serial_main_end)) {
			if(!leaks)
				VppLog() << "\n\nHeap leaks detected:\n";
			leaks = true;
			сим b[100];
			DbgFormat(b, p);
			VppLog() << '\n' << b << ": ";
			гексДамп(VppLog(), p + 1, (цел)(uintptr_t)p->size, 64);
			if(++n > 16) {
				while(p->next != &dbg_live && n < 10000000) {
					++n;
					p = p->next;
				}
				VppLog() << "\n*** TOO MANY LEAKS (" << n << ") TO LIST THEM ALL\n";
				break;
			}
		}
		p = p->next;
	}
	if(!leaks)
		return;
#ifdef PLATFORM_WIN32
	MessageBox(::GetActiveWindow(),
	           "Обнаружены утечки из кучи !",
	           "Предупреждение",
	           MB_ICONSTOP|MB_OK|MB_APPLMODAL);
#else
	if(!режимПаники_ли())
		паника("Обнаружены утечки из кучи!");
#endif
	Куча::AuxFinalCheck();
}

#ifdef COMPILER_GCC

проц диагнозИницПам()
{
}

#endif

#endif
#endif


#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(КУЧА_РНЦП)

MemDiagCls::MemDiagCls()
{
	if(сНачСчётДиагПам++ == 0)
		диагнозИницПам();
}

MemDiagCls::~MemDiagCls()
{
	if(--сНачСчётДиагПам == 0)
		MemoryDumpLeaks();
}

static const MemDiagCls sMemDiagHelper __attribute__ ((init_priority (101)));

#endif
