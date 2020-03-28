#pragma once
#include"GSystem.h"
class Wall
{
	
	SDL_Texture* texture;
public:
	bool Load(string path, SDL_Renderer* ren, SDL_Rect sdl_Rect);
	void Draw(SDL_Renderer* ren);
	SDL_Rect rect;
};

