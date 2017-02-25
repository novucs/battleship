#pragma once
#include "bot_id_collection.hpp"
class scenario_trigger
{
private:
	std::vector<bot_identity> bot_ids;
public:
	int trigger_scenario();
};

