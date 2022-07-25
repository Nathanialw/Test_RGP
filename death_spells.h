#pragma once
#include "entt.hpp"
#include "components.h"
#include "entity_loader.h"

namespace Death_Spells {

	/*
	The x and y can be got rid of and the Position components can be updated elswhere; no reason to couple like that here
	*/
	void Summon_Skeleton(entt::registry &zone, float x, float y, const char *name, SDL_Texture* texture) {
	
		Entity_Loader::Data data = Entity_Loader::parse_data(name);

		auto skeleton0 = zone.create();
		zone.emplace<Components::animation>(skeleton0, texture); /// need to load the texture nly once and pass the pointer intothis function
		zone.get<Components::animation>(skeleton0).sheet = { //populate the vector
			{ NULL },
			{ {0   , 0, 128, 128}, 0,    512,  1, 0, 75.0f, 0.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
			{ {512,  0, 128, 128}, 512,  1024, 0, 0, 75.0f, 0.0f},//walk
			{ {1536, 0, 128, 128}, 1536, 512,  0, 0, 75.0f, 0.0f},//atack
			{ {2048, 0, 128, 128}, 2048, 512,  0, 0, 75.0f, 0.0f},//cast
			{ {2560, 0, 128, 128}, 2560, 256,  0, 0, 75.0f, 0.0f},//block
			{ {2816, 0, 128, 128}, 2816, 768,  0, 0, 75.0f, 0.0f}, //reverse to summon
			{ {3584, 0, 128, 128}, 3584, 512,  1, 0, 75.0f, 0.0f},//ranged
		};
		zone.emplace<Components::Sprite_Offset>(skeleton0, 60.0f, 95.0f );
		zone.emplace<Components::Scale>(skeleton0, 1.0f);

		zone.emplace<Components::Actions>(skeleton0, Components::idle);
		zone.get<Components::Actions>(skeleton0).frameCount = { {0, 0}, { 4, 0}, {7, 0}, {4, 0}, {4,0}, {2,0}, {5,0}, {4,0} };

		zone.emplace<Components::Position>(skeleton0, x, y);
		zone.emplace<Components::Potential_Position>(skeleton0, x, y);
		zone.emplace<Components::Radius>(skeleton0, data.radius);
		zone.emplace<Components::Velocity>(skeleton0, 0.f, 0.0f, 0.f, 0.0f, data.speed);

		zone.emplace<Components::Direction>(skeleton0, Components::SE);
		zone.emplace<Components::handle>(skeleton0, "Skeleton");
		zone.emplace<Components::Mass>(skeleton0, data.mass);
		
		zone.emplace<Components::Alive>(skeleton0, true);
		zone.emplace<Components::Health>(skeleton0, 1);
		zone.emplace<Components::Sight_Range>(skeleton0, x - 250.0f, y - 250.0f, 500.0f, 500.0f);
				
		zone.emplace<Components::Soldier>(skeleton0);
		zone.emplace<Components::Commandable>(skeleton0);
		zone.emplace<Components::Spellbook>(skeleton0);

	}

}