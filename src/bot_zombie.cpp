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
	if (!server_connection.create_socket() ||
			!client_connection.create_socket() ||
			!client_connection.attach() ||
			!master_connection.create_socket() ||
			!zombie_connection.create_socket() ||
			!zombie_connection.attach()) {
		return false;
	}

	relay_server_thread = std::thread(&bot_zombie::relay_server, this);
	relay_master_thread = std::thread(&bot_zombie::relay_master, this);
	return true;
}

void bot_zombie::relay_server() {
	char buffer[4096];

	for (;;) {
		memset(buffer, '\0', sizeof(buffer));

		switch (zombie_connection.receive(master_connection, buffer, sizeof(buffer))) {
			case RETREIVE_SUCCESS:
				break;
			case RETREIVE_FAIL:
				return;
			case RETREIVE_IGNORE:
				continue;
		}

		server_connection.send(buffer);
	}
}

void bot_zombie::relay_master() {
	char buffer[4096];

	for (;;) {
		memset(buffer, '\0', sizeof(buffer));

		switch (client_connection.receive(server_connection, buffer, sizeof(buffer))) {
			case RETREIVE_SUCCESS:
				break;
			case RETREIVE_FAIL:
				return;
			case RETREIVE_IGNORE:
				continue;
		}

		if (isdigit(buffer[0])) {
			// Parse ships first and then send, to include our own ship type.
			master_connection.send_ships(read_ships(true, buffer));
		} else {
			master_connection.send(buffer);
		}
	}
}

void bot_zombie::close() {
	server_connection.close_socket();
	client_connection.close_socket();
	master_connection.close_socket();
	zombie_connection.close_socket();
	WSACleanup();

	relay_server_thread.join();
	relay_master_thread.join();
}
