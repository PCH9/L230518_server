#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <Winsock2.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

#pragma pack(1)
typedef struct _Packet
{
	char Code;
	short Number1;
	short Number2;

} Packet;
#pragma pack(pop)

//int main()
//{
//	Packet MyPacket;
//	MyPacket.Code = (short)PacketCode::Move; //Move
//	MyPacket.Number1 = 10;
//	MyPacket.Number2 = 20;
//
//	char Data[6];
//	memcpy(Data, &MyPacket, sizeof(Packet));
//
//	memcpy(&MyPacket, Data, sizeof(Packet));
//
//	cout << sizeof(MyPacket) << endl;
//}

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ListenSockAddr;
	ZeroMemory(&ListenSockAddr, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = 9090;

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));


	while (1)
	{
		listen(ListenSocket, 5);

		SOCKADDR_IN ClientSockAddr;
		ZeroMemory(&ClientSockAddr, sizeof(ClientSockAddr));
		int ClientSockAddrLength = sizeof(ClientSockAddr);
		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

		while (1)
		{
			char Buffer[5] = { 0, };
			int RecvLength = recv(ClientSocket, Buffer, 5, 0);
			if (RecvLength <= 0)
			{
				break;
			}

			Packet MyPacket;
			memcpy(&MyPacket, Buffer, 5);

			//short Number1 = 0;
			//short Number2 = 0;
			//((char*)(&Number1))[0] = Buffer[0];
			//((char*)(&Number1))[1] = Buffer[1];
			//int Operator = Buffer[2];
			//((char*)(&Number2))[0] = Buffer[3];
			//((char*)(&Number2))[1] = Buffer[4];

			char OperatorMark[4] = { '+', '-', '*', '/' };


			cout << MyPacket.Number1 << OperatorMark[MyPacket.Code] << MyPacket.Number2 << endl;

			int Number1 = MyPacket.Number1;
			int Number2 = MyPacket.Number2;
			int Operator = MyPacket.Code;

			int Number3 = 0;

			if (Operator == 0) // +
			{
				Number3 = Number1 + Number2;
			}
			else if (Operator == 1) // -
			{
				Number3 = Number1 - Number2;

			}
			else if (Operator == 2) // *
			{
				Number3 = Number1 * Number2;
			}
			else if (Operator == 3) // /
			{
				if (Number2 != 0)
				{
					Number3 = Number1 / Number2;
				}
				else
				{
					Number3 = 0;
				}
			}

			int SentLength = send(ClientSocket, (char*)&Number3, 4, 0);
		}

		//string ResultString = Number1String + OperatorString + Number2String + "=" + Number3String;
		//int SentLength = send(ClientSocket, ResultString.c_str(), (int)ResultString.length(), 0);

		closesocket(ClientSocket);
	}
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}