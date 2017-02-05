#pragma once

#include <vector>
#include "bot.hpp"
#include "connection.hpp"
#include "ship.hpp"
#include "student.hpp"

extern std::string server_ip;
extern std::string master_ip;

extern u_short server_port;
extern u_short client_port;
extern u_short master_port;
extern u_short zombie_port;

extern const int zombie_type;
extern const int master_type;
extern int bot_type;

extern const int battleship_class;
extern const int frigate_class;
extern const int submarine_class;
extern int bot_class;

extern student identity;
extern std::vector<student> allies;
