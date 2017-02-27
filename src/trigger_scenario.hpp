#pragma once
#include "bot_role.hpp"
#include "bot_id.hpp"
#include "bot_id_collection.hpp"
#include "bot.hpp"
#include <iostream>
#include <sstream>
#include <string>
static int scenario;
int trigger_scenario(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);
int create_scenario_key(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships);
