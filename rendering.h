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

	
}

namespace Rendering {
	bool debug = false;
	float fRenderable = 0.0f;

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

	SDL_Rect Frame_Update(spriteframes& a, Direction& b, Actions& act) {
		SDL_Rect x = a.clip;

		x.y = x.h * b.eDirection; //check which directioon is facing then change clip.y to sprite height x direction enum
		if (act.action != isStatic) {
			if (act.action != dead) {
				if (a.bReversable) {
					if (a.bReversing == true) {
						x.x += (x.w * act.frameCount[act.action].currentFrame);
						//std::cout << "forward :" << x.x << std::endl;
						if (x.x >= a.frameStart + a.sheetWidth - x.w) { a.bReversing = false; };
						act.frameCount[act.action].currentFrame++;
						return x;

					}
					else if (a.bReversing == false) {
						x.x = (x.x + (x.w * act.frameCount[act.action].currentFrame)) - x.w;
						//std::cout << "reversing :" << x.x << std::endl;
						if (x.x <= a.frameStart) { a.bReversing = true; };
						act.frameCount[act.action].currentFrame--;
						return x;
					}
				}
				
				if (!a.bReversable) {
					x.x += (x.w * act.frameCount[act.action].currentFrame);
					//std::cout << "forward :" << x.x << std::endl;
					if (x.x > a.frameStart + a.sheetWidth - x.w) {
						x.x = a.frameStart;
					};

					if (act.frameCount[act.action].currentFrame >= act.frameCount[act.action].NumFrames) {
						act.frameCount[act.action].currentFrame = 0;
						return x;
					}
					else {
						act.frameCount[act.action].currentFrame++;
						return x;
					}
				}
			}
		}

		if (act.action == dead){
			if (act.frameCount[act.action].currentFrame < act.frameCount[act.action].NumFrames) {
				act.frameCount[act.action].currentFrame++;
			}	
			x.x += (x.w * act.frameCount[act.action].currentFrame);
		}
		return x;
		//std::cout << act.frameCount[act.action].currentFrame << std::endl;
		//std::cout << act.action << std::endl;
	}
	
	void Animation_Frame() { //state
		SDL_Rect xClipPos;
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
				anim.sheet[act.action].currentFrameTime += Timer::timeStep;
				if (anim.sheet[act.action].currentFrameTime >= anim.sheet[act.action].timeBetweenFrames) {
					anim.sheet[act.action].currentFrameTime = 0;
					xClipPos = Frame_Update(anim.sheet[act.action], d, act);//get action and direction state sprite draw from
					anim.renderPosition = xClipPos;										//save sprite for vector
					anim.clipSprite = xClipPos;											//save position for renderer			
				}
				x.fSX = x.fX - camera_offset.screen.x;
				y.fSY = y.fY - camera_offset.screen.y;
				anim.renderPosition.x = x.fSX - anim.sheet[act.action].posOffset.x;
				anim.renderPosition.y = y.fSY - anim.sheet[act.action].posOffset.y;
				SDL_RenderCopy(Graphics::renderer, anim.pTexture, &anim.clipSprite, &anim.renderPosition);	
			}
		}
	}

	/*
	* Sets unit as dead
	* Stops units movement
	* removes interaction components
	*/
	void isDead() {
		auto view = scene.view<Actions, Alive, Velocity>(entt::exclude<Spell>);
		for (auto entity : view) {
			auto& b = view.get<Alive>(entity);
			if (b.bIsAlive == false) {				
				view.get<Actions>(entity).action = dead;				
				view.get<Actions>(entity).frameCount[view.get<Actions>(entity).action].currentFrame = 0;
				view.get<Velocity>(entity).magnitude.fX = 0.0f;
				view.get<Velocity>(entity).magnitude.fY = 0.0f;
				
				scene.remove<Commandable>(entity);
				scene.remove<Selected>(entity);
				scene.remove<Moving>(entity);
				scene.remove<Mouse_Move>(entity);
				scene.remove<Velocity>(entity);
			}
		}
	}


	void Check_Renderable(Map::Node3& terrain) { //doesn't need to happen every frame
		// check: grid, military structure, player (maybe a seperate grid for terrain?)
		// NEED to update grid position every frame to maake this work for mobile units
		if (debug) {
			std::cout << "--- Starting Check_Renderable() --- = Good" << std::endl;
		}
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
				if (debug) {
					std::cout << "terrain = Good" << std::endl;
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
				if (debug) {
					std::cout << "Map::map = Good" << std::endl;
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
				if (debug) {
					std::cout << "player_view = Good" << std::endl;
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
				if (debug) {
					std::cout << "spell_view = Good" << std::endl;
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
											//if (squad.bAlive.at(i) == true) {
												Renderable& show = scene.emplace_or_replace<Renderable>(squad.iSub_Units[i]);
												show.y = scene.get<Position_Y>(squad.iSub_Units[i]).fY;
											//}
											armynum++;
										}
									}
								}
							}
						}
					}
				}
				if (debug) {
					std::cout << "company_view = Good" << std::endl;
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
		if (debug) {
			std::cout << "--- ending Check_Renderable() --- = Good" << std::endl;
		}
	};

	void Rendering() {
		isDead();
		if (debug) {
			std::cout << "--- Starting Rendering() --- = Good" << std::endl;
		}
		Camera_Control::Update_Camera();
		if (debug) {
			std::cout << "Update_Camera = Good" << std::endl;
		}
		Check_Renderable(Map::terrain);
		if (debug) {
			std::cout << "Check_Renderable = Good" << std::endl;
		}
		Render_Terrain();
		//std::cout << "Render_Terrain = Good" << std::endl;
		sort_Positions();
		//std::cout << "sort_Positions = Good" << std::endl;
		Animation_Frame();
		//std::cout << "Animation_Frame = Good" << std::endl;
		Interface::Run_Interface();
		///std::cout << "Run_Interface = Good" << std::endl;
		SDL_RenderPresent(Graphics::renderer);
		//std::cout << "SDL_RenderPresent = Good" << std::endl;
		SDL_SetRenderDrawColor(Graphics::renderer, 12, 20, 20, SDL_ALPHA_OPAQUE);
		//std::cout << "SDL_SetRenderDrawColor = Good" << std::endl;
		SDL_RenderClear(Graphics::renderer);
		//std::cout << "SDL_RenderClear = Good" << std::endl;
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 100, 50, SDL_ALPHA_OPAQUE);
		//std::cout << "SDL_SetRenderDrawColor = Good" << std::endl;
		//std::cout << "--- Ending Rendering() ---  = Good" << std::endl;
	}


	
}










