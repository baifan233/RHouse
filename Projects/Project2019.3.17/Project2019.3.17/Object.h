#pragma once
#include"GSystem.h"
struct TextureSet
{
	SDL_Texture* texture[16];
	int size;
};
#define TEXTURESET_STAND 103
#define TEXTURESET_MOVE 104
#define TEXTURESET_ATTACK 105
#define TEXTURESET_SPECIAL 106
#define TEXTURESET_DIE 107

#define STATE_STAND_LEFT 201
#define STATE_STAND_RIGHT 202
#define STATE_MOVE_LEFT 203
#define STATE_MOVE_RIGHT 204
#define STATE_ATTACK_LEFT 205
#define STATE_ATTACK_RIGHT 206
#define STATE_SPECIAL_LEFT 207
#define STATE_SPECIAL_RIGHT 208
#define STATE_DIE_LEFT 208
#define STATE_DIE_RIGHT 208

#define OBJECT_TYPE_PLAYER 1
#define OBJECT_TYPE_WALL 2
#define OBJECT_TYPE_GROUND 3
#define OBJECT_TYPE_PARTICLES 3
class Object
{
protected:
	Object();
public:
	SDL_Rect rect;
	int type;
	void InitRect(SDL_Rect rect, SDL_Rect pRect);
	void InitRect(int X, int Y, int Width, int Height, int pX, int pY, int pWidth, int pHeight);
	SDL_Rect prect;
	int X_Speed;
	int Y_Speed;
	Object(int type);
	~Object();
};

