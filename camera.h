#pragma once
#include <SDL_assert.h>
#include "scene.h"


namespace Camera_Control {

	void Update_Camera() {
		auto view = Scenes::scene.view<Camera, Position>();
		for (auto focus : view) {
			auto& x = view.get<Position>(focus);
			auto& y = view.get<Position>(focus);
			auto& componentCamera = view.get<Camera>(focus);

			//center camera on the entity with the component
			componentCamera.screen.w = Graphics::resolution.w / componentCamera.scale.fX;
			componentCamera.screen.h = Graphics::resolution.h / componentCamera.scale.fY;
			componentCamera.screen.x = ((x.fX) - (componentCamera.screen.w / 2));
			componentCamera.screen.y = ((y.fY) - (componentCamera.screen.h / 2));

			//std::cout << componentCamera.screen.x << std::endl;
			//updates the global variable that may be useful for getting scrren/world positions
			Graphics::Screen = componentCamera.screen;
			//update mouse
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			Mouse::iXMouse = (float)mx;
			Mouse::iYMouse = (float)my;
			Mouse::iXWorld_Mouse = (Mouse::iXMouse / componentCamera.scale.fX) + componentCamera.screen.x;//getting mouse world Position corrected for scale
			Mouse::iYWorld_Mouse = (Mouse::iYMouse / componentCamera.scale.fY) + componentCamera.screen.y;//getting mouse world Position corrected for scale
			Mouse::iXMouse = Mouse::iXMouse / componentCamera.scale.fX;  // getting the screen mouse position corrected for scale
			Mouse::iYMouse = Mouse::iYMouse / componentCamera.scale.fY;  // getting the screen mouse position corrected for scale	
		}
	}

	SDL_FRect Convert_Rect_To_Screen_Coods(SDL_FRect& frect) {
		auto view = Scenes::scene.view<Camera>();
		
		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);

			SDL_FRect screenRect = {};

			screenRect.x = frect.x - componentCamera.screen.x;
			screenRect.y = frect.y - componentCamera.screen.y;
			screenRect.w = frect.w;
			screenRect.h = frect.h;
			return screenRect;
		}
		
	}
}