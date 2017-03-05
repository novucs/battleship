/*
 * protocol_utils.cpp
 *
 * Implements all methods defined in protocol_utils.hpp
 */

#include <sstream>
#include "main.hpp"
#include "protocol_utils.hpp"

/**
 * Encrypts and decrypts a message using XOR encryption.
 *
 * @param message The message to encrypt.
 * @return the encrypted message.
 */
std::string xor_encrypt(std::string message) {
	for (std::size_t i = 0; i < message.length(); i++) {
		message[i] ^= key[i % key.length()];
	}
	return message;
}

/**
 * Reads in a tick packet.
 *
 * @param message The message to read.
 * @return the parsed tick packet.
 */
tick_packet read_tick_packet(char* message) {
	int score;
	std::vector<ship> ships;
	std::stringstream stream(xor_encrypt(message));

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

		ships.push_back(ship(x, y, health, flag, type));
	}

	return tick_packet(score, ships);
}

/**
 * Reads in a list of ships.
 *
 * @param convert Apply the correct ship type to the first ship.
 * @param message The message to read.
 * @return the parsed list of ships.
 */
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
			type = ship_type;
			convert = false;
		} else {
			stream >> type >> separator;
		}

		ships.push_back(ship(x, y, health, flag, type));
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
std::string write_fire(std::string id, int x, int y) {
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
std::string write_move(std::string id, int x, int y) {
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
std::string write_flag(std::string id, int flag) {
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

/**
 * Writes a tick packet.
 *
 * @param packet The packet to write.
 * @return the tick packet.
 */
std::string write_tick_packet(tick_packet packet) {
	std::stringstream message;

	message << packet.get_score() << '$';

	for (ship& s : packet.get_ships()) {
		message << s.get_x() << ',';
		message << s.get_y() << ',';
		message << s.get_health() << ',';
		message << s.get_flag() << ',';
		message << s.get_type() << '|';
	}

	std::string str = message.str();
	str.pop_back();
	return xor_encrypt(str);
}

/**
 * Writes a ships packet.
 *
 * @param ships The ships to write.
 * @return the ships packet.
 */
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
