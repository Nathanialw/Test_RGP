#pragma once



namespace Mouse {
	namespace {
		float cursorSize = 5.0f;
		int Mouse_Selection_Box_x;
		int Mouse_Selection_Box_y;
	}
	bool bRight_Mouse_Pressed = false;
	bool bLeft_Mouse_Pressed = false;
	float iXMouse;
	float iYMouse;
	float iXWorld_Mouse;
	float iYWorld_Mouse;
	int Mouse_Selection_Box_x_Display;
	int Mouse_Selection_Box_y_Display;




	bool FRect_inside_Cursor(SDL_FRect &rect) {
		SDL_FRect cursor = { iXWorld_Mouse, iYWorld_Mouse, cursorSize, cursorSize };
		return Utilities::bFRect_Intersect(cursor, rect);
	}


	bool Point_Inside_Cursor(float targetX, float targetY) { 
		if (iXWorld_Mouse + cursorSize > targetX &&
			iXWorld_Mouse - cursorSize < targetX &&
			iYWorld_Mouse - cursorSize < targetY &&
			iYWorld_Mouse + cursorSize > targetY) {
			return true;
		}
		else return false;
	}

	bool Mouse_Selection_Box(SDL_FRect target) {
		SDL_FRect entity = { float(Mouse::Mouse_Selection_Box_x), float(Mouse::Mouse_Selection_Box_y), float(Mouse::iXWorld_Mouse - Mouse::Mouse_Selection_Box_x), float(Mouse::iYWorld_Mouse - Mouse::Mouse_Selection_Box_y) };
		if ((entity.y <= target.y + target.h) &&
			(entity.x <= target.x + target.w) &&
			(entity.y + entity.h >= target.y) &&
			(entity.x + entity.w >= target.x)) {
			return true;
		}
		return false;
		
	};


	
}