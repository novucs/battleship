#include <iostream>
#include <winsock2.h>
#include "connection.hpp"

connection* inner_create_connection(u_long host, u_short port);

connection::connection(SOCKADDR_IN address) {
	this->address = address;
}

int connection::get_port() {
	return port;
}

SOCKET connection::get_socket() {
	return m_socket;
}

SOCKADDR_IN connection::get_address() {
	return address;
}

bool connection::create_socket() {
	if (m_socket != 0) {
		std::cout << "Socket creation failed: Connection already has a socket!" << std::endl;
		return false;
	}

	SOCKET created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (!created) {
		std::cout << "Socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}

	m_socket = created;

	return true;
}

void connection::close_socket() {
	closesocket(m_socket);
}

bool connection::attach() {
	bool failed = (bool) bind(m_socket, (SOCKADDR *) &address, sizeof(SOCKADDR));

	if (failed) {
		std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

connection create_connection(u_short port) {
	port = htons(port);
	connection* created = inner_create_connection(INADDR_ANY, port);
	return *created;
}

connection create_connection(std::string hostname, u_short port) {
	u_long host = inet_addr(hostname.c_str());
	port = htons(port);
	connection* created = inner_create_connection(host, port);
	return *created;
}

connection* inner_create_connection(u_long host, u_short port) {
	SOCKADDR_IN address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = host;
	address.sin_port = port;
	return new connection(address);
}
