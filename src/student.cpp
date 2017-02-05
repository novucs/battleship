#include "student.hpp"

student::student(std::string id, std::string forename, std::string surname, std::string ip) {
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
