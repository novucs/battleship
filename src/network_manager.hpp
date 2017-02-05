#pragma once

#define RETREIVE_SUCCESS 0
#define RETREIVE_FAIL 1
#define RETREIVE_IGNORE 2

#include <vector>
#include "connection.hpp"
#include "main.hpp"
#include "ship.hpp"

class network_manager {
	private:
		connection server = create_connection(server_ip, server_port);
		connection client = create_connection(client_port);
		bool setup_windows();

	public:
		connection get_server();
		connection get_client();
		bool setup();
		void close();
		void send(connection connection, char* message);
		int receive(connection from, connection to, char* buffer, int size);
		std::vector<ship> read_ships(char* message);
		void send_fire(int x, int y);
		void send_move(int x, int y);
		void send_flag(int flag);
		void respawn(int ship_type);
};
