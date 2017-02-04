#pragma once

#include <thread>
#include "bot.hpp"
#include "network_manager.hpp"

class bot_zombie : public bot {
	private:
		network_manager net;
		std::thread relay_server_thread;
		std::thread relay_master_thread;
		connection master = create_connection(master_ip, master_port);
		connection zombie = create_connection(zombie_port);
	public:
		void run();
		bool setup();
		void relay_server();
		void relay_master();
		void close();
};
