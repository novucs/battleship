#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")

#include "bot.hpp"
#include "bot-master.hpp"
#include "bot-zombie.hpp"
#include "settings.hpp"

bot* bot;

// 16002374 = will
// 16000587 = josh
// 15019771 = jake
// 16014980 = gareth
std::unordered_set<int> friend_ids = {16002374, 16000587, 15019771, 16014980};

void load_bot() {
	switch (BOT_TYPE) {
		case MASTER_BOT:
			bot = new bot_master();
			break;
		case ZOMBIE_BOT:
			bot = new bot_zombie();
			break;
	}
}

int main(int argc, char* argv[]) {
	load_bot();
	bot->run();
	return 0;
}
