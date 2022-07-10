#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define STATUS_WX86_CONTINUE             0x4000001D
#define STATUS_WX86_SINGLE_STEP          0x4000001E
#define STATUS_WX86_BREAKPOINT           0x4000001F
#define STATUS_WX86_EXCEPTION_CONTINUE   0x40000020
#define STATUS_WX86_EXCEPTION_LASTCHANCE 0x40000021
#define STATUS_WX86_EXCEPTION_CHAIN      0x40000022

#define LLOG(x)  // DLOG(x)

Ткст Pdb::Гекс(adr_t a)
{
	return фмт64Гекс(a);
}

void Pdb::Ошибка(const char *s)
{
	Ткст txt = "Ошибка!&";
	if(s)
		txt << s << "&";
	LLOG("ОШИБКА: " << DeQtf(дайПоследнОшСооб()));
	Exclamation(txt + DeQtf(дайПоследнОшСооб()));
	running = false;
	стоп();
}

void Pdb::UnloadModuleSymbols()
{
	for(int i = 0; i < module.дайСчёт(); i++) {
		ИнфОМодуле& f = module[i];
		if(f.symbols) {
			SymUnloadModule64(hProcess, f.base);
			LLOG("Выгружены символы для " << f.path << ' ' << Гекс(module[i].base) << '/' << hProcess);
		}
	}
}

int  Pdb::FindModuleIndex(adr_t base)
{
	for(int i = 0; i < module.дайСчёт(); i++)
		if(module[i].base == base)
			return i;
	return -1;
}

void Pdb::LoadModuleInfo()
{
	fninfo_cache.очисть();

	ИнфОМодуле f;
	dword cb = 1;
	HMODULE  h;
	if(!EnumProcessModules(hProcess, &h, sizeof(HMODULE), &cb)) {
		Ошибка();
		return;
	}
	int n = cb / sizeof(HMODULE);
	Буфер<HMODULE> m(n);
	if(!EnumProcessModules(hProcess, m, cb, &cb)) {
		Ошибка();
		return;
	}
	Вектор<ИнфОМодуле> nm;
	for (int i = 0; i < n; i++) {
		MODULEINFO mi;
		if(GetModuleInformation(hProcess, m[i], &mi, sizeof(mi))) {
			ИнфОМодуле& f = nm.добавь();
			f.base = (adr_t)mi.lpBaseOfDll;
			f.size = mi.SizeOfImage;
			int q = FindModuleIndex(f.base);
			if(q >= 0) {
				ИнфОМодуле& of = module[q];
				f.path = of.path;
				f.symbols = of.symbols;
				of.symbols = false;
				LLOG("Stable " << Гекс(f.base) << " (" << Гекс(f.size) << "): " << f.path);
			}
			else {
				char имя[MAX_PATH];
				if(GetModuleFileNameEx(hProcess, m[i], имя, MAX_PATH)) {
					f.path = имя;
					adr_t w = (adr_t)SymLoadModule64(hProcess, NULL, имя, 0, f.base, f.size);
					if(w) {
						LLOG("Loading symbols " << имя << ' ' << Гекс(f.base) << '/' << hProcess << " returned base " << Гекс(w));
						f.symbols = true;
						LoadGlobals(w);
					}
				}
				LLOG(Гекс(f.base) << " (" << Гекс(f.size) << "): " << f.path);
			}
		}
	}
	UnloadModuleSymbols();
	module = pick(nm);
	refreshmodules = false;
}

bool Pdb::AddBp(adr_t address)
{
	LLOG("AddBp: " << Гекс(address));
	if(bp_set.найди(address) >= 0)
		return true;
	byte prev;
	if(!ReadProcessMemory(hProcess, (LPCVOID) address, &prev, 1, NULL))
		return false;
	LLOG("ReadProcessMemory OK");
	byte int3 = 0xcc;
	if(!WriteProcessMemory(hProcess, (LPVOID) address, &int3, 1, NULL))
		return false;
	LLOG("WriteProcessMemory OK");
//	FlushInstructionCache (hProcess, (LPCVOID)address, 1);
	bp_set.помести(address, prev);
	return true;
}

