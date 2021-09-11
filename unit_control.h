#pragma once
#include "scene.h"
#include "utilities.h"
#include "mouse_control.h"


using namespace Scene;

namespace User_Mouse_Input {

	void Select_Unit() {
		auto view = scene.view<Position_X, Position_Y, Commandable>();
		auto view2 = scene.view<Commanding>();
		for (auto commander : view2) {
			auto& comm = view2.get<Commanding>(commander);
			comm.bSelected = false;
			for (auto entity : view) {
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				if (Utilities::checkIfInside({ Mouse::Mouse_Selection_Box_x, Mouse::Mouse_Selection_Box_y, Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x, Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y }, { int(x.fPX), int(y.fPY), 15, 15 }) == true) { //input w, h into this function on release	
					scene.emplace_or_replace<Selected>(entity);
					comm.bSelected = true;
				}
				if (Mouse::Select_Unit_With_Mouse(Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse, x.fX, y.fY-30, 15.0f) == true) { //input w, h into this function on release
					scene.clear<Selected>();
					scene.emplace_or_replace<Selected>(entity);
					comm.bSelected = true;
				}
			}
			if (comm.bSelected == false) {
				scene.clear<Selected>();
			}
		}
		Mouse::bPressed = false;
	}

	void Command_Unit() {

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
				scene.emplace_or_replace<Commanded_Move>(entity);
				auto& mov = scene.get<Commanded_Move>(entity);

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
				scene.emplace_or_replace<Commanded_Move>(entity);
				auto& mov = scene.get<Commanded_Move>(entity);

				mov.fX_Destination = x.fX + (Mouse::iXWorld_Mouse - x.fX);
				mov.fY_Destination = y.fY + (Mouse::iYWorld_Mouse - y.fY);
			}
		}
		Mouse::bPressed = false;		
	}



	void Selection_Box() {
		Mouse::bPressed = true;
		Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
		Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
		Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
		Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
	}


}