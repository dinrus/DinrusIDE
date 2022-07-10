#include "CtrlCore.h"

#ifdef GUI_X11

namespace РНЦП {

#define LLOG(x)  //  DLOG(x)

Прям ТопОкно::windowFrameMargin;

void ТопОкно::SyncSizeHints()
{
	ЗамкниГип __;
	Размер min = дайМинРазм();
	Размер max = дайМаксРазм();
	if(!sizeable)
		min = max = дайПрям().размер();
	Window w = GetWindow();
	if(w && (min != xminsize || max != xmaxsize) && !frameless) {
		xminsize = min;
		xmaxsize = max;
		size_hints->min_width = min.cx;
		size_hints->min_height = min.cy;
		size_hints->max_width = max.cx;
		size_hints->max_height = max.cy;
		size_hints->flags = PMinSize|PMaxSize;
		XSetWMNormalHints(Xdisplay, w, size_hints);
	}
}

void ТопОкно::EndIgnoreTakeFocus()
{
	ЗамкниГип __;
	ignoretakefocus = false;
}

void ТопОкно::СобытиеProc(XWindow& w, XСобытие *event)
{
	ЗамкниГип __;
	Ук<Ктрл> this_ = this;
	if(event->тип == ClientMessage) {
		if(event->xclient.формат == 32 && event->xclient.message_type)
			if(event->xclient.message_type == XAtom("WM_PROTOCOLS")) {
				Atom a = event->xclient.data.l[0];
				if(a == XAtom("WM_DELETE_WINDOW") && включен_ли()) {
					LLOG("DELETE_WINDOW " << Имя());
					WhenClose();
					return;
				}
				if(a == XAtom("WM_TAKE_FOCUS")) {
					LLOG("TAKE_FOCUS serial: " << event->xclient.serial);
					Xeventtime = event->xclient.data.l[1];
					TakeFocus();
					return;
				}
				if(a == XAtom("_NET_WM_PING")) {
					XСобытие ev = *event;
					ev.xclient.window = Xroot;
					XSendСобытие(Xdisplay, Xroot, 0, SubstructureRedirectMask|SubstructureNotifyMask, &ev);
					return;
				}
				LLOG("Unknown WM_PROTOCOLS: " << XAtomName(a));
			}
	}
	else
	if(event->тип == PropertyNotify && event->xproperty.atom == XAtom("_NET_WM_STATE")) {
		LLOG("_NET_WM_STATE notify");
		Вектор<int> p = GetPropertyInts(GetWindow(), XAtom("_NET_WM_STATE"));
		if(найдиИндекс(p, (int)XAtom("_NET_WM_STATE_HIDDEN")) >= 0) {
			state = MINIMIZED;
			LLOG("MINIMIZED");
		}
		else
		if(найдиИндекс(p, (int)XAtom("_NET_WM_STATE_MAXIMIZED_HORZ")) >= 0 &&
		   найдиИндекс(p, (int)XAtom("_NET_WM_STATE_MAXIMIZED_VERT")) >= 0) {
			state = MAXIMIZED;
			LLOG("MAXIMIZED");
		}
		else {
			state = OVERLAPPED;
			LLOG("OVERLAPPED");
		}
	}
	if(this_) Ктрл::СобытиеProc(w, event);
	if(this_) SyncSizeHints();
}

void ТопОкно::DefSyncTitle()
{
	ЗамкниГип __;
	if(title2 == title)
		return;
	title2 = title;
	if(открыт() && GetWindow()) {
		Window w = GetWindow();
		XStoreName(Xdisplay, w, title.вТкст());
		XSetIconName(Xdisplay, w, title.вТкст());
		Ткст utf8title = изЮникода(title, CHARSET_UTF8);
		XChangeProperty(Xdisplay, w, XAtom("_NET_WM_NAME"), XAtom("UTF8_STRING"),
		                8, PropModeReplace,
		                (const unsigned char *)~utf8title, utf8title.дайДлину());
		XChangeProperty(Xdisplay, w, XAtom("_NET_WM_ICON_NAME"), XAtom("UTF8_STRING"),
		                8, PropModeReplace,
		                (const unsigned char *)~utf8title, utf8title.дайДлину());
	}
}

void ТопОкно::SyncTitle()
{
	ЗамкниГип __;
	LLOG("SyncTitle: " << title);
	глушиОбрвызВремени(TIMEID_DEFSYNCTITLE);
	устОбрвызВремени(0, THISBACK(DefSyncTitle), TIMEID_DEFSYNCTITLE);
	LLOG("*SyncTitle: " << title);
}

void WmState(Window w, bool set, Atom a1, Atom a2 = 0)
{
	XСобытие e;
	memset(&e, 0, sizeof(e));
	e.xclient.тип = ClientMessage;
	e.xclient.message_type = XAtom("_NET_WM_STATE");
	e.xclient.дисплей = Xdisplay;
	e.xclient.window = w;
	e.xclient.формат = 32;
	e.xclient.data.l[0] = set;
	e.xclient.data.l[1] = a1;
	e.xclient.data.l[2] = a2;
	XSendСобытие(Xdisplay, Xroot, false, SubstructureNotifyMask | SubstructureRedirectMask, &e);
}

void ТопОкно::SyncState()
{
	ЗамкниГип __;
	LLOG("SyncState");
	SyncCaption();
	if(открыт() && GetWindow()) {		
		Window w = GetWindow();
		WmState(w, topmost, XAtom("_NET_WM_STATE_ABOVE"));
		WmState(w, state == MAXIMIZED, XAtom("_NET_WM_STATE_MAXIMIZED_HORZ"), XAtom("_NET_WM_STATE_MAXIMIZED_VERT"));
		if(state == MINIMIZED)
			XIconifyWindow(Xdisplay, GetWindow(), Xscreenno);
		else
			XMapWindow(Xdisplay, GetWindow());

		WmState(w, state == MINIMIZED, XAtom("_NET_WM_STATE_HIDDEN"));
		WmState(w, fullscreen, XAtom("_NET_WM_STATE_FULLSCREEN"));
	}
}

typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
} MWMHints;

