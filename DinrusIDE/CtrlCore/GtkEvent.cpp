#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x)    // DLOG(x)
// #define LOG_EVENTS  _DBG_

БиВектор<Ктрл::GСобытие> Ктрл::Событиеs;

Точка         Ктрл::CurrentMousePos;
guint         Ктрл::CurrentState;
guint32       Ктрл::CurrentTime;
Ктрл::GСобытие  Ктрл::CurrentСобытие;
guint         Ктрл::MouseState;

bool  дайШифт() { return Ктрл::CurrentState & GDK_SHIFT_MASK; }
bool  дайКтрл() { return Ктрл::CurrentState & GDK_CONTROL_MASK; }
bool  дайАльт() { return Ктрл::CurrentState & GDK_MOD1_MASK; }
bool  дайКапсЛок() { return Ктрл::CurrentState & GDK_LOCK_MASK; }
bool  дайЛевуюМыши() { return Ктрл::CurrentState & GDK_BUTTON1_MASK; }
bool  дайПравуюМыши() { return Ктрл::CurrentState & GDK_BUTTON3_MASK; }
bool  дайСреднююМыши() { return Ктрл::CurrentState & GDK_BUTTON2_MASK; }
Точка дайПозМыши() { return Ктрл::CurrentMousePos; }

#ifdef LOG_EVENTS
Tuple2<int, const char *> xСобытие[] = {
	{ GDK_NOTHING, "GDK_NOTHING" },
	{ GDK_DELETE, "GDK_DELETE" },
	{ GDK_DESTROY, "GDK_DESTROY" },
	{ GDK_EXPOSE, "GDK_EXPOSE" },
	{ GDK_MOTION_NOTIFY, "GDK_MOTION_NOTIFY" },
	{ GDK_BUTTON_PRESS, "GDK_BUTTON_PRESS" },
	{ GDK_2BUTTON_PRESS, "GDK_2BUTTON_PRESS" },
	{ GDK_3BUTTON_PRESS, "GDK_3BUTTON_PRESS" },
	{ GDK_BUTTON_RELEASE, "GDK_BUTTON_RELEASE" },
	{ GDK_KEY_PRESS, "GDK_KEY_PRESS" },
	{ GDK_KEY_RELEASE, "GDK_KEY_RELEASE" },
	{ GDK_ENTER_NOTIFY, "GDK_ENTER_NOTIFY" },
	{ GDK_LEAVE_NOTIFY, "GDK_LEAVE_NOTIFY" },
	{ GDK_FOCUS_CHANGE, "GDK_FOCUS_CHANGE" },
	{ GDK_CONFIGURE, "GDK_CONFIGURE" },
	{ GDK_MAP, "GDK_MAP" },
	{ GDK_UNMAP, "GDK_UNMAP" },
	{ GDK_PROPERTY_NOTIFY, "GDK_PROPERTY_NOTIFY" },
	{ GDK_SELECTION_CLEAR, "GDK_SELECTION_CLEAR" },
	{ GDK_SELECTION_REQUEST, "GDK_SELECTION_REQUEST" },
	{ GDK_SELECTION_NOTIFY, "GDK_SELECTION_NOTIFY" },
	{ GDK_PROXIMITY_IN, "GDK_PROXIMITY_IN" },
	{ GDK_PROXIMITY_OUT, "GDK_PROXIMITY_OUT" },
	{ GDK_DRAG_ENTER, "GDK_DRAG_ENTER" },
	{ GDK_DRAG_LEAVE, "GDK_DRAG_LEAVE" },
	{ GDK_DRAG_MOTION, "GDK_DRAG_MOTION" },
	{ GDK_DRAG_STATUS, "GDK_DRAG_STATUS" },
	{ GDK_DROP_START, "GDK_DROP_START" },
	{ GDK_DROP_FINISHED, "GDK_DROP_FINISHED" },
	{ GDK_CLIENT_EVENT, "GDK_CLIENT_EVENT" },
	{ GDK_VISIBILITY_NOTIFY, "GDK_VISIBILITY_NOTIFY" },
	{ GDK_SCROLL, "GDK_SCROLL" },
	{ GDK_WINDOW_STATE, "GDK_WINDOW_STATE" },
	{ GDK_SETTING, "GDK_SETTING" },
	{ GDK_OWNER_CHANGE, "GDK_OWNER_CHANGE" },
	{ GDK_GRAB_BROKEN, "GDK_GRAB_BROKEN" },
	{ GDK_DAMAGE, "GDK_DAMAGE" },
	{ GDK_EVENT_LAST, "GDK_EVENT_LAST" },
};
#endif

