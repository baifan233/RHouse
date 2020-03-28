#include "Object.h"
Object::Object(int type)
{
	this->rect = {0};
	this->type = type;
}
Object::~Object()
{

}
Object::Object()
{
	this->rect = { 0 };
	this->type = 0;
	this->X_Speed = 0;
	this->Y_Speed = 0;
	this->prect = { 0 };
}
void Object::InitRect(int X, int Y, int Width, int Height, int pX, int pY, int pWidth, int pHeight)
{
	this->rect.h = Height;
	this->rect.x = X;
	this->rect.w = Width;
	this->rect.y = Y;

	this->prect.h = pHeight;
	this->prect.x = pX;
	this->prect.w = pWidth;
	this->prect.y = pY;
}
void Object::InitRect(SDL_Rect Rect, SDL_Rect pRect)
{
	this->rect = Rect;
	this->prect = pRect;
}