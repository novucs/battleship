#include <ctype.h>
#include <iostream>
#include "bot_zombie.hpp"
#include "main.hpp"
#include "protocol_utils.hpp"

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
	if (!server.create_socket() ||
			!client.create_socket() ||
			!client.attach() ||
			!master.create_socket() ||
			!zombie.create_socket() ||
			!zombie.attach()) {
		return false;
	}

	relay_server_thread = std::thread(&bot_zombie::relay_server, this);
	relay_master_thread = std::thread(&bot_zombie::relay_master, this);

	server.send_respawn(identity, bot_class);
	return true;
}

void bot_zombie::relay_server() {
	char buffer[4096];

	for (;;) {
		memset(buffer, '\0', sizeof(buffer));

		switch (zombie.receive(master, buffer, sizeof(buffer))) {
			case RETREIVE_SUCCESS:
				break;
			case RETREIVE_FAIL:
				return;
			case RETREIVE_IGNORE:
				continue;
		}

		server.send(buffer);
	}
}

void bot_zombie::relay_master() {
	char buffer[4096];

	for (;;) {
		memset(buffer, '\0', sizeof(buffer));

		switch (client.receive(server, buffer, sizeof(buffer))) {
			case RETREIVE_SUCCESS:
				break;
			case RETREIVE_FAIL:
				return;
			case RETREIVE_IGNORE:
				continue;
		}

		if (isdigit(buffer[0])) {
			// Parse ships first and then send, to include our own ship type.
			master.send_ships(read_ships(true, buffer));
		} else {
			master.send(buffer);
		}
	}
}

void bot_zombie::close() {
	server.close_socket();
	client.close_socket();
	master.close_socket();
	zombie.close_socket();
	WSACleanup();

	relay_server_thread.join();
	relay_master_thread.join();
}
