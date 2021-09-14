#pragma once
#include "scene.h"
#include "utilities.h"
#include "mouse_control.h"


using namespace Scene;

namespace User_Mouse_Input {

	bool Select_Squad() {
		bool test = false;
		auto view = scene.view<Position_X, Position_Y, Commandable, Soldier>();
		auto view2 = scene.view<Commanding>();
		auto view4 = scene.view<Squad>();
		for (auto commander : view2) {
			auto& comm = view2.get<Commanding>(commander);
			comm.bSelected = false;
			for (auto entity : view) {
				auto& soldier = view.get<Soldier>(entity);
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				if (Utilities::Inside_Mousebox({ float(Mouse::Mouse_Selection_Box_x), float(Mouse::Mouse_Selection_Box_y), float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x), float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y) }, { x.fPX, y.fPY, 15.0f, 15.0f }) == true) { //input w, h into this function on release	
					auto view4 = scene.view<Squad>();
					for (auto squads : view4) {
						auto& squad = view4.get<Squad>(squads);
						auto SOLDIER_ID = soldier.SquadAssgnedTo;
						const auto& SQUAD_ID = entt::to_entity(scene, squad);
						if (SQUAD_ID == SOLDIER_ID) {
							auto view3 = scene.view<Soldier>();
							for (auto Squad_Soldiers : view3) {
								auto& soldier2 = view.get<Soldier>(Squad_Soldiers);
								if (soldier2.SquadAssgnedTo == SQUAD_ID) {
									scene.emplace_or_replace<Selected>(Squad_Soldiers);
									comm.bSelected = true;
									test = true;									
								}
							}						
						}
					}
					
					//if (Mouse::Inside_Cursor(Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, x.fX, y.fY, 5.0f) == true) { //input w, h into this function on release
					//	scene.clear<Selected>();
					//	scene.emplace_or_replace<Selected>(entity);
					//	comm.bSelected = true;
					// 
					//}
				}
			}
			if (comm.bSelected == false) {
				scene.clear<Selected>();
			}
		}
		return test;
	}


	void Select_Unit() {
		auto view = scene.view<Position_X, Position_Y, Commandable>();
		auto view2 = scene.view<Commanding>();
		for (auto commander : view2) {
			auto& comm = view2.get<Commanding>(commander);
			comm.bSelected = false;
			for (auto entity : view) {
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				if (Utilities::Inside_Mousebox({ float(Mouse::Mouse_Selection_Box_x), float(Mouse::Mouse_Selection_Box_y), float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x), float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y) }, { x.fPX, y.fPY, 15.0f, 15.0f }) == true) { //input w, h into this function on release	
					scene.emplace_or_replace<Selected>(entity);
					comm.bSelected = true;
				}
				if (Mouse::Inside_Cursor(Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, x.fX, y.fY, 5.0f) == true) { //input w, h into this function on release
					scene.clear<Selected>();
					scene.emplace_or_replace<Selected>(entity);
					comm.bSelected = true;
				}
			}
			if (comm.bSelected == false) {
				scene.clear<Selected>();
			}
		}
	}


	void Select() {
		//if unit.Soldier then select all in his squad
		if (scene.empty<Selected>()) {	
			if (!Select_Squad()) {  //try this first
				Select_Unit();	// then try this
			}
			Mouse::bLeft_Mouse_Pressed = false;
		}
		else {
			scene.clear<Selected>();
			Mouse::bLeft_Mouse_Pressed = false;
		}

	}

	void Command_Unit() {
		//squad[0] 
		
	

		if (!scene.empty<Selected>()) {
			//scene.sort<Selected, Position_X>(); //sorts position least to	
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = scene.view<Selected>();
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
					
					scene.emplace_or_replace<Velocity>(entity);
					auto& mov = scene.emplace_or_replace<Commanded_Move>(entity);					
					
					mov.fX_Destination = x;
					mov.fY_Destination = y;
					
				
				}
			}
			else {
				auto view = scene.view<Selected, Position_X, Position_Y>();
				for (auto entity : view) {
					auto& x = scene.get<Position_X>(entity);
					auto& y = scene.get<Position_Y>(entity);
					///take the XPos and Ypos and add the WorldMouse X and World MouseY
					scene.emplace_or_replace<Velocity>(entity);
					auto& mov = scene.emplace_or_replace<Commanded_Move>(entity);
					mov.fX_Destination = x.fX + (Mouse::iXWorld_Mouse - x.fX);
					mov.fY_Destination = y.fY + (Mouse::iYWorld_Mouse - y.fY);
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
		auto view = scene.view<Soldier, Selected>();
		for (auto entity : view) {
			auto& sold = view.get<Soldier>(entity);
			
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
		auto view = scene.view<Position_X, Position_Y, Selected, Collision_Radius, Mass>(entt::exclude<Soldier>);
		
		int iUnit = 0;
		
		auto squad_ID = scene.create();
		auto& squad = scene.emplace<Squad>(squad_ID);


		for (auto entity : view) {
			if (iUnit < squad.size) { // caps number of units per squad				
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				auto& m = view.get<Mass>(entity);
				auto& r = view.get<Collision_Radius>(entity);
				auto& soldier = scene.emplace_or_replace<Soldier>(entity, 0, squad_ID);
				const auto SOLDIER_ID = entt::to_entity(scene, soldier);
				const auto SQUAD_ID = entt::to_entity(scene, squad);
				squad.fPX.emplace_back(x.fPX);
				squad.fPY.emplace_back(y.fPY);
				squad.fMass.emplace_back(m.fKilos);
				squad.fRadius.emplace_back(r.fCollisionRadius);
				squad.sSub_Units.emplace_back(SOLDIER_ID);
				soldier.iIndex = squad.sSub_Units.size() - 1;
				soldier.SquadAssgnedTo = SQUAD_ID;
				iUnit++;
			}
		}
	};


	void Create_Mass_Squads() {
		auto view = scene.view<Position_X, Position_Y, Collision_Radius, Mass, Selected>(entt::exclude<Soldier>);

		for (auto entity2 : view) {

			int iUnit = 0;

			auto squad_ID = scene.create();
			auto& squad = scene.emplace<Squad>(squad_ID);


			for (auto entity : view) {
				if (iUnit < squad.size) { // caps number of units per squad				
					auto& x = view.get<Position_X>(entity);
					auto& y = view.get<Position_Y>(entity);
					auto& m = view.get<Mass>(entity);
					auto& r = view.get<Collision_Radius>(entity);
					auto& soldier = scene.emplace_or_replace<Soldier>(entity, 0, squad_ID);
					const auto SOLDIER_ID = entt::to_entity(scene, soldier);
					const auto SQUAD_ID = entt::to_entity(scene, squad);
					squad.fPX.emplace_back(x.fPX);
					squad.fPY.emplace_back(y.fPY);
					squad.fMass.emplace_back(m.fKilos);
					squad.fRadius.emplace_back(r.fCollisionRadius);
					squad.sSub_Units.emplace_back(SOLDIER_ID);
					soldier.iIndex = squad.sSub_Units.size() - 1;
					soldier.SquadAssgnedTo = SQUAD_ID;
					iUnit++;
				}
			}
		}
	};





	void Create_Squad() {
		Delete_Squad();
		Add_Soldiers_To_Squad();
	};




}