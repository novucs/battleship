/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * protocol_utils.h - Protocol utils header.
 *
 * Provides a subset of protocol utilities for both the battleship bot and hive
 * bot protocols.
 */

#ifndef HIVE_BOT_PROTOCOL_UTILS_H_
#define HIVE_BOT_PROTOCOL_UTILS_H_

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <ws2tcpip.h>
#include <pcap.h>

#include "ship.h"
#include "tick_packet.h"

#define IP unsigned long
#define MAC unsigned long long

namespace hive_bot {

// The ethernet packet header.
struct EthernetHeader {
  u_char  destination[6];
  u_char  source[6];
  u_short protocol;
};

// The ARP packet header.
struct ArpHeader {
  u_short hardware_type;
  u_short protocol_type;
  u_char  hardware_length;
  u_char  protocol_length;
  u_short operation;
  u_char  sender_hardware_address[6];
  u_char  sender_protocol_address[4];
  u_char  target_hardware_address[6];
  u_char  target_protocol_address[4];
};

// The IPv4 packet header.
struct Ipv4Header {
  u_char header_length : 4;
  u_char version : 4;
  u_char type_of_service;
  u_short total_length;
  u_short id;
  u_short flags;
  u_char time_to_live;
  u_char protocol;
  u_short checksum;
  u_int source_address;
  u_int destination_address;
};

// The UDP packet header.
struct UdpHeader {
  u_short source_port;
  u_short destination_port;
  u_short length;
  u_short checksum;
};

// The current pcap session.
extern pcap_t* pcap;

// {@code true} if we're reading in everything from pcap.
extern bool pcap_in_loop;

// The pcap error buffer.
extern char pcap_error_buffer[PCAP_ERRBUF_SIZE];

// Our mac address.
extern std::string our_mac;

// The server mac address.
extern std::string server_mac;

// The ally ARP table (IP to MAC).
extern std::unordered_map<std::string, std::string> ally_arp_table;

// The enemy ARP table (IP to MAC).
extern std::unordered_map<std::string, std::string> enemy_arp_table;

// All captured ships from pcap.
extern std::vector<Ship> captured_ships;

// All captured student ships (IP to Ship).
extern std::unordered_map<std::string, Ship> captured_student_ships;

// All captured student IDs (IP to ID).
extern std::unordered_map<std::string, std::string> captured_ids;

// The mutex for synchronizing access to all protocol utils global variables.
extern std::mutex packet_handler_mutex;

/**
 * Checks if ship is already captured.
 *
 * @param ship The ship to check.
 */
bool IsCapturedShip(Ship& ship);

/**
 * Checks if ship is a student ship.
 *
 * @param ship The ship to check.
 */
bool IsCapturedStudentShip(Ship& ship);

/**
 * How to handle incoming and outgoing pcap packets.
 *
 * @param param The pcap parameters.
 * @param header The packet header.
 * @param pkt_data The packet data.
 */
void PacketHandler(u_char *param, const struct pcap_pkthdr* header,
                   const u_char* pkt_data);

/**
 * Writes a MAC address from human-readable to machine-readable.
 *
 * @param input The human-readable MAC.
 * @param output The machine-readable MAC.
 */
void WriteMac(char* input, u_char* output);

/**
 * Writes an ethernet packet header.
 *
 * @param packet The packet address to write to.
 * @param destination_mac The destination MAC address.
 * @param source_mac The source MAC address.
 * @param protocol The next header protocol.
 */
void WriteEthernet(u_char* packet,
                   char* destination_mac,
                   char* source_mac,
                   u_short protocol);

/**
 * Writes an ARP packet header.
 *
 * @param packet The packet to write to.
 * @param destination_mac The destination MAC address.
 * @param sourcE_mac The source MAC address.
 * @param sender_mac The sender MAC address.
 * @param target_mac The target MAC address.
 * @param sender_ip The sender IP address.
 * @param target_ip The target IP address.
 */
void WriteArp(u_char* packet,
              char* destination_mac,
              char* source_mac,
              char* sender_mac,
              char* target_mac,
              char* sender_ip,
              char* target_ip);

/**
 * Calculates the IPv4 checksum for a packet.
 *
 * @param packet The packet to calculate for.
 * @param length The header length.
 * @return The IPv4 checksum for this packet.
 */
u_short Ipv4Checksum(void* packet, size_t length);

/**
 * Writes an IPv4 packet header.
 *
 * @param length The packet length.
 * @param packet The packet to write to.
 * @param destination_mac The destination MAC address.
 * @param source_mac The source MAC address.
 * @param sender_ip The sender IP address.
 * @param target_ip The target IP address.
 * @param protool The next packet header protocol.
 */
void WriteIpv4(u_short length,
               u_char* packet,
               char* destination_mac,
               char* source_mac,
               char* sender_ip,
               char* target_ip,
               u_char protocol);

/**
 * Writes the UDP packet header.
 *
 * @param length The packet length.
 * @param packet The packet to write to.
 * @param source_port The source port
 * @param destination_port The destination port.
 * @param destination_mac The destination MAC address.
 * @param source_mac The source MAC address.
 * @param sender_ip The sender IP address.
 * @param target_ip The target IP address.
 */
void WriteUdp(u_short length,
              u_char* packet,
              u_short source_port,
              u_short destination_port,
              char* destination_mac,
              char* source_mac,
              char* sender_ip,
              char* target_ip);

/**
 * Checks if the provided IP is an allies IP.
 *
 * @param ip The IP to check.
 * @return {@code true} if the IP is an allies IP.
 */
bool IsAllyIp(std::string ip);

/**
 * Fetches a MAC address.
 *
 * @param ip The IP address to find the MAC for.
 * @param mac The MAC found.
 * @return {@code true} if retrieval was successful.
 */
bool FetchMac(IP ip, MAC* mac);

/**
 * Fetches all MAC addresses for a /24 network.
 *
 * @param c_network Any human-readable IP address in the /24 IP block.
 */
void FetchMacs(std::string c_network);

/**
 * Selects a device that pcap should listen to.
 *
 * @return {@code true} if device connection was successful.
 */
bool SelectDevice();

/**
 * Encrypts and decrypts a message using XOR encryption.
 *
 * @param message The message to encrypt.
 * @return the encrypted message.
 */
std::string XorEncrypt(std::string message);

/**
 * Reads in a tick packet.
 *
 * @param message The message to read.
 * @return the parsed tick packet.
 */
TickPacket ReadTickPacket(char* message);

/**
 * Reads in a list of ships.
 *
 * @param convert Apply the correct ship type to the first ship.
 * @param message The message to read.
 * @return the parsed list of ships.
 */
std::vector<Ship> ReadShips(bool convert, char* message);

/**
 * Writes a fire packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the fire packet.
 */
std::string WriteFire(std::string id, int x, int y);

/**
 * Writes a movement packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the movement packet.
 */
std::string WriteMove(std::string id, int x, int y);

/**
 * Writes a flag change packet.
 *
 * @param id   The student ID.
 * @param flag The new flag.
 * @return the flag change packet.
 */
std::string WriteFlag(std::string id, int flag);

/**
 * Writes a respawn packet.
 *
 * @param id        The student ID.
 * @param forename  The student forename.
 * @param surname   The student surname.
 * @param ship_type The new ship type.
 * @return the respawn packet.
 */
std::string WriteRespawn(std::string id,
                         std::string forename,
                         std::string surname,
                         int ship_type);

/**
 * Writes a tick packet.
 *
 * @param packet The packet to write.
 * @return the tick packet.
 */
std::string WriteTickPacket(TickPacket packet);

/**
 * Writes a ships packet.
 *
 * @param ships The ships to write.
 * @return the ships packet.
 */
std::string WriteShips(std::vector<Ship> ships);

} // namespace hive_bot

#endif // HIVE_BOT_PROTOCOL_UTILS_H_
