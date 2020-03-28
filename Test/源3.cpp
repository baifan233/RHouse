#include<Windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<TlHelp32.h>
using namespace std;
bool enableDebugPriv()
{
	HANDLE  hToken;
	LUID    sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)
		)
	{
		return false;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}
	return true;
}

bool InjectDll(const char DllPath[MAX_PATH], DWORD pid)
{
	char	Status[50] = { 0 };					//Return status text
	//DWORD	pid;								//Process that will be injected
	DWORD	tid;								//Thread ID of the created thread
	HANDLE	hProcess;							//Handle to the process
	HANDLE	hThread;							//Handle to the thread
	void* WriteAddr;							//Pointer to allocated memory

	//pid = GetDlgItemInt(DialogHwnd, IDC_EDIT_PID, NULL, false);
	//GetDlgItemText(DialogHwnd, IDC_EDIT_DLL_PATH, DllPath, MAX_PATH);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (hProcess != 0)
	{
		WriteAddr = VirtualAllocEx(hProcess,
			0,
			5 * 1024,
			MEM_COMMIT, PAGE_EXECUTE_READWRITE
		);
		if (WriteAddr == 0)
			return false;
		WriteProcessMemory(hProcess, WriteAddr, (LPVOID)DllPath, 5 * 1024, 0);
		LPVOID pFunc = LoadLibraryA;
		hThread = CreateRemoteThread(hProcess, 0, 0,
			(LPTHREAD_START_ROUTINE)pFunc, WriteAddr, 0, &tid);

		WaitForSingleObject(WriteAddr, INFINITE);
		VirtualFreeEx(hProcess, WriteAddr, 5 * 1024, MEM_COMMIT);
		//CloseHandle(WriteAddr);


		CloseHandle(hProcess);

		if (hThread)
			return true;
		else
			//MessageBox(0, L"失败", L"注入", 0);
			return false;
	}

}
DWORD GetProcessIdFromName(const wchar_t* name)
{
	HANDLE  hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE)
	{
		//printf("CreateToolhelp32Snapshot Error!\n");
		return 0;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	int flag = Process32First(hsnapshot, &pe);

	while (flag != 0)
	{
		if (lstrcmpW(pe.szExeFile, name) == 0)
		{
			return pe.th32ProcessID;
		}
		flag = Process32Next(hsnapshot, &pe);
	}

	CloseHandle(hsnapshot);

	return 0;
}

DWORD BanExe(vector<wstring> names)
{
	HANDLE  hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE)
	{
		//printf("CreateToolhelp32Snapshot Error!\n");
		return 0;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	int flag = Process32First(hsnapshot, &pe);
	int compareVar = -1;
	wstring wstemp;
	while (flag != 0)
	{

		for (size_t i = 0; i < names.size(); i++)
		{
			wstemp = pe.szExeFile;
			compareVar = names[i].compare(wstemp);
			
			if (compareVar == 0)
			{
				InjectDll("D:\\TryDll.dll", pe.th32ProcessID);
			}
		}
		flag = Process32Next(hsnapshot, &pe);
	}

	CloseHandle(hsnapshot);

	return 0;
}

int main()
{
	
	enableDebugPriv();
	//BOOL liu;
	//PROCESSENTRY32 pe32;
	//HANDLE hpxian;
	//pe32.dwSize = sizeof(pe32);
	//DWORD CurrentPid = GetCurrentProcessId();

	//hpxian = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//if (INVALID_HANDLE_VALUE == hpxian)
	//{
	//	return -1;
	//}

	////GetCurrentDirectory(DEF_BUF_SIZE);
	//DWORD dwProcessId;
	//liu = Process32First(hpxian, &pe32);
	//while (liu)
	//{
	//	liu = Process32Next(hpxian, &pe32);
	//	dwProcessId = pe32.th32ProcessID;
	//	if ((dwProcessId > 0) && (CurrentPid != dwProcessId)) {
	//		//开始尝试注入 
	//		if (lstrcmpW(L"devenv.exe",pe32.szExeFile))
	//		{
	//			if (lstrcmpW(L"Project1.exe", pe32.szExeFile))
	//			{
	//				InjectDll("D:\\TryDll.dll", pe32.th32ProcessID);
	//			}
	//		}

	//	}
	//	//Sleep(80);
	//}



	bool result = false;
	vector<wstring> names;
	names = { L"CrayonPhysicsDeluxe.exe",L"PlantsVsZombies.exe" };
	while (!result)
	{
		Sleep(500);
		//result = InjectDll("G:\\develop\\encode\\Project1\\Debug\\TryDll.dll", GetProcessIdFromName2(L"distraint.exe"));
		BanExe(names);
		
	}
	return 0;
	//HDC myDC;
	////获取屏幕分辨率
	//RECT ScrRect = {0,0,1920,1080};
	////GetWindowRect(GetDesktopWindow(), &ScrRect);
	//myDC = GetDC((HWND)0);   //获取设备环境
	//
	//int x, y, width, height;
	//for (long i = 0; i < 1000; i++)
	//{
	//	
	//	//width = ScrRect.right;
	//	//height = ScrRect.bottom;
	//	x=rand()%ScrRect.right;//0~1920
	//	y=rand()%ScrRect.bottom;//0~1080
	//	
	//	x = (rand() % ScrRect.right) / 2 - (width = rand() % 3);   //随机产生坐标
	//	y = rand() % ScrRect.bottom - (height = rand() % 3);

	//	BitBlt(myDC, x,y, x + width, y + height, myDC, x + rand() % 3 - 1, y - rand() % 2, SRCCOPY);       //绘图
	//	
	//	//BitBlt(myDC, x,y, width, height, myDC, x+rand()%3,y-rand()%2, SRCCOPY);       //绘图
	//}
	//
	//ReleaseDC((HWND)0, myDC);
	//InvalidateRect(0, NULL, TRUE);



	return 0;
}