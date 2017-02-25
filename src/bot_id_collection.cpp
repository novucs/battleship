#pragma once
#include "bot_id_collection.hpp"


//Initialises the default framework of all bot roles, their descriptions and by default activity is false.
std::vector<bot_identity> bot_id_collection::get_bot_ids()
{
	bot_ids = {
		bot_identity(
			0,
			std::string("Battleship 1 - Lone Bot"),
			false
		),
		bot_identity(
			1,
			std::string("Battleship 2 - Lone Bot"),
			false
		),
		bot_identity(
			2,
			std::string("Frigate 1 - Lone Bot"),
			false
		),
		bot_identity(
			3,
			std::string("Frigate 2 - Lone Bot"),
			false
		),
		bot_identity(
			4,
			std::string("Battleship 1 - In Pair of Battleships"),
			false
		),
		bot_identity(
			5,
			std::string("Battleship 2 - In Pair of Battleships"),
			false
		),
		bot_identity(
			6,
			std::string("Frigate 1 - In Pair of Frigates"),
			false
		),
		bot_identity(
			7,
			std::string("Frigate 2 - In Pair of Frigates"),
			false
		),
		bot_identity(
			8,
			std::string("Battleship - In Mixed Pair 1"),
			false
		),
		bot_identity(
			9,
			std::string("Frigate - in Mixed Pair 1"),
			false
		),
		bot_identity(
			10,
			std::string("Battleship - In Mixed Pair 2"),
			false
		),
		bot_identity(
			11,
			std::string("Frigate - in Mixed Pair 2"),
			false
		),
		bot_identity(
			12,
			std::string("Battleship 1 - in BBF Trio"),
			false
		),
		bot_identity(
			13,
			std::string("Battleship 2 - in BBF Trio"),
			false
		),
		bot_identity(
			14,
			std::string("Frigate - in BBF Trio"),
			false
		),
		bot_identity(
			15,
			std::string("Battleship - in BFF Trio"),
			false
		),
		bot_identity(
			16,
			std::string("Frigate 1 - in BFF Trio"),
			false
		),
		bot_identity(
			17,
			std::string("Frigate 2 - in BFF Trio"),
			false
		),
		bot_identity(
			18,
			std::string("Battleship 1 - in BBFF Pack"),
			false
		),
		bot_identity(
			19,
			std::string("Battleship 2 - in BBFF Pack"),
			false
		),
		bot_identity(
			20,
			std::string("Frigate 1 - in BBFF Pack"),
			false
		),
		bot_identity(
			21,
			std::string("Frigate 2 - in BBFF Pack"),
			false
		)
	};
	return bot_ids;
}

//sets the active role value of the input ID to true
void bot_id_collection::set_role_active(int id_num)
{
	for (int i = 0; i <= 21; i++) {
		int role_id = bot_ids[i].get_role_id();
		if (role_id == id_num) {
			bot_ids[i].set_active_role(true);
		}
	}
}
