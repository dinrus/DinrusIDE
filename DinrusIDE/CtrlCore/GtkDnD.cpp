#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x) // DLOG(x)

// --------------------------------------------------------------------------------------------

Ук<Ктрл>                          Ктрл::dnd_source;
const ВекторМап<Ткст, ClipData> *Ктрл::dnd_source_data;
Вектор<Ткст>                     Ктрл::dnd_fmts;
int                                Ктрл::dnd_result;
Рисунок                              Ктрл::dnd_icon;

Ктрл *Ктрл::GetDragAndDropSource()
{
	return dnd_source;
}

cairo_surface_t *CreateCairoSurface(const Рисунок& img);

void Ктрл::GtkDragBegin(GtkWidget *widget, GdkDragContext *context, gpointer user_data)
{
	if(пусто_ли(dnd_icon))
		gtk_drag_set_icon_default(context);
	else {
		cairo_surface_t *surface = CreateCairoSurface(dnd_icon);
	#if GTK_CHECK_VERSION(3, 10, 0)
		cairo_surface_set_device_scale(surface, SCL(1), SCL(1));
	#endif
		cairo_surface_set_device_offset(surface, DPI(8), DPI(8));
		gtk_drag_set_icon_surface(context, surface);
		cairo_surface_destroy(surface);
	}
	LLOG("GtkDragBegin");
}

void Ктрл::GtkDragDelete(GtkWidget *widget, GdkDragContext *context, gpointer user_data)
{
	LLOG("GtkDragDelete");
}

void Ктрл::GtkDragGetData(GtkWidget *widget, GdkDragContext *context, GtkSelectionData *data,
                          guint info, guint time, gpointer user_data)
{
	LLOG("GtkDragGetData");
	if(!dnd_source)
		return;
	if(info < (guint)dnd_source_data->дайСчёт())
		GtkSelectionDataSet(data, dnd_source_data->дайКлюч(info), (*dnd_source_data)[info].Render());
	else {
		info -= dnd_source_data->дайСчёт();
		if(info < (guint)dnd_fmts.дайСчёт()) {
			Ткст fmt = dnd_fmts[info];
			GtkSelectionDataSet(data, fmt, dnd_source->GetDropData(fmt));
		}
	}
}

void Ктрл::GtkDragEnd(GtkWidget *widget, GdkDragContext *context, gpointer user_data)
{
	LLOG("GtkDragEnd");
	dnd_result = DND_NONE;
	GdkDragAction a = gdk_drag_context_get_selected_action(context);
	dnd_result = a == GDK_ACTION_MOVE ? DND_MOVE : a == GDK_ACTION_COPY ? DND_COPY : DND_NONE;
	dnd_source = NULL;
}
                                                       
gboolean Ктрл::GtkDragFailed(GtkWidget *widget, GdkDragContext *context, GtkDragResult   result,
                             gpointer user_data)
{
	LLOG("GtkDragFailed");
	dnd_source = NULL;
	return FALSE;
}

Рисунок MakeDragImage(const Рисунок& arrow, Рисунок sample);

Рисунок MakeDragImage(const Рисунок& arrow, const Рисунок& arrow98, Рисунок sample)
{
	return arrow98;
}

int Ктрл::DoDragAndDrop(const char *fmts, const Рисунок& sample, dword actions,
                        const ВекторМап<Ткст, ClipData>& data)
{
	LLOG("------------------------------");
	LLOG("DoDragAndDrop " << fmts);
	ТопОкно *w = дайТопОкно();
	if(!w || !w->top)
		return DND_NONE;
	int gdk_actions = 0;
	if(actions & DND_MOVE)
		gdk_actions |= GDK_ACTION_MOVE;
	if(actions & DND_COPY)
		gdk_actions |= GDK_ACTION_COPY;
	GtkTargetList *list = CreateTargetList(data);
	dnd_fmts.очисть();
	for(int i = 0; i < data.дайСчёт(); i++)
		AddFmt(list, data.дайКлюч(i), i);
	Вектор<Ткст> f = разбей(fmts, ';');
	for(int i = 0; i < f.дайСчёт(); i++) {
		AddFmt(list, f[i], data.дайСчёт() + i);
		dnd_fmts.добавь(f[i]);
	}
	dnd_source_data = &data;
	dnd_result = DND_NONE;
	dnd_source = this;
	if(пусто_ли(sample))
		dnd_icon = Null;
	else {
		Размер sz = sample.дайРазм();
		int maxsize = DPI(128);
		if(sz.cx > maxsize)
			sz.cx = maxsize;
		if(sz.cy > maxsize)
			sz.cy = maxsize;
		sz += 2;
		ImageDraw iw(sz);
		iw.DrawRect(sz, белый());
		DrawFrame(iw, sz, чёрный());
		iw.DrawImage(1, 1, sz.cx, sz.cy, sample);
		dnd_icon = iw;
	}
#if GTK_CHECK_VERSION(3, 10, 0)
	gtk_drag_begin_with_coordinates(w->top->window, list, GdkDragAction(gdk_actions),
	                                дайЛевуюМыши() ? 1 : дайСреднююМыши() ? 2 : 3,
	                                CurrentСобытие.event, -1, -1);
#else
	gtk_drag_begin(w->top->window, list, GdkDragAction(gdk_actions),
	               дайЛевуюМыши() ? 1 : дайСреднююМыши() ? 2 : 3, CurrentСобытие.event);
#endif
	while(dnd_source && дайТопКтрлы().дайСчёт())
		обработайСобытия();
	dnd_source_data = NULL;
	gtk_target_list_unref (list);
	LLOG("-------- DoDragAndDrop");
	return dnd_result;
}

