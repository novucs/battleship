#pragma once

#include <vector>
#include "connection.hpp"
#include "ship.hpp"
#include "student.hpp"

#define SHIP_TYPE_BATTLESHIP 0
#define SHIP_TYPE_FRIGATE 1
#define SHIP_TYPE_SUBMARINE 2

extern bool debug;

extern std::string server_ip;

extern u_short server_port;
extern u_short client_port;
extern u_short hive_mind_port;

extern int ship_type;

extern student identity;
extern std::vector<student> allies;
