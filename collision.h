#pragma once
#include "components.h"
#include "scene.h"
#include "timer.h"
#include <vector>


namespace collision {

	int iActual_Resolves = 0;
	int iNumber = 0;
	int iSquad_Check = 0;
	int iPlatoon_Check = 0;
	int iCompany_Check = 0;
	int iunitGridCollisionCheck = 0;
	int stat = 0;


	Timer::Frame_Timer Collision_Calc_Rate(16.0f); // collision update rate
	Timer::Frame_Timer Print_calcs(500.0f);

	void resolveCollisons() {	
		auto view = Scenes::scene.view<Position, Potential_Position>();
		for (auto entity : view) {
			auto& x = view.get<Position>(entity);
			auto& y = view.get<Position>(entity);
			auto& px = view.get<Potential_Position>(entity);
			auto& py = view.get<Potential_Position>(entity);
			x.fX = px.fPX;
			y.fY = py.fPY;		
		}
	}

	void Update_Unit() { // updates positions of soldiers stored in units for collision box
		auto view = Scenes::scene.view<Potential_Position, Assigned, Soldier>();
		for (auto entity : view) {
			auto& x = view.get<Potential_Position>(entity);
			auto& y = view.get<Potential_Position>(entity);
			auto& soldier = view.get<Assigned>(entity);
			auto& squad = Scenes::scene.get<Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			squad.fPX.at(soldier.iIndex) = x.fPX;
			squad.fPY.at(soldier.iIndex) = y.fPY;
		}
	}


