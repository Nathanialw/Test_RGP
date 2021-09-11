#pragma once
#include <SDL.h>
#include "mouse_control.h"
#include "graphics.h"
#include "debug_system.h"






namespace Interface {

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

	void Update_Mouse_And_Camera() {
		auto view = scene.view<Camera, Position_X, Position_Y>();
		for (auto focus : view) {
			auto& x = view.get<Position_X>(focus);
			auto& y = view.get<Position_Y>(focus);
			auto& componentCamera = view.get<Camera>(focus);

			//center camera on the entity with the component
			//scales the x, y position for the source of camera amd scales the screen after the offset is applied
			componentCamera.screen.x = (x.fX * componentCamera.scale.fX - componentCamera.screen.w) / componentCamera.scale.fX;
			componentCamera.screen.y = (y.fY * componentCamera.scale.fY - componentCamera.screen.h) / componentCamera.scale.fY;
			//updates the global variable that may be useful for getting scrren/world positions
			Graphics::Screen = componentCamera.screen;


			//update mouse
			SDL_GetMouseState(&Mouse::iXMouse, &Mouse::iYMouse);
			Mouse::iXWorld_Mouse = (Mouse::iXMouse / componentCamera.scale.fX) + componentCamera.screen.x;//getting mouse world Position corrected for scale
			Mouse::iYWorld_Mouse = (Mouse::iYMouse / componentCamera.scale.fY) + componentCamera.screen.y;//getting mouse world Position corrected for scale
			Mouse::iXMouse = Mouse::iXMouse / componentCamera.scale.fX;  // getting the screen mouse position corrected for scale
			Mouse::iYMouse = Mouse::iYMouse / componentCamera.scale.fY;  // getting the screen mouse position corrected for scale	
			
		}
	}

	void Display_Selected() {
		auto view = scene.view<Position_X, Position_Y, Mass, Collision_Radius, Selected>();

		SDL_Color a = { 155, 255, 50, 255 };
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& c = view.get<Mass>(entity);
			auto& d = view.get<Collision_Radius>(entity);

			SDL_SetRenderDrawColor(renderer, 155,155,155,255);
			SDL_Rect p = {x.fSX - 15, y.fSY - 15, 30, 30};
			SDL_RenderDrawRect(Graphics::renderer, &p);
			//Debug_System::Entity_Data_Debug(x.fX, y.fY, x.fSX, y.fSY);
		}
	}


	void Run_Interface() {
		Debug_System::Debugger();
		Update_Mouse_And_Camera();
		Display_Selected();
		SDL_Color a = {235, 201, 100, 255};
		if (Mouse::bPressed) {
			SDL_SetRenderDrawColor(renderer, 155, 155, 155, 255);
			SDL_Rect p = { Mouse::Mouse_Selection_Box_x_Display, Mouse::Mouse_Selection_Box_y_Display, Mouse::iXMouse - Mouse::Mouse_Selection_Box_x_Display, Mouse::iYMouse - Mouse::Mouse_Selection_Box_y_Display };
			SDL_RenderDrawRect(Graphics::renderer, &p);
		}
		
	}
}