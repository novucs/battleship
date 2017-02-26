#include "trigger_scenario.hpp"


int trigger_scenario(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships)
{
	int scenario_key = create_scenario_key(bot_ids, team_ships);

	switch (scenario_key) {//so it can be used as a switch

	case 45://BattleShip Pair
		scenario = 2;
		break;
	case 67://Frigate Pair
		scenario = 3;
		break;
	case 89://Mixed Pair 1
		scenario = 4;
		break;
	case 1011://Mixed Pair 2
		scenario = 5;
		break;
	case 4567://Battleship Pair and Frigate Pair
		scenario = 6;
		break;
	case 891011://Mixed Pair 1 and Mixed Pair 2
		scenario = 7;
		break;
	case 121314://Trio of Two Battleships and One Frigate
		scenario = 8;
		break;
	case 151617://Trio of Two Frigates and One Battleship
		scenario = 9;
		break;
	case 18192021://Complete pack
		scenario = 10;
		break;

	default://No pair/trio/pack, only lone bots
		scenario = 1;
		break;
	}


	return scenario;
}

int create_scenario_key(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships) {
	check_all_roles(bot_ids, team_ships);
	std::string active_roles = "";
	for (int i = 4; i <= 21; i++) {
		if (bot_ids[i].is_active_role()) {//this just interates through the bot_id_collection and records which pair/trio/pack roles are active...
			active_roles = active_roles + std::to_string(bot_ids[i].get_role_id());//...by concatenating the role ID numbers to create a key....
		}
	}
	std::istringstream buffer(active_roles);
	int active_roles_num;
	buffer >> active_roles_num;//which is then converted to an integer

	return active_roles_num;
}


