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
			type = bot_class;
			convert = false;
		} else {
			stream >> type >> separator;
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
