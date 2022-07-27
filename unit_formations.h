#pragma once
#include "entt.hpp"
#include "components.h"


namespace Test_Units {

	// "squad" type unit_formation has a Test::Soldiers component to hold the unit data
	// All other units do not have this component

	entt::entity Create_New_Squad(entt::registry& zone) {
		auto squad = zone.create();
		auto& Soldiers = zone.emplace<Test::Soldiers_Assigned_List>(squad);
		auto& Unit = zone.emplace<Test::Unit_Formation_Data>(squad);
		zone.get<Test::Unit_Formation_Data>(squad).formationType = Test::squad;
		return squad;
	}
	entt::entity Create_New_Formation(entt::registry& zone) {
		auto formation = zone.create();
		auto& Unit = zone.emplace<Test::Unit_Formation_Data>(formation);
		return formation;
	}

	bool Is_ParentFormation_One_Level_Above(Test::Unit_Formation_Data &parentFormationData, Test::Unit_Formation_Data& subformationData) {
		int subformationType = (int)parentFormationData.formationType - 1;
		if (subformationType == (subformationData.formationType)) {
			return true;
		}
		return false;
	}
	Test::Unit_Formation_Data Set_ParentFormation_Formation_Type(entt::registry &zone, entt::entity &parentFormation_ID, Test::Unit_Formation_Data& subformationData) {
		int parentFormationType_int = (int)subformationData.formationType + 1;
		Test::Unit_Formation_Data& parentFormationData = zone.get<Test::Unit_Formation_Data>(parentFormation_ID);
		parentFormationData.formationType = (Test::Formation_Type)parentFormationType_int;
		return parentFormationData;
	}

	bool Replace_Subformation_In_Formation(entt::registry& zone, entt::entity& parentFormation_ID, Test::Unit_Formation_Data& parentFormationData, entt::entity& subformation_ID, Test::Unit_Formation_Data & subformationData) {

		//iterate though all the subformations of a formation
		for (int i = 0; i < parentFormationData.subformationData.size(); i++) {
			//check for an index without an active subformation
			if (parentFormationData.subformationData.at(i).bAlive == false) {
				zone.emplace_or_replace<Component::Assigned_To>(subformation_ID, i, parentFormation_ID);
				
				parentFormationData.subformationData.at(i) = subformationData;
				return true;
			}
		}
		return false;
	}
	bool Emplace_Subformation_In_Formation(entt::registry& zone, entt::entity& parentFormation_ID, Test::Unit_Formation_Data& parentFormationData, entt::entity& subformation_ID, Test::Unit_Formation_Data& subformationData) {
		if (parentFormationData.subformationData.size() < parentFormationData.size) {
			auto& subformation = zone.emplace_or_replace<Component::Assigned_To>(subformation_ID, 0, parentFormation_ID);
			subformation.iIndex = parentFormationData.subformationData.size() - 1;
			
			parentFormationData.subformationData.emplace_back(subformationData);
			return true;
		}
		return false;
	}

	bool Assign_Selected_Formations_To_ParentFormations(entt::registry& zone, entt::entity & subformation_ID, Test::Unit_Formation_Data &subformationData) {
		
		// get the selected formations => check which type they are => add them to the parent formation type
		
		auto parentFormations_view = zone.view<Test::Unit_Formation_Data>(); 

		for (auto parentFormation_ID : parentFormations_view) {
			Test::Unit_Formation_Data& parentFormationData = zone.get<Test::Unit_Formation_Data>(parentFormation_ID);
			if (Is_ParentFormation_One_Level_Above(parentFormationData, subformationData)) {
				if (Replace_Subformation_In_Formation(zone, parentFormation_ID, parentFormationData, subformation_ID, subformationData)) {
					return true;
				}
			}
		}
		//search exist formations for space and insert the subformation data
		for (auto parentFormation_ID : parentFormations_view) {
			Test::Unit_Formation_Data& parentFormationData = zone.get<Test::Unit_Formation_Data>(parentFormation_ID);
			if (Is_ParentFormation_One_Level_Above(parentFormationData, subformationData)) {
				if (Emplace_Subformation_In_Formation(zone, parentFormation_ID, parentFormationData, subformation_ID, subformationData)) {
					return true;
				}
			}
		}

		//create a new formation and insert the subformation data
		entt::entity new_parentFormation_ID = Create_New_Formation(zone);
		//set the formation type one level above the subformation being inserted
		Test::Unit_Formation_Data parentFormationData = Set_ParentFormation_Formation_Type(zone, new_parentFormation_ID, subformationData);
		//assign subformation to parent formation
		Emplace_Subformation_In_Formation(zone, new_parentFormation_ID, parentFormationData, subformation_ID, subformationData);
	};

