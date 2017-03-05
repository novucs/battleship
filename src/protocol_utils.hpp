/*
 * protocol_utils.hpp
 *
 * Provides utility methods for encoding and decoding the battleship bots
 * protocol.
 */

#pragma once

#include <vector>
#include "ship.hpp"
#include "tick_packet.hpp"

/**
 * Encrypts and decrypts a message using XOR encryption.
 *
 * @param message The message to encrypt.
 * @return the encrypted message.
 */
std::string xor_encrypt(std::string message);

/**
 * Reads in a tick packet.
 *
 * @param message The message to read.
 * @return the parsed tick packet.
 */
tick_packet read_tick_packet(char* message);

/**
 * Reads in a list of ships.
 *
 * @param convert Apply the correct ship type to the first ship.
 * @param message The message to read.
 * @return the parsed list of ships.
 */
std::vector<ship> read_ships(bool convert, char* message);

/**
 * Writes a fire packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the fire packet.
 */
std::string write_fire(std::string id, int x, int y);

/**
 * Writes a movement packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the movement packet.
 */
std::string write_move(std::string id, int x, int y);

/**
 * Writes a flag change packet.
 *
 * @param id   The student ID.
 * @param flag The new flag.
 * @return the flag change packet.
 */
std::string write_flag(std::string id, int flag);

/**
 * Writes a respawn packet.
 *
 * @param id        The student ID.
 * @param forename  The student forename.
 * @param surname   The student surname.
 * @param ship_type The new ship type.
 * @return the respawn packet.
 */
std::string write_respawn(std::string id, std::string forename,
  std::string surname, int ship_type);

/**
 * Writes a tick packet.
 *
 * @param packet The packet to write.
 * @return the tick packet.
 */
std::string write_tick_packet(tick_packet packet);

/**
 * Writes a ships packet.
 *
 * @param ships The ships to write.
 * @return the ships packet.
 */
std::string write_ships(std::vector<ship> ships);
