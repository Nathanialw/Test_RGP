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
	
	bool bPoint_RectIntersect(SDL_FPoint &p, SDL_FRect &r) {		
		return ((p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h))) ? SDL_TRUE : SDL_FALSE;
	};	
	

	SDL_Rect SDL_FRect_To_SDL_Rect(SDL_FRect a) {
		SDL_Rect b;
		b.x = (int)a.x;
		b.y = (int)a.y;
		b.w = (int)a.w;
		b.h = (int)a.h;
		return b;
	}

	SDL_FRect SDL_Rect_To_SDL_FRect(SDL_Rect a) {
		SDL_FRect b;
		b.x = (float)a.x;
		b.y = (float)a.y;
		b.w = (float)a.w;
		b.h = (float)a.h;
		return b;
	}

}
