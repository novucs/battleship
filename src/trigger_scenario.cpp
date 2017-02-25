#include "trigger_scenario.hpp"
#include "bot.hpp"
#include <iostream>
#include <sstream>
#include <string>


int scenario_trigger::trigger_scenario() {
	int scenario = 0;
	for (int i = 0; i <= 2; i++) {
		ship& ally_ship = allies[i].get_ship();
		for (int j = 0; i <= 3; i++) {
			if (i != j) {
				if (ally_ship.distance_to(allies[j].get_ship()) > 100) {//No ally bots are within 100 squares and...
					if (allies[i].get_bsf_id() == 11) {//...ship ID 11 belongs to BattleShip 1, so..
						allies[i].set_role_id(0); //...set bot role to "Battleship 1 - Lone Bot"...
						bot_ids[0].set_active_role(true); //and finally set this role as active
					}
					if (allies[i].get_bsf_id() == 12) {
						allies[i].set_role_id(1); //same as If statement above, only bsf_id 12 means this bot's role is "Battleship 2 - Lone Bot"
						bot_ids[1].set_active_role(true);
					}
					if (allies[i].get_bsf_id() == 21) {
						allies[i].set_role_id(2); //same as If statement above, only bsf_id 21 means this bot is "Frigate 1 - Lone Bot"
						bot_ids[2].set_active_role(true);
					}
					if (allies[i].get_bsf_id() == 22) {
						allies[i].set_role_id(3);  //same as If statement above, only bsf_id 21 means this bot is "Frigate 2 - Lone Bot"
						bot_ids[3].set_active_role(true);
					}
				}
			}
		}
	}


	
	std::string active_roles = "";
	for (int i = 0; i <= 21; i++) {
		if (bot_ids[i].is_active_role()) {//this just interates through the bot_id_collection and records which roles are active...
			active_roles = active_roles + std::to_string(i);//...by concatenating the role ID numbers to create a key....
		}
	}

	std::istringstream buffer(active_roles);
	int active_roles_num;
	buffer >> active_roles_num;//which is then converted to an integer

	switch (active_roles_num) {//so it can be used as a switch
	case 0123://here is roles 0,1,2 and 3 being active. In this case that is 4 lone bots, so it triggers scenario 1.
		scenario = 1;
		break;
	default:
		break;
	}


	return scenario;
}
