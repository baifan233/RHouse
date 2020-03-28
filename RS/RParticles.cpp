#include "RParticles.h"



bool RParticles::Init(int num, int maxsize, int screenwidth, int screenheight, ID2D1Bitmap* tex2)
{
	balld2d = tex2;
	if (!tex2)return false;
	this->screenHeight = screenheight;
	this->screenWidth = screenwidth;
	i = num;
	rect = new D2D1_RECT_F[i]{ 0 };
	Vx = new int[i] {0};
	Vy = new int[i] {0};
	life = new int[i] {0};
	int temp = 0;
	int sizetemp = 0;
	if (rect != 0 && Vx != 0 && Vy != 0)
	{
		srand((UINT)GetTickCount64());
		sizetemp = rand() % maxsize + 1;
		for (int j = 0; j < i; ++j)
		{

			life[j] = rand() % 500;
			Vx[j] = rand() % 8;
			Vy[j] = rand() % 8;

			Vx[j] == 0 ? Vx[j] = 2 : Vx[j];
			Vy[j] == 0 ? Vy[j] = 2 : Vy[j];

			if (temp == 20)
			{
				sizetemp = rand() % maxsize + 1;
				if (sizetemp <= 8)
					sizetemp = 12;
				temp = 0;
			}
			else
				++temp;

			rect[j].left = rand() % screenWidth;
			rect[j].top = rand() % screenHeight;

			rect[j].right = sizetemp + rect[j].left;
			rect[j].bottom = sizetemp + rect[j].top;


		}
	}

	else return false;

	return false;
}
void RParticles::UpdateSpeed(int Vx, int Vy, bool useRand)
{
	if (useRand)
	{
		srand((UINT)GetTickCount64());
		for (int j = 0; j < i; ++j)
		{
			this->Vx[j] = rand() % Vx;
			this->Vy[j] = rand() % Vy;
		}
	}
	else

		for (int j = 0; j < i; ++j)
		{
			this->Vx[j] = Vx;
			this->Vy[j] = Vy;
		}
}

void RParticles::Draw(ID2D1RenderTarget* ren, int num)
{
	float width = 0;
	for (int j = 0; j < num; ++j)
	{

		width = rect[j].right - rect[j].left;
		if (rect[j].left + Vx[j] + width >= screenWidth)
		{
			rect[j].left = screenWidth - width;
			rect[j].right = rect[j].left + width;
			Vx[j] = -Vx[j];
		}
		else if (rect[j].left + Vx[j] <= 0)
		{
			rect[j].left = 0;
			rect[j].right = rect[j].left + width;
			Vx[j] = -Vx[j];
		}
		else
		{
			rect[j].left = rect[j].left + Vx[j]+ *speedOffset;
			rect[j].right = rect[j].left + width;
		}

		if (rect[j].top + width + Vy[j] >= screenHeight)
		{
			rect[j].top = screenHeight - width;
			rect[j].bottom = rect[j].top + width;
			Vy[j] = -Vy[j];
		}
		else if (rect[j].top + Vy[j] <= 0)
		{
			rect[j].top = 0;
			rect[j].bottom = rect[j].top + width;
			Vy[j] = -Vy[j];
		}
		else
		{
			rect[j].top = rect[j].top + Vy[j]+*speedOffset;
			rect[j].bottom = rect[j].top + width;
		}


		ren->DrawBitmap(balld2d, &rect[j]);
	}
}

void RParticles::Draw2(ID2D1RenderTarget* ren, int num, bool Life)
{
	GetCursorPos(&mousePoint);
	int width = 0;
	int halfwidth = 0;
	int sizetemp = 0;
	srand((UINT)GetTickCount64());
	for (int j = 0; j < num; ++j)
	{		
		if (life[j] > 0)
		{
			width = rect[j].right - rect[j].left;
			halfwidth = width / 2;

			if (rect[j].left + halfwidth > mousePoint.x)
			{
				if (Vx[j] > 0)
				{
					rect[j].left -= Vx[j]- *speedOffset;
				}
				else
					rect[j].left += Vx[j]+ *speedOffset;
				rect[j].right = rect[j].left + width;
			}
			else if (rect[j].left + halfwidth < mousePoint.x)
			{
				if (Vx[j] > 0)
					rect[j].left += Vx[j]+ *speedOffset;
				else
					rect[j].left -= Vx[j]- *speedOffset;
				rect[j].right = rect[j].left + width;
			}
			else if (rect[j].left + halfwidth == mousePoint.x)
			{
				//Vx[j] = 0;
			}
			

			if (rect[j].top + halfwidth > mousePoint.y)
			{
				if (Vy[j] > 0)
					rect[j].top -= Vy[j]- *speedOffset;
				else
					rect[j].top += Vy[j]+ *speedOffset;
				rect[j].bottom = rect[j].top + width;
			}
			else if (rect[j].top + halfwidth < mousePoint.y)
			{
				if (Vy[j] > 0)
					rect[j].top += Vy[j]+ *speedOffset;
				else
					rect[j].top -= Vy[j]- *speedOffset;
				rect[j].bottom = rect[j].top + width;
			}
			else if (rect[j].top + halfwidth == mousePoint.y)
			{
				//Vy[j] = 0;
			}
			if(rect[j].top + halfwidth == mousePoint.y&& rect[j].left + halfwidth == mousePoint.x)
			{
				Vy[j] = 0;
				Vx[j] = 0;
				life[j] = 10000;
			}
			life[j]--;
			ren->DrawBitmap(balld2d, &rect[j]);
		}
		else
		{

			life[j] = rand() % 500 + 200;
			Vx[j] = rand() % 8;
			Vy[j] = rand() % 8;

			Vx[j] == 0 ? Vx[j] = 2 : Vx[j];
			Vy[j] == 0 ? Vy[j] = 2 : Vy[j];
			sizetemp = rand() % 100;
			rect[j].left = rand() % screenWidth;
			rect[j].top = rand() % screenHeight;

			rect[j].right = sizetemp + rect[j].left;
			rect[j].bottom = sizetemp + rect[j].top;
		}
	}
}
RParticles::~RParticles()
{
	if (life)delete[] life;
	if (rect)delete[] rect;
	if (Vx)delete[] Vx;
	if (Vy)delete[] Vy;

	//if (balld2d)balld2d->Release();
}