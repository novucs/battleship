#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <unordered_set>

#pragma comment(lib, "wsock32.lib")

#define SHIPTYPE_BATTLESHIP "0"
#define SHIPTYPE_FRIGATE "1"
#define SHIPTYPE_SUBMARINE "2"

#define STUDENT_NUMBER "16002374"
#define STUDENT_FIRSTNAME "Billy"
#define STUDENT_FAMILYNAME "Jenkins"
#define MY_SHIP SHIPTYPE_SUBMARINE

#define IP_ADDRESS_SERVER "127.0.0.1"
// #define IP_ADDRESS_SERVER "164.11.80.69"

#define PORT_SEND 1924
#define PORT_RECEIVE 1925

#define MAX_BUFFER_SIZE 500
#define MAX_SHIPS 200

#define FIRING_RANGE 100

#define MOVE_LEFT -1
#define MOVE_RIGHT 1
#define MOVE_UP 1
#define MOVE_DOWN -1
#define MOVE_FAST 2
#define MOVE_SLOW 1

SOCKADDR_IN sendto_addr;
SOCKADDR_IN receive_addr;

SOCKET sock_send;
SOCKET sock_recv;

WSADATA data;

char InputBuffer[MAX_BUFFER_SIZE];

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

bool message = false;
char MsgBuffer[MAX_BUFFER_SIZE];

bool fire = false;
int fireX;
int fireY;

bool moveShip = false;
int moveX;
int moveY;

bool setFlag = true;
int new_flag = 0;

void send_message(char* dest, char* source, char* msg);
void fire_at_ship(int X, int Y);
void move_in_direction(int left_right, int up_down);
void set_new_flag(int newFlag);

/*************************************************************/
/********* Your tactics code starts here *********************/
/*************************************************************/
std::string key = "JKXCFxsR7pG4CovkfhwBfWHFFqxbxhUaia1jwHsO \
	B72hImzDHw2J8IakZCc6BHzyhdvkxDxBpjr60Jar \
	3LqVOJN8tXCHYJSB6j9ToT8cdq7McjDTnohmiOSv \
	9Q9qLa8lVPkTnYMF1NdUuYDozObbDKtX1qoyKzez \
	n8aCLLbRy8kKGimBF7co9HKcpfdbVSTePAxDAZ6P \
	1xwE3UbBYf8z5DJY7wevXjDAKqcPaLCkvG5INHwQ \
	3naPbov5fNgrHkWHxjU4V1EkYIWiseQMTU1XrgEW \
	vd7Sd317oVRQ8XbDzOPieXYLZbsw1p4gO7xQzpqj \
	1myMpMa3gDCStUKKF1ihwQQOFRT4srnMIbEi9O5R \
	Ok2UKZ1MtmsTvrOi6kPTDIVcjBIbQyxxarTQoXEn \
	jyNmjDe81dp4czzRjA1A5FkkN9dvX7gYYB6T7coB \
	jdTx6X4gJmD4WtjjafGY1gnZmBlLsDIL54t3op4S \
	6utazo8brp48AuHrEfzvjdg4VIi2DT5x4YsE2XLb \
	Ktw9QVWVGE6SZR9n8LjDJLXrqvpI6OPc6JkDDbDl \
	kHBpj8KVYgJ27p6z1bgjnNbplJLLbhigQFf7uFkH \
	b1U58J3Gcznv2jFuSKZb5dajVVrdIgYzkyJ18hbE \
	ip8n1vvyi8QmHp5RJnGRZPFSmFR9fwqhuhjc2JqH \
	zaQYvcNAGa3MDMvT9uWgCv3tCNFku4A5BQVe9EiM \
	SJDl9fmQcB3aO5rZNyzK5cOmNj1gGJNxJSKcRFII \
	GBxQZOvmQ5ZRVMcrYXKCmykRQD0ZOFQjqA2LO5Ym \
	bSyebN4AUexy2mYMxi1S1PV1IvAZd9ErMELFM2Yl \
	6qMCJ65SqEomDlcykAUatLtTqSxcNwh2KRX1TXdI \
	1UoaV5eF3QcFL3mh94IAUJkKfHYuR1YgvKqGyYC1 \
	fYqAgtEro6ioiM8jFJwzx8vsAvURQecC1Wj195kW \
	wXSOfinLBGLZi4jm3ytrVn3Nzk23i31d34LcPMFm";

