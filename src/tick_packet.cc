/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * tick_packet.cc - Tick packet implementation.
 *
 * Tick packet data type and utilities.
 */

#include "tick_packet.h"

namespace hive_bot {

/**
 * Constructs a new tick packet.
 *
 * @param score The current predicted score of this ally.
 * @param ships All viewed ships, index 0 being the ally.
 */
TickPacket::TickPacket(int score, std::vector<Ship> ships) {
  score_ = score;
  ships_ = ships;
}

/**
 * Gets the current predicted score.
 *
 * @return the score.
 */
int TickPacket::GetScore() {
  return score_;
}

/**
 * Gets all viewed ships, index 0 being the ally.
 *
 * @return the ships;
 */
std::vector<Ship> TickPacket::GetShips() {
  return ships_;
}

} // namespace hive_bot
