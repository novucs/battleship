#include <chrono>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "bot.hpp"
#include "main.hpp"
#include "protocol_utils.hpp"

void bot::run() {
	if (!setup()) {
		return;
	}

	std::cout << std::endl << "==========================" << std::endl;
	std::cout << std::endl << "   Hive mind bot loaded   " << std::endl;
	std::cout << std::endl << "==========================" << std::endl;

	// Wait for user input.
	std::cout << std::endl << "Enter commands here, type '/help' for help." << std::endl;
	getchar();

	close();
}

bool bot::setup() {
	if (!server_connection.create_socket() ||
			!client_connection.create_socket() ||
			!client_connection.attach() ||
			!hive_mind_connection.create_socket() ||
			!hive_mind_connection.attach()) {
		return false;
	}

	loaded_ships.resize(allies.size() + 1);
	identity.set_connection(std::move(server_connection));

	for (std::vector<student>::size_type i = 0; i < allies.size(); i++) {
		student& ally = allies.at(i);
		connection ally_connection = create_connection(ally.get_ip(), hive_mind_port);

		if (!ally_connection.create_socket()) {
			continue;
		}

		ally.set_connection(std::move(ally_connection));

		std::thread hive_mind_thread(&bot::hive_mind_loop, this, i);
		hive_mind_threads.push_back(std::move(hive_mind_thread));
	}

	server_thread = std::thread(&bot::server_loop, this);

	respawn();

	return true;
}

void bot::hive_mind_loop(int id) {
	char buffer[4096];
	student& ally = allies.at(id);
	connection ally_connection = ally.get_connection();

	for (;;) {
		memset(buffer, '\0', sizeof(buffer));

		switch (hive_mind_connection.receive(ally_connection, buffer, sizeof(buffer))) {
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
		loaded_ships.at(id + 1) = ships;

		if (!ships.empty()) {
			ally.set_connected(true);
			ally.set_ship(std::move(ships.at(0)));
		}

		loaded_ships_mutex.unlock();
	}
}

void bot::server_loop() {
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

		for (student& ally : allies) {
			ally.get_connection().send_ships(loaded_ships.at(0));
		}

		loaded_ships_mutex.unlock();

		// Ghetto sleep to receive zombie data.
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		loaded_ships_mutex.lock();
		if (!merge_ships()) {
			continue;
		}

		perform_tactics();

		// Flag allies as not connected for next tick, allowing them to correct this.
		for (student& ally : allies) {
			ally.set_connected(false);
		}

		loaded_ships_mutex.unlock();
	}
}

bool bot::merge_ships() {
	enemy_ships.clear();

	if (loaded_ships.empty() || loaded_ships.at(0).empty()) {
		return false;
	}

	this_ship = loaded_ships.at(0).at(0);
	identity.set_ship(std::move(this_ship));

	for (std::vector<ship>& ships : loaded_ships) {
		for (ship& ship : ships) {
			if (!is_ally(ship) && !is_enemy(ship) && ship != this_ship) {
				enemy_ships.push_back(ship);
			}
		}
	}

	loaded_ships.clear();
	loaded_ships.resize(allies.size() + 1);
	return true;
}

bool bot::is_ally(ship& to_check) {
	for (student& ally : allies) {
		ship ship = ally.get_ship();
		if (ship == to_check) {
			return true;
		}
	}
	return false;
}

bool bot::is_enemy(ship& to_check) {
	for (ship& enemy : enemy_ships) {
		if (enemy == to_check) {
			return true;
		}
	}
	return false;
}

void bot::fire(int x, int y) {
	identity.get_connection().send_fire(identity.get_id(), x, y);
}

void bot::move(int x, int y) {
	identity.get_connection().send_move(identity.get_id(), x, y);
}

void bot::flag(int flag) {
	identity.get_connection().send_flag(identity.get_id(), flag);
}

void bot::respawn() {
	std::string id = identity.get_id();
	std::string forename = identity.get_forename();
	std::string surname = identity.get_surname();
	identity.get_connection().send_respawn(id, forename, surname, ship_type);
}

void bot::perform_tactics() {
	// Apply default movement: towards map center.
	int move_x = this_ship.get_x() < 500 ? 2 : -2;
	int move_y = this_ship.get_y() < 500 ? 2 : -2;

	// Calculate:
	// - number of connected allies
	// - nearby allies
	// - the overall center between all allies
	std::size_t active_ally_count = 0;
	std::vector<ship> nearby_allies;
	int center_x = this_ship.get_x();
	int center_y = this_ship.get_y();

	for (student& ally : allies) {
		if (!ally.is_connected()) {
			continue;
		}

		ship ally_ship = ally.get_ship();
		center_x += ally_ship.get_x();
		center_y += ally_ship.get_y();
		active_ally_count++;

		if (ally_ship.distance_to(this_ship) < 10) {
			nearby_allies.push_back(ally_ship);
		}
	}

	int group_size = active_ally_count + 1;
	center_x /= group_size;
	center_y /= group_size;

	// Move towards ally center when we are not nearby all allies.
	if (nearby_allies.size() < active_ally_count) {
		move_x = this_ship.get_x() < center_x ? 2 : -2;
		move_y = this_ship.get_y() < center_y ? 2 : -2;

		if (this_ship.get_x() == center_x) move_x = 0;
		if (this_ship.get_y() == center_y) move_y = 0;
	}

	ship target;
	double target_distance;
	bool target_found = false;

	for (ship& enemy_ship : enemy_ships) {
		if (!target_found || target_distance > enemy_ship.distance_to(this_ship)) {
			target = enemy_ship;
			target_distance = enemy_ship.distance_to(this_ship);
			target_found = true;
		}
	}

	if (target_found) {
		// Fire at target if theres a chance to damage.
		if (this_ship.get_final_damage(target) > 0) {
			std::cout << "Fired: " << time(0) << std::endl;
			fire(target.get_x(), target.get_y());
		}

		if (target.get_final_range() + 2 < this_ship.distance_to(target) &&
				target.get_final_range() <= this_ship.get_final_range()) {
			move_x = this_ship.get_x() < target.get_x() ? 2 : -2;
			move_y = this_ship.get_y() < target.get_y() ? 2 : -2;
		}

		// Otherwise move away.
		else {
			/*move_x = this_ship.get_x() > target.get_x() ? 2 : -2;
			move_y = this_ship.get_y() > target.get_y() ? 2 : -2;*/
		}
	}

	move(move_x, move_y);
}

void bot::close() {
	server_connection.close_socket();
	client_connection.close_socket();
	hive_mind_connection.close_socket();

	for (student& ally : allies) {
		ally.get_connection().close_socket();
	}

	WSACleanup();

	server_thread.join();

	for (std::thread& hive_mind_thread : hive_mind_threads) {
		hive_mind_thread.join();
	}
}