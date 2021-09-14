#pragma once



namespace Mouse {

	bool bRight_Mouse_Pressed = false;
	bool bLeft_Mouse_Pressed = false;
	int iXMouse;
	int iYMouse;
	int iXWorld_Mouse;
	int iYWorld_Mouse;
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
	
}