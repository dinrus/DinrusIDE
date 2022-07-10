#include "CtrlCore.h"

#ifdef GUI_X11

namespace РНЦП {

/////////////////////////////////////////////////////////////////////////////////////////
// Constructor
DHCtrl::DHCtrl()
{
	// Sets control NOT initialized
	isInitialized = false;

	// Sets control NOT mapped
	isMapped = false;

	// Resets Ошибка contition
	isError = false;

	// Sets the user visual to null
	UserVisualInfo = 0;

	// No background painting
	backpaint = NOBACKPAINT;
//	transparent = true;
	hwnd = 0;
} // END Constructor class DHCtrl

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
DHCtrl::~DHCtrl()
{
	// Destroys the associated window and clean up stuffs
	Terminate();

} // END Destructor class DHCtrl

/////////////////////////////////////////////////////////////////////////////////////////
// Maps/unmaps the window
void DHCtrl::MapWindow(bool map)
{
	ЗамкниГип __; 
	// no action if not initialized
	if(!isInitialized)
		return;

	if(map && !isMapped)
		XMapWindow(Xdisplay, top->window);
	else if(!map && isMapped)
		XUnmapWindow(Xdisplay, top->window);

	isMapped = map;

} // END DHCtrl::MapWndow()


/////////////////////////////////////////////////////////////////////////////////////////
// Initializes the view
bool DHCtrl::иниц()
{
	ЗамкниГип __; 
	static bool isInitializing = false;

	// Just for security sakes...
	if(isInitialized)
		return true;

	// Prevents reentrant call....
	if(isInitializing)
		return false;
	isInitializing = true;

	// Call BeforeInit user func...
	BeforeInit();

	// if дисплей is null, Ошибка
	if(!Xdisplay)
	{
		// Call AfterInit user func...
		AfterInit(true);

		// Sets the appropriate Ошибка message
		SetErrorMessage("DHCtrl : Bad дисплей");

		isError = true;
		isInitializing = false;
		return false;
	}

	// Calls the user visual function
	UserVisualInfo = CreateVisual();

	// If Ошибка, returns
	if(isError)
	{
		isInitializing = false;
		return false;
	}

	// Gets the default visual, if none is given
	Visual *visual;
	int Depth;
	if(UserVisualInfo)
	{
		visual = UserVisualInfo->visual;
		Depth = UserVisualInfo->depth;
	}
	else
	{
		visual = DefaultVisual(Xdisplay, DefaultScreen(Xdisplay));
		Depth = DefaultDepth(Xdisplay, DefaultScreen(Xdisplay));
	}

	// Initializes attribute setting flags
	unsigned long ValueMask =
		CWBorderPixel
		| CWColormap
		| CWSaveUnder
//		| CWBackPixel
//		| CWBorderPixel
		| CWColormap
//		| CWСобытиеMask
//		| CWWinGravity
//		| CWBitGravity
	; // END ValueMask

	// Initializes attribute structure
	XSetWindowAttributes winAttributes;
	// creates a ColorMap, in case we're not using default visual
	winAttributes.colormap = XCreateColormap(Xdisplay, дайТопОкно()->GetWindow(), visual, AllocNone);
	winAttributes.border_pixel = 0;
	winAttributes.save_under = XFalse;
//	winAttributes.win_gravity = StaticGravity;
//	winAttributes.bit_gravity = ForgetGravity;

	// Calls the attributes user setting routine
	SetAttributes(ValueMask, winAttributes);

	// Creates the X11 window
	Прям r = GetRectInParentWindow();
	if (!hwnd) {
		hwnd = XCreateWindow
		(
			Xdisplay,									// дисплей
	//		дайТопОкно()->GetWindow(), 				// parent
			GetParentWindow(),
	
			r.left, r.top, r.устШирину(), r.устВысоту(),		// x, y, width, height
			0,											// border width
			Depth,										// depth
			InputOutput,								// class
			visual,										// visual
			ValueMask,									// значение mask
			&winAttributes								// attributes
		);
	} else {
		XReparentWindow(Xdisplay, hwnd, GetParentWindow(), r.left, r.top);
		XResizeWindow(Xdisplay, hwnd, r.устШирину(), r.устВысоту());
		XChangeWindowAttributes(Xdisplay, hwnd, ValueMask, &winAttributes);
	}

/*
	// Frees VisualInfo
    if (UserVisualInfo)
    {
        XFree( (char *)UserVisualInfo);
        UserVisualInfo = 0;
    }
*/
    // If problem creating window, Ошибка
    if(!hwnd)
    {
		// Call AfterInit user func...
		AfterInit(true);

		// Sets the appropriate Ошибка message
		SetErrorMessage("DHCtrl : Can't create window");

        isError = true;
		isInitializing = false;
        return false;
    }

	// Adds window to РНЦП managed windows
	XWindow *cw = AddXWindow(hwnd);

	cw->xic = xim ? XCreateIC
					(
						xim,
						XNInputStyle,
						XIMPreeditNothing | XIMStatusNothing,
						XNClientWindow,
						hwnd,
						XNFocusWindow,
						hwnd,
	    				NULL
	    			)
	    : NULL;

	top = new Верх;
	top->window = hwnd;

	long im_event_mask = 0;
	if(cw->xic)
		XGetICValues(cw->xic, XNFilterСобытиеs, &im_event_mask, NULL);
	XSelectInput
	(
		Xdisplay,
		hwnd,
		ExposureMask
//		| StructureNotifyMask		// *very* important, flag MUST NOT be set
		| KeyPressMask
		| FocusChangeMask
		| KeyPressMask
		| KeyReleaseMask
		| PointerMotionMask
		| ButtonPressMask
		| ButtonReleaseMask
		| PropertyChangeMask
		| VisibilityChangeMask
		| im_event_mask
	);

	int version = 5;
	XChangeProperty
	(
		Xdisplay,
		hwnd,
		XAtom("XdndAware"),
		XA_ATOM,
		32,
		0,
		(byte *)&version,
		1
	);

	// Maps the window if needed
	if(показан_ли())
		MapWindow(true);

	// Flushes the дисплей
    XFlush(Xdisplay);

    // Stores the initial control size
    CurrentSize = дайРазм();

	// Exits from initializing lock
	isInitializing = false;

	// mark control as initialized
	isInitialized = true;

	// Resets the message
	isError = false;
	SetErrorMessage("");

	// Call AfterInit user func...
	AfterInit(false);

	return true;

} // END DHCtrl::иниц()


/////////////////////////////////////////////////////////////////////////////////////////
// Terminates the view
void DHCtrl::Terminate(void)
{
	ЗамкниГип __; 
	BeforeTerminate();

	if(!isInitialized)
		return;

	// Unmaps the window
	MapWindow(false);

	// gathers data from XWindow (needs Input Контекст...)
	XWindow *cw = XWindowFromWindow(top->window);

	// Frees input context as needed
	if(cw->xic)
	{
		XDestroyIC(cw->xic);
		cw->xic = NULL;
	}

	// Removes XWindow from РНЦП list
	RemoveXWindow(top->window);

	// Destroys the window
	// Not to do, it's done destroying the parent window by X11 system
//	XDestroyWindow(Xdisplay, top->window);

	// Destroys created верх struct
	delete top;
	top = NULL;

	// Resets initialization and Ошибка flags
	isInitialized = false;
	isError = false;

} // END DHCtrl::Terminate()

/////////////////////////////////////////////////////////////////////////////////////////
// State handler
void DHCtrl::State(int reason)
{
	ЗамкниГип __; 
//	Window dummy;
//	int x, y;
//	unsigned int width, height, border, depth;
	Прям r;

	// No handling if in Ошибка state
	if( isError)
		return;

	// Initializes the control if needed (and possible...)
	if(!isInitialized && дайТопОкно() && дайТопОкно()->GetWindow())
		иниц();

	if(isInitialized)
	{
		switch( reason )
		{
			case FOCUS      : // = 10,
				break;

			case ACTIVATE   : // = 11,
				break;

			case DEACTIVATE : // = 12,
				break;

			case SHOW       : // = 13,
				MapWindow(видим_ли());
				break;

			case ENABLE     : // = 14,
				break;

			case EDITABLE   : // = 15,
				break;

			case OPEN       : // = 16,
				MapWindow(показан_ли());
				break;

			case CLOSE      : // = 17,
				Terminate();
				break;

			case POSITION   : // = 100,
			case LAYOUTPOS  : // = 101,
				SyncNativeWindows();
				break;

			default:
				break;

		} // switch(reason)
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Property Visual
Visual *DHCtrl::GetVisual(void)
{
	ЗамкниГип __; 
	if(UserVisualInfo)
		return UserVisualInfo->visual;
	else
		return DefaultVisual(Xdisplay, DefaultScreen(Xdisplay));

} // END DHCtrl::getVisual()


/////////////////////////////////////////////////////////////////////////////////////////
// Property VisualInfo
XVisualInfo DHCtrl::GetVisualInfo(void)
{
	ЗамкниГип __; 
	// if present an user visual info, just return it
	if(UserVisualInfo)
		return *UserVisualInfo;

	XVisualInfo visualInfo;
	memset(&visualInfo, 0, sizeof(visualInfo));

	// get the active visual
	Visual *visual = GetVisual();

	// gets a list of all available XVisualinfo
	XVisualInfo *v = 0;
	XVisualInfo vtemplate;
	int nVis;
	XVisualInfo *vlist = XGetVisualInfo(Xdisplay, VisualNoMask, &vtemplate, &nVis);

	// search for current visual inside the list
	if(vlist)
	{
		for (v = vlist; v < vlist + nVis; v++)
		{
			if (v->visual == visual)
			{
				visualInfo = *v;
				break;
			}
		}
	    XFree(vlist);
	}
	else
	{
		isError = true;
		ErrorMessage = "DHCtrl: no XVisualInfo for current Visual";
	}

	// returns the found XVisualInfo struct
	return visualInfo;
}

}

#endif
