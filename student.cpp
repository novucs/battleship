#include "student.hpp"

student::student(int load_order, std::string id, std::string forename,
	std::string surname, std::string ip, int bsf_id, int role_id) {
	this->load_order = load_order;
	this->id = id;
	this->forename = forename;
	this->surname = surname;
	this->ip = ip;
	this->team_id = bsf_id;
	this->role_id = role_id;
}

int student::get_load_order() {
	return load_order;
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

bool student::is_connected() {
	return connected;
}

void student::set_connected(bool connected) {
	this->connected = connected;
}

int student::get_team_id() {
	return team_id;
}

void student::set_role_id(int role_id)
{
	this->role_id = role_id;
}

