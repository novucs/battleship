#pragma once

#include <vector>
#include "ship.hpp"

std::vector<ship> read_ships(bool convert, char* message);
std::string write_ships(std::vector<ship> ships);
std::string write_fire(std::string id, int x, int y);
std::string write_move(std::string id, int x, int y);
std::string write_flag(std::string id, int flag);
std::string write_respawn(std::string id, std::string forename,
  std::string surname, int ship_type);
