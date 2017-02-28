#pragma once

class ship {
	private:
		int x;
		int y;
		int health;
		int flag;
		int type;

	public:
		ship() {};
		ship(int x, int y, int health, int flag, int type);
		int get_x();
		int get_y();
		int get_health();
		int get_flag();
		int get_type();
		int get_range();
		int get_final_range();
		int get_max_speed();
		bool is_crippled();
		bool can_damage(ship& victim);
		int get_damage(ship& victim);
		int get_final_damage(ship& victim);
		double distance_to(ship& other);
};

bool operator==(ship& ship1, ship& ship2);
bool operator!=(ship& ship1, ship& ship2);
