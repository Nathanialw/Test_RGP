#pragma once
#include "entt.hpp"
#include "components.h"
#include "entity_loader.h"
#include "graphics.h"
#include "Scenes.h"


using namespace Components;
using namespace Graphics;
using namespace Scenes;

namespace Death_Spells {


	void Summon_Skeleton(float x, float y) {
		Entity_Loader::Data data = Entity_Loader::parse_data("'skeleton'");
		auto skeleton0 = scene.create();
		scene.emplace<animation>(skeleton0, skeleton_1); /// need to load the texture nly once and pass the pointer intothis function
		scene.get<animation>(skeleton0).sheet = { //populate the vector
			{ NULL },
			{ {0   , 0, 128, 128}, 0,    512,  1, 0, {60, 95}, 75.0f},//idle array[numframes] = { 2ms, 4ms, 2ms}
			{ {512,  0, 128, 128}, 512,  1024, 0, 0, {60, 95}, 75.0f},//walk
			{ {1536, 0, 128, 128}, 1536, 512,  1, 0, {60, 95}, 75.0f},//atack
			{ {2048, 0, 128, 128}, 2048, 512,  1, 0, {60, 95}, 75.0f},
			{ {2560, 0, 128, 128}, 2560, 256,  1, 0, {60, 95}, 75.0f},
			{ {2816, 0, 128, 128}, 2816, 768,  0, 0, {60, 95}, 75.0f}, //reverse to summon
			{ {3584, 0, 128, 128}, 3584, 512,  1, 0, {60, 95}, 75.0f},
		};

		scene.emplace<Actions>(skeleton0, idle);
		scene.get<Actions>(skeleton0).frameCount = { {0, 0}, { 0, 0}, {0, 0}, {4, 0}, {8,0}, {4,0}, {4,0}, {8,0} };

		scene.emplace<Position_X>(skeleton0, 0.0f, x, 0.0f);
		scene.emplace<Position_Y>(skeleton0, 0.0f, y, 0.0f);
		scene.emplace<Radius>(skeleton0, 15.0f);
		scene.emplace<Velocity>(skeleton0, 0.f, 0.0f, 0.f, 0.0f, data.speed);

		scene.emplace<Direction>(skeleton0, SE);
		scene.emplace<handle>(skeleton0, "Skeleton");
		scene.emplace<Mass>(skeleton0, 200.0f);
		
		scene.emplace<Alive>(skeleton0, true);
		scene.emplace<Health>(skeleton0, 5);
				
		scene.emplace<Soldier>(skeleton0);
	}

}