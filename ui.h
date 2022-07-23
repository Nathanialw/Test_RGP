#pragma once
#include "components.h"
#include "graphics.h"
#include "scenes.h"


namespace UI {
	namespace {
		bool toggleBag = false;
		SDL_Rect charui = { 0, 0, 554, 1080 };
		float scale = 1.0f;
		SDL_Rect defaultScreenPosition = { 100, 100, 554, 1080 };
		SDL_Rect currentScreenPosition = {};
	
		DataTypes::i2d bagoffset = { 32, 544 };
		DataTypes::i2d numOfSlots = { 8, 4 };
		int totalSlots = numOfSlots.x * numOfSlots.y;
		int bagslotsize = 64; 
		SDL_Rect Bag = { defaultScreenPosition.x + bagoffset.x, defaultScreenPosition.y + bagoffset.y, numOfSlots.x * bagslotsize, numOfSlots.y * bagslotsize };
		SDL_Rect screenBag = { };
		std::vector<entt::entity>UI_bagSlots(totalSlots);
	}



	void Place_Item_In_Bag() {
		if (toggleBag) {

		}
	}

	void Create_Bag_UI() {
		for (int i = 0; i < (totalSlots); i++) {
			UI_bagSlots.at(i) = Graphics::defaultIcon;
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
	int Get_Bag_Slot(SDL_Point& mousePoint) {

		SDL_Rect slotRect = {};
		slotRect.w = bagslotsize;
		slotRect.h = bagslotsize;
		int i;
		int j;
		int slot = 0;

		for (i = 0; i < numOfSlots.x; i++) {
			slotRect.x = (i * bagslotsize) + Bag.x;

			for (j = 0; j < numOfSlots.y; j++) {
				slotRect.y = (j * bagslotsize) + Bag.y;

				SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect);
				if (Utilities::bPoint_RectIntersect(mousePoint, scaledSlot)) {
					return slot;
				}
				if (j < (numOfSlots.y - 1)) {
					slot++;
				}
			}
			SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect);
			if (Utilities::bPoint_RectIntersect(mousePoint, scaledSlot)) {
				return slot;
			}
			if (i < (numOfSlots.x - 1)) {
				slot++;
			}
		}

	}
	//check if the Mouse point is in the rect and which one
	void Render_Bag_Slot() {

		SDL_Rect slotRect = {};
		slotRect.w = bagslotsize;
		slotRect.h = bagslotsize;
		int i;
		int j;
		int slot = 0;

		for (i = 0; i < numOfSlots.x; i++) {
			slotRect.x = (i * bagslotsize) + Bag.x;

			for (j = 0; j < numOfSlots.y; j++) {
				slotRect.y = (j * bagslotsize) + Bag.y;

				auto &icon = Scenes::scene.get<Icon>(UI_bagSlots.at(slot));
				SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect);
				SDL_RenderCopy(Graphics::renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
				if (j < (numOfSlots.y - 1)) {
					slot++;
				}
			}

			auto icon = Scenes::scene.get<Icon>(UI_bagSlots.at(slot));
			SDL_Rect scaledSlot = Camera_Control::Convert_Rect_To_Scale(slotRect);
			SDL_RenderCopy(Graphics::renderer, icon.pTexture, &icon.clipSprite, &scaledSlot);
			if (i < (numOfSlots.x - 1)) {
				slot++;
			}
		}

	}


	bool Interact_With_Bag(entt::entity& item, SDL_Point& mousePoint, bool &mouseHasItem) {
		if (Mouse::bRect_inside_Cursor(UI::currentScreenPosition)) {
			SDL_Point screenCursor = Camera_Control::Convert_Point_To_Scale(mousePoint);
			int slotNum = Get_Bag_Slot(screenCursor);

			if (Utilities::bPoint_RectIntersect(screenCursor, screenBag)) {
				if (mouseHasItem) {
					UI_bagSlots.at(slotNum) = item;
					mouseHasItem = false;
					Scenes::scene.remove<On_Mouse>(Mouse::mouseItem);
					return true;				
				}
				else if (UI_bagSlots.at(slotNum) != Graphics::defaultIcon) {
					item = UI_bagSlots.at(slotNum);
					UI_bagSlots.at(slotNum) = Graphics::defaultIcon;
					Scenes::scene.emplace<On_Mouse>(Mouse::mouseItem);
					mouseHasItem = true;
					return true;
				}
			}
			return false;
		}
		else {
			return false;
		}

	}

	void Render_UI() {
		if (toggleBag) {

			currentScreenPosition = Camera_Control::Convert_Rect_To_Scale(defaultScreenPosition);
			screenBag = Camera_Control::Convert_Rect_To_Scale(Bag);
			//render UI
			Graphics::Render_Rect(Graphics::itsmars_Inventory, charui, currentScreenPosition);
			//render Items in bag
			Render_Bag_Slot();
		}
	}

	//run at game start to init bag vector
	void Init_UI() {
		Create_Bag_UI();
	}











}