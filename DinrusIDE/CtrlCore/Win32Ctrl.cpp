#include "CtrlCore.h"

#ifdef GUI_WIN

#define LLOG(x)  // DLOG(x)

namespace РНЦП {

void Ктрл::GuiPlatformConstruct()
{
	activex = false;
	isdhctrl = false;
}

void Ктрл::GuiPlatformDestruct()
{
}

void Ктрл::GuiPlatformRemove()
{
}

void Ктрл::GuiPlatformGetTopRect(Прям& r) const
{
	if(activex)
		r = GetWndScreenRect();
}

bool Ктрл::GuiPlatformRefreshFrameSpecial(const Прям& r)
{
	if(isdhctrl) {
		InvalidateRect(((DHCtrl *)this)->дайОУК(), r, false);
		return true;
	}
	return false;
}

bool Ктрл::GuiPlatformSetFullRefreshSpecial()
{
	return isdhctrl;
}

void Ктрл::GuiPlatformSelection(PasteClip&)
{
}

void GuiPlatformAdjustDragImage(ImageBuffer&)
{
}

bool GuiPlatformHasSizeGrip()
{
	return true;
}

void GuiPlatformGripResize(ТопОкно *q)
{
	LLOG("GuiPlatformGripResize " << Имя(q));
	HWND hwnd = q->дайОУК();
	Точка p = дайПозМыши() - q->дайПрям().верхЛево();
	if(hwnd) {
		::SendMessage(hwnd, WM_SYSCOMMAND, 0xf008, MAKELONG(p.x, p.y));
		::SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELONG(p.x, p.y));
	}
}

Цвет GuiPlatformGetScreenPixel(int x, int y)
{
	HDC sdc = GetWindowDC(0);
	Цвет c = Цвет::FromCR(GetPixel(sdc, x, y));
	ReleaseDC(0, sdc);
	return c;
}

void GuiPlatformAfterMenuPopUp()
{
}

#if WINCARET
void Ктрл::устКаретку(int x, int y, int cx, int cy)
{
	ЗамкниГип __;
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	SyncCaret();
}

void Ктрл::SyncCaret() {
	ЗамкниГип __;
	Прям cr;
	cr.очисть();
	if(focusCtrl && focusCtrl->видим_ли()) {
		bool inframe = focusCtrl->InFrame();
		cr = focusCtrl->GetScreenView();
		cr = RectC(focusCtrl->caretx + cr.left, focusCtrl->carety + cr.top,
				   focusCtrl->caretcx, focusCtrl->caretcy) & cr;
		for(Ктрл *q = focusCtrl->дайРодителя(); q; q = q->дайРодителя()) {
			cr &= inframe ? q->дайПрямЭкрана() : q->GetScreenView();
			inframe = q->InFrame();
		}
	}
	if(focusCtrl != caretCtrl || cr != caretRect) {
		LLOG("Do SyncCaret focusCtrl: " << РНЦП::Имя(focusCtrl)
		     << ", caretCtrl: " << РНЦП::Имя(caretCtrl)
		     << ", cr: " << cr);
		разрушьКареткуОкна();
		if(focusCtrl && !cr.пустой())
			focusCtrl->дайТопКтрл()->создайКареткуОкна(cr);
		caretCtrl = focusCtrl;
		caretRect = cr;
	}
}
#endif

Ткст Ктрл::Имя() const {
	ЗамкниГип __;
	Ткст s = имя0();
	if(!отпрыск_ли())
		s << фмт(" (hwnd 0x%x)", (int)(intptr_t) дайОУК());
	return s;
}

}

#endif
