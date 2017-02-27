#pragma once
#include "bot_id_collection.hpp"




//Initialises the default framework of all bot roles, their descriptions and by default activity is false.
std::vector<bot_identity> build_bot_ids()
{
	std::vector<bot_identity> bot_ids = {
		bot_identity(
			1,
			std::string("Battleship 1 - Lone Bot"),
			false
		),
		bot_identity(
			2,
			std::string("Battleship 2 - Lone Bot"),
			false
		),
		bot_identity(
			3,
			std::string("Frigate 1 - Lone Bot"),
			false
		),
		bot_identity(
			4,
			std::string("Frigate 2 - Lone Bot"),
			false
		),
		bot_identity(
			5,
			std::string("Battleship 1 - In Pair of Battleships"),
			false
		),
		bot_identity(
			6,
			std::string("Battleship 2 - In Pair of Battleships"),
			false
		),
		bot_identity(
			7,
			std::string("Frigate 1 - In Pair of Frigates"),
			false
		),
		bot_identity(
			8,
			std::string("Frigate 2 - In Pair of Frigates"),
			false
		),
		bot_identity(
			9,
			std::string("Battleship - In Mixed Pair 1"),
			false
		),
		bot_identity(
			10,
			std::string("Frigate - in Mixed Pair 1"),
			false
		),
		bot_identity(
			11,
			std::string("Battleship - In Mixed Pair 2"),
			false
		),
		bot_identity(
			12,
			std::string("Frigate - in Mixed Pair 2"),
			false
		),
		bot_identity(
			13,
			std::string("Battleship 1 - in BBF Trio"),
			false
		),
		bot_identity(
			14,
			std::string("Battleship 2 - in BBF Trio"),
			false
		),
		bot_identity(
			15,
			std::string("Frigate - in BBF Trio"),
			false
		),
		bot_identity(
			16,
			std::string("Battleship - in BFF Trio"),
			false
		),
		bot_identity(
			17,
			std::string("Frigate 1 - in BFF Trio"),
			false
		),
		bot_identity(
			18,
			std::string("Frigate 2 - in BFF Trio"),
			false
		),
		bot_identity(
			19,
			std::string("Battleship 1 - in BBFF Pack"),
			false
		),
		bot_identity(
			20,
			std::string("Battleship 2 - in BBFF Pack"),
			false
		),
		bot_identity(
			21,
			std::string("Frigate 1 - in BBFF Pack"),
			false
		),
		bot_identity(
			22,
			std::string("Frigate 2 - in BBFF Pack"),
			false
		)
	};
	return bot_ids;
}

//sets the active role value of the input ID to true
void set_role_active(int id_num, std::vector<bot_identity> bot_ids)
{
	for (int i = 1; i <= 22; i++) {
		int role_id = bot_ids[i].get_role_id();
		if (role_id == id_num) {
			bot_ids[i].set_active_role(true);
		}
	}
}

student get_identity(int id, std::vector<student> allies) {
	return allies[id];
}
std::vector<student> build_allies() {
	std::vector<student> allies = {
		student(
			0,
			"16002374",
			"William", "Randall",
			"192.168.0.16",
			BATTLESHIP1,
			ROLE_NOT_SET

		),
		student(
			1,
			"16000587",
			"Joshua", "Ramini",
			"164.11.80.33",
			BATTLESHIP2,
			ROLE_NOT_SET
		),
		student(
			2,
			"15019771",
			"Jacob", "Johnston",
			"164.11.80.65",
			FRIGATE1,
			ROLE_NOT_SET
		),
		student(
			3,
			"16014980",
			"Gareth", "Perry",
			"192.168.0.17",
			FRIGATE2,
			ROLE_NOT_SET
		)
	};

	return allies;
}
