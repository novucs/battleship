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

struct EthernetHeader {
  u_char  destination[6];
  u_char  source[6];
  u_short protocol;
};

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

struct UdpHeader {
  u_short source_port;
  u_short destination_port;
  u_short length;
  u_short checksum;
};

extern pcap_t* pcap;
extern char pcap_error_buffer[PCAP_ERRBUF_SIZE];
extern char* our_mac;
extern char* server_mac;
extern std::unordered_map<char*, char*> ally_arp_table;
extern std::unordered_map<char*, char*> enemy_arp_table;
extern WORD last_server_tick;
extern std::vector<Ship> captured_ships;
extern std::unordered_map<char*, char*> captured_ids;

bool IsCapturedShip(Ship& ship);

void PacketHandler(u_char *param, const struct pcap_pkthdr* header,
                   const u_char* pkt_data);

void WriteMac(char* input, u_char* output);

void WriteEthernet(u_char* packet,
                   char* destination_mac,
                   char* source_mac,
                   u_short protocol);

void WriteArp(u_char* packet,
              char* destination_mac,
              char* source_mac,
              char* sender_mac,
              char* target_mac,
              char* sender_ip,
              char* target_ip);

u_short Ipv4Checksum(void* packet, size_t length);

void WriteIpv4(u_short length,
               u_char* packet,
               char* destination_mac,
               char* source_mac,
               char* sender_ip,
               char* target_ip,
               u_char protocol);

void WriteUdp(u_short length,
              u_char* packet,
              u_short source_port,
              u_short destination_port,
              char* destination_mac,
              char* source_mac,
              char* sender_ip,
              char* target_ip);

bool IsAllyIp(std::string ip);

bool FetchMac(IP ip, MAC* mac);

void FetchMacs(std::string c_network);

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