Ктрл *Ктрл::GetTopCtrlFromId(int id)
{
	int q = найдиИд(id);
	if(q >= 0) {
		Ктрл *p = wins[q].ctrl;
		if(p && p->top)
			return p;
	}
	return NULL;
}

gboolean Ктрл::GtkDraw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	ЗамкниГип __;
	Ктрл *p = GetTopCtrlFromId(user_data);
	if(p) {
		p->fullrefresh = false;
		cairo_scale(cr, 1.0 / scale, 1.0 / scale);
		p->SyncWndRect(p->GetWndScreenRect()); // avoid black areas when resizing

		SystemDraw w(cr);
		painting = true;
		double x1, y1, x2, y2;
		cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
		Прям r = RectC((int)x1, (int)y1, (int)ceil(x2 - x1), (int)ceil(y2 - y1));
		w.Clip(r); // Because of IsPainting

		cairo_rectangle_list_t *list = cairo_copy_clip_rectangle_list(cr);
		if(list->status == CAIRO_STATUS_SUCCESS && list->num_rectangles < 10) {
			Вектор<Прям> clip;
			for(int i = 0; i < list->num_rectangles; i++) {
				const cairo_rectangle_t& r = list->rectangles[i];
				clip.добавь(Прям((int)r.x, (int)r.y, (int)(r.x + r.width), (int)(r.y + r.height)));
			}
			w.PickInvalid(pick(clip));
		}
		cairo_rectangle_list_destroy(list);

		p->UpdateArea(w, r);
		w.стоп();
		painting = false;
	}
	return true;
}

gboolean Ктрл::GtkСобытие(GtkWidget *widget, GdkСобытие *event, gpointer user_data)
{
	ЗамкниГип __;
	GdkСобытиеKey *ключ;
	bool pressed = false;
	bool  retval = true;
	Значение значение;
	Ктрл *p = GetTopCtrlFromId(user_data);
#ifdef LOG_EVENTS
	Ткст ev = "?";
	Tuple2<int, const char *> *f = найдиКортеж(xСобытие, __countof(xСобытие), event->тип);
	if(f)
		ev = f->b;
	LOG(rmsecs() << " FETCH EVENT " << ev << " ctrl: " << Имя(p));
#endif

	switch(event->тип) {
	case GDK_DELETE:
		p->CancelPreedit();
		break;
	case GDK_FOCUS_CHANGE:
		p->CancelPreedit();
		if(p) {
			if(((GdkСобытиеFocus *)event)->in)
				gtk_im_context_focus_in(p->top->im_context);
			else
				gtk_im_context_focus_out(p->top->im_context);
			AddСобытие(user_data, EVENT_FOCUS_CHANGE, значение, event);
		}
		return false;
	case GDK_MOTION_NOTIFY:
	case GDK_LEAVE_NOTIFY:
		break;
	case GDK_BUTTON_PRESS:
		p->CancelPreedit();
		значение = DoButtonСобытие(event, true);
		if(пусто_ли(значение))
			return false;
		break;
	case GDK_2BUTTON_PRESS:
		p->CancelPreedit();
		значение = DoButtonСобытие(event, true);
		if(пусто_ли(значение))
			return false;
		break;
	case GDK_BUTTON_RELEASE:
		p->CancelPreedit();
		значение = DoButtonСобытие(event, false);
		if(пусто_ли(значение))
			return false;
		break;
	case GDK_SCROLL: {
		GdkСобытиеScroll *e = (GdkСобытиеScroll *)event;
		if(findarg(e->direction, GDK_SCROLL_UP, GDK_SCROLL_LEFT) >= 0)
			значение = 120;
		else
		if(findarg(e->direction, GDK_SCROLL_DOWN, GDK_SCROLL_RIGHT) >= 0)
			значение = -120;
		else
			return false;
		break;
	}
	case GDK_KEY_PRESS:
		pressed = true;
	case GDK_KEY_RELEASE:
		ключ = (GdkСобытиеKey *)event;
		значение << (int) ключ->keyval << (int) ключ->hardware_keycode;
		if(pressed) {
			p = GetTopCtrlFromId(user_data);
			if(p && gtk_im_context_filter_keypress(p->top->im_context, ключ))
				return true;
		}
		break;
	case GDK_CONFIGURE: {
		retval = false;
		GdkСобытиеConfigure *e = (GdkСобытиеConfigure *)event;
		значение = SCL(e->x, e->y, e->width, e->height);
		break;
	}
	default:
		return false;
	}
	AddСобытие(user_data, event->тип, значение, event);
	return retval;
}

