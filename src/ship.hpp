#ifndef __SHIP_HPP_INCLUDED__
#define __SHIP_HPP_INCLUDED__

class ship {
	private:
		int x;
		int y;
		int health;
		int flag;
		int type;

	public:
		ship(int x, int y, int health, int flag, int type);
		int get_x();
		int get_y();
		int get_health();
		int get_flag();
		int get_type();
		int distance_to(ship* other);
};

#endif
