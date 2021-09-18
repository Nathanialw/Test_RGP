#pragma once



namespace Mouse {

	float Cursor_Size = 5.0f;
	bool bRight_Mouse_Pressed = false;
	bool bLeft_Mouse_Pressed = false;
	int iXMouse;
	int iYMouse;
	float iXWorld_Mouse;
	float iYWorld_Mouse;
	int Mouse_Selection_Box_x;
	int Mouse_Selection_Box_y;
	int Mouse_Selection_Box_x_Display;
	int Mouse_Selection_Box_y_Display;

	bool Inside_Cursor(int aX, int aY, float bX, float bY, float squareRadius) { //a is mouse, b is target of the mouse
		if (aX > bX - squareRadius && aX < bX + squareRadius && aY < bY + squareRadius && aY > bY - squareRadius) {
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