/*
 * student.cpp
 *
 * Implements student related classes and methods defined in student.hpp.
 */

#include "student.hpp"

/**
 * Constructs a new student.
 *
 * @param load_order The order to load tick packets sent from this student.
 * @param offset_x   How far X to offset from the group center when moving.
 * @param offset_Y   How far Y to offset from the group center when moving.
 * @param id         The UWE student identity code.
 * @param forename   The students forename.
 * @param surname    The students surname.
 * @param ip         The students IP address.
 */
student::student(int load_order, int offset_x, int offset_y, std::string id,
		std::string forename, std::string surname, std::string ip) {
	this->load_order = load_order;
	this->offset_x = offset_x;
	this->offset_y = offset_y;
	this->id = id;
	this->forename = forename;
	this->surname = surname;
	this->ip = ip;
}

/**
 * Gets the order to load tick packets sent from this student.
 *
 * @return the load order.
 */
int student::get_load_order() {
	return load_order;
}

/**
 * Gets how far X this student is offset from the group center when moving.
 *
 * @return the X movement offset.
 */
int student::get_offset_x() {
	return offset_x;
}

/**
 * Gets how far Y this student is offset from the group center when moving.
 *
 * @return the Y movement offset.
 */
int student::get_offset_y() {
	return offset_y;
}

/**
 * Gets the current predicted score for this student.
 *
 * @return the score.
 */
int student::get_score() {
	return score;
}

/**
 * Sets the current predicted score for this student.
 *
 * @param score The predicted score.
 */
void student::set_score(int score) {
	this->score = score;
}

/**
 * Gets the UWE student identity code.
 *
 * @return the ID.
 */
std::string student::get_id() {
	return id;
}

/**
 * Gets the students forename.
 *
 * @return the forename.
 */
std::string student::get_forename() {
	return forename;
}

/**
 * Gets the students surname.
 *
 * @return the surname.
 */
std::string student::get_surname() {
	return surname;
}

/**
 * Gets the students IP address.
 *
 * @return the IP.
 */
std::string student::get_ip() {
	return ip;
}

/**
 * Gets the students ship details.
 *
 * @return the ship.
 */
ship student::get_ship() {
	return m_ship;
}

/**
 * Sets the students ship details.
 *
 * @param ship The updated students ship details.
 */
void student::set_ship(ship ship) {
	m_ship = ship;
}

/**
 * Gets the students connection.
 *
 * @return the connection;
 */
connection student::get_connection() {
	return m_connection;
}

/**
 * Sets the students connection.
 *
 * @param connection The updated students connection.
 */
void student::set_connection(connection connection) {
	m_connection = connection;
}

/**
 * Determines whether or not we have received any information from this
 * student in the current tick.
 *
 * @return {@code true} if they're connected, otherwise {@code false}.
 */
bool student::is_connected() {
	return connected;
}

/**
 * Sets if the student is connected or not.
 *
 * @param connected  {@code true} if they're connected, otherwise
 *                   {@code false}.
 */
void student::set_connected(bool connected) {
	this->connected = connected;
}
