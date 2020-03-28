
#include "iostream"

#include "fstream"

#include "string"

#include "windows.h"

#include "Iphlpapi.h"


#pragma comment(lib , "IpHlpApi.lib")

#pragma comment(lib , "ws2_32.lib")


#define MACFILE "mac.ini" //mac�����ļ�

#define GATEWAY "gateway.ini"//���ص�ַ�ļ�


using namespace std;

int main(int argc, char* argv[])

{

	string ipadd;

	string macadd;

	char bc;

	//��ȡ����ip��ַ

	fstream _ipadd;

	_ipadd.open(GATEWAY, ios::in);

	while (_ipadd.get(bc))

	{

		ipadd += bc;


	}

	cout << "ARP��ƭ��鹤�� By��Neeao \n";

	cout << "����IP:" << ipadd << endl;


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

		cout << "IP��ַ����: " << ipadd;

		return 1;

	}

	//ѭ����ȡmac��ַ

	while (TRUE)

	{


		// ����ARP��ѯ����� MAC ��ַ

		dwLen = 6;

		iReturn = SendARP(dwIP, 0, (PULONG)& byMAC, &dwLen);

		if (iReturn != NO_ERROR)

		{

			printf("������:ֻ�ܻ�ȡ��ǰ������������MAC��ַ.\n", argv[1]);

			//__leave;

			return 1;

		}


		char MACadd[50];

		sprintf_s(MACadd, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", byMAC[0], byMAC[1], byMAC[2], byMAC[3], byMAC[4], byMAC[5]);

		//cout << MACadd;


		//�ж�macadd�Ƿ�Ϊ�գ���Ϊ��ֱ������

		if (macadd == "")

		{

			fstream _mac;

			_mac.open(MACFILE, ios::in);

			if (!_mac)

			{

				//cout<<MACFILE<<"û�б�����";

				//��һ������д��mac��ַ��Ϊ��ʼmac��ַ��

				fstream _macr;

				_macr.open(MACFILE, ios::out | ios::app);

				if (!_macr)

				{

					cout << "�ļ�����ʧ��,���̲���д�����ļ�Ϊֻ��!";

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


				cout << "MAC��ַΪ��" << content << endl;


			}

		}

		//�ж�mac��ַ

		if (macadd == MACadd)

		{

			cout << "ok\n";

			//return 1;

		}
		else

		{

			cout << "MAC��ַ���޸���\n";

			//return 1;

		}

		Sleep(2000);//ÿ2���ӻ�ȡһ��

	}

	return 0;

}