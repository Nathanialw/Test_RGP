#pragma once
#include <SDL.h>
#include "scene.h"
#include "timer.h"
#include "utilities.h"
#include "unit_control.h"
#include "interface.h"


using namespace Scene;


namespace EventHandler {

	/*I will make a component that will be pased to this funtion tree so the functions can do work on the position value of an entity "<velocity> <player_controllable>" */
	

	SDL_Event event;

	void Keyboard_Input(Velocity&vel, Actions& act) { //can return bools for x and y dir, and 2 enums for direction and state
		if (event.key.repeat == 0) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym)
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
				{
				case SDLK_w: vel.magnitude.fY -= vel.speed; break;
				case SDLK_s: vel.magnitude.fY += vel.speed; break;
				case SDLK_a: vel.magnitude.fX -= vel.speed; break;
				case SDLK_d: vel.magnitude.fX += vel.speed; break;
				case SDLK_q: vel.magnitude.fY -= vel.speed; vel.magnitude.fX -= vel.speed; break;
				case SDLK_e: vel.magnitude.fY -= vel.speed; vel.magnitude.fX += vel.speed; break;
				case SDLK_c: vel.magnitude.fY += vel.speed; vel.magnitude.fX += vel.speed; break;
				case SDLK_z: vel.magnitude.fY += vel.speed; vel.magnitude.fX -= vel.speed; break;


					//push to front of the array when key down, read from front, remove when kep released
				case SDLK_3: act.action = slash; break;
				case SDLK_4: act.action = stab; break;
				case SDLK_5: act.action = block; break;
				case SDLK_6: act.action = dead; break;
				case SDLK_7: act.action = xbow; break;
				}
			}
			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym)
				{
				case SDLK_w: vel.magnitude.fY += vel.speed; break;
				case SDLK_s: vel.magnitude.fY -= vel.speed; break;
				case SDLK_a: vel.magnitude.fX += vel.speed; break;
				case SDLK_d: vel.magnitude.fX -= vel.speed; break;
				case SDLK_q: vel.magnitude.fY += vel.speed; vel.magnitude.fX += vel.speed;  break;
				case SDLK_e: vel.magnitude.fY += vel.speed; vel.magnitude.fX -= vel.speed;  break;
				case SDLK_c: vel.magnitude.fY -= vel.speed; vel.magnitude.fX -= vel.speed;  break;
				case SDLK_z: vel.magnitude.fY -= vel.speed; vel.magnitude.fX += vel.speed;  break;

				case SDLK_3: act.action = idle; break;
				case SDLK_4: act.action = idle; break;
				case SDLK_5: act.action = idle; break;
				case SDLK_6: act.action = idle; break;
				case SDLK_7: act.action = idle; break;
				}
			}
		}
	};


	void Mouse_Input() {
		if (event.key.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {				
				User_Mouse_Input::Selection_Box(); //if units are currently selected				
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				if (!scene.empty<Selected>()) {
					User_Mouse_Input::Selection_Box(); //if units are currently selected
				}
			}
		}

		if (event.key.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				User_Mouse_Input::Select_Unit();
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				User_Mouse_Input::Command_Unit();				
			}
		}
	}

	void Player_Input() {
		while (SDL_PollEvent(&event) != 0) {
			auto view = scene.view<Velocity, Actions, Input>();
			for (auto entity : view) {
				auto& vel = view.get<Velocity>(entity);
				auto& act = view.get<Actions>(entity);
				if (event.key.type == SDL_KEYDOWN || event.key.type == SDL_KEYUP) {
					Keyboard_Input(vel, act);
				}
				if (event.key.type == SDL_MOUSEWHEEL) {
					Interface::Update_Zoom(event);
				}
				if (event.key.type == SDL_MOUSEBUTTONDOWN || event.key.type == SDL_MOUSEBUTTONUP) {
					Mouse_Input();
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

	void Input_Handler() {
		Player_Input();
	}

}
