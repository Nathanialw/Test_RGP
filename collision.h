#pragma once
#include "components.h"
#include "timer.h"
#include <vector>
#include "formation_positions.h"


namespace collision {

	int iActual_Resolves = 0;
	int iNumber = 0;
	int iSquad_Check = 0;
	int iPlatoon_Check = 0;
	int iCompany_Check = 0;
	int iunitGridCollisionCheck = 0;
	int stat = 0;

	using namespace Component;

	Timer::Frame_Timer Collision_Calc_Rate(16.0f); // collision update rate
	Timer::Frame_Timer Print_calcs(500.0f);

	void Add_Or_Update_Collidee(entt::registry& zone, entt::entity &entity, Position &position, float &potentialX, float &potentialY) {
		if (zone.any_of<Collided>(entity)) {
			auto& potentialPosition = zone.get<Collided>(entity);
			potentialPosition.x += potentialX; // * normalized mass
			potentialPosition.y += potentialY;
		}
		else {
 			auto& potentialPosition = zone.emplace<Collided>(entity);
			potentialPosition = { position.x += potentialX, position.y += potentialY };
		}
	}
	void Add_Or_Update_Collider(entt::registry& zone, entt::entity& entity, Position& position, float& potentialX, float& potentialY) {
		if (zone.any_of<Collided>(entity)) {
 			auto& potentialPosition = zone.get<Collided>(entity);
			potentialPosition.x -= potentialX; // * normalized mass
			potentialPosition.y -= potentialY;
		}
		else {
			auto& potentialPosition = zone.emplace<Collided>(entity);
			potentialPosition = { position.x -= potentialX, position.y -= potentialY };
		}
	}
	
	void Update_Collided_Unit(entt::registry &zone) {
		auto view = zone.view<Position, Collided, Entity_Type, Alive>();
		for (auto entity : view) {
			auto& unitType = view.get<Entity_Type>(entity);
			auto& x = view.get<Position>(entity);
			auto& y = view.get<Position>(entity);
			auto& px = view.get<Collided>(entity);
			auto& py = view.get<Collided>(entity);
			auto& alive = view.get<Alive>(entity);
			x.x = px.x;
			y.y = py.y;
			zone.remove<Collided>(entity);

			if (unitType == spell) {
				alive.bIsAlive = false; //destroy spell on contact
			}
		}
	}
	
	

////////////////////////////////////////////////////////////////////////////////////	


	void Check_For_Unit_Collision_Rect(entt::registry& zone, entt::entity& entity, SDL_FRect attackRect, float& xUnit, float& yUnit, float& xTarget, float& yTarget, float& radiusUnit, float& radiusTarget, float& massUnit, float& massTarget, bool& aliveUnit, bool aliveTarget) {
		if (aliveTarget != false) {
			float fx = xTarget - xUnit;
			float fy = yTarget - yUnit;
			float fDistance = (fx * fx) + (fy * fy);
			// if the distance is zero it means they are the same unit or directly on top of each other
			if (fDistance != 0.0f) {
				SDL_FPoint target = { xTarget, yTarget };
				if (Utilities::bFPoint_FRectIntersect(target, attackRect)) {
				//if (fDistance <= ((radiusTarget + radiusUnit) * (radiusTarget + radiusUnit)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
					//fDistance = sqrtf(fDistance);
					//float fOverlap = fDistance - (radiusTarget + radiusUnit);
					//DataTypes::f2d resolver = {};
					//resolver.fX = fOverlap * (xUnit - xTarget) / fDistance;
					//resolver.fY = fOverlap * (yUnit - yTarget) / fDistance;
					//float fTotalmass = massTarget + massUnit;
					//float fNomalizedMassA = (massTarget / massUnit);
					//float fNomalizedMassB = (massUnit / fTotalmass);
					//xTarget += (resolver.fX * fNomalizedMassB); // * normalized mass
					//xUnit -= (resolver.fX * fNomalizedMassA);
					//yTarget += (resolver.fY * fNomalizedMassB);
					//yUnit -= (resolver.fY * fNomalizedMassA);
					//aliveUnit = false;

					if (zone.any_of<Struck>(entity)) {
						auto& struck = zone.get<Struck>(entity).struck;
						struck++;						
					}
					else {
						zone.emplace<Struck>(entity, 1);
					}
				}
			}
		}
	}

