#pragma once
#include "entt.hpp"
#include <iostream>
#include "components.h"
#include "graphics.h"
#include "entity_loader.h"
#include "death_spells.h"
#include "scenes.h"


using namespace Components;
using namespace Graphics;
using namespace Scenes;


namespace Scene {


	//create templates of a "type" of entity, like a skeleton
	//import from SQLite with a for loop where it just graps all the template data from tables and the only data I need to set manually is the position with the "potential position" variable. Not sure where to keep the position data so it is editable, maybe a separate file with all the map "tile" data
	//

	//adds Environment items to world grid
	void add_unit_to_grid(Map::Node3& map) {
		auto view = scene.view<Position_X, Position_Y, Radius, Environment>(entt::exclude<Assigned>);
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& r = view.get<Radius>(entity).fRadius;
			SDL_FRect rect = { x.fX - r, y.fY - r, r * 2, r * 2 };

			Map::Place_Rect_On_Grid(rect, Map::map, entity);
			scene.emplace_or_replace<Assigned>(entity);
		}
	}

	//adds Environment items to map grid

	void add_terrain_to_grid(Map::Node3& map) {
		auto view = scene.view<Terrain_Position_X, Terrain_Position_Y, Radius, Terrain>(entt::exclude<Assigned>);
		for (auto entity : view) {
			auto& x = view.get<Terrain_Position_X>(entity);
			auto& y = view.get<Terrain_Position_Y>(entity);
			auto& r = view.get<Radius>(entity);
			SDL_FRect rect = { x.fX, y.fY, 100, 100 };

			Map::Place_Rect_On_Grid_Once(rect, map, entity);
			scene.emplace_or_replace<Assigned>(entity);
		}
	}

	void Spawn_Skeletons(int x, int y) {
		Entity_Loader::Data data = Entity_Loader::parse_data("'skeleton'");
		for (auto j = 0; j < x; ++j) {
			for (auto i = 0; i < y; ++i) {
				Death_Spells::Summon_Skeleton((100.0f + (i * 60.0f)), (100.0f + (j * 60.0f)));
			}
		}
	}

	void Load_Entities() {		
		//player
		auto skeleton = scene.create();			//creates a unique handle for an entity
		scene.emplace<animation>(skeleton, skeleton_0); /// need to load the texture /only /once and pass the pointer into this function
		scene.get<animation>(skeleton).sheet = { //populate the vector
			{ NULL },
			{ {0   , 0, 128, 128}, 0,    512,  1, 0, {60, 95}, 16.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
			{ {512,  0, 128, 128}, 512,  1024, 0, 0, {60, 95}, 16.0f},//walk
			{ {1536, 0, 128, 128}, 1536, 512,  1, 0, {60, 95}, 16.0f},//atack
			{ {2048, 0, 128, 128}, 2048, 512,  1, 0, {60, 95}, 16.0f},
			{ {2560, 0, 128, 128}, 2560, 256,  1, 0, {60, 95}, 16.0f}, 
			{ {2816, 0, 128, 128}, 2816, 768,  0, 0, {60, 95}, 16.0f}, //reverse to summon
			{ {3584, 0, 128, 128}, 3584, 512,  1, 0, {60, 95}, 16.0f},
		};

		scene.emplace<Actions>(skeleton, idle);
		scene.get<Actions>(skeleton).frameCount = { {0, 0}, { 0, 0}, {0, 0}, {4, 0}, {8,0}, {4,0}, {4,0}, {8,0} };

		//positon components
		scene.emplace<Position_X>(skeleton, 1100.0f, 1100.0f, 0.0f);
		scene.emplace<Position_Y>(skeleton, 1100.0f, 1100.0f, 0.0f);
				
		scene.emplace<Radius>(skeleton, 15.0f );

		scene.emplace<Velocity>(skeleton, 0.f, 0.0f, 0.f, 0.0f, 0.175f);
		scene.emplace<Input>(skeleton);
		scene.emplace<Direction>(skeleton, SE);
		scene.emplace<Alive>(skeleton, true);
		scene.emplace<handle>(skeleton, "Skeleton");
		scene.emplace<Mass>(skeleton, 200.0f);

		scene.emplace<Camera>(skeleton, 0.0f, 0.0f, resolution.w, resolution.h, 1.0f, 1.0f );
		//scene.emplace<Component_Camera::Viewport>(skeleton, 0.0f, 0.0f, resolution.w/2.0f, resolution.h/2.0f, 1.0f, 1.0f );

		//grass
		for (auto j = 0; j < 64; j++) {
			for (auto i = 0; i < 64; i++) {
				auto grass = scene.create();

				scene.emplace<animation>(grass, grass_0, 0, 0, 100, 100, 0, 0, 100, 100); /// need to load hetexture	 only once and pass the pointer into this function
				scene.get<animation>(grass).sheet = {
					{{ 1, 1, 235, 235}, 0, 234, 0, 0, {0, 0}, 16.0f } }; //populate the vector
				scene.emplace<Actions>(grass, isStatic);
				scene.get<Actions>(grass).frameCount = { { 0, 0} };

				scene.emplace<Radius>(grass, 50.0f);
				scene.emplace<Terrain_Position_X>(grass, 0.0f, i * 100.0f, i * 100.0f);
				scene.emplace<Terrain_Position_Y>(grass, 0.0f, j * 100.0f, j * 100.0f);
			
				scene.emplace<Terrain>(grass);
			}
		}		
		
		//Skeletons
		Spawn_Skeletons(48, 80);
			
		
		//trees
		for (auto j = 0; j < 5; ++j) {
			for (auto i = 0; i < 5; ++i) {
				auto tree = scene.create();
				scene.emplace<animation>(tree, tree_0); /// need to load hetexture	 only once and pass the pointer into this function
				scene.get<animation>(tree).sheet = {
					{{ 0, 0, 631, 723}, 0, 631, 0, 0, {313, 609}, 16.0f } }; //populate the vector

				scene.emplace<Position_X>(tree, 100.0f, 100.0f + (i * 952.0f), 100.0f + (i * 952.0f));
				scene.emplace<Position_Y>(tree, 100.0f, 100.0f + (j * 1165.0f), 100.0f + (j * 1165.0f));
				scene.emplace<Actions>(tree, isStatic);
				scene.get<Actions>(tree).frameCount = { { 0, 0} };
				scene.emplace<Direction>(tree, W);


				scene.emplace<Radius>(tree, 30.0f);
				scene.emplace<Environment>(tree);
				scene.emplace<Mass>(tree, 40000.0f);


			}
		}

		//archers
		for (auto j = 0; j < 0; ++j) {
			for (auto i = 0; i < 0; ++i) {
				auto archer = scene.create();			//creates a unique handle for an entity
				scene.emplace<animation>(archer, archer_0); /// need to load the texture only onceand pass the pointer into this function
				scene.get<animation>(archer).sheet = { //populate the vector
					{ NULL },
					{ {0   , 0, 128, 128}, 0,    512,  1, 0, {60, 95}, 75.0f },
					{ {512,  0, 128, 128}, 512,  1024, 0, 0, {60, 95}, 75.0f },
					{ {1536, 0, 128, 128}, 1536, 512,  1, 0, {60, 95}, 75.0f },
					{ {2048, 0, 128, 128}, 2048, 512,  1, 0, {60, 95}, 75.0f },
					{ {2560, 0, 128, 128}, 2560, 256,  1, 0, {60, 95}, 75.0f },
					{ {2816, 0, 128, 128}, 2816, 768,  0, 0, {60, 95}, 75.0f }, //reverse to summon
					{ {3584, 0, 128, 128}, 3584, 512,  1, 0, {60, 95}, 75.0f },
				};
				scene.emplace<Actions>(archer, idle);
				scene.get<Actions>(archer).frameCount = { {0, 0}, { 0, 0}, {0, 0}, {4, 0}, {8,0}, {4,0}, {4,0}, {8,0} };
				scene.emplace<Velocity>(archer, 0.f, 0.0f, 0.f, 0.0f, 0.175f);


				scene.emplace<Position_X>(archer, 0.0f, 150.0f + (i * 144.0f), 0.0f);
				scene.emplace<Position_Y>(archer, 0.0f, 200.0f + (j * 144.0f), 0.0f);				
				scene.emplace<Radius>(archer, 15.0f);

				scene.emplace<Direction>(archer, SE);
				scene.emplace<Mass>(archer, 200.0f);
				scene.emplace<Soldier>(archer);

			}
		}

		//buildings
		//for (auto j = 0; j < 0; ++j) {
		//	for (auto i = 0; i < 0; ++i) {
		//		auto house = scene.create();
		//		scene.emplace<animation>(house, house_0);
		//		scene.get<animation>(house).sheet = {
		//			{{ 0, 0, 600, 509}, 0, 600, 0, 0, {313, 609}, 16.0f } }; //populate the vector

		//		scene.emplace<Position_X>(house, 0.0f, 200.0f + (i * 952.0f), 0.0f);
		//		scene.emplace<Position_Y>(house, 0.0f, 200.0f + (j * 1165.0f), 0.0f);
		//		scene.emplace<Building_Collider>(house);//needs to be made of lines ->  circle vs line collision

		//		scene.emplace<Actions>(house, isStatic);
		//		scene.get<Actions>(house).frameCount = { { 0, 0} };
		//		scene.emplace<Direction>(house, W);
		//		scene.emplace<Mass>(house, 4000000.0f);
		//	}
		//}

		//set positions
		auto view = scene.view<Position_X, Position_Y>();
		for (auto entity : view) {
			auto& xx = view.get<Position_X>(entity);
			auto& yy = view.get<Position_Y>(entity);
			xx.fX = xx.fPX;
			yy.fY = yy.fPY;
		}

		auto view2 = scene.view<Terrain_Position_X, Terrain_Position_Y>();
		for (auto entity2 : view2) {
			auto& xx = view2.get<Terrain_Position_X>(entity2);
			auto& yy = view2.get<Terrain_Position_Y>(entity2);
			xx.fX = xx.fPX;
			yy.fY = yy.fPY;
		}

	}	

	void Update_Army() {
		auto company_view = scene.view<Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);	
			for (int c = 0; c < company.iSub_Units.size(); c++) {
				auto& platoon = scene.get<Platoon>(company.iSub_Units[c]);
				for (int p = 0; p < platoon.iSub_Units.size(); p++) {
					auto& squad = scene.get<Squad>(platoon.iSub_Units[p]);
					for (int i = 0; i < squad.iSub_Units.size(); i++) {
						if (squad.bAlive.at(i) == false) {
							
						}
					}						
				}				
			}
		}
	}

	void update_scene() {
		//Update_Army();
		//std::cout << "Update_Army = Good" << std::endl;

		auto view = scene.view<Alive>();
		for (auto entity : view) {
			auto& x = view.get<Alive>(entity).bIsAlive;
			if (x == false) {
				scene.destroy(entity);
			}
		}

		add_unit_to_grid(Map::map);
		//std::cout << "add_unit_to_grid = Good" << std::endl;
		add_terrain_to_grid(Map::terrain);		
		//std::cout << "add_terrain_to_grid = Good" << std::endl;

	}

	void Init_Game() {
		Map::Build_Map(Map::map);
		Map::Build_Map(Map::terrain);
		Entity_Loader::init_db();
		Load_Entities();
	}
}
