#pragma once

#define RETREIVE_SUCCESS 0
#define RETREIVE_FAIL 1
#define RETREIVE_IGNORE 2

#include <string>
#include <ws2tcpip.h>
#include "student.hpp"

class connection {
	private:
		int port;
		SOCKET m_socket {0};
		SOCKADDR_IN address;

	public:
		connection(SOCKADDR_IN address);
		int get_port();
		SOCKET get_socket();
		SOCKADDR_IN get_address();
		bool create_socket();
		void close_socket();
		bool attach();
		void send(char* message);
		int receive(connection from, char* buffer, int size);
		void send_fire(student sender, int x, int y);
		void send_move(student sender, int x, int y);
		void send_flag(student sender, int flag);
		void send_respawn(student sender, int ship_type);
};

connection create_connection(u_short port);
connection create_connection(std::string hostname, u_short port);
