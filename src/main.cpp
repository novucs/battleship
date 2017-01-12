// BattleshipBot.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <winsock2.h>
#include <math.h>

#pragma comment(lib, "wsock32.lib")

#define SHIPTYPE_BATTLESHIP	"0"
#define SHIPTYPE_FRIGATE "1"
#define SHIPTYPE_SUBMARINE "2"

#define STUDENT_NUMBER "12345678"
#define STUDENT_FIRSTNAME "Fred"
#define STUDENT_FAMILYNAME "Bloggs"
#define MY_SHIP SHIPTYPE_BATTLESHIP

#define IP_ADDRESS_SERVER "127.0.0.1"
// #define IP_ADDRESS_SERVER "164.11.80.69"

#define PORT_SEND 1924 // We define a port that we are going to use.
#define PORT_RECEIVE 1925 // We define a port that we are going to use.

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

SOCKET sock_send;  // This is our socket, it is the handle to the IO address to read/write packets
SOCKET sock_recv;  // This is our socket, it is the handle to the IO address to read/write packets

WSADATA data;

char InputBuffer [MAX_BUFFER_SIZE];

/*
 * Firing range is 100
 * Visible range is 200
 */

int myX;
int myY;
int myHealth;
int myFlag;
int myType;

int number_of_ships; // Number of ships visible to us
int shipX[MAX_SHIPS]; // X locations of each ship within range
int shipY[MAX_SHIPS]; // Y locations of each ship within range
int shipHealth[MAX_SHIPS]; // Health of all visible ships
int shipFlag[MAX_SHIPS]; // Flag of each ship within range
int shipType[MAX_SHIPS]; // Type of each ship within range

bool message = false;
char MsgBuffer [MAX_BUFFER_SIZE];

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

int up_down = MOVE_LEFT * MOVE_SLOW;
int left_right = MOVE_UP * MOVE_FAST;

int shipDistance[MAX_SHIPS];

int number_of_friends;
int friendX[MAX_SHIPS];
int friendY[MAX_SHIPS];
int friendHealth[MAX_SHIPS];
int friendFlag[MAX_SHIPS];
int friendDistance[MAX_SHIPS];
int friendType[MAX_SHIPS];

int number_of_enemies;
int enemyX[MAX_SHIPS];
int enemyY[MAX_SHIPS];
int enemyHealth[MAX_SHIPS];
int enemyFlag[MAX_SHIPS];
int enemyDistance[MAX_SHIPS];
int enemyType[MAX_SHIPS];

bool IsaFriend(int index) {
	bool rc = false;

	if (shipFlag[index] == 123) {
		rc = true;  // I have just seen my friend 123
	}

	return rc;
}

void tactics() {
	if (myY > 900) {
		up_down = MOVE_DOWN * MOVE_SLOW;
	}

	if (myX < 200) {
		left_right = MOVE_RIGHT * MOVE_FAST;
	}

	if (myY < 100) {
		up_down = MOVE_UP * MOVE_FAST;
	}

	if (myX > 800) {
		left_right = MOVE_LEFT * MOVE_SLOW;
	}

	for (int i = 0; i < number_of_ships; i++) {
		shipDistance[i] = (int) sqrt((double) ((shipX[i]-shipX[0]) * (shipX[i]-shipX[0]) + (shipY[i] - shipY[0]) * (shipY[i] - shipY[0])));
	}

	number_of_friends = 0;
	number_of_enemies = 0;

	if (number_of_ships > 1) {
		for (int i = 1; i < number_of_ships; i++) {
			if (IsaFriend(i)) {
				friendX[number_of_friends] = shipX[i];
				friendY[number_of_friends] = shipY[i];
				friendHealth[number_of_friends] = shipHealth[i];
				friendFlag[number_of_friends] = shipFlag[i];
				friendDistance[number_of_friends] = shipDistance[i];
				friendType[number_of_friends] = shipType[i];
				number_of_friends++;
			} else {
				enemyX[number_of_enemies] = shipX[i];
				enemyY[number_of_enemies] = shipY[i];
				enemyHealth[number_of_enemies] = shipHealth[i];
				enemyFlag[number_of_enemies] = shipFlag[i];
				enemyDistance[number_of_enemies] = shipDistance[i];
				enemyType[number_of_enemies] = shipType[i];
				number_of_enemies++;
			}
		}

		if (number_of_enemies > 0) {
			fire_at_ship(enemyX[0], enemyY[0]);
		}
	}

	//char msg[100];
	//sprintf_s(msg, "Im at %d %d", myX, myY);
	//send_message("12345678", "23456789", msg);  // send my co-ordinates to myself

	move_in_direction(left_right, up_down);
}


