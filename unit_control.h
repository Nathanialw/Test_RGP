#pragma once
#include "utilities.h"
#include "components.h"


namespace User_Mouse_Input {

	enum Unit_Selection {
		soldiers,
		squads,
		platoons,
		companies,
		battalions
	};

	Unit_Selection eUnit_Selection;


	void Order_Move(entt::registry &zone) {
		auto view = World::zone.view<Components::Selected, Components::Commandable, Components::Soldier>();
		for (auto entity : view) {
			zone.emplace_or_replace<Components::Mouse_Move>(entity, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
			zone.emplace_or_replace<Components::Moving>(entity);
		}
	}

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

	bool Select_Battalion(entt::registry &zone) {
		bool bSelected = false;
		auto platoon_view = zone.view<Components::Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Components::Platoon>(platoons);
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				zone.emplace_or_replace<Components::Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			zone.clear<Components::Selected>();
		}
		return bSelected;
	}

	bool Select_Company(entt::registry& zone) {
		bool bSelected = false;
		auto platoon_view = zone.view<Components::Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Components::Platoon>(platoons);
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				zone.emplace_or_replace<Components::Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			zone.clear<Components::Selected>();
		}
		return bSelected;
	}

	bool Select_Platoon(entt::registry& zone) {
		bool bSelected = false;
		auto platoon_view = zone.view<Components::Platoon>();
		for (auto platoons : platoon_view) {
			auto& platoon = platoon_view.get<Components::Platoon>(platoons);
			if (Mouse::Mouse_Selection_Box(platoon.sCollide_Box)) {
				zone.emplace_or_replace<Components::Selected>(platoons);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			zone.clear<Components::Selected>();
		}
		return bSelected;
	}


	bool Select_Squad(entt::registry& zone) {
		bool bSelected = false;
		auto squad_view = zone.view<Components::Squad>();
		for (auto squads : squad_view) {
			auto& squad = squad_view.get<Components::Squad>(squads);
			if (Mouse::Mouse_Selection_Box(squad.sCollide_Box)) { 
				zone.emplace_or_replace<Components::Selected>(squads);
				bSelected = true;
			}
		}
		if (bSelected == false) {
			zone.clear<Components::Selected>();
		}
		return bSelected;
	}



	bool Select_Soldier(entt::registry& zone) {
		bool bSelected = false;
		auto soldier_view = zone.view<Components::Potential_Position, Components::Radius, Components::Soldier, Components::Commandable>();
		for (auto soldiers : soldier_view) {
			auto& x = soldier_view.get<Components::Potential_Position>(soldiers);
			auto& y = soldier_view.get<Components::Potential_Position>(soldiers);
			auto& r = soldier_view.get<Components::Radius>(soldiers);
			if (Mouse::Mouse_Selection_Box({ x.fPX - r.fRadius, y.fPY - r.fRadius, r.fRadius * 2, r.fRadius * 2 })) {
				zone.emplace_or_replace<Components::Selected>(soldiers);
				bSelected = true;					
			}
		}
		if (bSelected == false) {
			zone.clear<Components::Selected>();
		}		
		return bSelected;
	}


	void Select(entt::registry& zone) {
		Select_Platoon(zone);
		Select_Squad(zone);
		Select_Soldier(zone);
	}


	void Select_Units(entt::registry& zone) {
		//if unit.Soldier then select all in his squad
		if (zone.empty<Components::Selected>()) {
			switch (eUnit_Selection) {
				case soldiers: Select_Soldier(zone); break;
				case squads: Select_Squad(zone); break;
				case platoons: Select_Platoon(zone); break;
				case companies: Select_Company(zone); break;
				case battalions: Select_Battalion(zone); break;
			}
			Mouse::bLeft_Mouse_Pressed = false;
		}
		else {
			zone.clear<Components::Selected>();
			Mouse::bLeft_Mouse_Pressed = false;
		}

	}


	void Command_Unit(entt::registry& zone) {
		if (!zone.empty<Components::Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = zone.view<Components::Selected, Components::Soldier, Components::Commandable>();
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

					zone.emplace_or_replace<Components::Moving>(entity);
					zone.emplace_or_replace<Components::Mouse_Move>(entity, x, y);
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				Order_Move(zone);
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}


	void Command_Squad(entt::registry& zone) {
		if (!zone.empty<Components::Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = zone.view<Components::Selected, Components::Squad>();
				float x = 0;
				float y = Mouse::Mouse_Selection_Box_y;
				int i = 0;
				int spacing = 0;
				int z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50));// z is the # of units that can fit along x
				for (auto entity : view) {
					auto& squad = view.get<Components::Squad>(entity);
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

						zone.emplace_or_replace<Components::Moving>(squad.iSub_Units[j]);
						zone.emplace_or_replace<Components::Mouse_Move>(squad.iSub_Units[j], x, y);
					}
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				auto squads_view = zone.view<Components::Selected, Components::Squad>();
				for (auto squads : squads_view) {
					auto& squad = World::zone.get<Components::Squad>(squads);
					for (int j = 0; j < squad.iSub_Units.size(); j++) {
						zone.emplace_or_replace<Components::Moving>(squad.iSub_Units[j]);
						zone.emplace_or_replace<Components::Mouse_Move>(squad.iSub_Units[j], Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
					}
				}
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}

	void Command_Platoon(entt::registry& zone) {
		if (!zone.empty<Components::Selected>()) {
			if (abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse)) > 50) {
				auto view = zone.view<Components::Selected, Components::Platoon>();
				float x = 0;
				float y = Mouse::Mouse_Selection_Box_y;
				int i = 0;
				int spacing = 0;
				int z = abs((Mouse::Mouse_Selection_Box_x - Mouse::iXWorld_Mouse) / (50));// z is the # of units that can fit along x
				for (auto entity : view) {
					auto& squad = view.get<Components::Platoon>(entity);
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

						zone.emplace_or_replace<Components::Moving>(squad.iSub_Units[j]);
						zone.emplace_or_replace<Components::Mouse_Move>(squad.iSub_Units[j], x, y);
					}
				}
			}
			else { //moves all the units onto a single point, I want to have the spread out in some kind of formation
				auto squads_view = zone.view<Components::Selected, Components::Platoon>();
				for (auto squads : squads_view) {
					auto& squad = World::zone.get<Components::Platoon>(squads);
					for (int j = 0; j < squad.iSub_Units.size(); j++) {
						zone.emplace_or_replace<Components::Moving>(squad.iSub_Units[j]);
						zone.emplace_or_replace<Components::Mouse_Move>(squad.iSub_Units[j], Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
					}
				}
			}
			Mouse::bRight_Mouse_Pressed = false;
		}
	}

	void Selection_Box(entt::registry& zone) {
		if (zone.empty<Components::Selected>()) {
			Mouse::bLeft_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
		else {
			zone.clear<Components::Selected>();
			Mouse::bLeft_Mouse_Pressed = true;
			Mouse::Mouse_Selection_Box_x = Mouse::iXWorld_Mouse;
			Mouse::Mouse_Selection_Box_y = Mouse::iYWorld_Mouse;
			Mouse::Mouse_Selection_Box_x_Display = Mouse::iXMouse;
			Mouse::Mouse_Selection_Box_y_Display = Mouse::iYMouse;
		}
	}


	void Order_Unit(entt::registry &zone) {
		if (!zone.empty<Components::Selected>()) {
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
	
	void Delete_Squad(entt::registry& zone) {
		auto view = zone.view<Components::Assigned, Components::Selected>();
		for (auto entity : view) {
			auto& sold = view.get<Components::Assigned>(entity);
			
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

	void Add_Soldiers_To_Squad(entt::registry& zone) {
		auto view = zone.view<Components::Potential_Position, Components::Selected, Components::Radius, Components::Mass, Components::Soldier>(entt::exclude<Components::Assigned>);
		
		int iUnit = 0;
		
		auto squad_ID = zone.create();
		auto& squad = zone.emplace<Components::Squad>(squad_ID);

		for (auto entity : view) {
			if (iUnit < squad.size) { // caps number of units per squad				
				auto& x = view.get<Components::Potential_Position>(entity);
				auto& y = view.get<Components::Potential_Position>(entity);
				auto& m = view.get<Components::Mass>(entity);
				auto& r = view.get<Components::Radius>(entity);
				auto& soldier = zone.emplace_or_replace<Components::Assigned>(entity, 0, squad_ID);
				const auto SOLDIER_ID = entt::to_entity(zone, soldier);
				const auto SQUAD_ID = entt::to_entity(zone, squad);
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

	void Create_Companies(entt::registry& zone) {
		auto platoons_view = zone.view<Components::Platoon>(entt::exclude<Components::Assigned>);

		for (auto view : platoons_view) {
			int iUnit = 0;
			auto Company_ID = zone.create();
			auto& company = zone.emplace<Components::Company>(Company_ID);
			for (auto platoons : platoons_view) {
				if (iUnit < company.size) { // caps number of units per squad				
					auto& platoon = platoons_view.get<Components::Platoon>(platoons);
					const auto PLATOON_ID = entt::to_entity(zone, platoon);
					const auto COMPANY_ID = entt::to_entity(zone, company);
					auto& assigned = zone.emplace_or_replace<Components::Assigned>(platoons, 0, Company_ID);
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

	void Create_Platoons(entt::registry& zone) {
		auto squads_view = zone.view<Components::Squad>(entt::exclude<Components::Assigned>);

		for (auto entity2 : squads_view) {
			int iUnit = 0;
			auto Platoon_ID = zone.create();
			auto& platoon = zone.emplace<Components::Platoon>(Platoon_ID);
			for (auto squads : squads_view) {
				if (iUnit < platoon.size) { // caps number of units per squad				
					auto& squad = squads_view.get<Components::Squad>(squads);
					const auto SQUAD_ID = entt::to_entity(zone, squad);
					const auto PLATOON_ID = entt::to_entity(zone, platoon);
					auto& assigned = zone.emplace_or_replace<Components::Assigned>(squads, 0, PLATOON_ID);
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

	entt::entity Create_New_Squad(entt::registry &zone){		
		auto squad_ID = zone.create();
		auto& squad = zone.emplace<Components::Squad>(squad_ID);
		return squad_ID;
	}

	bool Replace_Umit_In_Squad (entt::registry& zone, entt::entity& squad_ID, entt::entity& entity, Components::Potential_Position& potential_position, Components::Mass& mass, Components::Radius& radius) {
		
		Components::Squad& squad = zone.get<Components::Squad>(squad_ID);
		for (int i = 0; i < squad.bAlive.size(); i++) {
			if (squad.bAlive.at(i) == false) {
				auto& x = potential_position.fPX;
				auto& y = potential_position.fPY;
				auto& m = mass.fKilos;
				auto& r = radius.fRadius;

				auto& soldier = zone.emplace_or_replace<Components::Assigned>(entity, 0, squad_ID);

				squad.fPX.at(i) = x;
				squad.fPY.at(i) = y;
				squad.vPosition.at(i) = { x, y };
				squad.fMass.at(i) = m;
				squad.fRadius.at(i) = r;
				squad.iSub_Units.at(i) = entity;
				squad.iStruck.at(i) = 0;
				squad.bAlive.at(i) = true;

				soldier.iIndex = i;
				return true;
			}
		}
		return false;
	}

	bool Emplace_Umit_In_Squad (entt::registry& zone, entt::entity &squad_ID, entt::entity& entity, Components::Potential_Position &potential_position, Components::Mass &mass, Components::Radius &radius) {
		Components::Squad& squad = zone.get<Components::Squad>(squad_ID);
		
		if (squad.bAlive.size() < squad.size) {
			auto& x = potential_position.fPX;
			auto& y = potential_position.fPY;
			auto& m = mass.fKilos;
			auto& r = radius.fRadius;
			auto& soldier = zone.emplace_or_replace<Components::Assigned>(entity, 0, squad_ID);

			squad.fPX.emplace_back(x);
			squad.fPY.emplace_back(y);
			squad.vPosition.emplace_back(x, y);
			squad.fMass.emplace_back(m);
			squad.fRadius.emplace_back(r);
			squad.iSub_Units.emplace_back(entity);
			squad.iStruck.emplace_back(0);
			squad.bAlive.emplace_back(true);

			soldier.iIndex = squad.iSub_Units.size() - 1;
			return true;
		}
		return false;
	}

	bool Assign_Selected_Units_To_Squad(entt::registry& zone, entt::entity& entity, Components::Potential_Position& potential_position, Components::Mass& mass, Components::Radius& radius) {
		//looks to see if any squads have empty slots and tries to fill it
		// if that finds notihng
		//will add a slot to a squad not yet full
		// if that fails
		///creates a new  squad and adds the unit to it

		auto view = zone.view<Components::Squad>();
		for (auto squad_ID : view) {		
			if (Replace_Umit_In_Squad(zone, squad_ID, entity, potential_position, mass, radius) == true) {
				return true;
			}
		}
		for (auto squad_ID : view) {	
			if (Emplace_Umit_In_Squad(zone, squad_ID, entity, potential_position, mass, radius) == true) {
				return true;
			}
		}
		entt::entity new_squad_ID = Create_New_Squad(zone);
		Emplace_Umit_In_Squad(zone, new_squad_ID, entity, potential_position, mass, radius);
		return true;
	}

	void Create_And_Fill_New_Squad(entt::registry& zone) {
		
		auto view = zone.view<Components::Potential_Position, Components::Radius, Components::Mass, Components::Soldier, Components::Selected>(entt::exclude<Components::Assigned>);
		for (auto unit_ID : view) {
			auto& position = view.get<Components::Potential_Position>(unit_ID);
			auto& mass = view.get<Components::Mass>(unit_ID);
			auto& radius = view.get<Components::Radius>(unit_ID);
			Assign_Selected_Units_To_Squad(zone, unit_ID, position, mass, radius);
		}
		
	};

	bool Assign_All_Units_To_Squad(entt::registry& zone, entt::entity& squad_ID) {
		Components::Squad& squad = zone.get<Components::Squad>(squad_ID);
		int iUnit = 0;

		auto view = zone.view<Components::Potential_Position, Components::Radius, Components::Mass, Components::Soldier>(entt::exclude<Components::Assigned>);
		for (auto entity : view) {

			auto& x = view.get<Components::Potential_Position>(entity).fPX;
			auto& y = view.get<Components::Potential_Position>(entity).fPY;
			auto& m = view.get<Components::Mass>(entity).fKilos;
			auto& r = view.get<Components::Radius>(entity).fRadius;
			auto& soldier = zone.emplace_or_replace<Components::Assigned>(entity, 0, squad_ID);

			squad.fPX.emplace_back(x);
			squad.fPY.emplace_back(y);
			squad.vPosition.emplace_back(x, y);
			squad.fMass.emplace_back(m);
			squad.fRadius.emplace_back(r);
			squad.iSub_Units.emplace_back(entity);
			squad.iStruck.emplace_back(0);
			squad.bAlive.emplace_back(true);

			soldier.iIndex = squad.iSub_Units.size() - 1;

			iUnit++;
			if (iUnit >= squad.size) {
				return true;
			}
		}
		return false;
	}

	void Create_Squads_From_All_Unassigned(entt::registry &zone) {
		entt::entity squad_ID;
		bool units = true;

		while (units) {
			squad_ID = Create_New_Squad(zone);
			units = Assign_All_Units_To_Squad(zone, squad_ID);;
		}
	}


	void Assign_Soldiers_On_Spawn(entt::registry& zone) {
		Create_Squads_From_All_Unassigned(zone);
		Create_Platoons(zone); 
		Create_Companies(zone);
	}

}
