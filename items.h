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

	namespace {
		std::vector<int>bag;
		int mousePickUp;
	}


	void Create_And_Drop_Item(float& xDrop, float& yDrop) {
		float scale = 0.5f;
		
		auto item = Scenes::scene.create();

		Scenes::scene.emplace<animation>(item, Graphics::longsword_default); /// need to load hetexture	 only once and pass the pointer into this function
		Scenes::scene.get<animation>(item).sheet = {
			{{ 0, 0, 64, 64}, 0, 64, 0, 0, 0.0f, 16.0f } }; //populate the vector
		auto& offset = Scenes::scene.emplace<Sprite_Offset>(item, 64.0f * scale, 64.0f * scale);
		Scenes::scene.emplace<Scale>(item, scale);

		Scenes::scene.emplace<Actions>(item, isStatic);
		Scenes::scene.emplace<Direction>(item, W);

		auto& x = Scenes::scene.emplace<Position_X>(item, 0.0f, xDrop);
		auto& y = Scenes::scene.emplace<Position_Y>(item, 0.0f, yDrop);		
	
		Scenes::scene.emplace<Ground_Item>(item, x.fX, y.fY, 64.0f, 64.0f);
		//Scenes::scene.emplace<Assigned>(item);	//stores the entity holding the item	
		
		//Scenes::scene.emplace<Item_Type>(item, weapon);
	}

	void Pick_Up_Item() {
		//check if unit it close enough to item
		if (/*unit rect is inside item rect*/1) {
			//check if mouse is inside item box
			if (/*mouse rect is inside item rect*/1) {

				//place item in mouse array on click and 
					//remove ground item component, 
					//replace  x and y coords with mouse coords, update every frame

					// add ground item component and replace x and y with mouse coords on click

					// if over bag
						//add item to bag array and 
						// place x,y coords in empty array slot coords update every frame the UI is open
			}
		}
	}



}