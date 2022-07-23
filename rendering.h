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
#include "camera.h"


namespace Rendering {

	namespace {
		bool showSpriteBox = false;
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
				anim.renderPosition.x = (int)(sx - position.offset.fX);
				anim.renderPosition.y = (int)(sy - position.offset.fY);
				Graphics::Render_Rect(anim.pTexture, anim.clipSprite, anim.renderPosition);
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
				if (act.action == struck) {
					if (x.x > a.frameStart + a.sheetWidth - x.w) {
						x.x = a.frameStart;
					}
					if (act.frameCount[act.action].currentFrame >= act.frameCount[act.action].NumFrames) {
						act.frameCount[act.action].currentFrame = 0;
						act.action = idle;
						return x;
					}
					else {
						act.frameCount[act.action].currentFrame++;
						return x;
					}
				}

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
		auto view1 = Scenes::scene.view<Renderable, Position, animation, Actions, Direction, Sprite_Offset, Scale>();
		auto view2 = Scenes::scene.view<Camera>();

		for (auto id : view2) {
			auto& camera_offset = view2.get<Camera>(id).screen;
			for (auto entity : view1) {
				auto& alpha = view1.get<Renderable>(entity).alpha;
				auto& d = view1.get<Direction>(entity);
				auto& scale = view1.get<Scale>(entity).scale;
				auto& anim = view1.get<animation>(entity);
				auto& x = view1.get<Position>(entity);
				auto& y = view1.get<Position>(entity);
				auto& act = view1.get<Actions>(entity);
				auto& spriteOffset = view1.get<Sprite_Offset>(entity).offset;
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

				anim.renderPosition.x = (int)(sx - spriteOffset.fX);
				anim.renderPosition.y = (int)(sy - spriteOffset.fY);

				//fade rendering objects at bottom of screen
				SDL_SetTextureAlphaMod(anim.pTexture, alpha);

				Graphics::Render_Rect(anim.pTexture, anim.clipSprite, anim.renderPosition);
				if (showSpriteBox) {
					SDL_RenderDrawRect(Graphics::renderer, &anim.renderPosition);
				}
				
			}

			//auto view3 = Scenes::scene.view<Ground_Item>();


			//for (auto entity : view3) {
			//	auto& x = view3.get<Ground_Item>(entity).box;
			//	SDL_FRect attackRect = Utilities::worldToScreen(x, camera_offset);
			//	SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			//	SDL_RenderDrawRectF(Graphics::renderer, &attackRect);
			//}
		}

	}

	SDL_Rect Explosion_Frame_Update(Sprite_Frames &frame, SDL_Rect frameToUpdateClipOf) {
		//reset X to zero and increment Y
		if (frame.frameX >= 8) {
			frame.frameX = 0;
			frame.frameY++;
		}
		// set Output Rect values
		int row = frame.frameX * 128;
		int column = frame.frameY * 128;
		int width = 128;
		int height = 128;
		SDL_Rect rect = { row, column, width, height };
		
		//increment X
		frame.frameX++;

		return rect;
	}

