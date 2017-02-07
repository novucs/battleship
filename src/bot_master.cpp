#include <chrono>
#include <iostream>
#include <sstream>
#include "bot_master.hpp"
#include "main.hpp"
#include "protocol_utils.hpp"

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
	if (!server.create_socket() ||
			!client.create_socket() ||
			!client.attach() ||
			!master.create_socket() ||
			!master.attach()) {
		return false;
	}

	loaded_ships.resize(allies.size() + 1);
	identity.set_connection(std::move(client));

	for (std::vector<student>::size_type i = 0; i < allies.size(); i++) {
		student& ally = allies.at(i);
		connection zombie = create_connection(ally.get_ip(), zombie_port);

		if (!zombie.create_socket()) {
			continue;
		}

		ally.set_connection(std::move(zombie));

		std::thread zombie_thread(&bot_master::zombie_loop, this, (i + 1), ally, zombie);
		zombie_threads.push_back(std::move(zombie_thread));
	}

	server_thread = std::thread(&bot_master::server_loop, this);

	server.send_respawn(identity, bot_class);
	return true;
}

void bot_master::zombie_loop(int id, student ally, connection zombie) {
	char buffer[4096];

	for (;;) {
		memset(buffer, '\0', sizeof(buffer));

		switch (master.receive(zombie, buffer, sizeof(buffer))) {
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

		loaded_ships_mutex.lock();

		std::vector<ship> ships = read_ships(false, buffer);
		loaded_ships.at(id) = ships;

		if (!ships.empty()) {
			ally.set_ship(std::move(ships.at(0)));
		}

		loaded_ships_mutex.unlock();
	}
}

void bot_master::server_loop() {
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

		if (buffer[0] == 'M') {
			continue;
		}

		loaded_ships_mutex.lock();
		loaded_ships.at(0) = read_ships(true, buffer);

		// Ghetto sleep to receive zombie data.
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if (!merge_ships()) {
			continue;
		}

		perform_tactics();
		loaded_ships_mutex.unlock();
	}
}

bool bot_master::merge_ships() {
	enemy_ships.clear();

	if (loaded_ships.empty() || loaded_ships.at(0).empty()) {
		return false;
	}

	me = loaded_ships.at(0).at(0);
	identity.set_ship(std::move(me));

	for (std::vector<ship>& ships : loaded_ships) {
		for (ship& ship : ships) {
			if (!is_ally(ship) && !is_enemy(ship)) {
				enemy_ships.push_back(ship);
			}
		}
	}

	loaded_ships.clear();
	loaded_ships.resize(allies.size() + 1);
	return true;
}

bool bot_master::is_ally(ship& to_check) {
	for (student& ally : allies) {
		ship ship = ally.get_ship();
		if (ship == to_check) {
			return true;
		}
	}
	return false;
}

bool bot_master::is_enemy(ship& to_check) {
	for (ship& enemy : enemy_ships) {
		if (enemy == to_check) {
			return true;
		}
	}
	return false;
}

void bot_master::perform_tactics() {
	server.send_move(identity, 2, 2);

	for (student& ally : allies) {
		ally.get_connection().send_move(ally, 2, 2);
	}
}

void bot_master::close() {
	server.close_socket();
	client.close_socket();
	master.close_socket();

	for (student& ally : allies) {
		connection c = ally.get_connection();
		c.close_socket();
	}

	WSACleanup();

	server_thread.join();

	for (std::thread& zombie_thread : zombie_threads) {
		zombie_thread.join();
	}
}
