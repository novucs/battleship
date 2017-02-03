#pragma once

#include "bot.hpp"

class bot_master : public bot {
	private:
		network_manager net;
		ship* me;
		std::vector<ship*> ships;
	public:
		void run();
};
