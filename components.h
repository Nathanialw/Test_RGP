#pragma once
#include <SDL.h>
#include "SDL_gpu.h"
#include <iostream>
#include <vector>
#include "debug_components.h"
#include "classes.h"


using namespace Mil_Struc;

namespace Components {

	struct unique_ID {
		Uint32 sdaf;
	};


	struct i2d {
		int x;
		int y;
	};

	struct f2d {
		float fX;
		float fY;
	};

	struct handle {
		std::string sName;
	};

	struct Soldier {
		int iIndex;
		entt::entity SquadAssgnedTo;
	};
	
	struct Health {
		int iHealth;
	};

	struct Alive {
		bool bIsAlive;
	};

	//Position		
	struct Position_X {
		float fX;
		float fPX;
		float fSX;
	};	
	
	struct Position_Z {
		float* fX;	
	};
	
	struct Position_Y {
		float fY;
		float fPY;
		float fSY;
	};
	////////////


	struct Collision_Radius {
		float fCollisionRadius;
	};

	struct Velocity {
		float dX, dY;
		f2d magnitude;
		float speed;
	};

	
	enum Action_States {
		isStatic,
		idle,
		walk,
		slash,
		stab,
		block,
		dead,
		xbow
	};

	struct Frame_Data {
		int currentFrame;
		int NumFrames;
	}; 

	struct Actions {
		Action_States action;
		std::vector<Frame_Data>frameCount;
	};

	enum Compass {
		W,
		NW,
		N,
		NE,
		E,
		SE,
		S,
		SW
	};

	struct Direction {
		Compass eDirection;
	};

	struct spriteframes {
		SDL_Rect clip;
		int frameStart;
		int sheetWidth;
		bool bReversable;
		bool bReversing;
		i2d posOffset;
		float timeBetweenFrames;
	};
	 
	struct animation { 
		SDL_Texture* pTexture;		//texture
		SDL_Rect clipSprite;
		SDL_Rect renderPosition;
		std::vector<spriteframes>sheet;
	};
	
	struct Input {
		//std::vector<bool>keysPressed;
	};

	struct Camera {
		SDL_Rect screen;
		f2d scale;
	};

	struct Mass {
		float fKilos;
	};

	struct Commanded_Move {
		float fX_Destination;
		float fY_Destination;
	};

	struct Commandable {
		
	};

	struct Commanding {
		bool bSelected;
	};

	struct Selected {

	};

	struct Collided {

	};

	struct Moving {

	};

	struct Mouse_Move {
		float fX_Destination;
		float fY_Destination;
	};

	struct Test_V {
		std::vector<Position_X>test_V;
		//SDL_FRect sCollider;
	};
}


