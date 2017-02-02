#include <iostream>
#include <math.h>
#include <unordered_set>
#include <vector>

#include "bot.hpp"
#include "connection.hpp"
#include "settings.hpp"
#include "ship.hpp"

network_manager net;
ship* me;
std::vector<ship*> ships;

// 16002374 = will
// 16000587 = josh
// 15019771 = jake
// 16014980 = gareth
std::unordered_set<int> friendIds = {16002374, 16000587, 15019771, 16014980};

void tactics() {
}

void run() {
	net.setup();
	char buffer[4096];
	net.respawn(0);

	std::cout << std::endl << "===========================" << std::endl;
	std::cout << std::endl << "     Master bot loaded     " << std::endl;
	std::cout << std::endl << "===========================" << std::endl;

	for (;;) {
		if (!net.receive(net.get_client(), buffer, sizeof(buffer))) {
			std::cout << "Failed to receive data: " << WSAGetLastError() << std::endl;
			return;
		}

		if (buffer[0] == 'M') {
			continue;
		}

		ships = net.load_ships(buffer);
		tactics();
	}
	net.close();
}
