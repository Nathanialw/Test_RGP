#include "scene.h"
#include "graphics.h"
#include <SDL.h>
#include "timer.h"
#include "interface.h"

using namespace Scene;

namespace Camera_Control {

	void Update_Camera() {
		auto view = scene.view<Camera, Position_X, Position_Y>();
		for (auto focus : view) {
			auto& x = view.get<Position_X>(focus);
			auto& y = view.get<Position_Y>(focus);
			auto& componentCamera = view.get<Camera>(focus);
			//center camera on the entity with the component
			componentCamera.screen.w = Graphics::resolution.w / componentCamera.scale.fX;
			componentCamera.screen.h = Graphics::resolution.h / componentCamera.scale.fY;
			componentCamera.screen.x = ((x.fX) - (componentCamera.screen.w / 2));
			componentCamera.screen.y = ((y.fY) - (componentCamera.screen.h / 2));

			//std::cout << componentCamera.screen.x << std::endl;
			//updates the global variable that may be useful for getting scrren/world positions
			Graphics::Screen = componentCamera.screen;
			//update mouse
			SDL_GetMouseState(&Mouse::iXMouse, &Mouse::iYMouse);
			Mouse::iXWorld_Mouse = (Mouse::iXMouse / componentCamera.scale.fX) + componentCamera.screen.x;//getting mouse world Position corrected for scale
			Mouse::iYWorld_Mouse = (Mouse::iYMouse / componentCamera.scale.fY) + componentCamera.screen.y;//getting mouse world Position corrected for scale
			Mouse::iXMouse = Mouse::iXMouse / componentCamera.scale.fX;  // getting the screen mouse position corrected for scale
			Mouse::iYMouse = Mouse::iYMouse / componentCamera.scale.fY;  // getting the screen mouse position corrected for scale	
		}
	}

	void Update_Mouse() {

	}
}

namespace Rendering {
	float fRenderable = 0.0f;

	void sort_Positions() {
		//scene.sort<Position_X>([](const auto& lhs, const auto& rhs) { return lhs.fPX < rhs.fPX; }); //sorts position least to highest
		
		scene.sort<Renderable>([](const auto& lhs, const auto& rhs) { return lhs.y < rhs.y; }); //sorts position least to		
		//scene.sort<Position_Y>([](const auto& lhs, const auto& rhs) {std::cout << rhs.fPY << std::endl; return lhs.fPY < rhs.fPY; }); //sorts position least to		
		//auto view = scene.view<Renderable>();
		//for (auto i : view) {			
		//	std::cout << view.get<Renderable>(i).y << std::endl;
		//}
		//system("CLS");
	}

	void Frame_Update(spriteframes& a, Direction& b, Actions& act) { // bug :: cuts off first frame of when it starts	
		
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
		auto view = scene.view<Position_X, Position_Y, Renderable>(entt::exclude<Terrain>);
		//render x, y position to screen within a a frame that follows the entity
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);

