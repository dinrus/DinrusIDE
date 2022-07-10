#include "CtrlCore.h"

#ifdef GUI_GTK

#ifdef GDK_WINDOWING_X11

namespace РНЦП {

#define Время        XTime
#define Шрифт        XFont
#define Дисплей     XDisplay
#define Picture     XPicture

#ifndef PLATFORM_OPENBSD // avoid warning
#define CurrentTime XCurrentTime
#endif

#include <gdk/gdkx.h>

#undef Picture
#undef Время
#undef Шрифт
#undef Дисплей

#ifndef PLATFORM_OPENBSD // avoid warning
#undef CurrentTime
#endif

XDisplay *Xdisplay()
{
	return GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
}

Window Xroot()
{
	return GDK_WINDOW_XID(gdk_screen_get_root_window(gdk_screen_get_default()));
}

Ткст GetProperty(Window w, Atom property, Atom rtype)
{
	ЗамкниГип __;
	Ткст result;
	int формат;
	unsigned long nitems, after = 1;
	long offset = 0;
	Atom тип = None;
	unsigned char *data;
	long rsize = minmax((long)(XMaxRequestSize(Xdisplay()) - 100), (long)256, (long)65536);
	while(after > 0) {
		if(XGetWindowProperty(Xdisplay(), w, property, offset, rsize, 0,
		                      rtype, &тип, &формат, &nitems, &after, &data) != Success)
			break;
		if(тип == None)
		    break;
		if(data) {
			int len = формат == 32 ? sizeof(unsigned long) * nitems : nitems * (формат >> 3);
			result.конкат(data, len);
			XFree((char *)data);
			offset += nitems / (32 / формат);
		}
		else
			break;
	}
	result.сожми();
	XFlush(Xdisplay());
	return result;
}

Atom XAtomRaw(const char *имя)
{
	return XInternAtom(Xdisplay(), имя, 0);
}

Atom XAtom(const char *имя)
{
	ЗамкниГип __;
	Atom x;
	INTERLOCKED {
		static ВекторМап<Ткст, int> atoms;
		int q = atoms.дай(имя, Null);
		if(пусто_ли(q)) {
			q = XAtomRaw(имя);
			atoms.добавь(имя, q);
		}
		x = q;
	}
	return x;
}

Вектор<int> GetPropertyInts(GdkWindow *w, const char *property)
{
	ЗамкниГип __;
	Вектор<int> result;
	Ткст p = GetProperty(GDK_WINDOW_XID(w), XAtom(property), AnyPropertyType);
	const long int *ptr = (const long int *)~p;
	const long int *lim = ptr + p.дайДлину() / sizeof(long int);
	result.резервируй(p.дайДлину() / sizeof(long int));
	while(ptr < lim)
		result.добавь(*ptr++);
	return result;
}

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

int Ктрл::RegisterSystemHotKey(dword ключ, Функция<void ()> cb)
{
	ЗамкниГип __;
	ПРОВЕРЬ(ключ >= K_DELTA);
	gdk_x11_display_error_trap_push(gdk_display_get_default());
	KeyCode k = XKeysymToKeycode(Xdisplay(), ключ & 0xffff);
	dword mod = X11mods(ключ);
	bool r = false;
	for(dword nlock = 0; nlock < 2; nlock++)
		for(dword clock = 0; clock < 2; clock++)
			for(dword slock = 0; slock < 2; slock++)
				r = XGrabKey(Xdisplay(), k,
				             mod | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
				             Xroot(), true, GrabModeAsync, GrabModeAsync) || r;
	gdk_x11_display_error_trap_pop_ignored(gdk_display_get_default());
	if(!r) return -1;
	int q = hotkey.дайСчёт();
	for(int i = 0; i < hotkey.дайСчёт(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.по(q) = Событие<> () << cb;
	keyhot.по(q) = k;
	modhot.по(q) = mod;
	return q;
}

void Ктрл::UnregisterSystemHotKey(int id)
{
	ЗамкниГип __;
	if(id >= 0 && id < hotkey.дайСчёт() && hotkey[id]) {
		gdk_x11_display_error_trap_push(gdk_display_get_default());
		for(dword nlock = 0; nlock < 2; nlock++)
			for(dword clock = 0; clock < 2; clock++)
				for(dword slock = 0; slock < 2; slock++)
					XUngrabKey(Xdisplay(), keyhot[id],
					           modhot[id] | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
					           Xroot());
		gdk_x11_display_error_trap_pop_ignored(gdk_display_get_default());
		hotkey[id].очисть();
	}
}

GdkFilterReturn Ктрл::RootKeyFilter(GdkXСобытие *xevent, GdkСобытие *Xevent, gpointer data)
{
	XСобытие *event = (XСобытие *)xevent;
	if(event->тип == KeyPress)
		for(int i = 0; i < hotkey.дайСчёт(); i++)
			if(hotkey[i] && keyhot[i] == event->xkey.keycode &&
			   modhot[i] == (event->xkey.state & (Mod1Mask|ShiftMask|ControlMask))) {
				hotkey[i]();
				return GDK_FILTER_REMOVE;
			}
	return GDK_FILTER_CONTINUE;
}

}

#endif

#endif
