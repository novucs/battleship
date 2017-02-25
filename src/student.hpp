#pragma once

#include <string>
#include "connection.hpp"
#include "ship.hpp"

class student {
	private:
		int load_order;
		std::string id;
		std::string forename;
		std::string surname;
		std::string ip;
		ship m_ship;
		connection m_connection;
		bool connected;
		int bsf_id;
		int role_id;

	public:
		student(int load_order, std::string id, std::string forename,
			std::string surname, std::string ip, int bsf_id, int role_id);
		int get_load_order();
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
		int get_bsf_id();
		void set_role_id(int role_id);
};
