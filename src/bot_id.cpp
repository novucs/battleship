#pragma once
#include "bot_id.hpp"


bot_identity::bot_identity()
{
	role_id = 0;
	ship_role = "";
	active_role = false;
}

bot_identity::bot_identity(int role_id, std::string ship_role, bool active_role)
{
	this->role_id = role_id;
	this->ship_role = ship_role;
	this->active_role = active_role;
}


bot_identity::~bot_identity()
{
}

int bot_identity::get_role_id() {
	return bot_identity::role_id;
}
std::string bot_identity::get_ship_role() {
	return bot_identity::ship_role;
}
bool bot_identity::is_active_role() {
	return bot_identity::active_role;
}
void bot_identity::set_active_role(bool active_role) {
	this->active_role = active_role;
}

