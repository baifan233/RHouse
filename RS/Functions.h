#pragma once
#include"Rdefines.h"

DWORD GetCurrentActiveWindowsProcessId()
{
	HWND hWnd = GetActiveWindow();
	DWORD processId = 0;
	GetWindowThreadProcessId(hWnd, &processId);
	return processId;
}

BOOL HasFocus()
{
	DWORD cp = GetCurrentProcessId();
	DWORD cawp = GetCurrentActiveWindowsProcessId();
	return cp == cawp;
}

HRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		break;
	case WM_DESTROY:
	{
		ShowWindowAsync(hwnd, SW_MINIMIZE);
		ShowWindowAsync(hwnd, SW_HIDE);
	}
	break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return wParam;
}


HWND _WORKERW = nullptr;
inline BOOL CALLBACK EnumWindowsProc(_In_ HWND tophandle, _In_ LPARAM topparamhandle)
{
	HWND defview = FindWindowEx(tophandle, 0, L"SHELLDLL_DefView", nullptr);
	if (defview != nullptr)
	{
		_WORKERW = FindWindowEx(0, tophandle, L"WorkerW", 0);
	}
	return true;
}
HWND GetWorkerW() {
	int result = 0;
	HWND windowHandle = FindWindow(L"Progman", nullptr);
	//使用 0x3e8 命令分割出两个 WorkerW
	SendMessageTimeout(windowHandle, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, (PDWORD_PTR)&result);
	//遍历窗体获得窗口句柄
	EnumWindows(EnumWindowsProc, (LPARAM)nullptr);
	// 显示WorkerW
	ShowWindow(_WORKERW, SW_HIDE);
	return windowHandle;
}

void autostart()
{
	HKEY hKey;
	wstring strRegPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	//1、找到系统的启动项  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath.c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///打开启动项       
	{
		//2、得到本程序自身的全路径
		TCHAR strExeFullDir[MAX_PATH];
		GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

		//3、判断注册表项是否已经存在
		TCHAR strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, nullptr, L"GISRestart", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4、已经存在
		if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
		{
			//5、添加一个子Key,并设置值，"GISRestart"是应用程序名字（不加后缀.exe） 
			RegSetValueEx(hKey, L"GISRestart", 0, REG_SZ, (LPBYTE)strExeFullDir, (lstrlen(strExeFullDir) + 1) * sizeof(TCHAR));

			//6、关闭注册表
			RegCloseKey(hKey);
		}
	}
	else
	{
		MessageBox(0, L"系统参数错误,不能随系统启动", 0, 0);
	}
}

//void gdiScreenCapture(LPVOID screenCaptureData)
//{
//	MResetDC();
//	HWND thwnd = GetDesktopWindow();
//	char ctemp[128] = "\0";
//	bool underPath = false;
//	string folderPath = "G:\\value\\value\\screenshot\\";
//	string folderPath2 = folderPath;
//	if (thwnd)
//	{
//		GetWindowTextA(thwnd, ctemp, 128);
//
//		folderPath2 += ctemp;
//		if (!(GetFileAttributesA(folderPath2.c_str()) & FILE_ATTRIBUTE_DIRECTORY)) {
//			//cout << "Directory already exists." << endl;
//		}
//		else {
//			CreateDirectoryA(folderPath2.c_str(), NULL);
//			underPath = true;
//			folderPath2 += "\\";
//		}
//	}
//	BitBlt(hCDC, 0, 0, nWidth, nHeight, hDDC, 0, 0, SRCCOPY);
//
//	GetBitmapBits(hBitmap, nWidth * nHeight * 4, screenCaptureData);//得到位图的数据，并存到screenCaptureData数组中。  
//	IplImage* img = cvCreateImage(cvSize(nWidth, nHeight), 8, 4);//创建一个rgba格式的IplImage,内容为空  
//	memcpy(img->imageData, screenCaptureData, nWidth * nHeight * 4);//这样比较浪费时间，但写的方便。这里必须得是*4，上面的链接写的是*3，这是不对的。  
//	IplImage* img2 = cvCreateImage(cvSize(nWidth, nHeight), 8, 3);//创建一个bgr格式的IplImage，可以没有这个Img2这个变量。  
//	cvCvtColor(img, img2, CV_BGRA2BGR);
//	// cvShowImage("img",img);  
//
//	SYSTEMTIME stime;
//	GetLocalTime(&stime);
//
//	char buffer[128] = "\0";
//	if (underPath)
//	{
//		_itoa_s(stime.wYear, buffer, 10);
//		folderPath2 += buffer; folderPath2 += ".";
//
//		_itoa_s(stime.wMonth, buffer, 10);
//		folderPath2 += buffer; folderPath2 += ".";
//
//		_itoa_s(stime.wDay, buffer, 10);
//		folderPath2 += buffer; folderPath2 += ".";
//
//		_itoa_s(stime.wHour, buffer, 10);
//		folderPath2 += buffer; folderPath2 += "h.";
//
//		_itoa_s(stime.wMinute, buffer, 10);
//		folderPath2 += buffer; folderPath2 += "min.";
//
//		_itoa_s(stime.wSecond, buffer, 10);
//		folderPath2 += buffer; folderPath2 += "s";
//
//		folderPath2 += ".jpg";
//		cvSaveImage(folderPath2.c_str(), img);
//	}
//	else
//	{
//		_itoa_s(stime.wYear, buffer, 10);
//		folderPath += buffer; folderPath += ".";
//
//		_itoa_s(stime.wMonth, buffer, 10);
//		folderPath += buffer; folderPath += ".";
//
//		_itoa_s(stime.wDay, buffer, 10);
//		folderPath += buffer; folderPath += ".";
//
//		_itoa_s(stime.wHour, buffer, 10);
//		folderPath += buffer; folderPath += "h.";
//
//		_itoa_s(stime.wMinute, buffer, 10);
//		folderPath += buffer; folderPath += "min.";
//
//		_itoa_s(stime.wSecond, buffer, 10);
//		folderPath += buffer; folderPath += "s";
//
//		folderPath2 += ".jpg";
//		cvSaveImage(folderPath.c_str(), img);
//	}
//
//}