#include "CtrlCore.h"

#ifdef GUI_X11

namespace РНЦП {

void Ктрл::GuiPlatformConstruct()
{
}

void Ктрл::GuiPlatformDestruct()
{
}

void Ктрл::GuiPlatformRemove()
{
	if(popupgrab) {
		EndPopupGrab();
		popupgrab = false;
	}
}

void Ктрл::GuiPlatformGetTopRect(Прям& r) const
{
}

Ткст Ктрл::Имя() const {
	return имя0();
}

bool Ктрл::GuiPlatformRefreshFrameSpecial(const Прям&)
{
	return false;
}


bool Ктрл::GuiPlatformSetFullRefreshSpecial()
{
	return false;
}

void Ктрл::GuiPlatformSelection(PasteClip& d)
{
	d.fmt.очисть();
	d.тип = 2;
}

void GuiPlatformAdjustDragImage(ImageBuffer& b)
{
	if(Ктрл::IsCompositedGui()) {
		Рисунок h = Rescale(b, 64, 64);
		b = h;
	}
}

bool GuiPlatformHasSizeGrip()
{
	return _NET_Supported().найди(XAtom("_NET_WM_MOVERESIZE")) >= 0;
}

void GuiPlatformGripResize(ТопОкно *q)
{
	if(_NET_Supported().найди(XAtom("_NET_WM_MOVERESIZE")) >= 0) {
		XUngrabPointer(Xdisplay, CurrentTime); // 2008-02-25 cxl/mdelfe... compiz fix... who has grabbed it anyway?...
		XClientMessageСобытие m;
		m.тип = ClientMessage;
		m.serial = 0;
		m.send_event = XTrue;
		m.window = q->GetWindow();
		m.message_type = XAtom("_NET_WM_MOVERESIZE");
		m.формат = 32;
		Точка p = дайПозМыши();
		m.data.l[0] = p.x;
		m.data.l[1] = p.y;
		m.data.l[2] = 4;
		m.data.l[3] = 0;
		m.data.l[4] = 0;
		XSendСобытие(Xdisplay, Xroot, 0, SubstructureNotifyMask|SubstructureRedirectMask,
		           (XСобытие*)&m);
	}
}

Цвет GuiPlatformGetScreenPixel(int x, int y)
{
	// TODO
	return чёрный;
}

void GuiPlatformAfterMenuPopUp()
{
	XSync(Xdisplay, false);
	Ктрл::обработайСобытия();
}

void Ктрл::PaintCaret(SystemDraw& w)
{
	ЗамкниГип __;
	if(this == caretCtrl && WndCaretVisible)
		w.DrawRect(caretx, carety, caretcx, caretcy, InvertColor);
}

void Ктрл::устКаретку(int x, int y, int cx, int cy)
{
	ЗамкниГип __;
	if(this == caretCtrl)
		RefreshCaret();
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	WndCaretTime = msecs();
	if(this == caretCtrl)
		RefreshCaret();
}

void Ктрл::SyncCaret() {
	ЗамкниГип __;
	if(focusCtrl != caretCtrl) {
		RefreshCaret();
		caretCtrl = focusCtrl;
		RefreshCaret();
	}
}

}

#endif
