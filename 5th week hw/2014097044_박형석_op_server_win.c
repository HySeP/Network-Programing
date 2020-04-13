#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define OPSZ 4
void ErrorHandling(char *message);
int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char opinfo[BUF_SIZE];
	int result, opndCnt, i;
	int recvCnt, recvLen;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	/*
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(32154);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	clntAdrSize = sizeof(clntAdr);

	for (i = 0; i < 5; i++)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		if (hClntSock == -1)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d \n", i + 1);
		int flag;

		while (1)
		{

			flag = 0;
			opndCnt = 0;
			recvLen = 0;
			recv(hClntSock, &opndCnt, 1, 0);

			while ((opndCnt*OPSZ + 1) > recvLen)
			{
				//strLen = recv(hClntSock, &message[recvLen], BUF_SIZE, 0);
				recvCnt = recv(hClntSock, &opinfo[recvLen], BUF_SIZE, 0);
				recvLen += recvCnt;

				if (recvCnt == -1 || recvCnt == 0)
				{
					flag = 1;
					break;
				}
			}

			if (flag == 1)
			{
				break;
			}

			result = calculate(opndCnt, (int*)opinfo, opinfo[recvLen - 1]);
			send(hClntSock, (char*)&result, sizeof(result), 0);
			printf("result: %d \n", result);
		}
			printf("Client %d is disconnected \n", i + 1);
			closesocket(hClntSock);
	}

	//printf("Client %d is disconnected \n", i + 1);
	//closesocket(hClntSock);
	closesocket(hServSock);


	WSACleanup();
	
	return 0;
}

int calculate(int opnum, int opnds[], char op)
{
	int result = opnds[0], i;

	switch (op)
	{
	case '+':
		for (i = 1; i < opnum; i++) result += opnds[i];
		break;
	case '-':
		for (i = 1; i < opnum; i++) result -= opnds[i];
		break;
	case '*':
		for (i = 1; i < opnum; i++) result *= opnds[i];
		break;
	}
	return result;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}