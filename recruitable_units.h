#pragma once
#include "entt.hpp"
#include <iostream>
#include "components.h"
#include "graphics.h"
#include "entity_loader.h"

#include "scenes.h"

namespace Units {

	void Create_Archer(float x, float y)  {
	for (auto j = 0; j < x; ++j) {
		for (auto i = 0; i < y; ++i) {
			auto archer = Scenes::scene.create();			//creates a unique handle for an entity
			Scenes::scene.emplace<Components::animation>(archer, Graphics::archer_0); /// need to load the texture only onceand pass the pointer into this function
			Scenes::scene.get<Components::animation>(archer).sheet = { //populate the vector
				{ NULL },
				{ {0   , 0, 128, 128}, 0,    512,  1, 0, 75.0f, 75.0f },//idle
				{ {512,  0, 128, 128}, 512,  1024, 0, 0, 75.0f, 75.0f },//walk
				{ {1536, 0, 128, 128}, 1536, 512,  0, 0, 75.0f, 0.0f },//attack
				
				{ {2048, 0, 128, 128}, 2048, 256,  0, 0, 75.0f, 0.0f },//struck
				{ {2304, 0, 128, 128}, 2560, 768,  0, 0, 75.0f, 0.0f }, //dead
				{ {3072, 0, 128, 128}, 2816, 512,  0, 0, 75.0f, 0.0f }, //cheer
				{ {3584, 0, 128, 128}, 3584, 512,  1, 0, 75.0f, 0.0f },//range
			};
			Scenes::scene.emplace<Components::Sprite_Offset>(archer, 60.0f, 95.0f);

			Scenes::scene.emplace<Components::Actions>(archer, Components::idle);
			Scenes::scene.get<Components::Actions>(archer).frameCount = { {0, 0}, { 4, 0}, {8, 0}, {4, 0}, {0, 0}, {2,0}, {5,0}, {4,0}, {4,0} };

			Scenes::scene.emplace<Components::Position_X>(archer, 0.0f, 150.0f);
			Scenes::scene.emplace<Components::Position_Y>(archer, 0.0f, 200.0f);
			Scenes::scene.emplace<Components::Radius>(archer, 15.0f);
			Scenes::scene.emplace<Components::Velocity>(archer, 0.f, 0.0f, 0.f, 0.0f, 0.175f);
								  
			Scenes::scene.emplace<Components::Direction>(archer, Components::SE);
			Scenes::scene.emplace<Components::handle>(archer, "archer");
			Scenes::scene.emplace<Components::Mass>(archer, 200.0f);
								  
			Scenes::scene.emplace<Components::Health>(archer, 5);
			Scenes::scene.emplace<Components::Alive>(archer, true);
								  
			Scenes::scene.emplace<Components::Soldier>(archer);
			Scenes::scene.emplace<Components::Commandable>(archer);
			}
		}
	}
	


}