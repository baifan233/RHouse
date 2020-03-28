#pragma once
#include"Rdefines.h"
class RParticles
{
	D2D1_RECT_F* rect = {0};
	
	int* life = {0};
	int i=0;
	int screenWidth=0;
	int screenHeight=0;
	ID2D1Bitmap* balld2d=0;
	POINT mousePoint = {500,500};
public:

	bool Init(int num, int size, int screenwidth, int screenheight, ID2D1Bitmap* tex2);
	int* Vx = { 0 };
	int* Vy = { 0 };
	void Draw(ID2D1RenderTarget* ren, int num);
	void UpdateSpeed(int Vx, int Vy, bool useRand);
	
	void Draw2(ID2D1RenderTarget* ren, int num, bool Life);
	int* speedOffset=0;
	~RParticles();
};

