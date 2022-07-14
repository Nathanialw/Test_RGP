#pragma once
#include "scenes.h"
#include "mouse_control.h"
#include "components.h"
#include "item_components.h"
#include "graphics.h"

// item

// item equip

//item bag

//item drop

using namespace Components;
using namespace Item_Components;

namespace Items {

	void Drop_Item(float& xDrop, float& yDrop) {
		auto item = Scenes::scene.create();

		Scenes::scene.emplace<animation>(item, Graphics::longsword_default); /// need to load hetexture	 only once and pass the pointer into this function
		Scenes::scene.get<animation>(item).sheet = {
			{{ 0, 0, 64, 64}, 0, 64, 0, 0, 0.0f, 16.0f } }; //populate the vector
		auto& offset = Scenes::scene.emplace<Sprite_Offset>(item, 32.0f, 32.0f);
		Scenes::scene.emplace<Scale>(item, 0.25f);

		Scenes::scene.emplace<Actions>(item, isStatic);
		Scenes::scene.emplace<Direction>(item, W);

		auto& x = Scenes::scene.emplace<Position_X>(item, 0.0f, xDrop);
		auto& y = Scenes::scene.emplace<Position_Y>(item, 0.0f, yDrop);		
	
		Scenes::scene.emplace<Ground_Item>(item, x.fX-offset.offset.fX, y.fY-offset.offset.fY, 64.0f, 64.0f);		
		//Scenes::scene.emplace<Assigned>(item);	//stores the entity holding the item	
		
		//Scenes::scene.emplace<Item_Type>(item, weapon);
	}



}