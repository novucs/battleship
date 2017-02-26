#pragma once
#include <string>

class bot_identity
{
private:
	int role_id;
	std::string ship_role;
	bool active_role;
public:
	bot_identity();
	bot_identity(int role_id, std::string ship_role, bool active_role);
	~bot_identity();

	int get_role_id();
	std::string get_ship_role();
	bool is_active_role();


	void set_active_role(bool active_role);

};

