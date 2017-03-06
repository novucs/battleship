/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * main.cc - Main implementation.
 *
 * Stores all program settings and is the program startpoint.
 */

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include "main.h"

#include <iostream>

#include "bot.h"

namespace hive_bot {

bool debug = false;

std::string key = "gY0HGe3oclNj9bc";
// std::string server_ip = "164.11.80.69";
std::string server_ip = "127.0.0.1";

u_short server_port = 1924;
u_short client_port = 1925;
u_short hive_port = 13337;

int team_member_id = 0;
int ship_type = SHIP_TYPE_BATTLESHIP;
int offset = 5;

std::vector<Student> team = {
  Student(
    3, offset, offset,
    "16002374",
    "William", "Randall",
    "164.11.80.37"
  ),
  Student(
    2, -offset, -offset,
    "16014980",
    "Gareth", "Perry",
    "164.11.80.26"
  ),
  Student(
    1, -offset, offset,
    "16000587",
    "Joshua", "Ramini",
    "164.11.80.33"
  ),
  Student(
    0, offset, -offset,
    "15019771",
    "Jacob", "Johnston",
    "164.11.80.65"
  )
};

Student identity = team.at(team_member_id);
std::vector<Student> allies;

} // namespace hive_bot

/**
 * Program starts here.
 *
 * @param argc The argument count.
 * @param argv The program arguments.
 * @return the exit code.
 */
int main(int argc, char* argv[]) {
  for (std::size_t i = 0; i < hive_bot::team.size(); i++) {
    if (i == (std::size_t) hive_bot::team_member_id) {
      continue;
    }

    hive_bot::allies.push_back(hive_bot::team.at(i));
  }

  WSADATA wsa_data;

  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    std::cout << "Failed to initialize WinSock2: " << WSAGetLastError();
    std::cout << std::endl;
  } else {
    hive_bot::Bot bot;

    if (bot.Setup()) {
      bot.Run();
    } else {
      std::cout << std::endl << "An error occured while initializing the bot.";
      std::cout << std::endl;
    }

    bot.Close();
  }

  system("pause");
  std::cout << std::endl << "Goodbye!" << std::endl;
  return 0;
}
