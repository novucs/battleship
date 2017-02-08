#pragma once

#include <vector>
#include "bot.hpp"
#include "connection.hpp"
#include "ship.hpp"
#include "student.hpp"

#define BOT_TYPE_ZOMBIE 0
#define BOT_TYPE_MASTER 1

#define SHIP_TYPE_BATTLESHIP 0
#define SHIP_TYPE_FRIGATE 1
#define SHIP_TYPE_SUBMARINE 2

extern std::string server_ip;

extern u_short server_port;
extern u_short client_port;
extern u_short master_port;
extern u_short zombie_port;

extern student master;
extern std::vector<student> zombies;

extern student& identity;
