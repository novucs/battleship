/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * connection.cc - Connection implementation.
 *
 * Provides an easy to use interface for managing networking tasks using the
 * battleship bot and hive bot protocols.
 */

#include "connection.h"

#include <winsock2.h>

#include <iostream>
#include <sstream>

#include "main.h"
#include "protocol_utils.h"

namespace hive_bot {

/**
 * Empty constructor, constructs a new connection.
 */
Connection::Connection() {
}

/**
 * Constructs a new connection.
 *
 * @param hostname The readable IP.
 * @param address The connection IP address.
 * @param port The port.
 */
Connection::Connection(std::string hostname, SOCKADDR_IN address,
                       u_short port) {
  address_ = address;
  port_ = port;
  hostname_ = hostname;
}

/**
 * Updates the connection to use pcap for sending.
 *
 * @param use_pcap {@code true} if pcap should be used.
 */
void Connection::SetUsePcap(bool use_pcap) {
  use_pcap_ = use_pcap;
}

/**
 * Gets the port.
 *
 * @return the port.
 */
u_short Connection::GetPort() {
  return port_;
}

/**
 * Gets the socket.
 *
 * @return the socket.
 */
SOCKET Connection::GetSocket() {
  return socket_;
}

/**
 * Gets the address.
 *
 * @return the address.
 */
SOCKADDR_IN Connection::GetAddress() {
  return address_;
}

/**
 * Creates a socket.
 *
 * @return {@code true} if the socket was created successfully, otherwise
 *         it will return {@code false}.
 */
bool Connection::CreateSocket() {
  // Error if the connection already has a socket created.
  if (socket_ != 0) {
    std::cout << "Socket creation failed: Connection already has a socket!";
    std::cout << std::endl;
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
  socket_ = created;

  return true;
}

/**
 * Closes the socket.
 */
void Connection::CloseSocket() {
  closesocket(socket_);
}

/**
 * Start listening to this connection by binding to it.
 */
bool Connection::Attach() {
  // Bind socket to the address.
  bool failed = (bool) bind(socket_, (SOCKADDR *) &address_, sizeof(SOCKADDR));

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
void Connection::Send(char* payload) {
  if (use_pcap_) {
    const u_short header_length = sizeof(struct UdpHeader) +
                                  sizeof(struct Ipv4Header) +
                                  sizeof(struct EthernetHeader);

    u_int length = strlen(payload);
    u_char* packet = (u_char*) malloc(length + header_length);

    u_char* message = packet + header_length;
    memcpy(message, payload, length);

    WriteUdp(length, packet, client_port, server_port,
             strdup(server_mac.c_str()), strdup(our_mac.c_str()),
             strdup(identity.GetIp().c_str()), strdup(server_ip.c_str()));

    if (pcap_sendpacket(pcap, packet, length + header_length) != 0) {
      std::cout << "Error sending packet: " << pcap_geterr(pcap) << std::endl;
      return;
    }

    free(packet);
  } else {
    sendto(socket_, payload, strlen(payload), 0, (SOCKADDR *) &address_,
           sizeof(SOCKADDR));
  }
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
int Connection::Receive(Connection from, char* buffer, int size) {
  int len = sizeof(SOCKADDR);
  SOCKADDR_IN address = GetAddress();

  int recover_code = recvfrom(socket_, buffer, size - 1, 0,
                              (SOCKADDR *) &address, &len);

  if (recover_code == SOCKET_ERROR) {
    // Do not print error when interrupted.
    if (WSAGetLastError() == 10004) {
      return RETREIVE_FAIL;
    }

    std::cout << "Failed to receive data: " << WSAGetLastError() << std::endl;
    return RETREIVE_FAIL;
  }

  u_long expected_address = from.GetAddress().sin_addr.s_addr;
  u_long actual_address = address.sin_addr.s_addr;

  // Spoof detected if not given the expected address.
  if (expected_address != actual_address) {
    return RETREIVE_IGNORE;
  }

  return RETREIVE_SUCCESS;
}

/**
 * Sends the tick packet to the connection.
 *
 * @param packet The packet to send.
 */
void Connection::SendTickPacket(TickPacket packet) {
  char* message = strdup(WriteTickPacket(packet).c_str());
  Send(message);
}

/**
 * Sends a list of ships to the connection.
 *
 * @param ships The ships to send.
 */
void Connection::SendShips(std::vector<Ship> ships) {
  char* message = strdup(WriteShips(ships).c_str());
  Send(message);
}

/**
 * Sends a fire packet to the connection.
 *
 * @param id The student identity code.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 */
void Connection::SendFire(std::string id, int x, int y) {
  char* message = strdup(WriteFire(id, x, y).c_str());
  Send(message);
}

/**
 * Sends a movement packet to the connection.
 *
 * @param id The student identity code.
 * @param x  The X speed to move.
 * @param y  The Y speed to move.
 */
void Connection::SendMove(std::string id, int x, int y) {
  char* message = strdup(WriteMove(id, x, y).c_str());
  Send(message);
}

/**
 * Sends a flag change packet to the connection.
 *
 * @param id   The student identity code.
 * @param flag The new flag.
 */
void Connection::SendFlag(std::string id, int flag) {
  char* message = strdup(WriteFlag(id, flag).c_str());
  Send(message);
}

/**
 * Sends a respawn packet to the connection.
 *
 * @param id        The student identity code.
 * @param forename  The students forename.
 * @param surname   The students surname.
 * @param ship_type The new ship type to spawn as.
 */
void Connection::SendRespawn(std::string id, std::string forename,
    std::string surname, int ship_type) {
  char* message = strdup(
      WriteRespawn(id, forename, surname, ship_type).c_str());
  Send(message);
}

/**
 * Creates a new connection.
 *
 * @param host The host this connection is on.
 * @param port The port this connection is on.
 */
Connection* InnerCreateConnection(std::string hostname, u_long host,
                                  u_short port) {
  SOCKADDR_IN address;
  port = htons(port);
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = host;
  address.sin_port = port;
  return new Connection(hostname, address, port);
}

/**
 * Creates a new localhost connection.
 *
 * @param port The port this connection is on.
 */
Connection CreateConnection(u_short port) {
  Connection* created = InnerCreateConnection("0.0.0.0", INADDR_ANY, port);
  return *created;
}

/**
 * Creates a new connection.
 *
 * @param hostname The hostname this connection is on.
 * @param port     The port this connection is on.
 */
Connection CreateConnection(std::string hostname, u_short port) {
  u_long host = inet_addr(hostname.c_str());
  Connection* created = InnerCreateConnection(hostname, host, port);
  return *created;
}

} // namespace hive_bot
