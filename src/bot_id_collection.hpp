#pragma once
#include <vector>
#include "bot_id.hpp"
#include "student.hpp"
#include "main.hpp"

void set_role_active(int id_num, std::vector<bot_identity> bot_ids);
std::vector<bot_identity> build_bot_ids();
student get_identity(int id, std::vector<student> allies);
std::vector<student> build_allies();