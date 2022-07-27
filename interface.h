#pragma once
#include <SDL.h>
#include "mouse_control.h"
#include "camera.h"
#include "graphics.h"
#include "debug_system.h"
#include "components.h"


namespace Interface {

	namespace {
		int iMousePollRate;
		Graphics::Surface_Data mouseX;
		Graphics::Surface_Data mouseY;
		int gridDepth = -1;
	}

	void Update_Zoom(SDL_Event& e) {
		auto view = World::zone.view<Camera>();
		for (auto focus : view) {
			auto& x = view.get<Camera>(focus).scale;
			if (e.wheel.y > 0) {
				x.fX *= 1.1f;
				x.fY *= 1.1f;
				SDL_RenderSetScale(Graphics::renderer, x.fX, x.fY);
			}
			if (e.wheel.y < 0) {
				x.fX *= 0.9f;
				x.fY *= 0.9f;
				SDL_RenderSetScale(Graphics::renderer, x.fX, x.fY);
			}
		}
	}

	void Display_Military_Groups() {
		auto camera_view = World::zone.view<Camera>();
		for (auto cameras : camera_view) {
			auto& cam = camera_view.get<Camera>(cameras);

			auto squad_view = World::zone.view<Squad>();
			for (auto squads : squad_view) {
				SDL_FRect o = squad_view.get<Squad>(squads).sCollide_Box;
				o.x -= cam.screen.x;
				o.y -= cam.screen.y;
				//SDL_SetRenderDrawColor(Graphics::renderer, 155, 55, 255, 255);
				//SDL_RenderDrawRectF(Graphics::renderer, &o);
			}

			auto platoon_view = World::zone.view<Platoon>();
			for (auto platoons : platoon_view) {
				auto& platoon = platoon_view.get<Platoon>(platoons);
				SDL_FRect o = platoon.sCollide_Box;
				o.x = platoon.sCollide_Box.x - cam.screen.x;
				o.y = platoon.sCollide_Box.y - cam.screen.y;
				//SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 255);
				//SDL_RenderDrawRectF(Graphics::renderer, &o);
			}		
			
			auto company_view = World::zone.view<Company>();
			for (auto companies : company_view) {
				auto& company = company_view.get<Company>(companies);
				SDL_FRect o = company.sCollide_Box;
				o.x = company.sCollide_Box.x - cam.screen.x;
				o.y = company.sCollide_Box.y - cam.screen.y;
				//SDL_SetRenderDrawColor(Graphics::renderer, 0, 200, 255, 255);
				//SDL_RenderDrawRectF(Graphics::renderer, &o);
			}
		}
	}

	void Display_Unite_Formations(Component::Camera &camera) {
		int i = 0;
		auto unitFormations_view = World::zone.view<Test::Unit_Formation_Data>();
		for (auto unitFormation : unitFormations_view) {
			i++;
			SDL_FRect formationRect = unitFormations_view.get<Test::Unit_Formation_Data>(unitFormation).sCollide_Box;
			formationRect.x -= camera.screen.x;
			formationRect.y -= camera.screen.y;
			SDL_SetRenderDrawColor(Graphics::renderer, 155, 55, 255, 255);
			SDL_RenderDrawRectF(Graphics::renderer, &formationRect);
		}
		std::cout << i << std::endl;
	}


	void Display_Selected() {
		auto view1 = World::zone.view<Camera>();
		auto view = World::zone.view<Selected, Position, Radius>();
		SDL_Color a = { 155, 255, 50, 255 };
		for (auto focus : view1) {
			for (auto entity : view) {
				auto& x = view.get<Position>(entity).x;
				auto& y = view.get<Position>(entity).y;
				auto& d = view.get<Radius>(entity).fRadius;
				SDL_SetRenderDrawColor(Graphics::renderer, 55, 255, 55, 255);
				SDL_FRect p = { x - d, y - d, d * 2, d * 2 };
				
				SDL_FRect s = Camera_Control::Convert_Rect_To_Screen_Coods(World::zone, p);
				
				SDL_RenderDrawRectF(Graphics::renderer, &s);
				//Debug_System::Entity_Data_Debug(x, y, x, y);
			}
		}
	}


	void Display_Selection_Box() {
		if (Mouse::bLeft_Mouse_Pressed) {
			SDL_SetRenderDrawColor(Graphics::renderer, 55, 255, 55, 255);
			SDL_FRect p = { Mouse::Mouse_Selection_Box_x_Display, Mouse::Mouse_Selection_Box_y_Display, Mouse::iXMouse - Mouse::Mouse_Selection_Box_x_Display, Mouse::iYMouse - Mouse::Mouse_Selection_Box_y_Display };
			SDL_RenderDrawRectF(Graphics::renderer, &p);
		}
	}


