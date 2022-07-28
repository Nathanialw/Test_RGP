#pragma once
#include "movement.h"

namespace Spells {

	void Spell_Move_Target(entt::entity entity, float& x, float& y) { //sends spell to where the mouse is
		World::zone.emplace<Moving>(entity);
		World::zone.emplace<Mouse_Move>(entity, x, y);
		World::zone.remove<Casted>(entity);
	}

	void Spell_Linear_Target(entt::entity& entity, float& x, float& y, float& sourceX, float& sourceY) { //sends spell to where the mouse is
		World::zone.emplace<Moving>(entity);
		World::zone.emplace<Linear_Move>(entity, x, y);
		World::zone.emplace<Spell_Range>(entity, sourceX, sourceY, 0.0f);
		World::zone.remove<Casted>(entity);		
	}

	void Spell_Stack_Spells(float& x, float& y) { //sends spell to where the mouse is
		auto view = World::zone.view<Spell>();
		for (auto entity : view) {
			World::zone.emplace_or_replace<Moving>(entity);
			World::zone.emplace_or_replace<Mouse_Move>(entity, x, y);			
		}
	}

	DataTypes::f2d Spell_Direction(DataTypes::f2d& pos, Component::Compass& direction, float& scale) {
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

	void create_spell(entt::entity& entity, DataTypes::f2d& pos, Compass& direction, const char* spellname, float& targetX, float& targetY) {
		
		float scale = 1.0f;
		
		Entity_Loader::Data data = Entity_Loader::parse_data(spellname);
		DataTypes::f2d spelldir = Spell_Direction(pos, direction, scale);
 
		//rendering data
		World::zone.emplace<animation>(entity, Graphics::fireball_0); /// need to load the texture /only /once and pass the pointer into this function
		World::zone.get<animation>(entity).sheet = { //populate the vector
			{ NULL },
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, 16.0f }, //idle
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, 16.0f } //walk
		};
		World::zone.emplace<Sprite_Offset>(entity, 32.0f * scale, 32.0f * scale);
		World::zone.emplace<Scale>(entity, scale);

		World::zone.emplace<Actions>(entity, walk);
		World::zone.get<Actions>(entity).frameCount = { {0, 0}, {0, 0}, {8, 0} };
		
		//positon data
		World::zone.emplace<Position>(entity, spelldir.fX, spelldir.fY); 

		//spell data
		World::zone.emplace<Radius>(entity, data.radius * scale);
		World::zone.emplace<Velocity>(entity, 0.f, 0.0f, 0.0f, 0.0f, data.speed);
		World::zone.emplace<Mass>(entity, data.mass * scale);
		World::zone.emplace<Entity_Type>(entity, spell);
		World::zone.emplace<Damage>(entity, 1);

		//Scenes::scene.emplace<Spell_Range>(spell, 1000.0f);

		//default data
		World::zone.emplace<Spell>(entity);
		World::zone.emplace<Casted>(entity);
		World::zone.emplace<Renderable>(entity);
		World::zone.emplace<Direction>(entity, direction); //match Direction of the caster
		World::zone.emplace<Alive>(entity, true);		
		Spell_Linear_Target(entity, targetX, targetY, spelldir.fX, spelldir.fY);
		//Spell_Move_Target(spell, targetX, targetY);
		//std::cout << "casted " << scene.get<handle>(spell).sName << std::endl;
	}


	void create_fireball(float& x, float& y, Compass& direction, const char* spellname, float& targetX, float& targetY) {
		auto fireball = World::zone.create();
		DataTypes::f2d pos = { x, y };
		create_spell(fireball, pos, direction, spellname, targetX, targetY);
	}



	void cast_fireball() {
		auto view = World::zone.view<Direction, Actions, Position, Cast, Spell_Name, Velocity>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			act.action = cast;
			act.frameCount[act.action].currentFrame = 0;
			auto& target = view.get<Cast>(entity);

			auto& direction = view.get<Direction>(entity).eDirection;
			auto& x = view.get<Position>(entity).x;
			auto& y = view.get<Position>(entity).y;
			auto& angle = view.get<Velocity>(entity).angle;
	
			auto& name = view.get<Spell_Name>(entity).spell;
			//look at target
			
			direction = Movement::Look_At_Target(x, y, target.targetX, target.targetY, angle);
			//cast Fireball
			create_fireball(x, y, direction, name, target.targetX, target.targetY);		
			//set into casting mode
			World::zone.emplace_or_replace<Casting>(entity);
			World::zone.remove<Cast>(entity);			
		}
	}


	void add_spells_to_scene() {
		cast_fireball();
	}

	void Create_Explosion(float& x, float y) { //creates the explosion for fireballs
		auto explosion = World::zone.create();

		World::zone.emplace<Position>(explosion, x, y);
		World::zone.emplace<Potential_Position>(explosion, x, y);
		World::zone.emplace<Sprite_Frames>(explosion, 63, 0, 0, 0);
		World::zone.emplace<Texture>(explosion, Graphics::fireball_explosion_0, 0, 0, 128, 128);
		World::zone.emplace<Frame_Delay>(explosion, 16.0f, 0.0f);
		World::zone.emplace<Explosion>(explosion, 0, 0, 0.0f, 0.0f, 128.0f, 128.0f, 64.0f, 100.0f);
	}


	void Destroy_NonMoving_Spells() {
		auto view = World::zone.view<Spell, Position>(entt::exclude<Mouse_Move, Linear_Move, Explosion>);
		for (auto entity : view) {
			auto& x = view.get<Position>(entity).x;
			auto& y = view.get<Position>(entity).y;
			
			//create explosion
			Create_Explosion(x, y);
			//destroy spell
			World::zone.destroy(entity);
		}
	}

	void Clear_Collided_Spells() {
		auto view = World::zone.view<Spell, Position, Alive>();
		for (auto entity : view)
		if (view.get<Alive>(entity).bIsAlive == false) {
			auto& x = view.get<Position>(entity).x;
			auto& y = view.get<Position>(entity).y;

			//create explosion
			Create_Explosion(x, y);
			//destroy spell
			World::zone.destroy(entity);
		}
	}
	
	void Casting_Updater() {
		auto view = World::zone.view<Casting, Actions>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			//std::cout << act.frameCount[act.action].currentFrame << "/" << act.frameCount[act.action].NumFrames <<std::endl;
			if (act.action != dead) {
				if (act.frameCount[act.action].currentFrame == act.frameCount[act.action].NumFrames) {
					act.action = idle;
					World::zone.remove<Casting>(entity);
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