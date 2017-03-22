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

class CommandManager {

  private:
    Bot* bot_;

    std::thread pcap_loop_thread;

    std::unique_ptr<DroneTask> drone_task_;

    std::unique_ptr<PoisonTask> ally_poison_task_;

    std::unique_ptr<PoisonTask> enemy_poison_task_;

    std::unique_ptr<PoisonTask> server_poison_task_;

    void PrintHelp(std::string message);

    void Respawn(std::string message);

    void CollectIds(std::string message);

    void AutomatedAttack(std::string message);

    void Flood(std::string message);

    void HealAllies(std::string message);

    void HealEnemies(std::string message);

    void PoisonEnemies(std::string message);

    void PoisonServer(std::string message);

  public:
    CommandManager(Bot* bot);

    void Device(std::string message);

    void Scan(std::string message);

    void Run();
};

typedef void (CommandManager::*Command)(std::string message);

} // hive_bot

#endif // HIVE_BOT_COMMAND_MANAGER_H_
