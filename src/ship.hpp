/*
 * ship.hpp
 *
 * Defines ship creation and helper interfaces.
 */

#pragma once

/**
 * An immutable ship, participating in battleship bots.
 */
class ship {

	private:
		// The X coordinate.
		int x;

		// The Y coordinate.
		int y;

		// The ship health.
		int health;

		// The ship flag.
		int flag;

		// The ship type.
		int type;

	public:
		/**
		 * Empty constructor, constructs a new zero'd ship.
		 */
		ship() : ship(0, 0, 0, 0, 0) {};

		/**
		 * Constructs a new ship.
		 *
		 * @param x      The X coordinate.
		 * @param y      The Y coordinate.
		 * @param health The ship health.
		 * @param flag   The ship flag.
		 * @param type   The ship type.
		 */
		ship(int x, int y, int health, int flag, int type);

		/**
		 * Gets the X coordinate.
		 *
		 * @return the X coordinate.
		 */
		int get_x();

		/**
		 * Gets the Y coordinate.
		 *
		 * @return the Y coordinate.
		 */
		int get_y();

		/**
		 * Gets the ship health.
		 *
		 * @return the ship health.
		 */
		int get_health();

		/**
		 * Gets the ship flag.
		 *
		 * @return the ship flag.
		 */
		int get_flag();

		/**
		 * Gets the ship type.
		 *
		 * @return the ship type.
		 */
		int get_type();

		/**
		 * Gets the distance from this ship to another.
		 *
		 * @param other The other ship.
		 * @return the distance between the two.
		 */
		double distance_to(ship& other);

		/**
		 * Gets the ships maximum range.
		 *
		 * @return the maximum range.
		 */
		int get_range();

		/**
		 * Gets the ships final range, accounting for the ship type and health.
		 *
		 * @return the final range.
		 */
		int get_final_range();

		/**
		 * Gets the fastest speed this ship can travel.
		 *
		 * @return the maximum speed.
		 */
		int get_max_speed();

		/**
		 * Returns if the ships maximum speed is zero, or unable to move.
		 *
		 * @return {@code true} if crippled, otherwise {@code false}.
		 */
		bool is_crippled();

		/**
		 * Returns if this ship is able to damage another ship.
		 *
		 * @param victim The potential target.
		 * @return {@code true} if able to hit the victim, otherwise {@code false}.
		 */
		bool can_damage(ship& victim);

		/**
		 * Gets the maximum damage able to be dealt to a victim, ignoring whether
		 * or not this ship can hit the victim.
		 *
		 * @param victim The potential target.
		 * @return the maximum damage.
		 */
		int get_damage(ship& victim);

		/**
		 * Optimistically predicts the final damage able to be dealt, taking into
		 * account the final range and damage of this ship.
		 *
		 * @param victim The potential target.
		 * @return the predicted damage.
		 */
		int get_final_damage(ship& victim);
};

/**
 * Checks if two ships are equal.
 *
 * @param ship1 The first ship to compare.
 * @param ship2 The second ship to compare.
 * @return {@code true} if they are equal, otherwise {@code false}.
 */
bool operator==(ship& ship1, ship& ship2);

/**
 * Checks if two ships are not equal.
 *
 * @param ship1 The first ship to compare.
 * @param ship2 The second ship to compare.
 * @return {@code true} if they are not equal, otherwise {@code false}.
 */
bool operator!=(ship& ship1, ship& ship2);
