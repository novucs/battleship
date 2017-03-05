/*
 * tick_packet.hpp
 *
 * Defines the tick packet class.
 */

#pragma once

#include <vector>
#include "ship.hpp"

/**
 * Packet sent between all allies to update the new game state.
 */
class tick_packet {

	private:
		// Predicted score.
		int score;

		// Ships sent from the server this tick.
		std::vector<ship> ships;

	public:
		/**
		 * Constructs a new tick packet.
		 *
		 * @param score The current predicted score of this ally.
		 * @param ships All viewed ships, index 0 being the ally.
		 */
		tick_packet(int score, std::vector<ship> ships);

		/**
		 * Gets the current predicted score.
		 *
		 * @return the score.
		 */
		int get_score();

		/**
		 * Gets all viewed ships, index 0 being the ally.
		 *
		 * @return the ships.
		 */
		std::vector<ship> get_ships();
};
