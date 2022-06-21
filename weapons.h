#pragma once
#include "components.h"
#include "scenes.h"
#include "timer.h"

namespace Weapons {



	//take input of unit Radius and Weapon_Size to fill this out for any wielder and any weapon
	SDL_FRect Attack_Direction(f2d& pos, Components::Compass& direction) {
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
	}


	void create_attack(f2d& pos, Components::Compass& direction) {
		SDL_FRect attackPos = Attack_Direction(pos, direction);

		auto weapon = Scenes::scene.create();
		//Scenes::scene.emplace<Components::Weapon_Type>(weapon, sword);
		Scenes::scene.emplace<Components::Damage>(weapon, 1, 4);
		Scenes::scene.emplace<Components::Melee>(weapon);
		Scenes::scene.emplace<Components::Attack_Box_Duration>(weapon, 0, 0);
		Scenes::scene.emplace<Components::Radius>(weapon, 20.0f);
		Scenes::scene.emplace<Components::Mass>(weapon, 500.0f);
		Scenes::scene.emplace<Components::Weapon_Size>(weapon, attackPos.x, attackPos.y, attackPos.w, attackPos.h); //set x, y to in front of char when he attacks
		Scenes::scene.emplace<Components::Position_X>(weapon, pos.fX, pos.fX, 0.0f);
		Scenes::scene.emplace<Components::Position_Y>(weapon, pos.fY, pos.fY, 0.0f);
		Scenes::scene.emplace<Components::Alive>(weapon, true);
	}


	void Attack() {
		auto view = scene.view<Direction, Position_X, Position_Y, Attacking>();
		for (auto entity : view) {
			auto& dir = view.get<Direction>(entity);
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			f2d pos = { x.fX, y.fY };
			create_attack(pos, dir.eDirection);
			//std::cout << "attacking!" << std::endl;
			scene.remove<Attacking>(entity);			
		}
		//create SDL_FRect in front of unit, size from a size component on weapon
	}

	void Attack_Box_Linger() {
		auto view = scene.view<Attack_Box_Duration, Alive>();
		for (auto entity : view) {
			auto& time = view.get<Attack_Box_Duration>(entity).lifeTime;
			auto& count = view.get<Attack_Box_Duration>(entity).count;
			auto& alive = view.get<Alive>(entity).bIsAlive;
			count += Timer::timeStep;
			if (count >= time) {
				alive = false;
			}
		}
	}

	void Update_Attacks() {
		Attack_Box_Linger();
		Attack();
	}
}