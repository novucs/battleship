/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
* ship.h - Ship header.
*
* Ship data type and utilities.
*/

#ifndef HIVE_BOT_SHIP_H_
#define HIVE_BOT_SHIP_H_

// All ship types.
#define SHIP_TYPE_BATTLESHIP 0
#define SHIP_TYPE_FRIGATE 1
#define SHIP_TYPE_SUBMARINE 2

namespace hive_bot {

/**
 * An immutable ship, participating in battleship bots.
 */
class Ship {

  private:
    // The X coordinate.
    int x_;

    // The Y coordinate.
    int y_;

    // The ship health.
    int health_;

    // The ship flag.
    int flag_;

    // The ship type.
    int type_;

  public:
    /**
     * Empty constructor, constructs a new zero'd ship.
     */
    Ship() : Ship(0, 0, 0, 0, 0) {};

    /**
     * Constructs a new ship.
     *
     * @param x      The X coordinate.
     * @param y      The Y coordinate.
     * @param health The ship health.
     * @param flag   The ship flag.
     * @param type   The ship type.
     */
    Ship(int x, int y, int health, int flag, int type);

    /**
     * Gets the X coordinate.
     *
     * @return the X coordinate.
     */
    int GetX();

    /**
     * Gets the Y coordinate.
     *
     * @return the Y coordinate.
     */
    int GetY();

    /**
     * Gets the ship health.
     *
     * @return the ship health.
     */
    int GetHealth();

    /**
     * Gets the ship flag.
     *
     * @return the ship flag.
     */
    int GetFlag();

    /**
     * Gets the ship type.
     *
     * @return the ship type.
     */
    int GetType();

    /**
     * Gets the distance from this ship to another.
     *
     * @param other The other ship.
     * @return the distance between the two.
     */
    double DistanceTo(Ship& other);

    /**
     * Gets the ships maximum range.
     *
     * @return the maximum range.
     */
    int GetRange();

    /**
     * Gets the ships final range, accounting for the ship type and health.
     *
     * @return the final range.
     */
    int GetFinalRange();

    /**
     * Gets the fastest speed this ship can travel.
     *
     * @return the maximum speed.
     */
    int GetMaxSpeed();

    /**
     * Returns if the ships maximum speed is zero, or unable to move.
     *
     * @return {@code true} if crippled, otherwise {@code false}.
     */
    bool IsCrippled();

    /**
     * Returns if this ship is able to damage another ship.
     *
     * @param victim The potential target.
     * @return {@code true} if able to hit the victim, otherwise {@code false}.
     */
    bool CanDamage(Ship& victim);

    /**
     * Gets the maximum damage able to be dealt to a victim, ignoring whether
     * or not this ship can hit the victim.
     *
     * @param victim The potential target.
     * @return the maximum damage.
     */
    int GetDamage(Ship& victim);

    /**
     * Optimistically predicts the final damage able to be dealt, taking into
     * account the final range and damage of this ship.
     *
     * @param victim The potential target.
     * @return the predicted damage.
     */
    int GetFinalDamage(Ship& victim);
};

/**
 * Checks if two ships are equal.
 *
 * @param ship1 The first ship to compare.
 * @param ship2 The second ship to compare.
 * @return {@code true} if they are equal, otherwise {@code false}.
 */
bool operator==(Ship& ship1, Ship& ship2);

/**
 * Checks if two ships are not equal.
 *
 * @param ship1 The first ship to compare.
 * @param ship2 The second ship to compare.
 * @return {@code true} if they are not equal, otherwise {@code false}.
 */
bool operator!=(Ship& ship1, Ship& ship2);

} // namespace hive_bot

#endif // HIVE_BOT_SHIP_H_
