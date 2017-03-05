/*
 * bot.cpp
 *
 * Implements all classes and methods defined in bot.hpp.
 */

#include <chrono>
#include <iostream>
#include <climits>
#include <sstream>
#include <unordered_map>
#include "bot.hpp"
#include "main.hpp"
#include "protocol_utils.hpp"
#include "tick_packet.hpp"

/**
 * Performs the tactics code for this tick.
 */
void bot::tactics() {
	// Calculate:
	// - number of connected allies
	// - nearby allies
	// - the overall center between all allies
	std::size_t active_ally_count = 0;
	std::vector<ship> nearby_allies;
	int center_x = this_ship.get_x();
	int center_y = this_ship.get_y();
	bool slow_group = false;
	int highest_ally_score = INT_MIN;
	int lowest_ally_score = INT_MAX;

	for (student& ally : allies) {
		if (!ally.is_connected()) {
			continue;
		}

		if (ally.get_score() > highest_ally_score) {
			highest_ally_score = ally.get_score();
		}

		if (ally.get_score() < lowest_ally_score) {
			lowest_ally_score = ally.get_score();
		}

		ship ally_ship = ally.get_ship();
		center_x += ally_ship.get_x();
		center_y += ally_ship.get_y();
		active_ally_count++;

		if (ally_ship.distance_to(this_ship) < ((offset * 2) + 2)) {
			if (ally_ship.get_max_speed() == 1) {
				slow_group = true;
			}

			nearby_allies.push_back(ally_ship);
		}
	}

	// Calculate speed to accomodate for all ships.
	int speed = 2;

	if (slow_group && this_ship.get_max_speed() == 2) {
		speed = 1;
	}

	// Apply default movement: towards map center.
	int move_x = this_ship.get_x() < (500 + identity.get_offset_x()) ? speed : -speed;
	int move_y = this_ship.get_y() < (500 + identity.get_offset_y()) ? speed : -speed;

	int group_size = active_ally_count + 1;
	center_x /= group_size;
	center_y /= group_size;
	center_x += identity.get_offset_x();
	center_y += identity.get_offset_y();

	// Equip the most beneficial flag.
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
		if (ally_ship.is_crippled()) {
			target = ally_ship;
			target_found = true;
		}
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

	if (target_found) {
		// Fire at target if theres a chance to damage.
		if (this_ship.get_final_damage(target) > 0) {
			fire(target.get_x(), target.get_y());

			// Increment our score.
			int score = identity.get_score() + this_ship.get_final_damage(target);
			identity.set_score(score);
		}

		move_x = this_ship.get_x() < target.get_x() ? speed : -speed;
		move_y = this_ship.get_y() < target.get_y() ? speed : -speed;
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

	if (active_ally_count > 0) {
		switch (ship_type) {
			case SHIP_TYPE_BATTLESHIP:
				if (identity.get_score() - 50 > highest_ally_score) {
					ship_type = SHIP_TYPE_FRIGATE;
					respawn();
				}
				break;
			case SHIP_TYPE_FRIGATE:
				if (identity.get_score() + 50 < lowest_ally_score) {
					ship_type = SHIP_TYPE_BATTLESHIP;
					respawn();
				}
				break;
		}
	}
}

/**
 * Sends a fire packet to the server.
 *
 * @param x The X coordinate to fire at.
 * @param y The Y coordinate to fire at.
 */
void bot::fire(int x, int y) {
	identity.get_connection().send_fire(identity.get_id(), x, y);
}

/**
 * Sends a movement packet to the server.
 *
 * @param x The speed X to move.
 * @param y The speed Y to move.
 */
void bot::move(int x, int y) {
	identity.get_connection().send_move(identity.get_id(), x, y);
}

/**
 * Sends a flag change packet to the server.
 *
 * @param flag The new flag.
 */
void bot::flag(int flag) {
	identity.get_connection().send_flag(identity.get_id(), flag);
}

/**
 * Sends a respawn packet to the server, using the gloabal ship_type
 * variable (defined in main.hpp) as the new ship type.
 */
void bot::respawn() {
	std::string id = identity.get_id();
	std::string forename = identity.get_forename();
	std::string surname = identity.get_surname();
	identity.get_connection().send_respawn(id, forename, surname, ship_type);
}

/**
 * Starts the hive-mind loop, loading data sent from the ally.
 *
 * @param id The position this ally is stored in the global allies list.
 */
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

		tick_packet packet = read_tick_packet(buffer);
		std::vector<ship> ships = packet.get_ships();
		loaded_ships.at(ally.get_load_order()) = ships;
		ally.set_score(packet.get_score());

		if (!ships.empty()) {
			ally.set_connected(true);
			ally.set_ship(std::move(ships.at(0)));
		}

		loaded_ships_mutex.unlock();
	}
}

