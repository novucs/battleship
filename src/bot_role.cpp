#include "bot_role.hpp"


bool is_near(ship check_ship, ship against_ship) {
	if (check_ship.distance_to(against_ship) > LONEDISTANCE) {
		return false;
	}
	else {
		return true;
	}
}

bool is_near_any(int bot_pos, std::vector<ship> team_ships) {
	lone_bot = true;
	if (bs1 && (bot_pos != bs1_pos)) {
		if (is_near(team_ships[bot_pos], team_ships[bs1_pos])) {
			lone_bot = false;
		}
	}
	if (bs2 && (bot_pos != bs2_pos)) {
		if (is_near(team_ships[bot_pos], team_ships[bs2_pos])) {
			lone_bot = false;
		}
	}
	if (f1 && (bot_pos != f1_pos)) {
		if (is_near(team_ships[bot_pos], team_ships[f1_pos])) {
			lone_bot = false;
		}
	}
	if (f2 && (bot_pos != f2_pos)) {
		if (is_near(team_ships[bot_pos], team_ships[f2_pos])) {
			lone_bot = false;
		}
	}
	return lone_bot;
}


std::vector<ship> build_active_team(std::vector<bot_identity> bot_ids) {
	std::vector<ship> team_ships = {};
	for (thisI = 0; thisI < 4; thisI++) {//Builds vector of team ships currently with ID names.
		if (allies[thisI].is_connected()) {
			ally_ID = allies[thisI].get_team_id();
			if (ally_ID == BATTLESHIP1) {
				ship& team_bs1 = allies[thisI].get_ship();
				team_bs1.set_team_id(BATTLESHIP1);
				team_ships.push_back(team_bs1);
				bs1 = true;
				bs1_pos = thisI;
			}
			else if (ally_ID == BATTLESHIP2) {
				ship& team_bs2 = allies[thisI].get_ship();
				team_bs2.set_team_id(BATTLESHIP2);
				team_ships.push_back(team_bs2);
				bs2 = true;
				bs2_pos = thisI;
			}
			else if (ally_ID == FRIGATE1) {
				ship& team_f1 = allies[thisI].get_ship();
				team_f1.set_team_id(FRIGATE1);
				team_ships.push_back(team_f1);
				f1 = true;
				f1_pos = thisI;
			}
			else if (ally_ID == FRIGATE2) {
				ship& team_f2 = allies[thisI].get_ship();
				team_f2.set_team_id(FRIGATE2);
				team_ships.push_back(team_f2);
				f2 = true;
				f2_pos = thisI;
			}
		}
	}
	return team_ships;
}

void check_lone_status(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships) {
	lone_count = 0;
	if (bs1) {//If BattleShip 1 is lone
		if (is_near_any(bs1_pos, team_ships)) {//Checks position compared to active team bot positions
			bot_ids[0].set_active_role(true);//Sets "Battle Ship 1 - Lone Bot" role to active.
			team_ships[bs1_pos].set_role_id(0);//Sets bot's role ID for use in Scenario
			lone_count++;
		}
	}
	if (bs2) {//If Battleship 2 is lone - same procedure as "If BattleShip 1 is lone"
		if (is_near_any(bs2_pos, team_ships)) {
			bot_ids[1].set_active_role(true);
			team_ships[bs2_pos].set_role_id(1);
			lone_count++;
		}
	}
	if (f1) {//If Frigate 1 is lone - same procedure as "If BattleShip 1 is lone"
		if (is_near_any(f1_pos, team_ships)) {
			bot_ids[2].set_active_role(true);
			team_ships[f1_pos].set_role_id(2);
			lone_count++;
		}
	}
	if (f2) {//If Frigate 1 is lone - same procedure as "If BattleShip 1 is lone"
		if (is_near_any(f2_pos, team_ships)) {
			bot_ids[3].set_active_role(true);
			team_ships[f2_pos].set_role_id(3);
			lone_count++;
		}
	}
}

void check_bship_pair(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships) {
	if (bs1&&bs2) {//Checks if Battleships are in a pair.
		bs_pair = true;
		if (!is_near(team_ships[bs1_pos], team_ships[bs2_pos])) {
			bs_pair = false;
		}
		if (f1) {
			if (is_near(team_ships[bs1_pos], team_ships[f1_pos])) {
				bs_pair = false;
			}
			if (is_near(team_ships[bs2_pos], team_ships[f1_pos])) {
				bs_pair = false;
			}
		}
		if (f2) {
			if (is_near(team_ships[bs1_pos], team_ships[f2_pos])) {
				bs_pair = false;
			}
			if (is_near(team_ships[bs2_pos], team_ships[f2_pos])) {
				bs_pair = false;
			}
		}

		if (bs_pair) {
			team_ships[bs1_pos].set_role_id(4);
			bot_ids[4].set_active_role(true);
			team_ships[bs1_pos].set_role_id(5);
			bot_ids[5].set_active_role(true);
		}
	}
}

