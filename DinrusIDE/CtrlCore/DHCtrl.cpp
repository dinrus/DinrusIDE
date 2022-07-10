#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

#ifndef PLATFORM_WINCE

Вектор<DHCtrl *> DHCtrl::all_active;

void DHCtrl::RemoveActive()
{
	for(;;) {
		int q = найдиИндекс(all_active, this);
		if(q < 0)
			return;
		all_active.удали(q);
	}
}

bool DHCtrl::PreprocessMessageAll(MSG& msg)
{
	for(auto q : all_active)
		if(q->предобработайСобытие(msg))
			return true;
	return false;
}

bool DHCtrl::предобработайСобытие(MSG& msg)
{
	return false;
}

void DHCtrl::NcCreate(HWND _hwnd)
{
	hwnd = _hwnd;
	all_active.добавь(this);
}

void DHCtrl::NcDestroy()
{
	hwnd = NULL;
	RemoveActive();
}

LRESULT DHCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	ЗамкниГип __;
	if(message == WM_ERASEBKGND)
		return 1L;
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void DHCtrl::CloseHWND()
{
	ЗамкниГип __;
	if(hwnd) {
		DestroyWindow(hwnd);
		hwnd = NULL;
		RemoveActive();
	}
}

void DHCtrl::OpenHWND()
{
	ЗамкниГип __;
	CloseHWND();
	HWND phwnd = дайТопКтрл()->дайОУК();
	if(phwnd)
		CreateWindowEx(0, "РНЦП-CLASS-A", "",
		               WS_CHILD|WS_DISABLED|WS_VISIBLE,
		               0, 0, 20, 20,
		               phwnd, NULL, hInstance, this);
}

void DHCtrl::SyncHWND()
{
	ЗамкниГип __;
	HWND phwnd = дайТопКтрл()->дайОУК();
	if(phwnd) {
		Прям r = GetScreenView() - GetScreenClient(phwnd).верхЛево();
		if(r != current_pos || видим_ли() != (bool)current_visible) {
			SetWindowPos(hwnd, NULL, r.left, r.top, r.устШирину(), r.устВысоту(), SWP_NOACTIVATE|SWP_NOZORDER);
			ShowWindow(hwnd, видим_ли() ? SW_SHOW : SW_HIDE);
			освежи();
			current_pos = r;
			current_visible = видим_ли();
		}
	}
}

void DHCtrl::State(int reason)
{
	switch(reason) {
	case OPEN:
		current_pos = Null;
		current_visible = Null;
		OpenHWND();
	default:
		SyncHWND();
		break;
	case CLOSE:
		CloseHWND();
	}
}

DHCtrl::DHCtrl()
{
	hwnd = NULL;
	isdhctrl = true;
	BackPaint(EXCLUDEPAINT);
}

DHCtrl::~DHCtrl()
{
	CloseHWND();
	BackPaint(EXCLUDEPAINT);
	RemoveActive();
}

void Ктрл::UpdateDHCtrls()
{ // we call this in WM_PAINT to force updating in single WM_PAINT, this makes things smoother e.g. with OpenGL in splitter
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ()) {
		DHCtrl *dh = dynamic_cast<DHCtrl *>(q);
		if(dh)
			UpdateWindow(dh->дайОУК());
		q->UpdateDHCtrls();
	}
}


#endif

}

#endif