// --------------------------------------------------------------------------------------------

Индекс<Ткст>   Ктрл::dnd_targets;
Ткст          Ктрл::dnd_text_target;
Ткст          Ктрл::dnd_image_target;
Ткст          Ктрл::dnd_files_target;
GtkWidget      *Ктрл::dnd_widget;
GdkDragContext *Ктрл::dnd_context;
guint           Ктрл::dnd_time;
Ткст          Ктрл::dnd_data;
Ткст          Ктрл::dnd_data_fmt;
bool            Ктрл::dnd_data_wait;
bool            Ктрл::dnd_events;

Ктрл *Ктрл::DragWnd(gpointer user_data)
{
	if(dnd_data_wait) // Do nothing if waiting for data
		return NULL;
	dnd_events = true; // prevent mouse up messages to be issued
	ProcessСобытиеs0(NULL, false); // process any events that are already fetched
	dnd_events = false;
	return GetTopCtrlFromId(user_data);
}

void ToIndex(GtkTargetList *target_list, Индекс<Ткст>& ndx)
{
	gint n;
	GtkTargetEntry *t = gtk_target_table_new_from_list(target_list, &n);
	for(int i = 0; i < n; i++)
		ndx.добавь(t[i].target);
	gtk_target_table_free(t, n);
	gtk_target_list_unref(target_list);
}

void Ктрл::DndTargets(GdkDragContext *context)
{
	static Индекс<Ткст> text_targets;
	static Индекс<Ткст> image_targets;
	static Индекс<Ткст> files_targets;
	ONCELOCK {
		GtkTargetList *target_list = gtk_target_list_new (NULL, 0);
		gtk_target_list_add_text_targets(target_list, 0);
		ToIndex(target_list, text_targets);
		GtkTargetList *target_list2 = gtk_target_list_new (NULL, 0);
		gtk_target_list_add_image_targets(target_list2, 0, TRUE);
		ToIndex(target_list2, image_targets);
		GtkTargetList *target_list3 = gtk_target_list_new (NULL, 0);
		gtk_target_list_add_uri_targets(target_list3, 0);
		ToIndex(target_list3, files_targets);
	}
	dnd_targets.очисть();
	dnd_text_target.очисть();
	for(GList *list = gdk_drag_context_list_targets(context); list; list = g_list_next (list)) {
		Ткст g = gdk_atom_name((GdkAtom)list->data);
		if(text_targets.найди(g) >= 0) {
			dnd_targets.добавь("text");
			if(dnd_text_target.пустой())
				dnd_text_target = g;
		}
		else
		if(image_targets.найди(g) >= 0) {
			dnd_targets.добавь("image");
			if(dnd_image_target.пустой())
				dnd_image_target = g;
		}
		else
		if(files_targets.найди(g) >= 0) {
			dnd_targets.добавь("files");
			if(dnd_files_target.пустой())
				dnd_files_target = g;
		}
		else
			dnd_targets.добавь(g);
	}
}

void Ктрл::GtkDragDataReceived(GtkWidget *widget, GdkDragContext *context,
                               gint x, gint y, GtkSelectionData *data,
                               guint info, guint time, gpointer user_data)
{
	LLOG("GtkDragDataReceived " << dnd_data_fmt);
	dnd_data_wait = false;
	if(dnd_data_fmt == "text") {
		guchar *s = gtk_selection_data_get_text(data);
		if(s) {
			dnd_data = (const char *)s;
			g_free(s);
		}
	}
	else
	if(dnd_data_fmt == "image")
		dnd_data = ImageClipFromPixbufUnref(gtk_selection_data_get_pixbuf(data));
	else
	if(dnd_data_fmt == "files")
		dnd_data = FilesClipFromUrisFree(gtk_selection_data_get_uris(data));
	else
		dnd_data = GtkDataGet(data);
}

