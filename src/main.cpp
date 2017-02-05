#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include "bot-master.hpp"
#include "bot-zombie.hpp"
#include "main.hpp"

std::string server_ip = "127.0.0.1";
std::string master_ip = "127.0.0.1";

u_short server_port = 1924;
u_short client_port = 1925;
u_short master_port = 13337;
u_short zombie_port = 13338;

const int zombie_type = 0;
const int master_type = 1;
int bot_type = master_type;

const int battleship_class = 0;
const int frigate_class = 1;
const int submarine_class = 2;
int bot_class = battleship_class;

bot* bot;

student identity(
	"16002374",
	"William", "Randall",
	"127.0.0.1"
);

std::vector<student> allies = {
	student(
		"16000587",
		"Joshua", "Ramini",
		"127.0.0.1"
	),
	student(
		"15019771",
		"Jacob", "Johnston",
		"127.0.0.1"
	),
	student(
		"16014980",
		"Gareth", "Perry",
		"127.0.0.1"
	)
};

bool setup_networking() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void load_bot() {
	switch (bot_type) {
		case master_type:
			bot = new bot_master();
			break;
		case zombie_type:
			bot = new bot_zombie();
			break;
	}
}

int main(int argc, char* argv[]) {
	if (!setup_networking()) {
		std::cout << "Failed to initialize WinSock2: " << WSAGetLastError() << std::endl;
	} else {
		load_bot();
		bot->run();
	}

	system("pause");
	std::cout << std::endl << "Goodbye!" << std::endl;
	return 0;
}
