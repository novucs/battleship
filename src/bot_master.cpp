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
	if (!server_connection.create_socket() ||
			!client_connection.create_socket() ||
			!client_connection.attach() ||
			!master_connection.create_socket() ||
			!master_connection.attach()) {
		return false;
	}

	loaded_ships.resize(zombies.size() + 1);
	master.set_connection(std::move(server_connection));

	for (std::vector<student>::size_type i = 0; i < zombies.size(); i++) {
		student& zombie = zombies.at(i);
		connection zombie_connection = create_connection(zombie.get_ip(), zombie_port);

		if (!zombie_connection.create_socket()) {
			continue;
		}

		zombie.set_connection(std::move(zombie_connection));

		std::thread zombie_thread(&bot_master::zombie_loop, this, (i + 1), zombie, zombie_connection);
		zombie_threads.push_back(std::move(zombie_thread));
	}

	server_thread = std::thread(&bot_master::server_loop, this);

	master.respawn();

	for (student& zombie : zombies) {
		zombie.respawn();
	}

	return true;
}

void bot_master::zombie_loop(int id, student zombie, connection zombie_connection) {
	char buffer[4096];

	for (;;) {
		memset(buffer, '\0', sizeof(buffer));

		switch (master_connection.receive(zombie_connection, buffer, sizeof(buffer))) {
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
			zombie.set_ship(std::move(ships.at(0)));
		}

		loaded_ships_mutex.unlock();
	}
}

void bot_master::server_loop() {
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

	master_ship = loaded_ships.at(0).at(0);
	master.set_ship(std::move(master_ship));

	for (std::vector<ship>& ships : loaded_ships) {
		for (ship& ship : ships) {
			if (!is_zombie(ship) && !is_enemy(ship) && ship != master_ship) {
				enemy_ships.push_back(ship);
			}
		}
	}

	loaded_ships.clear();
	loaded_ships.resize(zombies.size() + 1);
	return true;
}

bool bot_master::is_zombie(ship& to_check) {
	for (student& zombie : zombies) {
		ship ship = zombie.get_ship();
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
	// Locate the nearest enemy ship to the master bot ...
	ship target;
	double lowest_distance = 0;
	bool target_found = false;

	for (ship& enemy_ship : enemy_ships) {
		double distance = master_ship.distance_to(target);

		if (!target_found || distance < lowest_distance) {
			target = enemy_ship;
			lowest_distance = distance;
			target_found = true;
		}
	}

	// ... if an enemy was found...
	if (target_found) {
		// ... move the master ship towards and fire at it ...
		int move_x = target.get_x() > master_ship.get_x() ? 2 : -2;
		int move_y = target.get_x() > master_ship.get_y() ? 2 : -2;
		int fire_x = target.get_x();
		int fire_y = target.get_y();

		master.move(move_x, move_y);
		master.fire(fire_x, fire_y);

		// ... and make the zombie ships do the same.
		for (student& zombie : zombies) {
			ship zombie_ship = zombie.get_ship();

			move_x = target.get_x() > zombie_ship.get_x() ? 2 : -2;
			move_y = target.get_y() > zombie_ship.get_y() ? 2 : -2;

			zombie.move(move_x, move_y);
			zombie.fire(fire_x, fire_y);
		}
	}

	// ... otherwise ...
	else {
		// ... move the master bot slowly towards the top right corner ...
		int move_x = 900 > master_ship.get_x() ? 1 : -1;
		int move_y = 900 > master_ship.get_y() ? 1 : -1;

		master.move(move_x, move_y);

		// ... and make the zombies move towards the master bot.
		for (student& zombie : zombies) {
			ship zombie_ship = zombie.get_ship();

			move_x = master_ship.get_x() > zombie_ship.get_x() ? 2 : -2;
			move_y = master_ship.get_y() > zombie_ship.get_y() ? 2 : -2;

			zombie.move(move_x, move_y);
		}
	}
}

void bot_master::close() {
	server_connection.close_socket();
	client_connection.close_socket();
	master_connection.close_socket();

	for (student& zombie : zombies) {
		zombie.get_connection().close_socket();
	}

	WSACleanup();

	server_thread.join();

	for (std::thread& zombie_thread : zombie_threads) {
		zombie_thread.join();
	}
}
