#pragma once
#include "movement.h"

using namespace Scene;

namespace Spells {

	void Spell_Move_Target() { //sends spell to where the mouse is
		auto view = scene.view<Casted, Spell>();
		for (auto entity : view) {
			scene.emplace_or_replace<Moving>(entity);
			scene.emplace_or_replace<Mouse_Move>(entity);
			auto& mov = scene.emplace_or_replace<Mouse_Move>(entity);
			mov.fX_Destination = Mouse::iXWorld_Mouse;
			mov.fY_Destination = Mouse::iYWorld_Mouse;
			scene.remove<Casted>(entity);
		}
	}

	void create_spell(entt::entity& spell, f2d& pos, Compass& direction) {
		Entity_Loader::Data data = Entity_Loader::parse_data("'fireball'");

		//rendering data
		scene.emplace<animation>(spell, fireball_0); /// need to load the texture /only /once and pass the pointer into this function
		scene.get<animation>(spell).sheet = { //populate the vector
			{ NULL },
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, { 32, 32 }, 16.0f }, //idle
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, { 32, 32 }, 16.0f } //walk
		};
		scene.emplace<Actions>(spell, walk);
		scene.get<Actions>(spell).frameCount = { {0, 0}, {0, 0}, {8, 0} };
		

		//positon data
		scene.emplace<Position_X>(spell, pos.fX, pos.fX, 0.0f); //needs to be set to an offset position of the unit creating the spell
		scene.emplace<Position_Y>(spell, pos.fY, pos.fY, 0.0f); //needs to be set to an offset position of the unit creating the spell


		//spell data
		scene.emplace<Radius>(spell, data.radius);
		scene.emplace<Velocity>(spell, 0.f, 0.0f, 0.0f, 0.0f, data.speed);
		scene.emplace<Mass>(spell, data.mass);


		//default data
		scene.emplace<Casted>(spell);
		scene.emplace<Spell>(spell);
		scene.emplace<Renderable>(spell);
		scene.emplace<Direction>(spell, direction); //match Direction of the caster
		scene.emplace<Alive>(spell, true);
		scene.emplace<handle>(spell, "fireball");
		Spell_Move_Target();
		//std::cout << "casted " << scene.get<handle>(spell).sName << std::endl;
	}


	void create_fireball(float& x, float& y, Compass& direction) {
		auto fireball = scene.create();
		f2d pos = { x, y };
		create_spell(fireball, pos, direction);
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

	void Clear_NonMoving_Spells() {
		auto view = scene.view<Spell>(entt::exclude<Mouse_Move>);
		for (auto entity : view)	
			scene.destroy(entity);
	}

	void Clear_Collided_Spells() {
		auto view = scene.view<Spell, Alive>();
		for (auto entity : view)
			if (view.get<Alive>(entity).bIsAlive == false) {
				scene.destroy(entity);
			}
	}
	

	void Update_Spells() {
		Clear_NonMoving_Spells();
		Clear_Collided_Spells();
		add_spells_to_scene();
	}
}