int box_size = 300;
int map_size = 500;

int up_down = MOVE_UP * MOVE_FAST;
int left_right = MOVE_LEFT * MOVE_FAST;

void first_move();
void last_move();
void calculate_distances();
void search_relationships();
void handle_enemies();
void xor_encrypt(char *msg);
bool is_friend(Ship ship);

int avoidX;
int avoidY;
time_t lastAvoidSet = 0;

int targetX;
int targetY;
time_t lastTargetSet = 0;

void tactics() {
	time_t now = time(0);

	// Move ship according to position on the map.
	first_move();

	// Calculate the distance between ships.
	calculate_distances();

	// Seperate friends from enemies.
	search_relationships();

	Ship target;
	bool foundTarget;

	for (Ship enemy : enemies) {
		if (!foundTarget ||
				(enemy.distance < target.distance) &&
				(enemy.type != 1 && target.type == 1) &&
				(enemy.type == 0 && target.type != 0) &&
				(enemy.health < target.health)) {
			target = enemy;
			foundTarget = true;
			targetX = target.x;
			targetY = target.y;
			lastTargetSet = 0;
		}
	}

	// Persue the enemy.
	if (foundTarget) {
		left_right = (target.x > me.x ? MOVE_RIGHT : MOVE_LEFT) * MOVE_FAST;
		up_down = (target.y > me.y ? MOVE_UP : MOVE_DOWN) * MOVE_FAST;

		// Shoot enemy.
		fire_at_ship(target.x, target.y);

		// Use the target ship flag.
		set_new_flag(target.flag);
	} else if (now < lastTargetSet - 3) {
		left_right = (targetX > me.x ? MOVE_RIGHT : MOVE_LEFT) * MOVE_FAST;
		up_down = (targetY > me.y ? MOVE_UP : MOVE_DOWN) * MOVE_FAST;
	}

	bool packFound = false;

	for (Ship enemy1 : enemies) {

		int enemiesInRange = 0;
		int totalX = enemy1.x;
		int totalY = enemy1.y;

		for (Ship enemy2 : enemies) {

			if (enemy1 == enemy2) {
				continue;
			}

			int x = enemy1.x - enemy2.x;
			int y = enemy1.y - enemy2.y;
			int distance = (int) sqrt(x*x + y*y);

			if (distance < 15) {
				enemiesInRange++;
				totalX += enemy2.x;
				totalY += enemy2.y;
			}
		}

		if (enemiesInRange > 1) {
			set_new_flag(enemy1.flag);

			int meanX = totalX / enemiesInRange;
			int meanY = totalY / enemiesInRange;

			if (packFound) {
				avoidX += meanX;
				avoidX /= 2;

				avoidY += meanY;
				avoidY /= 2;
			} else {
				avoidX = meanX;
				avoidY = meanY;
				lastAvoidSet = now;
			}
		}
	}

	for (Ship enemy : enemies) {
		if (enemy.type == 1 ||
				(enemy.type == 2 && (enemy.health - 1) > me.health) ||
				(enemy.type == 1 && enemy.distance <= 10)) {
			if (now < lastAvoidSet + 4) {
				avoidX += enemy.x;
				avoidX /= 2;

				avoidY += enemy.y;
				avoidY /= 2;
			} else {
				avoidX = enemy.x;
				avoidY = enemy.y;
				lastAvoidSet = now;
			}
		}
	}

	// Do all other sorts of super cool stuff i guess maybe? idk

	if (now < lastAvoidSet + 4) {
		left_right = (avoidX > me.x ? MOVE_LEFT : MOVE_RIGHT) * MOVE_FAST;
		up_down = (avoidY > me.y ? MOVE_DOWN : MOVE_UP) * MOVE_FAST;
	}

	//
	// // Handle all found enemies when found.
	// if (!enemies.empty())
	// 	handle_enemies();
	//
	// // Ensure ship does not travel to the map center.
	// last_move();
	//
	// char msg[100];
	// sprintf(msg, "%d %d", me.x + left_right, me.y + up_down);
	// send_message("16002374", "16002374", msg);  // send my co-ordinates to myself
	//
	move_in_direction(left_right, up_down);
}