int Ктрл::DoButtonСобытие(GdkСобытие *event, bool press)
{
	GdkСобытиеButton *e = (GdkСобытиеButton *)event;
	static int mask[] = { GDK_BUTTON1_MASK, GDK_BUTTON2_MASK, GDK_BUTTON3_MASK };
	if(e->button >= 1 && e->button <= 3) {
		int m = mask[e->button - 1];
		if(press)
			MouseState |= m;
		else
			MouseState &= ~m;
		return e->button;
	}
	return findarg(e->button, 8, 9) >= 0 ? (int)e->button : (int)Null;
}

Ктрл::GСобытие::GСобытие()
{
	event = NULL;

	pen = false;
	pen_barrel = false;
	pen_inverted = false;
	pen_eraser = false;
	pen_pressure = Null;
	pen_rotation = Null;
	pen_tilt = Null;
}

void Ктрл::GСобытие::освободи()
{
	if(event) {
		gdk_event_free(event);
		event = NULL;
	}
}

void Ктрл::GСобытие::уст(const GСобытие& e)
{
	*(GСобытие0 *)this = e;
	event = e.event ? gdk_event_copy(e.event) : NULL;
}

Ктрл::GСобытие::~GСобытие()
{
	освободи();
}

Ктрл::GСобытие::GСобытие(const GСобытие& e)
{
	уст(e);
}

void Ктрл::GСобытие::operator=(const GСобытие& e)
{
	if(this == &e)
		return;
	освободи();
	уст(e);
}

static Точка s_mousepos;

Точка Ктрл::GetMouseInfo(GdkWindow *win, GdkModifierType& mod)
{
#if GTK_CHECK_VERSION(3, 20, 0)
	GdkDisplay *дисплей = gdk_window_get_display (win);
	GdkDevice *pointer = gdk_seat_get_pointer (gdk_display_get_default_seat (дисплей));
	double x, y;
	gdk_window_get_device_position_double (win, pointer, &x, &y, &mod);
	return s_mousepos; //return Точка((int)SCL(x), (int)SCL(y));
#else
	gint x, y;
	gdk_window_get_pointer(win, &x, &y, &mod);
	return Точка(SCL(x), SCL(y));
#endif
}

