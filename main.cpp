#include "rendering.h"
#include "init.h"
#include "eventHandler.h"
#include "collision.h"
#include "debug_system.h"

using namespace EventHandler;
using namespace Rendering;


int main(int argc, char* argv[]) {
	
	//initialization
	Init::init();
	createGraphicsContext();
	loadEntities();
  
	while (1) {

		
		
		isDead();
		Player_Input();
		AI_Loop();
		Movement();
		updateDirection();
		collision::sort_Positions();
		collision::collisionUpdate();
		collision::resolveCollisons();
		cameraTrack();
		animationFrame();
		SDL_RenderPresent(Graphics::renderer);
		Debug_System::Debugger();
		Timer::frameTime();
	
	}
	//close
	closeContext();
	
	
	return 0;
}