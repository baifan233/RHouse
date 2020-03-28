#include"RSystem.h"
//#include"Object.h"
int fpslimit = 60;
int frame = 0;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrehInstance, PSTR nCmdLine, int nCmdShow)
{

	RSystem rs;
	rs.Init(hInstance, nCmdShow);
	MSG msg = { 0 };
	DWORD tstart, t2;
	DWORD ft1= 0;//fps  time
	wchar_t temp[20];
	ft1 = GetTickCount();
	
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		else
		{

			tstart = GetTickCount();//获取绘图开始时间
			rs.Draw();//绘图				
			t2 = GetTickCount();//获取绘图后的时间
			if (t2 - ft1 > 1000)//如果当前时间已经离上次时间超过一秒
			{
				wsprintf(temp, L"fps:%d", frame);
				SetWindowText(rs.mainWindow, temp);//显示帧数
				ft1 = t2;//重置时间
				frame = 0;//重置帧数
			}
			else
			{
				frame++;
			}
			if (1000 / fpslimit > (t2-tstart))//如果有时间剩余
				Sleep(1000 / fpslimit - (t2-tstart));//将剩余的时间等待
			
			
		}
	}
	rs.Clean();
	UnregisterClass(rs.ClassName, hInstance);
	return 0;
}
