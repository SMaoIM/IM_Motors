
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8081);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据
	SOCKET sClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	//int revData[10] = { 0};
	//char sendBuf[100] = { 1 };


	//for (int i=0; i < 10; i++) {
	//	cout<< sendBuf[0] <<endl;
	//}

	
	//char revData[255];
	printf("等待连接...\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (1)
	{
		UINT32 revData[10] = {0};

		//UINT32 temp1 = NULL;
		//printf("等待连接...\n");
		//sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			
		}
//		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//接收数据
		int ret = recv(sClient, (char*)&revData, sizeof(revData), 0);
	
		//printf(ret);
		//printf("%u", revData[1]);
		for (int i = 0; i < 10; i++) {
			printf("%u\n", revData[i]);
		}
		


	}
	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}
