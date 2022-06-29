#pragma once
#include "components.h"
#include "scene.h"
#include "timer.h"

using namespace Scene;

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
		auto view = scene.view<Position_X, Position_Y>();			
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			x.fX = x.fPX;
			y.fY = y.fPY;		
		}
	}

	void Update_Unit() { // updates positions of soldiers stored in units for collision box
		auto view = scene.view<Position_X, Position_Y, Assigned, Soldier>();
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& soldier = view.get<Assigned>(entity);
			auto& squad = scene.get<Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			squad.fPX.at(soldier.iIndex) = x.fPX;
			squad.fPY.at(soldier.iIndex) = y.fPY;
		}
	}


	void Update_Company() { // updates positions of soldiers stored in units for collision box
		auto company_view = scene.view<Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			for (int i = 0; i < company.iSub_Units.size(); i++) {
				auto& platoon = scene.get<Platoon>(company.iSub_Units[i]); // gets the squad that the soldier is attached to
				company.fPX[i] = platoon.sCollide_Box.x;
				company.fPY[i] = platoon.sCollide_Box.y;
				company.fPW[i] = platoon.sCollide_Box.x + platoon.sCollide_Box.w;
				company.fPH[i] = platoon.sCollide_Box.y + platoon.sCollide_Box.h;
			}
		}
	}



	void Update_Platoons() { // updates positions of soldiers stored in units for collision box
		auto view = scene.view<Platoon>();
		for (auto platoons : view) {
			auto& platoon = view.get<Platoon>(platoons);
			for (int i = 0; i < platoon.iSub_Units.size(); i++) {
				auto& squad = scene.get<Squad>(platoon.iSub_Units[i]); // gets the squad that the soldier is attached to
				platoon.fPX[i] = squad.sCollide_Box.x;
				platoon.fPY[i] = squad.sCollide_Box.y;
				platoon.fPW[i] = squad.sCollide_Box.x + squad.sCollide_Box.w;
				platoon.fPH[i] = squad.sCollide_Box.y + squad.sCollide_Box.h;
			}
		}
	}




	void Update_Collided_Unit() {
		auto view = scene.view<Position_X, Position_Y, Assigned, Soldier, Health, Alive>();
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			//auto& z = view.get<Collision_Radius>(entity);
			auto& soldier = view.get<Assigned>(entity);
			auto& squad = scene.get<Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			x.fPX = squad.fPX.at(soldier.iIndex);
			y.fPY = squad.fPY.at(soldier.iIndex);
			
			//reduces health if Struck
			if (squad.iStruck.at(soldier.iIndex) > 0) {
				auto& h = view.get<Health>(entity).iHealth;
				auto& a = view.get<Alive>(entity).bIsAlive;
				h -= squad.iStruck.at(soldier.iIndex);
				squad.iStruck.at(soldier.iIndex) = 0;
				if (h <= 0) {
					squad.bAlive.at(soldier.iIndex) = false;
					a = false;					
				}
			}
		}
	}
	
