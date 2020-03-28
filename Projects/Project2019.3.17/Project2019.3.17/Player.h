#pragma once
#include"GSystem.h"
#include"Object.h"
class Player:public Object
{
public:
	SDL_Texture* stand;
	SDL_Texture* move;
	SDL_Texture* attack;
	SDL_Texture* special;
	SDL_Texture* die;
	//每秒移动速度 （单位：像素）
	int state;
	int frame = 0;
	int lastDrawFrame;
	Player();	
	bool Load(SDL_Texture* tex, int LoadTo);
	void FramePlus(int state);
	void UnLoad();
	
	void Draw(SDL_Renderer* ren, int State, int DrawSize);
	void Move(int x, int y);
	~Player();
private:

};

