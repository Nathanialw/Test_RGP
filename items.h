#pragma once
#include "scenes.h"
#include "mouse_control.h"
#include "components.h"
#include "item_components.h"
#include "graphics.h"
#include "utilities.h"

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

		auto& x = Scenes::scene.emplace<Position_X>(item, xDrop, xDrop);
		auto& y = Scenes::scene.emplace<Position_Y>(item, yDrop, yDrop);
	
		Scenes::scene.emplace<Ground_Item>(item, x.fX - 32.0f, y.fY - 32.0f, 64.0f, 64.0f);
		//Scenes::scene.emplace<Assigned>(item);	//stores the entity holding the item	
		
		//Scenes::scene.emplace<Item_Type>(item, weapon);
	}

	void Pick_Up_Item() {
		//check if input unit it close enough to item
		auto itemView = Scenes::scene.view<Position_X, Position_Y, Renderable, Ground_Item>();
		auto mouseInput = Scenes::scene.view<Position_X, Position_Y, Input, Radius>();
		
		for (auto entity : mouseInput) {
			auto &x = mouseInput.get<Position_X>(entity).fX;
			auto &y = mouseInput.get<Position_Y>(entity).fY;
			auto &radius = mouseInput.get<Radius>(entity).fRadius;
			for (auto item : itemView) {
				auto &itemBox = itemView.get<Ground_Item>(item).box;
				SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);
				SDL_SetRenderDrawColor(Graphics::renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawRectF(Graphics::renderer, &itemBox);
				SDL_RenderDrawRectF(Graphics::renderer, &rect);
				if (Utilities::bFRect_Intersect(itemBox, rect)) {
					SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawRectF(Graphics::renderer, &itemBox);
					SDL_RenderDrawRectF(Graphics::renderer, &rect);
					//check if mouse is inside item box
					
					if (Mouse::Inside_Cursor(itemBox.x, itemBox.y)) {
						SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
						SDL_RenderDrawRectF(Graphics::renderer, &itemBox);
						SDL_RenderDrawRectF(Graphics::renderer, &rect);
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
	}



}