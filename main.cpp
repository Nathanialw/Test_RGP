
#include "rendering.h"
#include "init.h"
#include "event_handler.h"
#include "collision.h"
#include "debug_system.h"
#include "ai_control.h"
#include "interface.h"
#include "movement.h"
int main(int argc, char* argv[]) {
	
	//initialization
	Init::init();
	createGraphicsContext();
	Init_Game();
  
	while (1) {			

		Event_Handler::Player_Input();
		AI::Run_AI();
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