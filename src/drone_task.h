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

/**
 * Handles controlling all captured enemy bots.
 */
class DroneTask {

  private:
    // Duration in milliseconds to sleep between server ticks.
    u_long sleep_duration_;

    // The thread the poison task is running on.
    std::thread task_thread_;

    // Determines if the task should still be running.
    std::atomic<bool> running_ = { false };

    // Ensures task is only ran once.
    std::mutex mutex_;

    /**
     * The main loop function.
     */
    void Loop();

    /**
     * Spoofs a move packet to the server using pcap.
     *
     * @param ip The IP to spoof.
     * @param mac The MAC to spoof.
     * @param id The user ID.
     * @param x The X velocity to move.
     * @param y The Y velocity to move.
     */
    void SendMove(char* ip, char* mac, char* id, int x, int y);

    /**
     * Spoofs a fire packet to the server using pcap.
     *
     * @param ip The IP to spoof.
     * @param mac The MAC to spoof.
     * @param id The user ID.
     * @param x The X coordinate to shoot.
     * @param y The Y coordinate to shoot.
     */
    void SendFire(char* ip, char* mac, char* id, int x, int y);

    /**
     * Spoofs a flag packet to the server using pcap.
     *
     * @param ip The IP to spoof.
     * @param mac The MAC to spoof.
     * @param id The user ID.
     * @param flag The new flag the ship should take.
     */
    void SendFlag(char* ip, char* mac, char* id, int flag);

    /**
     * Sends a custom spoofed packet to the server.
     *
     * @param ip The IP to spoof.
     * @param mac The MAC to spoof.
     * @param payload The message to send.
     */
    void SendServerPacket(char* ip, char* mac, char* payload);

  public:
    /**
     * Constructs a new drone task.
     *
     * @param sleep_duration How long to sleep before ticking again.
     */
    DroneTask(u_long sleep_duration);

    /**
     * Checks if the drone task is running.
     *
     * @return {@code true} if currently running.
     */
    bool IsRunning();

    /**
     * Starts a new drone task.
     *
     * @return {@code true} if successfully started.
     */
    bool Start();

    /**
     * Stops the drone task.
     */
    void Stop();
};

} // hive_bot

#endif // HIVE_BOT_DRONE_TASK_H_
