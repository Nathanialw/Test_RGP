#pragma once
#include <iostream>
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_TTF.h"


namespace Graphics {
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Rect Screen;
	SDL_Rect resolution = { 0, 0, 1280, 960 };
	
	TTF_Font* font;
	
	SDL_Texture* skeleton_0;
	SDL_Texture* tree_0;
	SDL_Texture* archer_0;
	SDL_Texture* house_0;
	SDL_Texture* grass_0;




	struct Surface_Data {
		SDL_Texture* pTexture;
		SDL_Rect k;
	};



	//Surface_Data Load_Text_Texture(std::string text, const std::string& fontFile, SDL_Color fontColor, int fontSize, SDL_Renderer* renderer) {
	//	//TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);						//save font from file
	//	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fontColor);		//convert font to Surface
	//	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);			//convert Surface to texture
	//	int h = surface->h;
	//	int w = surface->w;
	//	Surface_Data text_data = { texture, h, w };
	//	TTF_CloseFont(font);					
	//	SDL_FreeSurface(surface);														//free surface memory
	////	SDL_DestroyTexture(texture);
	//	return text_data;																	//return SDL_Texture *texture
	//};

	Surface_Data Load_Text_Texture(std::string text, SDL_Color fontColor, SDL_Renderer* renderer) {
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
	SDL_Texture* createTexture(const char* spritesheet) {
		SDL_Surface* surface = IMG_Load(spritesheet);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}

	void Load_Textures() {
		Create_Font();
		skeleton_0 = createTexture("sprites\\units\\skeleton\\skeleton_00.png");
		tree_0 = createTexture("sprites\\environment\\trees\\_tree_01\\_tree_01_00000.png");
		house_0 = createTexture("sprites\\buildings\\building_04.png");
		archer_0 = createTexture("sprites\\units\\archer\\archer_00.png");
		grass_0 = createTexture("sprites\\environment\\grass\\grass.png");
	}


	//scene.emplace<camera>(skeleton, 0, 0, -winX / 2, -winY / 2);

	void createGraphicsContext() {
		if (SDL_CreateWindowAndRenderer(resolution.w, resolution.h, NULL, &window, &renderer) != 0) {
			SDL_Log("error creating window and renderer", SDL_GetError);
		}

		Load_Textures();
	}



	//SDL_RenderPresent(renderer);
			
	void closeContext() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
	
}

