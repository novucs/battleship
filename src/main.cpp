#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include "bot.hpp"
#include "bot-master.hpp"
#include "bot-zombie.hpp"
#include "main.hpp"

std::string student_number = "16002374";
std::string student_firstname = "Billy";
std::string student_familyname = "Jenkins";

std::string server_ip = "127.0.0.1";
std::string master_ip = "127.0.0.1";
std::vector<std::string> zombie_ips = {
	"127.0.0.1"
};

u_short server_port = 1924;
u_short client_port = 1925;
u_short master_port = 13337;
u_short zombie_port = 13338;

const int zombie_type = 0;
const int master_type = 1;
int bot_type = master_type;

const int battleship_class = 0;
const int frigate_class = 1;
const int submarine_class = 2;
int bot_class = battleship_class;

// 16002374 = will
// 16000587 = josh
// 15019771 = jake
// 16014980 = gareth
std::vector<int> friend_ids = {16002374, 16000587, 15019771, 16014980};

bot* bot;

void load_bot() {
	switch (bot_type) {
		case master_type:
			bot = new bot_master();
			break;
		case zombie_type:
			bot = new bot_zombie();
			break;
	}
}

int main(int argc, char* argv[]) {
	load_bot();
	bot->run();
	return 0;
}
