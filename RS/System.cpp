#include "System.h"

//inp

void System::CheckInputDevice(DeviceType deviceType)
{
	inp_InitinputDevice(0,deviceType);
}

int System::inp_InitinputDevice(HINSTANCE hInstance, DeviceType deviceType)
{
	if (!inp_dinputDeivce)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&inp_dinputDeivce, 0)))
		{
			//MessageBox(0, L"Init Dinput Failed!", L"error", 0);
			return -1;
		}
	}	

	if ((deviceType == DeviceType_KeyBoard || deviceType == DeviceType_All) && !inp_KeyBoard)
	{
#pragma region keyboardInit
		if (FAILED(inp_dinputDeivce->CreateDevice(GUID_SysKeyboard, &inp_KeyBoard, 0)))
		{
			//MessageBox(0, L"Init Dinput Failed!", L"error", 0);
			return 0;
		}

		if (FAILED(inp_KeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		{
			//MessageBox(0, L"Init Dinput Failed!", L"error", 0);
			return 0;
		}

		if (FAILED(inp_KeyBoard->SetCooperativeLevel(mainWindow, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			//MessageBox(0, L"Init Dinput Failed!", L"error", 0);
			return 0;
		}
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = 8; // Arbitary buffer size
		if (FAILED(inp_KeyBoard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		{
			//MessageBox(0, L"Init Dinput Failed!", L"error", 0);
			return 0;
		}
		inp_KeyBoard->Acquire();
#pragma endregion

		
	}
	if ((deviceType == DeviceType_Joy || deviceType == DeviceType_All) && !inp_Joy)
	{
#pragma region JoyInit
		UINT choose = 0;
	retryJoy:
		if (FAILED(inp_dinputDeivce->CreateDevice(GUID_Joystick, &inp_Joy, 0)))
		{
			
			return 0;
		}

		if (FAILED(inp_Joy->SetDataFormat(&c_dfDIJoystick2)))
		{
			return 0;
			
		}

		if (FAILED(inp_Joy->SetCooperativeLevel(mainWindow, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			return 0;
		}
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_DEVICE;
		diprg.diph.dwObj = 0; // Specify the enumerated axis
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		if (FAILED(inp_Joy->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return 0;
		}
		inp_Joy->Acquire();
		
#pragma endregion

	}
	if ((deviceType == DeviceType_Mouse || deviceType == DeviceType_All) && !inp_Mouse)
	{
#pragma region mouseInit
		if (FAILED(inp_dinputDeivce->CreateDevice(GUID_SysMouse, &inp_Mouse, 0)))
		{
			return 0;
		}

		if (FAILED(inp_Mouse->SetDataFormat(&c_dfDIMouse)))
		{
			return 0;
		}

		if (FAILED(inp_Mouse->SetCooperativeLevel(mainWindow, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			return 0;
		}
		
#pragma endregion

		
	}
	return 1;
}
bool System::inp_GetDeviceState(DeviceType deviceType)
{
	switch (deviceType)
	{
	case DeviceType_Joy:
		return inp_bJoyReady;
	case DeviceType_KeyBoard:
		return inp_bKeyBoardReady;
	case DeviceType_Mouse:
		return inp_bMouseReady;
	default:
		break;
	}
	return false;
}
void System::inp_ShutDown(DeviceType deviceType)
{
	switch (deviceType)
	{
	case 0:
		if (inp_Joy)
		{
			//inp_Joy->Unacquire();
			inp_Joy->Release();
		}
		if (inp_KeyBoard)
		{
			//inp_KeyBoard->Unacquire();
			inp_KeyBoard->Release();
		}
		if (inp_Mouse)
		{
			//inp_Mouse->Unacquire();
			inp_Mouse->Release();
		}
		break;
	case DeviceType_Joy:
		if (inp_Joy)
		{
			//inp_Joy->Unacquire();
			inp_Joy->Release();
		}
		break;
	case DeviceType_KeyBoard:
		if (inp_KeyBoard)
		{
			//inp_KeyBoard->Unacquire();
			inp_KeyBoard->Release();
		}
		break;
	case DeviceType_Mouse:
		if (inp_Mouse)
		{
			//inp_Mouse->Unacquire();
			inp_Mouse->Release();
		}
		break;
	default:
		break;
	}
}
bool System::GetDeviceData(DeviceType deviceType)
{
	DWORD dwElements = 8;
	POINT pt = { 0 };
	DIJOYSTATE2 diJoyStateTemp = { 0 };
	switch (deviceType)
	{
	case DeviceType_KeyBoard:
		if (this->inp_KeyBoard == NULL)
			return false;
		ZeroMemory(inp_keyBoardDidod, sizeof(DIDEVICEOBJECTDATA) * 8);
		//g_KeyBoard->Poll();
		if (DI_OK != inp_KeyBoard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), inp_keyBoardDidod, &dwElements, 0))
			inp_KeyBoard->Acquire();
		return true;
		break;
	case DeviceType_Joy:
		if (this->inp_Joy == NULL)
			return false;

		//g_KeyBoard->Poll();
		/*if (DI_OK != inp_Joy->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), inp_joyDidod, &dwElements, 0))
			inp_Joy->Acquire();*/
		for (size_t i = 0; i < 128; i++)
		{
			if (inp_JoyState.rgbButtons[i] == RSKeyUp)
			{
				inp_JoyState.rgbButtons[i] = RSKey0;
			}
		}
		diJoyStateTemp = inp_JoyState;
		inp_Joy->Poll();
		if (DI_OK != inp_Joy->GetDeviceState(sizeof(DIJOYSTATE2), &inp_JoyState))
			inp_Joy->Acquire();
		for (size_t i = 0; i < 128; i++)
		{
			if (inp_JoyState.rgbButtons[i] == 0 && diJoyStateTemp.rgbButtons[i] == RSKeyDown)
			{
				inp_JoyState.rgbButtons[i] = RSKeyUp;
			}
		}
		return true;
		break;
	case DeviceType_Mouse:
		if (this->inp_Mouse == NULL)
			return false;

		inp_Mouse->Poll();
		if (DI_OK != inp_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), &inp_mouseState))
			inp_Mouse->Acquire();
		GetCursorPos(&pt);
		MouseX = pt.x;
		MouseY = pt.y;
		return true;
		break;
	default:
		break;
	}
	return false;
}
void System::inp_DealInput(DeviceType deviceType)
{
	for (int i = 0; i < 256; i++)//覆盖掉上次的KeyUp消息
	{
		if (KeyState[i] == RSKeyUp)
			KeyState[i] = RSKey0;
	}

	if (deviceType == DeviceType_KeyBoard)
	{
		for (int i = 0; i < 8; i++)
		{
			if (inp_keyBoardDidod[i].dwOfs != 0)
			{
				if (inp_keyBoardDidod[i].dwData == 128)
					KeyState[inp_keyBoardDidod[i].dwOfs] = RSKeyDown;
				else if (inp_keyBoardDidod[i].dwData == 0)
				{
					if (KeyState[inp_keyBoardDidod[i].dwOfs] == RSKeyDown)
						KeyState[inp_keyBoardDidod[i].dwOfs] = RSKeyUp;


				}

			}
		}
		if (KeyState[DIK_LCONTROL] == RSKeyDown && KeyState[DIK_Q] == RSKeyUp)
		{

			if (IDOK == MessageBox(0, L"", L"", MB_ICONQUESTION | MB_OKCANCEL))
			{
				PQuit = true;
				PostQuitMessage(0);

				exit(0);
			}
		}
		else if (KeyState[DIK_LALT] == RSKeyDown && KeyState[DIK_W] == RSKeyUp)
		{
			if (IsWindowVisible(menu))
			{
				ShowWindowAsync(menu, SW_MINIMIZE);//窗口在最小化之前HIDE 桌面那个窗口绘图会变慢 所以要先最小化再HIDE
				ShowWindowAsync(menu, SW_HIDE);
			}
			else
			{
				ShowWindowAsync(menu, SW_SHOWDEFAULT);
				SetForegroundWindow(menu);
				//SetWindowPos(menu, HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			}
		}


	}
	else if (deviceType == DeviceType_Joy)
	{
		//if ((inp_JoyState.rgbButtons[8] & 0x80) && (inp_JoyState.rgbButtons[9]))
		//{
		//	//gdiScreenCapture(screenCaptureData);
		//	inp_bJoyReady;
		//}
		if (inp_JoyState.rgbButtons[8] == RSKeyUp && inp_JoyState.lZ < 0)
		{
			mControlByJoy = !mControlByJoy;
		}
		if (mControlByJoy)
		{

			if (inp_JoyState.lX != 0 || inp_JoyState.lY != 0)
			{
				if (!slowMode)
				{
					int lx = inp_JoyState.lX / 30;
					int ly = 0;
					if (inp_JoyState.lY < 65 && inp_JoyState.lY>0)
						ly = 0;
					else
						ly = inp_JoyState.lY / 30;

					POINT pt = { 0 };
					GetCursorPos(&pt);
					SetCursorPos(pt.x + lx, pt.y + ly);
				}
				else
				{
					int lx = inp_JoyState.lX / 45;
					int ly = 0;
					if (inp_JoyState.lY < 65 && inp_JoyState.lY>0)
						ly = 0;
					else
						ly = inp_JoyState.lY / 45;
					POINT pt = { 0 };
					GetCursorPos(&pt);
					SetCursorPos(pt.x + lx, pt.y + ly);
				}
			}
			if (inp_JoyState.lRx != 0 || inp_JoyState.lRy != 0)
			{
				int lry = inp_JoyState.lRy / 20;
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -lry, 0);
			}
			if (inp_JoyState.rgbButtons[0] == RSKeyDown)
			{
				if (!simMouseButtonStateA[0])
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					simMouseButtonStateA[0] = true;
				}
			}
			else if (inp_JoyState.rgbButtons[0] == RSKeyUp)
			{
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				simMouseButtonStateA[0] = false;
			}
			if (inp_JoyState.rgbButtons[2] == RSKeyDown && inp_JoyState.rgbButtons[8] == RSKeyUp)
			{
				slowMode = !slowMode;
			}
			if (inp_JoyState.lZ < 0)
			{
				if (!simMouseButtonStateTrig[0])
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					simMouseButtonStateTrig[0] = true;
				}
			}
			else if (inp_JoyState.lZ > 0)
			{
				if (!simMouseButtonStateTrig[0])
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					simMouseButtonStateTrig[1] = true;
				}
			}
			else if (inp_JoyState.lZ == 0)
			{
				if (simMouseButtonStateTrig[0])
				{
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					simMouseButtonStateTrig[0] = false;
				}
				else if (simMouseButtonStateTrig[1])
				{
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					simMouseButtonStateTrig[1] = false;
				}
			}

			if (inp_JoyState.rgdwPOV[0] == 0)
			{

			}
		}
	}
	else if (deviceType == DeviceType_Mouse)
	{

		inp_mouseStateAfter.lX = inp_mouseState.lX;
		inp_mouseStateAfter.lY = inp_mouseState.lY;
		inp_mouseStateAfter.lZ = inp_mouseState.lZ;

		for (int i = 0; i < 3; i++)
		{
			if (RSKeyUp == inp_mouseStateAfter.rgbButtons[i])//处理上次的KeyUp消息 改为Key0
			{
				inp_mouseStateAfter.rgbButtons[i] = RSKey0;
			}


			if (inp_mouseState.rgbButtons[i] == RSKeyDown)
			{
				inp_mouseStateAfter.rgbButtons[i] = inp_mouseState.rgbButtons[i];
			}
			else if (inp_mouseState.rgbButtons[i] == RSKey0 && inp_mouseStateAfter.rgbButtons[i] == RSKeyDown)
			{
				inp_mouseStateAfter.rgbButtons[i] = RSKeyUp;
			}
		}


	}





	return;
}


//window

bool System::InitMainWindow(int width, int height, DWORD winStyle,
	HINSTANCE hInstance, WNDPROC mwWndProc, const wchar_t* ClassName)
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
	{
		MessageBox(0, L"Register Class Failed!", L"error", 0);
		return false;
	}
	this->width = width;
	this->height = height;
	this->ClassName = (wchar_t*)ClassName;
	this->hIns = hInstance;
	int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	mainWindow = CreateWindow(ClassName, L"", winStyle, x, y, width, height, 0, 0, hInstance, 0);
	if (mainWindow == NULL)
		return false;


	return true;
}
bool System::window_Show(bool show)
{
	int s = 0;
	if (show)
	{
		s = SW_NORMAL;
	}
	else
		s = SW_HIDE;
	return ShowWindow(mainWindow, s);
}
HWND System::GetWindowHwnd()
{
	return mainWindow;
}


void System::Quit()
{
}
System::~System()
{
	//if (g_bitmap)g_bitmap->Release();	
	ReleaseCom(g_image_factory);
	ReleaseCom(d2dFactory);
	ReleaseCom(d2dRenderTarget);
	ReleaseCom(dWriteFactory);
	ReleaseCom(p_BlackBrush);
	ReleaseCom(p_bControlsBrush);

	//input
	if (inp_bJoyReady)
	{
		inp_ShutDown(DeviceType_Joy);
	}
	if (inp_bKeyBoardReady)
	{
		inp_ShutDown(DeviceType_KeyBoard);
	}
	if (inp_bMouseReady)
	{
		inp_ShutDown(DeviceType_Mouse);
	}
	if (inp_dinputDeivce)ReleaseCom(inp_dinputDeivce);

	DestroyWindow(mainWindow);
	UnregisterClass(ClassName, hIns);
	if (MouseHook)UnhookWindowsHookEx(MouseHook);
	if (KeyBoardHook)UnhookWindowsHookEx(KeyBoardHook);
}

//Lock

static LRESULT CALLBACK KBHProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* p = (CWPSTRUCT*)lParam;
	if (nCode == HC_ACTION)
	{
		//switch (wParam) //wParam中是消息ID
		//{
		//case WM_KEYDOWN:
		//{
		//	if (p->lParam == 'l' || p->lParam == 'L')
		//	{
		//		// 'l' 键被按下 
		//		 //do somthing
		//	}

		//}
		//}
		return 1;
		//return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	return wParam;
}
static LRESULT CALLBACK MHProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code != HC_ACTION)
	{
		return 1;
	}
	if (wParam == WM_LBUTTONDOWN || wParam == WM_MOUSEMOVE || wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP || wParam == WM_LBUTTONUP)
	{
		return 1;
	}
	//return CallNextHookEx(MouseHook, code, wParam, lParam);
	return 1;
}
void System::UnLock(DeviceType device)
{
	if (device == DeviceType_All)
	{
		if (MouseHook)UnhookWindowsHookEx(MouseHook);
		if (KeyBoardHook)UnhookWindowsHookEx(KeyBoardHook);
	}
	else if (device == DeviceType_Mouse)
	{
		if (MouseHook)UnhookWindowsHookEx(MouseHook);
	}
	else if (device == DeviceType_KeyBoard)
	{
		if (KeyBoardHook)UnhookWindowsHookEx(KeyBoardHook);
	}
}
bool System::Lock(DeviceType device)
{
	if (device == DeviceType_All)
	{
		KeyBoardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KBHProc, GetModuleHandle(NULL), 0);

		MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MHProc, GetModuleHandle(0), NULL);
		if (KeyBoardHook == 0 || MouseHook == 0)
			return false;
	}
	else if (device == DeviceType_KeyBoard)
	{
		KeyBoardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KBHProc, GetModuleHandle(NULL), 0);
		if (KeyBoardHook == 0)
			return false;
	}
	else if (device == DeviceType_Mouse)
	{
		MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MHProc, GetModuleHandle(0), NULL);
		if (MouseHook == 0)
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

