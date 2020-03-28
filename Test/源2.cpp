
#include "iostream"

#include "fstream"

#include "string"

#include "windows.h"

#include "Iphlpapi.h"


#pragma comment(lib , "IpHlpApi.lib")

#pragma comment(lib , "ws2_32.lib")


#define MACFILE "mac.ini" //mac配置文件

#define GATEWAY "gateway.ini"//网关地址文件


using namespace std;

int main(int argc, char* argv[])

{

	string ipadd;

	string macadd;

	char bc;

	//读取网关ip地址

	fstream _ipadd;

	_ipadd.open(GATEWAY, ios::in);

	while (_ipadd.get(bc))

	{

		ipadd += bc;


	}

	cout << "ARP欺骗检查工具 By：Neeao \n";

	cout << "网关IP:" << ipadd << endl;


	int iReturn;

	DWORD dwIP;

	BYTE byMAC[6];

	DWORD dwLen;

	WSADATA WsaData;

	WSAStartup(MAKEWORD(2, 0), &WsaData);


	dwIP = inet_addr(ipadd.c_str());

	//cout << dwIP <<"\n";

	if (dwIP == INADDR_NONE)

	{

		cout << "IP地址出错: " << ipadd;

		return 1;

	}

	//循环获取mac地址

	while (TRUE)

	{


		// 发送ARP查询包获得 MAC 地址

		dwLen = 6;

		iReturn = SendARP(dwIP, 0, (PULONG)& byMAC, &dwLen);

		if (iReturn != NO_ERROR)

		{

			printf("出错了:只能获取当前网关下主机的MAC地址.\n", argv[1]);

			//__leave;

			return 1;

		}


		char MACadd[50];

		sprintf_s(MACadd, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", byMAC[0], byMAC[1], byMAC[2], byMAC[3], byMAC[4], byMAC[5]);

		//cout << MACadd;


		//判断macadd是否为空，不为空直接跳过

		if (macadd == "")

		{

			fstream _mac;

			_mac.open(MACFILE, ios::in);

			if (!_mac)

			{

				//cout<<MACFILE<<"没有被创建";

				//第一次运行写入mac地址，为初始mac地址。

				fstream _macr;

				_macr.open(MACFILE, ios::out | ios::app);

				if (!_macr)

				{

					cout << "文件创建失败,磁盘不可写或者文件为只读!";

					exit(1);

				}

				_macr << MACadd;

				_macr.close();

				macadd = MACadd;

			}

			else

			{

				char ch;

				string content;

				while (_mac.get(ch))

				{

					content += ch;


				}

				_mac.close();

				macadd = content;


				cout << "MAC地址为：" << content << endl;


			}

		}

		//判断mac地址

		if (macadd == MACadd)

		{

			cout << "ok\n";

			//return 1;

		}
		else

		{

			cout << "MAC地址被修改了\n";

			//return 1;

		}

		Sleep(2000);//每2秒钟获取一次

	}

	return 0;

}