void first_move() {
	if (me.y > 950) {
		up_down = MOVE_DOWN * MOVE_FAST;
	}

	if (me.y < 50) {
		up_down = MOVE_UP * MOVE_FAST;
	}

	if (me.x < 50) {
		left_right = MOVE_RIGHT * MOVE_FAST;
	}

	if (me.x > 950) {
		left_right = MOVE_LEFT * MOVE_FAST;
	}

	if (me.x < map_size + box_size && me.x > map_size - box_size &&
		me.y < map_size + box_size && me.y > map_size - box_size) {
		if (me.x > map_size) {
			left_right = MOVE_RIGHT * MOVE_FAST;
		} else {
			left_right = MOVE_LEFT * MOVE_FAST;
		}

		if (me.y > map_size) {
			up_down = MOVE_UP * MOVE_FAST;
		} else {
			up_down = MOVE_DOWN * MOVE_FAST;
		}
	}
}

void calculate_distances() {
	int x, y;

	for (Ship ship : ships) {
		x = ship.x - me.x;
		y = ship.y - me.y;
		ship.distance = (int) sqrt(x*x + y*y);
	}
}

void search_relationships() {
	enemies.clear();
	friends.clear();
	for (Ship ship : ships) {
		if (ship == me)
			continue;

		if (is_friend(ship)) {
			friends.push_back(ship);
		} else {
			enemies.push_back(ship);
		}
	}
}

void handle_enemies() {
	// Locate the closest enemy.
	Ship target;
	bool foundTarget = false;
	int closestDistance = 0;

	for (Ship enemy : enemies) {
		if (!foundTarget || closestDistance > enemy.distance) {
			target = enemy;
			closestDistance = target.distance;
			foundTarget = true;
		}
	}

	// Persue the enemy.
	left_right = (target.x > me.x ? MOVE_RIGHT : MOVE_LEFT) * MOVE_FAST;
	up_down = (target.y > me.y ? MOVE_UP : MOVE_DOWN) * MOVE_FAST;

	// Shoot enemy if within firing range.
	if (closestDistance < 100)
		fire_at_ship(target.x, target.y);

	// Use the target ship flag.
	me.flag = target.flag;
}

void message_received(char* msg) {
	int idFrom;
	int idTo;
	int x;
	int y;

	if (sscanf(msg, "Message %d, %d, %500[^\n]", &idTo, &idFrom, MsgBuffer) != 3) {
		return;
	}

	xor_encrypt(MsgBuffer);

	if (friendIds.count(idFrom)) {
		if (sscanf(MsgBuffer, "%d %d", &x, &y) == 2) {
			printf("Coords %d %d %d %d\n", x, y, me.x, me.y);
		}
	}
}

bool is_friend(Ship ship) {
	// TODO: Implement friend finding algorithm.
	return false;
}

void xor_encrypt(char *msg) {
	for (int i = 0; i < strlen(msg); i++) {
		msg[i] ^= key[i % key.length()];
	}
}

/*************************************************************/
/********* Your tactics code ends here ***********************/
/*************************************************************/

