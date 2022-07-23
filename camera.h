#pragma once
#include <SDL_assert.h>
#include "components.h"
#include "scenes.h"


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
			Mouse::mousePoint = { mx, my };
		}
	}

	SDL_FRect Convert_Rect_To_Screen_Coods(SDL_FRect& frect) {
		auto view = Scenes::scene.view<Camera>();
		SDL_FRect screenRect = {};
		
		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);


			screenRect.x = frect.x - componentCamera.screen.x;
			screenRect.y = frect.y - componentCamera.screen.y;
			screenRect.w = frect.w;
			screenRect.h = frect.h;
			return screenRect;
		}
		return screenRect;
	}


	SDL_Rect Convert_Rect_To_Scale(SDL_Rect& rect) {
		auto view = Scenes::scene.view<Camera>();

		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);

			SDL_FRect fRenderToScreen = {
				float(rect.x) / componentCamera.scale.fX,
				float(rect.y) / componentCamera.scale.fY,
				(float(rect.w) / componentCamera.scale.fX),
				(float(rect.h) / componentCamera.scale.fY) };

			return Utilities::SDL_FRect_To_SDL_Rect(fRenderToScreen);
		}
	}

	SDL_Point Convert_Point_To_Scale(SDL_Point& rect) {
		auto view = Scenes::scene.view<Camera>();

		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);

			SDL_FPoint fRenderToScreen = {
				float(rect.x) / componentCamera.scale.fX,
				float(rect.y) / componentCamera.scale.fY };

			return Utilities::SDL_FPoint_to_Point(fRenderToScreen);
		}
	}
}