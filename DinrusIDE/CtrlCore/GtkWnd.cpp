#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x)   // DLOG(x)

Вектор<Событие<> >  Ктрл::hotkey;
Вектор<dword>     Ктрл::keyhot;
Вектор<dword>     Ктрл::modhot;

Вектор<Ктрл::Win> Ктрл::wins;

Ук<Ктрл>         Ктрл::activeCtrl;

int               Ктрл::WndCaretTime;
bool              Ктрл::WndCaretVisible;

bool              Ктрл::invalids;

int Ктрл::найдиИд(int id)
{
	for(int i = 0; i < wins.дайСчёт(); i++)
		if(wins[i].id == id)
			return i;
	return -1;
}

int Ктрл::FindCtrl(Ктрл *ctrl)
{
	for(int i = 0; i < wins.дайСчёт(); i++)
		if(wins[i].ctrl == ctrl)
			return i;
	return -1;
}

int Ктрл::FindGtkWindow(GtkWidget *gtk)
{
	for(int i = 0; i < wins.дайСчёт(); i++)
		if(wins[i].gtk == gtk)
			return i;
	return -1;
}

int Ктрл::FindGdkWindow(GdkWindow *gdk)
{
	for(int i = 0; i < wins.дайСчёт(); i++)
		if(wins[i].gdk == gdk)
			return i;
	return -1;
}

bool Ктрл::IsAlphaSupported()
{
	return false;
}

bool Ктрл::IsCompositedGui()
{
	return true; // limits some GUI effects that do not play well with advanced desktops
}

Вектор<Ктрл *> Ктрл::дайТопКтрлы()
{
	ЗамкниГип __;
	Вектор<Ктрл *> h;
	for(int i = 0; i < wins.дайСчёт(); i++)
		h.добавь(wins[i].ctrl);
	return h;
}

cairo_surface_t *CreateCairoSurface(const Рисунок& img);

void  Ктрл::устКурсорМыши(const Рисунок& image)
{
	LLOG("устКурсорМыши");
	ЗамкниГип __;
	int64 id = image.GetSerialId();
	Ктрл *topctrl = NULL;
	Верх *top = NULL;
	if(mouseCtrl)
		topctrl = mouseCtrl->дайТопКтрл();
	else
		topctrl = дайАктивныйКтрл();
	if(topctrl)
		top = topctrl->top;
	if(top && id != top->cursor_id) {
		top->cursor_id = id;
		int64 aux = image.GetAuxData();
		GdkCursor *c = NULL;
		if(aux)
			c = gdk_cursor_new_for_display(gdk_display_get_default(), (GdkCursorType)(aux - 1));
		else
		if(пусто_ли(image))
			c = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_BLANK_CURSOR);
		else {
			Точка p = image.GetHotSpot();

#if GTK_CHECK_VERSION(3, 10, 0)
			cairo_surface_t *surface = CreateCairoSurface(image);
			double scale = SCL(1);
			cairo_surface_set_device_scale(surface, scale, scale);
			c = gdk_cursor_new_from_surface(gdk_display_get_default(), surface, p.x / scale, p.y / scale);
			cairo_surface_destroy(surface);
#else
			ImageGdk m;
			m.уст(image);
			GdkPixbuf *pb = m;
			if(pb)
				c = gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pb, p.x, p.y);
#endif
		}
		if(c && topctrl->открыт()) {
			gdk_window_set_cursor(topctrl->gdk(), c);
			g_object_unref(c);
			gdk_display_flush(gdk_display_get_default()); // сделай it visible immediately
		}
	}
}

Ктрл *Ктрл::дайВладельца()
{
	ЗамкниГип __;
	return открыт() ? top->owner : NULL;
}

Ктрл *Ктрл::дайАктивныйКтрл()
{
	ЗамкниГип __;
	if(focusCtrl)
		return focusCtrl->дайТопКтрл();
	return activeCtrl;
}

// Вектор<Событие<> > Ктрл::hotkey;

#ifndef GDK_WINDOWING_X11

