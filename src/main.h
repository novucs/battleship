/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * main.h - Main header.
 *
 * Stores all program settings and is the program startpoint.
 */

#ifndef HIVE_BOT_MAIN_H_
#define HIVE_BOT_MAIN_H_

#include <vector>

#include "connection.h"
#include "ship.h"
#include "student.h"

namespace hive_bot {

// Whether to print debug messages.
extern bool debug;

// The key used in hive-mind connections.
extern std::string key;

// The server IP address.
extern std::string server_ip;

// Server, client and hive ports.
extern u_short server_port;
extern u_short client_port;
extern u_short hive_port;

// The target number of allies to be frigates at one time.
extern int frigate_count;

// The current ship type.
extern int ship_type;

// How far to offset ships.
extern int offset;

// Our current student identity.
extern Student identity;

// All ally student identities.
extern std::vector<Student> allies;

} // namespace hive_bot

#endif // HIVE_BOT_MAIN_H_
