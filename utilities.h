#pragma once


namespace Utilities {

	void worldToScreen() {

	}

	void screenToWorld() {

	}




	bool bRect_Intersect(SDL_FRect &entity, SDL_FRect &target) {
		if ((entity.y <= target.y + target.h) &&
			(entity.x <= target.x + target.w) &&
			(entity.y + entity.h >= target.y) &&
			(entity.x + entity.w >= target.x)) {
			return true;
		}
		return false;
	};	
	




}
