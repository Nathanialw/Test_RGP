#pragma once
#include <vector>


namespace Mil_Struc {

	class Squad {

	public:
		int size;
		int index;

		SDL_FRect collide_Box;
		std::vector<bool>units;
		std::vector<float>x;
		std::vector<float>y;
	};


	std::vector<Squad> Squads;









}