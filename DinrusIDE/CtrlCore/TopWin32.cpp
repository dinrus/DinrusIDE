#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

#define LLOG(x)  // DLOG(x)

#if defined(COMPILER_MINGW) && !defined(FLASHW_ALL)
	// MINGW headers don't include this in (some versions of) windows
	extern "C"{
		struct FLASHWINFO {
			UINT  cbSize;
			HWND  hwnd;
			DWORD dwFlags;
			UINT  uCount;
			DWORD dwTimeout;
		};
		WINUSERAPI BOOL WINAPI FlashWindowEx(FLASHWINFO*);
	}
	#define FLASHW_STOP         0
	#define FLASHW_CAPTION      0x00000001
	#define FLASHW_TRAY         0x00000002
	#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
	#define FLASHW_TIMER        0x00000004
	#define FLASHW_TIMERNOFG    0x0000000C
#endif

void    ТопОкно::SyncSizeHints() {}

LRESULT ТопОкно::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	ЗамкниГип __;
	HWND hwnd = дайОУК();
#ifndef PLATFORM_WINCE
	bool inloop;
#endif
	switch(message) {
	case WM_QUERYENDSESSION:
		inloop = InLoop();
		WhenClose();
		return inloop ? !InLoop() : !открыт();
	case WM_CLOSE:
		if(включен_ли()) {
			IgnoreMouseUp();
			WhenClose();
		}
		return 0;
	case WM_DESTROY:
		if(overlapped.дайШирину() && overlapped.дайВысоту())
			устПрям(overlapped);
		break;
	case WM_WINDOWPOSCHANGED:
		if(!isopen)
			break;
		if(IsIconic(hwnd))
			state = MINIMIZED;
		else
		if(IsZoomed(hwnd))
			state = MAXIMIZED;
		else {
			state = OVERLAPPED;
			if(IsWindowVisible(hwnd))
				overlapped = GetScreenClient(hwnd);
		}
		LLOG("ТопОкно::WindowProc::WM_WINDOWPOSCHANGED: overlapped = " << overlapped);
		Выкладка();
		break;
	}
	return Ктрл::WindowProc(message, wParam, lParam);
}

void ТопОкно::SyncTitle()
{
	ЗамкниГип __;
	LLOG("ТопОкно::SyncTitle0 " << РНЦП::Имя(this));
	HWND hwnd = дайОУК();
	if(hwnd)
		::SetWindowTextW(hwnd, вСисНабсимШ(title));
}

void ТопОкно::удалиИконку()
{
	ЗамкниГип __;
	LLOG("ТопОкно::удалиИконку " << РНЦП::Имя(this));
	if(ico)
		DestroyIcon(ico);
	if(lico)
		DestroyIcon(lico);
	ico = lico = NULL;
}

Ткст стильОкнаСтрокой(dword style, dword exstyle)
{
	Ткст r1;
#define ДЕЛАЙ(x) if(style & x) { if(r1.дайСчёт()) r1 << "|"; r1 << #x; }
	ДЕЛАЙ(WS_OVERLAPPED)
	ДЕЛАЙ(WS_POPUP)
	ДЕЛАЙ(WS_CHILD)
	ДЕЛАЙ(WS_MINIMIZE)
	ДЕЛАЙ(WS_VISIBLE)
	ДЕЛАЙ(WS_DISABLED)
	ДЕЛАЙ(WS_CLIPSIBLINGS)
	ДЕЛАЙ(WS_CLIPCHILDREN)
	ДЕЛАЙ(WS_MAXIMIZE)
	ДЕЛАЙ(WS_CAPTION)
	ДЕЛАЙ(WS_BORDER)
	ДЕЛАЙ(WS_DLGFRAME)
	ДЕЛАЙ(WS_VSCROLL)
	ДЕЛАЙ(WS_HSCROLL)
	ДЕЛАЙ(WS_SYSMENU)
	ДЕЛАЙ(WS_THICKFRAME)
	ДЕЛАЙ(WS_GROUP)
	ДЕЛАЙ(WS_TABSTOP)
	ДЕЛАЙ(WS_MINIMIZEBOX)
	ДЕЛАЙ(WS_MAXIMIZEBOX)
#undef ДЕЛАЙ

	Ткст r2;
#define ДЕЛАЙ(x) if(exstyle & x) { if(r2.дайСчёт()) r2 << "|"; r2 << #x; }
	ДЕЛАЙ(WS_EX_DLGMODALFRAME)
	ДЕЛАЙ(WS_EX_NOPARENTNOTIFY)
	ДЕЛАЙ(WS_EX_TOPMOST)
	ДЕЛАЙ(WS_EX_ACCEPTFILES)
	ДЕЛАЙ(WS_EX_TRANSPARENT)
	ДЕЛАЙ(WS_EX_MDICHILD)
	ДЕЛАЙ(WS_EX_TOOLWINDOW)
	ДЕЛАЙ(WS_EX_WINDOWEDGE)
	ДЕЛАЙ(WS_EX_CLIENTEDGE)
	ДЕЛАЙ(WS_EX_CONTEXTHELP)
	ДЕЛАЙ(WS_EX_RIGHT)
	ДЕЛАЙ(WS_EX_LEFT)
	ДЕЛАЙ(WS_EX_RTLREADING)
	ДЕЛАЙ(WS_EX_LTRREADING)
	ДЕЛАЙ(WS_EX_LEFTSCROLLBAR)
	ДЕЛАЙ(WS_EX_RIGHTSCROLLBAR)
	ДЕЛАЙ(WS_EX_CONTROLPARENT)
	ДЕЛАЙ(WS_EX_STATICEDGE)
	ДЕЛАЙ(WS_EX_APPWINDOW)
#undef ДЕЛАЙ

	return r1 + ' ' + r2;
}

