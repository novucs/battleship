#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include "bot.hpp"
#include "main.hpp"


std::string server_ip = "164.11.80.69";

u_short server_port = 1924;
u_short client_port = 1925;
u_short hive_mind_port = 13337;

int ship_type = SHIP_TYPE_BATTLESHIP;

student identity(
	0,
	"16002374",
	"William", "Randall",
	"164.11.80.37",
	BATTLESHIP1,
	ROLE_NOT_SET

);

std::vector<student> allies = {
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
		"164.11.80.26",
		FRIGATE2,
		ROLE_NOT_SET
	)
};

bool setup_networking() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

int main(int argc, char* argv[]) {
	if (!setup_networking()) {
		std::cout << "Failed to initialize WinSock2: " << WSAGetLastError() << std::endl;
	} else {
		bot bot;
		bot.run();
	}

	system("pause");
	std::cout << std::endl << "Goodbye!" << std::endl;
	return 0;
}
