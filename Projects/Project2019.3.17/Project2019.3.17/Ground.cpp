#include "Ground.h"
bool Ground::Init(string groundTexture,SDL_Renderer* ren,int X,int Y,int W,int H)
{
	tex = IMG_LoadTexture(ren,groundTexture.c_str());
	if (tex == NULL)
		return false;
	this->rect.h = H;
	this->rect.y = Y;
	this->rect.x = X;
	this->rect.w = W;
	return true;
}
void Ground::Draw(SDL_Renderer* ren)
{
	SDL_RenderCopy(ren,tex,0,&rect);
}
Ground::Ground()
{
	this->rect = {0};
	this->tex = 0;
}
Ground::~Ground()
{
	if (this->tex)SDL_DestroyTexture(this->tex);
}