void ТопОкно::SyncCaption()
{
	ЗамкниГип __;
	LLOG("ТопОкно::SyncCaption " << РНЦП::Имя(this));
	if(fullscreen)
		return;
	HWND hwnd = дайОУК();
	if(hwnd) {
		style = ::GetWindowLong(hwnd, GWL_STYLE);
		exstyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
	}
	else style = exstyle = 0;
	style &= ~(WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU|WS_POPUP|WS_DLGFRAME);
	exstyle &= ~(WS_EX_TOOLWINDOW|WS_EX_DLGMODALFRAME);
	style |= WS_CAPTION|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	if(minimizebox && !дайВладельца())
		style |= WS_MINIMIZEBOX;
	if(maximizebox)
		style |= WS_MAXIMIZEBOX;
	if(sizeable)
		style |= WS_THICKFRAME;
	if(frameless)
		style = (style & ~WS_CAPTION) | WS_POPUP;
	else
	if(!maximizebox && !minimizebox || noclosebox) { // icon ignored because of FixIcons
		style |= WS_POPUPWINDOW|WS_DLGFRAME;
		exstyle |= WS_EX_DLGMODALFRAME;
		if(noclosebox)
			style &= ~WS_SYSMENU;
	}
	else
		style |= WS_SYSMENU;
	if(tool)
		exstyle |= WS_EX_TOOLWINDOW;
	if(fullscreen)
		style = WS_POPUP;
	if(hwnd) {
		::SetWindowLong(hwnd, GWL_STYLE, style);
		::SetWindowLong(hwnd, GWL_EXSTYLE, exstyle);
		SyncTitle();
		if(urgent) {
			if(пп_ли()) urgent = false;
			FLASHWINFO fi;
			memset(&fi, 0, sizeof(fi));
			fi.cbSize = sizeof(fi);
			fi.hwnd = hwnd;
			fi.dwFlags = urgent ? FLASHW_TIMER|FLASHW_ALL : FLASHW_STOP;
			FlashWindowEx(&fi);
		}
	}
	удалиИконку();

	if(hwnd) {
		::SendMessage(hwnd, WM_SETICON, false, (LPARAM)(ico = SystemDraw::IconWin32(icon)));
		::SendMessage(hwnd, WM_SETICON, true, (LPARAM)(lico = SystemDraw::IconWin32(largeicon)));
	}
}