/**
 * Starts the main server loop. Used for retreiving data from the server,
 * relaying that data to allies and then performing tactics code.
 */
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
		std::vector<ship> ships = read_ships(false, buffer);
		loaded_ships.at(identity.get_load_order()) = ships;

		this_ship = ships.at(0);
		identity.set_ship(std::move(this_ship));
		int score = identity.get_score();

		if (last_tick_health < this_ship.get_health()) {
			score -= (last_tick_health + (this_ship.get_health() - 10));
		} else if (last_tick_health > this_ship.get_health()) {
			score -= (last_tick_health - this_ship.get_health());
		}

		identity.set_score(score);

		tick_packet packet(score, ships);

		for (student& ally : allies) {
			ally.get_connection().send_tick_packet(packet);
		}

		loaded_ships_mutex.unlock();

		// Ghetto sleep to receive zombie data.
		std::this_thread::sleep_for(std::chrono::milliseconds(25));

		loaded_ships_mutex.lock();

		if (!merge_ships()) {
			continue;
		}

		if (debug) {
			print_debug();
		}

		tactics();

		// Flag allies as not connected for next tick, allowing them to correct this.
		for (student& ally : allies) {
			ally.set_connected(false);
		}

		loaded_ships_mutex.unlock();
	}
}

/**
 * Merges and sorts all loaded ships from hive-mind threads into enemies
 * and allies.
 */
bool bot::merge_ships() {
	enemy_ships.clear();

	if (loaded_ships.empty() || loaded_ships.at(identity.get_load_order()).empty()) {
		return false;
	}

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

/**
 * Prints the debug message for this tick.
 */
void bot::print_debug() {
	std::cout << "allies:";

	for (std::size_t i = 0; i < allies.size(); i++) {
		student& ally = allies.at(i);
		ship s = ally.get_ship();

		if (!ally.is_connected()) {
			continue;
		}

		std::cout << i << '-';
		std::cout << s.get_x() << ',';
		std::cout << s.get_y() << ',';
		std::cout << s.get_health() << ',';
		std::cout << s.get_flag() << ',';
		std::cout << s.get_type() << '|';
	}

	std::cout << "enemies:";

	for (ship& s : enemy_ships) {
		std::cout << s.get_x() << ',';
		std::cout << s.get_y() << ',';
		std::cout << s.get_health() << ',';
		std::cout << s.get_flag() << ',';
		std::cout << s.get_type() << '|';
	}

	std::cout << std::endl;
}

/**
 * Returns if a ship is considered an ally.
 *
 * @param to_check The ship to check.
 * @return {@code true} if the ship is an ally, otherwise {@code false}.
 */
bool bot::is_ally(ship& to_check) {
	for (student& ally : allies) {
		ship ship = ally.get_ship();
		if (ship == to_check) {
			return true;
		}
	}
	return false;
}

/**
 * Returns if the ship is considered an enemy.
 *
 * @param to_check The ship to check.
 * @return {@code true} if the ship is an enemy, otherwise {@code false}.
 */
bool bot::is_enemy(ship& to_check) {
	for (ship& enemy : enemy_ships) {
		if (enemy == to_check) {
			return true;
		}
	}
	return false;
}

/**
 * Initializes the bot state and connections.
 */
bool bot::setup() {
	// Create server socket.
	return server_connection.create_socket() &&

		// Create and attach the client socket.
		client_connection.create_socket() &&
		client_connection.attach() &&

		// Create and attach the hive mind socket.
		hive_mind_connection.create_socket() &&
		hive_mind_connection.attach();
}

/**
 * Begins all the bot threads and awaits user input.
 */
void bot::run() {
	// Print the startup messages.
	std::cout << std::endl << "==========================" << std::endl;
	std::cout << std::endl << "   Hive mind bot loaded   " << std::endl;
	std::cout << std::endl << "==========================" << std::endl;
	std::cout << std::endl << "Enter commands here, type '/help' for help." << std::endl;

	// Prepare loaded_ships vector for ships to be loaded in.
	loaded_ships.resize(allies.size() + 1);

	// Set our identities connection to the server connection.
	identity.set_connection(std::move(server_connection));

	// Load and run each ally into their own hive-mind relay thread.
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

	// Create and run the server thread.
	server_thread = std::thread(&bot::server_loop, this);

	// Respawn our bot.
	respawn();

	// Wait for user input.
	getchar();
}

/**
 * Closes opened sockets and discontinues running connection threads.
 */
void bot::close() {
	// Close server, client and hive-mind sockets.
	server_connection.close_socket();
	client_connection.close_socket();
	hive_mind_connection.close_socket();

	for (student& ally : allies) {
		ally.get_connection().close_socket();
	}

	// Cleanup Windows socket API.
	WSACleanup();

	// Join all connection threads as they shutdown.
	server_thread.join();

	for (std::thread& hive_mind_thread : hive_mind_threads) {
		hive_mind_thread.join();
	}
}
