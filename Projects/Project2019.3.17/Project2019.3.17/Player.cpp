#include "Player.h"



Player::Player()
{
	this->attack = 0;
	this->die = 0;
	this->stand = 0;
	this->move = 0;
	this->lastDrawFrame = 0;
	this->special = 0;
	this->state = 0;
	this->X_Speed = 0;
	this->Y_Speed = 0;
	this->type = OBJECT_TYPE_PLAYER;
}
void Player::UnLoad()
{
	//SDL_DestroyTexture(texture);		
}
SDL_Rect SelectFrame(int frame, int splitW, int splitH, int w, int h,int addX,int addY)
{
	SDL_Rect rect;
	rect.x = splitW * frame+addX;
	rect.y = splitH * frame+addY;
	rect.w = w;
	rect.h = h;
	return rect;
}
void Player::Draw(SDL_Renderer* ren, int State, int DrawSize)
{
	SDL_Rect srect = {0};
	switch (State)
	{
	case STATE_STAND_LEFT:
		srect = SelectFrame(frame, DrawSize, 0, 42, 55,12,4);
		SDL_RenderCopy(ren, stand,&srect, &rect);
		break;
	case STATE_STAND_RIGHT:
		srect = SelectFrame(frame, DrawSize, 0, 42, 55, 12, 4);
		SDL_RenderCopyEx(ren, stand,&srect, &rect, 180, 0, SDL_FLIP_VERTICAL);//SDL_FLIP_
		break;
	case STATE_MOVE_LEFT:
		srect = SelectFrame(frame, DrawSize, 0, 42, 55, 12, 4);
		SDL_RenderCopy(ren, stand, &srect, &rect);
		break;
	case STATE_MOVE_RIGHT: 53,
		srect = SelectFrame(frame, DrawSize, 0, 42, 55,12, 4);
		SDL_RenderCopyEx(ren, move, &srect, &rect, 180, 0, SDL_FLIP_VERTICAL);
		break;
	default:
		break;
	}
}
void Player::FramePlus(int State)
{
	switch (State)
	{
	case STATE_STAND_LEFT:
		frame < 8 - 1 ? frame++ : frame = 0;
		break;
	case STATE_STAND_RIGHT:
		frame < 8 - 1 ? frame++ : frame = 0;
		break;
	case STATE_MOVE_LEFT:
		frame < 8 - 1 ? frame++ : frame = 0;
		break;
	case STATE_MOVE_RIGHT:
		frame < 8 - 1 ? frame++ : frame = 0;
		break;
	default:
		break;
	}
}
bool Player::Load(SDL_Texture* tex, int LoadTo)
{
	if (tex == NULL)
		return false;
	switch (LoadTo)
	{
	case TEXTURESET_STAND:
		this->stand = tex;
		if (stand == NULL)
			return false;
		break;
	default:
		break;
	}
	return true;
}
void Player::Move(int X, int Y)
{
	rect.x = X;
	rect.y = Y;
}

Player::~Player()
{
	//__try {

	//	

	//}
	//__except (1)
	//{
	//	//如果清除纹理失败则不做处理
	//}
	if(attack)SDL_DestroyTexture(attack);
	if (move)SDL_DestroyTexture(move);
	if (stand)SDL_DestroyTexture(stand);
	if (special)SDL_DestroyTexture(special);
	if (die)SDL_DestroyTexture(die);
}