//D2D

bool System::InitD2D(IDXGISwapChain* SwapChain)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory)))
		return false;

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dWriteFactory),
		reinterpret_cast<IUnknown**>(&dWriteFactory))))

		return false;
	if (FAILED(dWriteFactory->CreateTextFormat(L"微软雅黑", 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_EXTRA_CONDENSED, 30, L"", &m_pText)))
		return false;
	m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);




	IDXGISurface* surface = 0;
	SwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&surface));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	if (FAILED(d2dFactory->CreateDxgiSurfaceRenderTarget(surface, props, &d2dRenderTarget)))
		return false;

	if (FAILED(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_BlackBrush)))
		return false;
	if (FAILED(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_bControlsBrush)))
		return false;
	g_bitmap = D2DCreateBitmap(d2dRenderTarget, g_image_factory, L"C:\\Program Files\\RS\\tex.jpg");

	d2dRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	if (!g_bitmap)return false;

	//surface.Reset();


	return true;
}
bool System::InitD2D()
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory)))
		return false;

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dWriteFactory),
		reinterpret_cast<IUnknown**>(&dWriteFactory))))

		return false;


	if (FAILED(dWriteFactory->CreateTextFormat(L"微软雅黑", 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_EXTRA_CONDENSED, 30, L"", &m_pText)))
		return false;
	m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);







	//surface.Reset();


	if (!this->d2dHwndRenderTarget)//如果还没初始化   (防止重复初始化)
	{
		if (mainWindow)//如果窗口句柄不为空就创建窗口句柄的
		{
			if (FAILED(d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(mainWindow, D2D1::SizeU(width, height)), &d2dHwndRenderTarget)))
				return false;

			if (FAILED(d2dHwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_bControlsBrush)))
				return false;
			if (FAILED(d2dHwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_BlackBrush)))
				return false;
			g_bitmap = D2DCreateBitmap(d2dHwndRenderTarget, g_image_factory, L"C:\\Program Files\\RS\\tex.jpg");

			d2dHwndRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
			if (!g_bitmap)return false;

			return true;
		}
		else
			return false;
	}
	else
		return true;
}
void System::D2DRender(bool Present)
{
	if (d2dRenderTarget)
	{
		wchar_t text[150] = L"在自己经历过瘦，经历过胖，又学到了相应的健身知识之后，我觉得，瘦子难增肌其实是个伪命题，根本不存在什么难度。\n你之所以感觉难，原因其实就是一个字，懒！\n你什么都不肯做，天天在那唉声叹气，说自己没办法增重，没办法增肌，怎么吃都吃不胖，其实你只是在为自己找借口而已，你没有去努力锻炼，你也没有吃多少。";




		d2dRenderTarget->BeginDraw();

		//swprintf_s(text, 18, L"X:%d,Y:%d", menuPoint.x, menuPoint.y);
		D2D1_RECT_F rect = { 0,0,(float)width,(float)height };

		d2dRenderTarget->DrawBitmap(g_bitmap, rect);

		d2dRenderTarget->DrawText(text, 24, m_pText, { 0, 0, 800, 800 }, p_BlackBrush);
		if (Present)
		{
			d2dRenderTarget->EndDraw();
		}
	}
	if (d2dHwndRenderTarget)
	{
		wchar_t text[150] = L"在自己经历过瘦，经历过胖，又学到了相应的健身知识之后，我觉得，瘦子难增肌其实是个伪命题，根本不存在什么难度。\n你之所以感觉难，原因其实就是一个字，懒！\n你什么都不肯做，天天在那唉声叹气，说自己没办法增重，没办法增肌，怎么吃都吃不胖，其实你只是在为自己找借口而已，你没有去努力锻炼，你也没有吃多少。";
		d2dHwndRenderTarget->BeginDraw();

		//swprintf_s(text, 18, L"X:%d,Y:%d", menuPoint.x, menuPoint.y);
		D2D1_RECT_F rect = { 0,0,(float)width,(float)height };

		d2dHwndRenderTarget->DrawBitmap(g_bitmap, rect);

		d2dHwndRenderTarget->DrawText(text, lstrlenW(text), m_pText, { 0, 0, 800, 800 }, p_BlackBrush);
		if (Present)
		{
			d2dHwndRenderTarget->EndDraw();
		}
	}
}

