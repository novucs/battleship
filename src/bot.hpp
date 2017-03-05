/*
 * bot.hpp
 *
 * Defines all hive-mind bot classes and methods.
 */

#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include "bot.hpp"
#include "connection.hpp"
#include "main.hpp"
#include "ship.hpp"
#include "student.hpp"

/**
 * The main bot worker, manages all networking and tactics.
 */
class bot {

	private:
		// Connection to the battleship bot server.
		connection server_connection = create_connection(server_ip, server_port);

		// Connection being locally hosted for this bot.
		connection client_connection = create_connection(client_port);

		// Connection used for communicating with other hive-mind bots.
		connection hive_mind_connection = create_connection(hive_mind_port);

		// The thread used for tick logic and communicating with the server.
		std::thread server_thread;

		// The threads used for synchronizing tick data between ally bots.
		std::vector<std::thread> hive_mind_threads;

		// Our current ship data for this tick.
		ship this_ship;

		// All viewed enemy ships in this tick.
		std::vector<ship> enemy_ships;

		// Mutex used for synchronizing hive-mind and server threads.
		std::mutex loaded_ships_mutex;

		// All loaded ships from fellow hive-mind bots.
		std::vector<std::vector<ship>> loaded_ships;

		// Health of this ship from last tick.
		int last_tick_health = 10;

		/**
		 * Performs the tactics code for this tick.
		 */
		void tactics();

		/**
		 * Sends a fire packet to the server.
		 *
		 * @param x The X coordinate to fire at.
		 * @param y The Y coordinate to fire at.
		 */
		void fire(int x, int y);

		/**
		 * Sends a movement packet to the server.
		 *
		 * @param x The speed X to move.
		 * @param y The speed Y to move.
		 */
		void move(int x, int y);

		/**
		 * Sends a flag change packet to the server.
		 *
		 * @param flag The new flag.
		 */
		void flag(int flag);

		/**
		 * Sends a respawn packet to the server, using the gloabal ship_type
		 * variable (defined in main.hpp) as the new ship type.
		 */
		void respawn();

		/**
		 * Starts the hive-mind loop, loading data sent from the ally.
		 *
		 * @param id The position this ally is stored in the global allies list.
		 */
		void hive_mind_loop(int id);

		/**
		 * Starts the main server loop. Used for retreiving data from the server,
		 * relaying that data to allies and then performing tactics code.
		 */
		void server_loop();

		/**
		 * Merges and sorts all loaded ships from hive-mind threads into enemies
		 * and allies.
		 */
		bool merge_ships();

		/**
		 * Prints the debug message for this tick.
		 */
		void print_debug();

		/**
		 * Returns if a ship is considered an ally.
		 *
		 * @param to_check The ship to check.
		 * @return {@code true} if the ship is an ally, otherwise {@code false}.
		 */
		bool is_ally(ship& to_check);

		/**
		 * Returns if the ship is considered an enemy.
		 *
		 * @param to_check The ship to check.
		 * @return {@code true} if the ship is an enemy, otherwise {@code false}.
		 */
		bool is_enemy(ship& to_check);

	public:
		/**
		 * Initializes the bot state and connections.
		 */
		bool setup();

		/**
		 * Begins all the bot threads and awaits user input.
		 */
		void run();

		/**
		 * Closes opened sockets and discontinues running connection threads.
		 */
		void close();
};
