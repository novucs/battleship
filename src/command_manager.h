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

/**
 * Handles all command execution.
 */
class CommandManager {

  private:
    // The master bot.
    Bot* bot_;

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

  public:
    /**
     * Constructs a new command manager.
     *
     * @param bot The master bot.
     */
    CommandManager(Bot* bot);

    /**
     * Runs the command manager on the current thread.
     */
    void Run();
};

// Defines a command function.
typedef void (CommandManager::*Command)(std::string message);

} // hive_bot

#endif // HIVE_BOT_COMMAND_MANAGER_H_
