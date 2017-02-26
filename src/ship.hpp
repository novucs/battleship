#pragma once
#define ENEMYID 99;

class ship {
	private:
		int x;
		int y;
		int health;
		int flag;
		int type;
		int team_id;
		int role_id;
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
		bool can_damage(ship& victim);
		int get_damage(ship& victim);
		int get_final_damage(ship& victim);
		double distance_to(ship& other);
		void set_team_id(int team_id);
		int get_team_id();
		void set_role_id(int role_id);
		int get_role_id();
};

bool operator==(ship& ship1, ship& ship2);
bool operator!=(ship& ship1, ship& ship2);
