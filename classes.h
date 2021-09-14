#pragma once
#include <vector>
#include <array>


namespace Mil_Struc {

	struct Unit {
		int name;
		SDL_FRect collide_Box;

		std::vector<entt::entity>sSub_Units;
		std::array<float, 12>fX;
		std::array<float, 12>fY;
	};


	struct Squad : Unit {
		int size;
		SDL_FRect collide_Box;

		std::vector<entt::entity>sSub_Units;
		std::vector<float>fPX;
		std::vector<float>fPY;
		std::vector<float>fMass;
		std::vector<float>fRadius;

		Squad() {			
			size = 12;
			collide_Box = { 0,0,0,0 };
			sSub_Units.reserve(size);
			fPX.reserve(size);
			fPY.reserve(size);
			fMass.reserve(size);
			fRadius.reserve(size);
		}
	};


	struct Company : Unit {
		int name;
		SDL_FRect collide_Box;

		std::array<entt::entity, 12>sSub_Units;
		std::array<float, 12>fX;
		std::array<float, 12>fY;
	};

	struct Battalion : Unit {
		int name;
		SDL_FRect collide_Box;

		std::array<entt::entity, 12>sSub_Units;
		std::array<float, 12>fX;
		std::array<float, 12>fY;
	};	
	
	struct Brigade : Unit {
		int name;
		SDL_FRect collide_Box;

		std::array<entt::entity, 12>sSub_Units;
		std::array<float, 12>fX;
		std::array<float, 12>fY;
	};	
	
	
	struct Division : Unit {
		int name;
		SDL_FRect collide_Box;

		std::array<entt::entity, 12>sSub_Units;
		std::array<float, 12>fX;
		std::array<float, 12>fY;
	};	
	
	
	struct Army : Unit {
		int name;
		SDL_FRect collide_Box;

		std::array<entt::entity, 12>sSub_Units;
		std::array<float, 12>fX;
		std::array<float, 12>fY;
	};




}