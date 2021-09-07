#include "pools.h"
#include "graphics.h"
#include <SDL.h>
#include "timer.h"
#include "Debug_System.h"

using namespace Scene;

namespace Rendering {

	SDL_Rect grass = { 0,0,234,234 };
	SDL_Rect cell = { 0,0,230,230 };

	void frameUpdate(spriteframes& a, Direction& b, Actions& act) { // bug :: cuts off first frame of when it starts		
		a.clip.y = a.clip.h * b.eDirection; //check which directioon is facing then change clip.y to sprite height x direction enum
		if (act.action != isStatic) {
			if (act.action != dead) {
				if (a.bReversable) {
					if (a.bReversing == true) {
						a.clip.x = (a.clip.x + a.clip.w);
						if (a.clip.x >= a.frameStart + a.sheetWidth - a.clip.w) { a.bReversing = false; };
					}
					else if (a.bReversing == false) {
						a.clip.x = (a.clip.x - a.clip.w);
						if (a.clip.x <= a.frameStart) { a.bReversing = true; };
					}
				}
				else {
					a.clip.x = (a.clip.x + a.clip.w);
					if (a.clip.x > a.frameStart + a.sheetWidth - a.clip.w) { a.clip.x = a.frameStart; };
				}

				if (act.frameCount[act.action].currentFrame >= act.frameCount[act.action].NumFrames) {
					act.frameCount[act.action].currentFrame = 0;
				}
				else {
					act.frameCount[act.action].currentFrame++;
				}
			}
		}

		else if (act.action == dead && act.frameCount[act.action].currentFrame <= act.frameCount[act.action].NumFrames) {
			act.frameCount[act.action].currentFrame++;
			a.clip.x = a.clip.x + a.clip.w;
		}
		//std::cout << act.frameCount[act.action].currentFrame << std::endl;
		//std::cout << act.action << std::endl;
	}

	void showData() {
		auto view = scene.view<Position_X, Position_Y>();
		//render x, y position to screen within a a frame that follows the entity
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);

			//print x,y  AT x,y
		};
	}


	void Draw_Tiles() {
		//2d array of tiles that are 100px by 100px
		//start the array -10000px, -100000px and end it at 10000px, 10000px
		auto view = scene.view<Camera>();
		for (auto e : view){
			auto& a = view.get<Camera>(e);
			for (int i = -100; i < 200; i++) {			
				for (int j = -100; j < 200; j++) {
					cell.x = i * 225 - a.screen.x;
					cell.y = j * 225 - a.screen.y;
					SDL_RenderCopy(Graphics::renderer, Graphics::grass_0, &grass, &cell);
				}
			}
		}

	}

	void animationFrame() { //state
		SDL_SetRenderDrawColor(Graphics::renderer, 12, 20, 20, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(Graphics::renderer);
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 100, 50, SDL_ALPHA_OPAQUE);

		Draw_Tiles();
		auto view1 = scene.view<Position_Y, Position_X, animation, Actions, Direction>();
		auto view2 = scene.view<Camera>();
		for (auto entity : view1) {
			auto& d = view1.get<Direction>(entity);
			auto& anim = view1.get<animation>(entity);
			auto& x = view1.get<Position_X>(entity);
			auto& y = view1.get<Position_Y>(entity);
			auto& act = view1.get<Actions>(entity);
			//only fire this at 60 frames/sec
			anim.sheet[act.action].timeBetweenFrames -= Timer::timeStep;
			//if (x.sheet[z.action].timeBetweenCurrent <= 0) {
			if (anim.sheet[act.action].timeBetweenFrames <= 0) {
				anim.sheet[act.action].timeBetweenFrames = 60;
				frameUpdate(anim.sheet[act.action], d, act);//get action and direction state sprite draw from
			}

			//continue update rendoring frames at full speed
			for (auto a : view2) {
				auto& camera_offset = view2.get<Camera>(a);
				anim.renderPosition = anim.sheet[act.action].clip;										//save sprite for vector
				anim.clipSprite = anim.sheet[act.action].clip;											//save position for renderer
				x.fSX = x.fX - camera_offset.screen.x;
				y.fSY = y.fY - camera_offset.screen.y;
				anim.renderPosition.x = x.fSX - anim.sheet[act.action].posOffset.x;
				anim.renderPosition.y = y.fSY - anim.sheet[act.action].posOffset.y;
				SDL_RenderCopy(Graphics::renderer, anim.pTexture, &anim.clipSprite, &anim.renderPosition);
			}
		}
		Debug_System::Debugger();
	}






	void isDead() {
		auto view = scene.view<Actions, Alive>();
		for (auto d : view) {
			auto& a = view.get<Actions>(d);
			auto& b = view.get<Alive>(d);
			if (a.action == dead && a.frameCount[a.action].currentFrame >= a.frameCount[a.action].NumFrames) {
				b.bIsAlive = false;
				scene.remove<Input>(d);
				//remove all input components
			}
		}
	}



	
}









//
//	void _slash(action & act) {
//		if numframes = 0
//			act.action = slash;
//	}
//
//	void _stab(action & act) {
//		act.action = stab;
//	}
//
//	void _block(action & act) {
//		act.action = block;
//	}
//
//	void _dead(action & act) {
//		act.action = dead;
//	}
//
//	void _xbow(action & act) {
//		act.action = xbow;
//	}
//
//}

