#pragma once
#include "scene.h"
#include "timer.h"





namespace Movement {

	void Update_Position() {
		auto group = Scene::scene.view<Position_X, Position_Y, Velocity>();
		for (auto entity : group) {
			auto& vel = group.get<Velocity>(entity);
			auto& pX = group.get<Position_X>(entity);
			auto& pY = group.get<Position_Y>(entity);
			if (vel.magnitude.fX != 0 || vel.magnitude.fY != 0) {
				Scene::scene.emplace_or_replace<Moving>(entity);
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

	void Movement_Handler() {
		Update_Position();
		Update_Direction();
	}

}