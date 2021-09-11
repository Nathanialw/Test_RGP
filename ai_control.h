#pragma once
#include "scene.h"
#include "mouse_control.h"
#include "timer.h"

using namespace Scene;

namespace AI {
	int AI_Move_Poll;



	void AI_Move_To() {
		AI_Move_Poll -= Timer::timeStep;
		if (AI_Move_Poll <= 0) {
			AI_Move_Poll = 200;
			auto view = scene.view<Position_X, Position_Y, Commanded_Move, Actions>();
			for (auto entity : view) {
				scene.emplace_or_replace<Velocity>(entity);
				auto& v = scene.get<Velocity>(entity);
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				auto& mov = view.get<Commanded_Move>(entity);
				auto& act = view.get<Actions>(entity);
				v.speed = 0.175;
				float fx = mov.fX_Destination - x.fPX;
				float fy = mov.fY_Destination - y.fPY;
				//	float fDistance = sqrt((fx * fx) + (fy * fy));
				float fNormalized_x = fx;
				float fNormalized_y = fy;
				v.magnitude.fX = v.speed * fNormalized_x;
				v.magnitude.fY = v.speed * fNormalized_y;
			}
		}
	}


	void AI_Moving_To() {
		auto view = scene.view<Position_X, Position_Y, Velocity, Actions, Commanded_Move>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);			
			auto& v = scene.get<Velocity>(entity);
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& mov = view.get<Commanded_Move>(entity);
			if (Mouse::Select_Unit_With_Mouse(x.fPX, y.fPY, mov.fX_Destination, mov.fY_Destination, 15.0f)) {
				v.magnitude.fX = 0.0f;
				v.magnitude.fY = 0.0f;
				act.action = idle;
				scene.remove<Velocity>(entity);
				scene.remove<Commanded_Move>(entity);
			}
		}
	}


	void Run_AI() {
		AI_Move_To();
		AI_Moving_To();
	}


}