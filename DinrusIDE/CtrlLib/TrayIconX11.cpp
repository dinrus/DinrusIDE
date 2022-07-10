#include "CtrlLib.h"

#define LLOG(x) // LOG(x)

#ifdef GUI_X11

namespace РНЦП {

Atom TraySelection()
{
	return XAtom(фмт("_NET_SYSTEM_TRAY_S%d", Xscreenno));
}

void ИконкаТрея::Call(int code, unsigned long d1, unsigned long d2, unsigned long d3)
{
	if(!traywin)
		return;
	bool x11trap = TrapX11Errors();
	XСобытие e;
	обнули(e);
	e.xclient.data.l[0] = CurrentTime;
	e.xclient.data.l[1] = code;
	e.xclient.data.l[2] = d1;
	e.xclient.data.l[3] = d2;
	e.xclient.data.l[4] = d3;
	e.xclient.window = traywin;
	e.xclient.тип = ClientMessage;
	e.xclient.message_type = XAtom("_NET_SYSTEM_TRAY_OPCODE");
	e.xclient.формат = 32;
	XSendСобытие(Xdisplay, traywin, XFalse, 0, &e);
	XSync(Xdisplay, XFalse);
	UntrapX11Errors(x11trap);
}

void ИконкаТрея::AddToTray()
{
	скрой();
	traywin = XGetSelectionOwner(Xdisplay, TraySelection());
	if(!traywin) return;

	XSelectInput(Xdisplay, traywin, StructureNotifyMask);
	XWindowAttributes attr;
	XGetWindowAttributes(Xdisplay, Xroot, &attr);
	XSelectInput(Xdisplay, Xroot, attr.your_event_mask | StructureNotifyMask);

	Window win = GetWindow();
	XSizeHints *size_hints = XAllocSizeHints();
	size_hints->min_width = 22;
	size_hints->min_height = 22;
	size_hints->max_width = 22;
	size_hints->max_height = 22;
	size_hints->width = 22;
	size_hints->height = 22;
	size_hints->x = 0;
	size_hints->y = 0;
	size_hints->win_gravity = StaticGravity;
	size_hints->flags = PMinSize|PMaxSize|PPosition|PSize|PWinGravity;
	XSetWMNormalHints(Xdisplay, win, size_hints);

	Call(0, win, 0, 0);

	XSetWMNormalHints(Xdisplay, win, size_hints);

	покажи();
	XSetWMNormalHints(Xdisplay, win, size_hints);
	XFree(size_hints);
}

void ИконкаТрея::Message(int тип, const char *title, const char *text, int timeout)
{
	if(!traywin)
		return;
	int len = strlen(text);
	static int stamp;
	Call(1, 1000 * timeout, len, ++stamp);
	bool x11trap = TrapX11Errors();
	while(len > 0) {
		XClientMessageСобытие ev;
		обнули(ev);
		ev.тип = ClientMessage;
		ev.window = GetWindow();
		ev.формат = 8;
		ev.message_type = XAtom("_NET_SYSTEM_TRAY_MESSAGE_DATA");
		int n = min(len, 20);
		memcpy(&ev.data, text, n);
		text += n;
		len -= n;
		XSendСобытие(Xdisplay, traywin, XFalse, StructureNotifyMask, (XСобытие *) &ev);
		XSync(Xdisplay, XFalse);
	}
	UntrapX11Errors(x11trap);
}

bool ИконкаТрея::HookProc(XСобытие *event)
{
	XAnyСобытие *e = (XAnyСобытие *)event;
	if(e->тип == DestroyNotify && e->window == traywin) {
		AddToTray();
		return true;
	}
	if(e->тип == ClientMessage && !traywin) {
		XClientMessageСобытие *cm = (XClientMessageСобытие *)event;
		if(cm->message_type == XAtom("MANAGER") && (Atom)cm->data.l[1] == TraySelection()) {
			AddToTray();
			return true;
		}
    }
	return false;
}

void ИконкаТрея::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Размер isz = icon.дайРазм();
	Рисунок m = icon;
	if(isz.cx > sz.cx || isz.cy > sz.cy) {
		m = Rescale(icon, sz);
		isz = sz;
	}
	Точка p = Прям(sz).позЦентра(isz);
	w.DrawRect(sz, SColorFace());
	w.DrawImage(p.x, p.y, m);
}

void ИконкаТрея::леваяВнизу()
{
	WhenLeftDown();
}

void ИконкаТрея::леваяВнизу(Точка p, dword keyflags)
{
	леваяВнизу();
}

void ИконкаТрея::леваяВверху()
{
	WhenLeftUp();
}

void ИконкаТрея::леваяВверху(Точка p, dword keyflags)
{
	леваяВверху();
}

void ИконкаТрея::леваяДКлик()
{
	WhenLeftDouble();
}

void ИконкаТрея::леваяДКлик(Точка p, dword keyflags)
{
	леваяДКлик();
}

void ИконкаТрея::Menu(Бар& bar)
{
	WhenBar(bar);
}

void ИконкаТрея::DoMenu(Бар& bar)
{
	Menu(bar);
}

void ИконкаТрея::праваяВнизу(Точка p, dword keyflags)
{
	БарМеню::выполни(this, THISBACK(DoMenu), дайПозМыши());
}

void ИконкаТрея::покажи(bool b)
{
/*	if(!!b == !!открыт()) return; // Needs fixing....
	if(b) {
	}
	else
		закрой();*/
}

ИконкаТрея::ИконкаТрея()
{
	устПрям(0, 0, 24, 24);
	ignoretakefocus = true;
	создай(NULL, true, false);
	if(top) {
		popup = true;
		Transparent();
		AddToTray();
	}
}

// Not implemented in X11 yet...
void ИконкаТрея::BalloonLeftDown()
{
}

void ИконкаТрея::BalloonShow()
{
}

void ИконкаТрея::BalloonHide()
{
}

void ИконкаТрея::BalloonTimeout()
{
}

}

#endif
