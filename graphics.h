#pragma once
#include <iostream>
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_TTF.h"



namespace Graphics {

	namespace {
		bool running;
		SDL_FRect resolution = { 0, 0, 1920, 1200 };
		SDL_Window* window;
		SDL_FRect Screen;
		TTF_Font* font;

		struct Surface_Data {
			SDL_Texture* pTexture;
			SDL_Rect k;
		};
	}

	SDL_Renderer* renderer;	
	
	SDL_Texture* skeleton_0;
	SDL_Texture* skeleton_1;
	SDL_Texture* skeleton_mage_0;
	SDL_Texture* warrior_axe;
	SDL_Texture* tree_0;
	SDL_Texture* archer_0;
	SDL_Texture* house_0;
	SDL_Texture* grass_0;
	SDL_Texture* fireball_0;	
	SDL_Texture* fireball_explosion_0;
	SDL_Texture* longsword_default;
	//SDL_Texture* item_ui;
	SDL_Texture* itsmars_Inventory;
	SDL_Texture* cursor_0;

	void Render_Rect(SDL_Texture *texture, const SDL_Rect *sourceRect, const SDL_Rect *targetRect) {
		SDL_RenderCopy(renderer, texture, sourceRect, targetRect);
	}

	void Render_FRect(SDL_Texture* texture, const SDL_Rect* sourceRect, const SDL_FRect* targetRect) {
		SDL_RenderCopyF(renderer, texture, sourceRect, targetRect);
	}

	Surface_Data Load_Text_Texture(std::string text, SDL_Color fontColor) {
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fontColor);		//convert font to Surface
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);			//convert Surface to texture 
		SDL_Rect k = surface->clip_rect;
		
		
		Surface_Data text_data = { texture, k};
		SDL_FreeSurface(surface);														//free surface memory
		return text_data;																	//return SDL_Texture *texture
	};

	void Create_Font() {
		font = TTF_OpenFont("fonts/FreeSans.ttf", 30);
	}

	//load texture when initializing components
	//SDL_Texture* createTexture2(const char* spritesheet) {
	//	SDL_Surface* surface = IMG_Load(spritesheet);
	//	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	//	SDL_FreeSurface(surface);
	//	return texture;
	//}

	SDL_Texture* createTexture(const char* spritesheet) {
		SDL_Surface* surface = IMG_Load(spritesheet);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}

	void Load_Textures() {
		Create_Font();
		skeleton_0 = createTexture("sprites/units/skeleton/armoured_skeleton_00.png");
		warrior_axe = createTexture("sprites/units/warrior/warrior_axe.png");
		skeleton_1 = createTexture("sprites/units/skeleton/skeleton_00.png");
		skeleton_mage_0 = createTexture("sprites/units/skeleton/skeleton_mage_00.png");
		tree_0 = createTexture("sprites/environment/trees/_tree_01/_tree_01_00000.png");
		house_0 = createTexture("sprites/buildings/building_04.png");
		archer_0 = createTexture("sprites/units/archer/archer_00.png");
		grass_0 = createTexture("sprites/environment/grass/grass.png");
		fireball_0 = createTexture("sprites/spells/fireball_0.png");		
		fireball_explosion_0 = createTexture("sprites/spells/fireball_explosion_0.png");
	
		longsword_default = createTexture("sprites/items/long_sword/w_longsword.png");
		//item_ui = createTexture("sprites/UI/equipment.jpg");
		itsmars_Inventory = createTexture("sprites/UI/itsmars_Inventory.png");
		cursor_0 = createTexture("sprites/UI/cursor.png");
	}

	void createGraphicsContext() {
		//SDL_GL_SetSwapInterval(0);
		//GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
		//renderer = GPU_Init(resolution.w, resolution.h, GPU_DEFAULT_INIT_FLAGS);
		//
		//cam = GPU_GetDefaultCamera();
	
		SDL_CreateWindowAndRenderer(resolution.w, resolution.h, NULL, &window, &renderer);
		SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
		Load_Textures();
	}
			
	void closeContext() {
		SDL_DestroyWindow(window);
		running = false;
	}
	
}
