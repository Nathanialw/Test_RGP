#pragma once
#include "scene.h"
#include "mouse_control.h"

using namespace Scene;

namespace Death_Spells {


	void Summon_Skeleton() {
		auto skeleton0 = scene.create();
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

		scene.emplace<Position_X>(skeleton0, 0.0f, float(Mouse::iXWorld_Mouse), 0.0f);
		scene.emplace<Position_Y>(skeleton0, 0.0f, float(Mouse::iYWorld_Mouse), 0.0f);
		scene.emplace<Collision_Radius>(skeleton0, 15.0f);

		scene.emplace<Direction>(skeleton0, SE);
		scene.emplace<Alive>(skeleton0, true);
		scene.emplace<handle>(skeleton0, "Skeleton");
		scene.emplace<Mass>(skeleton0, 200.0f);
		scene.emplace<Commandable>(skeleton0);

	}

}