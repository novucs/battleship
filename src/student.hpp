#pragma once

#include <string>
#include "connection.hpp"
#include "ship.hpp"

class student {
	private:
		std::string id;
		std::string forename;
		std::string surname;
		std::string ip;
		ship m_ship;
		connection m_connection;

	public:
		student(std::string id, std::string forename, std::string surname, std::string ip);
		std::string get_id();
		std::string get_forename();
		std::string get_surname();
		std::string get_ip();
		ship get_ship();
		void set_ship(ship ship);
		connection get_connection();
		void set_connection(connection connection);
};
