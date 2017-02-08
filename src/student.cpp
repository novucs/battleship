#include "student.hpp"

student::student(std::string id, std::string forename, std::string surname,
		std::string ip, int next_ship_type) {
	this->id = id;
	this->forename = forename;
	this->surname = surname;
	this->ip = ip;
	this->next_ship_type = next_ship_type;
}

std::string student::get_id() {
	return id;
}

std::string student::get_forename() {
	return forename;
}

std::string student::get_surname() {
	return surname;
}

std::string student::get_ip() {
	return ip;
}

int student::get_next_ship_type() {
	return next_ship_type;
}

void student::set_next_ship_type(int next_ship_type) {
	this->next_ship_type = next_ship_type;
}

ship student::get_ship() {
	return m_ship;
}

void student::set_ship(ship ship) {
	m_ship = ship;
}

connection student::get_connection() {
	return m_connection;
}

void student::set_connection(connection connection) {
	m_connection = connection;
}

void student::fire(int x, int y) {
	m_connection.send_fire(id, x, y);
}

void student::move(int x, int y) {
	m_connection.send_move(id, x, y);
}

void student::flag(int flag) {
	m_connection.send_flag(id, flag);
}

void student::respawn() {
	m_connection.send_respawn(id, forename, surname, next_ship_type);
}
