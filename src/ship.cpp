#include <math.h>
#include "main.hpp"
#include "ship.hpp"

ship::ship(int x, int y, int health, int flag, int type) {
	this->x = x;
	this->y = y;
	this->health = health;
	this->flag = flag;
	this->type = type;
	this->team_id = 0;
	this->role_id = ENEMYID;
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

void ship::set_team_id(int team_id)
{
	this->team_id = team_id;
}

int ship::get_team_id()
{
	return team_id;
}

void ship::set_role_id(int role_id)
{
	this->role_id = role_id;
}

int ship::get_role_id() {
	return role_id;
}

int ship::get_range() {
	switch (type) {
	case SHIP_TYPE_BATTLESHIP:
		return 100;
	case SHIP_TYPE_FRIGATE:
		return 75;
	case SHIP_TYPE_SUBMARINE:
		return 50;
	}
	return 0;
}

int ship::get_final_range() {
	return ((get_range() * health) / 8) + 1;
}

bool ship::can_damage(ship& victim) {
	double distance = distance_to(victim);
	int range = get_final_range();
	return distance <= range;
}

int ship::get_damage(ship& victim) {
	int attacker_ship_type = get_type();
	int victim_ship_type = victim.get_type();

	switch (attacker_ship_type) {
	case SHIP_TYPE_BATTLESHIP:
		switch (victim_ship_type) {
		case SHIP_TYPE_BATTLESHIP:
			return 1;
		case SHIP_TYPE_FRIGATE:
			return 2;
		case SHIP_TYPE_SUBMARINE:
			return 3;
		}
	case SHIP_TYPE_FRIGATE:
		switch (victim_ship_type) {
		case SHIP_TYPE_BATTLESHIP:
			return 1;
		case SHIP_TYPE_FRIGATE:
			return 1;
		case SHIP_TYPE_SUBMARINE:
			return 2;
		}
	case SHIP_TYPE_SUBMARINE:
		switch (victim_ship_type) {
		case SHIP_TYPE_BATTLESHIP:
			return 2;
		case SHIP_TYPE_FRIGATE:
			return 2;
		case SHIP_TYPE_SUBMARINE:
			return 2;
		}
	}

	return 0;
}

int ship::get_final_damage(ship& victim) {
	if (can_damage(victim)) {
		return get_damage(victim);
	}

	return 0;
}

bool operator==(ship& ship1, ship& ship2) {
	return ship1.get_x() == ship2.get_x() &&
		ship1.get_y() == ship2.get_y() &&
		ship1.get_health() == ship2.get_health() &&
		ship1.get_flag() == ship2.get_flag() &&
		ship1.get_type() == ship2.get_type();
}

bool operator!=(ship& ship1, ship& ship2) {
	return ship1.get_x() != ship2.get_x() ||
		ship1.get_y() != ship2.get_y() ||
		ship1.get_health() != ship2.get_health() ||
		ship1.get_flag() != ship2.get_flag() ||
		ship1.get_type() != ship2.get_type();
}


