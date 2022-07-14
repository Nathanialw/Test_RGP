#pragma once
#include "components.h"
#include "graphics.h"

namespace UI {
	namespace {
		SDL_FRect charui = {0.0f, 0.0f, 600.0f, 1200.0f };
		float scale = 1.0f;
	}


	void Create_Bag() {
		auto item = Scenes::scene.create();

		
		Scenes::scene.emplace<Scale>(item, 1);

		Scenes::scene.emplace<Actions>(item, isStatic);
		Scenes::scene.emplace<Direction>(item, W);

		auto& x = Scenes::scene.emplace<Position_X>(item, 0.0f, 100.0f);
		auto& y = Scenes::scene.emplace<Position_Y>(item, 0.0f, 100.0f);

		Scenes::scene.emplace<Ground_Item>(item, x.fX - offset.offset.fX, y.fY - offset.offset.fY, 64.0f, 64.0f);
		//Scenes::scene.emplace<Item_Type>(item, weapon);
	}
}
