#pragma once
#include "entt.hpp"
#include "components.h"

using namespace Component;

namespace Formation_Collision {

	void Add_Or_Update_Collidee(entt::registry& zone, entt::entity& entity, Position& position, float& potentialX, float& potentialY) {
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

	struct Formation_Data {
		Test::Unit_Formation_Data formation_1;
		Test::Unit_Formation_Data formation_2;
		int index1 = 0;
		int index2 = 0;
	};


	Formation_Data Pass_Data(Formation_Data& formationData_Input) {
		Formation_Data formationData_Output;
		formationData_Output.formation_1 = formationData_Input.formation_1.subformationData.at(formationData_Input.index1);
		formationData_Output.formation_2 = formationData_Input.formation_2.subformationData.at(formationData_Input.index2);
		return  formationData_Output;
	}

	struct Unit_Data {
		Test::Soldiers_Assigned_List soldier_1;
		Test::Soldiers_Assigned_List soldier_2;
		int index1 = 0;
		int index2 = 0;
	};

	void Unit_Intersect(entt::registry &zone, Unit_Data& unit_Data) {
		for (unit_Data.index1 = 0; unit_Data.index1 < unit_Data.soldier_1.unitData.size(); unit_Data.index1++) {
			for (unit_Data.index2 = 0; unit_Data.index2 < unit_Data.soldier_2.unitData.size(); unit_Data.index2++) {				
				Test::Soldier_Data soldier1 = unit_Data.soldier_1.unitData.at(unit_Data.index1);
				Test::Soldier_Data soldier2 = unit_Data.soldier_2.unitData.at(unit_Data.index2);
				
				if (soldier1.bAlive == true && soldier2.bAlive == true) {
					if (soldier1.unit_ID != soldier2.unit_ID) {
	
					float fx = soldier1.x - soldier2.x;
					float fy = soldier1.y - soldier2.y;
					float fDistance = (fx * fx) + (fy * fy);
																		
					if (fDistance <= ((soldier1.radius + soldier2.radius) * (soldier1.radius + soldier2.radius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
						fDistance = sqrtf(fDistance);
						float fOverlap = fDistance - (soldier1.radius + soldier2.radius);
						DataTypes::f2d resolver = {};
						resolver.fX = fOverlap * (soldier2.x - soldier1.x) / fDistance;
						resolver.fY = fOverlap * (soldier2.y - soldier1.y) / fDistance;
						float fTotalmass = soldier1.mass + soldier2.mass;
						float fNomalizedMassA = (soldier1.mass / fTotalmass);
						float fNomalizedMassB = (soldier2.mass / fTotalmass);


						Position position = { soldier2.x, soldier2.y };
						float resolverX = (resolver.fX * fNomalizedMassA);
						float resolverY = (resolver.fY * fNomalizedMassA);
						Add_Or_Update_Collider(zone, soldier2.unit_ID, position, resolverX, resolverY);

						position = { soldier1.x, soldier1.y };
						resolverX = (resolver.fX * fNomalizedMassB); // * normalized mass
						resolverY = (resolver.fY * fNomalizedMassB);
						Add_Or_Update_Collidee(zone, soldier1.unit_ID, position, resolverX, resolverY);
						
					}
					}
				}
			}
		}
	}


	void Formation_vs_Formation_Collision(entt::registry& zone) {

		Formation_Data armyData;
		Formation_Data corpData;
		Formation_Data divisionData;
		Formation_Data brigadeData;
		Formation_Data battalionData;
		Formation_Data companyData;
		Formation_Data platoonData;
		Formation_Data squadData;


		auto army_view = zone.view<Test::Army, Test::Unit_Formation_Data>();
			
		for (auto armies : army_view) {
			auto& army_1 = army_view.get<Test::Unit_Formation_Data>(armies);
			for (auto armies : army_view) {
				auto& army_2 = army_view.get<Test::Unit_Formation_Data>(armies);
				if (Utilities::bFRect_Intersect(army_1.sCollide_Box, army_2.sCollide_Box)) {
					armyData = { army_1, army_2 };
					corpData = Pass_Data(armyData);
					for (corpData.index1 = 0; corpData.index1 < corpData.formation_1.subformationData.size(); corpData.index1++) {
						for (corpData.index2 = 0; corpData.index2 < corpData.formation_2.subformationData.size(); corpData.index2++) {
							if (Utilities::bFRect_Intersect(corpData.formation_1.sCollide_Box, corpData.formation_2.sCollide_Box)) {
								divisionData = Pass_Data(corpData);
								for (divisionData.index1 = 0; divisionData.index1 < divisionData.formation_1.subformationData.size(); divisionData.index1++) {
									for (divisionData.index2 = 0; divisionData.index2 < divisionData.formation_2.subformationData.size(); divisionData.index2++) {
										if (Utilities::bFRect_Intersect(divisionData.formation_1.sCollide_Box, divisionData.formation_2.sCollide_Box)) {
											brigadeData = Pass_Data(divisionData);
											for (brigadeData.index1 = 0; brigadeData.index1 < brigadeData.formation_1.subformationData.size(); brigadeData.index1++) {
												for (brigadeData.index2 = 0; brigadeData.index2 < brigadeData.formation_2.subformationData.size(); brigadeData.index2++) {
													if (Utilities::bFRect_Intersect(brigadeData.formation_1.sCollide_Box, brigadeData.formation_2.sCollide_Box)) {
														battalionData = Pass_Data(brigadeData);
														for (battalionData.index1 = 0; battalionData.index1 < battalionData.formation_1.subformationData.size(); battalionData.index1++) {
															for (battalionData.index2 = 0; battalionData.index2 < battalionData.formation_2.subformationData.size(); battalionData.index2++) {
																if (Utilities::bFRect_Intersect(battalionData.formation_1.sCollide_Box, battalionData.formation_2.sCollide_Box)) {
																	companyData = Pass_Data(battalionData);
																	for (companyData.index1 = 0; companyData.index1 < companyData.formation_1.subformationData.size(); companyData.index1++) {
																		for (companyData.index2 = 0; companyData.index2 < companyData.formation_2.subformationData.size(); companyData.index2++) {
																			if (Utilities::bFRect_Intersect(companyData.formation_1.sCollide_Box, companyData.formation_2.sCollide_Box)) {
																				platoonData = Pass_Data(companyData);
																				for (platoonData.index1 = 0; platoonData.index1 < platoonData.formation_1.subformationData.size(); platoonData.index1++) {
																					for (platoonData.index2 = 0; platoonData.index2 < platoonData.formation_2.subformationData.size(); platoonData.index2++) {
																						if (Utilities::bFRect_Intersect(platoonData.formation_1.sCollide_Box, platoonData.formation_2.sCollide_Box)) {
										
																							auto& soldiersList_1 = zone.get<Test::Soldiers_Assigned_List>(platoonData.formation_1.subformationData.at(platoonData.index1).formation_ID);
																							auto& soldiersList_2 = zone.get<Test::Soldiers_Assigned_List>(platoonData.formation_2.subformationData.at(platoonData.index2).formation_ID);
																							Unit_Data unitData = { soldiersList_1 , soldiersList_2 };
																							Unit_Intersect(zone, unitData);
																							
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
					}
				}
			}
		}
	}

	struct Formation_Data_Single {
		Test::Unit_Formation_Data formation;	
		int index = 0;
	};

	Formation_Data_Single Pass_Data_Single(Formation_Data_Single& formationData_Input) {
		Formation_Data_Single formationData_Output;
		formationData_Output.formation = formationData_Input.formation.subformationData.at(formationData_Input.index);
		return  formationData_Output;
	}

	struct Unit_Data_Single {
		Test::Soldiers_Assigned_List soldier;
		Test::Soldier_Data entity;
		int index = 0;		
	};


	void Unit_Intersect(entt::registry& zone, Unit_Data_Single& unit_Data) {
		for (unit_Data.index = 0; unit_Data.index < unit_Data.soldier.unitData.size(); unit_Data.index++) {
			Test::Soldier_Data soldier1 = unit_Data.soldier.unitData.at(unit_Data.index);
			Test::Soldier_Data entity = unit_Data.entity;

			if (soldier1.bAlive == true && entity.bAlive == true) {
				if (soldier1.unit_ID != entity.unit_ID) {

					float fx = soldier1.x - entity.x;
					float fy = soldier1.y - entity.y;
					float fDistance = (fx * fx) + (fy * fy);

					if (fDistance <= ((soldier1.radius + entity.radius) * (soldier1.radius + entity.radius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
						fDistance = sqrtf(fDistance);
						float fOverlap = fDistance - (soldier1.radius + entity.radius);
						DataTypes::f2d resolver = {};
						resolver.fX = fOverlap * (entity.x - soldier1.x) / fDistance;
						resolver.fY = fOverlap * (entity.y - soldier1.y) / fDistance;
						float fTotalmass = soldier1.mass + entity.mass;
						float fNomalizedMassA = (soldier1.mass / fTotalmass);
						float fNomalizedMassB = (entity.mass / fTotalmass);


						Position position = { entity.x, entity.y };
						float resolverX = (resolver.fX * fNomalizedMassA);
						float resolverY = (resolver.fY * fNomalizedMassA);
						Add_Or_Update_Collider(zone, entity.unit_ID, position, resolverX, resolverY);

						position = { soldier1.x, soldier1.y };
						resolverX = (resolver.fX * fNomalizedMassB); // * normalized mass
						resolverY = (resolver.fY * fNomalizedMassB);
						Add_Or_Update_Collidee(zone, soldier1.unit_ID, position, resolverX, resolverY);

					}
				}
			}
			
		}
	}

	void Entity_vs_Formation_COllision(entt::registry &zone) {

		Formation_Data_Single armyData;
		Formation_Data_Single corpData;
		Formation_Data_Single divisionData;
		Formation_Data_Single brigadeData;
		Formation_Data_Single battalionData;
		Formation_Data_Single companyData;
		Formation_Data_Single platoonData;
		Formation_Data_Single squadData;
		
		auto view = zone.view<Radius, Position, Mass, Alive>(entt::exclude<Component::Assigned_To_Formation>);
		auto army_view = zone.view<Test::Army, Test::Unit_Formation_Data>();
		
		
		for (auto entity_ID : view) {
			auto& radius = view.get<Radius>(entity_ID).fRadius;
			auto& position = view.get<Position>(entity_ID);
			auto& mass = view.get<Mass>(entity_ID).fKilos;
			auto& alive= view.get<Alive>(entity_ID).bIsAlive;
			SDL_FRect entityColliderRect = { position.x - radius, position.y - radius, radius* 2.0f, radius * 2.0f };

			for (auto armies : army_view) {
				auto& army = army_view.get<Test::Unit_Formation_Data>(armies);
				if (Utilities::bFRect_Intersect(army.sCollide_Box, entityColliderRect)) {
					armyData = { army };
					corpData = Pass_Data_Single(armyData);
					for (corpData.index = 0; corpData.index < corpData.formation.subformationData.size(); corpData.index++) {
						if (Utilities::bFRect_Intersect(corpData.formation.sCollide_Box, entityColliderRect)) {
							divisionData = Pass_Data_Single(corpData);
							for (divisionData.index = 0; divisionData.index < divisionData.formation.subformationData.size(); divisionData.index++) {
								if (Utilities::bFRect_Intersect(divisionData.formation.sCollide_Box, entityColliderRect)) {
									brigadeData = Pass_Data_Single(divisionData);
									for (brigadeData.index = 0; brigadeData.index < brigadeData.formation.subformationData.size(); brigadeData.index++) {
										if (Utilities::bFRect_Intersect(brigadeData.formation.sCollide_Box, entityColliderRect)) {
											battalionData = Pass_Data_Single(brigadeData);
											for (battalionData.index = 0; battalionData.index < battalionData.formation.subformationData.size(); battalionData.index++) {
												if (Utilities::bFRect_Intersect(battalionData.formation.sCollide_Box, entityColliderRect)) {
													companyData = Pass_Data_Single(battalionData);
													for (companyData.index = 0; companyData.index < companyData.formation.subformationData.size(); companyData.index++) {
														if (Utilities::bFRect_Intersect(companyData.formation.sCollide_Box, entityColliderRect)) {
															platoonData = Pass_Data_Single(companyData);
															for (platoonData.index = 0; platoonData.index < platoonData.formation.subformationData.size(); platoonData.index++) {
																if (Utilities::bFRect_Intersect(platoonData.formation.sCollide_Box, entityColliderRect)) {

																	auto& soldiersList = zone.get<Test::Soldiers_Assigned_List>(platoonData.formation.subformationData.at(platoonData.index).formation_ID);
																	Test::Soldier_Data entity = {entity_ID, position.x, position.y, mass, radius, 0, alive};
																	Unit_Data_Single unitData = { soldiersList , entity };
																	Unit_Intersect(zone, unitData);

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


	void Update_Collided_Unit(entt::registry& zone) {
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



	void Test_Collision(entt::registry &zone) {
		Formation_vs_Formation_Collision(zone);
		Entity_vs_Formation_COllision(zone);
		Update_Collided_Unit(zone);
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


}