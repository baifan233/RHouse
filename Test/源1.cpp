#include <cwchar>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <conio.h>
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE PrehInstance,LPWSTR nCmdLine,int nCmdShow)
{
	//Sleep(2000);
	//unsigned int nKey = 'W';
	//unsigned int nMapVirtualKey = MapVirtualKey(nKey, MAPVK_VK_TO_VSC_EX);
	//keybd_event((unsigned char)nKey, (unsigned char)nMapVirtualKey, 0, 0);
	//Sleep(3000);
	//keybd_event((unsigned char)nKey, (unsigned char)nMapVirtualKey, KEYEVENTF_KEYUP, 0);


	//MouseInput[0].type = INPUT_MOUSE;//虚拟的类型 INPUT_MOUSE or INPUT_KEY
	//MouseInput[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE; //鼠标标志 MOUSEEVENTF_LEFTDOWN  MOUSEEVENTF_ABSOLUTE
	//MouseInput[0].mi.dx = (65535 / (GetSystemMetrics(SM_CXSCREEN) - 1)) * 150;//鼠标x坐标
	//MouseInput[0].mi.dy = (65535 / (GetSystemMetrics(SM_CYSCREEN) - 1)) * 150;//鼠标y坐标
	//Sleep(50);
	//if (SendInput(1, MouseInput, sizeof(INPUT)) == 0)
	//{
	//	MessageBox(0,"INPUT被占用", "错误",0);
	//}
	//Sleep(5000);
	//INPUT KeyInput[4];
	//memset(KeyInput, 0, sizeof(KeyInput));
	////设置模拟键盘输入
	//KeyInput[0].type = INPUT_KEYBOARD;
	//KeyInput[1].type = INPUT_KEYBOARD;
	//KeyInput[2].type = INPUT_KEYBOARD;
	//KeyInput[3].type = INPUT_KEYBOARD;
	//KeyInput[0].ki.wVk = VK_DOWN;
	//KeyInput[1].ki.wVk = VK_ESCAPE;
	//KeyInput[2].ki.wVk = VK_DOWN;
	//KeyInput[3].ki.wVk = VK_ESCAPE;
	//// 弹起按键
	//KeyInput[2].ki.dwFlags = KEYEVENTF_KEYUP;
	//KeyInput[3].ki.dwFlags = KEYEVENTF_KEYUP;
	//SendInput(4, KeyInput, sizeof(INPUT));//把消息发送到消息队列
	//

	return 0;
}