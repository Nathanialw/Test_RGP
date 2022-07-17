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
		auto platoon_view = Scenes::scene.view<Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Platoon>(platoons);
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				Scenes::scene.emplace_or_replace<Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			Scenes::scene.clear<Selected>();
		}
		return bSelected;
	}

	bool Select_Company() {
		bool bSelected = false;
		auto platoon_view = Scenes::scene.view<Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Platoon>(platoons);
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				Scenes::scene.emplace_or_replace<Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			Scenes::scene.clear<Selected>();
		}
		return bSelected;
	}

	bool Select_Platoon() {
		bool bSelected = false;
		auto platoon_view = Scenes::scene.view<Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Platoon>(platoons);		
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				Scenes::scene.emplace_or_replace<Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			Scenes::scene.clear<Selected>();
		}
		return bSelected;
	}


	bool Select_Squad() {
		bool bSelected = false;
		auto squad_view = Scenes::scene.view<Squad>();
		for (auto squads : squad_view) {
			auto& squad = squad_view.get<Squad>(squads);
			if (Mouse::Mouse_Selection_Box(squad.sCollide_Box)) { 
				Scenes::scene.emplace_or_replace<Selected>(squads);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			Scenes::scene.clear<Selected>();
		}
		return bSelected;
	}



	bool Select_Soldier() {
		bool bSelected = false;
		auto soldier_view = Scenes::scene.view<Potential_Position, Radius, Soldier, Commandable>();
		for (auto soldiers : soldier_view) {
			auto& x = soldier_view.get<Potential_Position>(soldiers);
			auto& y = soldier_view.get<Potential_Position>(soldiers);
			auto& r = soldier_view.get<Radius>(soldiers);
			if (Mouse::Mouse_Selection_Box({ x.fPX - r.fRadius, y.fPY - r.fRadius, r.fRadius * 2, r.fRadius * 2 })) {
				Scenes::scene.emplace_or_replace<Selected>(soldiers);
				bSelected = true;					
			}
		}
		if (bSelected == false) {
			Scenes::scene.clear<Selected>();
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
		if (Scenes::scene.empty<Selected>()) {
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
			Scenes::scene.clear<Selected>();
			Mouse::bLeft_Mouse_Pressed = false;
		}

	}

	void Order_Move() {
		auto view = Scenes::scene.view<Selected, Commandable, Soldier>();
		for (auto entity : view) {
			Scenes::scene.emplace_or_replace<Mouse_Move>(entity, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
			Scenes::scene.emplace_or_replace<Moving>(entity);
		}
	}

	void Command_Unit() {
		if (!Scenes::scene.empty<Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = Scenes::scene.view<Selected, Soldier, Commandable>();
				float x = 0;
				float y = Mouse::Mouse_Selection_Box_y;
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

					Scenes::scene.emplace_or_replace<Moving>(entity);
					Scenes::scene.emplace_or_replace<Mouse_Move>(entity, x, y);
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				Order_Move();
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}


	void Command_Squad() {
		if (!Scenes::scene.empty<Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = Scenes::scene.view<Selected, Squad>();
				float x = 0;
				float y = Mouse::Mouse_Selection_Box_y;
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

						Scenes::scene.emplace_or_replace<Moving>(squad.iSub_Units[j]);
						Scenes::scene.emplace_or_replace<Mouse_Move>(squad.iSub_Units[j], x, y);
					}
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				auto squads_view = Scenes::scene.view<Selected, Squad>();
				for (auto squads : squads_view) {
					auto& squad = Scenes::scene.get<Squad>(squads);
					for (int j = 0; j < squad.iSub_Units.size(); j++) {
						Scenes::scene.emplace_or_replace<Moving>(squad.iSub_Units[j]);
						Scenes::scene.emplace_or_replace<Mouse_Move>(squad.iSub_Units[j], Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
					}
				}
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}

	void Command_Platoon() {
		if (!Scenes::scene.empty<Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = Scenes::scene.view<Selected, Platoon>();
				float x = 0;
				float y = Mouse::Mouse_Selection_Box_y;
				int i = 0;
				int spacing = 0;
				int z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50));// z is the # of units that can fit along x
				for (auto entity : view) {
					auto& squad = view.get<Platoon>(entity);
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

						Scenes::scene.emplace_or_replace<Moving>(squad.iSub_Units[j]);
						Scenes::scene.emplace_or_replace<Mouse_Move>(squad.iSub_Units[j], x, y);
					}
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				auto squads_view = Scenes::scene.view<Selected, Platoon>();
				for (auto squads : squads_view) {
					auto& squad = Scenes::scene.get<Platoon>(squads);
					for (int j = 0; j < squad.iSub_Units.size(); j++) {
						Scenes::scene.emplace_or_replace<Moving>(squad.iSub_Units[j]);
						Scenes::scene.emplace_or_replace<Mouse_Move>(squad.iSub_Units[j], Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
					}
				}
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}

	void Selection_Box() {
		if (Scenes::scene.empty<Selected>()) {
			Mouse::bLeft_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
		else {
			Scenes::scene.clear<Selected>();
			Mouse::bLeft_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
	}


	void Order_Unit() {
		if (!Scenes::scene.empty<Selected>()) {
			Mouse::bRight_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
	}






	//
	//
	//Unit Oraginzation
	//
	
	void Delete_Squad() {		
		auto view = Scenes::scene.view<Assigned, Selected>();
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
		auto view = Scenes::scene.view<Potential_Position, Selected, Radius, Mass, Soldier>(entt::exclude<Assigned>);
		
		int iUnit = 0;
		
		auto squad_ID = Scenes::scene.create();
		auto& squad = Scenes::scene.emplace<Squad>(squad_ID);

		for (auto entity : view) {
			if (iUnit < squad.size) { // caps number of units per squad				
				auto& x = view.get<Potential_Position>(entity);
				auto& y = view.get<Potential_Position>(entity);
				auto& m = view.get<Mass>(entity);
				auto& r = view.get<Radius>(entity);
				auto& soldier = Scenes::scene.emplace_or_replace<Assigned>(entity, 0, squad_ID);
				const auto SOLDIER_ID = entt::to_entity(Scenes::scene, soldier);
				const auto SQUAD_ID = entt::to_entity(Scenes::scene, squad);
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
		auto platoons_view = Scenes::scene.view<Platoon>(entt::exclude<Assigned>);

		for (auto view : platoons_view) {
			int iUnit = 0;
			auto Company_ID = Scenes::scene.create();
			auto& company = Scenes::scene.emplace<Company>(Company_ID);
			for (auto platoons : platoons_view) {
				if (iUnit < company.size) { // caps number of units per squad				
					auto& platoon = platoons_view.get<Platoon>(platoons);
					const auto PLATOON_ID = entt::to_entity(Scenes::scene, platoon);
					const auto COMPANY_ID = entt::to_entity(Scenes::scene, company);
					auto& assigned = Scenes::scene.emplace_or_replace<Assigned>(platoons, 0, Company_ID);
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
		auto squads_view = Scenes::scene.view<Squad>(entt::exclude<Assigned>);

		for (auto entity2 : squads_view) {
			int iUnit = 0;
			auto Platoon_ID = Scenes::scene.create();
			auto& platoon = Scenes::scene.emplace<Platoon>(Platoon_ID);
			for (auto squads : squads_view) {
				if (iUnit < platoon.size) { // caps number of units per squad				
					auto& squad = squads_view.get<Squad>(squads);
					const auto SQUAD_ID = entt::to_entity(Scenes::scene, squad);
					const auto PLATOON_ID = entt::to_entity(Scenes::scene, platoon);
					auto& assigned = Scenes::scene.emplace_or_replace<Assigned>(squads, 0, PLATOON_ID);
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
		auto view = Scenes::scene.view<Potential_Position, Radius, Mass, Soldier>(entt::exclude<Assigned>);
		for (auto entity2 : view) {
			int iUnit = 0;
			auto squad_ID = Scenes::scene.create();

			//currently creates a new one every time instead of searching for empty spots in one that already exists
			//Needs to search current squads for an empty slot then make a new squad if needed
			auto& squad = Scenes::scene.emplace<Squad>(squad_ID);

			for (auto entity : view) {
				if (iUnit < squad.size) { // caps number of units per squad				
					auto& x = view.get<Potential_Position>(entity);
					auto& y = view.get<Potential_Position>(entity);
					auto& m = view.get<Mass>(entity);
					auto& r = view.get<Radius>(entity);
					auto& soldier = Scenes::scene.emplace_or_replace<Assigned>(entity, 0, squad_ID);
					const auto SOLDIER_ID = entt::to_entity(Scenes::scene, soldier);
					const auto SQUAD_ID = entt::to_entity(Scenes::scene, squad);
					squad.fPX.emplace_back(x.fPX);
					squad.fPY.emplace_back(y.fPY);
					squad.vPosition.emplace_back(x.fPX, y.fPY);
					squad.fMass.emplace_back(m.fKilos);
					squad.fRadius.emplace_back(r.fRadius);
					squad.iSub_Units.emplace_back(SOLDIER_ID);
					squad.iStruck.emplace_back(0);
					squad.bAlive.emplace_back(true);
					soldier.iIndex = squad.iSub_Units.size() - 1;
					soldier.iUnit_Assigned_To = SQUAD_ID;
					iUnit++;
					//std::cout << "4" << std::endl;
				}
			}
		}
	};

	void Assign_Soldiers_On_Spawn() {
		Create_Squads(); 
		Create_Platoons(); 
		Create_Companies();
	}


}