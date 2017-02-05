#pragma once

#include <thread>
#include <vector>
#include "bot.hpp"
#include "connection.hpp"
#include "main.hpp"
#include "ship.hpp"
#include "student.hpp"

class bot_master : public bot {
	private:
		connection server = create_connection(server_ip, server_port);
		connection client = create_connection(client_port);
		connection master = create_connection(master_port);
		std::vector<connection> zombies;
		std::thread server_thread;
		std::vector<std::thread> zombie_threads;
		ship me;
		std::vector<ship> ships;

	public:
		void run();
		bool setup();
		void zombie_loop(student ally, connection zombie);
		void server_loop();
		std::vector<ship> read_ships(char* message);
		void perform_tactics();
		void close();
};
