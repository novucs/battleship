#include <iostream>
#include <sstream>
#include "network_manager.hpp"

network_manager::network_manager() {
}

connection network_manager::get_server() {
	return server;
}

connection network_manager::get_client() {
	return client;
}

void setup_windows() {
	WSADATA data;

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		exit(0);
	}
}

void network_manager::setup() {
	setup_windows();

	server.set_socket(create_socket());
	client.set_socket(create_socket());

	bind(client.get_socket(), client.get_address());
}

void network_manager::close() {
	closesocket(server.get_socket());
	closesocket(client.get_socket());
	WSACleanup();
}

std::vector<ship*> network_manager::read_ships(char* message) {
	std::vector<ship*> ships;
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
			type = BOT_CLASS;
		}

		ships.push_back(new ship(x, y, health, flag, type));
	}

	return ships;
}

void network_manager::send(connection connection, char* message) {
	SOCKADDR_IN address = connection.get_address();
	sendto(connection.get_socket(), message, strlen(message), 0, (SOCKADDR *) &address, sizeof(SOCKADDR));
}

bool network_manager::receive(connection connection, char* buffer, int size) {
	int len = sizeof(SOCKADDR);
	SOCKADDR_IN address = connection.get_address();
	return recvfrom(connection.get_socket(), buffer, size - 1, 0, (SOCKADDR *) &address, &len) != SOCKET_ERROR;
}

void network_manager::send_fire(int x, int y) {
	std::stringstream message("Fire ");
	message << STUDENT_NUMBER << ',' << x << ',' << y;
	send(server, strdup(message.str().c_str()));
}

void network_manager::send_move(int x, int y) {
	std::stringstream message("Move ");
	message << STUDENT_NUMBER << ',' << x << ',' << y;
	send(server, strdup(message.str().c_str()));
}

void network_manager::send_flag(int flag) {
	std::stringstream message("Flag ");
	message << STUDENT_NUMBER << ',' << flag;
	send(server, strdup(message.str().c_str()));
}

void network_manager::respawn(int ship_type) {
	std::ostringstream message;
	message << "Register  " << STUDENT_NUMBER << ',' << STUDENT_FIRSTNAME << ',' << STUDENT_FAMILYNAME << ',' << ship_type;
	send(server, strdup(message.str().c_str()));
}

SOCKET create_socket() {
	SOCKET created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (!created) {
		std::cout << "Socket creation failed!" << std::endl;
		exit(0);
	}

	return created;
}

void bind(SOCKET socket, SOCKADDR_IN address) {
	bool failed = (bool) bind(socket, (SOCKADDR *) &address, sizeof(SOCKADDR));

	if (failed) {
		std::cout << "Bind failed!" << WSAGetLastError() << std::endl;
		exit(0);
	}
}
