/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * bot.cc - Bot implementation.
 *
 * Handles all communication between the battleship server, hive bots and
 * handles all tactics. So long as the ports and other hive bots are configured
 * correctly, this is able to be ran multiple times and the group as a whole
 * becomes more powerful.
 */

#include "bot.h"

#include <limits.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "command_manager.h"
#include "main.h"
#include "protocol_utils.h"
#include "tick_packet.h"

namespace hive_bot {

/**
 * Performs the tactics code for this tick.
 */
void Bot::Tactics() {
  // Calculate:
  // - number of connected allies
  // - nearby allies
  // - the overall center between all allies
  std::size_t active_ally_count = 0;
  std::vector<Ship> nearby_allies;
  int center_x = ship_.GetX();
  int center_y = ship_.GetY();
  bool slow_group = false;
  int highest_ally_score = INT_MIN;
  int lowest_ally_score = INT_MAX;
  int lowest_ally_frigate_score = INT_MAX;
  int ally_frigate_count = 0;

  for (Student& ally : allies) {
    if (!ally.IsConnected()) {
      continue;
    }

    if (ally.GetScore() > highest_ally_score) {
      highest_ally_score = ally.GetScore();
    }

    if (ally.GetScore() < lowest_ally_score) {
      lowest_ally_score = ally.GetScore();
    }

    Ship ally_ship = ally.GetShip();
    center_x += ally_ship.GetX();
    center_y += ally_ship.GetY();
    active_ally_count++;

    if (ally_ship.GetType() == SHIP_TYPE_FRIGATE) {
      ally_frigate_count++;

      if (lowest_ally_frigate_score > ally.GetScore()) {
        lowest_ally_frigate_score = ally.GetScore();
      }
    }

    if (ally_ship.DistanceTo(ship_) < ((offset * 2) + 2)) {
      if (ally_ship.GetMaxSpeed() == 1) {
        slow_group = true;
      }

      nearby_allies.push_back(ally_ship);
    }
  }

  // Calculate speed to accomodate for all ships.
  int speed = 2;

  if (slow_group && ship_.GetMaxSpeed() == 2) {
    speed = 1;
  }

  // Apply default movement: towards map center.
  int move_x = ship_.GetX() < (500 + identity.GetOffsetX()) ? speed : -speed;
  int move_y = ship_.GetY() < (500 + identity.GetOffsetY()) ? speed : -speed;

  int group_size = active_ally_count + 1;
  center_x /= group_size;
  center_y /= group_size;
  center_x += identity.GetOffsetX();
  center_y += identity.GetOffsetY();

  // Target crippled allies.
  Ship target;
  int target_weight = 0;
  bool target_found = false;

  for (Ship& ally_ship : nearby_allies) {
    if (ally_ship.IsCrippled()) {
      target = ally_ship;
      target_found = true;
    }
  }

  // Target closer enemies when no crippled allies found.
  if (!target_found) {
    for (Ship& enemy_ship : enemy_ships_) {
      int weight = 0;

      // weight += ship_.get_final_damage(enemy_ship);
      // weight -= enemy_ship.get_final_damage(ship_);
      weight -= ship_.DistanceTo(enemy_ship);

      for (Ship& ally_ship : nearby_allies) {
          // weight += ship_.get_final_damage(ally_ship);
          // weight -= ally_ship.get_final_damage(ship_);
          weight -= ally_ship.DistanceTo(enemy_ship);
      }

      if (!target_found || target_weight < weight) {
        target_weight = weight;
        target = enemy_ship;
        target_found = true;
      }
    }
  }

  if (target_found) {
    // Fire at target if theres a chance to damage.
    if (ship_.GetFinalDamage(target) > 0) {
      Fire(target.GetX(), target.GetY());

      // Increment our score.
      int score = identity.GetScore() + ship_.GetFinalDamage(target);
      identity.SetScore(score);
    }

    int target_x = target.GetX() + identity.GetOffsetX();
    int target_y = target.GetY() + identity.GetOffsetY();

    move_x = ship_.GetX() < target_x ? speed : -speed;
    move_y = ship_.GetY() < target_y ? speed : -speed;
  }

  // Always move towards ally center when we are not nearby all allies.
  if (nearby_allies.size() < active_ally_count) {
    move_x = ship_.GetX() < center_x ? 2 : -2;
    move_y = ship_.GetY() < center_y ? 2 : -2;
  }

  Move(move_x, move_y);

  int new_flag = ship_.GetX() ^ '.';
  Flag(new_flag);

  if (active_ally_count > 0) {
    switch (ship_type) {
      case SHIP_TYPE_BATTLESHIP:
        if (identity.GetScore() - 50 > highest_ally_score) {
          ship_type = SHIP_TYPE_FRIGATE;
          Respawn();
        }
        break;
      case SHIP_TYPE_FRIGATE:
        if (identity.GetScore() + 50 < lowest_ally_score) {
          ship_type = SHIP_TYPE_BATTLESHIP;
          Respawn();
        }
        break;
    }
  }
}

/**
 * Sends a fire packet to the server.
 *
 * @param x The X coordinate to fire at.
 * @param y The Y coordinate to fire at.
 */
void Bot::Fire(int x, int y) {
  identity.GetConnection().SendFire(identity.GetId(), x, y);
}

/**
 * Sends a movement packet to the server.
 *
 * @param x The speed X to move.
 * @param y The speed Y to move.
 */
void Bot::Move(int x, int y) {
  identity.GetConnection().SendMove(identity.GetId(), x, y);
}

/**
 * Sends a flag change packet to the server.
 *
 * @param flag The new flag.
 */
void Bot::Flag(int flag) {
  identity.GetConnection().SendFlag(identity.GetId(), flag);
}

/**
 * Sends a respawn packet to the server, using the gloabal ship_type
 * variable (defined in main.h) as the new ship type.
 */
void Bot::Respawn() {
  std::string id = identity.GetId();
  std::string forename = identity.GetForename();
  std::string surname = identity.GetSurname();
  identity.GetConnection().SendRespawn(id, forename, surname, ship_type);
}

/**
 * Starts the hive loop, loading data sent from the ally.
 *
 * @param id The position this ally is stored in the global allies list.
 */
void Bot::HiveLoop(int id) {
  char buffer[4096];
  Student& ally = allies.at(id);
  Connection ally_connection = ally.GetConnection();

  for (;;) {
    memset(buffer, '\0', sizeof(buffer));

    int receive_code = hive_connection_.Receive(ally_connection, buffer,
                                                sizeof(buffer));

    switch (receive_code) {
      case RETREIVE_SUCCESS:
        break;
      case RETREIVE_FAIL:
        return;
      case RETREIVE_IGNORE:
        continue;
    }

    if (buffer[0] == 'M') {
      continue;
    }

    mutex_.lock();

    TickPacket packet = ReadTickPacket(buffer);
    std::vector<Ship> ships = packet.GetShips();
    ships_.at(ally.GetLoadOrder()) = ships;
    ally.SetScore(packet.GetScore());

    if (!ships.empty()) {
      ally.SetConnected(true);
      ally.SetShip(std::move(ships.at(0)));
    }

    mutex_.unlock();
  }
}

/**
 * Starts the main server loop. Used for retreiving data from the server,
 * relaying that data to allies and then performing tactics code.
 */
void Bot::ServerLoop() {
  char buffer[4096];

  for (;;) {
    memset(buffer, '\0', sizeof(buffer));

    int receive_code = client_connection_.Receive(server_connection_, buffer,
                                                  sizeof(buffer));

    switch (receive_code) {
      case RETREIVE_SUCCESS:
        break;
      case RETREIVE_FAIL:
        return;
      case RETREIVE_IGNORE:
        continue;
    }

    if (buffer[0] == 'M') {
      continue;
    }

    mutex_.lock();
    std::vector<Ship> ships = ReadShips(false, buffer);
    ships_.at(identity.GetLoadOrder()) = ships;
    int last_tick_health = ship_.GetHealth();

    ship_ = ships.at(0);
    identity.SetShip(std::move(ship_));
    int score = identity.GetScore();

    if (last_tick_health < ship_.GetHealth()) {
      score -= (last_tick_health + (ship_.GetHealth() - 10));
    } else if (last_tick_health > ship_.GetHealth()) {
      score -= (last_tick_health - ship_.GetHealth());
    }

    identity.SetScore(score);

    TickPacket packet(score, ships);

    for (Student& ally : allies) {
      ally.GetConnection().SendTickPacket(packet);
    }

    mutex_.unlock();

    // Ghetto sleep to receive zombie data.
    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    mutex_.lock();

    if (!MergeShips()) {
      continue;
    }

    if (debug) {
      PrintDebug();
    }

    Tactics();

    // Flag allies as not connected for next tick.
    for (Student& ally : allies) {
      ally.SetConnected(false);
    }

    mutex_.unlock();
  }
}

/**
 * Merges and sorts all loaded ships from hive threads into enemies
 * and allies.
 */
bool Bot::MergeShips() {
  enemy_ships_.clear();

  if (ships_.empty() || ships_.at(identity.GetLoadOrder()).empty()) {
    return false;
  }

  for (std::vector<Ship>& ships : ships_) {
    for (Ship& ship : ships) {
      int ally_id = GetPreviousAllyId(ship);
      if (ally_id >= 0) {
        Student& ally = allies.at(ally_id);
        ally.SetShip(ship);
        ally.SetConnected(true);
      } else if (!IsAlly(ship) && !IsEnemy(ship) && ship != ship_) {
        enemy_ships_.push_back(ship);
      }
    }
  }

  previous_allies_.clear();

  for (Student& ally : allies) {
    previous_allies_.push_back(ally.GetShip());
  }

  ships_.clear();
  ships_.resize(allies.size() + 1);
  return true;
}

/**
 * Prints the debug message for this tick.
 */
void Bot::PrintDebug() {
  std::cout << "allies:";

  for (std::size_t i = 0; i < allies.size(); i++) {
    Student& ally = allies.at(i);
    Ship s = ally.GetShip();

    if (!ally.IsConnected()) {
      continue;
    }

    std::cout << i << '-';
    std::cout << s.GetX() << ',';
    std::cout << s.GetY() << ',';
    std::cout << s.GetHealth() << ',';
    std::cout << s.GetFlag() << ',';
    std::cout << s.GetType() << '|';
  }

  std::cout << "enemies:";

  for (Ship& s : enemy_ships_) {
    std::cout << s.GetX() << ',';
    std::cout << s.GetY() << ',';
    std::cout << s.GetHealth() << ',';
    std::cout << s.GetFlag() << ',';
    std::cout << s.GetType() << '|';
  }

  std::cout << std::endl;
}

/**
 * Checks if ship was probably an ally in the previous tick.
 *
 * @param to_check The ship to check.
 * @return {@code true} if the ship was considered an ally, otherwise
 *         {@code false}.
 */
int Bot::GetPreviousAllyId(Ship& to_check) {
  int i = 0;

  for (Ship& ally : previous_allies_) {
    if (ally.GetType() == to_check.GetType() &&
        ally.DistanceTo(to_check) <= 3) {
      return i;
    }
    i++;
  }
  return -1;
}

/**
 * Returns if a ship is considered an ally.
 *
 * @param to_check The ship to check.
 * @return {@code true} if the ship is an ally, otherwise {@code false}.
 */
bool Bot::IsAlly(Ship& to_check) {
  int flag = to_check.GetFlag() ^ '.';
  int difference_x = abs(flag - to_check.GetX());

  if (difference_x < 3) {
    return true;
  }

  for (Student& ally : allies) {
    Ship ship = ally.GetShip();
    if (ship == to_check) {
      return true;
    }
  }
  return false;
}

/**
 * Returns if the ship is considered an enemy.
 *
 * @param to_check The ship to check.
 * @return {@code true} if the ship is an enemy, otherwise {@code false}.
 */
bool Bot::IsEnemy(Ship& to_check) {
  for (Ship& enemy : enemy_ships_) {
    if (enemy == to_check) {
      return true;
    }
  }
  return false;
}

/**
 * Initializes the bot state and connections.
 */
bool Bot::Setup() {
  // Create server socket.
  return server_connection_.CreateSocket() &&

    // Create and attach the client socket.
    client_connection_.CreateSocket() &&
    client_connection_.Attach() &&

    // Create and attach the hive socket.
    hive_connection_.CreateSocket() &&
    hive_connection_.Attach();
}

/**
 * Begins all the bot threads and awaits user input.
 */
void Bot::Run() {
  // Print the startup messages.
  std::cout << std::endl << "=====================" << std::endl;
  std::cout << std::endl << "   Hive Bot Loaded   " << std::endl;
  std::cout << std::endl << "=====================" << std::endl;

  // Prepare ships_ vector for ships to be loaded in.
  ships_.resize(allies.size() + 1);

  // Set our identities connection to the server connection.
  identity.SetConnection(std::move(server_connection_));

  // Load and run each ally into their own hive relay thread.
  for (std::vector<Student>::size_type i = 0; i < allies.size(); i++) {
    Student& ally = allies.at(i);
    Connection ally_connection = CreateConnection(ally.GetIp(), hive_port);

    if (!ally_connection.CreateSocket()) {
      continue;
    }

    ally.SetConnection(std::move(ally_connection));

    std::thread hive_thread(&Bot::HiveLoop, this, i);
    hive_threads_.push_back(std::move(hive_thread));
  }

  // Create and run the server thread.
  server_thread_ = std::thread(&Bot::ServerLoop, this);

  // Run the command manager.
  CommandManager command_manager(this);
  command_manager.Run();
}

/**
 * Closes opened sockets and discontinues running connection threads.
 */
void Bot::Close() {
  // Close server, client and hive sockets.
  server_connection_.CloseSocket();
  client_connection_.CloseSocket();
  hive_connection_.CloseSocket();

  for (Student& ally : allies) {
    ally.GetConnection().CloseSocket();
  }

  // Cleanup Windows socket API.
  WSACleanup();

  // Join all connection threads as they shutdown.
  server_thread_.join();

  for (std::thread& hive_thread : hive_threads_) {
    hive_thread.join();
  }
}

} // namespace hive_bot
