#pragma once
#include "components.h"
#include "graphics.h"
#include "pools.h"
#include "sstream"
#include "timer.h"

using namespace Scene;

namespace Debug_System {
	int iFramePollRate = 500;
	Surface_Data framerate;



	void Entity_Data_Debug(float x, float y, float sx, float sy, float mas, float radius) {
	

		//displays :
		//	x,y position
		//	collision radius
		//	mass
			//SDL_Rect j = { sx, sy, 30, 30 };
			////SDL_RenderDrawRect(Graphics::renderer, &j);			
			//Surface_Data x_Position = Graphics::Load_Text_Texture(std::to_string(x), { 133,133,133 }, renderer);
			//Surface_Data y_Position = Graphics::Load_Text_Texture(std::to_string(y), { 133,133,133 }, renderer);
			//Surface_Data collision_Radius = Graphics::Load_Text_Texture(std::to_string(mas), { 133,133,133 }, renderer);
			//Surface_Data mass = Graphics::Load_Text_Texture(std::to_string(radius), { 133,133,133 }, renderer);

			//SDL_Rect e = { sx - 5, sy - 5, 27, 50 };
			//SDL_RenderDrawRect(renderer, &e);
			////SDL_RenderCopy(renderer, mass.pTexture, , );
		

			//SDL_Rect a = {sx, sy, 15, 10};
			//SDL_RenderCopy(renderer, x_Position.pTexture, &x_Position.k, &a);
			//SDL_Rect b = { sx, sy+10, 15, 10 };
			//SDL_RenderCopy(renderer, y_Position.pTexture, &y_Position.k, &b);
			//SDL_Rect c = { sx, sy+20, 15, 10 };
			//SDL_RenderCopy(renderer, collision_Radius.pTexture, &collision_Radius.k, &c);
			//SDL_Rect d = { sx, sy+30, 15, 10 };
			//SDL_RenderCopy(renderer, mass.pTexture, &mass.k, &d);			
			//SDL_DestroyTexture(x_Position.pTexture);
			//SDL_DestroyTexture(y_Position.pTexture);
			//SDL_DestroyTexture(collision_Radius.pTexture);
			//SDL_DestroyTexture(mass.pTexture);
		}


	void Framerate(int sx, int sy) {

		
		Debug_System::iFramePollRate -= Timer::timeStep;

		if (Debug_System::iFramePollRate <= 0) {
			Debug_System::iFramePollRate = 500;			
			SDL_DestroyTexture(framerate.pTexture);
			framerate = Load_Text_Texture(std::to_string(int(Timer::avgFPS)), { 133,255,133 }, Graphics::renderer);
		}

		SDL_Rect c = { sx, sy, 50, 50 };
	//	SDL_RenderSetViewport(renderer, &c);
		SDL_RenderCopy(Graphics::renderer, framerate.pTexture, &framerate.k, &c);
		
	//	SDL_RenderSetClipRect(renderer, &Graphics::Screen);		
	}





	void Debugger() {
		auto view = scene.view<Position_X, Position_Y, Mass, Collision_Radius>();
		for (auto entity : view) {
			auto& a = view.get<Position_X>(entity);
			auto& b = view.get<Position_Y>(entity);
			auto& c = view.get<Mass>(entity);
			auto& d = view.get<Collision_Radius>(entity);

			Entity_Data_Debug(a.fX, b.fY, a.fSX, b.fSY, c.fKilos, d.fCollisionRadius);
		}
		Framerate(0,0);
	}
}





