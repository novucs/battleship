#include "student.hpp"

student::student(std::string id, std::string forename, std::string surname,
		std::string ip) {
	this->id = id;
	this->forename = forename;
	this->surname = surname;
	this->ip = ip;
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
