#include "CtrlCore.h"

#ifdef GUI_X11

#include <locale.h>

namespace РНЦП {

#define LLOG(x)  // DLOG(x)

XIM Ктрл::xim;

Atom XAtomRaw(const char *имя)
{
	return XInternAtom(Xdisplay, имя, XFalse);
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

Ткст XAtomName(Atom atom)
{
	ЗамкниГип __;
	LLOG("GetAtomName");
	return XGetAtomName(Xdisplay, atom);
}

Ткст GetProperty(Window w, Atom property, Atom rtype)
{
	ЗамкниГип __;
	LLOG("GetProperty");
	Ткст result;
	int формат;
	unsigned long nitems, after = 1;
	long offset = 0;
	Atom тип = None;
	unsigned char *data;
	long rsize = minmax((long)(XMaxRequestSize(Xdisplay) - 100), (long)256, (long)65536);
	while(after > 0) {
		if(XGetWindowProperty(Xdisplay, w, property, offset, rsize, XFalse,
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
	XFlush(Xdisplay);
	return result;
}

bool WaitForСобытие(Window w, int тип, XСобытие& event){
	ЗамкниГип __;
	for(int i = 0; i < 80; i++) {
		if(XCheckTypedWindowСобытие(Xdisplay, w, тип, &event))
			return true;
		XFlush(Xdisplay);
		спи(50);
	}
	LOG("WaitForСобытие failed");
	return false;
}


Ткст ReadPropertyData(Window w, Atom property, Atom rtype)
{
	ЗамкниГип __;
	static Atom XA_INCR = XAtom("INCR");
	Atom тип;
	int формат;
	unsigned long nitems, after;
	unsigned char *ptr;
	Ткст r;
	if(XGetWindowProperty(Xdisplay, w, property, 0, 0, XFalse, AnyPropertyType,
	                      &тип, &формат, &nitems, &after, &ptr) == Success && тип != None) {
		XFree(ptr);
		if(тип == XA_INCR) {
			XDeleteProperty(Xdisplay, w, property);
			XСобытие event;
			for(;;) {
				XFlush(Xdisplay);
				if(!WaitForСобытие(w, PropertyNotify, event))
					break;
				if(event.xproperty.atom == property && event.xproperty.state == PropertyNewValue) {
					Ткст x = GetProperty(w, property);
					if(!x.дайДлину())
						break;
					r.конкат(x);
					XDeleteProperty(Xdisplay, w, property);
				}
			}
		}
		else {
			r = GetProperty(w, property);
			XDeleteProperty(Xdisplay, w, property);
		}
	}
	return r;
}

Вектор<int> GetPropertyInts(Window w, Atom property, Atom rtype)
{
	ЗамкниГип __;
	Вектор<int> result;
	Ткст p = GetProperty(w, property, rtype);
	const long int *ptr = (const long int *)~p;
	const long int *lim = ptr + p.дайДлину() / sizeof(long int);
	result.резервируй(p.дайДлину() / sizeof(long int));
	while(ptr < lim)
		result.добавь(*ptr++);
	return result;
}

Индекс<Atom>& _NET_Supported()
{
	static Индекс<Atom> q;
	return q;
}

bool X11ErrorTrap;

bool Ктрл::TrapX11Errors()
{
	ЗамкниГип __;
	bool b = X11ErrorTrap;
	X11ErrorTrap = true;
	return b;
}

void Ктрл::UntrapX11Errors(bool b)
{
	ЗамкниГип __;
	X11ErrorTrap = b;
}

void sPanicMessageBox(const char *title, const char *text)
{
	IGNORE_RESULT(
		write(2, text, strlen(text))
	);
	IGNORE_RESULT(
		write(2, "\n", 1)
	);
	if(Ктрл::grabWindow) {
		LLOG("RELEASE GRAB");
		XUngrabPointer(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
	}
	XDisplay *дисплей = XOpenDisplay(NULL);
	if(!дисплей)
		return;
	int screen = DefaultScreen(дисплей);
	int x = (DisplayWidth(дисплей, screen) - 600) / 2;
	int y = (DisplayHeight(дисплей, screen) - 120) / 2;
	Window win = XCreateSimpleWindow(дисплей, RootWindow(дисплей, screen),
	                                 x, y, 600, 120, 4,
	                                 BlackPixel(дисплей, screen),
	                                 WhitePixel(дисплей, screen));
	XSizeHints size_hints;
	size_hints.flags = PPosition|PSize|PMinSize;
	size_hints.x = x;
	size_hints.y = x;
	size_hints.width = 600;
	size_hints.height = 120;
	size_hints.min_width = 600;
	size_hints.min_height = 120;
	char *h[1];
	char hh[1];
	*hh = 0;
	h[0] = hh;
	XSetStandardProperties(дисплей, win, title, title, None, h, 0, &size_hints);
	XSelectInput(дисплей, win, ExposureMask|KeyPressMask|ButtonPressMask|StructureNotifyMask);
	XGCValues values;
	GC gc = XCreateGC(дисплей, win, 0, &values);
	unsigned long wina[1];
	wina[0] = XInternAtom(дисплей, "_NET_WM_STATE_ABOVE", XFalse);
	XChangeProperty(дисплей, win,
	                XInternAtom(дисплей, "_NET_WM_STATE", XFalse),
	                XInternAtom(дисплей, "ATOM", XFalse), 32,
	                PropModeReplace, (const unsigned char *)&wina, 1);
	XMapWindow(дисплей, win);
//	XSetInputFocus(дисплей, win, RevertToParent, CurrentTime);
	XRaiseWindow(дисплей, win);
	XFontStruct *font_info = XQueryFont(дисплей, XGContextFromGC(gc));
	for(;;) {
		XСобытие e;
		XNextСобытие(дисплей, &e);
		switch(e.тип) {
		case KeyPress:
		case ButtonPress:
			XFreeFont(дисплей, font_info);
			XFreeGC(дисплей, gc);
			XCloseDisplay(дисплей);
		#ifdef _ОТЛАДКА
			__ВСЁ__;
		#endif
			return;
		case Expose:
			int y = 20;
			const char *b = text;
			for(;;) {
				const char *e = strchr(b, '\n');
				if(!e) break;
				XDrawString(дисплей, win, gc, 20, y, b, e - b);
				y += font_info->max_bounds.ascent + font_info->max_bounds.descent;
				b = e + 1;
			}
			XDrawString(дисплей, win, gc, 20, y, b, strlen(b));
			break;
		}
	}
}

#ifdef _ОТЛАДКА
#define INI_PREFIX "DEBUG_"
#else
#define INI_PREFIX
#endif

int X11ErrorHandler(XDisplay *, XErrorСобытие *Ошибка)
{
	if(X11ErrorTrap || режимПаники_ли()) return 0;

	if(дайКлючИни(INI_PREFIX "X11_ERRORS") != "1")
		return 0;

	static const char *request[] = {
		"",
		"X_CreateWindow",
		"X_ChangeWindowAttributes",
		"X_GetWindowAttributes",
		"X_DestroyWindow",
		"X_DestroySubwindows",
		"X_ChangeSaveSet",
		"X_ReparentWindow",
		"X_MapWindow",
		"X_MapSubwindows",
		"X_UnmapWindow",
		"X_UnmapSubwindows",
		"X_ConfigureWindow",
		"X_CirculateWindow",
		"X_GetGeometry",
		"X_QueryTree",
		"X_InternAtom",
		"X_GetAtomName",
		"X_ChangeProperty",
		"X_DeleteProperty",
		"X_GetProperty",
		"X_ListProperties",
		"X_SetSelectionOwner",
		"X_GetSelectionOwner",
		"X_ConvertSelection",
		"X_SendСобытие",
		"X_GrabPointer",
		"X_UngrabPointer",
		"X_GrabButton",
		"X_UngrabButton",
		"X_ChangeActivePointerGrab",
		"X_GrabKeyboard",
		"X_UngrabKeyboard",
		"X_GrabKey",
		"X_UngrabKey",
		"X_AllowСобытиеs",
		"X_GrabServer",
		"X_UngrabServer",
		"X_QueryPointer",
		"X_GetMotionСобытиеs",
		"X_TranslateCoords",
		"X_WarpPointer",
		"X_SetInputFocus",
		"X_GetInputFocus",
		"X_QueryKeymap",
		"X_OpenFont",
		"X_CloseFont",
		"X_QueryFont",
		"X_QueryTextExtents",
		"X_ListFonts",
		"X_ListFontsWithInfo",
		"X_SetFontPath",
		"X_GetFontPath",
		"X_CreatePixmap",
		"X_FreePixmap",
		"X_CreateGC",
		"X_ChangeGC",
		"X_CopyGC",
		"X_SetDashes",
		"X_SetClipRectangles",
		"X_FreeGC",
		"X_ClearArea",
		"X_CopyArea",
		"X_CopyPlane",
		"X_PolyPoint",
		"X_PolyLine",
		"X_PolySegment",
		"X_PolyRectangle",
		"X_PolyArc",
		"X_FillPoly",
		"X_PolyFillRectangle",
		"X_PolyFillArc",
		"X_PutImage",
		"X_GetImage",
		"X_PolyText8",
		"X_PolyText16",
		"X_ImageText8",
		"X_ImageText16",
		"X_CreateColormap",
		"X_FreeColormap",
		"X_CopyColormapAndFree",
		"X_InstallColormap",
		"X_UninstallColormap",
		"X_ListInstalledColormaps",
		"X_AllocColor",
		"X_AllocNamedColor",
		"X_AllocColorCells",
		"X_AllocColorPlanes",
		"X_FreeColors",
		"X_StoreColors",
		"X_StoreNamedColor",
		"X_QueryColors",
		"X_LookupColor",
		"X_CreateCursor",
		"X_CreateGlyphCursor",
		"X_FreeCursor",
		"X_RecolorCursor",
		"X_QueryBestSize",
		"X_QueryExtension",
		"X_ListExtensions",
		"X_ChangeKeyboardMapping",
		"X_GetKeyboardMapping",
		"X_ChangeKeyboardControl",
		"X_GetKeyboardControl",
		"X_Bell",
		"X_ChangePointerControl",
		"X_GetPointerControl",
		"X_SetScreenSaver",
		"X_GetScreenSaver",
		"X_ChangeHosts",
		"X_ListHosts",
		"X_SetAccessControl",
		"X_SetCloseDownMode",
		"X_KillClient",
		"X_RotateProperties",
		"X_ForceScreenSaver",
		"X_SetPointerMapping",
		"X_GetPointerMapping",
		"X_SetModifierMapping",
		"X_GetModifierMapping",
		"X_NoOperation",
	};

	char h[512];
	XGetErrorText(Xdisplay, Ошибка->error_code, h, 512);
	Ткст e;
	e << "X Ошибка: " << h;
	if(Ошибка->request_code < __countof(request))
		e << "\nrequest: " << request[Ошибка->request_code];
	e << "\nresource id: " << (int)Ошибка->resourceid << " = " << фмт("%0X", (int)Ошибка->resourceid);

	RLOG(e);
	puts(e);

	паника(e);

	return 0;
}

void SetX11ErrorHandler()
{
	XSetErrorHandler(X11ErrorHandler);
}

ИНИЦБЛОК {
	устБоксСообПаники(sPanicMessageBox);
}

void Ктрл::InstallPanicBox()
{
	устБоксСообПаники(sPanicMessageBox);
}

void Ктрл::InitX11(const char *дисплей)
{
	ЗамкниГип __;

	XInitThreads();

	устБоксСообПаники(sPanicMessageBox);

	InitX11Draw(дисплей);
	иницТаймер();
	byte dummy[5];
	Xbuttons = XGetPointerMapping(Xdisplay, dummy, 5);

	Xeventtime = CurrentTime;
	SetX11ErrorHandler();
	if(дайКлючИни(INI_PREFIX "X11_SYNCHRONIZE") == "1")
		XSynchronize(Xdisplay, 1);
	Вектор<int> nets = GetPropertyInts(Xroot, XAtom("_NET_SUPPORTED"));
	for(int i = 0; i < nets.дайСчёт(); i++)
		_NET_Supported().добавь(nets[i]);

	Шрифт::SetDefaultFont(Arial(12));

	ReSkin();

	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_DragFullWindow_Write(1);
	GUI_PopUpEffect_Write(IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	GUI_DropShadows_Write(1);
	GUI_AltAccessKeys_Write(1);
	GUI_AKD_Conservative_Write(0);

	setlocale(LC_ALL, "en_US.utf8");
	if(XSupportsLocale()) {
		XSetLocaleModifiers("");
		xim = XOpenIM(Xdisplay, NULL, NULL, NULL);
	}
	else {
		xim = NULL;
		LOG("IM unsupported!");
	}

	Csizeinit();
	
	войдиВСтопорГип();
}

void Ктрл::ExitX11()
{
	ЗамкниГип __;
//	if(xic)
//		XDestroyIC(xic);
	ТопОкно::ShutdownWindows();
	закройТопКтрлы();
	for(int i = 0; i < hotkey.дайСчёт(); i++)
		UnregisterSystemHotKey(i);
	if(xim)
		XCloseIM(xim);
	покиньСтопорГип();
}

Вектор<Прям> FindScreensResolutions()
{
	Вектор<Прям> screensResolutions;
	int event, Ошибка;
	
	if(XineramaQueryExtension(Xdisplay, &event, &Ошибка)) {
		if(XineramaIsActive(Xdisplay)) {
			int screensNumber = 0;
			XineramaScreenInfo* info = XineramaQueryScreens(Xdisplay, &screensNumber);
			for(int i = 0; i < screensNumber; i++)
				screensResolutions.добавь(Прям(info[i].x_org, info[i].y_org, info[i].x_org + info[i].width, info[i].y_org + info[i].height));
			XFree(info);
		}
	}
	return screensResolutions;
}

Вектор<Прям> FindScreensStruts()
{
	Вектор<Прям> struts;
	
	Вектор<int> clients = GetPropertyInts(Xroot, XAtom("_NET_CLIENT_LIST"));
	for (int i = 0; i < clients.дайСчёт(); i++) {
		Вектор<int> strut = GetPropertyInts(clients[i], XAtom("_NET_WM_STRUT"));
		if(strut.дайСчёт() == 4)
			struts.добавь(Прям(strut[0], strut[2], strut[1], strut[3]));
	}
	return struts;
}

Прям Ктрл::GetDefaultWindowRect()
{
	ЗамкниГип __;
	static int width  = 0;
	static int height = 0;
	static int left   = 0;
	static int top    = 0;
	if (width == 0 && height == 0) {
		Вектор<Прям> screens = FindScreensResolutions();
		if(screens.дайСчёт()) {
			width  = screens[0].устШирину();
			height = screens[0].устВысоту();
			left   = screens[0].left;
			top    = screens[0].top;
		}
		else {
			width  = Xwidth;
			height = Xheight;
		}
	}
	
	static int pos = min(width / 10, 50);
	pos += 10;
	int cx = width * 2 / 3;
	int cy = height * 2 / 3;
	if(pos + cx + 50 > width || pos + cy + 50 > height)
		pos = 0;
	return RectC(left + pos + 20, top + pos + 20, cx, cy);
}

void Ктрл::GetWorkArea(Массив<Прям>& out)
{
	Вектор<Прям> workAreas = FindScreensResolutions();
	Вектор<Прям> struts    = FindScreensStruts();
	for (int i = 0; i < workAreas.дайСчёт(); i++) {
		if (i < struts.дайСчёт()) {
			workAreas[i].left   += struts[i].left;
			workAreas[i].right  -= struts[i].right;
			workAreas[i].top    += struts[i].top;
			workAreas[i].bottom -= struts[i].bottom;
		}
		out.добавь(workAreas[i]);
	}
	if (out.пустой())
		out.добавь(GetPrimaryWorkArea());
}

Прям Ктрл::GetWorkArea() const
{
	ЗамкниГип __;
	
	static Массив<Прям> rc;
	if (rc.пустой())
		GetWorkArea(rc);
	
	Точка pt = дайПрямЭкрана().верхЛево();
	for (int i = 0; i < rc.дайСчёт(); i++)
		if(rc[i].содержит(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

Прям Ктрл::GetVirtualWorkArea()
{
	ЗамкниГип __;
	static Прям r;
	if(r.right == 0) {
		Вектор<int> x = GetPropertyInts(Xroot, XAtom("_NET_WORKAREA"));
		if(x.дайСчёт())
			r = RectC(x[0], x[1], x[2], x[3]);
		else
			r = RectC(0, 0, Xwidth, Xheight);
	}
	return r;
}

Прям Ктрл::GetVirtualScreenArea()
{
	return RectC(0, 0, Xwidth, Xheight);
}

Прям Ктрл::GetPrimaryWorkArea()
{
	ЗамкниГип __;
	static Прям r;
	if(r.right == 0) {
		Массив<Прям> rc;
		GetWorkArea(rc);
		rc.дайСчёт() ? r = rc[0] : r = GetVirtualScreenArea();
	}
	return r;
}

Прям Ктрл::GetPrimaryScreenArea()
{
	ЗамкниГип __;
	static Прям r;
	if(r.right == 0) {
		Вектор<Прям> screens = FindScreensResolutions();
		screens.дайСчёт() ? r = screens[0] : r = GetVirtualScreenArea();
	}
	return r;
}

int Ктрл::GetKbdDelay()
{
	return 250;
}

int Ктрл::GetKbdSpeed()
{
	return 25;
}


#ifdef _ОТЛАДКА
extern bool __X11_Grabbing;
void _DBG_Ungrab(void) {
	if(__X11_Grabbing)
	{
		XUngrabPointer(Xdisplay, CurrentTime);
		XUngrabKeyboard(Xdisplay, CurrentTime);
		XFlush(Xdisplay);
		__X11_Grabbing = false;
	}
}
#endif

}

#endif
