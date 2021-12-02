#pragma once
#include <SDL.h>
#include "mouse_control.h"
#include "graphics.h"
#include "debug_system.h"


namespace Interface {

	int iMousePollRate;
	Surface_Data mouseX;
	Surface_Data mouseY;


	void Update_Zoom(SDL_Event& e) {
		auto view = scene.view<Camera>();
		for (auto focus : view) {
			auto& x = view.get<Camera>(focus);
			if (e.wheel.y > 0) {
				x.scale.fX *= 1.1f;
				x.scale.fY *= 1.1f;
				SDL_RenderSetScale(renderer, x.scale.fX, x.scale.fY);
			}
			if (e.wheel.y < 0) {
				x.scale.fX *= 0.9f;
				x.scale.fY *= 0.9f;
				SDL_RenderSetScale(renderer, x.scale.fX, x.scale.fY);
			}
		}
	}

	void Display_Selected() {
		auto camera_view = scene.view<Camera>();
		for (auto cameras : camera_view) {
			auto& cam = camera_view.get<Camera>(cameras);

			auto squad_view = scene.view<Squad>();
			for (auto squads : squad_view) {
				auto& x = squad_view.get<Squad>(squads);
				SDL_FRect o = x.sCollide_Box;
				o.x -= cam.screen.x;
				o.y -= cam.screen.y;
				SDL_SetRenderDrawColor(renderer, 155, 55, 255, 255);
				SDL_RenderDrawRectF(Graphics::renderer, &o);
			}

			auto platoon_view = scene.view<Platoon>();
			for (auto platoons : platoon_view) {
				auto& platoon = platoon_view.get<Platoon>(platoons);
				SDL_FRect o = platoon.sCollide_Box;
				o.x = platoon.sCollide_Box.x - cam.screen.x;
				o.y = platoon.sCollide_Box.y - cam.screen.y;
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawRectF(renderer, &o);
			}		
			
			auto company_view = scene.view<Company>();
			for (auto companies : company_view) {
				auto& company = company_view.get<Company>(companies);
				SDL_FRect o = company.sCollide_Box;
				o.x = company.sCollide_Box.x - cam.screen.x;
				o.y = company.sCollide_Box.y - cam.screen.y;
				SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
				SDL_RenderDrawRectF(renderer, &o);
			}
		}
		auto view = scene.view<Position_X, Position_Y, Mass, Radius, Selected>();
		SDL_Color a = { 155, 255, 50, 255 };
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& c = view.get<Mass>(entity);
			auto& d = view.get<Radius>(entity);
			SDL_SetRenderDrawColor(renderer, 55,255,55,255);
			SDL_Rect p = {x.fSX - 15, y.fSY - 15, 30, 30};
			SDL_RenderDrawRect(Graphics::renderer, &p);		
			//Debug_System::Entity_Data_Debug(x.fX, y.fY, x.fSX, y.fSY);
		}
	}


	void Display_Selection_Box() {
		if (Mouse::bLeft_Mouse_Pressed) {
			SDL_SetRenderDrawColor(renderer, 55, 255, 55, 255);
			SDL_Rect p = { Mouse::Mouse_Selection_Box_x_Display, Mouse::Mouse_Selection_Box_y_Display, Mouse::iXMouse - Mouse::Mouse_Selection_Box_x_Display, Mouse::iYMouse - Mouse::Mouse_Selection_Box_y_Display };
			SDL_RenderDrawRect(Graphics::renderer, &p);
		}
	}


	void Display_Mouse() {
		auto view = scene.view<Camera, Position_X, Position_Y>();
		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);
			auto& x = view.get<Position_X>(focus);
			auto& y = view.get<Position_Y>(focus);
			SDL_Color b = { 255, 255, 255, 255 };

			iMousePollRate -= Timer::timeStep;
			if (iMousePollRate <= 0) {
				iMousePollRate = 500;
				SDL_DestroyTexture(mouseX.pTexture);
				SDL_DestroyTexture(mouseY.pTexture);
				mouseX = Load_Text_Texture(std::to_string(Mouse::iXWorld_Mouse), { 133,255,133 });
				mouseY = Load_Text_Texture(std::to_string(Mouse::iYWorld_Mouse), { 133,255,133 });
			}
			SDL_SetRenderDrawColor(renderer, 155, 55, 255, 255);
			//GPU_Rect c = { Mouse::iXMouse, Mouse::iYMouse, 50, 50 };
			SDL_Rect d = { Mouse::iXMouse, Mouse::iYMouse, 50 / componentCamera.scale.fX, 50 / componentCamera.scale.fY };
			SDL_RenderDrawRect(Graphics::renderer, &d);

			//GPU_Rect v = Mouse::mouse_cursor;
			//v.y = v.y + 50;

			//SDL_RenderCopy(Graphics::renderer, mouseX.pTexture, &mouseX.k, &d);
			//SDL_RenderCopy(Graphics::renderer, mouseY.pTexture, &mouseY.k, &d);
		}
	}
		


	void Unit_Arrive_UI() {
		auto view = scene.view<Commanded_Move>();
		auto view2 = scene.view<Camera>();
		for (auto camera : view2) {
			auto& cam = view2.get<Camera>(camera);
			for (auto entity : view) {
				auto& mov = view.get<Commanded_Move>(entity);				
				SDL_Rect o = { (mov.fX_Destination - 15) -  cam.screen.x, (mov.fY_Destination - 15) - cam.screen.y, 30, 30 };				
				SDL_SetRenderDrawColor(renderer, 155, 155, 255, 255);				
				SDL_RenderFillRect(renderer, &o);
			}
		}
	}

	void Run_Interface() {
		Debug_System::Debugger();
		Unit_Arrive_UI();
		Display_Selected();
		Display_Mouse();
		Display_Selection_Box();
	}



}