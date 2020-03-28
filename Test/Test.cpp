#include<Windows.h>
#include<string>
#include<any>
#include<d2d1.h>
#include"RTimer.h"
#pragma comment(lib,"d2d1.lib")
using namespace std;
HWND mainWindow = 0;
int x = 0, y = 0;
HRESULT CALLBACK RWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_KEYDOWN:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
bool InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wnd = { 0 };
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(wnd);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = NULL;
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = 0;
	wnd.hIconSm = 0;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = RWndProc;
	wnd.lpszClassName = L"RPhysicalTest";
	wnd.lpszMenuName = 0;
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	if (!RegisterClassEx(&wnd))
		return false;
	mainWindow = CreateWindow(L"RPhysicalTest", L"", WS_OVERLAPPEDWINDOW,
		0, 0, 1024, 768, 0, 0, hInstance, 0);
	if (NULL != mainWindow)
		return true;

	return false;
}
#define SafeRelease(X){if(X){X->Release();X=NULL;}}
ID2D1Factory* factory = NULL;
ID2D1HwndRenderTarget* hwndRender = NULL;
RTimer rt1;
RTimer_t* rtt = 0;
thread mt;
thread mt2;

bool InitD2D()
{
	//rtt = rt1.CreateTimer(0, 45 * 60 * 1000, -1);
	//rt1.Start();

	//mt = thread(test);
	//mt2 = thread(test2);
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory)))
		return false;
	if (FAILED(factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(mainWindow,
			D2D1::SizeU(1024, 768)), &hwndRender)))
		return false;

	return true;
}
class Object
{

};
bool brun = false;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 
int speed = 1;

void Render()
{
	if (brun && speed == 1)
	{
		Sleep(1);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		
	}
	else if(brun&&speed==100)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		
		
		Sleep(1);
	}
	if (KEY_DOWN(VK_INSERT))
	{
		brun = !brun;
		Sleep(500);
	}
	else if (KEY_DOWN(VK_HOME))	
	{
		speed = 100;
	}
	else if (KEY_DOWN(VK_END))
	{
		speed = 1;
	}
	if (!brun)
	{
		Sleep(1);
		
		hwndRender->BeginDraw();
	
		hwndRender->EndDraw();
	}

}
void Clean()
{
	//mt.detach();
	//mt2.detach();
	SafeRelease(factory);
	SafeRelease(hwndRender);
}

//现在的WinMain函数是wWinMain然后 第三个的参数的类型是LPWSTR  每一个参数前面都是_In_
//但是第二个的前缀是_In_opt_

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE PrehIntance
	, _In_ LPWSTR nCmdLine, _In_ int nCmdShow)
{
	any as = 123;  any as2 = 123;
	any* a1=&as,*a2=&as2;
	
	*a1 = 200;
	*a2 = 123;
	string ty = a1->type().name();
	if(ty.compare("int")==0)
	int temp = any_cast<int>(*a1)+any_cast<int>(*a2);

	if (!InitWindow(hInstance))
		return -1;
	ShowWindow(mainWindow, nCmdShow);
	if (!InitD2D())
		return -1;
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
			//Sleep(1);
		}
	}
	Clean();
	return 0;
}