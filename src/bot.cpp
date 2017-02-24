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
		loaded_ships.at(ally.get_load_order()) = ships;

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
		loaded_ships.at(identity.get_load_order()) = read_ships(true, buffer);

		for (student& ally : allies) {
			ally.get_connection().send_ships(loaded_ships.at(identity.get_load_order()));
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

	if (loaded_ships.empty() || loaded_ships.at(identity.get_load_order()).empty()) {
		return false;
	}

	this_ship = loaded_ships.at(identity.get_load_order()).at(0);
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

		if (ally_ship.distance_to(this_ship) < 25) {
			nearby_allies.push_back(ally_ship);
		}
	}

	int group_size = active_ally_count + 1;
	center_x /= group_size;
	center_y /= group_size;

	// Set position to avoid in order to keep outside enemy fire ranges, and equip
	// the most beneficial flag.
	int avoid_total_weight = 0;
	int avoid_x = 0;
	int avoid_y = 0;
	std::unordered_map<int, int> flag_weights;

	for (ship& enemy_ship : enemy_ships) {
		int weight = enemy_ship.get_final_damage(this_ship);

		if ((weight == 0) ||
				(enemy_ship.get_type() == SHIP_TYPE_BATTLESHIP &&
				this_ship.get_type() == SHIP_TYPE_SUBMARINE)) {
			continue;
		}

		if (flag_weights.find(enemy_ship.get_flag()) == flag_weights.end()) {
			std::pair<int, int> flag_weight(enemy_ship.get_flag(), 0);
			flag_weights.insert(flag_weight);
		}

		flag_weights.at(enemy_ship.get_flag()) += weight;
		avoid_x += weight * enemy_ship.get_x();
		avoid_y += weight * enemy_ship.get_y();
		avoid_total_weight += weight;
	}

	if (avoid_total_weight > 0) {
		avoid_x /= avoid_total_weight;
		avoid_y /= avoid_total_weight;
	}

	int new_flag = 0;
	int highest_flag_weight = 0;

	for (auto& entry : flag_weights) {
		if (highest_flag_weight >= entry.second) {
			continue;
		}

		new_flag = entry.first;
		highest_flag_weight = entry.second;
	}

	// Target crippled allies.
	ship target;
	int target_weight = 0;
	bool target_found = false;

	for (ship& ally_ship : nearby_allies) {
		if (ally_ship.get_health() > 3) {
			continue;
		}

		target = ally_ship;
		target_found = true;
	}

	// Target closer enemies when no crippled allies found.
	if (!target_found) {
		for (ship& enemy_ship : enemy_ships) {
			int weight = 0;

			// weight += this_ship.get_final_damage(enemy_ship);
			// weight -= enemy_ship.get_final_damage(this_ship);
			weight -= this_ship.distance_to(enemy_ship);

			for (ship& ally_ship : nearby_allies) {
					// weight += this_ship.get_final_damage(ally_ship);
					// weight -= ally_ship.get_final_damage(this_ship);
					weight -= ally_ship.distance_to(enemy_ship);
			}

			if (!target_found || target_weight < weight) {
				target_weight = weight;
				target = enemy_ship;
				target_found = true;
			}
		}
	}

	int avoid_threshold = 0;

	if (target_found) {
		// Fire at target if theres a chance to damage.
		if (this_ship.get_final_damage(target) > 0) {
			fire(target.get_x(), target.get_y());
		}

		// Move towards target if either:
		// They are a battleship AND ...
		if ((target.get_type() == SHIP_TYPE_BATTLESHIP &&
				// ... we are not a frigate OR ...
				this_ship.get_type() != SHIP_TYPE_FRIGATE) ||
				// ... we are in a safe area AND ...
				(target.get_final_range() < this_ship.distance_to(target) &&
				// ... their range is less than ours.
				target.get_final_range() < this_ship.get_final_range())) {
			move_x = this_ship.get_x() < target.get_x() ? 2 : -2;
			move_y = this_ship.get_y() < target.get_y() ? 2 : -2;
			avoid_threshold += this_ship.get_final_damage(target);
		}

		// Otherwise move away.
		else {
			move_x = this_ship.get_x() > target.get_x() ? 2 : -2;
			move_y = this_ship.get_y() > target.get_y() ? 2 : -2;
		}
	}

	if (avoid_total_weight > avoid_threshold) {
		move_x = this_ship.get_x() > avoid_x ? 2 : -2;
		move_y = this_ship.get_y() > avoid_y ? 2 : -2;
	}

	// Always move towards ally center when we are not nearby all allies.
	if (nearby_allies.size() < active_ally_count) {
		move_x = this_ship.get_x() < center_x ? 2 : -2;
		move_y = this_ship.get_y() < center_y ? 2 : -2;
	}

	move(move_x, move_y);

	if (highest_flag_weight > 0) {
		flag(new_flag);
	}
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
