#include <iostream>
#include "bot-zombie.hpp"
#include "main.hpp"

void bot_zombie::run() {
	if (!setup()) {
		return;
	}

	std::cout << std::endl << "===========================" << std::endl;
	std::cout << std::endl << "     Zombie bot loaded     " << std::endl;
	std::cout << std::endl << "===========================" << std::endl;

	// Wait for user input.
	std::cout << std::endl << "Enter commands here, type '/help' for help." << std::endl;
	getchar();

	close();
}

bool bot_zombie::setup() {
	if (!net.setup() ||
			!master.create_socket() ||
			!zombie.create_socket() ||
			!zombie.attach()) {
		return false;
	}

	relay_server_thread = std::thread(&bot_zombie::relay_server, this);
	relay_master_thread = std::thread(&bot_zombie::relay_master, this);

	net.respawn(bot_class);
	return true;
}

void bot_zombie::relay_server() {
	char buffer[4096];

	for (;;) {
		switch (net.receive(master, zombie, buffer, sizeof(buffer))) {
			case RETREIVE_SUCCESS:
				break;
			case RETREIVE_FAIL:
				return;
			case RETREIVE_IGNORE:
				continue;
		}

		net.send(net.get_server(), buffer);
	}
}

void bot_zombie::relay_master() {
	char buffer[4096];

	for (;;) {
		switch (net.receive(net.get_server(), net.get_client(), buffer, sizeof(buffer))) {
			case RETREIVE_SUCCESS:
				break;
			case RETREIVE_FAIL:
				return;
			case RETREIVE_IGNORE:
				continue;
		}

		net.send(master, buffer);
	}
}

void bot_zombie::close() {
	net.close();
	closesocket(master.get_socket());
	closesocket(zombie.get_socket());
	WSACleanup();

	relay_server_thread.join();
	relay_master_thread.join();
}
