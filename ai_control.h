#pragma once
#include "scene.h"
#include "mouse_control.h"
#include "timer.h"



namespace AI {
	
	//check for targets periodically

	//use the target x,y to moiv towards it

	//if target is in range, melee attack


	

	void Move_Toward_Target(entt::entity& entity, float& x, float& y) {
		Scenes::scene.emplace_or_replace<Moving>(entity);
		Scenes::scene.emplace_or_replace<Mouse_Move>(entity, x, y);		
	}

	void Spell_Attack(entt::entity& entity, float& targetX, float& targetY, const char* name) {
	
		if (Scenes::scene.any_of<Casting>(entity) == false) { //locks out casing until cast animation has finished
			Scenes::scene.emplace_or_replace<Cast>(entity, targetX, targetY);
			Scenes::scene.emplace_or_replace<Spell_Name>(entity, name);
		}
	}

	void Melee_Attack(entt::entity& entity, float& x, float& y) {
		if (Scenes::scene.any_of<Attacking>(entity) == false) { //locks out attacking until attack animation has finished
			Scenes::scene.emplace_or_replace<Attack>(entity, x, y);
		}
	}

	void Check_For_Targets() {
		auto view = Scenes::scene.view<Sight_Range, Alive, Spellbook>();
		auto view2 = Scenes::scene.view<Position, Input>();

		for (auto entity : view) {
			auto sight = view.get<Sight_Range>(entity).sightBox;

			for (auto target : view2) {
				auto& x = view2.get<Position>(target).fX;
				auto& y = view2.get<Position>(target).fY;
				SDL_FPoint point = { x, y };
				if (Utilities::bPoint_RectIntersect(point, sight)) {
					Move_Toward_Target(entity, x, y);
					Spell_Attack(entity, x, y, "'fireball'");					
				}
			}
		}
	}

	void Update_Sight_Box() {
		auto view = Scenes::scene.view<Sight_Range, Position, Alive>();
		for (auto entity : view) {
			auto& sight = view.get<Sight_Range>(entity).sightBox;
			auto& x = view.get<Position>(entity).fX;
			auto& y = view.get<Position>(entity).fY;

			sight = { x - 250.0f, y - 250.0f, 500.0f, 500.0f };
		}
	}

	float time = 0.0f;
	bool on = false;

	void Turn_On() {
		if (on == false) {
			on = true;
		}
		else {
			on = false;
		}
	}

	void Run_AI() {
		time += Timer::timeStep;
		//std::cout << time << std::endl;
		if (time >= 1000.0f) {
			if (on) {
				time = 0.0f;
				Update_Sight_Box();
				Check_For_Targets();
			}
		}
	}
}