bool Pdb::RemoveBp(adr_t address)
{
	int pos = bp_set.найди(address);
	if(pos < 0)
		return true;
	if(!WriteProcessMemory(hProcess, (LPVOID)address, &bp_set[pos], 1, NULL))
		return false;
	FlushInstructionCache(hProcess, (LPCVOID)address, 1);
	bp_set.отлинкуй(pos);
	return true;
}

bool Pdb::RemoveBp()
{
	for(int i = bp_set.дайСчёт(); --i >= 0;)
		if(!bp_set.отлинкован(i)) {
			adr_t address = bp_set.дайКлюч(i);
			if(!WriteProcessMemory(hProcess, (LPVOID)address, &bp_set[i], 1, NULL))
				return false;
			FlushInstructionCache(hProcess, (LPCVOID)address, 1);
			bp_set.отлинкуй(i);
		}
	bp_set.очисть();
	return true;
}

void Pdb::SyncFrameButtons()
{
	int ii = framelist.дайИндекс();
	frame_down.вкл(!lock && ii >= 0 && ii < framelist.дайСчёт() - 1);
	frame_up.вкл(!lock && ii > 0);
}

void Pdb::блокируй()
{
	if(lock == 0) {
		исрСкройУк();
		исрОтладБлокируй();
		watches.откл();
		locals.откл();
		framelist.откл();
		SyncFrameButtons();
		frame_down.откл();
		frame_up.откл();
		dlock.покажи();
		исрУстБар();
	}
	lock++;
}

void Pdb::разблокируй()
{
	lock--;
	if(lock == 0) {
		исрОтладРазблокируй();
		watches.вкл();
		locals.вкл();
		framelist.вкл();
		SyncFrameButtons();
		dlock.скрой();
		исрУстБар();
	}
}

Pdb::Контекст Pdb::читайКонтекст(HANDLE hThread)
{
	DR_LOG("читайКонтекст");
	Контекст r;
#ifdef CPU_64
	if(win64) {
		CONTEXT ctx;
		ctx.ContextFlags = CONTEXT_FULL;
		if(!GetThreadContext(hThread, &ctx))
			Ошибка("GetThreadContext failed");
		memcpy(&r.context64, &ctx, sizeof(CONTEXT));
	}
	else {
		static BOOL (WINAPI *Wow64GetThreadContext)(HANDLE hThread, PWOW64_CONTEXT lpContext);
		ONCELOCK
			фнДлл(Wow64GetThreadContext, "Kernel32.dll", "Wow64GetThreadContext");
		
		WOW64_CONTEXT ctx;
		ctx.ContextFlags = WOW64_CONTEXT_FULL;
		if(!Wow64GetThreadContext || !Wow64GetThreadContext(hThread, &ctx))
			Ошибка("Wow64GetThreadContext failed");
		memcpy(&r.context32, &ctx, sizeof(WOW64_CONTEXT));
	}
#else
	CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_FULL;
	if(!GetThreadContext(hThread, &ctx))
			Ошибка("GetThreadContext failed");
	memcpy(&r.context32, &ctx, sizeof(CONTEXT));
#endif
	return r;
}

void Pdb::пишиКонтекст(HANDLE hThread, Контекст& context)
{
	DR_LOG("пишиКонтекст");
#ifdef CPU_64
	if(win64) {
		CONTEXT ctx;
		memcpy(&ctx, &context.context64, sizeof(CONTEXT));
		ctx.ContextFlags = CONTEXT_CONTROL;
		if(!SetThreadContext(hThread, &ctx))
			Ошибка("SetThreadContext failed");
	}
	else {
		static BOOL (WINAPI *Wow64SetThreadContext)(HANDLE hThread, PWOW64_CONTEXT lpContext);
		фнДлл(Wow64SetThreadContext, "Kernel32.dll", "Wow64SetThreadContext");

		WOW64_CONTEXT ctx;
		memcpy(&ctx, &context.context32, sizeof(WOW64_CONTEXT));
		ctx.ContextFlags = CONTEXT_CONTROL;
		if(!Wow64SetThreadContext || !Wow64SetThreadContext(hThread, &ctx))
			Ошибка("Wow64SetThreadContext failed");
	}
#else
	CONTEXT ctx;
	memcpy(&ctx, &context.context32, sizeof(WOW64_CONTEXT));
	ctx.ContextFlags = CONTEXT_CONTROL;
	if(!SetThreadContext(hThread, &ctx))
		Ошибка("SetThreadContext failed");
#endif
}

