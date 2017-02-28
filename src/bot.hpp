#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include "bot.hpp"
#include "connection.hpp"
#include "main.hpp"
#include "ship.hpp"
#include "student.hpp"

class bot {
	private:
		connection server_connection = create_connection(server_ip, server_port);
		connection client_connection = create_connection(client_port);
		connection hive_mind_connection = create_connection(hive_mind_port);
		std::thread server_thread;
		std::vector<std::thread> hive_mind_threads;
		ship this_ship;
		std::vector<ship> enemy_ships;
		std::mutex loaded_ships_mutex;
		std::vector<std::vector<ship>> loaded_ships;

	public:
		void run();
		bool setup();
		void hive_mind_loop(int id);
		void server_loop();
		bool merge_ships();
		void print_debug();
		bool is_ally(ship& to_check);
		bool is_enemy(ship& to_check);
		void fire(int x, int y);
		void move(int x, int y);
		void flag(int flag);
		void respawn();
		void perform_tactics();
		void close();
};