#define MWM_HINTS_DECORATIONS   (1L << 1)
#define PROP_MOTIF_WM_HINTS_ELEMENTS 5
#define PROP_MWM_HINTS_ELEMENTS PROP_MOTIF_WM_HINTS_ELEMENTS

void ТопОкно::SyncCaption()
{
	ЗамкниГип __;
	LLOG("SyncCaption0");
	SyncTitle();
	if(открыт() && GetWindow()) {
		unsigned long wina[6];
		memset(wina, 0, sizeof(wina));
		int n = 0;
		Window w = GetWindow();
		if(tool)
			wina[n++] = XAtom("_NET_WM_WINDOW_TYPE_UTILITY");
		if(дайВладельца())
			wina[n++] = XAtom("_NET_WM_WINDOW_TYPE_DIALOG");
		wina[n++] = XAtom("_NET_WM_WINDOW_TYPE_NORMAL");
		XChangeProperty(Xdisplay, GetWindow(), XAtom("_NET_WM_WINDOW_TYPE"), XAtom("ATOM"), 32,
		                PropModeReplace, (const unsigned char *)wina, n);

//		n = 0;
//		XChangeProperty(Xdisplay, GetWindow(), XAtom("_NET_WM_STATE"), XAtom("ATOM"), 32,
//		                PropModeReplace, (const unsigned char *)wina, n);
		wm_hints->flags = InputHint|WindowGroupHint|StateHint;
		if(urgent) {
			if(пп_ли()) urgent = false;
			wm_hints->flags |= XUrgencyHint;
			WmState(w, urgent, XAtom("_NET_WM_STATE_DEMANDS_ATTENTION"));
		}
		wm_hints->initial_state = NormalState;
		wm_hints->input = XTrue;
		Ктрл *owner = дайВладельца();
		wm_hints->window_group = owner ? owner->GetWindow() : w;
		if(!icon.пустой()) {
			int len = 0;
			Буфер<unsigned long> data = PreperIcon(icon, len);
			XChangeProperty(Xdisplay, w, XAtom("_NET_WM_ICON"), XA_CARDINAL, 32, PropModeReplace,
			               (const unsigned char *)~data, len);
			if (!largeicon.пустой()) {
				data = PreperIcon(largeicon, len);
				XChangeProperty(Xdisplay, w, XAtom("_NET_WM_ICON"), XA_CARDINAL, 32, PropModePrepend,
			                   (const unsigned char *)~data, len);
			}
		}
		XSetWMHints(Xdisplay, w, wm_hints);

		MWMHints mwmhints;
		memset(&mwmhints, 0, sizeof(mwmhints));
		mwmhints.flags = MWM_HINTS_DECORATIONS;
		mwmhints.decorations = !frameless;
		XChangeProperty(Xdisplay, w, XAtom("_MOTIF_WM_HINTS"), XAtom("_MOTIF_WM_HINTS"), 32,
		                PropModeReplace,
		                (unsigned char *) &mwmhints, PROP_MWM_HINTS_ELEMENTS);
	}
}

