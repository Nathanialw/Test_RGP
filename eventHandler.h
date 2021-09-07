#pragma once
#include <SDL.h>
#include "eventHandler.h"
#include "components.h"
#include "pools.h"
#include "timer.h"
#include "graphics.h"


using namespace Scene;


namespace EventHandler {

	/*I will make a component that will be pased to this funtion tree so the functions can do work on the position value of an entity "<velocity> <player_controllable>" */
	SDL_Event event;

	void  keyboardInput(Velocity&vel, Actions& act) { //can return bools for x and y dir, and 2 enums for direction and state
		if (event.key.repeat == 0) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym)
					//push vel.speed to front of an array std::vector<f2d>forceinputsWSAD(4) = { magnitude.x, magnitude.y} ...
					//push vel.speed to front of an array std::vector<f2d>forceinputsQEZC(4) = { magnitude.x, magnitude.y} ...
					// //read fron fornt remove when key is released
					//add element [0] of both vectors (need to have a default { 0, 0 }), NULL will break it I think
					// now we have x,y magnitude of the vector
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

	void Movement() {
		auto group = scene.view<Position_X, Position_Y, Velocity>();
		for (auto entity : group) {
			auto& vel = group.get<Velocity>(entity);
			auto& pX = group.get<Position_X>(entity);
			auto& pY = group.get<Position_Y>(entity);
			if (vel.magnitude.fX != 0 || vel.magnitude.fY != 0) {
				if (fabs(vel.magnitude.fX) < 0.01) { vel.magnitude.fX = 0; }; //clamp rounding errors
				if (fabs(vel.magnitude.fY) < 0.01) { vel.magnitude.fY = 0; };
				float angle2 = atan2f(vel.magnitude.fY, vel.magnitude.fX);
				float angle = atan2f(vel.magnitude.fX, vel.magnitude.fY);
				float velocityX = sinf(angle) * vel.speed;
				float velocityY = sinf(angle2) * vel.speed;
				vel.dX = velocityX;
				vel.dY = velocityY;
				pX.fPX += velocityX * Timer::timeStep;
				pY.fPY += velocityY * Timer::timeStep;
				//std::cout << "x:   " << y.magnitude.x << "y:  " << y.magnitude.y << std::endl;
			//	std::cout << velocityY << std::endl;
			//	std::cout << "x:  " << y.magnitude.x << "    y:   " << y.magnitude.y << std::endl;
			}
		};
	};

	void updateDirection() {
		auto view = scene.view<Direction, Actions, Velocity>();
		for (auto entity : view) {
			auto& vel = view.get<Velocity>(entity);
			auto& b = view.get<Direction>(entity);
			auto& c = view.get<Actions>(entity);
			if (vel.magnitude.fX > 0) { b.eDirection = E; c.action = walk;};
			if (vel.magnitude.fX < 0) { b.eDirection = W; c.action = walk;};
			if (vel.magnitude.fY > 0) { b.eDirection = S; c.action = walk;};
			if (vel.magnitude.fY < 0) { b.eDirection = N; c.action = walk;};
							
			if (vel.magnitude.fX > 0 && vel.magnitude.fY > 0) { b.eDirection = SE; c.action = walk;};
			if (vel.magnitude.fX < 0 && vel.magnitude.fY > 0) { b.eDirection = SW; c.action = walk;};
			if (vel.magnitude.fY < 0 && vel.magnitude.fX < 0) { b.eDirection = NW; c.action = walk;};
			if (vel.magnitude.fY < 0 && vel.magnitude.fX > 0) { b.eDirection = NE; c.action = walk;};
				 
			if (c.action == walk) {
				if (vel.magnitude.fX == 0 && vel.magnitude.fY == 0) { c.action = idle; };
			}
		}
	}

	void zoom() {
		auto view = scene.view<Camera>();
		for (auto focus : view) {
			auto& x = view.get<Camera>(focus);
			if (event.wheel.y > 0) {
				x.scale.fX *= 1.1f;
				x.scale.fY *= 1.1f;
				SDL_RenderSetScale(Graphics::renderer, x.scale.fX, x.scale.fX);
			}
			if (event.wheel.y < 0) {
				x.scale.fX *= 0.9f;
				x.scale.fY *= 0.9f;
				SDL_RenderSetScale(Graphics::renderer, x.scale.fX, x.scale.fX);
			}

		}
	}

	void Mouse_Input() {

	}
	void Select_Unit(){
	
	}

	void Player_Input() {
		while (SDL_PollEvent(&event) != 0) {
			auto view = scene.view<Velocity, Actions, Input>();
			for (auto entity : view) {
				auto& vel = view.get<Velocity>(entity);
				auto& act = view.get<Actions>(entity);
				if (event.key.type == SDL_KEYDOWN || event.key.type == SDL_KEYUP) {
					keyboardInput(vel, act);
				}
				if (event.key.type == SDL_MOUSEWHEEL) {
					zoom();
				}
				if (event.key.type == SDL_MOUSEBUTTONDOWN || event.key.type == SDL_MOUSEBUTTONUP) {
					
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


	void AI_Loop() {
		auto view = scene.view<AI, Velocity>();
		for (auto entity : view) {
			auto &mob = view.get<AI>(entity);
			auto &vel = view.get<Velocity>(entity);
			
			vel.magnitude = { 0, 1}; //direction
			mob.iCount++;
			if (mob.iCount >= mob.iMaxCount) {
				mob.iCount = 0;
			}
		}
	}

	void cameraTrack() {
		auto view = scene.view<Camera, Position_X, Position_Y>();
		for (auto focus : view) {
			auto& x = view.get<Position_X>(focus);
			auto& y = view.get<Position_Y>(focus);
			auto& componentCamera = view.get<Camera>(focus);
			
			//center camera on the entity with the component
			//scales the x, y position for the source of camera amd scales the screen after the offset is applied
			componentCamera.screen.x = (x.fX * componentCamera.scale.fX - componentCamera.screen.w ) / componentCamera.scale.fX;
			componentCamera.screen.y = (y.fY * componentCamera.scale.fY - componentCamera.screen.h ) / componentCamera.scale.fY;
			//updates the global variable that may be useful for getting scrren/world positions
			Graphics::Screen = componentCamera.screen;
		}
	}
}
