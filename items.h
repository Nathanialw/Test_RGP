#pragma once
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


	void Create_And_Drop_Item(float& xDrop, float& yDrop) {
		float scale = 0.5f;
		
		auto item = Scenes::scene.create();

		Scenes::scene.emplace<animation>(item, Graphics::longsword_default); /// need to load hetexture	 only once and pass the pointer into this function
		Scenes::scene.get<animation>(item).sheet = {
			{{ 0, 0, 64, 64}, 0, 64, 0, 0, 0.0f, 16.0f } }; //populate the vector
		auto& offset = Scenes::scene.emplace<Sprite_Offset>(item, 32.0f * scale, 32.0f * scale);
		Scenes::scene.emplace<Scale>(item, scale);

		Scenes::scene.emplace<Actions>(item, isStatic);
		Scenes::scene.emplace<Direction>(item, W);

		auto& position = Scenes::scene.emplace<Position>(item, xDrop, yDrop);
		Scenes::scene.emplace<Potential_Position>(item, xDrop, yDrop);
	
		Scenes::scene.emplace<Ground_Item>(item, 
			position.fX - (32.0f * scale), 
			position.fY - (32.0f * scale),
			64.0f * scale,
			64.0f * scale);
		//Scenes::scene.emplace<Assigned>(item);	//stores the entity holding the item	
		
		//Scenes::scene.emplace<Item_Type>(item, weapon);
	}

	void Pick_Up_Item() {
		//check if input unit it close enough to item
		auto itemView = Scenes::scene.view<Position, Renderable, Ground_Item>();
		auto mouseInput = Scenes::scene.view<Position, Input, Radius>();
		
		for (auto entity : mouseInput) {
			auto &x = mouseInput.get<Position>(entity).fX;
			auto &y = mouseInput.get<Position>(entity).fY;
			auto &radius = mouseInput.get<Radius>(entity).fRadius;
			for (auto item : itemView) {
				auto &itemBox = itemView.get<Ground_Item>(item).box;
				SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);
				SDL_FRect screenItemBox = Camera_Control::Convert_Rect_To_Screen_Coods(itemBox);
				SDL_FRect screenRect = Camera_Control::Convert_Rect_To_Screen_Coods(rect);

				if (pick_up_debug) {
					SDL_SetRenderDrawColor(Graphics::renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawRectF(Graphics::renderer, &screenItemBox);
					SDL_RenderDrawRectF(Graphics::renderer, &screenRect);
				}
				//check if next to item
				if (Utilities::bFRect_Intersect(itemBox, rect)) {
					if (pick_up_debug) {
						SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
						SDL_RenderDrawRectF(Graphics::renderer, &screenItemBox);
						SDL_RenderDrawRectF(Graphics::renderer, &screenRect);
					}
					//check if mouse is inside item box					
					if (Mouse::FRect_inside_Cursor(itemBox)) {
						if (pick_up_debug) {
							SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
							SDL_RenderDrawRectF(Graphics::renderer, &screenItemBox);
							SDL_RenderDrawRectF(Graphics::renderer, &screenRect);
						}
						//place item in mouse array on click and 
							//remove ground item component, 
						Scenes::scene.remove<Ground_Item>(item);
						Scenes::scene.remove<animation>(item);
							//replace  x and y coords with mouse coords, update every frame

							// add ground item component and replace x and y with mouse coords on click

							// if over bag
								//add item to bag array and 
								// place x,y coords in empty array slot coords update every frame the UI is open
					}
				}
				SDL_SetRenderDrawColor(Graphics::renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
			}
		}
	}

	//mouse click
	//walk over to item
	//when arrived at item pick it up into mouse array




}