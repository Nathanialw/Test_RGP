#include "rendering.h"
#include "init.h"
#include "eventHandler.h"
#include "collision.h"
#include "debug_system.h"
#include "ai_control.h"
#include "interface.h"
#include "movement.h"

int main(int argc, char* argv[]) {
	
	//initialization
	Init::init();
	createGraphicsContext();
	loadEntities();
  
	while (1) {			

		EventHandler::Player_Input();
		AI::Run_AI();
		Movement::Movement_Handler();
		collision::Collisions();		


		Rendering::Rendering();
		Interface::Run_Interface();
		Timer::frameTime();
		Rendering::Rendering();
	
	}
	//close
	closeContext();
	
	
	return 0;
}