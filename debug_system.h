#pragma once
#include "components.h"
#include "graphics.h"
#include "scene.h"
#include "sstream"
#include "timer.h"
#include "mouse_control.h"
#include <algorithm>
using namespace Scene;


namespace Debug_System {

	int iFramePollRate = 0;
	


	void Entity_Data_Debug(float x, float y, float sx, float sy) {
	//displays :
		//x,y position
		//collision radius
	//	mass
		//SDL_Rect j = { sx, sy, 30, 30 };
		////SDL_RenderDrawRect(Graphics::renderer, &j);			
		Graphics::Surface_Data x_Position = Graphics::Load_Text_Texture(std::to_string(int(x)), { 255,0,133 });
		Graphics::Surface_Data y_Position = Graphics::Load_Text_Texture(std::to_string(int(y)), { 255,133,0	});
		//Surface_Data collision_Radius = Graphics::Load_Text_Texture(std::to_string(mas), { 133,133,133 }, renderer);
		//Surface_Data mass = Graphics::Load_Text_Texture(std::to_string(radius), { 133,133,133 }, renderer);

		//SDL_Rect e = { sx - 5, sy - 5, 27, 50 };
		//SDL_RenderDrawRect(renderer, &e);
		////SDL_RenderCopy(renderer, mass.pTexture, , );

		SDL_Rect a = {sx+25, sy-20, 15, 15};
		SDL_RenderCopy(Graphics::renderer, x_Position.pTexture, &x_Position.k, &a);
		SDL_Rect b = { sx+25, sy-10, 15, 15 };
		SDL_RenderCopy(Graphics::renderer, y_Position.pTexture, &y_Position.k,  &b);
		//SDL_Rect c = { sx, sy+20, 15, 10 };
		//SDL_RenderCopy(renderer, collision_Radius.pTexture, &collision_Radius.k, &c);
		//SDL_Rect d = { sx, sy+30, 15, 10 };
		//SDL_RenderCopy(renderer, mass.pTexture, &mass.k, &d);			
		//SDL_DestroyTexture(collision_Radius.pTexture);
		//SDL_DestroyTexture(mass.pTexture);
		SDL_DestroyTexture(x_Position.pTexture);
		SDL_DestroyTexture(y_Position.pTexture);
	}

	Graphics::Surface_Data framerate;
	Graphics::Surface_Data timeStep;
	bool frameRateMode = true;
	bool frameTimeMode = false;

	void Toggle_Count_Rate_Mode() {
		if (frameRateMode) {
			frameRateMode = false;
			frameTimeMode = true;			
		}
		else {
			frameRateMode = true;
			frameTimeMode = false;
		}
		iFramePollRate = 500;
	}

	void Framerate() {		
		auto view = Scenes::scene.view<Camera, Position_X, Position_Y>();

		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);
			auto& x = view.get<Position_X>(focus);
			auto& y = view.get<Position_Y>(focus);

			iFramePollRate += Timer::timeStep;
			if (iFramePollRate >= 500) {
				iFramePollRate = 0;
				if (frameRateMode) {
					SDL_DestroyTexture(framerate.pTexture);
					framerate = Graphics::Load_Text_Texture(std::to_string(float(Timer::avgFPS)), { 133,255,133 });
				}
				if (frameTimeMode) {
					SDL_DestroyTexture(timeStep.pTexture);
					timeStep = Graphics::Load_Text_Texture(std::to_string(float(Timer::timeStep) / 1000), { 133,255,133 });
				}
			}
			SDL_Rect c = { 0, 0, 200 / componentCamera.scale.fX, 100 / componentCamera.scale.fY };
			if (frameRateMode) {
				SDL_RenderCopy(Graphics::renderer, framerate.pTexture, &framerate.k, &c);
			}
			if (frameTimeMode) {
				SDL_RenderCopy(Graphics::renderer, timeStep.pTexture, &timeStep.k, &c);
			}
		}
	}


	

		void Debugger() {
		Framerate();
		}
}





