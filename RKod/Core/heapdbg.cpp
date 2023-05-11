#include "Core.h"

// #define LOGAF

namespace РНЦПДинрус {

static bool  sIgnoreNonMainLeaks;
static bool  sIgnoreNonUppThreadsLeaks;

static dword serial_number = 0;
static dword serial_main_begin;
static dword serial_main_end;

dword MemoryGetCurrentSerial() { return serial_number; }

void MemoryIgnoreNonMainLeaks()
{ // ignore leaks outside _APP_MAIN
	sIgnoreNonMainLeaks = true;
}

void MemoryIgnoreNonUppThreadsLeaks()
{ // ignore leaks in threads not launched by U++ Нить
	sIgnoreNonUppThreadsLeaks = true;
}

void MemorySetMainBegin__()
{
	serial_main_begin = serial_number;
}

void MemorySetMainEnd__()
{
	serial_main_end = serial_number;
}

};

#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(КУЧА_РНЦП)

int сНачСчётДиагПам;

#endif

namespace РНЦПДинрус {

extern bool AppNormalExit;

#if defined(КУЧА_РНЦП)

#include "HeapImp.h"

#if defined(HEAPDBG)

extern bool PanicMode;
void паникаКучи(const char *text, void *pos, int size);

static СтатическийСтопор sHeapLock2;

struct alignas(16) DbgBlkHeader {
	size_t        size;
	DbgBlkHeader *prev;
	DbgBlkHeader *next;
	dword         serial;

