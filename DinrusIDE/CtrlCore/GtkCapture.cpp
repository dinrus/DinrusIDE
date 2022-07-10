#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x)   //   DLOG(x)

Ук<Ктрл> Ктрл::grabwindow;
Ук<Ктрл> Ктрл::grabpopup;

#if GTK_CHECK_VERSION(3, 20, 0)
GdkSeat *Ктрл::GetSeat()
{
	return gdk_display_get_default_seat(gdk_display_get_default());
}
#endif

GdkDevice *Ктрл::GetMouseDevice()
{
#if GTK_CHECK_VERSION(3, 20, 0)
	return gdk_seat_get_pointer(GetSeat());
#else
	return gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif
}

bool Ктрл::MouseIsGrabbed()
{
	return gdk_display_device_is_grabbed(gdk_display_get_default(), GetMouseDevice());
}

bool Ктрл::GrabMouse()
{
	return
#if GTK_CHECK_VERSION(3, 20, 0)
		gdk_seat_grab(GetSeat(), gdk(), GDK_SEAT_CAPABILITY_ALL_POINTING, true, NULL, NULL, NULL, 0)
#else
		gdk_device_grab(GetMouseDevice(), gdk(), GDK_OWNERSHIP_APPLICATION, true, (GdkСобытиеMask)0, NULL, GDK_CURRENT_TIME)
#endif
	    == GDK_GRAB_SUCCESS;
}

void Ктрл::UngrabMouse()
{
#if GTK_CHECK_VERSION(3, 20, 0)
		gdk_seat_ungrab(GetSeat());
#else
		gdk_device_ungrab(GetMouseDevice(), GDK_CURRENT_TIME);
#endif
}

void Ктрл::StopGrabPopup()
{
	if(grabpopup && MouseIsGrabbed()) {
		UngrabMouse();
		grabpopup = NULL;
	}
}

void Ктрл::StartGrabPopup()
{
	if(activePopup.дайСчёт() && !grabpopup) {
		Ктрл *w = activePopup[0];
		if(w && w->открыт()) {
			ReleaseWndCapture0();
			if(w->GrabMouse())
				grabpopup = w;
		}
	}
}

bool Ктрл::SetWndCapture()
{
	ЗамкниГип __;
	ПРОВЕРЬ(главнаяНить_ли());
	LLOG("SetWndCapture " << Имя());
	if(!открыт())
		return false;
	StopGrabPopup();
	ReleaseWndCapture();
	if(GrabMouse()) {
		grabwindow = this;
		return true;
	}
	return false;
}

bool Ктрл::ReleaseWndCapture0()
{
	ЗамкниГип __;
	ПРОВЕРЬ(главнаяНить_ли());
	LLOG("ReleaseWndCapture");
	if(grabwindow) {
		UngrabMouse();
		grabwindow = NULL;
		StartGrabPopup();
		return true;
	}
	return false;
}

bool Ктрл::ReleaseWndCapture()
{
	return ReleaseWndCapture0();
}

bool Ктрл::HasWndCapture() const
{
	ЗамкниГип __;
	return this == grabwindow && grabwindow->открыт() && MouseIsGrabbed();
}

void Ктрл::CaptureSync()
{
	if(grabwindow && grabwindow->открыт() && !MouseIsGrabbed() && !grabwindow->GrabMouse())
		grabwindow = NULL;
}

};

#endif