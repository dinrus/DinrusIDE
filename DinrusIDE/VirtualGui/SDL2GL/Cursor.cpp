#include "SDL2GL.h"

namespace РНЦП {

#define LLOG(x)  LOG(x)
#define LDUMP(x) //DDUMP(x)

struct RectSDL {
	SDL_Rect sr;
	
	operator SDL_Rect *() { return &sr; }
	
	RectSDL(const Прям& r)
	{
		sr.x = r.left;
		sr.y = r.top;
		sr.w = r.дайШирину();
		sr.h = r.дайВысоту();
	}
};

SDL_Texture *SDLTextureFromImage(SDL_Renderer *renderer, const Рисунок& m)
{
	Размер isz = m.дайРазм();
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                                         SDL_TEXTUREACCESS_STATIC, isz.cx, isz.cy);
	if(texture) {
		SDL_UpdateTexture(texture, RectSDL(isz), ~m, isz.cx * sizeof(КЗСА));
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}
	return texture;
}


void SDL2GUI::устКурсорМыши(const Рисунок& image)
{
	ЗамкниГип __;
	static Рисунок fbCursorImage;
	static Точка fbCursorPos;
	static SDL_Cursor  *sdl_cursor;
	static SDL_Surface *sdl_cursor_surface;
	static Буфер<КЗСА> data;
	if(image.GetSerialId() != fbCursorImage.GetSerialId()) {
		fbCursorImage = image;
		fbCursorPos = Null;
		SDL_ShowCursor(true);
		if(sdl_cursor)
			SDL_FreeCursor(sdl_cursor);
		if(sdl_cursor_surface)
			SDL_FreeSurface(sdl_cursor_surface);
		int64 a = image.GetAuxData();
		if(a)
			sdl_cursor = SDL_CreateSystemCursor(SDL_SystemCursor(a - 1));
		else {
			sdl_cursor = NULL;
			data.размести(image.дайДлину());
			копируй(data, image, image.дайДлину());
			sdl_cursor_surface = SDL_CreateRGBSurfaceFrom(~data, image.дайШирину(), image.дайВысоту(),
			                                              32, sizeof(КЗСА) * image.дайШирину(),
			                                              0xff0000, 0xff00, 0xff, 0xff000000);
			Точка h = image.GetHotSpot();
			if(sdl_cursor_surface)
				sdl_cursor = SDL_CreateColorCursor(sdl_cursor_surface, h.x, h.y);
		}
		if(sdl_cursor)
			SDL_SetCursor(sdl_cursor);
	}
}

}
