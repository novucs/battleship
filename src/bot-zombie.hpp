#pragma once

#include "bot.hpp"
#include "network_manager.hpp"

class bot_zombie : public bot {
	private:
		network_manager net;
		connection master = create_connection(MASTER_PORT);
		connection zombie = create_connection(MASTER_IP, ZOMBIE_PORT);
	public:
		void run();
		void setup();
		void relay_server();
		void relay_master();
		void close();
};
