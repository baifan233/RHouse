#include<Windows.h>
using namespace std;
PAINTSTRUCT ps;
HRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	//case WM_PAINT:
		//BeginPaint(hwnd, &ps);

		//EndPaint(hwnd, &ps);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);

	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrehInstance, PSTR nCmdLine, int nCmdShow)
{
	/*HWND wnd;
	wnd = GetForegroundWindow();
	SendMessage(wnd,WM_CREATE, 0, 0);*/

	WNDCLASSEX wnd = {0};
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = NULL;
	wnd.hCursor = LoadCursor(NULL,IDC_ARROW);
	wnd.hIcon = NULL;
	wnd.hIconSm = NULL;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = L"first CS";
	wnd.lpszMenuName = NULL;
	//wnd.cbClsExtra = 0;
	//wnd.cbSize = sizeof(WNDCLASSEX);
	//wnd.cbWndExtra = 0;
	//wnd.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wnd.hIcon = NULL/*(HICON)::LoadImage(NULL, L"doyougame.ico", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE)*/;
	//wnd.hInstance = hInstance;
	//wnd.lpszClassName = L"first CS";
	//wnd.lpszMenuName = NULL;
	wnd.style = CS_VREDRAW | CS_HREDRAW;
	//wnd.lpfnWndProc = WndProc;
	//wnd.hIconSm = NULL;



	//WNDCLASSEX wd = { 0 };
	if(!RegisterClassEx(&wnd))
	return 0;


	HWND hwnd = CreateWindow(L"first CS", L"", WS_OVERLAPPEDWINDOW, 0,0, 800, 600, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	//MoveWindow(hwnd,0,0,800,600,true);
	UpdateWindow(hwnd);

	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			Sleep(1);
	}
	UnregisterClass(L"first CS",hInstance);
	return 0;
}