	void Update_Company() { // updates positions of soldiers stored in units for collision box
		auto company_view = Scenes::scene.view<Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			for (int i = 0; i < company.iSub_Units.size(); i++) {
				auto& platoon = Scenes::scene.get<Platoon>(company.iSub_Units[i]); // gets the squad that the soldier is attached to
				company.fPX[i] = platoon.sCollide_Box.x;
				company.fPY[i] = platoon.sCollide_Box.y;
				company.fPW[i] = platoon.sCollide_Box.x + platoon.sCollide_Box.w;
				company.fPH[i] = platoon.sCollide_Box.y + platoon.sCollide_Box.h;
			}
		}
	}



	void Update_Platoons() { // updates positions of soldiers stored in units for collision box
		auto view = Scenes::scene.view<Platoon>();
		for (auto platoons : view) {
			auto& platoon = view.get<Platoon>(platoons);
			for (int i = 0; i < platoon.iSub_Units.size(); i++) {
				auto& squad = Scenes::scene.get<Squad>(platoon.iSub_Units[i]); // gets the squad that the soldier is attached to
				platoon.fPX[i] = squad.sCollide_Box.x;
				platoon.fPY[i] = squad.sCollide_Box.y;
				platoon.fPW[i] = squad.sCollide_Box.x + squad.sCollide_Box.w;
				platoon.fPH[i] = squad.sCollide_Box.y + squad.sCollide_Box.h;
			}
		}
	}

	void Update_Collided_Unit() {
		auto view = Scenes::scene.view<Potential_Position, Assigned, Soldier>();
		for (auto entity : view) {
			auto& x = view.get<Potential_Position>(entity);
			auto& y = view.get<Potential_Position>(entity);
			//auto& z = view.get<Collision_Radius>(entity);
			auto& soldier = view.get<Assigned>(entity);
			auto& squad = Scenes::scene.get<Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			x.fPX = squad.fPX.at(soldier.iIndex);
			y.fPY = squad.fPY.at(soldier.iIndex);		

		}
	}
	
	void Update_Health() {
			//reduces health if Struck

		//auto view = Scenes::scene.view<Assigned, Soldier, Health, Alive>();
		//for (auto entity : view) {
		//	auto& soldier = view.get<Assigned>(entity);
		//	auto& squad = Scenes::scene.get<Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to

		//	if (squad.iStruck.at(soldier.iIndex) > 0) {
		//		auto& h = view.get<Health>(entity).iHealth;
		//		auto& a = view.get<Alive>(entity).bIsAlive;
		//		h -= squad.iStruck.at(soldier.iIndex);
		//		squad.iStruck.at(soldier.iIndex) = 0;

		//		if (h <= 0) {
		//			squad.bAlive.at(soldier.iIndex) = false;
		//			a = false;
		//		}
		//	}
		//}
		
		auto view2 = Scenes::scene.view<Health, Struck>();
		
		for (auto entity : view2) {
			auto& struck = view2.get<Struck>(entity).struck;
			auto& health = view2.get<Health>(entity).iHealth;
			std::cout << "health = " << health << std::endl;
			health -= struck;
			Scenes::scene.remove<Struck>(entity);

			//if the soldier is in the assignment vector it will be set as dead if it dies
			if (health == 0) {
				if (Scenes::scene.any_of<Assigned>(entity)) {
					auto& assignment = Scenes::scene.get<Assigned>(entity);
					auto& squad = Scenes::scene.get<Squad>(assignment.iUnit_Assigned_To);
					squad.bAlive.at(assignment.iIndex) = false;
				}
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////////	
	void Update_Squad_Box() {
		auto view = Scenes::scene.view<Squad>();
		for (auto squads : view) { //update colliders
			auto& squad1 = view.get<Squad>(squads);
			if (!squad1.iSub_Units.empty()) { //will crash if vector is empty
				float xMax = *std::max_element(squad1.fPX.begin(), squad1.fPX.end());
				float xMin = *std::min_element(squad1.fPX.begin(), squad1.fPX.end());
				float yMax = *std::max_element(squad1.fPY.begin(), squad1.fPY.end());
				float yMin = *std::min_element(squad1.fPY.begin(), squad1.fPY.end());
				float rMax = *std::max_element(squad1.fRadius.begin(), squad1.fRadius.end());
				squad1.sCollide_Box = { xMin - rMax, yMin - rMax, (xMax - xMin) + (rMax * 2), (yMax - yMin) + (rMax * 2) };
			}
		}
	}

	void Update_Platoon_Box() {
		auto Platoon_View = Scenes::scene.view<Platoon>();
		for (auto platoons : Platoon_View) {
			auto& platoon1 = Platoon_View.get<Platoon>(platoons);
			if (!platoon1.iSub_Units.empty()) { //will crash if vector is empty {
				float xMin = *std::min_element(platoon1.fPX.begin(), platoon1.fPX.end());
				float yMin = *std::min_element(platoon1.fPY.begin(), platoon1.fPY.end());
				float wMax = *std::max_element(platoon1.fPW.begin(), platoon1.fPW.end());
				float hMax = *std::max_element(platoon1.fPH.begin(), platoon1.fPH.end());
				platoon1.sCollide_Box = { xMin, yMin, wMax - xMin, hMax - yMin };
			}
		}
	}
	
	void Update_Company_Box() {
		auto company_view = Scenes::scene.view<Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			if (!company.iSub_Units.empty()) { //will crash if vector is empty {
				float xMin = *std::min_element(company.fPX.begin(), company.fPX.end());
				float yMin = *std::min_element(company.fPY.begin(), company.fPY.end());
				float wMax = *std::max_element(company.fPW.begin(), company.fPW.end());
				float hMax = *std::max_element(company.fPH.begin(), company.fPH.end());
				company.sCollide_Box = { xMin, yMin, wMax - xMin, hMax - yMin };
			}
		}
	}

	void Check_For_Unit_Collision(entt::entity& entity, SDL_FRect attackRect, float& xUnit, float& yUnit, float& xTarget, float& yTarget, float& radiusUnit, float& radiusTarget, float& massUnit, float& massTarget, bool& aliveUnit, bool aliveTarget) {
		if (aliveTarget != false) {
			float fx = xTarget - xUnit;
			float fy = yTarget - yUnit;
			float fDistance = (fx * fx) + (fy * fy);
			// if the distance is zero it means they are the same unit or directly on top of each other
			if (fDistance != 0.0f) {
				SDL_FPoint target = { xTarget, yTarget };
				if (Utilities::bPoint_RectIntersect(target, attackRect)) {
					//if (fDistance <= ((radiusTarget + radiusUnit) * (radiusTarget + radiusUnit)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
					fDistance = sqrtf(fDistance);
					float fOverlap = fDistance - (radiusTarget + radiusUnit);
					DataTypes::f2d resolver = {};
					resolver.fX = fOverlap * (xUnit - xTarget) / fDistance;
					resolver.fY = fOverlap * (yUnit - yTarget) / fDistance;
					float fTotalmass = massTarget + massUnit;
					float fNomalizedMassA = (massTarget / massUnit);
					float fNomalizedMassB = (massUnit / fTotalmass);
					xTarget += (resolver.fX * fNomalizedMassB); // * normalized mass
					xUnit -= (resolver.fX * fNomalizedMassA);
					yTarget += (resolver.fY * fNomalizedMassB);
					yUnit -= (resolver.fY * fNomalizedMassA);
					aliveUnit = false;

					if (Scenes::scene.any_of<Struck>(entity)) {
						auto& struck = Scenes::scene.get<Struck>(entity).struck;
						struck++;
					}
					else {
						Scenes::scene.emplace<Struck>(entity, 1);
					}
					std::cout << "Hit!!!" << std::endl;
				}
			}
		}
	}

	void player_unit_Collision() { //seems to work and pushes the units
		if (1) {
			auto spells = Scenes::scene.view<Camera, Radius, Potential_Position, Mass>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& x = spells.get<Potential_Position>(spell);
				auto& y = spells.get<Potential_Position>(spell);
				auto& mass = spells.get<Mass>(spell);
				SDL_FRect spell_collider = { x.fPX - radius.fRadius, y.fPY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };

				auto company_view = Scenes::scene.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bFRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = Scenes::scene.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bFRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
									auto& squad = Scenes::scene.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bFRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											if (squad.bAlive.at(i) != false) {
												float fx = squad.fPX.at(i) - x.fPX;
												float fy = squad.fPY.at(i) - y.fPY;
												float fDistance = (fx * fx) + (fy * fy);
												if (fDistance <= ((squad.fRadius.at(i) + radius.fRadius) * (squad.fRadius.at(i) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
													fDistance = sqrtf(fDistance);
													float fOverlap = fDistance - (squad.fRadius.at(i) + radius.fRadius);
													DataTypes::f2d resolver = {};
													resolver.fX = fOverlap * (x.fPX - squad.fPX.at(i)) / fDistance;
													resolver.fY = fOverlap * (y.fPY - squad.fPY.at(i)) / fDistance;
													float fTotalmass = squad.fMass.at(i) + mass.fKilos;
													float fNomalizedMassA = (squad.fMass.at(i) / fTotalmass);
													float fNomalizedMassB = (mass.fKilos / fTotalmass);
													squad.fPX.at(i) += (resolver.fX * fNomalizedMassB); // * normalized mass
													x.fPX -= (resolver.fX * fNomalizedMassA);
													squad.fPY.at(i) += (resolver.fY * fNomalizedMassB);
													y.fPY -= (resolver.fY * fNomalizedMassA);
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

	void Unit_unit_Collision() {
		if (1) {
			auto company_view = Scenes::scene.view<Company>();
			for (auto companies : company_view) {
				auto& company1 = company_view.get<Company>(companies);
				for (auto companies : company_view) { 
					auto& company2 = company_view.get<Company>(companies);
					iCompany_Check++;
					if (Utilities::bFRect_Intersect(company1.sCollide_Box, company2.sCollide_Box)) {
						for (int c1 = 0; c1 < company1.iSub_Units.size(); c1++) {
							for (int c2 = 0; c2 < company2.iSub_Units.size(); c2++) {
								auto& platoon1 = Scenes::scene.get<Platoon>(company1.iSub_Units[c1]);
								auto& platoon2 = Scenes::scene.get<Platoon>(company2.iSub_Units[c2]);
								iPlatoon_Check++;
								if (Utilities::bFRect_Intersect(platoon1.sCollide_Box, platoon2.sCollide_Box)) {
									for (int s1 = 0; s1 < platoon1.iSub_Units.size(); s1++) {
										for (int s2 = 0; s2 < platoon2.iSub_Units.size(); s2++) {
											auto& squad1 = Scenes::scene.get<Squad>(platoon1.iSub_Units[s1]);
											auto& squad2 = Scenes::scene.get<Squad>(platoon2.iSub_Units[s2]);
											iSquad_Check++;							
											if (Utilities::bFRect_Intersect(squad1.sCollide_Box, squad2.sCollide_Box)) { //checks against itself too so that units with the squad will have collision
												for (int i = 0; i < squad1.iSub_Units.size(); i++) {
													for (int j = 0; j < squad2.iSub_Units.size(); j++) {
														if (squad1.bAlive.at(i) == true && squad2.bAlive.at(j) == true) {
															if (squad1.iSub_Units[i] != squad2.iSub_Units[j]) {
																float fx = squad1.fPX.at(i) - squad2.fPX.at(j);
																float fy = squad1.fPY.at(i) - squad2.fPY.at(j);
																float fDistance = (fx * fx) + (fy * fy);
																iNumber++;
																if (fDistance <= ((squad1.fRadius.at(i) + squad2.fRadius.at(j)) * (squad1.fRadius.at(i) + squad2.fRadius.at(j))) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
																	fDistance = sqrtf(fDistance);
																	float fOverlap = fDistance - (squad1.fRadius.at(i) + squad2.fRadius.at(j));
																	DataTypes::f2d resolver = {};
																	resolver.fX = fOverlap * (squad2.fPX.at(j) - squad1.fPX.at(i)) / fDistance;
																	resolver.fY = fOverlap * (squad2.fPY.at(j) - squad1.fPY.at(i)) / fDistance;
																	float fTotalmass = squad1.fMass.at(i) + squad2.fMass.at(j);
																	float fNomalizedMassA = (squad1.fMass.at(i) / fTotalmass);
																	float fNomalizedMassB = (squad2.fMass.at(j) / fTotalmass);
																	squad1.fPX.at(i) += (resolver.fX * fNomalizedMassB); // * normalized mass
																	squad2.fPX.at(j) -= (resolver.fX * fNomalizedMassA);
																	squad1.fPY.at(i) += (resolver.fY * fNomalizedMassB);
																	squad2.fPY.at(j) -= (resolver.fY * fNomalizedMassA);
																	iActual_Resolves;
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
				}				
			}
		}
	}


	//Use this for melee weapon collision as well
	void Spell_unit_Collision() { //seems to work and pushes the units
		if (1) {
			auto spells = Scenes::scene.view<Spell, Radius, Potential_Position, Mass, Alive>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell).fRadius;
				auto& x = spells.get<Potential_Position>(spell).fPX;
				auto& y = spells.get<Potential_Position>(spell).fPY;
				auto& mass = spells.get<Mass>(spell).fKilos;
				auto& alive = spells.get<Alive>(spell).bIsAlive;
				SDL_FRect spell_collider = Utilities::Get_FRect_From_Point_Radius(radius, x, y);

				auto company_view = Scenes::scene.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bFRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = Scenes::scene.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bFRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
								auto& squad = Scenes::scene.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bFRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											Check_For_Unit_Collision(squad.iSub_Units.at(i), spell_collider, x, y, squad.fPX.at(i), squad.fPY.at(i), radius, squad.fRadius.at(i), mass, squad.fMass.at(i), alive, squad.bAlive.at(i));
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

	void Spell_Player_Collision() {
		auto spells = Scenes::scene.view<Radius, Potential_Position, Mass, Spell, Alive>();
		auto players = Scenes::scene.view<Radius, Potential_Position, Mass, Input, Health>();

		for (auto spell : spells) {
			auto& spellRadius = spells.get<Radius>(spell);
			auto& spellAlive = spells.get<Alive>(spell);
			auto& spellXPos = spells.get<Potential_Position>(spell);
			auto& spellYPos = spells.get<Potential_Position>(spell);
			auto& spellMass = spells.get<Mass>(spell);
			for (auto player : players) {
				auto& playerRadius = players.get<Radius>(player);
				auto& playerXPos = players.get<Potential_Position>(player);
				auto& playerYPos = players.get<Potential_Position>(player);
				auto& playerMass = players.get<Mass>(player);
				auto& playerHealth = players.get<Health>(player);

				float fx = spellXPos.fPX - playerXPos.fPX;
				float fy = spellYPos.fPY - playerYPos.fPY;
				float fDistance = (fx * fx) + (fy * fy);
				if (fDistance <= ((spellRadius.fRadius + playerRadius.fRadius) * (spellRadius.fRadius + playerRadius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
					fDistance = sqrtf(fDistance);
					float fOverlap = fDistance - (spellRadius.fRadius + playerRadius.fRadius);
					DataTypes::f2d resolver = {};
					resolver.fX = fOverlap * (playerXPos.fPX - spellXPos.fPX) / fDistance;
					resolver.fY = fOverlap * (playerYPos.fPY - spellYPos.fPY) / fDistance;
					float fTotalmass = spellMass.fKilos + playerMass.fKilos;
					float fNomalizedMassA = (spellMass.fKilos / fTotalmass);
					float fNomalizedMassB = (playerMass.fKilos / fTotalmass);
					spellXPos.fPX += (resolver.fX * fNomalizedMassB); // * normalized mass
					playerXPos.fPX -= (resolver.fX * fNomalizedMassA);
					spellYPos.fPY += (resolver.fY * fNomalizedMassB);
					playerYPos.fPY -= (resolver.fY * fNomalizedMassA);
					//std::cout << "Hit!!!" << std::endl; 
					//trigger hit
					// -1 HP
					playerHealth.iHealth -= 1;
					//destroy spell
					spellAlive.bIsAlive = false;
				}
			}
		}
	}



	void Melee_Unit_Player_Collision() {
		auto spells = Scenes::scene.view<Radius, Potential_Position, Weapon_Size, Mass, Melee, Alive>();
		auto players = Scenes::scene.view<Radius, Potential_Position, Mass, Input, Alive>();

		for (auto spell : spells) {
			auto& attackRadius = spells.get<Radius>(spell).fRadius;
			auto& attackArea = spells.get<Weapon_Size>(spell).attackArea;
			auto& attackXPos = spells.get<Potential_Position>(spell).fPX;
			auto& attackYPos = spells.get<Potential_Position>(spell).fPY;
			auto& attackMass = spells.get<Mass>(spell).fKilos;
			auto& attackAlive = spells.get<Alive>(spell).bIsAlive;
			for (auto player : players) {
				auto& playerRadius = players.get<Radius>(player).fRadius;
				auto& playerXPos = players.get<Potential_Position>(player).fPX;
				auto& playerYPos = players.get<Potential_Position>(player).fPY;
				auto& playerMass = players.get<Mass>(player).fKilos;
				auto& playerAlive = players.get<Alive>(player).bIsAlive;

				Check_For_Unit_Collision(player, attackArea, attackXPos, attackYPos, playerXPos, playerYPos, attackRadius, playerRadius, attackMass, playerMass, attackAlive, playerAlive);
			}
		}
	}




	//change to use weapon size instead of weapon radius
	void MeleeAttack_unit_Collision() { //seems to work 
		if (1) {
			auto spells = Scenes::scene.view<Melee, Weapon_Size, Radius, Potential_Position, Mass, Alive>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell).fRadius;
				auto& attackArea = spells.get<Weapon_Size>(spell).attackArea;
				auto& x = spells.get<Potential_Position>(spell).fPX;
				auto& y = spells.get<Potential_Position>(spell).fPY;
				auto& mass = spells.get<Mass>(spell).fKilos;
				auto& alive = spells.get<Alive>(spell).bIsAlive;
				SDL_FRect spell_collider = Utilities::Get_FRect_From_Point_Radius(radius, x, y);

				auto company_view = Scenes::scene.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bFRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = Scenes::scene.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bFRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
									auto& squad = Scenes::scene.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bFRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											Check_For_Unit_Collision(squad.iSub_Units.at(i), attackArea, x, y, squad.fPX.at(i), squad.fPY.at(i), radius, squad.fRadius.at(i), mass, squad.fMass.at(i), alive, squad.bAlive.at(i));
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



	std::vector<std::vector<entt::entity>> grid_collision(SDL_FRect &unit_box, Map::Node3 &map) {
		int p = 0;
		std::vector<std::vector<entt::entity>>vec;
	//	std::cout << map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.x << "--------" << map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.y << std::endl;

		//	SDL_FRect port;
		if (Utilities::bFRect_Intersect(unit_box, map.sCollide_Box)) {
			for (int i = 0; i < Map::size; i++) {
				if (Utilities::bFRect_Intersect(unit_box, map.nodes[i].sCollide_Box)) {
					for (int j = 0; j < Map::size; j++) {
						if (Utilities::bFRect_Intersect(unit_box, map.nodes[i].nodes[j].sCollide_Box)) {
							for (int k = 0; k < Map::size; k++) {
								if (Utilities::bFRect_Intersect(unit_box, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
									for (int l = 0; l < Map::size; l++) {
										if (Utilities::bFRect_Intersect(unit_box, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
											//port.x = map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.x - offset.screen.x;
											//port.y = map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.y - offset.screen.y;
											//port.w = map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.w;
											//port.h = map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.h;
											//SDL_RenderDrawRectF(Graphics::renderer, &port);
												
											vec.push_back(map.nodes[i].nodes[j].nodes[k].cells[l].entities);
																								
										}
										p = i + j + k + l;
									}
								}
							}
						}
					}
				}
			}
			
		}
		stat += p;
		//std::cout << p << std::endl;
		return vec;
	}	



	void player_grid_collision(Map::Node3& map) {
		int p = 0;
		if (1) { // for the player
			auto spells = Scenes::scene.view<Radius, Potential_Position, Mass, Input>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& x = spells.get<Potential_Position>(spell);
				auto& y = spells.get<Potential_Position>(spell);
				auto& mass = spells.get<Mass>(spell);
				SDL_FRect unit_collider = { x.fPX - radius.fRadius, y.fPY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };

				std::vector<std::vector<entt::entity>> cell = grid_collision(unit_collider, map);

				for (int j = 0; j < cell.size(); j++) {
					for (int i = 0; i < cell[j].size(); i++) {
						if (spell != cell[j].at(i)) {
							p++;
							//if (scene.all_of<Radius, Mass, Position_Y, Position_X>(cell[j].at(i))) {
								//std::cout << cell.size() << " || " << cell[j].size() << std::endl;
							auto& map_x = Scenes::scene.get<Potential_Position>(cell[j].at(i));
							auto& map_y = Scenes::scene.get<Potential_Position>(cell[j].at(i));
							auto& map_mass = Scenes::scene.get<Mass>(cell[j].at(i));
							auto& map_radius = Scenes::scene.get<Radius>(cell[j].at(i));
							float fx = map_x.fPX - x.fPX;
							float fy = map_y.fPY - y.fPY;
							float fDistance = (fx * fx) + (fy * fy);
							if (fDistance <= ((map_radius.fRadius + radius.fRadius) * (map_radius.fRadius + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
								fDistance = sqrtf(fDistance);
								float fOverlap = fDistance - (map_radius.fRadius + radius.fRadius);
								DataTypes::f2d resolver = {};
								resolver.fX = fOverlap * (x.fPX - map_x.fPX) / fDistance;
								resolver.fY = fOverlap * (y.fPY - map_y.fPY) / fDistance;
								float fTotalmass = map_mass.fKilos + mass.fKilos;
								float fNomalizedMassA = (map_mass.fKilos / fTotalmass);
								float fNomalizedMassB = (mass.fKilos / fTotalmass);
								map_x.fPX += (resolver.fX * fNomalizedMassB); // * normalized mass
								x.fPX -= (resolver.fX * fNomalizedMassA);
								map_y.fPY += (resolver.fY * fNomalizedMassB);
								y.fPY -= (resolver.fY * fNomalizedMassA);
							}
							//	}
						}
					}
				}
			}
		}
		//std::cout << stat << std::endl;
		stat = 0;
	}


	
	void spell_grid_collision(Map::Node3& map) {
		int p = 0;
		if (1) { // for the player
			auto spells = Scenes::scene.view<Radius, Potential_Position, Mass, Spell, Alive>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& a = spells.get<Alive>(spell);
				auto& x = spells.get<Potential_Position>(spell);
				auto& y = spells.get<Potential_Position>(spell);
				auto& mass = spells.get<Mass>(spell);
				SDL_FRect unit_collider = { x.fPX - radius.fRadius, y.fPY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };
			
				std::vector<std::vector<entt::entity>> cell = grid_collision(unit_collider, map);				

				for (int j = 0; j < cell.size(); j++) {
					for (int i = 0; i < cell[j].size(); i++) {
						if (spell != cell[j].at(i)) {
							p++;
							//if (scene.all_of<Radius, Mass, Position_Y, Position_X>(cell[j].at(i))) {
								//std::cout << cell.size() << " || " << cell[j].size() << std::endl;
								auto& map_x = Scenes::scene.get<Potential_Position>(cell[j].at(i));
								auto& map_y = Scenes::scene.get<Potential_Position>(cell[j].at(i));
								auto& map_mass = Scenes::scene.get<Mass>(cell[j].at(i));
								auto& map_radius = Scenes::scene.get<Radius>(cell[j].at(i));
								float fx = map_x.fPX - x.fPX;
								float fy = map_y.fPY - y.fPY;
								float fDistance = (fx * fx) + (fy * fy);
								if (fDistance <= ((map_radius.fRadius + radius.fRadius) * (map_radius.fRadius + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
									fDistance = sqrtf(fDistance);
									float fOverlap = fDistance - (map_radius.fRadius + radius.fRadius);
									DataTypes::f2d resolver = {};
									resolver.fX = fOverlap * (x.fPX - map_x.fPX) / fDistance;
									resolver.fY = fOverlap * (y.fPY - map_y.fPY) / fDistance;
									float fTotalmass = map_mass.fKilos + mass.fKilos;
									float fNomalizedMassA = (map_mass.fKilos / fTotalmass);
									float fNomalizedMassB = (mass.fKilos / fTotalmass);
									map_x.fPX += (resolver.fX * fNomalizedMassB); // * normalized mass
									x.fPX -= (resolver.fX * fNomalizedMassA);
									map_y.fPY += (resolver.fY * fNomalizedMassB);
									y.fPY -= (resolver.fY * fNomalizedMassA);
									a.bIsAlive = false; //destroy spell on contact
								}
						//	}
						}
					}
				}
			}			
		}
		//std::cout << stat << std::endl;
		stat = 0;
	}



	void unit_grid_collision(Map::Node3& map) { //waaaaay too slow		

		auto company_view = Scenes::scene.view<Company>();
		
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			if (Utilities::bFRect_Intersect(company.sCollide_Box, map.sCollide_Box)) {
				for (int b = 0; b < company.iSub_Units.size(); b++) {
					for (int i = 0; i < Map::size; i++) {
						for (int j = 0; j < Map::size; j++) {

							if (Utilities::bFRect_Intersect(company.sCollide_Box, map.nodes[i].nodes[j].sCollide_Box)) {
								auto& platoon = Scenes::scene.get<Platoon>(company.iSub_Units[b]);
								for (int c = 0; c < platoon.iSub_Units.size(); c++) {
									for (int k = 0; k < Map::size; k++) {

										if (Utilities::bFRect_Intersect(platoon.sCollide_Box, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
											auto& squad = Scenes::scene.get<Squad>(platoon.iSub_Units[c]);
											for (int l = 0; l < Map::size; l++) {

												if (Utilities::bFRect_Intersect(squad.sCollide_Box, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
													for (int d = 0; d < squad.iSub_Units.size(); d++) {
														if (squad.bAlive.at(d) != false) {
															for (int m = 0; m < map.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); m++) {
																auto& radius = Scenes::scene.get<Radius>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& mass = Scenes::scene.get<Mass>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& x = Scenes::scene.get<Potential_Position>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& y = Scenes::scene.get<Potential_Position>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																for (int e = 0; e < squad.iSub_Units.size(); e++) {
																	float fx = squad.fPX.at(e) - x.fPX;
																	float fy = squad.fPY.at(e) - y.fPY;
																	float fDistance = (fx * fx) + (fy * fy);
																	if (fDistance <= ((squad.fRadius.at(e) + radius.fRadius) * (squad.fRadius.at(e) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
																		fDistance = sqrtf(fDistance);
																		float fOverlap = fDistance - (squad.fRadius.at(e) + radius.fRadius);
																		DataTypes::f2d resolver = {};
																		resolver.fX = fOverlap * (x.fPX - squad.fPX.at(e)) / fDistance;
																		resolver.fY = fOverlap * (y.fPY - squad.fPY.at(e)) / fDistance;
																		float fTotalmass = squad.fMass.at(e) + mass.fKilos;
																		float fNomalizedMassA = (squad.fMass.at(e) / fTotalmass);
																		float fNomalizedMassB = (mass.fKilos / fTotalmass);
																		squad.fPX.at(e) += (resolver.fX * fNomalizedMassB); // * normalized mass
																		x.fPX -= (resolver.fX * fNomalizedMassA);
																		squad.fPY.at(e) += (resolver.fY * fNomalizedMassB);
																		y.fPY -= (resolver.fY * fNomalizedMassA);
																		iunitGridCollisionCheck++;
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
					}
				}						
			}
		}
	}

	void CollisionsT() {
		if (Print_calcs.Calc()) {
			std::cout << "Company:      " << Scenes::scene.size<Company>() << "   checks:   " << iCompany_Check << std::endl;
			std::cout << "Platoon:      " << Scenes::scene.size<Platoon>() << "   checks:   " << iPlatoon_Check << std::endl;
			std::cout << "Squad:        " << Scenes::scene.size<Squad>()   << "   checks:   " << iSquad_Check << std::endl;
			std::cout << "Soldiers:     " << iNumber << std::endl;
			std::cout << "Resolves:     " << iActual_Resolves << std::endl;
			std::cout << "----------   " << iActual_Resolves << std::endl;
			std::cout << "iunitGridCollisionCheck:     " << iunitGridCollisionCheck << std::endl;
			std::cout << "----------   " << iActual_Resolves << std::endl;
		}
	}


	void Update_Unit_Boxes() {
		Update_Squad_Box();
		Update_Platoon_Box();
		Update_Company_Box();
	}

	void Collisions() {
		iActual_Resolves = 0;
		iNumber = 0;
		iSquad_Check = 0;
		iPlatoon_Check = 0;
		iCompany_Check = 0;


		Update_Unit();
		Update_Platoons();
		Update_Company();

		Update_Unit_Boxes();

		player_grid_collision(Map::map);
		spell_grid_collision(Map::map);
		unit_grid_collision(Map::map);

		player_unit_Collision();
		Unit_unit_Collision(); 
		
		Spell_Player_Collision();
		Spell_unit_Collision();		
		Melee_Unit_Player_Collision();
		MeleeAttack_unit_Collision();

		Update_Collided_Unit();
		resolveCollisons();
		Update_Health();

		//CollisionsT();
	}
}