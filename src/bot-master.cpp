#include <chrono>
#include <iostream>
#include "bot-master.hpp"
#include "main.hpp"

void bot_master::run() {
	setup();

	std::cout << std::endl << "===========================" << std::endl;
	std::cout << std::endl << "     Master bot loaded     " << std::endl;
	std::cout << std::endl << "===========================" << std::endl;

	for (std::vector<std::string>::size_type i = 0; i < zombie_ips.size(); i++) {
		std::thread zombie_thread(&bot_master::zombie_loop, this, i);
		zombie_threads.push_back(std::move(zombie_thread));
	}

	server_thread = std::thread(&bot_master::server_loop, this);

	getchar();

	net.close();
	exit(0);
}

void bot_master::setup() {
	net.setup();
	net.respawn(bot_class);

	for (std::string zombie_ip : zombie_ips) {
		connection zombie = create_connection(zombie_ip, zombie_port);
		zombie.set_socket(create_socket());
		zombies.push_back(std::move(zombie));
	}

	master.set_socket(create_socket());

	bind(master.get_socket(), master.get_address());
}

void bot_master::zombie_loop(int id) {
	char buffer[4096];
	connection zombie = zombies[id];

	for (;;) {
		if (!net.receive(zombie, master, buffer, sizeof(buffer))) {
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
		if (!net.receive(net.get_server(), net.get_client(), buffer, sizeof(buffer))) {
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
