#include "CtrlCore.h"

#ifdef GUI_WIN

#include <imm.h>

namespace РНЦП {

#define LLOG(x)    //   DLOG(x)
// #define LOGTIMING 1 _DBG_

#ifdef _ОТЛАДКА
#define LOGMESSAGES 0
#endif

#define ELOGW(x)   // RLOG(дайСисВремя() << ": " << x) // Only activate in MT!
#define ELOG(x)    // RLOG(дайСисВремя() << ": " << x)

template<>
hash_t дайХэшЗнач(const HWND& h)
{
	return (hash_t)(intptr_t)h;
}

bool Ктрл::дайСооб(MSG& msg)
{
	if(!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) return false;
	return PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE);
}

static bool sFinished;

bool покидает()
{
	return sFinished;
}

static BOOL CALLBACK sDumpWindow(HWND hwnd, LPARAM lParam) {
	Ткст dump;
	dump << (IsWindowEnabled(hwnd) ? "ena" : "dis") << ' '
	     << (IsWindowVisible(hwnd) ? "vis" : "hid") << ' '
	     << спринтф("owner=0x%x ", GetWindow(hwnd, GW_OWNER));
	Ктрл *ctrl = Ктрл::ктрлИзОУК(hwnd);
	if(ctrl) {
#ifdef _ОТЛАДКА
		dump << "Ктрл: " << РНЦП::Имя(ctrl);
#endif
	}
	else if(!lParam)
		return TRUE;
	else
	{
#ifdef PLATFORM_WINCE
		wchar clsname[256], title[1024];
#else
		char clsname[256], title[1024];
#endif
		::GetClassName(hwnd, clsname, __countof(clsname));
		::GetWindowText(hwnd, title, __countof(title));
		dump << "HWND: " << спринтф("0x%x", hwnd) << ", class = "
		     << clsname << ", title = " << title;
	}
	LLOG(dump);
	return TRUE;
}

void DumpWindowOrder(bool aliens) {
	LLOG("DumpWindowOrder" << LOG_BEGIN);
	EnumChildWindows(NULL, &sDumpWindow, (LPARAM)(aliens ? 1 : 0));
	LLOG(LOG_END << "//DumpWindowOrder");
}

HCURSOR   Ктрл::hCursor;
HINSTANCE Ктрл::hInstance;
#ifndef flagDLL
#ifndef PLATFORM_WINCE
HANDLE    Ктрл::OverwatchThread;
HWND      Ктрл::OverwatchHWND;

СобытиеВин32 Ктрл::OverwatchEndSession;
СобытиеВин32 Ктрл::ExitLoopEvent;
#endif
#endif

bool Ктрл::endsession;

void Ктрл::окончиСессию()
{
	ЗамкниГип __;
	EndSessionLoopNo = СобытиеLoopNo;
	endsession = true;
}

template <class U, class V>
void AutoCast(U& a, V b)
{
	a = (U)b;
}

#ifndef flagDLL
#ifndef PLATFORM_WINCE
LRESULT CALLBACK Ктрл::OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_USER) {
		ELOGW("WM_USER");
		PostQuitMessage(0);
	}
	if(msg == WM_QUERYENDSESSION) {
		static BOOL (WINAPI *ShutdownBlockReasonCreate)(HWND hWnd, LPCWSTR pwszReason);
		static BOOL (WINAPI *ShutdownBlockReasonDestroy)(HWND hWnd);
		ONCELOCK {
			if(HMODULE hDLL = LoadLibrary ("user32")) {
				AutoCast(ShutdownBlockReasonCreate, GetProcAddress(hDLL, "ShutdownBlockReasonCreate"));
				AutoCast(ShutdownBlockReasonDestroy, GetProcAddress(hDLL, "ShutdownBlockReasonDestroy"));
			}
		}
		if(ShutdownBlockReasonCreate)
			ShutdownBlockReasonCreate(hwnd, вСисНабсимШ(t_("ожидается ответ пользователя")));
		окончиСессию();
		ELOGW("WM_QUERYENDSESSION 1");
		OverwatchEndSession.жди();
		if(ShutdownBlockReasonDestroy)
			ShutdownBlockReasonDestroy(hwnd);
		ELOGW("WM_QUERYENDSESSION 2");
		return TRUE;
	}
	if(msg == WM_ENDSESSION) {
		окончиСессию();
		ELOGW("WM_ENDSESSION 1");
		ExitLoopEvent.уст();
		ELOGW("WM_ENDSESSION 2");
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD WINAPI Ктрл::Win32OverwatchThread(LPVOID)
{
    WNDCLASS  wc;
    обнули(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)OverwatchWndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = "РНЦП-OVERWATCH";
	RegisterClass(&wc);

	OverwatchHWND = CreateWindowEx(0, "РНЦП-OVERWATCH", "", WS_OVERLAPPEDWINDOW,
	                               -1000, -1000, 50, 50, NULL, NULL, hInstance, NULL);

	ELOGW("OverWatch 1");
	ExitLoopEvent.уст();
	ELOGW("OverWatch 2");
	MSG Msg;
	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
    }
	ELOGW("OverWatch 3");
	return 0;
}
#endif
#endif

HWND utilityHWND = 0;

extern ВекторМап<int, ClipData>& sClipMap();

