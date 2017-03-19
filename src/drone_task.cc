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

namespace hive_bot {

DroneTask::DroneTask(u_long sleep_duration) {
  sleep_duration_ = sleep_duration;
}

void DroneTask::Loop() {
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