			//print x,y  AT x,y
		};
	}
		
	void Render_Terrain() { //state
		auto view1 = scene.view<Terrain_Position_Y, Terrain_Position_X, animation, Actions, Terrain_Renderable>();
		auto view2 = scene.view<Camera>();
		for (auto id : view2) {
			auto& camera_offset = view2.get<Camera>(id);
			for (auto entity : view1) {
				auto& anim = view1.get<animation>(entity);
				auto& x = view1.get<Terrain_Position_X>(entity);
				auto& y = view1.get<Terrain_Position_Y>(entity);
				auto& act = view1.get<Actions>(entity);
				//only fire this at 60 frames/sec
				//anim.renderPosition = anim.sheet[act.action].clip;										//save sprite for vector
				anim.clipSprite = anim.sheet[act.action].clip;											//save position for renderer
				x.fSX = x.fX - camera_offset.screen.x;
				y.fSY = y.fY - camera_offset.screen.y;
				anim.renderPosition.x = x.fSX - anim.sheet[act.action].posOffset.x;
				anim.renderPosition.y = y.fSY - anim.sheet[act.action].posOffset.y;
				SDL_RenderCopy(Graphics::renderer, anim.pTexture, &anim.clipSprite, &anim.renderPosition);				
			}
		}
	}

	void Animation_Frame() { //state
		
		auto view1 = scene.view<Position_Y, Position_X, animation, Actions, Direction, Renderable>();
		auto view2 = scene.view<Camera>();
		for (auto id : view2) {
			auto& camera_offset = view2.get<Camera>(id);
			for (auto entity : view1) {
				auto& d = view1.get<Direction>(entity);
				auto& anim = view1.get<animation>(entity);
				auto& x = view1.get<Position_X>(entity);
				auto& y = view1.get<Position_Y>(entity);
				auto& act = view1.get<Actions>(entity);
				//only fire this at 60 frames/sec
				anim.sheet[act.action].timeBetweenFrames += Timer::timeStep;
				if (anim.sheet[act.action].timeBetweenFrames >= 75) {
					anim.sheet[act.action].timeBetweenFrames = 0;
					Frame_Update(anim.sheet[act.action], d, act);//get action and direction state sprite draw from
				}
				anim.renderPosition = anim.sheet[act.action].clip;										//save sprite for vector
				anim.clipSprite = anim.sheet[act.action].clip;											//save position for renderer
				x.fSX = x.fX - camera_offset.screen.x;
				y.fSY = y.fY - camera_offset.screen.y;
				anim.renderPosition.x = x.fSX - anim.sheet[act.action].posOffset.x;
				anim.renderPosition.y = y.fSY - anim.sheet[act.action].posOffset.y;
				SDL_RenderCopy(Graphics::renderer, anim.pTexture, &anim.clipSprite, &anim.renderPosition);	
			}
		}
	}

	void isDead() {
		auto view = scene.view<Actions, Alive>();
		for (auto d : view) {
			auto& a = view.get<Actions>(d);
			auto& b = view.get<Alive>(d);
			if (a.action == dead && a.frameCount[a.action].currentFrame >= a.frameCount[a.action].NumFrames) {
				b.bIsAlive = false;
				scene.remove<Input>(d);
			}
		}
	}


	void Check_Renderable(Map::Node3& terrain) { //doesn't need to happen every frame
		// check: grid, military structure, player (maybe a seperate grid for terrain?)
		// NEED to update grid position every frame to maake this work for mobile units
		fRenderable += Timer::timeStep;
		int grassnum = 0;
		int unitnum = 0;
		int playernum = 0;
		int armynum = 0;
		int spellnum = 0;

		if (fRenderable > 10) {
			fRenderable = 0;
			scene.clear<Renderable>();
			scene.clear<Terrain_Renderable>();
			auto view = scene.view<Camera>();

			SDL_SetRenderDrawColor(renderer, 255, 155, 255, 255);
									
			for (auto id : view) {
				auto& camera = view.get<Camera>(id);
				SDL_FRect screen = {camera.screen.x - (camera.screen.w * 0.5),camera.screen.y - (camera.screen.h * 0.5), camera.screen.w * 2, camera.screen.h * 2};
				SDL_FRect debug;

				if (Utilities::bRect_Intersect(screen, terrain.sCollide_Box)) {// checks terrain for visibility like grass and such	
					for (int i = 0; i < 16; i++) {
						if (Utilities::bRect_Intersect(screen, terrain.nodes[i].sCollide_Box)) {
							for (int j = 0; j < 16; j++) {
								if (Utilities::bRect_Intersect(screen, terrain.nodes[i].nodes[j].sCollide_Box)) {
									for (int k = 0; k < 16; k++) {
										if (Utilities::bRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
											for (int l = 0; l < 16; l++) {
												if (Utilities::bRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
													for (int a = 0; a < terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
														scene.emplace_or_replace<Terrain_Renderable>(terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.at(a));
																												
														/*debug.x = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.x - screen.x;
														debug.y = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.y - screen.y;
														debug.w = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.w;
														debug.h = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.h;
														SDL_RenderDrawRectF(Graphics::renderer, &debug);*/
														grassnum++;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}

				if (Utilities::bRect_Intersect(screen, Map::map.sCollide_Box)) {// checks individul units for visibility
					for (int i = 0; i < 16; i++) {
						if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].sCollide_Box)) {
							for (int j = 0; j < 16; j++) {
								if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].nodes[j].sCollide_Box)) {
									for (int k = 0; k < 16; k++) {
										if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
											for (int l = 0; l < 16; l++) {
												if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
													for (int a = 0; a < Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
														Renderable& show = scene.emplace_or_replace<Renderable>(Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(a));
														show.y = scene.get<Position_Y>(Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(a)).fY;
														/*debug.x = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.x - screen.x;
														debug.y = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.y - screen.y;
														debug.w = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.w;
														debug.h = Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.h;
														SDL_RenderDrawRectF(Graphics::renderer, &debug);*/
														unitnum++;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}

				auto player_view = scene.view<Input, Position_X, Position_Y>();

				for (auto player_id : player_view) {
					auto& x = player_view.get<Position_X>(player_id);
					auto& y = player_view.get<Position_Y>(player_id);
					SDL_FPoint point = { x.fX, y.fY };
					if (Utilities::bPoint_RectIntersect(point, screen)) {
						Renderable& show = scene.emplace_or_replace<Renderable>(player_id);
						show.y = y.fY;
						playernum++;
					}
				}

				auto spell_view = scene.view<Spell, Position_X, Position_Y>();

				for (auto spell_id : spell_view) {
					auto& x = spell_view.get<Position_X>(spell_id);
					auto& y = spell_view.get<Position_Y>(spell_id);
					SDL_FPoint point = { x.fX, y.fY };
					if (Utilities::bPoint_RectIntersect(point, screen)) {
						Renderable& show = scene.emplace_or_replace<Renderable>(spell_id);
						show.y = y.fY;
						spellnum++;
					}
				}
				
				auto company_view = scene.view<Company>();
				
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bRect_Intersect(company.sCollide_Box, screen)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = scene.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bRect_Intersect(platoon.sCollide_Box, screen)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
									auto& squad = scene.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bRect_Intersect(squad.sCollide_Box, screen)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											Renderable& show = scene.emplace_or_replace<Renderable>(squad.iSub_Units[i]);
											show.y = scene.get<Position_Y>(squad.iSub_Units[i]).fY;
											armynum++;
										}
									}
								}
							}
						}
					}
				}	
			}
		}

		//std::cout << " in Rendering.h" << std::endl;
		//std::cout << grassnum << std::endl;
		//std::cout << unitnum << std::endl;
		//std::cout << playernum << std::endl;
		//std::cout << armynum << std::endl;
		//std::cout << "----------------------------------" << std::endl;
		//system("CLS");
	};

	void Rendering() {
		Camera_Control::Update_Camera();
		Check_Renderable(Map::terrain);
		Render_Terrain();
		sort_Positions();
		Animation_Frame();
		Interface::Run_Interface();
		Interface::Unit_Arrive_UI();
		SDL_RenderPresent(Graphics::renderer);
		SDL_SetRenderDrawColor(Graphics::renderer, 12, 20, 20, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(Graphics::renderer);
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 100, 50, SDL_ALPHA_OPAQUE);
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

