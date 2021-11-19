#include <vector>
#include "base_structs.h"

using namespace base;

namespace map_objects {

	struct Cell {
		SDL_FRect sCollide_Box;
		std::vector<entt::entity> entities;
	};

	struct Region {
		SDL_FRect sCollide_Box;
		std::vector<Cell>cells;

		Region() {
			sCollide_Box = { 0,0,0,0 };
			cells.reserve(4);
		};
	};

	struct Zone {
		SDL_FRect sCollide_Box;
		std::vector<Region>regions;

		Zone() {
			sCollide_Box = { 0,0,0,0 };
			regions.reserve(4);
		};
	};

	// h = 235, w = 235

	std::vector<Zone> Map;

	void Build_Map() {
		//create the map tree

		//need to know the siza and location for the collide box

		//add each Recgion to the map
		//Add each cell to eah region
		
		for (int i = 0; i < 3; i++) {
			Zone zone;
			Map.emplace_back(zone);
		}



	}

	//void Add_Entity_To_Map(entt::entity entity) {
	//	//Add entities to the tree

	//	// define iIndex and iCell_Assigned_To
	//	int cell;
	//	entt::entity index;

	//	scene.emplace<iCell_Assigned>(entity, cell, index); 

	//}



}





namespace Mil_Struc {

	
	//wil have Regiments and Companies as administrative units

	struct Squad {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;
		std::vector<float>fMass;
		std::vector<float>fRadius;

		std::vector<f2d>vPosition; //


		Squad() {
			name = "Default";
			size = 8;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
			fMass.reserve(size);
			fRadius.reserve(size);
			vPosition.reserve(size);
		}
	};

	struct Platoon {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;
		std::vector<float>fPW;
		std::vector<float>fPH;


		Platoon() {
			name = "Default";
			size = 6;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
			fPW.reserve(size);
			fPH.reserve(size);
		}
	};

	struct Company {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;
		std::vector<float>fPW;
		std::vector<float>fPH;


		Company() {
			name = "Default";
			size = 4;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
			fPW.reserve(size);
			fPH.reserve(size);
		}
	};

	struct Battalion {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;
		std::vector<float>fPW;
		std::vector<float>fPH;

		Battalion() {
			name = "Default";
			size = 6;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
			fPW.reserve(size);
			fPH.reserve(size);
		}
	};	

	struct Regiment {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;

		Regiment() {
			name = "Default";
			size = 3;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
		}
	};
	
	struct Brigade {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;

		Brigade() {
			name = "Default";
			size = 2;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
		}
	};		
	
	struct Division {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;

		Division() {
			name = "Default";
			size = 3;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
		}
	};		
	
	struct Army {
		int size;
		std::string name;
		SDL_FRect sCollide_Box;

		std::vector<entt::entity>iSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;

		Army() {
			name = "Default";
			size = 4;
			sCollide_Box = { 0,0,0,0 };
			iSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
		}
	};




}