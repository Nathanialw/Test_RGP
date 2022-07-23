#pragma once
#include <vector>
#include "scenes.h"
#include "mouse_control.h"
#include "components.h"
#include "item_components.h"
#include "graphics.h"
#include "utilities.h"
#include "camera.h"


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
		bool pick_up_debug = true;
	}


	void Create_And_Drop_Item(Position position) {
		float scale = 0.5f;
		
		auto item = Scenes::scene.create();

		Scenes::scene.emplace<animation>(item, Graphics::longsword_default); /// need to load hetexture	 only once and pass the pointer into this function
		Scenes::scene.get<animation>(item).sheet = {
			{{ 0, 0, 64, 64}, 0, 64, 0, 0, 0.0f, 16.0f } }; //populate the vector
		auto& offset = Scenes::scene.emplace<Sprite_Offset>(item, 32.0f * scale, 32.0f * scale);
		Scenes::scene.emplace<Scale>(item, scale);

		auto &icon = Scenes::scene.emplace<Icon>(item, Graphics::longsword_default_icon);
		icon.clipSprite = { 0, 0, 32, 32 };
		icon.renderRectSize = { 48, 48 };
		icon.renderPositionOffset = { icon.renderRectSize.x / 2, icon.renderRectSize.y / 2 };

		Scenes::scene.emplace<Actions>(item, isStatic);
		Scenes::scene.emplace<Direction>(item, W);

		auto& position2 = Scenes::scene.emplace<Position>(item, position.fX, position.fY);
		Scenes::scene.emplace<Potential_Position>(item, position.fX, position.fY);
	
		Scenes::scene.emplace<Ground_Item>(item, 
			position2.fX - (32.0f * scale), 
			position2.fY - (32.0f * scale),
			64.0f * scale,
			64.0f * scale);
		//Scenes::scene.emplace<Assigned>(item);	//stores the entity holding the item	
		
		//Scenes::scene.emplace<Item_Type>(item, weapon);
	}

	void Pick_Up_Item(entt::entity item) {
		if (Mouse::itemCurrentlyHeld == false) {
			//removed pickup box from ground
			Scenes::scene.remove<Ground_Item>(item);
			//removes for main rendering loop
			Scenes::scene.remove<Direction>(item);
			//to render on mouse
			Scenes::scene.emplace<On_Mouse>(item);
			Mouse::mouseItem = item;
			Mouse::itemCurrentlyHeld = true;
		}
	}

	//maybe change to take in the mouse data as arguements so I don't have to include mouse_control.h, also can break out of check faster maybe
	//void Check_For_Item_Up_Item(float &xMouse, float &yMouse, bool &isItemCurrentlyHeld) {
	void Check_For_Item_To_Pick_Up(std::vector<entt::entity> &bag, int &totalSlots, entt::entity emptySlot, bool &isBagOpen) {
		//check if input unit it close enough to item
		auto itemView = Scenes::scene.view<Position, Renderable, Ground_Item>();
		auto mouseInput = Scenes::scene.view<Position, Input, Radius>();
		
		for (auto entity : mouseInput) {
			auto &x = mouseInput.get<Position>(entity).fX;
			auto &y = mouseInput.get<Position>(entity).fY;
			auto &radius = mouseInput.get<Radius>(entity).fRadius;
			// rect surrounding unit
			SDL_FRect unitRect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);
			// looks at every ground item in the scene 
			for (auto itemID : itemView) {
				auto &itemBox = itemView.get<Ground_Item>(itemID).box;
				// rect surrounding item
				SDL_FRect screenItemBox = Camera_Control::Convert_Rect_To_Screen_Coods(itemBox);			

				// check if next to item
				if (Utilities::bFRect_Intersect(itemBox, unitRect)) {
					// check if mouse is inside item box					
					if (Mouse::FRect_inside_Cursor(itemBox)) {
						if (isBagOpen) { //bag is closed
							Pick_Up_Item(itemID);
						}
						else {
							//find the first slot with a default icon
							for (int i = 0; i < totalSlots; i++) {
								if (bag.at(i) == emptySlot) {
									bag.at(i) = itemID;
									//removed pickup box from ground
									Scenes::scene.remove<Ground_Item>(itemID);
									//removes for main rendering loop
									Scenes::scene.remove<Direction>(itemID);
									break;
								}
								//either pick it up with the mouse using this function or have an overburdened function instead
							}							
						}
					}
				}
			}
		}
	}

	void Drop_Item_If_On_Mouse() {
		if (Mouse::itemCurrentlyHeld == true) {
			auto view = Scenes::scene.view<Input, Position>();
			for (auto entity : view) {
				auto &unitPosition = view.get<Position>(entity);

				auto &scale = Scenes::scene.get<Scale>(Mouse::mouseItem).scale;
				auto &position = Scenes::scene.get<Potential_Position>(Mouse::mouseItem);
				position.fPX = unitPosition.fX;
				position.fPY = unitPosition.fY;

				Scenes::scene.emplace<Ground_Item>(Mouse::mouseItem,
					position.fPX - (32.0f * scale),
					position.fPY - (32.0f * scale),
					64.0f * scale,
					64.0f * scale);

				//adds to rendering with the main animation loop
				Scenes::scene.emplace<Direction>(Mouse::mouseItem, W);
				// to remove from rendering on mouse
				Scenes::scene.remove<On_Mouse>(Mouse::mouseItem);
			}
			Mouse::itemCurrentlyHeld = false;
		}
	}

	
	void Update_Mouse_Slot_Position() {
		//set item in mouse array position to mouse x, y every frame
		if (Mouse::itemCurrentlyHeld == true) {
			Potential_Position& position = Scenes::scene.get<Potential_Position>(Mouse::mouseItem);
			position.fPX = Mouse::iXWorld_Mouse;
			position.fPY = Mouse::iYWorld_Mouse;
		}
	}

	//mouse click
	//walk over to item
	//when arrived at item pick it up into mouse array




}