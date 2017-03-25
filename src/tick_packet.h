/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * tick_packet.h - Tick packet header.
 *
 * Tick packet data type and utilities.
 */

#ifndef HIVE_BOT_TICK_PACKET_H_
#define HIVE_BOT_TICK_PACKET_H_

#include <vector>

#include "ship.h"

namespace hive_bot {

/**
 * Packet sent between all allies to update the new game state.
 */
class TickPacket {

  private:
    // Predicted score.
    int score_;

    // Ships sent from the server this tick.
    std::vector<Ship> ships_;

  public:
    /**
     * Constructs a new tick packet.
     *
     * @param score The current predicted score of this ally.
     * @param ships All viewed ships, index 0 being the ally.
     */
    TickPacket(int score, std::vector<Ship> ships);

    /**
     * Gets the current predicted score.
     *
     * @return the score.
     */
    int GetScore();

    /**
     * Gets all viewed ships, index 0 being the ally.
     *
     * @return the ships.
     */
    std::vector<Ship> GetShips();
};

} // namespace hive_bot

#endif // HIVE_BOT_TICK_PACKET_H_
