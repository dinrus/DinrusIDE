#include "CtrlLib.h"

#ifdef GUI_GTK

#include <libnotify/notify.h>
#ifdef NOTIFY_CHECK_VERSION
    #if NOTIFY_CHECK_VERSION(0,7,0)
        #define NOTIFY_VERSION_GT_0_7_0
    #endif
#endif

G_GNUC_BEGIN_IGNORE_DEPRECATIONS

namespace РНЦП {

ИконкаТрея::ИконкаТрея()
{
	tray_icon = gtk_status_icon_new ();
	g_signal_connect(tray_icon, "button-press-event", G_CALLBACK(DoButtonPress), this);
	g_signal_connect(tray_icon, "button-release-event", G_CALLBACK(DoButtonRelease), this);
	g_signal_connect(tray_icon, "activate", G_CALLBACK(DoActivate), this);
	g_signal_connect(tray_icon, "popup-menu", G_CALLBACK(PopupMenu), this);
	active = true;
}

gboolean ИконкаТрея::DoButtonPress(GtkStatusIcon *, GdkСобытиеButton *e, gpointer user_data)
{
	ИконкаТрея *q = (ИконкаТрея *)user_data;
	if(e->тип == GDK_BUTTON_PRESS) {
		if(e->button == 1) {
			static int clicktime = msecs() - 100000;
			if(msecs(clicktime) < 250)
				q->леваяДКлик();
			else
				q->леваяВнизу();
			clicktime = msecs();
		}
		if(e->button == 3)
			q->ExecuteMenu();
	}
	return TRUE;
}

gboolean ИконкаТрея::DoButtonRelease(GtkStatusIcon *, GdkСобытиеButton *e, gpointer user_data)
{
	ИконкаТрея *q = (ИконкаТрея *)user_data;
	if(e->button == 1)
		q->леваяВверху();
	return TRUE;
}

void ИконкаТрея::PopupMenu(GtkStatusIcon *, guint, guint32, gpointer user_data)
{
	((ИконкаТрея *)user_data)->ExecuteMenu();
}

void ИконкаТрея::DoActivate(GtkStatusIcon *icon, gpointer user_data)
{
	((ИконкаТрея *)user_data)->леваяВнизу();
	((ИконкаТрея *)user_data)->леваяДКлик();
}

void ИконкаТрея::синх()
{
	gtk_status_icon_set_from_pixbuf(tray_icon, image);
	gtk_status_icon_set_tooltip_text(tray_icon, tooltip);
}

void ИконкаТрея::DoMenu(Бар& bar)
{
	Menu(bar);
}

void ИконкаТрея::Message(int тип, const char *title, const char *text, int timeout)
{
	if(!notify_is_initted() && !notify_init(title))
		return;
	GError *Ошибка = NULL;
	NotifyNotification *notification = notify_notification_new (title, text
					, тип == 1 ? "gtk-dialog-info"
					: тип == 2 ? "gtk-dialog-warning"
					: "gtk-dialog-Ошибка"
#ifndef NOTIFY_VERSION_GT_0_7_0
					, NULL
#endif
					);
	notify_notification_set_timeout(notification, timeout * 1000);
	notify_notification_show(notification, &Ошибка);
}

void ИконкаТрея::ExecuteMenu()
{
	gint x, y;
	GdkModifierType mod;
	gdk_window_get_pointer(gdk_get_default_root_window(), &x, &y, &mod);
	БарМеню::выполни(NULL, THISBACK(DoMenu), Точка(x, y));
}

void ИконкаТрея::пуск()
{
	gtk_main();
}

void ИконкаТрея::Break()
{
	gtk_main_quit();
}

void ИконкаТрея::покажи(bool b)
{
	gtk_status_icon_set_visible(tray_icon, b);
}

bool ИконкаТрея::видим_ли() const
{
	return gtk_status_icon_get_visible(tray_icon);
}

void ИконкаТрея::леваяВнизу()
{
	WhenLeftDown();
}

void ИконкаТрея::леваяВверху()
{
	WhenLeftUp();
}

void ИконкаТрея::леваяДКлик()
{
	WhenLeftDouble();
}

void ИконкаТрея::Menu(Бар& bar)
{
	WhenBar(bar);
}

ИконкаТрея::~ИконкаТрея()
{
	g_object_unref(tray_icon);
}

}

G_GNUC_END_IGNORE_DEPRECATIONS

#endif