ИНИЦБЛОК
{
	sClipMap();
}

ЭКЗИТБЛОК
{
	if(utilityHWND) DestroyWindow(utilityHWND);
}

LRESULT CALLBACK Ктрл::UtilityProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	sClipMap();
	switch(message) {
	case WM_TIMER:
		TimerProc(msecs());
		анимируйКаретку();
		break;
	case WM_RENDERFORMAT:
		RenderFormat((dword)wParam);
		return 0;
	case WM_DESTROYCLIPBOARD:
		DestroyClipboard();
		return 0;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

#ifdef PLATFORM_WINCE
#define L_(x)  L##x
#else
#define L_(x)  x
#endif

static DWORD sMainThreadId;

void WakeUpGuiThread()
{
	::PostThreadMessage(sMainThreadId, WM_NULL, 0, 0);
}

void AvoidPaintingCheck__();

static void Win32PanicMessageBox(const char *title, const char *text)
{
	AvoidPaintingCheck__();
#ifdef PLATFORM_WINCE
	static wchar wtext[256], wtitle[256];
	вЮникод(wtext, text, strlen(text), CHARSET_DEFAULT);
	вЮникод(wtitle, title, strlen(title), CHARSET_DEFAULT);
	MessageBox(::GetActiveWindow(), wtext, wtitle, MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#else
	MessageBox(::GetActiveWindow(), text, title, MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#endif
}

void Ктрл::InstallPanicBox()
{
	устБоксСообПаники(&Win32PanicMessageBox);
}

void Ктрл::иницВин32(HINSTANCE hInstance)
{
	ЗамкниГип __;
	LLOG("иницВин32");

	устБоксСообПаники(&Win32PanicMessageBox);
//	RLOGBLOCK("Ктрл::иницВин32");
	sMainThreadId = GetCurrentThreadId();
#define ILOG(x) // RLOG(x)
	Ктрл::hInstance = hInstance;
	{
		ILOG("RegisterClassW");
		WNDCLASSW  wc;
		обнули(wc);
		wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)Ктрл::WndProc;
		wc.hInstance     = hInstance;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszClassName = L"RLLDP-CLASS-W";
		RegisterClassW(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L"RLLDP-CLASS-DS-W";
		RegisterClassW(&wc);
		wc.style         = CS_SAVEBITS|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L"RLLDP-CLASS-SB-W";
		RegisterClassW(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS;
		wc.lpszClassName = L"RLLDP-CLASS-SB-DS-W";
		RegisterClassW(&wc);
	}
	{
		ILOG("RegisterClassA");
		WNDCLASS  wc;
		обнули(wc);
		wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)Ктрл::WndProc;
		wc.hInstance     = hInstance;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = IsWinVista() ? (HBRUSH)(COLOR_WINDOW+1) : (HBRUSH)NULL;
		wc.lpszClassName = L_("RLLDP-CLASS-A");
		RegisterClass(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L_("RLLDP-CLASS-DS-A");
		RegisterClass(&wc);
		wc.style         = CS_SAVEBITS|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L_("RLLDP-CLASS-SB-A");
		RegisterClass(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS;
		wc.lpszClassName = L_("RLLDP-CLASS-SB-DS-A");
		RegisterClass(&wc);
	}

	WNDCLASS  wca;
	обнули(wca);
	wca.hInstance     = hInstance;
	wca.hbrBackground = (HBRUSH)NULL;
	wca.style         = 0;
	wca.lpszClassName = L_("RLLDP-TIMER");
	wca.hCursor       = NULL;
	wca.lpfnWndProc   = &Ктрл::UtilityProc;
	RegisterClass(&wca);

	ILOG("иницТаймер");
	иницТаймер();
	ILOG("SetTimer");
	utilityHWND = CreateWindow(L_("RLLDP-TIMER"), L_(""), WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	SetTimer(utilityHWND, 1, 10, NULL);
	ILOG("Windows");
	Windows(); //?? TRC: what's the use of this?

	ReSkin();

	OleInitialize(NULL);

	Csizeinit();
#undef ILOG

	GlobalBackPaint();
	
	войдиВСтопорГип();
}

typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);

static PSLWA SetLayeredWindowAttributes()
{
	static PSLWA pSet;
#ifndef PLATFORM_WINCE
	static bool inited = false;
	if(!inited) {
		inited = true;
		if(HMODULE hDLL = LoadLibrary ("user32"))
			pSet = (PSLWA) GetProcAddress(hDLL, "SetLayeredWindowAttributes");
	}
#endif
	return pSet;
}

bool Ктрл::IsAlphaSupported()
{
	return SetLayeredWindowAttributes();
}

bool Ктрл::IsCompositedGui()
{
	return false;
}

void Ктрл::покиньВин32()
{
	RenderAllFormats();

	ТопОкно::ShutdownWindows();
	закройТопКтрлы();

	OleUninitialize();

	sFinished = true;

	for(int i = 0; i < hotkey.дайСчёт(); i++)
		if(hotkey[i])
			UnregisterHotKey(NULL, i);
	
	for(int i = 0; i < Windows().дайСчёт(); i++) {
		HWND hwnd = Windows().дайКлюч(i);
		if(hwnd)
			::DestroyWindow(hwnd);
	}
	MSG msg;
	while(дайСооб(msg))
		if(msg.message != WM_QUIT)
			::PostQuitMessage(0);
#ifndef flagDLL
#ifndef PLATFORM_WINCE
	ELOG("покиньВин32 1");
	OverwatchEndSession.уст();
	ELOG("покиньВин32 2");
	PostMessage(OverwatchHWND, WM_USER, 0, 0);
	ELOG("покиньВин32 3");
	LLOG("Waiting for overwatch thread to finish...");
	WaitForSingleObject(OverwatchThread, INFINITE);
	ELOG("покиньВин32 4");
	LLOG("...overwatch thread finished");
#endif
#endif
	покиньСтопорГип();
}

void Ктрл::SetTimerGranularity(int ms)
{
	if(ms > 0)
		SetTimer(utilityHWND, 1, ms, NULL);
	else
		KillTimer(utilityHWND, 1);
}

ВекторМап< HWND, Ук<Ктрл> >& Ктрл::Windows()
{
	static ВекторМап< HWND, Ук<Ктрл> > map;
	return map;
}

Вектор<Ктрл *> Ктрл::дайТопКтрлы()
{
	Вектор<Ктрл *> v;
	ВекторМап< HWND, Ук<Ктрл> >& w = Windows();
	for(int i = 0; i < w.дайСчёт(); i++)
		if(w.дайКлюч(i) && w[i] && !w[i]->parent)
			v.добавь(w[i]);
	return v;
}

void  Ктрл::устКурсорМыши(const Рисунок& image)
{
	ЗамкниГип __;
#ifndef PLATFORM_WINCE
	static Рисунок img;
	if(image.GetSerialId() != img.GetSerialId()) {
		img = image;
		HCURSOR hc = SystemDraw::IconWin32(img, true);
		SetCursor(hc);
		if(hCursor)
			DestroyCursor(hCursor);
		hCursor = hc;
	}
#endif
}

Ктрл *Ктрл::ктрлИзОУК(HWND hwnd)
{
	ЗамкниГип __;
	return hwnd ? Windows().дай(hwnd, NULL) : NULL;
}

HWND Ктрл::дайОУКВладельца() const
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	if(!hwnd) return NULL;
	return GetWindow(hwnd, GW_OWNER);
}

Ктрл *Ктрл::дайВладельца()
{
	ЗамкниГип __;
	HWND hwnd = дайОУКВладельца();
	return hwnd ? ктрлИзОУК(hwnd) : NULL;
}

Ктрл *Ктрл::дайАктивныйКтрл()
{
	ЗамкниГип __;
	if(focusCtrl)
		return focusCtrl->дайТопКтрл();
	HWND actwnd = ::GetActiveWindow();
	Вектор<Ктрл *> top = дайТопКтрлы();
	for(int i = 0; i < top.дайСчёт(); i++)
		if(top[i]->IsActiveX() && top[i]->дайОУК()) {
			LLOG("-> top[" << i << "] = " << фмтЦелГекс(top[i]->дайОУК()));
			for(HWND hwnd = top[i]->дайОУК(); hwnd; hwnd = ::GetParent(hwnd))
				if(hwnd == actwnd) {
					LLOG("-> match for " <<РНЦП::Имя(top[i]));
					return top[i];
				}
		}
	LLOG("//Ктрл::дайАктивныйКтрл -> не найден (ПУСТО)");
	return NULL;
}

ЦельЮБроса *NewUDropTarget(Ктрл *);

Ткст стильОкнаСтрокой(dword style, dword exstyle);

void Ктрл::создай(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow)
{
	ЗамкниГип __;
	ПРОВЕРЬ_(главнаяНить_ли(), "Создание окна производится только в главном потоке");
	LLOG("Ктрл::создай(parent = " << (void *)parent << ") in " <<РНЦП::Имя(this) << LOG_BEGIN);
	ПРОВЕРЬ(!отпрыск_ли() && !открыт());
	Прям r = дайПрям();
	AdjustWindowRectEx(r, style, FALSE, exstyle);
	isopen = true;
	top = new Верх;
	ПРОВЕРЬ(!parent || IsWindow(parent));
	style &= ~WS_VISIBLE;
	dropshadow = false;
	top->hwnd = CreateWindowEx(exstyle,
	                           savebits ? dropshadow ? "RLLDP-CLASS-SB-DS-W" : "RLLDP-CLASS-SB-W"
	                                    : dropshadow ? "RLLDP-CLASS-DS-W" : "RLLDP-CLASS-W",
	                           "", style, 0, 0, 0, 0,
	                           parent, NULL, hInstance, this);

	inloop = false;

	ПРОВЕРЬ(top->hwnd);
	::MoveWindow(top->hwnd, r.left, r.top, r.устШирину(), r.устВысоту(), false); // To avoid "black corners" artifact effect
	::ShowWindow(top->hwnd, visible ? show : SW_HIDE);
//	::UpdateWindow(hwnd);
	StateH(OPEN);
	LLOG(LOG_END << "//Ктрл::создай в " <<РНЦП::Имя(this));
	RegisterDragDrop(top->hwnd, (LPDROPTARGET) (top->dndtgt = NewUDropTarget(this)));
	::ImmAssociateContextEx(top->hwnd, NULL, 0);
	режимОтмены();
	RefreshLayoutDeep();
}

void ReleaseUDropTarget(ЦельЮБроса *dt);

void Ктрл::освободиОкно()
{
	ЗамкниГип __;
	if(!top) return;
	RevokeDragDrop(дайОУК());
	ReleaseUDropTarget(top->dndtgt);
	isopen = false;
	if(!top) return;
	HWND owner = GetWindow(top->hwnd, GW_OWNER);// CXL 31.10.2003 z DoRemove
	bool focus = ::GetFocus() == top->hwnd;
	LLOG("Ктрл::разрушьОкно owner " << (void *)owner
	     << " focus " << focus
	     << " ::GetFocus() " << (void *)::GetFocus());
	if(owner && focus) {
		LLOG("Ктрл::освободиОкно->устФокус " << РНЦП::Имя(Ктрл::ктрлИзОУК(owner)));
		Ктрл *o = дайВладельца();
		if(o && !o->включен_ли()) // owner needs to be enabled, otherwise устФокус bounces back
			o->вкл();
		::SetFocus(owner);
	}
	LLOG(LOG_END << "//Ктрл::освободиОкно() in " <<РНЦП::Имя(this));
	delete top;
	top = NULL;
}

void Ктрл::разрушьОкно()
{
	ЗамкниГип __;
	if(top && top->hwnd) {
		HWND hwnd = top->hwnd;
		освободиОкно();
		::DestroyWindow(hwnd);
	}
}

Рисунок Ктрл::DoMouse(int e, Точка p, int zd)
{
//	LLOG("Ктрл::DoMouse(" << p << ", " << e << ")");
	ЗамкниГип __;
	eventCtrl = this;
	Рисунок img = DispatchMouse(e, p, zd);
	SyncCaret();
	return img;
}

#ifdef _ОТЛАДКА

bool Ктрл::LogMessages;

#define x_MSG(x)       { x, #x },

struct ОконСооб {
	int         ИД;
	const char *имя;
}
sWinMsg[] = {
#include "Win32Msg.i"
	{0, NULL}
};

#endif

void Ктрл::NcCreate(HWND hwnd)
{
	ЗамкниГип __;
	if(!parent)
		top->hwnd = hwnd;
}

void Ктрл::NcDestroy()
{
	ЗамкниГип __;
	if(!parent)
		освободиОкно();
}

bool Ктрл::предобработайСобытие(MSG& msg)
{
	return false;
}

LRESULT CALLBACK Ктрл::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ЗамкниГип __;
	if(sFinished)
		return DefWindowProc(hWnd, message, wParam, lParam);
#ifdef PLATFORM_WINCE
	if(message == WM_CREATE)
#else
	if(message == WM_NCCREATE)
#endif
	{
		Ктрл *w = (Ктрл *)((LPCREATESTRUCT) lParam)->lpCreateParams;
		if(w) {
			w->NcCreate(hWnd);
			int i = Windows().найди(NULL);
			if(i >= 0) {
				Windows().устКлюч(i, hWnd);
				Windows()[i] = w;
			}
			else
				Windows().добавь(hWnd) = w;
		}
	}
	Ктрл *w = Windows().дай(hWnd, NULL);
#ifdef PLATFORM_WINCE
	if(message == WM_DESTROY)
#else
	if(message == WM_NCDESTROY)
#endif
	{
		if(w) w->NcDestroy();
		int i = Windows().найди(hWnd);
		if(i >= 0)
			Windows().устКлюч(i, NULL);
	}
#if LOGMESSAGES
	bool logblk = false;
	if(message != WM_SETCURSOR && message != WM_CTLCOLORBTN && message != WM_TIMER &&
	   message != WM_NCHITTEST  &&  message != WM_ENTERIDLE &&
	   message != WM_CTLCOLORDLG && message != WM_CTLCOLOREDIT && message != WM_CTLCOLORLISTBOX &&
	   message != WM_CTLCOLORMSGBOX && message != WM_CTLCOLORSCROLLBAR &&
	   message != WM_CTLCOLORSTATIC && message != WM_CANCELMODE &&
	   message != 0x0118)
		for(ОконСооб *m = sWinMsg; m->ИД; m++)
			if(m->ИД == message) {
				RLOG(m->имя << ' ' << РНЦП::Имя(w) <<
					спринтф(", wParam = %d (0x%x), lParam = %d (0x%x)",
					       wParam, wParam, lParam, lParam));
				VppLog() << LOG_BEGIN;
				logblk = true;
				break;
			}
#endif
	LRESULT l = 0;
	if(w && (w->дайОУК() || w->isdhctrl)) {
#if LOGTIMING
			int ticks = msecs();
			Ткст wname = w->Имя();
#endif
			Ук<Ктрл> pw = w;
			l = w->WindowProc(message, wParam, lParam);
			if(pw)
				pw->SyncMoves();
#if LOGTIMING
			Ткст msgname;
			for(ОконСооб *m = sWinMsg; m->ИД; m++)
				if(m->ИД == message) {
					msgname = m->имя;
					break;
				}
			if(пусто_ли(msgname))
				msgname = фмтЧ("0x%04x", (int)message);
			RLOG(фмтЧ("T+%d %s 0x%08x 0x%08x -> %s", msecs(ticks), msgname, (int)wParam, (int)lParam, wname));
#endif
	}
	else
		l = DefWindowProc(hWnd, message, wParam, lParam);
#if LOGMESSAGES
	if(logblk)
		VppLog() << LOG_END;
#endif
	return l;
}

bool PassWindowsKey(int wParam)
{
	return wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9
	||     wParam == VK_INSERT || wParam == VK_DELETE
	||     wParam == VK_HOME || wParam == VK_END
	||     wParam == VK_PRIOR || wParam == VK_NEXT
	||     wParam == VK_UP || wParam == VK_DOWN
	||     wParam == VK_LEFT || wParam == VK_RIGHT
	||     wParam == VK_CLEAR || wParam == VK_SPACE
	||     wParam >= 0x90; // OEM keys
}

Вектор<Событие<> > Ктрл::hotkey;

int Ктрл::RegisterSystemHotKey(dword ключ, Функция<void ()> cb)
{
	ПРОВЕРЬ(ключ >= K_DELTA);
	int q = hotkey.дайСчёт();
	for(int i = 0; i < hotkey.дайСчёт(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.по(q) = Событие<> () << cb;
	dword mod = 0;
	if(ключ & K_ALT)
		mod |= MOD_ALT;
	if(ключ & K_SHIFT)
		mod |= MOD_SHIFT;
	if(ключ & K_CTRL)
		mod |= MOD_CONTROL;
	
	return RegisterHotKey(NULL, q, mod, ключ & 0xffff) ? q : -1;
}

void Ктрл::UnregisterSystemHotKey(int id)
{
	if(id >= 0 && id < hotkey.дайСчёт()) {
		UnregisterHotKey(NULL, id);
		hotkey[id].очисть();
	}
}

void Ктрл::sProcessMSG(MSG& msg)
{
	if (msg.message == WM_HOTKEY) {
		if((int)msg.wParam >= 0 && (int)msg.wParam < Ктрл::hotkey.дайСчёт())
			Ктрл::hotkey[(int)msg.wParam]();
		return;
	}
	
	if(!DHCtrl::PreprocessMessageAll(msg))
		if(msg.message != WM_SYSKEYDOWN && msg.message != WM_SYSKEYUP
		|| PassWindowsKey((dword)msg.wParam) || msg.wParam == VK_MENU) //17.11 Mirek - fix to get windows menu invoked on Alt+Space
			TranslateMessage(&msg); // 04/09/07: TRC fix to make barcode reader going better

#if 0
	DDUMP(msg.hwnd);
		for(ОконСооб *m = sWinMsg; m->ИД; m++)
			if(m->ИД == msg.message) {
				RLOG(m->имя << ' ' <<
					спринтф(", wParam = %d (0x%x), lParam = %d (0x%x)",
					        msg.wParam, msg.wParam, msg.lParam, msg.lParam));
				break;
			}

	char cls[200];
	GetClassName(msg.hwnd, cls, 200);
	DDUMP(cls);
#endif

	DispatchMessageW(&msg);
}

bool Ктрл::ожидаетСобытие()
{
	ПРОВЕРЬ_(главнаяНить_ли(), "ожидаетСобытие может выполняться только в главном потоке");
	MSG msg;
	return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool Ктрл::обработайСобытие(bool *quit)
{
	ПРОВЕРЬ_(главнаяНить_ли(), "обработайСобытие  может выполняться только в главном потоке");
	if(!дайЛевуюМыши() && !дайПравуюМыши() && !дайСреднююМыши())
		ReleaseCtrlCapture();
	MSG msg;
	if(дайСооб(msg)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": sProcessMSG " << фмтЦелГекс(msg.message));
		sProcessMSG(msg);
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": //sProcessMSG " << фмтЦелГекс(msg.message));
		DefferedFocusSync();
		SyncCaret();
		PreeditSync([](Ктрл *top, bool enable) {
			if(HWND hwnd = top->дайОУК())
			  ::ImmAssociateContextEx(hwnd, NULL, enable * IACE_DEFAULT);
		});
		return true;
	}
	return false;
}

void Ктрл::SysEndLoop()
{
}

bool Ктрл::обработайСобытия(bool *quit)
{
	ПРОВЕРЬ_(главнаяНить_ли(), "обработайСобытия  может выполняться только в главном потоке");
	if(обработайСобытие(quit)) {
		while(обработайСобытие(quit) && (!LoopCtrl || LoopCtrl->InLoop())); // LoopCtrl-MF 071008
		SweepMkImageCache();
		return true;
	}
	SweepMkImageCache();
	return false;
}

void Ктрл::циклСобытий(Ктрл *ctrl)
{
	ЗамкниГип __;
	ПРОВЕРЬ_(главнаяНить_ли(), "циклСобытий может выполняться только в главном потоке");
	ПРОВЕРЬ(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Вход в цикл событий на уровне " << LoopLevel << LOG_BEGIN);
	Ук<Ктрл> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	int64 loopno = ++СобытиеLoopNo;
	обработайСобытия(&quit);
	while(loopno > EndSessionLoopNo && !quit && (ctrl ? ctrl->открыт() && ctrl->InLoop() : дайТопКтрлы().дайСчёт()))
	{
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": циклСобытий / гипСпи");
		SyncCaret();
		гипСпи(1000);
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": циклСобытий / обработайСобытия");
		обработайСобытия(&quit);
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": циклСобытий / after обработайСобытия");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ктрл::гипСпи(int ms)
{
	ЗамкниГип __;
	ПРОВЕРЬ_(главнаяНить_ли(), "Только главный поток способен выполнять гипСпи");
	ELOG("гипСпи");
	int level = покиньВсеСтопорыГип();
#if !defined(flagDLL) && !defined(PLATFORM_WINCE)
	if(!OverwatchThread) {
		DWORD dummy;
		OverwatchThread = CreateThread(NULL, 0x100000, Win32OverwatchThread, NULL, 0, &dummy);
		ELOG("ExitLoopСобытиеWait 1");
		ExitLoopEvent.жди();
	}
	HANDLE h[1];
	*h = ExitLoopEvent.дайУк();
	ELOG("ExitLoopСобытиеWait 2 " << (void *)*h);
	MsgWaitForMultipleObjects(1, h, FALSE, ms, QS_ALLINPUT);
#else
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
#endif
	войдиВСтопорГип(level);
}

#if 0
void Ктрл::разрушьКареткуОкна()
{
	DLOG("Ктрл::разрушьКареткуОкна()");
	::DestroyCaret();
}

void Ктрл::создайКареткуОкна(const Прям& cr)
{
	ЗамкниГип __;
	DLOG("Ктрл::создайКареткуОкна(" << cr << ") в " << РНЦП::Имя(this));
	HWND hwnd = дайОУК();
	if(!hwnd) return;
	Прям r;
	::GetClientRect(hwnd, r);
	Точка p = r.верхЛево();
	::ClientToScreen(hwnd, p);
	::CreateCaret(hwnd, NULL, cr.устШирину(), cr.устВысоту());
	::SetCaretPos(cr.left - p.x, cr.top - p.y);
	::ShowCaret(hwnd);
}
#else

int  Ктрл::WndCaretTime;
bool Ктрл::WndCaretVisible;

void  Ктрл::анимируйКаретку()
{
	ЗамкниГип __;
	bool v = !(((msecs() - WndCaretTime) / GetCaretBlinkTime()) & 1);
	if(v != WndCaretVisible) {
		WndCaretVisible = v;
		RefreshCaret();
	}
}

void Ктрл::PaintCaret(SystemDraw& w)
{
	ЗамкниГип __;
	LLOG("PaintCaret " << Имя() << ", caretCtrl: " << caretCtrl << ", WndCaretVisible: " << WndCaretVisible);
	if(this == caretCtrl && WndCaretVisible)
		w.DrawRect(caretx, carety, caretcx, caretcy, InvertColor);
}

void Ктрл::устКаретку(int x, int y, int cx, int cy)
{
	ЗамкниГип __;
	LLOG("устКаретку " << Имя() << " " << RectC(x, y, cx, cy));
	if(this == caretCtrl)
		RefreshCaret();
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	if(this == caretCtrl) {
		WndCaretTime = msecs();
		RefreshCaret();
		анимируйКаретку();
	}
}

void Ктрл::SyncCaret() {
	ЗамкниГип __;
	LLOG("SyncCaret");
	if(focusCtrl != caretCtrl) {
		LLOG("SyncCaret ДЕЛАЙ " << РНЦП::Имя(caretCtrl) << " -> " << РНЦП::Имя(focusCtrl));
		RefreshCaret();
		caretCtrl = focusCtrl;
		RefreshCaret();
	}
}
#endif


Прям Ктрл::GetWndScreenRect() const
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	if(!hwnd) return Null;
	Прям r;
	::GetWindowRect(hwnd, r);
	return r;
}

void Ктрл::WndShow(bool b)
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	if(hwnd)
		::ShowWindow(hwnd, b ? SW_SHOW : SW_HIDE);
}

void Ктрл::обновиОкно()
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	if(hwnd) ::UpdateWindow(hwnd);
}

bool Ктрл::IsWndOpen() const {
	ЗамкниГип __;
	return дайОУК();
}

void Ктрл::SetAlpha(byte alpha)
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	if(!IsAlphaSupported() || parent || !top || !hwnd)
		return;
	if(alpha == 255) {
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~0x80000);
		return;
	}
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | 0x80000);
    SetLayeredWindowAttributes() (hwnd, 0, alpha, 2);
}

