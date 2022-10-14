#pragma once

struct Game
{
	int seconds, minutes, time_count, score, final_score, link, max_link, link_time, time_bonus;
	float time_multi;
	bool running, main_menu, started, paused, finished, single_player, vs_mode, dead_player;
	bool classic_mode, scatter_mode, battle_mode, split_screen, p1vp2, p1vcpu;
	char title_text[17], start_text[17], main_text[17], ready_text[17], go_back_text[17], back_text[17], quit_text[17];
	char pause_text[17], paused_menu[17], resume_text[17], restart_text[17], gbt_main_menu[17];
	char sp_text[17], vs_text[17], mode_text[17], classic_text[17], scatter_text[17], battle_text[17], split_text[17], p1vp2_text[17], p1vcpu_text[17];
	char controls_text[17],p1_controls[17], p2_controls[17], sp_objective1[17], sp_objective2[17],bm_objective1[17], bm_objective2[17];
	char vs_objective1[17], vs_objective2[17], p1vcpu_warning1[17], p1vcpu_warning2[17], p1vcpu_warning3[17];
	char score_text[17], final_score_text[17], time_multi_text[17], time_text[17], link_text[17], max_link_text[17];
	char p1_snack_count[17], p2_snack_count[17], cpu1_snack_counted[17], death_text[17], tie_text[17], win_text[17], lose_text[17];

	void init_()
	{
		running = true;
		main_menu = false;
		started = false;
		paused = false;
		finished = false;
		single_player = false;
		vs_mode = false;
		classic_mode = false;
		scatter_mode = false;
		battle_mode = false;
		split_screen = false;
		p1vp2 = false;
		p1vcpu = false;
		dead_player = false;
		sprintf(title_text, "Snakes");
		sprintf(start_text, "S-Start");
		sprintf(main_text, "Main Menu");
		sprintf(back_text, "T-Back to title");
		sprintf(sp_text, "1-Single Player");
		sprintf(vs_text, "2-VS Mode");
		sprintf(classic_text, "3-Classic Mode");
		sprintf(scatter_text, "4-Scatter Mode");
		sprintf(battle_text, "5-Battle Mode");
		sprintf(split_text, "6-Split Screen");
		sprintf(p1vp2_text, "7-1P vs 2P");
		sprintf(p1vcpu_text, "8-1P vs CPU");
		sprintf(controls_text, "Controls");
		sprintf(p1_controls, "1P-WASD");
		sprintf(p2_controls, "2P-Arrows");
		sprintf(sp_objective1, "Eat 50snacks");
		sprintf(sp_objective2, "Eat all snacks");
		sprintf(bm_objective1, "Eat 25snacks or");
		sprintf(bm_objective2, "Kill opponent");
		sprintf(vs_objective1, "Finish before");
		sprintf(vs_objective2, "your opponent");
		sprintf(p1vcpu_warning1, "Warning:");
		sprintf(p1vcpu_warning2, "The CPU cheats");
		sprintf(p1vcpu_warning3, "(sometimes)");
		sprintf(ready_text, "R-Ready");
		sprintf(go_back_text, "B-Back");
		sprintf(pause_text, "P-Pause");
		sprintf(death_text, "You Died");
		sprintf(tie_text, "Tie");
		sprintf(lose_text, "You Lose");
		sprintf(paused_menu, "Paused");
		sprintf(resume_text, "R-Resume");
		sprintf(restart_text, "Z-Start Over");
		sprintf(gbt_main_menu, "B-Back to main");
		sprintf(quit_text, "Q-Quit");
	}

	void sp_Stats_Init()
	{
		seconds = 0;
		minutes = 0;
		time_count = 0;
		score = 0;
		final_score = 0;
		link = 0;
		max_link = 0;
		link_time = 0;
		time_bonus = 0;
		time_multi = 10.f;
	}
};