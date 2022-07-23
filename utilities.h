#pragma once


namespace Utilities {

	void Log(const std::string &string) {
		std::cout << string << std::endl;
	}

	SDL_FRect worldToScreen(SDL_FRect& Rect, SDL_FRect &camera) {
		SDL_FRect screenRect = {};

		screenRect.x = Rect.x + camera.x;
		screenRect.y = Rect.y + camera.y;
		
		return screenRect;
	}

	SDL_FRect screenToWorld(SDL_FRect &Rect, SDL_FRect &camera) {
		SDL_FRect screenRect = {};
	
		screenRect.x = Rect.x - camera.x;
		screenRect.y = Rect.y - camera.y;
	
		return screenRect;
	}

	float Get_Hypotenuse(float& x, float &y) {
		return sqrtf((x * x) + (y + y));
	}


	bool bFRect_Intersect(SDL_FRect &entity, SDL_FRect &target) {
		if ((entity.y <= target.y + target.h) &&
			(entity.x <= target.x + target.w) &&
			(entity.y + entity.h >= target.y) &&
			(entity.x + entity.w >= target.x)) {
			return true;
		}
		return false;
	};	

	bool bRect_Intersect(SDL_Rect& entity, SDL_Rect& target) {
		if ((entity.y <= target.y + target.h) &&
			(entity.x <= target.x + target.w) &&
			(entity.y + entity.h >= target.y) &&
			(entity.x + entity.w >= target.x)) {
			return true;
		}
		return false;
	};

	bool bFPoint_FRectIntersect(SDL_FPoint &p, SDL_FRect &r) {		
		return ((p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h))) ? SDL_TRUE : SDL_FALSE;
	};	
	
	bool bPoint_RectIntersect(SDL_Point& p, SDL_Rect& r) {
		return ((p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h))) ? SDL_TRUE : SDL_FALSE;
	};

	SDL_Rect SDL_FRect_To_SDL_Rect(SDL_FRect& a) {
		SDL_Rect b = {};
		b.x = (int)a.x;
		b.y = (int)a.y;
		b.w = (int)a.w;
		b.h = (int)a.h;
		return b;
	}

	SDL_FRect SDL_Rect_To_SDL_FRect(SDL_Rect& a) {
		SDL_FRect b = {};
		b.x = (float)a.x;
		b.y = (float)a.y;
		b.w = (float)a.w;
		b.h = (float)a.h;
		return b;
	}

	float Get_Direction_Point(float &sourceX, float &sourceY, float& targetX, float& targetY) {
		float a = targetX - sourceX;
		float b = targetY - sourceY;

		return atan2f(a, b);
	}

	SDL_FRect Get_FRect_From_Point_Radius(float &radius, float &x, float &y) {
		float rectX = x - radius;
		float rectY = y - radius;
		float width = radius * 2.0f;
		float height = radius * 2.0f;

		SDL_FRect rect = { rectX, rectY, width , height };
		return rect;
	}

	

}
