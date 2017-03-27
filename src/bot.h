/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * bot.h - Bot header.
 *
 * Handles all communication between the battleship server, hive bots and
 * handles all tactics. So long as the ports and other hive bots are configured
 * correctly, this is able to be ran multiple times and the group as a whole
 * becomes more powerful.
 */

#ifndef HIVE_BOT_BOT_H_
#define HIVE_BOT_BOT_H_

#include <ctime>
#include <mutex>
#include <thread>
#include <vector>

#include "connection.h"
#include "main.h"
#include "ship.h"
#include "student.h"

namespace hive_bot {

/**
 * The main bot worker class, manages all networking and tactics.
 */
class Bot {

  private:
    // Connection to the battleship bot server.
    Connection server_connection_ = CreateConnection(server_ip, server_port);

    // Connection being locally hosted for this bot.
    Connection client_connection_ = CreateConnection(client_port);

    // Connection used for communicating with other hive bots.
    Connection hive_connection_ = CreateConnection(hive_port);

    // The thread used for tick logic and communicating with the server.
    std::thread server_thread_;

    // The threads used for synchronizing tick data between ally bots.
    std::vector<std::thread> hive_threads_;

    // Our current ship data for this tick.
    Ship ship_;

    // All viewed enemy ships in this tick.
    std::vector<Ship> enemy_ships_;

    // Mutex used for synchronizing hive and server threads.
    std::mutex mutex_;

    // All loaded ships from the hive network.
    std::vector<std::vector<Ship>> ships_;

    // All previous ally ships.
    std::vector<Ship> previous_allies_;

    /**
     * Performs the tactics code for this tick.
     */
    void Tactics();

    /**
     * Starts the hive loop, loading data sent from the ally.
     *
     * @param id The position this ally is stored in the global allies list.
     */
    void HiveLoop(int id);

    /**
     * Starts the main server loop. Used for retreiving data from the server,
     * relaying that data to allies and then performing tactics code.
     */
    void ServerLoop();

    /**
     * Merges and sorts all loaded ships from hive threads into enemies and
     * allies.
     */
    bool MergeShips();

    /**
     * Prints the debug message for this tick.
     */
    void PrintDebug();

    /**
     * Checks if ship was probably an ally in the previous tick.
     *
     * @param to_check The ship to check.
     * @return {@code true} if the ship was considered an ally, otherwise
     *         {@code false}.
     */
    int GetPreviousAllyId(Ship& to_check);

    /**
     * Returns if a ship is considered an ally.
     *
     * @param to_check The ship to check.
     * @return {@code true} if the ship is an ally, otherwise {@code false}.
     */
    bool IsAlly(Ship& to_check);

    /**
     * Returns if the ship is considered an enemy.
     *
     * @param to_check The ship to check.
     * @return {@code true} if the ship is an enemy, otherwise {@code false}.
     */
    bool IsEnemy(Ship& to_check);

    /**
     * Gets the attack value.
     *
     * @param id The ID used on the attack.
     * @return the attack value.
     */
     int GetAttackValue(int id);

  public:
    /**
     * Initializes the bot state and connections.
     */
    bool Setup();

    /**
     * Begins all the bot threads and awaits user input.
     */
    void Run();

    /**
     * Closes opened sockets and discontinues running connection threads.
     */
    void Close();

    /**
     * Sends a fire packet to the server.
     *
     * @param x The X coordinate to fire at.
     * @param y The Y coordinate to fire at.
     */
    void Fire(int x, int y);

    /**
     * Sends a movement packet to the server.
     *
     * @param x The speed X to move.
     * @param y The speed Y to move.
     */
    void Move(int x, int y);

    /**
     * Sends a flag change packet to the server.
     *
     * @param flag The new flag.
     */
    void Flag(int flag);

    /**
     * Sends a respawn packet to the server, using the gloabal ship_type
     * variable (defined in main.h) as the new ship type.
     */
    void Respawn();
};

} // namespace hive_bot

#endif // HIVE_BOT_BOT_H_
