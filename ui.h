#pragma once
#include "components.h"
#include "graphics.h"

namespace UI {
	namespace {
		bool toggleBag = false;
		SDL_Rect charui = { 0, 0, 554, 1080 };
		float scale = 1.0f;
		SDL_FRect screenPosition = { 100.0f, 100.0f, 554.0f, 1080.0f };
		
		
		//x 32
		//y 544
		DataTypes::i2d bagoffset = { 32, 544 };
		int bagslotsize = 64; // or 32 not even sure		
		std::vector<entt::entity>UI_bagSlots;
		
		//calculate slot position based on which slot its in and save the rect to the Icon component

		
	}

	SDL_FRect Convert_Rect_To_Scale(SDL_FRect &rect) {
		auto view = Scenes::scene.view<Camera>();
		
		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);
			SDL_FRect renderToScreen = {
				rect.x / componentCamera.scale.fX,
				rect.y / componentCamera.scale.fY,
				(rect.w / componentCamera.scale.fX),
				(rect.h / componentCamera.scale.fY) };
			return renderToScreen;
		}
	}

	void Place_Item_In_Bag() {
		if (toggleBag) {

		}
	}

	void Create_Bag_UI() {
		//for (int i = 0; i < 16; i++) {
			//I_bagSlots.at(i) = { };
			//charui.x + bagslotsize, charui. + bagslotsize
		//}
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
			SDL_FRect renderRect = Convert_Rect_To_Scale(screenPosition);
			SDL_RenderCopyF(Graphics::renderer, Graphics::itsmars_Inventory, &charui, &renderRect);
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
