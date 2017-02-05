#pragma once

#include <vector>
#include "ship.hpp"

std::vector<ship> read_ships(bool convert, char* message);

std::string write_fire(student sender, int x, int y);
std::string write_move(student sender, int x, int y);
std::string write_flag(student sender, int flag);
std::string write_respawn(student sender, int ship_type);
std::string write_ships(std::vector<ship> ships);
