/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * command_manager.h - Command manager header.
 *
 * Handles the execution of all commands sent to console.
 */

#ifndef HIVE_BOT_COMMAND_MANAGER_H_
#define HIVE_BOT_COMMAND_MANAGER_H_

#include <string>
#include <memory>
#include <thread>

#include "bot.h"
#include "drone_task.h"
#include "poison_task.h"

namespace hive_bot {

/**
 * Handles all command execution.
 */
class CommandManager {

  private:
    // The master bot.
    Bot* bot_;

    // Thread used for retreiving all found network data.
    std::thread pcap_loop_thread;

    // Task used for controlling poisoned enemies.
    std::unique_ptr<DroneTask> drone_task_;

    // Task for poisoning/healing allies.
    std::unique_ptr<PoisonTask> ally_poison_task_;

    // Task for poisoning/healing enemies.
    std::unique_ptr<PoisonTask> enemy_poison_task_;

    // Task for poisoning/healing the server.
    std::unique_ptr<PoisonTask> server_poison_task_;

    /**
     * Prints the command help page.
     *
     * @param message The command executed.
     */
    void PrintHelp(std::string message);

    /**
     * Respawns the battleship into the server.
     *
     * @param message The command executed.
     */
    void Respawn(std::string message);

    /**
     * Broadcasts a fake server packet, forcing poisoned clients to respond.
     *
     * @param message The command executed.
     */
    void CollectIds(std::string message);

    /**
     * Begins the drone task, controlling all poisoned enemies.
     *
     * @param message The command executed.
     */
    void AutomatedAttack(std::string message);

    /**
     * Floods the server with dummy bots that causes the server to halt.
     *
     * @param message The command executed.
     */
    void Flood(std::string message);

    /**
     * Heals all allies from ARP poisoning.
     *
     * @param message The command executed.
     */
    void HealAllies(std::string message);

    /**
     * Heals all enemies from ARP poisoning.
     *
     * @param message The command executed.
     */
    void HealEnemies(std::string message);

    /**
     * Poisons all enemies making them think we are the server.
     *
     * @param message The command executed.
     */
    void PoisonEnemies(std::string message);

    /**
     * Poisons the server, making it think we are every client.
     *
     * @param message The command executed.
     */
    void PoisonServer(std::string message);

  public:
    /**
     * Constructs a new command manager.
     *
     * @param bot The master bot.
     */
    CommandManager(Bot* bot);

    /**
     * Selects which network device the attacks should be used on.
     *
     * @param message The command executed.
     */
    void Device(std::string message);

    /**
     * Scans the network for all IP and MAC addresses.
     *
     * @param message The command executed.
     */
    void Scan(std::string message);

    /**
     * Runs the command manager on the current thread.
     */
    void Run();
};

// Defines a command function.
typedef void (CommandManager::*Command)(std::string message);

} // hive_bot

#endif // HIVE_BOT_COMMAND_MANAGER_H_
