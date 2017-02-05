#include <sstream>
#include "main.hpp"
#include "protocol_utils.hpp"

std::vector<ship> read_ships(char* message) {
	std::vector<ship> ships;
	std::stringstream stream(message);

	int x;
	int y;
	int health;
	int flag;
	int type = 0;
	char separator;

	while (!stream.eof()) {
		stream >> x >> separator;
		stream >> y >> separator;
		stream >> health >> separator;
		stream >> flag >> separator;

		if (separator != '|') {
			stream >> type >> separator;
		} else {
			type = bot_class;
		}

		ships.push_back(ship(x, y, health, flag, type));
	}

	return ships;
}

