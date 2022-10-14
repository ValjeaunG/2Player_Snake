#pragma once
#include "Object.h"
#include "Snake.h"

//misc functions here...

void draw_Text_Box(SDL_Renderer* renderer, int x, int y, int w, int h)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 100, 150); //dark blue

	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;

	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void draw_Text(SDL_Renderer* renderer, SDL_Texture* t, char* tex, int font_size, int dest_x, int dest_y)
{
	for (int i = 0; i < 16; i++)
	{
		//source
		SDL_Rect src;
		src.x = 64 * (tex[i] % 16);//row
		src.y = 64 * (tex[i] / 16);//column
		src.w = 64;
		src.h = 64;
		//destination
		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = font_size;
		dest.h = font_size;

		//draw image
		//copy from source texture to destination screen.
		//SDL_FLIP_XXX enumeration allows you to mirror the image
		SDL_RenderCopyEx(renderer, t, &src, &dest, 0, NULL, SDL_FLIP_NONE);

		//increment dest_x!
		dest_x += dest.w;
	}
}

void draw_Link_Bar(SDL_Renderer* renderer, int x, int y, int w, int h)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 150); //purple

	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;

	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void draw_Boundary_Line(SDL_Renderer* renderer, int x, int y, int w, int h)
{
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); //gray

	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;

	SDL_RenderFillRect(renderer, &rect);
}

void classic_Mode_Collision(Snake* s, Object* o, int index, int pos_x, int offset)
{
	if (s->head.x == o[index].x && s->head.y == o[index].y)
	{
		s->snack_eaten = true;
		o[index].x = (rand() % pos_x + offset) * 10;
		o[index].y = rand() % 60 * 10;
		std::for_each(s->body.begin(), s->body.end(), [&](auto& segment) {
			if (segment.x == o[index].x && segment.y == o[index].y)
			{
				o[index].x = (rand() % pos_x + offset) * 10;
				o[index].y = rand() % 60 * 10;
			}
			else if (segment.x != o[index].x && segment.y != o[index].y && s->snack_eaten)
			{
				s->length += 10;
				s->snack_count += 1;
				s->snack_eaten = false;
				///score is left out because score is irrelevent in vs mode
			}
		});
	}
}

void cm_Snack_Direction(CPU* c, Object* o, int index, int index2)
{
	if (c->head.x == o[index].x && c->head.y == o[index2].y)
	{
		c->dir_count++;
		if (c->dir_count == 1)
		{
			c->dir_change = rand() % 2;
			if (c->dir_change == 0 && o[index].y < c->head.y) c->dir = 0;
			else if (c->dir_change == 0 && o[index].y > c->head.y) c->dir = 2;
			else if (c->dir_change == 1 && o[index2].x < c->head.x) c->dir = 1;
			else if (c->dir_change == 1 && o[index2].x > c->head.x) c->dir = 3;
		}
	}
}

void scatter_Mode_Collision(Snake* s, Object* o, int num)
{
	for (int i = 0; i < num; i++)
	{
		if (s->head.x == o[i].x && s->head.y == o[i].y)
		{
			s->length += 10;
			s->snack_count += 1;
			o[i].x = -10;
			o[i].y = -10;
			///score is left out because score is irrelevent in vs mode
		}
	}
}

void snake_Collision(Snake* s1, Snake* s2)
{
	if (s1->head.x == s2->head.x && s1->head.y == s2->head.y)
	{
		if (s1->length > s2->length) s2->died = true;
		if (s1->length < s2->length) s1->died = true;
		if (s1->length == s2->length)
		{
			s1->died = true;
			s2->died = true;
		}
	}
}

//template<typename A, typename B>
//int collision_Detection(A* a, B* b)
//{
//	//1 = below, 2 = left, 3 = above, 4 = right
//
//	float width = 0.5f * (a->w + b->w);
//	float height = 0.5f * (a->h + b->h);
//	float dx = a->x - b->x + 0.5f * (a->w - b->w);
//	float dy = a->y - b->y + 0.5f * (a->h - b->h);
//
//	if (dx*dx <= width * width && dy*dy <= height * height)
//	{
//		float width_y = width * dy;
//		float height_x = height * dx;
//
//		if (width_y > height_x) return (width_y + height_x > 0) ? 3 : 4;
//		else return (width_y + height_x > 0) ? 2 : 1;
//	}
//
//	return 0;
//}