	void Render_Explosions() {
		SDL_Rect xClipPos;
		float sx;
		float sy;
		auto view = Scenes::scene.view<Explosion, Position, Frame_Delay, Texture, Sprite_Frames>();
		auto view2 = Scenes::scene.view<Camera>();

		for (auto cam : view2) {
			auto& camera_offset = view2.get<Camera>(cam);
			for (auto spell : view) {
				auto& anim = view.get<Explosion>(spell);
				auto& x = view.get<Position>(spell);
				auto& y = view.get<Position>(spell);
				auto& texture = view.get<Texture>(spell);
				auto& frames = view.get<Sprite_Frames>(spell);
				auto& delay = view.get<Frame_Delay>(spell);

				delay.currentFrameTime += Timer::timeStep;
				if (delay.currentFrameTime >= delay.timeBetweenFrames) {
					if (frames.currentFrame <= frames.maxFrames) { // if there are still frames remaining
					//only fire this at 60 frames/sec

						xClipPos = Explosion_Frame_Update(frames, texture.clippedSpriteFrame);		//get action and direction state sprite draw from
						anim.renderPosition = Utilities::SDL_Rect_To_SDL_FRect(xClipPos);		//save sprite for vector
						texture.clippedSpriteFrame = xClipPos;									//save position for renderer			
						frames.currentFrame++;
					}
					else {
						//remove explosion from scene and free the entity
						Scenes::scene.destroy(spell);
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

	void Render_Mouse_Item() {
		
		SDL_Rect DisplayRect = {};
		
		auto view = Scenes::scene.view<Position, Icon, On_Mouse>();
		auto view2 = Scenes::scene.view<Camera>();

		for (auto cam : view2) {
			auto& camera = view2.get<Camera>(cam);
			for (auto item : view) {
				
				const auto& icon = view.get<Icon>(item);
				const auto& x = view.get<Position>(item).fX;
				const auto& y = view.get<Position>(item).fY;
								
				DisplayRect.x = (x - camera.screen.x) - (icon.renderPositionOffset.x / camera.scale.fX);
				DisplayRect.y = (y - camera.screen.y) - (icon.renderPositionOffset.y / camera.scale.fY);
				DisplayRect.w = icon.renderRectSize.x / camera.scale.fX;
				DisplayRect.h = icon.renderRectSize.y / camera.scale.fY;

				//std::cout << "x: " << DisplayRect.x << " y: " << DisplayRect.y << " w: " << DisplayRect.w << " h: " << DisplayRect.h << std::endl;

				SDL_RenderCopy(Graphics::renderer, icon.pTexture, &icon.clipSprite, &DisplayRect);
				if (showSpriteBox) {
					SDL_RenderDrawRect(Graphics::renderer, &DisplayRect);
				}
			}
		}
	}

	void isDead() {
		auto view = Scenes::scene.view<Actions, Health, Position, Sprite_Offset>(entt::exclude<Spell>);
		for (auto entity : view) {
			auto& health = view.get<Health>(entity);
			if (health.iHealth <= 0) {
				view.get<Actions>(entity).action = dead;
				view.get<Actions>(entity).frameCount[view.get<Actions>(entity).action].currentFrame = 0;
				auto& position = view.get<Position>(entity);
				auto& offset = view.get<Sprite_Offset>(entity).offset;


				//sets the sprite to render so that it is always rendered behind living sprites
				Items::Create_And_Drop_Item(position);
				position.fX -= offset.fX;
				position.fY -= offset.fY;				
				offset.fX = 0.0f;
				offset.fY = 0.0f;


				Scenes::scene.get<Alive>(entity).bIsAlive = false;
				Scenes::scene.remove<Commandable>(entity);
				Scenes::scene.remove<Selected>(entity);
				Scenes::scene.remove<Moving>(entity);
				Scenes::scene.remove<Potential_Position>(entity);
				Scenes::scene.remove<Mouse_Move>(entity);
				Scenes::scene.remove<Velocity>(entity);
				Scenes::scene.remove<Spellbook>(entity);
				Scenes::scene.remove<Mass>(entity);
				Scenes::scene.remove<Sight_Range>(entity);
				Scenes::scene.remove<Health>(entity);
				Scenes::scene.remove<Radius>(entity);
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

	int Set_Render_Position_Alpha(float& screenEdge, float& renderEdge, float& yPosition) {
		if (yPosition <= screenEdge) {
			return 255;
		}
		else {
			float x = yPosition - screenEdge;
			float edgeBuffer = renderEdge - screenEdge;
			float y = edgeBuffer / x;
			int alpha = 255 - ( 255 / (int)y );
			
			return alpha;
		}
	}

	void Add_Remove_Renderable_Component() {
		auto camera_view = Scenes::scene.view<Camera>();
		//system("CLS");
		int i = 0;
		int j = 0;

		for (auto camera : camera_view) {
			auto& screenRect = camera_view.get<Camera>(camera).screen;
			SDL_FRect renderRect = { 
				screenRect.x - (screenRect.w / 2), 
				screenRect.y - (screenRect.h / 2), 
				screenRect.w * 2, 
				screenRect.h * 2 };
			
			auto terrainView = Scenes::scene.view<Terrain_Position_Y, Terrain_Position_X>();
			
			for (auto terrain : terrainView) {
				auto& x = terrainView.get<Terrain_Position_X>(terrain).fX;
				auto& y = terrainView.get<Terrain_Position_Y>(terrain).fY;
				SDL_FPoint point = { x, y};
				if (SDL_PointInFRect(&point, &renderRect)) {
					Scenes::scene.emplace_or_replace<Terrain_Renderable>(terrain);
					i++;
				}
				else {
					Scenes::scene.remove<Terrain_Renderable>(terrain);
				}
			}

			auto objectsView = Scenes::scene.view<Position>();
			float bottomOfScreenEdge = screenRect.y + screenRect.h;
			float bottomOfRenderRect = renderRect.y + renderRect.h;
			
			for (auto entity : objectsView) {
				auto& x = objectsView.get<Position>(entity).fX;
				auto& y = objectsView.get<Position>(entity).fY;
				SDL_FPoint point = { x, y};
				if (SDL_PointInFRect(&point, &renderRect)) {
					int alpha = Set_Render_Position_Alpha(bottomOfScreenEdge, bottomOfRenderRect, y);
					Scenes::scene.emplace_or_replace<Renderable>(entity, y, alpha);
					j++;
				}
				else {
					Scenes::scene.remove<Renderable>(entity);
				}
			}
			//std::cout << "entt render" << std::endl;
			//std::cout << scene.view<Terrain_Renderable>().size() << "  /  " << i << std::endl;
			//std::cout << "-------------------------------" << std::endl;
			//std::cout << scene.view<Renderable>().size() << "  /  " << j << std::endl;
		}			
	}

	void Check_Renderable() { //doesn't need to happen every frame
		fRenderable += Timer::timeStep;
		if (fRenderable >= 100) {		
			fRenderable = 0;
			if (renderType) {
				Add_Remove_Renderable_Component();
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
			std::cout << "Check_Renderable = Good" << std::endl;
		}
		SDL_RenderClear(Graphics::renderer);
		Render_Terrain();
		Check_Renderable();
		sort_Positions();
		//std::cout << "Render_Terrain = Good" << std::endl;
		//std::cout << "sort_Positions = Good" << std::endl;
		Animation_Frame();
		Render_Explosions();
		//std::cout << "Animation_Frame = Good" << std::endl;
		UI::Render_UI();
		Interface::Run_Interface();
		Items::Update_Mouse_Slot_Position();
		Render_Mouse_Item(); 
	
		///std::cout << "Run_Interface = Good" << std::endl;
		SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderPresent(Graphics::renderer);
		//std::cout << "SDL_RenderPresent = Good" << std::endl;
		//SDL_SetRenderDrawColor(Graphics::renderer, 12, 20, 20, SDL_ALPHA_OPAQUE);
		//std::cout << "SDL_SetRenderDrawColor = Good" << std::endl;
		//SDL_SetRenderDrawColor(Graphics::renderer, 255, 100, 50, SDL_ALPHA_OPAQUE);
		//std::cout << "SDL_SetRenderDrawColor = Good" << std::endl;
		//std::cout << "--- Ending Rendering() ---  = Good" << std::endl;
	}
}