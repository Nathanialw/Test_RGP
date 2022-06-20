#pragma once
#include "SDL.h"
#include <iostream>


namespace Timer {
	
	int64_t timeStep = 0;
	int64_t frameCount = 0;
	int64_t avgFPS = 0;
	int64_t prevFrame = 0;
	int64_t currentFrame = 0;
	bool pause = false;

	void Pause_Control() {
		if (pause) {
			pause = false;
		}
		else {
			pause = true;
		}
	}

	void frameTime() {
		frameCount++;
		int64_t i = 1;
		int64_t j = 1000;
		currentFrame = SDL_GetTicks64();
		timeStep = currentFrame - prevFrame;
		if (timeStep != 0) {
			//avgFPS = 1 / timeStep * j;
			//avgFPS = frameCount / currentFrame * 1000;
			avgFPS = 1000 / (currentFrame - prevFrame);
		}
		if (avgFPS > 2000000)
		{
			//avgFPS = 0;
		}
		if (pause) {
			timeStep = 0;
		}
		prevFrame = currentFrame;
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
			if (fCounter_MS <= 0.0f) {
				fCounter_MS = fTime_between; //every this many milliseconds
				return true;
			}
			else return false;
		}
	};

}