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
	//ʹ�� 0x3e8 ����ָ������ WorkerW
	SendMessageTimeout(windowHandle, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, (PDWORD_PTR)&result);
	//���������ô��ھ��
	EnumWindows(EnumWindowsProc, (LPARAM)nullptr);
	// ��ʾWorkerW
	ShowWindow(_WORKERW, SW_HIDE);
	return windowHandle;
}

void autostart()
{
	HKEY hKey;
	wstring strRegPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath.c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///��������       
	{
		//2���õ������������ȫ·��
		TCHAR strExeFullDir[MAX_PATH];
		GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

		//3���ж�ע������Ƿ��Ѿ�����
		TCHAR strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, nullptr, L"GISRestart", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4���Ѿ�����
		if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
		{
			//5�����һ����Key,������ֵ��"GISRestart"��Ӧ�ó������֣����Ӻ�׺.exe�� 
			RegSetValueEx(hKey, L"GISRestart", 0, REG_SZ, (LPBYTE)strExeFullDir, (lstrlen(strExeFullDir) + 1) * sizeof(TCHAR));

			//6���ر�ע���
			RegCloseKey(hKey);
		}
	}
	else
	{
		MessageBox(0, L"ϵͳ��������,������ϵͳ����", 0, 0);
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
//	GetBitmapBits(hBitmap, nWidth * nHeight * 4, screenCaptureData);//�õ�λͼ�����ݣ����浽screenCaptureData�����С�  
//	IplImage* img = cvCreateImage(cvSize(nWidth, nHeight), 8, 4);//����һ��rgba��ʽ��IplImage,����Ϊ��  
//	memcpy(img->imageData, screenCaptureData, nWidth * nHeight * 4);//�����Ƚ��˷�ʱ�䣬��д�ķ��㡣����������*4�����������д����*3�����ǲ��Եġ�  
//	IplImage* img2 = cvCreateImage(cvSize(nWidth, nHeight), 8, 3);//����һ��bgr��ʽ��IplImage������û�����Img2���������  
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