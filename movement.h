#pragma once
#include "scene.h"
#include "timer.h"





namespace Movement {
	int Player_Move_Poll;
	int Update_Position_Poll;
	int number = 0;

	void Update_Position() {
		auto group = Scene::scene.view<Position_X, Position_Y, Velocity>();
		Update_Position_Poll  -= Timer::timeStep;
		number = 0;
		if (Update_Position_Poll <= 0) {
			Update_Position_Poll = 0;
			for (auto entity : group) {
				auto& vel = group.get<Velocity>(entity);
				auto& pX = group.get<Position_X>(entity);
				auto& pY = group.get<Position_Y>(entity);
				if (vel.magnitude.fX != 0 || vel.magnitude.fY != 0) {
					number++;
					Scene::scene.emplace_or_replace<Moving>(entity); // turns on collision searching
					if (fabs(vel.magnitude.fX) < 0.01) { vel.magnitude.fX = 0; }; //clamp rounding errors
					if (fabs(vel.magnitude.fY) < 0.01) { vel.magnitude.fY = 0; };
					float angle2 = atan2f(vel.magnitude.fY, vel.magnitude.fX);
					float angle = atan2f(vel.magnitude.fX, vel.magnitude.fY);
					float velocityX = sinf(angle) * vel.speed;
					float velocityY = sinf(angle2) * vel.speed;
					vel.dX = velocityX;
					vel.dY = velocityY;
					pX.fPX += velocityX * Timer::timeStep;
					pY.fPY += velocityY * Timer::timeStep;
				}
			};
		}
		//std::cout << "moves:  " << number << std::endl;
	};



	void Update_Direction() {
		auto view = Scene::scene.view<Direction, Actions, Velocity>();
		for (auto entity : view) {
			auto& vel = view.get<Velocity>(entity);
			auto& b = view.get<Direction>(entity);
			auto& c = view.get<Actions>(entity);
			if (vel.magnitude.fX > 0) { b.eDirection = E; c.action = walk; };
			if (vel.magnitude.fX < 0) { b.eDirection = W; c.action = walk; };
			if (vel.magnitude.fY > 0) { b.eDirection = S; c.action = walk; };
			if (vel.magnitude.fY < 0) { b.eDirection = N; c.action = walk; };

			if (vel.magnitude.fX > 0 && vel.magnitude.fY > 0) { b.eDirection = SE; c.action = walk; };
			if (vel.magnitude.fX < 0 && vel.magnitude.fY > 0) { b.eDirection = SW; c.action = walk; };
			if (vel.magnitude.fY < 0 && vel.magnitude.fX < 0) { b.eDirection = NW; c.action = walk; };
			if (vel.magnitude.fY < 0 && vel.magnitude.fX > 0) { b.eDirection = NE; c.action = walk; };

			if (c.action == walk) {
				if (vel.magnitude.fX == 0 && vel.magnitude.fY == 0) {
					c.action = idle;
					Scene::scene.remove<Moving>(entity);
				};
			}
		}
	}


	void Mouse_Move_To() {
		Player_Move_Poll -= Timer::timeStep;
		if (Player_Move_Poll <= 0) {
			Player_Move_Poll = 200;
			auto view = scene.view<Position_X, Position_Y, Velocity, Mouse_Move>();
			for (auto entity : view) {	
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				auto& v = view.get<Velocity>(entity);
				auto& mov = view.get<Mouse_Move>(entity);				
				v.magnitude.fX = v.speed * (mov.fX_Destination - x.fPX);
				v.magnitude.fY = v.speed * (mov.fY_Destination - y.fPY);
			}
		}
	}
	
	
	void Mouse_Move_Arrived() {
		auto view = scene.view<Position_X, Position_Y, Velocity, Actions, Mouse_Move>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			auto& v = view.get<Velocity>(entity);
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& mov = view.get<Mouse_Move>(entity);
			if (Mouse::Inside_Cursor(x.fPX, y.fPY, mov.fX_Destination, mov.fY_Destination, 15.0f)) {
				v.magnitude.fX = 0.0f;
				v.magnitude.fY = 0.0f;
				act.action = idle;
				scene.remove<Mouse_Move>(entity);
				scene.remove<Moving>(entity);
			}
		}
	}

	void Mouse_Moving() {
		if (scene.empty<Selected>()) {
			if (Mouse::bRight_Mouse_Pressed) {
				auto view = scene.view<Input>();
				for (auto entity : view) {
					scene.emplace_or_replace<Mouse_Move>(entity);
					auto& mov = scene.get<Mouse_Move>(entity);
					mov.fX_Destination = Mouse::iXWorld_Mouse;
					mov.fY_Destination = Mouse::iYWorld_Mouse;
				}
			}
		}
	}

	void Movement_Handler() {
		Mouse_Moving();
		Mouse_Move_To();
		Mouse_Move_Arrived();
		Update_Position();
		Update_Direction();
	}

}