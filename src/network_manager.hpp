#ifndef __NETWORK_MANAGER_HPP_INCLUDED__
#define __NETWORK_MANAGER_HPP_INCLUDED__
#include <vector>
#include "connection.hpp"
#include "settings.hpp"
#include "ship.hpp"

class network_manager {
	private:
		connection server = create(1924);
		connection client = create(SERVER_IP, 1925);
	public:
		network_manager();
		connection get_server();
		connection get_client();
		void setup();
		void close();
		void send(connection connection, char* message);
		bool receive(connection connection, char* buffer, int size);
		std::vector<ship*> load_ships(char* message);
		ship* load_ship(char* message);
		void fire_at_ship(int x, int y);
		void move_in_direction(int x, int y);
		void set_flag(int flag);
		void respawn(int ship_type);
};

SOCKET create_socket();
void bind(SOCKET socket, SOCKADDR_IN address);

#endif
