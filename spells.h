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

	DataTypes::f2d Spell_Direction(DataTypes::f2d& pos, Components::Compass& direction, float& scale) {
		switch (direction) {
		case N: return { pos.fX, pos.fY - (20.0f * scale) };
		case S: return { pos.fX, pos.fY + (20.0f * scale) };
		case E: return { pos.fX + (20.0f * scale) , pos.fY };
		case W: return { pos.fX - (20.0f * scale) , pos.fY };
		case NW: return { pos.fX - (20.0f * scale) , pos.fY - (20.0f * scale)  };
		case NE: return { pos.fX + (20.0f * scale) , pos.fY - (20.0f * scale)  };
		case SW: return { pos.fX - (20.0f * scale) , pos.fY + (20.0f * scale)  };
		case SE: return { pos.fX + (20.0f * scale) , pos.fY + (20.0f * scale)  };
		}
	}

	void create_spell(entt::entity& spell, DataTypes::f2d& pos, Compass& direction, const char* spellname, float& targetX, float& targetY) {
		
		float scale = 1.0f;
		
		Entity_Loader::Data data = Entity_Loader::parse_data(spellname);
		DataTypes::f2d spelldir = Spell_Direction(pos, direction, scale);
 
		//rendering data
		Scenes::scene.emplace<animation>(spell, Graphics::fireball_0); /// need to load the texture /only /once and pass the pointer into this function
		Scenes::scene.get<animation>(spell).sheet = { //populate the vector
			{ NULL },
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, 16.0f }, //idle
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, 16.0f } //walk
		};
		Scenes::scene.emplace<Sprite_Offset>(spell, 32.0f * scale, 32.0f * scale);
		Scenes::scene.emplace<Scale>(spell, scale);

		Scenes::scene.emplace<Actions>(spell, walk);
		Scenes::scene.get<Actions>(spell).frameCount = { {0, 0}, {0, 0}, {8, 0} };
		
		//positon data
		Scenes::scene.emplace<Position_X>(spell, spelldir.fX, spelldir.fX); 
		Scenes::scene.emplace<Position_Y>(spell, spelldir.fY, spelldir.fY); 

		//spell data
		Scenes::scene.emplace<Radius>(spell, data.radius * scale);
		Scenes::scene.emplace<Velocity>(spell, 0.f, 0.0f, 0.0f, 0.0f, data.speed);
		Scenes::scene.emplace<Mass>(spell, data.mass * scale);
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
		auto view = Scenes::scene.view<Direction, Actions, Position_X, Position_Y, Cast, Spell_Name, Velocity>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			act.action = cast;
			act.frameCount[act.action].currentFrame = 0;
			auto& target = view.get<Cast>(entity);

			auto& direction = view.get<Direction>(entity).eDirection;
			auto& x = view.get<Position_X>(entity).fX;
			auto& y = view.get<Position_Y>(entity).fY;
			auto& angle = view.get<Velocity>(entity).angle;
	
			auto& name = view.get<Spell_Name>(entity).spell;
			//look at target
			direction = Movement::Look_At_Target(x, y, target.targetX, target.targetY, angle);
			//cast Fireball
			create_fireball(x, y, direction, name, target.targetX, target.targetY);		
			//set into casting mode
			Scenes::scene.emplace_or_replace<Casting>(entity);
			Scenes::scene.remove<Cast>(entity);			
		}
	}


	void add_spells_to_scene() {
		cast_fireball();
	}

	void Create_Explosion(float& x, float y) { //creates the explosion for fireballs
		auto explosion = Scenes::scene.create();

		Scenes::scene.emplace<Position_X>(explosion, x, x);
		Scenes::scene.emplace<Position_Y>(explosion, y, y);
		Scenes::scene.emplace<Sprite_Frames>(explosion, 15, 0);
		Scenes::scene.emplace<Texture>(explosion, Graphics::fireball_explosion_0, 0, 0, 128, 128);
		Scenes::scene.emplace<Frame_Delay>(explosion, 100.0f, 0.0f);
		Scenes::scene.emplace<Explosion>(explosion, 0, 0, 0.0f, 0.0f, 128.0f, 128.0f, 64.0f, 100.0f);
	}


	void Destroy_NonMoving_Spells() {
		auto view = Scenes::scene.view<Spell, Position_X, Position_Y>(entt::exclude<Mouse_Move, Linear_Move, Explosion>);
		for (auto entity : view) {
			auto& x = view.get<Position_X>(entity).fX;
			auto& y = view.get<Position_Y>(entity).fY;
			
			//create explosion
			Create_Explosion(x, y);
			//destroy spell
			Scenes::scene.destroy(entity);
		}
	}

	void Clear_Collided_Spells() {
		auto view = Scenes::scene.view<Spell, Position_X, Position_Y, Alive>();
		for (auto entity : view)
		if (view.get<Alive>(entity).bIsAlive == false) {
			auto& x = view.get<Position_X>(entity).fX;
			auto& y = view.get<Position_Y>(entity).fY;

			//create explosion
			Create_Explosion(x, y);
			//destroy spell
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
		
		Destroy_NonMoving_Spells();
		Clear_Collided_Spells();
		Casting_Updater();
		add_spells_to_scene();
	}
}