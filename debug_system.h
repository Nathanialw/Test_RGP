#pragma once
#include "components.h"
#include "graphics.h"
#include "scene.h"
#include "sstream"
#include "timer.h"
#include "mouse_control.h"

using namespace Scene;


namespace Debug_System {

	int iFramePollRate;
	int iMousePollRate;
	Surface_Data framerate;
	Surface_Data mouseX;
	Surface_Data mouseY;

	void Entity_Data_Debug(float x, float y, float sx, float sy) {
		//displays :
		//	x,y position
		//	collision radius
		////	mass
		//	//SDL_Rect j = { sx, sy, 30, 30 };
		//	////SDL_RenderDrawRect(Graphics::renderer, &j);			
		//	Surface_Data x_Position = Graphics::Load_Text_Texture(std::to_string(int(x)), { 255,0,133 });
		//	Surface_Data y_Position = Graphics::Load_Text_Texture(std::to_string(int(y)), { 255,133,0	});
		//	//Surface_Data collision_Radius = Graphics::Load_Text_Texture(std::to_string(mas), { 133,133,133 }, renderer);
		//	//Surface_Data mass = Graphics::Load_Text_Texture(std::to_string(radius), { 133,133,133 }, renderer);

		//	//SDL_Rect e = { sx - 5, sy - 5, 27, 50 };
		//	//SDL_RenderDrawRect(renderer, &e);
		//	////SDL_RenderCopy(renderer, mass.pTexture, , );

		//	GPU_Rect a = {sx+25, sy-20, 15, 15};
		//	GPU_BlitRect(x_Position.pTexture, &x_Position.k, renderer, &a);
		//	GPU_Rect b = { sx+25, sy-10, 15, 15 };
		//	GPU_BlitRect(y_Position.pTexture, &y_Position.k, renderer, &b);
		//	//SDL_Rect c = { sx, sy+20, 15, 10 };
		//	//SDL_RenderCopy(renderer, collision_Radius.pTexture, &collision_Radius.k, &c);
		//	//SDL_Rect d = { sx, sy+30, 15, 10 };
		//	//SDL_RenderCopy(renderer, mass.pTexture, &mass.k, &d);			
		//	//SDL_DestroyTexture(collision_Radius.pTexture);
		//	//SDL_DestroyTexture(mass.pTexture);
		//	GPU_FreeImage(x_Position.pTexture);
		//	GPU_FreeImage(y_Position.pTexture);
		}

	void Framerate() {		
		auto view = scene.view<Camera, Position_X, Position_Y>();
		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);
			auto& x = view.get<Position_X>(focus);
			auto& y = view.get<Position_Y>(focus);

			iFramePollRate -= Timer::timeStep;
			if (iFramePollRate <= 0) {
				iFramePollRate = 500;
				SDL_DestroyTexture(framerate.pTexture);
				framerate = Load_Text_Texture(std::to_string(int(Timer::avgFPS)), { 133,255,133 });
			}
			SDL_Rect c = { 0,0,50 / componentCamera.scale.fX,50 / componentCamera.scale.fY };
			SDL_RenderCopy(Graphics::renderer, framerate.pTexture, &framerate.k, &c);
		}
	}
//
	void Debug_Mouse() {
		auto view = scene.view<Camera, Position_X, Position_Y>();
		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);
			auto& x = view.get<Position_X>(focus);
			auto& y = view.get<Position_Y>(focus);
			SDL_Color b = { 255, 255, 255, 255 };

			Debug_System::iMousePollRate -= Timer::timeStep;
			if (iMousePollRate <= 0) {
				iMousePollRate = 500;
				SDL_DestroyTexture(mouseX.pTexture);
				SDL_DestroyTexture(mouseY.pTexture);
				mouseX = Load_Text_Texture(std::to_string(Mouse::iXWorld_Mouse), { 133,255,133 });
				mouseY = Load_Text_Texture(std::to_string(Mouse::iYWorld_Mouse), { 133,255,133 });
			}
			SDL_SetRenderDrawColor(renderer, 155, 55, 255, 255);
			//GPU_Rect c = { Mouse::iXMouse, Mouse::iYMouse, 50, 50 };
			SDL_Rect d = {Mouse::iXMouse, Mouse::iYMouse, 50, 50 };
			SDL_RenderDrawRect(Graphics::renderer, &d);

			//GPU_Rect v = Mouse::mouse_cursor;
			//v.y = v.y + 50;

			//SDL_RenderCopy(Graphics::renderer, mouseX.pTexture, &mouseX.k, &d);
			//SDL_RenderCopy(Graphics::renderer, mouseY.pTexture, &mouseY.k, &d);
		}
	}

		void Debugger() {
			
		Debug_Mouse();
		Framerate();
		}
}





