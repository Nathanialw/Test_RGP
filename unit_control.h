#pragma once
#include "scene.h"
#include "utilities.h"
#include "mouse_control.h"
#include "movement.h"


using namespace Scene;

namespace User_Mouse_Input {

	enum Unit_Selection {
		soldiers,
		squads,
		platoons,
		companies,
		battalions

	};

	Unit_Selection eUnit_Selection;

	void Selection_Soldiers () {
		eUnit_Selection = soldiers;
	}
	
	void Selection_Squads () {
		eUnit_Selection = squads;
	}
	
	void Selection_Platoons () {
		eUnit_Selection = platoons;
	}
		
	void Selection_Companies () {
		eUnit_Selection = companies;
	}
		
	void Selection_Battalions () {
		eUnit_Selection = battalions;
	}

	bool Select_Battalion() {
		bool bSelected = false;
		auto platoon_view = scene.view<Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Platoon>(platoons);
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				scene.emplace_or_replace<Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			scene.clear<Selected>();
		}
		return bSelected;
	}

	bool Select_Company() {
		bool bSelected = false;
		auto platoon_view = scene.view<Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Platoon>(platoons);
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				scene.emplace_or_replace<Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			scene.clear<Selected>();
		}
		return bSelected;
	}

	bool Select_Platoon() {
		bool bSelected = false;
		auto platoon_view = scene.view<Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Platoon>(platoons);		
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				scene.emplace_or_replace<Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			scene.clear<Selected>();
		}
		return bSelected;
	}


	bool Select_Squad() {
		bool bSelected = false;
		auto squad_view = scene.view<Squad>();
		for (auto squads : squad_view) {
			auto& squad = squad_view.get<Squad>(squads);
			if (Mouse::Mouse_Selection_Box(squad.sCollide_Box)) { 
				scene.emplace_or_replace<Selected>(squads);	
				bSelected = true;
			}
		}
		if (bSelected == false) {
			scene.clear<Selected>();
		}
		return bSelected;
	}



	bool Select_Soldier() {
		bool bSelected = false;
		auto soldier_view = scene.view<Position_X, Position_Y, Radius, Soldier>();				
		for (auto soldiers : soldier_view) {
			auto& x = soldier_view.get<Position_X>(soldiers);
			auto& y = soldier_view.get<Position_Y>(soldiers);
			auto& r = soldier_view.get<Radius>(soldiers);
			if (Mouse::Mouse_Selection_Box({ x.fPX - r.fRadius, y.fPY - r.fRadius, r.fRadius * 2, r.fRadius * 2 })) {
				scene.emplace_or_replace<Selected>(soldiers);
				bSelected = true;					
			}
		}
		if (bSelected == false) {
			scene.clear<Selected>();
		}		
		return bSelected;
	}


	void Select() {
		Select_Platoon();
		Select_Squad();
		Select_Soldier();
	}


	void Select_Units() {
		//if unit.Soldier then select all in his squad
		if (scene.empty<Selected>()) {				
			switch (eUnit_Selection) {
				case soldiers: Select_Soldier(); break;
				case squads: Select_Squad(); break;
				case platoons: Select_Platoon(); break;
				case companies: Select_Company(); break;
				case battalions: Select_Battalion(); break;
			}
			Mouse::bLeft_Mouse_Pressed = false;
		}
		else {
			scene.clear<Selected>();
			Mouse::bLeft_Mouse_Pressed = false;
		}

	}

	void Order_Move() {
		auto view = scene.view<Selected, Soldier>();
		for (auto entity : view) {
			auto& mov = scene.emplace_or_replace<Mouse_Move>(entity);
			scene.emplace_or_replace<Moving>(entity);
			scene.emplace_or_replace<Mouse_Move>(entity);
			mov.fX_Destination = Mouse::iXWorld_Mouse;
			mov.fY_Destination = Mouse::iYWorld_Mouse;
		}
	}

	void Command_Unit() {
		if (!scene.empty<Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = scene.view<Selected, Soldier>();
				int x = 0;
				int y = Mouse::Mouse_Selection_Box_y;
				int i = 0;
				int spacing = 0;
				int z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50));// z is the # of units that can fit along x
				for (auto entity : view) {
					x = Mouse::Mouse_Selection_Box_x + spacing;
					if (i == z) {
						spacing = 0;
						x = Mouse::Mouse_Selection_Box_x + spacing;
						y = y + 50;
						i = 0;
					}
					i++;
					spacing = spacing + 50; //spacing shoudl be stored in "battalion" component					

					scene.emplace_or_replace<Moving>(entity);					
					auto& mov = scene.emplace_or_replace<Mouse_Move>(entity);					
					mov.fX_Destination = x;
					mov.fY_Destination = y;
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				Order_Move();
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}


	void Command_Squad() {
		if (!scene.empty<Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = scene.view<Selected, Squad>();				
				int x = 0;
				int y = Mouse::Mouse_Selection_Box_y;
				int i = 0;
				int spacing = 0;
				int z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50));// z is the # of units that can fit along x
				for (auto entity : view) {
					auto& squad = view.get<Squad>(entity);
					for (int j = 0; j < squad.iSub_Units.size(); j++) {
						x = Mouse::Mouse_Selection_Box_x + spacing;
						if (i == z) {
							spacing = 0;
							x = Mouse::Mouse_Selection_Box_x + spacing;
							y = y + 50;
							i = 0;
						}
						i++;
						spacing = spacing + 50; //spacing shoudl be stored in "battalion" component					

						scene.emplace_or_replace<Moving>(squad.iSub_Units[j]);
						auto& mov = scene.emplace_or_replace<Mouse_Move>(squad.iSub_Units[j]);
						mov.fX_Destination = x;
						mov.fY_Destination = y;						
					}
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				auto squads_view = scene.view<Selected, Squad>();
				for (auto squads : squads_view) {
					auto& squad = scene.get<Squad>(squads);
					for (int j = 0; j < squad.iSub_Units.size(); j++) {
						scene.emplace_or_replace<Moving>(squad.iSub_Units[j]);
						auto& mov = scene.emplace_or_replace<Mouse_Move>(squad.iSub_Units[j]);
						mov.fX_Destination = Mouse::iXWorld_Mouse;
						mov.fY_Destination = Mouse::iYWorld_Mouse;
					}
				}
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}

	void Selection_Box() {
		if (scene.empty<Selected>()) {
			Mouse::bLeft_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
		else {
			scene.clear<Selected>();
			Mouse::bLeft_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
	}


	void Order_Unit() {
		if (!scene.empty<Selected>()) {
			Mouse::bRight_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
		else {
			//scene.clear<Selected>();
			//Mouse::bPressed = true;
			//Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			//Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			//Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			//Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
	}






	//
	//
	//Unit Oraginzation
	//
	
	void Delete_Squad() {		
		auto view = scene.view<Assigned, Selected>();
		for (auto entity : view) {
			auto& sold = view.get<Assigned>(entity);
			
			//auto &squad = scene.get<Squad>(sold.SquadAssgnedTo);
			
			//sold.SquadAssgnedTo
			//if (sold.SquadAssgnedTo.units[sold.index] == false) {
			//	scene.remove<Soldier>(entity);
			//};
			//for (int i = 0; i < Squads.size(); i++) {
				//if (sold.SquadAssgnedTo == Squads[i]) {
			
				//}
			//}
			//sold.SquadAssgnedTo->x.clear();
			//sold.SquadAssgnedTo->y.clear();		
		}
	};

	void Add_Soldiers_To_Squad() {			
		auto view = scene.view<Position_X, Position_Y, Selected, Radius, Mass>(entt::exclude<Assigned>);
		
		int iUnit = 0;
		
		auto squad_ID = scene.create();
		auto& squad = scene.emplace<Squad>(squad_ID);

		for (auto entity : view) {
			if (iUnit < squad.size) { // caps number of units per squad				
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				auto& m = view.get<Mass>(entity);
				auto& r = view.get<Radius>(entity);
				auto& soldier = scene.emplace_or_replace<Assigned>(entity, 0, squad_ID);
				const auto SOLDIER_ID = entt::to_entity(scene, soldier);
				const auto SQUAD_ID = entt::to_entity(scene, squad);
				squad.fPX.emplace_back(x.fPX);
				squad.fPY.emplace_back(y.fPY);
				squad.fMass.emplace_back(m.fKilos);
				squad.fRadius.emplace_back(r.fRadius);
				squad.iSub_Units.emplace_back(SOLDIER_ID);
				soldier.iIndex = squad.iSub_Units.size() - 1;
				soldier.iUnit_Assigned_To = SQUAD_ID;
				iUnit++;
			}
		}
	};

	//assign squads to Platoons

	void Create_Companies() {
		auto platoons_view = scene.view<Platoon, Selected>(entt::exclude<Assigned>);

		for (auto view : platoons_view) {
			int iUnit = 0;
			auto Company_ID = scene.create();
			auto& company = scene.emplace<Company>(Company_ID);
			for (auto platoons : platoons_view) {
				if (iUnit < company.size) { // caps number of units per squad				
					auto& platoon = platoons_view.get<Platoon>(platoons);
					const auto PLATOON_ID = entt::to_entity(scene, platoon);
					const auto COMPANY_ID = entt::to_entity(scene, company);
					auto& assigned = scene.emplace_or_replace<Assigned>(platoons, 0, Company_ID);
					//add squad ID to  to Platoon list
					company.iSub_Units.emplace_back(PLATOON_ID);	//?? add Platoon ID to squad???
					//add index to squad
					assigned.iIndex = company.iSub_Units.size() - 1;
					//add squad collider to Platoon coords 
					company.fPX.emplace_back(platoon.sCollide_Box.x);
					company.fPY.emplace_back(platoon.sCollide_Box.y);
					company.fPW.emplace_back(platoon.sCollide_Box.w);
					company.fPH.emplace_back(platoon.sCollide_Box.h);
					iUnit++;
				}
			}
		}
	};

	void Create_Platoons() {
		auto squads_view = scene.view<Squad, Selected>(entt::exclude<Assigned>);

		for (auto entity2 : squads_view) {
			int iUnit = 0;
			auto Platoon_ID = scene.create();
			auto& platoon = scene.emplace<Platoon>(Platoon_ID);
			for (auto squads : squads_view) {
				if (iUnit < platoon.size) { // caps number of units per squad				
					auto& squad = squads_view.get<Squad>(squads);
					const auto SQUAD_ID = entt::to_entity(scene, squad);
					const auto PLATOON_ID = entt::to_entity(scene, platoon);
					auto& assigned = scene.emplace_or_replace<Assigned>(squads, 0, PLATOON_ID);
					//add squad ID to  to Platoon list
					platoon.iSub_Units.emplace_back(SQUAD_ID);	//?? add Platoon ID to squad???
					//add index to squad
					assigned.iIndex = platoon.iSub_Units.size() - 1;
					//add squad collider to Platoon coords 
					platoon.fPX.emplace_back(squad.sCollide_Box.x);
					platoon.fPY.emplace_back(squad.sCollide_Box.y);
					platoon.fPW.emplace_back(squad.sCollide_Box.w);
					platoon.fPH.emplace_back(squad.sCollide_Box.h);
					iUnit++;
				}
			}
		}
	};


	void Create_Squads() {
		auto view = scene.view<Position_X, Position_Y, Radius, Mass, Selected>(entt::exclude<Assigned>);
		for (auto entity2 : view) {
			int iUnit = 0;
			auto squad_ID = scene.create();
			auto& squad = scene.emplace<Squad>(squad_ID);

			for (auto entity : view) {
				if (iUnit < squad.size) { // caps number of units per squad				
					auto& x = view.get<Position_X>(entity);
					auto& y = view.get<Position_Y>(entity);
					auto& m = view.get<Mass>(entity);
					auto& r = view.get<Radius>(entity);
					auto& soldier = scene.emplace_or_replace<Assigned>(entity, 0, squad_ID);
					const auto SOLDIER_ID = entt::to_entity(scene, soldier);
					const auto SQUAD_ID = entt::to_entity(scene, squad);
					squad.fPX.emplace_back(x.fPX);
					squad.fPY.emplace_back(y.fPY);
					squad.vPosition.emplace_back(x.fPX, y.fPY);
					squad.fMass.emplace_back(m.fKilos);
					squad.fRadius.emplace_back(r.fRadius);
					squad.iSub_Units.emplace_back(SOLDIER_ID);
					soldier.iIndex = squad.iSub_Units.size() - 1;
					soldier.iUnit_Assigned_To = SQUAD_ID;
					iUnit++;
				}
			}
		}
	};







}