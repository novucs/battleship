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

#include "bot.h"

namespace hive_bot {

class CommandManager {

  private:
    Bot* bot_;

    void PrintHelp(std::string message);
    void Respawn(std::string message);
    void ArpScan(std::string message);

  public:
    CommandManager(Bot* bot);

    void Run();
};

typedef void (CommandManager::*Command)(std::string message);

} // hive_bot

#endif // HIVE_BOT_COMMAND_MANAGER_H_
