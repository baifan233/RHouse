#pragma once
#include"Rdefines.h"
/*
Sentences Type:
Function
variable operation
process control
*/
class RExecuter
{
private:
	string retemp;
public:
	void ReadRE();
};


class System
{
public:
	void Quit();
	HWND GetWindowHwnd();

	//Input System
private:



	LPDIRECTINPUT8 inp_dinputDeivce = NULL;
	LPDIRECTINPUTDEVICE8 inp_KeyBoard = NULL;
	LPDIRECTINPUTDEVICE8 inp_Mouse = NULL;
	LPDIRECTINPUTDEVICE8 inp_Joy = NULL;
	DIJOYSTATE2 inp_JoyState = { 0 };
	DIDEVICEOBJECTDATA inp_keyBoardDidod[8] = { 0 };
	DIMOUSESTATE inp_mouseState = { 0 };
	DIMOUSESTATE inp_mouseStateAfter = { 0 };
	RSKEYSTATE KeyState[256] = { 0 };
	bool simMouseButtonStateA[3] = {false};  //A键控制鼠标状态
	bool simMouseButtonStateTrig[3] = {false};  //LT  RT (Triger)控制的鼠标状态
	bool slowMode = false;
	bool mControlByJoy = true;

	bool inp_bMouseReady = false;
	bool inp_bJoyReady = false;
	bool inp_bKeyBoardReady = false;
	int MouseY = 0;
	int MouseX = 0;
public:
	void CheckInputDevice(DeviceType deviceType);
	RSKEYSTATE* GetKeyState() { return this->KeyState; };
	int inp_InitinputDevice(HINSTANCE hInstance, DeviceType deviceType);
	//获取设备状态(是否启用)
	bool inp_GetDeviceState(DeviceType deviceType);

	void inp_DealInput(DeviceType deviceType);
	//获取设备数据(设备是否有信息)
	bool GetDeviceData(DeviceType deviceType);
	void inp_ShutDown(DeviceType deviceType);

	DIMOUSESTATE* GetMouseState() { return &inp_mouseStateAfter; };
	POINT GetMousePoint() { POINT pt = { MouseX,MouseY }; return pt; };


	//Window
private:
	HWND mainWindow = 0;
	wchar_t* ClassName = 0;
	int width = 0, height = 0;
	HINSTANCE hIns = 0;
	HWND menu;
public:
	bool InitMainWindow(int width, int height, DWORD winStyle
		, HINSTANCE hInstance, WNDPROC mwWndProc, const wchar_t* ClassName);
	bool window_Show(bool show);

	HWND GetMainWindow() { return mainWindow; };

	void SetMenu(HWND hwnd) { menu = hwnd; };

	int* PdrawNum = 0;
	int PdrawMeth = 0;


	//LockSystem
private:
	HHOOK KeyBoardHook;
	HHOOK MouseHook;

public:
	bool Lock(DeviceType device);
	void UnLock(DeviceType device);

	~System();

	//D2D
private:
	ID2D1Factory* d2dFactory = NULL;

	IDWriteFactory* dWriteFactory = NULL;
	IDWriteTextFormat* m_pText = NULL;
	ID2D1SolidColorBrush* p_BlackBrush = 0;
	ID2D1SolidColorBrush* p_bControlsBrush = 0;

	IWICImagingFactory* g_image_factory = NULL;
	ID2D1RenderTarget* d2dRenderTarget = NULL;
	ID2D1HwndRenderTarget* d2dHwndRenderTarget = NULL;

	ID2D1Bitmap* g_bitmap = 0;
	ID2D1Layer* g_bgLayer=0;
public:
	//ID2D1Factory* GetD2DFactory() { return d2dFactory; };
	ID2D1RenderTarget* GetD2DRen()
	{
		if (d2dRenderTarget)return d2dRenderTarget; if (d2dHwndRenderTarget)return d2dHwndRenderTarget;
	};
	IWICImagingFactory* GetimageFacotry() { return g_image_factory; };
	//IDWriteTextFormat* GetTextFormat() { return m_pText; };
	bool InitD2D(IDXGISwapChain* SwapChain);
	bool InitD2D();
	void D2DRender(bool Present);
	void D2DPresent()
	{
		if (d2dRenderTarget)d2dRenderTarget->EndDraw(); if (d2dHwndRenderTarget)d2dHwndRenderTarget->EndDraw();
	};


	//RExecuter
private:

public:


};

#define G_KEY_DOWN 128
#define G_KEY_UP 0 
