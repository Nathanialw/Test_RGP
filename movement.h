#pragma once
#include "scene.h"
#include "timer.h"





namespace Movement {
	namespace {
		int Player_Move_Poll;
		int Update_Position_Poll;
		int number_of_units = 0;
	}

	void Move_Order(entt::entity& entity, float& x, float& y) {
		Scenes::scene.emplace_or_replace<Mouse_Move>(entity, x, y);
		Scenes::scene.emplace_or_replace<Moving>(entity);
	}

	void Mouse_Order_Move() {
		if (Scenes::scene.empty<Selected>()) {
			if (Mouse::bRight_Mouse_Pressed) {
				auto view = Scenes::scene.view<Input>();
				for (auto entity : view) {
					Move_Order(entity, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
				}
			}
		}
	}
	
	void Update_Position() {
		auto view = Scenes::scene.view<Position_X, Position_Y, Velocity>();
		Update_Position_Poll += Timer::timeStep;
		number_of_units = 0;
		//std::cout << Update_Position_Poll << std::endl;
		if (Update_Position_Poll >= 20) {
			for (auto entity : view) {
				auto& vel = view.get<Velocity>(entity);
				auto& pX = view.get<Position_X>(entity);
				auto& pY = view.get<Position_Y>(entity);
				if (vel.magnitude.fX != 0 || vel.magnitude.fY != 0) {
					number_of_units++;
					if (fabs(vel.magnitude.fX) < 0.01) { vel.magnitude.fX = 0; }; //clamp rounding errors
					if (fabs(vel.magnitude.fY) < 0.01) { vel.magnitude.fY = 0; };
					vel.angle1 = atan2f(vel.magnitude.fX, vel.magnitude.fY);
					vel.angle2 = atan2f(vel.magnitude.fY, vel.magnitude.fX);
					float velocityX = sinf(vel.angle1) * vel.speed;
					float velocityY = sinf(vel.angle2) * vel.speed;
					vel.dX = velocityX;
					vel.dY = velocityY;
					pX.fPX += velocityX * Update_Position_Poll;
					pY.fPY += velocityY * Update_Position_Poll;
				}
			}
			Update_Position_Poll = 0;
		}
	};

	void Update_Direction() {
		auto view = Scenes::scene.view<Direction, Actions, Velocity, Moving>();
		for (auto entity : view) {
			auto& vel = view.get<Velocity>(entity);
			auto& b = view.get<Direction>(entity);
			auto& c = view.get<Actions>(entity);
			if (vel.angle1 > 2.74889 || vel.angle1 <= (-2.74889)) { b.eDirection = N;}
			else if (vel.angle1 > 1.96349 && vel.angle1 <= (2.74889)) { b.eDirection = NE; }
			else if (vel.angle1 > 1.17809 && vel.angle1 <= (1.96349)) { b.eDirection = E;}
			else if (vel.angle1 > 0.39269 && vel.angle1 <= (1.17809)) { b.eDirection = SE; }

			else if (vel.angle1 > (-0.39269) && vel.angle1 <= (0.39269)) { b.eDirection = S; }
			else if (vel.angle1 > (-1.17811) && vel.angle1 <= (-0.39269)) { b.eDirection = SW; }
			else if (vel.angle1 > (-1.96351) && vel.angle1 <= (-1.17811)) { b.eDirection = W; }
			else if (vel.angle1 > (-2.74889) && vel.angle1 <= (-1.96351)) { b.eDirection = NW; }

			if (c.action == walk) {
				if (vel.magnitude.fX == 0 && vel.magnitude.fY == 0) {
					c.action = idle;
				//	scene.remove<Moving>(entity);
				};
			}
		}
	}

	void Mouse_Move_To() { //calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
		Player_Move_Poll += Timer::timeStep;
		if (Player_Move_Poll >= 200) {
			Player_Move_Poll = 0;
			auto view = Scenes::scene.view<Position_X, Position_Y, Velocity, Mouse_Move, Actions, Moving>();
			for (auto entity : view) {	
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				auto& act = view.get<Actions>(entity);
				auto& v = view.get<Velocity>(entity);
				auto& mov = view.get<Mouse_Move>(entity);				
				act.action = walk;
				v.magnitude.fX = v.speed * (mov.fX_Destination - x.fPX);
				v.magnitude.fY = v.speed * (mov.fY_Destination - y.fPY);
			}
		}
	}
		
	void Mouse_Move_Arrived() {
		auto view = Scenes::scene.view<Position_X, Position_Y, Velocity, Actions, Mouse_Move>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			auto& v = view.get<Velocity>(entity);
			auto& x = view.get<Position_X>(entity);
			auto& y = view.get<Position_Y>(entity);
			auto& mov = view.get<Mouse_Move>(entity);
			if (Mouse::Inside_Cursor(x.fPX, y.fPY, mov.fX_Destination, mov.fY_Destination, Mouse::Cursor_Size)) {
				v.magnitude.fX = 0.0f;
				v.magnitude.fY = 0.0f;
				act.action = idle;
				Scenes::scene.remove<Mouse_Move>(entity);
				Scenes::scene.remove<Moving>(entity);
			}
		}
	}


	int linearMovePoll = 0;

	void Linear_Move_To() {
		linearMovePoll += Timer::timeStep;
		if (linearMovePoll >= 50) {
			auto view = Scenes::scene.view<Position_X, Position_Y, Velocity, Actions, Moving, Linear_Move, Spell_Range>();
			for (auto entity : view) {
				auto& x = view.get<Position_X>(entity);
				auto& y = view.get<Position_Y>(entity);
				auto& act = view.get<Actions>(entity);
				auto& v = view.get<Velocity>(entity);
				auto& mov = view.get<Linear_Move>(entity);
				auto& range = view.get<Spell_Range>(entity);
				
				if (range.fRange <= 1500) {
					act.action = walk;
					v.magnitude.fX = v.speed * (mov.fX_Direction - range.fSourceX);
					v.magnitude.fY = v.speed * (mov.fY_Direction - range.fSourceY);
					range.fRange += linearMovePoll;
					
				}
				else {
					Scenes::scene.remove<Linear_Move>(entity);
				}
			}
			linearMovePoll = 0;
		}
	}

	void Movement_Handler() {
		Mouse_Order_Move(); //runs every frame to see if mouse is down, if it is it moves you to the new location
		Linear_Move_To();
		Mouse_Move_To();
		Mouse_Move_Arrived();
		Update_Position();
		Update_Direction();
	}

}