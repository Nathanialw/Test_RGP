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
		zone.emplace<Component::animation>(skeleton0, texture); /// need to load the texture nly once and pass the pointer intothis function
		zone.get<Component::animation>(skeleton0).sheet = { //populate the vector
			{ NULL },
			{ {0   , 0, 128, 128}, 0,    512,  1, 0, 75.0f, 0.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
			{ {512,  0, 128, 128}, 512,  1024, 0, 0, 75.0f, 0.0f},//walk
			{ {1536, 0, 128, 128}, 1536, 512,  0, 0, 75.0f, 0.0f},//atack
			{ {2048, 0, 128, 128}, 2048, 512,  0, 0, 75.0f, 0.0f},//cast
			{ {2560, 0, 128, 128}, 2560, 256,  0, 0, 75.0f, 0.0f},//block
			{ {2816, 0, 128, 128}, 2816, 768,  0, 0, 75.0f, 0.0f}, //reverse to summon
			{ {3584, 0, 128, 128}, 3584, 512,  1, 0, 75.0f, 0.0f},//ranged
		};
		zone.emplace<Component::Sprite_Offset>(skeleton0, 60.0f, 95.0f );
		zone.emplace<Component::Scale>(skeleton0, 1.0f);

		zone.emplace<Component::Actions>(skeleton0, Component::idle);
		zone.get<Component::Actions>(skeleton0).frameCount = { {0, 0}, { 4, 0}, {7, 0}, {4, 0}, {4,0}, {2,0}, {5,0}, {4,0} };

		zone.emplace<Component::Position>(skeleton0, x, y);
		zone.emplace<Component::Radius>(skeleton0, data.radius);
		zone.emplace<Component::Velocity>(skeleton0, 0.f, 0.0f, 0.f, 0.0f, data.speed);

		zone.emplace<Component::Direction>(skeleton0, Component::SE);
		zone.emplace<Component::handle>(skeleton0, "Skeleton");
		zone.emplace<Component::Mass>(skeleton0, data.mass);
		
		zone.emplace<Component::Alive>(skeleton0, true);
		zone.emplace<Component::Health>(skeleton0, 1);
		zone.emplace<Component::Sight_Range>(skeleton0, x - 250.0f, y - 250.0f, 500.0f, 500.0f);
				
		zone.emplace<Component::Soldier>(skeleton0);
		zone.emplace<Component::Commandable>(skeleton0);
		zone.emplace<Component::Spellbook>(skeleton0);

	}

}