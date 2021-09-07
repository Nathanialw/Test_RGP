#include <SDL.h>
#include <SDL_ttf.h>
//#include "Joystick.h"

namespace Init {


	void init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			SDL_Log("init error", SDL_GetError());
		}
	//	Joystick::JoystickInit();
		TTF_Init();
		



	}
}