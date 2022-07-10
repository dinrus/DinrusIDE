#include "SDL2GL.h"

namespace РНЦП {

dword SDL2GUI::GetOptions()
{
	return GUI_SETMOUSECURSOR;
}

Размер SDL2GUI::дайРазм()
{
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	return Размер(w, h);
}

bool SDL2GUI::создай(const Прям& rect, const char *title)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

	win = SDL_CreateWindow(title, rect.left, rect.top, rect.дайШирину(), rect.дайВысоту(),
	                       SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_BORDERLESS);
	if(!win)
		return false;
	MemoryIgnoreLeaksBegin();
	glcontext = SDL_GL_CreateContext(win);
	MemoryIgnoreLeaksEnd();
	if(!glcontext) {
		разрушь();
		return false;
	}
	return true;
}

extern SDL_TimerID waketimer_id;

void SDL2GUI::разрушь()
{
	if(glcontext) {
		SDL_GL_DeleteContext(glcontext);
		glcontext = NULL;
		GLDraw::ResetCache();
	}
	if(win) {
		SDL_RemoveTimer(waketimer_id);
		SDL_DestroyWindow(win);
		win = NULL;
	}
}

void SDL2GUI::прикрепи(SDL_Window *win_, SDL_GLContext glcontext_)
{
	win = win_;
	glcontext = glcontext_;
}

void SDL2GUI::открепи()
{
	win = NULL;
	glcontext = NULL;
}

SDL2GUI::SDL2GUI()
{
	glcontext = NULL;
	win = NULL;
}

SDL2GUI::~SDL2GUI()
{
	разрушь();
}

void SDL2GUI::Quit()
{
	SDL_Quit();
}

}