	void Check_For_Unit_Collision_Circle(entt::registry& zone, entt::entity& entity_ID, entt::entity& target_ID, SDL_FRect attackRect, float& xUnit, float& yUnit, float& xTarget, float& yTarget, float& radiusUnit, float& radiusTarget, float& massUnit, float& massTarget) {
		
		float fx = xTarget - xUnit;
		float fy = yTarget - yUnit;
		float fDistance = (fx * fx) + (fy * fy);
		// if the distance is zero it means they are the same unit or directly on top of each other
		if (fDistance != 0.0f) {
			//SDL_FPoint target = { xTarget, yTarget };
			//if (Utilities::bPoint_RectIntersect(target, attackRect)) {
			if (fDistance <= ((radiusTarget + radiusUnit) * (radiusTarget + radiusUnit)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
				fDistance = sqrtf(fDistance);
				float fOverlap = fDistance - (radiusTarget + radiusUnit);
				DataTypes::f2d resolver = {};
				resolver.fX = fOverlap * (xUnit - xTarget) / fDistance;
				resolver.fY = fOverlap * (yUnit - yTarget) / fDistance;
				float fTotalmass = massTarget + massUnit;
				float fNomalizedMassA = (massTarget / massUnit);
				float fNomalizedMassB = (massUnit / fTotalmass);
					
					
					
				//xTarget += (resolver.fX * fNomalizedMassB); // * normalized mass
				//yTarget += (resolver.fY * fNomalizedMassB);
				//xUnit -= (resolver.fX * fNomalizedMassA);
				//yUnit -= (resolver.fY * fNomalizedMassA);
					
					
				//aliveUnit = false;

				Position position = { xUnit, yUnit };
				float resolverX = (resolver.fX * fNomalizedMassA);
				float resolverY = (resolver.fY * fNomalizedMassA);
				Add_Or_Update_Collider(zone, entity_ID, position, resolverX, resolverY);

				position = { xTarget, yTarget};
				resolverX = (resolver.fX * fNomalizedMassB); // * normalized mass
				resolverY = (resolver.fY * fNomalizedMassB);
				Add_Or_Update_Collidee(zone, target_ID, position, resolverX, resolverY);

				//PERHAPS emplace the ID of the unit that damaged and resolve the damage in another routine
				//if (zone.any_of<Struck>(target_ID)) {
				//	auto& struck = zone.get<Struck>(target_ID).struck;
				//	struck++;
				//}
				//else {
				//	zone.emplace<Struck>(target_ID, 1);
				//}
			}
		}
		
	}

	void Entity_vs_Formation_Collision(entt::registry& zone) { //seems to work and pushes the units
		if (1) {
			auto players_view = zone.view<Radius, Position, Mass>(entt::exclude<Assigned_To>);
			for (auto player : players_view) {
				auto& radius = players_view.get<Radius>(player);
				auto& position = players_view.get<Position>(player);
				auto& mass = players_view.get<Mass>(player);
				SDL_FRect spell_collider = { position.x - radius.fRadius, position.y - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };

				auto company_view = zone.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bFRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = zone.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bFRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
									auto& squad = zone.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bFRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											if (squad.bAlive.at(i) != false) {
												float fx = squad.fPX.at(i) - position.x;
												float fy = squad.fPY.at(i) - position.y;
												float fDistance = (fx * fx) + (fy * fy);
												if (fDistance <= ((squad.fRadius.at(i) + radius.fRadius) * (squad.fRadius.at(i) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
													fDistance = sqrtf(fDistance);
													float fOverlap = fDistance - (squad.fRadius.at(i) + radius.fRadius);
													DataTypes::f2d resolver = {};
													resolver.fX = fOverlap * (position.x - squad.fPX.at(i)) / fDistance;
													resolver.fY = fOverlap * (position.y - squad.fPY.at(i)) / fDistance;
													float fTotalmass = squad.fMass.at(i) + mass.fKilos;
													float fNomalizedMassA = (squad.fMass.at(i) / fTotalmass);
													float fNomalizedMassB = (mass.fKilos / fTotalmass);


													float resolverX = (resolver.fX * fNomalizedMassA);
													float resolverY = (resolver.fY * fNomalizedMassA);
													Add_Or_Update_Collider(zone, player, position, resolverX, resolverY);

													Position position = { squad.fPX.at(i), squad.fPY.at(i) };
													resolverX = (resolver.fX * fNomalizedMassB); // * normalized mass
													resolverY = (resolver.fY * fNomalizedMassB);
													Add_Or_Update_Collidee(zone, squad.iSub_Units.at(i), position, resolverX, resolverY);
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

	void Formation_vs_Formation_Collision(entt::registry& zone) {
		if (1) {
			auto company_view = zone.view<Company>();
			for (auto companies : company_view) {
				auto& company1 = company_view.get<Company>(companies);
				for (auto companies : company_view) { 
					auto& company2 = company_view.get<Company>(companies);
					iCompany_Check++;
					if (Utilities::bFRect_Intersect(company1.sCollide_Box, company2.sCollide_Box)) {
						for (int c1 = 0; c1 < company1.iSub_Units.size(); c1++) {
							for (int c2 = 0; c2 < company2.iSub_Units.size(); c2++) {
								auto& platoon1 = zone.get<Platoon>(company1.iSub_Units[c1]);
								auto& platoon2 = zone.get<Platoon>(company2.iSub_Units[c2]);
								iPlatoon_Check++;
								if (Utilities::bFRect_Intersect(platoon1.sCollide_Box, platoon2.sCollide_Box)) {
									for (int s1 = 0; s1 < platoon1.iSub_Units.size(); s1++) {
										for (int s2 = 0; s2 < platoon2.iSub_Units.size(); s2++) {
											auto& squad1 = zone.get<Squad>(platoon1.iSub_Units[s1]);
											auto& squad2 = zone.get<Squad>(platoon2.iSub_Units[s2]);
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
																//	squad1.fPX.at(i) += (resolver.fX * fNomalizedMassB); // * normalized mass
																//	squad1.fPY.at(i) += (resolver.fY * fNomalizedMassB);
																
																//	squad2.fPX.at(j) -= (resolver.fX * fNomalizedMassA);
																//	squad2.fPY.at(j) -= (resolver.fY * fNomalizedMassA);
																	iActual_Resolves;

																	Position position = { squad2.fPX.at(j), squad2.fPY.at(j) };
																	float resolverX = (resolver.fX * fNomalizedMassA);
																	float resolverY = (resolver.fY * fNomalizedMassA);
																	Add_Or_Update_Collider(zone, squad2.iSub_Units.at(j), position, resolverX, resolverY);

																	position = { squad1.fPX.at(i), squad1.fPY.at(i) };
																	resolverX = (resolver.fX * fNomalizedMassB); // * normalized mass
																	resolverY = (resolver.fY * fNomalizedMassB);
																	Add_Or_Update_Collidee(zone, squad1.iSub_Units.at(i), position, resolverX, resolverY);
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


	void Spell_Player_Collision(entt::registry& zone) {
		auto spells = zone.view<Radius, Potential_Position, Mass, Spell, Alive>();
		auto players = zone.view<Radius, Potential_Position, Mass, Input, Health>();

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

				float fx = spellXPos.x - playerXPos.x;
				float fy = spellYPos.y - playerYPos.y;
				float fDistance = (fx * fx) + (fy * fy);
				if (fDistance <= ((spellRadius.fRadius + playerRadius.fRadius) * (spellRadius.fRadius + playerRadius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
					fDistance = sqrtf(fDistance);
					float fOverlap = fDistance - (spellRadius.fRadius + playerRadius.fRadius);
					DataTypes::f2d resolver = {};
					resolver.fX = fOverlap * (playerXPos.x - spellXPos.x) / fDistance;
					resolver.fY = fOverlap * (playerYPos.y - spellYPos.y) / fDistance;
					float fTotalmass = spellMass.fKilos + playerMass.fKilos;
					float fNomalizedMassA = (spellMass.fKilos / fTotalmass);
					float fNomalizedMassB = (playerMass.fKilos / fTotalmass);
					spellXPos.x += (resolver.fX * fNomalizedMassB); // * normalized mass
					playerXPos.x -= (resolver.fX * fNomalizedMassA);
					spellYPos.y += (resolver.fY * fNomalizedMassB);
					playerYPos.y -= (resolver.fY * fNomalizedMassA);
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



	void Melee_Unit_Player_Collision(entt::registry& zone) {
		auto spells = zone.view<Radius, Potential_Position, Weapon_Size, Mass, Melee, Alive>();
		auto players = zone.view<Radius, Potential_Position, Mass, Input, Alive>();

		for (auto spell : spells) {
			auto& attackRadius = spells.get<Radius>(spell).fRadius;
			auto& attackArea = spells.get<Weapon_Size>(spell).attackArea;
			auto& attackXPos = spells.get<Potential_Position>(spell).x;
			auto& attackYPos = spells.get<Potential_Position>(spell).y;
			auto& attackMass = spells.get<Mass>(spell).fKilos;
			auto& attackAlive = spells.get<Alive>(spell).bIsAlive;
			for (auto player : players) {
				auto& playerRadius = players.get<Radius>(player).fRadius;
				auto& playerXPos = players.get<Potential_Position>(player).x;
				auto& playerYPos = players.get<Potential_Position>(player).y;
				auto& playerMass = players.get<Mass>(player).fKilos;
				auto& playerAlive = players.get<Alive>(player).bIsAlive;

				Check_For_Unit_Collision_Rect(zone, player, attackArea, attackXPos, attackYPos, playerXPos, playerYPos, attackRadius, playerRadius, attackMass, playerMass, attackAlive, playerAlive);
			}
		}
	}




	//change to use weapon size instead of weapon radius
	void MeleeAttack_unit_Collision(entt::registry& zone) { //seems to work 
		if (1) {
			auto spells = zone.view<Melee, Weapon_Size, Radius, Potential_Position, Mass, Alive>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell).fRadius;
				auto& attackArea = spells.get<Weapon_Size>(spell).attackArea;
				auto& x = spells.get<Potential_Position>(spell).x;
				auto& y = spells.get<Potential_Position>(spell).y;
				auto& mass = spells.get<Mass>(spell).fKilos;
				auto& alive = spells.get<Alive>(spell).bIsAlive;
				SDL_FRect spell_collider = Utilities::Get_FRect_From_Point_Radius(radius, x, y);

				auto company_view = zone.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bFRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = zone.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bFRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
									auto& squad = zone.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bFRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											Check_For_Unit_Collision_Rect(zone, squad.iSub_Units.at(i), attackArea, x, y, squad.fPX.at(i), squad.fPY.at(i), radius, squad.fRadius.at(i), mass, squad.fMass.at(i), alive, squad.bAlive.at(i));
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

	
	void Entity_vs_Grid_Collision(entt::registry& zone, Map::Node3& map) {
		int p = 0;
		if (1) { // for the player
			auto spells = zone.view<Radius, Position, Mass>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& position = spells.get<Position>(spell);
				auto& mass = spells.get<Mass>(spell);
				SDL_FRect unit_collider = { position.x - radius.fRadius, position.y - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };
			
				std::vector<std::vector<entt::entity>> cell = grid_collision(unit_collider, map);				

				for (int j = 0; j < cell.size(); j++) {
					for (int i = 0; i < cell[j].size(); i++) {
						if (spell != cell[j].at(i)) {
							p++;
							//if (scene.all_of<Radius, Mass, Position_Y, Position_X>(cell[j].at(i))) {
								//std::cout << cell.size() << " || " << cell[j].size() << std::endl;
							auto& map_x = zone.get<Position>(cell[j].at(i));
							auto& map_y = zone.get<Position>(cell[j].at(i));
							auto& map_mass = zone.get<Mass>(cell[j].at(i));
							auto& map_radius = zone.get<Radius>(cell[j].at(i));
							float fx = map_x.x - position.x;
							float fy = map_y.y - position.y;
							float fDistance = (fx * fx) + (fy * fy);
							if (fDistance <= ((map_radius.fRadius + radius.fRadius) * (map_radius.fRadius + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
								fDistance = sqrtf(fDistance);
								float fOverlap = fDistance - (map_radius.fRadius + radius.fRadius);
								DataTypes::f2d resolver = {};
								resolver.fX = fOverlap * (position.x - map_x.x) / fDistance;
								resolver.fY = fOverlap * (position.y - map_y.y) / fDistance;
								float fTotalmass = map_mass.fKilos + mass.fKilos;
								float fNomalizedMassA = (map_mass.fKilos / fTotalmass);
								float fNomalizedMassB = (mass.fKilos / fTotalmass);
								map_x.x += (resolver.fX * fNomalizedMassB); // * normalized mass
								map_y.y += (resolver.fY * fNomalizedMassB);

								float resolverX = (resolver.fX * fNomalizedMassA);
								float resolverY = (resolver.fY * fNomalizedMassA);

								Add_Or_Update_Collider(zone, spell, position, resolverX, resolverY);

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



	void Formation_vs_Grid_collision(entt::registry& zone, Map::Node3& map) { //waaaaay too slow		

		auto company_view = zone.view<Company>();
		
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			if (Utilities::bFRect_Intersect(company.sCollide_Box, map.sCollide_Box)) {
				for (int b = 0; b < company.iSub_Units.size(); b++) {
					for (int i = 0; i < Map::size; i++) {
						for (int j = 0; j < Map::size; j++) {

							if (Utilities::bFRect_Intersect(company.sCollide_Box, map.nodes[i].nodes[j].sCollide_Box)) {
								auto& platoon = zone.get<Platoon>(company.iSub_Units[b]);
								for (int c = 0; c < platoon.iSub_Units.size(); c++) {
									for (int k = 0; k < Map::size; k++) {

										if (Utilities::bFRect_Intersect(platoon.sCollide_Box, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
											auto& squad = zone.get<Squad>(platoon.iSub_Units[c]);
											for (int l = 0; l < Map::size; l++) {

												if (Utilities::bFRect_Intersect(squad.sCollide_Box, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
													for (int d = 0; d < squad.iSub_Units.size(); d++) {
														if (squad.bAlive.at(d) != false) {
															for (int m = 0; m < map.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); m++) {
																auto& radius = zone.get<Radius>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& mass = zone.get<Mass>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& x = zone.get<Position>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& y = zone.get<Position>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																for (int e = 0; e < squad.iSub_Units.size(); e++) {
																	float fx = squad.fPX.at(e) - x.x;
																	float fy = squad.fPY.at(e) - y.y;
																	float fDistance = (fx * fx) + (fy * fy);
																	if (fDistance <= ((squad.fRadius.at(e) + radius.fRadius) * (squad.fRadius.at(e) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
																		fDistance = sqrtf(fDistance);
																		float fOverlap = fDistance - (squad.fRadius.at(e) + radius.fRadius);
																		DataTypes::f2d resolver = {};
																		resolver.fX = fOverlap * (x.x - squad.fPX.at(e)) / fDistance;
																		resolver.fY = fOverlap * (y.y - squad.fPY.at(e)) / fDistance;
																		float fTotalmass = squad.fMass.at(e) + mass.fKilos;
																		float fNomalizedMassA = (squad.fMass.at(e) / fTotalmass);
																		float fNomalizedMassB = (mass.fKilos / fTotalmass);
																		squad.fPX.at(e) += (resolver.fX * fNomalizedMassB); // * normalized mass
																		x.x -= (resolver.fX * fNomalizedMassA);
																		squad.fPY.at(e) += (resolver.fY * fNomalizedMassB);
																		y.y -= (resolver.fY * fNomalizedMassA);
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

	void Update_Vector_From_Unit(entt::registry& zone) { // updates positions of soldiers stored in units for collision box
		auto view = zone.view<Component::Position, Component::Assigned_To, Component::Soldier>();
		for (auto entity : view) {
			auto& x = view.get<Component::Position>(entity);
			auto& y = view.get<Component::Position>(entity);
			auto& soldier = view.get<Component::Assigned_To>(entity);
			auto& squad = zone.get<Component::Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			squad.fPX.at(soldier.iIndex) = x.x;
			squad.fPY.at(soldier.iIndex) = y.y;
		}
	}


	void CollisionsT(entt::registry& zone) {
		if (Print_calcs.Calc()) {
			std::cout << "Company:      " << zone.size<Company>() << "   checks:   " << iCompany_Check << std::endl;
			std::cout << "Platoon:      " << zone.size<Platoon>() << "   checks:   " << iPlatoon_Check << std::endl;
			std::cout << "Squad:        " << zone.size<Squad>()   << "   checks:   " << iSquad_Check << std::endl;
			std::cout << "Soldiers:     " << iNumber << std::endl;
			std::cout << "Resolves:     " << iActual_Resolves << std::endl;
			std::cout << "----------   " << iActual_Resolves << std::endl;
			std::cout << "iunitGridCollisionCheck:     " << iunitGridCollisionCheck << std::endl;
			std::cout << "----------   " << iActual_Resolves << std::endl;
		}
	}
	

	void Resolve_Collisions(entt::registry& zone) {
		iActual_Resolves = 0;
		iNumber = 0;
		iSquad_Check = 0;
		iPlatoon_Check = 0;
		iCompany_Check = 0;



		Entity_vs_Grid_Collision(zone, Map::map);
		Formation_vs_Grid_collision(zone,Map::map);
		//
		Entity_vs_Formation_Collision(zone);
		Formation_vs_Formation_Collision(zone);
		//
		Spell_Player_Collision(zone);
		Melee_Unit_Player_Collision(zone);
		MeleeAttack_unit_Collision(zone);
		//
		Update_Collided_Unit(zone);



		//CollisionsT(zone);
	}
}