void Pdb::добавьНить(dword dwThreadId, HANDLE hThread)
{
	if(threads.найди(dwThreadId) >= 0)
		return;
	DR_LOG("добавьНить");
	Нить& f = threads.дайДобавь(dwThreadId);
	// Retrive "base-level" stack-pointer, to have limit for stackwalks:
	Контекст c = читайКонтекст(hThread);
#ifdef CPU_64
	f.sp = win64 ? c.context64.Rsp : c.context32.Esp;
#else
	f.sp = c.context32.Esp;
#endif
	f.hThread = hThread;
	LLOG("Adding thread " << dwThreadId << ", Нить SP: " << Гекс(f.sp) << ", handle: " << фмтЦелГекс((dword)(uintptr_t)(hThread)));
}

void Pdb::RemoveThread(dword dwThreadId)
{
	int q = threads.найди(dwThreadId);
	if(q >= 0) {
		Нить& f = threads[q];
		LLOG("Closing thread " << dwThreadId << ", handle: " << фмтЦелГекс((dword)(uintptr_t)(f.hThread)));
		CloseHandle(f.hThread);
		threads.удали(q);
	}
}

#define EXID(id)       { id, #id },

struct {
	adr_t code;
	const char *text;
}
ex_desc[] = {
	EXID(EXCEPTION_ACCESS_VIOLATION)
	EXID(EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
	EXID(EXCEPTION_DATATYPE_MISALIGNMENT)
	EXID(EXCEPTION_FLT_DENORMAL_OPERAND)
	EXID(EXCEPTION_FLT_DIVIDE_BY_ZERO)
	EXID(EXCEPTION_FLT_INEXACT_RESULT)
	EXID(EXCEPTION_FLT_INVALID_OPERATION)
	EXID(EXCEPTION_FLT_OVERFLOW)
	EXID(EXCEPTION_FLT_STACK_CHECK)
	EXID(EXCEPTION_FLT_UNDERFLOW)
	EXID(EXCEPTION_ILLEGAL_INSTRUCTION)
	EXID(EXCEPTION_IN_PAGE_ERROR)
	EXID(EXCEPTION_INT_DIVIDE_BY_ZERO)
	EXID(EXCEPTION_INT_OVERFLOW)
	EXID(EXCEPTION_INVALID_DISPOSITION)
	EXID(EXCEPTION_NONCONTINUABLE_EXCEPTION)
	EXID(EXCEPTION_PRIV_INSTRUCTION)
	EXID(EXCEPTION_STACK_OVERFLOW )
};

void Pdb::RestoreForeground()
{
	if(hWnd) {
		SetForegroundWindow(hWnd);
		LLOG("Restored foreground window: " << фмтЦелГекс((dword)(uintptr_t)hWnd));
	}
	hWnd = NULL;
}

void Pdb::SaveForeground()
{
	HWND hwnd = GetForegroundWindow();
	if(hwnd) {
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);
		if(pid == processid) {
			hWnd = hwnd;
			LLOG("Saved foreground window: " << Гекс((adr_t)hWnd));
		}
	}
}

void Pdb::ToForeground()
{
	ТопОкно *w = дайТопОкно();
	if(w && !w->пп_ли()) {
		LLOG("Setting theide as foreground");
		w->устПП();
	}
}

