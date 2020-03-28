#include "Object.h"



Object::Object()
{
}
bool Object::Load(TextureSet textureset, int LoadTo)
{

	switch (LoadTo)
	{
	case TEXTURESET_ATTACK:
		attack = textureset;
		if (attack.texture != NULL)
			return true;
		break;
	case TEXTURESET_MOVE:
		move = textureset;
		if (move.texture != NULL)
			return true;
		break;
	case TEXTURESET_STAND:
		stand = textureset;
		if (stand.texture != NULL)
			return true;
		break;
	case TEXTURESET_SPECIAL:
		special = textureset;
		if (special.texture != NULL)
			return true;
		break;
	case TEXTURESET_DIE:
		die = textureset;
		if (die.texture != NULL)
			return true;
		break;
	default:
		break;
	}
	return false;
}
void Object::UnLoad()
{
	//SDL_DestroyTexture(texture);		
}

void Object::InitRect(SDL_Rect Rect)
{
	this->rect = Rect;
}
void Object::Draw(SDL_Renderer* ren, int state)
{
	switch (state)
	{
	case STATE_STAND_LEFT:
		SDL_RenderCopy(ren, stand.texture[frame], 0, &rect);
		break;
	case STATE_STAND_RIGHT:
		SDL_RenderCopyEx(ren, stand.texture[frame], 0, &rect, 180, 0, SDL_FLIP_VERTICAL);//SDL_FLIP_
		break;
	case STATE_MOVE_LEFT:
		SDL_RenderCopy(ren, move.texture[frame], 0, &rect);
		break;		
	case STATE_MOVE_RIGHT:
		SDL_RenderCopyEx(ren, move.texture[frame], 0, &rect, 180, 0, SDL_FLIP_VERTICAL);
		break;
	default:
		break;
	}
}
void Object::Move(int x, int y)
{
	rect.x = x;
	rect.y = y;
}
void Object::InitRect(int x, int y, int Width, int Height)
{
	this->rect.h = Height;
	this->rect.x = x;
	this->rect.w = Width;
	this->rect.y = y;
}
Object::~Object()
{
	__try {
		for (int i = 0; i < attack.size; i++)
		{
			SDL_DestroyTexture(attack.texture[i]);
		}

		for (int i = 0; i < move.size; i++)
		{
			SDL_DestroyTexture(move.texture[i]);
		}

		for (int i = 0; i < stand.size; i++)
		{
			SDL_DestroyTexture(stand.texture[i]);
		}

		for (int i = 0; i < special.size; i++)
		{
			SDL_DestroyTexture(special.texture[i]);
		}

		for (int i = 0; i < die.size; i++)
		{
			SDL_DestroyTexture(die.texture[i]);
		}
	}
	__except (1)
	{
		//如果清除纹理失败则不做处理
	}
}
