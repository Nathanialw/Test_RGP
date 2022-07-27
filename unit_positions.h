#pragma once
#include "entt.hpp"
#include "components.h"

namespace Unit_Position {

	void Update_Unit_Positions(entt::registry& zone) {
		auto view = zone.view<Component::Position, Component::Potential_Position>();
		for (auto entity : view) {
			auto& position = view.get<Component::Position>(entity);
			auto& potentialPosition = view.get<Component::Potential_Position>(entity);
			
			position = {potentialPosition.x, potentialPosition.y};
		}
	}

	void Update_Formation_Unit_Positions(entt::registry& zone) { // updates positions of soldiers stored in units for collision box
		auto view = zone.view<Component::Position, Component::Assigned_To, Component::Soldier>();
		
		for (auto entity : view) {
			auto& x = view.get<Component::Position>(entity);
			auto& y = view.get<Component::Position>(entity);
			auto& soldier = view.get<Component::Assigned_To>(entity);
			
			
			auto& formation = zone.get<Test::Soldiers_Assigned_List>(soldier.iUnit_Assigned_To); // gets the squad that the soldier is attached to
			formation.unitData.at(soldier.iIndex).x = x.x;
			formation.unitData.at(soldier.iIndex).y = y.y;
		}
	}

	//updates the formation rect from the positions of the soldiers assigned to it
	void Update_Soldier_Formation_Rect(entt::registry& zone) {
		auto formation_view = zone.view<Test::Unit_Formation_Data, Test::Soldiers_Assigned_List>();

		for (auto formation_ID : formation_view) { //update colliders
			auto& formation = formation_view.get<Test::Unit_Formation_Data>(formation_ID);
			auto& soldierList = formation_view.get<Test::Soldiers_Assigned_List>(formation_ID);
			float MaxRadius = 0.0f;
			
			//formation.sCollide_Box.x = soldierList.unitData.at(0).x;
			//formation.sCollide_Box.y = soldierList.unitData.at(0).y;

			for (int j = 0; j < soldierList.unitData.size(); j++) {
				if (soldierList.unitData.at(j).radius > MaxRadius) {
					MaxRadius = soldierList.unitData.at(j).radius;
				}
			}
			
			for (int i = 0; i < soldierList.unitData.size(); i++) {
				if (soldierList.unitData.at(i).x > formation.sCollide_Box.x) {
					formation.sCollide_Box.x = soldierList.unitData.at(i).x;
				}
				if (soldierList.unitData.at(i).y > formation.sCollide_Box.y) {
					formation.sCollide_Box.y = soldierList.unitData.at(i).y;
				}
				if (soldierList.unitData.at(i).x < formation.sCollide_Box.x) {
					formation.sCollide_Box.w = (soldierList.unitData.at(i).x + (MaxRadius * 2.0f));
				}
				if (soldierList.unitData.at(i).y < formation.sCollide_Box.y) {
					formation.sCollide_Box.h = (soldierList.unitData.at(i).y + (MaxRadius * 2.0f));
				}
			}
		}
	}


	//updates the formation rect from the rects of its subformations
	void Update_Formation_Rect(entt::registry& zone) {
		auto view = zone.view<Test::Unit_Formation_Data>(entt::exclude<Test::Soldiers_Assigned_List>);
		for (auto formation_ID : view) { //update colliders
			auto& formation = view.get<Test::Unit_Formation_Data>(formation_ID);
			auto& subformation = view.get<Test::Unit_Formation_Data>(formation_ID).subformationData;
			
	//		formation.sCollide_Box = subformation.at(0).sCollide_Box;

			if (!subformation.empty()) { //will crash if vector is empty
				for (int i = 0; i < subformation.size(); i++) {
					if (subformation.at(i).sCollide_Box.x < formation.sCollide_Box.x) {
						formation.sCollide_Box.x = subformation.at(i).sCollide_Box.x;
					}
					if (subformation.at(i).sCollide_Box.y < formation.sCollide_Box.y) {
						formation.sCollide_Box.y = subformation.at(i).sCollide_Box.y;
					}
					if (subformation.at(i).sCollide_Box.w < formation.sCollide_Box.x) {
						formation.sCollide_Box.w = subformation.at(i).sCollide_Box.x;
					}
					if (subformation.at(i).sCollide_Box.h < formation.sCollide_Box.y) {
						formation.sCollide_Box.h = subformation.at(i).sCollide_Box.y;
					}
				}
			}
			
		}
	}

	void Run_Position(entt::registry& zone) {
		Update_Unit_Positions(zone);
		Update_Formation_Unit_Positions(zone);
		Update_Soldier_Formation_Rect(zone);
		Update_Formation_Rect(zone);
	}

}