#pragma once

#include <vector>
#include "connection.hpp"
#include "settings.hpp"
#include "ship.hpp"

class network_manager {
	private:
		connection server = create_connection(SERVER_PORT);
		connection client = create_connection(SERVER_IP, CLIENT_PORT);
	public:
		network_manager();
		connection get_server();
		connection get_client();
		void setup();
		void close();
		void send(connection connection, char* message);
		bool receive(connection connection, char* buffer, int size);
		std::vector<ship*> read_ships(char* message);
		void send_fire(int x, int y);
		void send_move(int x, int y);
		void send_flag(int flag);
		void respawn(int ship_type);
};

SOCKET create_socket();
void bind(SOCKET socket, SOCKADDR_IN address);
