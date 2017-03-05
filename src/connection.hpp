/*
 * connection.hpp
 *
 * Manages all connection related tasks.
 */

#pragma once

#include <string>
#include <vector>
#include <ws2tcpip.h>
#include "ship.hpp"
#include "tick_packet.hpp"

// All retreive completion opcodes.
#define RETREIVE_SUCCESS 0
#define RETREIVE_FAIL 1
#define RETREIVE_IGNORE 2

/**
 * Holds UDP connection information and provides simple battleship bot helper
 * methods.
 */
class connection {

	private:
		// The connection port.
		int port;

		// The connection IP address.
		SOCKADDR_IN address;

		// The connection socket.
		SOCKET m_socket {0};

	public:
		/**
		 * Empty constructor, constructs a new connection.
		 */
		connection();

		/**
		 * Constructs a new connection.
		 *
		 * @param address The connection IP address.
		 */
		connection(SOCKADDR_IN address);

		/**
		 * Gets the port.
		 *
		 * @return the port.
		 */
		int get_port();

		/**
		 * Gets the socket.
		 *
		 * @return the socket.
		 */
		SOCKET get_socket();

		/**
		 * Gets the address.
		 *
		 * @return the address.
		 */
		SOCKADDR_IN get_address();

		/**
		 * Creates a socket.
		 *
		 * @return {@code true} if the socket was created successfully, otherwise
		 *				 it will return {@code false}.
		 */
		bool create_socket();

		/**
		 * Closes the socket.
		 */
		void close_socket();

		/**
		 * Start listening to this connection by binding to it.
		 */
		bool attach();

		/**
		 * Sends a message to this connection.
		 *
		 * @param message the message.
		 */
		void send(char* message);

		/**
		 * Receives a message on this connection if this connection is being
		 * listened to.
		 *
		 * @param from   Where to expect the information from.
		 * @param buffer The buffer to read into.
		 * @param size   The buffer size.
		 * @return the retreival status opcode.
		 */
		int receive(connection from, char* buffer, int size);

		/**
		 * Sends the tick packet to the connection.
		 *
		 * @param packet The packet to send.
		 */
		void send_tick_packet(tick_packet packet);

		/**
		 * Sends a list of ships to the connection.
		 *
		 * @param ships The ships to send.
		 */
		void send_ships(std::vector<ship> ships);

		/**
		 * Sends a fire packet to the connection.
		 *
		 * @param id The student identity code.
		 * @param x  The X coordinate to fire at.
		 * @param y  The Y coordinate to fire at.
		 */
		void send_fire(std::string id, int x, int y);

		/**
		 * Sends a movement packet to the connection.
		 *
		 * @param id The student identity code.
		 * @param x  The X speed to move.
		 * @param y  The Y speed to move.
		 */
		void send_move(std::string id, int x, int y);

		/**
		 * Sends a flag change packet to the connection.
		 *
		 * @param id   The student identity code.
		 * @param flag The new flag.
		 */
		void send_flag(std::string id, int flag);

		/**
		 * Sends a respawn packet to the connection.
		 *
		 * @param id        The student identity code.
		 * @param forename  The students forename.
		 * @param surname   The students surname.
		 * @param ship_type The new ship type to spawn as.
		 */
		void send_respawn(std::string id, std::string forename, std::string surname,
			int ship_type);
};

/**
 * Creates a new connection.
 *
 * @param host The host this connection is on.
 * @param port The port this connection is on.
 */
connection* inner_create_connection(u_long host, u_short port);

/**
 * Creates a new localhost connection.
 *
 * @param port The port this connection is on.
 */
connection create_connection(u_short port);

/**
 * Creates a new connection.
 *
 * @param hostname The hostname this connection is on.
 * @param port     The port this connection is on.
 */
connection create_connection(std::string hostname, u_short port);