////////////////////////////////////////////////////////////////////////////////////	
	void Update_Squad_Box() {
		auto view = scene.view<Squad>();
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
		auto Platoon_View = scene.view<Platoon>();
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
		auto company_view = scene.view<Company>();
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

	void player_unit_Collision() { //seems to work and pushes the units
		if (1) {
			auto spells = scene.view<Camera, Radius, Position_X, Position_Y, Mass>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& x = spells.get<Position_X>(spell);
				auto& y = spells.get<Position_Y>(spell);
				auto& mass = spells.get<Mass>(spell);
				SDL_FRect spell_collider = { x.fX - radius.fRadius, y.fY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };

				auto company_view = scene.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = scene.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
									auto& squad = scene.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											if (squad.bAlive.at(i) != false) {
												float fx = squad.fPX.at(i) - x.fPX;
												float fy = squad.fPY.at(i) - y.fPY;
												float fDistance = (fx * fx) + (fy * fy);
												if (fDistance <= ((squad.fRadius.at(i) + radius.fRadius) * (squad.fRadius.at(i) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
													fDistance = sqrtf(fDistance);
													float fOverlap = fDistance - (squad.fRadius.at(i) + radius.fRadius);
													f2d resolver = {};
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
			auto company_view = scene.view<Company>();
			for (auto companies : company_view) {
				auto& company1 = company_view.get<Company>(companies);
				for (auto companies : company_view) { 
					auto& company2 = company_view.get<Company>(companies);
					iCompany_Check++;
					if (Utilities::bRect_Intersect(company1.sCollide_Box, company2.sCollide_Box)) {
						for (int c1 = 0; c1 < company1.iSub_Units.size(); c1++) {
							for (int c2 = 0; c2 < company2.iSub_Units.size(); c2++) {
								auto& platoon1 = scene.get<Platoon>(company1.iSub_Units[c1]);
								auto& platoon2 = scene.get<Platoon>(company2.iSub_Units[c2]);
								iPlatoon_Check++;
								if (Utilities::bRect_Intersect(platoon1.sCollide_Box, platoon2.sCollide_Box)) {
									for (int s1 = 0; s1 < platoon1.iSub_Units.size(); s1++) {
										for (int s2 = 0; s2 < platoon2.iSub_Units.size(); s2++) {
											auto& squad1 = scene.get<Squad>(platoon1.iSub_Units[s1]);
											auto& squad2 = scene.get<Squad>(platoon2.iSub_Units[s2]);									
											iSquad_Check++;							
											if (Utilities::bRect_Intersect(squad1.sCollide_Box, squad2.sCollide_Box)) { //checks against itself too so that units with the squad will have collision
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
																	f2d resolver = {};
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
			auto spells = scene.view<Spell, Radius, Position_X, Position_Y, Mass, Alive>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& x = spells.get<Position_X>(spell);
				auto& y = spells.get<Position_Y>(spell);
				auto& mass = spells.get<Mass>(spell);
				auto& alive = spells.get<Alive>(spell);
				SDL_FRect spell_collider = { x.fX - radius.fRadius, y.fY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };

				auto company_view = scene.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = scene.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
								auto& squad = scene.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											if (squad.bAlive.at(i) != false) {
												float fx = squad.fPX.at(i) - x.fPX;
												float fy = squad.fPY.at(i) - y.fPY;
												float fDistance = (fx * fx) + (fy * fy);
												if (fDistance <= ((squad.fRadius.at(i) + radius.fRadius) * (squad.fRadius.at(i) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
													fDistance = sqrtf(fDistance);
													float fOverlap = fDistance - (squad.fRadius.at(i) + radius.fRadius);
													f2d resolver = {};
													resolver.fX = fOverlap * (x.fPX - squad.fPX.at(i)) / fDistance;
													resolver.fY = fOverlap * (y.fPY - squad.fPY.at(i)) / fDistance;
													float fTotalmass = squad.fMass.at(i) + mass.fKilos;
													float fNomalizedMassA = (squad.fMass.at(i) / fTotalmass);
													float fNomalizedMassB = (mass.fKilos / fTotalmass);
													squad.fPX.at(i) += (resolver.fX * fNomalizedMassB); // * normalized mass
													x.fPX -= (resolver.fX * fNomalizedMassA);
													squad.fPY.at(i) += (resolver.fY * fNomalizedMassB);
													y.fPY -= (resolver.fY * fNomalizedMassA);
													squad.iStruck.at(i)++;
													alive.bIsAlive = false;
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



	//change to use weapon size instead of weapon radius
	void MeleeAttack_unit_Collision() { //seems to work 
		if (1) {
			auto spells = scene.view<Melee, Radius, Position_X, Position_Y, Mass, Alive>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& x = spells.get<Position_X>(spell);
				auto& y = spells.get<Position_Y>(spell);
				auto& mass = spells.get<Mass>(spell);
				auto& alive = spells.get<Alive>(spell);
				SDL_FRect spell_collider = { x.fX - radius.fRadius, y.fY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };

				auto company_view = scene.view<Company>();
				for (auto companies : company_view) {
					auto& company = company_view.get<Company>(companies);
					if (Utilities::bRect_Intersect(company.sCollide_Box, spell_collider)) {
						for (int c = 0; c < company.iSub_Units.size(); c++) {
							auto& platoon = scene.get<Platoon>(company.iSub_Units[c]);
							if (Utilities::bRect_Intersect(platoon.sCollide_Box, spell_collider)) {
								for (int p = 0; p < platoon.iSub_Units.size(); p++) {
									auto& squad = scene.get<Squad>(platoon.iSub_Units[p]);
									if (Utilities::bRect_Intersect(squad.sCollide_Box, spell_collider)) { //checks against itself too so that units with the squad will have collision
										for (int i = 0; i < squad.iSub_Units.size(); i++) {
											if (squad.bAlive.at(i) != false) {
												float fx = squad.fPX.at(i) - x.fPX;
												float fy = squad.fPY.at(i) - y.fPY;
												float fDistance = (fx * fx) + (fy * fy);
												if (fDistance <= ((squad.fRadius.at(i) + radius.fRadius) * (squad.fRadius.at(i) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
													fDistance = sqrtf(fDistance);
													float fOverlap = fDistance - (squad.fRadius.at(i) + radius.fRadius);
													f2d resolver = {};
													resolver.fX = fOverlap * (x.fPX - squad.fPX.at(i)) / fDistance;
													resolver.fY = fOverlap * (y.fPY - squad.fPY.at(i)) / fDistance;
													float fTotalmass = squad.fMass.at(i) + mass.fKilos;
													float fNomalizedMassA = (squad.fMass.at(i) / fTotalmass);
													float fNomalizedMassB = (mass.fKilos / fTotalmass);
													squad.fPX.at(i) += (resolver.fX * fNomalizedMassB); // * normalized mass
													x.fPX -= (resolver.fX * fNomalizedMassA);
													squad.fPY.at(i) += (resolver.fY * fNomalizedMassB);
													y.fPY -= (resolver.fY * fNomalizedMassA);
													squad.iStruck.at(i)++;
													alive.bIsAlive = false;
													//std::cout << "Hit!" << std::endl;
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



	std::vector<std::vector<entt::entity>> grid_collision(SDL_FRect &unit_box, Map::Node3 &map) {
		int p = 0;
		std::vector<std::vector<entt::entity>>vec;
	//	std::cout << map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.x << "--------" << map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box.y << std::endl;

		//	SDL_FRect port;
		if (Utilities::bRect_Intersect(unit_box, map.sCollide_Box)) {
			for (int i = 0; i < Map::size; i++) {
				if (Utilities::bRect_Intersect(unit_box, map.nodes[i].sCollide_Box)) {
					for (int j = 0; j < Map::size; j++) {
						if (Utilities::bRect_Intersect(unit_box, map.nodes[i].nodes[j].sCollide_Box)) {
							for (int k = 0; k < Map::size; k++) {
								if (Utilities::bRect_Intersect(unit_box, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
									for (int l = 0; l < Map::size; l++) {
										if (Utilities::bRect_Intersect(unit_box, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
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
			auto spells = scene.view<Radius, Position_X, Position_Y, Mass, Input>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& x = spells.get<Position_X>(spell);
				auto& y = spells.get<Position_Y>(spell);
				auto& mass = spells.get<Mass>(spell);
				SDL_FRect unit_collider = { x.fX - radius.fRadius, y.fY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };

				std::vector<std::vector<entt::entity>> cell = grid_collision(unit_collider, map);

				for (int j = 0; j < cell.size(); j++) {
					for (int i = 0; i < cell[j].size(); i++) {
						if (spell != cell[j].at(i)) {
							p++;
							//if (scene.all_of<Radius, Mass, Position_Y, Position_X>(cell[j].at(i))) {
								//std::cout << cell.size() << " || " << cell[j].size() << std::endl;
							auto& map_x = scene.get<Position_X>(cell[j].at(i));
							auto& map_y = scene.get<Position_Y>(cell[j].at(i));
							auto& map_mass = scene.get<Mass>(cell[j].at(i));
							auto& map_radius = scene.get<Radius>(cell[j].at(i));
							float fx = map_x.fPX - x.fPX;
							float fy = map_y.fPY - y.fPY;
							float fDistance = (fx * fx) + (fy * fy);
							if (fDistance <= ((map_radius.fRadius + radius.fRadius) * (map_radius.fRadius + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
								fDistance = sqrtf(fDistance);
								float fOverlap = fDistance - (map_radius.fRadius + radius.fRadius);
								f2d resolver = {};
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
			auto spells = scene.view<Radius, Position_X, Position_Y, Mass, Spell, Alive>();
			for (auto spell : spells) {
				auto& radius = spells.get<Radius>(spell);
				auto& a = spells.get<Alive>(spell);
				auto& x = spells.get<Position_X>(spell);
				auto& y = spells.get<Position_Y>(spell);
				auto& mass = spells.get<Mass>(spell);
				SDL_FRect unit_collider = { x.fX - radius.fRadius, y.fY - radius.fRadius, radius.fRadius * 2.0f, radius.fRadius * 2.0f };
			
				std::vector<std::vector<entt::entity>> cell = grid_collision(unit_collider, map);				

				for (int j = 0; j < cell.size(); j++) {
					for (int i = 0; i < cell[j].size(); i++) {
						if (spell != cell[j].at(i)) {
							p++;
							//if (scene.all_of<Radius, Mass, Position_Y, Position_X>(cell[j].at(i))) {
								//std::cout << cell.size() << " || " << cell[j].size() << std::endl;
								auto& map_x = scene.get<Position_X>(cell[j].at(i));
								auto& map_y = scene.get<Position_Y>(cell[j].at(i));
								auto& map_mass = scene.get<Mass>(cell[j].at(i));
								auto& map_radius = scene.get<Radius>(cell[j].at(i));
								float fx = map_x.fPX - x.fPX;
								float fy = map_y.fPY - y.fPY;
								float fDistance = (fx * fx) + (fy * fy);
								if (fDistance <= ((map_radius.fRadius + radius.fRadius) * (map_radius.fRadius + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
									fDistance = sqrtf(fDistance);
									float fOverlap = fDistance - (map_radius.fRadius + radius.fRadius);
									f2d resolver = {};
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

		auto company_view = scene.view<Company>();
		
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);
			if (Utilities::bRect_Intersect(company.sCollide_Box, map.sCollide_Box)) {
				for (int b = 0; b < company.iSub_Units.size(); b++) {
					for (int i = 0; i < Map::size; i++) {
						for (int j = 0; j < Map::size; j++) {

							if (Utilities::bRect_Intersect(company.sCollide_Box, map.nodes[i].nodes[j].sCollide_Box)) {
								auto& platoon = scene.get<Platoon>(company.iSub_Units[b]);
								for (int c = 0; c < platoon.iSub_Units.size(); c++) {
									for (int k = 0; k < Map::size; k++) {

										if (Utilities::bRect_Intersect(platoon.sCollide_Box, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
											auto& squad = scene.get<Squad>(platoon.iSub_Units[c]);
											for (int l = 0; l < Map::size; l++) {

												if (Utilities::bRect_Intersect(squad.sCollide_Box, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
													for (int d = 0; d < squad.iSub_Units.size(); d++) {
														if (squad.bAlive.at(d) != false) {
															for (int m = 0; m < map.nodes[i].nodes[j].nodes[k].cells[l].entities.size(); m++) {
																auto& radius = scene.get<Radius>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& mass = scene.get<Mass>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& x = scene.get<Position_X>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																auto& y = scene.get<Position_Y>(map.nodes[i].nodes[j].nodes[k].cells[l].entities.at(m));
																for (int e = 0; e < squad.iSub_Units.size(); e++) {
																	float fx = squad.fPX.at(e) - x.fPX;
																	float fy = squad.fPY.at(e) - y.fPY;
																	float fDistance = (fx * fx) + (fy * fy);
																	if (fDistance <= ((squad.fRadius.at(e) + radius.fRadius) * (squad.fRadius.at(e) + radius.fRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
																		fDistance = sqrtf(fDistance);
																		float fOverlap = fDistance - (squad.fRadius.at(e) + radius.fRadius);
																		f2d resolver = {};
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
			std::cout << "Company:      " << scene.size<Company>() << "   checks:   " << iCompany_Check << std::endl;
			std::cout << "Platoon:      " << scene.size<Platoon>() << "   checks:   " << iPlatoon_Check << std::endl;
			std::cout << "Squad:        " << scene.size<Squad>()   << "   checks:   " << iSquad_Check << std::endl;
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
		
		Spell_unit_Collision();		
		MeleeAttack_unit_Collision();

		Update_Collided_Unit();
		resolveCollisons();
	
		//CollisionsT();
	}
}