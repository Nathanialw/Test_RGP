#pragma once
#include "entt.hpp"
#include <iostream>
#include "components.h"
#include "graphics.h"
#include "entity_loader.h"
#include "death_spells.h"
#include "world.h"
#include "recruitable_units.h"


using namespace Components;

namespace Scene {


	//create templates of a "type" of entity, like a skeleton
	//import from SQLite with a for loop where it just graps all the template data from tables and the only data I need to set manually is the position with the "potential position" variable. Not sure where to keep the position data so it is editable, maybe a separate file with all the map "tile" data
	//

	//adds Environment items to world grid
	void add_unit_to_grid(Map::Node3& map) {
		auto view = World::zone.view<Potential_Position, Radius, Environment>(entt::exclude<Assigned>);
		for (auto entity : view) {
			auto& x = view.get<Potential_Position>(entity);
			auto& y = view.get<Potential_Position>(entity);
			auto& r = view.get<Radius>(entity).fRadius;
			SDL_FRect rect = { x.fPX - r, y.fPY - r, r * 2, r * 2 };

			Map::Place_Rect_On_Grid(rect, Map::map, entity);
			World::zone.emplace_or_replace<Assigned>(entity);
		}
	}

	//adds Environment items to map grid

	void add_terrain_to_grid(Map::Node3& map) {
		auto view = World::zone.view<Terrain_Position_X, Terrain_Position_Y, Radius, Terrain>(entt::exclude<Assigned>);
		for (auto entity : view) {
			auto& x = view.get<Terrain_Position_X>(entity);
			auto& y = view.get<Terrain_Position_Y>(entity);
			auto& r = view.get<Radius>(entity);
			SDL_FRect rect = { x.fX, y.fY, 100, 100 };

			Map::Place_Rect_On_Grid_Once(rect, map, entity);
			World::zone.emplace_or_replace<Assigned>(entity);
		}
	}

	void Spawn_Skeletons(entt::registry &zone, int x, int y) {
		for (float j = 0; j < x; ++j) {
			for (float i = 0; i < y; ++i) {
				Death_Spells::Summon_Skeleton(zone, (100.0f + (i * 60.0f)), (100.0f + (j * 60.0f)), "'skeleton'", Graphics::skeleton_1);
				Death_Spells::Summon_Skeleton(zone, (200.0f + (i * 60.0f)), (200.0f + (j * 60.0f)), "'skeleton_mage'", Graphics::skeleton_mage_0);
			}
		}
	}

	void Create_Tree(float& x, float& y, const char* name, SDL_Texture* texture) {
		Entity_Loader::Data data = Entity_Loader::parse_data(name);
		float scale = 1.0f;

		auto tree = World::zone.create();
		World::zone.emplace<animation>(tree, Graphics::tree_0); /// need to load hetexture	 only once and pass the pointer into this function
		World::zone.get<animation>(tree).sheet = {
			{{ 0, 0, 631, 723}, 0, 631, 0, 0, 16.0f, 0.0f } }; //populate the vector
		World::zone.emplace<Sprite_Offset>(tree, 313.0f * scale, 609.0f * scale);
		World::zone.emplace<Scale>(tree, scale);

		World::zone.emplace<Position>(tree, 100.0f, 100.0f);
		World::zone.emplace<Potential_Position>(tree, (x * 952.0f), 100.0f + (y * 1165.0f));
		World::zone.emplace<Actions>(tree, isStatic);
		World::zone.get<Actions>(tree).frameCount = { { 0, 0} };
		World::zone.emplace<Direction>(tree, W);

		World::zone.emplace<Radius>(tree, data.radius * scale);
		World::zone.emplace<Environment>(tree);
		World::zone.emplace<Mass>(tree, data.mass * scale);
	}

	void Spawn_Trees () {
		for (float j = 0; j < 10; ++j) {
			for (float i = 0; i < 10; ++i) {
				Create_Tree(i, j, "'tree'", Graphics::tree_0);
			}
		}
	}