// There is no generic support in GTK for горячаяКлав

int Ктрл::RegisterSystemHotKey(dword ключ, Функция<void ()> cb)
{
	return -1;
}

void Ктрл::UnregisterSystemHotKey(int id)
{
}

#endif

void  Ктрл::анимируйКаретку()
{
	ЗамкниГип __;
	int v = !(((msecs() - WndCaretTime) / 500) & 1);
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
	LLOG("устКаретку " << Имя());
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

Прям Ктрл::GetWndScreenRect() const
{
	ЗамкниГип __;
	if(открыт()) {
		gint x, y;
		gdk_window_get_position(gdk(), &x, &y);
		gint width = gdk_window_get_width(gdk());
		gint height = gdk_window_get_height(gdk());
		return SCL(x, y, width, height);
	}
	return Null;
}

void Ктрл::WndShow(bool b)
{
	ЗамкниГип __;
	LLOG("WndShow " << Имя() << ", " << b);
	if(открыт()) {
		if(b)
			gtk_widget_show_now(top->window);
		else
			gtk_widget_hide(top->window);
		StateH(SHOW);
	}
}

bool Ктрл::IsWndOpen() const {
	ЗамкниГип __;
	return top && top->window && gtk_widget_get_window(top->window);
}

void Ктрл::SetAlpha(byte alpha)
{
	ЗамкниГип __;
}

Прям Ктрл::GetWorkArea() const
{
	ЗамкниГип __;
	static Массив<Прям> rc;
	if(rc.пустой())
		GetWorkArea(rc);
	
	Точка pt = дайПрямЭкрана().верхЛево();
	for (int i = 0; i < rc.дайСчёт(); i++)
		if(rc[i].содержит(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

void Ктрл::GetWorkArea(Массив<Прям>& rc)
{
	ЗамкниГип __;
#if GTK_CHECK_VERSION(3, 22, 0)
	GdkDisplay *s = gdk_display_get_default();
	int n = gdk_display_get_n_monitors(s);
	rc.очисть();
	Вектор<int> netwa;
	for(int i = 0; i < n; i++) {
		GdkRectangle rr;
		gdk_monitor_get_workarea(gdk_display_get_monitor(s, i), &rr);
		rc.добавь(SCL(rr.x, rr.y, rr.width, rr.height));
	}
#else
	GdkScreen *s = gdk_screen_get_default();
	int n = gdk_screen_get_n_monitors(s);
	rc.очисть();
	Вектор<int> netwa;
	for(int i = 0; i < n; i++) {
		GdkRectangle rr;
		Прям r;
		gdk_screen_get_monitor_workarea(s, i, &rr);
		r = RectC(rr.x, rr.y, rr.width, rr.height);
		rc.добавь(r);
	}
#endif
}

Прям Ктрл::GetVirtualWorkArea()
{
	ЗамкниГип __;
	static Прям r;
	if(r.right == 0) {
		r = GetPrimaryWorkArea();
		Массив<Прям> rc;
		GetWorkArea(rc);
		for(int i = 0; i < rc.дайСчёт(); i++)
			r |= rc[i];
	}
	return r;
}

Прям Ктрл::GetVirtualScreenArea()
{
	ЗамкниГип __;
	static Прям r;
	if(r.right == 0) {
		gint x, y, width, height;
		gdk_window_get_geometry(gdk_screen_get_root_window(gdk_screen_get_default()),
	                            &x, &y, &width, &height);
	    r = SCL(x, y, width, height);
	}
	return r;
}

Прям Ктрл::GetPrimaryWorkArea()
{
	ЗамкниГип __;
#if GTK_CHECK_VERSION(3, 22, 0)
	GdkRectangle rr;
	gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &rr);
	return SCL(rr.x, rr.y, rr.width, rr.height);
#else
	static Прям r;
	if (r.right == 0) {
		Массив<Прям> rc;
		GetWorkArea(rc);
		int primary = gdk_screen_get_primary_monitor(gdk_screen_get_default());
		primary >= 0 && primary < rc.дайСчёт() ? r = rc[primary] : r = GetVirtualScreenArea();
	}
	return r;
#endif
}

Прям Ктрл::GetPrimaryScreenArea()
{
	return GetPrimaryWorkArea();
}

int Ктрл::GetKbdDelay()
{
	ЗамкниГип __;
	return 500;
}

int Ктрл::GetKbdSpeed()
{
	ЗамкниГип __;
	return 1000 / 32;
}

void Ктрл::устППОкна()
{
	ЗамкниГип __;
	if(открыт())
		gtk_window_present(gtk());
}

bool Ктрл::ппОкна_ли() const
{
	ЗамкниГип __;
	LLOG("ппОкна_ли");
	return открыт() && gtk_window_is_active(gtk());
}

bool Ктрл::естьФокусОкна() const
{
	ЗамкниГип __;
	return открыт() && gtk_window_is_active(gtk());
}

void Ктрл::FocusSync()
{
	ЗамкниГип __;
	static Ук<Ктрл> ctrl;
	if(focusCtrlWnd && focusCtrlWnd->открыт() && gtk_window_is_active(focusCtrlWnd->gtk())) {
		ctrl = focusCtrlWnd;
		return;
	}
	Ук<Ктрл> focus = NULL;
	for(int i = 0; i < wins.дайСчёт(); i++)
		if(gtk_window_is_active((GtkWindow *)wins[i].gtk)) {
			focus = wins[i].ctrl;
			break;
		}
	if(focus != ctrl) {
		if(ctrl)
			ctrl->KillFocusWnd();
		ctrl = focus;
		if(ctrl)
			ctrl->SetFocusWnd();
		SyncCaret();
	}
}

bool Ктрл::устФокусОкна()
{
	ЗамкниГип __;
	LLOG("SetWndFocus0 " << РНЦП::Имя(this) << ", top: " << top);
	if(top) {
		LLOG("SetWndFocus0 ДЕЛАЙ gdk: " << gdk());
		устППОкна();
		int t0 = msecs();
		while(!gtk_window_is_active(gtk()) && msecs() - t0 < 500) // жди up to 500ms for window to become active - not ideal, but only possibility
			FetchСобытиеs(true);
		FocusSync();
	}
	return true;
}

void Ктрл::DoCancelPreedit()
{
	if(!focusCtrl)
		return;
	if(focusCtrl->top)
		focusCtrl->HidePreedit();
	if(focusCtrl->top) {
		gtk_im_context_reset(focusCtrl->top->im_context);
		gtk_im_context_focus_out(focusCtrl->top->im_context);
		gtk_im_context_focus_in(focusCtrl->top->im_context);
	}
}

void WakeUpGuiThread();

void Ктрл::WndInvalidateRect(const Прям& r)
{
	ЗамкниГип __;
	LLOG("WndInvalidateRect " << r);
	Прям rr;
	if(scale > 1) {
		rr.left = r.left / 2;
		rr.top = r.top / 2;
		rr.right = (r.right + 1) / 2;
		rr.bottom = (r.bottom + 1) / 2;
	}
	else
		rr = r;

	// as gtk3 dropped thread locking, we need to push invalid rectangles onto main loop
	for(Win& win : wins) {
		if(win.ctrl == this) {
			if(win.invalid.дайСчёт() && win.invalid[0].right > 99999 && win.invalid[0].bottom > 99999)
				return;
			if(win.invalid.дайСчёт() > 200) { // keep things sane
				win.invalid.очисть();
				win.invalid.добавь(Прям(0, 0, 100000, 100000));
			}
			else
				win.invalid.добавь(rr);
			if(!invalids) {
				invalids = true;
				WakeUpGuiThread();
			}
		}
	}
}

void  Ктрл::WndScrollView(const Прям& r, int dx, int dy)
{
	ЗамкниГип __;
	LLOG("промотайОбзор " << rect);
	WndInvalidateRect(r);
}

bool Ктрл::SweepConfigure(bool wait)
{
	Ук<Ктрл> this_ = this;
	bool r = false;
	FetchСобытиеs(wait);
	for(int i = 0; i < Событиеs.дайСчёт() && this_; i++) {
		GСобытие& e = Событиеs[i];
		if(e.тип == GDK_CONFIGURE && this_ && top->id == e.windowid) {
			Прям rect = e.значение;
			LLOG("SweepConfigure " << rect);
			if(дайПрям() != rect)
				SetWndRect(rect);
			r = true;
			e.тип = EVENT_NONE;
		}
	}
	return r;
}

void Ктрл::WndSetPos(const Прям& rect)
{
	LLOG("WndSetPos " << РНЦП::Имя(this) << " " << rect);
	ЗамкниГип __;
	if(!открыт())
		return;
	Ук<Ктрл> this_ = this;
	SweepConfigure(false); // удали any previous GDK_CONFIGURE for this window
	if(!this_ || !открыт())
		return;

	Прям m(0, 0, 0, 0);
	if(dynamic_cast<ТопОкно *>(this))
		m = GetFrameMargins();
	SetWndRect(rect);
	if(ТопОкно *tw = dynamic_cast<ТопОкно *>(this))
		tw->SyncSizeHints();
	gdk_window_move_resize(gdk(), LSC(rect.left - m.left), LSC(rect.top - m.top), LSC(rect.дайШирину()), LSC(rect.дайВысоту()));
	LLOG("-- WndSetPos0 " << rect);
}

void Ктрл::WndEnable(bool b)
{
	ЗамкниГип __;
	if(открыт()) {
		gtk_widget_set_sensitive(top->window, b);
		StateH(ENABLE);
	}
}

void Ктрл::обновиОкно(const Прям& r)
{
	ЗамкниГип __;
	LLOG("WndUpdate0r " << r);
	обновиОкно(); // Not found a way how to update only part of window
}

void Ктрл::обновиОкно()
{
	ЗамкниГип __;
	LLOG("WndUpdate0");
//	gdk_window_process_updates(gdk(), TRUE); // deprecated
	FetchСобытиеs(FALSE); // Should pickup GDK_EXPOSE and repaint the window
	gdk_display_flush(gdk_display_get_default());
}

Прям Ктрл::GetDefaultWindowRect()
{
	ЗамкниГип __;
	Прям r  = GetPrimaryWorkArea();
	Размер sz = r.дайРазм();
	
	static int pos = min(sz.cx / 10, 50);
	pos += 10;
	int cx = sz.cx * 2 / 3;
	int cy = sz.cy * 2 / 3;
	if(pos + cx + 50 > sz.cx || pos + cy + 50 > sz.cy)
		pos = 0;
	return RectC(r.left + pos + 20, r.top + pos + 20, cx, cy);
}

TopFrameDraw::TopFrameDraw(Ктрл *ctrl, const Прям& r)
{
	войдиВСтопорГип();
	Ктрл *top = ctrl->дайТопКтрл();
#if GTK_CHECK_VERSION(3, 22, 0)
	cairo_rectangle_int_t rr;
	rr.x = Ктрл::LSC(r.left);
	rr.y = Ктрл::LSC(r.top);
	rr.width = Ктрл::LSC(r.дайШирину());
	rr.height = Ктрл::LSC(r.дайВысоту());
	cairo_region_t *rg = cairo_region_create_rectangle(&rr);
	ctx = gdk_window_begin_draw_frame(top->gdk(), rg);
	cairo_region_destroy(rg);
	cr = gdk_drawing_context_get_cairo_context(ctx);
#else
	cr = gdk_cairo_create(top->gdk());
#endif
	cairo_scale(cr, Ктрл::LSC(1), Ктрл::LSC(1));
	Clipoff(r);
}

TopFrameDraw::~TopFrameDraw()
{
	FlushText();
#if GTK_CHECK_VERSION(3, 22, 0)
	gdk_window_end_draw_frame(gdk_drawing_context_get_window(ctx), ctx);
#else
	cairo_destroy(cr);
#endif
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
