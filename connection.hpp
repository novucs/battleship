#pragma once

#define RETREIVE_SUCCESS 0
#define RETREIVE_FAIL 1
#define RETREIVE_IGNORE 2

#include <string>
#include <vector>
#include <ws2tcpip.h>
#include "ship.hpp"

#define RETREIVE_SUCCESS 0
#define RETREIVE_FAIL 1
#define RETREIVE_IGNORE 2


class connection {
	private:
		int port;
		SOCKADDR_IN address;

	public:
		SOCKET m_socket {0};
		connection();
		connection(SOCKADDR_IN address);
		int get_port();
		SOCKET get_socket();
		SOCKADDR_IN get_address();
		bool create_socket();
		void close_socket();
		bool attach();
		void send(char* message);
		int receive(connection from, char* buffer, int size);
		void send_ships(std::vector<ship> ships);
		void send_fire(std::string id, int x, int y);
		void send_move(std::string id, int x, int y);
		void send_flag(std::string id, int flag);
		void send_respawn(std::string id, std::string forename, std::string surname,
			int ship_type);
};

connection create_connection(u_short port);
connection create_connection(std::string hostname, u_short port);
