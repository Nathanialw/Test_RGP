#pragma once
#include <SDL.h>
#include "ui.h"
#include "timer.h"
#include "utilities.h"
#include "unit_control.h"
#include "movement.h"
#include "death_spells.h"
#include "spells.h"
#include "weapons.h"
#include "ai_control.h"

namespace Event_Handler {

	/*I will make a component that will be pased to this funtion tree so the functions can do work on the position value of an entity "<velocity> <player_controllable>" */
	

	SDL_Event event;

	void Keyboard_Input(entt::registry& zone, Velocity& vel, Actions& act, entt::entity entity) { //can return bools for x and y dir, and 2 enums for direction and state
		if (event.key.repeat == 0) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym)
				{
					//push vel.speed to front of an array std::vector<f2d>forceinputsWSAD(4) = { magnitude.x, magnitude.y} ...
					//push vel.speed to front of an array std::vector<f2d>forceinputsQEZC(4) = { magnitude.x, magnitude.y} ...
					//read fron fornt remove when key is released
					//add element [0] of both vectors (need to have a default { 0, 0 }), NULL will break it I think
					//now we have x,y magnitude of the vector
					//float angle = tan -1 (magnitude.x/magnitude.y)
					//sin(angle) * vel.speed = new vel.x this frame 
					//cos(angle) * vel.speed = new vel.y this frame

					/*
					To have velocity:
						on keydown:: vel.velocity = vel.speed
						on keyup::: vel.velocity = 0
						plug vel into movements as "magnitude"
						potential pos = vel * vel * time
						//collision on potential position 
						//the only area we need to test is betweem the current positiona and the potenital position (potential position being the max distance possible)
						//sort the poential world position vector and iterate a range between your current position and potential position
						//do for each entity
						/////plug new potential positions back into the actual position componenet


						///check all then after they are all check THEN ressolve the collisions  where the ammout move is aproportion of its mass
						poisition = potential position
					*/
					
				case SDLK_1: AI::Spell_Attack(zone, entity, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, "'fireball'"); break;
				case SDLK_2: Death_Spells::Summon_Skeleton(zone, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, "'skeleton'", Graphics::skeleton_mage_0);  break;
				case SDLK_3: AI::Melee_Attack(zone, entity, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);   break;
				case SDLK_4: Test_Units::Create_And_Fill_New_Squad(zone); break;
				case SDLK_5: Test_Units::Create_Formation(zone); break;
				//case SDLK_5: Debug_System::Toggle_Frame_Rate_Mode(); break;
				case SDLK_6: Interface::gridDepth++; break;
				case SDLK_7: Interface::gridDepth--; break;
				case SDLK_8: Rendering::RenderCullMode(World::zone); break;
				case SDLK_9: AI::Turn_On();  break;
				case SDLK_0: User_Mouse_Input::Selection_Soldiers();  break;
				case SDLK_ESCAPE: Graphics::closeContext();  break;
				case SDLK_PLUS: break;
				case SDLK_MINUS: break;
				case SDLK_p: Timer::Pause_Control();  break; 
				case SDLK_i: UI::Toggle_Bag();  break;
				}
			}
			if (act.action == idle || act.action ==  walk) {
				if (event.type == SDL_KEYDOWN) {
					auto& position = zone.get<Position>(entity);
					switch (event.key.keysym.sym) {
					case SDLK_e:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fY -= vel.speed; act.action = walk; break;
					case SDLK_d:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fY += vel.speed; act.action = walk; break;
					case SDLK_s:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fX -= vel.speed; act.action = walk; break;
					case SDLK_f:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fX += vel.speed; act.action = walk; break;
					case SDLK_w:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fY -= vel.speed; vel.magnitude.fX -= vel.speed; act.action = walk; break;
					case SDLK_r:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fY -= vel.speed; vel.magnitude.fX += vel.speed; act.action = walk; break;
					case SDLK_v:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fY += vel.speed; vel.magnitude.fX += vel.speed; act.action = walk; break;
					case SDLK_x:  zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); vel.magnitude.fY += vel.speed; vel.magnitude.fX -= vel.speed; act.action = walk; break;
					}
				}
			}
			if (event.type == SDL_KEYUP) {
				auto& position = zone.get<Position>(entity);
				switch (event.key.keysym.sym)
				{
				//case SDLK_e: vel.magnitude.fY += vel.speed; act.action = idle; break;
				//case SDLK_d: vel.magnitude.fY -= vel.speed; act.action = idle; break;
				//case SDLK_s: vel.magnitude.fX += vel.speed; act.action = idle; break;
				//case SDLK_f: vel.magnitude.fX -= vel.speed; act.action = idle; break;
				//case SDLK_w: vel.magnitude.fY += vel.speed; vel.magnitude.fX += vel.speed; act.action = idle; break;
				//case SDLK_r: vel.magnitude.fY += vel.speed; vel.magnitude.fX -= vel.speed; act.action = idle; break;
				//case SDLK_v: vel.magnitude.fY -= vel.speed; vel.magnitude.fX -= vel.speed; act.action = idle; break;
				//case SDLK_x: vel.magnitude.fY -= vel.speed; vel.magnitude.fX += vel.speed; act.action = idle; break;

				case SDLK_e: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fY) > 0) vel.magnitude.fY += vel.speed; break;
				case SDLK_d: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fY) > 0) vel.magnitude.fY -= vel.speed; break;
				case SDLK_s: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fX) > 0) vel.magnitude.fX += vel.speed; break;
				case SDLK_f: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fX) > 0) vel.magnitude.fX -= vel.speed; break;
				case SDLK_w: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fY) > 0) vel.magnitude.fY += vel.speed; if (fabs(vel.magnitude.fX) > 0) vel.magnitude.fX += vel.speed;  break;
				case SDLK_r: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fY) > 0) vel.magnitude.fY += vel.speed; if (fabs(vel.magnitude.fX) > 0) vel.magnitude.fX -= vel.speed;  break;
				case SDLK_v: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fY) > 0) vel.magnitude.fY -= vel.speed; if (fabs(vel.magnitude.fX) > 0) vel.magnitude.fX -= vel.speed;  break;
				case SDLK_x: zone.emplace_or_replace<Moving>(entity); zone.emplace_or_replace<Potential_Position>(entity, position.x, position.y); if (fabs(vel.magnitude.fY) > 0) vel.magnitude.fY -= vel.speed; if (fabs(vel.magnitude.fX) > 0) vel.magnitude.fX += vel.speed;  break;
				}
			}
			if (act.action == slash) {
				vel.magnitude.fX = 0.0f;
				vel.magnitude.fY = 0.0f;
			}
			if ((vel.magnitude.fX == 0.0f) && (vel.magnitude.fY == 0.0f) && zone.any_of<Moving>(entity)) {
				zone.remove<Moving>(entity);
				zone.remove<Potential_Position>(entity);
				act.action = idle;
			}

		}
	};


	void Mouse_Input(entt::entity &player) {
		if (event.key.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				//check if cursor is in the bag UI
				if (Mouse::bRect_inside_Cursor(UI::currentScreenPosition)){
					UI::Interact_With_Bag(World::zone, Mouse::mouseItem, Mouse::mousePoint, Mouse::itemCurrentlyHeld);
				}
				else {
					Items::Drop_Item_If_On_Mouse(World::zone, Mouse::mouseItem, Mouse::itemCurrentlyHeld);
				}
				User_Mouse_Input::Selection_Box(World::zone); //if units are currently selected				
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				if (Items::Check_For_Item_To_Pick_Up(World::zone, UI::UI_bagSlots, UI::iTotalSlots, Graphics::defaultIcon, UI::bToggleBag, Mouse::itemCurrentlyHeld)) {
					//
				}
				else {
					Mouse::bRight_Mouse_Pressed = true;
					User_Mouse_Input::Select_Unit(World::zone); //if units are currently selected
				}
			}
		}

		if (event.key.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				User_Mouse_Input::Select_Units(World::zone);
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {				
				Mouse::bRight_Mouse_Pressed = false;				
				User_Mouse_Input::Command_Squad(World::zone);
				User_Mouse_Input::Command_Unit(World::zone);				
			}
		}
	}

	void Update_User_Input(entt::registry &zone) {
		while (SDL_PollEvent(&event) != 0) {
			auto view = zone.view<Velocity, Actions, Input>();
			for (auto player : view) {
				auto& vel = view.get<Velocity>(player);
				auto& act = view.get<Actions>(player);
				if (event.key.type == SDL_MOUSEWHEEL) {
					Interface::Update_Zoom(event);
				}
				if (event.key.type == SDL_MOUSEBUTTONDOWN || event.key.type == SDL_MOUSEBUTTONUP) {
					Mouse_Input(player);
				}				
				if (event.key.type == SDL_KEYDOWN || event.key.type == SDL_KEYUP) {	
					if (zone.all_of<Mouse_Move>(player)) {
						zone.remove<Mouse_Move>(player);
						vel.magnitude.fX = 0;
						vel.magnitude.fY = 0;
					}
					Keyboard_Input(zone, vel, act, player);
				}
				
				//if (event.key.type == SDL_JOYAXISMOTION) { // it works!
				//	if (event.jaxis. == 0) {
					//Joystick::JoystickInput(event);
					//std::cout << Joystick::JoystickInput(event) << std::endl;
				//	if (event.jaxis.axis == 0) {
					//	std::cout << event.jaxis.value << std::endl;
				//	}				
			}
		}
	};

	void Input_Handler(entt::registry& zone) {
		Update_User_Input(zone);
	}

}
