#pragma once
#include "scene.h"
#include "graphics.h"
#include <SDL.h>
#include "timer.h"
#include "interface.h"
#include "utilities.h"
#include "items.h"
#include <vector>
#include "ui.h"


namespace Camera_Control {

	void Update_Camera() {
		auto view = Scenes::scene.view<Camera, Position_X, Position_Y>();
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
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			Mouse::iXMouse = (float)mx;
			Mouse::iYMouse = (float)my;
			Mouse::iXWorld_Mouse = (Mouse::iXMouse / componentCamera.scale.fX) + componentCamera.screen.x;//getting mouse world Position corrected for scale
			Mouse::iYWorld_Mouse = (Mouse::iYMouse / componentCamera.scale.fY) + componentCamera.screen.y;//getting mouse world Position corrected for scale
			Mouse::iXMouse = Mouse::iXMouse / componentCamera.scale.fX;  // getting the screen mouse position corrected for scale
			Mouse::iYMouse = Mouse::iYMouse / componentCamera.scale.fY;  // getting the screen mouse position corrected for scale	
		}
	}
}

namespace Rendering {

	namespace {
		bool showSpriteBox = true;
		bool renderType = true;
		bool debug = false;
		float fRenderable = 0.0f;
	}

	void Render_Terrain() { //state
		auto view1 = Scenes::scene.view<Terrain_Renderable, Terrain_Position_Y, Terrain_Position_X, animation, Actions, Sprite_Offset>();
		auto view2 = Scenes::scene.view<Camera>();
		float sx;
		float sy;
		for (auto id : view2) {
			auto& camera_offset = view2.get<Camera>(id);
			for (auto entity : view1) {
				auto& anim = view1.get<animation>(entity);
				auto& x = view1.get<Terrain_Position_X>(entity);
				auto& y = view1.get<Terrain_Position_Y>(entity);
				auto& act = view1.get<Actions>(entity);
				auto& position = view1.get<Sprite_Offset>(entity);
				//only fire this at 60 frames/sec
				//anim.renderPosition = anim.sheet[act.action].clip;										//save sprite for vector
				anim.clipSprite = anim.sheet[act.action].clip;											//save position for renderer
				sx = x.fX - camera_offset.screen.x;	
				sy = y.fY - camera_offset.screen.y;
				anim.renderPosition.x = sx - position.offset.fX;
				anim.renderPosition.y = sy - position.offset.fY;
				SDL_RenderCopy(Graphics::renderer, anim.pTexture, &anim.clipSprite, &anim.renderPosition);
			}
		}
	}

	void sort_Positions() {
		
		Scenes::scene.sort<Renderable>([](const auto& lhs, const auto& rhs) { return lhs.y < rhs.y; }); //sorts position least to	
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
					}
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

