#include"GSystem.h"
#include<SDL.h>
RSystem rs;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	if (!rs.Init(hInstance, nCmdShow))
	{
		rs.Clean();
		exit(0);
	}
	MSG msg = { 0 };
	DWORD tstart;
	DWORD tend = 0;
	int Fpslimit = 60;//16
	int frame = 0;
	DWORD t = (DWORD)GetTickCount64();
	char temp[20];
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			tstart = (DWORD)GetTickCount64();
			rs.Draw(frame);
			tend = (DWORD)GetTickCount64();
			if (tend - t > 1000)
			{
				sprintf_s(temp, "fps:%d", frame);
				//SetWindowText(rs.mainWindow,temp);
				SDL_SetWindowTitle(rs.sdl_window, temp);
				t = tend;
				frame = 0;
			}
			else
			{
				frame++;
			}
			if ((DWORD)(1000 / Fpslimit) > (tend - tstart))
				Sleep(1000 / Fpslimit - (tend - tstart));
		}
	}
	UnregisterClass(rs.ClassName, hInstance);
	return 0;
}

