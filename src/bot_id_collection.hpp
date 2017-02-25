#pragma once
#include <vector>
#include "bot_id.hpp"

class bot_id_collection
{
private:
	std::vector<bot_identity> bot_ids;
public:
	std::vector<bot_identity> get_bot_ids();
	void set_role_active(int id_num);
};


