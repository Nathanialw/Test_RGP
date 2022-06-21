#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include "debug_components.h"
#include "classes.h"


using namespace Mil_Struc;

namespace Components {



	struct handle {
		std::string sName;
	};

	struct Cell_Assigned {
		int iIndex;
		entt::entity iCell_Assigned_To;
	};

	struct Assigned {
		int iIndex;
		entt::entity iUnit_Assigned_To;
	};
	
	struct Soldier {

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
	
	struct Position_Y {
		float fY;
		float fPY;
		float fSY;
	};

	struct Terrain_Position_X {
		float fX;
		float fPX;
		float fSX;
	};

	struct Terrain_Position_Y {
		float fY;
		float fPY;
		float fSY;
	};
	////////////

	struct Radius {
		float fRadius;
	};


	struct Velocity {
		float dX, dY;
		f2d magnitude;
		float speed;
		float angle1, angle2;
	};

	struct Building_Collider {
		std::vector<i2d>walls;
	};
	
	enum Action_States {
		isStatic,
		idle,
		walk,
		slash,
		stab,
		struck,
		dead,
		ranged,
		cheer
	};

	struct Frame_Data {
		int NumFrames;
		int currentFrame;
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
		float currentFrameTime;
	};
	 
	struct animation { 
		SDL_Texture* pTexture;		//texture
		SDL_Rect clipSprite;
		SDL_Rect renderPosition;
		std::vector<spriteframes>sheet;
	};

	struct Renderable {
		float y;
	};

	struct Terrain_Renderable {
		float y;
	};
	
	struct Input {
		//std::vector<bool>keysPressed;
	};
		
	struct Camera {
		SDL_FRect screen;
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
	};

	struct Selected {

	};

	struct Collided {

	};

	struct Moving {

	};

	struct Environment {

	};

	struct Terrain {

	};

	struct Mouse_Move {
		float fX_Destination;
		float fY_Destination;
	};

	enum Casting {
		fireball
	};

	struct Spell {

	};

	struct Casted {

	};

	struct Melee {

	};

	struct Attacking {
		
	};
	
	enum Weapon_Type {
		sword
	};

	struct Damage {
		int min;
		int max;
	};

	struct Weapon_Size {
		SDL_FRect attackArea;
	};

	struct Attack_Box_Duration {
		int64_t lifeTime;
		int64_t count;
	};
}


namespace Component_Camera { //unused yet

	struct Viewport {
		SDL_FRect viewport;
	};

	struct Resolution {
		SDL_FRect resolution;
	};

	struct Screen {
		SDL_FRect screen;
	};

}