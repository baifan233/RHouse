#include<windows.h>
#include<iostream>
#include<vector>
using namespace std;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 
int main()
{	
	bool brun = false;
	int speed=1;
	while (true)
	{
		if (brun)
		{
			Sleep(speed);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		}
		if (KEY_DOWN(VK_INSERT))
		{
			brun = !brun;
			Sleep(1000);
		}
		else if (KEY_DOWN(VK_HOME))
		{
			speed = 100;
		}
		else if (KEY_DOWN(VK_END))
		{
			speed = 1;
		}
	}

	for (size_t i = 1; i <= 4; i++)
	{
		for (size_t j = 1; j <= 4; j++)
		{
			for (size_t k = 1; k <= 4; k++)
			{
				if (i != j && j != k&&k!=i)
				{
					cout << i<<" ";
					cout << j << " ";
					cout << k<<endl;
				}
			}
		}
	}

	system("pause");
	return 0;
}