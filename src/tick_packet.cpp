/*
 * tick_packet.cpp
 *
 * Implements the tick packet class defined in tick_packet.hpp.
 */

#include "tick_packet.hpp"

/**
 * Constructs the new tick packet.
 *
 * @param score The current predicted score of this ally.
 * @param ships All viewed ships, index 0 being the ally.
 */
tick_packet::tick_packet(int score, std::vector<ship> ships) {
	this->score = score;
	this->ships = ships;
}

/**
 * Gets the current predicted score.
 *
 * @return the score.
 */
int tick_packet::get_score() {
	return score;
}

/**
 * Gets all viewed ships, index 0 being the ally.
 *
 * @return the ships;
 */
std::vector<ship> tick_packet::get_ships() {
	return ships;
}
