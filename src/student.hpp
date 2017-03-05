/*
 * student.hpp
 *
 * Defines student related classes and methods.
 */

#pragma once

#include <string>
#include "connection.hpp"
#include "ship.hpp"

/**
 * Represents a fellow UWE student.
 */
class student {

	private:
		// The order to load tick packets sent from this student.
		int load_order;

		// How far X to offset from the group center when moving.
		int offset_x;

		// How far Y to offset from the group center when moving.
		int offset_y;

		// The current predicted score.
		int score = 0;

		// The UWE student identity code.
		std::string id;

		// The students forename.
		std::string forename;

		// The students surname.
		std::string surname;

		// The students IP address.
		std::string ip;

		// The students ship details.
		ship m_ship;

		// The students connection details.
		connection m_connection;

		// Has the student sent us any information this tick?
		bool connected = false;

	public:
		/**
		 * Constructs a new student.
		 *
		 * @param load_order The order to load tick packets sent from this student.
		 * @param offset_x   How far X to offset from the group center when moving.
		 * @param offset_Y   How far Y to offset from the group center when moving.
		 * @param id         The UWE student identity code.
		 * @param forename   The students forename.
		 * @param surname    The students surname.
		 * @param ip         The students IP address.
		 */
		student(int load_order, int offset_x, int offset_y, std::string id,
			std::string forename, std::string surname, std::string ip);

		/**
		 * Gets the order to load tick packets sent from this student.
		 *
		 * @return the load order.
		 */
		int get_load_order();

		/**
		 * Gets how far X this student is offset from the group center when moving.
		 *
		 * @return the X movement offset.
		 */
		int get_offset_x();

		/**
		 * Gets how far Y this student is offset from the group center when moving.
		 *
		 * @return the Y movement offset.
		 */
		int get_offset_y();

		/**
		 * Gets the current predicted score for this student.
		 *
		 * @return the score.
		 */
		int get_score();

		/**
		 * Sets the current predicted score for this student.
		 *
		 * @param score The predicted score.
		 */
		void set_score(int score);

		/**
		 * Gets the UWE student identity code.
		 *
		 * @return the ID.
		 */
		std::string get_id();

		/**
		 * Gets the students forename.
		 *
		 * @return the forename.
		 */
		std::string get_forename();

		/**
		 * Gets the students surname.
		 *
		 * @return the surname.
		 */
		std::string get_surname();

		/**
		 * Gets the students IP address.
		 *
		 * @return the IP.
		 */
		std::string get_ip();

		/**
		 * Gets the students ship details.
		 *
		 * @return the ship.
		 */
		ship get_ship();

		/**
		 * Sets the students ship details.
		 *
		 * @param ship The updated students ship details.
		 */
		void set_ship(ship ship);

		/**
		 * Gets the students connection.
		 *
		 * @return the connection;
		 */
		connection get_connection();

		/**
		 * Sets the students connection.
		 *
		 * @param connection The updated students connection.
		 */
		void set_connection(connection connection);

		/**
		 * Determines whether or not we have received any information from this
		 * student in the current tick.
		 *
		 * @return {@code true} if they're connected, otherwise {@code false}.
		 */
		bool is_connected();

		/**
		 * Sets if the student is connected or not.
		 *
		 * @param connected  {@code true} if they're connected, otherwise
		 *                   {@code false}.
		 */
		void set_connected(bool connected);
};
