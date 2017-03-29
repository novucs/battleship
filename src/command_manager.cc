/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * command_manager.cc - Command manager header.
 *
 * Handles the execution of all commands sent to console.
 */

#include "command_manager.h"

#include <iostream>
#include <string>
#include <unordered_map>

namespace hive_bot {

/**
 * Constructs a new command manager.
 *
 * @param bot The master bot.
 */
CommandManager::CommandManager(Bot* bot) {
  bot_ = bot;
}

/**
 * Prints the command help page.
 *
 * @param message The command executed.
 */
void CommandManager::PrintHelp(std::string message) {
  std::cout << "=====[ Hive Bot Help ]=====" << std::endl;
  std::cout << "h|help - Prints this help page" << std::endl;
  std::cout << "e|exit - Turns off the bot" << std::endl;
  std::cout << "r|respawn - Sends the server a respawn packet" << std::endl;
}

/**
 * Respawns the battleship into the server.
 *
 * @param message The command executed.
 */
void CommandManager::Respawn(std::string message) {
  bot_->Respawn();
  std::cout << "Bot respawned" << std::endl;
}

/**
 * Runs the command manager on the current thread.
 */
void CommandManager::Run() {
  std::unordered_map<std::string, Command> commands = {
    {"help", &CommandManager::PrintHelp},
    {"h", &CommandManager::PrintHelp},
    {"respawn", &CommandManager::Respawn},
    {"r", &CommandManager::Respawn},
  };

  std::cout << std::endl << "Enter commands here, type 'help' for help.";
  std::cout << std::endl;

  for (;;) {
    std::cout << identity.GetId() << " > ";
    std::string message;
    std::getline(std::cin, message);

    if (message == "exit" || message == "e" ||
        message == "quit" || message == "q") {
      return;
    }

    if (message == "") {
      continue;
    }

    auto command_mapping = commands.find(message);

    if (command_mapping == commands.end()) {
      std::cout << "Command not found" << std::endl;
    } else {
      (this->*command_mapping->second)(message);
    }
  }
}

} // hive_bot