	void Create_Formation(entt::registry& zone) {
		auto subformations_view = zone.view<Test::Unit_Formation_Data>(); // this should be only the selected formations I think
		for (auto subformation_ID : subformations_view) {
			auto & subformationData = subformations_view.get<Test::Unit_Formation_Data>(subformation_ID);
			Assign_Selected_Formations_To_ParentFormations(zone, subformation_ID, subformationData);
		}
	}


	bool Replace_Umit_In_Squad(entt::registry& zone, entt::entity& squad_ID, Test::Soldier_Data &soldierData) {

		Test::Soldiers_Assigned_List& squad = zone.get<Test::Soldiers_Assigned_List>(squad_ID);

		for (int i = 0; i < squad.unitData.size(); i++) {
			if (squad.unitData.at(i).bAlive == false) {				
				auto& soldier = zone.emplace_or_replace<Component::Assigned_To>(soldierData.unit_ID, i, squad_ID);
				squad.unitData.at(i) = soldierData;			
				return true;
			}
		}
		return false;
	}

	bool Emplace_Umit_In_Squad(entt::registry& zone, entt::entity& squad_ID, Test::Soldier_Data &soldierData) {
		Test::Soldiers_Assigned_List& squad = zone.get<Test::Soldiers_Assigned_List>(squad_ID);

		if (squad.unitData.size() < squad.size) {
			auto& soldier = zone.emplace_or_replace<Component::Assigned_To>(soldierData.unit_ID, 0, squad_ID);
			soldier.iIndex = squad.unitData.size() - 1;
			squad.unitData.emplace_back(soldierData);
			return true;
		}
		return false;
	}

	bool Assign_Selected_Units_To_Squad(entt::registry& zone, Test::Soldier_Data &soldierData) {

		auto view = zone.view<Test::Soldiers_Assigned_List>();

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
		Emplace_Umit_In_Squad(zone, new_squad_ID, soldierData);
		return true;
	}


	void Create_And_Fill_New_Squad(entt::registry& zone) {

		auto view = zone.view<Component::Position, Component::Radius, Component::Mass, Component::Soldier, Component::Selected>(entt::exclude<Component::Assigned_To>);
		
		for (auto unit_ID : view) {
			auto& x = view.get<Component::Position>(unit_ID).x;
			auto& y = view.get<Component::Position>(unit_ID).y;
			auto& mass = view.get<Component::Mass>(unit_ID).fKilos;
			auto& radius = view.get<Component::Radius>(unit_ID).fRadius;
			Test::Soldier_Data soldierData = { unit_ID, x, y, mass, radius, 0, true};

			Assign_Selected_Units_To_Squad(zone, soldierData);
		}
	};

	bool Assign_All_Units_To_Squad(entt::registry& zone, entt::entity& squad_ID) {
		Test::Soldiers_Assigned_List& squad = zone.get<Test::Soldiers_Assigned_List>(squad_ID);
		int iUnit = 0;

		auto view = zone.view<Component::Position, Component::Radius, Component::Mass, Component::Soldier>(entt::exclude<Component::Assigned_To>);
		for (auto entity : view) {
			auto& x = view.get<Component::Position>(entity).x;
			auto& y = view.get<Component::Position>(entity).y;
			auto& m = view.get<Component::Mass>(entity).fKilos;
			auto& r = view.get<Component::Radius>(entity).fRadius;
			
			Test::Soldier_Data soldier_data = {entity, x, y, m, r, 0, true};
			squad.unitData.emplace_back(soldier_data);
			
			auto& soldier = zone.emplace_or_replace<Component::Assigned_To>(entity, 0, squad_ID);
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
	}

}