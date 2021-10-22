#include <SDL.h>
#include <SDL_ttf.h>
//#include "Joystick.h"

namespace Init {


	void init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			SDL_Log("init error");
		}
	//	Joystick::JoystickInit();
		TTF_Init();
		
		
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}