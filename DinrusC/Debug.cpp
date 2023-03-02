#include "Core.h"

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif


namespace РНЦПДинрус {

#define LTIMING(x) // TIMING(x)

void __LOGF__(const char *fmt, ...) {
	char буфер[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(буфер, fmt, argptr);
	va_end(argptr);
	VppLog().помести(буфер);
}

Ткст дайИмяТипа(const char *s)
{
	static const char _struct[] = "struct ", _class[] = "class ";
	enum { LEN_S = sizeof(_struct) - 1, LEN_C = sizeof(_class) - 1 };
	int len = (dword)strlen(s);
	if(len > LEN_C && !memcmp(s, _class, LEN_C))
		s += LEN_C;
	else if(len > LEN_S && !memcmp(s, _struct, LEN_S))
		s += LEN_S;
	return s;
}

void проверьЭталонЛога(const char *etalon_path)
{
#ifdef _ОТЛАДКА
	auto LoadLog = [](const Ткст& path) {
		Ткст s = фильтруй(загрузиФайл(path), [](int c) { return c == '\r' ? 0 : c; });
		return s.середина(max(s.найдиПосле("\n"), 0));
	};
	
	Ткст log = LoadLog(GetStdLogPath());
	Ткст etalon = LoadLog(etalon_path);
	
	ПРОВЕРЬ(log == etalon);
	
	LOG("================= OK");
#endif
}

void проверьЭталонЛога()
{
	проверьЭталонЛога(дайФайлДан("Etalon.log"));
}

bool ТаймингИнспектор::active = true;

static ТаймингИнспектор s_zero; // time of старт / стоп without actual body to measure

ТаймингИнспектор::ТаймингИнспектор(const char *_name) {
	имя = _name ? _name : "";
	all_count = call_count = max_nesting = min_time = max_time = total_time = 0;
	static bool init;
	if(!init) {
#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
		timeBeginPeriod(1);
#endif
		init = true;
	}
}

ТаймингИнспектор::~ТаймингИнспектор() {
	if(this == &s_zero) return;
	Стопор::Замок __(mutex);
	StdLog() << дамп() << "\r\n";
}

void ТаймингИнспектор::добавь(dword time, int nesting)
{
	time = tmGetTime() - time;
	Стопор::Замок __(mutex);
	if(!active) return;
	all_count++;
	if(nesting > max_nesting)
		max_nesting = nesting;
	if(nesting == 0) {
		total_time += time;
		if(call_count++ == 0)
			min_time = max_time = time;
		else {
			if(time < min_time)
				min_time = time;
			if(time > max_time)
				max_time = time;
		}
	}
}

Ткст ТаймингИнспектор::дамп() {
	Стопор::Замок __(mutex);
	Ткст s = спринтф("TIMING %-15s: ", имя);
	if(call_count == 0)
		return s + "No active hit";
	ONCELOCK {
		int w = GetTickCount();
		while(GetTickCount() - w < 200) { // measure profiling overhead
			thread_local int nesting = 0;
			ТаймингИнспектор::Routine __(s_zero, nesting);
		}
	}
	double tm = max(0.0, double(total_time) / call_count / 1000 -
			             double(s_zero.total_time) / s_zero.call_count / 1000);
	return s
	       + времяФмт(tm * call_count)
	       + " - " + времяФмт(tm)
	       + " (" + времяФмт((double)total_time  / 1000) + " / "
	       + спринтф("%d )", call_count)
		   + ", min: " + времяФмт((double)min_time / 1000)
		   + ", max: " + времяФмт((double)max_time / 1000)
		   + спринтф(", nesting: %d - %d", max_nesting, all_count);
}

ИнспекторСчётаНажатий::~ИнспекторСчётаНажатий()
{
	RLOG("HITCOUNT " << имя << ": hit count = " << hitcount);
}

void  гексДампДанных(Поток& s, const void *ptr, int size, bool adr, int maxsize) {
	char h[256];
	int a, b;
	byte *q = (byte *)ptr;
	a = 0;
	if(size > maxsize) size = maxsize;
	while(a < size) {
		if(adr) {
		#ifdef CPU_64
			uint64 aa = a + (uint64)ptr;
			sprintf(h, "%+6d 0x%08X%08X ", a, (int)(aa >> 32), (int)aa);
			s.помести(h);
		#else
			sprintf(h, "%+6d 0x%08X ", a, int(a + dword(ptr)));
			s.помести(h);
		#endif
		}
		else {
			sprintf(h, "%+6d ", a);
			s.помести(h);
		}
		for(b = 0; b < 16; b++)
			if(a + b < size) {
				sprintf(h, "%02X ", q[a + b]);
				s.помести(h);
			}
			else
				s.помести("   ");
		s.помести("    ");
		for(b = 0; b < 16; b++)
			if(a + b < size) {
				int c = q[a + b];
				s.помести(c >= 32 && c < 128 ? c : '.');
			}
			else
				s.помести(' ');
		a += 16;
		s << '\n';
	}
}

void  гексДамп(Поток& s, const void *ptr, int size, int maxsize) {
	char h[256];
	sprintf(h, "Memory at 0x%p, size 0x%X = %d\n", ptr, size, size);
	s.помести(h);
#ifdef PLATFORM_WIN32
	if(IsBadReadPtr(ptr, size)) {
		s.помести("   <MEMORY ACCESS VIOLATION>\n");
		return;
	}
#endif
	гексДампДанных(s, ptr, size, true, maxsize);
}

void логГекс(const Ткст& s)
{
	гексДамп(VppLog(), ~s, s.дайДлину());
}

void логГекс(const ШТкст& s)
{
	гексДамп(VppLog(), ~s, sizeof(wchar) * s.дайДлину());
}

void логГекс(uint64 i)
{
	VppLog() << "0x" << фмт64Гекс(i) << '\n';
}

void логГекс(void *p)
{
	VppLog() << p << '\n';
}

#ifdef CPU_X86
Ткст какТкст(__m128i x)
{
	int32 h[4];
	memcpy(h, &x, 16);
	int16 w[8];
	memcpy(w, &x, 16);
	float f[4];
	memcpy(f, &x, 16);
	return спринтф("_%08x_%08x_%08x_%08x (%d, %d, %d, %d) : (%d, %d, %d, %d) (%f, %f, %f, %f)",
	               h[3], h[2], h[1], h[0], w[7], w[6], w[5], w[4], w[3], w[2], w[1], w[0],
	               f[3], f[2], f[1], f[0]);
}
#endif

void устМагию(byte *t, int count)
{
	for(int i = 0; i < count; i++)
		t[i] = i;
}

void проверьМагию(byte *t, int count)
{
	for(int i = 0; i < count; i++)
		if(t[i] != i)
			паника("Failed magic area!");
}

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)

template <class T>
void помести(HANDLE file, T& данные) {
	dword dummy;
	WriteFile(file, &данные, sizeof(данные), &dummy, NULL);
}

static LPTOP_LEVEL_EXCEPTION_FILTER sPrev;
static dword sESP;
static char  appInfo[20];
static char  crashfilename[MAX_PATH];

void устИмяКрашФайла(const char *cfile)
{
	ПРОВЕРЬ(strlen(cfile) < MAX_PATH);
	strcpy(crashfilename, cfile);
}

LONG __stdcall sDumpHandler(LPEXCEPTION_POINTERS ep) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	if(!*crashfilename) {
		::GetModuleFileName(NULL, crashfilename, 512);
		wsprintf(crashfilename + strlen(crashfilename), ".%d-%02d-%02d-%02d-%02d-%02d%s.crash",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, appInfo);
	}
	HANDLE file = CreateFile(crashfilename, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	dword v = 1;
	помести(file, v);
	EXCEPTION_RECORD *er = ep->ExceptionRecord;
	помести(file, er->ExceptionCode);
	помести(file, er->ExceptionAddress);
	помести(file, er->NumberParameters);
	for(int i = 0; i < (int)er->NumberParameters; i++)
		помести(file, er->ExceptionInformation[i]);

#ifdef CPU_AMD64
	qword esp = ep->ContextRecord->Rsp;
#else
	dword esp = ep->ContextRecord->Esp;
#endif

	WriteFile(file, (void *) esp, (dword)(sESP - esp), &v, NULL);
	/*	dword base = ep->ContextRecord->Ebp;
	for(;;) {
		dword new_base = *(dword *)base;
		dword caller = *(dword *)(base + 4);
		помести(file, caller);
		if(new_base < base)
			break;
		base = new_base;
	}*/
	CloseHandle(file);
	char h[200];
	sprintf(h, "CRASH: %d-%02d-%02d %02d:%02d:%02d code: 0x%X  address: 0x%p",
	        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
	        (int)er->ExceptionCode, er->ExceptionAddress);
	RLOG("============ CRASH ================================================");
	RLOG(h);
	return sPrev ? (*sPrev)(ep) : 0 /*EXCEPTION_CONTINUE_SEARCH*/;
}

void установиКрашДамп(const char *инфо) {
	memset(appInfo, 0, sizeof(appInfo));
	if(инфо && *инфо) {
		appInfo[0] = '.';
		strncpy(appInfo + 1, инфо, sizeof(appInfo) - 1);
		appInfo[sizeof(appInfo) - 1] = '\0';
	}
	if(!sPrev) {
		sPrev = SetUnhandledExceptionFilter(sDumpHandler);
#ifndef CPU_AMD64
#ifdef COMPILER_MSC
		__asm mov sESP, esp
#else
	//todo
#endif
#endif
	}
}

#endif

#ifdef _ОТЛАДКА
// значение inspectors for Gdb_MI2 frontend
dword   _DBG_Value_GetType(Значение const &v)	{ return v.дайТип(); }
Ткст  _DBG_Value_AsString(Значение const &v)	{ return какТкст(v); }
#endif

}

#if defined(__GNUG__) && defined(PLATFORM_POSIX)
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace РНЦПДинрус {

struct cpp_demangle_handle__ {
    char* p;
    cpp_demangle_handle__(char* ptr) : p(ptr) { }
    ~cpp_demangle_handle__() { std::free(p); }
};

Ткст разманглируйСиПП(const char* имя) {
    int status = -4;
    cpp_demangle_handle__ result( abi::__cxa_demangle(имя, NULL, NULL, &status) );
    return (status==0) ? result.p : имя ;
}

}

#else

namespace РНЦПДинрус {

Ткст разманглируйСиПП(const char* имя) {
    return обрежьЛево("struct ", обрежьЛево("class ", имя));
}

}

#endif

