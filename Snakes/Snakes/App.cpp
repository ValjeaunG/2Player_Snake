#pragma warning(disable:4996)
#include "Headers.h"

void App()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	//window properties
	int screen_w = 800;
	int screen_h = 600;
	SDL_Window* window = SDL_CreateWindow("Snakes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w, screen_h, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//frame rate stuff
	const int fps = 15;
	const int frame_delay = 1000 / fps;
	Uint32 frame_start;
	int frame_time;

	//keyboard stuff
	unsigned char prev_key_state[256];
	unsigned char* keys = (unsigned char*)SDL_GetKeyboardState(NULL);
	const Uint8* state = SDL_GetKeyboardState(NULL);

	//sprite stuff
	SDL_Surface* font_surface = IMG_Load("font_sheet.png");
	SDL_Texture* font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
	SDL_FreeSurface(font_surface);

	Game game;
	game.init_();
	game.sp_Stats_Init();

	enum Direction
	{
		UP, LEFT, DOWN, RIGHT
	};

	Snake player(400, 300, 10, 10, RIGHT);
	player.body;
	Snake player2(400, 350, 10, 10, LEFT);
	player2.body;
	CPU cpu1(400, 350, 10, 10, LEFT);
	cpu1.body;

	int max_snacks = 51;
	Object* snacks = (Object*)malloc(sizeof(Object)*max_snacks);

	SDL_Event event;

	srand(time(0));

	while (game.running)
	{
		//copies contents of keys to prev_key_state
		memcpy(prev_key_state, keys, 256);

		//gets milliseconds at the start of a frame
		frame_start = SDL_GetTicks();

		//consume all window events first
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) exit(0);
			if (event.type == SDL_KEYDOWN && game.started && !game.paused) //controls
			{
				if (event.key.keysym.sym == SDLK_w) { player.dir = UP; }
				if (event.key.keysym.sym == SDLK_a) { player.dir = LEFT; }
				if (event.key.keysym.sym == SDLK_s) { player.dir = DOWN; }
				if (event.key.keysym.sym == SDLK_d) { player.dir = RIGHT; }
				if (event.key.keysym.sym == SDLK_UP) { player2.dir = UP; }
				if (event.key.keysym.sym == SDLK_LEFT) { player2.dir = LEFT; }
				if (event.key.keysym.sym == SDLK_DOWN) { player2.dir = DOWN; }
				if (event.key.keysym.sym == SDLK_RIGHT) { player2.dir = RIGHT; }
			}
		}

		//set window screen color to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		//clear screen
		SDL_RenderClear(renderer);

		//start and quit
		if (state[SDL_SCANCODE_S] && !game.started) game.main_menu = true;
		if (state[SDL_SCANCODE_Q] && !game.main_menu && !game.started) game.running = false;

		//main menu
		if (game.main_menu && state[SDL_SCANCODE_1])
		{
			sprintf(game.mode_text, "Single Player");
			game.single_player = true;
			game.main_menu = false;
		}
		else if (game.main_menu && state[SDL_SCANCODE_2])
		{
			sprintf(game.mode_text, "VS Mode");
			game.vs_mode = true;
			game.main_menu = false;
		}
		else if (state[SDL_SCANCODE_T] && game.main_menu) game.main_menu = false;

		//single player mode
		if (game.single_player)
		{
			if (state[SDL_SCANCODE_B]) //back to main menu
			{
				game.single_player = false;
				game.main_menu = true;
				game.classic_mode = false;
				game.scatter_mode = false;
			}
			else if (state[SDL_SCANCODE_3]) game.classic_mode = true;
			else if (state[SDL_SCANCODE_4]) game.scatter_mode = true;

			if (game.classic_mode || game.scatter_mode)
			{
				if (state[SDL_SCANCODE_R] && !game.started)
				{
					player(400, 300, 10, 10, RIGHT);
					game.started = true;
					if (game.classic_mode) snacks[0](rand() % 80 * 10, rand() % 60 * 10, 10, 10); ///works because of operator() overload
					if (game.scatter_mode)
					{
						for (int i = 0; i < max_snacks - 1; i++) snacks[i](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
					}
				}
			}
		}

		//vs mode
		if (game.vs_mode)
		{
			if (state[SDL_SCANCODE_B]) //back to main menu
			{
				game.vs_mode = false;
				game.main_menu = true;
			}
			else if (state[SDL_SCANCODE_3]) game.classic_mode = true;
			else if (state[SDL_SCANCODE_4]) game.scatter_mode = true;

			if (game.classic_mode || game.scatter_mode)
			{
				if (state[SDL_SCANCODE_B])
				{
					game.classic_mode = false;
					game.scatter_mode = false;
					game.battle_mode = false;
					game.split_screen = false;
					game.p1vp2 = false;
					game.p1vcpu = false;
				}
				else if (state[SDL_SCANCODE_5]) game.battle_mode = true;
				else if (state[SDL_SCANCODE_6]) game.split_screen = true;

				if (game.battle_mode || game.split_screen)
				{
					if (state[SDL_SCANCODE_7]) game.p1vp2 = true;
					else if (state[SDL_SCANCODE_8]) game.p1vcpu = true;

					if (game.p1vp2 || game.p1vcpu)
					{
						if (state[SDL_SCANCODE_R] && !game.started)
						{
							game.started = true;
							if (game.battle_mode) player(400, 250, 10, 10, RIGHT);
							if (game.split_screen) player(200, 300, 10, 10, DOWN);
							if (game.p1vp2)
							{
								if (game.battle_mode) player2(400, 350, 10, 10, LEFT);
								if (game.split_screen) player2(600, 300, 10, 10, UP);
							}
							if (game.p1vcpu)
							{
								if (game.battle_mode) cpu1(400, 350, 10, 10, LEFT);
								if (game.split_screen) cpu1(600, 300, 10, 10, UP);
							}

							if (game.classic_mode && game.battle_mode)
							{
								snacks[0](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
								snacks[1](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
							}
							else if (game.classic_mode && game.split_screen)
							{
								snacks[0](rand() % 38 * 10, rand() % 60 * 10, 10, 10);
								snacks[1]((rand() % 38 + 42) * 10, rand() % 60 * 10, 10, 10);
							}

							if (game.scatter_mode && game.battle_mode)
							{
								for (int i = 0; i < max_snacks - 1; i++) snacks[i](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
							}
							else if (game.scatter_mode && game.split_screen)
							{
								for (int i = 0; i < 25; i++) snacks[i](rand() % 38 * 10, rand() % 60 * 10, 10, 10);
								for (int j = 26; j < max_snacks; j++) snacks[j]((rand() % 38 + 42) * 10, rand() % 60 * 10, 10, 10);
							}
						}
					}
				}
			}
		}

		//win and death states
		if (game.single_player && player.died) game.dead_player = true;
		if (game.single_player && player.snack_count == max_snacks - 1)
		{
			game.finished = true;
			sprintf(game.win_text, "You Win");
		}
		if (game.vs_mode)
		{
			if (game.p1vp2)
			{
				if (player.died && !player2.died || player2.snack_count == 25)
				{
					sprintf(game.win_text, "2P Wins");
					game.finished = true;
				}
				if (player2.died && !player.died || player.snack_count == 25)
				{
					sprintf(game.win_text, "1P Wins");
					game.finished = true;
				}
				if (player.died && player2.died) game.dead_player = true;
				if (player.snack_count == 25 && player2.snack_count == 25) game.finished = true;
			}
			if (game.p1vcpu)
			{
				if (cpu1.died && !player.died || player.snack_count == 25)
				{
					sprintf(game.win_text, "You Win");
					game.finished = true;
				}
				if (player.died && !cpu1.died || cpu1.snack_count == 25) game.finished = true;
				if (player.died && cpu1.died) game.dead_player = true;
				if (player.snack_count == 25 && cpu1.snack_count == 25) game.finished = true;
			}
		}

		//pause and endgame menu
		if (state[SDL_SCANCODE_P] && game.started && !game.finished && !game.dead_player) game.paused = true;
		if (state[SDL_SCANCODE_R] && game.paused) game.paused = false;
		else if (state[SDL_SCANCODE_Z] && game.paused || state[SDL_SCANCODE_Z] && game.finished || state[SDL_SCANCODE_Z] && game.dead_player)
		{
			//start over code
			game.paused = false;
			game.finished = false;
			game.dead_player = false;
			game.sp_Stats_Init();
			if (game.single_player) 
			{
				player(400, 300, 10, 10, RIGHT);
				if (game.classic_mode) snacks[0](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
				if (game.scatter_mode)
				{
					for (int i = 0; i < max_snacks - 1; i++) snacks[i](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
				}
			}
			if (game.vs_mode)
			{
				if (game.classic_mode && game.battle_mode)
				{
					player(400, 250, 10, 10, RIGHT);
					if (game.p1vp2) player2(400, 350, 10, 10, LEFT);
					if (game.p1vcpu) cpu1(400, 350, 10, 10, LEFT);
					snacks[0](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
					snacks[1](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
				}
				else if (game.classic_mode && game.split_screen)
				{
					player(200, 300, 10, 10, DOWN);
					if (game.p1vp2) player2(600, 300, 10, 10, UP);
					if (game.p1vcpu) cpu1(600, 300, 10, 10, UP);
					snacks[0](rand() % 38 * 10, rand() % 60 * 10, 10, 10);
					snacks[1]((rand() % 38 + 42) * 10, rand() % 60 * 10, 10, 10);
				}

				if (game.scatter_mode && game.battle_mode)
				{
					player(400, 250, 10, 10, RIGHT);
					if (game.p1vp2) player2(400, 350, 10, 10, LEFT);
					if (game.p1vcpu) cpu1(400, 350, 10, 10, LEFT);
					for (int i = 0; i < max_snacks - 1; i++) snacks[i](rand() % 80 * 10, rand() % 60 * 10, 10, 10);
				}
				else if (game.scatter_mode && game.split_screen)
				{
					player(200, 300, 10, 10, DOWN);
					if (game.p1vp2) player2(600, 300, 10, 10, UP);
					if (game.p1vcpu) cpu1(600, 300, 10, 10, UP);
					for (int i = 0; i < 25; i++) snacks[i](rand() % 38 * 10, rand() % 60 * 10, 10, 10);
					for (int j = 26; j < max_snacks; j++) snacks[j]((rand() % 38 + 42) * 10, rand() % 60 * 10, 10, 10);
				}
			}
		}
		else if (state[SDL_SCANCODE_B] && game.paused || state[SDL_SCANCODE_B] && game.finished || state[SDL_SCANCODE_B] && game.dead_player)
		{
			//back to main menu
			game.paused = false;
			game.started = false;
			game.finished = false;
			game.dead_player = false;
			game.p1vp2 = false;
			game.p1vcpu = false;
			game.battle_mode = false;
			game.split_screen = false;
			game.classic_mode = false;
			game.scatter_mode = false;
			game.single_player = false;
			game.vs_mode = false;
			game.main_menu = true;
			game.sp_Stats_Init();
		}

		//game stats
		{
			sprintf(game.time_text, "Time %d:%d", game.minutes, game.seconds);
			sprintf(game.time_multi_text, "Time Bonus x%d", game.time_bonus);
			sprintf(game.score_text, "Score:%d", game.score);
			sprintf(game.final_score_text, "Results:%d", game.final_score);
			sprintf(game.link_text, "Link:%d", game.link);
			sprintf(game.max_link_text, "Max Link:%d", game.max_link);
			if (game.link_time != 0 && !game.paused && !game.finished) game.link_time -= 2;
			if (game.link_time == 0) game.link = 0;
			if (game.single_player) sprintf(game.p1_snack_count, "Snacks:%d/50", player.snack_count);
			if (game.vs_mode) sprintf(game.p1_snack_count, "1Psnacks:%d/25", player.snack_count);
			sprintf(game.p2_snack_count, "2Psnacks:%d/25", player2.snack_count);
			sprintf(game.cpu1_snack_counted, "CPU1snacks:%d/25", cpu1.snack_count);
		}

		//time stuff for singleplayer classic mode
		{
			//clock
			if (game.single_player && game.classic_mode && game.started)
			{
				if (!game.paused && !game.finished && !game.dead_player)
				{
					game.time_count++;
					if (game.time_count > 14) ///14 is the magic number
					{                         ///actually no, its because the game runs at 15fps
						game.seconds += 1;    ///and the count starts at 0
						game.time_count = 0;
					}
				}
				if (game.seconds > 59)
				{
					game.minutes += 1;
					game.seconds = 0;
				}

				//score and time bonus multiplier
				if (game.time_count == 14)
				{
					if (game.seconds == 9 || game.seconds == 19 || game.seconds == 29 || game.seconds == 39 || game.seconds == 49 || game.seconds == 59)
					{
						if (game.score != 0) game.score--;
						if (game.minutes > 4 && game.time_multi > 1.f) game.time_multi -= 0.1f;
					}
				}
				if (game.finished)
				{
					game.time_bonus = game.time_multi * 10;
					game.final_score = game.score * game.time_bonus;
				}
			}
		}

		//movement
		if (game.started && !game.paused && !game.finished && !game.dead_player)
		{
			//player.move_Controls(state[SDL_SCANCODE_A], state[SDL_SCANCODE_D], state[SDL_SCANCODE_W], state[SDL_SCANCODE_S], 0, screen_w, 0, screen_h);
			player.movement_();
			if (game.p1vp2) player2.movement_();
			if (game.p1vcpu)
			{
				if (cpu1.wandering || cpu1.prioritize_wandering)
				{
					cpu1.dir_count++;
					if (cpu1.head.x == 10 || cpu1.head.x == screen_w - 10 || cpu1.head.y == 10 || cpu1.head.y == screen_h - 10) cpu1.dir_count = 10;
					if (game.split_screen && cpu1.head.x == 420) cpu1.dir_count = 10;
					if (cpu1.dir_count > 9)
					{
						cpu1.stuck_count = 0;
						cpu1.dir_change = rand() % 4;
						cpu1.direction_(UP);
						cpu1.direction_(DOWN);
						cpu1.direction_(LEFT);
						cpu1.direction_(RIGHT);

						if (cpu1.dir_change == UP && cpu1.dir == DOWN && cpu1.head.y == 10 || cpu1.dir_change == DOWN 
							&& cpu1.dir == UP && cpu1.head.y == screen_h - 10)
						{
							if (cpu1.head.x == 10) cpu1.dir = RIGHT;
							if (game.split_screen && cpu1.head.x == 420) cpu1.dir = RIGHT;
							if (cpu1.head.x == screen_w - 10) cpu1.dir = LEFT;
							cpu1.dir_count = 0;
							cpu1.prioritize_wandering = false;
						}
						if (cpu1.dir_change == LEFT && cpu1.dir == RIGHT && cpu1.head.x == 10 || cpu1.dir_change == RIGHT && cpu1.dir == LEFT 
							&& cpu1.head.x == screen_w - 10 || game.split_screen && cpu1.dir_change == LEFT && cpu1.dir != RIGHT && cpu1.head.x == 420)
						{
							if (cpu1.head.y == 10) cpu1.dir = DOWN;
							if (cpu1.head.y == screen_h - 10) cpu1.dir = UP;
							cpu1.dir_count = 0;
							cpu1.prioritize_wandering = false;
						}
					}
				}

				if (game.battle_mode || game.split_screen)
				{
					if (game.classic_mode)
					{
						if (cpu1.head.x == snacks[0].x || cpu1.head.x == snacks[1].x || cpu1.head.y == snacks[0].y 
							&& !game.split_screen || cpu1.head.y == snacks[1].y)
						{
							cpu1.wandering = false;
							cpu1.pursuit = true;
						}

						if (cpu1.pursuit)
						{
							if (cpu1.head.x == snacks[0].x || cpu1.head.x == snacks[1].x)
							{
								if (cpu1.head.y > snacks[0].y && cpu1.dir != DOWN || cpu1.head.y > snacks[1].y && cpu1.dir != DOWN) cpu1.dir = UP;
								else if (cpu1.head.y < snacks[0].y && cpu1.dir != UP || cpu1.head.y < snacks[1].y && cpu1.dir != UP) cpu1.dir = DOWN;
							}
							if (cpu1.head.y == snacks[0].y && !game.split_screen || cpu1.head.y == snacks[1].y)
							{
								if (cpu1.head.x > snacks[0].x && cpu1.dir != RIGHT || cpu1.head.x > snacks[1].x && cpu1.dir != RIGHT) cpu1.dir = LEFT;
								else if (cpu1.head.x < snacks[0].x && cpu1.dir != LEFT || cpu1.head.x < snacks[1].x && cpu1.dir != LEFT) cpu1.dir = RIGHT;
							}
							cm_Snack_Direction(&cpu1, snacks, 0, 1);
							cm_Snack_Direction(&cpu1, snacks, 1, 0);

							if (cpu1.head.x == snacks[0].x && cpu1.head.y == snacks[0].y || cpu1.head.x == snacks[1].x && cpu1.head.y == snacks[1].y)
							{
								cpu1.wandering = true;
								cpu1.pursuit = false;
								cpu1.dir_change = rand() % 4;
							}
						}
					}
					if (game.scatter_mode)
					{
						for (int i = 0; i < max_snacks; i++)
						{
							if (cpu1.head.x == snacks[i].x && !game.split_screen || snacks[i].x >= 420 
								&& cpu1.head.x == snacks[i].x && game.split_screen || cpu1.head.y == snacks[i].y)
							{
								cpu1.wandering = false;
								cpu1.pursuit = true;
							}

							if (cpu1.pursuit && !cpu1.prioritize_wandering)
							{
								if (cpu1.head.x == snacks[i].x && game.battle_mode || snacks[i].x >= 420 && cpu1.head.x == snacks[i].x && game.split_screen)
								{
									if (cpu1.head.y > snacks[i].y) cpu1.dir = UP;
									else if (cpu1.head.y < snacks[i].y) cpu1.dir = DOWN;
									else if (cpu1.head.y > snacks[i].y && cpu1.head.y < snacks[i].y)
									{
										cpu1.dir_count++;
										if (cpu1.dir_count == 1)
										{
											cpu1.dir_change = rand() % 2;
											if (cpu1.dir_change == 0) cpu1.dir = UP;
											else if (cpu1.dir_change == 1) cpu1.dir = DOWN;
										}
									}
								}
								if (cpu1.head.y == snacks[i].y)
								{
									if (cpu1.head.x == snacks[i].x && game.battle_mode || snacks[i].x >= 420
										&& cpu1.head.x > snacks[i].x && game.split_screen) cpu1.dir = LEFT;
									else if (cpu1.head.x < snacks[i].x) cpu1.dir = RIGHT;
									else if (cpu1.head.x > snacks[i].x && cpu1.head.x < snacks[i].x)
									{
										cpu1.dir_count++;
										if (cpu1.dir_count == 1)
										{
											cpu1.dir_change = rand() % 2;
											if (cpu1.dir_change == 0) cpu1.dir = LEFT;
											else if (cpu1.dir_change == 1) cpu1.dir = RIGHT;
										}
									}
								}

								if (cpu1.head.x == snacks[i].x && cpu1.head.y == snacks[i].y)
								{
									cpu1.wandering = true;
									cpu1.pursuit = false;
									cpu1.dir_change = rand() % 4;
								}
							}
						}
					}
				}

				cpu1.movement_();
			}
		}

		{} ///don't mess with this

		//collision detection
		{
			//with snack
			if (game.single_player)
			{
				if (game.classic_mode && player.head.x == snacks[0].x && player.head.y == snacks[0].y)
				{
					player.snack_eaten = true;
					snacks[0].x = rand() % 80 * 10;
					snacks[0].y = rand() % 60 * 10;
					std::for_each(player.body.begin(), player.body.end(), [&](auto& segment) {
						if (segment.x == snacks[0].x && segment.y == snacks[0].y) //if snack teleports directly to the snake's body
						{                                                         //teleport the snack somewhere else
							snacks[0].x = rand() % 80 * 10;
							snacks[0].y = rand() % 60 * 10;
						}
						else if (segment.x != snacks[0].x && segment.y != snacks[0].y && player.snack_eaten)
						{
							game.score += 10;
							player.length += 10;
							player.snack_count += 1;
							player.snack_eaten = false;
						}
					});
				}
				if (game.scatter_mode)
				{
					if (game.link > game.max_link) game.max_link = game.link;

					for (int i = 0; i < max_snacks - 1; i++)
					{
						if (player.head.x == snacks[i].x && player.head.y == snacks[i].y)
						{
							player.length += 10;
							player.snack_count += 1;
							snacks[i].x = -10;
							snacks[i].y = -10;
							game.link_time = 100;
							if (game.link_time >= 0) game.link += 1;
							game.score += 10 * game.link;
						}
					}

					//max link bonus multiplier
					if (game.finished) game.final_score = game.score * game.max_link;
				}
			}
			if (game.vs_mode)
			{
				if (game.classic_mode && game.battle_mode)
				{
					classic_Mode_Collision(&player, snacks, 0, 80, 0);
					classic_Mode_Collision(&player, snacks, 1, 80, 0);
					classic_Mode_Collision(&player2, snacks, 0, 80, 0);
					classic_Mode_Collision(&player2, snacks, 1, 80, 0);
					classic_Mode_Collision(&cpu1, snacks, 0, 80, 0);
					classic_Mode_Collision(&cpu1, snacks, 1, 80, 0);
				}
				else if (game.classic_mode && game.split_screen)
				{
					classic_Mode_Collision(&player, snacks, 0, 38, 0);
					classic_Mode_Collision(&player2, snacks, 1, 38, 42);
					classic_Mode_Collision(&cpu1, snacks, 1, 38, 42);
				}

				if (game.scatter_mode)
				{
					scatter_Mode_Collision(&player, snacks, max_snacks);
					scatter_Mode_Collision(&player2, snacks, max_snacks);
					scatter_Mode_Collision(&cpu1, snacks, max_snacks);
				}
			}

			//with body
			std::for_each(player.body.begin(), player.body.end(), [&](auto& segment) {
				if (player.head.x == segment.x && player.head.y == segment.y && !game.paused && !game.finished && player.length != 1) player.died = true;
				std::for_each(player2.body.begin(), player2.body.end(), [&](auto& p2_seg) {
					if (player.head.x == p2_seg.x && player.head.y == p2_seg.y && !game.paused && !game.finished) player.died = true;
				});
				std::for_each(cpu1.body.begin(), cpu1.body.end(), [&](auto& cpu1_seg) {
					if (player.head.x == cpu1_seg.x && player.head.y == cpu1_seg.y && !game.paused && !game.finished) player.died = true;
				});
			});
			std::for_each(player2.body.begin(), player2.body.end(), [&](auto& segment) {
				if (player2.head.x == segment.x && player2.head.y == segment.y && !game.paused && !game.finished && player2.length != 1) player2.died = true;
				std::for_each(player.body.begin(), player.body.end(), [&](auto& p1_seg) {
					if (player2.head.x == p1_seg.x && player2.head.y == p1_seg.y && !game.paused && !game.finished) player2.died = true;
				});
			});
			std::for_each(cpu1.body.begin(), cpu1.body.end(), [&](auto& segment) {
				if (cpu1.head.x == segment.x && cpu1.head.y == segment.y && cpu1.length != 1)
				{
					cpu1.stuck_count++;
					cpu1.body_Collision(UP, LEFT, DOWN, RIGHT);

					if (cpu1.stuck_count > 9) cpu1.died = true;
				}

				std::for_each(player.body.begin(), player.body.end(), [&](auto& p1_seg) {
					if (cpu1.head.x == p1_seg.x && cpu1.head.y == p1_seg.y)
					{
						cpu1.body_Collision(UP, LEFT, DOWN, RIGHT);
					}
				});
			});

			//head to head
			snake_Collision(&player, &player2);
			snake_Collision(&player, &cpu1);

			//with walls
			if (player.head.x == -10 || player.head.x == 390 && game.split_screen || player.head.x == screen_w || player.head.y == -10 || player.head.y == screen_h)
			{
				if (!game.finished) player.died = true;
			}
			if (player2.head.x == -10 || player2.head.x == 410 && game.split_screen || player2.head.x == screen_w || player2.head.y == -10 || player2.head.y == screen_h)
			{
				if (!game.finished) player2.died = true;
			}
			if (cpu1.head.x == -10 || cpu1.head.x == 410 && game.split_screen || cpu1.head.x == screen_w || cpu1.head.y == -10 || cpu1.head.y == screen_h)
			{
				cpu1.body_Collision(UP, LEFT, DOWN, RIGHT);
			}
		}

		if (!game.paused && !game.finished) //add newest head to snake
		{
			player.body.push_front(player.head);
			while (player.body.size() > player.length) player.body.pop_back();
			player2.body.push_front(player2.head);
			while (player2.body.size() > player2.length) player2.body.pop_back();
			cpu1.body.push_front(cpu1.head);
			while (cpu1.body.size() > cpu1.length) cpu1.body.pop_back();
		}

		//unsigned int current_time = SDL_GetTicks(); //for sprites

		//draw
		{
			//title screen text
			if (!game.started && !game.main_menu && !game.single_player && !game.vs_mode)
			{
				draw_Text(renderer, font_texture, game.title_text, 50, 275, 10);
				draw_Text(renderer, font_texture, game.start_text, 25, 315, 275);
				draw_Text(renderer, font_texture, game.quit_text, 25, 330, 500);
			}

			//menu screen text
			if (game.main_menu)
			{
				draw_Text(renderer, font_texture, game.main_text, 50, 200, 10);
				draw_Text(renderer, font_texture, game.sp_text, 25, 225, 225);
				draw_Text(renderer, font_texture, game.vs_text, 25, 300, 275);
				draw_Text(renderer, font_texture, game.back_text, 25, 225, 500);
			}

			//single player menu text
			if (game.single_player && !game.started)
			{
				draw_Text(renderer, font_texture, game.mode_text, 50, 75, 10);
				if (!game.classic_mode && !game.scatter_mode)
				{
					draw_Text(renderer, font_texture, game.classic_text, 25, 225, 225);
					draw_Text(renderer, font_texture, game.scatter_text, 25, 225, 275);
				}
				if (game.classic_mode || game.scatter_mode)
				{
					draw_Text(renderer, font_texture, game.controls_text, 25, 310, 175);
					draw_Text(renderer, font_texture, game.p1_controls, 25, 315, 225);
					if (game.classic_mode) draw_Text(renderer, font_texture, game.sp_objective1, 25, 250, 325);
					if (game.scatter_mode) draw_Text(renderer, font_texture, game.sp_objective2, 25, 225, 325);
					draw_Text(renderer, font_texture, game.ready_text, 25, 315, 450);
				}
				draw_Text(renderer, font_texture, game.go_back_text, 25, 325, 500);
			}

			//vs_mode menu text
			if (game.vs_mode && !game.started)
			{
				draw_Text(renderer, font_texture, game.mode_text, 50, 225, 10);

				if (!game.classic_mode && !game.scatter_mode)
				{
					draw_Text(renderer, font_texture, game.classic_text, 25, 225, 225);
					draw_Text(renderer, font_texture, game.scatter_text, 25, 225, 275);
				}
				if (game.classic_mode || game.scatter_mode)
				{
					if (!game.battle_mode && !game.split_screen)
					{
						draw_Text(renderer, font_texture, game.battle_text, 25, 225, 225);
						draw_Text(renderer, font_texture, game.split_text, 25, 225, 275);
					}
					if (game.battle_mode || game.split_screen)
					{
						if (!game.p1vp2 && !game.p1vcpu)
						{
							draw_Text(renderer, font_texture, game.p1vp2_text, 25, 275, 225);
							draw_Text(renderer, font_texture, game.p1vcpu_text, 25, 275, 275);
						}
						if (game.p1vp2 || game.p1vcpu)
						{
							draw_Text(renderer, font_texture, game.controls_text, 25, 300, 175);
							draw_Text(renderer, font_texture, game.p1_controls, 25, 315, 225);
							if (game.p1vp2) draw_Text(renderer, font_texture, game.p2_controls, 25, 295, 250);
							if (game.p1vcpu && game.battle_mode)
							{
								draw_Text(renderer, font_texture, game.p1vcpu_warning1, 25, 100, 375);
								draw_Text(renderer, font_texture, game.p1vcpu_warning2, 25, 325, 375);
								draw_Text(renderer, font_texture, game.p1vcpu_warning3, 15, 330, 400);
							}
							draw_Text(renderer, font_texture, game.ready_text, 25, 330, 450);
						}
					}
					if (game.battle_mode)
					{
						if (game.p1vp2 || game.p1vcpu)
						{
							draw_Text(renderer, font_texture, game.bm_objective1, 25, 225, 300);
							draw_Text(renderer, font_texture, game.bm_objective2, 25, 250, 325);
						}
					}
					if (game.split_screen)
					{
						if (game.p1vp2 || game.p1vcpu)
						{
							draw_Text(renderer, font_texture, game.vs_objective1, 25, 250, 300);
							draw_Text(renderer, font_texture, game.vs_objective2, 25, 250, 325);
						}
					}
				}
				draw_Text(renderer, font_texture, game.go_back_text, 25, 340, 500);
			}

			if (game.started)
			{
				//player.draw_(renderer);
				player.draw_Snake(renderer, 0, 255, 0);
				if (game.p1vp2) player2.draw_Snake(renderer, 255, 255, 0);
				if (game.p1vcpu) cpu1.draw_Snake(renderer, 255, 0, 0);

				//snacks
				if (game.single_player || game.battle_mode)
				{
					if (game.classic_mode)
					{
						snacks[0].draw_(renderer);
						if (game.battle_mode) snacks[1].draw_(renderer);
					}
					if (game.scatter_mode)
					{
						for (int i = 0; i < max_snacks - 1; i++) snacks[i].draw_(renderer);
					}
				}
				if (game.split_screen)
				{
					draw_Boundary_Line(renderer, 390, 0, 30, 600);

					if (game.classic_mode)
					{
						snacks[0].draw_(renderer);
						snacks[1].draw_(renderer);
					}
					if (game.scatter_mode)
					{
						for (int i = 0; i < 25; i++) snacks[i].draw_(renderer);
						for (int j = 26; j < max_snacks; j++) snacks[j].draw_(renderer);
					}
				}

				//game stats text
				draw_Text_Box(renderer, 0, 0, 800, 30);
				if (game.single_player)
				{
					draw_Text(renderer, font_texture, game.score_text, 15, 50, 10);
					if (game.classic_mode)
					{
						draw_Text(renderer, font_texture, game.time_text, 15, 250, 10);
						draw_Text(renderer, font_texture, game.p1_snack_count, 15, 425, 10);
					}
					if (game.scatter_mode)
					{
						draw_Text(renderer, font_texture, game.link_text, 15, 225, 10);
						draw_Link_Bar(renderer, 225, 10, game.link_time, 10);
					}
				}
				if (game.vs_mode)
				{
					draw_Text(renderer, font_texture, game.p1_snack_count, 15, 50, 10);
					if (game.p1vp2) draw_Text(renderer, font_texture, game.p2_snack_count, 15, 300, 10);
					if (game.p1vcpu) draw_Text(renderer, font_texture, game.cpu1_snack_counted, 15, 300, 10);
				}

				//pause menu text
				if (!game.paused && !game.dead_player && !game.finished) draw_Text(renderer, font_texture, game.pause_text, 15, 650, 10);
				else if (game.paused)
				{
					draw_Text_Box(renderer, 225, 125, 400, 200);
					draw_Text(renderer, font_texture, game.paused_menu, 50, 275, 125);
					draw_Text(renderer, font_texture, game.resume_text, 25, 325, 200);
					draw_Text(renderer, font_texture, game.restart_text, 25, 275, 250);
					draw_Text(renderer, font_texture, game.gbt_main_menu, 25, 250, 300);
				}

				//endgame menu text
				if (game.finished)
				{
					if (game.single_player)
					{
						if (game.classic_mode)
						{
							draw_Text_Box(renderer, 225, 125, 400, 350);
							draw_Text(renderer, font_texture, game.win_text, 50, 225, 125);
							draw_Text(renderer, font_texture, game.time_text, 25, 300, 200);
							draw_Text(renderer, font_texture, game.score_text, 25, 300, 225);
							draw_Text(renderer, font_texture, game.time_multi_text, 25, 225, 250);
							draw_Text(renderer, font_texture, game.final_score_text, 25, 250, 275);
							draw_Text(renderer, font_texture, game.restart_text, 25, 275, 325);
							draw_Text(renderer, font_texture, game.gbt_main_menu, 25, 250, 375);
						}
						if (game.scatter_mode)
						{
							draw_Text_Box(renderer, 225, 125, 400, 350);
							draw_Text(renderer, font_texture, game.win_text, 50, 225, 125);
							draw_Text(renderer, font_texture, game.max_link_text, 25, 275, 200);
							draw_Text(renderer, font_texture, game.score_text, 25, 300, 225);
							draw_Text(renderer, font_texture, game.final_score_text, 25, 250, 250);
							draw_Text(renderer, font_texture, game.restart_text, 25, 275, 325);
							draw_Text(renderer, font_texture, game.gbt_main_menu, 25, 250, 375);
						}
					}
					if (game.vs_mode)
					{
						draw_Text_Box(renderer, 225, 125, 400, 250);
						if (game.p1vp2 || game.p1vcpu && player.snack_count == 25 || cpu1.died) draw_Text(renderer, font_texture, game.win_text, 50, 250, 125);
						if (cpu1.snack_count == 25 && player.snack_count != 25 || game.p1vcpu && player.died) draw_Text(renderer, font_texture, game.lose_text, 50, 225, 125);
						draw_Text(renderer, font_texture, game.restart_text, 25, 275, 225);
						draw_Text(renderer, font_texture, game.gbt_main_menu, 25, 250, 275);
					}
				}

				//death and tie menu text
				if (game.single_player && game.dead_player && !game.finished)
				{
					draw_Text_Box(renderer, 225, 125, 400, 175);
					draw_Text(renderer, font_texture, game.death_text, 50, 225, 125);
					draw_Text(renderer, font_texture, game.restart_text, 25, 275, 200);
					draw_Text(renderer, font_texture, game.gbt_main_menu, 25, 250, 250);
				}
				if (game.vs_mode && game.dead_player && !game.finished || player.snack_count == 25 && player2.snack_count == 25 || player.snack_count == 25 && cpu1.snack_count == 25)
				{
					draw_Text_Box(renderer, 225, 125, 400, 175);
					draw_Text(renderer, font_texture, game.tie_text, 50, 350, 125);
					draw_Text(renderer, font_texture, game.restart_text, 25, 275, 200);
					draw_Text(renderer, font_texture, game.gbt_main_menu, 25, 250, 250);
				}
			}
		}

		//updates screen
		SDL_RenderPresent(renderer);

		//caps frame rate
		frame_time = SDL_GetTicks() - frame_start;
		if (frame_delay > frame_time) SDL_Delay(frame_delay - frame_time);
	}
}