		if (act.action == dead) {
			if (act.frameCount[act.action].currentFrame < act.frameCount[act.action].NumFrames) {
				act.frameCount[act.action].currentFrame++;
			}
			x.x += (x.w * act.frameCount[act.action].currentFrame);
		}
		return x;
	}


	SDL_Rect Scale_Sprite_for_Render(SDL_Rect& clippedSprite, float& scale) {
		SDL_FRect fScaledImage = Utilities::SDL_Rect_To_SDL_FRect(clippedSprite);
		fScaledImage = {
			fScaledImage.x - (fScaledImage.w * scale),
			fScaledImage.y - (fScaledImage.h * scale),
			fScaledImage.w * scale,
			fScaledImage.h * scale
		};

		return Utilities::SDL_FRect_To_SDL_Rect(fScaledImage);
	}

	void Animation_Frame() { //state

		SDL_Rect xClipPos;
		float sx;
		float sy;
		auto view1 = Scenes::scene.view<Renderable, Position_Y, Position_X, animation, Actions, Direction, Sprite_Offset, Scale>();
		auto view2 = Scenes::scene.view<Camera>();

		for (auto id : view2) {
			auto& camera_offset = view2.get<Camera>(id).screen;
			for (auto entity : view1) {
				auto& d = view1.get<Direction>(entity);
				auto& scale = view1.get<Scale>(entity).scale;
				auto& anim = view1.get<animation>(entity);
				auto& x = view1.get<Position_X>(entity);
				auto& y = view1.get<Position_Y>(entity);
				auto& act = view1.get<Actions>(entity);
				auto& offset = view1.get<Sprite_Offset>(entity).offset;
				//only fire this at 60 frames/sec
				anim.sheet[act.action].currentFrameTime += Timer::timeStep;
				if (anim.sheet[act.action].currentFrameTime >= anim.sheet[act.action].timeBetweenFrames) {
					anim.sheet[act.action].currentFrameTime = 0;
					xClipPos = Frame_Update(anim.sheet[act.action], d, act);//get action and direction state sprite draw from
					anim.renderPosition = Scale_Sprite_for_Render(xClipPos, scale);										//save sprite for vector
					anim.clipSprite = xClipPos;											//save position for renderer			
				}
				sx = x.fX - camera_offset.x;
				sy = y.fY - camera_offset.y;
								
				anim.renderPosition.x = sx - offset.fX;
				anim.renderPosition.y = sy - offset.fY;
				SDL_RenderCopy(Graphics::renderer, anim.pTexture, &anim.clipSprite, &anim.renderPosition);
				if (showSpriteBox) {
					SDL_RenderDrawRect(Graphics::renderer, &anim.renderPosition);					
				}
			}
		}
	}

	SDL_Rect Explosion_Frame_Update(int currentFrame, int maxFrames, SDL_Rect frameToUpdateClipOf) {
		//increment to next frame if the spritesheet
		// 128 x 128 pixels per
		// 8 x 8 spritesheet frames
		int row = currentFrame * 128;
		int column = currentFrame * 128;
		int width = 128;
		int height = 128;

		SDL_Rect rect = { row, column, width, height };

		return rect;
	}

	void Explosions() {
		SDL_Rect xClipPos;
		float sx;
		float sy;
		auto view = Scenes::scene.view<Explosion, Position_X, Position_Y, Frame_Delay, Texture, Sprite_Frames>();
		auto view2 = Scenes::scene.view<Camera>();

		for (auto cam : view2) {
			auto& camera_offset = view2.get<Camera>(cam);
			for (auto spell : view) {
				auto& anim = view.get<Explosion>(spell);
				auto& x = view.get<Position_X>(spell);
				auto& y = view.get<Position_Y>(spell);
				auto& texture = view.get<Texture>(spell);
				auto& frames = view.get<Sprite_Frames>(spell);
				auto& delay = view.get<Frame_Delay>(spell);

				delay.currentFrameTime += Timer::timeStep;
				if (delay.currentFrameTime >= delay.timeBetweenFrames) {
					if (frames.currentFrame <= frames.maxFrames) { // if there are still frames remaining
					//only fire this at 60 frames/sec

						xClipPos = Explosion_Frame_Update(frames.currentFrame, frames.maxFrames, texture.clippedSpriteFrame);		//get action and direction state sprite draw from
						anim.renderPosition = Utilities::SDL_Rect_To_SDL_FRect(xClipPos);		//save sprite for vector
						texture.clippedSpriteFrame = xClipPos;									//save position for renderer			
						frames.currentFrame++;
					}
					delay.currentFrameTime = 0;
				}
				else {
					//Scenes::scene.destroy(spell);
					//continue;
				}
				sx = x.fX - camera_offset.screen.x - anim.offsetToAlignWithFireball.fX; 
				sy = y.fY - camera_offset.screen.y - anim.offsetToAlignWithFireball.fY;
				anim.renderPosition.x = sx - anim.posOffset.x;
				anim.renderPosition.y = sy - anim.posOffset.y;
				SDL_RenderCopyF(Graphics::renderer, texture.pTexture, &texture.clippedSpriteFrame, &anim.renderPosition);
				if (showSpriteBox) {
					SDL_RenderDrawRectF(Graphics::renderer, &anim.renderPosition);
				}
			}
		}
	}

	void isDead() {
		auto view = Scenes::scene.view<Actions, Health, Velocity>(entt::exclude<Spell>);
		for (auto entity : view) {
			auto& health = view.get<Health>(entity);
			if (health.iHealth <= 0) {
				view.get<Actions>(entity).action = dead;
				view.get<Actions>(entity).frameCount[view.get<Actions>(entity).action].currentFrame = 0;
				view.get<Velocity>(entity).magnitude.fX = 0.0f;
				view.get<Velocity>(entity).magnitude.fY = 0.0f;

				Scenes::scene.remove<Commandable>(entity);
				Scenes::scene.remove<Selected>(entity);
				Scenes::scene.remove<Moving>(entity);
				Scenes::scene.remove<Mouse_Move>(entity);
				Scenes::scene.remove<Velocity>(entity);
				Scenes::scene.remove<Spellbook>(entity);
				Scenes::scene.remove<Mass>(entity);
			}
		}
	}

	void RenderCullMode() {
		if (renderType == true) {
			renderType = false;
			Scenes::scene.clear<Renderable>();
			}
		else {
			renderType = true;
			Scenes::scene.clear<Renderable>();
		}
	}

	void Component_Renerable() {
		auto view = Scenes::scene.view<Camera>();
		//system("CLS");
		int i = 0;
		int j = 0;

		for (auto id : view) {
			auto& camera = view.get<Camera>(id);
			SDL_FRect screen = { camera.screen.x - (camera.screen.w / 2),camera.screen.y - (camera.screen.h / 2), camera.screen.w * 2, camera.screen.h * 2 };

			auto view2 = Scenes::scene.view<Terrain_Position_Y, Terrain_Position_X>();
			for (auto pos : view2) {
				auto& x = view2.get<Terrain_Position_X>(pos);
				auto& y = view2.get<Terrain_Position_Y>(pos);
				SDL_FPoint point = { x.fX,y.fY };
				if (SDL_PointInFRect(&point, &screen)) {
					Scenes::scene.emplace_or_replace<Terrain_Renderable>(pos);
					i++;
				}
				else {
					Scenes::scene.remove<Terrain_Renderable>(pos);
				}
			}

			auto view3 = Scenes::scene.view<Position_Y, Position_X>();
			for (auto pos : view3) {
				auto& x = view3.get<Position_X>(pos);
				auto& y = view3.get<Position_Y>(pos);
				SDL_FPoint point = { x.fX,y.fY };
				if (SDL_PointInFRect(&point, &screen)) {
					Scenes::scene.emplace_or_replace<Renderable>(pos, y.fY);
					j++;
				}
				else {
					Scenes::scene.remove<Renderable>(pos);
				}
			}
			//std::cout << "entt render" << std::endl;
			//std::cout << scene.view<Terrain_Renderable>().size() << "  /  " << i << std::endl;
			//std::cout << "-------------------------------" << std::endl;
			//std::cout << scene.view<Renderable>().size() << "  /  " << j << std::endl;
		}			
	}

	void Vector_Renderable(Map::Node3& terrain) {
	// check: grid, military structure, player (maybe a seperate grid for terrain?)
		// NEED to update grid position every frame to maake this work for mobile units
		if (debug) {
			std::cout << "--- Starting Check_Renderable() --- = Good" << std::endl;
		}

		int grassnum = 0;
		int grassremoved = 0;
		int unitnum = 0;
		int unitremoved = 0;
		int playernum = 0;
		int playerremoved = 0;
		int armynum = 0;
		int armyremoved = 0;
		int spellnum = 0;
		int spellremoved = 0;

		auto view = Scenes::scene.view<Camera>();

		//SDL_SetRenderDrawColor(renderer, 255, 155, 255, 255);

		for (auto id : view) {
			auto& camera = view.get<Camera>(id);
			SDL_FRect screen = { camera.screen.x - (camera.screen.w / 2),camera.screen.y - (camera.screen.h / 2), camera.screen.w * 2, camera.screen.h * 2 };
			//SDL_FRect drawRect = screen;

			//SDL_RenderDrawRectF(renderer, &drawRect);
			//SDL_FRect screen = camera.screen;

			std::vector<entt::entity>grassvec(Scenes::scene.view<Terrain_Renderable>().size());
			//grassvec.reserve(scene.view<Terrain_Renderable>().size());
			std::vector<entt::entity>grassvecremove(Scenes::scene.view<Terrain_Renderable>().size());
			//grassvecremove.reserve(scene.view<Terrain_Renderable>().size());

			if (Utilities::bRect_Intersect(screen, terrain.sCollide_Box)) {// checks terrain for visibility like grass and such	
				for (int i = 0; i < Map::size; i++) {
					if (Utilities::bRect_Intersect(screen, terrain.nodes[i].sCollide_Box)) {
						for (int j = 0; j < Map::size; j++) {
							if (Utilities::bRect_Intersect(screen, terrain.nodes[i].nodes[j].sCollide_Box)) {
								for (int k = 0; k < Map::size; k++) {
									if (Utilities::bRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
										for (int l = 0; l < Map::size; l++) {
											if (Utilities::bRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
												for (int a = 0; a < terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
												//	scene.emplace_or_replace<Terrain_Renderable>(terrain.nodes[i].nodes[j].nodes[k].cells[l].entities[a]);
													grassvec.emplace_back(terrain.nodes[i].nodes[j].nodes[k].cells[l].entities[a]);
													grassnum++;
												}
											}
											else {
												for (int a = 0; a < terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
												//	scene.remove<Terrain_Renderable>(terrain.nodes[i].nodes[j].nodes[k].cells[l].entities[a]);
													grassvecremove.emplace_back(terrain.nodes[i].nodes[j].nodes[k].cells[l].entities[a]);
													grassremoved++;
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

					
			//std::cout << "vector render" << std::endl;
			//std::cout << "--------------------" << std::endl;
			//std::cout << grassvec.size() << std::endl;
			//std::cout << grassvecremove.size() << std::endl;
			for (int i = 0; i < grassvec.size(); i++) {
				Scenes::scene.emplace_or_replace<Terrain_Renderable>(grassvec[i]);
			}
			for (int i = 0; i < grassvecremove.size(); i++) {
				Scenes::scene.remove<Terrain_Renderable>(grassvecremove[i]);
			}


			std::vector<std::pair<entt::entity, float>>unitvec;
			//unitvec.resize(scene.view<Renderable>().size());
			std::vector<entt::entity>unitvecremove;
			//unitvecremove.resize(scene.view<Renderable>().size());

			if (debug) {
				std::cout << "terrain = Good" << std::endl;
			}

			if (Utilities::bRect_Intersect(screen, Map::map.sCollide_Box)) {// checks individul units for visibility
				for (int i = 0; i < Map::size; i++) {
					if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].sCollide_Box)) {
						for (int j = 0; j < Map::size; j++) {
							if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].nodes[j].sCollide_Box)) {
								for (int k = 0; k < Map::size; k++) {
									if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
										for (int l = 0; l < Map::size; l++) {
											if (Utilities::bRect_Intersect(screen, Map::map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
												for (int a = 0; a < Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
													/*Renderable& show = scene.emplace_or_replace<Renderable>(Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities[a]);
													show.y = scene.get<Position_Y>(Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities[a]).fY;*/
													unitvec.push_back({Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities[a], 15.0f});
													unitnum++;
												}
											}
											else {
												for (int a = 0; a < Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
													//scene.remove<Renderable>(Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities[a]);
													unitvecremove.push_back({Map::map.nodes[i].nodes[j].nodes[k].cells[l].entities[a]});
													unitremoved++;
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

			auto player_view = Scenes::scene.view<Input, Position_X, Position_Y>();

			for (auto player_id : player_view) {
				auto& x = player_view.get<Position_X>(player_id);
				auto& y = player_view.get<Position_Y>(player_id);
				SDL_FPoint point = { x.fX, y.fY };
				if (Utilities::bPoint_RectIntersect(point, screen)) {
					//Renderable& show = scene.emplace_or_replace<Renderable>(player_id);
					//show.y = y.fY;
					unitvec.push_back({ player_id, y.fY });
					playernum++;
				}
				else {
					//scene.remove<Renderable>(player_id);
					unitvecremove.push_back(player_id);
					playerremoved++;
				}
			}
			if (debug) {
				std::cout << "player_view = Good" << std::endl;
			}

			auto spell_view = Scenes::scene.view<Spell, Position_X, Position_Y>();

			for (auto spell_id : spell_view) {
				auto& x = spell_view.get<Position_X>(spell_id);
				auto& y = spell_view.get<Position_Y>(spell_id);
				SDL_FPoint point = { x.fX, y.fY };
				if (Utilities::bPoint_RectIntersect(point, screen)) {
					//Renderable& show = scene.emplace_or_replace<Renderable>(spell_id);
					//show.y = y.fY;
					unitvec.push_back({ spell_id, y.fY });
					spellnum++;
				}
				else {
					//scene.remove<Renderable>(spell_id);
					unitvecremove.push_back(spell_id);
					spellremoved++;
				}
			}
			if (debug) {
				std::cout << "spell_view = Good" << std::endl;
			}

			auto items_view = Scenes::scene.view<Ground_Item, Position_X, Position_Y>();

			for (auto item : items_view) {
				auto& x = items_view.get<Position_X>(item);
				auto& y = items_view.get<Position_Y>(item);
				SDL_FPoint point = { x.fX, y.fY };
				if (Utilities::bPoint_RectIntersect(point, screen)) {
					//Renderable& show = scene.emplace_or_replace<Renderable>(spell_id);
					//show.y = y.fY;
					unitvec.push_back({ item, y.fY });
					//spellnum++;
				}
				else {
					//scene.remove<Renderable>(spell_id);
					unitvecremove.push_back(item);
					//spellremoved++;
				}
			}

			auto company_view = Scenes::scene.view<Company>();

			for (auto companies : company_view) {
				auto& company = company_view.get<Company>(companies);
				if (Utilities::bRect_Intersect(company.sCollide_Box, screen)) {
					for (int c = 0; c < company.iSub_Units.size(); c++) {
						auto& platoon = Scenes::scene.get<Platoon>(company.iSub_Units[c]);
						if (Utilities::bRect_Intersect(platoon.sCollide_Box, screen)) {
							for (int p = 0; p < platoon.iSub_Units.size(); p++) {
								auto& squad = Scenes::scene.get<Squad>(platoon.iSub_Units[p]);
								if (Utilities::bRect_Intersect(squad.sCollide_Box, screen)) { //checks against itself too so that units with the squad will have collision
									for (int i = 0; i < squad.iSub_Units.size(); i++) {
										//if (squad.bAlive.at(i) == true) {
										//Renderable& show = scene.emplace_or_replace<Renderable>(squad.iSub_Units[i]);
										//show.y = scene.get<Position_Y>(squad.iSub_Units[i]).fY;
										//}
										unitvec.push_back({ squad.iSub_Units[i], squad.fPY[i]});
										armynum++;
									}
								}
								else {
									for (int i = 0; i < squad.iSub_Units.size(); i++) {
										//scene.remove<Renderable>(squad.iSub_Units[i]);
										unitvecremove.push_back(squad.iSub_Units[i]);
										armyremoved++;
									}
								}
							}
						}
					}
				}
			}

			//std::cout << "--------------------" << std::endl;
			//std::cout << unitvec.size() << std::endl;
			//std::cout << unitvecremove.size() << std::endl;
			for (int i = 0; i < unitvec.size(); i++) {
				Scenes::scene.emplace_or_replace<Renderable>(unitvec[i].first, unitvec[i].second);
						
			}
			for (int i = 0; i < unitvecremove.size(); i++) {
				Scenes::scene.remove<Renderable>(unitvecremove[i]);
			}

			if (debug) {
				std::cout << "company_view = Good" << std::endl;
			}
		}

		//system("CLS");
		//std::cout << "vector render" << std::endl;				
		////std::cout << " in Rendering.h" << std::endl;
		//std::cout << scene.view<Terrain_Renderable>().size() << "  /  " << grassnum << std::endl;
		////std::cout << grassnum << std::endl;
		//std::cout << scene.view<Renderable>().size() << "  /  " << unitnum + playernum + armynum << std::endl;
		////std::cout << unitnum + playernum + armynum << std::endl;
		///*std::cout << playernum << std::endl;
		//std::cout << armynum << std::endl;*/
		//std::cout << "----------------------------------" << std::endl;
		if (debug) {
			std::cout << "--- ending Check_Renderable() --- = Good" << std::endl;
		}
	}

	void Check_Renderable() { //doesn't need to happen every frame
		fRenderable += Timer::timeStep;
		if (fRenderable >= 100) {		
			fRenderable = 0;
			if (renderType) {
				Component_Renerable();
			}
			else {
			//	Vector_Renderable(Map::terrain);
			}
		}
	};

	void Rendering() {
		//std::cout << "SDL_RenderClear = Good" << std::endl;

		isDead();
		if (debug) {
			std::cout << "--- Starting Rendering() --- = Good" << std::endl;
		}
		Camera_Control::Update_Camera();
		if (debug) {
			std::cout << "Update_Camera = Good" << std::endl;
		}
		if (debug) {
			std::cout << "Check_Renderable = Good" << std::endl;
		}
		SDL_RenderClear(Graphics::renderer);
		Render_Terrain();
		Check_Renderable();
		sort_Positions();
		//std::cout << "Render_Terrain = Good" << std::endl;
		//std::cout << "sort_Positions = Good" << std::endl;
		Animation_Frame();
		Explosions();
		//std::cout << "Animation_Frame = Good" << std::endl;
		UI::Render_UI();
		Interface::Run_Interface();
		///std::cout << "Run_Interface = Good" << std::endl;
		SDL_RenderPresent(Graphics::renderer);
		//std::cout << "SDL_RenderPresent = Good" << std::endl;
		//SDL_SetRenderDrawColor(Graphics::renderer, 12, 20, 20, SDL_ALPHA_OPAQUE);
		//std::cout << "SDL_SetRenderDrawColor = Good" << std::endl;
		//SDL_SetRenderDrawColor(Graphics::renderer, 255, 100, 50, SDL_ALPHA_OPAQUE);
		//std::cout << "SDL_SetRenderDrawColor = Good" << std::endl;
		//std::cout << "--- Ending Rendering() ---  = Good" << std::endl;
	}
}