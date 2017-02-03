#pragma once

#include <unordered_set>
#include "network_manager.hpp"
#include "ship.hpp"

class bot {
	public:
		virtual ~bot() {}
		virtual void run() = 0;
};
