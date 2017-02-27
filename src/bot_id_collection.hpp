#pragma once
#include <vector>
#include "bot_id.hpp"


void set_role_active(int id_num, std::vector<bot_identity> bot_ids);
std::vector<bot_identity> build_bot_ids();