Буфер<unsigned long> ТопОкно::PreperIcon(const Рисунок& icon, int& len) {
	Размер isz = icon.дайРазм();
	len = 2 + isz.cx * isz.cy;
	Буфер<unsigned long> data(len);
	unsigned long *t = data;
	*t++ = isz.cx;
	*t++ = isz.cy;
	for(int y = 0; y < isz.cy; y++) {
		const КЗСА *q = icon[y];
		for(int x = isz.cx; x--;) {
			*t++ = ((dword)q->a << 24) |
				   (dword)q->b | ((dword)q->g << 8) | ((dword)q->r << 16);
			q++;
		}
	}
	return data;
}

void ТопОкно::центрПрям(Ктрл *owner)
{
	ЗамкниГип __;
	SetupRect(owner);
	if((owner && center == 1) || center == 2) {
		Прям r, wr;
		wr = Ктрл::GetWorkArea();
		Размер sz = дайПрям().размер();
		Прям fm = windowFrameMargin;
		if((fm.left|fm.right|fm.top|fm.bottom) == 0)
			fm = Прям(8, 32, 8, 8);
		if(owner && center == 1)
			r = owner->дайПрям();
		else
			r = wr;
		Точка p = r.позЦентра(sz);
		
		if (GetVirtualScreenArea().содержит(p)) {
			r = RectC(p.x, p.y, sz.cx, sz.cy);
			wr.left += fm.left;
			wr.right -= fm.right;
			wr.top += fm.top;
			wr.bottom -= fm.bottom;
			if(r.top < wr.top) {
				r.bottom += wr.top - r.top;
				r.top = wr.top;
			}
			if(r.bottom > wr.bottom)
				r.bottom = wr.bottom;
			minsize.cx = min(minsize.cx, r.дайШирину());
			minsize.cy = min(minsize.cy, r.дайВысоту());
			устПрям(r);
		}	
	}
}

