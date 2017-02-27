#pragma once

#include <vector>
#include "connection.hpp"
#include "ship.hpp"
#include "student.hpp"
#include "bot_id_collection.hpp"

#define SHIP_TYPE_BATTLESHIP 0
#define SHIP_TYPE_FRIGATE 1
#define SHIP_TYPE_SUBMARINE 2

#define ROLE_NOT_SET 99//Arbitrary number, can be used to catch a potential error

//Ship ID numbers
#define BATTLESHIP1 11
#define BATTLESHIP2 12
#define FRIGATE1 21
#define FRIGATE2 22

extern std::string server_ip;

extern u_short server_port;
extern u_short client_port;
extern u_short hive_mind_port;

extern int ship_type;

extern student identity;
extern std::vector<student> allies;
