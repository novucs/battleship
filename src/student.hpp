#pragma once

#include <string>

class student {
	private:
		std::string id;
		std::string forename;
		std::string surname;
		std::string ip;

	public:
		student(std::string id, std::string forename, std::string surname, std::string ip);
		std::string get_id();
		std::string get_forename();
		std::string get_surname();
		std::string get_ip();
};
