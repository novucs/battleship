#pragma once

#include "bot.hpp"
#include "network_manager.hpp"
#include "ship.hpp"

class bot_master : public bot {
	private:
		network_manager net;
		connection master = create_connection(master_port);
		std::vector<connection> zombies;
		ship me;
		std::vector<ship> ships;
	public:
		void run();
		void setup();
		void zombie_loop(int id);
		void server_loop();
		void perform_tactics();
};
