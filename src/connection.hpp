#ifndef __CONNECTION_HPP_INCLUDED__
#define __CONNECTION_HPP_INCLUDED__
#include <winsock2.h>

class connection {
	private:
		int port;
		SOCKET socket;
		SOCKADDR_IN address;

	public:
		int get_port();
		SOCKET get_socket();
		void set_socket(SOCKET socket);
		SOCKADDR_IN get_address();
		connection(SOCKADDR_IN address);
};

connection create(u_short port);
connection create(char* hostname, u_short port);

#endif
