#pragma once
#include "components.h"
#include "pools.h"
#include "timer.h"

using namespace Scene;

namespace collision {

	float frames = 60.0f;

	bool collisionCalc() { //controls how often collision calculates
		frames -= Timer::timeStep;
		if (frames <= 0) { 
			frames = 16.0f; //every this many milliseconds
			return true;
		}
		else return false;
	}


	void sort_Positions () {

			//auto entity1 = scene.group<Potential_Position_X, Potential_Position_Y>();
			scene.sort<Position_X>([](const auto& lhs, const auto& rhs) { return lhs.fPX < rhs.fPX; }); //sorts position least to ighest
			scene.sort<Position_Y>([](const auto& lhs, const auto& rhs) { return lhs.fPY < rhs.fPY; }); //sorts position least to	

	}


	//pushed other entities with a velozity componenets and just collides and stops with entities that don't I think

	void collisionUpdate(){ //checks every unit against one another need to make better
		if (collisionCalc()) {
		auto entity1 = scene.view<Position_X, Position_Y, Mass, Collision_Radius, Velocity>(); //vel is only here to filter for mobile entities
		auto entity2 = scene.view<Position_Y, Position_X, Collision_Radius, Mass>();





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
			//std::cout << aX.fX << std::endl;
			
			//std::upper_bound(it, v.end(), *it)
			

			for (auto ii : entity2) {
				auto& bX = entity2.get<Position_X>(ii);
				auto& bY = entity2.get<Position_Y>(ii);
				auto& bC = entity2.get<Collision_Radius>(ii);
				auto& bM = entity2.get<Mass>(ii);

				if (aX.fPX != bX.fPX && aY.fPY != bY.fPY) {						//x = a.x - tocheck.x
					//if (aX.fPX - 20 < bX.fPX && aX.fPX + 20 > bX.fPX) {
						//if (aY.fPY - 20 < bY.fPY && aY.fPY + 20 > bY.fPY) {
							float fx = aX.fPX - bX.fPX;
							//y = a.y - tocheck.y
							float fy = aY.fPY - bY.fPY;
							// pythagoras x,y gets the distance
							float fDistance = sqrtf((fx * fx) + (fy * fy));
							if (fDistance <= (aC.fCollisionRadius + bC.fCollisionRadius) * 0.9999f) { // the constant keeps it from check collisions overlapping by round errors
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
					//	}
					}				
				}
			}		
		};
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


}