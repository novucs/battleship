/*
 * main.cpp
 *
 * Stores all program settings and implements both the main method and main.hpp.
 */

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include "bot.hpp"
#include "main.hpp"

bool debug = false;

std::string key = "gY0HGe3oclNj9bc";
// std::string server_ip = "164.11.80.69";
std::string server_ip = "127.0.0.1";

u_short server_port = 1924;
u_short client_port = 1925;
u_short hive_mind_port = 13337;

int team_member_id = 0;
int ship_type = SHIP_TYPE_BATTLESHIP;
int offset = 5;

std::vector<student> team = {
	student(
		0, offset, offset,
		"16002374",
		"William", "Randall",
		"164.11.80.37"
	),
	student(
		1, -offset, -offset,
		"16014980",
		"Gareth", "Perry",
		"164.11.80.26"
	),
	student(
		2, -offset, offset,
		"16000587",
		"Joshua", "Ramini",
		"164.11.80.33"
	),
	student(
		3, offset, -offset,
		"15019771",
		"Jacob", "Johnston",
		"164.11.80.65"
	)
};

student identity = team.at(team_member_id);
std::vector<student> allies;

/**
 * Sets up the Windows socket API for our program.
 */
bool setup_networking() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

/**
 * Program starts here.
 *
 * @param argc The argument count.
 * @param argv The program arguments.
 * @return the exit code.
 */
int main(int argc, char* argv[]) {
	for (std::size_t i = 0; i < team.size(); i++) {
		if (i == (std::size_t) team_member_id) {
			continue;
		}

		allies.push_back(team.at(i));
	}

	if (!setup_networking()) {
		std::cout << "Failed to initialize WinSock2: " << WSAGetLastError() << std::endl;
	} else {
		bot bot;

		if (bot.setup()) {
			bot.run();
		} else {
			std::cout << std::endl << "An error occured while initializing the bot." << std::endl;
		}

		bot.close();
	}

	system("pause");
	std::cout << std::endl << "Goodbye!" << std::endl;
	return 0;
}
