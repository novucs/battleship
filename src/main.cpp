#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <unordered_set>

#pragma comment(lib, "wsock32.lib")

#define STUDENT_NUMBER "16002374"
#define STUDENT_FIRSTNAME "Billy"
#define STUDENT_FAMILYNAME "Jenkins"
#define MY_SHIP "2"

class Connection {
  public:
    int port;
    SOCKET socket;
    SOCKADDR_IN address;

    Connection() {
    }

    Connection(u_short port) {
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY;
      address.sin_port = htons(port);
    }

    Connection(u_short port, char* hostname) {
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = inet_addr(hostname);
      address.sin_port = htons(port);
    }
};

Connection client(1925);
Connection server(1924, "127.0.0.1");
Connection zombie;
Connection master;

class Ship {
	public:
		int x;
		int y;
		int health;
		int flag;
		int type;
		int distance;

		bool operator== (Ship &other) {
			if (this == &other) return true;
			return x == other.x &&
				y == other.y &&
				health == other.health &&
				flag == other.flag &&
				type == other.type;
		}
};

Ship me;
std::vector<Ship> ships;
std::vector<Ship> friends;
std::vector<Ship> enemies;

// 16002374 = will
// 16000587 = josh
// 15019771 = jake
// 16014980 = gareth
std::unordered_set<int> friendIds = {16002374, 16000587, 15019771, 16014980};

void tactics() {
}

void read_ship(char* message) {
	Ship ship;
	sscanf(message, "%d,%d,%d,%d,%d",
		&(ship.x), &(ship.y), &(ship.health), &(ship.flag), &(ship.type));
	ships.push_back(ship);
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

void send(Connection connection, char* message) {
	sendto(connection.socket, message, strlen(message), 0, (SOCKADDR *) &connection.address, sizeof(SOCKADDR));
}

bool receive(Connection connection, char* buffer, int size) {
	int len = sizeof(SOCKADDR);
	return recvfrom(connection.socket, buffer, size - 1, 0, (SOCKADDR *) &connection.address, &len) != SOCKET_ERROR;
}

void run() {
	char buffer[4096];

	sprintf(buffer, "Register  %s,%s,%s,%s", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME, MY_SHIP);
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

void setup_socket(SOCKET *sock) {
  *sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (!socket) {
    printf("Socket creation failed!\n");
    exit(0);
  }
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

  setup_socket(&server.socket);
  setup_socket(&client.socket);

  bind(client.socket, client.address);

	run();

	closesocket(server.socket);
	closesocket(client.socket);

	WSACleanup();

	return 0;
}
