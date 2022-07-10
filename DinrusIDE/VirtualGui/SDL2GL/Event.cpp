#include "SDL2GL.h"

#define LLOG(x)

namespace РНЦП {

extern dword fbKEYtoK(dword chr);

dword lastbdowntime[8] = {0};
dword isdblclick[8] = {0};

dword mouseb;
dword modkeys;
bool  sdlMouseIsIn;

bool SDL2GUI::IsMouseIn()
{
	return sdlMouseIsIn;
}

dword SDL2GUI::GetMouseButtons()
{
	return mouseb;
}

dword SDL2GUI::GetModKeys()
{
	return modkeys;
}

void SDL2GUI::HandleSDLСобытие(SDL_Событие* event)
{
	LLOG("HandleSDLСобытие " << event->тип);
	SDL_Событие next_event;
	dword keycode;
	switch(event->тип) {
//		case SDL_ACTIVEEVENT: //SDL_ActiveСобытие
//			break;
	case SDL_TEXTINPUT: {
			//send respective keyup things as char events as well
		ШТкст text = event->text.text;
		for(int i = 0; i < text.дайСчёт(); i++) {
			int c = text[i];
			if(c != 127)
				Ктрл::DoKeyFB(c, 1);
		}
		break;
	}
	case SDL_KEYDOWN:
		switch(event->ключ.keysym.sym) {
			case SDLK_LSHIFT: modkeys |= KM_LSHIFT; break;
			case SDLK_RSHIFT: modkeys |= KM_RSHIFT; break;
			case SDLK_LCTRL: modkeys |= KM_LCTRL; break;
			case SDLK_RCTRL: modkeys |= KM_RCTRL; break;
			case SDLK_LALT: modkeys |= KM_LALT; break;
			case SDLK_RALT: modkeys |= KM_RALT; break;
		}
		
		keycode = fbKEYtoK((dword)event->ключ.keysym.sym);
		
		if(keycode != K_SPACE) { //dont send space on keydown
			static int repeat_count;
			SDL_PumpСобытиеs();
			if(SDL_PeepСобытиеs(&next_event, 1, SDL_PEEKEVENT, SDL_KEYDOWN, SDL_KEYDOWN) &&
			   next_event.ключ.keysym.sym == event->ключ.keysym.sym) {
				repeat_count++; // Keyboard repeat compression
				break;
			}
			Ктрл::DoKeyFB(keycode, 1 + repeat_count);
			repeat_count = 0;
		}
		break;
	case SDL_KEYUP: //SDL_KeyboardСобытие
		switch(event->ключ.keysym.sym) {
			case SDLK_LSHIFT: modkeys &= ~KM_LSHIFT; break;
			case SDLK_RSHIFT: modkeys &= ~KM_RSHIFT; break;
			case SDLK_LCTRL: modkeys &= ~KM_LCTRL; break;
			case SDLK_RCTRL: modkeys &= ~KM_RCTRL; break;
			case SDLK_LALT: modkeys &= ~KM_LALT; break;
			case SDLK_RALT: modkeys &= ~KM_RALT; break;
		}

		Ктрл::DoKeyFB(fbKEYtoK((dword)event->ключ.keysym.sym) | K_KEYUP, 1);
		break;
	case SDL_MOUSEMOTION:
		SDL_PumpСобытиеs();
		if(SDL_PeepСобытиеs(&next_event, 1, SDL_PEEKEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION) > 0)
			break; // двигМыши compression
		Ктрл::DoMouseFB(Ктрл::MOUSEMOVE, Точка(event->motion.x, event->motion.y));
		break;
	case SDL_MOUSEWHEEL:
		Ктрл::DoMouseFB(Ктрл::MOUSEWHEEL, дайПозМыши(), зн(event->wheel.y) * 120);
		break;
	case SDL_MOUSEBUTTONDOWN: {
			Точка p(event->button.x, event->button.y);
			int bi = event->button.button;
			dword ct = SDL_GetTicks();
			if(isdblclick[bi] && (абс(int(ct) - int(lastbdowntime[bi])) < 400))
			{
				switch(bi)
				{
					case SDL_BUTTON_LEFT: Ктрл::DoMouseFB(Ктрл::LEFTDOUBLE, p); break;
					case SDL_BUTTON_RIGHT: Ктрл::DoMouseFB(Ктрл::RIGHTDOUBLE, p); break;
					case SDL_BUTTON_MIDDLE: Ктрл::DoMouseFB(Ктрл::MIDDLEDOUBLE, p); break;
				}
				isdblclick[bi] = 0; //reset, to go ahead sending repeats
			}
			else
			{
				lastbdowntime[bi] = ct;
				isdblclick[bi] = 0; //prepare for repeat
				switch(bi)
				{
					case SDL_BUTTON_LEFT: mouseb |= (1<<0); Ктрл::DoMouseFB(Ктрл::LEFTDOWN, p); break;
					case SDL_BUTTON_RIGHT: mouseb |= (1<<1); Ктрл::DoMouseFB(Ктрл::RIGHTDOWN, p); break;
					case SDL_BUTTON_MIDDLE: mouseb |= (1<<2); Ктрл::DoMouseFB(Ктрл::MIDDLEDOWN, p); break;
				}
			}
		}
		break;
	case SDL_MOUSEBUTTONUP: {
			int bi = event->button.button;
			isdblclick[bi] = 1; //indicate maybe a dblclick
	
			Точка p(event->button.x, event->button.y);
			switch(bi)
			{
				case SDL_BUTTON_LEFT: mouseb &= ~(1<<0); Ктрл::DoMouseFB(Ктрл::LEFTUP, p); break;
				case SDL_BUTTON_RIGHT: mouseb &= ~(1<<1); Ктрл::DoMouseFB(Ктрл::RIGHTUP, p); break;
				case SDL_BUTTON_MIDDLE: mouseb &= ~(1<<2); Ктрл::DoMouseFB(Ктрл::MIDDLEUP, p); break;
			}
		}
		break;
/*		case SDL_VIDEORESIZE: //SDL_ResizeСобытие
		{
			width = event->resize.w;
			height = event->resize.h;
	
			SDL_FreeSurface(screen);
			screen = CreateScreen(width, height, bpp, videoflags);
			ПРОВЕРЬ(screen);
			Ктрл::SetFramebufferSize(Размер(width, height));
		}
			break;
		case SDL_VIDEOEXPOSE: //SDL_ExposeСобытие
			break;*/
	case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            break;
        case SDL_WINDOWEVENT_MOVED:
            break;
//		case SDL_WINDOWEVENT_SIZE_CHANGED:
//			SDLwidth = event->window.data1;
//			SDLheight = event->window.data2;
//      	break;
        case SDL_WINDOWEVENT_RESIZED:
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            break;
        case SDL_WINDOWEVENT_RESTORED:
            break;
        case SDL_WINDOWEVENT_ENTER:
			sdlMouseIsIn = true;
			Ктрл::PaintAll();
            break;
        case SDL_WINDOWEVENT_LEAVE:
			sdlMouseIsIn = false;
			Ктрл::PaintAll();
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            break;
        case SDL_WINDOWEVENT_CLOSE:
            break;
        }
		break;
	case SDL_QUIT: //SDL_QuitСобытие
		Ктрл::окончиСессию();
		break;
	}
}

bool SDL2GUI::обработайСобытие(bool *quit)
{
	bool ret = false;
	SDL_Событие event;
	if(SDL_PollСобытие(&event)) {
		if(event.тип == SDL_QUIT && quit)
			*quit = true;
		HandleSDLСобытие(&event);
		ret = true;
	}
	return ret;
}


void SDL2GUI::WaitСобытие(int ms)
{
	SDL_WaitСобытиеTimeout(NULL, ms);
}

bool SDL2GUI::ожидаетСобытие()
{
	SDL_PumpСобытиеs();
	SDL_Событие events;
	return SDL_PeepСобытиеs(&events, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) > 0;
}

SDL_TimerID waketimer_id = 0;
Uint32 WakeCb(Uint32 interval, void *param)
{
	//wake up message que, FIXME maybe it can be done better?
	SDL_Событие event;
	event.тип=SDL_USEREVENT;
	SDL_PushСобытие(&event);
	return 0;
}

void SDL2GUI::WakeUpGuiThread()
{
	waketimer_id = SDL_AddTimer(20, WakeCb, NULL);
}

}
