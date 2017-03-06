/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * ship.cc - Ship implementation.
 *
 * Ship data type and utilities.
 */

#include "ship.h"

#include <math.h>

#include "main.h"

namespace hive_bot {

/**
 * Constructs a new ship.
 *
 * @param x      The X coordinate.
 * @param y      The Y coordinate.
 * @param health The ship health.
 * @param flag   The ship flag.
 * @param type   The ship type.
 */
Ship::Ship(int x, int y, int health, int flag, int type) {
  x_ = x;
  y_ = y;
  health_ = health;
  flag_ = flag;
  type_ = type;
}

/**
 * Gets the X coordinate.
 *
 * @return the X coordinate.
 */
int Ship::GetX() {
  return x_;
}

/**
 * Gets the Y coordinate.
 *
 * @return the Y coordinate.
 */
int Ship::GetY() {
  return y_;
}

/**
 * Gets the ship health.
 *
 * @return the ship health.
 */
int Ship::GetHealth() {
  return health_;
}

/**
 * Gets the ship flag.
 *
 * @return the ship flag.
 */
int Ship::GetFlag() {
  return flag_;
}

/**
 * Gets the ship type.
 *
 * @return the ship type.
 */
int Ship::GetType() {
  return type_;
}

/**
 * Gets the distance from this ship to another.
 *
 * @param other The other ship.
 * @return the distance between the two.
 */
double Ship::DistanceTo(Ship& other) {
  int x = other.GetX() - x_;
  int y = other.GetY() - y_;
  return sqrt((x * x) + (y * y));
}

/**
 * Gets the ships maximum range.
 *
 * @return the maximum range.
 */
int Ship::GetRange() {
  switch (type_) {
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
int Ship::GetFinalRange() {
  return ((GetRange() * health_) / 8) + 1;
}

/**
 * Gets the fastest speed this ship can travel.
 *
 * @return the maximum speed.
 */
int Ship::GetMaxSpeed() {
  return round(health_ * 2 / 10);
}

/**
 * Returns if the ships maximum speed is zero, or unable to move.
 *
 * @return {@code true} if crippled, otherwise {@code false}.
 */
bool Ship::IsCrippled() {
  return GetMaxSpeed() == 0;
}

/**
 * Returns if this ship is able to damage another ship.
 *
 * @param victim The potential target.
 * @return {@code true} if able to hit the victim, otherwise {@code false}.
 */
bool Ship::CanDamage(Ship& victim) {
  double distance = DistanceTo(victim);
  int range = GetFinalRange();
  return distance <= range;
}

/**
 * Gets the maximum damage able to be dealt to a victim, ignoring whether
 * or not this ship can hit the victim.
 *
 * @param victim The potential target.
 * @return the maximum damage.
 */
int Ship::GetDamage(Ship& victim) {
  switch (type_) {
    case SHIP_TYPE_BATTLESHIP:
      switch (victim.GetType()) {
        case SHIP_TYPE_BATTLESHIP:
          return 1;
        case SHIP_TYPE_FRIGATE:
          return 2;
        case SHIP_TYPE_SUBMARINE:
          return 3;
      }
    case SHIP_TYPE_FRIGATE:
      switch (victim.GetType()) {
        case SHIP_TYPE_BATTLESHIP:
        case SHIP_TYPE_FRIGATE:
          return 1;
        case SHIP_TYPE_SUBMARINE:
          return 2;
      }
    case SHIP_TYPE_SUBMARINE:
      return 2;
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
int Ship::GetFinalDamage(Ship& victim) {
  if (CanDamage(victim)) {
    return GetDamage(victim);
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
bool operator==(Ship& ship1, Ship& ship2) {
  return ship1.GetX() == ship2.GetX() &&
    ship1.GetY() == ship2.GetY() &&
    ship1.GetHealth() == ship2.GetHealth() &&
    ship1.GetFlag() == ship2.GetFlag() &&
    ship1.GetType() == ship2.GetType();
}

/**
 * Checks if two ships are not equal.
 *
 * @param ship1 The first ship to compare.
 * @param ship2 The second ship to compare.
 * @return {@code true} if they are not equal, otherwise {@code false}.
 */
bool operator!=(Ship& ship1, Ship& ship2) {
  return ship1.GetX() != ship2.GetX() ||
    ship1.GetY() != ship2.GetY() ||
    ship1.GetHealth() != ship2.GetHealth() ||
    ship1.GetFlag() != ship2.GetFlag() ||
    ship1.GetType() != ship2.GetType();
}

} // namespace hive_bot
