#include <vector>
#include "base_structs.h"

using namespace base;

namespace Map {



	struct Cell {
		SDL_FRect sCollide_Box = { 0,0,0,0 };
		//they hsould be all the same size at the given node level so this value only needs to be
		//saved once and calculated on the fly - less memory and maybe cache friendlier
		std::vector<entt::entity> entities;
		//Cell(float x, float y, float w, float h) {
		//	sCollide_Box = { x, y, w, h };
		//}
	};


	struct Node0 {
		SDL_FRect sCollide_Box = {0,0,0,0};
		Cell cells[16];
	};
	struct Node1 {
		SDL_FRect sCollide_Box = { 0,0,0,0 };
		Node0 nodes[16];
	};
	struct Node2 {
		SDL_FRect sCollide_Box = { 0,0,0,0 };
		Node1 nodes[16];
	};
	struct Node3 {
		SDL_FRect sCollide_Box = { 0.0f,0.0f,0.0f,0.0f };
		Node2 nodes[16];
	};


	Node3 map;
	
	void Create_Cell(Node0 &node) {
		float w = node.sCollide_Box.w / 4;
		float h = node.sCollide_Box.h / 4;
		int k = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				node.cells[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
				k++;
			//	std::cout << h << std::endl;
			}
		}
	}

	void Create_Node0(Node1 &node) {
		float w = node.sCollide_Box.w / 4;
		float h = node.sCollide_Box.h / 4;
		int k = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				node.nodes[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
				Create_Cell(node.nodes[k]);
				k++;
			}
		}
	}			

	void Create_Node1(Node2 &node) {
		float w = node.sCollide_Box.w / 4;
		float h = node.sCollide_Box.h / 4;
		int k = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				node.nodes[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
				Create_Node0(node.nodes[k]);
				k++;
			}
		}
	}

	void Create_Node2(Node3 &node) {
		float w = node.sCollide_Box.w / 4;
		float h = node.sCollide_Box.h / 4;
		int k = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				node.nodes[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
				Create_Node1(node.nodes[k]);
				k++;
			}
		}
	}


	// h = 235, w = eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

	void Build_Map() {
		map.sCollide_Box = { 0.0f, 0.0f, 25600.0f, 25600.0f };
		Create_Node2(map);


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