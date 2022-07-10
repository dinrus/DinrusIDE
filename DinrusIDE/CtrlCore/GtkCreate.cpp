#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x)    //  DLOG(x)

void Ктрл::создай(Ктрл *owner, bool popup)
{
	MemoryIgnoreLeaksBlock ___;
	ЗамкниГип __;
	ПРОВЕРЬ_(главнаяНить_ли(), "Only the main thread can open a window");
	LLOG("создай " << Имя() << " " << дайПрям());
	ПРОВЕРЬ(!отпрыск_ли() && !открыт());
	LLOG("Ungrab1");

	top = new Верх;
	top->window = gtk_window_new(popup && owner ? GTK_WINDOW_POPUP : GTK_WINDOW_TOPLEVEL);
	top->owner = owner;

	static int id;
	top->id = ++id;

	Win& w = wins.добавь();
	w.id = top->id;
	w.ctrl = this;
	w.gtk = top->window;

	ТопОкно *tw = dynamic_cast<ТопОкно *>(this);
	if(popup && !owner) {
		gtk_window_set_decorated(gtk(), FALSE);
		// gtk_window_set_has_frame(gtk(), FALSE);
		gtk_window_set_type_hint(gtk(), GDK_WINDOW_TYPE_HINT_POPUP_MENU);
	}
	else
		gtk_window_set_type_hint(gtk(), popup ? GDK_WINDOW_TYPE_HINT_COMBO
		                                : tw && tw->tool ? GDK_WINDOW_TYPE_HINT_UTILITY
		                                : owner ? GDK_WINDOW_TYPE_HINT_DIALOG
		                                : GDK_WINDOW_TYPE_HINT_NORMAL);

	top->cursor_id = -1;

	gtk_widget_set_events(top->window, GDK_ALL_EVENTS_MASK & ~GDK_POINTER_MOTION_HINT_MASK);
	g_signal_connect(top->window, "event", G_CALLBACK(GtkСобытие), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->window, "draw", G_CALLBACK(GtkDraw), (gpointer)(uintptr_t)top->id);

	GdkWindowTypeHint hint = gtk_window_get_type_hint(gtk());
	if(tw && findarg(hint, GDK_WINDOW_TYPE_HINT_NORMAL, GDK_WINDOW_TYPE_HINT_DIALOG, GDK_WINDOW_TYPE_HINT_UTILITY) >= 0)
		tw->SyncSizeHints();

	Прям r = дайПрям();

	gtk_window_set_default_size (gtk(), LSC(r.дайШирину()), LSC(r.дайВысоту()));

	gtk_window_move(gtk(), LSC(r.left), LSC(r.top));
	gtk_window_resize(gtk(), LSC(r.дайШирину()), LSC(r.дайВысоту()));

	gtk_widget_realize(top->window);

	w.gdk = gtk_widget_get_window(top->window);

	if(owner && owner->top)
		gtk_window_set_transient_for(gtk(), owner->gtk());
	gtk_widget_set_app_paintable(top->window, TRUE);
	gtk_widget_set_can_focus(top->window, TRUE);
	isopen = true;

	top->im_context = gtk_im_multicontext_new();
	gtk_im_context_set_client_window(top->im_context, gdk());
	gtk_im_context_set_use_preedit(top->im_context, true);
	g_signal_connect(top->im_context, "preedit-changed", G_CALLBACK(IMPreedit), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->im_context, "preedit-start", G_CALLBACK(IMPreedit), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->im_context, "preedit-end", G_CALLBACK(IMPreeditEnd), (gpointer)(uintptr_t)top->id);
	g_signal_connect(top->im_context, "commit", G_CALLBACK(IMCommit), (gpointer)(uintptr_t)top->id);

	WndShow(показан_ли());

	SweepConfigure(true);
	FocusSync();
	if(!popup)
		устФокусОкна();

	activeCtrl = this;

	DndInit();

	StateH(OPEN);

	GdkModifierType mod;
	Точка m = GetMouseInfo(gdk(), mod);
	r = GetWndScreenRect().дайРазм();
	if(r.содержит(m))
		DispatchMouse(MOUSEMOVE, m);

	RefreshLayoutDeep();
}

void Ктрл::разрушьОкно()
{
	ЗамкниГип __;
	LLOG("разрушьОкно " << Имя());
	DndExit();
	Ктрл *owner = дайВладельца();
	if(owner && owner->top) {
		for(int i = 0; i < wins.дайСчёт(); i++)
			if(wins[i].ctrl && wins[i].ctrl->дайВладельца() == this && wins[i].gtk) {
				LLOG("Changing owner");
				gtk_window_set_transient_for((GtkWindow *)wins[i].gtk, owner->gtk());
			}
		if(HasFocusDeep() || !GetFocusCtrl())
			activeCtrl = owner;
	}
	if(top->im_context)
		g_object_unref(top->im_context);
	gtk_widget_destroy(top->window);
	isopen = false;
	popup = false;
	delete top;
	top = NULL;
	int q = FindCtrl(this);
	if(q >= 0)
		wins.удали(q);
	if(owner)
		owner->обновиОкно();
	ТопОкно *w = dynamic_cast<ТопОкно *>(this);
	if(w && w->overlapped.дайШирину() && w->overlapped.дайВысоту())
		устПрям(w->overlapped);
}

Вектор< Ук<Ктрл> > Ктрл::activePopup;
Вектор< Ук<Ктрл> > Ктрл::visiblePopup;

void Ктрл::GuiPlatformRemove()
{
	for(int i = 0; i < visiblePopup.дайСчёт();)
		if(visiblePopup[i] == this)
			visiblePopup.удали(i);
		else
			i++;
	for(int i = 0; i < activePopup.дайСчёт();)
		if(activePopup[i] == this) {
			if(this == grabpopup && MouseIsGrabbed())
				StopGrabPopup();
			activePopup.удали(i);
			StartGrabPopup();
		}
		else
			i++;
}

void Ктрл::PopUp(Ктрл *owner, bool savebits, bool activate, bool, bool)
{
	ЗамкниГип __;
	LLOG("POPUP " << Имя() << ", " << дайПрям() << ", activate " << activate);
	создай(owner ? owner->дайТопКтрл() : дайАктивныйКтрл(), true);
	popup = true;
	Ук<Ктрл> _this = this;
	if(activate) {
		устФокус();
		if(_this) {
			activePopup.добавь(this);
			StartGrabPopup();
			CheckMouseCtrl();
		}
	}
	if(_this)
		visiblePopup.добавь(this);
}


}

#endif