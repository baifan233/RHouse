#include<windows.h>
#include<string>
#include"RSystem.h"
using namespace std;
#include<sdl.h>
#pragma comment(lib,"sdl2.lib")
RSystem rs;
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE PrehInstance,PSTR nCmdLine,int nCmdShow)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {

		return 1;
	}
	rs.Init(hInstance,nCmdShow);
	SDL_Window *win = SDL_CreateWindowFrom(rs.mainWindow);
//	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	//Make sure creating our window went ok
	if (win == nullptr) {
	
		return 1;
	}
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);

		SDL_Quit();
		return 1;
	}

	//SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
	//this lets us choose when to upload or remove textures from the GPU
	SDL_Surface *bmp = SDL_LoadBMP("F:\\bg.bmp");
	SDL_Surface* ball = SDL_LoadBMP("F:\\ball2.bmp");	
	SDL_Surface* stand[8];
	char temp[20];
	Uint32 colorkey = SDL_MapRGB(ball->format, 0, 0, 0);//把0,0,0的颜色设置成透明 
	
	for (int i = 1; i < 9; i++)
	{
		sprintf_s(temp,"F:\\stand\\%dm.bmp",i);
		stand[i-1]=SDL_LoadBMP(temp); 
		SDL_SetColorKey(stand[i-1], -1, colorkey);  //设置关键色
	}
	SDL_SetColorKey(ball,-1,colorkey);

	if (bmp == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);

		SDL_Quit();
		return 1;
	}

	//To use a hardware accelerated texture for rendering we can create one from
	//the surface we loaded
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_Texture *balltex = SDL_CreateTextureFromSurface(ren,ball);
	SDL_Texture* standtex[8] = {0};
	for (int i = 0; i < 8; i++)
	{
		standtex[i]=SDL_CreateTextureFromSurface(ren,stand[i]);
	}
	//We no longer need the surface
	SDL_FreeSurface(bmp);
	SDL_FreeSurface(ball);
	for (int i = 0; i < 8; i++)
	{
		SDL_FreeSurface(stand[i]);
	}
	if (tex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		
		SDL_Quit();
		return 1;
	}

	//A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	for (int i = 0; i < 3; ++i) {
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		//SDL_Delay(1000);
	}
	
	//Clean up our objects and quit
	
	MSG msg = {0};
	SDL_Rect sdlrect;
	sdlrect.x = 0;
	sdlrect.y = 0;
	sdlrect.h = 160;
	sdlrect.w = 160;
	SDL_Rect ballrect = {0,0,60,60};
	int i = 0;
	int bx=0, by=0, iXSpeed=2, iYSpeed = 0;
	int Gravity = 2;
	point i2;
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*bx += iXSpeed;
			iYSpeed = iYSpeed + Gravity;
			by += iYSpeed;	
			if (by >= 768 - 60)
			{
				by = 768 - 60;
				iYSpeed = -iYSpeed;
			}
			if (bx >= 1024 - 60)
			{
				bx = 1024 - 60;
				iXSpeed=-iXSpeed;
			}*/
			i2= rs.a();
			ballrect.x = i2.x;
			ballrect.y = i2.y;
			SDL_RenderClear(ren);
			SDL_RenderCopy(ren,tex,NULL,NULL);
			SDL_RenderCopy(ren,balltex,NULL,&ballrect);
			if (i > 7)
				i = 0;				
			SDL_RenderCopy(ren,standtex[i],NULL,&sdlrect);
			i++;
			SDL_RenderPresent(ren);
			Sleep(15);
		}
	}
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(balltex);
	for (int i = 0; i < 8; i++)
	{
		SDL_DestroyTexture(standtex[i]);
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();	
	return 0;
}