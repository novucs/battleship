#include <stdio.h>
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

ship* network_manager::load_ship(char* message) {
	int x;
	int y;
	int health;
	int flag;
	int type = 0;

	sscanf(message, "%d,%d,%d,%d,%d", &x, &y, &health, &flag, &type);

	return new ship(x, y, health, flag, type);
}

std::vector<ship*> network_manager::load_ships(char* message) {
	std::vector<ship*> ships;

	char buffer[512];
	int i = 0;
	int j = 0;
	int length = strlen(message) + 1;
	bool finished = false;

	while (!finished && i < length) {
		char chr = message[i];

		switch (chr) {
			case '\0':
				finished = true;
			// ... flow through ...
			case '|':
				ships.push_back(load_ship(buffer));
				buffer[j] = '\0';
				j = 0;
				break;

			default:
				buffer[j] = chr;
				j++;
				break;
		}

		i++;
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

void network_manager::fire_at_ship(int x, int y) {
	char buffer[128];
	sprintf(buffer, "Fire %s,%d,%d", STUDENT_NUMBER, x, y);
	send(server, buffer);
}

void network_manager::move_in_direction(int x, int y) {
	char buffer[128];
	sprintf(buffer, "Move %s,%d,%d", STUDENT_NUMBER, x, y);
	send(server, buffer);
}

void network_manager::set_flag(int flag) {
	char buffer[128];
	sprintf(buffer, "Flag %s,%d", STUDENT_NUMBER, flag);
	send(server, buffer);
}

void network_manager::respawn(int ship_type) {
	char buffer[128];
	sprintf(buffer, "Register  %s,%s,%s,%d", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME, ship_type);
	send(server, buffer);
}

SOCKET create_socket() {
	SOCKET created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (!created) {
		printf("Socket creation failed!\n");
		exit(0);
	}

	return created;
}

void bind(SOCKET socket, SOCKADDR_IN address) {
	bool failed = bind(socket, (SOCKADDR *) &address, sizeof(SOCKADDR));

	if (failed) {
		printf("Bind failed! %d\n", WSAGetLastError());
		exit(0);
	}
}
