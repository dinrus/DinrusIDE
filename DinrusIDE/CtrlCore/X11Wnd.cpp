#include "CtrlCore.h"

#ifdef GUI_X11

#include <X11/Xlocale.h>

namespace РНЦП {

#ifdef _ОТЛАДКА

bool Ктрл::LogMessages

// = true _DBG_
;
bool __X11_Grabbing = false;
#endif

#define LLOG(x)      // DLOG(x)
#define LTIMING(x)   // RTIMING(x)
#define LDUMP(x)     // DDUMP(x)
#define LDUMPC(x)    // DDUMPC(x)

// #define SYNCHRONIZE

#define x_Событие(x) { x, #x },

static
struct XСобытиеMap {
	int         ИД;
	const char *имя;
}
sXevent[] = {
#include "X11Событие.i"
	{ 0, NULL }
};

МассивМап<Window, Ктрл::XWindow>& Ктрл::Xwindow()
{
	return Single< МассивМап<Window, XWindow> >();
}

int       Ктрл::WndCaretTime;
bool      Ктрл::WndCaretVisible;
int       Ктрл::Xbuttons;
Window    Ктрл::grabWindow, Ктрл::focusWindow;
int       Ктрл::Xeventtime;

int       Ктрл::PopupGrab;
Ук<Ктрл> Ктрл::popupWnd;

Точка     Ктрл::mousePos;

static int s_starttime;

void Ктрл::DoPaint(const Вектор<Прям>& invalid)
{
	ЗамкниГип __;
	if(видим_ли()) {
		LTIMING("DoPaint");
		fullrefresh = false;
//		if(GLX) return;
		GC gc = XCreateGC(Xdisplay, (Drawable)top->window, 0, 0);
		XftDraw *xftdraw = XftDrawCreate(Xdisplay, (Drawable) top->window,
		                                 DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
		SystemDraw draw(top->window, gc, xftdraw, invalid);
		painting = true;
		UpdateArea(draw, draw.GetClip());
		painting = false;
		XftDrawDestroy(xftdraw);
		XFreeGC(Xdisplay, gc);
	}
}

void  Ктрл::WndScrollView(const Прям& r, int dx, int dy)
{
	ЗамкниГип __;
	if(r.пустой() || !GetWindow()) return;
	int cx = r.устШирину() - абс(dx);
	int cy = r.устВысоту() - абс(dy);
	GC gc = XCreateGC(Xdisplay, GetWindow(), 0, 0);
	XCopyArea(Xdisplay, GetWindow(), GetWindow(), gc,
	          r.left + max(-dx, 0), r.top + max(-dy, 0), cx, cy,
	          r.left + max(dx, 0), r.top + max(dy, 0));
	XFreeGC(Xdisplay, gc);
	XWindow *xw = GetXWindow();
	Вектор<Прям> ur;
	if(xw)
		for(int i = 0; i < xw->invalid.дайСчёт(); i++)
			if(xw->invalid[i].пересекается(r))
				ur.добавь(xw->invalid[i]);
	for(int i = 0; i < ur.дайСчёт(); i++)
		инивалидируй(*xw, ur[i].смещенец(dx, dy));
}

bool Ктрл::ожидаетСобытие()
{
	ПРОВЕРЬ_(главнаяНить_ли(), "ожидаетСобытие can only run in the main thread");
	ЗамкниГип __;
	return XPending(Xdisplay);
}

Ктрл *Ктрл::CtrlFromWindow(Window w)
{
	ЗамкниГип __;
	int q = Xwindow().найди(w);
	return q >= 0 ? Xwindow()[q].ctrl : NULL;
}

Ктрл::XWindow *Ктрл::GetXWindow()
{
	ЗамкниГип __;
	if(!top) return NULL;
	int q = Xwindow().найди(top->window);
	return q >= 0 ? &Xwindow()[q] : NULL;
}
// 01/12/2007 - mdelfede
// added support for windowed controls

// Gets handle of window containing control
Window Ктрл::GetParentWindow(void) const
{
	ЗамкниГип __;
	Ктрл const *q = GetParentWindowCtrl();
	if(q)
		return q->top->window;
	else
		return 0;

} // END Ктрл::GetParentWindow()

// дай control with parentwindow as handle
Ктрл *Ктрл::GetParentWindowCtrl(void) const
{
	ЗамкниГип __;
	Ктрл *q = parent;
	while(q && !q->top)
		q = q->parent;
	return q;

} // END Ктрл::GetParentWindowCtrl()

// Gets the rect inside the parent window
Прям Ктрл::GetRectInParentWindow(void) const
{
	ЗамкниГип __;
	Прям r = дайПрямЭкрана();
	Ктрл *q = parent;
	while(q && !q->top)
		q = q->parent;
	if(q)
	{
		Прям pr = q->дайПрямЭкрана();
		r -= pr.верхЛево();
	}
	return r;
}
// 01/12/2007 - END

bool Ктрл::HookProc(XСобытие *event) { return false; }

void DnDRequest(XSelectionRequestСобытие *se);
void DnDClear();

dword X11mods(dword ключ)
{
	dword mod = 0;
	if(ключ & K_ALT)
		mod |= Mod1Mask;
	if(ключ & K_SHIFT)
		mod |= ShiftMask;
	if(ключ & K_CTRL)
		mod |= ControlMask;
	return mod;
}

Вектор<Событие<> > Ктрл::hotkey;
Вектор<dword> Ктрл::keyhot;
Вектор<dword> Ктрл::modhot;

int Ктрл::RegisterSystemHotKey(dword ключ, Функция<void ()> cb)
{
	ЗамкниГип __;
	ПРОВЕРЬ(ключ >= K_DELTA);
	bool b = TrapX11Errors();
	KeyCode k = XKeysymToKeycode(Xdisplay, ключ & 0xffff);
	dword mod = X11mods(ключ);
	bool r = false;
	for(dword nlock = 0; nlock < 2; nlock++)
		for(dword clock = 0; clock < 2; clock++)
			for(dword slock = 0; slock < 2; slock++)
				r = XGrabKey(Xdisplay, k,
				             mod | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
				             Xroot, true, GrabModeAsync, GrabModeAsync) || r;
	UntrapX11Errors(b);
	if(!r) return -1;
	int q = hotkey.дайСчёт();
	for(int i = 0; i < hotkey.дайСчёт(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.по(q) << cb;
	keyhot.по(q) = k;
	modhot.по(q) = mod;
	return q;
}

void Ктрл::UnregisterSystemHotKey(int id)
{
	ЗамкниГип __;
	if(id >= 0 && id < hotkey.дайСчёт() && hotkey[id]) {
		bool b = TrapX11Errors();
		for(dword nlock = 0; nlock < 2; nlock++)
			for(dword clock = 0; clock < 2; clock++)
				for(dword slock = 0; slock < 2; slock++)
					XUngrabKey(Xdisplay, keyhot[id],
					           modhot[id] | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
					           Xroot);
		UntrapX11Errors(b);
		hotkey[id].очисть();
	}
}

void Ктрл::обработайСобытие(XСобытие *event)
{
	ЗамкниГип __;
	if(xim && XFilterСобытие(event, None))
		return;
	if(event->тип == KeyPress)
		for(int i = 0; i < hotkey.дайСчёт(); i++)
			if(hotkey[i] && keyhot[i] == event->xkey.keycode
			   && modhot[i] == (event->xkey.state & (Mod1Mask|ShiftMask|ControlMask))) {
				hotkey[i]();
				return;
			}
	МассивМап<Window, Ктрл::XWindow>& xmap = Xwindow();
	for(int i = 0; i < xmap.дайСчёт(); i++)
		if(xmap[i].ctrl && xmap[i].ctrl->HookProc(event))
			return;
	FocusSync();
	if(event->тип == PropertyNotify &&
	   (event->xproperty.atom == XAtom("_QT_SETTINGS_TIMESTAMP_") ||
	    event->xproperty.atom == XAtom("_KDE_NET_USER_TIME"))) {
		LLOG("Property notify " << XAtomName(event->xproperty.atom)
		     << " " << (void *)event->xany.window);
		for(int i = 0; i < Xwindow().дайСчёт(); i++) {
			if(Xwindow().дайКлюч(i)) {
				Ктрл *q = Xwindow()[i].ctrl;
				if(q) q->освежи();
			}
		}
	}
	if(event->тип == SelectionRequest &&
	   event->xselectionrequest.owner == xclipboard().win) {
		if(event->xselectionrequest.selection == XAtom("XdndSelection"))
			DnDRequest(&event->xselectionrequest);
		else
			xclipboard().Request(&event->xselectionrequest);
		return;
	}
	if(event->тип == SelectionClear &&
	   event->xselectionclear.window == xclipboard().win) {
		if(event->xselectionclear.selection == XAtom("PRIMARY")) {
			sel_formats.очисть();
			sel_ctrl = NULL;
		}
		if(event->xselectionclear.selection == XAtom("CLIPBOARD"))
			xclipboard().очисть();
		if(event->xselectionrequest.selection == XAtom("XdndSelection"))
			DnDClear();
		return;
	}
	int q = xmap.найди(event->xany.window);
#ifdef _ОТЛАДКА
	bool eo = false;
	if(LogMessages && event->тип != NoExpose && event->тип != PropertyNotify
	               && event->тип != MotionNotify)
		for(XСобытиеMap *m = sXevent; m->ИД; m++)
			if(m->ИД == event->тип) {
				if(!s_starttime)
					s_starttime = msecs();
				int t = msecs() - s_starttime;
				VppLog() << фмт("%d.%01d", t / 1000, t % 1000);
				VppLog() << " EVENT " << фмт("%-20.20s", m->имя);
				VppLog() << "[window: " << event->xany.window << "] ";
				if(q >= 0)
					VppLog() << '<' << РНЦП::Имя(Xwindow()[q].ctrl) << '>';
				else
					VppLog() << "<unknown ctrl> ";
				if(event->тип == ClientMessage)
					VppLog() << ": " << XAtomName(event->xclient.message_type);
				VppLog() << '\n';
				eo = true;
				break;
			}
#endif
	DropStatusСобытие(event);
	if(q < 0) {
		if(event->тип == ButtonRelease && popupWnd)
			popupWnd->устФокус();
		return;
	}
	XWindow& w = xmap[q];
	if(w.ctrl) {
		w.ctrl->СобытиеProc(w, event);
		if(w.ctrl)
			w.ctrl->SyncMoves();
	}
	else
		xmap.устКлюч(q, None);
	DefferedFocusSync();
#ifdef _ОТЛАДКА
#ifdef UPP_HEAP
	if(MemoryCheck)
		РНЦП::MemoryCheck();
#endif
	if(eo)
		LLOG("------ end of event processing ----------");
#endif
}

void Ктрл::TimerAndPaint() {
	ЗамкниГип __;
	LTIMING("TimerAndPaint");
	TimerProc(msecs());
	for(int i = 0; i < Xwindow().дайСчёт(); i++) {
		XWindow& xw = Xwindow()[i];
		if(Xwindow().дайКлюч(i) && xw.exposed && xw.invalid.дайСчёт()) {
			if(xw.ctrl) {
				LLOG("..and paint " << РНЦП::Имя(xw.ctrl));
				xw.ctrl->синхПромот();
				Вектор<Прям> x = pick(xw.invalid);
				xw.invalid.очисть();
				xw.ctrl->DoPaint(x);
			}
			else
				Xwindow().устКлюч(i, None);
		}
	}
	SyncCaret();
	анимируйКаретку();
	XSync(Xdisplay, false);
	SyncIMPosition();
}

bool Ктрл::обработайСобытие(bool *)
{
	ЗамкниГип __;
	if(!ожидаетСобытие()) return false;
	XСобытие event;
	XNextСобытие(Xdisplay, &event);
	обработайСобытие(&event);
	return true;
}

void SweepMkImageCache();

bool Ктрл::обработайСобытия(bool *)
{
	ПРОВЕРЬ_(главнаяНить_ли(), "обработайСобытия can only run in the main thread");
	ЗамкниГип __;
	if(обработайСобытие()) {
		while(обработайСобытие() && (!LoopCtrl || LoopCtrl->InLoop())); // LoopCtrl-MF 071008
		TimerAndPaint();
		SweepMkImageCache();
		return true;
	}
	TimerAndPaint();
	SweepMkImageCache();
	return false;
}

static bool WakePipeOK;
static int  WakePipe[2];

ИНИЦБЛОК {
	WakePipeOK = pipe(WakePipe) == 0;
	fcntl(WakePipe[0], F_SETFL, O_NONBLOCK);
	fcntl(WakePipe[1], F_SETFL, O_NONBLOCK);
}

void WakeUpGuiThread()
{
	if(WakePipeOK)
		IGNORE_RESULT(write(WakePipe[1], "\1", 1));
}

void Ктрл::гипСпи(int ms)
{
	ЗамкниГип __;
	LLOG(msecs() << " GUISLEEP " << ms);
	ПРОВЕРЬ_(главнаяНить_ли(), "Only main thread can perform гипСпи");
	timeval timeout;
	timeout.tv_sec = ms / 1000;
	timeout.tv_usec = ms % 1000 * 1000;
	XFlush(Xdisplay);
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(Xconnection, &fdset);
	if(WakePipeOK)
		FD_SET(WakePipe[0], &fdset);
	int level = покиньВсеСтопорыГип(); // Give  a chance to nonmain threads to touch GUI things
	select((WakePipeOK ? max(WakePipe[0], Xconnection) : Xconnection) + 1, &fdset, NULL, NULL, &timeout);
	char h;
	while(WakePipeOK && read(WakePipe[0], &h, 1) > 0) // There might be relatively harmless race condition here causing delay in ICall
		LLOG(msecs() << " WakeUpGuiThread detected!"); // or "void" passes through timer & paint
	войдиВСтопорГип(level);
}

static int granularity = 10;

void Ктрл::SetTimerGranularity(int ms)
{
	ЗамкниГип __;
	granularity = ms;
}

void Ктрл::SysEndLoop()
{
}

void Ктрл::циклСобытий(Ктрл *ctrl)
{
	ЗамкниГип __;
	ПРОВЕРЬ_(главнаяНить_ли(), "циклСобытий can only run in the main thread");
	ПРОВЕРЬ(LoopLevel == 0 || ctrl);
	LoopLevel++;
	int64 loopno = ++СобытиеLoopNo;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ктрл *ploop = NULL;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	while(loopno > EndSessionLoopNo && (ctrl ? ctrl->InLoop() && ctrl->открыт() : дайТопКтрлы().дайСчёт())) {
		XСобытие event;
		гипСпи(granularity);
		SyncMousePos();
		while(ожидаетСобытие()) {
			LTIMING("XNextСобытие");
			XNextСобытие(Xdisplay, &event);
			обработайСобытие(&event);
		}
		TimerAndPaint();
		SweepMkImageCache();
	}

	if(ctrl)
		LoopCtrl = ploop;

	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ктрл::SyncExpose()
{
	ЗамкниГип __;
	if(!top) return;
	XСобытие event;
	while(top && XCheckTypedWindowСобытие(Xdisplay, top->window, Expose, &event))
		обработайСобытие(&event);
	while(top && XCheckTypedWindowСобытие(Xdisplay, top->window, GraphicsExpose, &event))
		обработайСобытие(&event);
}

void Ктрл::создай(Ктрл *owner, bool redirect, bool savebits)
{
	ЗамкниГип __;
	ПРОВЕРЬ_(главнаяНить_ли(), "Only main thread can create windows");
	LLOG("создай " << Имя() << " " << дайПрям());
	ПРОВЕРЬ(!отпрыск_ли() && !открыт());
	LLOG("Ungrab1");
	ReleaseGrab();
	XSetWindowAttributes swa;
	swa.bit_gravity = ForgetGravity;
	swa.background_pixmap = None;
	swa.override_redirect = redirect;
	swa.save_under = savebits;
	Цвет c = SColorPaper();
	swa.background_pixel = GetXPixel(c.дайК(), c.дайЗ(), c.дайС());
	Прям r = дайПрям();
	isopen = true;
	Window w = XCreateWindow(Xdisplay, RootWindow(Xdisplay, Xscreenno),
	                         r.left, r.top, r.устШирину(), r.устВысоту(),
	                         0, CopyFromParent, InputOutput, CopyFromParent,
	                         CWBitGravity|CWSaveUnder|CWOverrideRedirect|CWBackPixmap,
	                         &swa);
	if(!w) XError("XCreateWindow failed !");
	int i = Xwindow().найди(None);
	if(i >= 0) Xwindow().устКлюч(i, w);
	XWindow& cw = i >= 0 ? Xwindow()[i] : Xwindow().добавь(w);
	cw.ctrl = this;
	cw.exposed = false;
	cw.owner = owner;
	
	cw.xic = NULL;
	
	if(xim) {
		cw.xic = XCreateIC(xim,
		                   XNInputStyle, XIMPreeditNothing|XIMStatusNothing,
		                   XNClientWindow, w,
		                   NULL);
	}
	top = new Верх;
	top->window = w;
	long im_event_mask = 0;
	if(cw.xic)
		XGetICValues(cw.xic, XNFilterСобытиеs, &im_event_mask, NULL);
	XSelectInput(Xdisplay, w, ExposureMask|StructureNotifyMask|KeyPressMask|
	             FocusChangeMask|KeyPressMask|KeyReleaseMask|PointerMotionMask|
	             ButtonPressMask|ButtonReleaseMask|PropertyChangeMask|
	             VisibilityChangeMask|im_event_mask);
	int version = 5;
	XChangeProperty(Xdisplay, w, XAtom("XdndAware"), XA_ATOM, 32,
					0, (byte *)&version, 1);
	режимОтмены();
	if(r.содержит(дайПозМыши()))
		DispatchMouse(MOUSEMOVE, дайПозМыши() - r.верхЛево());

	if(redirect) {
		int windowType = XInternAtom(Xdisplay, "_NET_WM_WINDOW_TYPE_POPUP_MENU", false);
		XChangeProperty(Xdisplay, w, XInternAtom(Xdisplay, "_NET_WM_WINDOW_TYPE", false), XA_ATOM, 32, 
		                PropModeReplace, (byte *)&windowType, 1);
	}
	
	RefreshLayoutDeep();
	
	SyncIMPosition();
}

void Ктрл::разрушьОкно()
{
	ЗамкниГип __;
	LLOG("разрушьОкно " << Имя());
	if(!top || !isopen) return;
	AddGlobalRepaint();
	bool revertfocus = естьФокусОкна() || !GetFocusCtrl();
	for(int i = 0; i < Xwindow().дайСчёт(); i++) {
		LOGBEGIN();
		XWindow& w = Xwindow()[i];
		if(Xwindow().дайКлюч(i) != None && w.owner == this && w.ctrl->открыт())
			w.ctrl->разрушьОкно();
		LOGEND();
	}
	Ук<Ктрл> owner;
	int i = Xwindow().найди(top->window);
	if(i >= 0) {
		XWindow& w = Xwindow()[i];
		owner = w.owner;
		w.invalid.очисть();
		if(w.xic)
			XDestroyIC(w.xic);
	}
	isopen = false;
	if(focusWindow == top->window)
		focusWindow = None;
	if(grabWindow == top->window)
		grabWindow = None;
	XDestroyWindow(Xdisplay, top->window);
	if(i >= 0) {
		Xwindow().устКлюч(i, None);
		top->window = None;
		Xwindow()[i].ctrl = NULL;
	}

	if(revertfocus && owner)
		owner->TakeFocus();

	if(focusWindow) {
		int q = Xwindow().найди(focusWindow);
		if(q >= 0) {
			XIC xic = Xwindow()[q].xic;
			if(xic) {
				XSetICFocus(xic);
				SyncIMPosition();
			}
		}
	}

	delete top;
	top = NULL;
	FocusSync();
}

Вектор<Ктрл *> Ктрл::дайТопКтрлы()
{
	ЗамкниГип __;
	Вектор<Ктрл *> v;
	const МассивМап<Window, Ктрл::XWindow>& w = Xwindow();
	for(int i = 0; i < w.дайСчёт(); i++)
		if(w.дайКлюч(i) && w[i].ctrl && !w[i].ctrl->parent)
			v.добавь(w[i].ctrl);
	return v;
}

void Ктрл::StartPopupGrab()
{
	ЗамкниГип __;
	if(PopupGrab == 0) {
		if(!top) return;
		if(XGrabPointer(
		   Xdisplay, top->window, true,
		   ButtonPressMask|ButtonReleaseMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask,
		   GrabModeAsync, GrabModeAsync, None, None, CurrentTime) == GrabSuccess) {
				PopupGrab++;
				popupWnd = дайТопОкно();
#ifdef _ОТЛАДКА
				__X11_Grabbing = true;
#endif
			}
	}
}

void Ктрл::EndPopupGrab()
{
	ЗамкниГип __;
	if(PopupGrab == 0) return;
	if(--PopupGrab == 0) {
		XUngrabPointer(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
#ifdef _ОТЛАДКА
		__X11_Grabbing = false;
#endif
	}
}

void Ктрл::PopUp(Ктрл *owner, bool savebits, bool activate, bool, bool)
{
	ЗамкниГип __;
	LLOG("POPUP: " << РНЦП::Имя(this));
	Ктрл *q = owner ? owner->дайТопКтрл() : дайАктивныйКтрл();
	ignoretakefocus = true;
	создай(q, true, savebits);
	if(activate) {
		q->StartPopupGrab();
		popupgrab = true;
	}
	if(top) popup = true;
	WndShow(visible);
	if(activate && включен_ли())
		устФокус();
	if(top) top->owner = owner;
	StateH(OPEN);
}

Ктрл *Ктрл::дайАктивныйКтрл()
{
	ЗамкниГип __;
	return focusCtrl ? focusCtrl->дайТопКтрл() : NULL;
}

bool  Ктрл::IsAlphaSupported()
{
	return IsCompositedGui();
}

void  Ктрл::SetAlpha(byte alpha)
{
	ЗамкниГип __;
	Window hwnd = GetWindow();
	if (!IsAlphaSupported() || parent || !top || !hwnd)
		return;
	unsigned int opacity = (unsigned int) 16843009 * alpha;
	Atom aw_opacity = XInternAtom(Xdisplay, "_NET_WM_WINDOW_OPACITY", XFalse);
	XChangeProperty(Xdisplay, hwnd, aw_opacity, XA_CARDINAL, 32, PropModeReplace,
					(unsigned char *) &opacity, 1);
}

bool Ктрл::IsCompositedGui()
{
	ЗамкниГип __;
	static bool b = XGetSelectionOwner(Xdisplay, XAtom(Ткст().конкат() << "_NET_WM_CM_S" << Xscreenno)) != None;
	return b;
}

Ктрл *Ктрл::дайВладельца()
{
	ЗамкниГип __;
	if(!открыт()) return NULL;
	int q = Xwindow().найди(GetWindow());
	return q >= 0 ? Xwindow()[q].owner : NULL;
}

void Ктрл::WndShow(bool b)
{
	ЗамкниГип __;
	LLOG("WndShow " << b);
	if(top) {
		XWindowAttributes xwa;
		XGetWindowAttributes(Xdisplay, top->window, &xwa);
		bool v = xwa.map_state == IsViewable;
		if(b == v) return;
		if(b)
			XMapWindow(Xdisplay, top->window);
		else
			XUnmapWindow(Xdisplay, top->window);
		visible = b;
		StateH(SHOW);
	}
}

void Ктрл::обновиОкно()
{
	ЗамкниГип __;
	LTIMING("обновиОкно");
	LLOG("WNDUPDATE");
	if(!top) return;
	SyncExpose();
	if(!top) return;
	XWindow& xw = Xwindow().дай(top->window);
	if(xw.exposed && xw.invalid.дайСчёт()) {
		синхПромот();
		DoPaint(xw.invalid);
		xw.invalid.очисть();
		LTIMING("обновиОкно XSync");
		XSync(Xdisplay, false);
	}
}

void Ктрл::обновиОкно(const Прям& r)
{
	ЗамкниГип __;
	LTIMING("обновиОкно Прям");
	LLOG("WNDUPDATE " << r);
	if(!top) return;
	SyncExpose();
	XWindow& xw = Xwindow().дай(top->window);
	bool dummy;
	синхПромот();
	DoPaint(пересек(xw.invalid, r, dummy));
	LTIMING("обновиОкно XSync");
	XSync(Xdisplay, false);
	xw.invalid = Subtract(xw.invalid, r, dummy);
}

void Ктрл::WndSetPos(const Прям& r)
{
	ЗамкниГип __;
	if(!top) return;
	LLOG("WndSetPos0 " << Имя() << r);
	AddGlobalRepaint();
	XMoveResizeWindow(Xdisplay, top->window, r.left, r.top, r.устШирину(), r.устВысоту());
	rect = r;
	SetWndRect(r);
}

bool Ктрл::IsWndOpen() const
{
	return top;
}

bool Ктрл::SetWndCapture()
{
	ЗамкниГип __;
	if(!включен_ли() || !top || !видим_ли()) return false;
	if(top->window == grabWindow) return true;
	int status = XGrabPointer(
		Xdisplay, top->window, false,
		ButtonPressMask|ButtonReleaseMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask,
		GrabModeAsync, GrabModeAsync, None, None, CurrentTime
	);
	if(status) return false;
#ifdef _ОТЛАДКА
		__X11_Grabbing = true;
#endif
	LLOG("Capture set ok");
	grabWindow = top->window;
	return true;
}

bool Ктрл::HasWndCapture() const
{
	ЗамкниГип __;
	return top && top->window == grabWindow;
}

void Ктрл::ReleaseGrab()
{
	ЗамкниГип __;
	if(grabWindow) {
		LLOG("RELEASE GRAB");
		XUngrabPointer(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
		grabWindow = None;
#ifdef _ОТЛАДКА
		__X11_Grabbing = false;
#endif
	}
}

bool Ктрл::ReleaseWndCapture()
{
	ЗамкниГип __;
	LLOG("Releasing capture");
	if(top && top->window == grabWindow) {
		LLOG("Ungrab3");
		ReleaseGrab();
		return true;
	}
	return false;
}

void Ктрл::устКурсорМыши(const Рисунок& image)
{
	ЗамкниГип __;
	static Рисунок img;
	static Cursor shc;
	if(img.GetSerialId() != image.GetSerialId()) {
		img = image;
		Cursor hc = (Cursor)CursorX11(пусто_ли(img) ? Рисунок::Arrow() : img);
		for(int i = 0; i < Xwindow().дайСчёт(); i++) {
			Window wnd = Xwindow().дайКлюч(i);
			if(wnd)
				XDefineCursor(Xdisplay, wnd, hc);
		}
		if(shc)
			XFreeCursor(Xdisplay, shc);
		shc = hc;
		XFlush(Xdisplay);
	}
}

void ClearKbdState_();

void Ктрл::TakeFocus()
{
	LLOG("TAKE_FOCUS0 " << Имя());
	ЗамкниГип __;
	if(отпрыск_ли()) {
		Ктрл *w = дайТопКтрл();
		if(w) w->TakeFocus();
		return;
	}
	XWindow *w = GetXWindow();
	if(!w)
		return;
	if(ignoretakefocus) {
		LLOG("IGNORED TAKE_FOCUS (caused by CreateWindow)");
		if(focusWindow != top->window && focusWindow != None)
			XSetInputFocus(Xdisplay, focusWindow, RevertToParent, CurrentTime);
		return;
	}
	if(w->owner) {
		LLOG("IGNORED TAKE_FOCUS (window is owned)");
		return;
	}
	LLOG("TAKE_FOCUS " << Имя());
	if(включен_ли() && видим_ли() && top->window != GetXServerFocusWindow()) {
		ClearKbdState_();
		устФокусОкна();
	}
	if(this != focusCtrlWnd) {
		if(включен_ли()) {
			SetLastActive(w, this);
			LLOG("активируй " << Имя());
			SetFocusWnd();
		}
		else {
			LLOG("активируй of disabled window" << Имя());
			if(w->last_active && w->last_active->открыт() && w->last_active->включен_ли() &&
			   w->last_active != this) {
				LLOG("    activating last active: " << РНЦП::Имя(w->last_active));
				w->last_active->TakeFocus();
			}
		}
	}

	return;
}

void Ктрл::KillFocus(Window window)
{
	ЗамкниГип __;
	if(!window)
		return;
	int q = Xwindow().найди(window);
	if(q < 0)
		return;
	XWindow& w = Xwindow()[q];
	if(w.ctrl)
		w.ctrl->KillFocusWnd();
}

bool Ктрл::устФокусОкна()
{
	ЗамкниГип __;
	LLOG("устФокусОкна " << Имя());
	if(top && top->window != focusWindow && включен_ли() && видим_ли()) {
		LLOG("Setting focus... ");
		LTIMING("XSetInfputFocus");
		Ук<Ктрл> _this = this;
		KillFocus(focusWindow);
		if(_this && top) {
			XSetInputFocus(Xdisplay, top->window, RevertToParent, CurrentTime);
			focusWindow = top->window;
			SetFocusWnd();
		}
		return true;
	}
	return false;
}

bool Ктрл::естьФокусОкна() const
{
	ЗамкниГип __;
	return top && top->window == focusWindow;
}

Window Ктрл::GetXServerFocusWindow()
{
	ЗамкниГип __;
	Window w;
	int    dummy;
	XGetInputFocus(Xdisplay, &w, &dummy);
	return w;
}

void Ктрл::FocusSync()
{
	ЗамкниГип __;
	Window fw = GetXServerFocusWindow();
	if(fw != focusWindow) {
		LLOG("FocusSync to " << фмтЦелГекс(fw));
		if(fw) {
			int q = Xwindow().найди(fw);
			if(q >= 0) {
				LLOG("Focus to app window");
				Window fwo = focusWindow;
				focusWindow = fw;
				XWindow& w = Xwindow()[q];
				if(w.ctrl) {
					if(w.ctrl->IsPopUp()) {
						анимируйКаретку();
						return;
					}
					KillFocus(fwo);
//					focusWindow = None; // 1010-10-13 cxl (устФокусОкна does not set this)
					w.ctrl->SetFocusWnd();
					анимируйКаретку();
					return;
				}
			}
		}
		KillFocus(focusWindow);
		focusWindow = None;
		анимируйКаретку();
	}
}

void  Ктрл::SyncIMPosition()
{
/*
	if(!focusWindow)
		return;
	int q = Xwindow().найди(focusWindow);
	if(q < 0)
		return;
	XWindow& xw = Xwindow()[q];
	XIC xic = xw.xic;
	if(xw.xic && xw.ctrl) {
		XVaNestedList   preedit_attr;
		XPoint          spot;
		spot.x = xw.ctrl->caretx + xw.ctrl->caretcx;
		spot.y = xw.ctrl->carety + xw.ctrl->caretcy;
		preedit_attr = XVaCreateNestedList(0, XNSpotLocation, &spot, NULL);
		XSetICValues(xw.xic, XNPreeditAttributes, preedit_attr, NULL);
		XFree(preedit_attr);
	}
*/
}

void  Ктрл::анимируйКаретку()
{
	ЗамкниГип __;
	int v = !(((msecs() - WndCaretTime) / 500) & 1);
	if(v != WndCaretVisible) {
		RefreshCaret();
		WndCaretVisible = v;
	}
}

void Ктрл::инивалидируй(XWindow& xw, const Прям& _r)
{
	ЗамкниГип __;
	LTIMING("инивалидируй");
	LLOG("инивалидируй " << РНЦП::Имя(xw.ctrl) << " " << _r);
	AddRefreshRect(xw.invalid, _r);
}

void Ктрл::AddGlobalRepaint()
{
	ЗамкниГип __;
	Прям rect = дайПрям();
	rect.инфлируй(32, 32);//TODO !!! Not correct !!! should read frame extent instead... - or shift GraphicsExposes /  NoExposes...
	МассивМап<Window, Ктрл::XWindow>& w = Xwindow();
	for(int i = 0; i < w.дайСчёт(); i++)
		if(w.дайКлюч(i) && w[i].ctrl) {
			const Прям& r = w[i].ctrl->rect;
			if(rect.пересекается(r))
				инивалидируй(w[i], Прям(rect.смещенец(-r.верхЛево())));
		}
}

void Ктрл::WndInvalidateRect(const Прям& r)
{
	ЗамкниГип __;
	if(!top) return;
	LLOG("WndInvalidateRect0 " << r);
	инивалидируй(Xwindow().дай(top->window), r);
}

void Ктрл::устППОкна()
{
	ЗамкниГип __;
	LLOG("устППОкна " << Имя());
	if(!top || !видим_ли()) return;
	if(!включен_ли()) {
		LLOG("Not enabled");
		XWindow *w = GetXWindow();
		if(w && w->last_active && w->last_active != this &&
		   w->last_active->открыт() && w->last_active->включен_ли())
			w->last_active->устППОкна();
	}
	else {
		Ук<Ктрл> _this = this;
		устФокусОкна();
		if(_this && top)
			XRaiseWindow(Xdisplay, top->window);
	}
}

bool Ктрл::ппОкна_ли() const
{
	ЗамкниГип __;
	const Ктрл *q = дайТопОкно();
	return ~focusCtrlWnd == (q ? q : дайТопКтрл());
}

void Ктрл::WndEnable(bool b)
{
	ЗамкниГип __;
	LLOG("WndEnable");
	if(!top) return;
	if(!b) {
		ReleaseCapture();
		if(естьФокусОкна())
			XSetInputFocus(Xdisplay, None, RevertToPointerRoot, CurrentTime);
	}
}

// 01/12/2007 - mdelfede
// added support for windowed controls
Ктрл::XWindow *Ктрл::AddXWindow(Window &w)
{
	ЗамкниГип __;
	int i = Xwindow().найди(None);
	if(i >= 0)
		Xwindow().устКлюч(i, w);
	XWindow& cw = i >= 0 ? Xwindow()[i] : Xwindow().добавь(w);
	cw.ctrl    = this;
	cw.exposed = true;
	cw.owner   = дайРодителя();
	cw.xic     = NULL;
	return &cw;
}

void Ктрл::RemoveXWindow(Window &w)
{
	ЗамкниГип __;
	int i = Xwindow().найди(w);
	if(i >= 0) {
		Xwindow().устКлюч(i, None);
		Xwindow()[i].ctrl = NULL;
	}

}
Ктрл::XWindow *Ктрл::XWindowFromWindow(Window &w)
{
	ЗамкниГип __;
	int i = Xwindow().найди(w);
	if(i >= 0)
		return &Xwindow()[i];
	else
		return NULL;
}

// Synchronizes the native windows inside ctrls
void Ктрл::SyncNativeWindows(void)
{
	ЗамкниГип __;
	МассивМап<Window, Ктрл::XWindow>& xwindows = Xwindow();
	for(int i = 0; i < xwindows.дайСчёт(); i++)
	{
		XWindow &xw = xwindows[i];
		Window w = xwindows.дайКлюч(i);
		if(xw.ctrl && xw.ctrl->parent && w)
		{
			Window dummy;
			int x, y;
			unsigned int width, height, border, depth;
			XGetGeometry(Xdisplay, w, &dummy, &x, &y, &width, &height, &border, &depth);
			Прям r = xw.ctrl->GetRectInParentWindow();
			if( (x != r.left || y != r.top) && ((int)width == r.устШирину() && (int)height == r.устВысоту()))
				XMoveWindow(Xdisplay, w, r.left, r.top);
			else if( (x == r.left && y == r.top) && ((int)width != r.устШирину() || (int)height != r.устВысоту()))
				XResizeWindow(Xdisplay, w, r.устШирину(), r.устВысоту());
			else if( x != r.left || y != r.top || (int)width != r.устШирину() || (int)height != r.устВысоту())
				XMoveResizeWindow(Xdisplay, w, r.left, r.top, r.устШирину(), r.устВысоту());
		}
	}

} // END Ктрл::SyncNativeWindows()

// 01/12/2007 - END

TopFrameDraw::TopFrameDraw(Ктрл *ctrl, const Прям& r)
{
	войдиВСтопорГип();
	Ктрл *top = ctrl->дайТопКтрл();
	Вектор<Прям> clip;
	clip.добавь(r);
	dw = top->GetWindow();
	gc = XCreateGC(Xdisplay, dw, 0, 0);
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw,
	                        DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
	иниц(clip, r.верхЛево());
}

TopFrameDraw::~TopFrameDraw()
{
	XFreeGC(Xdisplay, gc);
	покиньСтопорГип();
}

}

#endif
