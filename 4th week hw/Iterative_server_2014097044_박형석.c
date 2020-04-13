//#pragma commnet(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>


#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	SOCKET hServSock, hClntSock;		
	SOCKADDR_IN servAddr, clntAddr;		

	int szClntAddr;
	//char message[]="2014097044 박형석";
	char inMessage[30] = {'\0',};
	int idx = 0, readLen = 0, strLen = 0;
	int i;



	/*
	if(argc!=2) 
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	*/
	
  
	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
		ErrorHandling("WSAStartup() error!"); 
	
	hServSock=socket(PF_INET, SOCK_STREAM, 0);
	if(hServSock==INVALID_SOCKET)
		ErrorHandling("socket() error");
  
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port = htons(32154);
	//servAddr.sin_port=htons(atoi(argv[1]));
	
	if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
		ErrorHandling("bind() error");  
	
	if(listen(hServSock, 3)==SOCKET_ERROR)
		ErrorHandling("listen() error");

	szClntAddr=sizeof(clntAddr);

	for (i = 0; i < 3; i++)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		if (hClntSock == -1)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d\n", i + 1);
		while ((strLen = recv(hClntSock, inMessage, BUF_SIZE, 0)) != 0)
		{
			if (strLen == -1)
				break;
			send(hClntSock, inMessage, strLen, 0);
		}
		printf("Client %d is disconnected \n", i + 1);
		closesocket(hClntSock);
	}
	

	/*
	hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr,&szClntAddr);
	if(hClntSock==INVALID_SOCKET)
		ErrorHandling("accept() error");  

	//recv(hClntSock, inMessage, 512, 0);

	while (1)
	{
		if (inMessage[idx - 1] == '\n')
			break;
		readLen = recv(hClntSock, &inMessage[idx++], 1, 0);
		if (readLen == -1)
			ErrorHandling("read() error!");
		strLen += readLen;
	}

	printf("Message from client: %s \n", inMessage);
	printf("Function read call count: %d \n", strLen);
	
	send(hClntSock, inMessage, sizeof(inMessage), 0);
	
	closesocket(hClntSock);
	*/

	closesocket(hServSock);
	printf("서버 프로그램을 종료합니다.\n");
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
