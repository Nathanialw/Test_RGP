#pragma once
#include "scene.h"
#include "timer.h"
#include "ai_control.h"




namespace Movement {
	
	namespace {
		int64_t Player_Move_Poll = 0;
		int64_t Update_Position_Poll = 0;
		int64_t linearMovePoll = 0;
		int number_of_units = 0;
	}



	void Mouse_Moving() { // maybe change to move and attack?
		if (Scenes::scene.empty<Selected>()) {
			if (Mouse::bRight_Mouse_Pressed) {
				auto view = Scenes::scene.view<Input>();
				for (auto entity : view) {
					AI::Move_Order(entity, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
				}
			}
		}
	}




	
	void Update_Position() {
		auto view = Scenes::scene.view<Potential_Position, Velocity>();
		Update_Position_Poll += Timer::timeStep;
		number_of_units = 0;
		float angleY = 0.0f;
		//std::cout << Update_Position_Poll << std::endl;
		if (Update_Position_Poll >= 20) {
			for (auto entity : view) {
				auto& vel = view.get<Velocity>(entity);
				auto& pX = view.get<Potential_Position>(entity);
				auto& pY = view.get<Potential_Position>(entity);
				if (vel.magnitude.fX != 0 || vel.magnitude.fY != 0) {
					number_of_units++;
					if (fabs(vel.magnitude.fX) < 0.01) { vel.magnitude.fX = 0; }; //clamp rounding errors
					if (fabs(vel.magnitude.fY) < 0.01) { vel.magnitude.fY = 0; };
					vel.angle = atan2f(vel.magnitude.fX, vel.magnitude.fY);
					angleY = atan2f(vel.magnitude.fY, vel.magnitude.fX);
					float velocityX = sinf(vel.angle) * vel.speed;
					float velocityY = sinf(angleY) * vel.speed;
					vel.dX = velocityX;
					vel.dY = velocityY;
					pX.fPX += velocityX * Update_Position_Poll;
					pY.fPY += velocityY * Update_Position_Poll;
				}
			}
			Update_Position_Poll = 0;
		}
	};

	Compass Set_Direction(float angleInRadians) {
		
		if      (angleInRadians > 2.74889    || angleInRadians <= (-2.74889)) { return N; }
		else if (angleInRadians > 1.96349    && angleInRadians <= (2.74889))  { return NE; }
		else if (angleInRadians > 1.17809 	 && angleInRadians <= (1.96349))  { return E; }
		else if (angleInRadians > 0.39269	 && angleInRadians <= (1.17809))  { return SE; }
		else if (angleInRadians > (-0.39269) && angleInRadians <= (0.39269))  { return S; }
		else if (angleInRadians > (-1.17811) && angleInRadians <= (-0.39269)) { return SW; }
		else if (angleInRadians > (-1.96351) && angleInRadians <= (-1.17811)) { return W; }
		else if (angleInRadians > (-2.74889) && angleInRadians <= (-1.96351)) { return NW; }
	}

	Compass Look_At_Target(float& positionX, float& positionY, float& targetX, float& targetY, float& angleInRadians) {
		angleInRadians = Utilities::Get_Direction_Point(positionX, positionY, targetX, targetY);
		return Set_Direction(angleInRadians);
	}

	void Update_Direction() {
		auto view = Scenes::scene.view<Direction, Actions, Velocity, Moving>();
		for (auto entity : view) {
			auto& vel = view.get<Velocity>(entity);
			auto& b = view.get<Direction>(entity);
			auto& c = view.get<Actions>(entity);

			b.eDirection = Set_Direction(vel.angle);

			if (c.action == walk) {
				if (vel.magnitude.fX == 0 && vel.magnitude.fY == 0) {
					c.action = idle;
				};
			}
		}
	}

	void Mouse_Move_To() { //calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
		Player_Move_Poll += Timer::timeStep;
		if (Player_Move_Poll >= 200) {
			Player_Move_Poll = 0;
			auto view = Scenes::scene.view<Potential_Position, Velocity, Mouse_Move, Actions, Moving>();
			for (auto entity : view) {	
				auto& x = view.get<Potential_Position>(entity);
				auto& y = view.get<Potential_Position>(entity);
				auto& act = view.get<Actions>(entity);
				auto& v = view.get<Velocity>(entity);
				auto& mov = view.get<Mouse_Move>(entity);				
				act.action = walk;
				v.magnitude.fX = v.speed * (mov.fX_Destination - x.fPX);
				v.magnitude.fY = v.speed * (mov.fY_Destination - y.fPY);
			}
		}
	}

	//takes in the x,y of the unit and checks if it is within the destination x,y within an accuracy of the set variable
	bool Check_If_Arrived(float &unitX, float &unitY, float &destinationX, float &destinationY) {
		float accuracy = 5.0f;
		if (unitX + accuracy > destinationX &&
			unitX - accuracy < destinationX &&
			unitY - accuracy < destinationY &&
			unitY + accuracy > destinationY) {
			return true;			
		}
		else {
			return false;
		}
	}
		
	void Mouse_Move_Arrived() {
		auto view = Scenes::scene.view<Potential_Position, Velocity, Actions, Mouse_Move>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			auto& v = view.get<Velocity>(entity);
			auto& x = view.get<Potential_Position>(entity);
			auto& y = view.get<Potential_Position>(entity);
			auto& mov = view.get<Mouse_Move>(entity);
			if (Check_If_Arrived(x.fPX, y.fPY, mov.fX_Destination, mov.fY_Destination)) {
				v.magnitude.fX = 0.0f;
				v.magnitude.fY = 0.0f;
				act.action = idle;
				Scenes::scene.remove<Mouse_Move>(entity);
				Scenes::scene.remove<Moving>(entity);
			}
		}
	}




	void Linear_Move_To() {
		linearMovePoll += Timer::timeStep;
		if (linearMovePoll >= 50) {
			auto view = Scenes::scene.view<Potential_Position, Velocity, Actions, Moving, Linear_Move, Spell_Range>();
			for (auto entity : view) {
				auto& x = view.get<Potential_Position>(entity);
				auto& y = view.get<Potential_Position>(entity);
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
		//Mouse_Attack_Move(); //runs every frame to see if mouse is down, if it is it moves you to the new location
		Linear_Move_To();
		Mouse_Move_To();
		Mouse_Move_Arrived();
		Update_Position();
		Update_Direction();
	}

}