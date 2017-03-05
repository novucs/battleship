/*
 * ship.cpp
 *
 * Implements ship related methods and classes defined in ship.hpp.
 */

#include <math.h>
#include "main.hpp"
#include "ship.hpp"

/**
 * Constructs a new ship.
 *
 * @param x      The X coordinate.
 * @param y      The Y coordinate.
 * @param health The ship health.
 * @param flag   The ship flag.
 * @param type   The ship type.
 */
ship::ship(int x, int y, int health, int flag, int type) {
	this->x = x;
	this->y = y;
	this->health = health;
	this->flag = flag;
	this->type = type;
}

/**
 * Gets the X coordinate.
 *
 * @return the X coordinate.
 */
int ship::get_x() {
	return x;
}

/**
 * Gets the Y coordinate.
 *
 * @return the Y coordinate.
 */
int ship::get_y() {
	return y;
}

/**
 * Gets the ship health.
 *
 * @return the ship health.
 */
int ship::get_health() {
	return health;
}

/**
 * Gets the ship flag.
 *
 * @return the ship flag.
 */
int ship::get_flag() {
	return flag;
}

/**
 * Gets the ship type.
 *
 * @return the ship type.
 */
int ship::get_type() {
	return type;
}

/**
 * Gets the distance from this ship to another.
 *
 * @param other The other ship.
 * @return the distance between the two.
 */
double ship::distance_to(ship& other) {
	int x = other.get_x() - this->x;
	int y = other.get_y() - this->y;
	return sqrt((x * x) + (y * y));
}

/**
 * Gets the ships maximum range.
 *
 * @return the maximum range.
 */
int ship::get_range() {
	switch (type) {
		case SHIP_TYPE_BATTLESHIP:
			return 100;
		case SHIP_TYPE_FRIGATE:
			return 75;
		case SHIP_TYPE_SUBMARINE:
			return 50;
	}
	return 0;
}

/**
 * Gets the ships final range, accounting for the ship type and health.
 *
 * @return the final range.
 */
int ship::get_final_range() {
	return ((get_range() * health) / 8) + 1;
}

/**
 * Gets the fastest speed this ship can travel.
 *
 * @return the maximum speed.
 */
int ship::get_max_speed() {
	return round(get_health() * 2 / 10);
}

/**
 * Returns if the ships maximum speed is zero, or unable to move.
 *
 * @return {@code true} if crippled, otherwise {@code false}.
 */
bool ship::is_crippled() {
	return get_max_speed() == 0;
}

/**
 * Returns if this ship is able to damage another ship.
 *
 * @param victim The potential target.
 * @return {@code true} if able to hit the victim, otherwise {@code false}.
 */
bool ship::can_damage(ship& victim) {
	double distance = distance_to(victim);
	int range = get_final_range();
	return distance <= range;
}

/**
 * Gets the maximum damage able to be dealt to a victim, ignoring whether
 * or not this ship can hit the victim.
 *
 * @param victim The potential target.
 * @return the maximum damage.
 */
int ship::get_damage(ship& victim) {
	int attacker_ship_type = get_type();
	int victim_ship_type = victim.get_type();

	switch (attacker_ship_type) {
		case SHIP_TYPE_BATTLESHIP:
			switch (victim_ship_type) {
				case SHIP_TYPE_BATTLESHIP:
					return 1;
				case SHIP_TYPE_FRIGATE:
					return 2;
				case SHIP_TYPE_SUBMARINE:
					return 3;
			}
		case SHIP_TYPE_FRIGATE:
			switch (victim_ship_type) {
				case SHIP_TYPE_BATTLESHIP:
					return 1;
				case SHIP_TYPE_FRIGATE:
					return 1;
				case SHIP_TYPE_SUBMARINE:
					return 2;
			}
		case SHIP_TYPE_SUBMARINE:
			switch (victim_ship_type) {
				case SHIP_TYPE_BATTLESHIP:
					return 2;
				case SHIP_TYPE_FRIGATE:
					return 2;
				case SHIP_TYPE_SUBMARINE:
					return 2;
			}
	}

	return 0;
}

/**
 * Optimistically predicts the final damage able to be dealt, taking into
 * account the final range and damage of this ship.
 *
 * @param victim The potential target.
 * @return the predicted damage.
 */
int ship::get_final_damage(ship& victim) {
	if (can_damage(victim)) {
		return get_damage(victim);
	}

	return 0;
}

/**
 * Checks if two ships are equal.
 *
 * @param ship1 The first ship to compare.
 * @param ship2 The second ship to compare.
 * @return {@code true} if they are equal, otherwise {@code false}.
 */
bool operator==(ship& ship1, ship& ship2) {
	return ship1.get_x() == ship2.get_x() &&
		ship1.get_y() == ship2.get_y() &&
		ship1.get_health() == ship2.get_health() &&
		ship1.get_flag() == ship2.get_flag() &&
		ship1.get_type() == ship2.get_type();
}

/**
 * Checks if two ships are not equal.
 *
 * @param ship1 The first ship to compare.
 * @param ship2 The second ship to compare.
 * @return {@code true} if they are not equal, otherwise {@code false}.
 */
bool operator!=(ship& ship1, ship& ship2) {
	return ship1.get_x() != ship2.get_x() ||
		ship1.get_y() != ship2.get_y() ||
		ship1.get_health() != ship2.get_health() ||
		ship1.get_flag() != ship2.get_flag() ||
		ship1.get_type() != ship2.get_type();
}
