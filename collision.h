#pragma once
#include "components.h"
#include "scene.h"
#include "timer.h"

using namespace Scene;

namespace collision {

	int number = 0;
	int checked = 0;
	Timer::Frame_Timer Collision_Calc_Rate(500.0f);

	void sort_Positions () {
		//auto entity1 = scene.group<Potential_Position_X, Potential_Position_Y>();
		scene.sort<Position_X>([](const auto& lhs, const auto& rhs) { return lhs.fPX < rhs.fPX; }); //sorts position least to ighest
		scene.sort<Position_Y>([](const auto& lhs, const auto& rhs) { return lhs.fPY < rhs.fPY; }); //sorts position least to	
	}

	//pushed other entities with a velozity componenets and just collides and stops with entities that don't I think

	void collisionUpdate(){ //checks every unit against one another need to make better
		//scene.each([](auto entity) {
		//	auto& l = scene.get<Position_X>(entity);
		//	//	std::cout << &l << std::endl       ;
		//	l.fPX += 0.5f;
		//	});
		if (Collision_Calc_Rate.Calc()) {
			number = 0;
			checked = 0;
			auto entity1 = scene.view<Position_X, Position_Y, Mass, Collision_Radius, Velocity, Moving>(entt::exclude<Collided>); //vel is only here to filter for mobile entities
			auto entity2 = scene.view<Position_Y, Position_X, Collision_Radius, Mass>();

			auto observer = entt::observer{ scene, entt::collector.update<Position_X>() };
			for (auto entity9 : observer) {				

			}
			/*sne.on_construct<Position_X>().connect<>();
			scene.on_destroy<Position_X>().connect<>();
			scene.on_update<Position_X>().connect<>();*/


			//entt::id_type types[] = { entt::type_hash<Potential_Position_X>::value(), entt::type_hash<Potential_Position_X>::value() };
			//auto view = scene.runtime_view(std::cbegin(types), std::cend(types));

			//views print sorted by the first component placed in
			/*for (auto e : entity2) {
				auto& jj = entity2.get<Potential_Position_X>(e);
				std::cout << "  X:  " << jj.fX << std::endl;
			}
			for (auto f : entity2) {
				auto& kk = entity2.get<Potential_Position_Y>(f);
				std::cout << "  Y:  " << kk.fY << std::endl;
			}*/

			//when I iterate through them in individual views they print sorted
			/*auto entity3 = scene.view<Potential_Position_X>();
			auto entity4 = scene.view<Potential_Position_Y>();
			for (auto e : entity3) {
				auto& jj = entity3.get<Potential_Position_X>(e);
				std::cout << "  X:  " << jj.fX << std::endl;
			}
			for (auto f : entity4) {
				auto& kk = entity4.get<Potential_Position_Y>(f);
				std::cout << "  Y:  " << kk.fY << std::endl;
			}*/
			////////////////////////////////////////////////////////////////////////////

			//cycle all movable (moving?) entities
			//for each do a "range for" loop for the x component collision candidates
			//check each enitity's "y" component for if it is in range
			//if it is, run the algorith against that entity

			//OR, not sure which can  be done

			//cycle all movable (moving?) entities
			//for each do a "range for" loop for the x component collision candidates
			//for each do a "range for" loop for the y component collision candidates
			//compare and run the algorith against and that are in both




			for (auto uu : entity1) {
				auto& aX = entity1.get<Position_X>(uu);
				auto& aY = entity1.get<Position_Y>(uu);
				auto& aC = entity1.get<Collision_Radius>(uu);
				auto& aM = entity1.get<Mass>(uu);

				//get size of 


				for (auto ii : entity2) {
					auto& bX = entity2.get<Position_X>(ii);
					auto& bY = entity2.get<Position_Y>(ii);
					auto& bC = entity2.get<Collision_Radius>(ii);
					auto& bM = entity2.get<Mass>(ii);
					int s = scene.size<Position_X>();



					if (aX.fPX != bX.fPX && aY.fPY != bY.fPY) {						//x = a.x - tocheck.x
					//	if (aX.fPX - 30 < bX.fPX && aX.fPX + 30 > bX.fPX && aY.fPY - 30 < bY.fPY && aY.fPY + 30 > bY.fPY) {
						checked++;
						float fx = aX.fPX - bX.fPX;
						//y = a.y - tocheck.y
						float fy = aY.fPY - bY.fPY;
						// pythagoras x,y gets the distance							
						float fDistance = (fx * fx) + (fy * fy);
						if (fDistance <= ((aC.fCollisionRadius + bC.fCollisionRadius) * (aC.fCollisionRadius + bC.fCollisionRadius)) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
							number++;
							scene.emplace_or_replace<Collided>(ii);
							
							fDistance = sqrtf(fDistance);
							float fOverlap = fDistance - (aC.fCollisionRadius + bC.fCollisionRadius);
							f2d resolver;
							resolver.fX = fOverlap * (bX.fPX - aX.fPX) / fDistance;
							resolver.fY = fOverlap * (bY.fPY - aY.fPY) / fDistance;
							float fTotalmass = aM.fKilos + bM.fKilos;
							float fNomalizedMassA = (aM.fKilos / fTotalmass);
							float fNomalizedMassB = (bM.fKilos / fTotalmass);
							aX.fPX += (resolver.fX * fNomalizedMassB); // * normalized mass
							aY.fPY += (resolver.fY * fNomalizedMassB);
							bX.fPX -= (resolver.fX * fNomalizedMassA);
							bY.fPY -= (resolver.fY * fNomalizedMassA);

							}
						//}
					}
				}
			}		

			auto entity3 = scene.view<Position_X, Position_Y, Mass, Collision_Radius, Collided>();

			for (auto uu : entity3) {
				auto& aX = entity3.get<Position_X>(uu);
				auto& aY = entity3.get<Position_Y>(uu);
				auto& aC = entity3.get<Collision_Radius>(uu);
				auto& aM = entity3.get<Mass>(uu);
				
				

				for (auto ii : entity2) {
					auto& bX = entity2.get<Position_X>(ii);
					auto& bY = entity2.get<Position_Y>(ii);
					auto& bC = entity2.get<Collision_Radius>(ii);
					auto& bM = entity2.get<Mass>(ii);

					if (aX.fPX != bX.fPX && aY.fPY != bY.fPY) {						//x = a.x - tocheck.x
					//	if (aX.fPX - 30 < bX.fPX && aX.fPX + 30 > bX.fPX && aY.fPY - 30 < bY.fPY && aY.fPY + 30 > bY.fPY) {
						checked++;
						float fx = aX.fPX - bX.fPX;
						//y = a.y - tocheck.y
						float fy = aY.fPY - bY.fPY;
						// pythagoras x,y gets the distance							
						float fDistance = (fx * fx) + (fy * fy);
						if (fDistance <= ((aC.fCollisionRadius + bC.fCollisionRadius) * (aC.fCollisionRadius + bC.fCollisionRadius)) * 0.9f) { // the constant keeps it from check collisions overlapping by round errors							
							number++;			
							scene.emplace_or_replace<Collided>(ii);
							
							fDistance = sqrtf(fDistance);
							float fOverlap = fDistance - (aC.fCollisionRadius + bC.fCollisionRadius);
							f2d resolver;
							resolver.fX = fOverlap * (bX.fPX - aX.fPX) / fDistance;
							resolver.fY = fOverlap * (bY.fPY - aY.fPY) / fDistance;
							float fTotalmass = aM.fKilos + bM.fKilos;
							float fNomalizedMassA = (aM.fKilos / fTotalmass);
							float fNomalizedMassB = (bM.fKilos / fTotalmass);
							aX.fPX += (resolver.fX * fNomalizedMassB); // * normalized mass
							aY.fPY += (resolver.fY * fNomalizedMassB);
							bX.fPX -= (resolver.fX * fNomalizedMassA);
							bY.fPY -= (resolver.fY * fNomalizedMassA);	
							number++;
						}
						//}
					}
				}
				if (1) {
					scene.remove<Collided>(uu);
				}
			}
		}
		//if (testSquad.soldiers.size() != 0) {
		//	for (auto i : testSquad.soldiers) {
		//		std::cout << "checked:   " << i.assigned << std::endl;
		//	}
		//}
		//std::cout << "resolved:  " << number << std::endl;
	}