#define ФИМЯДЛЛ "User32.dll"
#define МОДУЛЬДЛИ   MultiMon
#define DLIHEADER   <CtrlCore/MultiMon.dli>
#include <Core/dli.h>

Прям MonitorRectForHWND(HWND hwnd)
{
	if(hwnd && MultiMon())
		if(HMONITOR monitor = MultiMon().MonitorFromWindow(hwnd, 2/*MONITOR_DEFAULTTONEAREST*/)) {
			MONITORINFO moninfo;
			обнули(moninfo);
			moninfo.cbSize = sizeof(moninfo);
			MultiMon().GetMonitorInfo(monitor, &moninfo);
			return Прям(moninfo.rcWork);
		}
	return Ктрл::GetPrimaryWorkArea();
}

Прям Ктрл::GetWorkArea() const
{
// return MonitorRectForHWND(дайОУК());
// mst:2008-12-08, hack for better multimonitor support.
	ЗамкниГип __;
	const Ктрл *topctl = дайТопКтрл();
	HWND hwnd = topctl->дайОУК();
	if(!hwnd && !((topctl = topctl->GetOwnerCtrl()) && (hwnd = topctl->дайОУК())))
		hwnd = ::GetFocus();
	return MonitorRectForHWND(hwnd);
}

static BOOL CALLBACK sMonEnumProc(HMONITOR monitor, HDC hdc, LPRECT lprcMonitor, LPARAM data)
{
	MONITORINFO moninfo;
	обнули(moninfo);
	moninfo.cbSize = sizeof(moninfo);
	MultiMon().GetMonitorInfo(monitor, &moninfo);
	((Массив<Прям> *)data)->добавь(Прям(moninfo.rcWork));
	return TRUE;
}

