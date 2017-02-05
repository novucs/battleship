#include <chrono>
#include <iostream>
#include "bot-master.hpp"
#include "main.hpp"

void bot_master::run() {
	if (!setup()) {
		return;
	}

	std::cout << std::endl << "===========================" << std::endl;
	std::cout << std::endl << "     Master bot loaded     " << std::endl;
	std::cout << std::endl << "===========================" << std::endl;

	// Wait for user input.
	std::cout << std::endl << "Enter commands here, type '/help' for help." << std::endl;
	getchar();

	close();
}

bool bot_master::setup() {
	if (!net.setup() || !master.create_socket() || !master.attach()) {
		return false;
	}

	for (student ally : allies) {
		connection zombie = create_connection(ally.get_ip(), zombie_port);

		if (!zombie.create_socket()) {
			continue;
		}

		zombies.push_back(std::move(zombie));

		std::thread zombie_thread(&bot_master::zombie_loop, this, ally, zombie);
		zombie_threads.push_back(std::move(zombie_thread));
	}

	server_thread = std::thread(&bot_master::server_loop, this);

	net.respawn(bot_class);
	return true;
}

void bot_master::zombie_loop(student ally, connection zombie) {
	char buffer[4096];

	for (;;) {
		switch (net.receive(zombie, master, buffer, sizeof(buffer))) {
			case RETREIVE_SUCCESS:
				break;
			case RETREIVE_FAIL:
				return;
			case RETREIVE_IGNORE:
				continue;
		}

		if (buffer[0] == 'M') {
			continue;
		}

		ships = net.read_ships(buffer);
	}
}

void bot_master::server_loop() {
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

		if (buffer[0] == 'M') {
			continue;
		}

		ships = net.read_ships(buffer);

		// Ghetto sleep to receive zombie data.
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		perform_tactics();
	}
}

void bot_master::perform_tactics() {
}

void bot_master::close() {
	net.close();

	for (connection zombie : zombies) {
		zombie.close_socket();
	}

	master.close_socket();

	WSACleanup();

	server_thread.join();

	for (std::thread& zombie_thread : zombie_threads) {
		zombie_thread.join();
	}
}