void messageReceived(char* msg) {
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

	sprintf_s(buffer, "Register  %s,%s,%s,%s", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME, MY_SHIP);
	sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));

	while (true) {
		if (!recvfrom(sock_recv, buffer, sizeof(buffer)-1, 0, (SOCKADDR *)&receive_addr, &len) != SOCKET_ERROR) {
			printf_s("recvfrom error = %d\n", WSAGetLastError());
			return;
		}

		p = ::inet_ntoa(receive_addr.sin_addr);

		if ((strcmp(IP_ADDRESS_SERVER, "127.0.0.1") != 0) && (strcmp(IP_ADDRESS_SERVER, p) != 0)) {
			return;
		}

		if (buffer[0] == 'M') {
			messageReceived(buffer);
		} else {
			i = 0;
			j = 0;
			finished = false;
			number_of_ships = 0;

			while ((!finished) && (i < 4096)) {
				chr = buffer[i];

				switch (chr) {
					case '|':
						InputBuffer[j] = '\0';
						j = 0;
						sscanf_s(InputBuffer, "%d,%d,%d,%d", &shipX[number_of_ships], &shipY[number_of_ships], &shipHealth[number_of_ships], &shipFlag[number_of_ships], &shipType[number_of_ships]);
						number_of_ships++;
						break;

					case '\0':
						InputBuffer[j] = '\0';
						sscanf_s(InputBuffer, "%d,%d,%d,%d", &shipX[number_of_ships], &shipY[number_of_ships], &shipHealth[number_of_ships], &shipFlag[number_of_ships], &shipType[number_of_ships]);
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

			myX = shipX[0];
			myY = shipY[0];
			myHealth = shipHealth[0];
			myFlag = shipFlag[0];
			myType = shipType[0];
		}

		tactics();

		if (message) {
			sendto(sock_send, MsgBuffer, strlen(MsgBuffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			message = false;
		}

		if (fire) {
			sprintf_s(buffer, "Fire %s,%d,%d", STUDENT_NUMBER, fireX, fireY);
			sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			fire = false;
		}

		if (moveShip) {
			sprintf_s(buffer, "Move %s,%d,%d", STUDENT_NUMBER, moveX, moveY);
			rc = sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			moveShip = false;
		}

		if (setFlag) {
			sprintf_s(buffer, "Flag %s,%d", STUDENT_NUMBER, new_flag);
			sendto(sock_send, buffer, strlen(buffer), 0, (SOCKADDR *)&sendto_addr, sizeof(SOCKADDR));
			setFlag = false;
		}
	}

	printf_s("Student %s\n", STUDENT_NUMBER);
}

void send_message(char* dest, char* source, char* msg) {
	message = true;
	sprintf_s(MsgBuffer, "Message %s,%s,%s,%s", STUDENT_NUMBER, dest, source, msg);
}

void fire_at_ship(int X, int Y) {
	fire = true;
	fireX = X;
	fireY = Y;
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

	//sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	//if (!sock)
	//{
	//	printf("Socket creation failed!\n");
	//}

	sock_send = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	if (!sock_send) {
		printf("Socket creation failed!\n");
		return 0;
	}

	sock_recv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
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
	//	receive_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS_SERVER);
	receive_addr.sin_addr.s_addr = INADDR_ANY;
	receive_addr.sin_port = htons(PORT_RECEIVE);

	int ret = bind(sock_recv, (SOCKADDR *)&receive_addr, sizeof(SOCKADDR));
	//	int ret = bind(sock_send, (SOCKADDR *)&receive_addr, sizeof(SOCKADDR));
	if (ret) {
		printf("Bind failed! %d\n", WSAGetLastError());
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
