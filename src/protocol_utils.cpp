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

std::string write_fire(student sender, int x, int y) {
	std::stringstream message;
	message << "Fire " << sender.get_id() << ',' << x << ',' << y;
	return message.str();
}

std::string write_move(student sender, int x, int y) {
	std::stringstream message;
	message << "Move " << sender.get_id() << ',' << x << ',' << y;
	return message.str();
}

std::string write_flag(student sender, int flag) {
	std::stringstream message;
	message << "Flag " << sender.get_id() << ',' << flag;
	return message.str();
}

std::string write_respawn(student sender, int ship_type) {
	std::stringstream message;
	message << "Register  ";
	message << sender.get_id() << ',';
	message << sender.get_forename() << ',';
	message << sender.get_surname() << ',';
	message << ship_type;
	return message.str();
}
