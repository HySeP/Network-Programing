#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma commnet(lib, "ws2_32.lib")

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30] = { '\0', }, message1[] = "2014097044 ¹ÚÇü¼®\n";
	int strLen=0;
	int idx=0, readLen=0;

	/*
	if(argc!=3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	*/
	

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");  
	
	hSocket=socket(PF_INET, SOCK_STREAM, 0);
	if(hSocket==INVALID_SOCKET)
		ErrorHandling("hSocket() error");
	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servAddr.sin_port=htons(32154);
	
	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
		ErrorHandling("connect() error!");

	send(hSocket, message1, sizeof(message1), 0);
	//recv(hSocket, message, sizeof(message), 0);
 
	while (1)
	{
		if (message[idx - 1] == '\n')
			break;
		readLen = recv(hSocket, &message[idx++], 1, 0);
		if (readLen == -1)
			ErrorHandling("read() error!");
		strLen += readLen;
	}

	/*
	while(readLen=recv(hSocket, &message[idx++], 1, 0))
	{
		if(readLen==-1)
			ErrorHandling("read() error!");
		
		strLen+=readLen;
	}
	*/

	printf("Message from server: %s \n", message);  
	printf("Function read call count: %d \n", strLen);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}