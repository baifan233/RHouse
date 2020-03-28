#include "Wall.h"
bool Wall::Load(string path, SDL_Renderer* ren,SDL_Rect sdl_Rect)
{
	this->texture = IMG_LoadTexture(ren, path.c_str());
	if (texture == NULL)
		return false;
	this->rect = sdl_Rect;
	return true;
}
void Wall::Draw(SDL_Renderer* ren)
{
	SDL_RenderCopy(ren,this->texture,0,&this->rect);
}