/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * protocol_utils.cc - Protocol utils implementation.
 *
 * Provides a subset of protocol utilities for both the battleship bot and hive
 * bot protocols.
 */

#include "protocol_utils.h"

#include <sstream>

#include "main.h"

namespace hive_bot {

/**
 * Encrypts and decrypts a message using XOR encryption.
 *
 * @param message The message to encrypt.
 * @return the encrypted message.
 */
std::string XorEncrypt(std::string message) {
  // for (std::size_t i = 0; i < message.length(); i++) {
  //   message[i] ^= key[i % key.length()];
  // }
  return message;
}

/**
 * Reads in a tick packet.
 *
 * @param message The message to read.
 * @return the parsed tick packet.
 */
TickPacket ReadTickPacket(char* message) {
  int score;

  std::vector<Ship> ships;
  std::stringstream stream(XorEncrypt(message));

  int x;
  int y;
  int health;
  int flag;
  int type = 0;
  char separator;

  stream >> score >> separator;

  while (!stream.eof() && stream.good()) {
    stream >> x >> separator;
    stream >> y >> separator;
    stream >> health >> separator;
    stream >> flag >> separator;
    stream >> type >> separator;

    ships.push_back(Ship(x, y, health, flag, type));
  }

  return TickPacket(score, ships);
}

/**
 * Reads in a list of ships.
 *
 * @param convert Apply the correct ship type to the first ship.
 * @param message The message to read.
 * @return the parsed list of ships.
 */
std::vector<Ship> ReadShips(bool convert, char* message) {
  std::vector<Ship> ships;
  std::stringstream stream(message);

  int x;
  int y;
  int health;
  int flag;
  int type = 0;
  char separator;

  while (!stream.eof() && stream.good()) {
    stream >> x >> separator;
    stream >> y >> separator;
    stream >> health >> separator;
    stream >> flag >> separator;

    if (convert) {
      type = ship_type;
      convert = false;
    } else {
      stream >> type >> separator;
    }

    ships.push_back(Ship(x, y, health, flag, type));
  }

  return ships;
}

/**
 * Writes a fire packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the fire packet.
 */
std::string WriteFire(std::string id, int x, int y) {
  std::stringstream message;
  message << "Fire " << id << ',' << x << ',' << y;
  return message.str();
}

/**
 * Writes a movement packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the movement packet.
 */
std::string WriteMove(std::string id, int x, int y) {
  std::stringstream message;
  message << "Move " << id << ',' << x << ',' << y;
  return message.str();
}

/**
 * Writes a flag change packet.
 *
 * @param id   The student ID.
 * @param flag The new flag.
 * @return the flag change packet.
 */
std::string WriteFlag(std::string id, int flag) {
  std::stringstream message;
  message << "Flag " << id << ',' << flag;
  return message.str();
}

/**
 * Writes a respawn packet.
 *
 * @param id        The student ID.
 * @param forename  The student forename.
 * @param surname   The student surname.
 * @param ship_type The new ship type.
 * @return the respawn packet.
 */
std::string WriteRespawn(std::string id, std::string forename,
    std::string surname, int ship_type) {
  std::stringstream message;
  message << "Register  ";
  message << id << ',';
  message << forename << ',';
  message << surname << ',';
  message << ship_type;
  return message.str();
}

/**
 * Writes a tick packet.
 *
 * @param packet The packet to write.
 * @return the tick packet.
 */
std::string WriteTickPacket(TickPacket packet) {
  std::stringstream message;

  message << packet.GetScore() << '$';

  for (Ship& s : packet.GetShips()) {
    message << s.GetX() << ',';
    message << s.GetY() << ',';
    message << s.GetHealth() << ',';
    message << s.GetFlag() << ',';
    message << s.GetType() << '|';
  }

  std::string str = message.str();
  str.pop_back();
  return XorEncrypt(str);
}

/**
 * Writes a ships packet.
 *
 * @param ships The ships to write.
 * @return the ships packet.
 */
std::string WriteShips(std::vector<Ship> ships) {
  std::stringstream message;

  for (Ship& s : ships) {
    message << s.GetX() << ',';
    message << s.GetY() << ',';
    message << s.GetHealth() << ',';
    message << s.GetFlag() << ',';
    message << s.GetType() << '|';
  }

  std::string str = message.str();
  str.pop_back();
  return str;
}

} // namespace hive_bot
