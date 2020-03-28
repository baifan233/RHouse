#include "FireWorks.h"
FireWorks::FireWorks()
{
	this->length = 0;
	this->particles = 0;
	this->r = 0;
	this->x = 0;
	this->y = 0;
}
void FireWorks::ReSet()
{
	int temp;
	for (int i = 0; i < length; i++)
	{
		particles[i].x = x;
		particles[i].y = y;
		temp = rand() % 8;
		if (temp == 0)
		{
			particles[i].x_Speed = -(rand() % r) + 1;
			particles[i].y_Speed = -(rand() % r) - 2;
		}
		else if (temp == 1)
		{
			particles[i].x_Speed = -(rand() % r) - 2;
			particles[i].y_Speed = -(rand() % r) + 1;
		}
		else if (temp == 2)
		{
			particles[i].x_Speed = -(rand() % r) - 2;
			particles[i].y_Speed = (rand() % r) + 1;
		}
		else if (temp == 3)
		{
			particles[i].x_Speed = -(rand() % r) + 1;
			particles[i].y_Speed = (rand() % r) - 2;
		}
		else if (temp == 4)
		{
			particles[i].x_Speed = (rand() % r) + 2;
			particles[i].y_Speed = (rand() % r) - 1;
		}
		else if (temp == 5)
		{
			particles[i].x_Speed = (rand() % r) - 1;
			particles[i].y_Speed = (rand() % r) + 2;
		}
		else if (temp == 6)
		{
			particles[i].x_Speed = (rand() % r) + 2;
			particles[i].y_Speed = -(rand() % r) + 1;
		}
		else if (temp == 7)
		{
			particles[i].x_Speed = (rand() % r) - 1;
			particles[i].y_Speed = -(rand() % r) - 2;
		}
		temp = rand() % 3;
		if (temp == 0)
			temp = 60;
		else if (temp == 1)
			temp = 50;
		particles[i].life = temp;
	}
}
void FireWorks::explode(SDL_Renderer* ren, SDL_Texture* point)
{
	SDL_Rect rect;
	rect.h = 15;
	rect.w = 15;
	SDL_SetTextureColorMod(point, 57, 229, 0);
	for (int i = 0; i < length; i++)
	{
		/*particles[i].x_Speed += -particles[i].x_Speed;
		particles[i].y_Speed += -particles[i].y_Speed+2;*/
		if (particles[i].life < 15)
		{
			particles[i].y_Speed++;
		}
		else
		{
			if (particles[i].y >= 760)
			{
				particles[i].y = 760;
				particles[i].y_Speed -= 10;
				if (particles[i].y_Speed < 0)
				{
					particles[i].y_Speed = 0;
				}
				particles[i].y_Speed = -particles[i].y_Speed;
			}
			if (particles[i].x >= 1024)
			{
				particles[i].x_Speed -= 1;
				if (particles[i].x_Speed < 0)
				{
					particles[i].x_Speed = 0;
				}
				particles[i].x_Speed = -particles[i].x_Speed;
			}
			else if (particles[i].x <= 0)
			{
				particles[i].x_Speed -= 1;
				if (particles[i].x_Speed < 0)
				{
					particles[i].x_Speed = 0;
				}
				particles[i].x_Speed = -particles[i].x_Speed;				
			}
			
			particles[i].x += particles[i].x_Speed;
			particles[i].y += particles[i].y_Speed;
			rect.x = particles[i].x;
			rect.y = particles[i].y;
			SDL_RenderCopy(ren, point, NULL, &rect);
			particles[i].life--;

		}


		if (true/*particles[i].life>0*/)
		{


			if (particles[i].y >= 760)
			{
				particles[i].y = 760;
				particles[i].y_Speed -= 10;
				if (particles[i].y_Speed < 0)
				{
					particles[i].y_Speed = 0;
				}
				particles[i].x_Speed -= 1;
				if (particles[i].x_Speed < 0)
				{
					particles[i].x_Speed = 0;
				}
				particles[i].y_Speed = -particles[i].y_Speed;
			}
			if (particles[i].x >= 1024)
			{
				particles[i].x_Speed -= 3;
				if (particles[i].x_Speed < 0)
				{
					particles[i].x_Speed = 0;
				}
				particles[i].x_Speed = -particles[i].x_Speed;
			}
			else if (particles[i].x <= 0)
			{
				particles[i].x_Speed -= 3;
				if (particles[i].x_Speed < 0)
				{
					particles[i].x_Speed = 0;
				}
				particles[i].x_Speed = -particles[i].x_Speed;
			}
			particles[i].x += particles[i].x_Speed;
			particles[i].y += particles[i].y_Speed;
			rect.x = particles[i].x;
			rect.y = particles[i].y;
			SDL_RenderCopy(ren, point, NULL, &rect);
		}
		//SDL_SetRenderTarget(ren, NULL);
		//SDL_RenderCopy(ren, tex, NULL, NULL);
		particles[i].life--;
	}
}
bool FireWorks::Init(int X, int Y, int R, int particlesNum)
{
	this->particles = new Particle[particlesNum];
	if (particles == NULL)
		return false;
	length = particlesNum;
	int temp;
	this->r = R;
	this->y = Y;
	this->x = X;
	for (int i = 0; i < length; i++)
	{
		particles[i].x = x;
		particles[i].y = y;
		temp = rand() % 8;
		if (temp == 0)
		{
			particles[i].x_Speed = -(rand() % r) + 1;
			particles[i].y_Speed = -(rand() % r) - 2;
		}
		else if (temp == 1)
		{
			particles[i].x_Speed = -(rand() % r) - 2;
			particles[i].y_Speed = -(rand() % r) + 1;
		}
		else if (temp == 2)
		{
			particles[i].x_Speed = -(rand() % r) - 2;
			particles[i].y_Speed = (rand() % r) + 1;
		}
		else if (temp == 3)
		{
			particles[i].x_Speed = -(rand() % r) + 1;
			particles[i].y_Speed = (rand() % r) - 2;
		}
		else if (temp == 4)
		{
			particles[i].x_Speed = (rand() % r) + 2;
			particles[i].y_Speed = (rand() % r) - 1;
		}
		else if (temp == 5)
		{
			particles[i].x_Speed = (rand() % r) - 1;
			particles[i].y_Speed = (rand() % r) + 2;
		}
		else if (temp == 6)
		{
			particles[i].x_Speed = (rand() % r) + 2;
			particles[i].y_Speed = -(rand() % r) + 1;
		}
		else if (temp == 7)
		{
			particles[i].x_Speed = (rand() % r) - 1;
			particles[i].y_Speed = -(rand() % r) - 2;
		}
		temp = rand() % 3;
		if (temp == 0)
			temp = 60;
		else if (temp == 1)
			temp = 50;
		particles[i].life = temp;
	}

	return true;
}
FireWorks::~FireWorks()
{
	if (particles) delete[] particles;
	particles = 0;
}