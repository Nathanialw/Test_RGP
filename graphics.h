#pragma once
#include <iostream>
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_TTF.h"
#include "SDL_gpu.h"



namespace Graphics {

	SDL_Renderer* renderer;
	SDL_Window* window;

	SDL_Rect resolution = { 0, 0, 1280, 960 };
	SDL_Rect Screen;
	
	TTF_Font* font;
	
	SDL_Texture* skeleton_0;
	SDL_Texture* skeleton_1;
	SDL_Texture* tree_0;
	SDL_Texture* archer_0;
	SDL_Texture* house_0;
	SDL_Texture* grass_0;





	struct Surface_Data {
		SDL_Texture* pTexture;
		SDL_Rect k;
	};


	Surface_Data Load_Text_Texture(std::string text, SDL_Color fontColor) {
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fontColor);		//convert font to Surface
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);			//convert Surface to texture
		SDL_Rect k = surface->clip_rect;
		
		
		Surface_Data text_data = { texture, k};
		SDL_FreeSurface(surface);														//free surface memory
		return text_data;																	//return SDL_Texture *texture
	};

	void Create_Font() {
		font = TTF_OpenFont("fonts\\FreeSans.ttf", 30);
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
		skeleton_0 = createTexture("sprites\\units\\skeleton\\armoured_skeleton_00.png");
		skeleton_1 = createTexture("sprites\\units\\skeleton\\skeleton_00.png");
		tree_0 = createTexture("sprites\\environment\\trees\\_tree_01\\_tree_01_00000.png");
		house_0 = createTexture("sprites\\buildings\\building_04.png");
		archer_0 = createTexture("sprites\\units\\archer\\archer_00.png");
		grass_0 = createTexture("sprites\\environment\\grass\\grass.png");
	}




	void createGraphicsContext() {
		//SDL_GL_SetSwapInterval(0);
		//GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
		//renderer = GPU_Init(resolution.w, resolution.h, GPU_DEFAULT_INIT_FLAGS);
		//
		//cam = GPU_GetDefaultCamera();


		SDL_CreateWindowAndRenderer(resolution.w, resolution.h, NULL, &window, &renderer);
		Load_Textures();
	}



			
	void closeContext() {
		//SDL_DestroyWindow(window);
	}
	
}

