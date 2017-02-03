#pragma once

#include <unordered_set>

#define STUDENT_NUMBER "16002374"
#define STUDENT_FIRSTNAME "Billy"
#define STUDENT_FAMILYNAME "Jenkins"

#define SERVER_IP "127.0.0.1"
#define MASTER_IP "127.0.0.1"
#define ZOMBIE_IPS { "127.0.0.1" }

#define SERVER_PORT 1924
#define CLIENT_PORT 1925
#define MASTER_PORT 13337
#define ZOMBIE_PORT 13338

#define ZOMBIE_BOT 0
#define MASTER_BOT 1

#define BATTLESHIP 0
#define FRIGATE 1
#define SUBMARINE 2

#define BOT_TYPE ZOMBIE_BOT
#define BOT_CLASS BATTLESHIP

extern std::unordered_set<int> friend_ids;
