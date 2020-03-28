#pragma once
#define DIRECTINPUT_VERSION 0x800
#include<Windows.h>
#include<time.h>
#include<iostream>
#include<dinput.h>
#include"SDL.h"
#include"SDL_image.h"
#include<SDL_ttf.h>
#include<string>
#pragma comment(lib,"sdl2.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"sdl2_image.lib")
#pragma comment(lib,"sdl2_ttf.lib")
using namespace std;
//#pragma comment(lib,"dxerr8.lib")
static int x, y;
static DEVMODE devmode;
static DISPLAY_DEVICE displaydevice;
static int ScreenWidth, ScreenHeight;
static int mwWidth = 1024;//主窗口宽度
static int mwHeight = 768;//主窗口高度
static bool fullscreen = false;

static SDL_Renderer *renderer;
//static char cc;
static LPDIRECTINPUT8 dinputDevice;
static LPDIRECTINPUTDEVICE8 g_KeyBoard;

static int PlayerState=0;
static bool quit = false;
static DIDEVICEOBJECTDATA didod[8];
void DealKeyMessage();
bool InitGame();
struct point
{
	int x;
	int y;
};
class RSystem
{
public:
	SDL_Window *sdl_window;
	RSystem();
	bool Init(HINSTANCE hInstance, int nCmdShow);
	~RSystem();
	wchar_t ClassName[24] = L"Game";
	void Clean();
	void Draw(int frame);
	point a();
	
	RECT mwRect;
	//FILE* file;
	
	HANDLE KeyBoradThread=NULL;
	
	HWND mainWindow = NULL;
private:
	static DWORD WINAPI keyBoardThread(LPVOID lpParamter);
	static HRESULT CALLBACK mwWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
void Changedisplay(int height, int width, bool fullscreen, DEVMODE devmode, DISPLAY_DEVICE displaydevice);
#define KEY_Q 16
#define KEY_W 17
#define KEY_E 18
#define KEY_R 19
#define KEY_T 20
#define KEY_Y 21
#define KEY_U 22
#define KEY_I 23
#define KEY_O 24
#define KEY_P 25
#define KEY_A 30
#define KEY_S 31
#define KEY_D 32
#define KEY_F 33
#define KEY_G 34
#define KEY_H 35
#define KEY_J 36
#define KEY_K 37
#define KEY_L 38
#define KEY_Z 44
#define KEY_X 45
#define KEY_C 46
#define KEY_V 47
#define KEY_B 48
#define KEY_N 49
#define KEY_M 50
#define G_KEY_DOWN 128
#define G_KEY_UP 0