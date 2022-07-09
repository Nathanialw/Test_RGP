#pragma once
#include "scenes.h"
#include "components.h"
#include "graphics.h"

// item

// item equip

//item bag

//item drop

using namespace Components;

namespace Items {

	void Create_Item() {
		auto item = Scenes::scene.create();

		Scenes::scene.emplace<animation>(item, Graphics::axe, 0, 0, 100, 100, 0, 0, 100, 100); /// need to load hetexture	 only once and pass the pointer into this function
		Scenes::scene.get<animation>(item).sheet = {
			{{ 1, 1, 235, 235}, 0, 234, 0, 0, {0, 0}, 16.0f } }; //populate the vector

		Scenes::scene.emplace<Position_X>(item, 100.0f, 200.0f);
		Scenes::scene.emplace<Position_Y>(item, 100.0f, 200.0f);
		
		
		
		Scenes::scene.emplace<Item_Type>(item, weapon);
	}

}