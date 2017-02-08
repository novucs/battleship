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
		int next_ship_type;
		ship m_ship;
		connection m_connection;

	public:
		student(std::string id, std::string forename, std::string surname,
			std::string ip, int next_ship_type);
		std::string get_id();
		std::string get_forename();
		std::string get_surname();
		std::string get_ip();
		int get_next_ship_type();
		void set_next_ship_type(int next_ship_type);
		ship get_ship();
		void set_ship(ship ship);
		connection get_connection();
		void set_connection(connection connection);
		void fire(int x, int y);
		void move(int x, int y);
		void flag(int flag);
		void respawn();
};
