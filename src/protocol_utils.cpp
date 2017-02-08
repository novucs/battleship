#include <sstream>
#include "main.hpp"
#include "protocol_utils.hpp"

std::vector<ship> read_ships(bool convert, char* message) {
	std::vector<ship> ships;
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
			type = identity.get_next_ship_type();
			convert = false;
		} else {
			stream >> type >> separator;
		}

		ships.push_back(ship(x, y, health, flag, type));
	}

	return ships;
}

std::string write_fire(std::string id, int x, int y) {
	std::stringstream message;
	message << "Fire " << id << ',' << x << ',' << y;
	return message.str();
}

std::string write_move(std::string id, int x, int y) {
	std::stringstream message;
	message << "Move " << id << ',' << x << ',' << y;
	return message.str();
}

std::string write_flag(std::string id, int flag) {
	std::stringstream message;
	message << "Flag " << id << ',' << flag;
	return message.str();
}

std::string write_respawn(std::string id, std::string forename,
		std::string surname, int ship_type) {
	std::stringstream message;
	message << "Register  ";
	message << id << ',';
	message << forename << ',';
	message << surname << ',';
	message << ship_type;
	return message.str();
}

std::string write_ships(std::vector<ship> ships) {
	std::stringstream message;

	for (ship& s : ships) {
		message << s.get_x() << ',';
		message << s.get_y() << ',';
		message << s.get_health() << ',';
		message << s.get_flag() << ',';
		message << s.get_type() << '|';
	}

	std::string str = message.str();
	str.pop_back();
	return str;
}
