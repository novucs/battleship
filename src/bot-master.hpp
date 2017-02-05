#pragma once

#include <thread>
#include "bot.hpp"
#include "network_manager.hpp"
#include "ship.hpp"
#include "student.hpp"

class bot_master : public bot {
	private:
		network_manager net;
		connection master = create_connection(master_port);
		std::thread server_thread;
		std::vector<std::thread> zombie_threads;
		std::vector<connection> zombies;
		ship me;
		std::vector<ship> ships;

	public:
		void run();
		bool setup();
		void zombie_loop(student ally, connection zombie);
		void server_loop();
		void perform_tactics();
		void close();
};