bool Ктрл::IsDragAvailable(const char *fmt)
{
	LLOG("IsDragAvailable " << fmt << " " << (Ктрл::dnd_targets.найди(fmt) >= 0));
	return dnd_targets.найди(fmt) >= 0;
}

Ткст Ктрл::DragGet(const char *fmt)
{
	LLOG("DragGet " << fmt << " " << (Ктрл::dnd_targets.найди(fmt) >= 0));
	if(Ктрл::dnd_targets.найди(fmt) < 0)
		return Null;
	dnd_data.очисть();
	dnd_data_wait = true;
	dnd_data_fmt = fmt;
	int t0 = msecs();
	gtk_drag_get_data(dnd_widget, dnd_context,
	                  GAtom(strcmp(fmt, "image") == 0 ? ~dnd_image_target :
	                        strcmp(fmt, "text") == 0 ? ~dnd_text_target :
	                        strcmp(fmt, "files") == 0 ? ~dnd_files_target : fmt),
	                  dnd_time);
	while(msecs() - t0 < 1000 && dnd_data_wait)
		FetchСобытиеs(true);
	LLOG("DragGet data length " << dnd_data.дайДлину());
	return dnd_data;
}

PasteClip Ктрл::GtkDnd(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
                       guint time, gpointer user_data, bool paste)
{
	DndTargets(context);
	g_object_ref(context); // make sure these always survive the action...
	g_object_ref(widget);
	dnd_context = context;
	dnd_widget = widget;
	dnd_time = time;
	PasteClip clip;
	clip.тип = 1;
	clip.paste = paste;
	clip.accepted = false;
	clip.allowed = DND_MOVE|DND_COPY;
	GdkModifierType mod;
	GetMouseInfo(gdk_get_default_root_window(), mod);
	clip.action = mod & GDK_CONTROL_MASK ? DND_COPY : DND_MOVE;
	Ктрл *w = DragWnd(user_data);
	if(w) {
		GetMouseInfo(gdk_get_default_root_window(), mod);
		CurrentState = mod;
		CurrentMousePos = Точка(SCL(x), SCL(y)) + w->дайПрямЭкрана().верхЛево();
		w->ТиБ(CurrentMousePos, clip);
	}
	gdk_drag_status(context, clip.IsAccepted() ? clip.GetAction() == DND_MOVE ? GDK_ACTION_MOVE
	                                                                          : GDK_ACTION_COPY
	                                           : GdkDragAction(0), time);
	return clip;
}

gboolean Ктрл::GtkDragMotion(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
                             guint time, gpointer user_data)
{
	LLOG("GtkDragMotion");

	PasteClip clip = GtkDnd(widget, context, x, y, time, user_data, false);
	g_object_unref(widget);
	g_object_unref(context);
	return TRUE;
}

void Ктрл::GtkDragLeave(GtkWidget *widget, GdkDragContext *context, guint time, gpointer user_data)
{
	LLOG("GtkDragLeave");
	DnDLeave();
}

gboolean Ктрл::GtkDragDrop(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
                           guint time, gpointer user_data)
{
	LLOG("GtkDragDrop");

	PasteClip clip = GtkDnd(widget, context, x, y, time, user_data, true);
	gtk_drag_finish(context, clip.IsAccepted(), clip.IsAccepted() && clip.GetAction() == DND_MOVE, time);
	g_object_unref(widget);
	g_object_unref(context);
	DnDLeave();
	return TRUE;
}

// -----------------------------------------------------------------------------------------

void Ктрл::DndInit()
{
	GtkWidget *w = top->window;
	gpointer id = (gpointer)(uintptr_t)top->id;
	g_signal_connect(w, "drag-begin", G_CALLBACK(GtkDragBegin), id);
	g_signal_connect(w, "drag-data-delete", G_CALLBACK(GtkDragDelete), id);
	g_signal_connect(w, "drag-data-get", G_CALLBACK(GtkDragGetData), id);
	g_signal_connect(w, "drag-data-received", G_CALLBACK(GtkDragDataReceived), id);
	g_signal_connect(w, "drag-drop", G_CALLBACK(GtkDragDrop), id);
	g_signal_connect(w, "drag-end", G_CALLBACK(GtkDragEnd), id);
	g_signal_connect(w, "drag-failed", G_CALLBACK(GtkDragFailed), id);
	g_signal_connect(w, "drag-leave", G_CALLBACK(GtkDragLeave), id);
	g_signal_connect(w, "drag-motion", G_CALLBACK(GtkDragMotion), id);
	gtk_drag_dest_set(w, GtkDestDefaults(0), NULL, 0, GdkDragAction(GDK_ACTION_COPY|GDK_ACTION_MOVE));
}

void Ктрл::DndExit()
{
	if(top)
		gtk_drag_dest_unset(top->window);
}

}

#endif
