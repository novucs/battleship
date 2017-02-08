#pragma once

#include <thread>
#include "bot.hpp"
#include "connection.hpp"
#include "main.hpp"

class bot_zombie : public bot {
	private:
		std::thread relay_server_thread;
		std::thread relay_master_thread;
		connection server_connection = create_connection(server_ip, server_port);
		connection client_connection = create_connection(client_port);
		connection master_connection = create_connection(master.get_ip(), master_port);
		connection zombie_connection = create_connection(zombie_port);

	public:
		void run();
		bool setup();
		void relay_server();
		void relay_master();
		void close();
};
