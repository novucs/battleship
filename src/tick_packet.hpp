#pragma once

#include <vector>
#include "ship.hpp"

class tick_packet {
	private:
		int score;
		std::vector<ship> ships;
	public:
		tick_packet(int score, std::vector<ship> ships);
		int get_score();
		std::vector<ship> get_ships();
};
