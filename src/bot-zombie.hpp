#pragma once

#include <thread>
#include "bot.hpp"
#include "connection.hpp"
#include "main.hpp"

class bot_zombie : public bot {
	private:
		std::thread relay_server_thread;
		std::thread relay_master_thread;
		connection server = create_connection(server_ip, server_port);
		connection client = create_connection(client_port);
		connection master = create_connection(master_ip, master_port);
		connection zombie = create_connection(zombie_port);

	public:
		void run();
		bool setup();
		void relay_server();
		void relay_master();
		void close();
};