void ТопОкно::центрПрям(HWND hwnd, int center)
{
	ЗамкниГип __;
	SetupRect(ктрлИзОУК(hwnd));
	if(hwnd && center == 1 || center == 2) {
		Размер sz = дайПрям().размер();
		Прям frmrc(sz);
	#ifndef PLATFORM_WINCE
		::AdjustWindowRect(frmrc, WS_OVERLAPPEDWINDOW, FALSE);
	#endif
		Прям r, wr;
		wr = Ктрл::GetWorkArea().дефлят(-frmrc.left, -frmrc.top,
			frmrc.right - sz.cx, frmrc.bottom - sz.cy);
		sz.cx = min(sz.cx, wr.устШирину());
		sz.cy = min(sz.cy, wr.устВысоту());
		if(center == 1) {
			::GetClientRect(hwnd, r);
			if(r.пустой())
				r = wr;
			else {
				Точка p = r.верхЛево();
				::ClientToScreen(hwnd, p);
				r.смещение(p);
			}
		}
		else
			r = wr;
		Точка p = r.позЦентра(sz);
		if(p.x + sz.cx > wr.right) p.x = wr.right - sz.cx;
		if(p.y + sz.cy > wr.bottom) p.y = wr.bottom - sz.cy;
		if(p.x < wr.left) p.x = wr.left;
		if(p.y < wr.top) p.y = wr.top;
		устПрям(p.x, p.y, sz.cx, sz.cy);
	}
}

static HWND trayHWND__;
HWND   GetTrayHWND__()          { return trayHWND__; }
void   SetTrayHWND__(HWND hwnd) { trayHWND__ = hwnd; }

void ТопОкно::открой(HWND hwnd)
{
	ЗамкниГип __;
	if(dokeys && (!GUI_AKD_Conservative() || GetAccessKeysDeep() <= 1))
		DistributeAccessKeys();
	USRLOG("   OPEN " << Desc(this));
	LLOG("ТопОкно::открой, owner HWND = " << hwnd << ", Active = " << hwnd);
	IgnoreMouseUp();
	SyncCaption();
	LLOG("WindowStyles: " << стильОкнаСтрокой(style, exstyle));
#ifdef PLATFORM_WINCE
	if(!дайПрям().пустой())
#endif
	if(fullscreen) {
		устПрям(дайПрямЭкрана()); // 12-05-23 Tom changed from GetScreenSize() to дайПрямЭкрана() in order to get full screen on correct дисплей
		создай(hwnd, WS_POPUP, 0, false, SW_SHOWMAXIMIZED, false);
	}
	else {
		центрПрям(hwnd, hwnd && hwnd == GetTrayHWND__() ? center ? 2 : 0 : center);
		создай(hwnd, style, exstyle, false, state == OVERLAPPED ? SW_SHOWNORMAL :
		                                    state == MINIMIZED  ? SW_MINIMIZE :
		                                                          SW_MAXIMIZE, false);
	}
	PlaceFocus();
	SyncCaption();
	FixIcons();
}

void ТопОкно::открой(Ктрл *owner)
{
	ЗамкниГип __;
	LLOG("ТопОкно::открой(Ктрл) -> " << РНЦП::Имя(owner));
	открой(owner ? owner->дайТопКтрл()->дайОУК() : NULL);
	if(открыт() && top)
		top->owner = owner;
}

void ТопОкно::открой()
{
	открой(::GetActiveWindow()); // :: needed because of ActiveX controls (to create modal dlgs owned by a HWND)
}

void ТопОкно::откройГлавн()
{
	открой((HWND) NULL);
}

void ТопОкно::сверни(bool effect)
{
	LLOG("ТопОкно::сверни " << РНЦП::Имя(this));
	state = MINIMIZED;
	if(открыт())
#ifdef PLATFORM_WINCE
		::ShowWindow(дайОУК(), SW_MINIMIZE);
#else
		::ShowWindow(дайОУК(), effect ? SW_MINIMIZE : SW_SHOWMINIMIZED);
#endif
}

ТопОкно& ТопОкно::полноэкранно(bool b)
{
	LLOG("ТопОкно::полноэкранно " << РНЦП::Имя(this));
	fullscreen = b;
	HWND hwnd = дайОУКВладельца();
	bool pinloop = inloop;
	bool open = открыт();
	разрушьОкно();
	нахлёст();
	Прям r = дайПрям();
	if(open)
		устПрям(overlapped); // restore to the same screen as before
	else
	if(r.left + r.top == 0)
		устПрям(GetDefaultWindowRect()); // open in primary screen
	открой(hwnd);
	inloop = pinloop;
	return *this;
}

void ТопОкно::разверни(bool effect)
{
	LLOG("ТопОкно::разверни " << РНЦП::Имя(this));
	state = MAXIMIZED;
	if(открыт()) {
		::ShowWindow(дайОУК(), effect ? SW_MAXIMIZE : SW_SHOWMAXIMIZED);
		SyncCaption();
	}
}