void Ктрл::AddСобытие(gpointer user_data, int тип, const Значение& значение, GdkСобытие *event)
{
	if(Событиеs.дайСчёт() > 50000)
		return;
	GСобытие& e = Событиеs.добавьХвост();
	e.windowid = (uint32)(uintptr_t)user_data;
	e.тип = тип;
	e.значение = значение;
	GdkModifierType mod;
	e.mousepos = GetMouseInfo(gdk_get_default_root_window(), mod);
	if(event && event->тип == GDK_MOTION_NOTIFY){
		GdkСобытиеMotion *mevent = (GdkСобытиеMotion *)event;
		e.mousepos = s_mousepos = Точка(SCL(mevent->x_root), SCL(mevent->y_root));
	}
	e.state = (mod & ~(GDK_BUTTON1_MASK|GDK_BUTTON2_MASK|GDK_BUTTON3_MASK)) | MouseState;
	e.count = 1;
	e.event = NULL;
#if GTK_CHECK_VERSION(3, 22, 0)
	GdkDevice *d = event ? gdk_event_get_source_device(event) : NULL;
	if(d && findarg(gdk_device_get_source(d), GDK_SOURCE_PEN, GDK_SOURCE_TOUCHSCREEN) >= 0) {
		e.pen = true;
		e.pen_barrel = MouseState & GDK_BUTTON3_MASK;
		double *axes = NULL;
		switch(event->тип){
			case GDK_BUTTON_PRESS:
				gdk_window_set_event_compression(((GdkСобытиеButton *)event)->window, false);
			case GDK_2BUTTON_PRESS:
			case GDK_3BUTTON_PRESS:
				axes = ((GdkСобытиеButton *)event)->axes;
				break;
			case GDK_BUTTON_RELEASE:
				gdk_window_set_event_compression(((GdkСобытиеButton *)event)->window, true);
				axes = ((GdkСобытиеButton *)event)->axes;
				break;
			case GDK_MOTION_NOTIFY:{
				GdkСобытиеMotion *mevent = (GdkСобытиеMotion *)event;
				e.mousepos = s_mousepos = Точка(SCL(mevent->x_root), SCL(mevent->y_root));
				axes = ((GdkСобытиеMotion *)event)->axes;
				break;
			}
			default:;
		}
		if(axes) {
			if(!gdk_device_get_axis(d, axes, GDK_AXIS_PRESSURE, &e.pen_pressure)) e.pen_pressure=Null;
			if(!gdk_device_get_axis(d, axes, GDK_AXIS_ROTATION, &e.pen_rotation)) e.pen_rotation=Null;
			if(!gdk_device_get_axis(d, axes, GDK_AXIS_XTILT, &e.pen_tilt.x)) e.pen_tilt.x=Null;
			if(!gdk_device_get_axis(d, axes, GDK_AXIS_YTILT, &e.pen_tilt.y)) e.pen_tilt.y=Null;
		}
	}
#endif
}

void Ктрл::IMCommit(GtkIMContext *context, gchar *str, gpointer user_data)
{
	ЗамкниГип __;
	AddСобытие(user_data, EVENT_TEXT, вУтф32(str), NULL);
}

void Ктрл::IMLocation(Ктрл *w)
{
	if(w && w->HasFocusDeep() && focusCtrl && !пусто_ли(focusCtrl->GetPreedit())) {
		GdkRectangle r;
		Прям e = w->GetPreeditScreenRect();
		Прям q = w->дайПрямЭкрана();
		GdkRectangle gr;
		gr.x = LSC(e.left - q.left);
		gr.y = LSC(e.top - q.top);
		gr.width = LSC(e.дайШирину());
		gr.height = LSC(e.дайВысоту());
		gtk_im_context_set_cursor_location(w->top->im_context, &gr);
	}
}

void Ктрл::IMPreedit(GtkIMContext *context, gpointer user_data)
{
	ЗамкниГип __;
	Ктрл *w = GetTopCtrlFromId((uint32)(uintptr_t)user_data);
	if(w && w->HasFocusDeep() && focusCtrl && !пусто_ли(focusCtrl->GetPreedit())) {
		PangoAttrList *attrs;
		gchar *str;
		gint   cursor_pos;
		gtk_im_context_get_preedit_string(context, &str, &attrs, &cursor_pos);
		ШТкст text = вУтф32(str);
		g_free(str);
		pango_attr_list_unref(attrs);
		w->ShowPreedit(text, cursor_pos);
		IMLocation(w);
	}
	else
		CancelPreedit();
}

void Ктрл::IMPreeditEnd(GtkIMContext *context, gpointer user_data)
{
	ЗамкниГип __;
	Ктрл *w = GetTopCtrlFromId((uint32)(uintptr_t)user_data);
	if(w && w->HasFocusDeep() && focusCtrl && !пусто_ли(focusCtrl->GetPreedit()))
		w->HidePreedit();
}

bool Ктрл::ProcessInvalids()
{
	ЗамкниГип __;
	if(invalids) {
		for(Win& win : wins) {
			for(const Прям& r : win.invalid)
				gdk_window_invalidate_rect(win.gdk, GdkRect(r), TRUE);
			win.invalid.очисть();
		}
		invalids = false;
	}
	return invalids;
}

