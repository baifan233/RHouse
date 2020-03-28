#include "RSystem.h"
//#include"Object.h"
//Object ob1;
RSystem::RSystem()
{
}
bool RSystem::Init(HINSTANCE hInstance, int nCmdShow)
{

	WNDCLASSEX wnd = { 0 };
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = NULL;
	wnd.hIconSm = NULL;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = mwWndProc;
	wnd.lpszClassName = ClassName;
	wnd.style = CS_VREDRAW | CS_HREDRAW;
	wnd.lpszMenuName = NULL;
	if (!RegisterClassEx(&wnd))
		return false;

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN); ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	if (fullscreen)
	{

		displaydevice.cb = sizeof(DISPLAY_DEVICE);
		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmDriverExtra = 0;
		EnumDisplayDevices(NULL, 0, &displaydevice, 0);
		EnumDisplaySettingsEx(displaydevice.DeviceName, ENUM_CURRENT_SETTINGS, &devmode, 0);
		devmode.dmPelsWidth = mwWidth;
		devmode.dmPelsHeight = mwHeight;

		devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;




		ChangeDisplaySettingsEx(displaydevice.DeviceName, &devmode, 0, CDS_FULLSCREEN, 0);
		mainWindow = CreateWindow
		(ClassName,
			ClassName,
			WS_POPUP,
			0,
			0
			, mwWidth, mwHeight, 0, 0
			, hInstance, 0);
	}
	else
	{
		mainWindow = CreateWindow
		(ClassName,
			ClassName,
			WS_OVERLAPPEDWINDOW,
			(ScreenWidth - mwWidth) / 2,
			(ScreenHeight - mwHeight) / 2
			, mwWidth, mwHeight, 0, 0
			, hInstance, 0);
	}
	if (mainWindow == NULL)return false;

	HBITMAP bmp;


	ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);




	/*if (DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(&devmode, CDS_FULLSCREEN))
		MessageBox(0,0,0,0);*/
	//HWND hw = GetDesktopWindow();
	hdc = GetDC(mainWindow);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	bmp = CreateCompatibleBitmap(hdc, mwWidth, mwHeight);
	SelectObject(mdc, bmp);
	ball = (HBITMAP)LoadImage(NULL, L"F:\\ball.bmp", IMAGE_BITMAP, 120, 60, LR_LOADFROMFILE);
	bg = (HBITMAP)LoadImage(NULL, L"F:\\bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//ob1.InitRect(0,0,60,60);
	//ob1.Load((HBITMAP)LoadImage(NULL, L"F:\\ball.bmp", IMAGE_BITMAP, 120, 60, LR_LOADFROMFILE));
	GetClientRect(mainWindow, &mwRect);
	return true;
}
void Changedisplay(int height, int width, bool Fullscreen, DEVMODE devmode, DISPLAY_DEVICE displaydevice)
{
	devmode.dmPelsHeight = height;
	devmode.dmPelsWidth = width;
	fullscreen = Fullscreen;
	if (fullscreen)
	{
		ChangeDisplaySettingsEx(displaydevice.DeviceName, &devmode, 0, CDS_FULLSCREEN, 0);
	}
	else
	{
		ChangeDisplaySettingsEx(displaydevice.DeviceName, &devmode, 0, 0, 0);
	}
}
HRESULT CALLBACK RSystem::mwWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static PAINTSTRUCT ps;
	switch (message)
	{
	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
	case WM_KILLFOCUS:
		Changedisplay(ScreenHeight,ScreenWidth,false,devmode,displaydevice);
		break;
	case WM_SETFOCUS:
		Changedisplay(mwHeight, mwWidth,true, devmode, displaydevice);
		break;
	case WM_KEYDOWN:
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);

	}
	return 0;
}
void RSystem::Clean()
{
	ReleaseDC(mainWindow, hdc);
	DeleteDC(hdc);	
	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(ball);
	DeleteObject(bg);
}
//point RSystem::a()
//{
//	iX += iXSpeed;
//	iYSpeed = iYSpeed + iYGravity;
//	iY += iYSpeed;
//	if (iY >= mwRect.bottom - 60)
//	{
//		iY = mwRect.bottom - 60;
//		iXSpeed += g_iXFriction;
//		if (iXSpeed < 0)
//			iXSpeed = 0;
//		iYSpeed += g_iYFriction;
//		if (iYSpeed < 0)
//			iYSpeed = 0;
//		iYSpeed = -iYSpeed;
//	}
//	if (iX >= mwRect.right - 60)
//	{
//		iXSpeed = -iXSpeed;
//	}
//	point a;
//	a.x = iX;
//	a.y = iY;
//	return a;
//}
void RSystem::Draw()
{
	
	//SelectObject(bufdc, bg);
	//BitBlt(mdc, 0, 0, mwWidth, mwHeight, bufdc, 0, 0, SRCCOPY);
	/*SelectObject(bufdc, ball);
	BitBlt(mdc, iX, iY, 60, 60, bufdc, 60, 0, SRCAND);
	BitBlt(mdc, iX, iY, 60, 60, bufdc, 0, 0, SRCPAINT);*/

	/*ob1.Move(iX,iY);
	ob1.Draw(bufdc,mdc,0,SRCAND,60,0);
	ob1.Draw(bufdc,mdc,0,SRCPAINT,0,0);*/
	//BitBlt(hdc, 0, 0, mwWidth, mwHeight, mdc, 0, 0, SRCCOPY);
}
RSystem::~RSystem()
{
}

void Changedisplay()
{
}
