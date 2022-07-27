#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include "classes.h"


namespace Component {



	struct handle {
		std::string sName;
	};

	struct Cell_Assigned {
		int iIndex;
		entt::entity iCell_Assigned_To;
	};

	struct Assigned_To {
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
	struct Position {
		float x;
		float y;
	};	
	
	struct Potential_Position {
		float x;
		float y;
	};

	struct Collided {
		float x;
		float y;
	};

	struct Terrain_Position_X {
		float fX;
	};

	struct Terrain_Position_Y {
		float fY;
	};
	////////////

	struct Radius {
		float fRadius;
	};


	struct Velocity {
		float dX, dY;
		DataTypes::f2d magnitude;
		float speed;
		float angle;
	};

	struct Building_Collider {
		std::vector<DataTypes::i2d>walls;
	};
	
	enum Action_States {
		isStatic,
		idle,
		walk,
		slash,
		cast,
		struck,
		dead,
		ranged,
		cheer,
		summon
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

	struct Update_Direction {

	};

	struct Sprite_Offset {
		DataTypes::f2d offset;
	};

	struct spriteframes {
		SDL_Rect clip;
		int frameStart;
		int sheetWidth;
		bool bReversable;
		bool bReversing;
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
		int alpha;
	};

	struct Scale {
		float scale;
	};

	struct Terrain_Renderable {
		float y;
	};
	
	struct Input {
		//std::vector<bool>keysPressed;
	};
		
	struct Camera {
		SDL_FRect screen;
		DataTypes::f2d scale;
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

	struct Linear_Move {
		float fX_Direction;
		float fY_Direction;
	};

	struct Spell_Range {
		float fSourceX;
		float fSourceY;
		float fRange;
	};

	struct Casting {
	};

	struct Cast {
		float targetX;
		float targetY;
	};

	struct Spell {

	};

	struct Spellbook {

	};

	struct Spell_Name {
		const char* spell;
	};

	struct Casted {

	};

	struct Sprite_Frames {
		int maxFrames;
		int currentFrame;
		int frameX;
		int frameY;
	};

	struct Texture {
		SDL_Texture* pTexture;		//texture
		SDL_Rect clippedSpriteFrame;
	};

	struct Frame_Delay {
		float timeBetweenFrames;
		float currentFrameTime;
	};

	struct Explosion {
		DataTypes::i2d posOffset;
		SDL_FRect renderPosition;
		DataTypes::f2d offsetToAlignWithFireball;

	};

	struct Melee {

	};

	struct Attacking {
		
	};

	struct Attack {
		float targetX;
		float targetY;
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

	struct Struck {
		int struck;
	};

	//AI

	struct Sight_Range {
		SDL_FRect sightBox;
	};

	struct Bag {
		std::vector<entt::entity> bag;
	};

	enum Item_Type {
		weapon,
		armour,
		potion
	};

	struct Icon {		
		SDL_Texture* pTexture;		
		SDL_Rect clipSprite;
		DataTypes::i2d renderRectSize;
		DataTypes::i2d renderPositionOffset;
	};

	struct On_Mouse {

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


