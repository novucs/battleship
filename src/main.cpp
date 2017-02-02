#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include "ship.hpp"
#include "connection.hpp"

#pragma comment(lib, "wsock32.lib")

#define STUDENT_NUMBER "16002374"
#define STUDENT_FIRSTNAME "Billy"
#define STUDENT_FAMILYNAME "Jenkins"
#define MY_SHIP "2"

connection client = create(1925);
connection server = create("127.0.0.1", 1924);
// connection zombie;
// connection master;

ship* me;
std::vector<ship*> ships;
std::vector<ship*> friends;
std::vector<ship*> enemies;

// 16002374 = will
// 16000587 = josh
// 15019771 = jake
// 16014980 = gareth
std::unordered_set<int> friendIds = {16002374, 16000587, 15019771, 16014980};

void tactics() {
}

void read_ship(char* message) {
	int x;
	int y;
	int health;
	int flag;
	int type = 0;

	sscanf(message, "%d,%d,%d,%d,%d", &x, &y, &health, &flag, &type);

	ship* created = new ship(x, y, health, flag, type);
	ships.push_back(created);
}

void read_ships(char* message) {
	ships.clear();

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
				read_ship(buffer);
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

	me = ships.at(0);
}

void send(connection connection, char* message) {
	SOCKADDR_IN address = connection.get_address();
	sendto(connection.get_socket(), message, strlen(message), 0, (SOCKADDR *) &address, sizeof(SOCKADDR));
}

bool receive(connection connection, char* buffer, int size) {
	int len = sizeof(SOCKADDR);
	SOCKADDR_IN address = connection.get_address();
	return recvfrom(connection.get_socket(), buffer, size - 1, 0, (SOCKADDR *) &address, &len) != SOCKET_ERROR;
}

void run() {
	char buffer[4096];

	sprintf(buffer, "Register	%s,%s,%s,%s", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME, MY_SHIP);
	send(server, buffer);

	for (;;) {
		if (!receive(client, buffer, sizeof(buffer))) {
			printf("Failed to receive data: %d\n", WSAGetLastError());
			return;
		}

		if (buffer[0] == 'M') {
			continue;
		}

		read_ships(buffer);
		tactics();
	}

	printf("Student %s\n", STUDENT_NUMBER);
}

void fire_at_ship(int x, int y) {
	char buffer[512];
	sprintf(buffer, "Fire %s,%d,%d", STUDENT_NUMBER, x, y);
	send(server, buffer);
}

void move_in_direction(int x, int y) {
	char buffer[512];
	sprintf(buffer, "Move %s,%d,%d", STUDENT_NUMBER, x, y);
	send(server, buffer);
}

void set_flag(int flag) {
	char buffer[512];
	sprintf(buffer, "Flag %s,%d", STUDENT_NUMBER, flag);
	send(server, buffer);
}

void setup_windows() {
	WSADATA data;

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		exit(0);
	}
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

int main(int argc, char* argv[]) {
	setup_windows();

	server.set_socket(create_socket());
	client.set_socket(create_socket());

	bind(client.get_socket(), client.get_address());

	run();

	closesocket(server.get_socket());
	closesocket(client.get_socket());

	WSACleanup();

	return 0;
}
