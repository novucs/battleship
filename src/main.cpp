#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include "bot.hpp"
#include "main.hpp"


//std::string server_ip = "164.11.80.69";
std::string server_ip = "127.0.0.1";

u_short server_port = 1924;
u_short client_port = 1925;
u_short hive_mind_port = 13337;
int ship_type = SHIP_TYPE_BATTLESHIP;
#define WILL 0
#define JOSH 1
#define JAKE_ 2
#define GARETH 3



std::vector<student> allies = build_allies();
student identity = get_identity(GARETH, allies);


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
