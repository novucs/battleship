#include <iostream>
#include <sstream>
#include "network_manager.hpp"

connection network_manager::get_server() {
	return server;
}

connection network_manager::get_client() {
	return client;
}

bool network_manager::setup() {
	return setup_windows() &&
		server.create_socket() &&
		client.create_socket() &&
		client.attach();
}

bool network_manager::setup_windows() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void network_manager::close() {
	server.close_socket();
	client.close_socket();
	WSACleanup();
}

std::vector<ship> network_manager::read_ships(char* message) {
	std::vector<ship> ships;
	std::stringstream stream(message);

	int x;
	int y;
	int health;
	int flag;
	int type = 0;
	char separator;

	while (!stream.eof()) {
		stream >> x >> separator;
		stream >> y >> separator;
		stream >> health >> separator;
		stream >> flag >> separator;

		if (separator != '|') {
			stream >> type >> separator;
		} else {
			type = bot_class;
		}

		ships.push_back(ship(x, y, health, flag, type));
	}

	return ships;
}

void network_manager::send(connection connection, char* message) {
	SOCKADDR_IN address = connection.get_address();
	sendto(connection.get_socket(), message, strlen(message), 0, (SOCKADDR *) &address, sizeof(SOCKADDR));
}

int network_manager::receive(connection from, connection to, char* buffer, int size) {
	int len = sizeof(SOCKADDR);
	SOCKADDR_IN address = to.get_address();

	if (recvfrom(to.get_socket(), buffer, size - 1, 0, (SOCKADDR *) &address, &len) == SOCKET_ERROR) {
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

void network_manager::send_fire(int x, int y) {
	std::stringstream message;
	message << "Fire " << student_number << ',' << x << ',' << y;
	send(server, strdup(message.str().c_str()));
}

void network_manager::send_move(int x, int y) {
	std::stringstream message;
	message << "Move " << student_number << ',' << x << ',' << y;
	send(server, strdup(message.str().c_str()));
}

void network_manager::send_flag(int flag) {
	std::stringstream message;
	message << "Flag " << student_number << ',' << flag;
	send(server, strdup(message.str().c_str()));
}

void network_manager::respawn(int ship_type) {
	std::stringstream message;
	message << "Register  " << student_number << ',' << student_firstname << ',' << student_familyname << ',' << ship_type;
	send(server, strdup(message.str().c_str()));
}