	void Load_Entities(entt::registry &zone) {
		float scale = 1.0f;
		//player
		auto skeleton = World::zone.create();			//creates a unique handle for an entity
		World::zone.emplace<animation>(skeleton, Graphics::warrior_axe); /// need to load the texture /only /once and pass the pointer into this function
		World::zone.get<animation>(skeleton).sheet = { //populate the vector
			{ NULL },
			{ {0   , 0, 128, 128}, 0,    512,  1, 0, 75.0f, 0.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
			{ {512,  0, 128, 128}, 512,  1024, 0, 0, 75.0f, 0.0f},//walk
			{ {1536, 0, 128, 128}, 1536, 512,  0, 0, 75.0f, 0.0f},//slash
			{ {2048, 0, 128, 128}, 2048, 512,  0, 0, 75.0f, 0.0f},//cast
			{ {2560, 0, 128, 128}, 2560, 256,  0, 0, 75.0f, 0.0f},//block
			{ {2816, 0, 128, 128}, 2816, 768,  0, 0, 75.0f, 0.0f},// death //reverse to summon
			{ {3584, 0, 128, 128}, 3584, 512,  1, 0, 75.0f, 0.0f},//ranged
		};
		World::zone.emplace<Sprite_Offset>(skeleton, 60.0f * scale, 95.0f *scale);
		World::zone.emplace<Scale>(skeleton, scale);

		World::zone.emplace<Actions>(skeleton, idle);
		World::zone.get<Actions>(skeleton).frameCount = { {0, 0}, { 4, 0}, {7, 0}, {4, 0}, {4, 0}, {2,0}, {5,0}, {4,0}, {4,0} };

		//positon components
		World::zone.emplace<Position>(skeleton, 1100.0f, 1100.0f);
		World::zone.emplace<Potential_Position>(skeleton, 1100.0f, 1100.0f);

		World::zone.emplace<Radius>(skeleton, 15.0f * scale);

		World::zone.emplace<Velocity>(skeleton, 0.f, 0.0f, 0.f, 0.0f, 0.2f);
		World::zone.emplace<Direction>(skeleton, SE);
		World::zone.emplace<Alive>(skeleton, true);
		World::zone.emplace<handle>(skeleton, "Skeleton");
		World::zone.emplace<Mass>(skeleton, 200.0f * scale);
		World::zone.emplace<Health>(skeleton, 10);

		World::zone.emplace<Input>(skeleton);
		World::zone.emplace<Camera>(skeleton, 0.0f, 0.0f, Graphics::resolution.w, Graphics::resolution.h, 2.0f, 2.0f);
		
		//scene.emplace<Component_Camera::Viewport>(skeleton, 0.0f, 0.0f, resolution.w/2.0f, resolution.h/2.0f, 1.0f, 1.0f );

		//grass
		for (auto j = 0; j < 16; j++) {
			for (auto i = 0; i < 16; i++) {
				auto grass = World::zone.create();

				World::zone.emplace<animation>(grass, Graphics::grass_0, 0, 0, 100, 100, 0, 0, 100, 100); /// need to load hetexture	 only once and pass the pointer into this function
				World::zone.get<animation>(grass).sheet = {
					{{ 1, 1, 235, 235}, 0, 234, 0, 0, 16.0f } }; //populate the vector
				World::zone.emplace<Sprite_Offset>(grass, 0.0f, 0.0f);
				World::zone.emplace<Actions>(grass, isStatic);
				World::zone.get<Actions>(grass).frameCount = { { 0, 0} };

				World::zone.emplace<Radius>(grass, 50.0f);
				World::zone.emplace<Terrain_Position_X>(grass, 0.0f, i * 100.0f);
				World::zone.emplace<Terrain_Position_Y>(grass, 0.0f, j * 100.0f);

				World::zone.emplace<Terrain>(grass);
			}
		}

		//Skeletons
		Spawn_Skeletons(zone, 8, 4);

		//archers
		Units::Create_Archer(0.0f, 0.0f);

		//trees
		Spawn_Trees();

		//set positions
		auto view = World::zone.view<Position, Potential_Position>();
		for (auto entity : view) {
			auto& x = view.get<Position>(entity);
			auto& y = view.get<Position>(entity);
			auto& px = view.get<Potential_Position>(entity);
			auto& py = view.get<Potential_Position>(entity);
			x.fX = px.fPX;
			y.fY = py.fPY;
		}

		auto view2 = World::zone.view<Terrain_Position_X, Terrain_Position_Y>();
		for (auto entity2 : view2) {
			auto& xx = view2.get<Terrain_Position_X>(entity2);
			auto& yy = view2.get<Terrain_Position_Y>(entity2);
			xx.fX = xx.fPX;
			yy.fY = yy.fPY;
		}
	}	

	void Update_Army() {
		auto company_view = World::zone.view<Company>();
		for (auto companies : company_view) {
			auto& company = company_view.get<Company>(companies);	
			for (int c = 0; c < company.iSub_Units.size(); c++) {
				auto& platoon = World::zone.get<Platoon>(company.iSub_Units[c]);
				for (int p = 0; p < platoon.iSub_Units.size(); p++) {
					auto& squad = World::zone.get<Squad>(platoon.iSub_Units[p]);
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

		auto view = World::zone.view<Alive>();
		for (auto entity : view) {
			auto& x = view.get<Alive>(entity).bIsAlive;
			if (x == false) {
				//scene.destroy(entity);
			}
		}

		add_unit_to_grid(Map::map);
		//std::cout << "add_unit_to_grid = Good" << std::endl;
		add_terrain_to_grid(Map::terrain);		
		//std::cout << "add_terrain_to_grid = Good" << std::endl;
	}

	void Init_Zone(entt::registry &zone) {
		Map::Build_Map(Map::map);
		Map::Build_Map(Map::terrain);
		Entity_Loader::init_db();
		Load_Entities(zone);
		SDL_RenderSetScale(Graphics::renderer, 2.0f, 2.0f);
		
	}
}
