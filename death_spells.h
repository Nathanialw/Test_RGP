#pragma once
#include "entt.hpp"
#include "components.h"
#include "entity_loader.h"
#include "graphics.h"
#include "Scenes.h"




namespace Death_Spells {

	/*
	The x and y can be got rid of and the Position components can be updated elswhere; no reason to couple like that here
	*/
	void Summon_Skeleton(float x, float y, const char *name) {
		Entity_Loader::Data data = Entity_Loader::parse_data(name);
		auto skeleton0 = Scenes::scene.create();
		Scenes::scene.emplace<Components::animation>(skeleton0, Graphics::skeleton_1); /// need to load the texture nly once and pass the pointer intothis function
		Scenes::scene.get<Components::animation>(skeleton0).sheet = { //populate the vector
			{ NULL },
			{ {0   , 0, 128, 128}, 0,    512,  1, 0, {60, 95}, 75.0f, 0.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
			{ {512,  0, 128, 128}, 512,  1024, 0, 0, {60, 95}, 75.0f, 0.0f},//walk
			{ {1536, 0, 128, 128}, 1536, 512,  0, 0, {60, 95}, 75.0f, 0.0f},//atack
			{ {2048, 0, 128, 128}, 2048, 512,  0, 0, {60, 95}, 75.0f, 0.0f},
			{ {2560, 0, 128, 128}, 2560, 256,  0, 0, {60, 95}, 75.0f, 0.0f},
			{ {2816, 0, 128, 128}, 2816, 768,  0, 0, {60, 95}, 75.0f, 0.0f}, //reverse to summon
			{ {3584, 0, 128, 128}, 3584, 512,  1, 0, {60, 95}, 75.0f, 0.0f},
		};

		Scenes::scene.emplace<Components::Actions>(skeleton0, Components::idle);
		Scenes::scene.get<Components::Actions>(skeleton0).frameCount = { {0, 0}, { 4, 0}, {8, 0}, {4, 0}, {4,0}, {2,0}, {5,0}, {4,0} };

		Scenes::scene.emplace<Components::Position_X>(skeleton0, 0.0f, x);
		Scenes::scene.emplace<Components::Position_Y>(skeleton0, 0.0f, y);
		Scenes::scene.emplace<Components::Radius>(skeleton0, 15.0f);
		Scenes::scene.emplace<Components::Velocity>(skeleton0, 0.f, 0.0f, 0.f, 0.0f, data.speed);

		Scenes::scene.emplace<Components::Direction>(skeleton0, Components::SE);
		Scenes::scene.emplace<Components::handle>(skeleton0, "Skeleton");
		Scenes::scene.emplace<Components::Mass>(skeleton0, 200.0f);
		
		Scenes::scene.emplace<Components::Alive>(skeleton0, true);
		Scenes::scene.emplace<Components::Health>(skeleton0, 5);
		Scenes::scene.emplace<Components::Sight_Range>(skeleton0, x - 250.0f, y - 250.0f, 500.0f, 500.0f);
				
		Scenes::scene.emplace<Components::Soldier>(skeleton0);
		Scenes::scene.emplace<Components::Commandable>(skeleton0);
		Scenes::scene.emplace<Components::Spellbook>(skeleton0);
	}

}