#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include "bot.hpp"
#include "connection.hpp"
#include "main.hpp"
#include "ship.hpp"
#include "student.hpp"

class bot_master : public bot {
	private:
		connection server_connection = create_connection(server_ip, server_port);
		connection client_connection = create_connection(client_port);
		connection master_connection = create_connection(master_port);
		std::thread server_thread;
		std::vector<std::thread> zombie_threads;
		ship master_ship;
		std::vector<ship> enemy_ships;
		std::mutex loaded_ships_mutex;
		std::vector<std::vector<ship>> loaded_ships;

	public:
		void run();
		bool setup();
		void zombie_loop(int id, student zombie, connection zombie_connection);
		void server_loop();
		bool merge_ships();
		bool is_zombie(ship& to_check);
		bool is_enemy(ship& to_check);
		void perform_tactics();
		void close();
};
