#pragma once
#include "entt.hpp"
#include <SDL.h>
#include "components.h"


namespace Camera_Control {

	class camera_object {



	};

	void Update_Camera_Follow(Components::Camera &camera, Components::Position &position, SDL_FRect &resolution) {
			//center camera on the entity with the component
			camera.screen.w = resolution.w / camera.scale.fX;
			camera.screen.h = resolution.h / camera.scale.fY;
			camera.screen.x = ((position.fX) - (camera.screen.w / 2));
			camera.screen.y = ((position.fY) - (camera.screen.h / 2));
	}

	SDL_FRect Convert_Rect_To_Screen_Coods(entt::registry& zone, SDL_FRect& frect) {
		auto view = zone.view<Components::Camera>();
		SDL_FRect screenRect = {};
		
		for (auto focus : view) {
			auto& componentCamera = view.get<Components::Camera>(focus);


			screenRect.x = frect.x - componentCamera.screen.x;
			screenRect.y = frect.y - componentCamera.screen.y;
			screenRect.w = frect.w;
			screenRect.h = frect.h;
			return screenRect;
		}
		return screenRect;
	}


	SDL_Rect Convert_Rect_To_Scale(entt::registry &zone, SDL_Rect& rect) {
		auto view = zone.view<Components::Camera>();

		for (auto focus : view) {
			auto& componentCamera = view.get<Components::Camera>(focus);

			SDL_FRect fRenderToScreen = {
				float(rect.x) / componentCamera.scale.fX,
				float(rect.y) / componentCamera.scale.fY,
				(float(rect.w) / componentCamera.scale.fX),
				(float(rect.h) / componentCamera.scale.fY) };

			return Utilities::SDL_FRect_To_SDL_Rect(fRenderToScreen);
		}
	}

	SDL_Point Convert_Point_To_Scale(entt::registry& zone, SDL_Point& rect) {
		auto view = zone.view<Components::Camera>();

		for (auto focus : view) {
			auto& componentCamera = view.get<Components::Camera>(focus);

			SDL_FPoint fRenderToScreen = {
				float(rect.x) / componentCamera.scale.fX,
				float(rect.y) / componentCamera.scale.fY };

			return Utilities::SDL_FPoint_to_Point(fRenderToScreen);
		}
	}
}