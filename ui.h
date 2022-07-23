#pragma once
#include "components.h"
#include "graphics.h"

namespace UI {
	namespace {
		bool toggleBag = false;
		SDL_Rect charui = { 0, 0, 554, 1080 };
		float scale = 1.0f;
		SDL_Rect defaultScreenPosition = { 100, 100, 554, 1080 };
		SDL_Rect currentScreenPosition = { };
		
		
		//x 32
		//y 544
		DataTypes::i2d bagoffset = { 32, 544 }; //i THIKN
		DataTypes::i2d numOfSlots = { 8, 4 };
		int bagslotsize = 64; // or 32 not even sure		
		SDL_Rect Bag = { currentScreenPosition.x + bagoffset.x, currentScreenPosition.y + bagoffset.y, numOfSlots.x * bagslotsize, numOfSlots.y * bagslotsize };
		SDL_Rect screenBag = { };
		std::vector<entt::entity>UI_bagSlots;
		
		//int totalSlots = numOfSlots.x * numOfSlots.y;
		std::vector<SDL_Rect>UI_bagSlotPosition(numOfSlots.x * numOfSlots.y);
		


		//calculate slot position based on which slot its in and save the rect to the Icon component

		
	}

	

	void Place_Item_In_Bag() {
		if (toggleBag) {

		}
	}

	void Create_Bag_UI() {
		for (int i = 0; i < (numOfSlots.x * numOfSlots.y); i++) {
			
			
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

	//check if the Mouse point is in the rect and which one
	int Get_Bag_Slot(SDL_Point &mousePoint) {		
		SDL_Rect slotRect = {};		
		slotRect.w = bagslotsize;
		slotRect.h = bagslotsize;
		
		for (int i = 0; i < numOfSlots.x; i++) {
			slotRect.x = (i * bagslotsize) + Bag.x;
			
			for (int j = 0; j < numOfSlots.y; j++) {
				slotRect.y = (j * bagslotsize) + Bag.y;
				if (Utilities::bPoint_RectIntersect(mousePoint, slotRect)) {
					
					return j;
				}
			}
			if (Utilities::bPoint_RectIntersect(mousePoint, slotRect)) {
				
				return i;
			}
		}
		
	}

	void Update_Bag_Scale(Camera &camera) {
		
		
		
	}

	bool Interact_With_Bag(entt::entity &item, SDL_Point &mousePoint) {
		if (Mouse::bRect_inside_Cursor(UI::currentScreenPosition)) {
			Utilities::Log("inside");
			SDL_Point cursorRelativeToBagUIPosition = { mousePoint.x - currentScreenPosition.x, mousePoint.y - currentScreenPosition.y };
			if (Utilities::bPoint_RectIntersect(cursorRelativeToBagUIPosition, Bag)) {
				int slotNum = Get_Bag_Slot(mousePoint);
				
				std::cout << slotNum << std::endl;
				//UI_bagSlots.at(slotNum) = item;
				Utilities::Log("in bag");
				return true;
				//remove from mouse
			}
			Utilities::Log("not in bag");
		}
		else {
			return false;
		}
		
	}

	void Render_UI() {
		if (toggleBag) {
			
			SDL_Rect renderRect = Camera_Control::Convert_Rect_To_Scale(defaultScreenPosition, currentScreenPosition);
			SDL_RenderCopy(Graphics::renderer, Graphics::itsmars_Inventory, &charui, &renderRect);
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
