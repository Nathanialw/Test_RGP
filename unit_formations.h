#pragma once
#include "entt.hpp"
#include "components.h"


namespace Test_Units {

	// "squad" type unit_formation has a Test::Soldiers component to hold the unit data
	// All other units do not have this component

	entt::entity Create_New_Squad(entt::registry& zone) {
		auto squad = zone.create();
		auto& Soldiers = zone.emplace<Test::Soldiers_Assigned>(squad);
		auto& Unit = zone.emplace<Test::Unit_Formation>(squad);
		return squad;
	}
	entt::entity Create_New_Formation(entt::registry& zone) {
		auto formation = zone.create();
		auto& Unit = zone.emplace<Test::Unit_Formation>(formation);
		return formation;
	}




	bool Replace_Umit_In_Formation(entt::registry& zone, entt::entity& platoon_ID, entt::entity squad_ID, Test::Unit_Formation soldierData) {

		Test::Unit_Formation& platoon = zone.get<Test::Unit_Formation>(squad_ID);
		
		for (int i = 0; i < platoon.formationData.size(); i++) {
			if (platoon.formationData.at(i).bAlive == false) {
				auto& squad = zone.emplace_or_replace<Components::Assigned_To>(squad_ID, i, squad_ID);
				platoon.formationData.at(i) = soldierData;
				return true;
			}
		}
		return false;
	}






	bool Emplace_Umit_In_Formation(entt::registry& zone, entt::entity& platoon_ID, entt::entity squad_ID, Test::Unit_Formation soldierData) {
		Test::Unit_Formation& platoon = zone.get<Test::Unit_Formation>(platoon_ID);

		if (platoon.formationData.size() < platoon.size) {
			auto& squad = zone.emplace_or_replace<Components::Assigned_To>(squad_ID, 0, platoon_ID);
			squad.iIndex = platoon.formationData.size() - 1;
			platoon.formationData.emplace_back(soldierData);
			return true;
		}
		return false;
	}






	bool Assign_Selected_Squads_To_Formation(entt::registry& zone, entt::entity &squad_ID, Test::Unit_Formation &formationData) {
		
		auto view = zone.view<Test::Unit_Formation>(); // get the selected formations => check which type they are => add them to the parent formation type

		for (auto platoon_ID : view) {
			Test::Unit_Formation& unit_formation = zone.get<Test::Unit_Formation>(platoon_ID);
			int a = (int)unit_formation.formationType - 1;
			if (a == (formationData.formationType)) {
				if (Emplace_Umit_In_Formation(zone, platoon_ID, squad_ID, formationData)) {
					return true;
				}
			}
		}
		entt::entity new_platoon_ID = Create_New_Formation(zone);
		Test::Unit_Formation& unit_formation = zone.get<Test::Unit_Formation>(new_platoon_ID);		
		auto i = (int)formationData.formationType + 1;
		unit_formation.formationType = zone.emplace<Test::Formation_Type>(new_platoon_ID, Test::Formation_Type(i));
		Emplace_Umit_In_Formation(zone, new_platoon_ID, squad_ID, formationData);
	};






	void Create_Platoon(entt::registry& zone) {
		auto squads_view = zone.view<Test::Unit_Formation>();
		for (auto squad_ID : squads_view) {
			auto &squadFormationData = squads_view.get<Test::Unit_Formation>(squad_ID);
			Assign_Selected_Squads_To_Formation(zone, squad_ID, squadFormationData);
		}
	}













