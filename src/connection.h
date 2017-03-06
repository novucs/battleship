/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * connection.h - Connection header.
 *
 * Provides an easy to use interface for managing networking tasks using the
 * battleship bot and hive bot protocols.
 */

#ifndef HIVE_BOT_CONNECTION_H_
#define HIVE_BOT_CONNECTION_H_

#include <ws2tcpip.h>

#include <string>
#include <vector>

#include "ship.h"
#include "tick_packet.h"

namespace hive_bot {

// All retreive completion opcodes.
#define RETREIVE_SUCCESS 0
#define RETREIVE_FAIL 1
#define RETREIVE_IGNORE 2

/**
 * Holds UDP connection information and provides simple battleship bot protocol
 * utilities.
 */
class Connection {

  private:
    // The connection port.
    int port_;

    // The connection IP address.
    SOCKADDR_IN address_;

    // The connection socket.
    SOCKET socket_ {0};

  public:
    /**
     * Empty constructor, constructs a new connection.
     */
    Connection();

    /**
     * Constructs a new connection.
     *
     * @param address The connection IP address.
     */
    Connection(SOCKADDR_IN address);

    /**
     * Gets the port.
     *
     * @return the port.
     */
    int GetPort();

    /**
     * Gets the socket.
     *
     * @return the socket.
     */
    SOCKET GetSocket();

    /**
     * Gets the address.
     *
     * @return the address.
     */
    SOCKADDR_IN GetAddress();

    /**
     * Creates a socket.
     *
     * @return {@code true} if the socket was created successfully, otherwise
     *         it will return {@code false}.
     */
    bool CreateSocket();

    /**
     * Closes the socket.
     */
    void CloseSocket();

    /**
     * Start listening to this connection by binding to it.
     */
    bool Attach();

    /**
     * Sends a message to this connection.
     *
     * @param message the message.
     */
    void Send(char* message);

    /**
     * Receives a message on this connection if this connection is being
     * listened to.
     *
     * @param from   Where to expect the information from.
     * @param buffer The buffer to read into.
     * @param size   The buffer size.
     * @return the retreival status opcode.
     */
    int Receive(Connection from, char* buffer, int size);

    /**
     * Sends the tick packet to the connection.
     *
     * @param packet The packet to send.
     */
    void SendTickPacket(TickPacket packet);

    /**
     * Sends a list of ships to the connection.
     *
     * @param ships The ships to send.
     */
    void SendShips(std::vector<Ship> ships);

    /**
     * Sends a fire packet to the connection.
     *
     * @param id The student identity code.
     * @param x  The X coordinate to fire at.
     * @param y  The Y coordinate to fire at.
     */
    void SendFire(std::string id, int x, int y);

    /**
     * Sends a movement packet to the connection.
     *
     * @param id The student identity code.
     * @param x  The X speed to move.
     * @param y  The Y speed to move.
     */
    void SendMove(std::string id, int x, int y);

    /**
     * Sends a flag change packet to the connection.
     *
     * @param id   The student identity code.
     * @param flag The new flag.
     */
    void SendFlag(std::string id, int flag);

    /**
     * Sends a respawn packet to the connection.
     *
     * @param id        The student identity code.
     * @param forename  The students forename.
     * @param surname   The students surname.
     * @param ship_type The new ship type to spawn as.
     */
    void SendRespawn(std::string id, std::string forename, std::string surname,
      int ship_type);
};

/**
 * Creates a new connection.
 *
 * @param host The host this connection is on.
 * @param port The port this connection is on.
 */
Connection* InnerCreateConnection(u_long host, u_short port);

/**
 * Creates a new localhost connection.
 *
 * @param port The port this connection is on.
 */
Connection CreateConnection(u_short port);

/**
 * Creates a new connection.
 *
 * @param hostname The hostname this connection is on.
 * @param port     The port this connection is on.
 */
Connection CreateConnection(std::string hostname, u_short port);

} // namespace hive_bot

#endif // HIVE_BOT_CONNECTION_H_