void Ктрл::FetchСобытиеs(bool may_block)
{
	LLOG("FetchСобытиеs " << may_block);
	int level = покиньВсеСтопорыГип();
	while(g_main_context_iteration(NULL, may_block))
		may_block = false;
	войдиВСтопорГип(level);
}

bool Ктрл::IsWaitingСобытие0(bool fetch)
{
	if(fetch)
		FetchСобытиеs(FALSE);
	return Событиеs.дайСчёт();
}

bool Ктрл::ожидаетСобытие()
{
	ПРОВЕРЬ_(главнаяНить_ли(), "ожидаетСобытие can only run in the main thread");
	return IsWaitingСобытие0(true);
}

struct ProcStop {
	ТаймСтоп tm;
	Ткст   ev;

	~ProcStop() { LOG("* " << ev << " elapsed " << tm); }
};

bool Ктрл::DispatchMouseIn(int act, int zd)
{
	Точка p = дайПозМыши();
	Прям r = дайПрямЭкрана();
	if(r.содержит(p)) {
		p -= r.верхЛево();
		DispatchMouse(act, p, zd);
		return true;
	}
	return false;
}

Рисунок Ктрл::GtkMouseEvent(int action, int act, int zd)
{
	if(grabpopup && activePopup.дайСчёт()) {
		for(int i = visiblePopup.дайСчёт(); --i >= 0;)
			if(visiblePopup[i] && visiblePopup[i]->DispatchMouseIn(act, zd))
				return Null;
		if(action == DOWN) { // откл active popup(s) if clicked outside of active popups
			IgnoreMouseUp();
			if(activePopup.верх())
				activePopup.верх()->дайТопОкно()->активируйОкно();
		}
		else
		if(activePopup[0]) { // Redirect other events to ТопОкно that spawned first popup
			Ук<ТопОкно> w = activePopup[0]->дайТопОкно();
			if(w)
				w->DispatchMouseIn(act, zd);
		}
		return Null;
	}
	return DispatchMouse(act, дайПозМыши() - дайПрямЭкрана().верхЛево(), zd);
}

void Ктрл::GtkButtonСобытие(int action)
{
	int act = action;
	int button = CurrentСобытие.значение;
	if(action != MOUSEMOVE)
		act |= button == 2 ? MIDDLE : button == 3 ? RIGHT : LEFT;
	GtkMouseEvent(action, act, 0);
}

