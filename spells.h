#include "movement.h"

using namespace Scene;

namespace Spells {

	void create_spell(entt::entity& spell, f2d& pos, Compass& direction) {
		scene.emplace<animation>(spell, fireball_0); /// need to load the texture /only /once and pass the pointer into this function
		scene.get<animation>(spell).sheet = { //populate the vector
			{ NULL },
			{ NULL },  //0, 0, 64, 64 }, 0, 512, 1, 0, { 60, 95 }, 16.0f }, //idle array[numframes] = { 2ms, 4ms, 2ms}
			{			{0, 0, 64, 64 }, 0, 512, 0, 0, { 32, 32 }, 16.0f} //walk
		};

		scene.emplace<Actions>(spell, walk);
		scene.get<Actions>(spell).frameCount = { {0, 0}, {0, 0}, {8, 0} };

		//positon components
		scene.emplace<Position_X>(spell, pos.fX, 0.0f, 0.0f); //needs to be set to an offset position of the unit creating the spell
		scene.emplace<Position_Y>(spell, pos.fY, 0.0f, 0.0f); //needs to be set to an offset position of the unit creating the spell

		scene.emplace<Radius>(spell, 5.0f);

		scene.emplace<Velocity>(spell, 0.f, 0.0f, 0.0f, 0.0f, 0.175f);
		scene.emplace<Direction>(spell, direction); //match Direction of the caster
		scene.emplace<Alive>(spell, true);
		scene.emplace<handle>(spell, "fireball");
		scene.emplace<Mass>(spell, 200.0f);
	///	scene.emplace<Mouse_Move>(spell);
		Movement::Mouse_Order_Move();
	}


	void create_fireball(float& x, float& y, Compass& direction) {
		auto fireball = scene.create();
		f2d pos = { x, y };
		create_spell(fireball, pos, direction);
	}

	void Move_Order(entt::entity& entity, float& x, float& y) {
		scene.emplace_or_replace<Mouse_Move>(entity);
		auto& mov = scene.get<Mouse_Move>(entity);
		mov.fX_Destination = x;
		mov.fY_Destination = y;
	}

	void cast_fireball() {
		auto view = scene.view<Direction, Position_X, Position_Y, Casting>();
		for (auto entity : view) {
			auto& dir = view.get<Direction>(entity);
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& spell = view.get<Casting>(entity);
			if (spell == fireball) {
				create_fireball(x.fX, y.fY, dir.eDirection);
			}
			scene.remove<Casting>(entity);
		}
	}


	void add_spells_to_scene() {
		cast_fireball();
	}

	 
}