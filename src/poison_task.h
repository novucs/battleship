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

#include <atomic>
#include <unordered_map>

#include "protocol_utils.h"

namespace hive_bot {

class PoisonTask {

  private:
    // Victim addresses (IP to MAC).
    std::unordered_map<char*, char*> victims_;

    // The IP address to spoof (127.0.0.1).
    char* spoof_ip_;

    // The MAC address to spoof (aabbccddeeff).
    char* spoof_mac_;

    // Duration in seconds to sleep before repoisoning.
    u_long sleep_duration_;

    // Determines if the task should still be running.
    std::atomic<bool> running;

  public:
    PoisonTask(std::unordered_map<char*, char*> victims, char* spoof_ip,
               char* spoof_mac, u_long sleep_duration);

    void Run();

    void Stop();
};

} // hive_bot

#endif // HIVE_BOT_POISON_TASK_H_
