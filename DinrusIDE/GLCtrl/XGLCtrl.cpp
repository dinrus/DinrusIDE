#include "ГЛКтрл.h"

#ifdef PLATFORM_POSIX

#include <GL/glx.h>
#include <GL/gl.h>

#define Время    XTime
#define Шрифт    XFont
#define Дисплей XDisplay
#define Picture XPicture
#define Статус  int

#ifdef GUI_GTK
#include <gdk/gdkx.h>
#endif

#undef Время
#undef Шрифт
#undef Дисплей
#undef Picture
#undef Статус

namespace РНЦП {
	
static XVisualInfo *s_XVisualInfo;
static Colormap     s_Colormap;
static GLXContext   s_GLXContext;
static ::XDisplay  *s_Display;

void ГЛКтрл::создай()
{
	MemoryIgnoreLeaksBlock __;

	Ктрл *top = дайТопКтрл();
	if(!top)
		return;

#ifdef GUI_GTK
	GdkWindow *gdk = top->gdk();
	if(!gdk)
		return;

	Window w = gdk_x11_window_get_xid(gdk);
#else
	Window w = top->GetWindow();
#endif

	ONCELOCK {
	#ifdef GUI_GTK
		s_Display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
	#else
		s_Display = Xdisplay;
	#endif
		int samples = numberOfSamples;

		do {
			Вектор<int> attr;
			attr << GLX_RGBA << GLX_DEPTH_SIZE << depthSize
			     << GLX_STENCIL_SIZE << stencilSize;
			if(doubleBuffering)
				attr << GLX_DOUBLEBUFFER;
			if(samples > 1)
				attr << GLX_SAMPLE_BUFFERS_ARB << 1 << GLX_SAMPLES_ARB << samples;
			attr << 0;
			samples >>= 1;
			s_XVisualInfo = glXChooseVisual(s_Display, DefaultScreen(s_Display), attr);
		}
		while(!s_XVisualInfo && samples > 0);
		if(!s_XVisualInfo)
			return;
		s_Colormap = XCreateColormap(s_Display, RootWindow(s_Display, s_XVisualInfo->screen), s_XVisualInfo->visual, AllocNone);
		s_GLXContext = glXCreateContext(s_Display, s_XVisualInfo, NULL, GL_TRUE);
	}
	
	if(!s_GLXContext)
		return;

	XSetWindowAttributes swa;
	swa.colormap = s_Colormap;
	swa.border_pixel = 0;
	swa.event_mask = 0;
 
	win = XCreateWindow(s_Display, w, 0, 0, 1, 1, 0,
                        s_XVisualInfo->depth, InputOutput, s_XVisualInfo->visual,
                        CWBorderPixel|CWColormap|CWСобытиеMask, &swa);
	visible = false;
	position = Null;
}

void ГЛКтрл::синх()
{
	MemoryIgnoreLeaksBlock __;

	if(win) {
		Прям r = GetScreenView() - дайТопКтрл()->дайПрямЭкрана().верхЛево();
		bool b = видим_ли() && r.дайШирину() > 0 && r.дайВысоту() > 0;
		if(b != visible) {
			visible = b;
			position = Null;
			if(b)
			    XMapWindow(s_Display, win);
			else
			    XUnmapWindow(s_Display, win);
		}
		if(r != position && visible) {
			position = r;
			XMoveResizeWindow(s_Display, win, r.left, r.top, r.устШирину(), r.устВысоту());
		}
	}
}

void ГЛКтрл::State(int reason)
{
	switch(reason) {
	case CLOSE:
		XDestroyWindow(s_Display, win);
		break;
	case OPEN:
		создай();
	default:
		синх();
		break;
	}
}

void ГЛКтрл::ExecuteGL(Событие<> paint, bool swap_buffers)
{
	MemoryIgnoreLeaksBlock __;

	glXMakeCurrent(s_Display, win, s_GLXContext);

	ONCELOCK {
		glewInit();
	}

	paint();

	if(swap_buffers)
		glXSwapBuffers(s_Display, win);
	else
		glFlush();

	glXMakeCurrent(s_Display, None, NULL);
}

void ГЛКтрл::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	if(!s_GLXContext || sz.cx == 0 || sz.cy == 0)
		return;

	ExecuteGL([&] { DoGLPaint(); }, doubleBuffering);
}

}

#endif
