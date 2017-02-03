#include <iostream>
#include <math.h>
#include <vector>
#include "bot-master.hpp"
#include "settings.hpp"

void bot_master::run() {
	net.setup();
	net.respawn(BOT_CLASS);

	std::cout << std::endl << "===========================" << std::endl;
	std::cout << std::endl << "     Master bot loaded     " << std::endl;
	std::cout << std::endl << "===========================" << std::endl;

	char buffer[4096];

	for (;;) {
		if (!net.receive(net.get_client(), buffer, sizeof(buffer))) {
			std::cout << "Failed to receive data: " << WSAGetLastError() << std::endl;
			return;
		}

		if (buffer[0] == 'M') {
			continue;
		}

		ships = net.read_ships(buffer);
	}

	net.close();
}
