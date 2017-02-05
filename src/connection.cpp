#include <iostream>
#include <sstream>
#include <winsock2.h>
#include "connection.hpp"
#include "protocol_utils.hpp"

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

void connection::send(char* message) {
	sendto(m_socket, message, strlen(message), 0, (SOCKADDR *) &address, sizeof(SOCKADDR));
}

int connection::receive(connection from, char* buffer, int size) {
	int len = sizeof(SOCKADDR);
	SOCKADDR_IN address = this->get_address();

	if (recvfrom(m_socket, buffer, size - 1, 0, (SOCKADDR *) &address, &len) == SOCKET_ERROR) {
		// Do not print error when interrupted.
		if (WSAGetLastError() == 10004) {
			return RETREIVE_FAIL;
		}

		std::cout << "Failed to receive data: " << WSAGetLastError() << std::endl;
		return RETREIVE_FAIL;
	}

	char* expected_address = inet_ntoa(from.get_address().sin_addr);
	char* actual_address = inet_ntoa(address.sin_addr);

	// Spoof detected if not given the expected address.
	if (strcmp(expected_address, "127.0.0.1") != 0 &&
			strcmp(expected_address, actual_address) != 0) {
		return RETREIVE_IGNORE;
	}

	return RETREIVE_SUCCESS;
}

void connection::send_fire(student sender, int x, int y) {
	char* message = strdup(write_fire(sender, x, y).c_str());
	send(message);
}

void connection::send_move(student sender, int x, int y) {
	char* message = strdup(write_move(sender, x, y).c_str());
	send(message);
}

void connection::send_flag(student sender, int flag) {
	char* message = strdup(write_flag(sender, flag).c_str());
	send(message);
}

void connection::send_respawn(student sender, int ship_type) {
	char* message = strdup(write_respawn(sender, ship_type).c_str());
	send(message);
}

void connection::send_ships(std::vector<ship> ships) {
	char* message = strdup(write_ships(ships).c_str());
	send(message);
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
