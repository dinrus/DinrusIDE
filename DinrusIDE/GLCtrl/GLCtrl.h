#ifndef _GLCtrl_GLCtrl_h
#define _GLCtrl_GLCtrl_h

#include <CtrlCore/CtrlCore.h>

#if defined(GUI_X11) || defined(GUI_GTK)
#define Время    XTime
#define Шрифт    XFont
#define Дисплей XDisplay
#define Picture XPicture
#define Статус  int
#endif

#define GLEW_STATIC

#include <plugin/glew/glew.h>

#ifdef PLATFORM_WIN32
#include <plugin/glew/wglew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#if defined(GUI_X11) || defined(GUI_GTK)

#include <GL/glx.h>

#undef  Статус
#undef  Picture
#undef  Время
#undef  Шрифт
#undef  Дисплей
#endif

namespace РНЦП {

void InitializeGlew();


class ГЛКтрл : public Ктрл {
	typedef ГЛКтрл ИМЯ_КЛАССА;

public:
	Рисунок  MouseEvent(int event, Точка p, int zdelta, dword keyflags) override;
#ifdef PLATFORM_POSIX
	void   рисуй(Draw& w) override;
#endif
	
private:
#ifdef PLATFORM_WIN32
	struct GLPane : DHCtrl {
		friend class ГЛКтрл;
		
		ГЛКтрл *ctrl;

		void DoGLPaint();
		
	public:
		GLPane() { NoWantFocus(); }
		
		virtual void    State(int reason);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual Рисунок   MouseEvent(int event, Точка p, int zdelta, dword keyflags);
		virtual void    Pen(Точка p, const ИнфОПере& pen, dword keyflags);
		
		void иниц();
		void разрушь();
		
		void ActivateContext();

		void ExecuteGL(HDC hdc, Событие<> paint, bool swap_buffers);
		void ExecuteGL(Событие<> paint, bool swap_buffers);
	};
#endif

	friend struct GLPane;

#ifdef PLATFORM_POSIX // we assume X11 or GTK
	unsigned long win = 0;
	bool visible;
	Прям position;

	void создай();
	void синх();
	void разрушь();
	
	void State(int reason) override;
#else
	GLPane pane;
#endif

	static int  depthSize;
	static int  stencilSize;
	static bool doubleBuffering;
	static int  numberOfSamples;

	static void MakeGLContext();
	void        DoGLPaint();

	static Размер current_viewport; // because we need to set different viewports in drawing code

	Ук<Ктрл> mouseTarget = NULL;

protected:
	// Called on paint events
	virtual void GLPaint() { WhenGLPaint(); }
	
	void иниц();

public:
	Callback WhenGLPaint;

	static void SetDepthBits(int n)               { depthSize = n; }
	static void SetStencilBits(int n)             { stencilSize = n; }
	static void SetDoubleBuffering(bool b = true) { doubleBuffering = b; }
	static void SetMSAA(int n = 4)                { numberOfSamples = n; }
	
	static void CreateContext();
	
	static Размер CurrentViewport()                 { return current_viewport; }
	static void SetCurrentViewport(); // intended to restore viewport after changing it in e.g. TextureDraw
	
	ГЛКтрл& RedirectMouse(Ктрл *target)           { mouseTarget = target; return *this; }
	
	void ExecuteGL(Событие<> gl, bool swap_buffers = false);

	ГЛКтрл()                                      { иниц(); }

#ifdef PLATFORM_WIN32
	void освежи()                                { pane.освежи(); }
#endif

	// deprecated (these настройки are now static, as we have just single context)
	ГЛКтрл& DepthBits(int n)               { depthSize = n; return *this; }
	ГЛКтрл& StencilBits(int n)             { stencilSize = n; return *this; }
	ГЛКтрл& DoubleBuffering(bool b = true) { doubleBuffering = b; return *this; }
	ГЛКтрл& MSAA(int n = 4)                { numberOfSamples = n; return *this; }

	ГЛКтрл(int  depthsize, int  stencilsize = 8, bool doublebuffer = true,
	       bool multisamplebuffering = false, int  numberofsamples = 0)
	{ иниц(); DepthBits(depthsize).StencilBits(stencilsize).DoubleBuffering(doublebuffer).MSAA(numberofsamples); }

	// deprecated (fixed pipeline is so out of fashion...)
	void StdView();
};

}

#endif
