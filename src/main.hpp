#pragma once

#include <vector>
#include "ship.hpp"

extern std::string student_number;
extern std::string student_firstname;
extern std::string student_familyname;

extern std::string server_ip;
extern std::string master_ip;
extern std::vector<std::string> zombie_ips;

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

extern std::vector<int> friend_ids;
