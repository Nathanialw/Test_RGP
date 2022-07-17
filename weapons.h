#pragma once
#include "components.h"
#include "scenes.h"
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

	void create_attack(DataTypes::f2d& pos, Components::Compass& direction) {
		SDL_FRect attackPos = Attack_Direction(pos, direction);

		auto weapon = Scenes::scene.create();
		//Scenes::scene.emplace<Components::Weapon_Type>(weapon, sword);
		Scenes::scene.emplace<Components::Damage>(weapon, 1, 4);
		Scenes::scene.emplace<Components::Melee>(weapon);
		Scenes::scene.emplace<Components::Attack_Box_Duration>(weapon, 0, 0);
		Scenes::scene.emplace<Components::Radius>(weapon, 20.0f);
		Scenes::scene.emplace<Components::Mass>(weapon, 500.0f);
		Scenes::scene.emplace<Components::Weapon_Size>(weapon, attackPos.x, attackPos.y, attackPos.w, attackPos.h); //set x, y to in front of char when he attacks
		Scenes::scene.emplace<Components::Position_X>(weapon, pos.fX, pos.fX);
		Scenes::scene.emplace<Components::Position_Y>(weapon, pos.fY, pos.fY);
		Scenes::scene.emplace<Components::Alive>(weapon, true);
	}

	void Attack_cast() {
		auto view = Scenes::scene.view<Direction, Position_X, Position_Y, Actions, Attack, Velocity>();
		for (auto entity : view) {
		//	act.action = slash;
			auto& act = view.get<Actions>(entity);
			act.action = slash;
			act.frameCount[act.action].currentFrame = 0;
			auto& dir = view.get<Direction>(entity);
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& angle = view.get<Velocity>(entity).angle;
			auto& target = view.get<Attack>(entity);

			DataTypes::f2d pos = { x.fX, y.fY };
			
			dir.eDirection = Movement::Look_At_Target(x.fX, y.fY, target.targetX, target.targetY, angle);
			create_attack(pos, dir.eDirection);	
			//std::cout << dir.eDirection << std::endl;
			Scenes::scene.emplace_or_replace<Attacking>(entity);
			Scenes::scene.remove<Attack>(entity);
		}
		//create SDL_FRect in front of unit, size from a size component on weapon
	}

	void Attack_Box_Destroy() {
		auto view = Scenes::scene.view<Attack_Box_Duration>();
		for (auto entity : view) {
			auto& time = view.get<Attack_Box_Duration>(entity).lifeTime;
			auto& count = view.get<Attack_Box_Duration>(entity).count;
			count += Timer::timeStep;
			if (count >= time) {
				Scenes::scene.destroy(entity);
			}
		}
	}

	//void Attacking_Updater() {
	//	auto view = Scenes::scene.view<Attacking, Actions>();
	//	for (auto entity : view) {
	//		auto &act = view.get<Actions>(entity);
	//		//std::cout << act.frameCount[act.action].currentFrame << "/" << act.frameCount[act.action].NumFrames <<std::endl;
	//		if (act.frameCount[act.action].currentFrame == act.frameCount[act.action].NumFrames) {
	//			act.action = idle;
	//			Scenes::scene.remove<Attacking>(entity);
	//		}
	//	}
	//}

	void Attacking_Updater() {
		auto view = Scenes::scene.view<Attacking, Actions>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			//std::cout << act.frameCount[act.action].currentFrame << "/" << act.frameCount[act.action].NumFrames <<std::endl;
			if (act.action != dead) {
				if (act.frameCount[act.action].currentFrame == act.frameCount[act.action].NumFrames) {
					act.action = idle;
					Scenes::scene.remove<Attacking>(entity);
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