void Ктрл::Proc()
{
#ifdef LOG_EVENTS
	Ткст ev = "?";
	Tuple2<int, const char *> *f = найдиКортеж(xСобытие, __countof(xСобытие), CurrentСобытие.тип);
	if(f)
		ev = f->b;
	LOG("> PROCESS EVENT " << РНЦП::Имя(this) << " " << ev);
	ProcStop tm;
	tm.ev = ev;
#endif
	if(!открыт())
		return;
	Ук<Ктрл> _this = this;
	bool pressed = false;
	int  kv, hw;
	static int clicktime = msecs() - 100000;

#if GTK_CHECK_VERSION(3, 22, 0)
	pen.barrel = CurrentСобытие.pen_barrel;
	pen.inverted = CurrentСобытие.pen_inverted;
	pen.eraser = CurrentСобытие.pen_eraser;
	pen.pressure = CurrentСобытие.pen_pressure;
	pen.rotation = CurrentСобытие.pen_rotation;
	pen.tilt = CurrentСобытие.pen_tilt;

	is_pen_event = CurrentСобытие.pen;

	auto DoPen = [&](Точка p) {
		ЗамкниГип __;
		eventCtrl = this;
		Ктрл *q = this;
		if(captureCtrl){
			q = captureCtrl;
			p += дайПрямЭкрана().верхЛево() - captureCtrl->дайПрямЭкрана().верхЛево();
		}
		else
			for(Ктрл *t = q; t; t=q->отпрыскИзТочки(p)) q = t;

		q->Pen(p, pen, дайФлагиМыши());
		SyncCaret();
		Рисунок m = CursorOverride();
		if(пусто_ли(m)) устКурсорМыши(q->рисКурсора(p,дайФлагиМыши()));
		else устКурсорМыши(m);
	};

	if(is_pen_event &&
	   findarg(CurrentСобытие.тип, GDK_MOTION_NOTIFY, GDK_BUTTON_PRESS, GDK_BUTTON_RELEASE) >= 0)
	{
		pen.action = decode(CurrentСобытие.тип, GDK_BUTTON_PRESS, PEN_DOWN, GDK_BUTTON_RELEASE, PEN_UP, 0);

		DoPen(дайПозМыши() - дайПрямЭкрана().верхЛево());
	}
#endif

	switch(CurrentСобытие.тип) {
	case GDK_MOTION_NOTIFY:
		GtkMouseEvent(MOUSEMOVE, MOUSEMOVE, 0);
		break;
	case GDK_BUTTON_PRESS:
		if(CurrentСобытие.значение == 8) {
			DispatchKey(K_MOUSE_BACKWARD, 1);
			break;
		}
		if(CurrentСобытие.значение == 9) {
			DispatchKey(K_MOUSE_FORWARD, 1);
			break;
		}
		if(!естьФокусОкна() && !popup)
			устФокусОкна();
		ClickActivateWnd();
		if(ignoremouseup) {
			KillRepeat();
			ignoreclick = false;
			ignoremouseup = false;
		}

		if(!ignoreclick) {
			bool dbl = msecs(clicktime) < 250;
			clicktime = dbl ? clicktime - 1000 : msecs();
			GtkButtonСобытие(dbl ? DOUBLE : DOWN);
		}
		break;
/*	case GDK_2BUTTON_PRESS:
		if(!ignoreclick)
			GtkButtonСобытие(DOUBLE);
		break;
*/	case GDK_BUTTON_RELEASE:
		if(CurrentСобытие.значение == 8) {
			DispatchKey(K_MOUSE_BACKWARD|K_KEYUP, 1);
			break;
		}
		if(CurrentСобытие.значение == 9) {
			DispatchKey(K_MOUSE_FORWARD|K_KEYUP, 1);
			break;
		}
		if(ignoreclick)
			EndIgnore();
		else
		if(!dnd_events)
			GtkButtonСобытие(UP);
		break;
	case GDK_SCROLL: {
		GtkMouseEvent(MOUSEWHEEL, MOUSEWHEEL, CurrentСобытие.значение);
		break;
	}
	case GDK_KEY_PRESS:
		pressed = true;
	case GDK_KEY_RELEASE:
		kv = CurrentСобытие.значение[0];
		hw = CurrentСобытие.значение[1];
		if(kv >= 0 && kv < K_CHAR_LIM) {
			LLOG("keyval " << фмтЦелГекс(kv) << ' ' << (char)kv);
			if(kv >= 'a' && kv <= 'z')
				kv = kv - 'a' + 'A';
			static Tuple2<int, int> cv[] = {
				{ GDKEY(BackSpace), K_BACKSPACE },
				{ GDKEY(Вкладка), K_TAB },
				{ GDKEY(ISO_Left_Tab), K_TAB },
				{ GDKEY(Return), K_ENTER },
				{ GDKEY(Escape), K_ESCAPE },
				{ GDKEY(space), K_SPACE },
				{ GDKEY(Control_L), K_CTRL_KEY },
				{ GDKEY(Control_R), K_CTRL_KEY },
				{ GDKEY(Shift_L), K_SHIFT_KEY },
				{ GDKEY(Shift_R), K_SHIFT_KEY },
				{ GDKEY(Alt_L), K_ALT_KEY },
				{ GDKEY(Alt_R), K_ALT_KEY },

				{ GDKEY(KP_Space), K_SPACE },
				{ GDKEY(KP_Tab), K_TAB },
				{ GDKEY(KP_Enter), K_ENTER },
				{ GDKEY(KP_F1), K_F1 },
				{ GDKEY(KP_F2), K_F2 },
				{ GDKEY(KP_F3), K_F3 },
				{ GDKEY(KP_F4), K_F4 },
				{ GDKEY(KP_Home), K_HOME },
				{ GDKEY(KP_Left), K_LEFT },
				{ GDKEY(KP_Up), K_UP },
				{ GDKEY(KP_Right), K_RIGHT },
				{ GDKEY(KP_Down), K_DOWN },
				{ GDKEY(KP_Page_Up), K_PAGEUP },
				{ GDKEY(KP_Page_Down), K_PAGEDOWN },
				{ GDKEY(KP_End), K_END },
				{ GDKEY(KP_Begin), K_HOME },
				{ GDKEY(KP_Insert), K_INSERT },
				{ GDKEY(KP_Delete), K_DELETE },
			};

			if(kv > 256) { // Non-latin keyboard layout should still produce accelerators like Ктрл+C etc...
				static ВекторМап<int, int> hwkv; // convert hw keycode to Latin GTK keyval
				ONCELOCK {
					for(int i = 1; i < 256; i++) { // Latin keyvals are in 0..255 range
						GdkKeymapKey *keys;
						gint n_keys;
						if(gdk_keymap_get_entries_for_keyval(gdk_keymap_get_for_display(gdk_display_get_default()), i, &keys, &n_keys)) {
							for(int j = 0; j < n_keys; j++)
								if(keys[j].группа == 0)
									hwkv.добавь(keys[j].keycode, i);
							g_free(keys);
						}
					}
				}
				kv = hwkv.дай(hw, kv);
			}

			Tuple2<int, int> *x = найдиКортеж(cv, __countof(cv), kv);
			if(x)
				kv = x->b;
			else {
				if(дайКтрл() | дайАльт()) {// fix Ктрл+Shift+1 etc...
					static ВекторМап<int, int> hwkv;
					ONCELOCK {
						for(int k : { GDKEY(0), GDKEY(1), GDKEY(2), GDKEY(3), GDKEY(4),
						              GDKEY(5), GDKEY(6), GDKEY(7), GDKEY(8), GDKEY(9) }) {
							GdkKeymapKey *keys;
							gint n_keys;
							if(gdk_keymap_get_entries_for_keyval(NULL, k, &keys, &n_keys)) {
								for(int j = 0; j < n_keys; j++)
									if(keys[j].группа == 0)
										hwkv.добавь(keys[j].keycode, k);
								g_free(keys);
							}
						}
					};
					kv = hwkv.дай(hw, kv);
				}
				kv += K_DELTA;
			}
			if(дайШифт() && kv != K_SHIFT_KEY)
				kv |= K_SHIFT;
			if(дайКтрл() && kv != K_CTRL_KEY)
				kv |= K_CTRL;
			if(дайАльт() && kv != K_ALT_KEY)
				kv |= K_ALT;
			LLOG(GetKeyDesc(kv) << ", pressed: " << pressed << ", count: " << CurrentСобытие.count);
#ifdef GDK_WINDOWING_X11
			if(pressed)
				for(int i = 0; i < hotkey.дайСчёт(); i++) {
					if(hotkey[i] && keyhot[i] == (dword)kv) {
						hotkey[i]();
						return;
					}
				}
#endif
			DispatchKey(!pressed * K_KEYUP + kv, CurrentСобытие.count);
		}
		break;
	case EVENT_TEXT: {
		ШТкст h = CurrentСобытие.значение;
		for(int i = 0; i < h.дайСчёт(); i++) // TODO: добавь compression
			DispatchKey(h[i], 1);
		break;
	}
	case EVENT_FOCUS_CHANGE:
		activeCtrl = NULL;
		break;
	case GDK_DELETE: {
		ТопОкно *w = dynamic_cast<ТопОкно *>(this);
		if(w) {
			if(включен_ли()) {
				IgnoreMouseUp();
				w->WhenClose();
			}
		}
		return;
	}
	case GDK_CONFIGURE:
		SyncWndRect(CurrentСобытие.значение);
		break;
	default:
		return;
	}
	if(_this)
		_this->PostInput();
}

