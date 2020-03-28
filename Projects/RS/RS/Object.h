#pragma once
#include"GSystem.h"
struct TextureSet
{
	SDL_Texture* texture[16];
	int size = 16;
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


class Object
{
public:
	TextureSet stand;
	TextureSet move;
	TextureSet attack;
	TextureSet special;
	TextureSet die;
	void Frameplus(int maxFrame) { if (frame < maxFrame-1) frame++; else frame = 0; }
	int state;
	
	Object();
	bool Load(TextureSet textureset,int LoadTo);
	//bool Load(SDL_Surface * Surface, SDL_Renderer * renderer,bool SetKeyColor,int R,int G,int B);	
	void UnLoad();
	void InitRect(SDL_Rect rect);
	void InitRect(int x, int y, int width, int height);
	SDL_Rect rect;
	void Draw(SDL_Renderer* ren,int state);
	void Move(int x, int y);
	~Object();
private:
	int frame = 0;
};

