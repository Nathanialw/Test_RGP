
#include "rendering.h"
#include "init.h"
#include "event_handler.h"
#include "collision.h"
#include "debug_system.h"
#include "ai_control.h"
#include "interface.h"
#include "movement.h"
#include "spells.h"
int main(int argc, char* argv[]) {
	
	//initialization
	Init::init();
	createGraphicsContext();
	Init_Game();
  
	while (Graphics::running == true) {
		update_scene();
		Weapons::Update_Attacks();
		Spells::Update_Spells();
		User_Mouse_Input::Assign_Soldiers_On_Spawn();
		Event_Handler::Player_Input();
		Movement::Movement_Handler();
		collision::Collisions();		
			
		Rendering::Rendering();
		Interface::Run_Interface();
		Timer::frameTime();	
	}
	//close
	closeContext();
		
	return 0;
}