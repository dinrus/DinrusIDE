#include "SDL2GL.h"

namespace РНЦП {

SystemDraw& SDL2GUI::BeginDraw()
{
	gldraw.иниц(дайРазм(), (uint64)glcontext);
	sysdraw.SetTarget(&gldraw);
	return sysdraw;
}

void SDL2GUI::CommitDraw()
{
	gldraw.финиш();
	SDL_GL_SwapWindow(win);
}

};