void Ктрл::GetWorkArea(Массив<Прям>& rc)
{
	ЗамкниГип __;
	MultiMon().EnumDisplayMonitors(NULL, NULL, &sMonEnumProc, (LPARAM)&rc);
}

Прям Ктрл::GetVirtualWorkArea()
{
	Прям out = GetPrimaryWorkArea();
	Массив<Прям> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.дайСчёт(); i++)
		out |= rc[i];
	return out;
}

Прям Ктрл::GetVirtualScreenArea()
{
	ЗамкниГип __;
	return RectC(GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN));
}

Прям Ктрл::GetPrimaryWorkArea()
{
	Прям r;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
	LLOG("Ктрл::GetWorkArea -> " << r);
	return r;
}

Прям Ктрл::GetPrimaryScreenArea()
{
	return Размер(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

int Ктрл::GetKbdDelay()
{
	ЗамкниГип __;
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &a, 0);
	return 250 + a * 750 / 4;
}

int Ктрл::GetKbdSpeed()
{
	ЗамкниГип __;
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &a, 0);
	return 1000 / (a + 2);
}

void Ктрл::устППОкна()
{
	ЗамкниГип __;
	LLOG("Ктрл::устППОкна() in " << РНЦП::Имя(this));
	HWND hwnd = дайОУК();
	if(hwnd)
		::SetForegroundWindow(hwnd);
}

