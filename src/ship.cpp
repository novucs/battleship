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

int ship::distance_to(ship* other) {
	int differenceX = other->x - x;
	int differenceY = other->y - y;
	return sqrt((differenceX * differenceX) + (differenceY * differenceY));
}