void Ктрл::SyncWndRect(const Прям& rect)
{
	if(дайПрям() != rect)
		SetWndRect(rect);
	ТопОкно *w = dynamic_cast<ТопОкно *>(this);
	if(w && w->state == ТопОкно::OVERLAPPED)
		w->overlapped = rect;
}

bool Ктрл::ProcessСобытие0(bool *quit, bool fetch)
{
	ПРОВЕРЬ(главнаяНить_ли());
	bool r = false;
	if(IsWaitingСобытие0(fetch)) {
		while(Событиеs.дайСчёт() > 1) { // GСобытие compression (coalesce autorepeat, mouse moves/wheel, configure)
			GСобытие& a = Событиеs[0];
			GСобытие& b = Событиеs[1];
			if(b.тип == a.тип && a.windowid == b.windowid && a.state == b.state) {
				if(a.тип == GDK_KEY_PRESS && a.значение == b.значение)
					b.count += a.count;
			    else
			    if(a.тип == GDK_SCROLL)
			        b.значение = (int)b.значение + (int)a.значение;
				else
				if(a.тип != GDK_CONFIGURE)
					break;
				Событиеs.сбросьГолову();
			}
			else
				break;
			FocusSync();
		}
		GСобытие& e = Событиеs.дайГолову();
		CurrentTime = e.time;
		CurrentMousePos = e.mousepos;
		CurrentState = e.state;
		CurrentСобытие = e;
		Значение val = e.значение;
		Событиеs.сбросьГолову();
		Ктрл *w = GetTopCtrlFromId(e.windowid);
		FocusSync();
		CaptureSync();
		if(w) {
			IMLocation(w);
			w->Proc();
		}
		r = true;
	}
	if(quit)
		*quit = IsEndSession();
	FocusSync();
	SyncCaret();
	return r;
}