	void Display_Mouse() {
		auto view = World::zone.view<Camera>();
		for (auto focus : view) {
			auto& componentCamera = view.get<Camera>(focus);

			SDL_Rect srcRect = { 0, 0 , 32, 32 };
			SDL_FRect d = { Mouse::iXMouse, Mouse::iYMouse, 32.0f / componentCamera.scale.fX, 32.0f / componentCamera.scale.fY };
			
			SDL_RenderCopyF(Graphics::renderer, Graphics::cursor_0, &srcRect, &d);
			
		}
	}


	
	void Show_Grid(Map::Node3& terrain) {
		SDL_SetRenderDrawColor(Graphics::renderer, 255, 155, 255, 255);
		
		if (gridDepth > 3) {
			gridDepth = 3;
		}
		if (gridDepth < -1) {
			gridDepth = -1;
		}
		else {
			auto view = World::zone.view<Camera>();

			for (auto id : view) {
				auto& camera = view.get<Camera>(id);
				SDL_FRect offset = camera.screen;
				SDL_FRect screen = { camera.screen.x - (camera.screen.w * 0.5f),camera.screen.y - (camera.screen.h * 0.5f), camera.screen.w * 2.0f, camera.screen.h * 2.0f };
				//SDL_FRect debug;


				if (Utilities::bFRect_Intersect(screen, terrain.sCollide_Box)) {// checks terrain for visibility like grass and such	
					for (int i = 0; i < Map::size; i++) {
						if (gridDepth == 0) {
							SDL_FRect o = terrain.sCollide_Box;
							o.x -= offset.x;
							o.y -= offset.y;
							SDL_RenderDrawRectF(Graphics::renderer, &o);
						}
						if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].sCollide_Box)) {
							for (int j = 0; j < Map::size; j++) {
								if (gridDepth == 1) {
									SDL_FRect o = terrain.nodes[i].sCollide_Box;
									o.x -= offset.x;
									o.y -= offset.y;
									SDL_RenderDrawRectF(Graphics::renderer, &o);
								}
								if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].nodes[j].sCollide_Box)) {
									for (int k = 0; k < Map::size; k++) {
										if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
											for (int l = 0; l < Map::size; l++) {
												if (gridDepth == 2) {
													SDL_FRect o = terrain.nodes[i].nodes[j].nodes[k].sCollide_Box;
													o.x -= offset.x;
													o.y -= offset.y;
													SDL_RenderDrawRectF(Graphics::renderer, &o);
												}
												if (Utilities::bFRect_Intersect(screen, terrain.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
													for (int a = 0; a < terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); a++) {
														if (gridDepth == 3) {
															SDL_FRect o = terrain.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box;
															o.x -= offset.x;
															o.y -= offset.y;
															SDL_RenderDrawRectF(Graphics::renderer, &o);
														}
													}
													//std::cout << terrain.nodes[i].nodes[j].nodes[k].cells[l].entities.size() << std::endl;
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
		}
	}

	void Show_Attacks() {
		SDL_SetRenderDrawColor(Graphics::renderer, 155, 155, 55, 255);
		auto weapon_view = World::zone.view<Weapon_Size>();
		auto camera_view = World::zone.view<Camera>();
		for (auto camera : camera_view) {
			auto& cam = camera_view.get<Camera>(camera);
			SDL_FRect offset = cam.screen;
			for (auto entity : weapon_view) {
				auto& o = weapon_view.get<Weapon_Size>(entity).attackArea;
				
				o.x -= offset.x;
				o.y -= offset.y;
				SDL_RenderDrawRectF(Graphics::renderer, &o);
			}
		}
	}

	void Unit_Arrive_UI() {
		auto view = World::zone.view<Mouse_Move>();
		auto view2 = World::zone.view<Camera>();
		for (auto camera : view2) {
			auto& cam = view2.get<Camera>(camera);
			for (auto entity : view) {
				auto& mov = view.get<Mouse_Move>(entity);
				SDL_FRect o = { (mov.fX_Destination - 15.0f) -  cam.screen.x, (mov.fY_Destination - 15.0f) - cam.screen.y, 30.0f, 30.0f };
				SDL_SetRenderDrawColor(Graphics::renderer, 155, 155, 255, 255);				
				SDL_RenderFillRectF(Graphics::renderer, &o);
			}
		}
	}

	void Run_Interface(entt::registry &zone) {
		auto camera_view = zone.view<Camera>();
		for (auto cameras : camera_view) {
			auto &camera = camera_view.get<Camera>(cameras);

			Show_Grid(Map::terrain);
			Display_Military_Groups();
			//Display_Unite_Formations(camera);
			Debug_System::Debugger();
			//Unit_Arrive_UI();
			Display_Selected();
			Display_Mouse();
			Display_Selection_Box();
			Show_Attacks();
		}
	}
}