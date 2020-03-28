#pragma once
#include<Windows.h>
#include<time.h>
static DEVMODE devmode;
static DISPLAY_DEVICE displaydevice;
static int ScreenWidth, ScreenHeight;
static int mwWidth = 1024;//主窗口宽度
static int mwHeight = 768;//主窗口高度
static bool fullscreen = false;
//struct point
//{
//	int x;
//	int y;
//};
class RSystem
{
public:

	RSystem();
	bool Init(HINSTANCE hInstance, int nCmdShow);
	~RSystem();
	wchar_t ClassName[24] = L"RS";
	void Clean();
	void Draw();
	//point a();

	RECT mwRect;
	HDC hdc;
	HDC mdc;
	HDC bufdc;
	HBITMAP ball;
	HBITMAP bg;


	HWND mainWindow = NULL;
private:
	static HRESULT CALLBACK mwWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
void Changedisplay(int height, int width, bool fullscreen, DEVMODE devmode, DISPLAY_DEVICE displaydevice);

