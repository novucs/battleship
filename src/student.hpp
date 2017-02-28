#pragma once

#include <string>
#include "connection.hpp"
#include "ship.hpp"

class student {
	private:
		int load_order;
		int offset_x;
		int offset_y;
		int score = 0;
		std::string id;
		std::string forename;
		std::string surname;
		std::string ip;
		ship m_ship;
		connection m_connection;
		bool connected;

	public:
		student(int load_order, int offset_x, int offset_y, std::string id,
			std::string forename, std::string surname, std::string ip);
		int get_load_order();
		int get_offset_x();
		int get_offset_y();
		int get_score();
		void set_score(int score);
		std::string get_id();
		std::string get_forename();
		std::string get_surname();
		std::string get_ip();
		ship get_ship();
		void set_ship(ship ship);
		connection get_connection();
		void set_connection(connection connection);
		bool is_connected();
		void set_connected(bool connected);
};
