#include "tick_packet.hpp"

tick_packet::tick_packet(int score, std::vector<ship> ships) {
	this->score = score;
	this->ships = ships;
}

int tick_packet::get_score() {
	return score;
}

std::vector<ship> tick_packet::get_ships() {
	return ships;
}
