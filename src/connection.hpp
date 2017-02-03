#pragma once

#include <string>
#include <ws2tcpip.h>

class connection {
	private:
		int port;
		SOCKET socket;
		SOCKADDR_IN address;

	public:
		connection(SOCKADDR_IN address);
		int get_port();
		SOCKET get_socket();
		void set_socket(SOCKET socket);
		SOCKADDR_IN get_address();
};

connection create_connection(u_short port);
connection create_connection(std::string hostname, u_short port);
