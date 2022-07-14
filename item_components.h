#pragma once
#include <SDL.h>

namespace Item_Components {

	enum Item_Type {
		weapon,
		armour,
		potion
	};


	enum Weapon_Type {
		sword
	};


	struct Ground_Item {
		SDL_FRect box;
	};



}