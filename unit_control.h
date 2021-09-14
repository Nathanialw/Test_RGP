#pragma once
#include "scene.h"
#include "utilities.h"
#include "mouse_control.h"


using namespace Scene;

namespace User_Mouse_Input {


	bool Select_Squad2() {
		bool test = false;
		auto view3 = scene.view<Soldier, Commandable>();
		auto view2 = scene.view<Commanding>();
		for (auto commander : view2) {
			auto& comm = view2.get<Commanding>(commander);
			comm.bSelected = false;
			for (auto soldier : view3) {
				auto& sold = view3.get<Soldier>(soldier);
				if (sold.SquadAssgnedTo == &Squads[0]) {
					test = true;
					scene.emplace_or_replace<Selected>(soldier);
					comm.bSelected = true;
				}
			}
			if (comm.bSelected == false) {
				scene.clear<Selected>();
			}
		}
		return test;
	}

	bool Select_Squad() {
		bool test = false;
		auto view = scene.view<Position_X, Position_Y, Commandable, Soldier>();
		auto view2 = scene.view<Commanding>();
		for (auto commander : view2) {
			auto& comm = view2.get<Commanding>(commander);
			comm.bSelected = false;
			for (auto entity : view) {
				auto& sold = view.get<Soldier>(entity);
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				if (Utilities::Inside_Mousebox({ Mouse::Mouse_Selection_Box_x, Mouse::Mouse_Selection_Box_y, Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x, Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y }, { int(x.fPX), int(y.fPY), 15, 15 }) == true) { //input w, h into this function on release	
					for (int i = 0; i < Squads.size(); i++) {
						if (sold.SquadAssgnedTo == &Squads[i]) {
							auto view3 = scene.view<Soldier>();
							for (auto soldier : view3) {
							auto& sold2 = view.get<Soldier>(soldier);
								if (sold.SquadAssgnedTo == sold2.SquadAssgnedTo) {
									scene.emplace_or_replace<Selected>(soldier);
									comm.bSelected = true;
									test = true;
								}
							}						
						}
					}
				}
				//if (Mouse::Select_Unit_With_Mouse(Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, x.fX, y.fY, 5.0f) == true) { //input w, h into this function on release
				//	scene.clear<Selected>();
				//	scene.emplace_or_replace<Selected>(entity);
				//	comm.bSelected = true;
				//}
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
				if (Utilities::Inside_Mousebox({ Mouse::Mouse_Selection_Box_x, Mouse::Mouse_Selection_Box_y, Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x, Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y }, { int(x.fPX), int(y.fPY), 15, 15 }) == true) { //input w, h into this function on release	
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
			if (sold.SquadAssgnedTo->units[sold.index] == false) {
				scene.remove<Soldier>(entity);
			};
			for (int i = 0; i < Squads.size(); i++) {
				if (sold.SquadAssgnedTo == &Squads[i]) {
			
				}
			}
			//sold.SquadAssgnedTo->x.clear();
			//sold.SquadAssgnedTo->y.clear();		
		}
	};

	void Add_Soldiers_To_Squad() {	
	
		auto view = scene.view<Position_X, Position_Y, Selected>(entt::exclude<Soldier >);
		int max_Units = 0;
		Squad new_Squad;		
		Squads.emplace_back(new_Squad);
		for (auto entity : view) {
			if (max_Units < 12) { // caps number of units per squad
				auto& sold = scene.emplace_or_replace<Soldier>(entity, true, 0, &Squads.back());
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				sold.bActive = true;
				sold.SquadAssgnedTo->x.emplace_back(x.fX);
				sold.SquadAssgnedTo->y.emplace_back(y.fY);
				sold.index = sold.SquadAssgnedTo->x.size() - 1;
				sold.SquadAssgnedTo->units.emplace_back(sold.bActive);
				max_Units++;
			}
		}
	};

	void Create_Squad() {
		Delete_Squad();
		Add_Soldiers_To_Squad();
	};





	void Get_Squad_Collider(Squad sq) {
		if (!sq.x.empty()) { //will crash if vector is empty
			float xMax = *std::max_element(sq.x.begin(), sq.x.end());
			float xMin = *std::min_element(sq.x.begin(), sq.x.end());
			float yMax = *std::max_element(sq.y.begin(), sq.y.end());
			float yMin = *std::min_element(sq.y.begin(), sq.y.end());

		sq.collide_Box = { xMin, yMin, (xMax - xMin), (yMax - yMin) };
		}
	}






}