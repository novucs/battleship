#include <math.h>
#include "ship.hpp"

ship::ship(int x, int y, int health, int flag, int type) {
	this->x = x;
	this->y = y;
	this->health = health;
	this->flag = flag;
	this->type = type;
}

int ship::get_x() {
	return x;
}

int ship::get_y() {
	return y;
}

int ship::get_health() {
	return health;
}

int ship::get_flag() {
	return flag;
}

int ship::get_type() {
	return type;
}

double ship::distance_to(ship& other) {
	int x = other.get_x() - this->x;
	int y = other.get_y() - this->y;
	return sqrt((x * x) + (y * y));
}

bool operator==(ship& ship1, ship& ship2) {
	return ship1.get_x() == ship2.get_x() &&
		ship1.get_y() == ship2.get_y() &&
		ship1.get_health() == ship2.get_health() &&
		ship1.get_flag() == ship2.get_flag() &&
		ship1.get_type() == ship2.get_type();
}
