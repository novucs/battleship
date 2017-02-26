#pragma once

#include "ship.hpp"
#include <vector>
#include "bot_id.hpp"
#include "main.hpp"

#define LONEDISTANCE 100
static int ally_ID = 0;//Team ID of ship having state defined
static int that_ally_ID = 0;//Team ID of ship being compared
static int thisI;//index of ship being compared
static int thatI; //index of ship being compared against
static int far_count = 0;
static bool bs1 = false;
static bool bs2 = false;
static bool f1 = false;
static bool f2 = false;
static bool lone_bot;
static int lone_count;
static bool bs_pair;
static bool f_pair;
static bool m_pair1;
static bool m_pair2;
static bool bbf_trio;
static bool bff_trio;
static bool complete_pack;
static int bs1_pos = 4;
static int bs2_pos = 4;
static int f1_pos = 4;
static int f2_pos = 4;


bool is_near(ship check_ship, ship against_ship);
bool is_near_any(int bot_pos, std::vector<ship> team_ships);
std::vector<ship> build_active_team(std::vector<bot_identity> bot_ids);
void check_lone_status(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 0,1,2,3
void check_bship_pair(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 4,5
void check_frigate_pair(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 6,7
void check_mixed_pair1(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 8,9
void check_mixed_pair2(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 10,11
void check_BBF_trio(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 12,13,14
void check_BFF_trio(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 15,16,17
void check_complete_pack(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);//Sets active_roles 18,19,20,21
void check_all_roles(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships); //Runs all methods, sets all roles.

