#pragma once
#include "movement.h"

namespace Spells {

	void Spell_Move_Target(entt::entity entity, float& x, float& y) { //sends spell to where the mouse is
		Scenes::scene.emplace<Moving>(entity);
		Scenes::scene.emplace<Mouse_Move>(entity, x, y);
		Scenes::scene.remove<Casted>(entity);
	}

	void Spell_Linear_Target(entt::entity& entity, float& x, float& y, float& sourceX, float& sourceY) { //sends spell to where the mouse is
		Scenes::scene.emplace<Moving>(entity);
		Scenes::scene.emplace<Linear_Move>(entity, x, y);
		Scenes::scene.emplace<Spell_Range>(entity, sourceX, sourceY, 0.0f);
		Scenes::scene.remove<Casted>(entity);		
	}

	void Spell_Stack_Spells(float& x, float& y) { //sends spell to where the mouse is
		auto view = Scenes::scene.view<Spell>();
		for (auto entity : view) {
			Scenes::scene.emplace_or_replace<Moving>(entity);
			Scenes::scene.emplace_or_replace<Mouse_Move>(entity, x, y);			
		}
	}

	DataTypes::f2d Spell_Direction(DataTypes::f2d& pos, Components::Compass& direction) {
		switch (direction) {
		case N: return { pos.fX, pos.fY - 25.0f };
		case S: return { pos.fX, pos.fY + 25.0f };
		case E: return { pos.fX + 25.0f, pos.fY };
		case W: return { pos.fX - 25.0f, pos.fY };
		case NW:return { pos.fX - 25.0f, pos.fY - 25.0f };
		case NE:return { pos.fX + 25.0f, pos.fY - 25.0f };
		case SW:return { pos.fX - 25.0f, pos.fY + 25.0f };
		case SE:return { pos.fX + 25.0f, pos.fY + 25.0f };
		}
	}

	void create_spell(entt::entity& spell, DataTypes::f2d& pos, Compass& direction, const char* spellname, float& targetX, float& targetY) {
		Entity_Loader::Data data = Entity_Loader::parse_data(spellname);
		DataTypes::f2d spelldir = Spell_Direction(pos, direction);
 
		//rendering data
		Scenes::scene.emplace<animation>(spell, Graphics::fireball_0); /// need to load the texture /only /once and pass the pointer into this function
		Scenes::scene.get<animation>(spell).sheet = { //populate the vector
			{ NULL },
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, { 32, 32 }, 16.0f }, //idle
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, { 32, 32 }, 16.0f } //walk
		};
		Scenes::scene.emplace<Actions>(spell, walk);
		Scenes::scene.get<Actions>(spell).frameCount = { {0, 0}, {0, 0}, {8, 0} };
		
		//positon data
		Scenes::scene.emplace<Position_X>(spell, spelldir.fX, spelldir.fX); 
		Scenes::scene.emplace<Position_Y>(spell, spelldir.fY, spelldir.fY); 

		//spell data
		Scenes::scene.emplace<Radius>(spell, data.radius);
		Scenes::scene.emplace<Velocity>(spell, 0.f, 0.0f, 0.0f, 0.0f, data.speed);
		Scenes::scene.emplace<Mass>(spell, data.mass);
		//Scenes::scene.emplace<Spell_Range>(spell, 1000.0f);

		//default data
		Scenes::scene.emplace<Spell>(spell);
		Scenes::scene.emplace<Casted>(spell);
		Scenes::scene.emplace<Renderable>(spell);
		Scenes::scene.emplace<Direction>(spell, direction); //match Direction of the caster
		Scenes::scene.emplace<Alive>(spell, true);		
		Spell_Linear_Target(spell, targetX, targetY, spelldir.fX, spelldir.fY);
		//Spell_Move_Target(spell, targetX, targetY);
		//std::cout << "casted " << scene.get<handle>(spell).sName << std::endl;
	}


	void create_fireball(float& x, float& y, Compass& direction, const char* spellname, float& targetX, float& targetY) {
		auto fireball = Scenes::scene.create();
		DataTypes::f2d pos = { x, y };
		create_spell(fireball, pos, direction, spellname, targetX, targetY);
	}

	void cast_fireball() {
		auto view = Scenes::scene.view<Direction, Actions, Position_X, Position_Y, Cast, Spell_Name>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			act.action = slash;
			act.frameCount[act.action].currentFrame = 0;
			auto& target = view.get<Cast>(entity);

			auto& dir = view.get<Direction>(entity);
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
	
			auto& name = view.get<Spell_Name>(entity);

			create_fireball(x.fX, y.fY, dir.eDirection, name.spell, target.targetX, target.targetY);		

			Scenes::scene.emplace_or_replace<Casting>(entity);
			Scenes::scene.remove<Cast>(entity);			
		}
	}


	void add_spells_to_scene() {
		cast_fireball();
	}

	void Clear_NonMoving_Spells() {
		auto view = Scenes::scene.view<Spell>(entt::exclude<Mouse_Move, Linear_Move>);
		for (auto entity : view)	
			Scenes::scene.destroy(entity);
	}

	void Clear_Collided_Spells() {
		auto view = Scenes::scene.view<Spell, Alive>();
		for (auto entity : view)
		if (view.get<Alive>(entity).bIsAlive == false) {
			Scenes::scene.destroy(entity);
		}
	}
	
	void Casting_Updater() {
		auto view = Scenes::scene.view<Casting, Actions>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			//std::cout << act.frameCount[act.action].currentFrame << "/" << act.frameCount[act.action].NumFrames <<std::endl;
			if (act.action != dead) {
				if (act.frameCount[act.action].currentFrame == act.frameCount[act.action].NumFrames) {
					act.action = idle;
					Scenes::scene.remove<Casting>(entity);
				}
			}
		}
	}

	void Update_Spells() {
		Clear_NonMoving_Spells();
		Clear_Collided_Spells();
		Casting_Updater();
		add_spells_to_scene();
	}
}