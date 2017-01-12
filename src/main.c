#include <stdio.h>
#include <winsock2.h>
#include <math.h>

#pragma comment(lib, "wsock32.lib")

typedef enum { false, true } bool;

#define SHIPTYPE_BATTLESHIP "0"
#define SHIPTYPE_FRIGATE "1"
#define SHIPTYPE_SUBMARINE "2"

#define STUDENT_NUMBER "12345678"
#define STUDENT_FIRSTNAME "Fred"
#define STUDENT_FAMILYNAME "Bloggs"
#define MY_SHIP SHIPTYPE_BATTLESHIP

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

char InputBuffer [MAX_BUFFER_SIZE];

/*
 * Firing range is 100
 * Visible range is 200
 */

struct ship {
	int x;
	int y;
	int health;
	int flag;
	int type;
	int distance;
};

struct ship me;

int number_of_ships; // Number of ships visible to us
struct ship ships[MAX_SHIPS];

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

int up_down = MOVE_UP * MOVE_FAST;
int left_right = MOVE_LEFT * MOVE_FAST;

int number_of_friends;
struct ship friends[MAX_SHIPS];

int number_of_enemies;
struct ship enemies[MAX_SHIPS];

bool is_friend(int index) {
	bool rc = false;

	if (ships[index].flag == 123) {
		rc = true;  // I have just seen my friend 123
	}

	return rc;
}

void tactics() {
	if (me.y > 900) {
		up_down = MOVE_DOWN * MOVE_FAST;
	}

	if (me.x < 200) {
		left_right = MOVE_RIGHT * MOVE_FAST;
	}

	if (me.y < 200) {
		up_down = MOVE_UP * MOVE_FAST;
	}

	if (me.x > 800) {
		left_right = MOVE_LEFT * MOVE_FAST;
	}

	for (int i = 0; i < number_of_ships; i++) {
		struct ship* ship = &ships[i];
		ship->distance = (int) sqrt((double) (ship->x - me.x) * (ship->x - me.x) + (ship->y - me.y) * (ship->y - me.y));
	}

	number_of_friends = 0;
	number_of_enemies = 0;

	if (number_of_ships > 1) {
		for (int i = 1; i < number_of_ships; i++) {
			struct ship ship = ships[i];

			if (is_friend(i)) {
				friends[number_of_friends] = ship;
				number_of_friends++;
			} else {
				enemies[number_of_friends] = ship;
				number_of_enemies++;
			}
		}

		if (number_of_enemies > 0) {
			// Persue enemy if only one is in sight.
			if (number_of_enemies == 1) {
				left_right = (enemies[0].x > me.x ? MOVE_RIGHT : MOVE_LEFT) * MOVE_FAST;
				up_down = (enemies[0].y > me.y ? MOVE_UP : MOVE_DOWN) * MOVE_FAST;
			}

			// When more than one enemy can be seen, flee.
			else {
				left_right = (enemies[0].x > me.x ? MOVE_LEFT : MOVE_RIGHT) * MOVE_FAST;
				up_down = (enemies[0].y > me.y ? MOVE_DOWN : MOVE_UP) * MOVE_FAST;
			}

			fire_at_ship(enemies[0].x, enemies[0].y);
		}
	}

	//char msg[100];
	//sprintf(msg, "Im at %d %d", myX, myY);
	//send_message("12345678", "23456789", msg);  // send my co-ordinates to myself

	move_in_direction(left_right, up_down);
}

void message_received(char* msg) {
	int x;
	int y;

	printf("%s\n", msg);

	if (sscanf_s(msg, "Message 12345678, 23456789, Im at %d %d", &x, &y) == 2) {
		printf("My friend is at %d %d\n", x, y);
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
		if (!recvfrom(sock_recv, buffer, sizeof(buffer) - 1, 0, (SOCKADDR *) &receive_addr, &len) == SOCKET_ERROR) {
			printf("recvfrom error = %d\n", WSAGetLastError());
			return;
		}

		p = inet_ntoa(receive_addr.sin_addr);

		if ((strcmp(IP_ADDRESS_SERVER, "127.0.0.1") != 0) && (strcmp(IP_ADDRESS_SERVER, p) != 0)) {
			return;
		}

		if (buffer[0] == 'M') {
			message_received(buffer);
		} else {
			i = 0;
			j = 0;
			finished = false;
			number_of_ships = 0;

			while ((!finished) && (i < 4096)) {
				chr = buffer[i];
				struct ship* ship = &ships[number_of_ships];

				switch (chr) {
					case '|':
						InputBuffer[j] = '\0';
						j = 0;
						sscanf(InputBuffer, "%d,%d,%d,%d", &(ship->x), &(ship->y), &(ship->health), &(ship->flag), &(ship->type));
						number_of_ships++;
						break;

					case '\0':
						InputBuffer[j] = '\0';
						sscanf(InputBuffer, "%d,%d,%d,%d", &(ship->x), &(ship->y), &(ship->health), &(ship->flag), &(ship->type));
						number_of_ships++;
						finished = true;
						break;

					default:
						InputBuffer[j] = chr;
						j++;
						break;
				}

				i++;
			}

			me = ships[0];
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
	sprintf(MsgBuffer, "Message %s,%s,%s,%s", STUDENT_NUMBER, dest, source, msg);
}

void fire_at_ship(int x, int y) {
	fire = true;
	fireX = x;
	fireY = y;
}

void move_in_direction(int x, int y) {
	if (x < -2) x = -2;
	if (x >  2) x =  2;
	if (y < -2) y = -2;
	if (y >  2) y =  2;

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

	if (bind(sock_recv, (SOCKADDR *) &receive_addr, sizeof(SOCKADDR))) {
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
