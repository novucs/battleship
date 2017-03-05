/*
 * connection.cpp
 *
 * Implementation of methods defined in connection.hpp.
 */

#include <iostream>
#include <sstream>
#include <winsock2.h>
#include "connection.hpp"
#include "protocol_utils.hpp"

/**
 * Empty constructor, constructs a new connection.
 */
connection::connection() {
}

/**
 * Constructs a new connection.
 *
 * @param address The connection IP address.
 */
connection::connection(SOCKADDR_IN address) {
	this->address = address;
}

/**
 * Gets the port.
 *
 * @return the port.
 */
int connection::get_port() {
	return port;
}

/**
 * Gets the socket.
 *
 * @return the socket.
 */
SOCKET connection::get_socket() {
	return m_socket;
}

/**
 * Gets the address.
 *
 * @return the address.
 */
SOCKADDR_IN connection::get_address() {
	return address;
}

/**
 * Creates a socket.
 *
 * @return {@code true} if the socket was created successfully, otherwise
 *				 it will return {@code false}.
 */
bool connection::create_socket() {
	// Error if the connection already has a socket created.
	if (m_socket != 0) {
		std::cout << "Socket creation failed: Connection already has a socket!" << std::endl;
		return false;
	}

	// Create a new socket.
	SOCKET created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Error if the socket creation failed.
	if (!created) {
		std::cout << "Socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}

	// Update to the new socket and return.
	m_socket = created;

	return true;
}

/**
 * Closes the socket.
 */
void connection::close_socket() {
	closesocket(m_socket);
}

/**
 * Start listening to this connection by binding to it.
 */
bool connection::attach() {
	// Bind socket to the address.
	bool failed = (bool) bind(m_socket, (SOCKADDR *) &address, sizeof(SOCKADDR));

	// Error out if failed.
	if (failed) {
		std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

/**
 * Sends a message to this connection.
 *
 * @param message the message.
 */
void connection::send(char* message) {
	sendto(m_socket, message, strlen(message), 0, (SOCKADDR *) &address, sizeof(SOCKADDR));
}

/**
 * Receives a message on this connection if this connection is being
 * listened to.
 *
 * @param from   Where to expect the information from.
 * @param buffer The buffer to read into.
 * @param size   The buffer size.
 * @return the retreival status opcode.
 */
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

/**
 * Sends the tick packet to the connection.
 *
 * @param packet The packet to send.
 */
void connection::send_tick_packet(tick_packet packet) {
	char* message = strdup(write_tick_packet(packet).c_str());
	send(message);
}

/**
 * Sends a list of ships to the connection.
 *
 * @param ships The ships to send.
 */
void connection::send_ships(std::vector<ship> ships) {
	char* message = strdup(write_ships(ships).c_str());
	send(message);
}

/**
 * Sends a fire packet to the connection.
 *
 * @param id The student identity code.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 */
void connection::send_fire(std::string id, int x, int y) {
	char* message = strdup(write_fire(id, x, y).c_str());
	send(message);
}

/**
 * Sends a movement packet to the connection.
 *
 * @param id The student identity code.
 * @param x  The X speed to move.
 * @param y  The Y speed to move.
 */
void connection::send_move(std::string id, int x, int y) {
	char* message = strdup(write_move(id, x, y).c_str());
	send(message);
}

/**
 * Sends a flag change packet to the connection.
 *
 * @param id   The student identity code.
 * @param flag The new flag.
 */
void connection::send_flag(std::string id, int flag) {
	char* message = strdup(write_flag(id, flag).c_str());
	send(message);
}

/**
 * Sends a respawn packet to the connection.
 *
 * @param id        The student identity code.
 * @param forename  The students forename.
 * @param surname   The students surname.
 * @param ship_type The new ship type to spawn as.
 */
void connection::send_respawn(std::string id, std::string forename,
		std::string surname, int ship_type) {
	char* message = strdup(write_respawn(id, forename, surname, ship_type).c_str());
	send(message);
}

/**
 * Creates a new localhost connection.
 *
 * @param port The port this connection is on.
 */
connection create_connection(u_short port) {
	port = htons(port);
	connection* created = inner_create_connection(INADDR_ANY, port);
	return *created;
}

/**
 * Creates a new connection.
 *
 * @param hostname The hostname this connection is on.
 * @param port     The port this connection is on.
 */
connection create_connection(std::string hostname, u_short port) {
	u_long host = inet_addr(hostname.c_str());
	port = htons(port);
	connection* created = inner_create_connection(host, port);
	return *created;
}

/**
 * Creates a new connection.
 *
 * @param host The host this connection is on.
 * @param port The port this connection is on.
 */
connection* inner_create_connection(u_long host, u_short port) {
	SOCKADDR_IN address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = host;
	address.sin_port = port;
	return new connection(address);
}