void communicate_with_server() {
	char buffer[4096];
	int  len = sizeof(SOCKADDR);
	char chr;
	bool finished;
	int  i;
	int  j;
	int  rc;
	char* p;

	sprintf(buffer, "Register  %s,%s,%s,%s", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME, MY_SHIP);
	sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));

	while (true) {
		if (!recvfrom(sock_recv, buffer, sizeof(buffer) - 1, 0, (SOCKADDR *)&receive_addr, &len) == SOCKET_ERROR) {
			printf("recvfrom error = %d\n", WSAGetLastError());
			return;
		}

		p = inet_ntoa(receive_addr.sin_addr);

		if ((strcmp(IP_ADDRESS_SERVER, "127.0.0.1") != 0) && (strcmp(IP_ADDRESS_SERVER, p) != 0)) {
			return;
		}

		if (buffer[0] == 'M') {
			message_received(buffer);
		}
		else {
			i = 0;
			j = 0;
			finished = false;
			ships.clear();

			while ((!finished) && (i < 4096)) {
				chr = buffer[i];
				Ship ship;

				switch (chr) {
				case '|':
					InputBuffer[j] = '\0';
					j = 0;
					sscanf(InputBuffer, "%d,%d,%d,%d,%d", &(ship.x), &(ship.y), &(ship.health), &(ship.flag), &(ship.type));
					ships.push_back(ship);
					break;

				case '\0':
					InputBuffer[j] = '\0';
					sscanf(InputBuffer, "%d,%d,%d,%d,%d", &(ship.x), &(ship.y), &(ship.health), &(ship.flag), &(ship.type));
					finished = true;
					ships.push_back(ship);
					break;

				default:
					InputBuffer[j] = chr;
					j++;
					break;
				}

				i++;
			}

			me = ships.at(0);
		}

		tactics();

		if (message) {
			sendto(sock_send, MsgBuffer, strlen(MsgBuffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			message = false;
		}

		if (fire) {
			sprintf(buffer, "Fire %s,%d,%d", STUDENT_NUMBER, fireX, fireY);
			sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			fire = false;
		}

		if (moveShip) {
			sprintf(buffer, "Move %s,%d,%d", STUDENT_NUMBER, moveX, moveY);
			rc = sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			moveShip = false;
		}

		if (setFlag) {
			sprintf(buffer, "Flag %s,%d", STUDENT_NUMBER, new_flag);
			sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			setFlag = false;
		}
	}

	printf("Student %s\n", STUDENT_NUMBER);
}

void send_message(char* dest, char* source, char* msg) {
	message = true;
	xor_encrypt(msg);
	sprintf(MsgBuffer, "Message %s,%s,%s,%s", STUDENT_NUMBER, dest, source, msg);
}

void fire_at_ship(int x, int y) {
	fire = true;
	fireX = x;
	fireY = y;
}

void move_in_direction(int x, int y) {
	if (x < -2) x = -2;
	if (x >  2) x = 2;
	if (y < -2) y = -2;
	if (y >  2) y = 2;

	moveShip = true;
	moveX = x;
	moveY = y;
}

void set_new_flag(int newFlag) {
	setFlag = true;
	new_flag = newFlag;
}

int main(int argc, char* argv[]) {
	char chr = '\0';

	printf("\n");
	printf("Battleship Bots\n");
	printf("UWE Computer and Network Systems Assignment 2 (2016-17)\n");
	printf("\n");

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		return 0;
	}

	// Here we create our sockets, which will be a UDP socket (SOCK_DGRAM).
	sock_send = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (!sock_send) {
		printf("Socket creation failed!\n");
		return 0;
	}

	sock_recv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (!sock_recv) {
		printf("Socket creation failed!\n");
		return 0;
	}

	memset(&sendto_addr, 0, sizeof(SOCKADDR_IN));
	sendto_addr.sin_family = AF_INET;
	sendto_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS_SERVER);
	sendto_addr.sin_port = htons(PORT_SEND);

	memset(&receive_addr, 0, sizeof(SOCKADDR_IN));
	receive_addr.sin_family = AF_INET;
	receive_addr.sin_addr.s_addr = INADDR_ANY;
	receive_addr.sin_port = htons(PORT_RECEIVE);

	if (bind(sock_recv, (SOCKADDR *)&receive_addr, sizeof(SOCKADDR))) {
		printf("Bind failed! %d\n", WSAGetLastError());
		return 0;
	}

	communicate_with_server();

	closesocket(sock_send);
	closesocket(sock_recv);
	WSACleanup();

	while (chr != '\n') {
		chr = getchar();
	}

	return 0;
}
