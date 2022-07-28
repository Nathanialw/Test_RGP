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
#include "unit_status.h"

int main(int argc, char* argv[]) {
	
	//initialization
	Init::init();
	Graphics::createGraphicsContext(World::zone);
	Init_Zone(World::zone);
	UI::Init_UI();
  
	while (Graphics::running == true) {
		//User_Mouse_Input::Assign_Soldiers_On_Spawn(World::zone);//tries to add new ungrouped units to the unit vector every frame
		Test_Units::Create_And_Fill_New_Squad(World::zone);
		Test_Units::Create_Formation(World::zone);
		update_scene(); //tries to add new environment objects and terrain the the world grid every frame
	
		
		Event_Handler::Update_User_Input(World::zone);

		//std::cout << "Player_Input = Good" << std::endl;
		AI::Update_AI(World::zone);

		//std::cout << "AI = Good" << std::endl;
		Weapons::Update_Attacks();

		//std::cout << "Update_Attacks = Good" << std::endl;
		Spells::Update_Spells();

		//std::cout << "Update_Spells = Good" << std::endl;
		Movement::Update_Entity_Positions(World::zone);

		//std::cout << "Movement_Handler = Good" << std::endl;		
		//Formation_Positions::Update_Formation_Positions(World::zone);
		//std::cout << "Update_Formation_Rects = Good" << std::endl;
		//collision::Resolve_Collisions(World::zone);		

		Unit_Position::Update_Formation_Positions(World::zone);

		//std::cout << "Collisions = Good" << std::endl;
		Unit_Status ::Update_Unit_Status(World::zone);

		//std::cout << "Update_Unit_Status = Good" << std::endl;	
		Rendering::Rendering(World::zone);

		//std::cout << "Rendering = Good" << std::endl;
		Timer::frameTime();	
		//std::cout << "frameTime = Good" << std::endl;
	}
	//close
	Graphics::closeContext();
		
	return 0;
}