void ТопОкно::нахлёст(bool effect)
{
	ЗамкниГип __;
	LLOG("ТопОкно::нахлёст " << РНЦП::Имя(this));
	state = OVERLAPPED;
	if(открыт()) {
		::ShowWindow(дайОУК(), effect ? SW_SHOWNORMAL : SW_RESTORE);
		SyncCaption();
	}
}

ТопОкно& ТопОкно::Стиль(dword _style)
{
	ЗамкниГип __;
	LLOG("ТопОкно::Стиль " << РНЦП::Имя(this));
	style = _style;
	if(дайОУК())
		::SetWindowLong(дайОУК(), GWL_STYLE, style);
	SyncCaption();
	return *this;
}

ТопОкно& ТопОкно::эксСтиль(dword _exstyle)
{
	ЗамкниГип __;
	LLOG("ТопОкно::эксСтиль " << РНЦП::Имя(this));
	exstyle = _exstyle;
	if(дайОУК())
		::SetWindowLong(дайОУК(), GWL_EXSTYLE, exstyle);
	SyncCaption();
	return *this;
}

ТопОкно& ТопОкно::наиверхнее(bool b, bool stay_top)
{
	ЗамкниГип __;
	LLOG("ТопОкно::наиверхнее " << РНЦП::Имя(this));
	HWND hwnd = дайОУК();
	if(hwnd)
		SetWindowPos(hwnd, b ? HWND_TOPMOST : (stay_top ? HWND_NOTOPMOST : HWND_BOTTOM),
		             0,0,0,0,SWP_NOMOVE|SWP_NOSIZE );
	return эксСтиль(b ? GetExStyle() | WS_EX_TOPMOST : GetExStyle() & ~WS_EX_TOPMOST);
}

bool ТопОкно::наиверхнее_ли() const
{
	return GetExStyle() & WS_EX_TOPMOST;
}

void ТопОкно::GuiPlatformConstruct()
{
	style = 0;
	exstyle = 0;
	ico = lico = NULL;
}

void ТопОкно::GuiPlatformDestruct()
{
	удалиИконку();
}

void ТопОкно::SerializePlacement(Поток& s, bool reminimize)
{
	ЗамкниГип __;
	int version = 1;
	s / version;
	Прям rect = дайПрям();
	s % overlapped % rect;
	bool mn = state == MINIMIZED;
	bool mx = state == MAXIMIZED;
	bool fs = fullscreen;
	if(version >= 1)
		s.Pack(mn, mx, fs);
	else
		s.Pack(mn, mx);
	LLOG(Имя(this) << "::SerializePlacement / " << (s.сохраняется() ? "write" : "read"));
	LLOG("minimized = " << mn << ", maximized = " << mx << ", fullscreen = " << fs);
	LLOG("rect = " << rect << ", overlapped = " << overlapped);
	if(s.грузится()) {
		rect = overlapped;
		Прям limit = GetVirtualWorkArea();
		Прям outer = rect;
		::AdjustWindowRect(outer, WS_OVERLAPPEDWINDOW, FALSE);
		limit.left   += rect.left   - outer.left;
		limit.top    += rect.top    - outer.top;
		limit.right  += rect.right  - outer.right;
		limit.bottom += rect.bottom - outer.bottom;
		Размер sz = min(rect.размер(), limit.размер());
		rect = RectC(
			minmax(rect.left, limit.left, limit.right - sz.cx),
			minmax(rect.top,  limit.top,  limit.bottom - sz.cy),
			sz.cx, sz.cy);

		нахлёст();
		устПрям(rect);
		
		if(mn && reminimize)
			state = MINIMIZED;
		if(mx)
			state = MAXIMIZED;
		if(min(sz.cx, sz.cy) < 50 && mn && !reminimize)
			state = MAXIMIZED; // Minimized tends to have invalid size, somewhat ugly patch here
		if(открыт()) {
			switch(state) {
			case MINIMIZED:
				сверни();
				break;
			case MAXIMIZED:
				разверни();
				break;
			}
			if(fs) {
				нахлёст(); // Needed to restore normal position before fullscreen mode
				полноэкранно();
			}
		}
		else
			fullscreen = fs;
	}
}

}

#endif
