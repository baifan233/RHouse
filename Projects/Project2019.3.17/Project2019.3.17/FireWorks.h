#pragma once
#include"GSystem.h"
struct Particle
{
	int x;
	int y;
	int x_Speed;
	int y_Speed;
	int life;
};
class FireWorks
{
private:
	Particle* particles;
	int length;
	int r;
	int x; int y;
public:
	void ReSet();
	void explode(SDL_Renderer* ren,SDL_Texture* point);
	bool Init(int X, int Y, int R, int particlesNum);
	~FireWorks();
	FireWorks();
	
};

