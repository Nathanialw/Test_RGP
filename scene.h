#pragma once
#include "entt.hpp"
#include <iostream>
#include "components.h"
#include "graphics.h"
#include "entity_loader.h"

using namespace Components;
using namespace Graphics;


namespace Scene {

	entt::registry scene;



	//create templates of a "type" of entity, like a skeleton
	//import from SQLite with a for loop where it just graps all the template data from tables and the only data I need to set manually is the position with the "potential position" variable. Not sure where to keep the position data so it is editable, maybe a separate file with all the map "tile" data
	//
	void Init_Mil_Struc() {
		//Squads.reserve(100);
	}



	void create_skeleton() {
		Entity_Loader::Data data = Entity_Loader::parse_data("'skeleton'");
		for (auto j = 0; j < 12; ++j) {
			for (auto i = 0; i < 12; ++i) {
				auto skeleton0 = scene.create();
				//unit data
				scene.emplace<Radius>(skeleton0, data.radius);
				scene.emplace<Velocity>(skeleton0, 0.f, 0.0f, 0.f, 0.0f, data.speed);
				scene.emplace<Mass>(skeleton0, data.mass);
				
				
				
				//rendering data
				scene.emplace<animation>(skeleton0, skeleton_1); /// need to load the texture nly once and pass the pointer intothis function
				scene.get<animation>(skeleton0).sheet = { //populate the vector
					{ NULL },
					{ {0   , 0, 128, 128}, 0,    512,  1, 0, {60, 95}, 16.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
					{ {512,  0, 128, 128}, 512,  1024, 0, 0, {60, 95}, 16.0f},//walk
					{ {1536, 0, 128, 128}, 1536, 512,  1, 0, {60, 95}, 16.0f},//atack
					{ {2048, 0, 128, 128}, 2048, 512,  1, 0, {60, 95}, 16.0f},
					{ {2560, 0, 128, 128}, 2560, 256,  1, 0, {60, 95}, 16.0f},
					{ {2816, 0, 128, 128}, 2816, 768,  0, 0, {60, 95}, 16.0f}, //reverse to summon
					{ {3584, 0, 128, 128}, 3584, 512,  1, 0, {60, 95}, 16.0f},
				};
				scene.emplace<Actions>(skeleton0, idle);
				scene.get<Actions>(skeleton0).frameCount = { {0, 0}, { 0, 0}, {0, 0}, {4, 0}, {8,0}, {4,0}, {4,0}, {8,0} };

				//map data
				scene.emplace<Position_X>(skeleton0, 0.0f, 100.0f + (i * 40.0f), 0.0f);
				scene.emplace<Position_Y>(skeleton0, 0.0f, 100.0f + (j * 40.0f), 0.0f);
				scene.emplace<Direction>(skeleton0, SE);

				//default data
				scene.emplace<Alive>(skeleton0, true);
				scene.emplace<handle>(skeleton0, "Skeleton");
				scene.emplace<Soldier>(skeleton0);




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
		scene.emplace<Position_X>(skeleton, 0.0f, 100.0f, 0.0f);
		scene.emplace<Position_Y>(skeleton, 0.0f, 100.0f, 0.0f);
				
		scene.emplace<Radius>(skeleton, 15.0f );

		scene.emplace<Velocity>(skeleton, 0.f, 0.0f, 0.f, 0.0f, 0.175f);
		scene.emplace<Input>(skeleton);
		scene.emplace<Direction>(skeleton, SE);
		scene.emplace<Alive>(skeleton, true);
		scene.emplace<handle>(skeleton, "Skeleton");
		scene.emplace<Camera>(skeleton, 0, 0, resolution.w/2, resolution.h/2, 1.0f, 1.0f );
		scene.emplace<Mass>(skeleton, 200.0f);



		
		//scene.emplace<IsInSquad>(skeleton, testSquad);

		//testSquad.assigned = scene.get<Position_X>(skeleton).assigned;

		//auto soldier = scene.create();			//creates an entity
		//scene.emplace<Soldier>(soldier);
		//
		//auto testing_entity = scene.create();
		//scene.emplace<Test_V>(testing_entity);
		//scene.get<Test_V>(testing_entity).test_V.emplace_back(scene.get<Position_X>(skeleton));
		//
		//
		//auto squad = scene.create();			//creates an entity
		//scene.emplace<Squad>(squad);
		//scene.get<Squad>(squad).soldier.emplace_back(scene.get<Soldier>(skeleton));//.Unit.emplace_back(scene.get<Soldier>(skeleton));
		//
		//auto company = scene.create();			//creates an entity
		//scene.emplace<Company>(company);
		//scene.get<Company>(company).squad.emplace_back(scene.get<Squad>(squad));
		
		//create a battalion entity with a battalion component that holds a reference to each entitty in the Battalion vector
		//auto 1sBattalion = scene.create();			//creates an entity
		//scene.emplace<Battalion>(1stBattalion);
		//scene.get<Battalion>(1stBattalion).company.emplace_back(scene.get<Company>(company));
		
		
		
		//for (auto Battalion_ : scene.get<Battalion>(battalion).company) {
		//	for (auto Company_ : Battalion_.squad) {
		//		for (auto Soldier_ : Company_.soldier) {
		//			Soldier_.bCantAssignEmptyComponents;
		//			
		//		}
		//	}
		//}

		//Position, radius, mass
		//attack
		// 
		// //check moving units against battalions(including his own), if they overlap? ->  else break
				//check moving units against companies(including his own), if they overlap -> else break
					//check moving units agaisnt sqauds(including his own), if they overlap -> else break
						//check soldiers againts each other soldiers
		// 
		// 
		// 
		// 
		//check battalions against each other, if they overlap? ->  else break
			//check comapanies against each other , if they overlap -> else break
				//check sqauds agaisnt each oter, if they overlap -> else break
					//check soldiers against each other


		
		
		//Skeletons
		create_skeleton();
			
		
		//trees
		for (auto j = 0; j < 5; ++j) {
			for (auto i = 0; i < 5; ++i) {
				auto tree = scene.create();
				scene.emplace<animation>(tree, tree_0); /// need to load hetexture	 only once and pass the pointer into this function
				scene.get<animation>(tree).sheet = {
					{{ 0, 0, 631, 723}, 0, 631, 0, 0, {313, 609}, 16.0f } }; //populate the vector

				scene.emplace<Position_X>(tree, 100.0f, 100.0f + (i * 952.0f), 0.0f);
				scene.emplace<Position_Y>(tree, 100.0f, 100.0f + (j * 1165.0f), 0.0f);
				scene.emplace<Radius>(tree, 30.0f);

				scene.emplace<Actions>(tree, isStatic);
				scene.get<Actions>(tree).frameCount = { { 0, 0} };
				scene.emplace<Direction>(tree, W);
				scene.emplace<Mass>(tree, 4000000.0f);

			}
		}

		//archers
		for (auto j = 0; j < 5; ++j) {
			for (auto i = 0; i < 5; ++i) {
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
				scene.emplace<Mass>(archer, 1000.0f);
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

	}


	

	void update_scene() {
		auto view = scene.view<Alive>();
		for (auto entity : view) {
			auto& x = view.get<Alive>(entity);
			if (x.bIsAlive == false) {
				scene.destroy(entity);
			}
		}
	}

	void Init_Game() {
		Init_Mil_Struc();
		Entity_Loader::init_db();
		Load_Entities();
	}
}