bool Ктрл::ппОкна_ли() const
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	if(!hwnd)
		return false;
	HWND fore = ::GetForegroundWindow();
	LLOG("Ктрл::ппОкна_ли(): hwnd = " << (void *)hwnd
	     << ", fore = " << (void *)fore << " - " << РНЦП::Имя(ктрлИзОУК(fore)));
	if(IsActiveX()) {
		while(hwnd && hwnd != fore && !!::GetParent(hwnd))
			hwnd = ::GetParent(hwnd);
	}
	return hwnd == fore;
}

void Ктрл::WndEnable(bool b)
{
	ЗамкниГип __;
	LLOG("Ктрл::WndEnable(" << b << ") in " << РНЦП::Имя(this) << ", focusCtrlWnd = " << РНЦП::Имя(~focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
	if(b)
		ReleaseCapture();
	LLOG("//Ктрл::WndEnable(" << b << ") -> false " <<РНЦП::Имя(this) << ", focusCtrlWnd = " <<РНЦП::Имя(~focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
}

bool Ктрл::устФокусОкна()
{
	ЗамкниГип __;
	LLOG("Ктрл::устФокусОкна() in " << РНЦП::Имя(this));
	HWND hwnd = дайОУК();
	if(hwnd) {
		LLOG("Ктрл::устФокусОкна() -> ::устФокус(" << (void *)hwnd << ")");
//		::SetActiveWindow(hwnd);
		::SetFocus(hwnd);
		return true;
	}
	LLOG("//Ктрл::устФокусОкна() in " <<РНЦП::Имя(this) << ", active window = " << (void *)::GetActiveWindow());
	return false;
}

bool Ктрл::естьФокусОкна() const
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	return hwnd && ::GetFocus() == hwnd;
}

bool Ктрл::SetWndCapture()
{
	ЗамкниГип __;
	LLOG("Ктрл::SetWndCapture() in " << РНЦП::Имя(this));
	ПРОВЕРЬ(главнаяНить_ли());
	HWND hwnd = дайОУК();
	if(hwnd) {
		::SetCapture(hwnd);
		LLOG("SetCapture succeeded");
		return true;
	}
	return false;
}

bool Ктрл::ReleaseWndCapture()
{
	ЗамкниГип __;
	LLOG("Ктрл::ReleaseWndCapture() in " << РНЦП::Имя(this));
	ПРОВЕРЬ(главнаяНить_ли());
	HWND hwnd = дайОУК();
	if(hwnd && HasWndCapture())
	{
		::ReleaseCapture();
		LLOG("ReleaseCapture succeeded");
		return true;
	}
	return false;
}

bool Ктрл::HasWndCapture() const
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
	return hwnd && hwnd == ::GetCapture();
}

void Ктрл::WndInvalidateRect(const Прям& r)
{
	ЗамкниГип __;
	LLOG("WndInvalidateRect " << РНЦП::Имя(this));
	HWND hwnd = дайОУК();
	if(hwnd)
		::InvalidateRect(hwnd, r, false);
}

void Ктрл::WndSetPos(const Прям& rect)
{
	ЗамкниГип __;
	LLOG("WndSetPos " << РНЦП::Имя(this) << " " << rect);
	HWND hwnd = дайОУК();
	if(hwnd) {
		Прям r = rect;
		AdjustWindowRectEx(r, ::GetWindowLong(hwnd, GWL_STYLE), FALSE,
		                   ::GetWindowLong(hwnd, GWL_EXSTYLE));
		SetWindowPos(hwnd, NULL, r.left, r.top, r.устШирину(), r.устВысоту(),
		             SWP_NOACTIVATE|SWP_NOZORDER);
		if(HasFocusDeep()) {
			caretCtrl = NULL;
			SyncCaret();
		}
	}
	fullrefresh = false;
}

void Ктрл::обновиОкно(const Прям& r)
{
	ЗамкниГип __;
	LLOG("обновиОкно " << РНЦП::Имя(this));
	Ктрл *top = дайТопКтрл();
	if(top->открыт()) {
		HWND hwnd = top->дайОУК();
		HDC hdc = GetDC(hwnd);
		HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
		if(GetUpdateRgn(hwnd, hrgn, FALSE) != NULLREGION) {
			SelectClipRgn(hdc, hrgn);
			SystemDraw draw(hdc);
			bool hcr = focusCtrl && focusCtrl->дайТопКтрл() == top &&
			           caretRect.пересекается(r + top->дайПрям().верхЛево());
			if(hcr) ::HideCaret(hwnd);
			draw.Clip(r);
			top->UpdateArea(draw, r);
			ValidateRect(hwnd, r);
			SelectClipRgn(hdc, NULL);
			if(hcr) ::ShowCaret(hwnd);
		}
		ReleaseDC(hwnd, hdc);
		DeleteObject(hrgn);
	}
}

void  Ктрл::WndScrollView(const Прям& r, int dx, int dy)
{
	ЗамкниГип __;
	LLOG("WndScrollView " << РНЦП::Имя(this));
	if(caretCtrl && caretCtrl->дайТопКтрл() == this) {
#if WINCARET
		разрушьКареткуОкна();
#else
		RefreshCaret();
#endif
		caretRect.очисть();
	}
#ifdef PLATFORM_WINCE
	::ScrollWindowEx(дайОУК(), dx, dy, r, r, NULL, NULL, 0);
#else
	::ScrollWindow(дайОУК(), dx, dy, r, r);
#endif
	SyncCaret();
}

void Ктрл::PopUpHWND(HWND owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	LLOG("PopUpHWND " << РНЦП::Имя(this) << ", owner: " << owner << ", activate: " << activate);
	popup = false;
	создай(owner, WS_POPUP, topmost ? WS_EX_TOPMOST : 0, savebits,
	       owner || !activate ? SW_SHOWNOACTIVATE : SW_SHOW,
	       dropshadow);
	HWND hwnd = дайОУК();
	if(hwnd) popup = true;
	if(activate) устФокус();
}

void Ктрл::PopUp(Ктрл *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	popup = false;
	Ктрл *q = owner ? owner->дайТопКтрл() : дайАктивныйКтрл();
	PopUpHWND(q ? q->дайОУК() : NULL, savebits, activate, dropshadow, topmost);
	if(top) top->owner = owner;
}

Прям Ктрл::GetScreenClient(HWND hwnd)
{
	Прям r;
	::GetClientRect(hwnd, r);
	Точка tl = r.верхЛево();
	Точка br = r.низПраво();
	::ClientToScreen(hwnd, tl);
	::ClientToScreen(hwnd, br);
	LLOG("Ктрл::GetScreenClient: hwnd = " << фмтУк(hwnd) << ", client = " << r
	     << ", screen(tl) = " << tl << ", screen(br) = " << br);
	return Прям(tl, br);
}

Прям Ктрл::GetDefaultWindowRect() {
	HWND hwnd = ::CreateWindow("РНЦП-CLASS-W", "", WS_OVERLAPPED,
		                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
							   NULL, NULL, NULL, NULL);
	Прям sr;
	if(hwnd) {
		::ShowWindow(hwnd, SW_HIDE);
		sr = GetScreenClient(hwnd);
		::DestroyWindow(hwnd);
	}
	else
		sr = RectC(20, 20, 500, 350);
	return sr;
}

TopFrameDraw::TopFrameDraw(Ктрл *ctrl, const Прям& r)
{
	войдиВСтопорГип();
	Ктрл *top = ctrl->дайТопКтрл();
	hwnd = top->дайОУК();
	ПРОВЕРЬ(hwnd);
	прикрепи(GetDC(hwnd));
	Clipoff(r);
}

TopFrameDraw::~TopFrameDraw()
{
	стоп();
	HDC hdc = открепи();
	if(hwnd && hdc)
		ReleaseDC(hwnd, hdc);
	покиньСтопорГип();
}

Вектор<ШТкст> SplitCmdLine__(const char *cmd)
{
	Вектор<ШТкст> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"') {
			ШТкст quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.конкат(изСисНабсима(Ткст(cmd, 1)).вШТкст());
			out.добавь(quoted);
		}
		else {
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.добавь(Ткст(begin, cmd).вШТкст());
		}
	return out;
}

}

#endif
