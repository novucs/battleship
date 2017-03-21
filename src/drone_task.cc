/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * drone_task.cc - Drone task implementation.
 *
 * Controls all enemies on the network to do our bidding.
 */

#include "drone_task.h"

#include <iostream>

#include "main.h"

namespace hive_bot {

DroneTask::DroneTask(u_long sleep_duration) {
  sleep_duration_ = sleep_duration;
}

void DroneTask::Loop() {
  std::unique_lock<std::mutex> lock(mutex_);
  running_ = true;

  while (running_) {
    packet_handler_mutex.lock();
    std::vector<Ship> enemy_ships;

    for (Ship ship : captured_ships) {
      if (!IsCapturedStudentShip(ship)) {
        enemy_ships.push_back(ship);
      }
    }

    for (auto& user_data : captured_ids) {
      std::string ip = user_data.first;
      auto mac_pos = enemy_arp_table.find(ip);
      auto ship_pos = captured_student_ships.find(ip);

      if (mac_pos == enemy_arp_table.end() ||
          ship_pos == captured_student_ships.end()) {
        continue;
      }

      Ship& ship = ship_pos->second;
      Ship target;
      bool target_found = false;
      double closest_distance = 1000;

      for (Ship& enemy_ship : enemy_ships) {
        if (!target_found || enemy_ship.DistanceTo(ship) < closest_distance) {
          target_found = true;
          target = enemy_ship;
        }
      }

      int move_x = ship.GetX() > 500 ? -2 : 2;
      int move_y = ship.GetY() > 500 ? -2 : 2;
      int flag = ship.GetX() ^ 0x41;
      char* ip_c_str = strdup(ip.c_str());
      char* id = strdup(user_data.second.c_str());
      char* mac = strdup(mac_pos->second.c_str());

      if (target_found) {
        int target_x = target.GetX();
        int target_y = target.GetY();
        SendFire(ip_c_str, mac, id, target_x, target_y);
      }

      SendMove(ip_c_str, mac, id, move_x, move_y);
      SendFlag(ip_c_str, mac, id, flag);
    }

    captured_student_ships.clear();
    captured_ships.clear();
    packet_handler_mutex.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration_));
  }

  std::cout << "Drone task stopped" << std::endl;
  running_ = false;
}

void DroneTask::SendMove(char* ip, char* mac, char* id, int x, int y) {
  static char payload[512];
  sprintf_s(payload, "Move %s,%d,%d", id, x, y);
  SendServerPacket(ip, mac, payload);
}

void DroneTask::SendFire(char* ip, char* mac, char* id, int x, int y) {
  static char payload[512];
  sprintf_s(payload, "Fire %s,%d,%d", id, x, y);
  SendServerPacket(ip, mac, payload);
}

void DroneTask::SendFlag(char* ip, char* mac, char* id, int flag) {
  static char payload[512];
  sprintf_s(payload, "Flag %s,%d", id, flag);
  SendServerPacket(ip, mac, payload);
}

void DroneTask::SendServerPacket(char* ip, char* mac, char* payload) {
  static const u_short header_length = sizeof(struct UdpHeader) +
                                       sizeof(struct Ipv4Header) +
                                       sizeof(struct EthernetHeader);
  u_int length = strlen(payload);
  u_char* packet = (u_char*) malloc(length + header_length);

  u_char* message = packet + header_length;
  memcpy(message, payload, length);

  WriteUdp(length, packet, client_port, server_port, strdup(server_mac.c_str()),
           mac, ip, strdup(server_ip.c_str()));

  if (pcap_sendpacket(pcap, packet, length + header_length) != 0) {
    std::cout << "Error sending packet: " << pcap_geterr(pcap) << std::endl;
    return;
  }

  free(packet);
}

bool DroneTask::IsRunning() {
  return running_;
}

bool DroneTask::Start() {
  if (running_) {
    std::cout << "Task already running!" << std::endl;
    return false;
  }

  if (pcap == NULL) {
    std::cout << "You must first choose a valid device" << std::endl;
    return false;
  }

  task_thread_ = std::thread(&DroneTask::Loop, this);
  return true;
}

void DroneTask::Stop() {
  running_ = false;
  task_thread_.join();
}

} // hive_bot