bool Ктрл::обработайСобытие(bool *quit)
{
	return ProcessСобытие0(quit, true);
}

gboolean Ктрл::TimeHandler(GtkWidget *)
{ // we only need timer to periodically break blocking g_main_context_iteration
	return true;
}

void SweepMkImageCache();

bool Ктрл::ProcessСобытиеs0(bool *quit, bool fetch)
{
	bool r = false;
	while(IsWaitingСобытие0(fetch) && (!LoopCtrl || LoopCtrl->InLoop()))
		r = ProcessСобытие0(quit, fetch) || r;
	TimerProc(msecs());
	анимируйКаретку();
	if(quit)
		*quit = IsEndSession();
	for(int i = 0; i < wins.дайСчёт(); i++)
		if(wins[i].ctrl)
			wins[i].ctrl->синхПромот();
	ProcessInvalids();
	FetchСобытиеs(FALSE); // To perform any pending GDK_EXPOSE
	gdk_display_flush(gdk_display_get_default());
	return r;
}

bool Ктрл::обработайСобытия(bool *quit)
{
	ПРОВЕРЬ_(главнаяНить_ли(), "обработайСобытия can only run in the main thread");
	return ProcessСобытиеs0(quit, true);
}

void Ктрл::SysEndLoop()
{
}

void WakeUpGuiThread()
{
	g_main_context_wakeup(g_main_context_default());
}

void Ктрл::циклСобытий(Ктрл *ctrl)
{
	ЗамкниГип __;

	cancel_preedit = DoCancelPreedit; // We really need this just once, but whatever..

	ПРОВЕРЬ_(главнаяНить_ли(), "циклСобытий can only run in the main thread");
	ПРОВЕРЬ(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	if(!дайПравуюМыши() && !дайСреднююМыши() && !дайЛевуюМыши())
		ReleaseCtrlCapture();
	Ук<Ктрл> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	while(!IsEndSession() &&
	      (ctrl ? ctrl->открыт() && ctrl->InLoop() : дайТопКтрлы().дайСчёт())) {
		FetchСобытиеs(TRUE);
		обработайСобытия();
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

gboolean sOnce(GtkWidget *)
{
	return false;
}

void Ктрл::гипСпи(int ms)
{
	ЗамкниГип __;
	ПРОВЕРЬ_(главнаяНить_ли(), "Only the main thread can perform гипСпи");
	if(ms < 20) // Periodic timer is each 20ms, so that is the longest possible wait
		g_timeout_add(ms, (GSourceFunc) sOnce, NULL); // otherwise setup shorter timer
	FetchСобытиеs(TRUE);
}

}

#endif