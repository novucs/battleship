/*
 * main.hpp
 *
 * Stores all program settings.
 */

#pragma once

#include <vector>
#include "connection.hpp"
#include "ship.hpp"
#include "student.hpp"

// All ship types.
#define SHIP_TYPE_BATTLESHIP 0
#define SHIP_TYPE_FRIGATE 1
#define SHIP_TYPE_SUBMARINE 2

// Whether to print debug messages.
extern bool debug;

// The key used in hive-mind connections.
extern std::string key;

// The server IP address.
extern std::string server_ip;

// Server, client and hive-mind ports.
extern u_short server_port;
extern u_short client_port;
extern u_short hive_mind_port;

// The current ship type.
extern int ship_type;

// How far to offset ships.
extern int offset;

// Our current student identity.
extern student identity;

// All ally student identities.
extern std::vector<student> allies;
