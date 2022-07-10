#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x)  // DLOG(x)

Прям Ктрл::frameMargins;

Прям Ктрл::GetFrameMargins()
{
	ЗамкниГип __;
	return frameMargins != Прям(0, 0, 0, 0) ? frameMargins : Прям(8, 32, 8, 8);
}

void    ТопОкно::SyncSizeHints()
{
	ЗамкниГип __;
	if(!top)
		return;
	GdkGeometry m;
	Размер sz0 = дайПрям().дайРазм();
	LLOG("SyncSizeHints sz0: " << sz0 << ", sizeable: " << sizeable << ", min: " << дайМинРазм() << ", max: " << дайМаксРазм());
	Размер sz = sz0;
	if(sizeable)
		sz = дайМинРазм();
	m.min_width = LSC(sz.cx);
	m.min_height = LSC(sz.cy);
	sz = sz0;
	if(sizeable)
		sz = дайМаксРазм();
	m.max_width = LSC(sz.cx);
	m.max_height = LSC(sz.cy);
	gtk_window_set_resizable(gtk(), sizeable);
	gtk_window_set_geometry_hints(gtk(), top->window, &m,
	                              GdkWindowHints(GDK_HINT_MIN_SIZE|GDK_HINT_MAX_SIZE));
	gtk_widget_set_size_request(top->window, m.min_width, m.min_height);
}

void ТопОкно::SyncTitle()
{
	ЗамкниГип __;
	if(top)
		gtk_window_set_title(gtk(), изЮникода(title, CHARSET_UTF8));
}

void ТопОкно::SyncCaption()
{
	ЗамкниГип __;
	SyncTitle();
	if(top) {
		GList *icons = NULL;
		if(gdk_icon.уст(icon))
			icons = g_list_append(icons, gdk_icon);
		if(gdk_largeicon.уст(largeicon))
			icons = g_list_append(icons, gdk_largeicon);
		if(icons != NULL) {
			gtk_window_set_icon_list(gtk(), icons);
			g_list_free(icons);
		}
		gtk_window_set_decorated(gtk(), !frameless);
		gtk_window_set_urgency_hint(gtk(), urgent);
	}
}

void ТопОкно::центрПрям(Ктрл *owner)
{
	ЗамкниГип __;
	SetupRect(owner);
	if(owner && center == 1 || center == 2) {
		Размер sz = дайПрям().размер();
		Прям r, wr;
		wr = Ктрл::GetWorkArea();
		Прям fm = GetFrameMargins();
		if(center == 1)
			r = owner->дайПрям();
		else
			r = wr;
		Точка p = r.позЦентра(sz);
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

gboolean ТопОкно::StateСобытие(GtkWidget *widget, GdkСобытиеWindowState *event, gpointer user_data)
{
	ТопОкно *w = (ТопОкно *)user_data;
	dword h = event->new_window_state;
	if(h & GDK_WINDOW_STATE_FULLSCREEN)
		w->state = FULLSCREEN;
	else
	if(h & GDK_WINDOW_STATE_ICONIFIED)
		w->state = MINIMIZED;
	else
	if(h & GDK_WINDOW_STATE_MAXIMIZED)
		w->state = MAXIMIZED;
	else {
		w->state = OVERLAPPED;
		w->overlapped = w->дайПрям();
	}
	w->topmost = h & GDK_WINDOW_STATE_ABOVE;
	return FALSE;
}

void ТопОкно::открой(Ктрл *owner)
{
	ЗамкниГип __;
	LLOG("OPEN " << Имя() << " owner: " << РНЦП::Имя(owner));
	if(dokeys && (!GUI_AKD_Conservative() || GetAccessKeysDeep() <= 1))
		DistributeAccessKeys();
	if(fullscreen)
		устПрям(GetPrimaryScreenArea());
	else
		центрПрям(owner);
	IgnoreMouseUp();
	создай(owner, false);
	g_signal_connect(top->window, "window-state-event", G_CALLBACK(StateСобытие), this);
	SyncSizeHints();
	SyncCaption();
	PlaceFocus();
	int q = state;
	state = OVERLAPPED;
	устРежим(q);
	SyncTopMost();
	GdkRectangle fr;
	gdk_window_get_frame_extents(gdk(), &fr);
	Прям r = дайПрям();
	frameMargins.left = max(frameMargins.left, minmax(r.left - SCL(fr.x), 0, 32));
	frameMargins.right = max(frameMargins.right, minmax(SCL(fr.x + fr.width) - r.right, 0, 32));
	frameMargins.top = max(frameMargins.top, minmax(r.top - SCL(fr.y), 0, 64));
	frameMargins.bottom = max(frameMargins.bottom, minmax(SCL(fr.y + fr.height) - r.bottom, 0, 48));
}

void ТопОкно::открой()
{
	открой(GetActiveWindow());
}

void ТопОкно::откройГлавн()
{
	открой(NULL);
}

void ТопОкно::SyncTopMost()
{
	ЗамкниГип __;
	if(top)
		gtk_window_set_keep_above(gtk(), topmost);
}

void ТопОкно::устРежим(int mode)
{
	ЗамкниГип __;
	GtkWindow *w = gtk();
	if(w)
		switch(state) {
		case MINIMIZED:
			gtk_window_deiconify(w);
			break;
		case MAXIMIZED:
			gtk_window_unmaximize(w);
			break;
		case FULLSCREEN:
			gtk_window_unfullscreen(w);
			break;
		}
	state = mode;
	if(w)
		switch(state) {
		case MINIMIZED:
			gtk_window_iconify(w);
			break;
		case MAXIMIZED:
			gtk_window_maximize(w);
			break;
		case FULLSCREEN:
			gtk_window_fullscreen(w);
			break;
		}
	fullscreen = state == FULLSCREEN;
}

void ТопОкно::сверни(bool effect)
{
	устРежим(MINIMIZED);
}

ТопОкно& ТопОкно::полноэкранно(bool b)
{
	устРежим(b ? (int)FULLSCREEN : (int)OVERLAPPED);
	return *this;
}

void ТопОкно::разверни(bool effect)
{
	устРежим(MAXIMIZED);
}

void ТопОкно::нахлёст(bool effect)
{
	устРежим(OVERLAPPED);
}

ТопОкно& ТопОкно::наиверхнее(bool b, bool)
{
	ЗамкниГип __;
	topmost = b;
	SyncTopMost();
	return *this;
}

bool ТопОкно::наиверхнее_ли() const
{
	ЗамкниГип __;
	return topmost;
}

void ТопОкно::GuiPlatformConstruct()
{
	topmost = false;
}

void ТопОкно::GuiPlatformDestruct()
{
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
		Прям fm = GetFrameMargins();
		limit.left += fm.left;
		limit.right -= fm.right;
		limit.top += fm.top;
		limit.bottom -= fm.bottom;
		Размер sz = min(rect.размер(), limit.размер());
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
