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



	Timer::Frame_Timer Collision_Calc_Rate(16.0f); // collision update rate
	Timer::Frame_Timer Print_calcs(500.0f);
	void sort_Positions () {
		scene.sort<Position_X>([](const auto& lhs, const auto& rhs) { return lhs.fPX < rhs.fPX; }); //sorts position least to ighest
		scene.sort<Position_Y>([](const auto& lhs, const auto& rhs) { return lhs.fPY < rhs.fPY; }); //sorts position least to	
	}

	//pushed other entities with a velozity componenets and just collides and stops with entities that don't I think

	//void collisionUpdate(){ //checks every unit against one another need to make better
	//	//scene.each([](auto entity) {
	//	//	auto& l = scene.get<Position_X>(entity);
	//	//	//	std::cout << &l << std::endl       ;
	//	//	l.fPX += 0.5f;
	//	//	});
	//	if (Collision_Calc_Rate.Calc()) {
	//		number = 0;
	//		//checked = 0;
	//		auto entity1 = scene.view<Position_X, Position_Y, Mass, Collision_Radius, Velocity, Moving>(entt::exclude<Collided>); //vel is only here to filter for mobile entities
	//		auto entity2 = scene.view<Position_Y, Position_X, Collision_Radius, Mass>();

	//		auto observer = entt::observer{ scene, entt::collector.update<Position_X>() };
	//		for (auto entity9 : observer) {				

	//		}
	//		/*sne.on_construct<Position_X>().connect<>();
	//		scene.on_destroy<Position_X>().connect<>();
	//		scene.on_update<Position_X>().connect<>();*/


	//		//entt::id_type types[] = { entt::type_hash<Potential_Position_X>::value(), entt::type_hash<Potential_Position_X>::value() };
	//		//auto view = scene.runtime_view(std::cbegin(types), std::cend(types));

	//		//views print sorted by the first component placed in
	//		/*for (auto e : entity2) {
	//			auto& jj = entity2.get<Potential_Position_X>(e);
	//			std::cout << "  X:  " << jj.fX << std::endl;
	//		}
	//		for (auto f : entity2) {
	//			auto& kk = entity2.get<Potential_Position_Y>(f);
	//			std::cout << "  Y:  " << kk.fY << std::endl;
	//		}*/

	//		//when I iterate through them in individual views they print sorted
	//		/*auto entity3 = scene.view<Potential_Position_X>();
	//		auto entity4 = scene.view<Potential_Position_Y>();
	//		for (auto e : entity3) {
	//			auto& jj = entity3.get<Potential_Position_X>(e);
	//			std::cout << "  X:  " << jj.fX << std::endl;
	//		}
	//		for (auto f : entity4) {
	//			auto& kk = entity4.get<Potential_Position_Y>(f);
	//			std::cout << "  Y:  " << kk.fY << std::endl;
	//		}*/
	//		////////////////////////////////////////////////////////////////////////////

	//		//cycle all movable (moving?) entities
	//		//for each do a "range for" loop for the x component collision candidates
	//		//check each enitity's "y" component for if it is in range
	//		//if it is, run the algorith against that entity

	//		//OR, not sure which can  be done

	//		//cycle all movable (moving?) entities
	//		//for each do a "range for" loop for the x component collision candidates
	//		//for each do a "range for" loop for the y component collision candidates
	//		//compare and run the algorith against and that are in both




	//		for (auto uu : entity1) {
	//			auto& aX = entity1.get<Position_X>(uu);
	//			auto& aY = entity1.get<Position_Y>(uu);
	//			auto& aC = entity1.get<Collision_Radius>(uu);
	//			auto& aM = entity1.get<Mass>(uu);

	//			//get size of 


	//			for (auto ii : entity2) {
	//				auto& bX = entity2.get<Position_X>(ii);
	//				auto& bY = entity2.get<Position_Y>(ii);
	//				auto& bC = entity2.get<Collision_Radius>(ii);
	//				auto& bM = entity2.get<Mass>(ii);
	//				int s = scene.size<Position_X>();



	//				if (aX.fPX != bX.fPX && aY.fPY != bY.fPY) {						//x = a.x - tocheck.x
	//				//	if (aX.fPX - 30 < bX.fPX && aX.fPX + 30 > bX.fPX && aY.fPY - 30 < bY.fPY && aY.fPY + 30 > bY.fPY) {
	//					//checked++;
	//					float fx = aX.fPX - bX.fPX;
	//					//y = a.y - tocheck.y
	//					float fy = aY.fPY - bY.fPY;
	//					// pythagoras x,y gets the distance							
	//					float fDistance = (fx * fx) + (fy * fy);
	//					if (fDistance <= ((aC.fCollisionRadius + bC.fCollisionRadius) * (aC.fCollisionRadius + bC.fCollisionRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
	//						//number++;
	//						scene.emplace_or_replace<Collided>(ii);
	//						
	//						fDistance = sqrtf(fDistance);
	//						float fOverlap = fDistance - (aC.fCollisionRadius + bC.fCollisionRadius);
	//						f2d resolver;
	//						resolver.fX = fOverlap * (bX.fPX - aX.fPX) / fDistance;
	//						resolver.fY = fOverlap * (bY.fPY - aY.fPY) / fDistance;
	//						float fTotalmass = aM.fKilos + bM.fKilos;
	//						float fNomalizedMassA = (aM.fKilos / fTotalmass);
	//						float fNomalizedMassB = (bM.fKilos / fTotalmass);
	//						aX.fPX += (resolver.fX * fNomalizedMassB); // * normalized mass
	//						aY.fPY += (resolver.fY * fNomalizedMassB);
	//						bX.fPX -= (resolver.fX * fNomalizedMassA);
	//						bY.fPY -= (resolver.fY * fNomalizedMassA);

	//						}
	//					//}
	//				}
	//			}
	//		}		

	//		auto entity3 = scene.view<Position_X, Position_Y, Mass, Collision_Radius, Collided>();

	//		for (auto uu : entity3) {
	//			auto& aX = entity3.get<Position_X>(uu);
	//			auto& aY = entity3.get<Position_Y>(uu);
	//			auto& aC = entity3.get<Collision_Radius>(uu);
	//			auto& aM = entity3.get<Mass>(uu);
	//			
	//			

	//			for (auto ii : entity2) {
	//				auto& bX = entity2.get<Position_X>(ii);
	//				auto& bY = entity2.get<Position_Y>(ii);
	//				auto& bC = entity2.get<Collision_Radius>(ii);
	//				auto& bM = entity2.get<Mass>(ii);

	//				if (aX.fPX != bX.fPX && aY.fPY != bY.fPY) {						//x = a.x - tocheck.x
	//				//	if (aX.fPX - 30 < bX.fPX && aX.fPX + 30 > bX.fPX && aY.fPY - 30 < bY.fPY && aY.fPY + 30 > bY.fPY) {
	//					checked++;
	//					float fx = aX.fPX - bX.fPX;
	//					//y = a.y - tocheck.y
	//					float fy = aY.fPY - bY.fPY;
	//					// pythagoras x,y gets the distance							
	//					float fDistance = (fx * fx) + (fy * fy);
	//					if (fDistance <= ((aC.fCollisionRadius + bC.fCollisionRadius) * (aC.fCollisionRadius + bC.fCollisionRadius)) * 0.9f) { // the constant keeps it from check collisions overlapping by round errors							
	//						number++;			
	//						scene.emplace_or_replace<Collided>(ii);
	//						
	//						fDistance = sqrtf(fDistance);
	//						float fOverlap = fDistance - (aC.fCollisionRadius + bC.fCollisionRadius);
	//						f2d resolver;
	//						resolver.fX = fOverlap * (bX.fPX - aX.fPX) / fDistance;
	//						resolver.fY = fOverlap * (bY.fPY - aY.fPY) / fDistance;
	//						float fTotalmass = aM.fKilos + bM.fKilos;
	//						float fNomalizedMassA = (aM.fKilos / fTotalmass);
	//						float fNomalizedMassB = (bM.fKilos / fTotalmass);
	//						aX.fPX += (resolver.fX * fNomalizedMassB); // * normalized mass
	//						aY.fPY += (resolver.fY * fNomalizedMassB);
	//						bX.fPX -= (resolver.fX * fNomalizedMassA);
	//						bY.fPY -= (resolver.fY * fNomalizedMassA);	
	//						number++;
	//					}
	//					//}
	//				}
	//			}
	//			if (1) {
	//				scene.remove<Collided>(uu);
	//			}
	//		}
	//	}
	//	//if (testSquad.soldiers.size() != 0) {
	//	//	for (auto i : testSquad.soldiers) {
	//	//		std::cout << "checked:   " << i.assigned << std::endl;
	//	//	}
	//	//}
	//	//std::cout << "resolved:  " << number << std::endl;
	//}



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
		auto view = scene.view<Position_X, Position_Y, Assigned>();
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
		auto view = scene.view<Position_X, Position_Y, Assigned>();
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			//auto& z = view.get<Collision_Radius>(entity);
			auto& soldier = view.get<Assigned>(entity);
			auto& squad = scene.get<Squad>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			x.fPX = squad.fPX.at(soldier.iIndex);
			y.fPY = squad.fPY.at(soldier.iIndex);
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

	void Unit_Collision() {
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

	void CollisionsT() {
		if (Print_calcs.Calc()) {
			std::cout << "Company:      " << scene.size<Company>() << "   checks:   " << iCompany_Check << std::endl;
			std::cout << "Platoon:      " << scene.size<Platoon>() << "   checks:   " << iPlatoon_Check << std::endl;
			std::cout << "Squad:        " << scene.size<Squad>()   << "   checks:   " << iSquad_Check << std::endl;
			std::cout << "Soldiers:     " << iNumber << std::endl;
			std::cout << "Resolves:     " << iActual_Resolves << std::endl;
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

		sort_Positions();

		Update_Unit();
		Update_Platoons();
		Update_Company();

		Update_Unit_Boxes();

		Unit_Collision(); 
		Update_Collided_Unit();
		resolveCollisons();

		CollisionsT();
	}
}