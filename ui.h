#pragma once
#include "components.h"
#include "graphics.h"

namespace UI {
	namespace {
		bool toggleBag = false;
		SDL_Rect charui = { 0, 0, 554, 1080 };
		float scale = 1.0f;
		SDL_FRect screenPosition = { 100.0f, 100.0f, 554.0f, 1080.0f };
		
		int bagslotsize = 16;
		DataTypes::i2d bagslotoffset;
		std::vector<DataTypes::i2d>UI_bagSlots;

	}

	void Create_Bag_UI() {
		for (int i = 0; i < 16, i++;) {
			UI_bagSlots.at(i) = { charui.x + bagslotsize, charui.y + bagslotsize };

		}
	}

	void Toggle_Bag() {
		if (!toggleBag) {
			toggleBag = true;
		}
		else {
			toggleBag = false;
		}
	}

	void Render_UI() {
		if (toggleBag) {
			auto view = Scenes::scene.view<Camera>();
			for (auto focus : view) {
				auto& componentCamera = view.get<Camera>(focus);
				SDL_FRect renderToScreen = {
					screenPosition.x / componentCamera.scale.fX,
					screenPosition.y / componentCamera.scale.fY,
					(screenPosition.w / componentCamera.scale.fX),
					(screenPosition.h / componentCamera.scale.fY) };
				
				SDL_RenderCopyF(Graphics::renderer, Graphics::itsmars_Inventory, &charui, &renderToScreen);
			}
		}
	}

	//void Create_Bag() {
	//	auto item = Scenes::scene.create();

	//	
	//	Scenes::scene.emplace<Scale>(item, 1);

	//	Scenes::scene.emplace<Actions>(item, isStatic);
	//	Scenes::scene.emplace<Direction>(item, W);

	//	auto& x = Scenes::scene.emplace<Position_X>(item, 0.0f, 100.0f);
	//	auto& y = Scenes::scene.emplace<Position_Y>(item, 0.0f, 100.0f);

	//	Scenes::scene.emplace<Ground_Item>(item, x.fX - offset.offset.fX, y.fY - offset.offset.fY, 64.0f, 64.0f);
	//	//Scenes::scene.emplace<Item_Type>(item, weapon);
	//}
}