	bool Replace_Umit_In_Squad(entt::registry& zone, entt::entity& squad_ID, Test::Soldier_Data soldierData) {

		Test::Soldiers_Assigned& squad = zone.get<Test::Soldiers_Assigned>(squad_ID);

		for (int i = 0; i < squad.unitData.size(); i++) {
			if (squad.unitData.at(i).bAlive == false) {				
				auto& soldier = zone.emplace_or_replace<Components::Assigned_To>(soldierData.unit_ID, i, squad_ID);

				squad.unitData.at(i) = soldierData;			

				return true;
			}
		}
		return false;
	}
	bool Emplace_Umit_In_Squad(entt::registry& zone, entt::entity& squad_ID, Test::Soldier_Data soldierData) {
		Test::Soldiers_Assigned& squad = zone.get<Test::Soldiers_Assigned>(squad_ID);

		if (squad.unitData.size() < squad.size) {
			auto& soldier = zone.emplace_or_replace<Components::Assigned_To>(soldierData.unit_ID, 0, squad_ID);
			soldier.iIndex = squad.unitData.size() - 1;

			squad.unitData.emplace_back(soldierData);

			return true;
		}
		return false;
	}
	bool Assign_Selected_Units_To_Squad(entt::registry& zone, Test::Soldier_Data soldierData) {

		auto view = zone.view<Test::Soldiers_Assigned>();

		for (auto squad_ID : view) {
			if (Replace_Umit_In_Squad(zone, squad_ID, soldierData) == true) {
				return true;
			}
		}
		for (auto squad_ID : view) {
			if (Emplace_Umit_In_Squad(zone, squad_ID, soldierData) == true) {
				return true;
			}
		}
		entt::entity new_squad_ID = Create_New_Squad(zone);
		auto &formationType = zone.get<Test::Unit_Formation>(new_squad_ID).formationType;
		formationType = zone.emplace<Test::Formation_Type>(new_squad_ID, Test::squad);
		Emplace_Umit_In_Squad(zone, new_squad_ID, soldierData);
		return true;
	}
	void Create_And_Fill_New_Squad(entt::registry& zone) {

		auto view = zone.view<Components::Potential_Position, Components::Radius, Components::Mass, Components::Soldier, Components::Selected>(entt::exclude<Components::Assigned_To>);
		
		for (auto unit_ID : view) {
			auto& x = view.get<Components::Potential_Position>(unit_ID).fPX;
			auto& y = view.get<Components::Potential_Position>(unit_ID).fPY;
			auto& mass = view.get<Components::Mass>(unit_ID).fKilos;
			auto& radius = view.get<Components::Radius>(unit_ID).fRadius;
			Test::Soldier_Data soldierData = { unit_ID, x, y, mass, radius, 0, true};

			Assign_Selected_Units_To_Squad(zone, soldierData);
		}
	};

	bool Assign_All_Units_To_Squad(entt::registry& zone, entt::entity& squad_ID) {
		Test::Soldiers_Assigned& squad = zone.get<Test::Soldiers_Assigned>(squad_ID);
		int iUnit = 0;

		auto view = zone.view<Components::Potential_Position, Components::Radius, Components::Mass, Components::Soldier>(entt::exclude<Components::Assigned_To>);
		for (auto entity : view) {
			auto& x = view.get<Components::Potential_Position>(entity).fPX;
			auto& y = view.get<Components::Potential_Position>(entity).fPY;
			auto& m = view.get<Components::Mass>(entity).fKilos;
			auto& r = view.get<Components::Radius>(entity).fRadius;
			
			Test::Soldier_Data soldier_data = {entity, x, y, m, r, 0, true};
			squad.unitData.emplace_back(soldier_data);
			
			auto& soldier = zone.emplace_or_replace<Components::Assigned_To>(entity, 0, squad_ID);
			soldier.iIndex = squad.unitData.size() - 1;

			iUnit++;
			if (iUnit >= squad.size) {
				return true;
			}
		}
		return false;
	}
	void Create_Squads_From_All_Unassigned(entt::registry& zone) {
		entt::entity squad_ID;
		bool units = true;

		while (units) {
			squad_ID = Create_New_Squad(zone);
			units = Assign_All_Units_To_Squad(zone, squad_ID);;
		}
	}

	void Assign_Soldiers_On_Spawn(entt::registry& zone) {
		Create_Squads_From_All_Unassigned(zone);
		//Create_Platoons(zone);
		//Create_Companies(zone);
	}

}