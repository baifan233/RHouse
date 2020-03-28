#include<Windows.h>
#include<io.h>
#include<thread>
#include<time.h>
using namespace std;

//struct RTimeStruct
//{
//	UINT day;
//	UINT Hour;
//	UINT Min;
//	UINT Second;
//};
//class RTimer
//{
//public:
//	void CountS(HWND hwnd) 
//	{
//		while (!IsHungAppWindow(hwnd))
//		{
//			Sleep(1000);						
//			rts.Second++;
//			if (rts.Second >= 60)
//			{
//				rts.Second -= 60;
//				rts.Min++;
//				if (rts.Min >= 60)
//				{
//					rts.Min -= 60;
//					rts.Hour++;
//					if (rts.Hour >= 24)
//					{
//						rts.Hour -= 24;
//						rts.day++;
//					}
//				}
//			}
//		}
//		//如果停止计时了
//		swprintf_s(text,L"%d day(s),%d h,%d min(s),%d s",rts.day,rts.Hour,rts.Min,rts.Second);
//		MessageBox(0,text,L"运行时长",0);
//	}
//private:
//	RTimeStruct rts = {0};
//	wchar_t text[260];
//};
//
//
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)

{

	DWORD dwCurProcessId = *((DWORD*)lParam);

	DWORD dwProcessId = 0;



	GetWindowThreadProcessId(hwnd, &dwProcessId);

	if (dwProcessId == dwCurProcessId && GetParent(hwnd) == NULL)

	{

		*((HWND*)lParam) = hwnd;

		return FALSE;

	}

	return TRUE;

}
HWND GetMainWindow()

{

	DWORD dwCurrentProcessId = GetCurrentProcessId();

	if (!EnumWindows(EnumWindowsProc, (LPARAM)&dwCurrentProcessId))

	{

		return (HWND)dwCurrentProcessId;

	}

	return NULL;

}

bool waitForPermit = true;

BOOL WINAPI DllMain(HMODULE hModule, DWORD reasonForCall, LPVOID lpReserved)
{
	//DLL_PROCESS_ATTACH，表示动态链接库刚被某一个进程加载，映射到了某一个地址空间，可以在此进行初始化
	//DLL_THREAD_ATTACH，动态链接库将被卸载，可以在这进行资源释放
	//DLL_THREAD_DETACH，应用程序创建了一个新的进程
	//DLL_PROCESS_DETACH，某个进程正常终止



	HWND menuhwnd = FindWindow(L"RMenu", L"");
	SendMessage(menuhwnd, 314, 20, 20);
	int time=0;
	switch (reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		while (waitForPermit)//如果还没有得到允许就歇会
		{
			if(!_access("C:\\Program Files\\permisson.rf", 0))
			{	
				waitForPermit = false;
				break;
			}
			Sleep(100);
			++time;
			if (time >= 200)
			{
				exit(0);
			}
		}

		break;
	case DLL_PROCESS_DETACH:

		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:

		break;
	default:
		break;
	}
	return true;
}