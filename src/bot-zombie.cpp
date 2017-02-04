#include <iostream>
#include <math.h>
#include <thread>
#include <vector>
#include "bot-zombie.hpp"
#include "main.hpp"

void bot_zombie::run() {
	setup();

	std::cout << std::endl << "===========================" << std::endl;
	std::cout << std::endl << "     Zombie bot loaded     " << std::endl;
	std::cout << std::endl << "===========================" << std::endl;

	std::thread relay_server_thread(&bot_zombie::relay_server, this);
	std::thread relay_master_thread(&bot_zombie::relay_master, this);

	getchar();

	close();
	exit(0);
}

void bot_zombie::setup() {
	net.setup();
	net.respawn(bot_class);

	master.set_socket(create_socket());
	zombie.set_socket(create_socket());

	bind(zombie.get_socket(), zombie.get_address());
}

void bot_zombie::relay_server() {
	char buffer[4096];

	for (;;) {
		if (!net.receive(master, zombie, buffer, sizeof(buffer))) {
			continue;
		}

		net.send(net.get_server(), buffer);
	}
}

void bot_zombie::relay_master() {
	char buffer[4096];

	for (;;) {
		if (!net.receive(net.get_server(), net.get_client(), buffer, sizeof(buffer))) {
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
}