	void resolveCollisons() {	
		auto view = scene.view<Position_X, Position_Y>();			
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			x.fX = x.fPX;
			y.fY = y.fPY;		
		}
	}

	void Update_Squad() { // updates positions of soldiers stored in units for collision box
		auto view = scene.view<Position_X, Position_Y, Soldier>();
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& soldier = view.get<Soldier>(entity);			
			auto& squad = scene.get<Squad>(soldier.SquadAssgnedTo); // gets the squad that the soldier is attached to
			squad.fPX.at(soldier.iIndex) = x.fPX;
			squad.fPY.at(soldier.iIndex) = y.fPY;
		}
	}


	void Update_Collided_Soldier() {
		auto view = scene.view<Position_X, Position_Y, Soldier>();
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& soldier = view.get<Soldier>(entity);
			auto& squad = scene.get<Squad>(soldier.SquadAssgnedTo); // gets the squad that the soldier is attached to
			x.fPX = squad.fPX.at(soldier.iIndex);
			y.fPY = squad.fPY.at(soldier.iIndex);
		}
	}

	/// 
	/// Collision Calculate stuff
	/// 
	/// 
	void Get_Unit_Collider(Squad &unit) {

	}




	
////////////////////////////////////////////////////////////////////////////////////
	

	void Update_Units() {
		auto view = scene.view<Squad>();
		for (auto squads : view) { //update colliders
			auto& squad = view.get<Squad>(squads);
			for (int i = 0; i < squad.sSub_Units.size(); i++) {
				for (int j = 0; j < squad.sSub_Units.size(); j++) {
					if (squad.sSub_Units.at(i) != squad.sSub_Units.at(j)) {
						float fx = squad.fPX.at(i) - squad.fPX.at(j);
						float fy = squad.fPY.at(i) - squad.fPY.at(j);
						float fDistance = (fx * fx) + (fy * fy);
						number++;
						if (fDistance <= ((squad.fRadius.at(i) + squad.fRadius.at(j)) * (squad.fRadius.at(i) + squad.fRadius.at(j))) * 0.99f) { // the constant keeps it from check collisions overlapping by round errors							
							fDistance = sqrtf(fDistance);
							float fOverlap = fDistance - (squad.fRadius.at(i) + squad.fRadius.at(j));
							f2d resolver;
							resolver.fX = fOverlap * (squad.fPX.at(j) - squad.fPX.at(i)) / fDistance;
							resolver.fY = fOverlap * (squad.fPY.at(j) - squad.fPY.at(i)) / fDistance;
							float fTotalmass = squad.fMass.at(i) + squad.fMass.at(j);
							float fNomalizedMassA = (squad.fMass.at(i) / fTotalmass);
							float fNomalizedMassB = (squad.fMass.at(j) / fTotalmass);
							squad.fPX.at(i) += (resolver.fX * fNomalizedMassB); // * normalized mass
							squad.fPX.at(j) -= (resolver.fX * fNomalizedMassA);
							squad.fPY.at(i) += (resolver.fY * fNomalizedMassB);
							squad.fPY.at(j) -= (resolver.fY * fNomalizedMassA);
						}
					}
				}
			}

			if (!squad.sSub_Units.empty()) { //will crash if vector is empty
				float xMax = *std::max_element(squad.fPX.begin(), squad.fPX.end());
				float xMin = *std::min_element(squad.fPX.begin(), squad.fPX.end());
				float yMax = *std::max_element(squad.fPY.begin(), squad.fPY.end());
				float yMin = *std::min_element(squad.fPY.begin(), squad.fPY.end());

				squad.collide_Box = { xMin, yMin, (xMax - xMin), (yMax - yMin) };
				//auto view2 = scene.view<Camera>();
				//for (auto camera : view2) {
				//	auto& cam = view2.get<Camera>(camera);
				//	SDL_Rect o = { (xMin - 15) - cam.screen.x, (yMin - 15) - cam.screen.y, xMax - xMin, yMax - yMin };
				//	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				//	SDL_RenderDrawRect(renderer, &o);
				//}
			}
		}
		for (auto squads : view) { //update colliders
			auto& squad1 = view.get<Squad>(squads);
			for (auto squads : view) { //update colliders
				auto& squad2 = view.get<Squad>(squads);
				if (Utilities::Inside_Mousebox(squad1.collide_Box, squad2.collide_Box)) {
					for (int i = 0; i < squad1.sSub_Units.size(); i++) {
						for (int j = 0; j < squad2.sSub_Units.size(); j++) {
							if (squad1.sSub_Units[i] != squad2.sSub_Units[j]) {
								float fx = squad1.fPX.at(i) - squad2.fPX.at(j);
								float fy = squad1.fPY.at(i) - squad2.fPY.at(j);
								float fDistance = (fx * fx) + (fy * fy);
								number++;
								if (fDistance <= ((squad1.fRadius.at(i) + squad2.fRadius.at(j)) * (squad1.fRadius.at(i) + squad2.fRadius.at(j))) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors							
									fDistance = sqrtf(fDistance);
									float fOverlap = fDistance - (squad1.fRadius.at(i) + squad2.fRadius.at(j));
									f2d resolver;
									resolver.fX = fOverlap * (squad2.fPX.at(j) - squad1.fPX.at(i)) / fDistance;
									resolver.fY = fOverlap * (squad2.fPY.at(j) - squad1.fPY.at(i)) / fDistance;
									float fTotalmass = squad1.fMass.at(i) + squad2.fMass.at(j);
									float fNomalizedMassA = (squad1.fMass.at(i) / fTotalmass);
									float fNomalizedMassB = (squad2.fMass.at(j) / fTotalmass);
									squad1.fPX.at(i) += (resolver.fX * fNomalizedMassB); // * normalized mass
									squad2.fPX.at(j) -= (resolver.fX * fNomalizedMassA);
									squad1.fPY.at(i) += (resolver.fY * fNomalizedMassB);
									squad2.fPY.at(j) -= (resolver.fY * fNomalizedMassA);
								}
							}
						}
					}
				}
			}
		}
	}

	void CollisionsT() {
		if (Collision_Calc_Rate.Calc()) {
			std::cout << number << std::endl;
		}
	}


	void Collisions() {
		number = 0;
		sort_Positions();
		Update_Squad();
		Update_Units(); 
		Update_Collided_Soldier();
		resolveCollisons();
		CollisionsT();
	}

}