#pragma once

#include <string>
#include <ws2tcpip.h>

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
};

connection create_connection(u_short port);
connection create_connection(std::string hostname, u_short port);
