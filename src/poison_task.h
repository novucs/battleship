/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * poison_task.h - Poison task header.
 *
 * Regularly ARP poisons a group of victims at a given interval.
 */

#ifndef HIVE_BOT_POISON_TASK_H_
#define HIVE_BOT_POISON_TASK_H_

#include "protocol_utils.h"

#include <atomic>
#include <mutex>
#include <unordered_map>
#include <thread>

namespace hive_bot {

/**
 * Regularly ARP poisons a specified victim.
 */
class PoisonTask {

  private:
    // Victim addresses (IP to MAC).
    std::unordered_map<std::string, std::string> victim_addresses_;

    // Spoof addresses (IP to MAC).
    std::unordered_map<std::string, std::string> spoof_addresses_;

    // Duration in seconds to sleep before repoisoning.
    u_long sleep_duration_;

    // The thread the poison task is running on.
    std::thread task_thread_;

    // Determines if the task should still be running.
    std::atomic<bool> running_ = { false };

    // Ensures task is only ran once.
    std::mutex mutex_;

    /**
     * The main poison loop.
     */
    void Loop();

  public:
    /**
     * Constructs a new poison task.
     *
     * @param victim_addresses Who to poison.
     * @param spoof_addresses What the attack should be.
     * @param sleep_duration How long between poisoning attacks (in seconds).
     */
    PoisonTask(std::unordered_map<std::string, std::string> victim_addresses,
               std::unordered_map<std::string, std::string> spoof_addresses,
               u_long sleep_duration);

    /**
     * Checks if this task is running.
     *
     * @return {@code} true if it is running.
     */
    bool IsRunning();

    /**
     * Starts the poison task.
     *
     * @return {@code true} if successful.
     */
    bool Start();

    /**
     * Stops the poison task.
     */
    void Stop();
};

} // hive_bot

#endif // HIVE_BOT_POISON_TASK_H_
