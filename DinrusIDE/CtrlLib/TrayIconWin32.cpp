#include "CtrlLib.h"

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#include <cderr.h>
#include <shellapi.h>

#endif
#endif

namespace РНЦП {


#ifdef GUI_WIN

#ifndef PLATFORM_WINCE
#define LLOG(x)

enum {
	UM_TASKBAR_ = WM_USER + 1024,
	NIN_BALLOONSHOW_ = WM_USER + 2,
	NIN_BALLOONHIDE_ = WM_USER + 3,
	NIN_BALLOONTIMEOUT_ = WM_USER + 4,
	NIN_BALLOONUSERCLICK_ = WM_USER + 5,
};

ИконкаТрея::ИконкаТрея()
{
	создай(NULL, WS_POPUP, 0, false, 0, 0);
	Ктрл::скрой();
	обнули(nid);
	nid.cbSize = sizeof(nid);
	nid.uCallbackMessage = UM_TASKBAR_;
	nid.hWnd = дайОУК();
	static int id;
	nid.uID = ++id;
	visible = false;
	покажи();
}

ИконкаТрея::~ИконкаТрея()
{
	скрой();
	if(nid.hIcon)
		DestroyIcon(nid.hIcon);
}

void Wcpy(char16 *t, const Ткст& s, int sz)
{
	Вектор<char16> w = вСисНабсимШ(s);
	if(w.дайСчёт() > sz) {
		w.устСчёт(sz - 1);
		w.добавь(0);
	}
	memcpy(t, w, w.дайСчёт() * sizeof(char16));
}

void ИконкаТрея::Notify(dword msg)
{
	if(visible) {
		nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
		if(nid.hIcon)
			DestroyIcon(nid.hIcon);
		nid.hIcon = SystemDraw::IconWin32(icon);
		Wcpy(nid.szTip, tip, 128);
		BOOL Статус = Shell_NotifyIconW(msg, &nid);
		// To prevent from Shell_NotifyIcon bugs...
		// discussed here : http://msdn.microsoft.com/en-us/library/bb762159(v=vs.85).aspx
		// and here : http://issuetracker.delphi-jedi.org/bug_view_advanced_page.php?bug_id=3747
		if (Статус == FALSE) {
			// The status of Shell_NotifyIcon is FALSE, in the case of NIM_ADD/NIM_MODIFY, we will try to Modify
			// If the modify is OK then we cas consider that the add was worked.
			// Same, case with delete, we can try modify and if KO then we can consider that is icon
			// was deleted correctly. In other cases, we will retry after 50ms
			DWORD ErrorCode = GetLastError();
			if(ErrorCode == ERROR_SUCCESS || ErrorCode == ERROR_TIMEOUT) {
				for(int retry = 0; retry < 60; retry++) {
					Sleep(50);
					if(Shell_NotifyIconW(NIM_MODIFY, &nid) == (msg != NIM_DELETE))
						break;
				}
			}
		}
    }
}

void ИконкаТрея::Message(int тип, const char *title, const char *text, int timeout)
{
	nid.uFlags = 0x10;
	Wcpy(nid.szInfo, text, 256);
	Wcpy(nid.szInfoTitle, text, 64);
	nid.dwInfoFlags = тип;
	nid.uTimeout = minmax(timeout, 10, 30) * 1000;
	Shell_NotifyIconW(NIM_MODIFY, &nid);
}

void ИконкаТрея::покажи(bool b)
{
	if(b == visible)
		return;
	if(visible)
		Notify(NIM_DELETE);
	visible = b;
	if(visible)
		Notify(NIM_ADD);
}

ИконкаТрея& ИконкаТрея::Иконка(const Рисунок &img)
{
	icon = img;
	Notify(NIM_MODIFY);
	return *this;
}

ИконкаТрея& ИконкаТрея::Подсказка(const char *text)
{
	tip = text;
	Notify(NIM_MODIFY);
	return *this;
}

void ИконкаТрея::Menu(Бар& bar)
{
	WhenBar(bar);
}

void ИконкаТрея::леваяВнизу()
{
	WhenLeftDown();
}

void ИконкаТрея::леваяВверху()
{
	WhenLeftUp();
}

void ИконкаТрея::леваяДКлик()
{
	WhenLeftDouble();
}

void ИконкаТрея::DoMenu(Бар& bar)
{
	Menu(bar);
}

void ИконкаТрея::BalloonLeftDown()
{
	WhenBalloonLeftDown();
}

void ИконкаТрея::BalloonShow()
{
	WhenBalloonShow();
}

void ИконкаТрея::BalloonHide()
{
	WhenBalloonHide();
}

void ИконкаТрея::BalloonTimeout()
{
	WhenBalloonTimeout();
}

LRESULT ИконкаТрея::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_QUERYENDSESSION) {
		Shutdown();
		return true;
	}
	if(message == UM_TASKBAR_)
		switch(lParam) {
		case WM_LBUTTONDOWN:
			леваяВнизу();
			return TRUE;
		case WM_LBUTTONUP:
			леваяВверху();
			return TRUE;
		case WM_LBUTTONDBLCLK:
			::SetForegroundWindow(nid.hWnd);
			леваяДКлик();
			return TRUE;
		case WM_RBUTTONDOWN:
			::SetForegroundWindow(nid.hWnd);
			БарМеню::выполни(NULL, THISBACK(DoMenu), дайПозМыши());
			return TRUE;
		case NIN_BALLOONSHOW_:
			BalloonShow();
			return TRUE;
		case NIN_BALLOONHIDE_:
			BalloonHide();
			return TRUE;
		case NIN_BALLOONTIMEOUT_:
			BalloonTimeout();
			return TRUE;
		case NIN_BALLOONUSERCLICK_:
			BalloonLeftDown();
			return TRUE;
		}
	static UINT WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");
	if(message == WM_TASKBARCREATED) {
		LLOG("ИконкаТрея::WM_TASKBARCREATED");
		visible = false;
		покажи();
	}
	return Ктрл::WindowProc(message, wParam, lParam);
}

#endif
#endif

}
