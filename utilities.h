#pragma once


namespace Utilities {

	void worldToScreen() {

	}

	void screenToWorld() {

	}




	bool Inside_Mousebox(SDL_Rect entity, SDL_Rect target) {
		if ((entity.y <= target.y + target.h) &&
			(entity.x <= target.x + target.w) &&
			(entity.y + entity.h >= target.y) &&
			(entity.x + entity.w >= target.x)) {
			return true;
		}
		return false;
	};



}
