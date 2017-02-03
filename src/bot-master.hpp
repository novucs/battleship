#pragma once

#include "bot.hpp"
#include "network_manager.hpp"
#include "ship.hpp"

class bot_master : public bot {
	private:
		network_manager net;
		ship* me;
		std::vector<ship*> ships;
	public:
		void run();
};
