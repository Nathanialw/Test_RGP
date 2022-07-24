#pragma once
#include "scene.h"
#include "mouse_control.h"
#include "timer.h"




namespace AI {
	
	//check for targets periodically

	//use the target x,y to moiv towards it

	//if target is in range, melee attack


	void Move_Order(entt::entity& entity, float& x, float& y) {
		World::zone.emplace_or_replace<Mouse_Move>(entity, x, y);
		World::zone.emplace_or_replace<Moving>(entity);
	}

	void Spell_Attack(entt::entity& entity, float& targetX, float& targetY, const char* name) {

		if (World::zone.any_of<Casting>(entity) == false) { //locks out casing until cast animation has finished
			World::zone.emplace_or_replace<Cast>(entity, targetX, targetY);
			World::zone.emplace_or_replace<Spell_Name>(entity, name);
		}
	}

	void Melee_Attack(entt::entity& entity, float& x, float& y) {		
		if (World::zone.any_of<Attacking>(entity) == false) { //locks out attacking until attack animation has finished
			World::zone.emplace_or_replace<Attack>(entity, x, y);
		}
	}

	bool In_Range(float &radius, float &x, float &y) {		
		SDL_FRect unit_collide_rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);

		auto company_view = World::zone.view<Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			if (Utilities::bFRect_Intersect(company.sCollide_Box, unit_collide_rect)) {
				for (int c = 0; c < company.iSub_Units.size(); c++) {
					auto& platoon = World::zone.get<Platoon>(company.iSub_Units[c]);
					if (Utilities::bFRect_Intersect(platoon.sCollide_Box, unit_collide_rect)) {
						for (int p = 0; p < platoon.iSub_Units.size(); p++) {
							auto& squad = World::zone.get<Squad>(platoon.iSub_Units[p]);
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

	bool Player_In_Melee_Range(float& radius, float &x, float &y) {
		
		SDL_FRect unit_collide_rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);
		
		auto units = World::zone.view<Input, Position, Radius>();		
		for (auto unit : units) {
			float tarx = units.get<Position>(unit).fX;
			float tary = units.get<Position>(unit).fY;
			float tarradius = units.get<Radius>(unit).fRadius;
			
			SDL_FRect target_collide_rect = Utilities::Get_FRect_From_Point_Radius(tarradius, tarx, tary);
			
			if (Utilities::bFRect_Intersect(unit_collide_rect, target_collide_rect)) {
				return true;
			}

		}
		return false;
	}

	void Mouse_Attack_Move() { // maybe change to move and attack?
		if (World::zone.empty<Selected>()) {
			if (Mouse::bRight_Mouse_Pressed) {
				auto units = World::zone.view<Input, Position, Radius>();

				for (auto unit : units) {					
					auto& radius = units.get<Radius>(unit).fRadius;
					auto& x = units.get<Position>(unit).fX;
					auto& y = units.get<Position>(unit).fY;
					
					if (World::zone.any_of<Attacking>(unit) == false) {
						//attack target if it is next to you
						if (In_Range(radius, x, y)) { //check if center of attack rect is in the target
 							Melee_Attack(unit, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
							return;
						}
						//else move to cursor
						else {
							Move_Order(unit, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
						}
					}
					
				}
			}
		}
	}


	void Attack_Move(entt::entity &entity, float &radius, Position &unitPosition, Position &targetPosition) { // maybe change to move and attack?
		if (World::zone.any_of<Attacking>(entity) == true) {
			return;
		}
		if (World::zone.any_of<Attacking>(entity) == false) {
			if (Player_In_Melee_Range(radius, unitPosition.fX, unitPosition.fY)) { //check if center of attack rect is in the target
				Melee_Attack(entity, targetPosition.fX, targetPosition.fY);
				return;
			}
			else {
				Move_Order(entity, targetPosition.fX, targetPosition.fY);
			}
		}
			//else move to cursor
	}
		
	


	void Check_For_Targets() {
		auto units = World::zone.view<Sight_Range, Radius, Position, Alive>();
		auto targets = World::zone.view<Position, Input>();

		for (auto unit : units) {
			auto sightBox = units.get<Sight_Range>(unit).sightBox;
			auto& radius = units.get<Radius>(unit).fRadius;
			auto& position = units.get<Position>(unit);

			for (auto target : targets) {
				auto& targetPosition = targets.get<Position>(target);
				
				SDL_FPoint targetPoint = { targetPosition.fX, targetPosition.fY };
				if (Utilities::bFPoint_FRectIntersect(targetPoint, sightBox)) {
					Attack_Move(unit, radius, position, targetPosition);
					//Spell_Attack(unit, tarx, tary, "'fireball'");					
					
				}
			}
		}
	}

	void Update_Sight_Box() {
		auto view = World::zone.view<Sight_Range, Position, Alive>();
		for (auto entity : view) {
			auto& sight = view.get<Sight_Range>(entity).sightBox;
			auto& x = view.get<Position>(entity).fX;
			auto& y = view.get<Position>(entity).fY;

			sight = { x - 250.0f, y - 250.0f, 500.0f, 500.0f };
		}
	}

	int64_t time = 0.0f;
	bool on = false;

	void Turn_On() {
		if (on == false) {
			on = true;
		}
		else {
			on = false;
		}
	}

	void Run_AI() {
		time += Timer::timeStep;
		//std::cout << time << std::endl;
		if (time >= 1000) {
			if (on) {
				time = 0;
				Update_Sight_Box();
				Check_For_Targets();
			}
		}

		Mouse_Attack_Move();
	}
}