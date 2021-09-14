#pragma once
#include "SDL.h"
#include <iostream>


namespace Timer {
	
	Uint32 timeStep = 0;
	Uint32 startTime = 0;
	int frameCount = 0;
	float avgFPS;
	

	void frameTime() {
		Timer::frameCount++;
		Uint32 frequency = SDL_GetTicks();
		Timer::timeStep = (frequency - Timer::startTime);
		avgFPS = 1.0f / (timeStep / 1000.f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}
		startTime = frequency;		
	}


	class Frame_Timer {
		float fTime_between;
		float fCounter_MS;

		public:
		Frame_Timer(float frequency) {
			fTime_between = frequency;
			fCounter_MS = 0.0f;
		}

		bool Calc() { //controls how often collision calculates
			fCounter_MS -= Timer::timeStep;
			if (fCounter_MS <= 0) {
				fCounter_MS = fTime_between; //every this many milliseconds
				return true;
			}
			else return false;
		}
	};

}