#pragma once
#include "components.h"
#include "camera.h"

namespace UI {
	namespace {
		bool bToggleBag = false;
		SDL_Rect charui = { 0, 0, 554, 1080 };
		
		SDL_Rect defaultScreenPosition = { 100, 100, 554, 1080 };
		SDL_Rect currentScreenPosition = {};
	
		DataTypes::i2d bagoffset = { 32, 544 };
		DataTypes::i2d numOfSlots = { 8, 4 };
		int iTotalSlots = numOfSlots.x * numOfSlots.y;
		int iBagSlotPixelSize = 64; 
		SDL_Rect Bag = { defaultScreenPosition.x + bagoffset.x, defaultScreenPosition.y + bagoffset.y, numOfSlots.x * iBagSlotPixelSize, numOfSlots.y * iBagSlotPixelSize };
		SDL_Rect screenBag = { };
		std::vector<entt::entity>UI_bagSlots(iTotalSlots);
	}

	void Place_Item_In_Bag(entt::registry& zone, entt::entity &item, bool &mouseHasItem, int & slotNum) {
		UI_bagSlots.at(slotNum) = item;
		mouseHasItem = false;
		zone.remove<On_Mouse>(item);
	}

	void Remove_Item_From_Bag(entt::registry& zone, entt::entity& item, bool& mouseHasItem, int& slotNum) {
		item = UI_bagSlots.at(slotNum);
		UI_bagSlots.at(slotNum) = Graphics::defaultIcon;
		zone.emplace<On_Mouse>(item);
		mouseHasItem = true;
	}

	void Create_Bag_UI() {
		for (int i = 0; i < (iTotalSlots); i++) {
			UI_bagSlots.at(i) = Graphics::defaultIcon;
		}
	}

	void Toggle_Bag() {
		if (!bToggleBag) {
			bToggleBag = true;
		}
		else {
			bToggleBag = false;
		}
	}

	//check if the Mouse point is in the rect and which one
	int Get_Bag_Slot(entt::registry &zone, SDL_Point& mousePoint) {

		SDL_Rect slotRect = {};
		slotRect.w = iBagSlotPixelSize;
		slotRect.h = iBagSlotPixelSize;
		int i;
		int j;
		int slot = 0;

		for (i = 0; i < numOfSlots.x; i++) {
			slotRect.x = (i * iBagSlotPixelSize) + Bag.x;

			for (j = 0; j < numOfSlots.y; j++) {
				slotRect.y = (j * iBagSlotPixelSize) + Bag.y;

				SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(zone, slotRect);
				if (Utilities::bPoint_RectIntersect(mousePoint, scaledSlot)) {
					return slot;
				}
				if (j < (numOfSlots.y - 1)) {
					slot++;
				}
			}
			SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(zone, slotRect);
			if (Utilities::bPoint_RectIntersect(mousePoint, scaledSlot)) {
				return slot;
			}
			if (i < (numOfSlots.x - 1)) {
				slot++;
			} 
		}

	}
	//check if the Mouse point is in the rect and which one
	void Render_Bag_Slot(entt::registry &zone, SDL_Renderer *renderer) {

		SDL_Rect slotRect = {};
		slotRect.w = iBagSlotPixelSize;
		slotRect.h = iBagSlotPixelSize;
		int i;
		int j;
		int slot = 0;

		for (i = 0; i < numOfSlots.x; i++) {
			slotRect.x = (i * iBagSlotPixelSize) + Bag.x;

			for (j = 0; j < numOfSlots.y; j++) {
				slotRect.y = (j * iBagSlotPixelSize) + Bag.y;

				auto &icon = zone.get<Icon>(UI_bagSlots.at(slot));
				/*could be injected instead if it was a class object method*/SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(zone, slotRect);
				SDL_RenderCopy(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
				if (j < (numOfSlots.y - 1)) {
					slot++;
				}
			}

			auto &icon = zone.get<Icon>(UI_bagSlots.at(slot));
			/*could be injected instead if it was a class object method*/SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(zone, slotRect);
			SDL_RenderCopy(renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
			if (i < (numOfSlots.x - 1)) {
				slot++;
			}
		}

	}


	void Interact_With_Bag(entt::registry& zone, entt::entity& item, SDL_Point& mousePoint, bool &mouseHasItem) {
		/*could be injected instead*/SDL_Point screenCursor = Camera_Control::Convert_Point_To_Scale(zone, mousePoint);
		int slotNum = Get_Bag_Slot(zone, screenCursor);

		if (Utilities::bPoint_RectIntersect(screenCursor, screenBag)) {
			if (mouseHasItem) {
				Place_Item_In_Bag(zone, item, mouseHasItem, slotNum);			
			}
			else if (UI_bagSlots.at(slotNum) != Graphics::defaultIcon) {
				Remove_Item_From_Bag(zone, item, mouseHasItem, slotNum);
			}
		}
	}

	void Render_UI(entt::registry& zone, SDL_Renderer* renderer) {
		if (bToggleBag) {

			currentScreenPosition = Camera_Control::Convert_Rect_To_Scale(zone, defaultScreenPosition);
			screenBag = Camera_Control::Convert_Rect_To_Scale(zone, Bag);
			//render UI
			Graphics::Render_Rect(Graphics::itsmars_Inventory, charui, currentScreenPosition);
			//render Items in bag
			Render_Bag_Slot(zone, renderer);
		}
	}

	//run at game start to init bag vector
	void Init_UI() {
		Create_Bag_UI();
	}











}