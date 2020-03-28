#include "GSystem.h"
#include"Object.h"
#include"engine.cpp"
#include"Wall.h"
#include"FireWorks.h"
#include"Ground.h"
#include"Player.h"
bool jump = false;
Player* ob1 = new Player;
SDL_Texture* bg = NULL;
Wall* wall0 = new Wall;
FireWorks* firework;
SDL_Texture* firepoint;
Ground* ground;
unsigned char direction = 0;
RSystem::RSystem()
{
}
bool ghost = false;
void DealKeyMessage()
{
	if (g_KeyBoard == NULL)
		return;
	DWORD dwElements = 8;
	ZeroMemory(didod, sizeof(DIDEVICEOBJECTDATA) * 8);
	//g_KeyBoard->Poll();
	if (DI_OK != g_KeyBoard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0))
		g_KeyBoard->Acquire();
	//char temp[20];
	for (DWORD i = 0; i < dwElements; i++)
	{
		switch (didod[i].dwOfs)
		{
		case KEY_A:
			if (didod[i].dwData == G_KEY_DOWN)
			{
				PlayerState = STATE_MOVE_LEFT;
				ob1->frame = 0;
			}
			else
			{
				PlayerState = STATE_STAND_LEFT;
				ob1->frame = 0;
			}
			break;
		case KEY_C:
			if (didod[i].dwData==G_KEY_DOWN)
			{
				jump = true;
			}else
				jump = false;
			break;
		case KEY_D:
			if (didod[i].dwData == G_KEY_DOWN)
			{
				PlayerState = STATE_MOVE_RIGHT;
				ob1->frame = 0;
			}
			else
			{
				PlayerState = STATE_STAND_RIGHT;
				ob1->frame = 0;
			}
			break;
		case KEY_Q:
			if (didod[i].dwData == G_KEY_DOWN)
			{
				ghost = true;
			}
			else
			{
				ghost = false;
			}
			break;
		case KEY_R:
			firework->ReSet();
			break;
		default:
			PlayerState = 0;
			break;
		}


		/*if (didod[i].dwData == 256)
			MessageBox(0,L"256",L"",0);*/
	}
}
DWORD WINAPI RSystem::keyBoardThread(LPVOID lpParamter)
{
	if (lpParamter)
	{

	}
	while (quit != true)
	{
		DealKeyMessage();
		Sleep(1);
	}
	return 0;
}
bool RSystem::Init(HINSTANCE hInstance, int nCmdShow)
{
	TTF_Init();
	this->KeyBoradThread = CreateThread(NULL, 0, this->keyBoardThread, 0, 0, 0);
	if (0 != SDL_Init(SDL_INIT_VIDEO))
	{
		MessageBox(0, L"Init SDL Failed!", L"error", 0);
		return false;
	}
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



	ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);
	/*if (DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(&devmode, CDS_FULLSCREEN))
		MessageBox(0,0,0,0);*/
		//HWND hw = GetDesktopWindow();
	sdl_window = SDL_CreateWindowFrom(mainWindow);
	renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sdl_window == nullptr || renderer == nullptr)
		return false;
	GetClientRect(mainWindow, &mwRect);

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& dinputDevice, 0)))
	{
		MessageBox(0, L"Init Dinput Failed!", L"error", 0);
		return false;
	}

	if (FAILED(dinputDevice->CreateDevice(GUID_SysKeyboard, &g_KeyBoard, 0)))
	{
		MessageBox(0, L"Init Dinput Failed!", L"error", 0);
		return false;
	}

	if (FAILED(g_KeyBoard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(0, L"Init Dinput Failed!", L"error", 0);
		return false;
	}

	if (FAILED(g_KeyBoard->SetCooperativeLevel(mainWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		MessageBox(0, L"Init Dinput Failed!", L"error", 0);
		return false;
	}
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 8; // Arbitary buffer size
	if (FAILED(g_KeyBoard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	{
		MessageBox(0, L"Init Dinput Failed!", L"error", 0);
		return false;
	}

	g_KeyBoard->Acquire();
	if (!InitGame())
		return false;

	return true;
}
bool InitGame()
{
	ob1->InitRect(0, 0, 126, 159, 0, 0, 93, 159);
	TextureSet stand;
	stand.size = 8;
	bg = IMG_LoadTexture(renderer, "bg.bmp");
	ob1->Load(IMG_LoadTexture(renderer, "stand.png"), TEXTURESET_STAND);
	ob1->Load(IMG_LoadTexture(renderer, "stand.png"), TEXTURESET_MOVE);
	PlayerState = STATE_STAND_RIGHT;
	SDL_Rect wrect = { 0 };
	wrect.x = 600;
	wrect.y = 0;
	wrect.w = 89;
	wrect.h = 202;
	wall0->Load("J:\\wall.jpg", renderer, wrect);
	firework = new FireWorks;
	firework->Init(400, 400, 5, 100);
	firepoint = IMG_LoadTexture(renderer, "J:\\point.png");

	ground = new Ground;

	ground->Init("stone.png", renderer, 0, 500, 300, 50);

	return true;
}
void Changedisplay(int height, int width, bool Fullscreen, DEVMODE devMode, DISPLAY_DEVICE displayDevice)
{
	devmode.dmPelsHeight = height;
	devmode.dmPelsWidth = width;
	fullscreen = Fullscreen;
	if (fullscreen)
	{
		ChangeDisplaySettingsEx(displayDevice.DeviceName, &devMode, 0, CDS_FULLSCREEN, 0);
	}
	else
	{
		ChangeDisplaySettingsEx(displayDevice.DeviceName, &devMode, 0, 0, 0);
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
		Changedisplay(ScreenHeight, ScreenWidth, false, devmode, displaydevice);
		break;
	case WM_SETFOCUS:
		Changedisplay(mwHeight, mwWidth, true, devmode, displaydevice);
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
	//fclose(file);
	quit = true;
	CloseHandle(KeyBoradThread);
	if (g_KeyBoard)g_KeyBoard->Unacquire();
	if (g_KeyBoard)g_KeyBoard->Release();
	if (dinputDevice)dinputDevice->Release();


	SDL_DestroyTexture(bg);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(sdl_window);
	//SDL_DestroyTexture();
	ob1->~Player();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
point RSystem::a()
{
	/*iX += iXSpeed;
	iYSpeed = iYSpeed + iYGravity;
	iY += iYSpeed;
	if (iY >= mwRect.bottom - 60)
	{
		iY = mwRect.bottom - 60;
		iXSpeed += g_iXFriction;
		if (iXSpeed < 0)
			iXSpeed = 0;
		iYSpeed += g_iYFriction;
		if (iYSpeed < 0)
			iYSpeed = 0;
		iYSpeed = -iYSpeed;
	}
	if (iX >= mwRect.right - 60)
	{
		iXSpeed = -iXSpeed;
	}*/
	point a;
	a.x = 0;
	a.y = 0;
	return a;
}
bool check_collision(SDL_Rect A, SDL_Rect B)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;



	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//如果没有一条边在矩形B外
	return true;
}
int cameraX = 0;
//void FastBlurARGB8888(SDL_Texture* pOrg, int radius)
//{
//	void* pixt;
//	int pitch;
//	int tw, th, taccess;
//	unsigned int format;
//	SDL_QueryTexture(pOrg, &format, &taccess, &tw, &th);
//	int rs = SDL_LockTexture(pOrg, NULL, &pixt, &pitch);
//	if (radius < 1)
//	{
//		SDL_UnlockTexture(pOrg);
//		return;
//	}
//	int w = tw;
//	int h = th;
//	int wm = w - 1;
//	int hm = h - 1;
//	int wh = w * h;
//	int div = radius + radius + 1;
//	int* r = new int[wh];
//	int* g = new int[wh];
//	int* b = new int[wh];
//	int rsum, gsum, bsum, x, y, i, p, p1, p2, yp, yi, yw;
//	int* vmin = new int[max(w, h)];
//	int* vmax = new int[max(w, h)];
//	int* pix = (int*)pixt;
//
//	yw = yi = 0;
//
//	for (y = 0; y < h; y++)
//	{
//		rsum = gsum = bsum = 0;
//		for (i = -radius; i <= radius; i++)
//		{
//			p = pix[yi + min(wm, max(i, 0))];
//			rsum += (p & 0xff0000) >> 16;
//			gsum += (p & 0x00ff00) >> 8;
//			bsum += p & 0x0000ff;
//		}
//		for (x = 0; x < w; x++)
//		{
//
//			r[yi] = rsum / div;
//			g[yi] = gsum / div;
//			b[yi] = bsum / div;
//
//			if (y == 0)
//			{
//				vmin[x] = min(x + radius + 1, wm);
//				vmax[x] = max(x - radius, 0);
//			}
//			p1 = pix[yw + vmin[x]];
//			p2 = pix[yw + vmax[x]];
//
//			rsum += ((p1 & 0xff0000) - (p2 & 0xff0000)) >> 16;
//			gsum += ((p1 & 0x00ff00) - (p2 & 0x00ff00)) >> 8;
//			bsum += (p1 & 0x0000ff) - (p2 & 0x0000ff);
//			yi++;
//		}
//		yw += w;
//	}
//
//	for (x = 0; x < w; x++)
//	{
//		rsum = gsum = bsum = 0;
//		yp = -radius * w;
//		for (i = -radius; i <= radius; i++)
//		{
//			yi = max(0, yp) + x;
//			rsum += r[yi];
//			gsum += g[yi];
//			bsum += b[yi];
//			yp += w;
//		}
//		yi = x;
//		for (y = 0; y < h; y++)
//		{
//			pix[yi] = 0xff000000 | ((rsum / div) << 16) | ((gsum / div) << 8) |
//				(bsum / div);
//			if (x == 0)
//			{
//				vmin[y] = min(y + radius + 1, hm) * w;
//				vmax[y] = max(y - radius, 0) * w;
//			}
//			p1 = x + vmin[y];
//			p2 = x + vmax[y];
//
//			rsum += r[p1] - r[p2];
//			gsum += g[p1] - g[p2];
//			bsum += b[p1] - b[p2];
//
//			yi += w;
//		}
//	}
//
//	delete[] r;
//	delete[] g;
//	delete[] b;
//	delete[] vmin;
//	delete[] vmax;
//
//	SDL_UnlockTexture(pOrg);
//}
void G_Gravity(Object* object, int objNum, Ground* grounds, int groundNum, SDL_Rect* srect, int state)
{
	int tempo;
	int tempg;
	int gw;//groundright
	int ow;//objectright
	int objbottom;
	bool inair = true;
	bool ag = false;//aboveGround

	for (int i = 0; i < objNum; i++)
	{
		for (int j = 0; j < groundNum; j++)
		{
			object[i].prect.x = srect[i].x;
			object[i].prect.y = srect[i].y;
			objbottom = object[i].prect.y + object[i].prect.h;
			if (objbottom < ground[j].rect.y)
			{
				inair = true;
			}
			else
			{
				inair = false;
			}
			gw = ground[j].rect.x + ground[j].rect.w;
			ow = object[i].prect.x + object[i].prect.w;
			int index = 0;
			if (object[i].type == OBJECT_TYPE_PLAYER && direction == 1)
			{
				index = 42;
			}
			if (ow<ground[j].rect.x || object[i].prect.x + index>gw)
			{
				ag = false;
			}
			else
			{
				ag = true;
			}

			/*tempo = object[i].x + object[i].w;
			tempg = grounds[j].rect.x;
			if ((object[i].x < grounds[j].rect.x) && tempo<tempg)
			{

			}*/
		}
		if (inair || !ag)
		{
			object[i].Y_Speed+=1;
		}
		else
		{
			object[i].Y_Speed = 0;
		}
	}
}
void DealSpeed(Object* obj,int objnum)
{
	for (int i = 0; i < objnum; i++)
	{
		obj[i].rect.x += obj[i].X_Speed;
		obj[i].rect.y += obj[i].Y_Speed;
	}
}
void RSystem::Draw(int frame)
{
	//this->DealKeyMessage();	
	SDL_RenderClear(renderer);
	SDL_Rect rect = { 0 };
	rect.h = 768;
	rect.w = 1024;
	rect.x = cameraX;
	//FastBlurARGB8888(bg,5);
	SDL_RenderCopy(renderer, bg, &rect, 0);

	ground->Draw(renderer);

	G_Gravity(ob1, 1, ground, 1, &ob1->rect, PlayerState);
	if (jump)
	{
		ob1->Y_Speed -= 2;
	}
	DealSpeed(ob1,1);	
	if (!check_collision(ob1->prect, wall0->rect))
	{
		if (PlayerState == STATE_MOVE_RIGHT)
		{
			direction = 1;
			ob1->rect.x += 5;
			if (cameraX < 1920 - rect.w)
			{
				cameraX += 1;
			}
		}
		else if (PlayerState == STATE_MOVE_LEFT)
		{
			direction = -1;
			ob1->rect.x -= 5;
			if (cameraX > 0)
			{
				cameraX -= 1;
			}
		}
	}
	else
	{
		if (!ghost)
		{
			int right = wall0->rect.x + wall0->rect.w;
			if (ob1->rect.x < wall0->rect.x)
			{
				ob1->rect.x -= 5;
			}
			else if (ob1->rect.x > right - 10)
			{
				ob1->rect.x += 5;
			}
		}
	}

	if (ghost)
	{
		if (PlayerState == STATE_MOVE_RIGHT)
		{
			direction = 1;
			ob1->rect.x += 5;
		}
		else if (PlayerState == STATE_MOVE_LEFT)
		{
			direction = -1;
			ob1->rect.x -= 5;
		}
	}

	ob1->Draw(renderer, PlayerState, 64);

	if (frame == 0)
	{
		ob1->lastDrawFrame = 0;
	}
	else if (frame - ob1->lastDrawFrame == 4)
	{
		ob1->FramePlus(PlayerState);
		ob1->lastDrawFrame = frame;
	}

	wall0->Draw(renderer);
	if (ghost)
	{
		firework->explode(renderer, firepoint);
	}
	SDL_RenderPresent(renderer);
}
RSystem::~RSystem()
{
}

void Changedisplay()
{
}
