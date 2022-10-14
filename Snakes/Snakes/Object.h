#pragma once

//a thing here...with stuff...
struct Object
{
	int w, h, x, y, img_source_x, img_source_y, img_source_w, img_source_h, sprite_sheet;
	unsigned int frame_update;

	Object(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void operator()(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	//hurtbox or if there's no sprite for the object
	void draw_(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect rect;
		rect.w = w;
		rect.h = h;
		rect.x = x;
		rect.y = y;
		SDL_RenderFillRect(renderer, &rect);
	}

	//void init_Source_Img(int x_src, int y_src, int w_src, int h_src, int n_frames)
	//{
	//	img_source_x = x_src;
	//	img_source_y = y_src;
	//	img_source_w = w_src;
	//	img_source_h = h_src;
	//	sprite_sheet = img_source_h * n_frames;
	//}

	//void draw_Img(SDL_Renderer *renderer, SDL_Texture *t, int x_src, int y_src, int w_src, int h_src)
	//{
	//	//source
	//	SDL_Rect src;
	//	src.x = x_src;
	//	src.y = y_src;
	//	src.w = w_src;
	//	src.h = h_src;
	//	//destination
	//	SDL_Rect dest;
	//	dest.x = x;
	//	dest.y = y;
	//	dest.w = src.w;
	//	dest.h = src.h;
	//	//draw image
	//	//copy from source texture to destination screen.
	//	SDL_RenderCopyEx(renderer, t, &src, &dest, 0, NULL, SDL_FLIP_NONE);
	//}

	//void animate_Func(unsigned int time, int num)
	//{
	//	if (time - frame_update > num)
	//	{
	//		frame_update = time;
	//		//update
	//		img_source_y += img_source_h; //or img_source_x += img_source_w;
	//	}
	//	//reset
	//	if (img_source_y >= sprite_sheet) img_source_y = 0; //or if (img_source_x >= sprite_sheet) img_source_x = 0;
	//}
};