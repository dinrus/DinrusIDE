#ifndef _CtrlCore_CocoMM_h_
#define _CtrlCore_CocoMM_h_

#include <Core/config.h>

#if defined(PLATFORM_COCOA) && !defined(VIRTUALGUI)

#define Точка NS_Point
#define Прям  NS_Rect
#define Размер  NS_Size
#include <AppKit/AppKit.h>
#undef  Точка
#undef  Прям
#undef  Размер

#endif

#include "CtrlCore.h"

#ifdef GUI_COCOA

namespace РНЦП {

template <class T>
struct CFRef {
	T ptr;
	T operator~()   { return ptr; }
	operator T()    { return ptr; }
	T  operator->() { return ptr; }
	T  открепи()     { T h = ptr; ptr = NULL; return h; }
	CFRef(T p)      { ptr = p; }
	~CFRef()        { if(ptr) CFRelease(ptr); }
};

struct AutoreleasePool {
	NSAutoreleasePool *pool;

	AutoreleasePool() {
		pool = [[NSAutoreleasePool alloc] init];
	}
	~AutoreleasePool() {
	    [pool release];
	}
};

CGImageRef createCGImage(const Рисунок& img);

NSImage *GetNSImage(const Рисунок& img);

// From Draw/FontCoco.cpp
ШТкст вШТкст(CFStringRef s);
Ткст  вТкст(CFStringRef s);

inline ШТкст вШТкст(NSString *s) { return вШТкст((CFStringRef)s); }
inline Ткст  вТкст(NSString *s)  { return вТкст((CFStringRef)s); }

inline CGContextRef GetCG(SystemDraw& w) { return (CGContextRef)w.GetCGHandle(); }

#define      cgHandle   (CGContextRef)handle

struct PointCG {
	int x, y;

	operator CGPoint() const { return CGPointMake(x, y); }
	
	PointCG(int x, int y) : x(x), y(y) {}
	PointCG();
};

struct RectCG {
	int x, y, cx, cy;

	operator CGRect() const { return CGRectMake(x, y, cx, cy); }
	
	RectCG(int x, int y, int cx, int cy) : x(x), y(y), cx(cx), cy(cy) {}
	RectCG();
};

NSRect DesktopRect(const РНЦП::Прям& r);

}

@interface CocoView : NSView <NSWindowDelegate, NSTextInputClient>
{
	@public
	РНЦП::Ук<РНЦП::Ктрл> ctrl;
}
@end

@interface CocoWindow : NSWindow
{
	@public
	РНЦП::Ук<РНЦП::Ктрл> ctrl;
	bool active;
}
@end

struct РНЦП::MMCtrl {
	static void SyncRect(CocoView *view);
};

struct РНЦП::CocoTop {
	CocoWindow *window = NULL;
	CocoView *view = NULL;
	Ук<Ктрл> owner;
};

void SyncRect(CocoView *view);

inline РНЦП::Прям MakeRect(const CGRect& r, int dpi) {
	return РНЦП::RectC(dpi * r.origin.x, dpi * r.origin.y, dpi * r.size.width, dpi * r.size.height);
}

inline CGRect CGRectDPI(const РНЦП::Прям& r) {
	if(РНЦП::IsUHDMode())
		return CGRectMake(0.5 * r.left, 0.5 * r.top, 0.5 * r.дайШирину(), 0.5 * r.дайВысоту());
	else
		return CGRectMake(r.left, r.top, r.дайШирину(), r.дайВысоту());
}

#endif

#endif
