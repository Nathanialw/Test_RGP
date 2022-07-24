#pragma once
#include "components.h"
#include "world.h"
#include "timer.h"
#include "movement.h"

namespace Weapons {

	//take input of unit Radius and Weapon_Size to fill this out for any wielder and any weapon
	SDL_FRect Attack_Direction(DataTypes::f2d& pos, Components::Compass& direction) {
		switch (direction) {
		case N: return { pos.fX - 15, pos.fY - 55, 30, 40 };
		case S: return { pos.fX - 15, pos.fY + 15, 30, 40 };
		case E: return { pos.fX + 15, pos.fY - 15, 40, 30 };
		case W: return { pos.fX - 55, pos.fY - 15, 40, 30 };
		case NW :return { pos.fX - 45, pos.fY - 45, 30, 30 };
		case NE :return { pos.fX + 15, pos.fY - 45, 30, 30 };
		case SW :return { pos.fX - 45, pos.fY + 15, 30, 30 };
		case SE :return { pos.fX + 15, pos.fY + 15, 30, 30 };
		}
		std::cout << "Weapons::Attack_Direction() passthrough error" << std::endl;
		return {0,0,0,0};
	}

	SDL_FRect Check_For_Target(float &x, float &y, Components::Compass& direction, float &radius) {
		switch (direction) {
		case N: return { x - radius, y - 55,    30, 40 };
		case S: return { x - radius, y + radius, 30, 40 };
		case E: return { x + radius, y - radius, 40, 30 };
		case W: return { x - 55,     y - radius, 40, 30 };
		case NW:return { x - 45,     y - 45,    30, 30 };
		case NE:return { x + radius, y - 45,    30, 30 };
		case SW:return { x - 45,     y + radius, 30, 30 };
		case SE:return { x + radius, y + radius, 30, 30 };
		}
		std::cout << "Weapons::Check_For_Target() passthrough error" << std::endl;
		return { 0,0,0,0 };
	}

	void create_attack(DataTypes::f2d& pos, Components::Compass& direction) {
		SDL_FRect attackPos = Attack_Direction(pos, direction);

		auto weapon = World::zone.create();
		//Scenes::scene.emplace<Components::Weapon_Type>(weapon, sword);
		World::zone.emplace<Components::Damage>(weapon, 1, 4);
		World::zone.emplace<Components::Melee>(weapon);
		World::zone.emplace<Components::Attack_Box_Duration>(weapon, 0, 0);
		World::zone.emplace<Components::Radius>(weapon, 20.0f);
		World::zone.emplace<Components::Mass>(weapon, 500.0f);
		World::zone.emplace<Components::Weapon_Size>(weapon, attackPos.x, attackPos.y, attackPos.w, attackPos.h); //set x, y to in front of char when he attacks
		World::zone.emplace<Components::Position>(weapon, pos.fX, pos.fY);
		World::zone.emplace<Components::Potential_Position>(weapon, pos.fX, pos.fY);
		World::zone.emplace<Components::Alive>(weapon, true);
	}

	void Attack_cast() {
		auto view = World::zone.view<Direction, Position, Actions, Attack, Velocity>();
		for (auto entity : view) {
		//	act.action = slash;
			auto& act = view.get<Actions>(entity);
			act.action = slash;
			act.frameCount[act.action].currentFrame = 0;
			auto& dir = view.get<Direction>(entity);
			auto& x = view.get<Position>(entity).fX;
			auto& y = view.get<Position>(entity).fY;
			auto& angle = view.get<Velocity>(entity).angle;
			auto& target = view.get<Attack>(entity);
			
			DataTypes::f2d pos = { x, y };
			
			dir.eDirection = Movement::Look_At_Target(x, y, target.targetX, target.targetY, angle);
			create_attack(pos, dir.eDirection);	
			//std::cout << dir.eDirection << std::endl;
			World::zone.emplace_or_replace<Attacking>(entity);
			World::zone.remove<Attack>(entity);
		}
		//create SDL_FRect in front of unit, size from a size component on weapon
	}

	void Attack_Box_Destroy() {
		auto view = World::zone.view<Attack_Box_Duration>();
		for (auto entity : view) {
			auto& time = view.get<Attack_Box_Duration>(entity).lifeTime;
			auto& count = view.get<Attack_Box_Duration>(entity).count;
			count += Timer::timeStep;
			if (count >= time) {
				World::zone.destroy(entity);
			}
		}
	}


	void Attacking_Updater() {
		auto view = World::zone.view<Attacking, Actions>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			//std::cout << act.frameCount[act.action].currentFrame << "/" << act.frameCount[act.action].NumFrames <<std::endl;
			if (act.action != dead) {
				if (act.frameCount[act.action].currentFrame == act.frameCount[act.action].NumFrames) {
					act.action = idle;
					World::zone.remove<Attacking>(entity);
				}
			}
		}
	}

	void Update_Attacks() {
		
		Attack_Box_Destroy();
		Attacking_Updater();
		Attack_cast();
	}
}