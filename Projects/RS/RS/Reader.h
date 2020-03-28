#pragma once
#include<Windows.h>
class Reader
{
public:
	Reader();
	~Reader();	
	WNDPROC WndProc;
	PAINTSTRUCT ps;
};