void ТопОкно::открой(Ктрл *owner)
{
	LLOG("ТопОкно::открой");
	ЗамкниГип __;
	if(dokeys && (!GUI_AKD_Conservative() || GetAccessKeysDeep() <= 1))
		DistributeAccessKeys();
	USRLOG("   OPEN " + Desc(this));
	LLOG("OPEN " << Имя() << " owner: " << РНЦП::Имя(owner));
	IgnoreMouseUp();
	bool weplace = owner && center == 1 || center == 2 || !дайПрям().пустой();
	if(fullscreen)
		устПрям(0, 0, Xwidth, Xheight);
	else
		центрПрям(owner);
	LLOG("открой NextRequest " << NextRequest(Xdisplay));
	создай(owner, false, false);
	XSetWMProperties (Xdisplay, GetWindow(), NULL, NULL, NULL, 0, NULL, NULL, NULL);
	xminsize.cx = xmaxsize.cx = Null;
	title2.очисть();
	if(!weplace) {
		LLOG("SyncCaption");
		SyncCaption();
	}
	LLOG("SyncSizeHints");
	size_hints->flags = 0;
	SyncSizeHints();
	Прям r = дайПрям();
	size_hints->x = r.left;
	size_hints->y = r.top;
	size_hints->width = r.устШирину();
	size_hints->height = r.устВысоту();
	size_hints->win_gravity = StaticGravity;
	size_hints->flags |= PPosition|PSize|PWinGravity;
	if(owner) {
		ПРОВЕРЬ(owner->открыт());
		LLOG("XSetTransientForHint");
		XSetTransientForHint(Xdisplay, GetWindow(), owner->GetWindow());
	}
	LLOG("XSetWMNormalHints");
	XSetWMNormalHints(Xdisplay, GetWindow(), size_hints);
	Atom protocols[3];
	protocols[0] = XAtom("WM_DELETE_WINDOW");
	protocols[1] = XAtom("WM_TAKE_FOCUS");
	protocols[2] = XAtom("_NET_WM_PING");
	LLOG("XSetWMProtocols");
	XSetWMProtocols(Xdisplay, GetWindow(), protocols, 3);
	Ткст x = дайТитулИсп().вТкст();
	const char *progname = ~x;
	class_hint->res_name = (char *)progname;
	class_hint->res_class = (char *)progname;
	XSetClassHint(Xdisplay, GetWindow(), class_hint);
	LLOG("WndShow(" << visible << ")");
	WndShow(visible);
	if(visible) {
		XСобытие e;
		LLOG("XWindowСобытие");
		XWindowСобытие(Xdisplay, top->window, VisibilityChangeMask, &e);
		ignoretakefocus = true;
		устОбрвызВремени(500, THISBACK(EndIgnoreTakeFocus));
		LLOG("устФокусОкна");
		устФокусОкна();
		for(int i = 0; i < 50; i++) {
			// X11 tries to move our window, so ignore the first set of ConfigureNotify
			// and move the window into position after FocusIn - but not if we want WM to
			// place the window
			if(weplace)
				while(XCheckTypedWindowСобытие(Xdisplay, top->window, ConfigureNotify, &e)) {
					if(e.xconfigure.window != top->window)
						обработайСобытие(&e);
				}
			if(XCheckTypedWindowСобытие(Xdisplay, top->window, FocusIn, &e)) {
				обработайСобытие(&e);
				if(e.xfocus.window == top->window)
					break;
			}
			спи(10);
		}
	}
	if(weplace) {
		WndSetPos(дайПрям());
		LLOG("SyncCaption");
		SyncCaption();
	}
	LLOG(">открой NextRequest " << NextRequest(Xdisplay));
	LLOG(">OPENED " << Имя());
	PlaceFocus();
	StateH(OPEN);
	Вектор<int> fe = GetPropertyInts(top->window, XAtom("_NET_FRAME_EXTENTS"));
	if(fe.дайСчёт() >= 4 &&
	   fe[0] >= 0 && fe[0] <= 16 && fe[1] >= 0 && fe[1] <= 16 && //fluxbox returns wrong numbers - quick&dirty workaround
	   fe[2] >= 0 && fe[2] <= 64 && fe[3] >= 0 && fe[3] <= 48)
	{
		ЗамкниГип __;
		windowFrameMargin.left = max(windowFrameMargin.left, fe[0]);
		windowFrameMargin.right = max(windowFrameMargin.right, fe[1]);
		windowFrameMargin.top = max(windowFrameMargin.top, fe[2]);
		windowFrameMargin.bottom = max(windowFrameMargin.bottom, fe[3]);
	}
	if(открыт() && top)
		top->owner = owner;

	long curr_pid = getpid();

	static Window wm_client_leader;
	ONCELOCK {
		wm_client_leader = XCreateSimpleWindow(Xdisplay, Xroot, 0, 0, 1, 1, 0, 0, 0);
		XChangeProperty(Xdisplay, wm_client_leader, XAtom("WM_CLIENT_LEADER"),
		                XA_WINDOW, 32, PropModeReplace, (byte *)&wm_client_leader, 1);
		XChangeProperty(Xdisplay, wm_client_leader, XAtom("_NET_WM_PID"), XA_CARDINAL, 32,
		                PropModeReplace, (byte *) &curr_pid, 1);
	}

	Window win = GetWindow();
	XChangeProperty(Xdisplay, win, XAtom("_NET_WM_PID"), XA_CARDINAL, 32,
	                PropModeReplace, (byte *) &curr_pid, 1);
	XChangeProperty(Xdisplay, win, XAtom("WM_CLIENT_LEADER"),
	                XA_WINDOW, 32, PropModeReplace, (byte *)&wm_client_leader, 1);

	int version = 5;
	XChangeProperty(Xdisplay, win, XAtom("XdndAware"), XA_ATOM, 32,
					0, (byte *)&version, 1);

	SyncState();
	FixIcons();
}

