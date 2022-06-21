
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
		//std::cout << "update_scene = Good" << std::endl;
		Weapons::Update_Attacks();
		//std::cout << "Update_Attacks = Good" << std::endl;
		Spells::Update_Spells();
		//std::cout << "Update_Spells = Good" << std::endl;
		User_Mouse_Input::Assign_Soldiers_On_Spawn();
		//std::cout << "Assign_Soldiers_On_Spawn = Good" << std::endl;
		Event_Handler::Player_Input();
		//std::cout << "Player_Input = Good" << std::endl;
		Movement::Movement_Handler();
		//std::cout << "Movement_Handler = Good" << std::endl;
		collision::Collisions();		
		//std::cout << "Collisions = Good" << std::endl;
			
		Rendering::Rendering();
		//std::cout << "Rendering = Good" << std::endl;
		Timer::frameTime();	
		//std::cout << "frameTime = Good" << std::endl;
	}
	//close
	closeContext();
		
	return 0;
}