	void линкуйся() {
		next = prev = this;
	}
	void отлинкуй() {
		prev->next = next;
		next->prev = prev;
	}
	void вставь(DbgBlkHeader *lnk) {
		lnk->prev = this;
		lnk->next = next;
		next->prev = lnk;
		next = lnk;
	}
};

static const char *DbgFormat(char *b, DbgBlkHeader *p)
{
	sprintf(b, "--memory-breakpoint__ %u ", (unsigned int)~(p->serial ^ (uintptr_t)p));
	return b;
}

static void DbgHeapPanic(const char *text, DbgBlkHeader *p)
{
	char h[256];
	char b[100];
	strcpy(h, text);
	strcat(h, DbgFormat(b, p));
	паникаКучи(h, p + 1, (int)(uintptr_t)p->size);
}

static DbgBlkHeader dbg_live = { 0, &dbg_live, &dbg_live, 0 };

static dword s_allocbreakpoint;
static thread_local dword s_ignoreleaks;

void MemoryIgnoreLeaksBegin()
{
	Стопор::Замок __(sHeapLock2);
	s_ignoreleaks++;
}

void MemoryIgnoreLeaksEnd()
{
	Стопор::Замок __(sHeapLock2);
	s_ignoreleaks--;
}

void MemoryBreakpoint(dword serial)
{
	s_allocbreakpoint = serial;
}

void *MemoryAllocSz_(size_t& size);

void  DbgSet(DbgBlkHeader *p, size_t size)
{
	bool allow_leak = s_ignoreleaks ||
	                  sIgnoreNonUppThreadsLeaks && !Нить::рнцп_ли() && !Нить::главная_ли()
#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(КУЧА_РНЦП)
	                  || сНачСчётДиагПам == 0
#endif
	;

	p->serial = allow_leak ? 0 : ~ ++serial_number ^ (dword)(uintptr_t) p;
	p->size = size;
	if(s_allocbreakpoint && s_allocbreakpoint == serial_number)
		__ВСЁ__;
	dbg_live.вставь(p);
	Poke32le((byte *)(p + 1) + p->size, p->serial);
}

void *MemoryAllocSz(size_t& size)
{
	if(PanicMode)
		return malloc(size);
	Стопор::Замок __(sHeapLock2);
	size += sizeof(DbgBlkHeader) + sizeof(dword);
	DbgBlkHeader *p = (DbgBlkHeader *)MemoryAllocSz_(size);
	size -= sizeof(DbgBlkHeader) + sizeof(dword);
	DbgSet(p, size);
#ifdef LOGAF
	char h[200];
	sprintf(h, "ALLOCATED %d at %p - %p", size, p + 1, (byte *)(p + 1) + size);
	DLOG(h);
#endif
	return p + 1;
}

void *MemoryAlloc(size_t size)
{
	return MemoryAllocSz(size);
}

void MemoryFree_(void *ptr);

void DbgCheck(DbgBlkHeader *p)
{
	if((dword)Peek32le((byte *)(p + 1) + p->size) != p->serial)
		DbgHeapPanic("Куча повреждена ", p);
}

void MemoryFree(void *ptr)
{
#ifdef LOGAF
	char h[200];
	sprintf(h, "FREE %p", ptr);
	DLOG(h);
#endif
	if(PanicMode)
		return;
	if(!ptr) return;
	Стопор::Замок __(sHeapLock2);
	DbgBlkHeader *p = (DbgBlkHeader *)ptr - 1;
	DbgCheck(p);
	p->отлинкуй();
	MemoryFree_(p);
}

bool MemoryTryRealloc_(void *ptr, size_t& newsize);

bool MemoryTryRealloc__(void *ptr, size_t& newsize)
{
	if(!ptr || PanicMode) return false;
	Стопор::Замок __(sHeapLock2);
	DbgBlkHeader *p = (DbgBlkHeader *)ptr - 1;
	DbgCheck(p);
	size_t sz = newsize;
	sz += sizeof(DbgBlkHeader) + sizeof(dword);
	if(MemoryTryRealloc_((DbgBlkHeader *)ptr - 1, sz)) {
		newsize = sz - sizeof(DbgBlkHeader) - sizeof(dword);
		p->отлинкуй();
		DbgSet(p, newsize);
		return true;
	}
	return false;
}

size_t GetMemoryBlockSize_(void *ptr);

size_t GetMemoryBlockSize(void *ptr)
{
	if(!ptr) return 0;
	return ((DbgBlkHeader *)ptr - 1)->size;
}

void *MemoryAlloc32()             { return MemoryAlloc(32); }
void  MemoryFree32(void *ptr)     { return MemoryFree(ptr); }

void MemoryCheckDebug()
{
	if(PanicMode)
		return;
	MemoryCheck();
	Стопор::Замок __(sHeapLock2);
	DbgBlkHeader *p = dbg_live.next;
	while(p != &dbg_live) {
		if((dword)Peek32le((byte *)(p + 1) + p->size) != p->serial)
			DbgHeapPanic("ПРОВЕРКА КУЧИ: Куча повреждена ", p);
		p = p->next;
	}
	while(p != &dbg_live);
}

void MemoryDumpLeaks()
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
	DbgBlkHeader *p = dbg_live.next;
	bool leaks = false;
	int n = 0;
	while(p != &dbg_live) {
		dword serial = (unsigned int)~(p->serial ^ (uintptr_t)p);
		if(p->serial && (!sIgnoreNonMainLeaks || serial >= serial_main_begin && serial < serial_main_end)) {
			if(!leaks)
				VppLog() << "\n\nHeap leaks detected:\n";
			leaks = true;
			char b[100];
			DbgFormat(b, p);
			VppLog() << '\n' << b << ": ";
			гексДамп(VppLog(), p + 1, (int)(uintptr_t)p->size, 64);
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

#ifdef COMPILER_MSC

#pragma warning(disable: 4074)
#pragma init_seg(compiler)

ЭКЗИТБЛОК { MemoryDumpLeaks(); }

#endif

#ifdef COMPILER_GCC

void диагностикаИницПамяти()
{
}

#endif

#endif
#endif


}

#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(COMPILER_GCC) && defined(КУЧА_РНЦП)

MemDiagCls::MemDiagCls()
{
	if(сНачСчётДиагПам++ == 0)
		РНЦП::диагностикаИницПамяти();
}

MemDiagCls::~MemDiagCls()
{
	if(--сНачСчётДиагПам == 0)
		РНЦП::MemoryDumpLeaks();
}

static const MemDiagCls sMemDiagHelper __attribute__ ((init_priority (101)));

#endif