void ТопОкно::открой()
{
	ЗамкниГип __;
	открой(GetActiveWindow());
}

void ТопОкно::откройГлавн()
{
	ЗамкниГип __;
	открой(NULL);
}

void ТопОкно::сверни(bool)
{
	ЗамкниГип __;
	state = MINIMIZED;
	SyncState();
}

void ТопОкно::разверни(bool effect)
{
	ЗамкниГип __;
	state = MAXIMIZED;
	SyncState();
}

void ТопОкно::нахлёст(bool effect)
{
	ЗамкниГип __;
	state = OVERLAPPED;
	SyncState();
}

ТопОкно& ТопОкно::полноэкранно(bool b)
{
	ЗамкниГип __;
	fullscreen = b;
	SyncState();
	return *this;
}

ТопОкно& ТопОкно::наиверхнее(bool b, bool)
{
	ЗамкниГип __;
	topmost = b;
	SyncState();
	return *this;
}

bool ТопОкно::наиверхнее_ли() const
{
	ЗамкниГип __;
	return topmost;
}

void ТопОкно::GuiPlatformConstruct()
{
	size_hints = XAllocSizeHints();
	wm_hints = XAllocWMHints();
	class_hint = XAllocClassHint();
	topmost = false;
}

void ТопОкно::GuiPlatformDestruct()
{
	XFree(size_hints);
	XFree(wm_hints);
	XFree(class_hint);
}

void ТопОкно::SerializePlacement(Поток& s, bool reminimize)
{
	ЗамкниГип __;
	int version = 0;
	s / version;
	Прям rect = дайПрям();
	s % overlapped % rect;
	bool mn = state == MINIMIZED;
	bool mx = state == MAXIMIZED;
	s.Pack(mn, mx);
	LLOG("ТопОкно::SerializePlacement / " << (s.сохраняется() ? "write" : "read"));
	LLOG("minimized = " << mn << ", maximized = " << mx);
	LLOG("rect = " << rect << ", overlapped = " << overlapped);
	if(s.грузится()) {
		if(mn) rect = overlapped;
		Прям limit = GetVirtualWorkArea();
		Прям fm = windowFrameMargin;
		if((fm.left|fm.right|fm.top|fm.bottom) == 0)
			fm = Прям(8, 32, 8, 8);
		limit.left += fm.left;
		limit.right -= fm.right;
		limit.top += fm.top;
		limit.bottom -= fm.bottom;
		Размер sz = min(max(дайМинРазм(), rect.размер()), limit.размер());
		rect = RectC(
			minmax(rect.left, limit.left, limit.right - sz.cx),
			minmax(rect.top,  limit.top,  limit.bottom - sz.cy),
			sz.cx, sz.cy);
		state = OVERLAPPED;
		if(mn && reminimize)
			state = MINIMIZED;
		if(mx)
			state = MAXIMIZED;
		if(state == OVERLAPPED)
			устПрям(rect);
		if(открыт()) {
			if(state == MINIMIZED)
				сверни(false);
			if(state == MAXIMIZED)
				разверни(false);
		}
	}
}

}

#endif