bool Pdb::RunToException()
{
	DR_LOG("RunToException");
	LLOG("RUN TO EXCEPTION");
	ТаймСтоп ts;
	bool disasfocus = disas.естьФокус();
	bool locked = false;
	bool frestored = false;
	invalidpage.очисть();
	mempage.очисть();
	int opn = 0;
	for(;;) {
		if(terminated) {
			if(locked)
				разблокируй();
			return false;
		}
		opn++;
		DR_LOG("WaitForDebugEvent");
		if(WaitForDebugEvent(&event, 0)) {
			DR_LOG("WaitForDebugEvent ended");
			debug_threadid = event.dwThreadId;
			opn = 0;
			running = false;
			switch(event.dwDebugEventCode) {
			case EXCEPTION_DEBUG_EVENT: {
				DR_LOG("EXCEPTION_DEBUG_EVENT");
				LLOG("Exception: " << фмтЦелГекс(event.u.Exception.ExceptionRecord.ExceptionCode) <<
				     " at: " << фмтЦелГекс(event.u.Exception.ExceptionRecord.ExceptionAddress) <<
				     " first: " << event.u.Exception.dwFirstChance);
				SaveForeground();
				const EXCEPTION_RECORD& x = event.u.Exception.ExceptionRecord;
				if(findarg(x.ExceptionCode, EXCEPTION_BREAKPOINT, EXCEPTION_SINGLE_STEP,
				                            STATUS_WX86_BREAKPOINT, STATUS_WX86_SINGLE_STEP) < 0)
				{
					LLOG("Non-debug EXCEPTION");
					Ткст desc = фмт("Exception: [* %lX] at [* %16llX]&",
					                     (int64)x.ExceptionCode, (int64)x.ExceptionAddress);
					bool known = false;
					for(int i = 0; i < __countof(ex_desc); i++)
						if(ex_desc[i].code == x.ExceptionCode) {
							known = true;
							desc << "[* " << DeQtf(ex_desc[i].text) << "]&";
							break;
						}
					if(event.u.Exception.dwFirstChance && !known) {
						LLOG("First chance " << фмтЦелГекс(x.ExceptionCode));
						break;
					}
					if(x.ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
						desc << (x.ExceptionInformation[0] ? "[*@3 writing]" : "[*@4 reading]");
						desc << фмт(" at [* %08llX]", (int64)x.ExceptionInformation[1]);
					}
					ToForeground();
					бипОшибка();
					if(first_exception) {
						first_exception = false;
						ErrorOK(desc);
					}
					else
					if(!Prompt(Ктрл::дайИмяПрил(), CtrlImg::error(), desc, t_("OK"), t_("Стоп"))) {
						стоп();
						return false;
					}
				}
#ifdef CPU_64
				if(!win64 && x.ExceptionCode == EXCEPTION_BREAKPOINT && !break_running) // Ignore x64 breakpoint in wow64
					break;
#endif
				if(break_running)
					debug_threadid = mainThreadId;
				break_running = false;
				ToForeground();
				if(disasfocus)
					disas.устФокус();
				if(locked)
					разблокируй();
				if(refreshmodules)
					LoadModuleInfo();
				LLOG("event.dwThreadId = " << event.dwThreadId);
				bool isbreakpoint = findarg(x.ExceptionCode, EXCEPTION_BREAKPOINT, STATUS_WX86_BREAKPOINT) >= 0;
				for(int i = 0; i < threads.дайСчёт(); i++) {
					Нить& t = threads[i];
					(Контекст&)t = читайКонтекст(threads[i].hThread);
					if(event.dwThreadId == threads.дайКлюч(i)) {
						LLOG("Setting current context");
						if(isbreakpoint
#ifdef CPU_64
						   && bp_set.найди((win64 ? t.context64.Rip : t.context32.Eip) - 1) >= 0
#else
						   && bp_set.найди(t.context32.Eip - 1) >= 0
#endif
						) { // We have stopped at breakpoint, need to move address back
					#ifdef CPU_64
							if(win64)
								t.context64.Rip--;
							else
					#endif
								t.context32.Eip--;
						}
						context = t;
					}
				}
				RemoveBp();
				return true;
			}
			case CREATE_THREAD_DEBUG_EVENT:
				DR_LOG("CREATE_THREAD_DEBUG_EVENT");
				LLOG("создай thread: " << event.dwThreadId);
				добавьНить(event.dwThreadId, event.u.CreateThread.hThread);
				break;
			case EXIT_THREAD_DEBUG_EVENT:
				DR_LOG("EXIT_THREAD_DEBUG_EVENT");
				LLOG("выход thread: " << event.dwThreadId);
				RemoveThread(event.dwThreadId);
				break;
			case CREATE_PROCESS_DEBUG_EVENT:
				DR_LOG("CREATE_PROCESS_DEBUG_EVENT");
				LLOG("создай process: " << event.dwProcessId);
				processid = event.dwProcessId;
				добавьНить(event.dwThreadId, event.u.CreateProcessInfo.hThread);
				CloseHandle(event.u.CreateProcessInfo.hFile);
				CloseHandle(event.u.CreateProcessInfo.hProcess);
				break;
			case EXIT_PROCESS_DEBUG_EVENT:
				DR_LOG("EXIT_PROCESS_DEBUG_EVENT");
				LLOG("выход process: " << event.dwProcessId);
				if(locked)
					разблокируй();
				стоп();
				return false;
			case LOAD_DLL_DEBUG_EVENT: {
				DR_LOG("LOAD_DLL_DEBUG_EVENT");
				LLOG("грузи dll: " << event.u.LoadDll.lpBaseOfDll);
				CloseHandle(event.u.LoadDll.hFile);
				refreshmodules = true;
				break;
			}
			case UNLOAD_DLL_DEBUG_EVENT:
				DR_LOG("UNLOAD_DLL_DEBUG_EVENT");
				LLOG("UnLoad dll: " << event.u.UnloadDll.lpBaseOfDll);
				refreshmodules = true;
				break;
			case RIP_EVENT:
				DR_LOG("RIP_EVENT");
				LLOG("RIP!");
				Exclamation("Process being debugged died unexpectedly!");
				if(locked)
					разблокируй();
				стоп();
				return false;
			}
			DR_LOG("ContinueDebugEvent");
			ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
			running = true;
		}
		if(ts.прошло() > 200) {
			DR_LOG("ts.Elpsed() > 200");
			if(!lock) {
				блокируй();
				locked = true;
			}
			if(!frestored) {
				RestoreForeground();
				frestored = true;
			}
		}
		if(lock) {
			DR_LOG("гипСпи");
			гипСпи(opn < 1000 ? 0 : 100);
			Ктрл::обработайСобытия();
		}
		else {
			DR_LOG("спи");
			Sleep(opn < 1000 ? 0 : 100);
		}
	}
}

