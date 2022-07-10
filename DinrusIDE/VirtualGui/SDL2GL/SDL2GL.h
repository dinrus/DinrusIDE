#ifndef _SDL2GUI_SDL2GUI_h
#define _SDL2GUI_SDL2GUI_h

#include <CtrlLib/CtrlLib.h>
#include <GLDraw/GLDraw.h>

#ifdef PLATFORM_POSIX
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

namespace РНЦП {
	
struct SDL2GUI : VirtualGui {
	virtual dword       GetOptions();
	virtual Размер        дайРазм();
	virtual dword       GetMouseButtons();
	virtual dword       GetModKeys();
	virtual bool        IsMouseIn();
	virtual bool        обработайСобытие(bool *quit);
	virtual void        WaitСобытие(int ms);
	virtual bool        ожидаетСобытие();
	virtual void        WakeUpGuiThread();
	virtual void        устКурсорМыши(const Рисунок& image);
	virtual SystemDraw& BeginDraw();
	virtual void        CommitDraw();

	virtual void        Quit();
	virtual void        HandleSDLСобытие(SDL_Событие* event);

	SDL_Window   *win;
	SDL_GLContext glcontext;
	int64         serial;
	GLDraw        gldraw;
	SystemDraw    sysdraw;

	void прикрепи(SDL_Window *win, SDL_GLContext glcontext);
	void открепи();

	bool создай(const Прям& rect, const char *title);
	void разрушь();
	
	SDL2GUI();
	~SDL2GUI();
};

};

#endif
