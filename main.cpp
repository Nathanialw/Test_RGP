#include "rendering.h"
#include "init.h"
#include "event_handler.h"
#include "collision.h"
#include "debug_system.h"
#include "ai_control.h"
#include "interface.h"
#include "movement.h"
#include "spells.h"
#include "unit_Positions.h"

int main(int argc, char* argv[]) {
	
	//initialization
	Init::init();
	Graphics::createGraphicsContext(World::zone);
	Init_Zone(World::zone);
	UI::Init_UI();
  
	while (Graphics::running == true) {
		User_Mouse_Input::Assign_Soldiers_On_Spawn(World::zone);
		AI::Run_AI(World::zone);

		//std::cout << "AI = Good" << std::endl;
		Weapons::Update_Attacks();
		//std::cout << "Update_Attacks = Good" << std::endl;
		Spells::Update_Spells();
		//std::cout << "Update_Spells = Good" << std::endl;
		Event_Handler::Player_Input(World::zone);
		//std::cout << "Player_Input = Good" << std::endl;
		Movement::Movement_Handler(World::zone);
		//std::cout << "Movement_Handler = Good" << std::endl;
		collision::Collisions(World::zone);		
		//Unit_Position::Run_Position(World::zone);
		//std::cout << "Collisions = Good" << std::endl;
		update_scene();
			
		Rendering::Rendering(World::zone);
		//std::cout << "Rendering = Good" << std::endl;
		Timer::frameTime();	
		//std::cout << "frameTime = Good" << std::endl;
	}
	//close
	Graphics::closeContext();
		
	return 0;
}