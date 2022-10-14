#pragma once
#include <deque>
#include <algorithm>

struct Snake : public Object
{
	//float xvel, yvel;
	int dir, snack_count;
	unsigned int length;
	bool died, snack_eaten;
	SDL_Rect head;
	std::deque<SDL_Rect> body;

	Snake(int x, int y, int w, int h, int dir) : Object(x, y, w, h)
	{
		head.w = w;
		head.h = h;
		head.x = x;
		head.y = y;
		this->dir = dir;
		length = 1;
		snack_count = 0;
		died = false;
	}

	void operator()(int x, int y, int w, int h, int dir)
	{
		head.w = w;
		head.h = h;
		head.x = x;
		head.y = y;
		this->dir = dir;
		length = 1;
		snack_count = 0;
		died = false;
	}

	void draw_Snake(SDL_Renderer* renderer, int r, int g, int b)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		std::for_each(body.begin(), body.end(), [&](auto& segment) {
			SDL_RenderFillRect(renderer, &segment);
		});
	}

	void movement_()
	{
		switch (dir)
		{
		case 0:
			head.y -= 10;
			break;
		case 1:
			head.x -= 10;
			break;
		case 2:
			head.y += 10;
			break;
		case 3:
			head.x += 10;
			break;
		}
	}

	//void move_Controls(const Uint8 input1, const Uint8 input2, const Uint8 input3, const Uint8 input4, int bound1, int bound2, int bound3, int bound4)
	//{
	//	//if not at edges of screen
	//	if (input1) xvel = -7.f;
	//	else if (input2) xvel = 7.f;
	//	if (input3) yvel = -7.f;
	//	else if (input4) yvel = 7.f;
	//	if (input1 || input2) x += xvel;
	//	if (input3 || input4) y += yvel;
	//	//if at edges of screen
	//	if (x <= bound1) x = 0;
	//	else if (x >= bound2 - w) x = bound2 - w;
	//	if (y <= bound3) y = 0;
	//	else if (y >= bound4 - h) y = bound4 - h;
	//}
};

struct CPU : public Snake
{
	int dir_count, dir_change, stuck_count;
	bool wandering, pursuit, prioritize_wandering;

	CPU(int x, int y, int w, int h, int dir) : Snake(x, y, w, h, dir)
	{
		dir_count = 0;
		dir_change = 1;
		stuck_count = 0;
		wandering = true;
		pursuit = false;
		prioritize_wandering = false;
	}

	void operator()(int x, int y, int w, int h, int dir)
	{
		head.w = w;
		head.h = h;
		head.x = x;
		head.y = y;
		this->dir = dir;
		length = 1;
		snack_count = 0;
		dir_count = 0;
		dir_change = 1;
		stuck_count = 0;
		died = false;
		wandering = true;
		pursuit = false;
		prioritize_wandering = false;
	}

	void direction_(int dir)
	{
		if (dir_change == dir)
		{
			this->dir = dir;
			dir_count = 0;
			prioritize_wandering = false;
		}
	}

	void body_Collision(int dir, int dir2, int dir3, int dir4)
	{
		if (this->dir == dir || this->dir == dir3)
		{
			if (this->dir == dir) head.y += 10;
			else if (this->dir == dir3) head.y -= 10;
			dir_change = rand() % 2;
			if (dir_change == 0) this->dir = dir2;
			else if (dir_change == 1) this->dir = dir4;
			prioritize_wandering = true;
		}
		else if (this->dir == dir2 || this->dir == dir4)
		{
			if (this->dir == dir2) head.x += 10;
			else if (this->dir == dir4) head.x -= 10;
			dir_change = rand() % 2;
			if (dir_change == 0) this->dir = dir;
			else if (dir_change == 1) this->dir = dir3;
			prioritize_wandering = true;
		}
	}
};