void check_frigate_pair(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships) {
	if (f1&&f2) {//Checks if Frigates are in a pair.
		f_pair = true;
		if (!is_near(team_ships[f1_pos], team_ships[f2_pos])) {
			f_pair = false;
		}
		if (bs1) {
			if (is_near(team_ships[bs1_pos], team_ships[f1_pos])) {
				f_pair = false;
			}
			if (is_near(team_ships[bs2_pos], team_ships[f1_pos])) {
				f_pair = false;
			}
		}
		if (bs2) {
			if (is_near(team_ships[bs1_pos], team_ships[f2_pos])) {
				f_pair = false;
			}
			if (is_near(team_ships[bs2_pos], team_ships[f2_pos])) {
				f_pair = false;
			}
		}

		if (f_pair) {
			team_ships[bs1_pos].set_role_id(6);
			bot_ids[6].set_active_role(true);
			team_ships[bs1_pos].set_role_id(7);
			bot_ids[7].set_active_role(true);
		}
	}
}

void check_mixed_pair1(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships)//Checks if bs1 is in a pair with a frigate
{
	m_pair1 = true;
	int frig_id = 0;
	if (bs1&&f1) {//Checks if bs1 is in a pair with f1
		if (!(bs_pair&&f_pair)) {
			if (!is_near(team_ships[bs1_pos], team_ships[f1_pos])) {
				m_pair1 = false;
				frig_id = 1;
			}
		}
		if (bs1&&f2) {//Checks if bs1 is in a pair with f2
			if (!(bs_pair&&f_pair)) {
				if (!is_near(team_ships[bs1_pos], team_ships[f2_pos])) {
					m_pair1 = false;
					frig_id = 2;
				}
			}
		}
		if (m_pair1) {
			team_ships[bs1_pos].set_role_id(8);
			bot_ids[8].set_active_role(true);
			if (frig_id == 1) {
				team_ships[f1_pos].set_role_id(9);
				bot_ids[9].set_active_role(true);
			}
			if (frig_id == 2) {
				team_ships[f2_pos].set_role_id(9);
				bot_ids[9].set_active_role(true);
			}
			if (frig_id == 0) {
				printf_s("m_pair1 Error - Frigate ID not set.");
			}
		}
	}
}

void check_mixed_pair2(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships) {//Checks if bs2 is in a pair with a frigate
	m_pair2 = true;
	int frig_id = 0;
	if (bs2&&f1) {//Checks if bs1 is in a pair with f1
		if (!(bs_pair&&f_pair)) {
			if (!is_near(team_ships[bs2_pos], team_ships[f1_pos])) {
				m_pair2 = false;
				frig_id = 1;
			}
		}
		if (bs2&&f2) {//Checks if bs1 is in a pair with f2
			if (!(bs_pair&&f_pair)) {
				if (!is_near(team_ships[bs2_pos], team_ships[f2_pos])) {
					m_pair2 = false;
					frig_id = 2;
				}
			}
		}
		if (m_pair2) {
			team_ships[bs2_pos].set_role_id(10);
			bot_ids[10].set_active_role(true);
			if (frig_id == 1) {
				team_ships[f1_pos].set_role_id(11);
				bot_ids[11].set_active_role(true);
			}
			if (frig_id == 2) {
				team_ships[f2_pos].set_role_id(11);
				bot_ids[11].set_active_role(true);
			}
			if (frig_id == 0) {
				printf_s("m_pair2 Error - Frigate ID not set.");
			}
		}
	}
}

