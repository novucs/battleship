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

#include "protocol_utils.h"

namespace hive_bot {

CommandManager::CommandManager(Bot* bot) {
  bot_ = bot;
}

void CommandManager::PrintHelp(std::string message) {
  std::cout << "=====[ Hive Bot Help ]=====" << std::endl;
  std::cout << "h|help - Prints this help page" << std::endl;
  std::cout << "e|exit - Turns off the bot" << std::endl;
  std::cout << "r|respawn - Sends the server a respawn packet" << std::endl;
  std::cout << "d|device - Selects which network to attack" << std::endl;
  std::cout << "arp scan - Refresh all MAC addresses" << std::endl;
}

void CommandManager::Respawn(std::string message) {
  bot_->Respawn();
  std::cout << "Bot respawned" << std::endl;
}

void CommandManager::Device(std::string message) {
  SelectDevice();
}

void CommandManager::ArpScan(std::string message) {
  FetchMacs(server_ip);
}

void CommandManager::Run() {
  std::unordered_map<std::string, Command> commands = {
    {"help", &(CommandManager::PrintHelp)},
    {"h", &(CommandManager::PrintHelp)},
    {"respawn", &(CommandManager::Respawn)},
    {"r", &(CommandManager::Respawn)},
    {"device", &(CommandManager::Device)},
    {"d", &(CommandManager::Device)},
    {"arp scan", &(CommandManager::ArpScan)},
  };

  std::cout << std::endl << "Enter commands here, type 'help' for help.";
  std::cout << std::endl;

  for (;;) {
    std::cout << "> ";
    std::string message;
    std::getline(std::cin, message);

    if (message == "exit" || message == "e") {
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
