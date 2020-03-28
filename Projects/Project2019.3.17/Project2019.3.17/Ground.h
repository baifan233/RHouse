#pragma once
#include"GSystem.h"
class Ground
{
	
	SDL_Texture* tex;
public:
	Ground();
	~Ground();
	SDL_Rect rect;
	void Draw(SDL_Renderer* ren);
	bool Init(string groundTexture, SDL_Renderer* ren, int X, int Y, int W, int H);
};

