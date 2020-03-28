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

			tstart = GetTickCount();//��ȡ��ͼ��ʼʱ��
			rs.Draw();//��ͼ				
			t2 = GetTickCount();//��ȡ��ͼ���ʱ��
			if (t2 - ft1 > 1000)//�����ǰʱ���Ѿ����ϴ�ʱ�䳬��һ��
			{
				wsprintf(temp, L"fps:%d", frame);
				SetWindowText(rs.mainWindow, temp);//��ʾ֡��
				ft1 = t2;//����ʱ��
				frame = 0;//����֡��
			}
			else
			{
				frame++;
			}
			if (1000 / fpslimit > (t2-tstart))//�����ʱ��ʣ��
				Sleep(1000 / fpslimit - (t2-tstart));//��ʣ���ʱ��ȴ�
			
			
		}
	}
	rs.Clean();
	UnregisterClass(rs.ClassName, hInstance);
	return 0;
}
