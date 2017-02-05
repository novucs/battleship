#include <chrono>
#include <iostream>
#include <sstream>
#include "bot_master.hpp"
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
	if (!server.create_socket() ||
			!client.create_socket() ||
			!client.attach() ||
			!master.create_socket() ||
			!master.attach()) {
		return false;
	}

	loaded_ships.resize(allies.size() + 1);

	for (std::vector<student>::size_type i = 0; i < allies.size(); i++) {
		student ally = allies.at(i);
		connection zombie = create_connection(ally.get_ip(), zombie_port);

		if (!zombie.create_socket()) {
			continue;
		}

		zombies.push_back(std::move(zombie));

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
		loaded_ships.at(id) = read_ships(buffer);
		loaded_ships_mutex.unlock();
	}
}

void bot_master::server_loop() {
	char buffer[4096];

	for (;;) {
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

		loaded_ships.at(0) = read_ships(buffer);

		// Ghetto sleep to receive zombie data.
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if (!merge_ships()) {
			continue;
		}

		perform_tactics();
	}
}

std::vector<ship> bot_master::read_ships(char* message) {
	std::vector<ship> ships;
	std::stringstream stream(message);

	int x;
	int y;
	int health;
	int flag;
	int type = 0;
	char separator;

	while (!stream.eof()) {
		stream >> x >> separator;
		stream >> y >> separator;
		stream >> health >> separator;
		stream >> flag >> separator;

		if (separator != '|') {
			stream >> type >> separator;
		} else {
			type = bot_class;
		}

		ships.push_back(ship(x, y, health, flag, type));
	}

	return ships;
}

bool bot_master::merge_ships() {
	loaded_ships_mutex.lock();
	ally_ships.clear();
	enemy_ships.clear();

	if (loaded_ships.empty() || loaded_ships.at(0).empty()) {
		return false;
	}

	me = loaded_ships.at(0).at(0);

	for (std::vector<ship>& ships : loaded_ships) {
		if (!ships.empty() && !contains_similar(ally_ships, ships.at(0))) {
			ally_ships.push_back(ships.at(0));
		}
	}

	for (std::vector<ship>& ships : loaded_ships) {
		for (ship& ship : ships) {
			if (!contains_similar(ally_ships, ship) && !contains_similar(enemy_ships, ship)) {
				enemy_ships.push_back(ship);
			}
		}
	}

	loaded_ships.clear();
	loaded_ships.resize(allies.size() + 1);
	loaded_ships_mutex.unlock();
	return true;
}

bool bot_master::contains_similar(std::vector<ship>& ships, ship to_check) {
	for (ship& s : ships) {
		if (s.get_x() == to_check.get_x() &&
				s.get_y() == to_check.get_y() &&
				s.get_health() == to_check.get_health() &&
				s.get_flag() == to_check.get_flag()) {
			return true;
		}
	}
	return false;
}

void bot_master::perform_tactics() {
}

void bot_master::close() {
	server.close_socket();
	client.close_socket();
	master.close_socket();

	for (connection& zombie : zombies) {
		zombie.close_socket();
	}

	WSACleanup();

	server_thread.join();

	for (std::thread& zombie_thread : zombie_threads) {
		zombie_thread.join();
	}
}