void check_BBF_trio(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships)
{
	bbf_trio = false;
	if (bs1&&bs2) {
		if (f1&&f2) {
			if (!bs_pair && !f_pair && !m_pair1 && !m_pair2 && (lone_count = 1)) {
				team_ships[bs1_pos].set_role_id(12);
				bot_ids[12].set_active_role(true);
				team_ships[bs2_pos].set_role_id(13);
				bot_ids[13].set_active_role(true);
				if (is_near(team_ships[bs1_pos], team_ships[f1_pos])) {
					team_ships[f1_pos].set_role_id(14);
					bot_ids[14].set_active_role(true);
					bbf_trio = true;
				}
				if (is_near(team_ships[bs1_pos], team_ships[f2_pos])) {
					team_ships[f2_pos].set_role_id(14);
					bot_ids[14].set_active_role(true);
					bbf_trio = true;
				}
			}
		}
		if (f1 && !f2) {
			if (!bs_pair && !f_pair && !m_pair1 && !m_pair2 && (lone_count = 0)) {
				team_ships[bs1_pos].set_role_id(12);
				bot_ids[12].set_active_role(true);
				team_ships[bs2_pos].set_role_id(13);
				bot_ids[13].set_active_role(true);
				if (is_near(team_ships[bs1_pos], team_ships[f1_pos])) {
					team_ships[f1_pos].set_role_id(14);
					bot_ids[14].set_active_role(true);
					bbf_trio = true;
				}
			}
		}
		if (!f1 && f2) {
			if (!bs_pair && !f_pair && !m_pair1 && !m_pair2 && (lone_count = 0)) {
				team_ships[bs1_pos].set_role_id(12);
				bot_ids[12].set_active_role(true);
				team_ships[bs2_pos].set_role_id(13);
				bot_ids[13].set_active_role(true);
				if (is_near(team_ships[bs1_pos], team_ships[f2_pos])) {
					team_ships[f2_pos].set_role_id(14);
					bot_ids[14].set_active_role(true);
					bbf_trio = true;
				}
			}
		}
	}
}

void check_BFF_trio(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships)
{
	bff_trio = false;
	if (f1&&f2) {
		if (bs1&&bs2) {
			if (!bs_pair && !f_pair && !m_pair1 && !m_pair2 && !bbf_trio && (lone_count = 1)) {
				team_ships[f1_pos].set_role_id(15);
				bot_ids[15].set_active_role(true);
				team_ships[f2_pos].set_role_id(16);
				bot_ids[16].set_active_role(true);
				if (is_near(team_ships[f1_pos], team_ships[bs1_pos])) {
					team_ships[bs1_pos].set_role_id(17);
					bot_ids[17].set_active_role(true);
					bff_trio = true;
				}
				if (is_near(team_ships[f1_pos], team_ships[bs2_pos])) {
					team_ships[bs2_pos].set_role_id(17);
					bot_ids[17].set_active_role(true);
					bff_trio = true;
				}
			}
		}
		if (bs1 && !bs2) {
			if (!bs_pair && !f_pair && !m_pair1 && !m_pair2 && !bbf_trio && (lone_count = 0)) {
				team_ships[f1_pos].set_role_id(15);
				bot_ids[15].set_active_role(true);
				team_ships[f2_pos].set_role_id(16);
				bot_ids[16].set_active_role(true);
				if (is_near(team_ships[f1_pos], team_ships[bs1_pos])) {
					team_ships[bs1_pos].set_role_id(17);
					bot_ids[17].set_active_role(true);
					bff_trio = true;
				}
			}
		}
		if (!bs1 && bs2) {
			if (!bs_pair && !f_pair && !m_pair1 && !m_pair2 && !bbf_trio && (lone_count = 0)) {
				team_ships[f1_pos].set_role_id(15);
				bot_ids[15].set_active_role(true);
				team_ships[f2_pos].set_role_id(16);
				bot_ids[16].set_active_role(true);
				if (is_near(team_ships[f1_pos], team_ships[bs2_pos])) {
					team_ships[bs2_pos].set_role_id(17);
					bot_ids[17].set_active_role(true);
					bff_trio = true;
				}
			}
		}
	}
}

void check_complete_pack(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships){
	if (bs1&&bs2&&f1&&f2) {
		if (!bs_pair && !f_pair && !m_pair1 && !m_pair2 && !bbf_trio && !bff_trio) {
			bot_ids[18].set_active_role(true);
			team_ships[bs1_pos].set_role_id(18);
			bot_ids[19].set_active_role(true);
			team_ships[bs2_pos].set_role_id(19);
			bot_ids[20].set_active_role(true);
			team_ships[f1_pos].set_role_id(20);
			bot_ids[21].set_active_role(true);
			team_ships[f2_pos].set_role_id(21);
		}
	}

}

void check_all_roles(std::vector<bot_identity> bot_ids, std::vector<ship> team_ships)
{
	team_ships = build_active_team(bot_ids);
	check_lone_status(bot_ids, team_ships);//Sets active_roles 0,1,2,3
	check_bship_pair(bot_ids, team_ships);//Sets active_roles 4,5
	check_frigate_pair(bot_ids, team_ships);//Sets active_roles 6,7
	check_mixed_pair1(bot_ids, team_ships);//Sets active_roles 8,9
	check_mixed_pair2(bot_ids, team_ships);//Sets active_roles 10,11
	check_BBF_trio(bot_ids, team_ships);//Sets active_roles 12,13,14
	check_BFF_trio(bot_ids, team_ships);//Sets active_roles 15,16,17
	check_complete_pack(bot_ids, team_ships);//Sets active_roles 18,19,20,21
}


