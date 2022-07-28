#pragma once
#include "mouse_control.h"
#include "timer.h"
#include "components.h"



namespace AI {
	
	//check for targets periodically

	//use the target x,y to moiv towards it

	//if target is in range, melee attack


	void Move_Order(entt::registry& zone, entt::entity& entity, float& x, float& y) {
		zone.emplace_or_replace<Component::Mouse_Move>(entity, x, y);
		zone.emplace_or_replace<Component::Moving>(entity);
		auto &position = zone.get<Position>(entity);
		zone.emplace_or_replace<Component::Potential_Position>(entity, position.x, position.y);
	}

	void Spell_Attack(entt::registry& zone, entt::entity& entity, float& targetX, float& targetY, const char* name) {

		if (zone.any_of<Casting>(entity) == false) { //locks out casing until cast animation has finished
			zone.emplace_or_replace<Component::Cast>(entity, targetX, targetY);
			zone.emplace_or_replace<Component::Spell_Name>(entity, name);
		}
	}

	void Melee_Attack(entt::registry& zone, entt::entity& entity, float& x, float& y) {
		if (zone.any_of<Component::Attacking>(entity) == false) { //locks out attacking until attack animation has finished
			zone.emplace_or_replace<Component::Attack>(entity, x, y);
		}
	}

	bool In_Range(entt::registry& zone, float &radius, float &x, float &y) {
		SDL_FRect unit_collide_rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);

		auto company_view = zone.view<Component::Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Component::Company>(companies);
			if (Utilities::bFRect_Intersect(company.sCollide_Box, unit_collide_rect)) {
				for (int c = 0; c < company.iSub_Units.size(); c++) {
					auto& platoon = World::zone.get<Component::Platoon>(company.iSub_Units[c]);
					if (Utilities::bFRect_Intersect(platoon.sCollide_Box, unit_collide_rect)) {
						for (int p = 0; p < platoon.iSub_Units.size(); p++) {
							auto& squad = World::zone.get<Component::Squad>(platoon.iSub_Units[p]);
							if (Utilities::bFRect_Intersect(squad.sCollide_Box, unit_collide_rect)) { //checks against itself too so that units with the squad will have collision
								for (int i = 0; i < squad.iSub_Units.size(); i++) {
									if (squad.bAlive.at(i) != false) {

										//check if the unit is in range
										float tarx = squad.fPX.at(i);
										float tary = squad.fPY.at(i);
										float tarradius = squad.fRadius.at(i);
										SDL_FRect target_collide_rect = Utilities::Get_FRect_From_Point_Radius(tarradius, tarx, tary);
										if (Utilities::bFRect_Intersect(unit_collide_rect, target_collide_rect)) {
											//std::cout << "attacking" << std::endl;
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	bool Player_In_Melee_Range(entt::registry& zone, float& radius, float &x, float &y) {
		
		SDL_FRect unit_collide_rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);
		
		auto units = zone.view<Component::Input, Component::Position, Component::Radius>();
		for (auto unit : units) {
			float tarx = units.get<Component::Position>(unit).x;
			float tary = units.get<Component::Position>(unit).y;
			float tarradius = units.get<Component::Radius>(unit).fRadius;
			
			SDL_FRect target_collide_rect = Utilities::Get_FRect_From_Point_Radius(tarradius, tarx, tary);
			
			if (Utilities::bFRect_Intersect(unit_collide_rect, target_collide_rect)) {
				return true;
			}

		}
		return false;
	}

	void Mouse_Attack_Move(entt::registry& zone) { // maybe change to move and attack?
		if (zone.empty<Component::Selected>()) {
			if (Mouse::bRight_Mouse_Pressed) {
				auto units = zone.view<Component::Input, Component::Position, Component::Radius>();

				for (auto unit : units) {					
					auto& radius = units.get<Component::Radius>(unit).fRadius;
					auto& x = units.get<Component::Position>(unit).x;
					auto& y = units.get<Component::Position>(unit).y;
					
					if (World::zone.any_of<Component::Attacking>(unit) == false) {
						//attack target if it is next to you
						if (In_Range(zone, radius, x, y)) { //check if center of attack rect is in the target
 							Melee_Attack(zone, unit, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
							return;
						}
						//else move to cursor
						else {
							Move_Order(zone, unit, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
						}
					}
					
				}
			}
		}
	}


	void Attack_Move(entt::registry &zone, entt::entity &entity, float &radius, Position &unitPosition, Position &targetPosition) { // maybe change to move and attack?
		if (World::zone.any_of<Attacking>(entity) == true) {
			return;
		}
		if (World::zone.any_of<Component::Attacking>(entity) == false) {
			if (Player_In_Melee_Range(zone, radius, unitPosition.x, unitPosition.y)) { //check if center of attack rect is in the target
				Melee_Attack(zone, entity, targetPosition.x, targetPosition.y);
				return;
			}
			else {
				Move_Order(zone, entity, targetPosition.x, targetPosition.y);
			}
		}
			//else move to cursor
	}
		
	


	void Check_For_Targets(entt::registry& zone) {
		auto units = zone.view<Component::Sight_Range, Component::Radius, Component::Position, Component::Alive>();
		auto targets = zone.view<Component::Position, Component::Input>();

		for (auto unit : units) {
			auto sightBox = units.get<Component::Sight_Range>(unit).sightBox;
			auto& radius = units.get<Component::Radius>(unit).fRadius;
			auto& position = units.get<Component::Position>(unit);

			for (auto target : targets) {
				auto& targetPosition = targets.get<Component::Position>(target);
				
				SDL_FPoint targetPoint = { targetPosition.x, targetPosition.y };
				if (Utilities::bFPoint_FRectIntersect(targetPoint, sightBox)) {
					Attack_Move(zone, unit, radius, position, targetPosition);
					//Spell_Attack(zone, unit, targetPoint.x, targetPoint.y, "'fireball'");					
					
				}
			}
		}
	}

	void Update_Sight_Box(entt::registry &zone) {
		auto view = zone.view<Component::Sight_Range, Component::Position, Component::Alive>();
		for (auto entity : view) {
			auto& sight = view.get<Component::Sight_Range>(entity).sightBox;
			auto& x = view.get<Component::Position>(entity).x;
			auto& y = view.get<Component::Position>(entity).y;

			sight = { x - 250.0f, y - 250.0f, 500.0f, 500.0f };
		}
	}

	int64_t time = 0;
	bool on = false;

	void Turn_On() {
		if (on == false) {
			on = true;
		}
		else {
			on = false;
		}
	}

	void Update_AI(entt::registry& zone) {
		time += Timer::timeStep;
		//std::cout << time << std::endl;
		if (time >= 1000) {
			if (on) {
				time = 0;
				Update_Sight_Box(zone);
				Check_For_Targets(zone);
			}
		}

		Mouse_Attack_Move(zone);
	}
}