/*
const CONTEXT& Pdb::CurrentContext()
{
	return threads.дай((int)~threadlist).context;
}
*/

void Pdb::пишиКонтекст()
{
	пишиКонтекст(threads.дай(event.dwThreadId).hThread, context);
}

bool Pdb::SingleStep()
{
	LLOG("SINGLE STEP 0");
#if CPU_64
	if(win64)
		context.context64.EFlags |= 0x100;
	else
#endif
		context.context32.EFlags |= 0x100;
	пишиКонтекст();
	running = true;
	ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
	if(!RunToException())
		return false;
#if CPU_64
	if(win64)
		context.context64.EFlags &= ~0x100;
	else
#endif
		context.context32.EFlags &= ~0x100;
	пишиКонтекст();
	return true;
}

bool Pdb::Continue()
{
	LLOG("** Continue");
	running = true;
	ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
	return RunToException();
}

void Pdb::SetBreakpoints()
{
	RemoveBp();
	for(int i = 0; i < breakpoint.дайСчёт(); i++)
		AddBp(breakpoint[i]);
}

void Pdb::BreakRunning() //TODO: фиксируй in wow64?
{
	stop = true;
	if(running) {
		BOOL (WINAPI *debugbreak)(HANDLE Process);
		debugbreak = (BOOL (WINAPI *)(HANDLE))
		             GetProcAddress(GetModuleHandle("kernel32.dll"), "DebugBreakProcess");
		if(debugbreak) {
			LLOG("=== DebugBreakProcess");
			break_running = true;
			(*debugbreak)(hProcess);
		}
		else
			Exclamation("Операция на этой ОС не поддерживается");
	}
}

#endif
