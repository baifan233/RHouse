#include"System.h"
//#include"D3D.h"
#include"RParticles.h"
#include"Functions.h"
#include"UI.h"
#include"RTimer.h"
UI ui;
System rsystem;
TRACKMOUSEEVENT tme;
HRESULT CALLBACK MenuWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (!tme.hwndTrack)
	{
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = HOVER_DEFAULT;
		tme.hwndTrack = hwnd;
	}

	switch (message)
	{
	case 315:
		if (wParam == 20 && lParam == 20)
		{
			ui.pageCurrent = 15;
			ShowWindowAsync(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
		}
		break;
	case 314:
		if (wParam == 20 && lParam == 20)
		{
			ui.pageCurrent = 14;
			ShowWindowAsync(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
		}
		break;
	case WM_CHAR:
		inputStr.push_back(wParam);
		break;
	case WM_DESTROY:
		ShowWindowAsync(hwnd, SW_MINIMIZE);//窗口在最小化之前HIDE 桌面那个窗口绘图会变慢 所以要先最小化再HIDE
		ShowWindowAsync(hwnd, SW_HIDE);
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MINIMIZE)
		{
			ShowWindow(hwnd, SW_MINIMIZE);
			//AnimateWindow(hwnd,200,AW_HIDE);
		}
		else if (wParam == 61728)
		{
			//AnimateWindow(hwnd, 200, AW_BLEND);
			ShowWindow(hwnd, SW_SHOWNORMAL);
		}
		else if (wParam == SC_CLOSE)
		{
			ShowWindowAsync(hwnd, SW_MINIMIZE);//窗口在最小化之前HIDE 桌面那个窗口绘图会变慢 所以要先最小化再HIDE
			ShowWindowAsync(hwnd, SW_HIDE);

		}
		break;
	case WM_MOUSELEAVE:
		menuPoint.x = -1;
		menuPoint.y = -1;
		break;
	case WM_MOUSEMOVE:
		TrackMouseEvent(&tme);
		menuPoint.x = LOWORD(lParam);
		menuPoint.y = HIWORD(lParam);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return wParam;
}
BYTE edit1;
BYTE Button15; 
BYTE Edit15;
void EnterCode()
{
	time_t t = time(0);
	tm tm2;
	localtime_s(&tm2, &t);
	int temp = tm2.tm_mday * 28;
	WPARAM wp = tm2.tm_hour + temp;
	wchar_t wcTemp[30] = { L"\0" };
	_itow_s(wp, wcTemp, 30, 10);
	wstring compareTemp = ui.GetEditText(edit1);
	
	if (compareTemp.compare(wcTemp) == 0)
	{
		HANDLE handle = CreateFile(L"C:\\Program Files\\permisson.rf", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		CloseHandle(handle);
		ui.SetCurrentPage(0);		
	}
	else
	{
		ui.SetEditText(edit1,L"密钥错误!");
	}
}
void Button_Back()
{
	ui.SetCurrentPage(0);
}
void Button_Desktopmenu()
{
	ui.SetCurrentPage(1);
}
void ChangeDrawMeth()
{
	rsystem.PdrawMeth == 0 ? rsystem.PdrawMeth = 1 : rsystem.PdrawMeth = 0;
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
	return false;
}
DWORD BanExeFunc(vector<wstring> names)
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
	//bool bInjected=false; //该PID是否已注入
	bool result;//注入结果
	while (flag != 0)
	{

		for (size_t i = 0; i < names.size(); i++)
		{
			wstemp = pe.szExeFile;
			compareVar = names[i].compare(wstemp);

			if (compareVar == 0)
			{
				/*for (size_t i = 0; i < injectedPID.size(); i++)
				{
					if (injectedPID[i] == pe.th32ProcessID)
						bInjected = true;
				}*/
				//if (!bInjected)
				//{
					result = InjectDll("C:\\Program Files\\RS\\TryDll.dll", pe.th32ProcessID);
					//if (result)injectedPID.push_back(pe.th32ProcessID);//如果注入成功加入已注入队列
				//}
				
			}
			Sleep(1);
		}
		flag = Process32Next(hsnapshot, &pe);
	}

	CloseHandle(hsnapshot);

	return 0;
}
DWORD BanExeThread(vector<wstring> names)
{	
	int CheckFileTime = 0;
	int deleteFileTime = 0;
	//int clearInjectedTime=0;
	bool bFile=false;//文件是否存在
	
	while (!PQuit)
	{
		if (bBanExe)
		{

			if (CheckFileTime > 5)
			{
				bFile = !_access("C:\\Program Files\\permisson.rf", 0);
				CheckFileTime = 0;
			}
			else
			{
				CheckFileTime++;
			}
			if (!bFile)
			{
				BanExeFunc(names);
			}
			else
			{
				deleteFileTime++;
				if (deleteFileTime >= 2 * 60 * 60) //一小时期限
				{
					DeleteFile(L"C:\\Program Files\\permisson.rf");
					deleteFileTime = 0;
				}
			}		
			Sleep(800);
		}
		else
		{
			Sleep(1000);
		}
	}	
	return 0;
}
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
vector<wstring> names;
void LoadPList()
{
	string filePath = "C:\\Program Files\\\RS\\list.rl";
	ifstream ifile(filePath, std::ios::binary);
	if (!ifile.is_open())
		return;
	string line = "";
	vector<string> lines;
	wchar_t temp[260] = L"\0";
	while (getline(ifile, line))
	{
		for (size_t i = 0; i < line.size(); i++)
		{
			if (line[i] == '\r')
			{
				line[i] = '\0';
			}
		}
		MultiByteToWideChar(CP_ACP,0,line.c_str(),260,temp,260);
		
		
		names.push_back(temp);
	}	
}

HRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{	



	LoadPList();
	enableDebugPriv();
	DeleteFile(L"C:\\Program Files\\permisson.rf");
	autostart();

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	

	//ui.LoadControlFromFile(L"C:\\controls.txt");

	RParticles rparticles;
	
	HDC hdc = GetDC(NULL);
	int ScreenWidth = GetDeviceCaps(hdc, DESKTOPHORZRES);
	int ScreenHeight = GetDeviceCaps(hdc, DESKTOPVERTRES);
	ReleaseDC(NULL, hdc);

	//D3D mD3D;
	if (!rsystem.InitMainWindow(ScreenWidth, ScreenHeight, WS_POPUP, hInstance, (WNDPROC)WndProc, L"asd"))
	{
		rsystem.Quit();
		//mD3D.Clean(hInstance);
		return -1;
	}


	MoveWindow(rsystem.GetMainWindow(), 0, 0, ScreenWidth, ScreenHeight, false);
	SetParent(rsystem.GetMainWindow(), GetWorkerW());
	rsystem.window_Show(true);


	if (-1==rsystem.inp_InitinputDevice(hInstance,DeviceType_All))
	{
		rsystem.Quit();
		//mD3D.Clean(hInstance);
		return -1;
	}
	
	//if (!mD3D.InitDirect3D(rsystem.GetWindowHwnd(), ScreenWidth, ScreenHeight))
	//{
	//	rsystem.Quit();
	//	//mD3D.Clean(hInstance);
	//	return -1;
	//}
	//if (!mD3D.LoadContent())
	//{
	//	rsystem.Quit();
	//	mD3D.Clean(hInstance);
	//	return -1;
	//}
	MSG msg = { 0 };

	if (!rsystem.InitD2D())
		return -1;


	rparticles.Init(20000, 100, ScreenWidth, ScreenHeight,
		D2DCreateBitmap(rsystem.GetD2DRen(), rsystem.GetimageFacotry(), L"C:\\Program Files\\RS\\dot.png"));


	HWND menuHwnd = ui.RCreateWindow(1024, 768, WS_MINIMIZEBOX | WS_POPUP, hInstance, (WNDPROC)MenuWndProc, L"RMenu", L"");
	rsystem.SetMenu(menuHwnd);
	ui.Init(menuHwnd, 1024, 768);
	BYTE back14 = ui.CreateButton({ 0.64f,0.64f,0.64f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, L"返回", { 0.0f,0.0f,80.0f,40.0f }, Button_Back);
	BYTE enterCode = ui.CreateButton({ 0.64f,0.64f,0.64f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, L"确定", { 150.0f,180.0f,250.0f,230.0f }, EnterCode);
	edit1 = ui.CreateEdit({ 0,350,200,480 }, { 0.764f,0.76f,0.776f,1.0f });
	BYTE BDesktopMenu = ui.CreateButton({ 0.64f,0.64f,0.64f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, L"桌面设置", { 61.0f,110.0f,161.0f,190.0f }, Button_Desktopmenu);
	BYTE BBack = ui.CreateButton({ 0.64f,0.64f,0.64f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, L"返回", { 0.0f,0.0f,80.0f,40.0f }, Button_Back);
	//ui.CreateButton({ 0.64f,0.64f,0.64f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, L"2", { 181.0f,110.0f,281.0f,190.0f }, 4, );
	BYTE Slip1 = ui.CreateSlip({ 0.61f,0.61f,0.61f,1.0f }, { 100.0f,300.0f,200.0f,320.0f }, L"");
	//BYTE Edit1 = ui.CreateEdit({ 0,350,200,480 }, {0.764f,0.76f,0.776f,1.0f});
	//BYTE Edit2 = ui.CreateEdit({ 350,350,550,480 }, {0.764f,0.76f,0.776f,1.0f});
	BYTE Slip2=ui.CreateSlip({ 0.61f,0.61f,0.61f,1.0f }, { 100.0f,350.0f,200.0f,370.0f }, L"");
	BYTE Slip3 = ui.CreateSlip({ 0.61f,0.61f,0.61f,1.0f }, {100.0f,400.0f,200.0f,430.0f},L"");;
	BYTE Button2 = ui.CreateButton({ 0.64f,0.64f,0.64f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, L"粒子运动方式", { 150,180,250,230 }, ChangeDrawMeth);

	ui.SetPageForControl(edit1, 14);
	ui.SetPageForControl(back14, 14);
	ui.SetPageForControl(enterCode, 14);
	ui.SetPageBackGounrdColor({ 0.117f,0.117f,0.117f,1.0f }, 14);

	ui.SetPageForControl(BBack, 1);
	ui.SetPageForControl(Slip1, 1);
	ui.SetPageForControl(Button2, 1);
	ui.SetPageForControl(BDesktopMenu, 0);
	ui.SetPageForControl(Slip2,1);
	ui.SetPageForControl(Slip3,1);
	//ui.SetPageForControl(Edit1,0);
	//ui.SetPageForControl(Edit2,0);
	ui.SetPageBackGounrdColor({ 0.117f,0.117f,0.117f,1.0f }, 1);
	ui.SetPageBackGounrdColor({ 0.117f,0.117f,0.117f,1.0f }, 0);
	ui.LoadBitmapForPageBackgournd(1, L"C:\\Program Files\\RS\\bg.jpg");
	ui.LoadBitmapForControl(BDesktopMenu, L"C:\\Program Files\\RS\\bg.jpg");
	rsystem.PdrawNum = ui.GetSlipValueAddress(Slip1);
	rparticles.speedOffset = ui.GetSlipValueAddress(Slip2);
	
	UIInputStruct uis = { 0 };
	uis.inp_mouseState = rsystem.GetMouseState();
	uis.rsKeyState = rsystem.GetKeyState();

	uis.pt = &menuPoint;
	uis.wParam = &inputStr;
	ui.SetUIS(uis);
	ui.CreateRenderThread();
	rsystem.GetKeyState();
	//thread t(threadF, ref(rsystem), ref(mD3D), ref(rparticles), ref(ui));
	long long tine = 0;

	thread tBanExe(BanExeThread,ref(names));
	RTimer rtimer;
	rtimer.Start();
	RTimer_t* checkInputdeviceTimer= rtimer.CreateTimer(1,5000,-1);
	while (!PQuit)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (checkInputdeviceTimer->var == RTimer_State_ISTIME)
			{
				rsystem.CheckInputDevice(DeviceType_All);
				rtimer.resetTimerNOCall(checkInputdeviceTimer->id);
			}
			rsystem.GetDeviceData(DeviceType_KeyBoard);
			rsystem.inp_DealInput(DeviceType_KeyBoard);
			rsystem.GetDeviceData(DeviceType_Mouse);
			rsystem.inp_DealInput(DeviceType_Mouse);
			rsystem.GetDeviceData(DeviceType_Joy);
			rsystem.inp_DealInput(DeviceType_Joy);

			//mD3D.Render(false);
			rsystem.D2DRender(false);
			if (rsystem.PdrawMeth == 0)
				rparticles.Draw(rsystem.GetD2DRen(), *rsystem.PdrawNum * 30);
			else
				rparticles.Draw2(rsystem.GetD2DRen(), *rsystem.PdrawNum * 30, true);

			//ui.DrawPage();

			//ui.Draw(rsystem.GetMouseState(), menuPoint,*rsystem.GetKeyState());
			rsystem.D2DPresent();
			//mD3D.Present();


			/*if (PQuit)
				t.detach();*/


				/*SDL_RenderCopy(rsystem.ren, rsystem.tex, 0, 0);
				if (rsystem.PdrawMeth == 0)
					rparticles.Draw(rsystem.ren, rsystem.PdrawNum);
				else
					rparticles.Draw2(rsystem.ren, rsystem.PdrawNum, px, py);
				rsystem.D2DPresent();
				SDL_RenderPresent(rsystem.ren);*/

			Sleep(16);
		}
	}
	//t.join();
	tBanExe.detach();
	//mD3D.Clean(hInstance);
	rsystem.Quit();
	return 0;
}
